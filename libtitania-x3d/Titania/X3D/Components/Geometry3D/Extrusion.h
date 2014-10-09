/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_COMPONENTS_GEOMETRY3D_EXTRUSION_H__
#define __TITANIA_X3D_COMPONENTS_GEOMETRY3D_EXTRUSION_H__

#include "../../Rendering/Tessellator.h"
#include "../Rendering/X3DGeometryNode.h"

namespace titania {
namespace X3D {

class Extrusion :
	public X3DGeometryNode
{
public:

	///  @name Construction

	Extrusion (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Fields

	SFBool &
	beginCap ()
	{ return *fields .beginCap; }

	const SFBool &
	beginCap () const
	{ return *fields .beginCap; }

	SFBool &
	endCap ()
	{ return *fields .endCap; }

	const SFBool &
	endCap () const
	{ return *fields .endCap; }

	SFBool &
	solid ()
	{ return *fields .solid; }

	const SFBool &
	solid () const
	{ return *fields .solid; }

	SFBool &
	ccw ()
	{ return *fields .ccw; }

	const SFBool &
	ccw () const
	{ return *fields .ccw; }

	SFBool &
	convex ()
	{ return *fields .convex; }

	const SFBool &
	convex () const
	{ return *fields .convex; }

	SFFloat &
	creaseAngle ()
	{ return *fields .creaseAngle; }

	const SFFloat &
	creaseAngle () const
	{ return *fields .creaseAngle; }

	MFVec2f &
	crossSection ()
	{ return *fields .crossSection; }

	const MFVec2f &
	crossSection () const
	{ return *fields .crossSection; }

	MFRotation &
	orientation ()
	{ return *fields .orientation; }

	const MFRotation &
	orientation () const
	{ return *fields .orientation; }

	MFVec2f &
	scale ()
	{ return *fields .scale; }

	const MFVec2f &
	scale () const
	{ return *fields .scale; }

	MFVec3f &
	spine ()
	{ return *fields .spine; }

	const MFVec3f &
	spine () const
	{ return *fields .spine; }

	///  @name Member access

	virtual
	bool
	isLineGeometry () const final override
	{ return false; }

	///  @name Operations

	virtual
	SFNode
	toPrimitive () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) final override;


private:

	typedef opengl::tessellator <size_t, size_t> Tessellator;

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Operations

	std::vector <Vector3f>
	createPoints (const bool);

	std::vector <Matrix4f>
	createRotations ();

	virtual
	void
	build () final override;

	void
	tessellateCap (const Tessellator &,
	              std::vector <Vector3f> &,
	              const Vector2f &,
	              const float);


	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

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
