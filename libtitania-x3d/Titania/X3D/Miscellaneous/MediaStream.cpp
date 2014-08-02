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

#include "MediaStream.h"

#include "../Rendering/OpenGL.h"

namespace titania {
namespace X3D {

/* Playback speed
 *   http://docs.gstreamer.com/display/GstSDK/Basic+tutorial+13%3A+Playback+speed
 * Player
 *   http://code.metager.de/source/xref/freedesktop/gstreamer/gstreamermm/examples/media_player_gtkmm/player_window.cc
 * PlayBin2
 *   http://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/gst-plugins-base-plugins-0.10/gst-plugins-base-plugins-playbin2.html
 */

MediaStream::MediaStream () :
	 player (),
	  vsink (),
	 pixmap (0),
	display (nullptr),
	 volume (0)
{
	// Static init

	int    argc = 0;
	char** argv = nullptr;

	Gst::init_check (argc, argv);

	// Construct

	player = Player::create ("player");
	vsink  = VideoSink::create ("vsink");

	player -> set_property ("video-sink", vsink);
	player -> set_property ("volume", volume);

	const auto bus = player -> get_bus ();
	bus -> enable_sync_message_emission ();
	bus -> signal_sync_message () .connect (sigc::mem_fun (*this, &MediaStream::on_bus_message_sync));
}

void
MediaStream::setup ()
{
	// X11

	if (glXGetCurrentContext ())
		display = XOpenDisplay (nullptr);

	if (display)
		pixmap = XCreatePixmap (display, 0, 0, 0, 0);
}

bool
MediaStream::setUri (const basic::uri & uri)
{
	player -> set_state (Gst::STATE_NULL);
	player -> set_property ("uri", uri .str ());
	player -> set_state (Gst::STATE_PAUSED);

	return true;
}

double
MediaStream::getDuration () const
{
	auto   format   = Gst::FORMAT_TIME;
	gint64 duration = 0;

	if (player -> query_duration (format, duration))
		return duration / 1000000000.0;

	return -1;
}

void
MediaStream::setVolume (double value)
{
	value = math::clamp (value, 0.0, 1.0);

	if (std::abs (value - volume) > 0.05)
	{
		volume = value;
		player -> set_property ("volume", volume);
	}
}

Gst::State
MediaStream::getState () const
{
	Gst::State             state;
	Gst::State             pending;
	Gst::StateChangeReturn ret = player -> get_state (state, pending, 10 * Gst::SECOND);

	switch (ret)
	{
		case Gst::STATE_CHANGE_SUCCESS:
		{
			return state;
		}
		case Gst::STATE_CHANGE_ASYNC:
		{
			__LOG__ << "Query state failed, still performing change" << std::endl;
			break;
		}
		default:
		{
			__LOG__ << "Query state failed, hard failure" << std::endl;
			break;
		}
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

bool
MediaStream::sync () const
{
	return getState () not_eq Gst::STATE_NULL;
}

void
MediaStream::start (const double speed, const double position)
{
	const auto format = Gst::FORMAT_TIME;

	player -> seek (format,
	                Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
	                position * 1000000000.0);

	player -> set_state (Gst::STATE_PLAYING);
}

void
MediaStream::pause ()
{
	player -> set_state (Gst::STATE_PAUSED);
}

void
MediaStream::resume ()
{
	player -> set_state (Gst::STATE_PLAYING);
}

void
MediaStream::stop ()
{
	player -> set_state (Gst::STATE_PAUSED);
}

void
MediaStream::on_bus_message_sync (const Glib::RefPtr <Gst::Message> & message)
{
	// ignore anything but 'prepare-xwindow-id' element messages
	if (message -> get_message_type () not_eq Gst::MESSAGE_ELEMENT)
		return;

	if (not message -> get_structure () .has_name ("prepare-xwindow-id"))
		return;

	const auto element  = Glib::RefPtr <Gst::Element>::cast_dynamic (message -> get_source ());
	const auto xOverlay = Gst::Interface::cast <Gst::XOverlay> (element);

	if (xOverlay)
	{
		if (pixmap)
			xOverlay -> set_xwindow_id (pixmap);
	}
}

MediaStream::~MediaStream ()
{
	player -> set_state (Gst::STATE_NULL);

	sync ();

	if (pixmap)
		XFreePixmap (display, pixmap);

	if (display)
		XCloseDisplay (display);
}

} // X3D
} // titania
