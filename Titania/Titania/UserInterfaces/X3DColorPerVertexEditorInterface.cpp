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
#include "X3DColorPerVertexEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DColorPerVertexEditorInterface::m_widgetName = "ColorPerVertexEditor";

void
X3DColorPerVertexEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_AmbientIntensityAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AmbientIntensityAdjustment"));
	m_BackAmbientIntensityAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackAmbientIntensityAdjustment"));
	m_BackDiffuseColorAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackDiffuseColorAdjustment"));
	m_BackEmissiveColorAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackEmissiveColorAdjustment"));
	m_BackShininessAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackShininessAdjustment"));
	m_BackSpecularColorAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackSpecularColorAdjustment"));
	m_BackTransparencyAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackTransparencyAdjustment"));
	m_ColorAdjustment                              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorAdjustment"));
	m_DiffuseColorAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DiffuseColorAdjustment"));
	m_EmissiveColorAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EmissiveColorAdjustment"));
	m_FillPropertiesHatchColorAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FillPropertiesHatchColorAdjustment"));
	m_FillPropertiesHatchStyleAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FillPropertiesHatchStyleAdjustment"));
	m_LinePropertiesLinetypeAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LinePropertiesLinetypeAdjustment"));
	m_LinePropertiesLinewidthScaleFactorAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LinePropertiesLinewidthScaleFactorAdjustment"));
	m_ShininessAdjustment                          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShininessAdjustment"));
	m_SpecularColorAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpecularColorAdjustment"));
	m_TransparencyAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransparencyAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_builder -> get_widget ("LookAtButton", m_LookAtButton);
	m_builder -> get_widget ("ColorBox", m_ColorBox);
	m_builder -> get_widget ("ColorButton", m_ColorButton);
	m_builder -> get_widget ("AddColorButton", m_AddColorButton);
	m_builder -> get_widget ("ColorsScrolledWindow", m_ColorsScrolledWindow);

	// Connect object Gtk::ToolButton with id 'LookAtButton'.
	m_LookAtButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorPerVertexEditorInterface::on_look_at_all_clicked));

	// Connect object Gtk::Button with id 'AddColorButton'.
	m_AddColorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorPerVertexEditorInterface::on_add_color_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DColorPerVertexEditorInterface::~X3DColorPerVertexEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
