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

#include "../../Rendering/OpenGL.h"

#include "MediaStream.h"

extern "C"
{
#include <gdk/gdkx.h>
}

namespace titania {
namespace X3D {

/* GStreamer
 *   https://developer.gnome.org/gstreamermm/1.4/classGst_1_1Sample.html
 * PlayBin
 *   https://github.com/GNOME/gstreamermm/blob/master/examples/media_player_gtkmm/player_window.cc
 */

MediaStream::MediaStream (const Glib::RefPtr <Gdk::Display> & display) :
	                  display (display),
	                  xWindow (createWindow (gdk_x11_display_get_xdisplay (display -> gobj ()), 16, 16)),
	                   player (),
	                    vsink (),
	             currentFrame (),
	                   volume (0),
	                    speed (1),
	                 duration (-1),
	             emitDuration (false),
	   videoChangedDispatcher (),
	  bufferChangedDispatcher (),
	            endDispatcher (),
	durationChangedDispatcher ()
{
	// Static init

	int32_t argc = 0;
	char**  argv = nullptr;

	Gst::init_check (argc, argv);

	// Construct

	player = Player::create ("player");
	vsink  = VideoSink::create ("vsink");
}

Window
MediaStream::createWindow (Display* const xDisplay, const int32_t width, const int32_t height) const
{
	const auto xScreen = DefaultScreen (xDisplay);
	const auto xBlack  = BlackPixel (xDisplay, xScreen);
	const auto xWhite  = WhitePixel (xDisplay, xScreen);

	return XCreateSimpleWindow (xDisplay, RootWindow (xDisplay, xScreen), 0, 0, width, height, 0, xBlack, xWhite);
}

void
MediaStream::setup ()
{
	// GStreamer

	const auto bus = player -> get_bus ();

	bus -> enable_sync_message_emission ();
	bus -> signal_sync_message () .connect (sigc::mem_fun (this, &MediaStream::on_bus_message_sync));

	bus -> add_signal_watch ();
	bus -> signal_message () .connect (sigc::mem_fun (this, &MediaStream::on_message));

	vsink -> set_last_sample_enabled (true);
	vsink -> handle_events (false);

	player -> property_video_sink () = vsink;
	player -> property_volume ()     = volume;
	player -> signal_video_changed () .connect (sigc::mem_fun (this, &MediaStream::on_video_changed));
	player -> signal_audio_changed () .connect (sigc::mem_fun (this, &MediaStream::on_audio_changed));
}

bool
MediaStream::setUri (const basic::uri & uri)
{
	emitDuration = true;

	player -> set_state (Gst::STATE_NULL);

	player -> property_volume () = volume = 0;
	player -> property_uri ()    = uri .str ();
	player -> set_state (Gst::STATE_PAUSED);

	return true;
}

time_type
MediaStream::getQueryDuration () const
{
	gint64 duration = 0;

	if (vsink -> query_duration (Gst::FORMAT_TIME, duration) and duration >= 0)
		return duration / time_type (Gst::SECOND);

	return -1;
}

void
MediaStream::setVolume (time_type value)
{
	static constexpr double granularity = 0.05;

	value = std::round (math::clamp (value, 0.0, 1.0) / granularity) * granularity;

	if (value not_eq volume)
	{
		volume = value;
		player -> property_volume () = value;
	}
}

void
MediaStream::setSpeed (const time_type value)
{
	speed = value;
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
//				__LOG__ << position / time_type (Gst::SECOND) << std::endl;
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

Gst::State
MediaStream::getState () const
{
	Gst::State             state;
	Gst::State             pending;
	Gst::StateChangeReturn ret = player -> get_state (state, pending, 1 * Gst::SECOND);

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

bool
MediaStream::sync () const
{
	return getState () not_eq Gst::STATE_NULL;
}

void
MediaStream::seek (const time_type position)
{
//	if (position)
//	{
//		player -> seek (speed,
//				          Gst::FORMAT_TIME,
//				          Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
//				          Gst::SEEK_TYPE_SET, position * time_type (Gst::SECOND),
//				          Gst::SEEK_TYPE_SET, 0);
//	}
//
//	player -> set_state (Gst::STATE_NULL);
}

void
MediaStream::start ()
{
	player -> set_state (Gst::STATE_NULL);
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
	if (not gst_is_video_overlay_prepare_window_handle_message (message -> gobj ()))
		return;

	if (not xWindow)
	{
		stop ();
		return;
	}

	XMoveResizeWindow (gdk_x11_display_get_xdisplay (display -> gobj ()), xWindow, 0, 0, vsink -> get_width (), vsink -> get_height ());

	vsink -> set_window_handle (xWindow);
}

void
MediaStream::on_message (const Glib::RefPtr <Gst::Message> & message)
{
	switch (message -> get_message_type ())
	{
		case Gst::MESSAGE_EOS:
		{
			// XXX: Force set volume, as the volume is interally reseted to maximum sometimes.
			player -> property_volume () = volume;

			endDispatcher .emit ();
			break;
		}
		case Gst::MESSAGE_ASYNC_DONE:
		{
			// XXX: Force set volume, as the volume is interally reseted to maximum sometimes.
			player -> property_volume () = volume;

			if (emitDuration)
			{
				emitDuration = false;
				duration     = getQueryDuration ();
				durationChangedDispatcher .emit ();
			}
			break;
		}
		case Gst::MESSAGE_ERROR:
		{
//			__LOG__
//				<< "MESSAGE_ERROR: "
//				<< Glib::RefPtr <Gst::MessageError>::cast_static (message) -> parse () .what ()
//				<< std::endl;

			stop ();
			break;
		}
		default:
			break;
	}
}

void
MediaStream::on_audio_changed ()
{
}

void
MediaStream::on_video_changed ()
{
	Glib::RefPtr <Gst::Pad> pad = player -> get_video_pad (0);

	if (pad)
		pad -> add_probe (Gst::PAD_PROBE_TYPE_BUFFER, sigc::mem_fun (this, &MediaStream::on_video_pad_got_buffer));

	videoChangedDispatcher .emit ();
}

Gst::PadProbeReturn
MediaStream::on_video_pad_got_buffer (const Glib::RefPtr <Gst::Pad> & pad, const Gst::PadProbeInfo & data)
{
	// This function is process in another thread!

	const auto width  = vsink -> get_width ();
	const auto height = vsink -> get_height ();
	const auto size   = width * 4 * height;
	const auto sample = vsink -> get_last_sample ();

	if (sample)
	{
		const auto buffer = sample -> get_buffer ();

		if (buffer)
		{
			const auto frame = std::make_shared <VideoFrame> ();

			frame -> width  = width;
			frame -> height = height;
			frame -> image .resize (size);

			buffer -> extract (0, frame -> image .data (), frame -> image .size ());

			flip (frame -> image, frame -> width, frame -> height);

			currentFrame = frame;

			bufferChangedDispatcher .emit ();
		}
	}

	return Gst::PAD_PROBE_OK;
}

void
MediaStream::flip (std::vector <uint8_t> & image, const int32_t width, const int32_t height)
{
	// Flip image vertically

	const size_t width4   = width * 4;
	const size_t height_1 = height - 1;

	for (size_t r = 0, height1_2 = height / 2; r < height1_2; ++ r)
	{
		for (size_t c = 0; c < width4; ++ c)
		{
			std::swap (image [r * width4 + c], image [(height_1 - r) * width4 + c]);
		}
	}
}

MediaStream::~MediaStream ()
{
	player -> set_state (Gst::STATE_NULL);
	sync ();

	if (xWindow)
		XDestroyWindow (gdk_x11_display_get_xdisplay (display -> gobj ()), xWindow);
}

} // X3D
} // titania
