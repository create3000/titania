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

#include "X3DAppearanceEditorInterface.h"

namespace titania {
namespace puck {

void
X3DAppearanceEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DAppearanceEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DAppearanceEditorInterface::create ()
{
	// Get objects.
	m_AmbientIntensityAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AmbientIntensityAdjustment"));
	m_BackAmbientIntensityAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackAmbientIntensityAdjustment"));
	m_BackDiffuseColorAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackDiffuseColorAdjustment"));
	m_BackEmissiveColorAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackEmissiveColorAdjustment"));
	m_BackShininessAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackShininessAdjustment"));
	m_BackSpecularColorAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackSpecularColorAdjustment"));
	m_BackTransparencyAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackTransparencyAdjustment"));
	m_BlendModeBlendColorAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BlendModeBlendColorAdjustment"));
	m_DiffuseColorAdjustment                         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DiffuseColorAdjustment"));
	m_EmissiveColorAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EmissiveColorAdjustment"));
	m_FillPropertiesHatchColorAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FillPropertiesHatchColorAdjustment"));
	m_FillPropertiesHatchStyleAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FillPropertiesHatchStyleAdjustment"));
	m_LinePropertiesLinetypeAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LinePropertiesLinetypeAdjustment"));
	m_LinePropertiesLinewidthScaleFactorAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LinePropertiesLinewidthScaleFactorAdjustment"));
	m_PointPropertiesPointSizeAttenuation0Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointPropertiesPointSizeAttenuation0Adjustment"));
	m_PointPropertiesPointSizeAttenuation1Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointPropertiesPointSizeAttenuation1Adjustment"));
	m_PointPropertiesPointSizeAttenuation2Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointPropertiesPointSizeAttenuation2Adjustment"));
	m_PointPropertiesPointSizeMaxValueAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointPropertiesPointSizeMaxValueAdjustment"));
	m_PointPropertiesPointSizeMinValueAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointPropertiesPointSizeMinValueAdjustment"));
	m_PointPropertiesPointSizeScaleFactorAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointPropertiesPointSizeScaleFactorAdjustment"));
	m_ShininessAdjustment                            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShininessAdjustment"));
	m_SpecularColorAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpecularColorAdjustment"));
	m_TransparencyAdjustment                         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransparencyAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("CopyMenuItem", m_CopyMenuItem);
	m_builder -> get_widget ("PasteMenuItem", m_PasteMenuItem);
	m_builder -> get_widget ("Paned", m_Paned);
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_builder -> get_widget ("CopyButton", m_CopyButton);
	m_builder -> get_widget ("PasteButton", m_PasteButton);
	m_builder -> get_widget ("SphereButton", m_SphereButton);
	m_builder -> get_widget ("ModelButton", m_ModelButton);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("SelectAppearanceBox", m_SelectAppearanceBox);
	m_builder -> get_widget ("AppearanceCheckButton", m_AppearanceCheckButton);
	m_builder -> get_widget ("AppearanceUnlinkButton", m_AppearanceUnlinkButton);
	m_builder -> get_widget ("AppearanceChildNotebook", m_AppearanceChildNotebook);
	m_builder -> get_widget ("MaterialBox", m_MaterialBox);
	m_builder -> get_widget ("SelectMaterialBox", m_SelectMaterialBox);
	m_builder -> get_widget ("MaterialComboBoxText", m_MaterialComboBoxText);
	m_builder -> get_widget ("MaterialUnlinkButton", m_MaterialUnlinkButton);
	m_builder -> get_widget ("MaterialNameGrid", m_MaterialNameGrid);
	m_builder -> get_widget ("MaterialNameBox", m_MaterialNameBox);
	m_builder -> get_widget ("MaterialNameEntry", m_MaterialNameEntry);
	m_builder -> get_widget ("MaterialRenameButton", m_MaterialRenameButton);
	m_builder -> get_widget ("FrontExpander", m_FrontExpander);
	m_builder -> get_widget ("FrontBox", m_FrontBox);
	m_builder -> get_widget ("AmbientIntensityBox", m_AmbientIntensityBox);
	m_builder -> get_widget ("AmbientIntensityScale", m_AmbientIntensityScale);
	m_builder -> get_widget ("ShininessBox", m_ShininessBox);
	m_builder -> get_widget ("ShininessScale", m_ShininessScale);
	m_builder -> get_widget ("TransparencyBox", m_TransparencyBox);
	m_builder -> get_widget ("TransparencyScale", m_TransparencyScale);
	m_builder -> get_widget ("DiffuseColorBox", m_DiffuseColorBox);
	m_builder -> get_widget ("DiffuseColorButton", m_DiffuseColorButton);
	m_builder -> get_widget ("SpecularColorBox", m_SpecularColorBox);
	m_builder -> get_widget ("SpecularColorButton", m_SpecularColorButton);
	m_builder -> get_widget ("EmissiveColorBox", m_EmissiveColorBox);
	m_builder -> get_widget ("EmissiveColorButton", m_EmissiveColorButton);
	m_builder -> get_widget ("BackExpander", m_BackExpander);
	m_builder -> get_widget ("BackBox", m_BackBox);
	m_builder -> get_widget ("SeparateBackColorCheckButton", m_SeparateBackColorCheckButton);
	m_builder -> get_widget ("BackAmbientIntensityBox", m_BackAmbientIntensityBox);
	m_builder -> get_widget ("BackAmbientIntensityScale", m_BackAmbientIntensityScale);
	m_builder -> get_widget ("BackShininessBox", m_BackShininessBox);
	m_builder -> get_widget ("BackShininessScale", m_BackShininessScale);
	m_builder -> get_widget ("BackTransparencyBox", m_BackTransparencyBox);
	m_builder -> get_widget ("BackTransparencyScale", m_BackTransparencyScale);
	m_builder -> get_widget ("BackDiffuseColorBox", m_BackDiffuseColorBox);
	m_builder -> get_widget ("BackDiffuseColorButton", m_BackDiffuseColorButton);
	m_builder -> get_widget ("BackSpecularColorBox", m_BackSpecularColorBox);
	m_builder -> get_widget ("BackSpecularColorButton", m_BackSpecularColorButton);
	m_builder -> get_widget ("BackEmissiveColorBox", m_BackEmissiveColorBox);
	m_builder -> get_widget ("BackEmissiveColorButton", m_BackEmissiveColorButton);
	m_builder -> get_widget ("PointPropertiesExpander", m_PointPropertiesExpander);
	m_builder -> get_widget ("PointPropertiesMainBox", m_PointPropertiesMainBox);
	m_builder -> get_widget ("PointFillPropertiesBox", m_PointFillPropertiesBox);
	m_builder -> get_widget ("PointPropertiesCheckButton", m_PointPropertiesCheckButton);
	m_builder -> get_widget ("PointPropertiesUnlinkButton", m_PointPropertiesUnlinkButton);
	m_builder -> get_widget ("PointPropertiesBox", m_PointPropertiesBox);
	m_builder -> get_widget ("PointPropertiesNameBox", m_PointPropertiesNameBox);
	m_builder -> get_widget ("PointPropertiesNameEntry", m_PointPropertiesNameEntry);
	m_builder -> get_widget ("PointPropertiesRenameButton", m_PointPropertiesRenameButton);
	m_builder -> get_widget ("PointPropertiesPointSizeScaleFactorSpinButton", m_PointPropertiesPointSizeScaleFactorSpinButton);
	m_builder -> get_widget ("PointPropertiesPointSizeMinValueSpinButton", m_PointPropertiesPointSizeMinValueSpinButton);
	m_builder -> get_widget ("PointPropertiesPointSizeMaxValueSpinButton", m_PointPropertiesPointSizeMaxValueSpinButton);
	m_builder -> get_widget ("PointPropertiesPointSizeAttenuationBox", m_PointPropertiesPointSizeAttenuationBox);
	m_builder -> get_widget ("PointPropertiesPointSizeAttenuation0SpinButton", m_PointPropertiesPointSizeAttenuation0SpinButton);
	m_builder -> get_widget ("PointPropertiesPointSizeAttenuation1SpinButton", m_PointPropertiesPointSizeAttenuation1SpinButton);
	m_builder -> get_widget ("PointPropertiesPointSizeAttenuation2SpinButton", m_PointPropertiesPointSizeAttenuation2SpinButton);
	m_builder -> get_widget ("PointPropertiesColorModeComboBoxText", m_PointPropertiesColorModeComboBoxText);
	m_builder -> get_widget ("LinePropertiesExpander", m_LinePropertiesExpander);
	m_builder -> get_widget ("LinePropertiesMainBox", m_LinePropertiesMainBox);
	m_builder -> get_widget ("LinePropertiesCheckButton", m_LinePropertiesCheckButton);
	m_builder -> get_widget ("LinePropertiesUnlinkButton", m_LinePropertiesUnlinkButton);
	m_builder -> get_widget ("LinePropertiesBox", m_LinePropertiesBox);
	m_builder -> get_widget ("LinePropertiesFilledCheckButton", m_LinePropertiesFilledCheckButton);
	m_builder -> get_widget ("LinePropertiesLinetypeSpinButton", m_LinePropertiesLinetypeSpinButton);
	m_builder -> get_widget ("LinePropertiesLinewidthScaleFactorSpinButton", m_LinePropertiesLinewidthScaleFactorSpinButton);
	m_builder -> get_widget ("LinePropertiesNameBox", m_LinePropertiesNameBox);
	m_builder -> get_widget ("LinePropertiesNameEntry", m_LinePropertiesNameEntry);
	m_builder -> get_widget ("LinePropertiesRenameButton", m_LinePropertiesRenameButton);
	m_builder -> get_widget ("FillPropertiesExpander", m_FillPropertiesExpander);
	m_builder -> get_widget ("FillPropertiesMainBox", m_FillPropertiesMainBox);
	m_builder -> get_widget ("SelectFillPropertiesBox", m_SelectFillPropertiesBox);
	m_builder -> get_widget ("FillPropertiesCheckButton", m_FillPropertiesCheckButton);
	m_builder -> get_widget ("FillPropertiesUnlinkButton", m_FillPropertiesUnlinkButton);
	m_builder -> get_widget ("FillPropertiesBox", m_FillPropertiesBox);
	m_builder -> get_widget ("FillPropertiesFilledCheckButton", m_FillPropertiesFilledCheckButton);
	m_builder -> get_widget ("FillPropertiesHatchedCheckButton", m_FillPropertiesHatchedCheckButton);
	m_builder -> get_widget ("FillPropertiesHatchColorBox", m_FillPropertiesHatchColorBox);
	m_builder -> get_widget ("FillPropertiesHatchColorButton", m_FillPropertiesHatchColorButton);
	m_builder -> get_widget ("FillPropertiesHatchStyleSpinButton", m_FillPropertiesHatchStyleSpinButton);
	m_builder -> get_widget ("FillPropertiesNameBox", m_FillPropertiesNameBox);
	m_builder -> get_widget ("FillPropertiesNameEntry", m_FillPropertiesNameEntry);
	m_builder -> get_widget ("FillPropertiesRenameButton", m_FillPropertiesRenameButton);
	m_builder -> get_widget ("BlendModeExpander", m_BlendModeExpander);
	m_builder -> get_widget ("BlendModeMainBox", m_BlendModeMainBox);
	m_builder -> get_widget ("SelectBlendModeBox", m_SelectBlendModeBox);
	m_builder -> get_widget ("BlendModeCheckButton", m_BlendModeCheckButton);
	m_builder -> get_widget ("BlendModeUnlinkButton", m_BlendModeUnlinkButton);
	m_builder -> get_widget ("BlendModeBox", m_BlendModeBox);
	m_builder -> get_widget ("BlendModeBlendColorBox", m_BlendModeBlendColorBox);
	m_builder -> get_widget ("BlendModeBlendColorButton", m_BlendModeBlendColorButton);
	m_builder -> get_widget ("BlendModeBlendColorScale", m_BlendModeBlendColorScale);
	m_builder -> get_widget ("BlendModeNameBox", m_BlendModeNameBox);
	m_builder -> get_widget ("BlendModeNameEntry", m_BlendModeNameEntry);
	m_builder -> get_widget ("BlendModeRenameButton", m_BlendModeRenameButton);
	m_builder -> get_widget ("BlendModeSourceColorFactorButton", m_BlendModeSourceColorFactorButton);
	m_builder -> get_widget ("BlendModeSourceAlphaFactorButton", m_BlendModeSourceAlphaFactorButton);
	m_builder -> get_widget ("BlendModeDestinationColorFactorButton", m_BlendModeDestinationColorFactorButton);
	m_builder -> get_widget ("BlendModeDestinationAlphaFactorButton", m_BlendModeDestinationAlphaFactorButton);
	m_builder -> get_widget ("BlendModeColorEquationButton", m_BlendModeColorEquationButton);
	m_builder -> get_widget ("BlendModeAlphaEquationButton", m_BlendModeAlphaEquationButton);
	m_builder -> get_widget ("BlendModeRedLabel", m_BlendModeRedLabel);
	m_builder -> get_widget ("BlendModeGreenLabel", m_BlendModeGreenLabel);
	m_builder -> get_widget ("BlendModeBlueLabel", m_BlendModeBlueLabel);
	m_builder -> get_widget ("BlendModeAlphaLabel", m_BlendModeAlphaLabel);
	m_builder -> get_widget ("UsedMaterialsBox", m_UsedMaterialsBox);
	m_builder -> get_widget ("PaletteBox", m_PaletteBox);
	m_builder -> get_widget ("PaletteFaceCombo", m_PaletteFaceCombo);
	m_builder -> get_widget ("PalettePreviewBox", m_PalettePreviewBox);
	m_builder -> get_widget ("ChangePaletteBox", m_ChangePaletteBox);
	m_builder -> get_widget ("PaletteComboBoxText", m_PaletteComboBoxText);
	m_builder -> get_widget ("PalettePreviousButton", m_PalettePreviousButton);
	m_builder -> get_widget ("PaletteNextButton", m_PaletteNextButton);
	m_builder -> get_widget ("EditPaletteDialog", m_EditPaletteDialog);
	m_builder -> get_widget ("EditPaletteCancelButton", m_EditPaletteCancelButton);
	m_builder -> get_widget ("EditPaletteOkButton", m_EditPaletteOkButton);
	m_builder -> get_widget ("PaletteNameEntry", m_PaletteNameEntry);
	m_builder -> get_widget ("PaletteMenu", m_PaletteMenu);
	m_builder -> get_widget ("AddPaletteMenuItem", m_AddPaletteMenuItem);
	m_builder -> get_widget ("RemovePaletteMenuItem", m_RemovePaletteMenuItem);
	m_builder -> get_widget ("EditPaletteMenuItem", m_EditPaletteMenuItem);
	m_builder -> get_widget ("AddObjectToPaletteMenuItem", m_AddObjectToPaletteMenuItem);
	m_builder -> get_widget ("UpdateObjectInPaletteMenuItem", m_UpdateObjectInPaletteMenuItem);
	m_builder -> get_widget ("RemoveObjectFromPaletteMenuItem", m_RemoveObjectFromPaletteMenuItem);
	m_builder -> get_widget ("ShowDefaultPalettesMenuItem", m_ShowDefaultPalettesMenuItem);

	// Connect object Gtk::ImageMenuItem with id 'CopyMenuItem'.
	m_CopyMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_copy));
	m_PasteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_paste));

	// Connect object Gtk::Button with id 'CopyButton'.
	m_CopyButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_copy));
	m_PasteButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_paste));
	m_SphereButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_sphere_clicked));
	m_ModelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_model_clicked));

	// Connect object Gtk::CheckButton with id 'AppearanceCheckButton'.
	m_AppearanceCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_appearance_toggled));

	// Connect object Gtk::Button with id 'AppearanceUnlinkButton'.
	m_AppearanceUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_appearance_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'MaterialComboBoxText'.
	m_MaterialComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_material_changed));

	// Connect object Gtk::Button with id 'MaterialUnlinkButton'.
	m_MaterialUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_material_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'PointPropertiesCheckButton'.
	m_PointPropertiesCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_pointProperties_toggled));

	// Connect object Gtk::Button with id 'PointPropertiesUnlinkButton'.
	m_PointPropertiesUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_pointProperties_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'LinePropertiesCheckButton'.
	m_LinePropertiesCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_lineProperties_toggled));

	// Connect object Gtk::Button with id 'LinePropertiesUnlinkButton'.
	m_LinePropertiesUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_lineProperties_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'FillPropertiesCheckButton'.
	m_FillPropertiesCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_fillProperties_toggled));

	// Connect object Gtk::Button with id 'FillPropertiesUnlinkButton'.
	m_FillPropertiesUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_fillProperties_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'BlendModeCheckButton'.
	m_BlendModeCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_blendMode_toggled));

	// Connect object Gtk::Button with id 'BlendModeUnlinkButton'.
	m_BlendModeUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_blendMode_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'PaletteFaceCombo'.
	m_PaletteFaceCombo -> signal_changed () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_palette_face_changed));

	// Connect object Gtk::Box with id 'PalettePreviewBox'.
	m_PalettePreviewBox -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_palette_button_press_event));

	// Connect object Gtk::ComboBoxText with id 'PaletteComboBoxText'.
	m_PaletteComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_palette_changed));

	// Connect object Gtk::Button with id 'PalettePreviousButton'.
	m_PalettePreviousButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_palette_previous_clicked));
	m_PaletteNextButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_palette_next_clicked));
	m_EditPaletteCancelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_edit_palette_cancel_clicked));
	m_EditPaletteOkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_edit_palette_ok_clicked));

	// Connect object Gtk::Entry with id 'PaletteNameEntry'.
	m_PaletteNameEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_palette_name_changed));
	m_PaletteNameEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_palette_name_delete_text), false);
	m_PaletteNameEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_palette_name_insert_text), false);

	// Connect object Gtk::ImageMenuItem with id 'AddPaletteMenuItem'.
	m_AddPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_add_palette_activate));
	m_RemovePaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_remove_palette_activate));
	m_EditPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_edit_palette_activate));
	m_AddObjectToPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_add_object_to_palette_activate));
	m_UpdateObjectInPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_update_object_in_palette_activate));
	m_RemoveObjectFromPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_remove_object_from_palette_activate));

	// Connect object Gtk::CheckMenuItem with id 'ShowDefaultPalettesMenuItem'.
	m_ShowDefaultPalettesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DAppearanceEditorInterface::on_show_default_palettes_toggled));
}

X3DAppearanceEditorInterface::~X3DAppearanceEditorInterface ()
{
	delete m_Window;
	delete m_EditPaletteDialog;
}

} // puck
} // titania
