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

#include "X3DBrowserContext.h"

#include "../Browser/Console.h"
#include "../Browser/Notification.h"
#include "../Browser/PointingDeviceSensor/Selection.h"
#include "../Browser/Properties/BrowserOptions.h"
#include "../Browser/Properties/BrowserProperties.h"
#include "../Browser/Properties/RenderingProperties.h"
#include "../Rendering/Context.h"

#include <cassert>

namespace titania {
namespace X3D {

X3DBrowserContext::X3DBrowserContext () :
	                   X3DBaseNode (),
	           X3DExecutionContext (),
	                X3DCoreContext (),
	     X3DKeyDeviceSensorContext (),
	            X3DLayeringContext (),
	              X3DLayoutContext (),
	            X3DLightingContext (),
	          X3DNavigationContext (),
	          X3DNetworkingContext (),
	     X3DParticleSystemsContext (),
	X3DPointingDeviceSensorContext (),
	           X3DRenderingContext (),
	           X3DScriptingContext (),
	               X3DShapeContext (),
	                X3DTextContext (),
	           X3DTexturingContext (),
	                X3DTimeContext (),
	           renderingProperties (new RenderingProperties (this)),
	             browserProperties (new BrowserProperties   (this)),
	                browserOptions (new BrowserOptions      (this)),
	             initializedOutput (),
	                reshapedOutput (),
	           prepareEventsOutput (),
	                 sensorsOutput (),
	               displayedOutput (),
	                finishedOutput (),
	                 changedOutput (),
	                        router (),
	                   changedTime (0),
	                     selection (new Selection (this)),
	                  notification (new Notification (this)),
	                       console (new Console (this))
{
	initialized () .setName ("initialized");

	addChildren (initialized (),
	             renderingProperties,
	             browserProperties,
	             browserOptions,
	             selection,
	             notification,
	             console);
}

void
X3DBrowserContext::initialize ()
{
	X3DExecutionContext::initialize ();
	X3DCoreContext::initialize ();
	X3DKeyDeviceSensorContext::initialize ();
	X3DLayeringContext::initialize ();
	X3DLayoutContext::initialize ();
	X3DLightingContext::initialize ();
	X3DNavigationContext::initialize ();
	X3DNetworkingContext::initialize ();
	X3DParticleSystemsContext::initialize ();
	X3DPointingDeviceSensorContext::initialize ();
	X3DRenderingContext::initialize ();
	X3DScriptingContext::initialize ();
	X3DShapeContext::initialize ();
	X3DTextContext::initialize ();
	X3DTexturingContext::initialize ();
	X3DTimeContext::initialize ();

	if (glXGetCurrentContext ())
	{
		renderingProperties -> setup ();
		browserProperties   -> setup ();
		browserOptions      -> setup ();
		selection           -> setup ();
		notification        -> setup ();
		console             -> setup ();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
X3DBrowserContext::addEvent ()
{
	if (changedTime == getCurrentTime ())
		return;

	changedTime = getCurrentTime ();
	changed () .processInterests ();
}

void
X3DBrowserContext::reshape ()
{
	std::lock_guard <ContextMutex> contextLock (getContextMutex ());

	if (makeCurrent ())
		reshaped () .processInterests ();
}

/*
 * a) Update camera based on currently bound Viewpoint's position and orientation.
 * b) Evaluate input from sensors.
 * c) Evalute routes.
 * d) If any events were generated from steps b and c, go to step b and continue.
 * e) If particle system evaluation is to take place, evaluate the particle systems here.
 * f) If physics model evaluation is to take place, evaluate the physics model.
 */

void
X3DBrowserContext::update ()
{
	try
	{
		std::lock_guard <ContextMutex> contextLock (getContextMutex ());

		if (makeCurrent ())
		{
			// Prepare

			getClock () -> advance ();

			prepareEvents () .processInterests ();
			router .processEvents ();

			getWorld () -> traverse (TraverseType::CAMERA);
			getWorld () -> traverse (TraverseType::COLLISION);

			sensors () .processInterests ();
			router .processEvents ();

			getGarbageCollector () .dispose ();

			// Debug
			router .debug ();
			assert (router .empty ());

			// Display

			glClearColor (0, 0, 0, 0);
			glClear (GL_COLOR_BUFFER_BIT);

			getWorld () -> traverse (TraverseType::DISPLAY);

			displayed () .processInterests ();
			swapBuffers ();

			// Finish

			finished () .processInterests ();

			GLenum errorNum = glGetError ();

			if (errorNum not_eq GL_NO_ERROR)
				std::clog << "OpenGL Error at " << SFTime (getCurrentTime ()) .toUTCString () << ": " << gluErrorString (errorNum) << std::endl;
		}
	}
	catch (const std::exception & exception)
	{
		std::clog
			<< getCurrentTime () << " Execution Error:" << std::endl
			<< "  " << exception .what () << std::endl;
	}
}

void
X3DBrowserContext::dispose ()
{
	initializedOutput   .dispose ();
	reshapedOutput      .dispose ();
	prepareEventsOutput .dispose ();
	sensorsOutput       .dispose ();
	displayedOutput     .dispose ();
	finishedOutput      .dispose ();
	changedOutput       .dispose ();

	X3DTimeContext::dispose ();
	X3DTexturingContext::dispose ();
	X3DTextContext::dispose ();
	X3DShapeContext::dispose ();
	X3DScriptingContext::dispose ();
	X3DRenderingContext::dispose ();
	X3DPointingDeviceSensorContext::dispose ();
	X3DParticleSystemsContext::dispose ();
	X3DNetworkingContext::dispose ();
	X3DNavigationContext::dispose ();
	X3DLightingContext::dispose ();
	X3DLayoutContext::dispose ();
	X3DLayeringContext::dispose ();
	X3DKeyDeviceSensorContext::dispose ();
	X3DCoreContext::dispose ();
	X3DExecutionContext::dispose ();
}

X3DBrowserContext::~X3DBrowserContext ()
{
	__LOG__ << std::endl;
}

} // X3D
} // titania
