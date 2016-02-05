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

#ifndef __TITANIA_X3D_TOOLS_RENDERING_X3DCOMPOSED_GEOMETRY_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_RENDERING_X3DCOMPOSED_GEOMETRY_NODE_TOOL_H__

#include "../Rendering/X3DGeometryNodeTool.h"

namespace titania {
namespace X3D {

template <class Type>
class X3DComposedGeometryNodeTool :
	virtual public X3DGeometryNodeTool <Type>
{
public:

	///  @name Fields

	virtual
	SFBool &
	colorPerVertex () final override
	{ return getNode () -> colorPerVertex (); }

	virtual
	const SFBool &
	colorPerVertex () const final override
	{ return getNode () -> colorPerVertex (); }

	virtual
	SFBool &
	normalPerVertex () final override
	{ return getNode () -> normalPerVertex (); }

	virtual
	const SFBool &
	normalPerVertex () const final override
	{ return getNode () -> normalPerVertex (); }

	virtual
	SFBool &
	solid () final override
	{ return getNode () -> solid (); }

	virtual
	const SFBool &
	solid () const final override
	{ return getNode () -> solid (); }

	virtual
	SFBool &
	ccw () final override
	{ return getNode () -> ccw (); }

	virtual
	const SFBool &
	ccw () const final override
	{ return getNode () -> ccw (); }

	virtual
	MFNode &
	attrib () final override
	{ return getNode () -> attrib (); }

	virtual
	const MFNode &
	attrib () const final override
	{ return getNode () -> attrib (); }

	virtual
	SFNode &
	fogCoord () final override
	{ return getNode () -> fogCoord (); }

	virtual
	const SFNode &
	fogCoord () const final override
	{ return getNode () -> fogCoord (); }

	virtual
	SFNode &
	color () final override
	{ return getNode () -> color (); }

	virtual
	const SFNode &
	color () const final override
	{ return getNode () -> color (); }

	virtual
	SFNode &
	texCoord () final override
	{ return getNode () -> texCoord (); }

	virtual
	const SFNode &
	texCoord () const final override
	{ return getNode () -> texCoord (); }

	virtual
	SFNode &
	normal () final override
	{ return getNode () -> normal (); }

	virtual
	const SFNode &
	normal () const final override
	{ return getNode () -> normal (); }

	virtual
	SFNode &
	coord () final override
	{ return getNode () -> coord (); }

	virtual
	const SFNode &
	coord () const final override
	{ return getNode () -> coord (); }

	///  @name Operations

	virtual
	void
	addNormals () final override
	{ getNode () -> addNormals (); }


protected:

	using X3DNodeTool <Type>::addType;
	using X3DNodeTool <Type>::getNode;

	///  @name Construction

	X3DComposedGeometryNodeTool () :
		X3DGeometryNodeTool <Type> ()
	{
		addType (X3DConstants::X3DComposedGeometryNodeTool);
	}

};

} // X3D
} // titania

#endif
