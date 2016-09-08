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

#include "X3DMediaStream.h"

#include <Titania/Basic/URI.h>

#include <gstreamermm.h>
#include <gstreamermm/message.h>
#include <gstreamermm/playbin.h>
#include <gstreamermm/ximagesink.h>
#include <glibmm/dispatcher.h>

extern "C"
{
#include <X11/Xlib.h>
}

namespace titania {
namespace X3D {

class MediaStream :
	X3DMediaStream
{
public:

	///  @name Construction

	MediaStream ();

	void
	setup ();

	///  @name Signals

	Glib::Dispatcher &
	signal_loaded ()
	{ return loaded; }

	const Glib::Dispatcher &
	signal_loaded () const
	{ return loaded; }

	Glib::Dispatcher &
	signal_buffer_changed ()
	{ return buffer_changed; }

	const Glib::Dispatcher &
	signal_buffer_changed () const
	{ return buffer_changed; }

	Glib::Dispatcher &
	signal_end ()
	{ return end; }

	const Glib::Dispatcher &
	signal_end () const
	{ return end; }

	Glib::Dispatcher &
	signal_duration_changed ()
	{ return duration_changed; }

	const Glib::Dispatcher &
	signal_duration_changed () const
	{ return duration_changed; }

	///  @name Member access

	bool
	setUri (const basic::uri & uri);

	void
	setVolume (double value);

	double
	getDuration () const;

	int32_t
	getWidth () const
	{ return vsink -> get_width (); }

	int32_t
	getHeight () const
	{ return vsink -> get_height (); }

	const std::vector <uint8_t> &
	getBuffer () const
	{ return image; }

	///  @name Operations

	bool
	sync () const;

	void
	start (const double speed, const double position);

	void
	pause ();

	void
	resume ();

	void
	stop ();

	///  @name Destruction

	~MediaStream ();


private:

	///  @name Operations

	Gst::State
	getState () const;

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
	update ();

	void
	flip (const size_t, const size_t);

	///  @name Member access

	Glib::Dispatcher loaded;
	Glib::Dispatcher buffer_changed;
	Glib::Dispatcher end;
	Glib::Dispatcher duration_changed;

	Glib::RefPtr <Player>    player;
	Glib::RefPtr <VideoSink> vsink;

	Display* display;
	Pixmap   pixmap;

	std::vector <uint8_t> image;
	double                volume;

};

} // X3D
} // titania

#endif
