/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "AnimationEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"

#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Time/TimeSensor.h>
#include <Titania/X3D/Components/EventUtilities/BooleanSequencer.h>
#include <Titania/X3D/Components/EventUtilities/IntegerSequencer.h>
#include <Titania/X3D/Components/Interpolation/ScalarInterpolator.h>
#include <Titania/X3D/Components/Interpolation/PositionInterpolator2D.h>
#include <Titania/X3D/Components/Interpolation/PositionInterpolator.h>
#include <Titania/X3D/Parser/Filter.h>

#include <Titania/Math/Algorithms/CatmullRomSplineInterpolator.h>
#include <Titania/Math/Algorithms/SquadInterpolator.h>

#include <regex>

namespace titania {
namespace puck {

using math::M_PHI;

static constexpr int32_t FRAME_SIZE          = 7;            // in pixel
static constexpr double  DEFAULT_TRANSLATION = 8;            // in pixel
static constexpr double  DEFAULT_SCALE       = 16;           // in pixel
static constexpr double  SCROLL_FACTOR       = 1 + 1 / 16.0; // something nice

static constexpr double infinity = std::numeric_limits <double>::infinity ();

static
std::string
strfframes (const size_t value, const size_t framesPerSecond)
{
	size_t time = value;

	const size_t frames = time % framesPerSecond;
	time /= framesPerSecond;

	const size_t seconds = time % 60;
	time /= 60;

	const size_t minutes = time % 60;
	time /= 60;

	const size_t hours = time;

	std::ostringstream osstream;
	
	osstream
		<< std::setfill ('0')
		<< std::setw (2) << hours << ":"
		<< std::setw (2) << minutes << ":" 
		<< std::setw (2) << seconds << ":" 
		<< std::setw (2) << frames;

	return osstream .str ();
}

const std::map <X3D::X3DConstants::NodeType, size_t> AnimationEditor::interpolatorComponents = {
	std::make_pair (X3D::X3DConstants::BooleanSequencer,        1),
	std::make_pair (X3D::X3DConstants::IntegerSequencer,        1),
	std::make_pair (X3D::X3DConstants::ColorInterpolator,       3),
	std::make_pair (X3D::X3DConstants::ScalarInterpolator,      1),
	std::make_pair (X3D::X3DConstants::OrientationInterpolator, 4),
	std::make_pair (X3D::X3DConstants::PositionInterpolator2D,  2),
	std::make_pair (X3D::X3DConstants::PositionInterpolator,    3)
};

AnimationEditor::AnimationEditor (X3DBrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DAnimationEditorInterface (get_ui ("AnimationEditor.glade")),
	            X3DEditorObject (),
	                    columns (),
	                  treeStore (Gtk::TreeStore::create (columns)),
	            treeModelFilter (Gtk::TreeModelFilter::create (treeStore)),
	                  nodeIndex (new NodeIndex (browserWindow)),
	                   nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                  animation (),
	                 timeSensor (),
	          interpolatorIndex (),
	              interpolators (),
	                      nodes (),
	                  fromPoint (),
	                translation (0),
	                      scale (1),
	                     button (0),
	                frameChange (0),
	                     frames (),
	               activeFrames (),
	             selectedFrames (),
	             selectedBounds (),
	              selectedRange (),
	            activeSelection (false),
	                movedFrames (),
	               copiedFrames (),
	                 activeType ("SPLINE"),
	                   changing (false),
	                       keys ()
{
	addChildren (animation, timeSensor);

	getTranslationAdjustment () -> set_lower (-DEFAULT_TRANSLATION);
	getTranslationAdjustment () -> set_upper (-DEFAULT_TRANSLATION);

	getTreeModelFilter () -> set_visible_column (columns .visible);
	getTreeView () .set_model (getTreeModelFilter ());

	getNameColumn () -> add_attribute (*getNameCellRenderer () .operator -> (), "style-set", columns .tainted);

	getCutButton ()   .add_accelerator ("clicked", getAccelGroup (), GDK_KEY_X, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
	getCopyButton ()  .add_accelerator ("clicked", getAccelGroup (), GDK_KEY_C, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
	getPasteButton () .add_accelerator ("clicked", getAccelGroup (), GDK_KEY_V, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);

	setup ();
}

void
AnimationEditor::initialize ()
{
	X3DAnimationEditorInterface::initialize ();
	X3DEditorObject::initialize ();

	getScaleKeyframesButton () .set_active (getConfig () -> getBoolean ("scaleKeyframes"));

	if (getConfig () -> hasItem ("hPaned"))
		getAnimationBox () .set_position (getConfig () -> getInteger ("hPaned"));

	getCurrentBrowser ()  .addInterest (this, &AnimationEditor::set_animation, nullptr);
	getCurrentContext ()  .addInterest (this, &AnimationEditor::set_animation, nullptr);

	nodeIndex -> getNode () .addInterest (this, &AnimationEditor::set_animation);
	nodeIndex -> getHeaderBox () .set_visible (false);
	nodeIndex -> getFooterBox () .set_visible (false);
	nodeIndex -> getScrolledWindow () .set_size_request (0, 0);
	nodeIndex -> reparent (getNodeIndexBox (), getWindow ());
	nodeIndex -> setAnimations ();

	set_animation (nullptr);
}

void
AnimationEditor::setScale (const double value)
{
	scale = value;

	setLowerTranslation ();
	getDrawingArea () .queue_draw ();
}

void
AnimationEditor::on_map ()
{
	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &AnimationEditor::set_selection);

	set_selection (getBrowserWindow () -> getSelection () -> getChildren ());
}

void
AnimationEditor::on_unmap ()
{
	getBrowserWindow () -> getSelection () -> getChildren () .removeInterest (this, &AnimationEditor::set_selection);
}

void
AnimationEditor::set_selection (const X3D::MFNode & selection)
{
	X3DAnimationEditorInterface::set_selection (selection);

	const bool haveSelection = not getBrowserWindow () -> getSelection () -> getChildren () .empty ();
	const auto groups        = getNodes <X3D::X3DGroupingNode> (selection, { X3D::X3DConstants::X3DGroupingNode });

	getNewButton ()       .set_sensitive (not groups .empty ());
	getAddMemberButton () .set_sensitive (animation and haveSelection);
}

int32_t
AnimationEditor::getDuration () const
{
	try
	{
		if (animation)
			return std::max <int32_t> (animation -> getMetaData <X3D::MFInt32> ("/Animation/duration", false) .at (0), 1);
	}
	catch (...)
	{ }
	
	return 10;
}

int32_t
AnimationEditor::getFramesPerSecond () const
{
	try
	{
		if (animation)
			return std::max <int32_t> (animation -> getMetaData <X3D::MFInt32> ("/Animation/framesPerSecond", false) .at (0), 1);
	}
	catch (...)
	{ }

	return 10;
}

bool
AnimationEditor::isActive () const
{
	return timeSensor -> isActive () and not timeSensor -> isPaused ();
}

void
AnimationEditor::on_translation_changed ()
{
	translation = -getTranslationAdjustment () -> get_value ();

	getDrawingArea () .queue_draw ();
}

void
AnimationEditor::setTranslation (const double value)
{
	const auto width = getDrawingArea () .get_width ();
	const auto max   = (width - DEFAULT_TRANSLATION) - (getDuration () * getScale ());

	translation  = value;
	translation  = std::max (translation, max);
	translation  = std::min (translation, DEFAULT_TRANSLATION);

	getTranslationAdjustment () -> set_value (-translation);
}

void
AnimationEditor::setLowerTranslation ()
{
	const auto width = getDrawingArea () .get_width ();
	const auto max   = (width - DEFAULT_TRANSLATION) - (getDuration () * getScale ());
	const auto lower = std::min (DEFAULT_TRANSLATION, max);

	//getTranslationSlider () .set_visible (lower < DEFAULT_TRANSLATION);

	getTranslationAdjustment () -> set_page_size (getDrawingArea () .get_width ());
	getTranslationAdjustment () -> set_upper (-lower + getDrawingArea () .get_width ());
}

void
AnimationEditor::on_new ()
{
	// Open ªNew Animation Dialog´.

	getNewNameEntry () .set_text (_ ("Animation"));
	getDurationAdjustment () -> set_value (10);
	getFPSAdjustment () -> set_value (10);
	getLoopSwitch () .set_active (false);

	const auto responseId = getPropertiesDialog () .run ();

	getPropertiesDialog () .hide ();

	if (responseId not_eq Gtk::RESPONSE_OK)
		return;

	// Create new animation.

	const auto undoStep   = std::make_shared <X3D::UndoStep> (_ ("Create New Animation"));
	const auto name       = getCurrentContext () -> getUniqueName (getNewNameEntry () .get_text ());
	const auto groups     = getSelection <X3D::X3DGroupingNode> ({ X3D::X3DConstants::X3DGroupingNode });
	const auto group      = groups .back ();
	const auto animation  = getCurrentContext () -> createNode <X3D::Group> ();
	const auto timeSensor = getCurrentContext () -> createNode <X3D::TimeSensor> ();

	group -> children () .emplace_front (animation);
	animation -> children () .emplace_front (timeSensor);

	getCurrentContext () -> updateNamedNode (name, X3D::SFNode (animation));
	animation -> setMetaData <int32_t> ("/Animation/duration",        getDurationAdjustment () -> get_value ());
	animation -> setMetaData <int32_t> ("/Animation/framesPerSecond", getFPSAdjustment () -> get_value ());

	animation -> getMetaData <X3D::MFInt32> ("/Animation/duration", false)        .addInterest (this, &AnimationEditor::set_duration);
	animation -> getMetaData <X3D::MFInt32> ("/Animation/framesPerSecond", false) .addInterest (this, &AnimationEditor::set_frames_per_second);
	
	timeSensor -> loop ()     = getLoopSwitch () .get_active ();
	timeSensor -> stopTime () = 1;
	getCurrentContext () -> realize ();

	set_animation (X3D::SFNode (animation));

	// Undo/Redo

	const auto undoRemoveNode = std::make_shared <X3D::UndoStep> ();
	getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), { animation }, true, undoRemoveNode);
	undoStep -> addUndoFunction (&X3D::UndoStep::redo, undoRemoveNode);
	undoStep -> addRedoFunction (&X3D::UndoStep::undo, undoRemoveNode);
	undoRemoveNode -> undo ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
AnimationEditor::on_new_name_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (getNewNameEntry (), text, *position);
}

void
AnimationEditor::on_new_name_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (getNewNameEntry (), start_pos, end_pos);
}

void
AnimationEditor::on_new_name_changed ()
{
	const std::string name = getNewNameEntry () .get_text ();

	getNewOkButton () .set_sensitive (not name .empty ());
}

void
AnimationEditor::on_new_cycle_interval_changed ()
{
	getCycleIntervalLabel () .set_text (strfframes (getDurationAdjustment () -> get_value (), getFPSAdjustment () -> get_value ()));
}

void
AnimationEditor::set_animation (const X3D::SFNode & value)
{
	// Remove animation.
	
	if (animation)
	{
		animation -> isLive ()   .removeInterest (this, &AnimationEditor::set_animation_live);
		animation -> children () .removeInterest (this, &AnimationEditor::set_interpolators);
	}

	if (timeSensor)
	{
		timeSensor -> isLive ()           .removeInterest (this, &AnimationEditor::set_animation_live);
		timeSensor -> isPaused ()         .removeInterest (this, &AnimationEditor::set_active);
		timeSensor -> isActive ()         .removeInterest (this, &AnimationEditor::set_active);
		timeSensor -> fraction_changed () .removeInterest (this, &AnimationEditor::set_fraction);

		timeSensor -> isEvenLive (false);
		timeSensor -> range () = { 0, 0, 1 };

		if (timeSensor -> loop () and timeSensor -> isActive ())
		{
			timeSensor -> stopTime ()  = 0;
			timeSensor -> startTime () = 0;
		}
		else
		{
			timeSensor -> startTime () = 0;
			timeSensor -> stopTime ()  = 1;
		}

		timeSensor -> fraction_changed () = 0;
	}

	for (const auto & pair : nodes)
		removeNode (pair .second);

	removeInterpolators ();

	// Reset all.

	animation  = value;
	timeSensor = nullptr;
	nodes .clear ();
	on_clear_selection ();
	on_clear_clipboard ();

	getTreeStore () -> clear ();
	const auto master = getTreeStore () -> append ();
	(*master) [columns .name]    = "<b>" + Glib::Markup::escape_text (_ ("Master")) + "</b>";
	(*master) [columns .type]    = Gdk::Pixbuf::create_from_file (get_ui ("icons/Master.svg"));
	(*master) [columns .tainted] = false;
	(*master) [columns .visible] = true;

	getCutButton ()            .set_sensitive (animation);
	getCopyButton ()           .set_sensitive (animation);
	getPasteButton ()          .set_sensitive (animation);
	getFirstFrameButton ()     .set_sensitive (animation);
	getPlayPauseButton ()      .set_sensitive (animation);
	getLastFrameButton ()      .set_sensitive (animation);
	getFrameSpinButton ()      .set_sensitive (animation);
	getTimeButton ()           .set_sensitive (animation);
	getKeyTypeButton ()        .set_sensitive (animation);
	getCloseAnimationButton () .set_sensitive (animation);
	getDrawingAreaBox ()       .set_sensitive (animation);
	getToolBar2 ()             .set_sensitive (animation);
	getToolBar3 ()             .set_sensitive (animation);
	getNotebook ()             .set_current_page (animation ? 1 : 0);

	if (animation)
	{
		animation -> isLive ()   .addInterest (this, &AnimationEditor::set_animation_live);
		animation -> children () .addInterest (this, &AnimationEditor::set_interpolators);

		const auto timeSensors = getNodes <X3D::TimeSensor> ({ animation }, { X3D::X3DConstants::TimeSensor });

		if (timeSensors .empty ())
		{
			timeSensor = getCurrentContext () -> createNode <X3D::TimeSensor> ();

			animation -> children () .emplace_front (timeSensor);

			getCurrentContext () -> realize ();
		}
		else
			timeSensor = timeSensors .back ();

		timeSensor -> isLive ()           .addInterest (this, &AnimationEditor::set_animation_live);
		timeSensor -> isPaused ()         .addInterest (this, &AnimationEditor::set_active);
		timeSensor -> isActive ()         .addInterest (this, &AnimationEditor::set_active);
		timeSensor -> fraction_changed () .addInterest (this, &AnimationEditor::set_fraction);

		timeSensor -> cycleInterval () = getDuration () / (double) getFramesPerSecond ();
		timeSensor -> isEvenLive (true);

		set_active (); // Call this before set_interpolators.
		set_interpolators ();
	}

	set_selection (getBrowserWindow () -> getSelection () -> getChildren ());

	nodeName .setNode (X3D::SFNode (animation));

	getFrameAdjustment () -> set_upper (getDuration ());
	getFrameAdjustment () -> set_value (0);

	if (animation)
		on_zoom_fit ();

	getDrawingArea () .queue_draw ();
}

void
AnimationEditor::set_animation_live (const bool value)
{
	if (not value)
		set_animation (nullptr);
}

void
AnimationEditor::set_interpolators ()
{
	removeInterpolators ();

	std::map <size_t, X3D::SFNode> foundNodes;

	for (const auto & node : animation -> children ())
	{
		try
		{	
			const X3D::X3DPtr <X3D::X3DNode> interpolator (node);
			
			if (not interpolator)
				continue;

			interpolator -> getField <X3D::SFFloat> ("set_fraction");
			interpolator -> getField <X3D::MFFloat> ("key");

			if (not interpolator -> getField ("keyValue") -> isArray ())
				continue;

			const auto value_changed = interpolator -> getField ("value_changed");
			
			for (const auto & route : value_changed -> getOutputRoutes ())
			{
				try
				{
					const auto node  = route -> getDestinationNode ();
					const auto field = node -> getField (route -> getDestinationField ());

					foundNodes .emplace (node -> getId (), node);
					interpolatorIndex .emplace (field, interpolator);
					interpolators .emplace (interpolator);
					interpolator -> getField ("value_changed") -> addInterest (this, &AnimationEditor::set_value);
				}
				catch (const X3D::X3DError &)
				{ }
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Remove interpolators not found.

	std::vector <std::pair <size_t, X3D::SFNode>> difference;

	std::set_difference (nodes .begin (), nodes .end (),
	                     foundNodes .begin (), foundNodes .end (),
	                     std::back_inserter (difference),
	                     nodes .value_comp ());

	for (const auto & pair : difference)
	{
		removeNode (pair .second);	
		nodes .erase (pair .first);
	}

	for (const auto & master : getTreeStore () -> children ())
	{
		for (const auto & parent : master -> children ())
		{
			if (not nodes .count ((*parent) [columns .id]))
				(*parent) [columns .visible] = false;
		}
	}

	// Add newly found interpolators.

	for (const auto & node : foundNodes)
		addNode (node .second);

	getDrawingArea () .queue_draw ();
}

void
AnimationEditor::removeInterpolators ()
{
	for (const auto & interpolator : interpolators)
		interpolator -> getField ("value_changed") -> removeInterest (this, &AnimationEditor::set_value);

	interpolatorIndex .clear ();
	interpolators     .clear ();
}

void
AnimationEditor::on_add_member ()
{
	for (const auto & node : getBrowserWindow () -> getSelection () -> getChildren ())
		addNode (node);
}

void
AnimationEditor::on_remove_member ()
{
	if (getTreeView () .get_selection () -> get_selected_rows () .empty ())
		return;
	
	const auto selected = getTreeView () .get_selection () -> get_selected ();
	const auto path     = getTreeModelFilter () -> get_path (selected);
	
	switch (path .size ())
	{
		case 1:
		{
		   // Remove Animation

			const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Animation"));

			getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), { animation }, true, undoStep);
			getBrowserWindow () -> addUndoStep (undoStep);
			break;
		}
		case 2:
		{
		   // Remove Node

			try
			{
				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Animation Member"));

				undoStep -> addUndoFunction (&AnimationEditor::set_interpolators, this);

				// Find all interpolators connected to this node:

				const auto & node = nodes .at ((*selected) [columns .id]);
				
				Interpolators interpolatorsToRemove;

				for (const auto & field : node -> getFieldDefinitions ())
				{
					const auto iter = interpolatorIndex .find (field);

					if (iter == interpolatorIndex .end ())
						continue;

					const auto & interpolator = iter -> second;

					interpolatorsToRemove .emplace (interpolator);

					getBrowserWindow () -> deleteRoute (getCurrentContext (), X3D::SFNode (interpolator), "value_changed", node, field -> getName (), undoStep);

					interpolatorIndex .erase (iter);
				}

			   // If a interpolator is connected to other members, remove this interpolator from the list of interpolators to remove.
				
				for (const auto & pair : interpolatorIndex)
				{
					const auto iter = interpolatorsToRemove .find (pair .second);

					if (iter not_eq interpolatorsToRemove .end ())
						interpolatorsToRemove .erase (iter);
				}

			   // Remove all Interpolators now found.
				
				if (not interpolators .empty ())
				{
					getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), X3D::MFNode (interpolatorsToRemove .begin (), interpolatorsToRemove .end ()), true, undoStep);
				}

				undoStep -> addRedoFunction (&AnimationEditor::set_interpolators, this);
				getBrowserWindow () -> addUndoStep (undoStep);

				// Must be explicitly removed if no interpolator is connected to member.
				removeNode (node);
				nodes .erase ((*selected) [columns .id]);
				(*selected) [columns .visible] = false;
			}
			catch (const std::out_of_range &)
			{ }
			
			break;
		}
		case 3:
		{
		   // Remove field

			try
			{
				const auto   parent       = selected -> parent ();
				const auto & node         = nodes .at ((*parent) [columns .id]);
				const auto   field        = node -> getFieldDefinitions () .at ((*selected) [columns .id]);
				const auto & interpolator = interpolatorIndex .at (field);
					
				// We have found the interpolators connected to this node and field.
				
				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Interpolator"));

				undoStep -> addUndoFunction (&AnimationEditor::set_interpolators, this);
				getBrowserWindow () -> deleteRoute (getCurrentContext (), X3D::SFNode (interpolator), "value_changed", node, field -> getName (), undoStep);

				interpolatorIndex .erase (field);

			   // If the interpolator is connected to other members, don't remove the interpolator.

			   bool isConnectedToOtherMembers = false;
				
				for (const auto & pair : interpolatorIndex)
				{
					if (pair .second == interpolator)
					{
					   isConnectedToOtherMembers = true;
						break;
					}
				}
						
				if (not isConnectedToOtherMembers)
					getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), { interpolator }, true, undoStep);

				undoStep -> addRedoFunction (&AnimationEditor::set_interpolators, this);
				getBrowserWindow () -> addUndoStep (undoStep);

				set_interpolators ();
			}
			catch (const std::out_of_range &)
			{ }

			break;
		}
		default:
			break;
	}
}

void
AnimationEditor::on_tree_view_selection_changed ()
{
	getRemoveMemberButton () .set_sensitive (not getTreeView () .get_selection () -> get_selected_rows () .empty ());
}

void
AnimationEditor::addNode (const X3D::SFNode & node)
{
	if (not nodes .emplace (node -> getId (), node) .second)
		return;

	const auto master = getTreeStore () -> get_iter (Gtk::TreePath ("0"));
	auto       parent = getTreeStore () -> append (master -> children ());
	(*parent) [columns .id]      = node -> getId ();
	(*parent) [columns .type]    = Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/X3DBaseNode.svg"));
	(*parent) [columns .name]    = getNodeName (node);
	(*parent) [columns .tainted] = false;
	(*parent) [columns .visible] = true;

	getTreeView () .expand_row (Gtk::TreePath ("0"), false);

	node -> name_changed ()   .addInterest (this, &AnimationEditor::set_name,                node -> getId (), getTreeStore () -> get_path (parent));
	node -> fields_changed () .addInterest (this, &AnimationEditor::set_user_defined_fields, node -> getId (), getTreeStore () -> get_path (parent));
	node -> isLive ()         .addInterest (this, &AnimationEditor::set_live,                node -> getId (), getTreeStore () -> get_path (parent));

	addFields (node, parent);
}

void
AnimationEditor::removeNode (const X3D::SFNode & node)
{
	node -> name_changed ()   .removeInterest (this, &AnimationEditor::set_name);
	node -> fields_changed () .removeInterest (this, &AnimationEditor::set_user_defined_fields);
	node -> isLive ()         .removeInterest (this, &AnimationEditor::set_animation_live);

	for (const auto & field : node -> getFieldDefinitions ())
		field -> removeInterest (this, &AnimationEditor::set_tainted);
}

void
AnimationEditor::addFields (const X3D::SFNode & node, Gtk::TreeIter & parent)
{
	static const std::set <X3D::X3DConstants::FieldType> fieldTypes = {
	   X3D::X3DConstants::SFBool,
		X3D::X3DConstants::SFColor,
		X3D::X3DConstants::SFFloat,
		X3D::X3DConstants::SFInt32,
		X3D::X3DConstants::SFRotation,
		X3D::X3DConstants::SFVec2f,
		X3D::X3DConstants::SFVec3f
	};

	for (size_t i = 0, size = node -> getFieldDefinitions () .size (); i < size; ++ i)
	{
		const auto field = node -> getFieldDefinitions () [i];
	
		if (not field -> isInput ())
			continue;

		if (not fieldTypes .count (field -> getType ()))
			continue;

		const auto child = getTreeStore () -> append (parent -> children ());
		(*child) [columns .id]      = i;
		(*child) [columns .type]    = Gdk::Pixbuf::create_from_file (get_ui ("icons/FieldType/" + field -> getTypeName () + ".svg"));
		(*child) [columns .name]    = Glib::Markup::escape_text (field -> getName ());
		(*child) [columns .tainted] = false;
		(*child) [columns .visible] = true;

		field -> addInterest (this, &AnimationEditor::set_tainted, getTreeStore () -> get_path (child));
	}
}

std::string
AnimationEditor::getNodeName (const X3D::SFNode & node) const
{
	static const std::regex LastNumber_ (R"/(_\d+$)/");

	const auto name = std::regex_replace (node -> getName (), LastNumber_, "");

	return "<b>" + Glib::Markup::escape_text (node -> getTypeName ()) + "</b> " + Glib::Markup::escape_text (name);
}

void
AnimationEditor::set_name (const size_t id, const Gtk::TreePath & path)
{
	try
	{
		const auto & node = nodes .at (id);

		auto iter = getTreeStore () -> get_iter (path);

		(*iter) [columns .name] = getNodeName (node);
	}
	catch (const std::out_of_range &)
	{ }
}

void
AnimationEditor::set_live (const size_t id, const Gtk::TreePath & path)
{
	try
	{
		const auto & node = nodes .at (id);

		auto iter = getTreeStore () -> get_iter (path);

		(*iter) [columns .visible] = node -> isLive () .getValue ();

		getTreeModelFilter () -> row_has_child_toggled (getTreeModelFilter () -> convert_child_path_to_path (path),
		                                                getTreeModelFilter () -> convert_child_iter_to_iter (iter));
	}
	catch (const std::out_of_range &)
	{ }
}

void
AnimationEditor::set_tainted (const Gtk::TreePath & path)
{
	if (frameChange)
	{
		-- frameChange;
		return;
	}

	// Mark node and field as tainted.

	setTainted (path, true);
}

void
AnimationEditor::setTainted (const Gtk::TreePath & path, const bool value)
{
	if (value)
	{
		if (path .size () == 3)
		{
			auto parentPath = path;
			parentPath .up ();

			const auto master = getTreeStore () -> get_iter (Gtk::TreePath ("0"));
			const auto parent = getTreeStore () -> get_iter (parentPath);
			const auto child  = getTreeStore () -> get_iter (path);

			(*master) [columns .tainted] = true;
			(*parent) [columns .tainted] = true;
			(*child)  [columns .tainted] = true;
		}
	}
	else
	{
		switch (path .size ())
		{
			case 1:
			{
				const auto master = getTreeModelFilter () -> get_iter (path);
				
				if (not (*master) [columns .tainted])
					break;

				(*master) [columns .tainted] = false;

				for (const auto & parent : master -> children ())
				{
					if (not (*parent) [columns .tainted])
						continue;

					(*parent) [columns .tainted] = false;
				
					for (const auto & child : parent -> children ())
						(*child) [columns .tainted] = false;
				}
				
				break;
			}
			case 3:
			{
				auto parentPath = path;
				parentPath .up ();

				const auto master = getTreeStore () -> get_iter (Gtk::TreePath ("0"));
				const auto parent = getTreeStore () -> get_iter (parentPath);
				const auto child  = getTreeStore () -> get_iter (path);

				(*child)  [columns .tainted] = false;
				(*parent) [columns .tainted] = hasTaintedChildren (parent);
				(*master) [columns .tainted] = hasTaintedChildren (master);
				
				break;
			}
			default:
				break;
		}
	}
}

bool
AnimationEditor::hasTaintedChildren (const Gtk::TreeIter & parent) const
{
	for (const auto & child : parent -> children ())
	{
		if ((*child) [columns .tainted])
			return true;
	}

	return false;
}

void
AnimationEditor::set_user_defined_fields (const size_t id, const Gtk::TreePath & path)
{
	// A Script node's fields have changed.  We must update all children of path now.
	
	on_clear_selection ();
	on_clear_clipboard ();

	try
	{
		const auto & node = nodes .at (id);

		auto parent = getTreeStore () -> get_iter (path);

		for (auto child = parent -> children () .begin (); child; child = getTreeStore () -> erase (*child))
			;
		
		addFields (node, parent);
	}
	catch (const std::out_of_range &)
	{ }
}

void
AnimationEditor::on_close ()
{
	set_animation (nullptr);
}

void
AnimationEditor::on_cut ()
{
	on_copy ();

	removeKeyframes ();
}

void
AnimationEditor::on_copy ()
{
	copiedFrames .clear ();

	for (auto & frame : selectedFrames)
	{
		try
		{
			const auto & interpolator = interpolatorIndex .at (std::get <1> (frame));
			const auto & components   = interpolatorComponents .at (interpolator -> getType () .back ());
			auto &       key          = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key",       true);
			auto &       keyValue     = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
			auto &       keyType      = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType",  true);
			const auto   iter         = std::lower_bound (key .begin (), key .end (), std::get <0> (frame));
			const auto   index        = iter - key .begin ();
			const auto   indexN       = index * components;

			if (iter == key .end ())
				continue;

			std::vector <double> value;

			for (size_t i = 0; i < components; ++ i)
					value .emplace_back (keyValue .at (indexN + i));

			copiedFrames .emplace_back (std::get <0> (frame), std::get <1> (frame), std::get <2> (frame), value, keyType .at (index));
		}
		catch (const std::out_of_range &)
		{ }
	}

	getPasteButton () .set_sensitive (not copiedFrames .empty ());
}

void
AnimationEditor::on_paste ()
{
	if (copiedFrames .empty ())
		return;
		
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Paste Keyframes"));

	std::sort (copiedFrames .begin (), copiedFrames .end (), [ ] (const CopiedFrame & lhs, const CopiedFrame & rhs)
	{
		return std::get <0> (lhs) < std::get <0> (rhs);
	});

	const int32_t offset = std::round (getFrameAdjustment () -> get_value ()) - std::get <0> (copiedFrames .front ());

	std::set <X3D::X3DPtr <X3D::X3DNode>> affectedInterpolators;

	on_clear_selection ();

	for (auto & frame : copiedFrames)
	{
		try
		{
			std::get <0> (frame) += offset;

			const auto & interpolator = interpolatorIndex .at (std::get <1> (frame));
		
			addKeyframe (interpolator, std::get <0> (frame), std::get <3> (frame), std::get <4> (frame), undoStep);

			affectedInterpolators .emplace (interpolator);

			selectedFrames .emplace (std::get <0> (frame), std::get <1> (frame), std::get <2> (frame));
		}
		catch (const std::out_of_range &)
		{ }
	}

	on_selection_changed ();

	for (const auto & interpolator : affectedInterpolators)
		setInterpolator (interpolator, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
AnimationEditor::on_clear_clipboard ()
{
	copiedFrames .clear ();

	getPasteButton () .set_sensitive (false);
}

void
AnimationEditor::on_first_frame ()
{
	timeSensor -> stopTime () = chrono::now ();

	getFrameAdjustment () -> set_value (0);
}

void
AnimationEditor::on_play_pause ()
{
	// Determine range to play and frame where the animation should start.

	if (not timeSensor -> isActive ())
	{
		const double  duration     = getDuration ();
		int32_t       firstFrame   = selectedRange .first;
		int32_t       lastFrame    = selectedRange .second;
		double        currentFrame = getFrameAdjustment () -> get_value ();

		if (firstFrame > lastFrame)
			std::swap (firstFrame, lastFrame);

		if (firstFrame == lastFrame)
		{
			firstFrame = 0;
			lastFrame  = getDuration ();
		}
		else
			currentFrame = math::clamp <double> (currentFrame, firstFrame, lastFrame);

		if (currentFrame >= lastFrame)
			currentFrame = firstFrame;

		timeSensor -> range () = { firstFrame / duration, currentFrame / duration, lastFrame / duration };
	}

	// Start TimeSensor.

	timeSensor -> stopTime () = chrono::now ();

	if (not isActive ())
		timeSensor -> startTime () = chrono::now ();

	// Clear tainted states.

	setTainted (Gtk::TreePath ("0"), false);
}

void
AnimationEditor::on_last_frame ()
{
	timeSensor -> stopTime () = chrono::now ();

	getFrameAdjustment () -> set_value (getDuration ());
}

void
AnimationEditor::on_current_frame_changed ()
{
	// Display current time frame.
	
	const int32_t frame = std::round (getFrameAdjustment () -> get_value ());

	getTimeLabel () .set_text (strfframes (frame, getFramesPerSecond ()));

	if (animation)
	{
		if (isActive ())
		{
			// Follow cursor if animation is active.

			const int32_t x     = frame * getScale () + getTranslation ();
			const int32_t width = getDrawingArea () .get_width ();

			if (x < 0 or x > width)
				setTranslation (width * (2 - M_PHI) - frame * getScale ());
		}
		else
		{
			on_update_fraction ();
		}
	}

	getDrawingArea () .queue_draw ();
}

void
AnimationEditor::on_update_fraction ()
{
	// Clear tainted states.

	setTainted (Gtk::TreePath ("0"), false);

	// Update interpolator fraction.

	const double fraction = getFrameAdjustment () -> get_value () / getDuration ();

	for (const auto & interpolator : interpolators)
	{
		try
		{
			interpolator -> getField <X3D::SFFloat> ("set_fraction") = fraction;
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

void
AnimationEditor::on_time ()
{
	getNewNameEntry () .set_text (animation -> getName ());
	getDurationAdjustment () -> set_value (getDuration ());
	getFPSAdjustment () -> set_value (getFramesPerSecond ());
	getLoopSwitch () .set_active (timeSensor -> loop ());

	const auto responseId = getPropertiesDialog () .run ();

	getPropertiesDialog () .hide ();

	if (responseId not_eq Gtk::RESPONSE_OK)
		return;

	using setMetaData = void (X3D::X3DNode::*) (const std::string &, const int32_t &);

	const auto undoStep        = std::make_shared <X3D::UndoStep> (_ ("Edit Keyframe Animation Properties"));
	const auto name            = getNewNameEntry () .get_text ();
	const bool durationChanged = getDuration () not_eq getDurationAdjustment () -> get_value ();

	undoStep -> addUndoFunction (&Gtk::DrawingArea::queue_draw, std::ref (getDrawingArea ()));

	if (durationChanged)
		undoStep -> addUndoFunction (&AnimationEditor::on_zoom_fit, this);
	
	if (getScaleKeyframesButton () .get_active ())
		scaleKeyframes (getDuration (), getDurationAdjustment () -> get_value (), undoStep);

	undoStep -> addObjects (timeSensor);

	getBrowserWindow () -> updateNamedNode (getCurrentContext (), name, X3D::SFNode (animation), undoStep);

	// Adjust metadata

	undoStep -> addUndoFunction ((setMetaData) &X3D::X3DNode::setMetaData, animation, "/Animation/duration", getDuration ());
	undoStep -> addRedoFunction ((setMetaData) &X3D::X3DNode::setMetaData, animation, "/Animation/duration", getDurationAdjustment () -> get_value ());
	animation -> setMetaData <int32_t> ("/Animation/duration", getDurationAdjustment () -> get_value ());
	animation -> getMetaData <X3D::MFInt32> ("/Animation/duration", true) .addInterest (this, &AnimationEditor::set_duration);

	undoStep -> addUndoFunction ((setMetaData) &X3D::X3DNode::setMetaData, animation, "/Animation/framesPerSecond", getFramesPerSecond ());
	undoStep -> addRedoFunction ((setMetaData) &X3D::X3DNode::setMetaData, animation, "/Animation/framesPerSecond", getFPSAdjustment () -> get_value ());
	animation -> setMetaData <int32_t> ("/Animation/framesPerSecond", getFPSAdjustment () -> get_value ());
	animation -> getMetaData <X3D::MFInt32> ("/Animation/framesPerSecond", true) .addInterest (this, &AnimationEditor::set_frames_per_second);

	// Adjust TimeSensor

	const auto cycleInterval = getDuration () / (double) getFramesPerSecond ();
	undoStep -> addUndoFunction (&X3D::SFTime::setValue, std::ref (timeSensor -> cycleInterval ()), timeSensor -> cycleInterval ());
	undoStep -> addRedoFunction (&X3D::SFTime::setValue, std::ref (timeSensor -> cycleInterval ()), cycleInterval);
	timeSensor -> cycleInterval () = cycleInterval;

	undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (timeSensor -> loop ()), timeSensor -> loop ());
	undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (timeSensor -> loop ()), getLoopSwitch () .get_active ());
	timeSensor -> loop () = getLoopSwitch () .get_active ();

	undoStep -> addUndoFunction (&X3D::SFTime::setValue, std::ref (timeSensor -> stopTime ()), std::bind (&chrono::now));
	undoStep -> addRedoFunction (&X3D::SFTime::setValue, std::ref (timeSensor -> stopTime ()), std::bind (&chrono::now));
	timeSensor -> stopTime () = chrono::now ();

	// Build interpolators.
	setInterpolators (undoStep);

	if (durationChanged)
	{
		undoStep -> addRedoFunction (&AnimationEditor::on_zoom_fit, this);
		on_zoom_fit ();
	}

	undoStep -> addRedoFunction (&Gtk::DrawingArea::queue_draw, std::ref (getDrawingArea ()));

	getBrowserWindow () -> addUndoStep (undoStep);

}

void
AnimationEditor::set_duration ()
{
	getFrameAdjustment () -> set_value (std::min <double> (getFrameAdjustment () -> get_value (), getDuration ()));
	getFrameAdjustment () -> set_upper (getDuration ());
	setTranslation (getTranslation ());
	setLowerTranslation ();
}

void
AnimationEditor::set_frames_per_second ()
{
	setTranslation (getTranslation ());
}

void
AnimationEditor::set_active ()
{
	const bool active = isActive ();

	getCutButton ()        .set_sensitive (not active);
	getCopyButton ()       .set_sensitive (not active);
	getPasteButton ()      .set_sensitive (not active);
	getFirstFrameButton () .set_sensitive (not active);
	getLastFrameButton ()  .set_sensitive (not active);
	getTimeButton ()       .set_sensitive (not active);

	getPlayPauseButton () .set_stock_id (Gtk::StockID (active ? "gtk-media-pause" : "gtk-media-play"));
}

void
AnimationEditor::set_fraction (const float value)
{
	getFrameAdjustment () -> set_value (getDuration () * value);
}

void
AnimationEditor::set_value ()
{
	++ frameChange;
}

void
AnimationEditor::on_key_type_changed ()
{
	if (changing)
		return;

	// Set active key frame type choice.

	static std::vector <std::string> keyTypes = {
		"CONSTANT",
		"LINEAR",
		"SPLINE",
		"SPLIT"
	};

	try
	{
		activeType = keyTypes .at (getKeyTypeButton () .get_active_row_number ());
	}
	catch (const std::out_of_range &)
	{
		activeType = "SPLINE";
	}

	// Change key frame type of selection.

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Keyframe Type"));

	std::set <X3D::X3DPtr <X3D::X3DNode>> affectedInterpolators;

	for (const auto & frame : selectedFrames)
	{
		try
		{
			const auto & interpolator = interpolatorIndex .at (std::get <1> (frame));
			const auto & key          = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key",      true);
			auto &       keyType      = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType", true);
			const auto   iter         = std::lower_bound (key .begin (), key .end (), std::get <0> (frame));
			const auto   index        = iter - key .begin ();

			if (key .at (index) not_eq std::get <0> (frame))
				continue;

			if (affectedInterpolators .emplace (interpolator) .second)
				undoStep -> addUndoFunction ((setMetaDataString) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyType", keyType);
	
			switch (interpolator -> getType () .back ())
			{
				case X3D::X3DConstants::BooleanSequencer:
				case X3D::X3DConstants::IntegerSequencer:
				{
					keyType .set1Value (index, "CONSTANT");
					break;
				}
				case X3D::X3DConstants::ColorInterpolator:
				case X3D::X3DConstants::ScalarInterpolator:
				case X3D::X3DConstants::OrientationInterpolator:
				case X3D::X3DConstants::PositionInterpolator2D:
				case X3D::X3DConstants::PositionInterpolator:
				{
					keyType .set1Value (index, activeType);
					break;
				}
				default:
					break;
			}
		}
		catch (const std::exception &)
		{ }		
	}

	for (const auto & interpolator : affectedInterpolators)
	{
		try
		{
			const auto & keyType = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType", true);

			undoStep -> addRedoFunction ((setMetaDataString) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyType", keyType);

			setInterpolator (interpolator, undoStep);
		}
		catch (const std::exception &)
		{ }		
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
AnimationEditor::set_key_type ()
{
	bool first        = true;
	bool inconsistent = false;
	auto currentType  = activeType;

	for (const auto & frame : selectedFrames)
	{
		try
		{
			const auto & interpolator = interpolatorIndex .at (std::get <1> (frame));
			auto &       key          = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key", false);
			auto &       keyType      = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType", false);
			const auto   iter         = std::lower_bound (key .begin (), key .end (), std::get <0> (frame));
			const auto   index        = iter - key .begin ();

			if (key .at (index) not_eq std::get <0> (frame))
				continue;

			if (first)
			{
				currentType = keyType .at (index);
				first       = false;
			}
			else
			{
				if (keyType .at (index) not_eq currentType)
				{
					inconsistent = true;
					break;
				}
			}
		}
		catch (const std::exception &)
		{ }		
	}

	changing = true;

	if (inconsistent)
		getKeyTypeButton () .set_active (-1);
	else
		getKeyTypeButton () .set_active_text (currentType);

	changing = false;
}

void
AnimationEditor::on_zoom_out ()
{
	on_zoom (getDrawingArea () .get_width () / 2, GDK_SCROLL_DOWN);
}

void
AnimationEditor::on_zoom_in ()
{
	on_zoom (getDrawingArea () .get_width () / 2, GDK_SCROLL_UP);
}

void
AnimationEditor::on_zoom (const double position, const GdkScrollDirection direction)
{
	const auto fromFrame = (position - getTranslation ()) / getScale ();

	if (direction == GDK_SCROLL_DOWN)     // Move backwards.
	{
		setScale (getScale () / SCROLL_FACTOR);
	}

	else if (direction == GDK_SCROLL_UP) // Move forwards.
	{
		setScale (getScale () * SCROLL_FACTOR);
	}

	const auto toFrame = (position - getTranslation ()) / getScale ();
	const auto offset  = (toFrame - fromFrame) * getScale ();

	setTranslation (getTranslation () + offset);
}

void
AnimationEditor::on_zoom_fit ()
{
	if (animation)
	{
		const double width = getDrawingArea () .get_width () - 2 * DEFAULT_TRANSLATION;

		setScale (width / getDuration ());
		setTranslation (DEFAULT_TRANSLATION);
	}
}

void
AnimationEditor::on_zoom_100 ()
{
	const auto frame = getFrameAdjustment () -> get_value ();
	const auto x     = frame * getScale () + getTranslation ();

	setScale (DEFAULT_SCALE);
	setTranslation (x - frame * DEFAULT_SCALE);
}

void
AnimationEditor::on_row_activated (const Gtk::TreePath & path, Gtk::TreeViewColumn*)
{
	addKeyframe (path);
}

void
AnimationEditor::on_tainted_toggled (const Glib::ustring & path)
{
	addKeyframe (Gtk::TreePath (path));
}

void
AnimationEditor::addKeyframe (const Gtk::TreePath & path)
{
	if (isActive ())
		return;

	switch (path .size ())
	{
		case 1:
		{
			const auto master = getTreeModelFilter () -> get_iter (path);

			for (const auto & parent : master -> children ())
			{
				const auto parentPath = getTreeModelFilter () -> get_path (parent);
			
				for (const auto & child : parent -> children ())
				{
					if ((*child) [columns .tainted])
						addKeyframe (parentPath, getTreeModelFilter () -> get_path (child));
				}
			}

			break;
		}
		case 2:
		{
			const auto parent = getTreeModelFilter () -> get_iter (path);
			
			for (const auto & child : parent -> children ())
			{
				if ((*child) [columns .tainted])
					addKeyframe (path, getTreeModelFilter () -> get_path (child));
			}

			break;
		}
		case 3:
		{
			auto parentPath = path;
			parentPath .up ();
			addKeyframe (parentPath, path);
			break;
		}
		default:
			break;
	}
}

void
AnimationEditor::addKeyframe (const Gtk::TreePath & parentPath, const Gtk::TreePath & path)
{
	try
	{
		const auto   parent = getTreeModelFilter () -> get_iter (parentPath);
		const auto   child  = getTreeModelFilter () -> get_iter (path);
		const size_t id     = (*parent) [columns .id];
		const size_t i      = (*child) [columns .id];
		const auto & node   = nodes .at (id);
		const auto   field  = node -> getFieldDefinitions () .at (i);

		addKeyframe (node, field);

		setTainted (getTreeModelFilter () -> convert_path_to_child_path (path), false);
	}
	catch (const std::out_of_range &)
	{ }
}

void
AnimationEditor::addKeyframe (const X3D::SFNode & node, const X3D::X3DFieldDefinition* const field)
{
	const auto    undoStep = std::make_shared <X3D::UndoStep> (_ ("Add Keyframe"));
	const int32_t frame    = std::round (getFrameAdjustment () -> get_value ());

	std::vector <double> value;

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		{
			const auto   interpolator = getInterpolator ("BooleanSequencer", node, field, undoStep);
			const auto & boolean      = *static_cast <const X3D::SFBool*> (field);

			value .emplace_back (boolean);

			addKeyframe (interpolator, frame, value, "CONSTANT", undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		case X3D::X3DConstants::SFInt32:
		{
			const auto   interpolator = getInterpolator ("IntegerSequencer", node, field, undoStep);
			const auto & integer      = *static_cast <const X3D::SFInt32*> (field);

			value .emplace_back (integer);

			addKeyframe (interpolator, frame, value, "CONSTANT", undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		case X3D::X3DConstants::SFColor:
		{
			const auto   interpolator = getInterpolator ("ColorInterpolator", node, field, undoStep);
			const auto & color        = *static_cast <const X3D::SFColor*> (field);

			value .emplace_back (color .getRed ());
			value .emplace_back (color .getGreen ());
			value .emplace_back (color .getBlue ());

			addKeyframe (interpolator, frame, value, activeType, undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		case X3D::X3DConstants::SFFloat:
		{
			const auto   interpolator = getInterpolator ("ScalarInterpolator", node, field, undoStep);
			const auto & scalar       = *static_cast <const X3D::SFFloat*> (field);

			value .emplace_back (scalar);

			addKeyframe (interpolator, frame, value, activeType, undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		case X3D::X3DConstants::SFRotation:
		{
			const auto   interpolator = getInterpolator ("OrientationInterpolator", node, field, undoStep);
			const auto & rotation     = *static_cast <const X3D::SFRotation*> (field);

			value .emplace_back (rotation .getX ());
			value .emplace_back (rotation .getY ());
			value .emplace_back (rotation .getZ ());
			value .emplace_back (rotation .getAngle ());

			addKeyframe (interpolator, frame, value, activeType, undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		case X3D::X3DConstants::SFVec2f:
		{
			const auto   interpolator = getInterpolator ("PositionInterpolator2D", node, field, undoStep);
			const auto & vector       = *static_cast <const X3D::SFVec2f*> (field);

			value .emplace_back (vector .getX ());
			value .emplace_back (vector .getY ());

			addKeyframe (interpolator, frame, value, activeType, undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		case X3D::X3DConstants::SFVec3f:
		{
			const auto   interpolator = getInterpolator ("PositionInterpolator", node, field, undoStep);
			const auto & vector       = *static_cast <const X3D::SFVec3f*> (field);

			value .emplace_back (vector .getX ());
			value .emplace_back (vector .getY ());
			value .emplace_back (vector .getZ ());

			addKeyframe (interpolator, frame, value, activeType, undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		default:
			break;
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
AnimationEditor::addKeyframe (const X3D::X3DPtr <X3D::X3DNode> & interpolator,
                              const int32_t frame,
                              const std::vector <double> & value,
                              const std::string & type,
                              const X3D::UndoStepPtr & undoStep)
{
	const size_t components = value .size ();

	auto &     key      = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key",       true);
	auto &     keyValue = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
	auto &     keyType  = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType",  true);
	const auto iter     = std::lower_bound (key .begin (), key .end (), frame);
	const auto index    = iter - key .begin ();
	const auto indexN   = index * components;

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	// Set meta data

	undoStep -> addUndoFunction ((setMetaDataInteger) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/key",      key);
	undoStep -> addUndoFunction ((setMetaDataDouble)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addUndoFunction ((setMetaDataString)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyType",  keyType);

	if (iter == key .end () or frame == key .get1Value (index))
	{
		key .set1Value (index, frame);
		keyType .set1Value (index, type);

		for (size_t i = 0; i < components; ++ i)
			keyValue .set1Value (indexN + i, value [i]);
	}
	else
	{
		key .emplace (key .begin () + index, frame);
		keyType .emplace (keyType .begin () + index, type);
		
		for (const auto & v : basic::make_reverse_range (value))
			keyValue .emplace (keyValue .begin () + indexN, v);
	}

	undoStep -> addRedoFunction ((setMetaDataInteger) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/key",      key);
	undoStep -> addRedoFunction ((setMetaDataDouble)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addRedoFunction ((setMetaDataString)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyType",  keyType);
}

void
AnimationEditor::moveKeyframes ()
{
	// Move frames within interpolators.

	using FrameToMove = std::tuple <int32_t, int32_t, const X3D::X3DFieldDefinition*>; // fromFrame, toFrame, field

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Keyframes"));

	// Sort
	// The keyframes must be sorted depending on distance to avoid accidantialy removal of keyframes.

	std::vector <FrameToMove> framesToMove;

	auto movedFrame    = movedFrames .begin ();
	auto selectedFrame = selectedFrames .begin ();
	
	for (; movedFrame not_eq movedFrames .end () and selectedFrame not_eq selectedFrames .end (); ++ movedFrame, ++ selectedFrame)
	{
		framesToMove .emplace_back (std::get <0> (*selectedFrame), // fromFrame
		                            std::get <0> (*movedFrame),    // toFrame
		                            std::get <1> (*movedFrame));   // field
	}

	if (framesToMove .empty ())
		return;

	const int32_t distance = std::get <1> (framesToMove [0]) - std::get <0> (framesToMove [0]);

	std::sort (framesToMove .begin (), framesToMove .end (), [distance] (const FrameToMove & lhs, const FrameToMove & rhs)
	{
		if (distance < 0)
			return std::get <0> (lhs) < std::get <0> (rhs);

		return std::get <0> (lhs) > std::get <0> (rhs);
	});

	// Move

	std::set <X3D::X3DPtr <X3D::X3DNode>> affectedInterpolators;

	for (const auto & frame : framesToMove)
	{
		try
		{
			const auto & interpolator = interpolatorIndex .at (std::get <2> (frame));
		
			affectedInterpolators .emplace (interpolator);

			moveKeyframe (interpolator, std::get <0> (frame), std::get <1> (frame), undoStep);
		}
		catch (...)
		{ }
	}

	for (const auto & interpolator : affectedInterpolators)
		setInterpolator (interpolator, undoStep);

	selectedFrames .clear ();
	selectedFrames .insert (movedFrames .begin (), movedFrames .end ());
	movedFrames .clear ();
	on_selection_changed ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
AnimationEditor::moveKeyframe (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const int32_t fromFrame, const int32_t toFrame, const X3D::UndoStepPtr & undoStep)
{
	if (fromFrame == toFrame)
		return;

	try
	{
		moveKeyframe (interpolator, interpolatorComponents .at (interpolator -> getType () .back ()), fromFrame, toFrame, undoStep);
	}
	catch (const std::out_of_range &)
	{ }
}

void
AnimationEditor::moveKeyframe (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const size_t components, const int32_t fromFrame, const int32_t toFrame, const X3D::UndoStepPtr & undoStep)
{
	auto &     key      = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key",       true);
	auto &     keyValue = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
	auto &     keyType  = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType",  true);
	const auto iter     = std::lower_bound (key .begin (), key .end (), fromFrame);
	const auto index    = iter - key .begin ();
	const auto indexN   = index * components;

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	if (iter == key .end () or *iter not_eq fromFrame)
		return;

	const auto type = std::string (keyType [index]);

	std::vector <double> value;
	
	for (size_t i = 0; i < components; ++ i)
		value .emplace_back (keyValue [indexN + i]);

	removeKeyframe (X3D::X3DPtr <X3D::X3DNode> (interpolator), components, fromFrame, undoStep);
	addKeyframe (interpolator, toFrame, value, type, undoStep);
}

void
AnimationEditor::removeKeyframes ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Keyframes"));

	std::set <X3D::X3DPtr <X3D::X3DNode>> affectedInterpolators;

	for (const auto & frame : selectedFrames)
	{
		try
		{
			const auto & interpolator = interpolatorIndex .at (std::get <1> (frame));
		
			affectedInterpolators .emplace (interpolator);

			removeKeyframe (interpolator, std::get <0> (frame), undoStep);
		}
		catch (...)
		{ }
	}

	on_clear_selection ();

	for (const auto & interpolator : affectedInterpolators)
		setInterpolator (interpolator, undoStep);

	// Remove empty interpolators.

	X3D::MFNode interpolatorsToRemove;

	for (const auto & interpolator : interpolators)
	{
		if (interpolator  -> getMetaData <X3D::MFInt32> ("/Interpolator/key", true) .empty ())
			interpolatorsToRemove .emplace_back (interpolator);
	}

	getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), interpolatorsToRemove, true, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
AnimationEditor::removeKeyframe (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const int32_t frame, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		removeKeyframe (interpolator, interpolatorComponents .at (interpolator -> getType () .back ()), frame, undoStep);
	}
	catch (const std::out_of_range &)
	{ }
}

void
AnimationEditor::removeKeyframe (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const size_t components, const int32_t frame, const X3D::UndoStepPtr & undoStep)
{
	auto &     key      = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key",      true);
	auto &     keyValue = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
	auto &     keyType  = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType", true);
	const auto iter     = std::lower_bound (key .begin (), key .end (), frame);
	const auto index    = iter - key .begin ();
	const auto indexN   = index * components;

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	if (iter == key .end () or *iter not_eq frame)
		return;

	undoStep -> addUndoFunction ((setMetaDataInteger) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/key",      key);
	undoStep -> addUndoFunction ((setMetaDataDouble)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addUndoFunction ((setMetaDataString)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyType",  keyType);

	key      .erase (key .begin () + index);
	keyValue .erase (keyValue .begin () + indexN, keyValue .begin () + indexN + components);
	keyType  .erase (keyType .begin () + index);

	undoStep -> addRedoFunction ((setMetaDataInteger) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/key",      key);
	undoStep -> addRedoFunction ((setMetaDataDouble)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addRedoFunction ((setMetaDataString)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyType",  keyType);
}

void
AnimationEditor::scaleKeyframes (const int32_t fromDuration, const int32_t toDuration, const X3D::UndoStepPtr & undoStep)
{
	if (fromDuration == toDuration)
		return;

	for (const auto & interpolator : interpolators)
		scaleKeyframes (interpolator, fromDuration, toDuration, undoStep);
}

void
AnimationEditor::scaleKeyframes (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const int32_t fromDuration, const int32_t toDuration, const X3D::UndoStepPtr & undoStep)
{
	auto & key = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key", true);

	undoStep -> addUndoFunction ((setMetaDataInteger) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/key", key);

	for (auto & frame : key)
	{
		frame = std::round (frame / (double) fromDuration * toDuration);
	}

	undoStep -> addRedoFunction ((setMetaDataInteger) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/key", key);
}

void
AnimationEditor::setInterpolators (const X3D::UndoStepPtr & undoStep)
{
	// Build interpolators.

	for (const auto & interpolator : interpolators)
		setInterpolator (interpolator, undoStep);
}

void
AnimationEditor::setInterpolator (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const X3D::UndoStepPtr & undoStep)
{
	switch (interpolator -> getType () .back ())
	{
		case X3D::X3DConstants::BooleanSequencer:
		{
			resizeInterpolator (interpolator, 1, undoStep);
			setSequencer <X3D::BooleanSequencer, X3D::MFBool, bool> (X3D::X3DPtr <X3D::BooleanSequencer> (interpolator), undoStep);
			break;
		}
		case X3D::X3DConstants::IntegerSequencer:
		{
			resizeInterpolator (interpolator, 1, undoStep);
			setSequencer <X3D::IntegerSequencer, X3D::MFInt32, int32_t> (X3D::X3DPtr <X3D::IntegerSequencer> (interpolator), undoStep);
			break;
		}
		case X3D::X3DConstants::ColorInterpolator:
		{
			resizeInterpolator (interpolator, 3, undoStep);
			setInterpolator (X3D::X3DPtr <X3D::ColorInterpolator> (interpolator), undoStep);
			break;
		}
		case X3D::X3DConstants::ScalarInterpolator:
		{
			resizeInterpolator (interpolator, 1, undoStep);
			setInterpolator <X3D::ScalarInterpolator, X3D::MFFloat, double> (X3D::X3DPtr <X3D::ScalarInterpolator> (interpolator), undoStep);
			break;
		}
		case X3D::X3DConstants::OrientationInterpolator:
		{
			resizeInterpolator (interpolator, 4, undoStep);
			setInterpolator (X3D::X3DPtr <X3D::OrientationInterpolator> (interpolator), undoStep);
			break;
		}
		case X3D::X3DConstants::PositionInterpolator2D:
		{
			resizeInterpolator (interpolator, 2, undoStep);
			setInterpolator <X3D::PositionInterpolator2D, X3D::MFVec2f, X3D::Vector2d> (X3D::X3DPtr <X3D::PositionInterpolator2D> (interpolator), undoStep);
			break;
		}
		case X3D::X3DConstants::PositionInterpolator:
		{
			resizeInterpolator (interpolator, 3, undoStep);
			setInterpolator <X3D::PositionInterpolator, X3D::MFVec3f, X3D::Vector3d> (X3D::X3DPtr <X3D::PositionInterpolator> (interpolator), undoStep);
			break;
		}
		default:
			break;
	}
}

void
AnimationEditor::setInterpolator (const X3D::X3DPtr <X3D::ColorInterpolator> & interpolator, const X3D::UndoStepPtr & undoStep)
{
	using Type = X3D::Color3d;

	const auto   components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto & key        = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key",      true);
	auto &       keyValue   = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
	auto &       keyType    = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType", true);

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	undoStep -> addObjects (interpolator);
	undoStep -> addUndoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addUndoFunction (&X3D::MFColor::setValue, std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());

	size_t       i        = 0;
	size_t       iN       = 0;
	const size_t size     = key .size ();
	const auto   duration = getDuration ();

	interpolator -> key ()      .clear ();
	interpolator -> keyValue () .clear ();

	while (i < size)
	{
		if (key [i] < 0 or key [i] > duration)
			continue;

		const auto fraction = key [i] / (double) duration;
		const auto value    = Type (keyValue [iN], keyValue [iN + 1], keyValue [iN + 2]);

		if (keyType [i] == "CONSTANT")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);

			if (key [i] < duration)
			{
				const auto nextFraction = (i == size - 1 ? 1 : key [i + 1] / (double) duration - epsilon);

				interpolator -> key ()      .emplace_back (nextFraction);
				interpolator -> keyValue () .emplace_back (value);
			}
		}
		else if (keyType [i] == "LINEAR")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);
		}
		else if (keyType [i] == "SPLINE" or keyType [i] == "SPLIT")
		{
			std::vector <int32_t> keys;
			std::vector <X3D::Rotation4d> keyValuesH;
			std::vector <X3D::Vector2d> keyValuesSV;
			std::vector <X3D::Vector2d> keyVelocitysSV;
			std::vector <Type> keyValues;

			for (; i < size; ++ i, iN += components)
			{
				double h, s, v;
				const auto value = Type (keyValue [iN], keyValue [iN + 1], keyValue [iN + 2]);
	
				value .get_hsv (h, s, v);

				keys .emplace_back (key [i]);
				keyValuesH  .emplace_back (0, 0, 1, h);
				keyValuesSV .emplace_back (s, v);
				keyValues .emplace_back (value);
				
				if (keys .size () == 1)
					continue;

				if (keyType [i] not_eq "SPLINE")
					break;
			}
	
			if (keys .size () < 2)
			{
				// This can happen if only the last frame is of type SPLINE.
				interpolator -> key ()      .emplace_back (fraction);
				interpolator -> keyValue () .emplace_back (value);
				break;
			}
	
			const bool normalizeVelocity = false;
			const bool closed            = keys .front () == 0 and keys .back () == duration and keyValues .front () == keyValues .back ();

			const math::squad_interpolator <X3D::Rotation4d, double> squad (closed, keys, keyValuesH);
			const math::catmull_rom_spline_interpolator <X3D::Vector2d, double> spline (closed, keys, keyValuesSV, keyVelocitysSV, normalizeVelocity);

			for (size_t k = 0, size = keys .size () - 1; k < size; ++ k)
			{
				const int32_t frames   = keys [k + 1] - keys [k];
				const double  fraction = keys [k] / (double) duration;
				const double  distance = frames / (double) duration;

				for (int32_t f = 0; f < frames; ++ f)
				{
					const auto weight = f / (double) frames;
	
					try
					{
						const auto valueH  = squad  .interpolate (k, k + 1, weight, keyValuesH);
						const auto valueSV = spline .interpolate (k, k + 1, weight, keyValuesSV);
						const bool negate  = math::dot (valueH .axis (), X3D::Vector3d (0, 0, 1)) < 0;
						const auto hue     = negate ? -valueH .angle () : valueH .angle ();

						Type value;
						value .set_hsv (hue, valueSV .x (), valueSV .y ());

						interpolator -> key ()      .emplace_back (fraction + weight * distance);
						interpolator -> keyValue () .emplace_back (value);
					}
					catch (const std::domain_error &)
					{
						interpolator -> key () .emplace_back (fraction + weight * distance);
						
						if (interpolator -> keyValue () .empty ())
							interpolator -> keyValue () .emplace_back (keyValues [k]);
						else
							interpolator -> keyValue () .emplace_back (interpolator -> keyValue () .back ());
					}
				}
			}

			if (i == size)
			{
				// If this is the last part then we must insert the last keyframe.
				interpolator -> key ()      .emplace_back (keys .back () / (double) duration);
				interpolator -> keyValue () .emplace_back (keyValues .back ());
				break;
			}

			continue;
		}

		++ i;
		iN += components;
	}

	undoStep -> addRedoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addRedoFunction (&X3D::MFColor::setValue, std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());
}

void
AnimationEditor::setInterpolator (const X3D::X3DPtr <X3D::OrientationInterpolator> & interpolator, const X3D::UndoStepPtr & undoStep)
{
	using Type = X3D::Rotation4d;

	const auto   components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto & key        = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key",      true);
	auto &       keyValue   = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
	auto &       keyType    = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType",  true);

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	undoStep -> addObjects (interpolator);
	undoStep -> addUndoFunction (&X3D::MFFloat::setValue,    std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addUndoFunction (&X3D::MFRotation::setValue, std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());

	size_t       i        = 0;
	size_t       iN       = 0;
	const size_t size     = key .size ();
	const auto   duration = getDuration ();

	interpolator -> key ()      .clear ();
	interpolator -> keyValue () .clear ();

	while (i < size)
	{
		if (key [i] < 0 or key [i] > duration)
			continue;

		const auto fraction = key [i] / (double) duration;
		const auto value    = Type (keyValue [iN], keyValue [iN + 1], keyValue [iN + 2], keyValue [iN + 3]);

		if (keyType [i] == "CONSTANT")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);

			if (key [i] < duration)
			{
				const auto nextFraction = (i == size - 1 ? 1 : key [i + 1] / (double) duration - epsilon);

				interpolator -> key ()      .emplace_back (nextFraction);
				interpolator -> keyValue () .emplace_back (value);
			}
		}
		else if (keyType [i] == "LINEAR")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);
		}
		else if (keyType [i] == "SPLINE" or keyType [i] == "SPLIT")
		{
			std::vector <int32_t> keys;
			std::vector <Type> keyValues;

			for (; i < size; ++ i, iN += components)
			{
				const auto value = Type (keyValue [iN], keyValue [iN + 1], keyValue [iN + 2], keyValue [iN + 3]);

				keys .emplace_back (key [i]);
				keyValues .emplace_back (value);
				
				if (keys .size () == 1)
					continue;

				if (keyType [i] not_eq "SPLINE")
					break;
			}
	
			if (keys .size () < 2)
			{
				// This can happen if only the last frame is of type SPLINE.
				interpolator -> key ()      .emplace_back (fraction);
				interpolator -> keyValue () .emplace_back (value);
				break;
			}
	
			const bool closed = keys .front () == 0 and keys .back () == duration and keyValues .front () == keyValues .back ();

			const math::squad_interpolator <Type, double> squad (closed, keys, keyValues);

			for (size_t k = 0, size = keys .size () - 1; k < size; ++ k)
			{
				const int32_t frames   = keys [k + 1] - keys [k];
				const double  fraction = keys [k] / (double) duration;
				const double  distance = frames / (double) duration;

				for (int32_t f = 0; f < frames; ++ f)
				{
					const auto weight = f / (double) frames;
	
					try
					{
						const auto value = squad .interpolate (k, k + 1, weight, keyValues);

						interpolator -> key ()      .emplace_back (fraction + weight * distance);
						interpolator -> keyValue () .emplace_back (value);
					}
					catch (const std::domain_error &)
					{
						interpolator -> key () .emplace_back (fraction + weight * distance);
						
						if (interpolator -> keyValue () .empty ())
							interpolator -> keyValue () .emplace_back (keyValues [k]);
						else
							interpolator -> keyValue () .emplace_back (interpolator -> keyValue () .back ());
					}
				}
			}

			if (i == size)
			{
				// If this is the last part then we must insert the last keyframe.
				interpolator -> key ()      .emplace_back (keys .back () / (double) duration);
				interpolator -> keyValue () .emplace_back (keyValues .back ());
				break;
			}

			continue;
		}

		++ i;
		iN += components;
	}

	undoStep -> addRedoFunction (&X3D::MFFloat::setValue,    std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addRedoFunction (&X3D::MFRotation::setValue, std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());
}

void
AnimationEditor::resizeInterpolator (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const size_t components, const X3D::UndoStepPtr & undoStep)
{
	auto &     key      = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key",      true);
	auto &     keyValue = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
	auto &     keyType  = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType",  true);
	const auto iter     = std::upper_bound (key .begin (), key .end (), getDuration ());
	const auto size     = iter - key .begin ();
	const auto sizeN    = size * components;

	// Remove frames greater than duration.

	undoStep -> addUndoFunction ((setMetaDataInteger) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/key",      key);
	undoStep -> addUndoFunction ((setMetaDataDouble)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addUndoFunction ((setMetaDataString)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyType",  keyType);

	key      .resize (size);
	keyValue .resize (sizeN);
	keyType  .resize (size);

	undoStep -> addRedoFunction ((setMetaDataInteger) &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/key",      key);
	undoStep -> addRedoFunction ((setMetaDataDouble)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addRedoFunction ((setMetaDataString)  &X3D::X3DNode::setMetaData, interpolator, "/Interpolator/keyType",  keyType);
}

X3D::X3DPtr <X3D::X3DNode>
AnimationEditor::getInterpolator (const std::string & typeName, 
                                  const X3D::SFNode & node,
                                  const X3D::X3DFieldDefinition* const field,
                                  const X3D::UndoStepPtr & undoStep)
{
	try
	{
		return interpolatorIndex .at (field);
	}
	catch (const std::out_of_range &)
	{
		const auto interpolator = getCurrentContext () -> createNode (typeName);
		const auto name         = getInterpolatorName (node, field);
		
		interpolator -> getField ("value_changed") -> addInterest (this, &AnimationEditor::set_value);

		getCurrentContext () -> addUninitializedNode (interpolator);
		getCurrentContext () -> realize ();

		const X3D::X3DPtr <X3D::X3DNode> interpolatorNode (interpolator);
		interpolatorIndex .emplace (field, interpolatorNode);
		interpolators .emplace (interpolatorNode);

		undoStep -> addObjects (animation);
		getBrowserWindow () -> pushBackIntoArray (X3D::SFNode (animation), animation -> children (), interpolator, undoStep);
		getBrowserWindow () -> addRoute (getCurrentContext (), X3D::SFNode (timeSensor), "fraction_changed", interpolator, "set_fraction", undoStep);
		getBrowserWindow () -> addRoute (getCurrentContext (), interpolator, "value_changed", node, field -> getName (), undoStep);
		getBrowserWindow () -> updateNamedNode (getCurrentContext (), name, interpolator, undoStep);
		return interpolatorNode;
	}
}

std::string
AnimationEditor::getInterpolatorName (const X3D::SFNode & node, const X3D::X3DFieldDefinition* const field)
{
	Glib::ustring name = field -> getName ();
	name .replace (0, 1, Glib::ustring (1, name [0]) .uppercase ());

	return X3D::get_display_name (node) + name + "Interpolator";
}

bool
AnimationEditor::on_configure_event (GdkEventConfigure*)
{
	setLowerTranslation ();
	return false;
}

bool
AnimationEditor::on_focus_in_event (GdkEventFocus*)
{
	getBrowserWindow () -> hasAccelerators (false);
	getBrowserWindow () -> getWindow () .add_accel_group (getAccelGroup ());
	getDrawingArea () .queue_draw ();
	return false;
}

bool
AnimationEditor::on_focus_out_event (GdkEventFocus*)
{
	keys .clear ();
	getBrowserWindow () -> getWindow () .remove_accel_group (getAccelGroup ());
	getBrowserWindow () -> hasAccelerators (true);
	getDrawingArea () .queue_draw ();
	return false;
}

bool
AnimationEditor::on_key_press_event (GdkEventKey* event)
{
	keys .press (event);

	switch (event -> keyval)
	{
		case GDK_KEY_a:
		{
			if (event -> state == GDK_CONTROL_MASK)
			{
				selectedRange .first  = 0;
				selectedRange .second = getDuration ();
				on_select_range ();
			}

			return true;
		}
		case GDK_KEY_A:
		{
			if (event -> state == (GDK_CONTROL_MASK | GDK_SHIFT_MASK))
				on_clear_selection ();

			return true;
		}
		case GDK_KEY_z:
		{
			if (event -> state == GDK_CONTROL_MASK)
				getBrowserWindow () -> on_undo_activated ();

			getDrawingArea () .grab_focus ();
			return true;
		}
		case GDK_KEY_Z:
		{
			if (event -> state == (GDK_CONTROL_MASK | GDK_SHIFT_MASK))
				getBrowserWindow () -> on_redo_activated ();

			getDrawingArea () .grab_focus ();
			return true;
		}
		case GDK_KEY_Delete:
		{
			removeKeyframes ();
			return true;
		}
		default:
			break;
	}

	return false;
}

bool
AnimationEditor::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);
	return false;
}

bool
AnimationEditor::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;

	getDrawingArea () .grab_focus ();

	if (button == 1)
	{
		fromPoint = X3D::Vector2d (event -> x, event -> y);

		if (pick (X3D::Vector2d (event -> x, event -> y)))
		{
			if (not keys .shift ())
			{
				if (not isSelected ())
				{
					selectedFrames = activeFrames;
					on_selection_changed ();
				}

				selectedBounds = getSelectedBounds ();
			}
			
			activeSelection = true;
		}

		else
		{
			getDrawingArea () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::XTERM));
			
			const auto frame = std::round ((event -> x - getTranslation ()) / getScale ());
		
			if (keys .shift ())
			{
				on_expand_selected_range (frame);
				on_select_range ();
			}
			else
			{
				selectedFrames .clear ();
				selectedRange .first  = frame;
				selectedRange .second = frame;
				on_selection_changed ();
			}

			activeSelection = false;

			if (not isActive ())	
				getFrameAdjustment () -> set_value (frame);
		}
	}

	else if (button == 2)
	{
		getDrawingArea () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), "move"));

		fromPoint = X3D::Vector2d (event -> x, event -> y);
	}

	return false;
}

bool
AnimationEditor::on_button_release_event (GdkEventButton* event)
{
	getDrawingArea () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), "default"));
	getDrawingArea () .queue_draw ();

	if (button == 1)
	{
		if (not movedFrames .empty ())
		{
			moveKeyframes ();
		}

		else if (not activeFrames .empty ())
		{
			// Select or erase active frames.
		
			if (keys .shift ())
			{
				if (pick (X3D::Vector2d (event -> x, event -> y)))
				{
					if (isSelected ())
					{
						for (const auto & activeFrame : activeFrames)
							selectedFrames .erase (activeFrame);
					}
					else
						selectedFrames .insert (activeFrames .begin (), activeFrames .end ());
						
					on_selection_changed ();
				}
			}
		}
	}

	button = 0;
	return false;
}

bool
AnimationEditor::on_motion_notify_event (GdkEventMotion* event)
{
	pick (X3D::Vector2d (event -> x, event -> y));

	if (button == 1)
	{
		const auto toPoint = X3D::Vector2d (event -> x, event -> y);
		const auto length  = math::abs (toPoint - fromPoint);
	
		if (length < FRAME_SIZE)
			return false;

		fromPoint .y (infinity); // Drag has started.

		if (activeSelection)
		{
			if (keys .shift ())
				return false;

			// Drag selected frames.

			const int32_t fromFrame = std::round ((fromPoint .x () - getTranslation ()) / getScale ());
			const int32_t toFrame   = std::round ((event -> x - getTranslation ()) / getScale ());
			const int32_t distance  = math::clamp (toFrame - fromFrame, -selectedBounds .first, getDuration () - selectedBounds .second);

			// Perform drag.

			movedFrames .assign (selectedFrames .begin (), selectedFrames .end ());

			for (auto & movedFrame : movedFrames)
				std::get <0> (movedFrame) += distance;
		}
		else
		{
			const auto frame = std::round ((event -> x - getTranslation ()) / getScale ());
		
			if (keys .shift ())
				on_expand_selected_range (frame);

			else
				selectedRange .second = frame;

			on_select_range ();
	
			if (not isActive ())
				getFrameAdjustment () -> set_value (frame);
		}
	}

	else if (button == 2)
	{
		const auto toPoint = X3D::Vector2d (event -> x, event -> y);
		const auto offset  = toPoint - fromPoint;

		setTranslation (getTranslation () + offset .x ());
		fromPoint = toPoint;
	}

	return false;
}

bool
AnimationEditor::on_scroll_event (GdkEventScroll* event)
{
	on_zoom (event -> x, event -> direction);
	return false;
}

void
AnimationEditor::on_selection_changed ()
{
	set_key_type ();
	getDrawingArea () .queue_draw ();
}

void
AnimationEditor::on_clear_selection ()
{
	activeSelection = false;
	activeFrames   .clear ();
	selectedFrames .clear ();
	movedFrames    .clear ();
	selectedBounds = std::make_pair (0, 0);
	selectedRange  = std::make_pair (0, 0);
	on_selection_changed ();
}

void
AnimationEditor::on_expand_selected_range (const int32_t frame)
{
	const auto medium = (selectedRange .first + selectedRange .second) / 2;

	if (selectedRange .first < selectedRange .second)
	{
		if (frame <= medium)
			selectedRange .first = frame;

		else
			selectedRange .second = frame;
	}
	else
	{
		if (frame >= medium)
			selectedRange .first = frame;

		else
			selectedRange .second = frame;			
	}
}

void
AnimationEditor::on_select_range ()
{
	if (selectedRange .first == selectedRange .second)
		return;

	int32_t firstFrame = selectedRange .first;
	int32_t lastFrame  = selectedRange .second;
	
	if (firstFrame > lastFrame)
		std::swap (firstFrame, lastFrame);

	selectedFrames .clear ();

	for (const auto & frame : frames)
	{
		const auto & f = frame .first;
	
		std::get <0> (f);

		if (std::get <0> (f) >= firstFrame and std::get <0> (f) <= lastFrame)
			selectedFrames .emplace (f);
	}

	on_selection_changed ();
}

bool
AnimationEditor::isSelected () const
{
	for (const auto & activeFrame : activeFrames)
	{
		if (not selectedFrames .count (activeFrame))
			return false;
	}

	return true;
}

std::pair <int32_t, int32_t>
AnimationEditor::getSelectedBounds () const
{
	int32_t min = getDuration ();
	int32_t max = 0;

	for (auto & selectedFrame : selectedFrames)
	{
		min = std::min (min, std::get <0> (selectedFrame));
		max = std::max (max, std::get <0> (selectedFrame));
	}

	if (min > max)
		return std::make_pair (0, 0);

	return std::make_pair (min, max);
}

bool
AnimationEditor::pick (const X3D::Vector2d & point)
{
	frames .clear ();

	getTreeModelFilter () -> foreach (sigc::mem_fun (*this, &AnimationEditor::buildFrames));

	// Find all keyframes that intersect with the pointer.

	std::set <FrameKey> intersections;

	for (const auto & frame : frames)
	{
		if (frame .second .intersects (point))
			intersections .emplace (frame .first);
	}

	activeFrames = intersections;
	
	// Add all keyframes to activeFrames that are a descendant of the intersection keyframe.

	for (const auto & i : intersections)
	{
		for (const auto & frame : frames)
		{
			const auto & f = frame .first;
		
			if (std::tie (std::get <0> (i), std::get <1> (i)) == std::tie (std::get <0> (f), std::get <1> (f)))
			{
				if (std::get <2> (i) .is_ancestor (std::get <2> (f)))
					activeFrames .emplace (f);
			}
		}
	}

	getDrawingArea () .queue_draw ();

	return not activeFrames .empty ();
}

bool
AnimationEditor::buildFrames (const Gtk::TreePath & path, const Gtk::TreeIter & iter)
{
	try
	{
		Gdk::Rectangle rectangle;
		getTreeView () .get_cell_area (path, *getNameColumn () .operator -> (), rectangle);

		const auto yPad = getNameCellRenderer () -> property_ypad ();
		const int32_t y = rectangle .get_y () + rectangle .get_height () * 0.5 - yPad;

		switch (path .size ())
		{
			case 1:
			{
				for (const auto & parent : iter -> children ())
				{
					for (const auto & child : parent -> children ())
						addKeyframes (path, child, y);
				}

				break;
			}
			case 2:
			{
				for (const auto & child : iter -> children ())
					addKeyframes (path, child, y);

				break;
			}
			case 3:
			{
				addKeyframes (path, iter, y);
				break;
			}
			default:
				break;
		}
	}
	catch (const std::out_of_range &)
	{ }
	
	return false;
}

void
AnimationEditor::addKeyframes (const Gtk::TreePath & path,
                               const Gtk::TreeIter & child,
                               const double y)
{
	try
	{
		const auto   parent       = child -> parent ();
		const auto   id           = (*parent) [columns .id];
		const auto   i            = (*child) [columns .id];
		const auto & node         = nodes .at (id);
		const auto   field        = node -> getFieldDefinitions () .at (i);
		const auto & interpolator = interpolatorIndex .at (field);
		const auto & key          = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key", false);
		const auto   y1           = y - (FRAME_SIZE / 2 - 1);
		const auto   y2           = y1 + FRAME_SIZE;

		for (const auto & frame : key)
		{
			const int32_t x    = frame * getScale () + getTranslation ();
			const double  x1   = x - (FRAME_SIZE / 2);
			const double  x2   = x1 + FRAME_SIZE;
			const auto    key  = std::make_tuple (frame, field, path);
			const auto    bbox = X3D::Box2d (X3D::Vector2d (x1, y1), X3D::Vector2d (x2, y2), math::extents_type ());

			frames .emplace_back (key, bbox);
		}
	}
	catch (...)
	{ }
}

void
AnimationEditor::queue_draw ()
{
	getDrawingArea () .queue_draw ();
}

bool
AnimationEditor::on_tree_view_draw (const Cairo::RefPtr <Cairo::Context> &)
{
	getDrawingArea () .queue_draw ();
	return false;
}

bool
AnimationEditor::on_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	if (not animation)
		return false;

	const int  width      = getDrawingArea () .get_width ();
	const int  height     = getDrawingArea () .get_height ();
	const auto fc         = getTreeView () .get_style_context () -> get_color (Gtk::STATE_FLAG_NORMAL);
	const auto sc         = getTreeView () .get_style_context () -> get_color (Gtk::STATE_FLAG_SELECTED);
	const auto sb         = getTreeView () .get_style_context () -> get_background_color (Gtk::STATE_FLAG_SELECTED);
	const auto yPad       = getNameCellRenderer () -> property_ypad ();
	const auto firstFrame = std::max <int32_t> (0, std::floor (-getTranslation () / getScale ()));
	const auto lastFrame  = std::min <int32_t> (getDuration (), std::ceil ((width - getTranslation ()) / getScale ())) + 1;

	// Draw selection range.

	if (selectedRange .first not_eq selectedRange .second)
	{
		const auto    minFrame = math::clamp (selectedRange .first,  firstFrame, lastFrame - 1);
		const auto    maxFrame = math::clamp (selectedRange .second, firstFrame, lastFrame - 1);
		const int32_t x0       = minFrame * getScale () + getTranslation ();
		const int32_t x1       = maxFrame * getScale () + getTranslation ();

		context -> set_source_rgba (sb .get_red (), sb .get_green (), sb .get_blue (), sb .get_alpha () * (2 - M_PHI));
		context -> rectangle (std::min (x0, x1) - 1, 0, std::abs (x1 - x0) + 3, height);
		context -> fill ();
	}

	// Draw all time lines.

	Gtk::TreePath firstPath, lastPath;
	getTreeView () .get_visible_range (firstPath, lastPath);
	
	while (not firstPath .empty () and firstPath <= lastPath)
	{
		const auto iter = getTreeModelFilter () -> get_iter (firstPath);

		if (iter)
		{
			// Draw one time line.

			Gdk::Rectangle rectangle;
			getTreeView () .get_cell_area (firstPath, *getNameColumn () .operator -> (), rectangle);

			// Draw horizontal line.

			const int32_t x0 = firstFrame * getScale () + getTranslation ();
			const int32_t x1 = lastFrame  * getScale () + getTranslation () - getScale () + 1;
			const int32_t y1 = rectangle .get_y () + rectangle .get_height () - yPad;

			context -> move_to (x0, y1 - 0.5);
			context -> line_to (x1, y1 - 0.5);

			// Draw vertical lines.

			const auto frameParams = getFrameParams ();
			const auto frameStep   = frameParams .first;
			const auto frameFactor = frameParams .second;

			for (auto frame = firstFrame - (firstFrame % frameStep); frame < lastFrame; frame += frameStep)
			{
				const bool    s = frame % frameFactor;
				const int32_t x = frame * getScale () + getTranslation ();

				context -> move_to (x + 0.5, rectangle .get_y () + rectangle .get_height () * (s ? 0.75 : 0.5) - yPad);
				context -> line_to (x + 0.5, y1);
			}

			const auto & c = firstPath .size () > 1 ? fc : sb;

			context -> set_source_rgba (c .get_red (), c .get_green (), c .get_blue (), c .get_alpha ());
			context -> set_line_width (firstPath .size () > 2 ? 1 : 3);
			context -> stroke ();

			// Draw keyframes.

			try
			{
				const int32_t y = rectangle .get_y () + rectangle .get_height () * 0.5 - yPad;

				switch (firstPath .size ())
				{
					case 1:
					{
						for (const auto & parent : iter -> children ())
						{
							for (const auto & child : parent -> children ())
								on_draw_keyframes (context, firstPath, child, firstFrame, lastFrame, y, fc, sb);
						}

						break;
					}
					case 2:
					{
						for (const auto & child : iter -> children ())
							on_draw_keyframes (context, firstPath, child, firstFrame, lastFrame, y, fc, sb);

						break;
					}
					case 3:
					{
						on_draw_keyframes (context, firstPath, iter, firstFrame, lastFrame, y, sb, fc);
						break;
					}
					default:
						break;
				}
			}
			catch (const std::out_of_range &)
			{ }

			// Step to next path.

			if (getTreeView () .row_expanded (firstPath))
				firstPath .down ();
			else
				firstPath .next ();
		}
		else
		{
			firstPath .up ();

			if (not firstPath .empty ())
				firstPath .next ();
		}
	}

	// Draw frame cursor.

	const auto    frame = getFrameAdjustment () -> get_value ();
	const int32_t x     = (isActive () ? frame : std::round (frame)) * getScale () + getTranslation ();

	context -> move_to (x + 0.5, 0);
	context -> line_to (x + 0.5, height);

	context -> set_source_rgba (sb .get_red (), sb .get_green (), sb .get_blue (), sb .get_alpha ());
	context -> set_line_width (3);
	context -> stroke ();
	
	on_draw_moved_keyframes (context, sb);

	return false;
}

void
AnimationEditor::on_draw_keyframes (const Cairo::RefPtr <Cairo::Context> & context,
                                    const Gtk::TreePath & path,
                                    const Gtk::TreeIter & child,
                                    const int32_t firstFrame,
                                    const int32_t lastFrame,
                                    const double y,
                                    const Gdk::RGBA & fc,
                                    const Gdk::RGBA & sc)
{
	try
	{
		const auto   parent       = child -> parent ();
		const auto   id           = (*parent) [columns .id];
		const auto   i            = (*child) [columns .id];
		const auto & node         = nodes .at (id);
		const auto   field        = node -> getFieldDefinitions () .at (i);
		const auto & interpolator = interpolatorIndex .at (field);
		const auto & key          = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key", false);
		const auto   first        = std::lower_bound (key .begin (), key .end (), firstFrame);
		const auto   last         = std::upper_bound (key .begin (), key .end (), lastFrame);
		const auto   y1           = y - (FRAME_SIZE / 2 - 1);

		for (const auto & frame : std::make_pair (first, last))
		{
			const int32_t x        = frame * getScale () + getTranslation ();
			const double  x1       = x - (FRAME_SIZE / 2);
			const auto    key      = std::make_tuple (frame, field, path);
			const auto    selected = selectedFrames .count (key);

			if (not movedFrames .empty () and selected)
				continue;

			if (activeFrames .count (key) or selected)
				context -> set_source_rgba (sc .get_red (), sc .get_green (), sc .get_blue (), sc .get_alpha ());
			else
				context -> set_source_rgba (fc .get_red (), fc .get_green (), fc .get_blue (), fc .get_alpha ());

			context -> rectangle (x1, y1, FRAME_SIZE, FRAME_SIZE);
			context -> fill ();
		}
	}
	catch (...)
	{ }
}

void
AnimationEditor::on_draw_moved_keyframes (const Cairo::RefPtr <Cairo::Context> & context, const Gdk::RGBA & sb)
{
	const auto yPad = getNameCellRenderer () -> property_ypad ();

	context -> set_source_rgba (sb .get_red (), sb .get_green (), sb .get_blue (), sb .get_alpha ());

	for (const auto & frame : movedFrames)
	{
		Gdk::Rectangle rectangle;
		getTreeView () .get_cell_area (std::get <2> (frame), *getNameColumn () .operator -> (), rectangle);

		const int32_t x  = std::get <0> (frame) * getScale () + getTranslation ();
		const int32_t y  = rectangle .get_y () + rectangle .get_height () * 0.5 - yPad;
		const double  x1 = x - (FRAME_SIZE / 2);
		const double  y1 = y - (FRAME_SIZE / 2 - 1);

		context -> rectangle (x1, y1, FRAME_SIZE, FRAME_SIZE);
		context -> fill ();
	}
}

std::pair <int32_t, int32_t>
AnimationEditor::getFrameParams () const
{
	static const std::map <double, std::pair <int32_t, int32_t>> params = {
		std::make_pair (5 / 1.0,        std::make_pair (10,        50)),
		std::make_pair (5 / 10.0,       std::make_pair (100,       500)),
		std::make_pair (5 / 100.0,      std::make_pair (1000,      5000)),
		std::make_pair (5 / 1000.0,     std::make_pair (10000,     50000)),
		std::make_pair (5 / 10000.0,    std::make_pair (100000,    500000)),
		std::make_pair (5 / 100000.0,   std::make_pair (1000000,   5000000)),
		std::make_pair (5 / 1000000.0,  std::make_pair (10000000,  50000000)),
		std::make_pair (5 / 10000000.0, std::make_pair (100000000, 500000000)),
	};

	const auto iter = params .upper_bound (getScale ());

	if (iter not_eq params .end ())
		return iter -> second;

	return std::make_pair (1, 5);
}

void
AnimationEditor::dispose ()
{
	X3DEditorObject::dispose ();
	X3DAnimationEditorInterface::dispose ();
}

AnimationEditor::~AnimationEditor ()
{
	getConfig () -> setItem ("scaleKeyframes", getScaleKeyframesButton () .get_active ());
	getConfig () -> setItem ("hPaned",         getAnimationBox () .get_position ());

	dispose ();
}

} // puck
} // titania
