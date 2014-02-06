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

#include <gstreamermm.h>

#include "MovieTexture.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string MovieTexture::componentName  = "Texturing";
const std::string MovieTexture::typeName       = "MovieTexture";
const std::string MovieTexture::containerField = "texture";

MovieTexture::MovieTexture (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DTexture2DNode (),
	X3DSoundSourceNode (),
	      X3DUrlObject ()
{
	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "enabled",           enabled ());     // non standard
	addField (inputOutput,    "description",       description ());
	addField (inputOutput,    "url",               url ());
	addField (inputOutput,    "speed",             speed ());
	addField (inputOutput,    "pitch",             pitch ());
	addField (inputOutput,    "loop",              loop ());
	addField (inputOutput,    "startTime",         startTime ());
	addField (inputOutput,    "pauseTime",         pauseTime ());
	addField (inputOutput,    "resumeTime",        resumeTime ());
	addField (inputOutput,    "stopTime",          stopTime ());
	addField (outputOnly,     "isPaused",          isPaused ());
	addField (outputOnly,     "isActive",          isActive ());
	addField (outputOnly,     "cycleTime",         cycleTime ());   // non standard
	addField (outputOnly,     "elapsedTime",       elapsedTime ());
	addField (outputOnly,     "duration_changed",  duration_changed ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "textureProperties", textureProperties ());
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

	url () .addInterest (this, &MovieTexture::update);

	requestImmediateLoad ();
}

void
MovieTexture::requestImmediateLoad ()
{
	if (not glXGetCurrentContext ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	if (url () .empty ())
	{
		setLoadState (FAILED_STATE);
		return;
	}

	for (const auto & URL : url ())
	{
		setUri (getExecutionContext () -> getWorldURL () .transform (URL .str ()));

		// Sync stream

		if (not sync ())
			continue;

		// Set image
		
		// We use the c versions of this functions here because get_last_buffer has a memory leak.

		GstBuffer* buffer = gst_base_sink_get_last_buffer (Glib::RefPtr <Gst::BaseSink>::cast_static (getVideoSink ()) -> gobj ());

		if (buffer)
		{
			duration_changed () = getDuration ();

			setImage (GL_RGB, 3, 
			          getVideoSink () -> get_width (),
			          getVideoSink () -> get_height (),
			          GL_BGRA,
			          GST_BUFFER_DATA (buffer));

			gst_buffer_unref (buffer);

			setLoadState (COMPLETE_STATE);

			break;
		}
	}

	if (checkLoadState () not_eq COMPLETE_STATE)
	{
		duration_changed () = -1;

		setImage (GL_RGB, 3, 0, 0, GL_BGRA, nullptr);

		setLoadState (FAILED_STATE);
	}
}

void
MovieTexture::prepareEvents ()
{
	X3DSoundSourceNode::prepareEvents ();

	// We use the c versions of this functions here because get_last_buffer has a memory leak.

	GstBuffer* buffer = gst_base_sink_get_last_buffer (Glib::RefPtr <Gst::BaseSink>::cast_static (getVideoSink ()) -> gobj ());

	if (buffer)
	{
		updateImage (GL_BGRA,
		             getVideoSink () -> get_width (),
		             getVideoSink () -> get_height (),
		             GST_BUFFER_DATA (buffer));

		gst_buffer_unref (buffer);
	}	
}

void
MovieTexture::update ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();

	if (isActive () and not isPaused ())
	{
		if (loop ())
		{
			do_stop ();
			do_start ();
		}
		else
			do_stop ();
	}
}

void
MovieTexture::dispose ()
{
	X3DUrlObject::dispose ();
	X3DSoundSourceNode::dispose ();
	X3DTexture2DNode::dispose ();
}

} // X3D
} // titania
