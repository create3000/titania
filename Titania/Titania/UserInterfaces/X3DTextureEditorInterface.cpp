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
	m_TexturePropertiesBorderWidthAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesBorderWidthAdjustment"));
	m_TexturePropertiesTexturePriorityAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesTexturePriorityAdjustment"));
	m_TextureTransformCenterXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformCenterXAdjustment"));
	m_TextureTransformCenterYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformCenterYAdjustment"));
	m_TextureTransformRotationAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformRotationAdjustment"));
	m_TextureTransformScaleXAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformScaleXAdjustment"));
	m_TextureTransformScaleYAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformScaleYAdjustment"));
	m_TextureTransformTranslationXAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformTranslationXAdjustment"));
	m_TextureTransformTranslationYAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformTranslationYAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("TextureExpander", m_TextureExpander);
	m_TextureExpander -> set_name ("TextureExpander");
	m_builder -> get_widget ("TextureButton", m_TextureButton);
	m_TextureButton -> set_name ("TextureButton");
	m_builder -> get_widget ("TextureUnlinkButton", m_TextureUnlinkButton);
	m_TextureUnlinkButton -> set_name ("TextureUnlinkButton");
	m_builder -> get_widget ("TextureNotebook", m_TextureNotebook);
	m_TextureNotebook -> set_name ("TextureNotebook");
	m_builder -> get_widget ("TextureLabel", m_TextureLabel);
	m_TextureLabel -> set_name ("TextureLabel");
	m_builder -> get_widget ("ImageTextureBox", m_ImageTextureBox);
	m_ImageTextureBox -> set_name ("ImageTextureBox");
	m_builder -> get_widget ("Texture2DBox", m_Texture2DBox);
	m_Texture2DBox -> set_name ("Texture2DBox");
	m_builder -> get_widget ("Texture2DNodeRepeatSCheckButton", m_Texture2DNodeRepeatSCheckButton);
	m_Texture2DNodeRepeatSCheckButton -> set_name ("Texture2DNodeRepeatSCheckButton");
	m_builder -> get_widget ("Texture2DNodeRepeatTCheckButton", m_Texture2DNodeRepeatTCheckButton);
	m_Texture2DNodeRepeatTCheckButton -> set_name ("Texture2DNodeRepeatTCheckButton");
	m_builder -> get_widget ("TexturePropertiesExpander", m_TexturePropertiesExpander);
	m_TexturePropertiesExpander -> set_name ("TexturePropertiesExpander");
	m_builder -> get_widget ("TexturePropertiesCheckButton", m_TexturePropertiesCheckButton);
	m_TexturePropertiesCheckButton -> set_name ("TexturePropertiesCheckButton");
	m_builder -> get_widget ("TexturePropertiesUnlinkButton", m_TexturePropertiesUnlinkButton);
	m_TexturePropertiesUnlinkButton -> set_name ("TexturePropertiesUnlinkButton");
	m_builder -> get_widget ("TexturePropertiesBox", m_TexturePropertiesBox);
	m_TexturePropertiesBox -> set_name ("TexturePropertiesBox");
	m_builder -> get_widget ("GenerateMipMapsCheckButton", m_GenerateMipMapsCheckButton);
	m_GenerateMipMapsCheckButton -> set_name ("GenerateMipMapsCheckButton");
	m_builder -> get_widget ("TextureTransformExpander", m_TextureTransformExpander);
	m_TextureTransformExpander -> set_name ("TextureTransformExpander");
	m_builder -> get_widget ("TextureTransformButton", m_TextureTransformButton);
	m_TextureTransformButton -> set_name ("TextureTransformButton");
	m_builder -> get_widget ("TextureTransformUnlinkButton", m_TextureTransformUnlinkButton);
	m_TextureTransformUnlinkButton -> set_name ("TextureTransformUnlinkButton");
	m_builder -> get_widget ("TextureTransformNotebook", m_TextureTransformNotebook);
	m_TextureTransformNotebook -> set_name ("TextureTransformNotebook");
	m_builder -> get_widget ("TextureTransformLabel", m_TextureTransformLabel);
	m_TextureTransformLabel -> set_name ("TextureTransformLabel");
	m_builder -> get_widget ("TextureTransformBox", m_TextureTransformBox);
	m_TextureTransformBox -> set_name ("TextureTransformBox");
	m_builder -> get_widget ("TextureTransformTranslationBox", m_TextureTransformTranslationBox);
	m_TextureTransformTranslationBox -> set_name ("TextureTransformTranslationBox");
	m_builder -> get_widget ("TextureTransformTranslationXSpinButton", m_TextureTransformTranslationXSpinButton);
	m_TextureTransformTranslationXSpinButton -> set_name ("TextureTransformTranslationXSpinButton");
	m_builder -> get_widget ("TextureTransformTranslationYSpinButton", m_TextureTransformTranslationYSpinButton);
	m_TextureTransformTranslationYSpinButton -> set_name ("TextureTransformTranslationYSpinButton");
	m_builder -> get_widget ("TextureTransformScaleBox", m_TextureTransformScaleBox);
	m_TextureTransformScaleBox -> set_name ("TextureTransformScaleBox");
	m_builder -> get_widget ("TextureTransformScaleXSpinButton", m_TextureTransformScaleXSpinButton);
	m_TextureTransformScaleXSpinButton -> set_name ("TextureTransformScaleXSpinButton");
	m_builder -> get_widget ("TextureTransformScaleYSpinButton", m_TextureTransformScaleYSpinButton);
	m_TextureTransformScaleYSpinButton -> set_name ("TextureTransformScaleYSpinButton");
	m_builder -> get_widget ("TextureTransformCenterBox", m_TextureTransformCenterBox);
	m_TextureTransformCenterBox -> set_name ("TextureTransformCenterBox");
	m_builder -> get_widget ("TextureTransformCenterXSpinButton", m_TextureTransformCenterXSpinButton);
	m_TextureTransformCenterXSpinButton -> set_name ("TextureTransformCenterXSpinButton");
	m_builder -> get_widget ("TextureTransformCenterYSpinButton", m_TextureTransformCenterYSpinButton);
	m_TextureTransformCenterYSpinButton -> set_name ("TextureTransformCenterYSpinButton");
	m_builder -> get_widget ("TextureTransformRotationSpinButton", m_TextureTransformRotationSpinButton);
	m_TextureTransformRotationSpinButton -> set_name ("TextureTransformRotationSpinButton");
	m_builder -> get_widget ("TextureCoordinateGeneratorExpander", m_TextureCoordinateGeneratorExpander);
	m_TextureCoordinateGeneratorExpander -> set_name ("TextureCoordinateGeneratorExpander");
	m_builder -> get_widget ("TextureCoordinateGeneratorMainBox", m_TextureCoordinateGeneratorMainBox);
	m_TextureCoordinateGeneratorMainBox -> set_name ("TextureCoordinateGeneratorMainBox");
	m_builder -> get_widget ("TextureCoordinateGeneratorCheckButton", m_TextureCoordinateGeneratorCheckButton);
	m_TextureCoordinateGeneratorCheckButton -> set_name ("TextureCoordinateGeneratorCheckButton");
	m_builder -> get_widget ("TextureCoordinateGeneratorUnlinkButton", m_TextureCoordinateGeneratorUnlinkButton);
	m_TextureCoordinateGeneratorUnlinkButton -> set_name ("TextureCoordinateGeneratorUnlinkButton");
	m_builder -> get_widget ("TextureCoordinateGeneratorBox", m_TextureCoordinateGeneratorBox);
	m_TextureCoordinateGeneratorBox -> set_name ("TextureCoordinateGeneratorBox");
	m_builder -> get_widget ("TextureCoordinateGeneratorModeButton", m_TextureCoordinateGeneratorModeButton);
	m_TextureCoordinateGeneratorModeButton -> set_name ("TextureCoordinateGeneratorModeButton");

	// Connect object Gtk::ComboBoxText with id 'TextureButton'.
	m_TextureButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_texture_changed));

	// Connect object Gtk::Button with id 'TextureUnlinkButton'.
	m_TextureUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_texture_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'Texture2DNodeRepeatSCheckButton'.
	m_Texture2DNodeRepeatSCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_texture2DNode_repeatS_toggled));
	m_Texture2DNodeRepeatTCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_texture2DNode_repeatT_toggled));
	m_TexturePropertiesCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureProperties_toggled));

	// Connect object Gtk::Button with id 'TexturePropertiesUnlinkButton'.
	m_TexturePropertiesUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureProperties_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'TextureTransformButton'.
	m_TextureTransformButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_changed));

	// Connect object Gtk::Button with id 'TextureTransformUnlinkButton'.
	m_TextureTransformUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'TextureCoordinateGeneratorCheckButton'.
	m_TextureCoordinateGeneratorCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureCoordinateGenerator_toggled));

	// Connect object Gtk::Button with id 'TextureCoordinateGeneratorUnlinkButton'.
	m_TextureCoordinateGeneratorUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureCoordinateGenerator_unlink_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DTextureEditorInterface::~X3DTextureEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
