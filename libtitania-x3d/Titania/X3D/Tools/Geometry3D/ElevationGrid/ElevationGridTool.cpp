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

#include "ElevationGridTool.h"

#include "../../../Execution/X3DExecutionContext.h"

#include "../../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

ElevationGridTool::Fields::Fields ()
{ }

ElevationGridTool::ElevationGridTool (X3DBaseNode* const node) :
	        X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	      ElevationGrid (node -> getExecutionContext ()),
	        X3DBaseTool (node),
	X3DGeometryNodeTool (),
	             fields (),
	      startXSpacing (0),
	      startZSpacing (0),
	        startHeight (),
	  startMinMaxHeight (0, 0)
{
	addType (X3DConstants::ElevationGridTool);

	addField (inputOutput, "toolType",   toolType ());
	addField (inputOutput, "normalTool", normalTool ());
	addField (inputOutput, "coordTool",  coordTool ());

	toolType () = "TRANSFORM";
}

void
ElevationGridTool::initialize ()
{
	X3DGeometryNodeTool::initialize ();

	getTransformTools () .addInterest (&ElevationGridTool::set_transform_tools, this);
}

void
ElevationGridTool::set_transform_tools ()
{
	const auto & transformTool = getTransformTools () [0];

	xSpacing () .addInterest (&ElevationGridTool::set_xSpacing, this);
	zSpacing () .addInterest (&ElevationGridTool::set_zSpacing, this);
	height ()   .addInterest (&ElevationGridTool::set_height,   this);

	transformTool -> scaleXBackAxis ()  = false;
	transformTool -> scaleZBackAxis ()  = false;
	transformTool -> scaleUniform ()    = false;
	transformTool -> scaleFromCenter () = false;

	set_xSpacing ();
	set_zSpacing ();
	set_height ();
}

void
ElevationGridTool::set_xSpacing ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> removeInterest (&ElevationGridTool::set_scale, this);
	transformTool -> addInterest (&ElevationGridTool::connectScale, this);

	const auto xScale = xSpacing () * (xDimension () - 1);

	transformTool -> translation () .setX (xScale / 2);
	transformTool -> scale ()       .setX (xScale);
}

void
ElevationGridTool::set_zSpacing ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> removeInterest (&ElevationGridTool::set_scale, this);
	transformTool -> addInterest (&ElevationGridTool::connectScale, this);

	const auto zScale = zSpacing () * (zDimension () - 1);

	transformTool -> translation () .setZ (zScale / 2);
	transformTool -> scale ()       .setZ (zScale);
}

void
ElevationGridTool::set_height ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> removeInterest (&ElevationGridTool::set_scale, this);
	transformTool -> addInterest (&ElevationGridTool::connectScale, this);

	const auto pair             = getMinMaxHeight ();
	const auto calculatedHeight = pair .second - pair .first;
	const auto calculatedCenter = (pair .second + pair .first) / 2;

	transformTool -> translation () .setY (calculatedCenter);
	transformTool -> scale ()       .setY (calculatedHeight);
}

void
ElevationGridTool::set_scale ()
{
	const auto & transformTool = getTransformTools () [0];

	xSpacing () .removeInterest (&ElevationGridTool::set_xSpacing, this);
	xSpacing () .addInterest (&ElevationGridTool::connectXSpacing, this);

	zSpacing () .removeInterest (&ElevationGridTool::set_zSpacing, this);
	zSpacing () .addInterest (&ElevationGridTool::connectZSpacing, this);

	const auto scale = transformTool -> getMatrix () .mult_dir_matrix (Vector3d (1, 1, 1));

	xSpacing () = scale .x () / (xDimension () - 1);
	zSpacing () = scale .z () / (zDimension () - 1);

	// height

	set_yScale (scale .y ());
}

void
ElevationGridTool::set_yScale (const float yScale)
{
	// height

	const auto calculatedHeight = startMinMaxHeight .second - startMinMaxHeight .first;

	if (yScale not_eq calculatedHeight)
	{
		const auto & transformTool = getTransformTools () [0];

		height () .removeInterest (&ElevationGridTool::set_height, this);
		height () .addInterest (&ElevationGridTool::connectHeight, this);

		const auto center    = transformTool -> translation () .getY ();
		const auto minHeight = center - yScale / 2;
		const auto maxHeight = center + yScale / 2;

		height () .resize (startHeight .size ());

		for (size_t i = 0, size = startHeight .size (); i < size; ++ i)
			height () [i] = project <float> (startHeight [i], startMinMaxHeight .first, startMinMaxHeight .second, minHeight, maxHeight);
	}
}

void
ElevationGridTool::connectXSpacing (const SFFloat & field)
{
	field .removeInterest (&ElevationGridTool::connectXSpacing, this);
	field .addInterest (&ElevationGridTool::set_xSpacing, this);
}

void
ElevationGridTool::connectZSpacing (const SFFloat & field)
{
	field .removeInterest (&ElevationGridTool::connectZSpacing, this);
	field .addInterest (&ElevationGridTool::set_zSpacing, this);
}

void
ElevationGridTool::connectHeight (const MFFloat & field)
{
	field .removeInterest (&ElevationGridTool::connectHeight, this);
	field .addInterest (&ElevationGridTool::set_height, this);
}

void
ElevationGridTool::connectScale ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> removeInterest (&ElevationGridTool::connectScale, this);
	transformTool -> addInterest (&ElevationGridTool::set_scale, this);
}

void
ElevationGridTool::beginUndo ()
{
	startXSpacing     = xSpacing ();
	startZSpacing     = zSpacing ();
	startHeight       = height ();
	startMinMaxHeight = getMinMaxHeight ();
}

void
ElevationGridTool::endUndo (const UndoStepPtr & undoStep)
{
	if (xSpacing () not_eq startXSpacing or
	    zSpacing () not_eq startZSpacing or
	    height ()   not_eq startHeight)
	{
		const auto & transformTool = getTransformTools () [0];

		undoStep -> addUndoFunction (&MFFloat::setValue, std::ref (height ()),   startHeight);
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (zSpacing ()), startZSpacing);
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (xSpacing ()), startXSpacing);
		undoStep -> addUndoFunction (&ElevationGridTool::setChanging, X3DPtr <ElevationGrid> (this), 0, true);

		undoStep -> addRedoFunction (&ElevationGridTool::setChanging, X3DPtr <ElevationGrid> (this), 0, true);
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (xSpacing ()), xSpacing ());
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (zSpacing ()), zSpacing ());
		undoStep -> addRedoFunction (&MFFloat::setValue, std::ref (height ()),   height ());

		// Scale Y must always be positive after transformation end as it cannot be determined a negative scale Y from height.

		transformTool -> removeInterest (&ElevationGridTool::set_scale, this);
		transformTool -> addInterest (&ElevationGridTool::connectScale, this);

		auto scale = transformTool -> getMatrix () .mult_dir_matrix (Vector3d (1, 1, 1));
		
		scale .y (std::abs (scale .y ()));

		transformTool -> rotation () = Rotation4d ();
		transformTool -> scale ()    = scale;
	}
}

void
ElevationGridTool::dispose ()
{
	__LOG__ << std::endl;

	X3DGeometryNodeTool::dispose ();
}

ElevationGridTool::~ElevationGridTool ()
{ }

} // X3D
} // titania
