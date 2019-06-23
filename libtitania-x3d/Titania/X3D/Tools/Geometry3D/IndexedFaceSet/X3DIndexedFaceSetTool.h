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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_INDEXED_FACE_SET_X3DINDEXED_FACE_SET_TOOL_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_INDEXED_FACE_SET_X3DINDEXED_FACE_SET_TOOL_H__

#include "../../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Rendering/X3DComposedGeometryNodeTool.h"

namespace titania {
namespace X3D {

class X3DIndexedFaceSetTool :
	virtual public IndexedFaceSet,
	public X3DComposedGeometryNodeTool
{
public:

	///  @name Fields

	virtual
	MFInt32 &
	set_colorIndex () final override
	{ return getNode <IndexedFaceSet> () -> set_colorIndex (); }

	virtual
	const MFInt32 &
	set_colorIndex () const final override
	{ return getNode <IndexedFaceSet> () -> set_colorIndex (); }

	virtual
	MFInt32 &
	set_texCoordIndex () final override
	{ return getNode <IndexedFaceSet> () -> set_texCoordIndex (); }

	virtual
	const MFInt32 &
	set_texCoordIndex () const final override
	{ return getNode <IndexedFaceSet> () -> set_texCoordIndex (); }

	virtual
	MFInt32 &
	set_normalIndex () final override
	{ return getNode <IndexedFaceSet> () -> set_normalIndex (); }

	virtual
	const MFInt32 &
	set_normalIndex () const final override
	{ return getNode <IndexedFaceSet> () -> set_normalIndex (); }

	virtual
	MFInt32 &
	set_coordIndex () final override
	{ return getNode <IndexedFaceSet> () -> set_coordIndex (); }

	virtual
	const MFInt32 &
	set_coordIndex () const final override
	{ return getNode <IndexedFaceSet> () -> set_coordIndex (); }

	virtual
	SFBool &
	convex () final override
	{ return getNode <IndexedFaceSet> () -> convex (); }

	virtual
	const SFBool &
	convex () const final override
	{ return getNode <IndexedFaceSet> () -> convex (); }

	virtual
	SFFloat &
	creaseAngle () final override
	{ return getNode <IndexedFaceSet> () -> creaseAngle (); }

	virtual
	const SFFloat &
	creaseAngle () const final override
	{ return getNode <IndexedFaceSet> () -> creaseAngle (); }

	virtual
	MFInt32 &
	colorIndex () final override
	{ return getNode <IndexedFaceSet> () -> colorIndex (); }

	virtual
	const MFInt32 &
	colorIndex () const final override
	{ return getNode <IndexedFaceSet> () -> colorIndex (); }

	virtual
	MFInt32 &
	texCoordIndex () final override
	{ return getNode <IndexedFaceSet> () -> texCoordIndex (); }

	virtual
	const MFInt32 &
	texCoordIndex () const final override
	{ return getNode <IndexedFaceSet> () -> texCoordIndex (); }

	virtual
	MFInt32 &
	normalIndex () final override
	{ return getNode <IndexedFaceSet> () -> normalIndex (); }

	virtual
	const MFInt32 &
	normalIndex () const final override
	{ return getNode <IndexedFaceSet> () -> normalIndex (); }

	virtual
	MFInt32 &
	coordIndex () final override
	{ return getNode <IndexedFaceSet> () -> coordIndex (); }

	virtual
	const MFInt32 &
	coordIndex () const final override
	{ return getNode <IndexedFaceSet> () -> coordIndex (); }

	///  @name Hidden fields

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

	///  @name Member access

	virtual
	size_t
	getGeometryType () const final override
	{ return getNode <IndexedFaceSet> () -> getGeometryType (); }

	///  @name Operations

	virtual
	bool
	isColorPerVertex () const final override
	{ return getNode <IndexedFaceSet> () -> isColorPerVertex (); }

	virtual
	bool
	isNormalPerVertex () const final override
	{ return getNode <IndexedFaceSet> () -> isNormalPerVertex (); }

	virtual
	MFInt32
	getColorIndex (const bool colorPerVertex) const final override
	{ return getNode <IndexedFaceSet> () -> getColorIndex (colorPerVertex); }

	virtual
	MFInt32
	getNormalIndex (const bool normalPerVertex) const final override
	{ return getNode <IndexedFaceSet> () -> getNormalIndex (normalPerVertex); }

	virtual
	void
	addColorIndex () final override
	{ getNode <IndexedFaceSet> () -> addColorIndex (); }

	virtual
	void
	addTexCoordIndex () final override
	{ getNode <IndexedFaceSet> () -> addTexCoordIndex (); }

	virtual
	void
	addNormalIndex () final override
	{ getNode <IndexedFaceSet> () -> addNormalIndex (); }

	virtual
	void
	addFogCoords () final override
	{ getNode <IndexedFaceSet> () -> addFogCoords (); }

	virtual
	void
	addColors () final override
	{ getNode <IndexedFaceSet> () -> addColors (); }

	virtual
	void
	addTexCoords () final override
	{ getNode <IndexedFaceSet> () -> addTexCoords (); }

	virtual
	void
	addNormals () final override
	{ getNode <IndexedFaceSet> () -> addNormals (); }

	virtual
	SFNode
	toPrimitive () const final override
	{ return getNode <IndexedFaceSet> () -> toPrimitive (); }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DIndexedFaceSetTool () override;


protected:

	///  @name Member types

	struct ToolNumber
	{
		static const int32_t NONE;
		static const int32_t SELECT ;
		static const int32_t CUT;
		static const int32_t SCULP;
	};

	///  @name Construction

	X3DIndexedFaceSetTool ();

	virtual
	void
	initialize () override;

	void
	set_loadState ();

	///  @name Operations

	void
	rewriteArray (const std::map <int32_t, int32_t> &, std::vector <int32_t> &) const;

	void
	undoSetColorIndex (const UndoStepPtr &);

	void
	redoSetColorIndex (const UndoStepPtr &);

	void
	undoSetTexCoordIndex (const UndoStepPtr &);

	void
	redoSetTexCoordIndex (const UndoStepPtr &);

	void
	undoSetNormalIndex (const UndoStepPtr &);

	void
	redoSetNormalIndex (const UndoStepPtr &);

	void
	undoSetCoordIndex (const UndoStepPtr &);

	void
	redoSetCoordIndex (const UndoStepPtr &);


private:

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const isActive;
		SFTime* const touchTime;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
