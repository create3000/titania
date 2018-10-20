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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_ELEVATION_GRID_ELEVATION_GRID_TOOL_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_ELEVATION_GRID_ELEVATION_GRID_TOOL_H__

#include "../../../Components/Geometry3D/ElevationGrid.h"
#include "../../Rendering/X3DGeometryNodeTool.h"

namespace titania {
namespace X3D {

class ElevationGridTool :
	virtual public ElevationGrid,
	public X3DGeometryNodeTool
{
public:

	///  @name Construction

	ElevationGridTool (X3DBaseNode* const node);

	///  @name Fields

	virtual
	SFInt32 &
	xDimension () final override
	{ return getNode <ElevationGrid> () -> xDimension (); }

	virtual
	const SFInt32 &
	xDimension () const final override
	{ return getNode <ElevationGrid> () -> xDimension (); }

	virtual
	SFInt32 &
	zDimension () final override
	{ return getNode <ElevationGrid> () -> zDimension (); }

	virtual
	const SFInt32 &
	zDimension () const final override
	{ return getNode <ElevationGrid> () -> zDimension (); }

	virtual
	SFFloat &
	xSpacing () final override
	{ return getNode <ElevationGrid> () -> xSpacing (); }

	virtual
	const SFFloat &
	xSpacing () const final override
	{ return getNode <ElevationGrid> () -> xSpacing (); }

	virtual
	SFFloat &
	zSpacing () final override
	{ return getNode <ElevationGrid> () -> zSpacing (); }

	virtual
	const SFFloat &
	zSpacing () const final override
	{ return getNode <ElevationGrid> () -> zSpacing (); }

	virtual
	SFFloat &
	creaseAngle () final override
	{ return getNode <ElevationGrid> () -> creaseAngle (); }

	virtual
	const SFFloat &
	creaseAngle () const final override
	{ return getNode <ElevationGrid> () -> creaseAngle (); }

	virtual
	SFBool &
	solid () final override
	{ return getNode <ElevationGrid> () -> solid (); }

	virtual
	const SFBool &
	solid () const final override
	{ return getNode <ElevationGrid> () -> solid (); }

	virtual
	SFBool &
	ccw () final override
	{ return getNode <ElevationGrid> () -> ccw (); }

	virtual
	const SFBool &
	ccw () const final override
	{ return getNode <ElevationGrid> () -> ccw (); }

	virtual
	SFBool &
	colorPerVertex () final override
	{ return getNode <ElevationGrid> () -> colorPerVertex (); }

	virtual
	const SFBool &
	colorPerVertex () const final override
	{ return getNode <ElevationGrid> () -> colorPerVertex (); }

	virtual
	SFBool &
	normalPerVertex () final override
	{ return getNode <ElevationGrid> () -> normalPerVertex (); }

	virtual
	const SFBool &
	normalPerVertex () const final override
	{ return getNode <ElevationGrid> () -> normalPerVertex (); }

	virtual
	MFNode &
	attrib () final override
	{ return getNode <ElevationGrid> () -> attrib (); }

	virtual
	const MFNode &
	attrib () const final override
	{ return getNode <ElevationGrid> () -> attrib (); }

	virtual
	SFNode &
	fogCoord () final override
	{ return getNode <ElevationGrid> () -> fogCoord (); }

	virtual
	const SFNode &
	fogCoord () const final override
	{ return getNode <ElevationGrid> () -> fogCoord (); }

	virtual
	SFNode &
	color () final override
	{ return getNode <ElevationGrid> () -> color (); }

	virtual
	const SFNode &
	color () const final override
	{ return getNode <ElevationGrid> () -> color (); }

	virtual
	SFNode &
	texCoord () final override
	{ return getNode <ElevationGrid> () -> texCoord (); }

	virtual
	const SFNode &
	texCoord () const final override
	{ return getNode <ElevationGrid> () -> texCoord (); }

	virtual
	SFNode &
	normal () final override
	{ return getNode <ElevationGrid> () -> normal (); }

	virtual
	const SFNode &
	normal () const final override
	{ return getNode <ElevationGrid> () -> normal (); }

	virtual
	MFFloat &
	height () final override
	{ return getNode <ElevationGrid> () -> height (); }

	virtual
	const MFFloat &
	height () const final override
	{ return getNode <ElevationGrid> () -> height (); }

	///  @name Member access

	virtual
	bool
	isTransparent () const final override
	{ return getNode <ElevationGrid> () -> isTransparent (); }

	///  @name Height map handling

	virtual
	std::pair <float, float>
	getMinMaxHeight () const final override
	{ return getNode <ElevationGrid> () -> getMinMaxHeight (); }

	virtual
	void
	loadHeightMap (const MFString & url, const float minHeight, const float maxHeight) final override
	{ getNode <ElevationGrid> () -> loadHeightMap (url, minHeight, maxHeight); }

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const final override
	{ return getNode <ElevationGrid> () -> checkLoadState (); }

	///  @name Operations

	virtual
	void
	addNormals () final override
	{ getNode <ElevationGrid> () -> addNormals (); }

	virtual
	SFNode
	toPrimitive () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) final override
	{ return getNode <ElevationGrid> () -> toPrimitive (); }

	virtual
	void
	beginUndo () final override;

	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~ElevationGridTool () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Event handlers

	void
	set_transform_tools ();
	
	void
	set_xSpacing ();
	
	void
	set_zSpacing ();

	void
	set_height ();
	
	void
	set_scale ();

	void
	set_yScale (const float yScale);

	void
	connectXSpacing (const SFFloat & field);
	
	void
	connectZSpacing (const SFFloat & field);
	
	void
	connectHeight (const MFFloat & field);
	
	void
	connectScale ();

	///  @name Fields

	struct Fields
	{
		Fields ();

	};

	Fields fields;

	///  @name Members

	float                    startXSpacing;
	float                    startZSpacing;
	MFFloat                  startHeight;
	std::pair <float, float> startMinMaxHeight;

};

} // X3D
} // titania

#endif
