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

#include <Titania/Stream/InputFileStream.h>
#include <glibmm.h>

extern "C"
{
#include <gdk/gdkx.h>
#include <X11/Xatom.h>
}

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

/* GStreamer
 *   https://developer.gnome.org/gstreamermm/1.4/classGst_1_1Sample.html
 * PlayBin
 *   https://github.com/GNOME/gstreamermm/blob/master/examples/media_player_gtkmm/player_window.cc
 */

MediaStream::MediaStream (const bool video) :
	                    video (video),
	                 xDisplay (nullptr),
	                  xWindow (0),
	                   player (),
	                    vsink (),
	                   thread (),
	                    width (-1),
	                   height (-1),
	             currentFrame (),
	               frameMutex (),
	                emitAudio (false),
	                emitVideo (false),
	                   volume (0),
	                    speed (1),
	              updateSpeed (false),
	                 duration (-1),
	             emitDuration (false),
	   audioChangedDispatcher (),
	   videoChangedDispatcher (),
	          errorDispatcher (),
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

	if (video)
		vsink  = VideoSink::create ("vsink");
}

Display*
MediaStream::getDisplay () const
{
	if (not xDisplay)
		const_cast <MediaStream*> (this) -> xDisplay = XOpenDisplay (nullptr);

	return xDisplay;
}

Window
MediaStream::getWindow () const
{
	if (not xWindow)
		const_cast <MediaStream*> (this) -> xWindow = createWindow (getDisplay (), 16, 16);

	return xWindow;
}

Window
MediaStream::createWindow (Display* const xDisplay, const int32_t width, const int32_t height) const
{
	if (not xDisplay)
		return 0;

	const auto xScreen = DefaultScreen (xDisplay);
	const auto xBlack  = BlackPixel (xDisplay, xScreen);
	const auto xWhite  = WhitePixel (xDisplay, xScreen);
	const auto xWindow = XCreateSimpleWindow (xDisplay, RootWindow (xDisplay, xScreen), 0, 0, width, height, 0, xBlack, xWhite);

	XSync (xDisplay, FALSE);

	return xWindow;
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

	if (video)
	{
		vsink -> set_last_sample_enabled (true);
		player -> property_video_sink () = vsink;
	}

	player -> property_volume ()     = volume;
	player -> signal_audio_changed () .connect (sigc::mem_fun (this, &MediaStream::on_audio_changed));
	player -> signal_video_changed () .connect (sigc::mem_fun (this, &MediaStream::on_video_changed));
}

void
MediaStream::setUri (const basic::uri & uri)
{
	const auto getTmpFileName = [ ] (const basic::uri & uri)
	{
		if (uri .is_local ())
			return uri;

		basic::ifilestream istream (uri, 30'000);
	
		if (not istream)
			return uri;

		istream .send ();

		if (not istream)
			return uri;

__LOG__ << uri << std::endl;
__LOG__ << uri .extension () << std::endl;

		std::string tmpFilename = "/tmp/titania-XXXXXX" + uri .extension ();

		::close (Glib::mkstemp (tmpFilename));

		std::ofstream ofstream (tmpFilename);

		ofstream << istream .rdbuf ();

		return basic::uri ("file://" + tmpFilename);
	};

	emitAudio    = true;
	emitVideo    = true;
	emitDuration = true;

	player -> set_state (Gst::STATE_NULL);

	if (thread)
		thread -> join ();

	thread = std::make_unique <std::thread> ([&] (const basic::uri & uri)
	{
		player -> property_volume () = volume = 0;
		player -> property_uri ()    = getTmpFileName (uri) .str ();

		player -> set_state (Gst::STATE_PAUSED);
	},
	uri);
}

basic::uri
MediaStream::getUri () const
{
	return basic::uri (player -> property_uri () .get_value ());
}

void
MediaStream::setVolume (double value)
{
	static constexpr double granularity = 0.05;

	value = std::round (std::clamp (value, 0.0, 1.0) / granularity) * granularity;

	if (value not_eq volume)
	{
		volume = value;
		player -> property_volume () = value;
	}
}

void
MediaStream::setSpeed (const time_type value)
{
	speed       = value;
	updateSpeed = false;

	if (speed > 0)
	{
		gint64 position = 0;

		if (not player -> query_position (Gst::FORMAT_TIME, position))
			position = 0;

		player -> seek (speed,
		                Gst::FORMAT_TIME,
		                Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
		                Gst::SEEK_TYPE_SET, position,
		                Gst::SEEK_TYPE_END, 0);
	}
	else if (speed < 0)
	{
		gint64 position = 0;

		if (not player -> query_position (Gst::FORMAT_TIME, position))
			position = 0;

		player -> seek (speed,
		                Gst::FORMAT_TIME,
		                Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
		                Gst::SEEK_TYPE_SET, 0,
		                Gst::SEEK_TYPE_SET, position);
	}
}

time_type
MediaStream::getPosition () const
{
	gint64 position = 0;

	if (not player -> query_position (Gst::FORMAT_TIME, position))
		position = 0;

	return position / (long double) Gst::SECOND;
}

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

time_type
MediaStream::getQueryDuration () const
{
	gint64 duration = 0;

	if ((video ? vsink -> query_duration (Gst::FORMAT_TIME, duration) : player -> query_duration (Gst::FORMAT_TIME, duration)) and duration >= 0)
		return duration / (long double) Gst::SECOND;

	return -1;
}

std::shared_ptr <VideoFrame>
MediaStream::getCurrentFrame ()
{
	std::lock_guard <std::mutex> lock (frameMutex);

	return std::move (currentFrame);
}

bool
MediaStream::sync () const
{
	return getState () not_eq Gst::STATE_NULL;
}

void
MediaStream::seek (const time_type position)
{
	if (speed > 0)
	{
		player -> seek (speed,
		                Gst::FORMAT_TIME,
		                Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
		                Gst::SEEK_TYPE_SET, position * (long double) Gst::SECOND,
		                Gst::SEEK_TYPE_END, 0);
	}
	else if (speed < 0)
	{
		player -> seek (speed,
		                Gst::FORMAT_TIME,
		                Gst::SEEK_FLAG_FLUSH | Gst::SEEK_FLAG_ACCURATE,
		                Gst::SEEK_TYPE_SET, 0,
		                Gst::SEEK_TYPE_SET, position * (long double) Gst::SECOND);
	}
}

void
MediaStream::play ()
{
	updateSpeed = true;
	player -> set_state (Gst::STATE_PLAYING);
}

void
MediaStream::pause ()
{
	player -> set_state (Gst::STATE_PAUSED);
}

void
MediaStream::stop ()
{
	player -> set_state (Gst::STATE_NULL);
}

void
MediaStream::on_bus_message_sync (const Glib::RefPtr <Gst::Message> & message)
{
	if (not video)
		return;

	if (not gst_is_video_overlay_prepare_window_handle_message (message -> gobj ()))
		return;

	const auto xWindow = getWindow ();

	if (not xWindow)
	{
		stop ();
		return;
	}

	XMoveResizeWindow (xDisplay, xWindow, 0, 0, vsink -> get_width (), vsink -> get_height ());
	XSync (xDisplay, FALSE);

	vsink -> set_window_handle (xWindow);
}

void
MediaStream::on_message (const Glib::RefPtr <Gst::Message> & message)
{
	switch (message -> get_message_type ())
	{
		case Gst::MESSAGE_EOS:
		{
			endDispatcher .emit ();
			break;
		}
		case Gst::MESSAGE_STATE_CHANGED:
		{
			const auto state = Glib::RefPtr <Gst::MessageStateChanged>::cast_static (message) -> parse_new_state ();

			if (state == Gst::STATE_PLAYING)
			{
				// XXX: Force set volume, as the volume is interally reseted to maximum sometimes.
				player -> property_volume () = volume;
			}

			break;
		}
		case Gst::MESSAGE_ASYNC_DONE:
		{
			if (updateSpeed)
				setSpeed (speed);

			if (emitDuration)
			{
				duration     = getQueryDuration ();
				emitDuration = duration == -1;
				durationChangedDispatcher .emit ();
			}

			break;
		}
		case Gst::MESSAGE_ERROR:
		{
			__LOG__
				<< "MESSAGE_ERROR: "
				<< Glib::RefPtr <Gst::MessageError>::cast_static (message) -> parse_debug ()
				<< std::endl;

			emitAudio = false;
			emitVideo = false;
			errorDispatcher .emit ();
			break;
		}
		default:
			break;
	}
}

void
MediaStream::on_audio_changed ()
{
	if (emitAudio)
	{
		emitAudio = false;
		audioChangedDispatcher .emit ();
	}
}

void
MediaStream::on_video_changed ()
{
	if (not video)
		return;

	Glib::RefPtr <Gst::Pad> pad = player -> get_video_pad (0);

	if (pad)
		pad -> add_probe (Gst::PAD_PROBE_TYPE_BUFFER, sigc::mem_fun (this, &MediaStream::on_video_pad_got_buffer));

	if (emitVideo)
	{
		emitVideo = false;
		videoChangedDispatcher .emit ();
	}
}

Gst::PadProbeReturn
MediaStream::on_video_pad_got_buffer (const Glib::RefPtr <Gst::Pad> & pad, const Gst::PadProbeInfo & data)
{
	if (not video)
		return Gst::PAD_PROBE_OK;

	// Resize window

	if (pad)
	{
		auto caps = pad -> get_current_caps ();
	
		if (caps)
		{
			caps = caps -> create_writable ();
	
			const auto structure = caps -> get_structure (0);
	
			if (structure)
			{
				int32_t w = 0;
				int32_t h = 0;
	
				structure .get_field ("width",  w);
				structure .get_field ("height", h);

				if (w not_eq width or h not_eq height)
				{
					const auto xWindow = getWindow ();

					if (xWindow)
					{
						width  = w;
						height = h;
		
						XMoveResizeWindow (xDisplay, xWindow, 0, 0, width, height);
						XSync (xDisplay, FALSE);
					}
					else
					{
						return Gst::PAD_PROBE_OK;
					}
				}
			}
		}
	}

	// This function is process in another thread!

	const auto width  = vsink -> get_width ();  // This width and height are the values for the buffer.
	const auto height = vsink -> get_height (); // This width and height are the values for the buffer.
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

			// Exchange current frame
			{
				std::lock_guard <std::mutex> lock (frameMutex);

				currentFrame = frame;
			}

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
	if (thread)
		thread -> join ();

	player -> set_state (Gst::STATE_NULL);

	if (xWindow)
		XDestroyWindow (xDisplay, xWindow);

	if (xDisplay)
		XCloseDisplay (xDisplay);
}

} // X3D
} // titania
