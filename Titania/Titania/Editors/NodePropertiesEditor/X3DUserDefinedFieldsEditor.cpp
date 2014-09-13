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

#include "X3DUserDefinedFieldsEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

X3DUserDefinedFieldsEditor::X3DUserDefinedFieldsEditor () :
	X3DNodePropertiesEditorInterface (),
	                            node (),
	                         columns (),
	      userDefinedFieldsListStore (Gtk::ListStore::create (columns)),
	                userDefinedField (nullptr),
	                         editing (false)
{
	getInitializeOnlyMenuItem () .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_access_type_activate), "initializeOnly"));
	getInputOnlyMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_access_type_activate), "inputOnly"));
	getOutputOnlyMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_access_type_activate), "outputOnly"));
	getInputOutputMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_access_type_activate), "inputOutput"));

	getSFBoolMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFBool"));
	getSFColorRGBAMenuItem () .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFColorRGBA"));
	getSFColorMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFColor"));
	getSFDoubleMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFDouble"));
	getSFFloatMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFFloat"));
	getSFImageMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFImage"));
	getSFInt32MenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFInt32"));
	getSFMatrix3dMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFMatrix3d"));
	getSFMatrix3fMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFMatrix3f"));
	getSFMatrix4dMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFMatrix4d"));
	getSFMatrix4fMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFMatrix4f"));
	getSFNodeMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFNode"));
	getSFRotationMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFRotation"));
	getSFStringMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFString"));
	getSFTimeMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFTime"));
	getSFVec2dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec2d"));
	getSFVec2fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec2f"));
	getSFVec3dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec3d"));
	getSFVec3fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec3f"));
	getSFVec4dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec4d"));
	getSFVec4fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "SFVec4f"));

	getMFBoolMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFBool"));
	getMFColorRGBAMenuItem () .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFColorRGBA"));
	getMFColorMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFColor"));
	getMFDoubleMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFDouble"));
	getMFFloatMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFFloat"));
	getMFImageMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFImage"));
	getMFInt32MenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFInt32"));
	getMFMatrix3dMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFMatrix3d"));
	getMFMatrix3fMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFMatrix3f"));
	getMFMatrix4dMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFMatrix4d"));
	getMFMatrix4fMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFMatrix4f"));
	getMFNodeMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFNode"));
	getMFRotationMenuItem ()  .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFRotation"));
	getMFStringMenuItem ()    .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFString"));
	getMFTimeMenuItem ()      .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFTime"));
	getMFVec2dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec2d"));
	getMFVec2fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec2f"));
	getMFVec3dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec3d"));
	getMFVec3fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec3f"));
	getMFVec4dMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec4d"));
	getMFVec4fMenuItem ()     .signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &X3DUserDefinedFieldsEditor::on_field_type_activate), "MFVec4f"));

	//  Drag & Drop

	getUserDefinedFieldsTreeView () .enable_model_drag_source ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);
	getUserDefinedFieldsTreeView () .enable_model_drag_dest ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::ACTION_MOVE);

	getUserDefinedFieldsTreeView () .set_model (getUserDefinedFieldsListStore ());
}

void
X3DUserDefinedFieldsEditor::setNode (const X3D::SFNode & value)
{

	if (node and node -> canUserDefinedFields ())
		node -> fields_changed () .removeInterest (this, &X3DUserDefinedFieldsEditor::set_fields);

	node             = value;
	userDefinedField = nullptr;

	if (node and node -> canUserDefinedFields ())
	{
		getUserDefinedFieldsExpander () .set_visible (true);

		node -> fields_changed () .addInterest (this, &X3DUserDefinedFieldsEditor::set_fields);

		set_fields ();
	}
	else
	{
		getUserDefinedFieldsExpander () .set_visible (false);

		getUserDefinedFieldsListStore () -> clear ();
	}
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
			userDefinedFields .insert (userDefinedFields .begin () + dest, field);
			userDefinedFields .erase (userDefinedFields .begin () + index);
		}
		else
		{
			userDefinedFields .erase (userDefinedFields .begin () + index);
			userDefinedFields .insert (userDefinedFields .begin () + dest, field);
		}

		// Set user defined field.

		const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Reorder User-Defined Field »%s«"), field -> getName () .c_str ()));

		node -> fields_changed () .removeInterest (this, &X3DUserDefinedFieldsEditor::set_fields);
		node -> fields_changed () .addInterest (this, &X3DUserDefinedFieldsEditor::connectFields);

		setUserDefinedFields (node, userDefinedFields, undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (...)
	{ }

	context -> drag_finish (false, false, time);
}

void
X3DUserDefinedFieldsEditor::on_user_defined_field_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	try
	{
		editing          = true;
		userDefinedField = node -> getUserDefinedFields () .at (path .front ());

		getAccessTypeLabel () .set_text (X3D::Generator::X3DAccessTypes [userDefinedField]);
		getFieldTypeLabel () .set_text (userDefinedField -> getTypeName ());
		getFieldNameEntry () .set_text (userDefinedField -> getName ());
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

	getInitializeOnlyMenuItem () .activate ();
	getSFBoolMenuItem () .activate ();
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

	const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Remove User-Defined Field »%s«"), field -> getName () .c_str ()));

	node -> fields_changed () .removeInterest (this, &X3DUserDefinedFieldsEditor::set_fields);
	node -> fields_changed () .addInterest (this, &X3DUserDefinedFieldsEditor::connectFields);

	removeUserDefinedField (node, field, undoStep);

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
		std::make_pair ("initializeOnly", X3D::initializeOnly),
		std::make_pair ("inputOnly",      X3D::inputOnly),
		std::make_pair ("outputOnly",     X3D::outputOnly),
		std::make_pair ("inputOutput",    X3D::inputOutput)
	};

	try
	{
		getUserDefinedFieldDialog () .hide ();

		if (editing)
		{
			if (userDefinedField)
			{
				// Edit field.

				X3D::X3DFieldDefinition* const field = getBrowser () -> getSupportedField (getFieldTypeLabel () .get_text ()) -> create ();

				field -> setName (getFieldNameEntry () .get_text ());
				field -> setAccessType (accessTypes .at (getAccessTypeLabel () .get_text ()));

				// Replace field in list store.

				replaceUserDefinedField (userDefinedField, field);

				// Add user defined field.

				const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Update User-Defined Field »%s«"), field -> getName () .c_str ()));

				node -> fields_changed () .removeInterest (this, &X3DUserDefinedFieldsEditor::set_fields);
				node -> fields_changed () .addInterest (this, &X3DUserDefinedFieldsEditor::connectFields);

				replaceUserDefinedField (node, userDefinedField, field, undoStep);

				getBrowserWindow () -> addUndoStep (undoStep);
			}
		}
		else
		{
			// Create field.

			X3D::X3DFieldDefinition* const field = getBrowser () -> getSupportedField (getFieldTypeLabel () .get_text ()) -> create ();

			field -> setName (getFieldNameEntry () .get_text ());
			field -> setAccessType (accessTypes .at (getAccessTypeLabel () .get_text ()));

			// Add field to list store.

			addUserDefinedField (field);

			// Add user defined field.

			const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Add User-Defined Field »%s«"), field -> getName () .c_str ()));

			node -> fields_changed () .removeInterest (this, &X3DUserDefinedFieldsEditor::set_fields);
			node -> fields_changed () .addInterest (this, &X3DUserDefinedFieldsEditor::connectFields);

			addUserDefinedField (node, field, undoStep);

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
	(*iter) [columns .accessType] = Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/" + X3D::Generator::X3DAccessTypes [field] + ".png"));
}

void
X3DUserDefinedFieldsEditor::replaceUserDefinedField (X3D::X3DFieldDefinition* const oldField, X3D::X3DFieldDefinition* const newField)
{
	const auto userDefinedFields = node -> getUserDefinedFields ();
	const auto pos               = std::find (userDefinedFields .begin (), userDefinedFields .end (), oldField);

	if (pos == userDefinedFields .end ())
		return;

	const size_t index = pos - userDefinedFields .begin ();

	// Replace in list store.

	const auto iter = getUserDefinedFieldsListStore () -> children () [index];

	(*iter) [columns .type]       = Gdk::Pixbuf::create_from_file (get_ui ("icons/FieldType/" + newField -> getTypeName () + ".svg"));
	(*iter) [columns .name]       = newField -> getName ();
	(*iter) [columns .accessType] = Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/" + X3D::Generator::X3DAccessTypes [newField] + ".png"));
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
	field .removeInterest (this, &X3DUserDefinedFieldsEditor::connectFields);
	field .addInterest (this, &X3DUserDefinedFieldsEditor::set_fields);
}

/***********************************************************************************************************************
 *
 *  Undo operations
 *
 **********************************************************************************************************************/

void
X3DUserDefinedFieldsEditor::addUserDefinedField (const X3D::SFNode & node, X3D::X3DFieldDefinition* const field, const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (X3D::FieldPtr (field));

	undoStep -> addUndoFunction (&X3D::X3DBaseNode::removeUserDefinedField, node, field -> getName ());
	undoStep -> addRedoFunction (&X3D::X3DBaseNode::addUserDefinedField, node, field -> getAccessType (), field -> getName (), field);

	node -> addUserDefinedField (field -> getAccessType (), field -> getName (), field);
}

void
X3DUserDefinedFieldsEditor::replaceUserDefinedField (const X3D::SFNode & node, X3D::X3DFieldDefinition* const oldField, X3D::X3DFieldDefinition* const newField, const UndoStepPtr & undoStep)
{
	auto userDefinedFields = node -> getUserDefinedFields ();
	auto iter              = std::find (userDefinedFields .begin (), userDefinedFields .end (), oldField);

	if (iter == userDefinedFields .end ())
		return;

	// If possible we want to reassign the routes from the old field to the new fields.  In this step we create addRoutes
   // functions we will execute later.
   
	std::deque <std::function <void ()>>  addRoutes;

   if (newField -> getType () == oldField -> getType ())
   {
      // Assign the old value to the new field.
   
      newField -> set (*oldField);

      // Reassign IS reference to the new field.
 
      for (const auto & reference : oldField -> getIsReferences ())
      {
         if (newField -> getAccessType () == reference -> getAccessType () or newField -> getAccessType () == X3D::inputOutput)
            newField -> addIsReference (reference);
      }

      // Create addRoutes functions for input and output routes.
 
      if (newField -> isInput () and oldField -> isInput ())
      {
         for (const auto & route : oldField -> getInputRoutes ())
         {
            const bool selfConnection = route -> getSourceNode () == node and route -> getSourceField () == oldField -> getName ();

            addRoutes .emplace_back (std::bind (&X3DBrowserWindow::addRoute,
                                                node -> getExecutionContext (),
                                                route -> getSourceNode (),
                                                selfConnection ? newField -> getName () : route -> getSourceField (),
                                                node,
                                                newField -> getName (),
                                                undoStep));
         }
      }

      if (newField -> isOutput () and oldField -> isOutput ())
      {
         for (const auto & route : oldField -> getOutputRoutes ())
         {
            const bool selfConnection = route -> getDestinationNode () == node and route -> getDestinationField () == oldField -> getName ();

            addRoutes .emplace_back (std::bind (&X3DBrowserWindow::addRoute,
                                                node -> getExecutionContext (),
                                                node,
                                                newField -> getName (),
                                                route -> getDestinationNode (),
                                                selfConnection ? newField -> getName () : route -> getDestinationField (),
                                                undoStep));
         }
      }
   }
  
	// Save all involved fields.

	undoStep -> addObjects (X3D::FieldArray (userDefinedFields .begin (), userDefinedFields .end ()), X3D::FieldPtr (newField));
 
	// Remove user data from old field.

	undoStep -> addUndoFunction (&X3D::X3DFieldDefinition::setUserData, oldField, nullptr);
	undoStep -> addRedoFunction (&X3D::X3DFieldDefinition::setUserData, newField, nullptr);
	newField -> setUserData (nullptr);

	// Remove routes from field.  We must do this as routes are associated with a node and we are self responsible for doing this.
   
   removeRoutes (oldField, undoStep);

	// Restore old user defined fields in undo.

	undoStep -> addUndoFunction (&X3D::X3DBaseNode::setUserDefinedFields, node, userDefinedFields);

	// Replace old field with new field in temporary array.

	*iter = newField;
	
	// Set new user defined fields.

	undoStep -> addRedoFunction (&X3D::X3DBaseNode::setUserDefinedFields, node, userDefinedFields);
	
	node -> setUserDefinedFields (userDefinedFields);

	// Now process the addRoutes functions recorded above to reassign the routes from the old field to the new field.

	for (const auto & addRoute : addRoutes)
	{
	   try
	   {
	      addRoute ();
	   }
	   catch (const X3D::X3DError &)
	   { }
	}
}

void
X3DUserDefinedFieldsEditor::removeUserDefinedField (const X3D::SFNode & node, X3D::X3DFieldDefinition* const field, const UndoStepPtr & undoStep)
{
	const auto userDefinedFields = node -> getUserDefinedFields ();

	undoStep -> addObjects (X3D::FieldArray (userDefinedFields .begin (), userDefinedFields .end ()), X3D::FieldPtr (field));

	// Remove user data from old field.

	undoStep -> addUndoFunction (&X3D::X3DFieldDefinition::setUserData, field, nullptr);
 
	// Remove routes from field.  We must do this as routes are associated with a node and we are self responsible for doing this.
   
   removeRoutes (field, undoStep);

	undoStep -> addUndoFunction (&X3D::X3DBaseNode::setUserDefinedFields, node, userDefinedFields);
	undoStep -> addRedoFunction (&X3D::X3DBaseNode::removeUserDefinedField, node, field -> getName ());

	node -> removeUserDefinedField (field -> getName ());	
}

void
X3DUserDefinedFieldsEditor::setUserDefinedFields (const X3D::SFNode & node, const X3D::FieldDefinitionArray & userDefinedFields, const UndoStepPtr & undoStep)
{
	// Remove any routes and user data.

	const auto currentUserDefinedFields = node -> getUserDefinedFields ();
	
	std::set <X3D::X3DFieldDefinition*>    lhs (currentUserDefinedFields .begin (), currentUserDefinedFields .end ()); 
	std::set <X3D::X3DFieldDefinition*>    rhs (userDefinedFields .begin (), userDefinedFields .end ()); 
	std::vector <X3D::X3DFieldDefinition*> difference;

	std::set_difference (lhs .begin (), lhs .end (), rhs .begin (), rhs .end (), std::back_inserter (difference));

	for (const auto field : difference)
	{
		undoStep -> addUndoFunction (&X3D::X3DFieldDefinition::setUserData, field, nullptr);
		removeRoutes (field, undoStep);
	}

	// Replace the user defined fields.

	undoStep -> addUndoFunction (&X3D::X3DBaseNode::setUserDefinedFields, node, currentUserDefinedFields);
	undoStep -> addRedoFunction (&X3D::X3DBaseNode::setUserDefinedFields, node, userDefinedFields);

	node -> setUserDefinedFields (userDefinedFields);	
}

void
X3DUserDefinedFieldsEditor::removeRoutes (X3D::X3DFieldDefinition* const field, const UndoStepPtr & undoStep)
{
	// Remove routes from field.

   for (const auto & route : X3D::RouteSet (field -> getInputRoutes ()))
   {
      X3DBrowserWindow::deleteRoute (route -> getExecutionContext (),
                                     route -> getSourceNode (),
                                     route -> getSourceField (),
                                     route -> getDestinationNode (),
                                     route -> getDestinationField (),
                                     undoStep);
   }

   for (const auto & route : X3D::RouteSet (field -> getOutputRoutes ()))
   {
      X3DBrowserWindow::deleteRoute (route -> getExecutionContext (),
                                     route -> getSourceNode (),
                                     route -> getSourceField (),
                                     route -> getDestinationNode (),
                                     route -> getDestinationField (),
                                     undoStep);
   }
}

X3DUserDefinedFieldsEditor::~X3DUserDefinedFieldsEditor ()
{ }

} // puck
} // titania
