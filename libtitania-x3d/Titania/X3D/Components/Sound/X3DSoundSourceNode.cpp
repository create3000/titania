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

#include "X3DSoundSourceNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Types/MediaStream.h"

namespace titania {
namespace X3D {

X3DSoundSourceNode::Fields::Fields () :
	enabled (new SFBool (true)),
	description (new SFString ()),
	speed (new SFFloat (1)),
	pitch (new SFFloat (1)),
	isActive (new SFBool ()),
	duration_changed (new SFTime (-1))
{ }

X3DSoundSourceNode::X3DSoundSourceNode () :
	X3DTimeDependentNode (),
	              fields (),
	                 end (),
	         mediaStream (new MediaStream ())
{
	addNodeType (X3DConstants::X3DSoundSourceNode);
	
	addChildren (end);
}

void
X3DSoundSourceNode::initialize ()
{
	X3DTimeDependentNode::initialize ();

	speed () .addInterest (this, &X3DSoundSourceNode::set_speed);
	pitch () .addInterest (this, &X3DSoundSourceNode::set_pitch);
	end      .addInterest (this, &X3DSoundSourceNode::set_end);

	auto bus = mediaStream -> getPlayer () -> get_bus ();

	bus -> add_signal_watch ();
	bus -> signal_message () .connect (sigc::mem_fun (*this, &X3DSoundSourceNode::on_message));
}

void
X3DSoundSourceNode::setUri (const basic::uri & uri)
{
	mediaStream -> setUri (uri);
}

float
X3DSoundSourceNode::getDuration () const
{
	return mediaStream -> getDuration ();
}

void
X3DSoundSourceNode::setVolume (float value)
{
	mediaStream -> setVolume (value);
}

const Glib::RefPtr <Gst::XImageSink> &
X3DSoundSourceNode::getVideoSink () const
{
	return mediaStream -> getVideoSink ();
}

bool
X3DSoundSourceNode::sync () const
{
	return mediaStream -> sync ();
}

void
X3DSoundSourceNode::prepareEvents ()
{
	elapsedTime () = getElapsedTime ();
}

void
X3DSoundSourceNode::on_message (const Glib::RefPtr <Gst::Message> & message)
{
	switch (message -> get_message_type ())
	{
		case Gst::MESSAGE_EOS:
		{
			end = getCurrentTime ();
			break;
		}
		case Gst::MESSAGE_ERROR:
		{
			__LOG__
				<< "MESSAGE_ERROR: "
				<< Glib::RefPtr <Gst::MessageError>::cast_dynamic (message) -> parse () .what ()
				<< std::endl;

			stop ();
			break;
		}
		default:
			break;
	}
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
	if (speed ())
		mediaStream -> start (speed (), 0);
}

void
X3DSoundSourceNode::set_pause ()
{
	mediaStream -> pause ();
}

void
X3DSoundSourceNode::set_resume (time_type)
{
	if (speed ())
		mediaStream -> resume ();
}

void
X3DSoundSourceNode::set_stop ()
{
	mediaStream -> stop ();
}

void
X3DSoundSourceNode::set_end ()
{
	if (loop ())
	{
		if (speed ())
			mediaStream -> start (speed (), 0);

      // The event order below is very important.
 
		elapsedTime () = getElapsedTime ();
		cycleTime ()   = getCurrentTime ();
	}
	else
		stop ();
}

void
X3DSoundSourceNode::dispose ()
{
	mediaStream .reset ();

	X3DTimeDependentNode::dispose ();
}

} // X3D
} // titania
