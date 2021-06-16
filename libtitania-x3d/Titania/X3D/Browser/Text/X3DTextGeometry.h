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

#ifndef __TITANIA_X3D_BROWSER_TEXT_X3DTEXT_GEOMETRY_H__
#define __TITANIA_X3D_BROWSER_TEXT_X3DTEXT_GEOMETRY_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Types/Geometry.h"

namespace titania {
namespace X3D {

class Text;
class X3DFontStyleNode;
class ShapeContainer;

class X3DTextGeometry :
	virtual public X3DBaseNode
{
public:

	virtual
	bool
	isTransparent () const = 0;

	virtual
	const Matrix4d &
	getMatrix () const
	{ return matrix; }

	virtual
	const Box3d &
	getBBox () const
	{ return bbox; }

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override
	{ }

	virtual
	void
	draw (ShapeContainer* const shapeContainer);

	virtual
	SFNode
	toPrimitive () const = 0;

	virtual
	~X3DTextGeometry () override;


protected:

	X3DTextGeometry (Text* const, const X3DFontStyleNode* const);

	void
	initialize () final override;

	X3DBrowser*
	getBrowser () const final override;

	Text*
	getText () const
	{ return text; }

	void
	setBBox (const Box3d & value)
	{ bbox = value; }

	const std::vector <double> &
	getCharSpacing () const
	{ return charSpacings; }

	const Vector2d &
	getBearing () const
	{ return bearing; }

	const Vector2d &
	getMinorAlignment () const
	{ return minorAlignment; }

	const std::vector <Vector2d> &
	getTranslations () const
	{ return translations; }

	virtual
	void
	getLineExtents (const String & line, Vector2d & min, Vector2d & max) const = 0;

	virtual
	void
	build () = 0;


private:

	void
	horizontal ();

	void
	vertical ();

	void
	getHorizontalLineExtents (const String &, Vector2d &, Vector2d &) const;

	void
	getGlyphExtents (const String::value_type &, Vector2d &, Vector2d &) const;

	///  @name Static members

	static const Matrix4d matrix;;

	///  @name Members

	Text* const                   text;
	const X3DFontStyleNode* const fontStyle;
	Box3d                         bbox;
	std::vector <double>          charSpacings;
	Vector2d                      bearing;
	Vector2d                      minorAlignment;
	std::vector <Vector2d>        translations;

};

} // X3D
} // titania

#endif
