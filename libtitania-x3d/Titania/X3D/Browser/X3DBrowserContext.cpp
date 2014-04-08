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

#include "X3DBrowserContext.h"

#include "../Context.h"
#include "../Browser/Console.h"
#include "../Browser/Notification.h"
#include "../Browser/Picking/Selection.h"
#include "../Browser/Properties/BrowserOptions.h"
#include "../Browser/Properties/BrowserProperties.h"
#include "../Browser/Properties/RenderingProperties.h"
#include "../JavaScript/SpiderMonkey.h"
#include "../Rendering/ViewVolume.h"

#include <Titania/Chrono/CountingClock.h>
#include <Titania/Chrono/SystemClock.h>

#include <cassert>

namespace titania {
namespace X3D {

static constexpr int32_t MAX_DOWNLOAD_THREADS = 4;

X3DBrowserContext::X3DBrowserContext () :
	                X3DBaseNode (),
	        X3DExecutionContext (),
	          X3DPickingContext (),
	       X3DNavigationContext (),
	        renderingProperties (new RenderingProperties (this)),
	          browserProperties (new BrowserProperties   (this)),
	             browserOptions (new BrowserOptions      (this)),
	           javaScriptEngine (new SpiderMonkey        (this)),
	          initializedOutput (),
	             reshapedOutput (),
	              sensorsOutput (),
	        prepareEventsOutput (),
	            displayedOutput (),
	             finishedOutput (),
	              changedOutput (),
	                      clock (new chrono::system_clock <time_type> ()),
	                     router (),
	                   viewport (),
	                     layers (),
	                    layouts (),
	                     lights (),
	                 clipPlanes (),
	               textureUnits (),
	       combinedTextureUnits (),
	              textureStages (),
	                    texture (false),
	        keyDeviceSensorNode (nullptr),
	                changedTime (clock -> cycle ()),
	               currentSpeed (0),
	           currentFrameRate (0),
	         downloadMutexIndex (0),
	            downloadMutexes (1),
	              downloadMutex (),
	                  selection (new Selection (this)),
	               notification (new Notification (this)),
	                    console (new Console (this))
{
	initialized () .setName ("initialized");

	addChildren (initialized (),
	             renderingProperties,
	             browserProperties,
	             browserOptions,
	             javaScriptEngine,
	             keyDeviceSensorNodeOutput,
	             selection,
	             notification,
	             console);
}

void
X3DBrowserContext::initialize ()
{
	X3DExecutionContext::initialize ();
	X3DPickingContext::initialize ();
	X3DNavigationContext::initialize ();

	// Initialize clock

	advanceClock ();

	// Initialize OpenGL context

	if (glXGetCurrentContext ())
	{
		glClearColor (0, 0, 0, 0);
		glClearDepth (1);

		glCullFace (GL_BACK);
		glEnable (GL_NORMALIZE);

		glDepthFunc (GL_LEQUAL);

		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquationSeparate (GL_FUNC_ADD, GL_FUNC_ADD);

		GLfloat light_model_ambient [ ] = { 0, 0, 0, 1 };

		glLightModelfv (GL_LIGHT_MODEL_AMBIENT,       light_model_ambient);
		glLightModeli  (GL_LIGHT_MODEL_LOCAL_VIEWER,  GL_FALSE);
		glLightModeli  (GL_LIGHT_MODEL_TWO_SIDE,      GL_TRUE);
		glLightModeli  (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

		glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);

		glHint (GL_FOG_HINT, GL_NICEST);

		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

		// Properties

		renderingProperties -> setup ();
		browserProperties   -> setup ();
		browserOptions      -> setup ();
		javaScriptEngine    -> setup ();
		selection           -> setup ();
		notification        -> setup ();
		console             -> setup ();

		// Lights

		for (int32_t i = renderingProperties -> maxLights () - 1; i >= 0; -- i)
			lights .push (GL_LIGHT0 + i);

		// ClipPlanes

		for (int32_t i = renderingProperties -> maxClipPlanes () - 1; i >= 0; -- i)
			clipPlanes .push (GL_CLIP_PLANE0 + i);

		// TextureUnits

		for (int32_t i = renderingProperties -> textureUnits () - 1; i >= 0; -- i)
			textureUnits .push (i);                                                                                                                                                                                                                       // Don't add GL_TEXTURE0

		for (int32_t i = renderingProperties -> textureUnits (); i < renderingProperties -> combinedTextureUnits (); ++ i)
			combinedTextureUnits .push (i);                                                                                                                                                                                                               // Don't add GL_TEXTURE0

		downloadMutexes .resize (std::min <int32_t> (renderingProperties -> maxThreads () * 2, MAX_DOWNLOAD_THREADS));
	}
}

void
X3DBrowserContext::advanceClock ()
{
	clock -> advance ();

	if (getActiveLayer ())
		currentSpeed .setPosition (getActiveLayer () -> getViewpoint () -> getTransformationMatrix () .origin (), currentFrameRate);

	else
		currentSpeed .setPosition (Vector3f (), 0);

	currentFrameRate = 1 / clock -> interval ();
}

time_type
X3DBrowserContext::getCurrentTime () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return clock -> cycle ();
}

double
X3DBrowserContext::getCurrentSpeed () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return currentSpeed;
}

double
X3DBrowserContext::getCurrentFrameRate () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return currentFrameRate;
}

std::mutex &
X3DBrowserContext::getDownloadMutex ()
{
	std::lock_guard <std::mutex> lock (downloadMutex);

	downloadMutexIndex = (downloadMutexIndex + 1) % downloadMutexes .size ();

	return downloadMutexes [downloadMutexIndex];
}

void
X3DBrowserContext::lock ()
{
	for (auto & downloadMutex : downloadMutexes)
		downloadMutex .lock ();
}

void
X3DBrowserContext::unlock ()
{
	for (auto & downloadMutex : downloadMutexes)
		downloadMutex .unlock ();
}

// Key device

void
X3DBrowserContext::setKeyDeviceSensorNode (X3DKeyDeviceSensorNode* const value)
{
	if (keyDeviceSensorNode)
		keyDeviceSensorNode -> shutdown () .removeInterest (this, &X3DBrowserContext::setKeyDeviceSensorNode);

	keyDeviceSensorNode         = value;
	keyDeviceSensorNodeEvent () = getCurrentTime ();

	if (keyDeviceSensorNode)
		keyDeviceSensorNode -> shutdown () .addInterest (this, &X3DBrowserContext::setKeyDeviceSensorNode, nullptr);
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
	std::lock_guard <ContextMutex> contextLock (contextMutex);

	if (makeCurrent ())
	{
		viewport = Viewport4i ();

		reshaped () .processInterests ();
	}
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
		std::lock_guard <ContextMutex> contextLock (contextMutex);

		if (makeCurrent ())
		{
			// Prepare

			advanceClock ();

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
	displayedOutput     .dispose ();
	finishedOutput      .dispose ();
	changedOutput       .dispose ();

	X3DNavigationContext::dispose ();
	X3DPickingContext::dispose ();
	X3DExecutionContext::dispose ();
}

X3DBrowserContext::~X3DBrowserContext ()
{
	__LOG__ << std::endl;
}

} // X3D
} // titania
