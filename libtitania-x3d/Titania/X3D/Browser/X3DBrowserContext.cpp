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

#include <Titania/Chrono/CountingClock.h>
#include <Titania/Chrono/SystemClock.h>

namespace titania {
namespace X3D {

X3DBrowserContext::X3DBrowserContext () :
	X3DExecutionContext (),                                        
	            sensors (),                                        // [out]  sensors
	           reshaped (),                                        // [out]  reshape
	      prepareEvents (),                                        // [out]  prepareEvents
	          displayed (),                                        // [out]  displayed
	           finished (),                                        // [out]  finished
	            changed (),                                        // [out]  changed
	             select (false),                                   // SFBool  [in,out]  select              FALSE
	renderingProperties (new RenderingProperties (this)),          // SFNode  [ ]       renderingProperties NULL   [RenderingProperties]
	  browserProperties (new BrowserProperties   (this)),          // SFNode  [ ]       browserProperties   NULL   [BrowserProperties]
	     browserOptions (new BrowserOptions      (this)),          // SFNode  [ ]       browserOptions      NULL   [BrowserOptions]
	   javaScriptEngine (new SpiderMonkey        (this)),          // SFNode  [ ]       javaScriptEngine    NULL   [JavaScriptEngine]
	              clock (new chrono::system_clock <time_type> ()), 
	             router (),                                        
	             layers (),                                        
	             lights (),                                        
	       textureUnits (),                                        
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
	            console (new Console (this))                       // SFNode  [ ]   console    NULL  [Console]
{
	setChildren (select,
	             renderingProperties,
	             browserProperties,
	             browserOptions,
	             javaScriptEngine,
	             selection,
	             console);
}

void
X3DBrowserContext::initialize ()
{
	X3DExecutionContext::initialize ();

	// Initialize clock

	clock -> advance ();

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
	}

	initialized .addInterest (this, &X3DBrowserContext::set_initialized);
}

void
X3DBrowserContext::set_initialized ()
{
	hits .clear ();

	overSensors   .clear ();
	activeSensors .clear ();
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

// Event handling

Router &
X3DBrowserContext::getRouter ()
{
	return router;
}

// Selection

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

	std::sort (hits .begin (), hits .end (), hitComp);

	enabledSensors = { NodeSet () };
}

Line3f
X3DBrowserContext::getHitRay () const
{
	GLint                viewport [4]; // x, y, width, heigth
	Matrix4d::array_type modelview, projection;

	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev (GL_PROJECTION_MATRIX, projection);

	GLdouble px, py, pz;

	// Near plane point
	gluUnProject (x, y, 0, modelview, projection, viewport, &px, &py, &pz);
	Vector3f near (px, py, pz);

	// Far plane point
	gluUnProject (x, y, 1, modelview, projection, viewport, &px, &py, &pz);
	Vector3f far (px, py, pz);

	return Line3f (near, far);
}

void
X3DBrowserContext::addHit (const Matrix4f & transformationMatrix, const std::shared_ptr <Intersection> & intersection, X3DBaseNode* const node)
{
	hits .emplace_back (new Hit (transformationMatrix, hitRay, intersection, enabledSensors .back (), node));
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
	}

	// Set isOver to TRUE for appropriate nodes

	if (getHits () .size ())
	{
		overSensors = getHits () .front () -> sensors;

		for (const auto & node : overSensors)
		{
			auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

			if (pointingDeviceSensorNode)
			{
				pointingDeviceSensorNode -> set_over (getHits () .front (), true);
			}
		}
	}
	else
		overSensors .clear ();

	// Forward motion event to active drag sensor nodes

	for (const auto & node : activeSensors)
	{
		auto dragSensorNode = dynamic_cast <X3DDragSensorNode*> (node);

		if (dragSensorNode)
		{
			dragSensorNode -> set_motion (
			   getHits () .size ()
			   ? getHits () .front ()
				: std::make_shared <Hit> (Matrix4f (), hitRay, std::make_shared <Intersection> (), enabledSensors .back (), nullptr)
			   );
		}
	}
}

void
X3DBrowserContext::buttonPressEvent ()
{
	activeSensors = getHits () .front () -> sensors;

	for (const auto & node : activeSensors)
	{
		auto anchor = dynamic_cast <Anchor*> (node);

		if (anchor)
			anchor -> set_active (true);

		else
		{
			auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

			if (pointingDeviceSensorNode)
				pointingDeviceSensorNode -> set_active (getHits () .front (), true);
		}
	}
}

void
X3DBrowserContext::buttonReleaseEvent ()
{
	for (const auto & node : activeSensors)
	{
		auto anchor = dynamic_cast <Anchor*> (node);

		if (anchor)
			anchor -> set_active (false);

		else
		{
			auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

			if (pointingDeviceSensorNode)
				pointingDeviceSensorNode -> set_active (
				   std::make_shared <Hit> (Matrix4f (), hitRay, std::make_shared <Intersection> (), enabledSensors .back (), nullptr),
				   false
				   );
		}
	}

	activeSensors .clear ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
X3DBrowserContext::notify ()
{
	if (changedTime == getCurrentTime ())
		return;

	changedTime = getCurrentTime ();
	changed .processInterests ();
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

		clock -> advance ();

		X3DViewpointNode* activeViewpoint = getActiveViewpoint ();
		
		if (activeViewpoint)
			currentSpeed .setPosition (activeViewpoint -> getTransformationMatrix () .translation (), currentFrameRate);
		
		else
			currentSpeed .setPosition (Vector3f (), 0);
	
		currentFrameRate = 1 / clock -> interval ();

		prepareEvents .processInterests ();
		router .processEvents ();

		getWorld () -> traverse (TraverseType::CAMERA);
		getWorld () -> traverse (TraverseType::COLLISION);

		sensors .processInterests ();
		router .processEvents ();
		
		getGarbageCollector () .dispose ();
		assert (router .size () == 0);
		
		// Display

		glClearColor (0, 0, 0, 0);
		glClear (GL_COLOR_BUFFER_BIT);

		getWorld () -> traverse (TraverseType::COLLECT);

		displayed .processInterests ();
		
		swapBuffers ();

		// Finish

		finished .processInterests ();

		GLenum errorNum = glGetError ();

		if (errorNum not_eq GL_NO_ERROR)
			std::clog << "OpenGL Error at " << SFTime (getCurrentTime ()) .toLocaleString () << ": " << gluErrorString (errorNum) << std::endl;

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
	initialized   .dispose ();
	reshaped      .dispose ();
	prepareEvents .dispose ();
	displayed     .dispose ();
	finished      .dispose ();
	shutdown      .dispose ();
	changed       .dispose ();
	
	renderingProperties .dispose ();
	browserProperties   .dispose ();
	browserOptions      .dispose ();
	selection           .dispose ();
	console             .dispose ();
	
	javaScriptEngine .dispose ();

	X3DExecutionContext::dispose ();
}

} // X3D
} // titania
