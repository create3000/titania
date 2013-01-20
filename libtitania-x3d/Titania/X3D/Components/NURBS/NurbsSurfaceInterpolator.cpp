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

#include "NurbsSurfaceInterpolator.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

NurbsSurfaceInterpolator::NurbsSurfaceInterpolator (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	    X3DChildNode (),                                                    
	    set_fraction (),                                                    // SFVec2f  [in]     set_fraction                (-∞,∞)
	    controlPoint (),                                                    // SFNode   [in,out] controlPoint      [ ]       [X3DCoordinateNode]
	          weight (),                                                    // MFDouble [in,out] weight            [ ]       (-∞,∞)
	position_changed (),                                                    // SFVec3f  [out]    position_changed
	  normal_changed (),                                                    // SFVec3f  [out]    normal_changed
	      uDimension (),                                                    // SFInt32  [ ]      uDimension        0         [0,∞)
	           uKnot (),                                                    // MFDouble [ ]      uKnot             [ ]        (-∞,∞)
	          uOrder (3),                                                   // SFInt32  [ ]      uOrder            3         [2,∞)
	      vDimension (),                                                    // SFInt32  [ ]      vDimension        0         [0,∞)
	           vKnot (),                                                    // MFDouble [ ]      vKnot             [ ]        (-∞,∞)
	          vOrder (3)                                                    // SFInt32  [ ]      vOrder            3         [2,∞)
{
	setComponent ("NURBS");
	setTypeName ("NurbsSurfaceInterpolator");

	addField (inputOutput,    "metadata",         metadata);
	addField (inputOnly,      "set_fraction",     set_fraction);
	addField (inputOutput,    "controlPoint",     controlPoint);
	addField (inputOutput,    "weight",           weight);
	addField (outputOnly,     "position_changed", position_changed);
	addField (outputOnly,     "normal_changed",   normal_changed);
	addField (initializeOnly, "uDimension",       uDimension);
	addField (initializeOnly, "uKnot",            uKnot);
	addField (initializeOnly, "uOrder",           uOrder);
	addField (initializeOnly, "vDimension",       vDimension);
	addField (initializeOnly, "vKnot",            vKnot);
	addField (initializeOnly, "vOrder",           vOrder);
}

X3DBaseNode*
NurbsSurfaceInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsSurfaceInterpolator (executionContext);
}

} // X3D
} // titania
