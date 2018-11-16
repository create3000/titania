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

#include "X3DUserDefinedFieldsEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Prototype/ProtoDeclaration.h>

namespace titania {
namespace puck {

X3DUserDefinedFieldsEditor::X3DUserDefinedFieldsEditor () :
	X3DNodePropertiesEditorInterface (),
	                            node (),
	                         columns (),
	      userDefinedFieldsListStore (Gtk::ListStore::create (columns)),
	                userDefinedField (nullptr),
	                         editing (false),
	                   isExternproto (false)
{
	addChildObjects (node);

	getInitializeOnlyMenuItem () .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_access_type_activate), "initializeOnly"));
	getInputOnlyMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_access_type_activate), "inputOnly"));
	getOutputOnlyMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_access_type_activate), "outputOnly"));
	getInputOutputMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_access_type_activate), "inputOutput"));

	getSFBoolMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFBool"));
	getSFColorRGBAMenuItem () .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFColorRGBA"));
	getSFColorMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFColor"));
	getSFDoubleMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFDouble"));
	getSFFloatMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFFloat"));
	getSFImageMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFImage"));
	getSFInt32MenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFInt32"));
	getSFMatrix3dMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFMatrix3d"));
	getSFMatrix3fMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFMatrix3f"));
	getSFMatrix4dMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFMatrix4d"));
	getSFMatrix4fMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFMatrix4f"));
	getSFNodeMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFNode"));
	getSFRotationMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFRotation"));
	getSFStringMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFString"));
	getSFTimeMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFTime"));
	getSFVec2dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec2d"));
	getSFVec2fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec2f"));
	getSFVec3dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec3d"));
	getSFVec3fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec3f"));
	getSFVec4dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec4d"));
	getSFVec4fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec4f"));

	getMFBoolMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFBool"));
	getMFColorRGBAMenuItem () .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFColorRGBA"));
	getMFColorMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFColor"));
	getMFDoubleMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFDouble"));
	getMFFloatMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFFloat"));
	getMFImageMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFImage"));
	getMFInt32MenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFInt32"));
	getMFMatrix3dMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFMatrix3d"));
	getMFMatrix3fMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFMatrix3f"));
	getMFMatrix4dMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFMatrix4d"));
	getMFMatrix4fMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFMatrix4f"));
	getMFNodeMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFNode"));
	getMFRotationMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFRotation"));
	getMFStringMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFString"));
	getMFTimeMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFTime"));
	getMFVec2dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec2d"));
	getMFVec2fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec2f"));
	getMFVec3dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec3d"));
	getMFVec3fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec3f"));
	getMFVec4dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec4d"));
	getMFVec4fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec4f"));

	getUserDefinedFieldsTreeView () .set_model (getUserDefinedFieldsListStore ());
	getUserDefinedFieldsTreeView () .set_reorderable (true);
}

void
X3DUserDefinedFieldsEditor::initialize ()
{
	// Initialize FieldNameCompletion:

	std::set <std::string> fieldNames;

	for (const auto & pair : getCurrentBrowser () -> getSupportedNodes ())
	{
		const auto node = pair .second;

		for (const auto field : node -> getFieldDefinitions ())
		{
			switch (field -> getAccessType ())
			{
				case X3D::initializeOnly:
				case X3D::inputOnly:
				case X3D::outputOnly:
				{
					fieldNames .emplace (field -> getName ());
					break;
				}
				case X3D::inputOutput:
				{
					fieldNames .emplace (field -> getName ());
					fieldNames .emplace ("set_" + field -> getName ());
					fieldNames .emplace (field -> getName () + "_changed");
					break;
				}
			}
		}
	}

	for (const auto & fieldName : fieldNames)
		getFieldNameListStore () -> append () -> set_value (0, fieldName);
}

void
X3DUserDefinedFieldsEditor::setNode (const X3D::SFNode & value)
{
	if (node and node -> canUserDefinedFields ())
		node -> fields_changed () .removeInterest (&X3DUserDefinedFieldsEditor::set_fields, this);

	node             = value;
	userDefinedField = nullptr;

	if (node and node -> canUserDefinedFields ())
	{
	   isExternproto = node -> getType () .back () == X3D::X3DConstants::ExternProtoDeclaration;

		getUserDefinedFieldsBox ()    .set_visible (true);
		getUserDefinedFieldsWidget () .set_sensitive (true);

		node -> fields_changed () .addInterest (&X3DUserDefinedFieldsEditor::set_fields, this);

		set_fields ();

		//  Add and Remove Buttons

		getUserDefinedFieldsActionBox () .set_sensitive (not isExternproto);

		//  Drag & Drop

		if (isExternproto)
		{
			getUserDefinedFieldsTreeView () .unset_rows_drag_source ();
			getUserDefinedFieldsTreeView () .unset_rows_drag_dest ();
		}
		else
		{
			getUserDefinedFieldsTreeView () .enable_model_drag_source ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::BUTTON1_MASK, Gdk::ACTION_COPY);
			getUserDefinedFieldsTreeView () .enable_model_drag_dest   ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::ACTION_COPY);
		}
	}
	else
	{
		getUserDefinedFieldsBox ()    .set_visible (false);
		getUserDefinedFieldsWidget () .set_sensitive (false);

		getUserDefinedFieldsListStore () -> clear ();
	}
}

bool
X3DUserDefinedFieldsEditor::on_user_defined_field_focus_in_event (GdkEventFocus* event)
{
	getBrowserWindow () -> setAccelerators (false);
	return false;
}

bool
X3DUserDefinedFieldsEditor::on_user_defined_field_focus_out_event (GdkEventFocus* event)
{
	getBrowserWindow () -> setAccelerators (true);
	return false;
}

bool
X3DUserDefinedFieldsEditor::on_user_defined_field_key_press_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_BackSpace or event -> keyval == GDK_KEY_Delete)
	{
		if (getUserDefinedFieldsTreeView () .get_selection () -> get_selected_rows () .empty ())
			return false;

		on_remove_user_defined_field_clicked ();
		return true;
	}

	return false;
}

void
X3DUserDefinedFieldsEditor::on_user_defined_field_changed ()
{
	getRemoveUserDefinedFieldButton () .set_sensitive (not getUserDefinedFieldsTreeView () .get_selection () -> get_selected_rows () .empty ());
}

void
X3DUserDefinedFieldsEditor::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                                   int x, int y,
                                                   const Gtk::SelectionData & selection_data,
                                                   guint info,
                                                   guint time)
{
	const auto   selected = getUserDefinedFieldsTreeView () .get_selection () -> get_selected ();
	const size_t index    = getUserDefinedFieldsListStore () -> get_path (selected) .front ();

	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;
	size_t                    dest = 0;

	if (getUserDefinedFieldsTreeView () .get_dest_row_at_pos (x, y, destinationPath, position))
	{
		auto destination = getUserDefinedFieldsListStore () -> get_iter (destinationPath);

		dest = getUserDefinedFieldsListStore () -> get_path (destination) .front ();

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
				getUserDefinedFieldsListStore () -> move (selected, destination);
				break;
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
				getUserDefinedFieldsListStore () -> move (selected, ++ destination);
				++ dest;
				break;
		}
	}
	else
	{
		const auto children = getUserDefinedFieldsListStore () -> children ();
		getUserDefinedFieldsListStore () -> move (selected, children .end ());
		dest = children .size ();
	}

	try
	{
		auto       userDefinedFields = node -> getUserDefinedFields ();
		const auto field             = userDefinedFields .at (index);

		if (index < dest)
		{
			userDefinedFields .emplace (userDefinedFields .begin () + dest, field);
			userDefinedFields .erase (userDefinedFields .begin () + index);
		}
		else
		{
			userDefinedFields .erase (userDefinedFields .begin () + index);
			userDefinedFields .emplace (userDefinedFields .begin () + dest, field);
		}

		// Set user defined field.

		const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Reorder User-Defined Field »%s«"), field -> getName () .c_str ()));

		node -> fields_changed () .removeInterest (&X3DUserDefinedFieldsEditor::set_fields, this);
		node -> fields_changed () .addInterest (&X3DUserDefinedFieldsEditor::connectFields, this);

		X3D::X3DEditor::setUserDefinedFields (node, userDefinedFields, undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (...)
	{ }

	context -> drag_finish (true, false, time);
}

void
X3DUserDefinedFieldsEditor::on_user_defined_field_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	try
	{
	   if (isExternproto)
	      return;

		editing          = true;
		userDefinedField = node -> getUserDefinedFields () .at (path .front ());

		getAccessTypeLabel () .set_text (X3D::to_string (userDefinedField -> getAccessType ()));
		getFieldTypeLabel ()  .set_text (userDefinedField -> getTypeName ());
		getFieldNameEntry ()  .set_text (userDefinedField -> getName ());
		on_field_name_changed ();

		getUserDefinedFieldDialog () .set_title (_ ("Edit User Defined Field"));
		getUserDefinedFieldDialog () .present ();

		getFieldNameEntry () .grab_focus ();
	}
	catch (...)
	{ }
}

void
X3DUserDefinedFieldsEditor::on_add_user_defined_field_clicked ()
{
	editing = false;

	getFieldNameEntry () .set_text ("");
	on_field_name_changed ();

	getUserDefinedFieldDialog () .set_title (_ ("Add User Defined Field"));
	getUserDefinedFieldDialog () .present ();

	getFieldNameEntry () .grab_focus ();
}

void
X3DUserDefinedFieldsEditor::on_remove_user_defined_field_clicked ()
{
	const auto selected = getUserDefinedFieldsTreeView () .get_selection () -> get_selected ();
	const auto index    = getUserDefinedFieldsListStore () -> get_path (selected) .front ();

	const auto userDefinedFields = node -> getUserDefinedFields ();
	const auto field             = userDefinedFields [index];

	// Erase from list store.

	getUserDefinedFieldsListStore () -> erase (selected);

	// Remove user defined field.

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove User-Defined Field »%s«"), field -> getName () .c_str ()));

	node -> fields_changed () .removeInterest (&X3DUserDefinedFieldsEditor::set_fields, this);
	node -> fields_changed () .addInterest (&X3DUserDefinedFieldsEditor::connectFields, this);

	X3D::X3DEditor::removeUserDefinedField (node, field, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DUserDefinedFieldsEditor::on_access_type_activate (const std::string & accessType)
{
	getAccessTypeLabel () .set_text (accessType);

	on_field_name_changed ();
}

void
X3DUserDefinedFieldsEditor::on_field_type_activate (const std::string & fieldType)
{
	getFieldTypeLabel () .set_text (fieldType);
}

void
X3DUserDefinedFieldsEditor::on_field_name_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (getFieldNameEntry (), text, *position);
}

void
X3DUserDefinedFieldsEditor::on_field_name_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (getFieldNameEntry (), start_pos, end_pos);
}

void
X3DUserDefinedFieldsEditor::on_field_name_changed ()
{
	// Set Ok button sensitive or not.

	const std::string name   = getFieldNameEntry () .get_text ();
	bool              exists = hasField (name);

	if (getAccessTypeLabel () .get_text () == "inputOutput")
	{
		exists |= hasField ("set_" + name);
		exists |= hasField (name + "_changed");
	}

	getAddUserDefinedFieldOkButton () .set_sensitive (not name .empty () and not exists);
}

bool
X3DUserDefinedFieldsEditor::hasField (const std::string & name) const
{
	try
	{
		const auto field = node -> getField (name);

		if (editing)
			return field not_eq userDefinedField;

		return true;
	}
	catch (...)
	{
		return false;
	}
}

void
X3DUserDefinedFieldsEditor::on_add_field_ok_clicked ()
{
	static const std::map <std::string, X3D::AccessType> accessTypes = {
		std::pair ("initializeOnly", X3D::initializeOnly),
		std::pair ("inputOnly",      X3D::inputOnly),
		std::pair ("outputOnly",     X3D::outputOnly),
		std::pair ("inputOutput",    X3D::inputOutput)
	};

	try
	{
		getUserDefinedFieldDialog () .hide ();

		if (editing)
		{
			if (userDefinedField)
			{
				// Edit field.

				const auto accessType = accessTypes .at (getAccessTypeLabel () .get_text ());
				const auto typeName   = getFieldTypeLabel () .get_text ();
				const auto name       = getFieldNameEntry () .get_text ();

				// Replace field in list store.

				updateUserDefinedField (accessType, typeName, name, userDefinedField);

				// Add user defined field.

				const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Update User-Defined Field »%s«"), name .c_str ()));

				node -> fields_changed () .removeInterest (&X3DUserDefinedFieldsEditor::set_fields, this);
				node -> fields_changed () .addInterest (&X3DUserDefinedFieldsEditor::connectFields, this);

				if (typeName == userDefinedField -> getTypeName ())
				{
					X3D::X3DEditor::updateUserDefinedField (node, accessType, name, userDefinedField, undoStep);
				}
				else
				{
					const auto newField = getCurrentBrowser () -> getSupportedField (typeName) -> create ();
	
					newField -> setName (name);
					newField -> setAccessType (accessType);

					X3D::X3DEditor::replaceUserDefinedField (node, userDefinedField, newField, undoStep);
				}

				getBrowserWindow () -> addUndoStep (undoStep);
			}
		}
		else
		{
			// Create field.

			X3D::X3DFieldDefinition* const field = getCurrentBrowser () -> getSupportedField (getFieldTypeLabel () .get_text ()) -> create ();

			field -> setName (getFieldNameEntry () .get_text ());
			field -> setAccessType (accessTypes .at (getAccessTypeLabel () .get_text ()));

			// Add field to list store.

			addUserDefinedField (field);

			// Add user defined field.

			const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Add User-Defined Field »%s«"), field -> getName () .c_str ()));

			node -> fields_changed () .removeInterest (&X3DUserDefinedFieldsEditor::set_fields, this);
			node -> fields_changed () .addInterest (&X3DUserDefinedFieldsEditor::connectFields, this);

			X3D::X3DEditor::addUserDefinedField (node, field, undoStep);

			getBrowserWindow () -> addUndoStep (undoStep);
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DUserDefinedFieldsEditor::on_add_field_cancel_clicked ()
{
	getUserDefinedFieldDialog () .hide ();
}

void
X3DUserDefinedFieldsEditor::addUserDefinedField (X3D::X3DFieldDefinition* const field)
{
	const auto iter = getUserDefinedFieldsListStore () -> append ();
	(*iter) [columns .type]       = Gdk::Pixbuf::create_from_file (get_ui ("icons/FieldType/" + field -> getTypeName () + ".svg"));
	(*iter) [columns .name]       = field -> getName ();
	(*iter) [columns .accessType] = Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/" + X3D::to_string (field -> getAccessType ()) + ".png"));
}

void
X3DUserDefinedFieldsEditor::updateUserDefinedField (const X3D::AccessType accessType, const std::string & typeName, const std::string & name, X3D::X3DFieldDefinition* const field)
{
	const auto userDefinedFields = node -> getUserDefinedFields ();
	const auto pos               = std::find (userDefinedFields .begin (), userDefinedFields .end (), field);

	if (pos == userDefinedFields .end ())
		return;

	const size_t index = pos - userDefinedFields .begin ();

	// Replace in list store.

	const auto iter = getUserDefinedFieldsListStore () -> children () [index];

	(*iter) [columns .type]       = Gdk::Pixbuf::create_from_file (get_ui ("icons/FieldType/" + typeName + ".svg"));
	(*iter) [columns .name]       = name;
	(*iter) [columns .accessType] = Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/" + X3D::to_string (accessType) + ".png"));
}

void
X3DUserDefinedFieldsEditor::set_fields ()
{
	getUserDefinedFieldDialog () .hide ();
	getUserDefinedFieldsListStore () -> clear ();

	for (const auto & field : node -> getUserDefinedFields ())
		addUserDefinedField (field);

	getRemoveExportedNodeButton () .set_sensitive (false);
}

void
X3DUserDefinedFieldsEditor::connectFields (const X3D::SFTime & field)
{
	field .removeInterest (&X3DUserDefinedFieldsEditor::connectFields, this);
	field .addInterest (&X3DUserDefinedFieldsEditor::set_fields, this);
}

void
X3DUserDefinedFieldsEditor::store ()
{
	if (getUserDefinedFieldsTreeView () .has_focus ())
		getCurrentBrowser () -> grab_focus ();
}

X3DUserDefinedFieldsEditor::~X3DUserDefinedFieldsEditor ()
{
}

} // puck
} // titania
