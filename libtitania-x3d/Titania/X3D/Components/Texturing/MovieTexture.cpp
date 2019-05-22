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

#include "MovieTexture.h"

#include "../../Browser/Sound/MediaStream.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   MovieTexture::component      = Component ("Texturing", 3);
const std::string MovieTexture::typeName       = "MovieTexture";
const std::string MovieTexture::containerField = "texture";

MovieTexture::MovieTexture (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DTexture2DNode (),
	X3DSoundSourceNode (true),
	      X3DUrlObject (),
	   sigc::trackable (),
	            buffer (),
	          urlStack (),
	               URL ()
{
	addType (X3DConstants::MovieTexture);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "enabled",           enabled ());     // non standard
	addField (inputOutput,    "description",       description ());
	addField (inputOutput,    "url",               url ());
	addField (inputOutput,    "speed",             speed ());
	addField (inputOutput,    "pitch",             pitch ());
	addField (inputOutput,    "loop",              loop ());
	addField (inputOutput,    "startTime",         startTime ());
	addField (inputOutput,    "resumeTime",        resumeTime ());
	addField (inputOutput,    "pauseTime",         pauseTime ());
	addField (inputOutput,    "stopTime",          stopTime ());
	addField (outputOnly,     "isPaused",          isPaused ());
	addField (outputOnly,     "isActive",          isActive ());
	addField (outputOnly,     "cycleTime",         cycleTime ());   // non standard
	addField (outputOnly,     "elapsedTime",       elapsedTime ());
	addField (outputOnly,     "duration_changed",  duration_changed ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "textureProperties", textureProperties ());

	addChildObjects (buffer);
}

X3DBaseNode*
MovieTexture::create (X3DExecutionContext* const executionContext) const
{
	return new MovieTexture (executionContext);
}

void
MovieTexture::initialize ()
{
	X3DTexture2DNode::initialize ();
	X3DSoundSourceNode::initialize ();
	X3DUrlObject::initialize ();

	getStream () -> signal_video_changed ()  .connect (sigc::mem_fun (this, &MovieTexture::on_video_changed));
	getStream () -> signal_error ()          .connect (sigc::mem_fun (this, &MovieTexture::on_error));
	getStream () -> signal_buffer_changed () .connect (sigc::mem_fun (this, &MovieTexture::on_buffer_changed));

	url () .addInterest (&MovieTexture::set_url, this);

	buffer .addInterest (&MovieTexture::set_buffer, this);

	set_url ();
}

void
MovieTexture::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DUrlObject::setExecutionContext (executionContext);
	X3DTexture2DNode::setExecutionContext (executionContext);
}

const X3DPtr <TextureProperties> &
MovieTexture::getDefaultTextureProperties () const
{
	return getBrowser () -> getDefaultMovieTextureProperties ();
}

void
MovieTexture::requestImmediateLoad ()
{
	if (not getBrowser () -> getLoadUrlObjects ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	buffer .addEvent ();
}

void
MovieTexture::set_buffer ()
{
	urlStack = url ();

	setMedia (false);
	load ();
}

void
MovieTexture::load ()
{
	if (urlStack .empty ())
	{
		stop ();
		duration_changed () = -1;
		components ()       = 0;

		clearTexture ();

		setLoadState (FAILED_STATE);
		setLoadedUrl ("");
		return;
	}

	URL = urlStack .front ();
	urlStack .pop_front ();

	getStream () -> setUri (getExecutionContext () -> getWorldURL () .transform (URL .str ()));
}

void
MovieTexture::on_video_changed ()
{
	components () = 3;

	setMedia (true);
	setLoadState (COMPLETE_STATE);
	setLoadedUrl (getStream () -> getUri ());
}

void
MovieTexture::on_error ()
{
	if (checkLoadState () not_eq IN_PROGRESS_STATE)
		return;

	load ();
}

void
MovieTexture::on_buffer_changed ()
{
	try
	{
		ContextLock lock (getBrowser ());

		const auto frame = getStream () -> getCurrentFrame ();

		if (frame)
		{
			if (width () not_eq frame -> width)
				width () = frame -> width;

			if (height () not_eq frame -> height)
				height () = frame -> height;

			if (width () .getTainted () or height () .getTainted ())
				setImage (GL_RGB, 3, width (), height (), GL_BGRA, frame -> image .data ());
			else
				updateImage (GL_BGRA, frame -> image .data ());
		}
	}
	catch (const X3DError & error)
	{
	   __LOG__ << error .what () << std::endl;
	}
}

void
MovieTexture::set_url ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();
}

void
MovieTexture::update ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();
}

void
MovieTexture::dispose ()
{
	notify_callbacks ();

	X3DUrlObject::dispose ();
	X3DSoundSourceNode::dispose ();
	X3DTexture2DNode::dispose ();
}

MovieTexture::~MovieTexture ()
{ }

} // X3D
} // titania
