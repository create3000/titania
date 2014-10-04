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

#include "X3DGeoTransformEditor.h"

#include "../../ComposedWidgets/MFStringGeoSystem.h"
#include "../../ComposedWidgets/RotationTool.h"

namespace titania {
namespace puck {

X3DGeoTransformEditor::X3DGeoTransformEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                          geoSystem (new MFStringGeoSystem (getBrowserWindow (),
	                                     getGeoTransformCoordinateSystemComboBoxText (),
	                                     getGeoTransformEllipsoidComboBoxText (),
	                                     getGeoTransformGDOrderComboBoxText (),
	                                     getGeoTransformZoneAdjustment (),
	                                     getGeoTransformHemisphereComboBoxText (),
	                                     getGeoTransformUTMOrderComboBoxText (),
	                                     getGeoTransformEllipsoidBox (),
	                                     getGeoTransformGDOrderComboBoxText (),
	                                     getGeoTransformGeoSystemUTMBox ())),
	                        translation (getBrowserWindow (),
	                                     getGeoTransformTranslationXAdjustment (),
	                                     getGeoTransformTranslationYAdjustment (),
	                                     getGeoTransformTranslationZAdjustment (),
	                                     getGeoTransformTranslationBox (),
	                                     "translation"),
	                           rotation (getBrowserWindow (),
	                                     getGeoTransformRotationXAdjustment (),
	                                     getGeoTransformRotationYAdjustment (),
	                                     getGeoTransformRotationZAdjustment (),
	                                     getGeoTransformRotationAAdjustment (),
	                                     getGeoTransformRotationBox (),
	                                     "rotation"),
	                       rotationTool (new RotationTool (getBrowserWindow (), getGeoTransformRotationToolBox (), "rotation")),
	                              scale (getBrowserWindow (),
	                                     getGeoTransformScaleXAdjustment (),
	                                     getGeoTransformScaleYAdjustment (),
	                                     getGeoTransformScaleZAdjustment (),
	                                     getGeoTransformScaleBox (),
	                                     "scale"),
	                   scaleOrientation (getBrowserWindow (),
	                                     getGeoTransformScaleOrientationXAdjustment (),
	                                     getGeoTransformScaleOrientationYAdjustment (),
	                                     getGeoTransformScaleOrientationZAdjustment (),
	                                     getGeoTransformScaleOrientationAAdjustment (),
	                                     getGeoTransformScaleOrientationBox (),
	                                     "scaleOrientation"),
	               scaleOrientationTool (new RotationTool (getBrowserWindow (), getGeoTransformScaleOrientationToolBox (), "scaleOrientation")),
	                          geoCenter (getBrowserWindow (),
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
X3DGeoTransformEditor::initialize ()
{
	getGeoTransformUniformScaleButton () .set_active (getConfig () .getBoolean ("geoTransformUniformScale"));

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DGeoTransformEditor::set_selection);

	set_selection (getBrowserWindow () -> getSelection () -> getChildren ());
}

void
X3DGeoTransformEditor::set_selection (const X3D::MFNode & selection)
{
	const X3D::X3DPtr <X3D::GeoTransform> transform (selection .empty () ? nullptr : selection .back ());
	const X3D::MFNode                     transforms (transform ? X3D::MFNode ({ transform }) : X3D::MFNode ());

	getGeoTransformExpander () .set_visible (transform);

	geoSystem -> setNode (X3D::SFNode (transform));
	translation      .setNodes (transforms);
	rotation         .setNodes (transforms);
	scale            .setNodes (transforms);
	scaleOrientation .setNodes (transforms);
	geoCenter        .setNodes (transforms);

	rotationTool -> setNodes (transforms);
	scaleOrientationTool -> setNodes (transforms);
}

void
X3DGeoTransformEditor::on_geo_transform_uniform_scale_clicked ()
{
	if (getGeoTransformUniformScaleButton () .get_active ())
	{
		getGeoTransformUniformScaleImage () .set_from_icon_name ("connect_established", Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getGeoTransformUniformScaleImage () .set_from_icon_name ("connect_no", Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

X3DGeoTransformEditor::~X3DGeoTransformEditor ()
{
	getConfig () .setItem ("geoTransformUniformScale", getGeoTransformUniformScaleButton () .get_active ());
}

} // puck
} // titania
