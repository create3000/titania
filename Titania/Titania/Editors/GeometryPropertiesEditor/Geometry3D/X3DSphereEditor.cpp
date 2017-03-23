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

#include "X3DSphereEditor.h"

#include <Titania/X3D/Browser/Geometry3D/QuadSphereProperties.h>
#include <Titania/X3D/Browser/Geometry3D/SphereOptions.h>
#include <Titania/X3D/Components/Geometry3D/Sphere.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

X3DSphereEditor::X3DSphereEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                              radius (this, getSphereRadiusAdjustment (), getSphereRadiusSpinButton (), "radius"),
	                    useGlobalOptions (this, getSphereUseGlobalOptionsCheckButton (),"useGlobalOptions"),
	                          xDimension (this, getSphereXDimensionAdjustment (), getSphereXDimensionSpinButton (), "xDimension"),
	                          yDimension (this, getSphereYDimensionAdjustment (), getSphereYDimensionSpinButton (), "yDimension")
{
	getSphereUseGlobalOptionsCheckButton () .property_inconsistent () .signal_changed () .connect (sigc::mem_fun (this, &X3DSphereEditor::on_sphere_use_global_options_toggled));
}

void
X3DSphereEditor::set_geometry ()
{
	// Hidden fields

	for (const auto & node : useGlobalOptions .getNodes ())
		node -> getField <X3D::SFNode> ("properties") .removeInterest (&X3DSphereEditor::set_properties, this);

	// Fields

	const auto nodes = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::Sphere });

	getSphereExpander () .set_visible (not nodes .empty ());

	radius .setNodes (nodes);

	// Hidden fields

	useGlobalOptions .setNodes (nodes);

	for (const auto & node : useGlobalOptions .getNodes ())
		node -> getField <X3D::SFNode> ("properties") .addInterest (&X3DSphereEditor::set_properties, this);

	set_properties ();
}

void
X3DSphereEditor::on_sphere_use_global_options_toggled ()
{
	const auto global = getSphereUseGlobalOptionsCheckButton () .get_active () or getSphereUseGlobalOptionsCheckButton () .get_inconsistent ();

	getSphereXDimensionBox () .set_sensitive (not global);
	getSphereYDimensionBox () .set_sensitive (not global);

	set_properties ();
}

void
X3DSphereEditor::set_properties ()
{
	X3D::MFNode nodes;

	for (const auto & node : useGlobalOptions .getNodes ())
	{
		auto & propertiesNode = node -> getField <X3D::SFNode> ("properties");

		if (not propertiesNode)
			propertiesNode = getCurrentBrowser () -> getSphereOptions () -> properties () -> copy (getCurrentContext (), X3D::FLAT_COPY);

		nodes .emplace_back (propertiesNode);
	}

	xDimension .setNodes (nodes);
	yDimension .setNodes (nodes);
}

X3DSphereEditor::~X3DSphereEditor ()
{ }

} // puck
} // titania
