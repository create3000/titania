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
 ******************************************************************************/

#include "X3DBrowserContext.h"

#include <Titania/Chrono/CountingClock.h>
#include <Titania/Chrono/SystemClock.h>

namespace titania {
namespace X3D {

X3DBrowserContext::X3DBrowserContext () :
	X3DExecutionContext (),                                        
	        initialized (),                                        // [out]    initialized
	           reshaped (),                                        // [out]    reshape
	            exposed (),                                        // [out]    exposed
	          displayed (),                                        // [out]    displayed
	           finished (),                                        // [out]    finished
	           shutdown (),                                        // [out]    shutdown
	            changed (),                                        // [out]    changed
	renderingProperties (new RenderingProperties (this)),          // SFSting  [ ] renderingProperties NULL  [RenderingProperties]
	  browserProperties (new BrowserProperties   (this)),          // SFSting  [ ] browserProperties   NULL  [BrowserProperties]
	     browserOptions (new BrowserOptions      (this)),          // SFSting  [ ] browserOptions      NULL  [BrowserOptions]
	   javaScriptEngine (new JavaScriptEngine    (this)),          // SFSting  [ ] javaScriptEngine    NULL  [JavaScriptEngine]
	              clock (new chrono::system_clock <time_type> ()), 
	             router (),                                        
	             layers (),                                        
	             lights (),                                        
	           textures (),                                        
	            sensors (),                                        
	                  x (0),                                       
	                  y (0),                                       
	     sensitiveNodes (),                                        
	               hits (),                                        
	        changedTime (clock -> cycle ()),                       
	      priorPosition (),                                        
	       currentSpeed (0),                                       
	   currentFrameRate (0)                                        
{
	//supportedFields, // make X3DBaseNodes of this
	//supportedNodes,
	//supportedComponents,
	//supportedProfiles,

	addField (initializeOnly, "renderingProperties", renderingProperties);
	addField (initializeOnly, "browserProperties",   browserProperties);
	addField (initializeOnly, "browserOptions",      browserOptions);
	addField (initializeOnly, "javaScriptEngine",    javaScriptEngine);
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

	// Lights

	for (int32_t i = 0; i < renderingProperties -> maxLights; ++ i)
		lights .push (GL_LIGHT0 + i);

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

// NavigationInfo handling

NavigationInfo*
X3DBrowserContext::getActiveNavigationInfo () const
{
	return getExecutionContext () -> getActiveLayer () -> getNavigationInfo ();
}

// Viewpoint handling

X3DViewpointNode*
X3DBrowserContext::getActiveViewpoint ()
{
	return getExecutionContext () -> getActiveLayer () -> getViewpoint ();
}

// Texture list handling

void
X3DBrowserContext::addTexture (const std::string & URL, GLuint textureId)
{
	textures [URL] = std::make_pair (textureId, 1);
}

bool
X3DBrowserContext::removeTexture (const std::string & URL, GLuint textureId)
{
	auto texture = textures .find (URL);

	if (texture not_eq textures .end ())
	{
		if (-- texture -> second .second == 0)
		{
			textures .erase (texture);
			return true;
		}
	}

	return false;
}

GLuint
X3DBrowserContext::getTexture (const std::string & URL)
{
	auto texture = textures .find (URL);

	if (texture not_eq textures .end ())
	{
		++ texture -> second .second;
		return texture -> second .first;
	}

	return 0;
}

const TextureIndex &
X3DBrowserContext::getTextures () const
{
	return textures;
}

/// @name Sensors

void
X3DBrowserContext::addSensor (X3DSensorNode* const sensor)
{
	std::clog << "\tAdding sensor " << sensor -> getTypeName () << " to world: " << sensors .getRequesters () .size () << " are registered until now." << std::endl;
	sensors .addInterest (sensor, &X3DSensorNode::update);
	std::clog << "\tNow are " << sensors .getRequesters () .size () << " registered." << std::endl;
}

void
X3DBrowserContext::removeSensor (X3DSensorNode* const sensor)
{
	std::clog << "\tRemoving sensor " << sensor -> getTypeName () << " from world: " << sensors .getRequesters () .size () << " are registered until now." << std::endl;
	sensors .removeInterest (sensor, &X3DSensorNode::update);
	std::clog << "\tNow are " << sensors .getRequesters () .size () << " registered." << std::endl;
}

void
X3DBrowserContext::updateSensors ()
{
	sensors .processInterests ();
}

// Selection

void
X3DBrowserContext::pick (const double _x, const double _y)
{
	x = _x;
	y = _y;

	// Clear hits.

	for (const auto & hit : hits)
		delete hit;

	hits .clear ();

	// Pick.

	pick ();

	// Selection end.

	sensitiveNodes .clear ();

	std::sort (hits .begin (), hits .end (), hitComp);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
X3DBrowserContext::notify (X3DBaseNode* const node)
{
	assert (node);

	router .notify (node);

	if (changedTime == getCurrentTime ())
		return;

	changedTime = getCurrentTime ();
	changed .processInterests ();
}

void
X3DBrowserContext::pick ()
{
	getExecutionContext () -> pick ();
}

void
X3DBrowserContext::prepare ()
{
	clock -> advance ();
	exposed .processInterests ();

	currentFrameRate = 1 / clock -> interval ();

	Vector3d position = getActiveViewpoint () -> getTransformationMatrix () .translation ();
	currentSpeed  = abs (position - priorPosition) * currentFrameRate;
	priorPosition = position;

	updateSensors ();

	router .processEvents ();
	getGarbageCollector () .dispose ();
}

void
X3DBrowserContext::display ()
{
	glClear (GL_COLOR_BUFFER_BIT);

	getExecutionContext () -> display ();

	displayed .processInterests ();
}

void
X3DBrowserContext::finish ()
{
	finished .processInterests ();

	GLenum errorNum = glGetError ();

	if (errorNum not_eq GL_NO_ERROR)
		std::clog << "OpenGL Error at " << SFTime (getCurrentTime ()) .toLocaleString () << ": " << gluErrorString (errorNum) << std::endl;
}

void
X3DBrowserContext::dispose ()
{
	initialized .dispose ();
	reshaped    .dispose ();
	exposed     .dispose ();
	displayed   .dispose ();
	finished    .dispose ();
	shutdown    .dispose ();
	changed     .dispose ();

	X3DChildObject::dispose ();
	//X3DExecutionContext::dispose ();

	getGarbageCollector () .dispose ();
}

} // X3D
} // titania
