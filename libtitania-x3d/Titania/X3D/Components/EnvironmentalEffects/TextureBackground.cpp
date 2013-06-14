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

#include "TextureBackground.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

TextureBackground::Fields::Fields () :
	backTexture (new SFNode <X3DBaseNode> ()),
	bottomTexture (new SFNode <X3DBaseNode> ()),
	frontTexture (new SFNode <X3DBaseNode> ()),
	leftTexture (new SFNode <X3DBaseNode> ()),
	rightTexture (new SFNode <X3DBaseNode> ()),
	topTexture (new SFNode <X3DBaseNode> ())
{ }

TextureBackground::TextureBackground (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DBackgroundNode (),                                           
	           fields ()                                                     
{
	setComponent ("EnvironmentalEffects");
	setTypeName ("TextureBackground");

	addField (inputOutput, "metadata",      metadata ());
	addField (inputOnly,   "set_bind",      set_bind ());
	addField (outputOnly,  "bindTime",      bindTime ());
	addField (outputOnly,  "isBound",       isBound ());
	addField (inputOutput, "skyAngle",      skyAngle ());
	addField (inputOutput, "skyColor",      skyColor ());
	addField (inputOutput, "groundAngle",   groundAngle ());
	addField (inputOutput, "groundColor",   groundColor ());
	addField (inputOutput, "transparency",  transparency ());
	addField (inputOutput, "backTexture",   backTexture ());
	addField (inputOutput, "bottomTexture", bottomTexture ());
	addField (inputOutput, "frontTexture",  frontTexture ());
	addField (inputOutput, "leftTexture",   leftTexture ());
	addField (inputOutput, "rightTexture",  rightTexture ());
	addField (inputOutput, "topTexture",    topTexture ());
}

X3DBaseNode*
TextureBackground::create (X3DExecutionContext* const executionContext) const
{
	return new TextureBackground (executionContext);
}

} // X3D
} // titania
