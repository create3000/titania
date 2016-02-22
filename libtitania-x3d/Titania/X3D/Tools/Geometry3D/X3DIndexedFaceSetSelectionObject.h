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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_SELECTION_OBJECT_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_SELECTION_OBJECT_H__

#include "../Rendering/X3DComposedGeometryNodeTool.h"

namespace titania {
namespace X3D {

class FaceSelection;
class PlaneSensor;
class TouchSensor;
class CoordinateDouble;
class IndexedLineSet;
class IndexedFaceSet;

class X3DIndexedFaceSetSelectionObject :
	virtual public X3DComposedGeometryNodeTool
{
public:

	///  @name Fields

	virtual
	SFBool &
	convex () = 0;

	virtual
	const SFBool &
	convex () const = 0;

	virtual
	MFInt32 &
	coordIndex () = 0;

	virtual
	const MFInt32 &
	coordIndex () const = 0;

	///  @name Destruction

	virtual
	void
	dispose ()
	{ }

	~X3DIndexedFaceSetSelectionObject ();


protected:

	///  @name Construction

	X3DIndexedFaceSetSelectionObject ();

	virtual
	void
	initialize () override;

	///  @name Member access

	const X3DPtr <FaceSelection> &
	getFaceSelection () const
	{ return selection; }

	int32_t
	getMasterPoint () const
	{ return masterPoint; }

	const std::map <int32_t, Vector3d> &
	getSelectedPoints () const
	{ return selectedPoints; }


private:

	///  @name Event handler

	void
	set_loadState ();

	void
	set_coord ();

	void
	set_coord_point ();

	void
	set_touch_sensor_over (const bool);

	void
	set_touch_sensor_active (const bool);

	void
	set_touch_sensor_hitPoint (const Vector3f &);

	void
	set_selection_ (const MFVec3d &);

	void
	set_touch_sensor_touchTime ();

	void
	set_point (const Vector3d &, const bool, const bool, const bool);

	void
	set_plane_sensor_active (const bool);

	void
	set_plane_sensor_translation (const Vector3f &);

	void
	setActiveSelection (const Vector3f &, const std::vector <int32_t> &);

	void
	setActivePoints ();

	void
	selectPoints (const Vector3d &, const std::vector <int32_t> &, const bool, const bool);

	void
	selectFaces (const Vector3d &, const std::vector <int32_t> &, const bool, const bool, const bool);

	void
	addSelectedPoint (const int32_t);

	void
	removeSelectedPoint (const int32_t);

	void
	updateSelectedPoints ();

	void
	addSelectedEdges (const std::vector <size_t> &);

	void
	removeSelectedEdges (const std::vector <size_t> &);

	void
	updateSelectedEdges ();

	void
	addSelectedFace (const size_t);

	void
	removeSelectedFace (const size_t);

	void
	updateSelectedFaces ();

	bool
	isInSelection (const std::vector <size_t> &) const;

	double
	getDistance (const Vector3d &, const Vector3d &);

	///  @name Members

	using SelectedEdges = std::map <std::pair <int32_t, int32_t>, std::set <std::pair <size_t, size_t>>>;

	///  @name Members

	X3DPtr <PlaneSensor>       planeSensor;
	X3DPtr <TouchSensor>       touchSensor;
	X3DPtr <CoordinateDouble>  activePointCoord;
	X3DPtr <IndexedLineSet>    activeEdgesGeometry;
	X3DPtr <CoordinateDouble>  selectionCoord;
	X3DPtr <IndexedLineSet>    selectedEdgesGeometry;
	X3DPtr <IndexedFaceSet>    selectedFacesGeometry;
	X3DPtr <X3DCoordinateNode> coordNode;

	std::map <int32_t, Vector3d> activePoints;   // coord index, point
	std::vector <size_t>         activeVertices; // indices of vertices to coordIndex array
	size_t                       activeFace;

	X3DPtr <FaceSelection>       selection;
	int32_t                      masterPoint;    // coord index,
	std::map <int32_t, Vector3d> selectedPoints; // coord index, point
	SelectedEdges                selectedEdges;  // index to vertex of face to coordIndex array
	std::set <size_t>            selectedFaces;  // index to first vertex of face to coordIndex array

	Vector3d translation;

};

} // X3D
} // titania

#endif
