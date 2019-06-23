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

#ifndef __TITANIA_X3D_COMPONENTS_GEOMETRY3D_EXTRUSION_H__
#define __TITANIA_X3D_COMPONENTS_GEOMETRY3D_EXTRUSION_H__

#include "../Rendering/X3DGeometryNode.h"

namespace titania {
namespace math {

template <class Type, class ... Args>
class tessellator;

} // math
} // titania

namespace titania {
namespace X3D {

class Extrusion :
	virtual public X3DGeometryNode
{
public:

	///  @name Construction

	Extrusion (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	virtual
	MFVec2f &
	set_crossSection ()
	{ return *fields .set_crossSection; }

	virtual
	const MFVec2f &
	set_crossSection () const
	{ return *fields .set_crossSection; }

	virtual
	MFRotation &
	set_orientation ()
	{ return *fields .set_orientation; }

	virtual
	const MFRotation &
	set_orientation () const
	{ return *fields .set_orientation; }

	virtual
	MFVec2f &
	set_scale ()
	{ return *fields .set_scale; }

	virtual
	const MFVec2f &
	set_scale () const
	{ return *fields .set_scale; }

	virtual
	MFVec3f &
	set_spine ()
	{ return *fields .set_spine; }

	virtual
	const MFVec3f &
	set_spine () const
	{ return *fields .set_spine; }

	virtual
	SFBool &
	beginCap ()
	{ return *fields .beginCap; }

	virtual
	const SFBool &
	beginCap () const
	{ return *fields .beginCap; }

	virtual
	SFBool &
	endCap ()
	{ return *fields .endCap; }

	virtual
	const SFBool &
	endCap () const
	{ return *fields .endCap; }

	virtual
	SFBool &
	solid ()
	{ return *fields .solid; }

	virtual
	const SFBool &
	solid () const
	{ return *fields .solid; }

	virtual
	SFBool &
	ccw ()
	{ return *fields .ccw; }

	virtual
	const SFBool &
	ccw () const
	{ return *fields .ccw; }

	virtual
	SFBool &
	convex ()
	{ return *fields .convex; }

	virtual
	const SFBool &
	convex () const
	{ return *fields .convex; }

	virtual
	SFFloat &
	creaseAngle ()
	{ return *fields .creaseAngle; }

	virtual
	const SFFloat &
	creaseAngle () const
	{ return *fields .creaseAngle; }

	virtual
	MFVec2f &
	crossSection ()
	{ return *fields .crossSection; }

	virtual
	const MFVec2f &
	crossSection () const
	{ return *fields .crossSection; }

	virtual
	MFRotation &
	orientation ()
	{ return *fields .orientation; }

	virtual
	const MFRotation &
	orientation () const
	{ return *fields .orientation; }

	virtual
	MFVec2f &
	scale ()
	{ return *fields .scale; }

	virtual
	const MFVec2f &
	scale () const
	{ return *fields .scale; }

	virtual
	MFVec3f &
	spine ()
	{ return *fields .spine; }

	virtual
	const MFVec3f &
	spine () const
	{ return *fields .spine; }

	///  @name Operations

	virtual
	NodeType
	getPrimitiveType () const final override
	{ return X3DConstants::IndexedFaceSet; }

	virtual
	SFNode
	toPrimitive () const override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	using Tessellator = math::tessellator <double, size_t, size_t>;

	///  @name Operations

	bool
	getClosedCrossSection () const;

	bool
	getClosedSpine () const;

	std::vector <Vector3d>
	createPoints (const bool hasCaps) const;

	std::vector <Matrix4d>
	createRotations () const;

	virtual
	void
	build () final override;

	void
	tessellateCap (const Tessellator & tessellator,
	               const std::vector <Vector3d> & points,
	               const Vector2f & min,
	               const float capMax);

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFVec2f* const set_crossSection;
		MFRotation* const set_orientation;
		MFVec2f* const set_scale;
		MFVec3f* const set_spine;
		SFBool* const beginCap;
		SFBool* const endCap;
		SFBool* const solid;
		SFBool* const ccw;
		SFBool* const convex;
		SFFloat* const creaseAngle;
		MFVec2f* const crossSection;
		MFRotation* const orientation;
		MFVec2f* const scale;
		MFVec3f* const spine;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
