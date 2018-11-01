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

#include "X3DSoundSourceNode.h"

#include "../../Browser/Sound/MediaStream.h"

#include "../../Browser/X3DBrowser.h"

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

X3DSoundSourceNode::X3DSoundSourceNode (const bool video) :
	X3DTimeDependentNode (),
	              fields (),
	                 end (),
	         mediaStream (),
	               video (video)
{
	addType (X3DConstants::X3DSoundSourceNode);

	addChildObjects (end);
}

void
X3DSoundSourceNode::initialize ()
{
	X3DTimeDependentNode::initialize ();

	getBrowser () -> addSoundSource (this);

	mediaStream .reset (new MediaStream (video));

	speed () .addInterest (&X3DSoundSourceNode::set_speed, this);
	pitch () .addInterest (&X3DSoundSourceNode::set_pitch, this);
	end      .addInterest (&X3DSoundSourceNode::set_end, this);

	mediaStream -> signal_end ()              .connect (sigc::mem_fun (this, &X3DSoundSourceNode::on_end));
	mediaStream -> signal_duration_changed () .connect (sigc::mem_fun (this, &X3DSoundSourceNode::on_duration_changed));
	mediaStream -> setup ();

	set_speed ();
}

void
X3DSoundSourceNode::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> removeSoundSource (this);

	X3DTimeDependentNode::setExecutionContext (executionContext);

	getBrowser () -> addSoundSource (this);
}

void
X3DSoundSourceNode::setVolume (const float value)
{
	mediaStream -> setVolume (clamp <float> (value * getBrowser () -> getVolume () * (not getBrowser () -> getMute ()), 0, 1));
}

const std::unique_ptr <MediaStream> &
X3DSoundSourceNode::getStream () const
{
	return mediaStream;
}

void
X3DSoundSourceNode::on_end ()
{
	end = getCurrentTime ();
}

void
X3DSoundSourceNode::set_speed ()
{
	mediaStream -> setSpeed (speed ());

	if (speed () == 0.0f)
	{
		mediaStream -> pause ();
	}
	else
	{
		if (isActive () and not isPaused ())
			mediaStream -> play ();
	}
}

void
X3DSoundSourceNode::set_pitch ()
{ }

void
X3DSoundSourceNode::set_start ()
{
	if (speed () == 0.0f)
	{
		mediaStream -> pause ();
	}
	else
	{
		mediaStream -> stop ();
		mediaStream -> play ();
	}
}

void
X3DSoundSourceNode::set_resume (const time_type)
{
	if (speed () == 0.0f)
		return;

	mediaStream -> play ();
}

void
X3DSoundSourceNode::set_pause ()
{
	mediaStream -> pause ();
}

void
X3DSoundSourceNode::set_stop ()
{
	mediaStream -> pause ();
}

void
X3DSoundSourceNode::set_end ()
{
	if (loop ())
	{
		if (speed () > 0)
		{
			mediaStream -> stop ();
			mediaStream -> play ();
		}
		else if (speed () < 0)
		{
			mediaStream -> seek (duration_changed ());
			mediaStream -> play ();
		}

		// The event order below is very important.

		elapsedTime () = getElapsedTime ();
		cycleTime ()   = getCurrentTime ();
	}
	else
	{
		stop ();
	}
}

void
X3DSoundSourceNode::on_duration_changed ()
{
	duration_changed () = mediaStream -> getDuration ();
}

void
X3DSoundSourceNode::set_time ()
{
	elapsedTime () = getElapsedTime ();
}

void
X3DSoundSourceNode::dispose ()
{
	getBrowser () -> removeSoundSource (this);

	mediaStream .reset ();

	X3DTimeDependentNode::dispose ();
}

X3DSoundSourceNode::~X3DSoundSourceNode ()
{ }

} // X3D
} // titania
