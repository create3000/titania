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
 ******************************************************************************/

#include "Background.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Texturing/ImageTexture.h"

#include <iostream>

namespace titania {
namespace X3D {

Background::Background (X3DExecutionContext* const executionContext, bool displayed) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DBackgroundNode (displayed),                                                    
	         frontUrl (),                                                    // MFString [in,out] frontUrl   [ ]        [URI]
	          backUrl (),                                                    // MFString [in,out] backUrl    [ ]        [URI]
	          leftUrl (),                                                    // MFString [in,out] leftUrl    [ ]        [URI]
	         rightUrl (),                                                    // MFString [in,out] rightUrl   [ ]        [URI]
	           topUrl (),                                                    // MFString [in,out] topUrl     [ ]        [URI]
	        bottomUrl (),                                                    // MFString [in,out] bottomUrl  [ ]        [URI]
	     frontTexture (new ImageTexture (executionContext)),                 
	      backTexture (new ImageTexture (executionContext)),                 
	      leftTexture (new ImageTexture (executionContext)),                 
	     rightTexture (new ImageTexture (executionContext)),                 
	       topTexture (new ImageTexture (executionContext)),                 
	    bottomTexture (new ImageTexture (executionContext))                  
{
	setComponent ("EnvironmentalEffects");
	setTypeName ("Background");

	addField (inputOutput, "metadata",     metadata);
	addField (inputOnly,   "set_bind",     set_bind);
	addField (outputOnly,  "bindTime",     bindTime);
	addField (outputOnly,  "isBound",      isBound);
	addField (inputOutput, "skyAngle",     skyAngle);
	addField (inputOutput, "skyColor",     skyColor);
	addField (inputOutput, "groundAngle",  groundAngle);
	addField (inputOutput, "groundColor",  groundColor);
	addField (inputOutput, "transparency", transparency);
	addField (inputOutput, "frontUrl",     frontUrl);
	addField (inputOutput, "backUrl",      backUrl);
	addField (inputOutput, "leftUrl",      leftUrl);
	addField (inputOutput, "rightUrl",     rightUrl);
	addField (inputOutput, "topUrl",       topUrl);
	addField (inputOutput, "bottomUrl",    bottomUrl);

	setChildren (frontTexture,
	             backTexture,
	             leftTexture,
	             rightTexture,
	             topTexture,
	             bottomTexture);
}

X3DBaseNode*
Background::create (X3DExecutionContext* const executionContext) const
{
	return new Background (executionContext, true);
}

void
Background::initialize ()
{
	X3DBackgroundNode::initialize ();

	frontUrl  .addInterest (frontTexture  -> url);
	backUrl   .addInterest (backTexture   -> url);
	leftUrl   .addInterest (leftTexture   -> url);
	rightUrl  .addInterest (rightTexture  -> url);
	topUrl    .addInterest (topTexture    -> url);
	bottomUrl .addInterest (bottomTexture -> url);

	frontTexture  -> repeatS = frontTexture  -> repeatT = false;
	backTexture   -> repeatS = backTexture   -> repeatT = false;
	leftTexture   -> repeatS = leftTexture   -> repeatT = false;
	rightTexture  -> repeatS = rightTexture  -> repeatT = false;
	topTexture    -> repeatS = topTexture    -> repeatT = false;
	bottomTexture -> repeatS = bottomTexture -> repeatT = false;

	frontTexture  -> url = frontUrl;
	backTexture   -> url = backUrl;
	leftTexture   -> url = leftUrl;
	rightTexture  -> url = rightUrl;
	topTexture    -> url = topUrl;
	bottomTexture -> url = bottomUrl;
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

	if (frontTexture -> checkLoadState () == COMPLETE_STATE)
	{
		if (frontTexture -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		frontTexture -> display ();
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

		backTexture -> display ();
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

		leftTexture -> display ();
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

		rightTexture -> display ();
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

		topTexture -> display ();
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

		bottomTexture -> display ();
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

} // X3D
} // titania
