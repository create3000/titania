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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "../../Bits/String.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"
#include "../../Editors/NodeIndex/NodeIndex.h"

#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Time/TimeSensor.h>
#include <Titania/X3D/Components/EventUtilities/BooleanSequencer.h>
#include <Titania/X3D/Components/EventUtilities/IntegerSequencer.h>
#include <Titania/X3D/Components/Interpolation/ScalarInterpolator.h>
#include <Titania/X3D/Components/Interpolation/PositionInterpolator2D.h>
#include <Titania/X3D/Components/Interpolation/PositionInterpolator.h>
#include <Titania/X3D/Components/Interpolation/CoordinateInterpolator2D.h>
#include <Titania/X3D/Components/Interpolation/CoordinateInterpolator.h>
#include <Titania/X3D/Parser/Filter.h>

#include <Titania/Math/Algorithms/CatmullRomSplineInterpolator.h>
#include <Titania/Math/Algorithms/SquadInterpolator.h>

namespace titania {
namespace puck {

using math::phi;

static constexpr int32_t FRAME_SIZE          = 7;            // in pixel
static constexpr double  DEFAULT_TRANSLATION = 8;            // in pixel
static constexpr double  DEFAULT_SCALE       = 16;           // in pixel
static constexpr double  SCROLL_FACTOR       = 1 + 1 / 16.0; // something nice

static constexpr double infinity = std::numeric_limits <double>::infinity ();

const std::set <X3D::X3DConstants::NodeType> AnimationEditor::arrayInterpolators = {
	X3D::X3DConstants::CoordinateInterpolator2D,
	X3D::X3DConstants::CoordinateInterpolator,
};

const std::map <X3D::X3DConstants::NodeType, size_t> AnimationEditor::interpolatorComponents = {
	std::pair (X3D::X3DConstants::BooleanSequencer,         1),
	std::pair (X3D::X3DConstants::IntegerSequencer,         1),
	std::pair (X3D::X3DConstants::ColorInterpolator,        3),
	std::pair (X3D::X3DConstants::ScalarInterpolator,       1),
	std::pair (X3D::X3DConstants::OrientationInterpolator,  4),
	std::pair (X3D::X3DConstants::PositionInterpolator2D,   2),
	std::pair (X3D::X3DConstants::PositionInterpolator,     3),
	std::pair (X3D::X3DConstants::CoordinateInterpolator2D, 2),
	std::pair (X3D::X3DConstants::CoordinateInterpolator,   3)
};

AnimationEditor::AnimationEditor (X3DBrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DAnimationEditorInterface (get_ui ("Widgets/AnimationEditor.glade")),
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
	            fromTranslation (0),
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
	addChildObjects (animation, timeSensor);

	setTitleBar (getPropertiesDialog (), getPropertiesHeaderBar ());

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
AnimationEditor::setup ()
{
	X3DAnimationEditorInterface::setup ();
	X3DEditorObject::setup ();
}

void
AnimationEditor::initialize ()
{
	X3DAnimationEditorInterface::initialize ();
	X3DEditorObject::initialize ();

	getCurrentBrowser () -> getExecutionContext () .addInterest (&AnimationEditor::set_animation, this, nullptr);

	nodeIndex -> getNode () .addInterest (&AnimationEditor::set_animation, this);
	nodeIndex -> reparent (getNodeIndexBox (), getWindow ());
	nodeIndex -> setShowWidget (true);
	nodeIndex -> setAnimations ();
	nodeIndex -> setName (getName () + "." + nodeIndex -> getName ());

	set_animation (nullptr);
}

void
AnimationEditor::configure ()
{
	X3DAnimationEditorInterface::configure ();

	getScaleKeyframesButton () .set_active (getConfig () -> getItem <bool> ("scaleKeyframes"));

	if (getConfig () -> hasItem ("hPaned"))
		getAnimationBox () .set_position (getConfig () -> getItem <int32_t> ("hPaned"));
}

void
AnimationEditor::addUndoStep (const X3D::UndoStepPtr & undoStep)
{
	// Proto support

	X3D::X3DEditor::requestUpdateInstances (animation, undoStep);

	// Add undo step

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
AnimationEditor::setScale (const double value)
{
	scale = value;

	setLowerTranslation ();
	getDrawingArea () .queue_draw ();
}

void
AnimationEditor::set_selection (const X3D::MFNode & selection)
{
	X3DAnimationEditorInterface::set_selection (selection);

	const bool haveSelection = not selection .empty ();
	const auto groups        = getNodes <X3D::X3DNode> (selection, { X3D::X3DConstants::X3DGroupingNode, X3D::X3DConstants::ViewpointGroup });

	getNewButton ()       .set_sensitive (not groups .empty ());
	getAddMemberButton () .set_sensitive (animation and haveSelection);
}

void
AnimationEditor::setDuration (const int32_t value)
{
	double scale = 1;

	if (animation)
	{
		scale = double (value) / double (getDuration ());

		animation -> setMetaData ("/Animation/duration", value);
	}

	getFrameAdjustment () -> set_upper (getDuration ());
	getFrameAdjustment () -> set_value (std::min <double> (std::round (getFrameAdjustment () -> get_value () * scale), getDuration ()));
	setTranslation (getTranslation ());
	setLowerTranslation ();

	// Scale seclection.

	selectedBounds .first  = std::round (selectedBounds .first  * scale);
	selectedBounds .second = std::round (selectedBounds .second * scale);
	selectedRange  .first  = std::round (selectedRange  .first  * scale);
	selectedRange  .second = std::round (selectedRange  .second * scale);

	std::set <FrameKey> scaledActiveFrames;

	for (const auto & frame : activeFrames)
		scaledActiveFrames .emplace (std::round (std::get <0> (frame) * scale), std::get <1> (frame), std::get <2> (frame));

	activeFrames = scaledActiveFrames;

	std::set <FrameKey> scaledSelectedFrames;

	for (const auto & frame : selectedFrames)
		scaledSelectedFrames .emplace (std::round (std::get <0> (frame) * scale), std::get <1> (frame), std::get <2> (frame));

	selectedFrames = scaledSelectedFrames;

	for (auto & frame : movedFrames)
		std::get <0> (frame) = std::round (std::get <0> (frame) * scale);

	for (auto & frame : copiedFrames)
		std::get <0> (frame) = std::round (std::get <0> (frame) * scale);
}

int32_t
AnimationEditor::getDuration () const
{
	if (animation)
		return std::max <int32_t> (animation -> getMetaData <int32_t> ("/Animation/duration", 10), 1);

	return 10;
}

void
AnimationEditor::setFrameRate (const int32_t value)
{
	if (animation)
		animation -> setMetaData ("/Animation/frameRate", value);

	setTranslation (getTranslation ());
}

int32_t
AnimationEditor::getFrameRate () const
{
	if (animation)
	{
		// Never romove this block!

		const auto framesPerSecond = animation -> getMetaData <int32_t> ("/Animation/framesPerSecond", -1);

		animation -> removeMetaData ("/Animation/framesPerSecond");

		if (framesPerSecond not_eq -1)
			const_cast <AnimationEditor*> (this) -> setFrameRate (framesPerSecond);
	}

	if (animation)
		return std::max <int32_t> (animation -> getMetaData <int32_t> ("/Animation/frameRate", 10), 1);

	return 10;
}

bool
AnimationEditor::isActive () const
{
	if (timeSensor)
		return timeSensor -> isActive () and not timeSensor -> isPaused ();
	
	return false;
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
	getFrameRateAdjustment () -> set_value (10);
	getLoopSwitch () .set_active (false);

	const auto responseId = getPropertiesDialog () .run ();

	getPropertiesDialog () .hide ();

	if (responseId not_eq Gtk::RESPONSE_OK)
		return;

	// Create new animation.

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Create New Animation"));
	const auto groups           = getSelection <X3D::X3DNode> ({ X3D::X3DConstants::X3DGroupingNode, X3D::X3DConstants::ViewpointGroup });
	const auto group            = groups .back ();
	const auto executionContext = X3D::MakePtr (group -> getExecutionContext ());
	const auto name             = executionContext -> getUniqueName (getNewNameEntry () .get_text ());
	const auto animation        = executionContext -> createNode <X3D::Group> ();
	const auto timeSensor       = executionContext -> createNode <X3D::TimeSensor> ();
	const auto timerName        = executionContext -> getUniqueName (getNewNameEntry () .get_text () + "Timer");

	group -> getField <X3D::MFNode> ("children") .emplace_front (animation);
	animation -> children () .emplace_front (timeSensor);
	executionContext -> updateNamedNode (name, animation);
	executionContext -> updateNamedNode (timerName, timeSensor);

	timeSensor -> loop ()     = getLoopSwitch () .get_active ();
	timeSensor -> stopTime () = 1;

	set_animation (animation);

	setDuration (getDurationAdjustment () -> get_value ());
	setFrameRate (getFrameRateAdjustment () -> get_value ());

	// Undo/Redo

	const auto undoRemoveNode = std::make_shared <X3D::UndoStep> ();

	X3D::X3DEditor::removeNodesFromScene (executionContext, { animation }, true, undoRemoveNode);

	undoStep -> addUndoFunction (&X3D::UndoStep::redo, undoRemoveNode);
	undoStep -> addRedoFunction (&X3D::UndoStep::undo, undoRemoveNode);
	undoRemoveNode -> undo ();

	getBrowserWindow () -> getSelection () -> undoRestoreNodes (undoStep);
	getBrowserWindow () -> getSelection () -> redoRestoreNodes (undoStep);

	addUndoStep (undoStep);
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
	getCycleIntervalLabel () .set_text (strfframes (getDurationAdjustment () -> get_value (), getFrameRateAdjustment () -> get_value ()));
}

void
AnimationEditor::set_animation (const X3D::SFNode & value)
{
	// Remove animation.
	
	if (animation)
	{
		animation -> isLive ()   .removeInterest (&AnimationEditor::set_animation_live, this);
		animation -> children () .removeInterest (&AnimationEditor::set_interpolators, this);

		X3D::X3DEditor::requestUpdateInstances (animation, std::make_shared <X3D::UndoStep> ());
	}

	if (timeSensor)
	{
		timeSensor -> isLive ()           .removeInterest (&AnimationEditor::set_animation_live, this);
		timeSensor -> isPaused ()         .removeInterest (&AnimationEditor::set_active, this);
		timeSensor -> isActive ()         .removeInterest (&AnimationEditor::set_active, this);
		timeSensor -> fraction_changed () .removeInterest (&AnimationEditor::set_fraction, this);

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
		animation -> isLive ()   .addInterest (&AnimationEditor::set_animation_live, this);
		animation -> children () .addInterest (&AnimationEditor::set_interpolators, this);

		const auto timeSensors = getNodes <X3D::TimeSensor> ({ animation }, { X3D::X3DConstants::TimeSensor });

		if (timeSensors .empty ())
		{
			const auto timerName = animation -> getExecutionContext () -> getUniqueName (X3D::GetDisplayName (animation) + "Timer");

			timeSensor = animation -> getExecutionContext () -> createNode <X3D::TimeSensor> ();

			animation -> getExecutionContext () -> updateNamedNode (timerName, timeSensor);

			animation -> children () .emplace_front (timeSensor);
		}
		else
			timeSensor = timeSensors .back ();

		timeSensor -> isLive ()           .addInterest (&AnimationEditor::set_animation_live, this);
		timeSensor -> isPaused ()         .addInterest (&AnimationEditor::set_active, this);
		timeSensor -> isActive ()         .addInterest (&AnimationEditor::set_active, this);
		timeSensor -> fraction_changed () .addInterest (&AnimationEditor::set_fraction, this);

		timeSensor -> cycleInterval () = getDuration () / (double) getFrameRate ();
		timeSensor -> isEvenLive (true);

		set_active (); // Call this before set_interpolators.
		set_interpolators ();
	}

	set_selection (getBrowserWindow () -> getSelection () -> getNodes ());

	nodeName .setNode (animation);

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
					interpolator -> addInterest (&AnimationEditor::queue_draw, this);
					interpolator -> getField ("value_changed") -> addInterest (&AnimationEditor::set_value, this);
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
	{
		interpolator -> removeInterest (&AnimationEditor::queue_draw, this);
		interpolator -> getField ("value_changed") -> removeInterest (&AnimationEditor::set_value, this);
	}

	interpolatorIndex .clear ();
	interpolators     .clear ();
}

void
AnimationEditor::on_add_member ()
{
	for (const auto & node : getBrowserWindow () -> getSelection () -> getNodes ())
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

			const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Remove Animation"));
			const auto executionContext = X3D::MakePtr (animation -> getExecutionContext ());

			X3D::X3DEditor::removeNodesFromScene (executionContext, { animation }, true, undoStep);
		
			getBrowserWindow () -> getSelection () -> removeNodes ({ animation }, undoStep);

			addUndoStep (undoStep);
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

					const auto   executionContext = X3D::MakePtr (animation -> getExecutionContext ());
					const auto & interpolator     = iter -> second;

					interpolatorsToRemove .emplace (interpolator);

					X3D::X3DEditor::deleteRoute (executionContext, interpolator, "value_changed", node, field -> getName (), undoStep);

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
					const auto executionContext = X3D::MakePtr (animation -> getExecutionContext ());
					const auto interpolators    = X3D::MFNode (interpolatorsToRemove .begin (), interpolatorsToRemove .end ());

					X3D::X3DEditor::removeNodesFromScene (executionContext, interpolators, true, undoStep);
				
					getBrowserWindow () -> getSelection () -> removeNodes (interpolators, undoStep);
				}

				undoStep -> addRedoFunction (&AnimationEditor::set_interpolators, this);
				addUndoStep (undoStep);

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
				const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Remove Interpolator"));
				const auto executionContext = X3D::MakePtr (animation -> getExecutionContext ());
				const auto parent           = selected -> parent ();
				const auto node             = nodes .at ((*parent) [columns .id]);
				const auto field            = node -> getFieldDefinitions () .at ((*selected) [columns .id]);
				const auto interpolator     = interpolatorIndex .at (field);

				// We have found the interpolators connected to this node and field.
				
				undoStep -> addUndoFunction (&AnimationEditor::set_interpolators, this);
				X3D::X3DEditor::deleteRoute (executionContext, interpolator, "value_changed", node, field -> getName (), undoStep);

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
				{
					const auto executionContext = X3D::MakePtr (animation -> getExecutionContext ());

					X3D::X3DEditor::removeNodesFromScene (executionContext, { interpolator }, true, undoStep);
				
					getBrowserWindow () -> getSelection () -> removeNodes ({ interpolator }, undoStep);
				}

				undoStep -> addRedoFunction (&AnimationEditor::set_interpolators, this);
				addUndoStep (undoStep);

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

	node -> name_changed ()   .addInterest (&AnimationEditor::set_name, this,                node -> getId (), getTreeStore () -> get_path (parent));
	node -> fields_changed () .addInterest (&AnimationEditor::set_user_defined_fields, this, node -> getId (), getTreeStore () -> get_path (parent));
	node -> isLive ()         .addInterest (&AnimationEditor::set_live, this,                node -> getId (), getTreeStore () -> get_path (parent));

	addFields (node, parent);
}

void
AnimationEditor::removeNode (const X3D::SFNode & node)
{
	node -> name_changed ()   .removeInterest (&AnimationEditor::set_name, this);
	node -> fields_changed () .removeInterest (&AnimationEditor::set_user_defined_fields, this);
	node -> isLive ()         .removeInterest (&AnimationEditor::set_animation_live, this);

	for (const auto & field : node -> getFieldDefinitions ())
		field -> removeInterest (&AnimationEditor::set_tainted, this);
}

void
AnimationEditor::addFields (const X3D::SFNode & node, Gtk::TreeIter & parent)
{
	static const std::set <X3D::FieldType> fieldTypes = {
	   X3D::X3DConstants::SFBool,
		X3D::X3DConstants::SFColor,
		X3D::X3DConstants::SFFloat,
		X3D::X3DConstants::SFInt32,
		X3D::X3DConstants::SFRotation,
		X3D::X3DConstants::SFVec2f,
		X3D::X3DConstants::SFVec3f,
		X3D::X3DConstants::MFVec2f,
		X3D::X3DConstants::MFVec3f
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

		field -> addInterest (&AnimationEditor::set_tainted, this, getTreeStore () -> get_path (child));
	}
}

std::string
AnimationEditor::getNodeName (const X3D::SFNode & node) const
{
	const auto name = X3D::RemoveTrailingNumber (node -> getName ());

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
			const auto   key          = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key");
			const auto   keyValue     = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
			const auto   keyType      = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");
			const auto   keySize      = interpolator -> getMetaData <X3D::SFInt32>  ("/Interpolator/keySize", X3D::SFInt32 (1));
			const auto   iter         = std::lower_bound (key .begin (), key .end (), std::get <0> (frame));
			const auto   index        = iter - key .begin ();
			const auto   indexN       = index * components * keySize;

			if (iter == key .end ())
				continue;

			std::vector <double> value;

			for (size_t i = 0, size = components * keySize; i < size; ++ i)
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

	addUndoStep (undoStep);
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
	const auto selectedRange = getSelectedRange ();

	if (selectedRange .first == selectedRange .second)
	{
		if (timeSensor)
			timeSensor -> range () [0] = 0;

		getFrameAdjustment () -> set_value (0);
	}
	else
	{
		if (timeSensor)
			timeSensor -> range () [0] = double (selectedRange .first) / double (getDuration ());

		getFrameAdjustment () -> set_value (selectedRange .first);
	}
}

void
AnimationEditor::on_play_pause ()
{
	// Determine range to play and frame where the animation should start.

	if (not timeSensor)
		return;

	if (not timeSensor -> isActive ())
	{
		const double duration      = getDuration ();
		const auto   selectedRange = getSelectedRange ();
		int32_t      firstFrame    = selectedRange .first;
		int32_t      lastFrame     = selectedRange .second;
		double       currentFrame  = getFrameAdjustment () -> get_value ();
	
		if (firstFrame == lastFrame)
		{
			firstFrame = 0;
			lastFrame  = getDuration ();
		}
		else
			currentFrame = std::clamp <double> (currentFrame, firstFrame, lastFrame);
	
		if (currentFrame >= lastFrame)
			currentFrame = firstFrame;
	
		timeSensor -> range () = { currentFrame / duration, firstFrame / duration, lastFrame / duration };
	}

	// Start TimeSensor.

	timeSensor -> stopTime () = X3D::SFTime::now ();

	if (not isActive ())
		timeSensor -> startTime () = X3D::SFTime::now ();

	// Clear tainted states.

	setTainted (Gtk::TreePath ("0"), false);
}

void
AnimationEditor::on_last_frame ()
{
	const auto selectedRange = getSelectedRange ();

	if (selectedRange .first == selectedRange .second)
	{
		if (timeSensor)
			timeSensor -> range () [0] = 1;

		getFrameAdjustment () -> set_value (getDuration ());
	}
	else
	{
		if (timeSensor)
			timeSensor -> range () [0] =  double (selectedRange .second) /  double (getDuration ());

		getFrameAdjustment () -> set_value (selectedRange .second);
	}
}

void
AnimationEditor::on_current_frame_changed ()
{
	// Display current time frame.
	
	const int32_t frame = std::round (getFrameAdjustment () -> get_value ());

	getTimeLabel () .set_text (strfframes (frame, getFrameRate ()));

	if (animation)
	{
		if (isActive ())
		{
			// Follow cursor if animation is active.

			const int32_t x     = frame * getScale () + getTranslation ();
			const int32_t width = getDrawingArea () .get_width ();

			if (x < 0 or x > width)
				setTranslation (width * (2 - phi <double>) - frame * getScale ());
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
	getFrameRateAdjustment () -> set_value (getFrameRate ());
	getLoopSwitch () .set_active (timeSensor -> loop ());

	const auto responseId = getPropertiesDialog () .run ();

	getPropertiesDialog () .hide ();

	if (responseId not_eq Gtk::RESPONSE_OK)
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Edit Keyframe Animation Properties"));
	const auto executionContext = X3D::MakePtr (animation -> getExecutionContext ());
	const auto name             = getNewNameEntry () .get_text ();
	const bool durationChanged  = getDuration () not_eq getDurationAdjustment () -> get_value ();

	undoStep -> addUndoFunction (&Gtk::DrawingArea::queue_draw, std::ref (getDrawingArea ()));

	if (durationChanged)
		undoStep -> addUndoFunction (&AnimationEditor::on_zoom_fit, this);
	
	if (getScaleKeyframesButton () .get_active ())
		scaleKeyframes (getDuration (), getDurationAdjustment () -> get_value (), undoStep);

	undoStep -> addObjects (timeSensor);

	X3D::X3DEditor::updateNamedNode (executionContext, name, animation, undoStep);

	// Adjust metadata

	undoStep -> addUndoFunction (&AnimationEditor::setDuration, this, getDuration ());
	undoStep -> addRedoFunction (&AnimationEditor::setDuration, this, getDurationAdjustment () -> get_value ());
	setDuration (getDurationAdjustment () -> get_value ());

	undoStep -> addUndoFunction (&AnimationEditor::setFrameRate, this, getFrameRate ());
	undoStep -> addRedoFunction (&AnimationEditor::setFrameRate, this, getFrameRateAdjustment () -> get_value ());
	setFrameRate (getFrameRateAdjustment () -> get_value ());

	// Adjust TimeSensor

	const auto cycleInterval = getDuration () / (double) getFrameRate ();
	undoStep -> addUndoFunction (&X3D::SFTime::setValue, std::ref (timeSensor -> cycleInterval ()), timeSensor -> cycleInterval ());
	undoStep -> addRedoFunction (&X3D::SFTime::setValue, std::ref (timeSensor -> cycleInterval ()), cycleInterval);
	timeSensor -> cycleInterval () = cycleInterval;

	undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (timeSensor -> loop ()), timeSensor -> loop ());
	undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (timeSensor -> loop ()), getLoopSwitch () .get_active ());
	timeSensor -> loop () = getLoopSwitch () .get_active ();

	// Build interpolators.
	setInterpolators (undoStep);

	if (durationChanged)
	{
		undoStep -> addRedoFunction (&AnimationEditor::on_zoom_fit, this);
		on_zoom_fit ();
	}

	undoStep -> addRedoFunction (&Gtk::DrawingArea::queue_draw, std::ref (getDrawingArea ()));

	addUndoStep (undoStep);

}

void
AnimationEditor::set_active ()
{
	const bool active = isActive ();

	getCutButton ()   .set_sensitive (not active);
	getCopyButton ()  .set_sensitive (not active);
	getPasteButton () .set_sensitive (not active);

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
			const auto   key          = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key");
			auto         keyType      = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");
			const auto   iter         = std::lower_bound (key .begin (), key .end (), std::get <0> (frame));
			const auto   index        = iter - key .begin ();

			if (key .at (index) not_eq std::get <0> (frame))
				continue;

			if (affectedInterpolators .emplace (interpolator) .second)
				undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFString>, interpolator, "/Interpolator/keyType", keyType);
	
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
				case X3D::X3DConstants::CoordinateInterpolator2D:
				case X3D::X3DConstants::CoordinateInterpolator:
				{
					keyType .set1Value (index, activeType);
					break;
				}
				default:
					break;
			}

			interpolator -> setMetaData ("/Interpolator/keyType", keyType);
		}
		catch (const std::exception &)
		{ }		
	}

	for (const auto & interpolator : affectedInterpolators)
	{
		try
		{
			const auto keyType = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");

			undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFString>, interpolator, "/Interpolator/keyType", keyType);

			setInterpolator (interpolator, undoStep);
		}
		catch (const std::exception &)
		{ }		
	}

	addUndoStep (undoStep);
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
			const auto   key          = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key");
			const auto   keyType      = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");
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
		case X3D::X3DConstants::MFVec2f:
		{
			const auto   interpolator = getInterpolator ("CoordinateInterpolator2D", node, field, undoStep);
			const auto   keySize      = interpolator -> getMetaData <X3D::SFInt32> ("/Interpolator/keySize");
			const auto & array        = *static_cast <const X3D::MFVec2f*> (field);

			if (array .empty ())
				return;

			if (keySize not_eq 0 and keySize not_eq (int32_t) array .size ())
			{
				showArraySizeErrorDialog ();
				return;
			}

			interpolator -> setMetaData <int32_t> ("/Interpolator/keySize", array .size ());

			for (const auto & vector : array)
			{
				value .emplace_back (vector .x ());
				value .emplace_back (vector .y ());
			}

			addKeyframe (interpolator, frame, value, activeType, undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		case X3D::X3DConstants::MFVec3f:
		{
			const auto   interpolator = getInterpolator ("CoordinateInterpolator", node, field, undoStep);
			const auto   keySize      = interpolator -> getMetaData <X3D::SFInt32> ("/Interpolator/keySize");
			const auto & array        = *static_cast <const X3D::MFVec3f*> (field);

			if (array .empty ())
				return;

			if (keySize not_eq 0 and keySize not_eq (int32_t) array .size ())
			{
				showArraySizeErrorDialog ();
				return;
			}

			interpolator -> setMetaData <int32_t> ("/Interpolator/keySize", array .size ());

			for (const auto & vector : array)
			{
				value .emplace_back (vector .x ());
				value .emplace_back (vector .y ());
				value .emplace_back (vector .z ());
			}

			addKeyframe (interpolator, frame, value, activeType, undoStep);
			setInterpolator (interpolator, undoStep);
			break;
		}
		default:
			break;
	}

	addUndoStep (undoStep);
}

void
AnimationEditor::addKeyframe (const X3D::X3DPtr <X3D::X3DNode> & interpolator,
                              const int32_t frame,
                              const std::vector <double> & value,
                              const std::string & type,
                              const X3D::UndoStepPtr & undoStep)
{
	const size_t components = interpolatorComponents .at (interpolator -> getType () .back ());

	auto       key      = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType  = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");
	auto       keySize  = interpolator -> getMetaData <X3D::SFInt32>  ("/Interpolator/keySize", X3D::SFInt32 (1));
	const auto iter     = std::lower_bound (key .begin (), key .end (), frame);
	const auto index    = iter - key .begin ();
	const auto indexN   = index * components * keySize;

	keyValue .resize (key .size () * components * keySize);
	keyType  .resize (key .size ());

	// Set meta data

	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFInt32>,  interpolator, "/Interpolator/key",      key);
	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFDouble>, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFString>, interpolator, "/Interpolator/keyType",  keyType);

	if (iter == key .end () or frame == key .get1Value (index))
	{
		key     .set1Value (index, frame);
		keyType .set1Value (index, type);

		for (size_t i = 0, size = components * keySize; i < size; ++ i)
			keyValue .set1Value (indexN + i, value [i]);
	}
	else
	{
		key      .emplace (key .begin () + index, frame);
		keyType  .emplace (keyType .begin () + index, type);
		keyValue .insert (keyValue .begin () + indexN, value .begin (), value .end ());
	}

	interpolator -> setMetaData ("/Interpolator/key",      key);
	interpolator -> setMetaData ("/Interpolator/keyValue", keyValue);
	interpolator -> setMetaData ("/Interpolator/keyType",  keyType);

	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFInt32>,  interpolator, "/Interpolator/key",      key);
	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFDouble>, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFString>, interpolator, "/Interpolator/keyType",  keyType);
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

	undoStep -> addUndoFunction (&AnimationEditor::setSelectedFrames, this, selectedFrames, animation);
	undoStep -> addRedoFunction (&AnimationEditor::setSelectedFrames, this, std::set <FrameKey> (movedFrames .begin (), movedFrames .end ()), animation);
	setSelectedFrames (std::set <FrameKey> (movedFrames .begin (), movedFrames .end ()), animation);

	movedFrames .clear ();
	on_selection_changed ();

	addUndoStep (undoStep);
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
	const auto key      = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType  = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");
	const auto keySize  = interpolator -> getMetaData <X3D::SFInt32>  ("/Interpolator/keySize", X3D::SFInt32 (1));
	const auto iter     = std::lower_bound (key .begin (), key .end (), fromFrame);
	const auto index    = iter - key .begin ();
	const auto indexN   = index * components * keySize;

	if (iter == key .end () or *iter not_eq fromFrame)
		return;

	keyValue .resize (key .size () * components * keySize);
	keyType  .resize (key .size ());

	const auto type = std::string (keyType [index] .get ());

	std::vector <double> value;
	
	for (size_t i = 0, size = components * keySize; i < size; ++ i)
		value .emplace_back (keyValue [indexN + i]);

	removeKeyframe (X3D::X3DPtr <X3D::X3DNode> (interpolator), components, fromFrame, undoStep);
	addKeyframe (interpolator, toFrame, value, type, undoStep);
}

void
AnimationEditor::removeKeyframes ()
{
	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Remove Keyframes"));
	const auto executionContext = X3D::MakePtr (animation -> getExecutionContext ());

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
		if (interpolator  -> getMetaData <X3D::MFInt32> ("/Interpolator/key") .empty ())
			interpolatorsToRemove .emplace_back (interpolator);
	}

	X3D::X3DEditor::removeNodesFromScene (executionContext, interpolatorsToRemove, true, undoStep);
				
	getBrowserWindow () -> getSelection () -> removeNodes (interpolatorsToRemove, undoStep);

	addUndoStep (undoStep);
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
	auto       key      = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType  = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");
	auto       keySize  = interpolator -> getMetaData <X3D::SFInt32>  ("/Interpolator/keySize", X3D::SFInt32 (1));
	const auto iter     = std::lower_bound (key .begin (), key .end (), frame);
	const auto index    = iter - key .begin ();
	const auto indexN   = index * components * keySize;

	keyValue .resize (key .size () * components * keySize);
	keyType  .resize (key .size ());

	if (iter == key .end () or *iter not_eq frame)
		return;

	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFInt32>,  interpolator, "/Interpolator/key",      key);
	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFDouble>, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFString>, interpolator, "/Interpolator/keyType",  keyType);
	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::SFInt32>,  interpolator, "/Interpolator/keySize",  keySize);

	key      .erase (key .begin () + index);
	keyValue .erase (keyValue .begin () + indexN, keyValue .begin () + (indexN + components * keySize));
	keyType  .erase (keyType .begin () + index);

	if (interpolator -> isType (arrayInterpolators))
	{
		if (key .empty ())
			keySize = 0;
	}

	interpolator -> setMetaData ("/Interpolator/key",      key);
	interpolator -> setMetaData ("/Interpolator/keyValue", keyValue);
	interpolator -> setMetaData ("/Interpolator/keyType",  keyType);
	interpolator -> setMetaData ("/Interpolator/keySize",  keySize);

	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFInt32>,  interpolator, "/Interpolator/key",      key);
	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFDouble>, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFString>, interpolator, "/Interpolator/keyType",  keyType);
	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::SFInt32>,  interpolator, "/Interpolator/keySize",  keySize);
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
	auto key = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key");

	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFInt32>, interpolator, "/Interpolator/key", key);

	for (X3D::MFInt32::reference frame : key)
	{
		frame = std::round (frame / (double) fromDuration * toDuration);
	}

	interpolator -> setMetaData <X3D::MFInt32> ("/Interpolator/key", key);

	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFInt32>, interpolator, "/Interpolator/key", key);
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
		case X3D::X3DConstants::CoordinateInterpolator2D:
		{
			resizeInterpolator (interpolator, 2, undoStep);
			setArrayInterpolator <X3D::CoordinateInterpolator2D, X3D::MFVec2f, X3D::Vector2d> (X3D::X3DPtr <X3D::CoordinateInterpolator2D> (interpolator), undoStep);
			break;
		}
		case X3D::X3DConstants::CoordinateInterpolator:
		{
			resizeInterpolator (interpolator, 3, undoStep);
			setArrayInterpolator <X3D::CoordinateInterpolator, X3D::MFVec3f, X3D::Vector3d> (X3D::X3DPtr <X3D::CoordinateInterpolator> (interpolator), undoStep);
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

	const auto components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto key        = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue   = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType    = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");

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
		{
			++ i;
			continue;
		}

		const auto value    = Type (keyValue [iN], keyValue [iN + 1], keyValue [iN + 2]);
		const auto fraction = key [i] / (double) duration;
		auto       iT       = i;

		if (keyType [iT] == "SPLIT" and iT + 1 < size)
			++ iT;

		if (keyType [iT] == "CONSTANT")
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
		else if (keyType [iT] == "LINEAR" or keyType [iT] == "SPLIT")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);
		}
		else if (keyType [iT] == "SPLINE")
		{
			std::vector <int32_t> keys;
			std::vector <X3D::Rotation4d> keyValuesH;
			std::vector <X3D::Vector2d> keyValuesSV;
			std::vector <X3D::Vector2d> keyVelocitysSV;
			std::vector <Type> keyValues;

			for (; i < size; ++ i, iN += components)
			{
				const auto value = Type (keyValue [iN], keyValue [iN + 1], keyValue [iN + 2]);
				const auto hsv   = value .hsv ();

				keys .emplace_back (key [i]);
				keyValuesH  .emplace_back (0, 0, 1, hsv [0]);
				keyValuesSV .emplace_back (hsv [1], hsv [2]);
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
				const auto    framesN  = k + 1 == size and i == key .size () ? frames + 1 : frames;

				for (int32_t f = 0; f < framesN; ++ f)
				{
					const auto weight = f / (double) frames;
	
					try
					{
						const auto valueH  = squad  .interpolate (k, k + 1, weight, keyValuesH);
						const auto valueSV = spline .interpolate (k, k + 1, weight, keyValuesSV);
						const bool negate  = math::dot (valueH .axis (), X3D::Vector3d (0, 0, 1)) < 0;
						const auto hue     = negate ? -valueH .angle () : valueH .angle ();
						const Type value   = math::make_hsv <double> (hue, valueSV .x (), valueSV .y ());

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

			if (i + 1 not_eq size)
			{
				i  -= 1;
				iN -= components;
			}
		}

		i  += 1;
		iN += components;
	}

	undoStep -> addRedoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addRedoFunction (&X3D::MFColor::setValue, std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());
}

void
AnimationEditor::setInterpolator (const X3D::X3DPtr <X3D::OrientationInterpolator> & interpolator, const X3D::UndoStepPtr & undoStep)
{
	using Type = X3D::Rotation4d;

	const auto components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto key        = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue   = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType    = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");

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
		{
			++ i;
			continue;
		}

		const auto value    = Type (keyValue [iN], keyValue [iN + 1], keyValue [iN + 2], keyValue [iN + 3]);
		const auto fraction = key [i] / (double) duration;
		auto       iT       = i;

		if (keyType [iT] == "SPLIT" and iT + 1 < size)
			++ iT;

		if (keyType [iT] == "CONSTANT")
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
		else if (keyType [iT] == "LINEAR" or keyType [iT] == "SPLIT")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);
		}
		else if (keyType [iT] == "SPLINE")
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
				const auto    framesN  = k + 1 == size and i == key .size () ? frames + 1 : frames;

				for (int32_t f = 0; f < framesN; ++ f)
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

			if (i + 1 not_eq size)
			{
				i  -= 1;
				iN -= components;
			}
		}

		i  += 1;
		iN += components;
	}

	undoStep -> addRedoFunction (&X3D::MFFloat::setValue,    std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addRedoFunction (&X3D::MFRotation::setValue, std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());
}

void
AnimationEditor::resizeInterpolator (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const size_t components, const X3D::UndoStepPtr & undoStep)
{
	auto       key      = interpolator -> getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue = interpolator -> getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType  = interpolator -> getMetaData <X3D::MFString> ("/Interpolator/keyType");
	auto       keySize  = interpolator -> getMetaData <X3D::SFInt32>  ("/Interpolator/keySize", X3D::SFInt32 (1));
	const auto iter     = std::upper_bound (key .begin (), key .end (), getDuration ());
	const auto size     = iter - key .begin ();
	const auto sizeN    = size * components * keySize;

	// Remove frames greater than duration.

	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFInt32>,  interpolator, "/Interpolator/key",      key);
	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFDouble>, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::MFString>, interpolator, "/Interpolator/keyType",  keyType);
	undoStep -> addUndoFunction (&X3D::X3DNode::setMetaData <X3D::SFInt32>,  interpolator, "/Interpolator/keySize",  keySize);

	key      .resize (size);
	keyValue .resize (sizeN);
	keyType  .resize (size);

	if (interpolator -> isType (arrayInterpolators))
	{
		if (key .empty ())
			keySize = 0;
	}

	interpolator -> setMetaData <X3D::MFInt32>  ("/Interpolator/key",      key);
	interpolator -> setMetaData <X3D::MFDouble> ("/Interpolator/keyValue", keyValue);
	interpolator -> setMetaData <X3D::MFString> ("/Interpolator/keyType",  keyType);
	interpolator -> setMetaData <X3D::SFInt32>  ("/Interpolator/keySize",  keySize);

	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFInt32>,  interpolator, "/Interpolator/key",      key);
	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFDouble>, interpolator, "/Interpolator/keyValue", keyValue);
	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::MFString>, interpolator, "/Interpolator/keyType",  keyType);
	undoStep -> addRedoFunction (&X3D::X3DNode::setMetaData <X3D::SFInt32>,  interpolator, "/Interpolator/keySize",  keySize);
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
		const auto executionContext = X3D::MakePtr (animation -> getExecutionContext ());
		const auto interpolator     = executionContext -> createNode (typeName);
		const auto name             = getInterpolatorName (node, field);
		
		interpolator -> addInterest (&AnimationEditor::queue_draw, this);
		interpolator -> getField ("value_changed") -> addInterest (&AnimationEditor::set_value, this);

		const X3D::X3DPtr <X3D::X3DNode> interpolatorNode (interpolator);
		interpolatorIndex .emplace (field, interpolatorNode);
		interpolators .emplace (interpolatorNode);

		undoStep -> addObjects (animation);
		X3D::X3DEditor::pushBackIntoArray (animation, animation -> children (), interpolator, undoStep);
		X3D::X3DEditor::addRoute (executionContext, timeSensor, "fraction_changed", interpolator, "set_fraction", undoStep);
		X3D::X3DEditor::addRoute (executionContext, interpolator, "value_changed", node, field -> getName (), undoStep);
		X3D::X3DEditor::updateNamedNode (executionContext, name, interpolator, undoStep);

		return interpolatorNode;
	}
}

std::string
AnimationEditor::getInterpolatorName (const X3D::SFNode & node, const X3D::X3DFieldDefinition* const field)
{
	Glib::ustring name = field -> getName ();
	name .replace (0, 1, Glib::ustring (1, name [0]) .uppercase ());

	return X3D::GetDisplayName (node) + name + "Interpolator";
}

void
AnimationEditor::showArraySizeErrorDialog () const
{
	// Show error dialog.

	const auto dialog = createDialog <MessageDialog> ("MessageDialog");

	dialog -> setType (Gtk::MESSAGE_ERROR);
	dialog -> setMessage (_ ("Key size has changed!"));
	dialog -> setText (_ ("The number of values of the key frames must remain equal for the whole animation."));
	dialog -> run ();
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
	getBrowserWindow () -> setAccelerators (false);
	getBrowserWindow () -> getWindow () .add_accel_group (getAccelGroup ());
	getDrawingArea () .queue_draw ();
	return false;
}

bool
AnimationEditor::on_focus_out_event (GdkEventFocus*)
{
	keys .clear ();
	getBrowserWindow () -> getWindow () .remove_accel_group (getAccelGroup ());
	getBrowserWindow () -> setAccelerators (true);
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
	const auto pointer = X3D::Vector2d (event -> x, event -> y);

	button = event -> button;

	getDrawingArea () .grab_focus ();

	if (button == 1)
	{
		fromPoint       = pointer;
		fromTranslation = getTranslation ();

		if (pick (pointer))
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
			const auto frame = std::round ((event -> x - getTranslation ()) / getScale ());

			getDrawingArea () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), "default"));

			if (timeSensor)
			{
				if (timeSensor -> isActive ())
					timeSensor -> pauseTime () = X3D::SFTime::now ();
			}

			getFrameAdjustment () -> set_value (frame);
		
			if (keys .shift ())
			{
				on_expand_selected_range (frame);
				on_select_range ();
			}
			else
			{
				// on_clear_selected_range (frame);
				selectedFrames .clear ();
				selectedRange .first  = frame;
				selectedRange .second = frame;
				on_selection_changed ();
			}

			activeSelection = false;
		}
	}
	else if (button == 2)
	{
		getDrawingArea () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), "move"));

		fromPoint = pointer;
	}

	return false;
}

bool
AnimationEditor::on_button_release_event (GdkEventButton* event)
{
	const auto pointer = X3D::Vector2d (event -> x, event -> y);

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
				if (pick (pointer))
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

		if (timeSensor)
		{
			if (timeSensor -> isPaused ())
				timeSensor -> resumeTime () = X3D::SFTime::now ();
		}
	}

	button = 0;
	return false;
}

bool
AnimationEditor::on_motion_notify_event (GdkEventMotion* event)
{
	const auto pointer = X3D::Vector2d (event -> x, event -> y);

	pick (pointer);

	if (button == 1)
	{
		// Autoscroll area

		const auto width = getDrawingArea () .get_width ();

		if (event -> x < 0)
			setTranslation (getTranslation () - event -> x);

		else if (event -> x > width)
			setTranslation (getTranslation () - (event -> x - width));

		// Drag

		const auto toPoint = pointer;
		const auto length  = math::abs (toPoint - fromPoint);
	
		if (length < FRAME_SIZE)
			return false;

		fromPoint .y (infinity); // Drag has started.

		if (activeSelection)
		{
			if (keys .shift ())
				return false;

			// Drag selected frames.

			const int32_t fromFrame = std::round ((fromPoint .x () - fromTranslation) / getScale ());
			const int32_t toFrame   = std::round ((event -> x - getTranslation ()) / getScale ());
			const int32_t distance  = std::clamp (toFrame - fromFrame, -selectedBounds .first, getDuration () - selectedBounds .second);

			// Perform drag.

			movedFrames .assign (selectedFrames .begin (), selectedFrames .end ());

			for (auto & movedFrame : movedFrames)
				std::get <0> (movedFrame) += distance;
		}
		else
		{
			// Keyframe selection

			const auto frame = std::round ((event -> x - getTranslation ()) / getScale ());
		
			getFrameAdjustment () -> set_value (frame);

			if (keys .shift ())
				on_expand_selected_range (frame);

			else
				selectedRange .second = frame;

			on_select_range ();
		}
	}

	else if (button == 2)
	{
		const auto toPoint = pointer;
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

	if (timeSensor)
	{
		const auto selectedRange = getSelectedRange ();

		if (selectedRange .first == selectedRange .second)
		{
			timeSensor -> range () = { double (getFrameAdjustment () -> get_value ()) / double (getDuration ()), 0, 1 };
		}
		else
		{
			timeSensor -> range () = { double (getFrameAdjustment () -> get_value ()) / double (getDuration ()),
			                           double (selectedRange .first)  / double (getDuration ()),
			                           double (selectedRange .second) / double (getDuration ()) };
		}
	}
}

void
AnimationEditor::on_clear_selection ()
{
	activeSelection = false;
	activeFrames   .clear ();
	selectedFrames .clear ();
	movedFrames    .clear ();
	selectedBounds = std::pair (0, 0);
	selectedRange  = std::pair (0, 0);
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
	const auto selectedRange = getSelectedRange ();

	if (selectedRange .first == selectedRange .second)
		return;

	selectedFrames .clear ();

	for (const auto & frame : frames)
	{
		const auto & f = frame .first;
	
		std::get <0> (f);

		if (std::get <0> (f) >= selectedRange .first and std::get <0> (f) <= selectedRange .second)
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

/// Returns the range of the first selected keyframe to last selected keyframe;
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
		return std::pair (0, 0);

	return std::pair (min, max);
}

std::pair <int32_t, int32_t>
AnimationEditor::getSelectedRange () const
{
	int32_t firstFrame = selectedRange .first;
	int32_t lastFrame  = selectedRange .second;

	if (firstFrame > lastFrame)
		std::swap (firstFrame, lastFrame);

	return std::pair (firstFrame, lastFrame);
}

void
AnimationEditor::setSelectedFrames (const std::set <FrameKey> & value, const X3D::X3DPtr <X3D::Group> & affectedAnimation)
{
	if (animation not_eq affectedAnimation)
		return;

	selectedFrames .clear ();
	selectedFrames .insert (value .begin (), value .end ());
}

bool
AnimationEditor::pick (const X3D::Vector2d & point)
{
	frames .clear ();

	getTreeModelFilter () -> foreach (sigc::mem_fun (this, &AnimationEditor::buildFrames));

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
		const auto   key          = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key");
		const auto   y1           = y - (FRAME_SIZE / 2 - 1);
		const auto   y2           = y1 + FRAME_SIZE;

		for (const auto & frame : key)
		{
			const int32_t x    = frame * getScale () + getTranslation ();
			const double  x1   = x - (FRAME_SIZE / 2);
			const double  x2   = x1 + FRAME_SIZE;
			const auto    key  = std::tuple (frame, field, path);
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

	const int  width         = getDrawingArea () .get_width ();
	const int  height        = getDrawingArea () .get_height ();
	const auto fc            = getTreeView () .get_style_context () -> get_color (Gtk::STATE_FLAG_NORMAL);
	const auto sc            = getTreeView () .get_style_context () -> get_color (Gtk::STATE_FLAG_SELECTED);
	const auto sb            = getTreeView () .get_style_context () -> get_background_color (Gtk::STATE_FLAG_SELECTED);
	const auto yPad          = getNameCellRenderer () -> property_ypad ();
	const auto firstFrame    = std::max <int32_t> (0, std::floor (-getTranslation () / getScale ()));
	const auto lastFrame     = std::min <int32_t> (getDuration (), std::ceil ((width - getTranslation ()) / getScale ())) + 1;
	const auto selectedRange = getSelectedRange ();

	// Draw selection range.

	if (selectedRange .first not_eq selectedRange .second)
	{
		const auto    minFrame = std::clamp (selectedRange .first,  firstFrame, lastFrame - 1);
		const auto    maxFrame = std::clamp (selectedRange .second, firstFrame, lastFrame - 1);
		const int32_t x0       = minFrame * getScale () + getTranslation ();
		const int32_t x1       = maxFrame * getScale () + getTranslation ();

		context -> set_source_rgba (sb .get_red (), sb .get_green (), sb .get_blue (), sb .get_alpha () * (2 - phi <double>));
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
		const auto   key          = interpolator -> getMetaData <X3D::MFInt32> ("/Interpolator/key");
		const auto   first        = std::lower_bound (key .begin (), key .end (), firstFrame);
		const auto   last         = std::upper_bound (key .begin (), key .end (), lastFrame);
		const auto   y1           = y - (FRAME_SIZE / 2 - 1);

		for (const auto & frame : std::pair (first, last))
		{
			const int32_t x        = frame * getScale () + getTranslation ();
			const double  x1       = x - (FRAME_SIZE / 2);
			const auto    key      = std::tuple (frame, field, path);
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
		std::pair (5 / 1.0,        std::pair (10,        50)),
		std::pair (5 / 10.0,       std::pair (100,       500)),
		std::pair (5 / 100.0,      std::pair (1000,      5000)),
		std::pair (5 / 1000.0,     std::pair (10000,     50000)),
		std::pair (5 / 10000.0,    std::pair (100000,    500000)),
		std::pair (5 / 100000.0,   std::pair (1000000,   5000000)),
		std::pair (5 / 1000000.0,  std::pair (10000000,  50000000)),
		std::pair (5 / 10000000.0, std::pair (100000000, 500000000)),
	};

	const auto iter = params .upper_bound (getScale ());

	if (iter not_eq params .end ())
		return iter -> second;

	return std::pair (1, 5);
}

void
AnimationEditor::store ()
{
	getConfig () -> setItem ("scaleKeyframes", getScaleKeyframesButton () .get_active ());
	getConfig () -> setItem ("hPaned",         getAnimationBox () .get_position ());

	X3DAnimationEditorInterface::store ();
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
