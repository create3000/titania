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

#include "X3DObjectOperations.h"

#include "../Base/X3DEditorObject.h"
#include "../Browser/X3DBrowserWindow.h"
#include <Titania/Utility/Map.h>

namespace titania {
namespace puck {

X3DObjectOperations::X3DObjectOperations () :
	X3DBrowserWindowInterface ()
{ }

void
X3DObjectOperations::on_combine_activate ()
{
	__LOG__ << std::endl;

	try
	{
		const auto undoStep = std::make_shared <UndoStep> (_ ("Combine Objects"));

		auto selection  = getBrowserWindow () -> getSelection () -> getChildren ();
		auto shapes     = X3DEditorObject::getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });

		if (shapes .size () < 2)
			return;

		// Choose master

		const auto masterShape = shapes .back ();
		shapes .pop_back ();

		const X3D::X3DPtr <X3D::IndexedFaceSet> masterGeometry (masterShape -> geometry ());
		
		if (not masterGeometry)
			return;

		const X3D::X3DPtr <X3D::X3DCoordinateNode> masterCoord (masterGeometry -> coord ());

		if (not masterCoord)
			return;

		const auto masterMatrix = ~getBrowserWindow () -> findModelViewMatrix (masterShape);

		// Combine geometries

		for (const auto & shape : shapes)
		{
			const X3D::X3DPtr <X3D::IndexedFaceSet> geometry (shape -> geometry ());

			if (not geometry)
				continue;

			const X3D::X3DPtr <X3D::X3DCoordinateNode> coord (geometry -> coord ());

			if (not coord)
				continue;

			const auto matrix = getBrowserWindow () -> findModelViewMatrix (shape) * masterMatrix;

			std::map <int32_t, int32_t> coordIndex;

			for (const auto & index : geometry -> coordIndex ())
			{
				if (index >= 0)
					coordIndex .emplace (index, coordIndex .size ());
			}

			for (const auto & index : geometry -> coordIndex ())
			{
				if (index < 0)
					masterGeometry -> coordIndex () .emplace_back (-1);
				else
					masterGeometry -> coordIndex () .emplace_back (coordIndex [index] + masterCoord -> getSize ());
			}

			for (const auto & index : basic::reverse (coordIndex))
				masterCoord -> set1Point (masterCoord -> getSize (), coord -> get1Point (index .second) * matrix);
		}

		const auto hierarchy = X3D::find (selection, masterShape, X3D::TRAVERSE_ROOT_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);

		getBrowserWindow () -> getSelection () -> setChildren ({ X3D::SFNode (hierarchy .front ()) }, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);

		getExecutionContext () -> realize ();
	}
	catch (const std::exception &)
	{ }
}

X3DObjectOperations::~X3DObjectOperations ()
{ }

} // puck
} // titania
