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

#include "Background.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

Background::Fields::Fields () :
	frontUrl (new MFString ()),
	backUrl (new MFString ()),
	leftUrl (new MFString ()),
	rightUrl (new MFString ()),
	topUrl (new MFString ()),
	bottomUrl (new MFString ())
{ }

Background::Background (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DBackgroundNode (),
	           fields (),
	     frontTexture (new ImageTexture (executionContext)),
	      backTexture (new ImageTexture (executionContext)),
	      leftTexture (new ImageTexture (executionContext)),
	     rightTexture (new ImageTexture (executionContext)),
	       topTexture (new ImageTexture (executionContext)),
	    bottomTexture (new ImageTexture (executionContext)),
	textureProperties (new TextureProperties (executionContext))
{
	setComponent ("EnvironmentalEffects");
	setTypeName ("Background");

	addField (inputOutput, "metadata",     metadata ());
	addField (inputOnly,   "set_bind",     set_bind ());
	addField (inputOutput, "frontUrl",     frontUrl ());
	addField (inputOutput, "backUrl",      backUrl ());
	addField (inputOutput, "leftUrl",      leftUrl ());
	addField (inputOutput, "rightUrl",     rightUrl ());
	addField (inputOutput, "topUrl",       topUrl ());
	addField (inputOutput, "bottomUrl",    bottomUrl ());
	addField (inputOutput, "skyAngle",     skyAngle ());
	addField (inputOutput, "skyColor",     skyColor ());
	addField (inputOutput, "groundAngle",  groundAngle ());
	addField (inputOutput, "groundColor",  groundColor ());
	addField (inputOutput, "transparency", transparency ());
	addField (outputOnly,  "isBound",      isBound ());
	addField (outputOnly,  "bindTime",     bindTime ());

	addChildren (frontTexture,
	             backTexture,
	             leftTexture,
	             rightTexture,
	             topTexture,
	             bottomTexture,
	             textureProperties);
}

X3DBaseNode*
Background::create (X3DExecutionContext* const executionContext) const
{
	return new Background (executionContext);
}

void
Background::initialize ()
{
	X3DBackgroundNode::initialize ();

	frontUrl ()  .addInterest (frontTexture  -> url ());
	backUrl ()   .addInterest (backTexture   -> url ());
	leftUrl ()   .addInterest (leftTexture   -> url ());
	rightUrl ()  .addInterest (rightTexture  -> url ());
	topUrl ()    .addInterest (topTexture    -> url ());
	bottomUrl () .addInterest (bottomTexture -> url ());

	textureProperties -> boundaryModeS () = "CLAMP_TO_EDGE";
	textureProperties -> boundaryModeT () = "CLAMP_TO_EDGE";
	textureProperties -> boundaryModeR () = "CLAMP_TO_EDGE";

	frontTexture  -> url () = frontUrl ();
	backTexture   -> url () = backUrl ();
	leftTexture   -> url () = leftUrl ();
	rightTexture  -> url () = rightUrl ();
	topTexture    -> url () = topUrl ();
	bottomTexture -> url () = bottomUrl ();

	frontTexture  -> textureProperties () = *textureProperties;
	backTexture   -> textureProperties () = *textureProperties;
	leftTexture   -> textureProperties () = *textureProperties;
	rightTexture  -> textureProperties () = *textureProperties;
	topTexture    -> textureProperties () = *textureProperties;
	bottomTexture -> textureProperties () = *textureProperties;

	textureProperties -> setup ();
	frontTexture      -> setup ();
	backTexture       -> setup ();
	leftTexture       -> setup ();
	rightTexture      -> setup ();
	topTexture        -> setup ();
	bottomTexture     -> setup ();
}

void
Background::draw ()
{
	X3DBackgroundNode::draw ();

	float radius = 10000;
	float s      = std::sqrt (std::pow (2 * radius, 2) / 2) / 2;

	glMatrixMode (GL_TEXTURE);
	glLoadIdentity ();
	glTranslatef (0, 1, 0);
	glScalef (1, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	glFrontFace (GL_CCW);
	glColor4f (1, 1, 1, 1);

	if (frontTexture -> checkLoadState () == COMPLETE_STATE)
	{
		if (frontTexture -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		frontTexture -> draw ();
		glBegin (GL_QUADS);
		glTexCoord2f (1, 1);
		glVertex3f (s, s, -s);
		glTexCoord2f (0, 1);
		glVertex3f (-s, s, -s);
		glTexCoord2f (0, 0);
		glVertex3f (-s, -s, -s);
		glTexCoord2f (1, 0);
		glVertex3f (s, -s, -s);
		glEnd ();
	}

	if (backTexture -> checkLoadState () == COMPLETE_STATE)
	{
		if (frontTexture -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		backTexture -> draw ();
		glBegin (GL_QUADS);
		glTexCoord2f (0, 0);
		glVertex3f (s, -s, s);
		glTexCoord2f (1, 0);
		glVertex3f (-s, -s, s);
		glTexCoord2f (1, 1);
		glVertex3f (-s, s, s);
		glTexCoord2f (0, 1);
		glVertex3f (s, s, s);
		glEnd ();
	}

	if (leftTexture -> checkLoadState () == COMPLETE_STATE)
	{
		if (frontTexture -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		leftTexture -> draw ();
		glBegin (GL_QUADS);
		glTexCoord2f (0, 1);
		glVertex3f (-s, s, s);
		glTexCoord2f (0, 0);
		glVertex3f (-s, -s, s);
		glTexCoord2f (1, 0);
		glVertex3f (-s, -s, -s);
		glTexCoord2f (1, 1);
		glVertex3f (-s, s, -s);
		glEnd ();
	}

	if (rightTexture -> checkLoadState () == COMPLETE_STATE)
	{
		if (frontTexture -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		rightTexture -> draw ();
		glBegin (GL_QUADS);
		glTexCoord2f (0, 1);
		glVertex3f (s, s, -s);
		glTexCoord2f (0, 0);
		glVertex3f (s, -s, -s);
		glTexCoord2f (1, 0);
		glVertex3f (s, -s, s);
		glTexCoord2f (1, 1);
		glVertex3f (s, s, s);
		glEnd ();
	}

	if (topTexture -> checkLoadState () == COMPLETE_STATE)
	{
		if (frontTexture -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		topTexture -> draw ();
		glBegin (GL_QUADS);
		glTexCoord2f (0, 1);
		glVertex3f (-s, s, s);
		glTexCoord2f (0, 0);
		glVertex3f (-s, s, -s);
		glTexCoord2f (1, 0);
		glVertex3f (s, s, -s);
		glTexCoord2f (1, 1);
		glVertex3f (s, s, s);
		glEnd ();
	}

	if (bottomTexture -> checkLoadState () == COMPLETE_STATE)
	{
		if (frontTexture -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		bottomTexture -> draw ();
		glBegin (GL_QUADS);
		glTexCoord2f (1, 0);
		glVertex3f (s, -s, s);
		glTexCoord2f (1, 1);
		glVertex3f (s, -s, -s);
		glTexCoord2f (0, 1);
		glVertex3f (-s, -s, -s);
		glTexCoord2f (0, 0);
		glVertex3f (-s, -s, s);
		glEnd ();
	}

	glDisable (GL_TEXTURE_2D);
}

void
Background::dispose ()
{
	textureProperties .dispose ();
	frontTexture      .dispose ();
	backTexture       .dispose ();
	leftTexture       .dispose ();
	rightTexture      .dispose ();
	topTexture        .dispose ();
	bottomTexture     .dispose ();

	X3DBackgroundNode::dispose ();
}

} // X3D
} // titania
