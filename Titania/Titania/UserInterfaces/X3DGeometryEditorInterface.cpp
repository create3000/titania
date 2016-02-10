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
#include "X3DGeometryEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DGeometryEditorInterface::m_widgetName = "GeometryEditor";

void
X3DGeometryEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("GeometryEditorBox", m_GeometryEditorBox);
	m_builder -> get_widget ("HammerButton", m_HammerButton);
	m_builder -> get_widget ("EditToggleButton", m_EditToggleButton);
	m_builder -> get_widget ("PaintSelectionToggleButton", m_PaintSelectionToggleButton);
	m_builder -> get_widget ("PaintSelectionImage", m_PaintSelectionImage);
	m_builder -> get_widget ("NormalEnabledToggleButton", m_NormalEnabledToggleButton);
	m_builder -> get_widget ("SelectionTypeMenu", m_SelectionTypeMenu);
	m_builder -> get_widget ("BrushMenuItem", m_BrushMenuItem);
	m_builder -> get_widget ("RectangleMenuItem", m_RectangleMenuItem);
	m_builder -> get_widget ("LassoMenuItem", m_LassoMenuItem);

	// Connect object Gtk::Revealer with id 'Widget'.
	m_connections .emplace_back (m_Widget -> signal_map () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_map)));
	m_connections .emplace_back (m_Widget -> signal_unmap () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_unmap)));

	// Connect object Gtk::Button with id 'HammerButton'.
	m_connections .emplace_back (m_HammerButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_hammer_clicked)));

	// Connect object Gtk::ToggleButton with id 'EditToggleButton'.
	m_connections .emplace_back (m_EditToggleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_edit_toggled)));
	m_connections .emplace_back (m_PaintSelectionToggleButton -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_selection_type_button_press_event)));
	m_connections .emplace_back (m_PaintSelectionToggleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_paint_selection_toggled)));

	// Connect object Gtk::ImageMenuItem with id 'BrushMenuItem'.
	m_connections .emplace_back (m_BrushMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_brush_activated)));
	m_connections .emplace_back (m_RectangleMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_rectangle_activated)));
	m_connections .emplace_back (m_LassoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DGeometryEditorInterface::on_lasso_activated)));

	// Call construct handler of base class.
	construct ();
}

X3DGeometryEditorInterface::~X3DGeometryEditorInterface ()
{
	for (auto & connection : m_connections)
		connection .disconnect ();

	delete m_Window;
}

} // puck
} // titania
