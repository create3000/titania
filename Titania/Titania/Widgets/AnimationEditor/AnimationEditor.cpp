/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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

namespace titania {
namespace puck {

static constexpr int NAME_COLUMN    = 0;
static constexpr int ID_COLUMN      = 1;
static constexpr int VISIBLE_COLUMN = 2;

static constexpr double DEFAULT_TRANSLATION = 8;
static constexpr double DEFAULT_SCALE       = 16;
static constexpr double KEY_FRAME_SIZE      = 7;
static constexpr double SCROLL_FACTOR       = 1 + 1 / 16.0;

AnimationEditor::AnimationEditor (X3DBrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DAnimationEditorInterface (get_ui ("AnimationEditor.xml"), gconf_dir ()),
	            X3DEditorObject (),
	                  nodeIndex (new NodeIndex (browserWindow)),
	                   nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                  animation (),
	                 timeSensor (),
	              interpolators (),
	                      nodes (),
	                  fromPoint (),
	                translation (0),
	                      scale (1),
	                     button (0)
{
	getTreeModelFilter () -> set_visible_column (VISIBLE_COLUMN);

	setup ();
}

void
AnimationEditor::initialize ()
{
	X3DAnimationEditorInterface::initialize ();

	getBrowser ()           .addInterest (this, &AnimationEditor::set_animation, nullptr);
	getExecutionContext ()  .addInterest (this, &AnimationEditor::set_animation, nullptr);
	nodeIndex -> getNode () .addInterest (this, &AnimationEditor::set_animation);
}

int32_t
AnimationEditor::getDuration () const
{
	try
	{
		if (animation)
		{
			const auto & v = animation -> getMetaData <X3D::MFInt32> ("/Animation/duration");

			return v .at (0);
		}
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
		{
			const auto & v = animation -> getMetaData <X3D::MFInt32> ("/Animation/framesPerSecond");

			return v .at (0);
		}
	}
	catch (...)
	{ }

	return 10;
}

void
AnimationEditor::on_map ()
{
	getBrowserWindow () -> getFooterLabel () .set_text (_ ("Keyframe Animation"));

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &AnimationEditor::set_selection);

	set_selection ();
}

void
AnimationEditor::on_unmap ()
{
	getBrowserWindow () -> getSelection () -> getChildren () .removeInterest (this, &AnimationEditor::set_selection);
}

void
AnimationEditor::set_selection ()
{
	const bool haveSelection = not getBrowserWindow () -> getSelection () -> getChildren () .empty ();
	const auto groups        = getSelection <X3D::X3DGroupingNode> ({ X3D::X3DConstants::X3DGroupingNode });

	getAddObjectButton () .set_sensitive (animation and haveSelection);
	getNewButton ()       .set_sensitive (not groups .empty ());
}

void
AnimationEditor::on_new ()
{
	// Open »New Animation Dialog«.

	getNewNameEntry () .set_text ("");

	const auto responseId = getNewDialog () .run ();

	getNewDialog () .hide ();

	if (responseId not_eq Gtk::RESPONSE_OK)
		return;

	// Create new animation.

	const auto undoStep  = std::make_shared <UndoStep> (_ ("Create New Animation"));
	const auto name      = getExecutionContext () -> getUniqueName (getNewNameEntry () .get_text ());
	const auto groups    = getSelection <X3D::X3DGroupingNode> ({ X3D::X3DConstants::X3DGroupingNode });
	const auto group     = groups .back ();
	const auto animation = getExecutionContext () -> createNode <X3D::Group> ();

	group -> children () .emplace_front (animation);

	getExecutionContext () -> updateNamedNode (name, X3D::SFNode (animation));
	animation -> setMetaData <int32_t> ("/Animation/duration",        10);
	animation -> setMetaData <int32_t> ("/Animation/framesPerSecond", 10);

	getExecutionContext () -> realize ();

	set_animation (X3D::SFNode (animation));

	// Undo/Redo

	const auto undoRemoveNode = std::make_shared <UndoStep> ();
	getBrowserWindow () -> removeNodesFromScene (getExecutionContext (), { animation }, undoRemoveNode);
	undoStep -> addUndoFunction (&UndoStep::redoChanges, undoRemoveNode);
	undoStep -> addRedoFunction (&UndoStep::undoChanges, undoRemoveNode);
	undoRemoveNode -> undoChanges ();

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
AnimationEditor::on_open ()
{
	nodeIndex -> getWindow () .show ();
	nodeIndex -> setAnimations ();
}

void
AnimationEditor::set_animation (const X3D::SFNode & value)
{
	// Remove animation.
	
	if (animation)
	{
		animation ->  isLive () .removeInterest (this, &AnimationEditor::set_remove_animation);
		timeSensor -> isLive () .removeInterest (this, &AnimationEditor::set_remove_animation);
	}

	for (const auto & pair : nodes)
	{
		pair .second -> name_changed ()   .removeInterest (this, &AnimationEditor::set_name);
		pair .second -> fields_changed () .removeInterest (this, &AnimationEditor::set_fields);
		pair .second -> isLive ()         .removeInterest (this, &AnimationEditor::set_live);

		for (const auto & field : pair .second -> getFieldDefinitions ())
			field -> removeInterest (this, &AnimationEditor::set_field);
	}

	// Reset all.

	animation  = value;
	timeSensor = nullptr;

	if (animation)
	{
		animation -> isLive () .addInterest (this, &AnimationEditor::set_remove_animation);

		const auto timeSensors = getNodes <X3D::TimeSensor> ({ animation }, { X3D::X3DConstants::TimeSensor });

		if (timeSensors .empty ())
		{
			timeSensor = getExecutionContext () -> createNode <X3D::TimeSensor> ();

			animation -> children () .emplace_front (timeSensor);

			getExecutionContext () -> realize ();
		}
		else
			timeSensor = timeSensors .back ();

		timeSensor -> isLive () .addInterest (this, &AnimationEditor::set_remove_animation);
	}

	set_selection ();

	getFrameSpinButton () .set_sensitive (animation);
	getAnimationBox ()    .set_sensitive (animation);

	getTreeStore () -> clear ();

	nodeName .setNode (X3D::SFNode (animation));
	
	interpolators .clear ();
	nodes         .clear ();

	translation = DEFAULT_TRANSLATION;
	scale       = DEFAULT_SCALE;

	getFrameAdjustment () -> set_upper (getDuration ());
	getFrameAdjustment () -> set_value (0);

	getDrawingArea () .queue_draw ();
}

void
AnimationEditor::set_remove_animation (const bool value)
{
	if (not value)
		set_animation (nullptr);
}

void
AnimationEditor::on_add_object ()
{
	for (const auto & node : getBrowserWindow () -> getSelection () -> getChildren ())
		addNode (node);
}

void
AnimationEditor::addNode (const X3D::SFNode & node)
{
	if (not nodes .emplace ((size_t) node -> getId (), node) .second)
		return;

	auto parent = getTreeStore () -> append ();
	parent -> set_value (ID_COLUMN,      (size_t) node -> getId ());
	parent -> set_value (NAME_COLUMN,    getNodeName (node));
	parent -> set_value (VISIBLE_COLUMN, true);

	node -> name_changed ()   .addInterest (this, &AnimationEditor::set_name,   (size_t) node -> getId (), getTreeStore () -> get_path (parent));
	node -> fields_changed () .addInterest (this, &AnimationEditor::set_fields, (size_t) node -> getId (), getTreeStore () -> get_path (parent));
	node -> isLive ()         .addInterest (this, &AnimationEditor::set_live,   (size_t) node -> getId (), getTreeStore () -> get_path (parent));

	addFields (node, parent);
}

void
AnimationEditor::addFields (const X3D::SFNode & node, Gtk::TreeIter & parent)
{
	static const std::set <X3D::X3DConstants::FieldType> fieldTypes = {
		X3D::X3DConstants::SFColor,
		X3D::X3DConstants::SFFloat,
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

		auto child = getTreeStore () -> append (parent -> children ());
		child -> set_value (ID_COLUMN,      i);
		child -> set_value (NAME_COLUMN,    Glib::Markup::escape_text (field -> getName ()));
		child -> set_value (VISIBLE_COLUMN, true);
		
		field -> addInterest (this, &AnimationEditor::set_field, field, getTreeStore () -> get_path (child));
	}
}

std::string
AnimationEditor::getNodeName (const X3D::SFNode & node) const
{
	auto name = node -> getName ();

	X3D::RegEx::LastNumber_ .Replace ("", &name);

	return "<b>" + Glib::Markup::escape_text (node -> getTypeName ()) + "</b> " + Glib::Markup::escape_text (name);
}

void
AnimationEditor::set_name (const size_t id, const Gtk::TreePath & path)
{
	try
	{
		const auto & node = nodes .at (id);

		auto iter = getTreeStore () -> get_iter (path);

		iter -> set_value (NAME_COLUMN, getNodeName (node));
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

		iter -> set_value (VISIBLE_COLUMN, node -> isLive () .getValue ());

		getTreeModelFilter () -> row_has_child_toggled (getTreeModelFilter () -> convert_child_path_to_path (path),
		                                                getTreeModelFilter () -> convert_child_iter_to_iter (iter));
	}
	catch (const std::out_of_range &)
	{ }
}

void
AnimationEditor::set_field (X3D::X3DFieldDefinition* const field, const Gtk::TreePath & path)
{
	auto iter = getTreeStore () -> get_iter (path);

	iter -> set_value (NAME_COLUMN, "<i>" + Glib::Markup::escape_text (field -> getName ()) + "</i>");
}

void
AnimationEditor::set_fields (const size_t id, const Gtk::TreePath & path)
{
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

bool
AnimationEditor::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;

	if (button == 2)
	{
		getDrawingArea () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::FLEUR));

		fromPoint = X3D::Vector2d (event -> x, event -> y);
	}

	return false;
}

bool
AnimationEditor::on_button_release_event (GdkEventButton* event)
{
	getDrawingArea () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));

	button = 0;
	return false;
}

bool
AnimationEditor::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 2)
	{
		const auto toPoint = X3D::Vector2d (event -> x, event -> y);
		const auto offset  = toPoint - fromPoint;

		translation += offset .x ();
		//translation  = std::min (translation, DEFAULT_TRANSLATION);
		fromPoint    = toPoint;
	}

	getDrawingArea () .queue_draw ();
	return false;
}


bool
AnimationEditor::on_scroll_event (GdkEventScroll* event)
{
	const auto fromFrame = (event -> x - translation) / scale;

	if (event -> direction == GDK_SCROLL_DOWN)     // Move backwards.
	{
		scale /= SCROLL_FACTOR;
	}

	else if (event -> direction == GDK_SCROLL_UP) // Move forwards.
	{
		scale *= SCROLL_FACTOR;
	}

	const auto toFrame = (event -> x - translation) / scale;
	const auto offset  = (toFrame - fromFrame) * scale;

	translation += offset;

	getDrawingArea () .queue_draw ();
	return false;
}

void
AnimationEditor::on_current_frame_changed ()
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
	const int  width  = getDrawingArea () .get_width ();
	const int  height = getDrawingArea () .get_height ();
	const auto fc     = getDrawingArea () .get_style_context () -> get_color (Gtk::STATE_FLAG_NORMAL);
	const auto sc     = getDrawingArea () .get_style_context () -> get_color (Gtk::STATE_FLAG_SELECTED);
	const auto sb     = getDrawingArea () .get_style_context () -> get_background_color (Gtk::STATE_FLAG_SELECTED);

	getDrawingArea () .get_style_context () -> render_background (context, 0, 0, width, height);

	// Draw all time lines.

	Gtk::TreePath firstPath, lastPath;
	getTreeView () .get_visible_range (firstPath, lastPath);

	context -> set_source_rgba (fc .get_red (), fc .get_green (), fc .get_blue (), fc .get_alpha ());

	while (not firstPath .empty () and firstPath <= lastPath)
	{
		const auto iter = getTreeModelFilter () -> get_iter (firstPath);

		if (iter)
		{
			// Draw one time line.

			int32_t       firstFrame = std::max <int32_t> (0, std::floor (-translation / scale));
			const int32_t lastFrame  = std::min <int32_t> (getDuration (), std::ceil ((width - translation) / scale)) + 1;

			Gdk::Rectangle rectangle;
			getTreeView () .get_cell_area (firstPath, *getNameColumn () .operator -> (), rectangle);

			// Draw horizontal line.

			const int32_t x0 = firstFrame * scale + translation;
			const int32_t x1 = lastFrame  * scale + translation - scale + 1;

			context -> move_to (x0, rectangle .get_y () + rectangle .get_height () - 0.5);
			context -> line_to (x1, rectangle .get_y () + rectangle .get_height () - 0.5);

			// Draw vertical lines.

			for (; firstFrame < lastFrame; ++ firstFrame)
			{
				const bool    s = firstFrame % 5;
				const int32_t x = firstFrame * scale + translation;

				context -> move_to (x + 0.5, rectangle .get_y () + rectangle .get_height () * (s ? 0.75 : 0.5));
				context -> line_to (x + 0.5, rectangle .get_y () + rectangle .get_height ());
			}

			context -> set_line_width (firstPath .size () > 1 ? 1 : 3);
			context -> stroke ();

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

	const int32_t x = getFrameAdjustment () -> get_value () * scale + translation;

	context -> move_to (x + 0.5, 0);
	context -> line_to (x + 0.5, height);

	context -> set_source_rgba (sb .get_red (), sb .get_green (), sb .get_blue (), sb .get_alpha ());
	context -> set_dash (std::vector <double> { 2.0, 2.0 }, 0.0);
	context -> set_line_width (1);
	context -> stroke ();

	return false;
}

AnimationEditor::~AnimationEditor ()
{
	dispose ();
}

} // puck
} // titania
