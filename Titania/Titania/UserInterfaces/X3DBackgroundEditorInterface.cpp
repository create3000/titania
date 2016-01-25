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
#include "X3DBackgroundEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DBackgroundEditorInterface::m_widgetName = "BackgroundEditor";

void
X3DBackgroundEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_GroundAngleAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GroundAngleAdjustment"));
	m_GroundColorAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GroundColorAdjustment"));
	m_SkyAngleAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SkyAngleAdjustment"));
	m_SkyColorAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SkyColorAdjustment"));
	m_TransparencyAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransparencyAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("ColorBox", m_ColorBox);
	m_builder -> get_widget ("ColorsExpander", m_ColorsExpander);
	m_builder -> get_widget ("SkyGradientBox", m_SkyGradientBox);
	m_builder -> get_widget ("SkyColorGrid", m_SkyColorGrid);
	m_builder -> get_widget ("SkyColorsScrolledWindow", m_SkyColorsScrolledWindow);
	m_builder -> get_widget ("AddSkyColorButton", m_AddSkyColorButton);
	m_builder -> get_widget ("RemoveSkyColorButton", m_RemoveSkyColorButton);
	m_builder -> get_widget ("SkyColorBox", m_SkyColorBox);
	m_builder -> get_widget ("SkyColorButton", m_SkyColorButton);
	m_builder -> get_widget ("GroundGradientBox", m_GroundGradientBox);
	m_builder -> get_widget ("GroundColorGrid", m_GroundColorGrid);
	m_builder -> get_widget ("GroundColorsScrolledWindow", m_GroundColorsScrolledWindow);
	m_builder -> get_widget ("AddGroundColorButton", m_AddGroundColorButton);
	m_builder -> get_widget ("RemoveGroundColorButton", m_RemoveGroundColorButton);
	m_builder -> get_widget ("GroundColorBox", m_GroundColorBox);
	m_builder -> get_widget ("GroundColorButton", m_GroundColorButton);
	m_builder -> get_widget ("TransparencyScale", m_TransparencyScale);

	// Call construct handler of base class.
	construct ();
}

X3DBackgroundEditorInterface::~X3DBackgroundEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
