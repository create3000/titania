/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "RenderThread.h"

#include "RenderClock.h"
#include "VideoEncoder.h"

#include <Titania/X3D/Browser/Core/BrowserOptions.h>
#include <Titania/X3D/Routing/Router.h>

#include <Titania/String/toupper.h>
#include <glibmm/main.h>
#include <regex>

namespace titania {
namespace puck {

RenderThread::RenderThread (const basic::uri & url,
                            const basic::uri & filename,
                            const std::string & codec,
                            const size_t duration,
                            const size_t frameRate,
                            const size_t width,
                            const size_t height,
                            const size_t antialiasing,
                            const std::string & shading) :
	X3D::X3DInterruptibleThread (),
	              X3D::X3DInput (),
	            sigc::trackable (),
	                    browser (X3D::createBrowser ({ url .str () })),
	                   duration (duration),
	                  frameRate (frameRate),
	                      width (width),
	                     height (height),
	               antialiasing (antialiasing),
	                    shading (shading),
	                      clock (std::make_shared <RenderClock> (X3D::SFTime::now (), 1 / X3D::time_type (frameRate))),
	               currentFrame (),
	                frameNumber (0),
	            loadCountSignal (),
	            frameDispatcher (),
	               videoEncoder (std::make_unique <VideoEncoder> (filename, codec, frameRate, duration, width, height)),
	                     thread ()
{
	browser -> initialized () .addInterest (&RenderThread::set_initialized, this);
	browser -> setName ("Renderer");
	browser -> setClock (clock);
	browser -> setMute (true);
}

std::string
RenderThread::getStdout ()
{
	return videoEncoder -> getStdout ();
}

std::string
RenderThread::getStderr ()
{
	return videoEncoder -> getStderr ();
}

///  throws std::runtime_error
void
RenderThread::start ()
{
	videoEncoder -> open ();

	browser -> setup ();
}

bool
RenderThread::stop ()
{
	X3D::X3DInterruptibleThread::stop ();

	if (thread and thread -> joinable ())
		thread -> join ();

	return videoEncoder -> close ();
}

void
RenderThread::set_initialized ()
{
	try
	{
		checkForInterrupt ();

		browser -> initialized () .removeInterest (&RenderThread::set_initialized, this);
		browser -> getLoadCount () .addInterest (&RenderThread::set_loadCount, this);

		set_loadCount ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (const X3D::InterruptThreadException & error)
	{ }
}

void
RenderThread::set_loadCount ()
{
	try
	{
		checkForInterrupt ();

		loadCountSignal .emit (browser -> getLoadCount ());

		if (browser -> getLoadCount ())
			return;

		browser -> getLoadCount () .removeInterest (&RenderThread::set_loadCount, this);
		browser -> setProcessRenderEvents (false);
		browser -> getBrowserOptions () -> Shading () = basic::toupper (shading, std::locale::classic ());

		Glib::signal_timeout () .connect (sigc::mem_fun (this, &RenderThread::on_timeout), 10, Glib::PRIORITY_LOW);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (const X3D::InterruptThreadException & error)
	{ }
}

bool
RenderThread::on_timeout ()
{
	try
	{
		checkForInterrupt ();

		currentFrame = getFrame (frameNumber);

		frameDispatcher .emit ();

		++ frameNumber;

		return frameNumber < duration;
	}
	catch (const X3D::InterruptThreadException & error)
	{
		return false;
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
		frameDispatcher .emit ();
		return false;
	}
}

RenderThreadFramePtr
RenderThread::getFrame (const int32_t frameNumber)
{
	auto frame = std::make_shared <RenderThreadFrame> ();

	frame -> frameNumber = frameNumber;
	frame -> image       = browser -> getSnapshot (width, height, false, antialiasing);

	// Make PNG always RGB, otherwise the image is sometimes GRAY if ImageMagick desides that this is possible.
	frame -> image .defineValue ("PNG", "color-type", "2");

	videoEncoder -> write (frame -> image);

	clock -> real_advance ();

	return frame;
}

Glib::Dispatcher &
RenderThread::signal_stdout ()
{
	return videoEncoder -> signal_stdout ();
}

Glib::Dispatcher &
RenderThread::signal_stderr ()
{
	return videoEncoder -> signal_stderr ();
}

RenderThread::~RenderThread ()
{
	stop ();

	browser .dispose ();

	dispose ();
}

} // puck
} // titania
