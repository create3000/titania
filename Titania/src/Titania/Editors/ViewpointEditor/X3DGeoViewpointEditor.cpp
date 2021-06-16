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

#include "X3DGeoViewpointEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../ComposedWidgets/MFStringGeoSystem.h"

#include <Titania/X3D/Components/Geospatial/GeoViewpoint.h>
#include <Titania/X3D/Execution/World.h>

namespace titania {
namespace puck {

using math::pi;

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
	            orientationTool (this, getGeoViewpointOrientationToolBox (), "orientation"),
	                orientation (this,
	                             getGeoViewpointOrientationXAdjustment (),
	                             getGeoViewpointOrientationYAdjustment (),
	                             getGeoViewpointOrientationZAdjustment (),
	                             getGeoViewpointOrientationAAdjustment (),
	                             getGeoViewpointOrientationBox (),
	                             "orientation"),
	           orientationEuler (this,
	                             getGeoViewpointOrientationEulerXAdjustment (),
	                             getGeoViewpointOrientationEulerYAdjustment (),
	                             getGeoViewpointOrientationEulerZAdjustment (),
	                             getGeoViewpointOrientationEulerBox (),
	                             "orientation"),
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

	getGeoViewpointFieldOfViewAdjustment () -> set_upper (pi <double>); // getCurrentContext () -> fromRadiant (pi <double>);
}

void
X3DGeoViewpointEditor::configure ()
{
	switch (getConfig () -> getItem <int32_t> ("geoViewpointOrientationType"))
	{
		case 1:
			getGeoViewpointOrientationEulerButton () .set_active (true);
			break;
		default:
			getGeoViewpointOrientationAxisAngleButton () .set_active (true);
			break;
	}
}

void
X3DGeoViewpointEditor::setGeoViewpoint (const X3D::X3DPtr <X3D::X3DViewpointNode> & viewpointNode, const bool inScene)
{
	const X3D::X3DPtr <X3D::GeoViewpoint> geoViewpoint (viewpointNode);

	getGeoViewpointBox ()      .set_sensitive (inScene);
	getGeoViewpointExpander () .set_visible (geoViewpoint);

	const auto geoViewpoints = geoViewpoint ? X3D::MFNode ({ geoViewpoint }) : X3D::MFNode ();

	geoSystem -> setNodes (geoViewpoints);

	position         .setNodes (geoViewpoints);
	orientationTool  .setNodes (geoViewpoints);
	orientation      .setNodes (geoViewpoints);
	orientationEuler .setNodes (geoViewpoints);
	centerOfRotation .setNodes (geoViewpoints);
	fieldOfView      .setNodes (geoViewpoints);
	speedFactor      .setNodes (geoViewpoints);
}

void
X3DGeoViewpointEditor::on_new_geo_viewpoint_clicked ()
{
	getNewViewpointPopover () .popdown ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New GeoViewpoint"));
	const auto node     = X3D::X3DPtr <X3D::GeoViewpoint> (X3D::X3DEditor::createNode (getCurrentWorld (), getCurrentContext (), "GeoViewpoint", undoStep));

	node -> set_bind () = true;

	try
	{
		const auto & activeViewpoint = getCurrentWorld () -> getActiveLayer () -> getViewpoint ();

		const X3D::X3DPtr <X3D::GeoViewpoint> geoViewpoint (activeViewpoint);

		if (geoViewpoint)
		{
			node -> geoOrigin ()   = geoViewpoint -> geoOrigin ();
			node -> geoSystem ()   = geoViewpoint -> geoSystem ();
			node -> fieldOfView () = geoViewpoint -> fieldOfView ();
			node -> setup ();
		}

		const auto & cameraSpaceMatrix = activeViewpoint -> getCameraSpaceMatrix ();

		X3D::Vector3f   position;
		X3D::Rotation4d orientation;

		cameraSpaceMatrix .get (position, orientation);
		node -> setPosition (position);
		node -> setOrientation (orientation);
		node -> setCenterOfRotation (activeViewpoint -> getUserCenterOfRotation () * activeViewpoint -> getModelMatrix ());
	}
	catch (const X3D::X3DError &)
	{ }	

	getBrowserWindow () -> getSelection () -> setNodes ({ node }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	getViewpointList () -> setSelection (X3D::X3DPtr <X3D::X3DViewpointNode> (node), true);
}

void
X3DGeoViewpointEditor::on_geo_viewpoint_orientation_type_clicked ()
{
	getGeoViewpointOrientationPopover () .popup ();
}

void
X3DGeoViewpointEditor::on_geo_viewpoint_orientation_axis_angle_toggled ()
{
	getGeoViewpointOrientationPopover () .popdown ();

	if (not getGeoViewpointOrientationAxisAngleButton () .get_active ())
		return;

	for (const auto widget : getGeoViewpointOrientationNotebook () .get_children ())
		widget -> set_visible (false);

	getGeoViewpointOrientationBox () .set_visible (true);
	getGeoViewpointOrientationNotebook () .set_current_page (0);

	getConfig () -> setItem <int32_t> ("geoViewpointOrientationType", 0);
}

void
X3DGeoViewpointEditor::on_geo_viewpoint_orientation_euler_toggled ()
{
	getGeoViewpointOrientationPopover () .popdown ();

	if (not getGeoViewpointOrientationEulerButton () .get_active ())
		return;

	for (const auto widget : getGeoViewpointOrientationNotebook () .get_children ())
		widget -> set_visible (false);

	getGeoViewpointOrientationEulerBox () .set_visible (true);
	getGeoViewpointOrientationNotebook () .set_current_page (1);

	getConfig () -> setItem <int32_t> ("geoViewpointOrientationType", 1);
}

X3DGeoViewpointEditor::~X3DGeoViewpointEditor ()
{ }

} // puck
} // titania
