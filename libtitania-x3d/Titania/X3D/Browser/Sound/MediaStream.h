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

#ifndef __TITANIA_X3D_BROWSER_SOUND_MEDIA_STREAM_H__
#define __TITANIA_X3D_BROWSER_SOUND_MEDIA_STREAM_H__

#include "../../Types/Time.h"

#include <Titania/Basic/URI.h>

#include <gstreamermm.h>
#include <gstreamermm/message.h>
#include <gstreamermm/playbin.h>
#include <gstreamermm/ximagesink.h>
#include <glibmm/dispatcher.h>
#include <gdkmm/display.h>

extern "C"
{
#include <X11/Xlib.h>
}

#include <atomic>
#include <mutex>

namespace Gst {

class PlayBin;
class XImageSink;
class Message;

}

namespace titania {
namespace X3D {

struct VideoFrame
{
	int32_t               width;
	int32_t               height;
	std::vector <uint8_t> image;
};

class MediaStream
{
public:

	///  @name Member types

	using Player    = Gst::PlayBin;
	using VideoSink = Gst::XImageSink;

	///  @name Construction

	MediaStream ();

	void
	setup ();

	///  @name Member access

	void
	setVideo (const bool value)
	{ video = value; }

	bool
	getVideo () const
	{ return video; }

	bool
	setUri (const basic::uri & uri);

	basic::uri
	getUri () const;

	void
	setVolume (double value);

	void
	setSpeed (const time_type value);

	time_type
	getPosition () const;

	time_type
	getDuration () const
	{ return duration; }

	std::shared_ptr <VideoFrame>
	getCurrentFrame ();

	int32_t
	getWidth () const
	{ return vsink -> get_width (); }

	int32_t
	getHeight () const
	{ return vsink -> get_height (); }

	///  @name Operations

	bool
	sync () const;

	void
	seek (const time_type position);

	void
	play ();

	void
	pause ();

	void
	stop ();

	///  @name Signals

	Glib::Dispatcher &
	signal_audio_changed ()
	{ return audioChangedDispatcher; }

	Glib::Dispatcher &
	signal_video_changed ()
	{ return videoChangedDispatcher; }

	Glib::Dispatcher &
	signal_error ()
	{ return errorDispatcher; }

	Glib::Dispatcher &
	signal_buffer_changed ()
	{ return bufferChangedDispatcher; }

	Glib::Dispatcher &
	signal_end ()
	{ return endDispatcher; }

	Glib::Dispatcher &
	signal_duration_changed ()
	{ return durationChangedDispatcher; }

	///  @name Destruction

	~MediaStream ();


private:

	///  @name Construction

	Window
	getWindow () const;

	Window
	createWindow (Display* const xDisplay, const int32_t width, const int32_t height) const;

	///  @name Operations

	Gst::State
	getState () const;

	time_type
	getQueryDuration () const;

	///  @name Event handlers

	void
	on_bus_message_sync (const Glib::RefPtr <Gst::Message> & message);

	void
	on_message (const Glib::RefPtr <Gst::Message> &);

	void
	on_audio_changed ();

	void
	on_video_changed ();

	Gst::PadProbeReturn
	on_video_pad_got_buffer (const Glib::RefPtr <Gst::Pad> &, const Gst::PadProbeInfo &);

	void
	flip (std::vector <uint8_t> & image, const int32_t width, const int32_t height);

	///  @name Member access

	std::atomic <bool> video;

	Display* xDisplay;
	Window   xWindow;

	Glib::RefPtr <Player>    player;
	Glib::RefPtr <VideoSink> vsink;

	std::atomic <int32_t>        width;
	std::atomic <int32_t>        height;
	std::shared_ptr <VideoFrame> currentFrame;
	std::mutex                   frameMutex;

	bool      emitAudio;
	bool      emitVideo;
	double    volume;
	time_type speed;
	bool      updateSpeed;
	time_type duration;
	bool      emitDuration;

	Glib::Dispatcher audioChangedDispatcher;
	Glib::Dispatcher videoChangedDispatcher;
	Glib::Dispatcher errorDispatcher;
	Glib::Dispatcher bufferChangedDispatcher;
	Glib::Dispatcher endDispatcher;
	Glib::Dispatcher durationChangedDispatcher;


};

} // X3D
} // titania

#endif
