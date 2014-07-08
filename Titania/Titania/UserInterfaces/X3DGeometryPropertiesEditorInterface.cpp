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
#include "X3DGeometryPropertiesEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DGeometryPropertiesEditorInterface::m_widgetName = "GeometryPropertiesEditor";

void
X3DGeometryPropertiesEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_CreaseAngleAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CreaseAngleAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("SolidCheckButton", m_SolidCheckButton);
	m_SolidCheckButton -> set_name ("SolidCheckButton");
	m_builder -> get_widget ("CCWCheckButton", m_CCWCheckButton);
	m_CCWCheckButton -> set_name ("CCWCheckButton");
	m_builder -> get_widget ("CreaseAngleBox", m_CreaseAngleBox);
	m_CreaseAngleBox -> set_name ("CreaseAngleBox");
	m_builder -> get_widget ("CreaseAngleScale", m_CreaseAngleScale);
	m_CreaseAngleScale -> set_name ("CreaseAngleScale");
	m_builder -> get_widget ("ConvexCheckButton", m_ConvexCheckButton);
	m_ConvexCheckButton -> set_name ("ConvexCheckButton");
	m_builder -> get_widget ("TextureCoordinateGeneratorExpander", m_TextureCoordinateGeneratorExpander);
	m_TextureCoordinateGeneratorExpander -> set_name ("TextureCoordinateGeneratorExpander");
	m_builder -> get_widget ("TextureCoordinateGeneratorBox", m_TextureCoordinateGeneratorBox);
	m_TextureCoordinateGeneratorBox -> set_name ("TextureCoordinateGeneratorBox");
	m_builder -> get_widget ("TextureCoordinateGeneratorCheckButton", m_TextureCoordinateGeneratorCheckButton);
	m_TextureCoordinateGeneratorCheckButton -> set_name ("TextureCoordinateGeneratorCheckButton");
	m_builder -> get_widget ("TextureCoordinateGeneratorModeButton", m_TextureCoordinateGeneratorModeButton);
	m_TextureCoordinateGeneratorModeButton -> set_name ("TextureCoordinateGeneratorModeButton");

	// Connect object Gtk::CheckButton with id 'SolidCheckButton'.
	m_SolidCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_solid_toggled));
	m_CCWCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_ccw_toggled));

	// Connect object Gtk::Scale with id 'CreaseAngleScale'.
	m_CreaseAngleScale -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_creaseAngle_changed));

	// Connect object Gtk::CheckButton with id 'ConvexCheckButton'.
	m_ConvexCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_convex_toggled));
	m_TextureCoordinateGeneratorCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_textureCoordinateGenerator_toggled));

	// Connect object Gtk::ComboBoxText with id 'TextureCoordinateGeneratorModeButton'.
	m_TextureCoordinateGeneratorModeButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_textureCoordinateGenerator_mode_changed));

	// Call construct handler of base class.
	construct ();
}

X3DGeometryPropertiesEditorInterface::~X3DGeometryPropertiesEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
