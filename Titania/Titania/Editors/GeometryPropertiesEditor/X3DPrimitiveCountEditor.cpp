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

#include "X3DPrimitiveCountEditor.h"

namespace titania {
namespace puck {

X3DPrimitiveCountEditor::X3DPrimitiveCountEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                             browser (),
	                    executionContext (),
			                        points (),
			                         lines (),
			                     triangles ()
{
	getPrimitiveCountCountButton () .set_active (getConfig () .getInteger ("primitiveCount"));
}

void
X3DPrimitiveCountEditor::on_map_primitive_count ()
{
	on_primitive_count_count_changed ();
}

void
X3DPrimitiveCountEditor::on_unmap_primitive_count ()
{
	getBrowserWindow () -> getSelection () -> getChildren () .removeInterest (this, &X3DPrimitiveCountEditor::update);

	getBrowser ()          .removeInterest (this, &X3DPrimitiveCountEditor::update);
	getBrowser ()          .removeInterest (this, &X3DPrimitiveCountEditor::set_browser);
	getExecutionContext () .removeInterest (this, &X3DPrimitiveCountEditor::update);
	getExecutionContext () .removeInterest (this, &X3DPrimitiveCountEditor::set_executionContext);

	if (browser)
		browser -> displayed () .removeInterest (this, &X3DPrimitiveCountEditor::update);

	if (executionContext)
		executionContext -> sceneGraph_changed () .removeInterest (this, &X3DPrimitiveCountEditor::update);
}

bool
X3DPrimitiveCountEditor::on_crossing_notify_event (GdkEventCrossing*)
{
	update ();
	return false;
}

void
X3DPrimitiveCountEditor::on_primitive_count_count_changed ()
{
	on_unmap_primitive_count ();

	getExecutionContext () .addInterest (this, &X3DPrimitiveCountEditor::set_executionContext);
	getExecutionContext () .addInterest (this, &X3DPrimitiveCountEditor::update);

	switch (getPrimitiveCountCountButton () .get_active_row_number ())
	{
		case 0:
			getBrowser () .addInterest (this, &X3DPrimitiveCountEditor::update);
			break;
		case 1:
			getBrowser () .addInterest (this, &X3DPrimitiveCountEditor::set_browser);
			set_browser ();
			break;
		case 2:
			getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DPrimitiveCountEditor::update);
			break;
	}

	set_executionContext ();
	update ();
}

void
X3DPrimitiveCountEditor::update ()
{
	using namespace std::placeholders;

	if (not getWorld ())
		return;

	points    = 0;
	lines     = 0;
	triangles = 0;

	switch (getPrimitiveCountCountButton () .get_active_row_number ())
	{
		case 0:
		{
			for (const auto & layer : getWorld () -> getLayerSet () -> getLayers ())
			{
				X3D::traverse (layer -> children (),
				               std::bind (&X3DPrimitiveCountEditor::traverse, this, _1),
				               true,
				               X3D::TRAVERSE_ROOT_NODES |
				               X3D::TRAVERSE_PROTOTYPE_INSTANCES |
				               X3D::TRAVERSE_INLINE_NODES |
				               X3D::TRAVERSE_VISIBLE_NODES |
				               X3D::TRAVERSE_CLONED_NODES);
			}

			break;
		}
		case 1:
		{
			for (const auto & layer : getWorld () -> getLayerSet () -> getLayers ())
			{
				for (const auto & container : basic::make_range (layer -> getOpaqueShapes () .begin (), layer -> getNumOpaqueShapes ()))
				{
					const auto & geometry = container -> getShape () -> getGeometry ();

					if (geometry)
					{
						for (const auto & element : geometry -> getElements ())
							count (element);
					}
				}

				for (const auto & container : basic::make_range (layer -> getTransparentShapes () .begin (), layer -> getNumTransparentShapes ()))
				{
					const auto & geometry = container -> getShape () -> getGeometry ();

					if (geometry)
					{
						for (const auto & element : geometry -> getElements ())
							count (element);
					}
				}
			}

			break;
		}
		case 2:
		{
			auto selection = getBrowserWindow () -> getSelection () -> getChildren ();

			X3D::traverse (selection,
		                  std::bind (&X3DPrimitiveCountEditor::traverse, this, _1),
		                  true,
		                  X3D::TRAVERSE_ROOT_NODES |
		                  X3D::TRAVERSE_PROTOTYPE_INSTANCES |
		                  X3D::TRAVERSE_INLINE_NODES |
		                  X3D::TRAVERSE_VISIBLE_NODES |
		                  X3D::TRAVERSE_CLONED_NODES);

			break;
		}
	}

	getPrimitiveCountPointsLabel ()    .set_text (X3D::SFInt32 (points)    .toLocaleString ());
	getPrimitiveCountLinesLabel ()     .set_text (X3D::SFInt32 (lines)     .toLocaleString ());
	getPrimitiveCountTrianglesLabel () .set_text (X3D::SFInt32 (triangles) .toLocaleString ());
}

bool
X3DPrimitiveCountEditor::traverse (X3D::SFNode & node)
{
	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		if (type == X3D::X3DConstants::X3DGeometryNode)
		{
			const auto geometry = dynamic_cast <X3D::X3DGeometryNode*> (node .getValue ());
		
			for (const auto & element : geometry -> getElements ())
				count (element);

			break;
		}
	}

	return true;
}

void
X3DPrimitiveCountEditor::count (const X3D::X3DGeometryNode::Element & element)
{
	switch (element .vertexMode)
	{
		case GL_POINTS:
			points += element .count;
			break;
		case GL_LINES:
			lines += element .count / 2;
			break;
		case GL_LINE_LOOP:
			lines += element .count;
			break;
		case GL_LINE_STRIP:
			lines += element .count - 1;
			break;
		case GL_TRIANGLES:
			triangles += element .count / 3;
			break;
		case GL_QUADS:
			triangles += 2 * element .count / 4;
			break;
		case GL_QUAD_STRIP:
			triangles += 2 * (element .count / 2 - 1);
			break;
		case GL_TRIANGLE_FAN:
		case GL_TRIANGLE_STRIP:
		case GL_POLYGON:
			triangles += element .count - 2;
			break;
	}
}

void
X3DPrimitiveCountEditor::set_browser ()
{
	if (browser)
		browser -> displayed () .removeInterest (this, &X3DPrimitiveCountEditor::update);

	browser = getBrowser ();
	browser -> displayed () .addInterest (this, &X3DPrimitiveCountEditor::update);
}

void
X3DPrimitiveCountEditor::set_executionContext ()
{
	if (executionContext)
		executionContext -> sceneGraph_changed () .removeInterest (this, &X3DPrimitiveCountEditor::update);

	executionContext = getExecutionContext ();
	executionContext -> sceneGraph_changed () .addInterest (this, &X3DPrimitiveCountEditor::update);
}

X3DPrimitiveCountEditor::~X3DPrimitiveCountEditor ()
{
	getConfig () .setItem ("primitiveCount", getPrimitiveCountCountButton () .get_active_row_number ());
}

} // puck
} // titania
