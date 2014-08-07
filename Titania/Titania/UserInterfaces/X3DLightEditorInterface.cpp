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
#include "X3DLightEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DLightEditorInterface::m_widgetName = "LightEditor";

void
X3DLightEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_AmbientIntensityAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AmbientIntensityAdjustment"));
	m_ColorAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorAdjustment"));
	m_DirectionXAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DirectionXAdjustment"));
	m_DirectionYAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DirectionYAdjustment"));
	m_DirectionZAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DirectionZAdjustment"));
	m_IntensityAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IntensityAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("GeometryExpander", m_GeometryExpander);
	m_GeometryExpander -> set_name ("GeometryExpander");
	m_builder -> get_widget ("GlobalCheckButton", m_GlobalCheckButton);
	m_GlobalCheckButton -> set_name ("GlobalCheckButton");
	m_builder -> get_widget ("OnCheckButton", m_OnCheckButton);
	m_OnCheckButton -> set_name ("OnCheckButton");
	m_builder -> get_widget ("ColorBox", m_ColorBox);
	m_ColorBox -> set_name ("ColorBox");
	m_builder -> get_widget ("ColorButton", m_ColorButton);
	m_ColorButton -> set_name ("ColorButton");
	m_builder -> get_widget ("IntensityBox", m_IntensityBox);
	m_IntensityBox -> set_name ("IntensityBox");
	m_builder -> get_widget ("IntensityScale", m_IntensityScale);
	m_IntensityScale -> set_name ("IntensityScale");
	m_builder -> get_widget ("AmbientIntensityBox", m_AmbientIntensityBox);
	m_AmbientIntensityBox -> set_name ("AmbientIntensityBox");
	m_builder -> get_widget ("AmbientIntensityScale", m_AmbientIntensityScale);
	m_AmbientIntensityScale -> set_name ("AmbientIntensityScale");
	m_builder -> get_widget ("DirectionBox", m_DirectionBox);
	m_DirectionBox -> set_name ("DirectionBox");
	m_builder -> get_widget ("DirectionXSpinButton", m_DirectionXSpinButton);
	m_DirectionXSpinButton -> set_name ("DirectionXSpinButton");
	m_builder -> get_widget ("DirectionYSpinButton", m_DirectionYSpinButton);
	m_DirectionYSpinButton -> set_name ("DirectionYSpinButton");
	m_builder -> get_widget ("DirectionZSpinButton1", m_DirectionZSpinButton1);
	m_DirectionZSpinButton1 -> set_name ("DirectionZSpinButton1");

	// Call construct handler of base class.
	construct ();
}

X3DLightEditorInterface::~X3DLightEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
