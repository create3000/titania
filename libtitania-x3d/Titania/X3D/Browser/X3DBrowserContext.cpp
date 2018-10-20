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

#include "X3DBrowserContext.h"

#include "../Browser/Console.h"
#include "../Browser/Notification.h"
#include "../Browser/Selection.h"
#include "../Rendering/FrameBuffer.h"

#include "../Routing/Router.h"

#include <thread>

namespace titania {
namespace X3D {

X3DBrowserContext::X3DBrowserContext (const X3DBrowserContextPtr & other) :
	           X3DRenderingSurface (other),
	                      X3DScene (),
	                X3DCoreContext (),
	             X3DShadersContext (),
	          X3DGeometry2DContext (),
	          X3DGeometry3DContext (),
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
	               X3DSoundContext (),
	                X3DTextContext (),
	           X3DTexturingContext (),
	    X3DCubeMapTexturingContext (),
	X3DEnvironmentalEffectsContext (),
	                X3DTimeContext (),
	                X3DToolContext (),
	             initializedOutput (),
	           prepareEventsOutput (),
	              timeEventsOutput (),
	            sensorEventsOutput (),
	               displayedOutput (),
	                finishedOutput (),
	                 changedOutput (),
	                   currentTime (0),
	                   changedTime (0),
	                   freezedTime (0),
	                        router (new Router ()),
	                 sharedContext (other),
	              dependentContext (),
	                         world (new World (getExecutionContext ())),
	                 headUpDisplay (new World (getExecutionContext ())),
	                     selection (new Selection (this)),
	                  notification (new Notification (this)),
	                       console (new Console (this))
{
	initialized () .setName ("initialized");

	addType (X3DConstants::X3DBrowserContext);

	addChildObjects (initialized (),
	                 sharedContext,
	                 dependentContext,
	                 world,
	                 headUpDisplay,
	                 selection,
	                 notification,
	                 console);

	add_events (Gdk::BUTTON_PRESS_MASK |
	            Gdk::POINTER_MOTION_MASK |
	            Gdk::POINTER_MOTION_HINT_MASK |
	            Gdk::BUTTON_RELEASE_MASK |
	            Gdk::FOCUS_CHANGE_MASK |
	            Gdk::LEAVE_NOTIFY_MASK |
	            Gdk::SCROLL_MASK |
	            Gdk::KEY_PRESS_MASK |
	            Gdk::KEY_RELEASE_MASK);

	set_focus_on_click (true);
	set_can_focus (true);
}

void
X3DBrowserContext::initialize ()
{
	X3DScene::initialize ();
	X3DCoreContext::initialize ();
	X3DShadersContext::initialize ();
	X3DGeometry2DContext::initialize ();
	X3DGeometry3DContext::initialize ();
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
	X3DSoundContext::initialize ();
	X3DTextContext::initialize ();
	X3DTexturingContext::initialize ();
	X3DCubeMapTexturingContext::initialize ();
	X3DEnvironmentalEffectsContext::initialize ();
	X3DTimeContext::initialize ();
	X3DToolContext::initialize ();

	getHeadUpDisplay () -> order () .clear ();

	headUpDisplay -> setup ();
	selection     -> setup ();
	notification  -> setup ();
	console       -> setup ();

	queue_render ();
}

const X3DPtr <LayerSet> &
X3DBrowserContext::getHeadUpDisplay () const
{
	return headUpDisplay -> getLayerSet ();
}

void
X3DBrowserContext::setSelection (const SelectionPtr & value)
{
	selection = value;
}

void
X3DBrowserContext::setDependentContext (const X3DBrowserContextPtr & value)
{
	if (dependentContext)
		dependentContext -> changed () .removeInterest (&X3DBrowserContext::addEvent, this);

	dependentContext = value;

	router -> processEvents ();

	if (dependentContext)
	{
		router = dependentContext -> getRouter ();

		if (get_mapped ())
			dependentContext -> changed () .addInterest (&X3DBrowserContext::addEvent, this);
	}
	else
	{
		router .reset (new Router ());
	}
}

void
X3DBrowserContext::on_map ()
{
	X3DRenderingSurface::on_map ();

	if (dependentContext)
		dependentContext -> changed () .addInterest (&X3DBrowserContext::addEvent, this);

	queue_render ();
}

void
X3DBrowserContext::on_unmap ()
{
	X3DRenderingSurface::on_unmap ();

	if (dependentContext)
		dependentContext -> changed () .removeInterest (&X3DBrowserContext::addEvent, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***
 *  Returns a snapshot of the current browser surface in the form of a Magick::Image.  Query for getMaxRenderBufferSize
 *  to get the maximum width and height of the surface.  Query for getMaxSamples to get the maximum number of samples to
 *  use for antialising.  Antialiasing is only available if the browser supports GL_EXT_framebuffer_multisample.
 *
 *  @param  width         Width of the image.
 *  @param  height        Height of the image.
 *  @param  alphaChannel  Whether or not the image should have a alpha channel.
 *  @param  antialiasing  Number of samples used for antialising.
 */
Magick::Image
X3DBrowserContext::getSnapshot (const size_t width, const size_t height, const bool alphaChannel, const size_t antialiasing)
throw (Error <INSUFFICIENT_CAPABILITIES>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	// Make snapshot.

	ContextLock lock (this);

	if (getWorld ())
	{
		const auto viewport = getViewport ();
	
		// Render to frame buffer.
	
		FrameBuffer frameBuffer (this, width, height, std::min <size_t> (antialiasing, getMaxSamples ()));
	
		frameBuffer .setup ();
		frameBuffer .bind ();
	
		getAlphaChannel () .push (alphaChannel);
		getDisplayTools () .push (false);
	
		reshape (Vector4i (0, 0, width, height));
		update ();
		reshape (Vector4i (viewport [0], viewport [1], viewport [2], viewport [3]));
	
		frameBuffer .readPixels ();
		frameBuffer .unbind ();
	
		getDisplayTools () .pop ();
		getAlphaChannel () .pop ();
	
		// Process image.
	
		Magick::Image image (width, height, "RGBA", Magick::CharPixel, frameBuffer .getPixels () .data ());
	
		if (alphaChannel)
			image .type (Magick::TrueColorMatteType);
		else
		{
			image .matte (false);
			image .type (Magick::TrueColorType);
		}
	
		image .flip ();
		image .resolutionUnits (Magick::PixelsPerInchResolution);
		image .density (Magick::Geometry (72, 72));
	
		return image;
	}

	throw Error <DISPOSED> ("X3DBrowserContext::getSnapshot: Disposed.");
}

void
X3DBrowserContext::addEvent ()
{
	if (changedTime == currentTime)
		return;

	changedTime = currentTime;
	changed () .processInterests ();

	queue_render ();
}

void
X3DBrowserContext::beginUpdateForFrame ()
{
	changedTime = freezedTime;
}

void
X3DBrowserContext::endUpdateForFrame ()
{
	freezedTime = changedTime;
	changedTime = currentTime;
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
		// Prepare

		currentTime = SFTime::now ();
		getClock () -> advance ();

		prepareEvents () .processInterests ();
		getRouter () -> processEvents ();

		timeEvents () .processInterests ();
		getRouter () -> processEvents ();

		getWorld () -> traverse (TraverseType::CAMERA, nullptr);

		if (not getActiveCollisions () .empty ())
			getWorld () -> traverse (TraverseType::COLLISION, nullptr);

		sensorEvents () .processInterests ();
		getRouter () -> processEvents ();

		// Garbage Collection

		deleteObjectsAsync ();

		// Debug

		getRouter () -> debug ();

		// Clear surface.

		glViewport (getViewport () [0], getViewport () [1], getViewport () [2], getViewport () [3]);
		glScissor  (getViewport () [0], getViewport () [1], getViewport () [2], getViewport () [3]);
	
		glClearColor (0, 0, 0, 0);
		glClear (GL_COLOR_BUFFER_BIT);

		// Display scene.

		getWorld () -> traverse (TraverseType::DISPLAY, nullptr);

		if (getDisplayTools () .top ())
			getHeadUpDisplay () -> traverse (TraverseType::DISPLAY, nullptr);

		displayed () .processInterests ();
		finished ()  .processInterests ();

		#ifdef TITANIA_DEBUG
		const auto errorNum = glGetError ();

		if (errorNum not_eq GL_NO_ERROR)
		{
			std::clog
			   << getName () << " "
				<< "OpenGL Error at " << SFTime (SFTime::now ()) .toUTCString () << ": "
				<< gluErrorString (errorNum)
				<< " in " << getWorldURL () << std::endl;
		}
		#endif
	}
	catch (const Glib::Error & error)
	{
		__LOG__
		   << getName () << " "
			<< SFTime (SFTime::now ()) .toUTCString () << " Unhandled exception:" << std::endl
			<< "  " << error .what () << std::endl;
	}
	catch (const std::exception & error)
	{
		__LOG__
		   << getName () << " "
			<< SFTime (SFTime::now ()) .toUTCString () << " Unhandled exception:" << std::endl
			<< "  " << error .what () << std::endl;
	}
	catch (...)
	{
		__LOG__
		   << getName () << " "
			<< SFTime (SFTime::now ()) .toUTCString () << " Unhandled unkowm exception." << std::endl;
	}
}

void
X3DBrowserContext::on_setup ()
{
	X3DRenderingSurface::on_setup ();

	if (not isInitialized ())
		setup ();
}

void
X3DBrowserContext::on_reshape (const int32_t x, const int32_t y, const int32_t width, const int32_t height)
{
	X3DRenderingSurface::on_reshape (x, y, width, height);

	reshape (Vector4i (x, y, width, height));
}

bool
X3DBrowserContext::on_render ()
{
	X3DRenderingSurface::on_render ();

	update ();

	return false;
}

void
X3DBrowserContext::dispose ()
{
	initializedOutput   .dispose ();
	prepareEventsOutput .dispose ();
	timeEventsOutput    .dispose ();
	sensorEventsOutput  .dispose ();
	displayedOutput     .dispose ();
	finishedOutput      .dispose ();
	changedOutput       .dispose ();

	X3DToolContext::dispose ();
	X3DTimeContext::dispose ();
	X3DEnvironmentalEffectsContext::dispose ();
	X3DCubeMapTexturingContext::dispose ();
	X3DTexturingContext::dispose ();
	X3DTextContext::dispose ();
	X3DSoundContext::dispose ();
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
	X3DGeometry3DContext::dispose ();
	X3DGeometry2DContext::dispose ();
	X3DShadersContext::dispose ();
	X3DCoreContext::dispose ();
	X3DScene::dispose ();
	X3DRenderingSurface::dispose ();
}

X3DBrowserContext::~X3DBrowserContext ()
{
	__LOG__ << std::endl;
}

} // X3D
} // titania
