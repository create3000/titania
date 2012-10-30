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
 ******************************************************************************/

#include "PositionChaser2D.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

PositionChaser2D::PositionChaser2D (X3DExecutionContext* const executionContext) :
	      X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	     X3DChaserNode (),                                                    
	   set_destination (),                                                    // SFVec2f [in]     set_destination
	         set_value (),                                                    // SFVec2f [in]     set_value
	          isActive (),                                                    // SFBool  [out]    isActive
	     value_changed (),                                                    // SFVec2f [out]    value_changed
	          duration (),                                                    // SFTime  [ ]      duration            0           [0,∞)
	initialDestination (),                                                    // SFVec2f [ ]      initialDestination  0 0
	      defaultValue ()                                                     // SFVec2f [ ]      defaultValue        0 0
{
	setComponent ("Followers");
	setTypeName ("PositionChaser2D");

	appendField (inputOutput,    "metadata",           metadata);
	appendField (inputOnly,      "set_destination",    set_destination);
	appendField (inputOnly,      "set_value",          set_value);
	appendField (outputOnly,     "isActive",           isActive);
	appendField (outputOnly,     "value_changed",      value_changed);
	appendField (initializeOnly, "duration",           duration);
	appendField (initializeOnly, "initialDestination", initialDestination);
	appendField (initializeOnly, "defaultValue",       defaultValue);
}

X3DBasicNode*
PositionChaser2D::create (X3DExecutionContext* const executionContext) const
{
	return new PositionChaser2D (executionContext);
}

} // X3D
} // titania
