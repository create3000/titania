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
#include "X3DMaterialEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DMaterialEditorInterface::m_widgetName = "MaterialEditor";

void
X3DMaterialEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_ambientAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AmbientAdjustment"));
	m_backAmbientAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackAmbientAdjustment"));
	m_backShininessAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackShininessAdjustment"));
	m_backTransparencyAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackTransparencyAdjustment"));
	m_shininessAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShininessAdjustment"));
	m_transparencyAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransparencyAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("BackDiffuseDialog", m_backDiffuseDialog);
	m_backDiffuseDialog -> set_name ("BackDiffuseDialog");
	m_builder -> get_widget ("BackEmissiveDialog", m_backEmissiveDialog);
	m_backEmissiveDialog -> set_name ("BackEmissiveDialog");
	m_builder -> get_widget ("BackSpecularDialog", m_backSpecularDialog);
	m_backSpecularDialog -> set_name ("BackSpecularDialog");
	m_builder -> get_widget ("DiffuseDialog", m_diffuseDialog);
	m_diffuseDialog -> set_name ("DiffuseDialog");
	m_builder -> get_widget ("EmissiveDialog", m_emissiveDialog);
	m_emissiveDialog -> set_name ("EmissiveDialog");
	m_builder -> get_widget ("SpecularDialog", m_specularDialog);
	m_specularDialog -> set_name ("SpecularDialog");
	m_builder -> get_widget ("Window", m_window);
	m_window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_widget);
	m_widget -> set_name ("Widget");
	m_builder -> get_widget ("EditMenuItem", m_editMenuItem);
	m_editMenuItem -> set_name ("EditMenuItem");
	m_builder -> get_widget ("CopyMenuItem", m_copyMenuItem);
	m_copyMenuItem -> set_name ("CopyMenuItem");
	m_builder -> get_widget ("PasteMenuItem", m_pasteMenuItem);
	m_pasteMenuItem -> set_name ("PasteMenuItem");
	m_builder -> get_widget ("PreviewBox", m_previewBox);
	m_previewBox -> set_name ("PreviewBox");
	m_builder -> get_widget ("FrontAndBackButton", m_frontAndBackButton);
	m_frontAndBackButton -> set_name ("FrontAndBackButton");
	m_builder -> get_widget ("FrontExpander", m_frontExpander);
	m_frontExpander -> set_name ("FrontExpander");
	m_builder -> get_widget ("AmbientScale", m_ambientScale);
	m_ambientScale -> set_name ("AmbientScale");
	m_builder -> get_widget ("ShininessScale", m_shininessScale);
	m_shininessScale -> set_name ("ShininessScale");
	m_builder -> get_widget ("TransparencyScale", m_transparencyScale);
	m_transparencyScale -> set_name ("TransparencyScale");
	m_builder -> get_widget ("DiffuseButton", m_diffuseButton);
	m_diffuseButton -> set_name ("DiffuseButton");
	m_builder -> get_widget ("DiffuseArea", m_diffuseArea);
	m_diffuseArea -> set_name ("DiffuseArea");
	m_builder -> get_widget ("SpecularButton", m_specularButton);
	m_specularButton -> set_name ("SpecularButton");
	m_builder -> get_widget ("SpecularArea", m_specularArea);
	m_specularArea -> set_name ("SpecularArea");
	m_builder -> get_widget ("EmissiveButton", m_emissiveButton);
	m_emissiveButton -> set_name ("EmissiveButton");
	m_builder -> get_widget ("EmissiveArea", m_emissiveArea);
	m_emissiveArea -> set_name ("EmissiveArea");
	m_builder -> get_widget ("BackExpander", m_backExpander);
	m_backExpander -> set_name ("BackExpander");
	m_builder -> get_widget ("BackAmbientScale", m_backAmbientScale);
	m_backAmbientScale -> set_name ("BackAmbientScale");
	m_builder -> get_widget ("BackShininessScale", m_backShininessScale);
	m_backShininessScale -> set_name ("BackShininessScale");
	m_builder -> get_widget ("BackTransparencyScale", m_backTransparencyScale);
	m_backTransparencyScale -> set_name ("BackTransparencyScale");
	m_builder -> get_widget ("BackDiffuseButton", m_backDiffuseButton);
	m_backDiffuseButton -> set_name ("BackDiffuseButton");
	m_builder -> get_widget ("BackDiffuseArea", m_backDiffuseArea);
	m_backDiffuseArea -> set_name ("BackDiffuseArea");
	m_builder -> get_widget ("BackSpecularButton", m_backSpecularButton);
	m_backSpecularButton -> set_name ("BackSpecularButton");
	m_builder -> get_widget ("BackSpecularArea", m_backSpecularArea);
	m_backSpecularArea -> set_name ("BackSpecularArea");
	m_builder -> get_widget ("BackEmissiveButton", m_backEmissiveButton);
	m_backEmissiveButton -> set_name ("BackEmissiveButton");
	m_builder -> get_widget ("BackEmissiveArea", m_backEmissiveArea);
	m_backEmissiveArea -> set_name ("BackEmissiveArea");

	// Connect object Gtk::ImageMenuItem with id 'CopyMenuItem'.
	m_copyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_copy));
	m_pasteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_paste));

	// Connect object Gtk::ToggleButton with id 'FrontAndBackButton'.
	m_frontAndBackButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_frontAndBackButton_toggled));

	// Connect object Gtk::Scale with id 'AmbientScale'.
	m_ambientScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_ambient));
	m_shininessScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_shininess));
	m_transparencyScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_transparency));

	// Connect object Gtk::Button with id 'DiffuseButton'.
	m_diffuseButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_diffuse_clicked));

	// Connect object Gtk::DrawingArea with id 'DiffuseArea'.
	m_diffuseArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_diffuse_draw));

	// Connect object Gtk::Button with id 'SpecularButton'.
	m_specularButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_specular_clicked));

	// Connect object Gtk::DrawingArea with id 'SpecularArea'.
	m_specularArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_specular_draw));

	// Connect object Gtk::Button with id 'EmissiveButton'.
	m_emissiveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_emissive_clicked));

	// Connect object Gtk::DrawingArea with id 'EmissiveArea'.
	m_emissiveArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_emissive_draw));

	// Connect object Gtk::Scale with id 'BackAmbientScale'.
	m_backAmbientScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backAmbient));
	m_backShininessScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backShininess));
	m_backTransparencyScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backTransparency));

	// Connect object Gtk::Button with id 'BackDiffuseButton'.
	m_backDiffuseButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backDiffuse_clicked));

	// Connect object Gtk::DrawingArea with id 'BackDiffuseArea'.
	m_backDiffuseArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backDiffuse_draw));

	// Connect object Gtk::Button with id 'BackSpecularButton'.
	m_backSpecularButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backSpecular_clicked));

	// Connect object Gtk::DrawingArea with id 'BackSpecularArea'.
	m_backSpecularArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backSpecular_draw));

	// Connect object Gtk::Button with id 'BackEmissiveButton'.
	m_backEmissiveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backEmissive_clicked));

	// Connect object Gtk::DrawingArea with id 'BackEmissiveArea'.
	m_backEmissiveArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backEmissive_draw));

	// Call construct handler of base class.
	construct ();
}

X3DMaterialEditorInterface::~X3DMaterialEditorInterface ()
{
	delete m_backDiffuseDialog;
	delete m_emissiveDialog;
	delete m_specularDialog;
	delete m_backEmissiveDialog;
	delete m_backSpecularDialog;
	delete m_window;
	delete m_diffuseDialog;
}

} // puck
} // titania
