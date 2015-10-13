/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DGeoViewpointEditor.h"

#include "../../ComposedWidgets/MFStringGeoSystem.h"
#include "../../ComposedWidgets/RotationTool.h"

#include <Titania/X3D/Components/Geospatial/GeoViewpoint.h>

namespace titania {
namespace puck {

X3DGeoViewpointEditor::X3DGeoViewpointEditor () :
	X3DViewpointEditorInterface (),
	                  geoSystem (new MFStringGeoSystem (this,
	                             getGeoViewpointCoordinateSystemComboBoxText (),
	                             getGeoViewpointEllipsoidComboBoxText (),
	                             getGeoViewpointGDOrderComboBoxText (),
	                             getGeoViewpointZoneAdjustment (),
	                             getGeoViewpointHemisphereComboBoxText (),
	                             getGeoViewpointUTMOrderComboBoxText (),
	                             getGeoViewpointEllipsoidBox (),
	                             getGeoViewpointGDOrderComboBoxText (),
	                             getGeoViewpointGeoSystemUTMBox ())),
	                   position (this,
	                             getGeoViewpointPositionXAdjustment (),
	                             getGeoViewpointPositionYAdjustment (),
	                             getGeoViewpointPositionZAdjustment (),
	                             getGeoViewpointPositionBox (),
	                             "position"),
	                orientation (this,
	                             getGeoViewpointOrientationXAdjustment (),
	                             getGeoViewpointOrientationYAdjustment (),
	                             getGeoViewpointOrientationZAdjustment (),
	                             getGeoViewpointOrientationAAdjustment (),
	                             getGeoViewpointOrientationBox (),
	                             "orientation"),
	            orientationTool (new RotationTool (this, getGeoViewpointOrientationToolBox (), "orientation")),
	           centerOfRotation (this,
	                             getGeoViewpointCenterOfRotationXAdjustment (),
	                             getGeoViewpointCenterOfRotationYAdjustment (),
	                             getGeoViewpointCenterOfRotationZAdjustment (),
	                             getGeoViewpointCenterOfRotationBox (),
	                             "centerOfRotation"),
	                fieldOfView (this,
	                             getGeoViewpointFieldOfViewAdjustment (),
	                             getGeoViewpointFieldOfViewBox (),
	                             "fieldOfView"),
	                speedFactor (this,
	                             getGeoViewpointSpeedFactorAdjustment (),
	                             getGeoViewpointSpeedFactorSpinButton (),
	                             "speedFactor")
{
	getGeoViewpointPositionXAdjustment () -> set_step_increment (1e-7);
	getGeoViewpointPositionYAdjustment () -> set_step_increment (1e-7);
	getGeoViewpointPositionZAdjustment () -> set_step_increment (1e-2);

	getGeoViewpointPositionXAdjustment () -> set_page_increment (1e-4);
	getGeoViewpointPositionYAdjustment () -> set_page_increment (1e-4);
	getGeoViewpointPositionZAdjustment () -> set_page_increment (1);

	getGeoViewpointPositionXSpinButton () .property_climb_rate () = 1e-6;
	getGeoViewpointPositionYSpinButton () .property_climb_rate () = 1e-6;
	getGeoViewpointPositionZSpinButton () .property_climb_rate () = 1e-1;

	getGeoViewpointCenterOfRotationXAdjustment () -> set_step_increment (1e-7);
	getGeoViewpointCenterOfRotationYAdjustment () -> set_step_increment (1e-7);
	getGeoViewpointCenterOfRotationZAdjustment () -> set_step_increment (1e-2);

	getGeoViewpointCenterOfRotationXAdjustment () -> set_page_increment (1e-4);
	getGeoViewpointCenterOfRotationYAdjustment () -> set_page_increment (1e-4);
	getGeoViewpointCenterOfRotationZAdjustment () -> set_page_increment (1);

	getGeoViewpointCenterOfRotationXSpinButton () .property_climb_rate () = 1e-6;
	getGeoViewpointCenterOfRotationYSpinButton () .property_climb_rate () = 1e-6;
	getGeoViewpointCenterOfRotationZSpinButton () .property_climb_rate () = 1e-1;

	getGeoViewpointFieldOfViewAdjustment () -> set_upper (M_PI); // getExecutionContext () -> fromRadiant (M_PI);
}

void
X3DGeoViewpointEditor::setGeoViewpoint (const X3D::X3DPtr <X3D::X3DViewpointNode> & viewpointNode, const bool inScene)
{
	const X3D::X3DPtr <X3D::GeoViewpoint> geoViewpoint (viewpointNode);

	getGeoViewpointBox () .set_sensitive (inScene);
	getGeoViewpointExpander () .set_visible (geoViewpoint);

	const auto geoViewpoints = geoViewpoint ? X3D::MFNode ({ geoViewpoint }) : X3D::MFNode ();

	geoSystem -> setNode (X3D::SFNode (geoViewpoint));

	position         .setNodes (geoViewpoints);
	orientation      .setNodes (geoViewpoints);
	centerOfRotation .setNodes (geoViewpoints);
	fieldOfView      .setNodes (geoViewpoints);
	speedFactor      .setNodes (geoViewpoints);

	orientationTool -> setNodes (geoViewpoints);
}

void
X3DGeoViewpointEditor::on_new_geo_viewpoint_activated ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("Create New GeoViewpoint"));
	const X3D::X3DPtr <X3D::X3DBindableNode> node (getBrowserWindow () -> createNode ("GeoViewpoint", undoStep));
	node -> set_bind () = true;
	getBrowserWindow () -> addUndoStep (undoStep);
}

X3DGeoViewpointEditor::~X3DGeoViewpointEditor ()
{ }

} // puck
} // titania
