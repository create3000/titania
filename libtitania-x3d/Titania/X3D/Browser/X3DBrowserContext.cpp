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

#include "../Components/Networking/Anchor.h"
#include "../Components/PointingDeviceSensor/X3DDragSensorNode.h"
#include "../Components/PointingDeviceSensor/X3DTouchSensorNode.h"
#include "../JavaScript/SpiderMonkey.h"
#include "../Rendering/ViewVolume.h"

#include <Titania/Chrono/CountingClock.h>
#include <Titania/Chrono/SystemClock.h>

namespace titania {
namespace X3D {

static constexpr int32_t MAX_DOWNLOAD_THREADS = 8;

X3DBrowserContext::X3DBrowserContext () :
	 X3DExecutionContext (),
	 renderingProperties (new RenderingProperties (this)),          // SFNode  [ ]       renderingProperties NULL   [RenderingProperties]
	   browserProperties (new BrowserProperties   (this)),          // SFNode  [ ]       browserProperties   NULL   [BrowserProperties]
	      browserOptions (new BrowserOptions      (this)),          // SFNode  [ ]       browserOptions      NULL   [BrowserOptions]
	    javaScriptEngine (new SpiderMonkey        (this)),          // SFNode  [ ]       javaScriptEngine    NULL   [JavaScriptEngine]
	   initializedOutput (),                                        // SFTime [out]  initialized
	      reshapedOutput (),                                        // [out]  reshaped
	       sensorsOutput (),                                        // [out]  sensors
	 prepareEventsOutput (),                                        // [out]  prepareEvents
	     displayedOutput (),                                        // [out]  displayed
	      finishedOutput (),                                        // [out]  finished
	       changedOutput (),                                        // [out]  changed
	               clock (new chrono::system_clock <time_type> ()),
	              router (),
	              layers (),
	             layouts (),
	              lights (),
	        textureUnits (),
	         activeLayer (),
	activeNavigationInfo (nullptr),
	activeNavigationInfoChanged (),
	              viewer (ViewerType::NONE),
	       examineViewer (),
	          walkViewer (),
	           flyViewer (),
	          noneViewer (),
	              lookAt (),
 activeViewpointChanged (),
	 keyDeviceSensorNode (nullptr),
	                   x (0),
	                   y (0),
	              hitRay (),
	                hits (),
	             hitComp (),
	      enabledSensors ({ NodeSet () }),
	         overSensors (),
	       activeSensors (),
	           selection (new Selection (this)),                    // SFNode  [ ]   selection    NULL  [Selection]
	         changedTime (clock -> cycle ()),
	        currentSpeed (0),
	    currentFrameRate (0),
	         threadIndex (0),
	             threads (1),
	         threadMutex (),
	        notification (new Notification (this)),
	             console (new Console (this))                       // SFNode  [ ]   console    NULL  [Console]
{
	addChildren (initialized (),
	             renderingProperties,
	             browserProperties,
	             browserOptions,
	             javaScriptEngine,
	             activeLayer,
	             activeNavigationInfoChanged,
	             viewer,
	             examineViewer,
	             walkViewer,
	             flyViewer,
	             noneViewer,
	             lookAt,
	             activeViewpointChanged,
	             keyDeviceSensorNodeOutput,
	             overSensors,
	             activeSensors,
	             selection,
	             notification,
	             console);

	initialized () .isTainted (true);
}

void
X3DBrowserContext::initialize ()
{
	X3DExecutionContext::initialize ();

	// Initialize clock

	advanceClock ();

	// Properties

	renderingProperties -> setup ();
	browserProperties   -> setup ();
	browserOptions      -> setup ();
	javaScriptEngine    -> setup ();
	selection           -> setup ();
	console             -> setup ();

	// Lights

	for (int32_t i = 0; i < renderingProperties -> maxLights (); ++ i)
		lights .push (GL_LIGHT0 + i);

	// TextureUnits

	for (int32_t i = 1; i < renderingProperties -> textureUnits (); ++ i)
		textureUnits .push (i);

	threads .resize (std::min <int32_t> (renderingProperties -> maxThreads (), MAX_DOWNLOAD_THREADS));

	// Initialize OpenGL context

	if (glXGetCurrentContext ())
	{
		glClearColor (0, 0, 0, 0);
		glClearDepth (1);

		glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
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

		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glHint (GL_GENERATE_MIPMAP_HINT,        GL_NICEST);
		glHint (GL_FOG_HINT,                    GL_NICEST);

		//	glHint (GL_POINT_SMOOTH_HINT,   GL_NICEST);
		//	glHint (GL_LINE_SMOOTH_HINT,    GL_NICEST);
		//	glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		//
		//	glEnable (GL_POINT_SMOOTH);
		//	glEnable (GL_LINE_SMOOTH);
		//	glEnable (GL_POLYGON_SMOOTH);

		notification -> setup ();
	}

	initialized () .addInterest (this, &X3DBrowserContext::set_initialized);
	shutdown () .addInterest (this, &X3DBrowserContext::set_shutdown);
}

void
X3DBrowserContext::set_initialized ()
{
	getWorld () -> getActiveLayer () .addInterest (this, &X3DBrowserContext::set_activeLayer);

	set_activeLayer ();
}

void
X3DBrowserContext::set_shutdown ()
{
	hits .clear ();

	overSensors   .clear ();
	activeSensors .clear ();
}

void
X3DBrowserContext::advanceClock ()
{
	clock -> advance ();

	if (getActiveLayer ())
		currentSpeed .setPosition (getActiveLayer () -> getViewpoint () -> getTransformationMatrix () .translation (), currentFrameRate);

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
X3DBrowserContext::getThread ()
{
	std::lock_guard <std::mutex> lock (threadMutex);

	threadIndex = (threadIndex + 1) % threads .size ();

	return threads [threadIndex];
}

void
X3DBrowserContext::set_activeLayer ()
{
	if (activeLayer not_eq getWorld () -> getActiveLayer ())
	{
		if (activeLayer)
		{
			activeLayer -> getNavigationInfoStack () -> bindTime () .removeInterest (this, &X3DBrowserContext::set_navigationInfo);
			activeLayer -> getViewpointStack () -> bindTime () .removeInterest (this, &X3DBrowserContext::set_viewpoint);
		}

		activeLayer = getWorld () -> getActiveLayer ();

		if (activeLayer)
		{
			activeLayer -> getNavigationInfoStack () -> bindTime () .addInterest (this, &X3DBrowserContext::set_navigationInfo);
			activeLayer -> getViewpointStack () -> bindTime () .addInterest (this, &X3DBrowserContext::set_viewpoint);
		}

		set_navigationInfo ();
		set_viewpoint ();
	}
}

void
X3DBrowserContext::set_navigationInfo ()
{
	if (activeNavigationInfo)
		activeNavigationInfo -> type () .removeInterest (this, &X3DBrowserContext::set_navigationInfo_type);

	activeNavigationInfo        = activeLayer ? activeLayer -> getNavigationInfo () : nullptr;
	activeNavigationInfoChanged = getCurrentTime ();

	if (activeNavigationInfo)
	{
		activeNavigationInfo -> shutdown () .addInterest (this, &X3DBrowserContext::remove_navigationInfo);
		activeNavigationInfo -> type () .addInterest (this, &X3DBrowserContext::set_navigationInfo_type);
	}

	set_navigationInfo_type ();
}

void
X3DBrowserContext::remove_navigationInfo ()
{
	activeNavigationInfo        = nullptr;
	activeNavigationInfoChanged = getCurrentTime ();
	set_navigationInfo_type ();
}

void
X3DBrowserContext::set_viewpoint ()
{
	activeViewpointChanged = getCurrentTime ();
}

void
X3DBrowserContext::set_navigationInfo_type ()
{
	if (activeNavigationInfo)
	{
		for (const auto & type : activeNavigationInfo -> type ())
		{
			if (type == "NONE")
			{
				viewer = ViewerType::NONE;
				goto END;
			}
			else if (type == "WALK")
			{
				viewer = ViewerType::WALK;
				goto END;
			}
			else if (type == "FLY")
			{
				viewer = ViewerType::FLY;
				goto END;
			}
			else if (type == "LOOKAT")
				;
			else if (type == "ANY")
				;
			else
			{
				viewer = ViewerType::EXAMINE;
				goto END;
			}
		}

		viewer = ViewerType::EXAMINE;

END:;

		examineViewer = activeNavigationInfo -> type () .empty ();
		walkViewer    = false;
		flyViewer     = false;
		noneViewer    = false;
		lookAt        = false;

		for (const auto & type : activeNavigationInfo -> type ())
		{
			if (type == "NONE")
				noneViewer = true;

			else if (type == "EXAMINE")
				examineViewer = true;

			else if (type == "WALK")
				walkViewer = true;

			else if (type == "FLY")
				flyViewer = true;

			else if (type == "LOOKAT")
				lookAt = true;

			else if (type == "ANY")
			{
				examineViewer = true;
				walkViewer    = true;
				flyViewer     = true;
				lookAt        = true;
				noneViewer    = true;
			}
			else
				examineViewer = true;
		}
	}
	else
	{
		viewer        = ViewerType::NONE;
		examineViewer = false;
		walkViewer    = false;
		flyViewer     = false;
		noneViewer    = false;
		lookAt        = false;
	}
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

// Picking

void
X3DBrowserContext::pick (const double _x, const double _y)
{
	x = _x;
	y = _y;

	// Clear hits.

	hits .clear ();

	// Pick.

	getWorld () -> traverse (TraverseType::PICKING);

	// Selection end.

	std::stable_sort (hits .begin (), hits .end (), hitComp);

	enabledSensors = { NodeSet () };
}

Line3f
X3DBrowserContext::getHitRay () const
{
	try
	{
		return ViewVolume::unProjectLine (x, y, ModelViewMatrix4d (), ProjectionMatrix4d (), Viewport4i ());
	}
	catch (const std::domain_error &)
	{
		return Line3f (Vector3f (), Vector3f ());
	}
}

void
X3DBrowserContext::addHit (const Matrix4f & transformationMatrix, const IntersectionPtr & intersection, X3DBaseNode* const node)
{
	hits .emplace_front (new Hit (transformationMatrix, hitRay, intersection, enabledSensors .back (), node));
}

void
X3DBrowserContext::motionNotifyEvent ()
{
	// Set isOver to FALSE for appropriate nodes

	std::deque <X3DBaseNode*> difference;

	if (getHits () .size ())
	{
		std::set_difference (overSensors .begin (), overSensors .end (),
		                     getHits () .front () -> sensors .begin (), getHits () .front () -> sensors .end (),
		                     std::back_inserter (difference));
	}
	else
		difference .assign (overSensors .begin (), overSensors .end ());

	for (const auto & node : difference)
	{
		auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> set_over (getHits () .front (), false);

		else
		{
			auto anchor = dynamic_cast <Anchor*> (node);

			if (anchor)
				anchor -> set_over (false);
		}
	}

	// Set isOver to TRUE for appropriate nodes

	if (getHits () .size ())
	{
		overSensors .assign (getHits () .front () -> sensors .begin (),
		                     getHits () .front () -> sensors .end ());

		for (const auto & node : overSensors)
		{
			auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node .getValue ());

			if (pointingDeviceSensorNode)
				pointingDeviceSensorNode -> set_over (getHits () .front (), true);

			else
			{
				auto anchor = dynamic_cast <Anchor*> (node .getValue ());

				if (anchor)
					anchor -> set_over (true);
			}
		}
	}
	else
		overSensors .clear ();

	// Forward motion event to active drag sensor nodes

	for (const auto & node : activeSensors)
	{
		auto dragSensorNode = dynamic_cast <X3DDragSensorNode*> (node .getValue ());

		if (dragSensorNode)
		{
			dragSensorNode -> set_motion (getHits () .size ()
			                              ? getHits () .front ()
													: std::make_shared <Hit> (Matrix4f (), hitRay, std::make_shared <Intersection> (), NodeSet (), nullptr));
		}
	}
}

void
X3DBrowserContext::buttonPressEvent ()
{
	activeSensors .assign (getHits () .front () -> sensors .begin (),
	                       getHits () .front () -> sensors .end ());

	for (const auto & node : activeSensors)
	{
		auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node .getValue ());

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> set_active (getHits () .front (), true);

		else
		{
			auto anchor = dynamic_cast <Anchor*> (node .getValue ());

			if (anchor)
				anchor -> set_active (true);
		}
	}
}

void
X3DBrowserContext::buttonReleaseEvent ()
{
	for (const auto & node : activeSensors)
	{
		auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node .getValue ());

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> set_active (std::make_shared <Hit> (Matrix4f (), hitRay, std::make_shared <Intersection> (), NodeSet (), nullptr),
			                                        false);

		else
		{
			auto anchor = dynamic_cast <Anchor*> (node .getValue ());

			if (anchor)
				anchor -> set_active (false);
		}
	}

	activeSensors .clear ();
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
			assert (router .size () == 0);

			// Display

			glClearColor (0, 0, 0, 0);
			glClear (GL_COLOR_BUFFER_BIT);

			getWorld () -> traverse (TraverseType::COLLECT);

			displayed () .processInterests ();

			swapBuffers ();

			// Finish

			finished () .processInterests ();

			GLenum errorNum = glGetError ();

			if (errorNum not_eq GL_NO_ERROR)
				std::clog << "OpenGL Error at " << SFTime (getCurrentTime ()) .toLocaleString () << ": " << gluErrorString (errorNum) << std::endl;
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

	renderingProperties  .dispose ();
	browserProperties    .dispose ();
	browserOptions       .dispose ();
	javaScriptEngine     .dispose ();
	activeLayer          .dispose ();
	overSensors          .clear ();
	activeSensors        .clear ();
	selection            .dispose ();
	notification         .dispose ();
	console              .dispose ();

	javaScriptEngine .dispose ();

	X3DExecutionContext::dispose ();
}

} // X3D
} // titania
