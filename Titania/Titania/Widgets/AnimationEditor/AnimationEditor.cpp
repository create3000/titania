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

namespace titania {
namespace puck {

static constexpr int NAME_COLUMN    = 0;
static constexpr int ENABLED_COLUMN = 1;
static constexpr int ID_COLUMN      = 2;
static constexpr int VISIBLE_COLUMN = 3;

AnimationEditor::AnimationEditor (X3DBrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DAnimationEditorInterface (get_ui ("AnimationEditor.xml"), gconf_dir ()),
	            X3DEditorObject (),
	                   nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                  animation (),
	                 timeSensor (),
	                      nodes ()
{
	getTreeModelFilter () -> set_visible_column (VISIBLE_COLUMN);

	setup ();
}

void
AnimationEditor::initialize ()
{
	X3DAnimationEditorInterface::initialize ();
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
	const auto groups = getSelection <X3D::X3DGroupingNode> ({ X3D::X3DConstants::X3DGroupingNode });
	
	const bool haveSelection = not groups .empty ();

	getNewButton () .set_sensitive (haveSelection);
}

void
AnimationEditor::on_new ()
{
	getNewNameEntry () .set_text ("");

	const auto responseId = getNewDialog () .run ();

	getNewDialog () .hide ();

	if (responseId not_eq Gtk::RESPONSE_OK)
		return;

	const auto name   = getExecutionContext () -> getUniqueName (getNewNameEntry () .get_text ());
	const auto groups = getSelection <X3D::X3DGroupingNode> ({ X3D::X3DConstants::X3DGroupingNode });
	const auto group  = groups .back ();

	animation  = getExecutionContext () -> createNode <X3D::Group> ();
	timeSensor = getExecutionContext () -> createNode <X3D::TimeSensor> ();

	group     -> children () .emplace_front (animation);
	animation -> children () .emplace_back (timeSensor);

	getExecutionContext () -> updateNamedNode (name, X3D::SFNode (animation));
	animation -> setMetaData <int32_t> ("/Animation/duration",        10);
	animation -> setMetaData <int32_t> ("/Animation/framesPerSecond", 10);

	nodeName .setNode  (X3D::SFNode (animation));

	getExecutionContext () -> realize ();

	set_animation (animation);
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
}

void
AnimationEditor::set_animation (const X3D::X3DPtr <X3D::Group> & animation)
{
	// Remove animation.

	for (const auto & pair : nodes)
	{
		pair .second -> name_changed ()   .removeInterest (this, &AnimationEditor::set_name);
		pair .second -> fields_changed () .removeInterest (this, &AnimationEditor::set_fields);
		pair .second -> isLive ()         .removeInterest (this, &AnimationEditor::set_live);

		for (const auto & field : pair .second -> getFieldDefinitions ())
			field -> removeInterest (this, &AnimationEditor::set_field);
	}

	// Reset all.

	getAddObjectButton () .set_sensitive (animation);

	getTreeStore () -> clear ();
	nodes .clear ();

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
	parent -> set_value (ENABLED_COLUMN, true);
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
		child -> set_value (ENABLED_COLUMN, true);
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

AnimationEditor::~AnimationEditor ()
{
	dispose ();
}

} // puck
} // titania
