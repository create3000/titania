/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "X3DEnvironmentalSensorEditor.h"

#include "../../Browser/BrowserSelection.h"

#include <Titania/X3D/Components/EnvironmentalSensor/X3DEnvironmentalSensorNode.h>

namespace titania {
namespace puck {

X3DEnvironmentalSensorEditor::X3DEnvironmentalSensorEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                            enabled (this, getEnvironmentalSensorEnabledCheckButton (), "enabled"),
	                               size (this,
	                                     getEnvironmentalSensorSizeXAdjustment (),
	                                     getEnvironmentalSensorSizeYAdjustment (),
	                                     getEnvironmentalSensorSizeZAdjustment (),
	                                     getEnvironmentalSensorSizeBox (),
	                                     "size"),
	                             center (this,
	                                     getEnvironmentalSensorCenterXAdjustment (),
	                                     getEnvironmentalSensorCenterYAdjustment (),
	                                     getEnvironmentalSensorCenterZAdjustment (),
	                                     getEnvironmentalSensorCenterBox (),
	                                     "center")
{ }

void
X3DEnvironmentalSensorEditor::configure ()
{
	getEnvironmentalSensorUniformSizeButton () .set_active (getConfig () -> getItem <bool> ("environmentalSensorUniformSize"));
}

void
X3DEnvironmentalSensorEditor::set_selection (const X3D::MFNode & selection)
{
	const auto sensor  = X3D::X3DPtr <X3D::X3DEnvironmentalSensorNode> (selection .empty () ? nullptr : selection .back ());
	const auto sensors = sensor ? X3D::MFNode ({ sensor }) : X3D::MFNode ();

	getEnvironmentalSensorExpander () .set_visible (sensor);

	if (sensor)
		getEnvironmentalSensorExpander () .set_label (_ (refineName (sensor -> getTypeName ())));

	enabled .setNodes (sensors);
	size    .setNodes (sensors);
	center  .setNodes (sensors);
}

void
X3DEnvironmentalSensorEditor::on_proximity_sensor_uniform_size_clicked ()
{
	if (getEnvironmentalSensorUniformSizeButton () .get_active ())
	{
		getEnvironmentalSensorUniformSizeImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		size .setUniform (true);
	}
	else
	{
		getEnvironmentalSensorUniformSizeImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		size .setUniform (false);
	}
}

void
X3DEnvironmentalSensorEditor::store ()
{
	getConfig () -> setItem ("environmentalSensorUniformSize", getEnvironmentalSensorUniformSizeButton () .get_active ());
}

X3DEnvironmentalSensorEditor::~X3DEnvironmentalSensorEditor ()
{ }

} // puck
} // titania
