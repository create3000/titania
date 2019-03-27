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

#include "LinePickSensor.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/IndexedLineSet.h"
#include "../Rendering/LineSet.h"

namespace titania {
namespace X3D {

const Component   LinePickSensor::component      = Component ("Picking", 1);
const std::string LinePickSensor::typeName       = "LinePickSensor";
const std::string LinePickSensor::containerField = "children";

LinePickSensor::Fields::Fields () :
	pickedTextureCoordinate (new MFVec3f ()),
	           pickedNormal (new MFVec3f ()),
	            pickedPoint (new MFVec3f ())
{ }

LinePickSensor::LinePickSensor (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DPickSensorNode (),
	             fields (),
	pickingGeometryNode ()
{
	addType (X3DConstants::LinePickSensor);

	addField (inputOutput,    "metadata",                metadata ());
	addField (inputOutput,    "enabled",                 enabled ());
	addField (inputOutput,    "objectType",              objectType ());
	addField (inputOutput,    "matchCriterion",          matchCriterion ());
	addField (initializeOnly, "intersectionType",        intersectionType ());
	addField (initializeOnly, "sortOrder",               sortOrder ());
	addField (inputOutput,    "pickingGeometry",         pickingGeometry ());
	addField (inputOutput,    "pickTarget",              pickTarget ());
	addField (outputOnly,     "isActive",                isActive ());
	addField (outputOnly,     "pickedTextureCoordinate", pickedTextureCoordinate ());
	addField (outputOnly,     "pickedNormal",            pickedNormal ());
	addField (outputOnly,     "pickedPoint",             pickedPoint ());
	addField (outputOnly,     "pickedGeometry",          pickedGeometry ());

	addChildObjects (pickingGeometryNode);
}

X3DBaseNode*
LinePickSensor::create (X3DExecutionContext* const executionContext) const
{
	return new LinePickSensor (executionContext);
}

void
LinePickSensor::initialize ()
{
	X3DPickSensorNode::initialize ();

	pickingGeometry () .addInterest (&LinePickSensor::set_pickingGeometry, this);

	set_pickingGeometry ();
}

void
LinePickSensor::set_pickingGeometry ()
{
	try
	{
		pickingGeometryNode = nullptr;

		if (not pickingGeometry ())
			return;

		const auto innerNode = pickingGeometry () -> getInnerNode ();

		for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
		{
			switch (type)
			{
				case X3DConstants::IndexedLineSet:
				case X3DConstants::LineSet:
				{
					pickingGeometryNode = dynamic_cast <X3DGeometryNode*> (innerNode);
					break;
				}
				default:
					continue;
			}

			break;
		}
	}
	catch (const X3DError &)
	{ }
}

void
LinePickSensor::process ()
{
	if (pickingGeometryNode)
	{
		switch (getIntersectionType ())
		{
			case IntersectionType::BOUNDS:
			{
				// Intersect bboxes.

				for (const auto & modelMatrix : getModelMatrices ())
				{
					const auto pickingBBox = pickingGeometryNode -> getBBox () * modelMatrix;

					for (const auto & target : getTargets ())
					{
						const auto targetBBox = target -> geometryNode -> getBBox () * target -> modelMatrix;

						if (not pickingBBox .intersects (targetBBox))
							continue;

						target -> intersected = true;
						target -> distance    = distance (pickingBBox .center (), targetBBox .center ());
					}
				}

				// Send events.

				auto &     pickedGeometries = getPickedGeometries ();
				const auto active           = not pickedGeometries .empty ();

				pickedGeometries .remove (nullptr);

				if (active not_eq isActive ())
					isActive () = active;

				if (not (pickedGeometry () .equals (pickedGeometries)))
					pickedGeometry () = pickedGeometries;

				break;
			}
			case IntersectionType::GEOMETRY:
			{
				// Intersect geometry.

				for (const auto & modelMatrix : getModelMatrices ())
				{
					const auto pickingBBox = pickingGeometryNode -> getBBox () * modelMatrix;

					for (const auto & target : getTargets ())
					{
						try
						{
							const auto targetBBox = target -> geometryNode -> getBBox () * target -> modelMatrix;
							const auto vertices   = pickingGeometryNode -> getPolygonVertices ();
							const auto matrix     = modelMatrix * inverse (target -> modelMatrix);
	
							for (size_t i = 0, size = vertices .size (); i < size; i += 2)
							{
								const auto point1        = vertices [i + 0] * matrix;
								const auto point2        = vertices [i + 1] * matrix;
								const auto line          = Line3d (point1, point2, points_type ());
								auto       intersections = std::vector <IntersectionPtr> ();
	
								if (target -> geometryNode -> intersects (line, { }, target -> modelMatrix, intersections))
								{
									for (const auto & intersection : intersections)
									{
										// Test if intersection -> point is between point1 and point2.
	
										const auto a = intersection -> getPoint () - point1;
										const auto b = intersection -> getPoint () - point2;
										const auto c = abs (a + b);
										const auto s = abs (point1 - point2);
	
										if (c <= s)
											target -> intersections .emplace_back (intersection);
									}
								}
							}
	
							if (target -> intersections .empty ())
								continue;
	
							target -> intersected = true;
							target -> distance    = distance (pickingBBox .center (), targetBBox .center ());
						}
						catch (const std::domain_error & error)
						{
							// Catch inverse.
						}
					}
				}

				// Send events.

				auto &     pickedGeometries = getPickedGeometries ();
				const auto active           = not pickedGeometries .empty ();

				pickedGeometries .remove (nullptr);

				if (active not_eq isActive ())
					isActive () = active;

				if (not (pickedGeometry () .equals (pickedGeometries)))
					pickedGeometry () = pickedGeometries;

				MFVec3f pickedTextureCoordinates;
				MFVec3f pickedNormals;
				MFVec3f pickedPoints;

				for (const auto & target : getTargets ())
				{
					for (const auto & intersection : target -> intersections)
					{
						const auto texCoord = intersection -> getTexCoord ();

						pickedTextureCoordinates .emplace_back (texCoord .x (), texCoord .y (), texCoord .z ());
						pickedNormals            .emplace_back (intersection -> getNormal ());
						pickedPoints             .emplace_back (intersection -> getPoint ());
					}
				}

				if (not (pickedTextureCoordinate () .equals (pickedTextureCoordinates)))
					pickedTextureCoordinate () = pickedTextureCoordinates;

				if (not (pickedNormal () .equals (pickedNormals)))
					pickedNormal () = pickedNormals;

				if (not (pickedPoint () .equals (pickedPoints)))
					pickedPoint () = pickedPoints;

				break;
			}
		}
	}

	X3DPickSensorNode::process ();
}

LinePickSensor::~LinePickSensor ()
{ }

} // X3D
} // titania
