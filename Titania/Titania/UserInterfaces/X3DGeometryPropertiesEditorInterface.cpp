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
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("GeometryChildNotebook", m_GeometryChildNotebook);
	m_builder -> get_widget ("GeometryExpander", m_GeometryExpander);
	m_builder -> get_widget ("SolidCheckButton", m_SolidCheckButton);
	m_builder -> get_widget ("CCWCheckButton", m_CCWCheckButton);
	m_builder -> get_widget ("ConvexCheckButton", m_ConvexCheckButton);
	m_builder -> get_widget ("CreaseAngleBox", m_CreaseAngleBox);
	m_builder -> get_widget ("CreaseAngleScaleSpinButton", m_CreaseAngleScaleSpinButton);
	m_builder -> get_widget ("CreaseAngleScale", m_CreaseAngleScale);
	m_builder -> get_widget ("NormalsBox", m_NormalsBox);
	m_builder -> get_widget ("AddNormalsButton", m_AddNormalsButton);
	m_builder -> get_widget ("RemoveNormalsButton", m_RemoveNormalsButton);
	m_builder -> get_widget ("PrimitiveCountEventBox", m_PrimitiveCountEventBox);
	m_builder -> get_widget ("PrimitiveCountExpander", m_PrimitiveCountExpander);
	m_builder -> get_widget ("PrimitiveCountBox", m_PrimitiveCountBox);
	m_builder -> get_widget ("PrimitiveCountPointsLabel", m_PrimitiveCountPointsLabel);
	m_builder -> get_widget ("PrimitiveCountLinesLabel", m_PrimitiveCountLinesLabel);
	m_builder -> get_widget ("PrimitiveCountTrianglesLabel", m_PrimitiveCountTrianglesLabel);
	m_builder -> get_widget ("PrimitiveCountCountButton", m_PrimitiveCountCountButton);
	m_AddNormalsButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_add_normals_clicked));
	m_RemoveNormalsButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_remove_normals_clicked));

	// Connect object Gtk::EventBox with id 'PrimitiveCountEventBox'.
	m_PrimitiveCountEventBox -> signal_enter_notify_event () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_crossing_notify_event));
	m_PrimitiveCountEventBox -> signal_leave_notify_event () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_crossing_notify_event));

	// Connect object Gtk::Box with id 'PrimitiveCountBox'.
	m_PrimitiveCountBox -> signal_map () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_map_primitive_count));
	m_PrimitiveCountBox -> signal_unmap () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_unmap_primitive_count));

	// Connect object Gtk::ComboBoxText with id 'PrimitiveCountCountButton'.
	m_PrimitiveCountCountButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_primitive_count_count_changed));

	// Call construct handler of base class.
	construct ();
}

X3DGeometryPropertiesEditorInterface::~X3DGeometryPropertiesEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
