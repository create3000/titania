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

#include "TwoSidedMaterial.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

TwoSidedMaterial::TwoSidedMaterial (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	     X3DMaterialNode (),                                                    
	    ambientIntensity (0.2),                                                 // SFFloat [in,out] ambientIntensity      0.2                [0,1]
	backAmbientIntensity (0.2),                                                 // SFFloat [in,out] backAmbientIntensity  0.2                [0,1]
	    backDiffuseColor (0.8, 0.8, 0.8),                                       // SFColor [in,out] backDiffuseColor      0.8 0.8 0.8        [0,1]
	   backEmissiveColor (),                                                    // SFColor [in,out] backEmissiveColor     0 0 0              [0,1]
	       backShininess (0.2),                                                 // SFFloat [in,out] backShininess         0.2                [0,1]
	   backSpecularColor (),                                                    // SFColor [in,out] backSpecularColor     0 0 0              [0,1]
	    backTransparency (),                                                    // SFFloat [in,out] backTransparency      0                  [0,1]
	        diffuseColor (0.8, 0.8, 0.8),                                       // SFColor [in,out] diffuseColor          0.8 0.8 0.8        [0,1]
	       emissiveColor (),                                                    // SFColor [in,out] emissiveColor         0 0 0              [0,1]
	           shininess (0.2),                                                 // SFFloat [in,out] shininess             0.2                [0,1]
	   separateBackColor (),                                                    // SFBool  [in,out] separateBackColor     FALSE
	       specularColor (),                                                    // SFColor [in,out] specularColor         0 0 0              [0,1]
	        transparency ()                                                     // SFFloat [in,out] transparency          0                  [0,1]
{
	setComponent ("Shape");
	setTypeName ("TwoSidedMaterial");

	addField (inputOutput, "metadata",             metadata);
	addField (inputOutput, "ambientIntensity",     ambientIntensity);
	addField (inputOutput, "backAmbientIntensity", backAmbientIntensity);
	addField (inputOutput, "backDiffuseColor",     backDiffuseColor);
	addField (inputOutput, "backEmissiveColor",    backEmissiveColor);
	addField (inputOutput, "backShininess",        backShininess);
	addField (inputOutput, "backSpecularColor",    backSpecularColor);
	addField (inputOutput, "backTransparency",     backTransparency);
	addField (inputOutput, "diffuseColor",         diffuseColor);
	addField (inputOutput, "emissiveColor",        emissiveColor);
	addField (inputOutput, "shininess",            shininess);
	addField (inputOutput, "separateBackColor",    separateBackColor);
	addField (inputOutput, "specularColor",        specularColor);
	addField (inputOutput, "transparency",         transparency);
}

X3DBaseNode*
TwoSidedMaterial::create (X3DExecutionContext* const executionContext) const
{
	return new TwoSidedMaterial (executionContext);
}

} // X3D
} // titania
