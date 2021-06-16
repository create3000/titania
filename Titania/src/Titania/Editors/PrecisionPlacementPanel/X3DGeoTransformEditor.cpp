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

#include "X3DGeoTransformEditor.h"

#include "../../ComposedWidgets/MFStringGeoSystem.h"

#include <Titania/X3D/Components/Geospatial/GeoTransform.h>

namespace titania {
namespace puck {

X3DGeoTransformEditor::X3DGeoTransformEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                          geoSystem (new MFStringGeoSystem (this,
	                                     getGeoTransformCoordinateSystemComboBoxText (),
	                                     getGeoTransformEllipsoidComboBoxText (),
	                                     getGeoTransformGDOrderComboBoxText (),
	                                     getGeoTransformZoneAdjustment (),
	                                     getGeoTransformHemisphereComboBoxText (),
	                                     getGeoTransformUTMOrderComboBoxText (),
	                                     getGeoTransformEllipsoidBox (),
	                                     getGeoTransformGDOrderComboBoxText (),
	                                     getGeoTransformGeoSystemUTMBox ())),
	                        translation (this,
	                                     getGeoTransformTranslationXAdjustment (),
	                                     getGeoTransformTranslationYAdjustment (),
	                                     getGeoTransformTranslationZAdjustment (),
	                                     getGeoTransformTranslationBox (),
	                                     "translation"),
	                       rotationTool (this, getGeoTransformRotationToolBox (), "rotation"),
	                           rotation (this,
	                                     getGeoTransformRotationXAdjustment (),
	                                     getGeoTransformRotationYAdjustment (),
	                                     getGeoTransformRotationZAdjustment (),
	                                     getGeoTransformRotationAAdjustment (),
	                                     getGeoTransformRotationBox (),
	                                     "rotation"),
	                      rotationEuler (this,
	                                     getGeoTransformRotationEulerXAdjustment (),
	                                     getGeoTransformRotationEulerYAdjustment (),
	                                     getGeoTransformRotationEulerZAdjustment (),
	                                     getGeoTransformRotationEulerBox (),
	                                     "rotation"),
	                              scale (this,
	                                     getGeoTransformScaleXAdjustment (),
	                                     getGeoTransformScaleYAdjustment (),
	                                     getGeoTransformScaleZAdjustment (),
	                                     getGeoTransformScaleBox (),
	                                     "scale"),
	               scaleOrientationTool (this, getGeoTransformScaleOrientationToolBox (), "scaleOrientation"),
	                   scaleOrientation (this,
	                                     getGeoTransformScaleOrientationXAdjustment (),
	                                     getGeoTransformScaleOrientationYAdjustment (),
	                                     getGeoTransformScaleOrientationZAdjustment (),
	                                     getGeoTransformScaleOrientationAAdjustment (),
	                                     getGeoTransformScaleOrientationBox (),
	                                     "scaleOrientation"),
	              scaleOrientationEuler (this,
	                                     getGeoTransformScaleOrientationEulerXAdjustment (),
	                                     getGeoTransformScaleOrientationEulerYAdjustment (),
	                                     getGeoTransformScaleOrientationEulerZAdjustment (),
	                                     getGeoTransformScaleOrientationEulerBox (),
	                                     "scaleOrientation"),
	                          geoCenter (this,
	                                     getGeoTransformGeoCenterXAdjustment (),
	                                     getGeoTransformGeoCenterYAdjustment (),
	                                     getGeoTransformGeoCenterZAdjustment (),
	                                     getGeoTransformGeoCenterBox (),
	                                     "geoCenter")
{
	getGeoTransformGeoCenterXAdjustment () -> set_step_increment (1e-7);
	getGeoTransformGeoCenterYAdjustment () -> set_step_increment (1e-7);
	getGeoTransformGeoCenterZAdjustment () -> set_step_increment (1e-2);

	getGeoTransformGeoCenterXAdjustment () -> set_page_increment (1e-4);
	getGeoTransformGeoCenterYAdjustment () -> set_page_increment (1e-4);
	getGeoTransformGeoCenterZAdjustment () -> set_page_increment (1);

	getGeoTransformGeoCenterXSpinButton () .property_climb_rate () = 1e-6;
	getGeoTransformGeoCenterYSpinButton () .property_climb_rate () = 1e-6;
	getGeoTransformGeoCenterZSpinButton () .property_climb_rate () = 1e-1;
}

void
X3DGeoTransformEditor::configure ()
{
	switch (getConfig () -> getItem <int32_t> ("geoTransformRotationType"))
	{
		case 1:
			getGeoTransformRotationEulerButton () .set_active (true);
			break;
		default:
			getGeoTransformRotationAxisAngleButton () .set_active (true);
			break;
	}

	getGeoTransformUniformScaleButton () .set_active (getConfig () -> getItem <bool> ("geoTransformUniformScale"));

	switch (getConfig () -> getItem <int32_t> ("geoTransformScaleOrientationType"))
	{
		case 1:
			getGeoTransformScaleOrientationEulerButton () .set_active (true);
			break;
		default:
			getGeoTransformScaleOrientationAxisAngleButton () .set_active (true);
			break;
	}
}

void
X3DGeoTransformEditor::set_selection (const X3D::MFNode & selection)
{
	const auto transform  = X3D::X3DPtr <X3D::GeoTransform> (selection .empty () ? nullptr : selection .back ());
	const auto transforms = transform ? X3D::MFNode ({ transform }) : X3D::MFNode ();

	getGeoTransformExpander () .set_visible (transform);

	geoSystem -> setNodes (transforms);

	translation           .setNodes (transforms);
	rotationTool          .setNodes (transforms);
	rotation              .setNodes (transforms);
	rotationEuler         .setNodes (transforms);
	scale                 .setNodes (transforms);
	scaleOrientationTool  .setNodes (transforms);
	scaleOrientation      .setNodes (transforms);
	scaleOrientationEuler .setNodes (transforms);
	geoCenter             .setNodes (transforms);
}

void
X3DGeoTransformEditor::on_geo_transform_rotation_type_clicked ()
{
	getGeoTransformRotationPopover () .popup ();
}

void
X3DGeoTransformEditor::on_geo_transform_rotation_axis_angle_toggled ()
{
	getGeoTransformRotationPopover () .popdown ();

	if (not getGeoTransformRotationAxisAngleButton () .get_active ())
		return;

	for (const auto widget : getGeoTransformRotationNotebook () .get_children ())
		widget -> set_visible (false);

	getGeoTransformRotationBox () .set_visible (true);
	getGeoTransformRotationNotebook () .set_current_page (0);

	getConfig () -> setItem <int32_t> ("geoTransformRotationType", 0);
}

void
X3DGeoTransformEditor::on_geo_transform_rotation_euler_toggled ()
{
	getGeoTransformRotationPopover () .popdown ();

	if (not getGeoTransformRotationEulerButton () .get_active ())
		return;

	for (const auto widget : getGeoTransformRotationNotebook () .get_children ())
		widget -> set_visible (false);

	getGeoTransformRotationEulerBox () .set_visible (true);
	getGeoTransformRotationNotebook () .set_current_page (1);

	getConfig () -> setItem <int32_t> ("geoTransformRotationType", 1);
}

void
X3DGeoTransformEditor::on_geo_transform_uniform_scale_clicked ()
{
	if (getGeoTransformUniformScaleButton () .get_active ())
	{
		getGeoTransformUniformScaleImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getGeoTransformUniformScaleImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

void
X3DGeoTransformEditor::on_geo_transform_scale_orientation_type_clicked ()
{
	getGeoTransformScaleOrientationPopover () .popup ();
}

void
X3DGeoTransformEditor::on_geo_transform_scale_orientation_axis_angle_toggled ()
{
	getGeoTransformScaleOrientationPopover () .popdown ();

	if (not getGeoTransformScaleOrientationAxisAngleButton () .get_active ())
		return;

	for (const auto widget : getGeoTransformScaleOrientationNotebook () .get_children ())
		widget -> set_visible (false);

	getGeoTransformScaleOrientationBox () .set_visible (true);
	getGeoTransformScaleOrientationNotebook () .set_current_page (0);

	getConfig () -> setItem <int32_t> ("geoTransformScaleOrientationType", 0);
}

void
X3DGeoTransformEditor::on_geo_transform_scale_orientation_euler_toggled ()
{
	getGeoTransformScaleOrientationPopover () .popdown ();

	if (not getGeoTransformScaleOrientationEulerButton () .get_active ())
		return;

	for (const auto widget : getGeoTransformScaleOrientationNotebook () .get_children ())
		widget -> set_visible (false);

	getGeoTransformScaleOrientationEulerBox () .set_visible (true);
	getGeoTransformScaleOrientationNotebook () .set_current_page (1);

	getConfig () -> setItem <int32_t> ("geoTransformScaleOrientationType", 1);
}

void
X3DGeoTransformEditor::store ()
{
	getConfig () -> setItem ("geoTransformUniformScale", getGeoTransformUniformScaleButton () .get_active ());
}

X3DGeoTransformEditor::~X3DGeoTransformEditor ()
{ }

} // puck
} // titania
