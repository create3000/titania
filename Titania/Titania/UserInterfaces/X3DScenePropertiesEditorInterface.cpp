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

#include "X3DScenePropertiesEditorInterface.h"

namespace titania {
namespace puck {

void
X3DScenePropertiesEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DScenePropertiesEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DScenePropertiesEditorInterface::create ()
{
	// Get objects.
	m_ComponentListStore           = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ComponentListStore"));
	m_ComponentsListStore          = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ComponentsListStore"));
	m_MetaDataListStore            = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("MetaDataListStore"));
	m_UnitAngleAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("UnitAngleAdjustment"));
	m_UnitForceAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("UnitForceAdjustment"));
	m_UnitLengthAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("UnitLengthAdjustment"));
	m_UnitMassAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("UnitMassAdjustment"));
	m_WorldInfoInfoTextBuffer      = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("WorldInfoInfoTextBuffer"));
	m_WorldInfoInfoTitleTextBuffer = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("WorldInfoInfoTitleTextBuffer"));
	m_ComponentsTeeSelection       = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("ComponentsTeeSelection"));
	m_ComponentsColumn             = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ComponentsColumn"));
	m_ComponentCellRenderer        = Glib::RefPtr <Gtk::CellRendererCombo>::cast_dynamic (m_builder -> get_object ("ComponentCellRenderer"));
	m_MetaDataTreeSelection        = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("MetaDataTreeSelection"));
	m_MetaDataNameColumn           = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("MetaDataNameColumn"));
	m_CellRendererMetaDataName     = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("CellRendererMetaDataName"));
	m_MetaDataContentColumn        = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("MetaDataContentColumn"));
	m_CellRendererMetaDataContent  = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("CellRendererMetaDataContent"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("Notebook", m_Notebook);
	m_builder -> get_widget ("ProfileComponentsBox", m_ProfileComponentsBox);
	m_builder -> get_widget ("ProfileExpander", m_ProfileExpander);
	m_builder -> get_widget ("ProfileButton", m_ProfileButton);
	m_builder -> get_widget ("ComponentsExpander", m_ComponentsExpander);
	m_builder -> get_widget ("ComponentsTreeView", m_ComponentsTreeView);
	m_builder -> get_widget ("ComponentsAddButton", m_ComponentsAddButton);
	m_builder -> get_widget ("ComponentsRemoveButton", m_ComponentsRemoveButton);
	m_builder -> get_widget ("InferProfileAndComponentsButton", m_InferProfileAndComponentsButton);
	m_builder -> get_widget ("UnitsExpander", m_UnitsExpander);
	m_builder -> get_widget ("UnitMassCombo", m_UnitMassCombo);
	m_builder -> get_widget ("UnitMassEntry", m_UnitMassEntry);
	m_builder -> get_widget ("UnitLengthCombo", m_UnitLengthCombo);
	m_builder -> get_widget ("UnitLengthEntry", m_UnitLengthEntry);
	m_builder -> get_widget ("UnitForceCombo", m_UnitForceCombo);
	m_builder -> get_widget ("UnitForceEntry", m_UnitForceEntry);
	m_builder -> get_widget ("UnitAngleCombo", m_UnitAngleCombo);
	m_builder -> get_widget ("UnitAngleEntry", m_UnitAngleEntry);
	m_builder -> get_widget ("MetaDataExpander", m_MetaDataExpander);
	m_builder -> get_widget ("MetaDataTreeView", m_MetaDataTreeView);
	m_builder -> get_widget ("UserDefinedFieldsActionBox", m_UserDefinedFieldsActionBox);
	m_builder -> get_widget ("AddMetaDataButton", m_AddMetaDataButton);
	m_builder -> get_widget ("RemoveMetaDataButton", m_RemoveMetaDataButton);
	m_builder -> get_widget ("AddStandardMetaDataButton", m_AddStandardMetaDataButton);
	m_builder -> get_widget ("WorldInfoBox", m_WorldInfoBox);
	m_builder -> get_widget ("WorldInfoExpander", m_WorldInfoExpander);
	m_builder -> get_widget ("WorldInfoTitleTextView", m_WorldInfoTitleTextView);
	m_builder -> get_widget ("WorldInfoInfoTextView", m_WorldInfoInfoTextView);
	m_builder -> get_widget ("MetaDataDialog", m_MetaDataDialog);
	m_builder -> get_widget ("MetaDataCancelButton", m_MetaDataCancelButton);
	m_builder -> get_widget ("MetaDataOkButton", m_MetaDataOkButton);
	m_builder -> get_widget ("MetaDataNameEntry", m_MetaDataNameEntry);
	m_builder -> get_widget ("MetaDataContentEntry", m_MetaDataContentEntry);

	// Connect object Gtk::Adjustment with id 'UnitAngleAdjustment'.
	m_UnitAngleAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_angle_changed));
	m_UnitForceAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_force_changed));
	m_UnitLengthAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_length_changed));
	m_UnitMassAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_mass_changed));

	// Connect object Gtk::Box with id 'ProfileComponentsBox'.
	m_ProfileComponentsBox -> signal_map () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_profile_components_map));
	m_ProfileComponentsBox -> signal_unmap () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_profile_components_unmap));

	// Connect object Gtk::ComboBoxText with id 'ProfileButton'.
	m_ProfileButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_profile_changed));

	// Connect object Gtk::TreeSelection with id 'ComponentsTeeSelection'.
	m_ComponentsTeeSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_components_selection_changed));

	// Connect object Gtk::CellRendererCombo with id 'ComponentCellRenderer'.
	m_ComponentCellRenderer -> signal_edited () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_component_edited));

	// Connect object Gtk::Button with id 'ComponentsAddButton'.
	m_ComponentsAddButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_add_component_clicked));
	m_ComponentsRemoveButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_remove_component_clicked));

	// Connect object Gtk::CheckButton with id 'InferProfileAndComponentsButton'.
	m_InferProfileAndComponentsButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_infer_profile_and_components_toggled));

	// Connect object Gtk::Entry with id 'UnitMassEntry'.
	m_UnitMassEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_mass_changed));
	m_UnitMassEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_mass_delete_text), false);
	m_UnitMassEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_mass_insert_text), false);
	m_UnitLengthEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_length_changed));
	m_UnitLengthEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_length_delete_text), false);
	m_UnitLengthEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_length_insert_text), false);
	m_UnitForceEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_force_changed));
	m_UnitForceEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_force_delete_text), false);
	m_UnitForceEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_force_insert_text), false);
	m_UnitAngleEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_angle_changed));
	m_UnitAngleEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_angle_delete_text), false);
	m_UnitAngleEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_unit_angle_insert_text), false);

	// Connect object Gtk::TreeView with id 'MetaDataTreeView'.
	m_MetaDataTreeView -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_meta_data_focus_in_event));
	m_MetaDataTreeView -> signal_focus_out_event () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_meta_data_focus_out_event));
	m_MetaDataTreeView -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_meta_data_key_press_event), false);

	// Connect object Gtk::TreeSelection with id 'MetaDataTreeSelection'.
	m_MetaDataTreeSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_meta_data_changed));

	// Connect object Gtk::CellRendererText with id 'CellRendererMetaDataName'.
	m_CellRendererMetaDataName -> signal_edited () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_meta_data_name_edited));
	m_CellRendererMetaDataContent -> signal_edited () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_meta_data_content_edited));

	// Connect object Gtk::Button with id 'AddMetaDataButton'.
	m_AddMetaDataButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_add_meta_data_clicked));
	m_RemoveMetaDataButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_remove_meta_data_clicked));

	// Connect object Gtk::Box with id 'WorldInfoBox'.
	m_WorldInfoBox -> signal_map () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_world_info_map));
	m_WorldInfoBox -> signal_unmap () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_world_info_unmap));

	// Connect object Gtk::Button with id 'MetaDataCancelButton'.
	m_MetaDataCancelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_add_meta_data_cancel_clicked));
	m_MetaDataOkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_add_meta_data_ok_clicked));

	// Connect object Gtk::Entry with id 'MetaDataNameEntry'.
	m_MetaDataNameEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DScenePropertiesEditorInterface::on_meta_data_name_changed));
}

X3DScenePropertiesEditorInterface::~X3DScenePropertiesEditorInterface ()
{
	delete m_Window;
	delete m_MetaDataDialog;
}

} // puck
} // titania
