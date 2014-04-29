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
#include "X3DNodePropertiesEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DNodePropertiesEditorInterface::m_widgetName = "NodePropertiesEditor";

void
X3DNodePropertiesEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_cellRendererType       = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("CellRendererType"));
	m_cellRendererName       = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("CellRendererName"));
	m_cellRendererAccessType = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("CellRendererAccessType"));

	// Get widgets.
	m_builder -> get_widget ("AccessTypeMenu", m_accessTypeMenu);
	m_accessTypeMenu -> set_name ("AccessTypeMenu");
	m_builder -> get_widget ("InitializeOnlyMenuItem", m_initializeOnlyMenuItem);
	m_initializeOnlyMenuItem -> set_name ("InitializeOnlyMenuItem");
	m_builder -> get_widget ("InputOnlyMenuItem", m_inputOnlyMenuItem);
	m_inputOnlyMenuItem -> set_name ("InputOnlyMenuItem");
	m_builder -> get_widget ("OutputOnlyMenuItem", m_outputOnlyMenuItem);
	m_outputOnlyMenuItem -> set_name ("OutputOnlyMenuItem");
	m_builder -> get_widget ("InputOutputMenuItem", m_inputOutputMenuItem);
	m_inputOutputMenuItem -> set_name ("InputOutputMenuItem");
	m_builder -> get_widget ("FieldTypeMenu", m_fieldTypeMenu);
	m_fieldTypeMenu -> set_name ("FieldTypeMenu");
	m_builder -> get_widget ("SFBoolMenuItem", m_sFBoolMenuItem);
	m_sFBoolMenuItem -> set_name ("SFBoolMenuItem");
	m_builder -> get_widget ("SFColorMenuItem", m_sFColorMenuItem);
	m_sFColorMenuItem -> set_name ("SFColorMenuItem");
	m_builder -> get_widget ("SFColorRGBAMenuItem", m_sFColorRGBAMenuItem);
	m_sFColorRGBAMenuItem -> set_name ("SFColorRGBAMenuItem");
	m_builder -> get_widget ("SFDoubleMenuItem", m_sFDoubleMenuItem);
	m_sFDoubleMenuItem -> set_name ("SFDoubleMenuItem");
	m_builder -> get_widget ("SFFloatMenuItem", m_sFFloatMenuItem);
	m_sFFloatMenuItem -> set_name ("SFFloatMenuItem");
	m_builder -> get_widget ("SFImageMenuItem", m_sFImageMenuItem);
	m_sFImageMenuItem -> set_name ("SFImageMenuItem");
	m_builder -> get_widget ("SFInt32MenuItem", m_sFInt32MenuItem);
	m_sFInt32MenuItem -> set_name ("SFInt32MenuItem");
	m_builder -> get_widget ("SFMatrix3fMenuItem", m_sFMatrix3fMenuItem);
	m_sFMatrix3fMenuItem -> set_name ("SFMatrix3fMenuItem");
	m_builder -> get_widget ("SFMatrix3dMenuItem", m_sFMatrix3dMenuItem);
	m_sFMatrix3dMenuItem -> set_name ("SFMatrix3dMenuItem");
	m_builder -> get_widget ("SFMatrix4fMenuItem", m_sFMatrix4fMenuItem);
	m_sFMatrix4fMenuItem -> set_name ("SFMatrix4fMenuItem");
	m_builder -> get_widget ("SFMatrix4dMenuItem", m_sFMatrix4dMenuItem);
	m_sFMatrix4dMenuItem -> set_name ("SFMatrix4dMenuItem");
	m_builder -> get_widget ("SFNodeMenuItem", m_sFNodeMenuItem);
	m_sFNodeMenuItem -> set_name ("SFNodeMenuItem");
	m_builder -> get_widget ("SFRotationMenuItem", m_sFRotationMenuItem);
	m_sFRotationMenuItem -> set_name ("SFRotationMenuItem");
	m_builder -> get_widget ("SFStringMenuItem", m_sFStringMenuItem);
	m_sFStringMenuItem -> set_name ("SFStringMenuItem");
	m_builder -> get_widget ("SFTimeMenuItem", m_sFTimeMenuItem);
	m_sFTimeMenuItem -> set_name ("SFTimeMenuItem");
	m_builder -> get_widget ("SFVec2fMenuItem", m_sFVec2fMenuItem);
	m_sFVec2fMenuItem -> set_name ("SFVec2fMenuItem");
	m_builder -> get_widget ("SFVec2dMenuItem", m_sFVec2dMenuItem);
	m_sFVec2dMenuItem -> set_name ("SFVec2dMenuItem");
	m_builder -> get_widget ("SFVec3fMenuItem", m_sFVec3fMenuItem);
	m_sFVec3fMenuItem -> set_name ("SFVec3fMenuItem");
	m_builder -> get_widget ("SFVec3dMenuItem", m_sFVec3dMenuItem);
	m_sFVec3dMenuItem -> set_name ("SFVec3dMenuItem");
	m_builder -> get_widget ("SFVec4fMenuItem", m_sFVec4fMenuItem);
	m_sFVec4fMenuItem -> set_name ("SFVec4fMenuItem");
	m_builder -> get_widget ("SFVec4dMenuItem", m_sFVec4dMenuItem);
	m_sFVec4dMenuItem -> set_name ("SFVec4dMenuItem");
	m_builder -> get_widget ("MFBoolMenuItem", m_mFBoolMenuItem);
	m_mFBoolMenuItem -> set_name ("MFBoolMenuItem");
	m_builder -> get_widget ("MFColorMenuItem", m_mFColorMenuItem);
	m_mFColorMenuItem -> set_name ("MFColorMenuItem");
	m_builder -> get_widget ("MFColorRGBAMenuItem", m_mFColorRGBAMenuItem);
	m_mFColorRGBAMenuItem -> set_name ("MFColorRGBAMenuItem");
	m_builder -> get_widget ("MFDoubleMenuItem", m_mFDoubleMenuItem);
	m_mFDoubleMenuItem -> set_name ("MFDoubleMenuItem");
	m_builder -> get_widget ("MFFloatMenuItem", m_mFFloatMenuItem);
	m_mFFloatMenuItem -> set_name ("MFFloatMenuItem");
	m_builder -> get_widget ("MFImageMenuItem", m_mFImageMenuItem);
	m_mFImageMenuItem -> set_name ("MFImageMenuItem");
	m_builder -> get_widget ("MFInt32MenuItem", m_mFInt32MenuItem);
	m_mFInt32MenuItem -> set_name ("MFInt32MenuItem");
	m_builder -> get_widget ("MFMatrix3fMenuItem", m_mFMatrix3fMenuItem);
	m_mFMatrix3fMenuItem -> set_name ("MFMatrix3fMenuItem");
	m_builder -> get_widget ("MFMatrix3dMenuItem", m_mFMatrix3dMenuItem);
	m_mFMatrix3dMenuItem -> set_name ("MFMatrix3dMenuItem");
	m_builder -> get_widget ("MFMatrix4fMenuItem", m_mFMatrix4fMenuItem);
	m_mFMatrix4fMenuItem -> set_name ("MFMatrix4fMenuItem");
	m_builder -> get_widget ("MFMatrix4dMenuItem", m_mFMatrix4dMenuItem);
	m_mFMatrix4dMenuItem -> set_name ("MFMatrix4dMenuItem");
	m_builder -> get_widget ("MFNodeMenuItem", m_mFNodeMenuItem);
	m_mFNodeMenuItem -> set_name ("MFNodeMenuItem");
	m_builder -> get_widget ("MFRotationMenuItem", m_mFRotationMenuItem);
	m_mFRotationMenuItem -> set_name ("MFRotationMenuItem");
	m_builder -> get_widget ("MFStringMenuItem", m_mFStringMenuItem);
	m_mFStringMenuItem -> set_name ("MFStringMenuItem");
	m_builder -> get_widget ("MFTimeMenuItem", m_mFTimeMenuItem);
	m_mFTimeMenuItem -> set_name ("MFTimeMenuItem");
	m_builder -> get_widget ("MFVec2fMenuItem", m_mFVec2fMenuItem);
	m_mFVec2fMenuItem -> set_name ("MFVec2fMenuItem");
	m_builder -> get_widget ("MFVec2dMenuItem", m_mFVec2dMenuItem);
	m_mFVec2dMenuItem -> set_name ("MFVec2dMenuItem");
	m_builder -> get_widget ("MFVec3fMenuItem", m_mFVec3fMenuItem);
	m_mFVec3fMenuItem -> set_name ("MFVec3fMenuItem");
	m_builder -> get_widget ("MFVec3dMenuItem", m_mFVec3dMenuItem);
	m_mFVec3dMenuItem -> set_name ("MFVec3dMenuItem");
	m_builder -> get_widget ("MFVec4fMenuItem", m_mFVec4fMenuItem);
	m_mFVec4fMenuItem -> set_name ("MFVec4fMenuItem");
	m_builder -> get_widget ("MFVec4dMenuItem", m_mFVec4dMenuItem);
	m_mFVec4dMenuItem -> set_name ("MFVec4dMenuItem");
	m_builder -> get_widget ("Window", m_window);
	m_window -> set_name ("Window");
	m_builder -> get_widget ("CancelButton", m_cancelButton);
	m_cancelButton -> set_name ("CancelButton");
	m_builder -> get_widget ("OkButton", m_okButton);
	m_okButton -> set_name ("OkButton");
	m_builder -> get_widget ("Widget", m_widget);
	m_widget -> set_name ("Widget");
	m_builder -> get_widget ("HeaderLabel", m_headerLabel);
	m_headerLabel -> set_name ("HeaderLabel");
	m_builder -> get_widget ("NodePropertiesExpander", m_nodePropertiesExpander);
	m_nodePropertiesExpander -> set_name ("NodePropertiesExpander");
	m_builder -> get_widget ("TypeNameLabel", m_typeNameLabel);
	m_typeNameLabel -> set_name ("TypeNameLabel");
	m_builder -> get_widget ("NameLabel", m_nameLabel);
	m_nameLabel -> set_name ("NameLabel");
	m_builder -> get_widget ("TypeNameEntry", m_typeNameEntry);
	m_typeNameEntry -> set_name ("TypeNameEntry");
	m_builder -> get_widget ("NameEntry", m_nameEntry);
	m_nameEntry -> set_name ("NameEntry");
	m_builder -> get_widget ("UserDefinedFieldsExpander", m_userDefinedFieldsExpander);
	m_userDefinedFieldsExpander -> set_name ("UserDefinedFieldsExpander");
	m_builder -> get_widget ("UserDefinedFieldsTreeView", m_userDefinedFieldsTreeView);
	m_userDefinedFieldsTreeView -> set_name ("UserDefinedFieldsTreeView");
	m_builder -> get_widget ("AddFieldButton", m_addFieldButton);
	m_addFieldButton -> set_name ("AddFieldButton");
	m_builder -> get_widget ("RemoveFieldButton", m_removeFieldButton);
	m_removeFieldButton -> set_name ("RemoveFieldButton");
	m_builder -> get_widget ("AddFieldDialog", m_addFieldDialog);
	m_addFieldDialog -> set_name ("AddFieldDialog");
	m_builder -> get_widget ("AddFieldCancelButton", m_addFieldCancelButton);
	m_addFieldCancelButton -> set_name ("AddFieldCancelButton");
	m_builder -> get_widget ("AddFieldOkButton", m_addFieldOkButton);
	m_addFieldOkButton -> set_name ("AddFieldOkButton");
	m_builder -> get_widget ("AccessTypeMenuButton", m_accessTypeMenuButton);
	m_accessTypeMenuButton -> set_name ("AccessTypeMenuButton");
	m_builder -> get_widget ("AccessTypeLabel", m_accessTypeLabel);
	m_accessTypeLabel -> set_name ("AccessTypeLabel");
	m_builder -> get_widget ("FieldTypeMenuButton", m_fieldTypeMenuButton);
	m_fieldTypeMenuButton -> set_name ("FieldTypeMenuButton");
	m_builder -> get_widget ("FieldTypeLabel", m_fieldTypeLabel);
	m_fieldTypeLabel -> set_name ("FieldTypeLabel");
	m_builder -> get_widget ("FieldNameEntry", m_fieldNameEntry);
	m_fieldNameEntry -> set_name ("FieldNameEntry");

	// Connect object Gtk::Button with id 'CancelButton'.
	m_cancelButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_cancel));
	m_okButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_ok));

	// Connect object Gtk::Entry with id 'TypeNameEntry'.
	m_typeNameEntry -> signal_delete_text () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_type_name_delete_text), false);
	m_typeNameEntry -> signal_insert_text () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_type_name_insert_text), false);
	m_nameEntry -> signal_delete_text () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_name_delete_text), false);
	m_nameEntry -> signal_insert_text () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_name_insert_text), false);

	// Connect object Gtk::Button with id 'AddFieldButton'.
	m_addFieldButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_add_field_clicked));
	m_removeFieldButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_remove_field_clicked));
	m_addFieldCancelButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_add_field_cancel_clicked));
	m_addFieldOkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_add_field_ok_clicked));

	// Connect object Gtk::Entry with id 'FieldNameEntry'.
	m_fieldNameEntry -> signal_changed () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_field_name_changed));
	m_fieldNameEntry -> signal_delete_text () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_field_name_delete_text), false);
	m_fieldNameEntry -> signal_insert_text () .connect (sigc::mem_fun (*this, &X3DNodePropertiesEditorInterface::on_field_name_insert_text), false);

	// Call construct handler of base class.
	construct ();
}

X3DNodePropertiesEditorInterface::~X3DNodePropertiesEditorInterface ()
{
	delete m_window;
	delete m_addFieldDialog;
}

} // puck
} // titania
