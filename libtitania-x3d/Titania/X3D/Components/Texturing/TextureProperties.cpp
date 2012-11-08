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
 ******************************************************************************/

#include "TextureProperties.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

// http://new.web3d.org/files/specifications/19775-1/V3.2/Part01/components/texturing.html#TextureProperties

TextureProperties::TextureProperties (X3DExecutionContext* const executionContext) :
	       X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	    X3DPropertyNode (),                                                    
	        borderColor (),                                                    // SFColorRGBA [ ] borderColor         0 0 0 0    [0,1]
	        borderWidth (),                                                    // SFInt32     [ ] borderWidth         0          [0,1]
	      boundaryModeS ("REPEAT"),                                            // SFString    [ ] boundaryModeS       "REPEAT"
	      boundaryModeT ("REPEAT"),                                            // SFString    [ ] boundaryModeT       "REPEAT"
	      boundaryModeR ("REPEAT"),                                            // SFString    [ ] boundaryModeR       "REPEAT"
	 minificationFilter ("FASTEST"),                                           // SFString    [ ] minificationFilter  "FASTEST"
	magnificationFilter ("FASTEST"),                                           // SFString    [ ] magnificationFilter "FASTEST"
	 textureCompression ("FASTEST"),                                           // SFString    [ ] textureCompression  "FASTEST"
	    generateMipMaps (),                                                    // SFBool      [ ] generateMipMaps      FALSE
	  anisotropicDegree (1),                                                   // SFFloat     [ ] anisotropicDegree    1.0        [1,?)
	    texturePriority ()                                                     // SFFloat     [ ] texturePriority      0
{
	setComponent ("Texturing");
	setTypeName ("TextureProperties");

	appendField (inputOutput,    "metadata",            metadata);
	appendField (inputOutput,    "borderColor",         borderColor);
	appendField (inputOutput,    "borderWidth",         borderWidth);
	appendField (inputOutput,    "boundaryModeS",       boundaryModeS);
	appendField (inputOutput,    "boundaryModeT",       boundaryModeT);
	appendField (inputOutput,    "boundaryModeR",       boundaryModeR);
	appendField (inputOutput,    "minificationFilter",  minificationFilter);
	appendField (inputOutput,    "magnificationFilter", magnificationFilter);
	appendField (inputOutput,    "textureCompression",  textureCompression);
	appendField (initializeOnly, "generateMipMaps",     generateMipMaps);
	appendField (inputOutput,    "anisotropicDegree",   anisotropicDegree);
	appendField (inputOutput,    "texturePriority",     texturePriority);
}

X3DBasicNode*
TextureProperties::create (X3DExecutionContext* const executionContext) const
{
	return new TextureProperties (executionContext);
}

} // X3D
} // titania
