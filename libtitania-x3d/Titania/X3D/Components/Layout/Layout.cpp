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

#include "Layout.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../Navigation/NavigationInfo.h"
#include "../Navigation/X3DViewpointNode.h"

#include <Titania/Math/Functional.h>

namespace titania {
namespace X3D {

const ComponentType Layout::component      = ComponentType::LAYOUT;
const std::string   Layout::typeName       = "Layout";
const std::string   Layout::containerField = "layout";

Layout::Fields::Fields () :
	      align (new MFString ({ "CENTER", "CENTER" })),
	offsetUnits (new MFString ({ "WORLD", "WORLD" })),
	     offset (new MFFloat ({ 0, 0 })),
	  sizeUnits (new MFString ({ "WORLD", "WORLD" })),
	       size (new MFFloat ({ 1, 1 })),
	  scaleMode (new MFString ({ "NONE", "NONE" }))
{ }

Layout::Layout (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DLayoutNode (),
	         fields (),
	         alignX (HorizontalAlignType::CENTER),
	         alignY (VerticalAlignType::CENTER),
	    offsetUnitX (SizeUnitType::WORLD),
	    offsetUnitY (SizeUnitType::WORLD),
	        offsetX (0),
	        offsetY (0),
	      sizeUnitX (SizeUnitType::WORLD),
	      sizeUnitY (SizeUnitType::WORLD),
	          sizeX (1),
	          sizeY (1),
	     scaleModeX (ScaleModeType::NONE),
	     scaleModeY (ScaleModeType::NONE),
	         parent (nullptr),
	rectangleCenter (),
	  rectangleSize (),
	         matrix ()
{
	addType (X3DConstants::Layout);

	addField (inputOutput, "metadata",    metadata ());
	addField (inputOutput, "align",       align ());
	addField (inputOutput, "offsetUnits", offsetUnits ());
	addField (inputOutput, "offset",      offset ());
	addField (inputOutput, "sizeUnits",   sizeUnits ());
	addField (inputOutput, "size",        size ());
	addField (inputOutput, "scaleMode",   scaleMode ());
}

X3DBaseNode*
Layout::create (X3DExecutionContext* const executionContext) const
{
	return new Layout (executionContext);
}

void
Layout::initialize ()
{
	X3DLayoutNode::initialize ();

	align ()       .addInterest (&Layout::set_align, this);
	offsetUnits () .addInterest (&Layout::set_offsetUnits, this);
	offset ()      .addInterest (&Layout::set_offset, this);
	sizeUnits ()   .addInterest (&Layout::set_sizeUnits, this);
	size ()        .addInterest (&Layout::set_size, this);
	scaleMode ()   .addInterest (&Layout::set_scaleMode, this);

	set_align ();
	set_offsetUnits ();
	set_offset ();
	set_sizeUnits ();
	set_size ();
	set_scaleMode ();
}

Layout::SizeUnitType
Layout::getOffsetUnitX () const
{
	if (offsetUnitX == SizeUnitType::WORLD)
	{
		if (parent)
			return parent -> getOffsetUnitX ();

		else
			return SizeUnitType::FRACTION;
	}

	return offsetUnitX;
}

Layout::SizeUnitType
Layout::getOffsetUnitY () const
{
	if (offsetUnitY == SizeUnitType::WORLD)
	{
		if (parent)
			return parent -> getOffsetUnitY ();

		else
			return SizeUnitType::FRACTION;
	}

	return offsetUnitY;
}

Layout::SizeUnitType
Layout::getSizeUnitX () const
{
	if (sizeUnitX == SizeUnitType::WORLD)
	{
		if (parent)
			return parent -> getSizeUnitX ();

		else
			return SizeUnitType::FRACTION;
	}

	return sizeUnitX;
}

Layout::SizeUnitType
Layout::getSizeUnitY () const
{
	if (sizeUnitY == SizeUnitType::WORLD)
	{
		if (parent)
			return parent -> getSizeUnitY ();

		else
			return SizeUnitType::FRACTION;
	}

	return sizeUnitY;
}

Layout::ScaleModeType
Layout::getScaleModeX () const
{
	if (parent)
		return scaleModeX;

	if (scaleModeX == ScaleModeType::NONE)
		return ScaleModeType::FRACTION;

	return scaleModeX;
}

Layout::ScaleModeType
Layout::getScaleModeY () const
{
	if (parent)
		return scaleModeY;

	if (scaleModeY == ScaleModeType::NONE)
		return ScaleModeType::FRACTION;

	return scaleModeY;
}

void
Layout::set_align ()
{
	// X

	if (align () .size () > 0)
	{
		if (align () [0] == "LEFT")
			alignX = HorizontalAlignType::LEFT;

		else if (align () [0] == "RIGHT")
			alignX = HorizontalAlignType::RIGHT;

		else
			alignX = HorizontalAlignType::CENTER;
	}
	else
		alignX = HorizontalAlignType::CENTER;

	// Y

	if (align () .size () > 1)
	{
		if (align () [1] == "BOTTOM")
			alignY = VerticalAlignType::BOTTOM;

		else if (align () [1] == "TOP")
			alignY = VerticalAlignType::TOP;

		else
			alignY = VerticalAlignType::CENTER;
	}
	else
		alignY = VerticalAlignType::CENTER;
}

void
Layout::set_offsetUnits ()
{
	if (offsetUnits () .size () > 0)
	{
		// X

		if (offsetUnits () [0] == "FRACTION")
			offsetUnitX = SizeUnitType::FRACTION;

		else if (offsetUnits () [0] == "PIXEL")
			offsetUnitX = SizeUnitType::PIXEL;

		else
			offsetUnitX = SizeUnitType::WORLD;

		// Y

		if (offsetUnits () .size () > 1)
		{
			if (offsetUnits () [1] == "FRACTION")
				offsetUnitY = SizeUnitType::FRACTION;

			else if (offsetUnits () [1] == "PIXEL")
				offsetUnitY = SizeUnitType::PIXEL;

			else
				offsetUnitY = SizeUnitType::WORLD;
		}
		else
			offsetUnitY = offsetUnitX;
	}
	else
	{
		offsetUnitX = SizeUnitType::WORLD;
		offsetUnitY = SizeUnitType::WORLD;
	}
}

void
Layout::set_offset ()
{
	if (offset () .size () > 0)
	{
		// X

		offsetX = offset () [0];

		// Y

		if (offset () .size () > 1)
			offsetY = offset () [1];

		else
			offsetY = offsetX;
	}
	else
	{
		offsetX = 0;
		offsetY = 0;
	}
}

void
Layout::set_scaleMode ()
{
	if (scaleMode () .size () > 0)
	{
		// X

		if (scaleMode () [0] == "FRACTION")
			scaleModeX = ScaleModeType::FRACTION;

		else if (scaleMode () [0] == "PIXEL")
			scaleModeX = ScaleModeType::PIXEL;

		else if (scaleMode () [0] == "STRETCH")
			scaleModeX = ScaleModeType::STRETCH;

		else
			scaleModeX = ScaleModeType::NONE;

		// Y

		if (scaleMode () .size () > 1)
		{
			if (scaleMode () [1] == "FRACTION")
				scaleModeY = ScaleModeType::FRACTION;

			else if (scaleMode () [1] == "PIXEL")
				scaleModeY = ScaleModeType::PIXEL;

			else if (scaleMode () [1] == "STRETCH")
				scaleModeY = ScaleModeType::STRETCH;

			else
				scaleModeY = ScaleModeType::NONE;
		}
		else
			scaleModeY = scaleModeX;
	}
	else
	{
		scaleModeX = ScaleModeType::NONE;
		scaleModeY = ScaleModeType::NONE;
	}
}

void
Layout::set_sizeUnits ()
{
	if (sizeUnits () .size () > 0)
	{
		// X

		if (sizeUnits () [0] == "FRACTION")
			sizeUnitX = SizeUnitType::FRACTION;

		else if (sizeUnits () [0] == "PIXEL")
			sizeUnitX = SizeUnitType::PIXEL;

		else
			sizeUnitX = SizeUnitType::WORLD;

		// Y

		if (sizeUnits () .size () > 1)
		{
			if (sizeUnits () [1] == "FRACTION")
				sizeUnitY = SizeUnitType::FRACTION;

			else if (sizeUnits () [1] == "PIXEL")
				sizeUnitY = SizeUnitType::PIXEL;

			else
				sizeUnitY = SizeUnitType::WORLD;
		}
		else
			sizeUnitY = sizeUnitX;
	}
	else
	{
		sizeUnitX = SizeUnitType::WORLD;
		sizeUnitY = SizeUnitType::WORLD;
	}
}

void
Layout::set_size ()
{
	if (size () .size () > 0)
	{
		// X

		sizeX = size () [0];

		// Y

		if (size () .size () > 1)
			sizeY = size () [1];

		else
			sizeY = sizeX;
	}
	else
	{
		sizeX = 0;
		sizeY = 0;
	}
}

const Matrix4d &
Layout::transform (const TraverseType type, X3DRenderObject* const renderObject)
{
	parent = renderObject -> getParentLayout ();

	// Calculate rectangleSize

	const auto       viewpoint           = renderObject -> getViewpoint ();
	const auto       nearValue           = renderObject -> getNavigationInfo () -> getNearValue ();   // in meter
	const auto &     viewport            = renderObject -> getViewVolumes () .back () .getScissor (); // in pixel
	const auto       viewportPixel       = Vector2i (viewport [2], viewport [3]);                     // in pixel
	const Vector2d   viewportMeter       = viewpoint -> getViewportSize (viewport, nearValue);        // in meter
	const Vector2d & parentRectangleSize = parent ? parent -> getRectangleSize () : viewportMeter;    // in meter
	const Vector2d   pixelSize           = viewportMeter / Vector2d (viewportPixel);                  // size of one pixel in meter

	switch (getSizeUnitX ())
	{
		case SizeUnitType::FRACTION:
			rectangleSize .x (sizeX * parentRectangleSize .x ());
			break;
		case SizeUnitType::PIXEL:
			rectangleSize .x (sizeX * pixelSize .x ());
			break;
		default:
			break;
	}

	switch (getSizeUnitY ())
	{
		case SizeUnitType::FRACTION:
			rectangleSize .y (sizeY * parentRectangleSize .y ());
			break;
		case SizeUnitType::PIXEL:
			rectangleSize .y (sizeY * pixelSize .y ());
			break;
		default:
			break;
	}

	// Calculate translation

	Vector3d translation;

	switch (getAlignX ())
	{
		case HorizontalAlignType::LEFT:
			translation .x (-(parentRectangleSize .x () - rectangleSize .x ()) / 2);
			break;
		case HorizontalAlignType::CENTER:

			if (getSizeUnitX () == SizeUnitType::PIXEL and math::is_odd (viewportPixel .x ()))
				translation .x (-pixelSize .x () / 2);

			break;
		case HorizontalAlignType::RIGHT:
			translation .x ((parentRectangleSize .x () - rectangleSize .x ()) / 2);
			break;
	}

	switch (getAlignY ())
	{
		case VerticalAlignType::BOTTOM:
			translation .y (-(parentRectangleSize .y () - rectangleSize .y ()) / 2);
			break;
		case VerticalAlignType::CENTER:

			if (getSizeUnitX () == SizeUnitType::PIXEL and math::is_odd (viewportPixel .y ()))
				translation .y (-pixelSize .y () / 2);

			break;
		case VerticalAlignType::TOP:
			translation .y ((parentRectangleSize .y () - rectangleSize .y ()) / 2);
			break;
	}

	// Calculate offset

	Vector3d offset;

	switch (getOffsetUnitX ())
	{
		case SizeUnitType::FRACTION:
			offset .x (offsetX * parentRectangleSize .x ());
			break;
		case SizeUnitType::PIXEL:
			offset .x (offsetX * viewportMeter .x () / viewportPixel .x ());
			break;
		default:
			break;
	}

	switch (getOffsetUnitY ())
	{
		case SizeUnitType::FRACTION:
			offset .y (offsetY * parentRectangleSize .y ());
			break;
		case SizeUnitType::PIXEL:
			offset .y (offsetY * viewportMeter .y () / viewportPixel .y ());
			break;
		default:
			break;
	}

	// Calculate scale

	Vector3d scale (1, 1, 1);

	Vector3d   currentTranslation, currentScale;
	Rotation4d currentRotation;

	const Matrix4d modelViewMatrix = renderObject -> getModelViewMatrix () .get ();
	modelViewMatrix .get (currentTranslation, currentRotation, currentScale);

	switch (getScaleModeX ())
	{
		case ScaleModeType::NONE:
			scale .x (currentScale .x ());
			break;
		case ScaleModeType::FRACTION:
			scale .x (rectangleSize .x ());
			break;
		case ScaleModeType::STRETCH:
			break;
		case ScaleModeType::PIXEL:
			scale .x (viewportMeter .x () / viewportPixel .x ());
			break;
	}

	switch (getScaleModeY ())
	{
		case ScaleModeType::NONE:
			scale .y (currentScale .y ());
			break;
		case ScaleModeType::FRACTION:
			scale .y (rectangleSize .y ());
			break;
		case ScaleModeType::STRETCH:
			break;
		case ScaleModeType::PIXEL:
			scale .y (viewportMeter .y () / viewportPixel .y ());
			break;
	}

	// Calculate scale for scaleMode STRETCH

	if (getScaleModeX () == ScaleModeType::STRETCH)
	{
		if (getScaleModeY () == ScaleModeType::STRETCH)
		{
			if (rectangleSize .x () > rectangleSize .y ())
			{
				scale .x (rectangleSize .x ());
				scale .y (scale .x ());
			}
			else
			{
				scale .y (rectangleSize .y ());
				scale .x (scale .y ());
			}
		}
		else
			scale .x (scale .y ());
	}
	else if (getScaleModeY () == ScaleModeType::STRETCH)
		scale .y (scale .x ());

	// Transform

	const auto position = translation + offset;

	rectangleCenter .x (position .x ());
	rectangleCenter .y (position .y ());

	matrix .set (currentTranslation, currentRotation);
	matrix .translate (translation + offset);
	matrix .scale (scale);

	//__LOG__ << this << " : " << rectangleSize << std::endl;
	//__LOG__ << this << " : " << scale << std::endl;
	//__LOG__ << matrix << std::endl;

	return matrix;
}

} // X3D
} // titania
