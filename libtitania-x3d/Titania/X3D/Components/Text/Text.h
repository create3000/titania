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

#ifndef __TITANIA_X3D_COMPONENTS_TEXT_TEXT_H__
#define __TITANIA_X3D_COMPONENTS_TEXT_TEXT_H__

#include "../Rendering/X3DGeometryNode.h"

namespace titania {
namespace X3D {

class X3DTextGeometry;

class Text :
	virtual public X3DGeometryNode
{
public:

	///  @name Construction

	Text (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Fields

	virtual
	MFString &
	string ()
	{ return *fields .string; }

	virtual
	const MFString &
	string () const
	{ return *fields .string; }

	virtual
	MFFloat &
	length ()
	{ return *fields .length; }

	virtual
	const MFFloat &
	length () const
	{ return *fields .length; }

	virtual
	SFFloat &
	maxExtent ()
	{ return *fields .maxExtent; }

	virtual
	const SFFloat &
	maxExtent () const
	{ return *fields .maxExtent; }

	virtual
	SFBool &
	solid ()
	{ return *fields .solid; }

	virtual
	const SFBool &
	solid () const
	{ return *fields .solid; }

	virtual
	SFVec3f &
	origin ()
	{ return *fields .origin; }

	virtual
	const SFVec3f &
	origin () const
	{ return *fields .origin; }

	virtual
	SFVec2f &
	textBounds ()
	{ return *fields .textBounds; }

	virtual
	const SFVec2f &
	textBounds () const
	{ return *fields .textBounds; }

	virtual
	MFVec2f &
	lineBounds ()
	{ return *fields .lineBounds; }

	virtual
	const MFVec2f &
	lineBounds () const
	{ return *fields .lineBounds; }

	virtual
	SFNode &
	fontStyle ()
	{ return *fields .fontStyle; }

	virtual
	const SFNode &
	fontStyle () const
	{ return *fields .fontStyle; }

	///  @name Member access

	virtual
	const Box3d &
	getBBox () const override;

	virtual
	bool
	isTransparent () const;

	virtual
	float
	getLength (const size_t index);

	virtual
	const X3DPtr <X3DFontStyleNode> &
	getFontStyle () const
	{ return fontStyleNode; }

	virtual
	const X3DPtr <X3DTextGeometry> &
	getTextGeometry () const
	{ return textGeometry; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	virtual
	void
	draw (ShapeContainer* const context) override;

	virtual
	NodeType
	getPrimitiveType () const final override
	{ return X3DConstants::IndexedFaceSet; }

	virtual
	SFNode
	toPrimitive () const override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~Text () override;


protected:

	///  @name Friends

	friend class PolygonText;
	friend class ScreenText;

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Member access
	virtual
	Box3d
	createBBox () const override;

	virtual
	const Matrix4d &
	getMatrix () const final override;

	///  @name Event handlers

	void
	set_fontStyle ();

	///  @name Operations

	virtual
	void
	build () final override;

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFString* const string;
		MFFloat* const length;
		SFFloat* const maxExtent;
		SFBool* const solid;
		SFVec3f* const origin;
		SFVec2f* const textBounds;
		MFVec2f* const lineBounds;
		SFNode* const fontStyle;
	};

	Fields fields;

	X3DPtr <X3DFontStyleNode> fontStyleNode;
	X3DPtr <X3DTextGeometry>  textGeometry;

};

} // X3D
} // titania

#endif
