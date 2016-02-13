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

#ifndef __TITANIA_X3D_EDITING_SELECTION_FACE_SELECTION_H__
#define __TITANIA_X3D_EDITING_SELECTION_FACE_SELECTION_H__

#include "../../Editing/Selection/X3DFaceSelection.h"
#include "../../Fields/X3DPtr.h"
#include "../../Types/Geometry.h"
#include "../../Types/Numbers.h"

namespace titania {
namespace X3D {

class IndexedFaceSet;
class X3DCoordinateNode;

class FaceSelection :
	public X3DFaceSelection
{
public:

	///  @name Member types

	struct Edge
	{
		int32_t index0;
		int32_t index1;
		Vector3d point0;
		Vector3d point1;
		Line3d line;
	};

	///  @name Construction

	FaceSelection (X3DExecutionContext* const);

	virtual
	FaceSelection*
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

	///  @name Member access

	void
	setGeometry (const X3DPtr <IndexedFaceSet> &);

	void
	setCoord (const X3DPtr <X3DCoordinateNode> &);

	void
	setCoincidentPoints (const Vector3d &);

	void
	setCoincidentPoints (const Vector3d &, const MFVec3f &);

	void
	setAdjacentFaces (const Vector3d &);

	const std::vector <size_t> &
	getCoincidentPoints () const
	{ return indices; }

	std::vector <size_t>
	getVertices (const size_t) const;

	const std::pair <size_t, size_t> &
	getFace () const
	{ return face; }

	const std::vector <std::pair <size_t, size_t>> &
	getAdjacentFaces () const
	{ return faces; }

	std::set <size_t>
	getFaces () const;

	Edge
	getEdge (const std::vector <size_t> &, const Vector3d &) const;

	bool
	isEdge (const std::vector <size_t> &, const int32_t, const int32_t) const;

	///  @name Destruction

	virtual
	~FaceSelection ();


private:

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	X3DPtr <IndexedFaceSet>                              geometry;
	X3DPtr <X3DCoordinateNode>                           coord;
	std::vector <size_t>                                 indices;
	std::multimap <Vector3d, size_t>                     pointIndex;
	std::multimap <int32_t, std::pair <size_t, size_t>>  faceIndex;
	std::pair <size_t, size_t>                           face;
	std::vector <std::pair <size_t, size_t>>             faces;
	std::vector <int32_t>                                triangle;

};

} // X3D
} // titania

#endif
