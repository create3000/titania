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

#include "X3DCylinderEditor.h"

#include <Titania/X3D/Browser/Geometry3D/CylinderOptions.h>
#include <Titania/X3D/Components/Geometry3D/Cylinder.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

X3DCylinderEditor::X3DCylinderEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                                 top (this, getCylinderTopCheckButton (),"top"),
	                                side (this, getCylinderSideCheckButton (),"side"),
	                              bottom (this, getCylinderBottomCheckButton (), "bottom"),
	                              height (this, getCylinderHeightAdjustment (), getCylinderHeightSpinButton (), "height"),
	                              radius (this, getCylinderRadiusAdjustment (), getCylinderRadiusSpinButton (), "radius"),
	                          xDimension (this, getCylinderXDimensionAdjustment (), getCylinderXDimensionSpinButton (), "xDimension"),
	                            changing (false)
{ }

void
X3DCylinderEditor::set_geometry ()
{
	const auto nodes  = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::Cylinder });
	const auto global = X3D::MFNode ({ getCurrentBrowser () -> getCylinderOptions () });

	getCylinderExpander () .set_visible (not nodes .empty ());

	top        .setNodes (nodes);
	side       .setNodes (nodes);
	bottom     .setNodes (nodes);
	height     .setNodes (nodes);
	radius     .setNodes (nodes);
	xDimension .setNodes (nodes);

	if (nodes .empty ())
		getCylinderUseGlobalOptionsCheckButton () .set_sensitive (false);

	else
	{
		changing = true;

		const auto global = nodes .back () -> getField <X3D::SFInt32> ("xDimension") < 3;

		getCylinderXDimensionBox ()               .set_sensitive (not global);
		getCylinderUseGlobalOptionsCheckButton () .set_sensitive (true);
		getCylinderUseGlobalOptionsCheckButton () .set_active (global);

		changing = false;
	}
}

void
X3DCylinderEditor::on_cylinder_use_global_options_toggled ()
{
	if (changing)
		return;

	if (getCylinderUseGlobalOptionsCheckButton () .get_active ())
	{
		getCylinderXDimensionBox () .set_sensitive (false);

		for (const auto & node : xDimension .getNodes ())
			node -> setField <X3D::SFInt32> ("xDimension", 0);
	}
	else
	{
		getCylinderXDimensionBox () .set_sensitive (true);

		for (const auto & node : xDimension .getNodes ())
			node -> setField <X3D::SFInt32> ("xDimension", getCurrentBrowser () -> getCylinderOptions () -> xDimension ());	
	}
}

X3DCylinderEditor::~X3DCylinderEditor ()
{ }

} // puck
} // titania
