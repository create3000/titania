/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "ScreenGroup.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Navigation/Viewpoint.h"
#include "../../Tools/Layout/ScreenGroupTool.h"

namespace titania {
namespace X3D {

const ComponentType ScreenGroup::component      = ComponentType::LAYOUT;
const std::string   ScreenGroup::typeName       = "ScreenGroup";
const std::string   ScreenGroup::containerField = "children";

ScreenGroup::ScreenGroup (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	modelViewMatrix (),
	   screenMatrix (),
	         matrix ()
{
	addType (X3DConstants::ScreenGroup);

	addField (inputOutput,    "metadata",       metadata ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());
}

X3DBaseNode*
ScreenGroup::create (X3DExecutionContext* const executionContext) const
{
	return new ScreenGroup (executionContext);
}

Box3d
ScreenGroup::getBBox () const
{
	return X3DGroupingNode::getBBox () * getMatrix ();
}

const Matrix4d &
ScreenGroup::getMatrix () const
{
	try
	{
		const_cast <ScreenGroup*> (this) -> matrix = screenMatrix * inverse (modelViewMatrix);
	}
	catch (const std::domain_error &)
	{ }
	
	return matrix;
}

// Same as in Text
void
ScreenGroup::scale (const TraverseType type)
throw (std::domain_error)
{
		modelViewMatrix = getModelViewMatrix (type);
	
		Vector3d   translation, scale;
		Rotation4d rotation;
	
		modelViewMatrix .get (translation, rotation, scale);
	
		const auto & projectionMatrix = getBrowser () -> getProjectionMatrix ();
		const auto   viewport         = Viewport4i ();
		const auto   screenScale      = getCurrentViewpoint () -> getScreenScale (translation, viewport);
	
		screenMatrix .set (translation, rotation, Vector3d (screenScale .x () * (scale .x () < 0 ? -1 : 1),
		                                                    screenScale .y () * (scale .y () < 0 ? -1 : 1),
		                                                    screenScale .z () * (scale .z () < 0 ? -1 : 1)));
	
	
		// Snap to whole pixel
			
		auto screenPoint = ViewVolume::projectPoint (Vector3d (), screenMatrix, projectionMatrix, viewport);
	
		screenPoint .x (std::round (screenPoint .x ()));
		screenPoint .y (std::round (screenPoint .y ()));
	
		auto offset = ViewVolume::unProjectPoint (screenPoint .x (), screenPoint .y (), screenPoint .z (), screenMatrix, projectionMatrix, viewport);
	
		offset .z (0);
		screenMatrix .translate (offset);
	
		// Assign modelViewMatrix and relative matrix
	
		getModelViewMatrix () .set (screenMatrix);
}

void
ScreenGroup::traverse (const TraverseType type)
{
		getModelViewMatrix () .push ();
	
		try
		{
			scale (type);
		
			X3DGroupingNode::traverse (type);
		}
		catch (const std::domain_error &)
		{ }
	
		getModelViewMatrix () .pop ();
}

void
ScreenGroup::addTool ()
{
	X3DGroupingNode::addTool (new ScreenGroupTool (this));
}

} // X3D
} // titania
