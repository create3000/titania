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

#ifndef __TITANIA_X3D_COMPONENTS_TEXT_X3DFONT_STYLE_NODE_H__
#define __TITANIA_X3D_COMPONENTS_TEXT_X3DFONT_STYLE_NODE_H__

#include "../Core/X3DNode.h"
#include "../../Types/Geometry.h"
#include "../../Miscellaneous/FontConfig.h"

namespace titania {
namespace X3D {

class Text;
class X3DFontStyleNode;

class X3DTextGeometry
{
public:

	X3DTextGeometry ();

	const Box3d &
	getBBox () const
	{ return bbox; }

	virtual
	bool
	isTransparent () const = 0;

	virtual
	void
	display ();

	virtual
	~X3DTextGeometry ();


protected:

	void
	initialize (Text* const, const X3DFontStyleNode* const);

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
	getTranslation () const
	{ return translation; }

	virtual
	void
	getLineExtends (const String &, Vector2d &, Vector2d &) const = 0;

	void
	compile (Text* const);

	virtual
	void
	draw () = 0;


private:

	void
	getLineExtends (const X3DFontStyleNode* const, const String &, Vector2d &, Vector2d &) const;

	Box3d bbox;

	std::vector <double>   charSpacings;
	Vector2d               bearing;
	Vector2d               minorAlignment;
	std::vector <Vector2d> translation;
	GLuint                 listId;

};

class X3DFontStyleNode :
	public X3DNode
{
public:

	enum Alignment
	{
		BEGIN,
		FIRST,
		MIDDLE,
		END
	};

	///  @name Fields

	MFString &
	family ()
	{ return *fields .family; }

	const MFString &
	family () const
	{ return *fields .family; }

	SFString &
	style ()
	{ return *fields .style; }

	const SFString &
	style () const
	{ return *fields .style; }

	SFFloat &
	spacing ()
	{ return *fields .spacing; }

	const SFFloat &
	spacing () const
	{ return *fields .spacing; }

	SFBool &
	horizontal ()
	{ return *fields .horizontal; }

	const SFBool &
	horizontal () const
	{ return *fields .horizontal; }

	MFString &
	justify ()
	{ return *fields .justify; }

	const MFString &
	justify () const
	{ return *fields .justify; }

	SFBool &
	topToBottom ()
	{ return *fields .topToBottom; }

	const SFBool &
	topToBottom () const
	{ return *fields .topToBottom; }

	SFBool &
	leftToRight ()
	{ return *fields .leftToRight; }

	const SFBool &
	leftToRight () const
	{ return *fields .leftToRight; }

	SFString &
	language ()
	{ return *fields .language; }

	const SFString &
	language () const
	{ return *fields .language; }

	///  @name Member access

	virtual
	std::shared_ptr <X3DTextGeometry>
	getTextGeometry (Text* const) const = 0;

	bool
	isItalic () const
	{ return italic; }

	bool
	isBold () const
	{ return bold; }

	virtual
	double
	getLineHeight () const = 0;

	virtual
	double
	getScale () const = 0;

	Alignment
	getMajorAlignment () const
	{ return alignments [0]; }

	Alignment
	getMinorAlignment () const
	{ return alignments [1]; }


protected:

	///  @name Construction

	X3DFontStyleNode ();

	virtual
	void
	initialize () override;

	Font
	getFont () const;

	Font
	getFont (const String &, bool &) const;


private:

	///  @name Member access

	Alignment
	getAlignment (const size_t, const bool) const;

	///  @name Event handlers

	void
	set_style ();

	void
	set_justify ();

	///  @name Members

	struct Fields
	{
		Fields ();

		MFString* const family;
		SFString* const style;
		SFFloat* const spacing;
		SFBool* const horizontal;
		MFString* const justify;
		SFBool* const topToBottom;
		SFBool* const leftToRight;
		SFString* const language;
	};

	Fields fields;

	bool italic;
	bool bold;

	std::array <Alignment, 2> alignments;

};

} // X3D
} // titania

#endif
