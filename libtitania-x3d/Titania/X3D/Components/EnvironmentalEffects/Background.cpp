/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#include "Background.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Texturing/ImageTexture.h"

#include <iostream>

namespace titania {
namespace X3D {

class Background::Textures
{
public:

	Textures (X3DExecutionContext* const executionContext) :
		front  (new ImageTexture (executionContext)),
		back   (new ImageTexture (executionContext)),
		left   (new ImageTexture (executionContext)),
		right  (new ImageTexture (executionContext)),
		top    (new ImageTexture (executionContext)),
		bottom (new ImageTexture (executionContext))
	{ }

	SFNode <ImageTexture> front;
	SFNode <ImageTexture> back;
	SFNode <ImageTexture> left;
	SFNode <ImageTexture> right;
	SFNode <ImageTexture> top;
	SFNode <ImageTexture> bottom;

};

Background::Background (X3DExecutionContext* const executionContext) :
	     X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DBackgroundNode (),                                 
	         frontUrl (),                                 // MFString [in,out] frontUrl   [ ]        [URI]
	          backUrl (),                                 // MFString [in,out] backUrl    [ ]        [URI]
	          leftUrl (),                                 // MFString [in,out] leftUrl    [ ]        [URI]
	         rightUrl (),                                 // MFString [in,out] rightUrl   [ ]        [URI]
	           topUrl (),                                 // MFString [in,out] topUrl     [ ]        [URI]
	        bottomUrl (),                                 // MFString [in,out] bottomUrl  [ ]        [URI]
	         textures ()
{
	setComponent ("EnvironmentalEffects");
	setTypeName ("Background");

	appendField (inputOutput, "metadata",     metadata);
	appendField (inputOnly,   "set_bind",     set_bind);
	appendField (outputOnly,  "bindTime",     bindTime);
	appendField (outputOnly,  "isBound",      isBound);
	appendField (inputOutput, "skyAngle",     skyAngle);
	appendField (inputOutput, "skyColor",     skyColor);
	appendField (inputOutput, "groundAngle",  groundAngle);
	appendField (inputOutput, "groundColor",  groundColor);
	appendField (inputOutput, "transparency", transparency);
	appendField (inputOutput, "frontUrl",     frontUrl);
	appendField (inputOutput, "backUrl",      backUrl);
	appendField (inputOutput, "leftUrl",      leftUrl);
	appendField (inputOutput, "rightUrl",     rightUrl);
	appendField (inputOutput, "topUrl",       topUrl);
	appendField (inputOutput, "bottomUrl",    bottomUrl);
}

X3DBasicNode*
Background::create (X3DExecutionContext* const executionContext) const
{
	return new Background (executionContext);
}

void
Background::initialize ()
{
	X3DBackgroundNode::initialize ();

	textures = new Textures (getExecutionContext ());

	frontUrl  .addCallback (&textures -> front  -> url);
	backUrl   .addCallback (&textures -> back   -> url);
	leftUrl   .addCallback (&textures -> left   -> url);
	rightUrl  .addCallback (&textures -> right  -> url);
	topUrl    .addCallback (&textures -> top    -> url);
	bottomUrl .addCallback (&textures -> bottom -> url);

	textures -> front  -> repeatS = textures -> front  -> repeatT = false;
	textures -> back   -> repeatS = textures -> back   -> repeatT = false;
	textures -> left   -> repeatS = textures -> left   -> repeatT = false;
	textures -> right  -> repeatS = textures -> right  -> repeatT = false;
	textures -> top    -> repeatS = textures -> top    -> repeatT = false;
	textures -> bottom -> repeatS = textures -> bottom -> repeatT = false;

	textures -> front  -> url = frontUrl;
	textures -> back   -> url = backUrl;
	textures -> left   -> url = leftUrl;
	textures -> right  -> url = rightUrl;
	textures -> top    -> url = topUrl;
	textures -> bottom -> url = bottomUrl;
}

void
Background::draw ()
{
	glPushMatrix ();

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

	if (textures -> front -> checkLoadState () == COMPLETE_STATE)
	{
		if (textures -> front -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		textures -> front -> display ();
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

	if (textures -> back -> checkLoadState () == COMPLETE_STATE)
	{
		if (textures -> front -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		textures -> back -> display ();
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

	if (textures -> left -> checkLoadState () == COMPLETE_STATE)
	{
		if (textures -> front -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		textures -> left -> display ();
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

	if (textures -> right -> checkLoadState () == COMPLETE_STATE)
	{
		if (textures -> front -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		textures -> right -> display ();
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

	if (textures -> top -> checkLoadState () == COMPLETE_STATE)
	{
		if (textures -> front -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		textures -> top -> display ();
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

	if (textures -> bottom -> checkLoadState () == COMPLETE_STATE)
	{
		if (textures -> front -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		textures -> bottom -> display ();
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
	glPopMatrix ();
}

void
Background::dispose ()
{
	delete textures;

	X3DBackgroundNode::dispose ();
}

} // X3D
} // titania
