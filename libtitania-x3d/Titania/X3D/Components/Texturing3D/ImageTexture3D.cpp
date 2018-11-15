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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "ImageTexture3D.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType ImageTexture3D::component      = ComponentType::TEXTURING_3D;
const std::string   ImageTexture3D::typeName       = "ImageTexture3D";
const std::string   ImageTexture3D::containerField = "texture";

ImageTexture3D::ImageTexture3D (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture3DNode (),
	    X3DUrlObject (),
	          buffer (),
	          future ()
{
	addType (X3DConstants::ImageTexture3D);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "url",               url ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "repeatR",           repeatR ());
	addField (initializeOnly, "textureProperties", textureProperties ());

	addChildObjects (buffer, future);
}

X3DBaseNode*
ImageTexture3D::create (X3DExecutionContext* const executionContext) const
{
	return new ImageTexture3D (executionContext);
}

void
ImageTexture3D::initialize ()
{
	X3DTexture3DNode::initialize ();
	X3DUrlObject::initialize ();

	url () .addInterest (&ImageTexture3D::set_url, this);

	buffer .addInterest (&ImageTexture3D::update, this);

	set_url ();
}

void
ImageTexture3D::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (future)
		future -> setExecutionContext (executionContext);

	X3DUrlObject::setExecutionContext (executionContext);
	X3DTexture3DNode::setExecutionContext (executionContext);
}

void
ImageTexture3D::setTexture (const basic::uri URL, const Texture3DPtr & texture)
{
	X3DTexture3DNode::setTexture (texture);

	if (texture)
	{
		setLoadState (COMPLETE_STATE);
		setLoadedUrl (URL);
	}
	else
	{
		setLoadState (FAILED_STATE);
		setLoadedUrl ("");
	}
}

void
ImageTexture3D::requestImmediateLoad ()
{
	requestAsyncLoad ();
}

void
ImageTexture3D::requestAsyncLoad ()
{
	if (not getBrowser () -> getLoadUrlObjects ())
		return;

	using namespace std::placeholders;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	future .setValue (new Texture3DFuture (getExecutionContext (),
	                                       url (),
	                                       getBrowser () -> getMinTextureSize (),
	                                       getBrowser () -> getMaxTextureSize (),
	                                       std::bind (&ImageTexture3D::setTexture, this, _1, _2)));
}

void
ImageTexture3D::set_url ()
{
	buffer .addEvent ();
}

void
ImageTexture3D::update ()
{
	setLoadState (NOT_STARTED_STATE);

	requestAsyncLoad ();
}

void
ImageTexture3D::dispose ()
{
	X3DUrlObject::dispose ();
	X3DTexture3DNode::dispose ();
}

ImageTexture3D::~ImageTexture3D ()
{ }

} // X3D
} // titania
