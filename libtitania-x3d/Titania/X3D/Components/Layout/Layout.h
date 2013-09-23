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

#ifndef __TITANIA_X3D_COMPONENTS_LAYOUT_LAYOUT_H__
#define __TITANIA_X3D_COMPONENTS_LAYOUT_LAYOUT_H__

#include "../Layout/X3DLayoutNode.h"

namespace titania {
namespace X3D {

class Layout :
	public X3DLayoutNode
{
public:

	///  @name Construction

	Layout (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final
	{ return containerField; }

	///  @name Fields

	MFString &
	align ()
	{ return *fields .align; }

	const MFString &
	align () const
	{ return *fields .align; }

	MFString &
	offsetUnits ()
	{ return *fields .offsetUnits; }

	const MFString &
	offsetUnits () const
	{ return *fields .offsetUnits; }

	MFFloat &
	offset ()
	{ return *fields .offset; }

	const MFFloat &
	offset () const
	{ return *fields .offset; }

	MFString &
	sizeUnits ()
	{ return *fields .sizeUnits; }

	const MFString &
	sizeUnits () const
	{ return *fields .sizeUnits; }

	MFFloat &
	size ()
	{ return *fields .size; }

	const MFFloat &
	size () const
	{ return *fields .size; }

	MFString &
	scaleMode ()
	{ return *fields .scaleMode; }

	const MFString &
	scaleMode () const
	{ return *fields .scaleMode; }

	///  @name Member access

	virtual
	HorizontalAlignType
	getAlignX () const final
	{ return alignX; }

	virtual
	VerticalAlignType
	getAlignY () const final
	{ return alignY; }

	virtual
	SizeUnitType
	getOffsetUnitX () const final;

	virtual
	SizeUnitType
	getOffsetUnitY () const final;

	float
	getOffsetX () const
	{ return offsetX; }

	float
	getOffsetY () const
	{ return offsetY; }

	virtual
	SizeUnitType
	getSizeUnitX () const final;

	virtual
	SizeUnitType
	getSizeUnitY () const final;

	float
	getSizeX () const
	{ return sizeX; }

	float
	getSizeY () const
	{ return sizeY; }

	ScaleModeType
	getScaleModeX () const;

	ScaleModeType
	getScaleModeY () const;

	virtual
	Vector2f
	getRectangleCenter () const final;

	virtual
	Vector2f
	getRectangleSize () const final;

	///  @name Operations

	virtual
	void
	transform (const TraverseType type) final;


private:

	///  @name Construction

	virtual
	void
	initialize () final;

	///  @name Event handler

	void
	set_align ();

	void
	set_offsetUnits ();

	void
	set_offset ();

	void
	set_scaleMode ();

	void
	set_sizeUnits ();

	void
	set_size ();


	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFString* const align;
		MFString* const offsetUnits;
		MFFloat* const offset;
		MFString* const sizeUnits;
		MFFloat* const size;
		MFString* const scaleMode;
	};

	Fields fields;

	HorizontalAlignType alignX;
	VerticalAlignType   alignY;
	SizeUnitType        offsetUnitX;
	SizeUnitType        offsetUnitY;
	float               offsetX;
	float               offsetY;
	SizeUnitType        sizeUnitX;
	SizeUnitType        sizeUnitY;
	float               sizeX;
	float               sizeY;
	ScaleModeType       scaleModeX;
	ScaleModeType       scaleModeY;

	X3DLayoutNode* parent;
	Vector4i       viewport;
	Vector2f       rectangleCenter;   // In m
	Vector2f       rectangleSize;     // In m

};

} // X3D
} // titania

#endif
