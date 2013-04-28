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

#include "X3DFogObject.h"

#include "../../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

X3DFogObject::Fields::Fields () :
	color (new SFColor (1, 1, 1)),
	fogType (new SFString ("LINEAR")),
	visibilityRange (new SFFloat ()),
	transparency (0)
{ }

X3DFogObject::X3DFogObject () :
	X3DBaseNode (), 
	     fields ()  
{
	addNodeType (X3DConstants::X3DFogObject);

	setChildren (transparency ());
}

void
X3DFogObject::initialize ()
{
	color ()           .addInterest (this, &X3DFogObject::set_color);
	fogType ()         .addInterest (this, &X3DFogObject::set_fogType);
	visibilityRange () .addInterest (this, &X3DFogObject::set_fogType);
	transparency ()    .addInterest (this, &X3DFogObject::set_transparency);

	set_color        ();
	set_transparency ();
	set_fogType      ();
}

float
X3DFogObject::getVisibilityRange ()
{
	if (visibilityRange ())
		return visibilityRange ();

	return getBrowser () -> getLayers () .top () -> getNavigationInfo () -> getFarPlane ();
}

float
X3DFogObject::getDensitiy (float visibilityRange)
{
	switch (glMode)
	{
		case GL_EXP2:
			return 4 / visibilityRange;
		case GL_EXP:
			return 2 / visibilityRange;
		default:
			return 1;
	}
}

void
X3DFogObject::set_color ()
{
	glColor [0] = color () .getR ();
	glColor [1] = color () .getG ();
	glColor [2] = color () .getB ();
}

void
X3DFogObject::set_transparency ()
{
	glColor [3] = 1 - transparency ();
}

void
X3DFogObject::set_fogType ()
{
	if (fogType () == "EXPONENTIAL2")
	{
		glMode = GL_EXP2;
	}
	else if (fogType () == "EXPONENTIAL")
	{
		glMode = GL_EXP;
	}
	else  // LINEAR
	{
		glMode = GL_LINEAR;
	}
}

void
X3DFogObject::enable ()
{
	if (glColor [3])
	{
		float glVisibilityRange = getVisibilityRange ();
		float glDensity         = getDensitiy (glVisibilityRange);

		glEnable (GL_FOG);

		glFogi  (GL_FOG_MODE,    glMode);
		glFogf  (GL_FOG_DENSITY, glDensity);
		glFogf  (GL_FOG_START,   0);
		glFogf  (GL_FOG_END,     glVisibilityRange);
		glFogfv (GL_FOG_COLOR,   glColor);
	}
}

void
X3DFogObject::dispose ()
{ }

} // X3D
} // titania
