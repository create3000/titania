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

#include "X3DNodePropertiesEditorInterface.h"

namespace titania {
namespace puck {

void
X3DNodePropertiesEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DNodePropertiesEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DNodePropertiesEditorInterface::create ()
{
	// Get objects.
	m_ExportedNodesListStore                    = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ExportedNodesListStore"));
	m_FieldNameListStore                        = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("FieldNameListStore"));
	m_FieldNameCompletion                       = Glib::RefPtr <Gtk::EntryCompletion>::cast_dynamic (m_builder -> get_object ("FieldNameCompletion"));
	m_ImportedNodesListStore                    = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ImportedNodesListStore"));
	m_UserDefinedFieldsTreeSelection            = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("UserDefinedFieldsTreeSelection"));
	m_CellRendererType                          = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("CellRendererType"));
	m_CellRendererName                          = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("CellRendererName"));
	m_CellRendererAccessType                    = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("CellRendererAccessType"));
	m_ImportedNodesTreeSelection                = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("ImportedNodesTreeSelection"));
	m_ImportedNodesImportedTreeviewColumn       = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ImportedNodesImportedTreeviewColumn"));
	m_ImportedNodesImportedCellRendererToggle   = Glib::RefPtr <Gtk::CellRendererToggle>::cast_dynamic (m_builder -> get_object ("ImportedNodesImportedCellRendererToggle"));
	m_ImportedNodesTypeNameCellRendererText     = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("ImportedNodesTypeNameCellRendererText"));
	m_ImportedNodesExportedNameCellRendererText = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("ImportedNodesExportedNameCellRendererText"));
	m_ImportedNodesImportedNameTreeviewColumn   = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ImportedNodesImportedNameTreeviewColumn"));
	m_ImportedNodesImportedNameCellRendererText = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("ImportedNodesImportedNameCellRendererText"));
	m_ExportedNodesTreeSelection                = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("ExportedNodesTreeSelection"));

	// Get widgets.
	m_builder -> get_widget ("AccessTypeMenu", m_AccessTypeMenu);
	m_builder -> get_widget ("InitializeOnlyMenuItem", m_InitializeOnlyMenuItem);
	m_builder -> get_widget ("InputOnlyMenuItem", m_InputOnlyMenuItem);
	m_builder -> get_widget ("OutputOnlyMenuItem", m_OutputOnlyMenuItem);
	m_builder -> get_widget ("InputOutputMenuItem", m_InputOutputMenuItem);
	m_builder -> get_widget ("FieldTypeMenu", m_FieldTypeMenu);
	m_builder -> get_widget ("SFBoolMenuItem", m_SFBoolMenuItem);
	m_builder -> get_widget ("SFColorMenuItem", m_SFColorMenuItem);
	m_builder -> get_widget ("SFColorRGBAMenuItem", m_SFColorRGBAMenuItem);
	m_builder -> get_widget ("SFDoubleMenuItem", m_SFDoubleMenuItem);
	m_builder -> get_widget ("SFFloatMenuItem", m_SFFloatMenuItem);
	m_builder -> get_widget ("SFImageMenuItem", m_SFImageMenuItem);
	m_builder -> get_widget ("SFInt32MenuItem", m_SFInt32MenuItem);
	m_builder -> get_widget ("SFMatrix3dMenuItem", m_SFMatrix3dMenuItem);
	m_builder -> get_widget ("SFMatrix3fMenuItem", m_SFMatrix3fMenuItem);
	m_builder -> get_widget ("SFMatrix4dMenuItem", m_SFMatrix4dMenuItem);
	m_builder -> get_widget ("SFMatrix4fMenuItem", m_SFMatrix4fMenuItem);
	m_builder -> get_widget ("SFNodeMenuItem", m_SFNodeMenuItem);
	m_builder -> get_widget ("SFRotationMenuItem", m_SFRotationMenuItem);
	m_builder -> get_widget ("SFStringMenuItem", m_SFStringMenuItem);
	m_builder -> get_widget ("SFTimeMenuItem", m_SFTimeMenuItem);
	m_builder -> get_widget ("SFVec2dMenuItem", m_SFVec2dMenuItem);
	m_builder -> get_widget ("SFVec2fMenuItem", m_SFVec2fMenuItem);
	m_builder -> get_widget ("SFVec3dMenuItem", m_SFVec3dMenuItem);
	m_builder -> get_widget ("SFVec3fMenuItem", m_SFVec3fMenuItem);
	m_builder -> get_widget ("SFVec4dMenuItem", m_SFVec4dMenuItem);
	m_builder -> get_widget ("SFVec4fMenuItem", m_SFVec4fMenuItem);
	m_builder -> get_widget ("MFBoolMenuItem", m_MFBoolMenuItem);
	m_builder -> get_widget ("MFColorMenuItem", m_MFColorMenuItem);
	m_builder -> get_widget ("MFColorRGBAMenuItem", m_MFColorRGBAMenuItem);
	m_builder -> get_widget ("MFDoubleMenuItem", m_MFDoubleMenuItem);
	m_builder -> get_widget ("MFFloatMenuItem", m_MFFloatMenuItem);
	m_builder -> get_widget ("MFImageMenuItem", m_MFImageMenuItem);
	m_builder -> get_widget ("MFInt32MenuItem", m_MFInt32MenuItem);
	m_builder -> get_widget ("MFMatrix3dMenuItem", m_MFMatrix3dMenuItem);
	m_builder -> get_widget ("MFMatrix3fMenuItem", m_MFMatrix3fMenuItem);
	m_builder -> get_widget ("MFMatrix4dMenuItem", m_MFMatrix4dMenuItem);
	m_builder -> get_widget ("MFMatrix4fMenuItem", m_MFMatrix4fMenuItem);
	m_builder -> get_widget ("MFNodeMenuItem", m_MFNodeMenuItem);
	m_builder -> get_widget ("MFRotationMenuItem", m_MFRotationMenuItem);
	m_builder -> get_widget ("MFStringMenuItem", m_MFStringMenuItem);
	m_builder -> get_widget ("MFTimeMenuItem", m_MFTimeMenuItem);
	m_builder -> get_widget ("MFVec2dMenuItem", m_MFVec2dMenuItem);
	m_builder -> get_widget ("MFVec2fMenuItem", m_MFVec2fMenuItem);
	m_builder -> get_widget ("MFVec3dMenuItem", m_MFVec3dMenuItem);
	m_builder -> get_widget ("MFVec3fMenuItem", m_MFVec3fMenuItem);
	m_builder -> get_widget ("MFVec4dMenuItem", m_MFVec4dMenuItem);
	m_builder -> get_widget ("MFVec4fMenuItem", m_MFVec4fMenuItem);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("NodeChildNotebook", m_NodeChildNotebook);
	m_builder -> get_widget ("NodePropertiesExpander", m_NodePropertiesExpander);
	m_builder -> get_widget ("TypeNameLabel", m_TypeNameLabel);
	m_builder -> get_widget ("NameLabel", m_NameLabel);
	m_builder -> get_widget ("TypeNameEntry", m_TypeNameEntry);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("CommonPropertiesExpander", m_CommonPropertiesExpander);
	m_builder -> get_widget ("ComponentEntry", m_ComponentEntry);
	m_builder -> get_widget ("ContainerFieldEntry", m_ContainerFieldEntry);
	m_builder -> get_widget ("UserDefinedFieldsBox", m_UserDefinedFieldsBox);
	m_builder -> get_widget ("UserDefinedFieldsExpander", m_UserDefinedFieldsExpander);
	m_builder -> get_widget ("UserDefinedFieldsWidget", m_UserDefinedFieldsWidget);
	m_builder -> get_widget ("UserDefinedFieldsTreeView", m_UserDefinedFieldsTreeView);
	m_builder -> get_widget ("UserDefinedFieldsActionBox", m_UserDefinedFieldsActionBox);
	m_builder -> get_widget ("AddUserDefinedFieldButton", m_AddUserDefinedFieldButton);
	m_builder -> get_widget ("RemoveUserDefinedFieldButton", m_RemoveUserDefinedFieldButton);
	m_builder -> get_widget ("ImportedNodesBox", m_ImportedNodesBox);
	m_builder -> get_widget ("ImportedNodesExpander", m_ImportedNodesExpander);
	m_builder -> get_widget ("ImportedNodesTreeView", m_ImportedNodesTreeView);
	m_builder -> get_widget ("ExportedNodesBox", m_ExportedNodesBox);
	m_builder -> get_widget ("ExportedNodesExpander", m_ExportedNodesExpander);
	m_builder -> get_widget ("ExportedNodesTreeView", m_ExportedNodesTreeView);
	m_builder -> get_widget ("AddExportedNodeButton", m_AddExportedNodeButton);
	m_builder -> get_widget ("RemoveExportedNodeButton", m_RemoveExportedNodeButton);
	m_builder -> get_widget ("ExportedNodeDialog", m_ExportedNodeDialog);
	m_builder -> get_widget ("ExportedNodeCancelButton", m_ExportedNodeCancelButton);
	m_builder -> get_widget ("ExportedNodeOkButton", m_ExportedNodeOkButton);
	m_builder -> get_widget ("ExportedNameEntry", m_ExportedNameEntry);
	m_builder -> get_widget ("UserDefinedFieldDialog", m_UserDefinedFieldDialog);
	m_builder -> get_widget ("UserDefinedFieldCancelButton", m_UserDefinedFieldCancelButton);
	m_builder -> get_widget ("AddUserDefinedFieldOkButton", m_AddUserDefinedFieldOkButton);
	m_builder -> get_widget ("AccessTypeMenuButton", m_AccessTypeMenuButton);
	m_builder -> get_widget ("AccessTypeLabel", m_AccessTypeLabel);
	m_builder -> get_widget ("FieldTypeMenuButton", m_FieldTypeMenuButton);
	m_builder -> get_widget ("FieldTypeLabel", m_FieldTypeLabel);
	m_builder -> get_widget ("FieldNameEntry", m_FieldNameEntry);

	// Connect object Gtk::TreeView with id 'UserDefinedFieldsTreeView'.
	m_UserDefinedFieldsTreeView -> signal_drag_data_received () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_drag_data_received));
	m_UserDefinedFieldsTreeView -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_user_defined_field_focus_in_event));
	m_UserDefinedFieldsTreeView -> signal_focus_out_event () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_user_defined_field_focus_out_event));
	m_UserDefinedFieldsTreeView -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_user_defined_field_key_press_event), false);
	m_UserDefinedFieldsTreeView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_user_defined_field_activated));

	// Connect object Gtk::TreeSelection with id 'UserDefinedFieldsTreeSelection'.
	m_UserDefinedFieldsTreeSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_user_defined_field_changed));

	// Connect object Gtk::Button with id 'AddUserDefinedFieldButton'.
	m_AddUserDefinedFieldButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_add_user_defined_field_clicked));
	m_RemoveUserDefinedFieldButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_remove_user_defined_field_clicked));

	// Connect object Gtk::CellRendererToggle with id 'ImportedNodesImportedCellRendererToggle'.
	m_ImportedNodesImportedCellRendererToggle -> signal_toggled () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_imported_toggled));

	// Connect object Gtk::CellRendererText with id 'ImportedNodesImportedNameCellRendererText'.
	m_ImportedNodesImportedNameCellRendererText -> signal_edited () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_imported_name_edited));

	// Connect object Gtk::TreeView with id 'ExportedNodesTreeView'.
	m_ExportedNodesTreeView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_exported_node_activated));

	// Connect object Gtk::TreeSelection with id 'ExportedNodesTreeSelection'.
	m_ExportedNodesTreeSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_exported_node_changed));

	// Connect object Gtk::Button with id 'AddExportedNodeButton'.
	m_AddExportedNodeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_add_exported_node));
	m_RemoveExportedNodeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_remove_exported_node));
	m_ExportedNodeCancelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_exported_node_cancel_clicked));
	m_ExportedNodeOkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_exported_node_ok_clicked));

	// Connect object Gtk::Entry with id 'ExportedNameEntry'.
	m_ExportedNameEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_exported_name_changed));
	m_ExportedNameEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_exported_name_delete_text), false);
	m_ExportedNameEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_exported_name_insert_text), false);

	// Connect object Gtk::Button with id 'UserDefinedFieldCancelButton'.
	m_UserDefinedFieldCancelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_add_field_cancel_clicked));
	m_AddUserDefinedFieldOkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_add_field_ok_clicked));

	// Connect object Gtk::Entry with id 'FieldNameEntry'.
	m_FieldNameEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_field_name_changed));
	m_FieldNameEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_field_name_delete_text), false);
	m_FieldNameEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DNodePropertiesEditorInterface::on_field_name_insert_text), false);
}

X3DNodePropertiesEditorInterface::~X3DNodePropertiesEditorInterface ()
{
	delete m_Window;
	delete m_ExportedNodeDialog;
	delete m_UserDefinedFieldDialog;
}

} // puck
} // titania
