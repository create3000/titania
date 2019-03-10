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

#include "X3DPickSensorNode.h"

#include "../Rendering/X3DGeometryNode.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

X3DPickSensorNode::Target::Target (X3DGeometryNode* geometryNode,
                                   const Matrix4d & modelMatrix,
                                   const std::vector <X3DChildNode*> & pickingHierarchy) :
	    geometryNode (geometryNode),
	     modelMatrix (modelMatrix),
	pickingHierarchy (pickingHierarchy),
	     intersected (false),
	        distance (std::numeric_limits <double>::infinity ())
{ }

X3DPickSensorNode::Fields::Fields () :
	      objectType (new MFString ({ "ALL" })),
	intersectionType (new SFString ("BOUNDS")),
	       sortOrder (new SFString ("CLOSEST")),
	 pickingGeometry (new SFNode ()),
	      pickTarget (new MFNode ()),
	  pickedGeometry (new MFNode ())
{ }

X3DPickSensorNode::X3DPickSensorNode () :
	        X3DSensorNode (),
	               fields (),
	      objectTypeIndex (),
	intersectionTypeValue (IntersectionType::BOUNDS),
	        sortOrderType (SortOrderType::CLOSEST),
	      pickTargetNodes (),
	        modelMatrices (),
	              targets ()
{
	addType (X3DConstants::X3DPickSensorNode);

	addChildObjects (pickTargetNodes);
}

void
X3DPickSensorNode::initialize ()
{
	X3DSensorNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (&X3DPickSensorNode::set_live, this);
	isLive () .addInterest (&X3DPickSensorNode::set_live, this);

	enabled ()          .addInterest (&X3DPickSensorNode::set_live,             this);
	objectType  ()      .addInterest (&X3DPickSensorNode::set_objectType,       this);
	intersectionType () .addInterest (&X3DPickSensorNode::set_intersectionType, this);
	sortOrder ()        .addInterest (&X3DPickSensorNode::set_sortOrder,        this);
	pickTarget  ()      .addInterest (&X3DPickSensorNode::set_pickTarget,       this);

	set_objectType ();
	set_intersectionType ();
	set_sortOrder ();
	set_pickTarget ();
}

void
X3DPickSensorNode::setExecutionContext (X3DExecutionContext* const executionContext)
{
	getBrowser () -> removePickSensor (this);

	X3DSensorNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_live ();
}

void
X3DPickSensorNode::set_live ()
{
	if (getExecutionContext () -> isLive () and isLive () and enabled () and not objectTypeIndex .count ("NONE"))
	{
		getBrowser () -> addPickSensor (this);
		setPickableObject (true);
	}
	else
	{
		getBrowser () -> removePickSensor (this);
		setPickableObject (false);
	}
}

void
X3DPickSensorNode::set_objectType ()
{
	objectTypeIndex .clear ();

	for (const auto & value : basic::make_const_range (objectType ()))
		objectTypeIndex .emplace (value);

	set_live ();
}

void
X3DPickSensorNode::set_intersectionType ()
{
	static const std::map <std::string, IntersectionType> intersectionTypes = {
		std::pair ("BOUNDS",   IntersectionType::BOUNDS),
		std::pair ("GEOMETRY", IntersectionType::GEOMETRY),
	};

	try
	{
		intersectionTypeValue = intersectionTypes .at (intersectionType ());
	}
	catch (const std::out_of_range & error)
	{
		intersectionTypeValue = IntersectionType::BOUNDS;
	}
}

void
X3DPickSensorNode::set_sortOrder ()
{
	static const std::map <std::string, SortOrderType> sortOrderTypes = {
		std::pair ("ANY",        SortOrderType::ANY),
		std::pair ("CLOSEST",    SortOrderType::CLOSEST),
		std::pair ("ALL",        SortOrderType::ALL),
		std::pair ("ALL_SORTED", SortOrderType::ALL_SORTED),
	};

	try
	{
		sortOrderType = sortOrderTypes .at (sortOrder ());
	}
	catch (const std::out_of_range & error)
	{
		sortOrderType = SortOrderType::CLOSEST;
	}
}

void
X3DPickSensorNode::set_pickTarget ()
{
	pickTargetNodes .clear ();

	for (const auto & node : pickTarget ())
	{
		try
		{
			if (not node)
				continue;

			const auto innerNode = node -> getInnerNode ();
	
			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3DConstants::Inline:
					case X3DConstants::Shape:
					case X3DConstants::X3DGroupingNode:
					{
						pickTargetNodes .emplace_back (dynamic_cast <X3DChildNode*> (innerNode));
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
}

std::vector <X3DNode*>
X3DPickSensorNode::getPickedGeometries () const
{
	targets .erase (std::remove_if (targets .begin (), targets .end (),
	[] (const TargetPtr & target)
	{
		return not target -> intersected;
	}),
	targets .end ());

	std::vector <X3DNode*> pickedGeometries;

	if (targets .empty ())
		return pickedGeometries;

	switch (getSortOrder ())
	{
		case SortOrderType::ANY:
		{
			pickedGeometries .emplace_back (getPickedGeometry (targets .front ()));
			break;
		}
		case SortOrderType::CLOSEST:
		{
			std::sort (targets .begin (), targets .end (),
			[ ] (const TargetPtr & lhs, const TargetPtr & rhs)
			{
				return lhs -> distance < rhs -> distance;
			});

			pickedGeometries .emplace_back (getPickedGeometry (targets .front ()));
			break;
		}
		case SortOrderType::ALL:
		{
			for (const auto & target : targets)
				pickedGeometries .emplace_back (getPickedGeometry (target));

			break;
		}
		case SortOrderType::ALL_SORTED:
		{
			std::sort (targets .begin (), targets .end (),
			[ ] (const TargetPtr & lhs, const TargetPtr & rhs)
			{
				return lhs -> distance < rhs -> distance;
			});

			for (const auto & target : targets)
				pickedGeometries .emplace_back (getPickedGeometry (target));

			break;
		}
	}

	pickedGeometries .erase (std::remove (pickedGeometries .begin (), pickedGeometries .end (), nullptr),	pickedGeometries .end ());

	return pickedGeometries;
}

X3DNode*
X3DPickSensorNode::getPickedGeometry (const TargetPtr & target) const
{
	const auto geometryNode = target -> geometryNode;

	if (geometryNode -> getExecutionContext () == getExecutionContext ())
		return geometryNode;

	const auto instance = geometryNode  -> getExecutionContext ();

	if (instance -> isType ({ X3DConstants::X3DPrototypeInstance }) and instance -> getExecutionContext () == getExecutionContext ())
		return dynamic_cast <X3DNode*> (instance);

	for (const auto node : target -> pickingHierarchy)
	{
		if (node -> getExecutionContext () == getExecutionContext ())
			return node;

		const auto instance = node  -> getExecutionContext ();

		if (instance -> isType ({ X3DConstants::X3DPrototypeInstance }) and instance -> getExecutionContext () == getExecutionContext ())
			return dynamic_cast <X3DNode*> (instance);
	}

	return nullptr;
}

void
X3DPickSensorNode::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	modelMatrices .emplace_back (renderObject -> getModelViewMatrix () .get ());
}

void
X3DPickSensorNode::collect (const X3DPtr <X3DGeometryNode> & geometryNode,
                            const Matrix4d & modelMatrix,
                            const std::vector <X3DChildNode*> & pickingHierarchy)
{
	const auto haveTarget = std::any_of (pickingHierarchy .begin (), pickingHierarchy .end (),
	[&] (X3DChildNode* const node)
	{
		return std::count (pickTargetNodes .begin (), pickTargetNodes .end (), node);
	});

	if (not haveTarget)
		return;

	targets .emplace_back (std::make_shared <Target> (geometryNode, modelMatrix, pickingHierarchy));
}

void
X3DPickSensorNode::process ()
{
	modelMatrices .clear ();
	targets       .clear ();
}

void
X3DPickSensorNode::dispose ()
{
	getBrowser () -> removePickSensor (this);

	X3DSensorNode::dispose ();
}

X3DPickSensorNode::~X3DPickSensorNode ()
{ }

} // X3D
} // titania
