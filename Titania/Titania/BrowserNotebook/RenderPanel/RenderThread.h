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

#ifndef __TITANIA_BROWSER_NOTEBOOK_RENDER_PANEL_RENDER_THREAD_H__
#define __TITANIA_BROWSER_NOTEBOOK_RENDER_PANEL_RENDER_THREAD_H__

#include <Titania/X3D/Thread/X3DInterruptibleThread.h>

#include <Titania/X3D.h>

#include <thread>

namespace titania {
namespace puck {

class RenderClock;
class VideoEncoder;

struct RenderThreadFrame
{
	size_t        frameNumber;
	Magick::Image image;
};

using RenderThreadFramePtr = std::shared_ptr <RenderThreadFrame>;

class RenderThread :
	protected X3D::X3DInterruptibleThread,
	public X3D::X3DInput,
	public sigc::trackable
{
public:

	///  @name Construction

	RenderThread (const basic::uri & url,
	              const basic::uri & filename,
	              const std::string & codec,
	              const size_t duration,
	              const size_t frameRate,
	              const size_t width,
	              const size_t height,
	              const size_t antialiasing,
	              const std::string & shading);

	///  @name Member access

	size_t
	getDuration () const
	{ return duration; }

	size_t
	getFrameRate () const
	{ return frameRate; }

	size_t
	getWidth () const
	{ return width; }

	size_t
	getHeight () const
	{ return height; }

	size_t
	getAntialiasing () const
	{ return antialiasing; }

	RenderThreadFramePtr
	getCurrentFrame ()
	{ return std::move (currentFrame); }

	std::string
	getStdout ();
	
	std::string
	getStderr ();

	///  @name Operations

	///  throws std::runtime_error
	void
	start ();

	bool
	stop ();

	///  @name Signals

	///  Signal load_count_changed.
	sigc::signal <void, size_t> &
	signal_load_count_changed ()
	{ return loadCountSignal; }

	///  Signal frame_changed.
	Glib::Dispatcher &
	signal_frame_changed ()
	{ return frameDispatcher; }

	///  Signal stdout.
	Glib::Dispatcher &
	signal_stdout ();

	///  Signal stdout.
	Glib::Dispatcher &
	signal_stderr ();

	///  @name Destruction

	virtual
	~RenderThread () final override;


private:

	///  @name Event handlers

	void
	set_initialized ();

	void
	set_loadCount ();

	void
	run ();

	bool
	on_timeout ();

	RenderThreadFramePtr
	getFrame (const int32_t frameNumber);

	///  @name Members

	X3D::BrowserPtr                browser;
	const std::atomic <size_t>     duration;
	const std::atomic <size_t>     frameRate;
	const std::atomic <size_t>     width;
	const std::atomic <size_t>     height;
	const std::atomic <size_t>     antialiasing;
	const std::string              shading;
	std::shared_ptr <RenderClock>  clock;
	RenderThreadFramePtr           currentFrame;
	size_t                         frameNumber; // Only used for timout version.
	sigc::signal <void, size_t>    loadCountSignal;
	Glib::Dispatcher               frameDispatcher;
	std::unique_ptr <VideoEncoder> videoEncoder;
	std::unique_ptr <std::thread>  thread;

};

} // puck
} // titania

#endif
