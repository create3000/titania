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

#ifndef __TITANIA_X3D_EDITING_COMBINE_H__
#define __TITANIA_X3D_EDITING_COMBINE_H__

#include "../Components/Geometry3D/IndexedFaceSet.h"
#include "../Components/Rendering/X3DCoordinateNode.h"
#include "../Components/Shape/X3DShapeNode.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Editing/Undo/UndoStep.h"

#include <Titania/Math/Mesh/Combine.h>

namespace titania {
namespace X3D {

class Combine
{
public:

	using BooleanOperation = std::function <mesh3 <double> (const mesh3 <double> &, const mesh3 <double> &)>;

	///  @name Construction

	Combine ();

	///  @name Operations

	static
	X3DPtrArray <IndexedFaceSet>
	getIndexedFaceSets (const X3DPtrArray <X3DShapeNode> & shapes);

	static
	mesh3 <double>
	toMesh (const X3DPtr <IndexedFaceSet> & geometryNode,
	         const X3DPtr <X3DCoordinateNode> & coordNode,
	         const Matrix4d & matrix);

	///  throws Error <INVALID_NODE>, Error <DISPOSED>, std::domain_error
	static
	bool
	geometryUnion (const X3DExecutionContextPtr & executionContext,
	               const X3DPtrArray <X3DShapeNode> & shapes,
	               const UndoStepPtr & undoStep);

	///  throws Error <INVALID_NODE>, Error <DISPOSED>, std::domain_error
	static
	bool
	geometryDifference (const X3DExecutionContextPtr & executionContext,
	                    const X3DPtrArray <X3DShapeNode> & shapes,
	                    const UndoStepPtr & undoStep);

	///  throws Error <INVALID_NODE>, Error <DISPOSED>, std::domain_error
	static
	bool
	geometryIntersection (const X3DExecutionContextPtr & executionContext,
	                      const X3DPtrArray <X3DShapeNode> & shapes,
	                      const UndoStepPtr & undoStep);

	///  throws Error <INVALID_NODE>, Error <DISPOSED>, std::domain_error
	static
	bool
	geometryExclusion (const X3DExecutionContextPtr & executionContext,
	                   const X3DPtrArray <X3DShapeNode> & shapes,
	                   const UndoStepPtr & undoStep);

	///  throws Error <INVALID_NODE>, Error <DISPOSED>, std::domain_error
	static
	bool
	combineGeometry (const X3DExecutionContextPtr & executionContext,
	                 const X3DPtrArray <X3DShapeNode> & shapes,
	                 const UndoStepPtr & undoStep);

	static
	std::vector <int32_t>
	combine (const X3DExecutionContextPtr & executionContext,
	         const X3DPtrArray <IndexedFaceSet> & geometryNodes,
	         const X3DPtr <IndexedFaceSet> & targetGeometry,
	         const X3DPtr <X3DCoordinateNode> & targetCoord,
	         const Matrix4d & targetMatrix);

	static
	void
	removeShapes (const X3DExecutionContextPtr & executionContext,
	              const MFNode & selection,
	              const X3DPtrArray <X3DGroupingNode> & groups,
	              const X3DPtrArray <X3DShapeNode> & shapes,
	              const X3DPtr <X3DShapeNode> & masterShape,
	              const UndoStepPtr & undoStep);

	///  @name Destruction

	~Combine ();


private:

	///  throws Error <INVALID_NODE>, Error <DISPOSED>, std::domain_error
	static
	bool
	geometryBoolean (const BooleanOperation & booleanOperation,
	                 const X3DExecutionContextPtr & executionContext,
	                 const X3DPtrArray <X3DShapeNode> & shapes,
	                 const bool front,
	                 const UndoStepPtr & undoStep);

};

} // X3D
} // titania

#endif
