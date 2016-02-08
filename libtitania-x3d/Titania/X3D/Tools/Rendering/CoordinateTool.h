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

#ifndef __TITANIA_X3D_TOOLS_RENDERING_COORDINATE_TOOL_H__
#define __TITANIA_X3D_TOOLS_RENDERING_COORDINATE_TOOL_H__

#include "../../Rendering/ShapeContainer.h"
#include "../Rendering/X3DCoordinateNodeTool.h"

namespace titania {
namespace X3D {

class CoordinateTool :
	public X3DCoordinateNodeTool
{
public:

	///  @name Construction

	CoordinateTool (X3DExecutionContext* const);

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
	enabled ()
	{ return *fields .enabled; }

	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	SFMatrix4f &
	modelViewMatrix ()
	{ return *fields .modelViewMatrix; }

	const SFMatrix4f &
	modelViewMatrix () const
	{ return *fields .modelViewMatrix; }

	SFColorRGBA &
	color ()
	{ return *fields .color; }

	const SFColorRGBA &
	color () const
	{ return *fields .color; }

	MFInt32 &
	vertexCount ()
	{ return *fields .vertexCount; }

	const MFInt32 &
	vertexCount () const
	{ return *fields .vertexCount; }

	MFVec3f &
	point ()
	{ return *fields .point; }

	const MFVec3f &
	point () const
	{ return *fields .point; }

	SFNode &
	geometry ()
	{ return *fields .geometry; }

	const SFNode &
	geometry () const
	{ return *fields .geometry; }

	SFVec3f &
	hitPoint_changed ()
	{ return *fields .hitPoint_changed; }

	const SFVec3f &
	hitPoint_changed () const
	{ return *fields .hitPoint_changed; }

	SFBool &
	isActive ()
	{ return *fields .isActive; }

	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	SFTime &
	touchTime ()
	{ return *fields .touchTime; }

	const SFTime &
	touchTime () const
	{ return *fields .touchTime; }

	///  @name Operations

	void
	draw (const ShapeContainer* const);


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	realize () final override;

	///  @name Event handler

	void
	prepareEvent ();

	void
	set_color ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const enabled;
		SFMatrix4f* const modelViewMatrix;
		SFColorRGBA* const color;
		MFInt32* const vertexCount;
		MFVec3f* const point;
		SFNode* const geometry;
		SFVec3f* const hitPoint_changed;
		SFBool* const isActive;
		SFTime* const touchTime;
	};

	Fields fields;

	bool show;

};

} // X3D
} // titania

#endif
