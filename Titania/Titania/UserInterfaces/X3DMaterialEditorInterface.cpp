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
	m_AmbientAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AmbientAdjustment"));
	m_BackAmbientAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackAmbientAdjustment"));
	m_BackShininessAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackShininessAdjustment"));
	m_BackTransparencyAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackTransparencyAdjustment"));
	m_ShininessAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShininessAdjustment"));
	m_TransparencyAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransparencyAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("EditMenuItem", m_EditMenuItem);
	m_EditMenuItem -> set_name ("EditMenuItem");
	m_builder -> get_widget ("CopyMenuItem", m_CopyMenuItem);
	m_CopyMenuItem -> set_name ("CopyMenuItem");
	m_builder -> get_widget ("PasteMenuItem", m_PasteMenuItem);
	m_PasteMenuItem -> set_name ("PasteMenuItem");
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_PreviewBox -> set_name ("PreviewBox");
	m_builder -> get_widget ("MaterialBox", m_MaterialBox);
	m_MaterialBox -> set_name ("MaterialBox");
	m_builder -> get_widget ("FrontAndBackButton", m_FrontAndBackButton);
	m_FrontAndBackButton -> set_name ("FrontAndBackButton");
	m_builder -> get_widget ("FrontExpander", m_FrontExpander);
	m_FrontExpander -> set_name ("FrontExpander");
	m_builder -> get_widget ("AmbientScale", m_AmbientScale);
	m_AmbientScale -> set_name ("AmbientScale");
	m_builder -> get_widget ("ShininessScale", m_ShininessScale);
	m_ShininessScale -> set_name ("ShininessScale");
	m_builder -> get_widget ("TransparencyScale", m_TransparencyScale);
	m_TransparencyScale -> set_name ("TransparencyScale");
	m_builder -> get_widget ("DiffuseButton", m_DiffuseButton);
	m_DiffuseButton -> set_name ("DiffuseButton");
	m_builder -> get_widget ("DiffuseArea", m_DiffuseArea);
	m_DiffuseArea -> set_name ("DiffuseArea");
	m_builder -> get_widget ("SpecularButton", m_SpecularButton);
	m_SpecularButton -> set_name ("SpecularButton");
	m_builder -> get_widget ("SpecularArea", m_SpecularArea);
	m_SpecularArea -> set_name ("SpecularArea");
	m_builder -> get_widget ("EmissiveButton", m_EmissiveButton);
	m_EmissiveButton -> set_name ("EmissiveButton");
	m_builder -> get_widget ("EmissiveArea", m_EmissiveArea);
	m_EmissiveArea -> set_name ("EmissiveArea");
	m_builder -> get_widget ("BackExpander", m_BackExpander);
	m_BackExpander -> set_name ("BackExpander");
	m_builder -> get_widget ("BackAmbientScale", m_BackAmbientScale);
	m_BackAmbientScale -> set_name ("BackAmbientScale");
	m_builder -> get_widget ("BackShininessScale", m_BackShininessScale);
	m_BackShininessScale -> set_name ("BackShininessScale");
	m_builder -> get_widget ("BackTransparencyScale", m_BackTransparencyScale);
	m_BackTransparencyScale -> set_name ("BackTransparencyScale");
	m_builder -> get_widget ("BackDiffuseButton", m_BackDiffuseButton);
	m_BackDiffuseButton -> set_name ("BackDiffuseButton");
	m_builder -> get_widget ("BackDiffuseArea", m_BackDiffuseArea);
	m_BackDiffuseArea -> set_name ("BackDiffuseArea");
	m_builder -> get_widget ("BackSpecularButton", m_BackSpecularButton);
	m_BackSpecularButton -> set_name ("BackSpecularButton");
	m_builder -> get_widget ("BackSpecularArea", m_BackSpecularArea);
	m_BackSpecularArea -> set_name ("BackSpecularArea");
	m_builder -> get_widget ("BackEmissiveButton", m_BackEmissiveButton);
	m_BackEmissiveButton -> set_name ("BackEmissiveButton");
	m_builder -> get_widget ("BackEmissiveArea", m_BackEmissiveArea);
	m_BackEmissiveArea -> set_name ("BackEmissiveArea");
	m_builder -> get_widget ("BackDiffuseDialog", m_BackDiffuseDialog);
	m_BackDiffuseDialog -> set_name ("BackDiffuseDialog");
	m_builder -> get_widget ("BackEmissiveDialog", m_BackEmissiveDialog);
	m_BackEmissiveDialog -> set_name ("BackEmissiveDialog");
	m_builder -> get_widget ("BackSpecularDialog", m_BackSpecularDialog);
	m_BackSpecularDialog -> set_name ("BackSpecularDialog");
	m_builder -> get_widget ("DiffuseDialog", m_DiffuseDialog);
	m_DiffuseDialog -> set_name ("DiffuseDialog");
	m_builder -> get_widget ("EmissiveDialog", m_EmissiveDialog);
	m_EmissiveDialog -> set_name ("EmissiveDialog");
	m_builder -> get_widget ("SpecularDialog", m_SpecularDialog);
	m_SpecularDialog -> set_name ("SpecularDialog");

	// Connect object Gtk::ImageMenuItem with id 'CopyMenuItem'.
	m_CopyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_copy));
	m_PasteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_paste));

	// Connect object Gtk::ToggleButton with id 'FrontAndBackButton'.
	m_FrontAndBackButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_frontAndBackButton_toggled));

	// Connect object Gtk::Scale with id 'AmbientScale'.
	m_AmbientScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_ambient));
	m_ShininessScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_shininess));
	m_TransparencyScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_transparency));

	// Connect object Gtk::Button with id 'DiffuseButton'.
	m_DiffuseButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_diffuse_clicked));

	// Connect object Gtk::DrawingArea with id 'DiffuseArea'.
	m_DiffuseArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_diffuse_draw));

	// Connect object Gtk::Button with id 'SpecularButton'.
	m_SpecularButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_specular_clicked));

	// Connect object Gtk::DrawingArea with id 'SpecularArea'.
	m_SpecularArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_specular_draw));

	// Connect object Gtk::Button with id 'EmissiveButton'.
	m_EmissiveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_emissive_clicked));

	// Connect object Gtk::DrawingArea with id 'EmissiveArea'.
	m_EmissiveArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_emissive_draw));

	// Connect object Gtk::Scale with id 'BackAmbientScale'.
	m_BackAmbientScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backAmbient));
	m_BackShininessScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backShininess));
	m_BackTransparencyScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backTransparency));

	// Connect object Gtk::Button with id 'BackDiffuseButton'.
	m_BackDiffuseButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backDiffuse_clicked));

	// Connect object Gtk::DrawingArea with id 'BackDiffuseArea'.
	m_BackDiffuseArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backDiffuse_draw));

	// Connect object Gtk::Button with id 'BackSpecularButton'.
	m_BackSpecularButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backSpecular_clicked));

	// Connect object Gtk::DrawingArea with id 'BackSpecularArea'.
	m_BackSpecularArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backSpecular_draw));

	// Connect object Gtk::Button with id 'BackEmissiveButton'.
	m_BackEmissiveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backEmissive_clicked));

	// Connect object Gtk::DrawingArea with id 'BackEmissiveArea'.
	m_BackEmissiveArea -> signal_draw () .connect (sigc::mem_fun (*this, &X3DMaterialEditorInterface::on_backEmissive_draw));

	// Call construct handler of base class.
	construct ();
}

X3DMaterialEditorInterface::~X3DMaterialEditorInterface ()
{
	delete m_BackDiffuseDialog;
	delete m_BackEmissiveDialog;
	delete m_BackSpecularDialog;
	delete m_DiffuseDialog;
	delete m_EmissiveDialog;
	delete m_SpecularDialog;
	delete m_Window;
}

} // puck
} // titania
