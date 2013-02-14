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

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

/* Playback speed
 *   http://docs.gstreamer.com/display/GstSDK/Basic+tutorial+13%3A+Playback+speed
 * Player
 *   http://code.metager.de/source/xref/freedesktop/gstreamer/gstreamermm/examples/media_player_gtkmm/player_window.cc
 * PlayBin2
 *   http://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/gst-plugins-base-plugins-0.10/gst-plugins-base-plugins-playbin2.html
 */

namespace titania {
namespace X3D {

class MovieTexture::GStream
{
public:

	GStream (MovieTexture* movie) :
		movie (movie),
		player (),
		vsink (),
		message (),
		pixmap (0),
		display (XOpenDisplay (NULL))
	{
		// Static init

		int    argc = 0;
		char** argv = NULL;

		Gst::init_check (argc, argv);

		// X11

		if (display)
			pixmap = XCreatePixmap (display, 0, 0, 0, 0);

		// Construct

		player = Gst::PlayBin::create ("player");
		vsink  = Gst::XImageSink::create ("vsink");

		player -> set_property ("video-sink", vsink);

		auto bus = player -> get_bus ();

		bus -> enable_sync_message_emission ();
		bus -> signal_sync_message ().connect (sigc::mem_fun (*this, &GStream::on_bus_message_sync));

		bus -> add_signal_watch ();
		message = bus -> signal_message () .connect (sigc::mem_fun (*this, &GStream::on_message));
	}

	bool
	setUri (const basic::uri & uri)
	{
		player -> set_property ("uri", uri .str ());
		player -> set_state (Gst::STATE_PAUSED);

		return true;
	}

	time_type
	getDuration () const
	{
		auto   format   = Gst::FORMAT_TIME;
		gint64 duration = 0;

		if (player -> query_duration (format, duration))
			return duration / 1000000000.0;

		return -1;
	}

	Gst::State
	getState () const
	{
		Gst::State             state;
		Gst::State             pending;
		Gst::StateChangeReturn ret = player -> get_state (state, pending, 10 * Gst::SECOND);

		if (ret == Gst::STATE_CHANGE_SUCCESS)
		{
			return state;
		}
		else if (ret == Gst::STATE_CHANGE_ASYNC)
		{
			//Log::debug ("Query state failed, still performing change");
		}
		else
		{
			//Log::debug ("Query state failed, hard failure");
		}

		return Gst::STATE_NULL;
	}

//	void
//	setSpeed (double speed)
//	{
//		auto format = Gst::FORMAT_TIME;
//		
//		if (speed > 0)
//		{
//			player -> seek (speed,
//			                format,
//			                Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
//			                Gst::SEEK_TYPE_CUR, 0,
//			                Gst::SEEK_TYPE_END, 0);
//		}
//		else
//		{
//			gint64 position = 0;
//
//			if (player -> query_position (format, position))
//			{
//				__LOG__ << position / 1000000000.0 << std::endl;
//				
//				player -> seek (speed,
//				                format,
//				                Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
//				                Gst::SEEK_TYPE_SET, 0,
//				                Gst::SEEK_TYPE_SET, position);
//			}
//			else
//				__LOG__ << "*** query_position failed" << std::endl;
//		}
//	}

	void
	start (double speed, time_type position)
	{
		auto format = Gst::FORMAT_TIME;
		
		player -> seek (format,
		                Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
		                position * 1000000000.0);
			
		player -> set_state (Gst::STATE_PLAYING);
	}

	void
	stop ()
	{
		player -> set_state (Gst::STATE_PAUSED);
	}

	void
	on_message (const Glib::RefPtr <Gst::Message> & message)
	{
		switch (message -> get_message_type ())
		{
			case Gst::MESSAGE_EOS:
			{
				movie -> set_end ();
				break;
			}
			case Gst::MESSAGE_ERROR:
			{
				__LOG__ << "MESSAGE_ERROR" << std::endl;
				movie -> set_stop ();
				break;
			}
			default:
			{ }
		}
	}

	void
	on_bus_message_sync (const Glib::RefPtr <Gst::Message> & message)
	{
		// ignore anything but 'prepare-xwindow-id' element messages
		if (message -> get_message_type () not_eq Gst::MESSAGE_ELEMENT)
			return;

		if (not message -> get_structure () .has_name ("prepare-xwindow-id"))
			return;

		auto element  = Glib::RefPtr <Gst::Element>::cast_dynamic (message -> get_source ());
		auto xoverlay = Gst::Interface::cast <Gst::XOverlay> (element);

		if (xoverlay)
		{
			if (pixmap)
				xoverlay -> set_xwindow_id (pixmap);
		}
	}

	~GStream ()
	{
		message .disconnect ();
		player -> set_state (Gst::STATE_NULL);

		XFreePixmap (display, pixmap);
		XCloseDisplay (display);
	}

	MovieTexture* movie;

	Glib::RefPtr <Gst::PlayBin>    player;
	Glib::RefPtr <Gst::XImageSink> vsink;
	sigc::connection               message;

	Pixmap   pixmap;
	Display* display;

};

MovieTexture::MovieTexture (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	  X3DTexture2DNode (),                                                    
	X3DSoundSourceNode (),                                                    
	      X3DUrlObject (),                                                    
	             speed (1),                                                   // SFFloat [in,out] speed  1.0        (-∞,∞)
	           gstream ()                                                     
{
	setComponent ("Texturing");
	setTypeName ("MovieTexture");

	addField (inputOutput,    "metadata",          metadata);
	addField (inputOutput,    "enabled",           enabled);     // non standard
	addField (inputOutput,    "description",       description);
	addField (inputOutput,    "url",               url);
	addField (inputOutput,    "speed",             speed);
	addField (inputOutput,    "pitch",             pitch);
	addField (initializeOnly, "repeatS",           repeatS);
	addField (initializeOnly, "repeatT",           repeatT);
	addField (inputOutput,    "loop",              loop);
	addField (inputOutput,    "startTime",         startTime);
	addField (inputOutput,    "stopTime",          stopTime);
	addField (inputOutput,    "pauseTime",         pauseTime);
	addField (inputOutput,    "resumeTime",        resumeTime);
	addField (outputOnly,     "isPaused",          isPaused);
	addField (outputOnly,     "isActive",          isActive);
	addField (outputOnly,     "cycleTime",         cycleTime);   // non standard
	addField (outputOnly,     "elapsedTime",       elapsedTime);
	addField (outputOnly,     "duration_changed",  duration_changed);
	addField (initializeOnly, "textureProperties", textureProperties);
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

	initialized .addInterest (this, &MovieTexture::set_initialized);

	url   .addInterest (this, &MovieTexture::set_url);
	speed .addInterest (this, &MovieTexture::set_speed);
	pitch .addInterest (this, &MovieTexture::set_pitch);
}

void
MovieTexture::set_initialized ()
{
	gstream .reset (new GStream (this));

	requestImmediateLoad ();
}

void
MovieTexture::requestImmediateLoad ()
{
	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	if (not url .size ())
	{
		setLoadState (FAILED_STATE);
		return;
	}

	auto vsink = gstream -> vsink;

	for (const auto & URL : url)
	{
		gstream -> setUri (transformURI (URL .getValue () .raw ()));

		// Sync stream

		if (gstream -> getState () == Gst::STATE_NULL)
			continue;

		// Set image

		if (vsink -> get_last_buffer ())
		{
			duration_changed = gstream -> getDuration ();

			setImage (4, GL_BGRA,
			          vsink -> get_width (),
			          vsink -> get_height (),
			          vsink -> get_last_buffer () -> get_data ());

			setLoadState (COMPLETE_STATE);

			break;
		}
	}

	if (not vsink -> get_last_buffer ())
	{
		duration_changed = -1;
		setLoadState (FAILED_STATE);
	}
}

void
MovieTexture::prepareEvents ()
{
	elapsedTime = getElapsedTime ();
}

void
MovieTexture::set_url ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();
}

void
MovieTexture::set_speed ()
{
	// Speed is not supported maybe with playbin2
}

void
MovieTexture::set_pitch ()
{ }

void
MovieTexture::set_start ()
{
	if (not isActive)
	{
		if (speed)
			gstream -> start (speed, 0);

		isActive    = true;
		cycleTime   = getCurrentTime ();
		elapsedTime = 0;

		getBrowser () -> prepareEvents .addInterest (this, &MovieTexture::prepareEvents);
	}
}

void
MovieTexture::set_stop ()
{
	if (isActive)
	{
		gstream -> stop ();
		isActive = false;

		getBrowser () -> prepareEvents .removeInterest (this, &MovieTexture::prepareEvents);
	}
}

void
MovieTexture::set_pause ()
{ }

void
MovieTexture::set_resume ()
{ }

void
MovieTexture::set_end ()
{
	// XXX thread save???

	if (loop)
	{
		if (speed)
			gstream -> start (speed, 0);

		cycleTime   = getCurrentTime ();
		elapsedTime = getElapsedTime ();
	}
	else
	{
		isActive = false;

		getBrowser () -> prepareEvents .removeInterest (this, &MovieTexture::prepareEvents);
	}
}

void
MovieTexture::display ()
{
	auto vsink = gstream -> vsink;

	if (vsink -> get_last_buffer ())
	{
		updateImage (GL_BGRA,
		             vsink -> get_width (),
		             vsink -> get_height (),
		             vsink -> get_last_buffer () -> get_data ());

		X3DTexture2DNode::display ();
	}
}

void
MovieTexture::dispose ()
{
	gstream .reset ();

	getBrowser () -> prepareEvents .removeInterest (this, &MovieTexture::prepareEvents);

	X3DUrlObject::dispose ();
	X3DSoundSourceNode::dispose ();
	X3DTexture2DNode::dispose ();
}

} // X3D
} // titania
