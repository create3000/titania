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

#include "X3DSphereEditor.h"

#include <Titania/X3D/Browser/Geometry3D/IcosahedronOptions.h>
#include <Titania/X3D/Browser/Geometry3D/OctahedronOptions.h>
#include <Titania/X3D/Browser/Geometry3D/QuadSphereOptions.h>
#include <Titania/X3D/Components/Geometry3D/Sphere.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
 
namespace titania {
namespace puck {

X3DSphereEditor::X3DSphereEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                              radius (this, getSphereRadiusAdjustment (), getSphereRadiusSpinButton (), "radius"),
	    octahedronSphericalInterpolation (this, getOctahedronSphericalInterpolationCheckButton (), "sphericalInterpolation"),
	                     octahedronOrder (this, getOctahedronDimensionAdjustment (), getOctahedronOrderSpinButton (), "order"),
	   icosahedronSphericalInterpolation (this, getIcosahedronSphericalInterpolationCheckButton (), "sphericalInterpolation"),
	                    icosahedronOrder (this, getIcosahedronDimensionAdjustment (), getIcosahedronOrderSpinButton (), "order"),
	                      quadXDimension (this, getQuadSphereXDimensionAdjustment (), getQuadSphereXDimensionSpinButton (), "xDimension"),
	                      quadYDimension (this, getQuadSphereYDimensionAdjustment (), getQuadSphereYDimensionSpinButton (), "yDimension"),
	                               nodes (),
	                            changing (false)
{
	addChildObjects (nodes);
}

void
X3DSphereEditor::set_geometry ()
{
	// Hidden fields

	for (const auto & node : nodes)
		node -> getField <X3D::SFNode> ("options") .removeInterest (&X3DSphereEditor::set_options, this);

	// Fields

	nodes = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::Sphere });

	getSphereBox () .set_visible (not nodes .empty ());

	radius .setNodes (nodes);

	// Hidden fields

	for (const auto & node : nodes)
		node -> getField <X3D::SFNode> ("options") .addInterest (&X3DSphereEditor::set_options, this);

	set_options ();
}

void
X3DSphereEditor::on_sphere_use_global_options_toggled ()
{
	if (changing)
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Toggle Sphere Use Global Options To »%s«", getSphereUseGlobalOptionsCheckButton () .get_active () ? "TRUE" : "FALSE")));
	const auto executionContext = X3D::MakePtr (getSelectionContext (nodes));

	if (getSphereUseGlobalOptionsCheckButton () .get_active ())
	{
		for (const auto & node : nodes)
		{
			auto & options = node -> getField <X3D::SFNode> ("options");

			X3D::X3DEditor::replaceNode (executionContext, node, options, nullptr, undoStep);
		}
	}
	else
	{
		for (const auto & node : nodes)
		{
			auto &     options    = node -> getField <X3D::SFNode> ("options");
			const auto optionNode = X3D::SFNode (getCurrentBrowser () -> getSphereOptions () -> copy (executionContext, X3D::FLAT_COPY));

			X3D::X3DEditor::replaceNode (executionContext, node, options, optionNode, undoStep);
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DSphereEditor::on_sphere_type_changed ()
{
	if (changing)
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Change Sphere Type To »%s«", getSphereTypeButton () .get_active_text () .c_str ())));
	const auto executionContext = X3D::MakePtr (getSelectionContext (nodes, true));
	auto       optionNode       = X3D::SFNode ();

	switch (getSphereTypeButton () .get_active_row_number ())
	{
		case 0:
		{
			optionNode = X3D::MakePtr <X3D::OctahedronOptions> (executionContext);
			break;
		}
		case 1:
		{
			optionNode = X3D::MakePtr <X3D::IcosahedronOptions> (executionContext);
			break;
		}
		default:
		{
			optionNode = X3D::MakePtr <X3D::QuadSphereOptions> (executionContext);
			break;
		}
	}

	for (const auto & node : nodes)
	{
		auto &     options = node -> getField <X3D::SFNode> ("options");
		const auto copy    = X3D::SFNode (optionNode -> copy (executionContext, X3D::FLAT_COPY));

		X3D::X3DEditor::replaceNode (executionContext, node, options, copy, undoStep);
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DSphereEditor::set_options ()
{
	// Set composed widgets.

	X3D::MFNode              optionsNodes;
	std::set <X3D::NodeType> types;

	for (const auto & node : nodes)
	{
		const auto & optionsNode = node -> getField <X3D::SFNode> ("options");

		if (optionsNode)
		{
			optionsNodes .emplace_back (optionsNode);
			types .emplace (optionsNode -> getType () .back ());
		}
	}

	const auto global       = optionsNodes .empty ();
	const auto inconsistent = not (optionsNodes .size () == nodes .size () and types .size () == 1);

	if (optionsNodes .empty ())
		optionsNodes .emplace_back (getCurrentBrowser () -> getSphereOptions ());

	quadXDimension                    .setNodes (optionsNodes);
	quadYDimension                    .setNodes (optionsNodes);
	icosahedronSphericalInterpolation .setNodes (optionsNodes);
	icosahedronOrder                  .setNodes (optionsNodes);
	octahedronSphericalInterpolation  .setNodes (optionsNodes);
	octahedronOrder                   .setNodes (optionsNodes);

	// Set global widget.

	changing = true;

	getSphereUseGlobalOptionsCheckButton () .set_active (global);
	getSphereUseGlobalOptionsCheckButton () .set_inconsistent (inconsistent);

	getSphereTypeButton ()       .set_sensitive (not global and not inconsistent);
	getOctahedronOptionsGrid ()  .set_sensitive (not global and not inconsistent);
	getIcosahedronOptionsGrid () .set_sensitive (not global and not inconsistent);
	getQuadSphereOptionsGrid ()  .set_sensitive (not global and not inconsistent);

	getOctahedronOptionsGrid ()  .set_visible (false);
	getIcosahedronOptionsGrid () .set_visible (false);
	getQuadSphereOptionsGrid ()  .set_visible (false);

	if (global)
	{
		getSphereTypeButton ()      .set_active (2);
		getQuadSphereOptionsGrid () .set_visible (true);
	}
	else if (inconsistent)
	{
		getSphereTypeButton () .set_active (-1);
	}
	else
	{
		switch (optionsNodes .back () -> getType () .back ())
		{
			case X3D::X3DConstants::OctahedronOptions:
			{
				getSphereTypeButton ()      .set_active (0);
				getOctahedronOptionsGrid () .set_visible (true);
				break;
			}
			case X3D::X3DConstants::IcosahedronOptions:
			{
				getSphereTypeButton ()       .set_active (1);
				getIcosahedronOptionsGrid () .set_visible (true);
				break;
			}
			case X3D::X3DConstants::QuadSphereOptions:
			{
				getSphereTypeButton ()      .set_active (2);
				getQuadSphereOptionsGrid () .set_visible (true);
				break;
			}
			default:
			{
				getSphereTypeButton () .set_active (-1);
				break;
			}
		}
	}

	changing = false;
}

X3DSphereEditor::~X3DSphereEditor ()
{ }

} // puck
} // titania
