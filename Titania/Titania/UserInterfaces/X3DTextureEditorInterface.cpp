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
	m_TextureTransformCenterXAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformCenterXAdjustment"));
	m_TextureTransformCenterYAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformCenterYAdjustment"));
	m_TextureTransformRotationAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformRotationAdjustment"));
	m_TextureTransformScaleXAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformScaleXAdjustment"));
	m_TextureTransformScaleYAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformScaleYAdjustment"));
	m_TextureTransformTranslationXAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformTranslationXAdjustment"));
	m_TextureTransformTranslationYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformTranslationYAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("TextureExpander", m_TextureExpander);
	m_TextureExpander -> set_name ("TextureExpander");
	m_builder -> get_widget ("TextureButton", m_TextureButton);
	m_TextureButton -> set_name ("TextureButton");
	m_builder -> get_widget ("TextureNotebook", m_TextureNotebook);
	m_TextureNotebook -> set_name ("TextureNotebook");
	m_builder -> get_widget ("RepeatSCheckbutton", m_RepeatSCheckbutton);
	m_RepeatSCheckbutton -> set_name ("RepeatSCheckbutton");
	m_builder -> get_widget ("RepeatTCheckButton", m_RepeatTCheckButton);
	m_RepeatTCheckButton -> set_name ("RepeatTCheckButton");
	m_builder -> get_widget ("TextureTransformExpander", m_TextureTransformExpander);
	m_TextureTransformExpander -> set_name ("TextureTransformExpander");
	m_builder -> get_widget ("TextureTransformButton", m_TextureTransformButton);
	m_TextureTransformButton -> set_name ("TextureTransformButton");
	m_builder -> get_widget ("TextureTransformNotebook", m_TextureTransformNotebook);
	m_TextureTransformNotebook -> set_name ("TextureTransformNotebook");
	m_builder -> get_widget ("TextureTransformLabel", m_TextureTransformLabel);
	m_TextureTransformLabel -> set_name ("TextureTransformLabel");
	m_builder -> get_widget ("TextureTransformBox", m_TextureTransformBox);
	m_TextureTransformBox -> set_name ("TextureTransformBox");
	m_builder -> get_widget ("TextureTransformTranslationXSpinButton", m_TextureTransformTranslationXSpinButton);
	m_TextureTransformTranslationXSpinButton -> set_name ("TextureTransformTranslationXSpinButton");
	m_builder -> get_widget ("TextureTransformTranslationYSpinButton", m_TextureTransformTranslationYSpinButton);
	m_TextureTransformTranslationYSpinButton -> set_name ("TextureTransformTranslationYSpinButton");
	m_builder -> get_widget ("TextureTransformScaleXSpinButton", m_TextureTransformScaleXSpinButton);
	m_TextureTransformScaleXSpinButton -> set_name ("TextureTransformScaleXSpinButton");
	m_builder -> get_widget ("TextureTransformScaleYSpinButton", m_TextureTransformScaleYSpinButton);
	m_TextureTransformScaleYSpinButton -> set_name ("TextureTransformScaleYSpinButton");
	m_builder -> get_widget ("TextureTransformCenterXSpinButton", m_TextureTransformCenterXSpinButton);
	m_TextureTransformCenterXSpinButton -> set_name ("TextureTransformCenterXSpinButton");
	m_builder -> get_widget ("TextureTransformCenterYSpinButton", m_TextureTransformCenterYSpinButton);
	m_TextureTransformCenterYSpinButton -> set_name ("TextureTransformCenterYSpinButton");
	m_builder -> get_widget ("TextureTransformRotationSpinButton", m_TextureTransformRotationSpinButton);
	m_TextureTransformRotationSpinButton -> set_name ("TextureTransformRotationSpinButton");

	// Connect object Gtk::Adjustment with id 'TextureTransformCenterXAdjustment'.
	m_TextureTransformCenterXAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_center_changed));
	m_TextureTransformCenterYAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_center_changed));
	m_TextureTransformRotationAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_rotation_changed));
	m_TextureTransformScaleXAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_scale_changed));
	m_TextureTransformScaleYAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_scale_changed));
	m_TextureTransformTranslationXAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_translation_changed));
	m_TextureTransformTranslationYAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_translation_changed));

	// Connect object Gtk::ComboBoxText with id 'TextureTransformButton'.
	m_TextureTransformButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextureEditorInterface::on_textureTransform_changed));

	// Call construct handler of base class.
	construct ();
}

X3DTextureEditorInterface::~X3DTextureEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
