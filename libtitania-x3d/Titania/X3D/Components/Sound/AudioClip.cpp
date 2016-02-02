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

#include "AudioClip.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType AudioClip::component      = ComponentType::SOUND;
const std::string   AudioClip::typeName       = "AudioClip";
const std::string   AudioClip::containerField = "source";

AudioClip::AudioClip (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DSoundSourceNode (),
	      X3DUrlObject ()
{
	addType (X3DConstants::AudioClip);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "enabled",          enabled ());             // non standard
	addField (inputOutput, "description",      description ());
	addField (inputOutput, "url",              url ());
	addField (inputOutput, "speed",            speed ());               // non standard
	addField (inputOutput, "pitch",            pitch ());
	addField (inputOutput, "loop",             loop ());
	addField (inputOutput, "startTime",        startTime ());
	addField (inputOutput, "resumeTime",       resumeTime ());
	addField (inputOutput, "pauseTime",        pauseTime ());
	addField (inputOutput, "stopTime",         stopTime ());
	addField (outputOnly,  "isPaused",         isPaused ());
	addField (outputOnly,  "isActive",         isActive ());
	addField (outputOnly,  "cycleTime",        cycleTime ());           // non standard
	addField (outputOnly,  "elapsedTime",      elapsedTime ());
	addField (outputOnly,  "duration_changed", duration_changed ());
}

X3DBaseNode*
AudioClip::create (X3DExecutionContext* const executionContext) const
{
	return new AudioClip (executionContext);
}

void
AudioClip::initialize ()
{
	X3DSoundSourceNode::initialize ();
	X3DUrlObject::initialize ();

	url () .addInterest (this, &AudioClip::set_url);

	requestImmediateLoad ();
}

void
AudioClip::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DUrlObject::setExecutionContext (executionContext);
	X3DSoundSourceNode::setExecutionContext (executionContext);
}

void
AudioClip::requestImmediateLoad ()
{
	if (not glXGetCurrentContext ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	if (url () .empty ())
	{
		setLoadState (FAILED_STATE);
		return;
	}

	for (const auto & URL : url ())
	{
		setUri (getExecutionContext () -> getWorldURL () .transform (URL .str ()));

		// Sync stream

		if (not sync ())
			continue;

		// Get audio

		duration_changed () = getDuration ();

		setLoadState (COMPLETE_STATE);

		break;
	}

	set_loop ();

	if (checkLoadState () not_eq COMPLETE_STATE)
	{
		duration_changed () = -1;
		setLoadState (FAILED_STATE);
	}
}

void
AudioClip::set_url ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();

	if (isActive () and not isPaused ())
	{
		if (loop ())
		{
			do_stop ();
			do_start ();
		}
		else
			do_stop ();
	}
}

void
AudioClip::dispose ()
{
	X3DUrlObject::dispose ();
	X3DSoundSourceNode::dispose ();
}

} // X3D
} // titania
