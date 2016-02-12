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

#ifndef __TITANIA_X3D_EDITING_FACE_SELECTION_H__
#define __TITANIA_X3D_EDITING_FACE_SELECTION_H__

#include "../Editing/X3DFaceSelection.h"

#include <Titania/X3D/Components/Geometry3D/IndexedFaceSet.h>

namespace titania {
namespace X3D {

class FaceSelection :
	public X3DFaceSelection
{
public:

	///  @name Construction

	FaceSelection ();

	///  @name Member access

	void
	setGeometry (const X3DPtr <IndexedFaceSet> &);

	virtual
	void
	setCoord (const X3DPtr <X3DCoordinateNode> &) final override;

	virtual
	void
	setIndices (const Vector3d &) final override;

	virtual
	void
	setIndices (const Vector3d &, const MFVec3f &) final override;

	virtual
	void
	setFaces (const Vector3d &) final override;

	virtual
	const std::vector <size_t> &
	getIndices () const final override
	{ return indices; }

	virtual
	std::vector <size_t>
	getVertices (const size_t) const final override;

	virtual
	const std::pair <size_t, size_t> &
	getFace () const final override
	{ return face; }

	virtual
	const std::vector <std::pair <size_t, size_t>> &
	getAdjacentFaces () const final override
	{ return faces; }

	virtual
	std::set <size_t>
	getFaces () const final override;

	virtual
	Edge
	getEdge (const std::vector <size_t> &, const Vector3d &) const final override;

	virtual
	bool
	isEdge (const std::vector <size_t> &, const int32_t, const int32_t) const final override;

	///  @name Destruction

	~FaceSelection ();


private:

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
