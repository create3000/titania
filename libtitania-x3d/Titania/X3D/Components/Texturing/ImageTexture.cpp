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

#include "ImageTexture.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType ImageTexture::component      = ComponentType::TEXTURING;
const std::string   ImageTexture::typeName       = "ImageTexture";
const std::string   ImageTexture::containerField = "texture";

ImageTexture::ImageTexture (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture2DNode (),
	    X3DUrlObject (),
	      imageWidth (0),
	     imageHeight (0),
	           scale (true),
	          future ()
{
	addType (X3DConstants::ImageTexture);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "url",               url ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "textureProperties", textureProperties ());
}

X3DBaseNode*
ImageTexture::create (X3DExecutionContext* const executionContext) const
{
	return new ImageTexture (executionContext);
}

void
ImageTexture::initialize ()
{
	X3DTexture2DNode::initialize ();
	X3DUrlObject::initialize ();

	url () .addInterest (this, &ImageTexture::update);

	requestAsyncLoad ();
}

void
ImageTexture::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (future)
		future -> setExecutionContext (executionContext);

	X3DUrlObject::setExecutionContext (executionContext);
	X3DTexture2DNode::setExecutionContext (executionContext);
}

void
ImageTexture::setTexture (const TexturePtr & texture)
{
	X3DTexture2DNode::setTexture (texture);

	if (texture)
	{
		imageWidth  = texture -> getImageWidth ();
		imageHeight = texture -> getImageHeight ();
		setLoadState (COMPLETE_STATE);
	}
	else
	{
		imageWidth  = 0;
		imageHeight = 0;
		setLoadState (FAILED_STATE);
	}
}

void
ImageTexture::requestImmediateLoad ()
{
	requestAsyncLoad ();
}

void
ImageTexture::requestAsyncLoad ()
{
	if (not glXGetCurrentContext ())
		return;

	using namespace std::placeholders;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	future .reset (new TextureLoader (getExecutionContext (),
	                                  url (),
	                                  scale ? getBrowser () -> getMinTextureSize () : 0,
	                                  scale ? getBrowser () -> getMaxTextureSize () : 0,
	                                  std::bind (&ImageTexture::setTexture, this, _1)));
}

void
ImageTexture::update ()
{
	setLoadState (NOT_STARTED_STATE);

	requestAsyncLoad ();
}

void
ImageTexture::dispose ()
{
	if (future)
		future -> dispose ();

	X3DUrlObject::dispose ();
	X3DTexture2DNode::dispose ();
}

} // X3D
} // titania
