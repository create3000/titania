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

#ifndef __TITANIA_X3D_COMPONENTS_PICKING_X3DPICK_SENSOR_NODE_H__
#define __TITANIA_X3D_COMPONENTS_PICKING_X3DPICK_SENSOR_NODE_H__

#include "../Core/X3DSensorNode.h"

#include "../../Browser/PointingDeviceSensor/Intersection.h"

namespace titania {
namespace X3D {

class X3DPickSensorNode :
	public X3DSensorNode
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

	///  @name Fields

	MFString &
	objectType ()
	{ return *fields .objectType; }

	const MFString &
	objectType () const
	{ return *fields .objectType; }

	SFString &
	intersectionType ()
	{ return *fields .intersectionType; }

	const SFString &
	intersectionType () const
	{ return *fields .intersectionType; }

	SFString &
	sortOrder ()
	{ return *fields .sortOrder; }

	const SFString &
	sortOrder () const
	{ return *fields .sortOrder; }

	SFNode &
	pickingGeometry ()
	{ return *fields .pickingGeometry; }

	const SFNode &
	pickingGeometry () const
	{ return *fields .pickingGeometry; }

	MFNode &
	pickTarget ()
	{ return *fields .pickTarget; }

	const MFNode &
	pickTarget () const
	{ return *fields .pickTarget; }

	MFNode &
	pickedGeometry ()
	{ return *fields .pickedGeometry; }

	const MFNode &
	pickedGeometry () const
	{ return *fields .pickedGeometry; }

	///  @name Member access

	const std::set <std::string> &
	getObjectType () const
	{ return objectTypeIndex; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~X3DPickSensorNode () override;


protected:

	///  @name Friends

	friend class Shape;
	friend class X3DPickingContext;

	///  @name Member types.

	enum class IntersectionType
	{
		BOUNDS,
		GEOMETRY
	};

	enum class SortOrderType
	{
		ANY,
		CLOSEST,
		ALL,
		ALL_SORTED
	};

	struct Target
	{
		Target (X3DGeometryNode* geometryNode,
		        const Matrix4d & modelMatrix,
		        const std::vector <X3DChildNode*> & pickingHierarchy);

		X3DGeometryNode*              geometryNode;
		Matrix4d                      modelMatrix;
		std::vector <X3DChildNode*>   pickingHierarchy;
		bool                          intersected;
		double                        distance;
		std::vector <Vector3d>        pickedPoints;
		std::vector <IntersectionPtr> intersections;
	};

	using TargetPtr = std::shared_ptr <Target>;
	using Targets   = std::vector <TargetPtr>;

	///  @name Construction

	X3DPickSensorNode ();

	virtual
	void
	initialize () override;

	///  @name Member access

	IntersectionType
	getIntersectionType () const
	{ return intersectionTypeValue; }

	SortOrderType
	getSortOrder () const
	{ return sortOrderType; }

	const std::vector <Matrix4d> &
	getModelMatrices () const
	{ return modelMatrices; }

	const Targets &
	getTargets () const
	{ return targets; }

	///  @name Operations

	const X3DPtr <CollidableShape> &
	getPickShape (X3DGeometryNode* const geometryNode) const;

	MFNode &
	getPickedGeometries ();

	void
	collect (const X3DPtr <X3DGeometryNode> & geometryNode,
	         const Matrix4d & modelMatrix,
	         const std::vector <X3DChildNode*> & pickingHierarchy);

	virtual
	void
	process ();


private:

	///  @name Event handlers

	void
	set_live ();

	void
	set_objectType ();

	void
	set_intersectionType ();

	void
	set_sortOrder ();

	void
	set_pickTarget ();

	X3DNode*
	getPickedGeometry (const TargetPtr & target) const;

	///  @name Fields

	struct Fields
	{
		Fields ();

		MFString* const objectType;
		SFString* const intersectionType;
		SFString* const sortOrder;
		SFNode* const pickingGeometry;
		MFNode* const pickTarget;
		MFNode* const pickedGeometry;
	};

	Fields fields;

	///  @name Members

	std::set <std::string>   objectTypeIndex;
	IntersectionType         intersectionTypeValue;
	SortOrderType            sortOrderType;
	std::set <X3DChildNode*> pickTargetNodes;
	std::vector <Matrix4d>   modelMatrices;
	mutable Targets          targets;
	mutable MFNode           pickedGeometries;

};

} // X3D
} // titania

#endif
