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

#include "X3DSoundSourceNode.h"

#include "../../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

/* Playback speed
 *   http://docs.gstreamer.com/display/GstSDK/Basic+tutorial+13%3A+Playback+speed
 * Player
 *   http://code.metager.de/source/xref/freedesktop/gstreamer/gstreamermm/examples/media_player_gtkmm/player_window.cc
 * PlayBin2
 *   http://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/gst-plugins-base-plugins-0.10/gst-plugins-base-plugins-playbin2.html
 */

class X3DSoundSourceNode::GStream
{
public:

	GStream (X3DSoundSourceNode* source) :
		source (source),
		player (),
		message (),
		pixmap (0),
		display (NULL)
	{
		// Static init

		int    argc = 0;
		char** argv = NULL;

		Gst::init_check (argc, argv);

		// X11

		if (glXGetCurrentContext ())
			display = XOpenDisplay (NULL);

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
				source -> set_end ();
				break;
			}
			case Gst::MESSAGE_ERROR:
			{
				__LOG__ << "MESSAGE_ERROR" << std::endl;
				source -> set_stop ();
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

		if (pixmap)
			XFreePixmap (display, pixmap);

		if (display)
			XCloseDisplay (display);
	}

	X3DSoundSourceNode* source;

	Glib::RefPtr <Gst::PlayBin>    player;
	Glib::RefPtr <Gst::XImageSink>  vsink;
	sigc::connection               message;

	Pixmap   pixmap;
	Display* display;

};

X3DSoundSourceNode::X3DSoundSourceNode () :
	X3DTimeDependentNode (),                  
	             enabled (true),              // SFBool   [in,out] enabled           TRUE       non standard
	         description (),                  // SFString [in,out] description       ""
	               speed (1),                 // SFFloat [in,out] speed  1.0        (-∞,∞)
	               pitch (1),                 // SFFloat  [in,out] pitch             1.0        (0,∞)
	            isActive (),                  // SFBool   [ ]      isActive
	    duration_changed (-1),                // SFTime   [out]    duration_changed
	             gstream (new GStream (this)) 
{
	addNodeType (X3DConstants::X3DSoundSourceNode);
}

void
X3DSoundSourceNode::initialize ()
{
	X3DTimeDependentNode::initialize ();

	speed .addInterest (this, &X3DSoundSourceNode::set_speed);
	pitch .addInterest (this, &X3DSoundSourceNode::set_pitch);
}

void
X3DSoundSourceNode::setUri (const basic::uri & uri)
{
	gstream -> setUri (uri);
}

float
X3DSoundSourceNode::getDuration ()
{
	return gstream -> getDuration ();
}

bool
X3DSoundSourceNode::sync ()
{
	return gstream -> getState () not_eq Gst::STATE_NULL;
}

const Glib::RefPtr <Gst::XImageSink> &
X3DSoundSourceNode::getVideoSink ()
{
	return gstream -> vsink;
}

void
X3DSoundSourceNode::prepareEvents ()
{
	elapsedTime = getElapsedTime ();
}

void
X3DSoundSourceNode::set_speed ()
{
	// Speed is not supported maybe with playbin2
}

void
X3DSoundSourceNode::set_pitch ()
{ }

void
X3DSoundSourceNode::set_start ()
{
	if (not isActive)
	{
		if (speed)
			gstream -> start (speed, 0);

		isActive    = true;
		cycleTime   = getCurrentTime ();
		elapsedTime = 0;

		getBrowser () -> prepareEvents .addInterest (this, &X3DSoundSourceNode::prepareEvents);
	}
}

void
X3DSoundSourceNode::set_stop ()
{
	if (isActive)
	{
		gstream -> stop ();
		isActive = false;

		getBrowser () -> prepareEvents .removeInterest (this, &X3DSoundSourceNode::prepareEvents);
	}
}

void
X3DSoundSourceNode::set_pause ()
{ }

void
X3DSoundSourceNode::set_resume ()
{ }

void
X3DSoundSourceNode::set_end ()
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

		getBrowser () -> prepareEvents .removeInterest (this, &X3DSoundSourceNode::prepareEvents);
	}
}

void
X3DSoundSourceNode::dispose ()
{
	gstream .reset ();

	getBrowser () -> prepareEvents .removeInterest (this, &X3DSoundSourceNode::prepareEvents);

	X3DTimeDependentNode::dispose ();
}

} // X3D
} // titania
