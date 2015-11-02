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
#include "X3DTextureEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DTextureEditorInterface::m_widgetName = "TextureEditor";

void
X3DTextureEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_TexturePropertiesAnisotropicDegreeAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesAnisotropicDegreeAdjustment"));
	m_TexturePropertiesBorderColorAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesBorderColorAdjustment"));
	m_TexturePropertiesBorderWidthAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesBorderWidthAdjustment"));
	m_TexturePropertiesTexturePriorityAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesTexturePriorityAdjustment"));
	m_TextureTransformCenterXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformCenterXAdjustment"));
	m_TextureTransformCenterYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformCenterYAdjustment"));
	m_TextureTransformRotationAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformRotationAdjustment"));
	m_TextureTransformScaleXAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformScaleXAdjustment"));
	m_TextureTransformScaleYAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformScaleYAdjustment"));
	m_TextureTransformTranslationXAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformTranslationXAdjustment"));
	m_TextureTransformTranslationYAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformTranslationYAdjustment"));
	m_ImageTextureURLCellRendererText              = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("ImageTextureURLCellRendererText"));
	m_ImageTextureURLChooserColumn                 = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ImageTextureURLChooserColumn"));
	m_ImageTextureURLCellrendererPixbuf            = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("ImageTextureURLCellrendererPixbuf"));
	m_MovieTextureURLCellRendererText              = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("MovieTextureURLCellRendererText"));
	m_MovieTextureURLChooserColumn                 = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("MovieTextureURLChooserColumn"));
	m_MovieTextureURLCellrendererPixbuf            = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("MovieTextureURLCellrendererPixbuf"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("TextureChildNotebook", m_TextureChildNotebook);
	m_builder -> get_widget ("TextureExpander", m_TextureExpander);
	m_builder -> get_widget ("TextureBox", m_TextureBox);
	m_builder -> get_widget ("SelectTextureBox", m_SelectTextureBox);
	m_builder -> get_widget ("TextureComboBoxText", m_TextureComboBoxText);
	m_builder -> get_widget ("TextureUnlinkButton", m_TextureUnlinkButton);
	m_builder -> get_widget ("TextureFormatLabel", m_TextureFormatLabel);
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_builder -> get_widget ("TextureNotebook", m_TextureNotebook);
	m_builder -> get_widget ("TextureLabel", m_TextureLabel);
	m_builder -> get_widget ("ImageTextureBox", m_ImageTextureBox);
	m_builder -> get_widget ("ImageTextureURLBox", m_ImageTextureURLBox);
	m_builder -> get_widget ("ImageTextureURLTreeView", m_ImageTextureURLTreeView);
	m_builder -> get_widget ("ImageTextureURLAddButton", m_ImageTextureURLAddButton);
	m_builder -> get_widget ("ImageTextureURLRemoveButton", m_ImageTextureURLRemoveButton);
	m_builder -> get_widget ("ImageTextureEmbedButton", m_ImageTextureEmbedButton);
	m_builder -> get_widget ("ImageTextureURLReloadButton", m_ImageTextureURLReloadButton);
	m_builder -> get_widget ("PixelTextureBox", m_PixelTextureBox);
	m_builder -> get_widget ("MovieTextureBox", m_MovieTextureBox);
	m_builder -> get_widget ("MovieTextureURLBox", m_MovieTextureURLBox);
	m_builder -> get_widget ("MovieTextureURLTreeView", m_MovieTextureURLTreeView);
	m_builder -> get_widget ("MovieTextureURLAddButton", m_MovieTextureURLAddButton);
	m_builder -> get_widget ("MovieTextureURLRemoveButton", m_MovieTextureURLRemoveButton);
	m_builder -> get_widget ("MovieTextureURLReloadButton", m_MovieTextureURLReloadButton);
	m_builder -> get_widget ("Texture2DBox", m_Texture2DBox);
	m_builder -> get_widget ("Texture2DNodeRepeatSCheckButton", m_Texture2DNodeRepeatSCheckButton);
	m_builder -> get_widget ("Texture2DNodeRepeatTCheckButton", m_Texture2DNodeRepeatTCheckButton);
	m_builder -> get_widget ("TexturePropertiesExpander", m_TexturePropertiesExpander);
	m_builder -> get_widget ("SelectTexturePropertiesBox", m_SelectTexturePropertiesBox);
	m_builder -> get_widget ("TexturePropertiesCheckButton", m_TexturePropertiesCheckButton);
	m_builder -> get_widget ("TexturePropertiesUnlinkButton", m_TexturePropertiesUnlinkButton);
	m_builder -> get_widget ("TexturePropertiesBox", m_TexturePropertiesBox);
	m_builder -> get_widget ("TexturePropertiesGenerateMipMapsCheckButton", m_TexturePropertiesGenerateMipMapsCheckButton);
	m_builder -> get_widget ("TexturePropertiesBorderWidthSpinButton", m_TexturePropertiesBorderWidthSpinButton);
	m_builder -> get_widget ("TexturePropertiesAnisotropicDegreeSpinButton", m_TexturePropertiesAnisotropicDegreeSpinButton);
	m_builder -> get_widget ("TexturePropertiesMinificationFilterComboBoxText", m_TexturePropertiesMinificationFilterComboBoxText);
	m_builder -> get_widget ("TexturePropertiesMagnificationFilterComboBoxText", m_TexturePropertiesMagnificationFilterComboBoxText);
	m_builder -> get_widget ("TexturePropertiesBoundaryModeSComboBoxText", m_TexturePropertiesBoundaryModeSComboBoxText);
	m_builder -> get_widget ("TexturePropertiesTextureCompressionComboBoxText", m_TexturePropertiesTextureCompressionComboBoxText);
	m_builder -> get_widget ("TexturePropertiesTexturePrioritySpinButton", m_TexturePropertiesTexturePrioritySpinButton);
	m_builder -> get_widget ("TexturePropertiesBoundaryModeTComboBoxText", m_TexturePropertiesBoundaryModeTComboBoxText);
	m_builder -> get_widget ("TexturePropertiesBoundaryModeRComboBoxText", m_TexturePropertiesBoundaryModeRComboBoxText);
	m_builder -> get_widget ("TexturePropertiesBorderColorBox", m_TexturePropertiesBorderColorBox);
	m_builder -> get_widget ("TexturePropertiesBorderColorButton", m_TexturePropertiesBorderColorButton);
	m_builder -> get_widget ("TexturePropertiesBorderColorScale", m_TexturePropertiesBorderColorScale);
	m_builder -> get_widget ("TextureTransformExpander", m_TextureTransformExpander);
	m_builder -> get_widget ("TextureTransformNodeBox", m_TextureTransformNodeBox);
	m_builder -> get_widget ("SelectTextureTransBox", m_SelectTextureTransBox);
	m_builder -> get_widget ("TextureTransformComboBoxText", m_TextureTransformComboBoxText);
	m_builder -> get_widget ("TextureTransformUnlinkButton", m_TextureTransformUnlinkButton);
	m_builder -> get_widget ("TextureTransformNotebook", m_TextureTransformNotebook);
	m_builder -> get_widget ("TextureTransformLabel", m_TextureTransformLabel);
	m_builder -> get_widget ("TextureTransformBox", m_TextureTransformBox);
	m_builder -> get_widget ("TextureTransformTranslationBox", m_TextureTransformTranslationBox);
	m_builder -> get_widget ("TextureTransformTranslationXSpinButton", m_TextureTransformTranslationXSpinButton);
	m_builder -> get_widget ("TextureTransformTranslationYSpinButton", m_TextureTransformTranslationYSpinButton);
	m_builder -> get_widget ("TextureTransformScaleBox", m_TextureTransformScaleBox);
	m_builder -> get_widget ("TextureTransformScaleXSpinButton", m_TextureTransformScaleXSpinButton);
	m_builder -> get_widget ("TextureTransformScaleYSpinButton", m_TextureTransformScaleYSpinButton);
	m_builder -> get_widget ("TextureTransformUniformScaleButton", m_TextureTransformUniformScaleButton);
	m_builder -> get_widget ("TextureTransformUniformScaleImage", m_TextureTransformUniformScaleImage);
	m_builder -> get_widget ("TextureTransformCenterBox", m_TextureTransformCenterBox);
	m_builder -> get_widget ("TextureTransformCenterXSpinButton", m_TextureTransformCenterXSpinButton);
	m_builder -> get_widget ("TextureTransformCenterYSpinButton", m_TextureTransformCenterYSpinButton);
	m_builder -> get_widget ("TextureTransformRotationSpinButton", m_TextureTransformRotationSpinButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorExpander", m_TextureCoordinateGeneratorExpander);
	m_builder -> get_widget ("TextureCoordinateGeneratorMainBox", m_TextureCoordinateGeneratorMainBox);
	m_builder -> get_widget ("SelectTextureCoordinateGeneratorBox", m_SelectTextureCoordinateGeneratorBox);
	m_builder -> get_widget ("TextureCoordinateGeneratorCheckButton", m_TextureCoordinateGeneratorCheckButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorUnlinkButton", m_TextureCoordinateGeneratorUnlinkButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorBox", m_TextureCoordinateGeneratorBox);
	m_builder -> get_widget ("TextureCoordinateGeneratorModeComboBoxText", m_TextureCoordinateGeneratorModeComboBoxText);
	m_builder -> get_widget ("PaletteBox", m_PaletteBox);
	m_builder -> get_widget ("PalettePreviewBox", m_PalettePreviewBox);
	m_builder -> get_widget ("PaletteComboBoxText", m_PaletteComboBoxText);
	m_builder -> get_widget ("PalettePreviousButton", m_PalettePreviousButton);
	m_builder -> get_widget ("PaletteNextButton", m_PaletteNextButton);

	// Connect object Gtk::ComboBoxText with id 'TextureComboBoxText'.
	m_TextureComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_texture_changed));

	// Connect object Gtk::Button with id 'TextureUnlinkButton'.
	m_TextureUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_texture_unlink_clicked));
	m_ImageTextureEmbedButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_embed_image_clicked));

	// Connect object Gtk::CheckButton with id 'TexturePropertiesCheckButton'.
	m_TexturePropertiesCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureProperties_toggled));

	// Connect object Gtk::Button with id 'TexturePropertiesUnlinkButton'.
	m_TexturePropertiesUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureProperties_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'TextureTransformComboBoxText'.
	m_TextureTransformComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_changed));

	// Connect object Gtk::Button with id 'TextureTransformUnlinkButton'.
	m_TextureTransformUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_unlink_clicked));

	// Connect object Gtk::ToggleButton with id 'TextureTransformUniformScaleButton'.
	m_TextureTransformUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_texture_transform_uniform_scale_clicked));

	// Connect object Gtk::CheckButton with id 'TextureCoordinateGeneratorCheckButton'.
	m_TextureCoordinateGeneratorCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureCoordinateGenerator_toggled));

	// Connect object Gtk::Button with id 'TextureCoordinateGeneratorUnlinkButton'.
	m_TextureCoordinateGeneratorUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureCoordinateGenerator_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'PaletteComboBoxText'.
	m_PaletteComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_palette_changed));

	// Connect object Gtk::Button with id 'PalettePreviousButton'.
	m_PalettePreviousButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_palette_previous_clicked));
	m_PaletteNextButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_palette_next_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DTextureEditorInterface::~X3DTextureEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
