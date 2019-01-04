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

#ifndef __TITANIA_X3D_EDITING_SELECTION_FACE_SELECTION_H__
#define __TITANIA_X3D_EDITING_SELECTION_FACE_SELECTION_H__

#include "../../Editing/Selection/X3DFaceSelection.h"

#include <Titania/Math/Geometry/LineSegment3.h>

namespace titania {
namespace X3D {

class IndexedFaceSet;
class X3DCoordinateNode;

class FaceSelection :
	public X3DFaceSelection
{
public:

	///  @name Construction

	FaceSelection (X3DExecutionContext* const executionContext);

	virtual
	FaceSelection*
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

	///  @name Fields

	SFNode &
	geometry ()
	{ return *fields .geometry; }

	const SFNode &
	geometry () const
	{ return *fields .geometry; }

	///  @name Operations

	std::vector <size_t>
	getFaces () const;

	size_t
	getNumFaces () const;

	size_t
	getFaceNumber (const size_t) const;

	Faces
	getAdjacentFaces (const int32_t) const;

	Faces
	getAdjacentFaces (const Points &) const;

	std::vector <size_t>
	getAdjacentFaces (const std::pair <size_t, size_t> &) const;

	Face
	getClosestFace (const Vector3d & , const Faces &) const;

	Face
	getClosestFace (const Vector3d & , const std::vector <size_t> &) const;

	double
	getFaceDistance (const Vector3d &, const std::vector <size_t> &) const;

	std::vector <size_t>
	getFaceVertices (const size_t) const;

	std::vector <std::pair <size_t, size_t>>
	getFaceEdges (const size_t) const;

	Vector3d
	getFaceCenter (const size_t) const;

	std::vector <std::pair <size_t, size_t>>
	getAdjacentEdges (const Points &) const;

	std::vector <std::pair <size_t, size_t>>
	getHorizonEdges (const std::vector <size_t> &) const;

	Edge
	getClosestEdge (const Vector3d &, const std::vector <size_t> &) const;

	///  throws std::domain_error
	std::pair <Edge, size_t>
	getClosestEdge (const LineSegment3d &, const std::vector <size_t> &) const;

	bool
	isEdge (const std::vector <size_t> & vertices, const size_t, const size_t) const;

	Points
	getCoincidentPoints (const Vector3d &) const;

	std::vector <size_t>
	getSharedVertices (const int32_t) const;

	std::vector <size_t>
	getVertices (const int32_t) const;

	size_t
	getClosestVertex (const Vector3d &, const std::vector <size_t> &) const;

	///  @name Destruction

	virtual
	~FaceSelection () final  override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handler

	void
	set_geometry (const SFNode &);

	void
	set_coordIndex (const MFInt32 &);

	void
	set_coord (const X3DPtr <X3DCoordinateNode> &);

	void
	set_point ();

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const geometry;
	};

	Fields fields;

	X3DPtr <IndexedFaceSet>    geometryNode;
	X3DPtr <X3DCoordinateNode> coordNode;
	FaceIndex                  faceIndex;
	FaceNumbers                faceNumbers;
	size_t                     numFaces;
	PointIndex                 pointIndex;

};

} // X3D
} // titania

#endif
