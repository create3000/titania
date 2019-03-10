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

#ifndef __TITANIA_X3D_TOOLS_TEXT_TEXT_TOOL_H__
#define __TITANIA_X3D_TOOLS_TEXT_TEXT_TOOL_H__

#include "../../Components/Text/Text.h"
#include "../Rendering/X3DGeometryNodeTool.h"

namespace titania {
namespace X3D {

class TextTool :
	virtual public Text,
	public X3DGeometryNodeTool
{
public:

	///  @name Construction

	TextTool (X3DBaseNode* const node);

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override
	{ X3DGeometryNodeTool::setExecutionContext (executionContext); }

	///  @name Fields

	virtual
	MFString &
	string () final override
	{ return getNode <Text> () -> string (); }

	virtual
	const MFString &
	string () const final override
	{ return getNode <Text> () -> string (); }

	virtual
	MFFloat &
	length () final override
	{ return getNode <Text> () -> length (); }

	virtual
	const MFFloat &
	length () const final override
	{ return getNode <Text> () -> length (); }

	virtual
	SFFloat &
	maxExtent () final override
	{ return getNode <Text> () -> maxExtent (); }

	virtual
	const SFFloat &
	maxExtent () const final override
	{ return getNode <Text> () -> maxExtent (); }

	virtual
	SFBool &
	solid () final override
	{ return getNode <Text> () -> solid (); }

	virtual
	const SFBool &
	solid () const final override
	{ return getNode <Text> () -> solid (); }

	virtual
	SFVec3f &
	origin () final override
	{ return getNode <Text> () -> origin (); }

	virtual
	const SFVec3f &
	origin () const final override
	{ return getNode <Text> () -> origin (); }

	virtual
	SFVec2f &
	textBounds () final override
	{ return getNode <Text> () -> textBounds (); }

	virtual
	const SFVec2f &
	textBounds () const final override
	{ return getNode <Text> () -> textBounds (); }

	virtual
	MFVec2f &
	lineBounds () final override
	{ return getNode <Text> () -> lineBounds (); }

	virtual
	const MFVec2f &
	lineBounds () const final override
	{ return getNode <Text> () -> lineBounds (); }

	virtual
	SFNode &
	fontStyle () final override
	{ return getNode <Text> () -> fontStyle (); }

	virtual
	const SFNode &
	fontStyle () const final override
	{ return getNode <Text> () -> fontStyle (); }

	///  @name Member access

	virtual
	const Box3d &
	getBBox () const final override
	{ return getNode <Text> () -> getBBox (); }

	virtual
	float
	getLength (const size_t index) final override
	{ return getNode <Text> () -> getLength (index); }

	virtual
	const X3DPtr <X3DFontStyleNode> &
	getFontStyle () const final override
	{ return getNode <Text> () -> getFontStyle (); }

	virtual
	const X3DPtr <X3DTextGeometry> &
	getTextGeometry () const final override
	{ return getNode <Text> () -> getTextGeometry (); }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{ return X3DGeometryNodeTool::traverse (type, renderObject); }

	virtual
	void
	draw (ShapeContainer* const context) final override
	{ return X3DGeometryNodeTool::draw (context); }

	virtual
	SFNode
	toPrimitive () const final override
	{ return getNode <Text> () -> toPrimitive (); }

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~TextTool () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Construction

	virtual
	Box3d
	createBBox () const final override
	{ return Box3d (); }

	///  @name Members

	struct Fields
	{
		Fields ();

	};

	Fields fields;

};

} // X3D
} // titania

#endif
