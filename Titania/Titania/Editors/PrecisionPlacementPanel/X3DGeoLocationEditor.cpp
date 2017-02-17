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

#include "X3DGeoLocationEditor.h"

#include "../../ComposedWidgets/MFStringGeoSystem.h"

#include <Titania/X3D/Components/Geospatial/GeoLocation.h>

namespace titania {
namespace puck {

X3DGeoLocationEditor::X3DGeoLocationEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                          geoSystem (new MFStringGeoSystem (this,
	                                     getGeoLocationCoordinateSystemComboBoxText (),
	                                     getGeoLocationEllipsoidComboBoxText (),
	                                     getGeoLocationGDOrderComboBoxText (),
	                                     getGeoLocationZoneAdjustment (),
	                                     getGeoLocationHemisphereComboBoxText (),
	                                     getGeoLocationUTMOrderComboBoxText (),
	                                     getGeoLocationEllipsoidBox (),
	                                     getGeoLocationGDOrderComboBoxText (),
	                                     getGeoLocationGeoSystemUTMBox ())),
	                          geoCoords (this,
	                                     getGeoLocationGeoCoordsXAdjustment (),
	                                     getGeoLocationGeoCoordsYAdjustment (),
	                                     getGeoLocationGeoCoordsZAdjustment (),
	                                     getGeoLocationGeoCoordsBox (),
	                                     "geoCoords")
{
	getGeoLocationGeoCoordsXAdjustment () -> set_step_increment (1e-7);
	getGeoLocationGeoCoordsYAdjustment () -> set_step_increment (1e-7);
	getGeoLocationGeoCoordsZAdjustment () -> set_step_increment (1e-2);

	getGeoLocationGeoCoordsXAdjustment () -> set_page_increment (1e-4);
	getGeoLocationGeoCoordsYAdjustment () -> set_page_increment (1e-4);
	getGeoLocationGeoCoordsZAdjustment () -> set_page_increment (1);

	getGeoLocationGeoCoordsXSpinButton () .property_climb_rate () = 1e-6;
	getGeoLocationGeoCoordsYSpinButton () .property_climb_rate () = 1e-6;
	getGeoLocationGeoCoordsZSpinButton () .property_climb_rate () = 1e-1;
}

void
X3DGeoLocationEditor::initialize ()
{ }

void
X3DGeoLocationEditor::set_selection (const X3D::MFNode & selection)
{
	const auto geoLocation  = X3D::X3DPtr <X3D::GeoLocation> (selection .empty () ? nullptr : selection .back ());
	const auto geoLocations = geoLocation ? X3D::MFNode ({ geoLocation }) : X3D::MFNode ();

	getGeoLocationExpander () .set_visible (geoLocation);

	geoSystem -> setNodes (geoLocations);
	geoCoords .setNodes (geoLocations);
}

X3DGeoLocationEditor::~X3DGeoLocationEditor ()
{ }

} // puck
} // titania
