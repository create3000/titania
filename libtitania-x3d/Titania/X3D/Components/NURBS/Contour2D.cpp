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

#include "Contour2D.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../NURBS/ContourPolyline2D.h"
#include "../NURBS/NurbsCurve2D.h"

namespace titania {
namespace X3D {

const std::string Contour2D::componentName  = "NURBS";
const std::string Contour2D::typeName       = "Contour2D";
const std::string Contour2D::containerField = "trimmingContour";

Contour2D::Fields::Fields () :
	   addChildren (new MFNode ()),
	removeChildren (new MFNode ()),
	      children (new MFNode ())
{ }

Contour2D::Contour2D (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DNode (),
	     fields (),
	 curveNodes ()
{
	addType (X3DConstants::Contour2D);

	addField (inputOutput, "metadata",       metadata ());
	addField (inputOnly,   "addChildren",    addChildren ());
	addField (inputOnly,   "removeChildren", removeChildren ());
	addField (inputOutput, "children",       children ());

	addChildObjects (curveNodes);
}

X3DBaseNode*
Contour2D::create (X3DExecutionContext* const executionContext) const
{
	return new Contour2D (executionContext);
}

void
Contour2D::initialize ()
{
	X3DNode::initialize ();

	addChildren ()    .addInterest (&Contour2D::set_addChildren,    this);
	removeChildren () .addInterest (&Contour2D::set_removeChildren, this);
	children ()       .addInterest (&Contour2D::set_children,       this);

	set_children ();
}

void
Contour2D::set_addChildren ()
{
	addChildren () .setTainted (true);
	addChildren () .set_difference (children ());

	children () .insert (children () .end (), addChildren () .cbegin (), addChildren () .cend ());

	addChildren () .set ({ });
	addChildren () .setTainted (false);
}

void
Contour2D::set_removeChildren ()
{
	children () .set_difference (removeChildren ());

	removeChildren () .set ({ });
}

void
Contour2D::set_children ()
{
	curveNodes .clear ();

	for (const auto & child : children ())
	{
		const auto curveNode = x3d_cast <X3DNurbsControlCurveNode*> (child);

		if (curveNode)
			curveNodes .emplace_back (curveNode);
	}
}

void
Contour2D::trimSurface (nurbs_tessellator & tessellator) const
{
	tessellator .begin_trim ();

	for (const auto & curveNode : curveNodes)
		curveNode -> trim (tessellator);

	tessellator .end_trim ();
}

} // X3D
} // titania
