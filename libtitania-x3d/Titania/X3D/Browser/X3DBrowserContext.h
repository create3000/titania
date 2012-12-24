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

#ifndef __TITANIA_X3D_BROWSER_X3DBROWSER_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_X3DBROWSER_CONTEXT_H__

#include "../Base/Output.h"
#include "../Browser/Properties/BrowserOptions.h"
#include "../Browser/Properties/BrowserProperties.h"
#include "../Browser/Properties/RenderingProperties.h"
#include "../Components/Core/X3DSensorNode.h"
#include "../Components/Navigation/NavigationInfo.h"
#include "../Components/Navigation/Viewpoint.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Execution/Scene.h"
#include "../JavaScript/JavaScriptEngine.h"

#include "../Browser/HitArray.h"
#include "../Rendering/X3DRenderer.h"
#include "../Routing/Router.h"

#include <Titania/Chrono/ClockBase.h>

#include <memory>
#include <stack>

namespace titania {
namespace X3D {

typedef chrono::clock_base <time_type>                      X3DClock;
typedef std::stack <GLenum>                                 LightStack;
typedef std::map <std::string, std::pair <GLuint, size_t>> TextureIndex;

class X3DBrowserContext :
	public X3DExecutionContext
{
public:

	Output initialized;
	Output reshaped;
	Output exposed;
	Output displayed;
	Output finished;
	Output shutdown;
	Output changed;

	///  @name Time handling

	virtual
	time_type
	getCurrentTime () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	double
	getCurrentSpeed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	double
	getCurrentFrameRate () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);
	  
	virtual
	Scene*
	getExecutionContext () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	///  @name Event handling

	Router &
	getRouter ();

	///  @name Layer handling

	void
	pushLayer (X3DLayerNode* const);

	void
	popLayer ();

	X3DLayerNode*
	getCurrentLayer () const;

	///  @name NavigationInfo handling

	NavigationInfo*
	getActiveNavigationInfo () const;

	///  @name Viewpoint handling

	X3DViewpointNode*
	getActiveViewpoint ();

	///  @name Light stack handling

	LightStack &
	getLights ();

	///  @name Texture handling

	void addTexture (const std::string &, GLuint);

	bool removeTexture (const std::string &, GLuint);

	GLuint
	getTexture (const std::string &);

	const TextureIndex &
	getTextures () const;

	///  @name Sensor handling

	void
	addSensor (X3DSensorNode* const);

	void
	removeSensor (X3DSensorNode* const);

	void
	updateSensors ();

	///  @name Event handling

	virtual
	void
	notify (X3DBaseNode* const);

	///  @name Rendering handling

	virtual
	void
	intersect ();

	void
	prepare ();

	virtual
	void
	display ();

	void
	finish ();

	///  @name Dispose

	void
	dispose ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual
	void
	pushRenderer (X3DRenderer*) = 0;

	virtual
	void
	popRenderer () = 0;

	virtual
	X3DRenderer*
	getCurrentRenderer () = 0;

	virtual
	const X3DRenderer*
	getCurrentRenderer () const = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////  pushPointingDeviceSensorNode
	virtual
	void
	pushSensitiveNode (X3DBaseNode* node) = 0;

	virtual
	void
	popSensitiveNode () = 0;

	virtual
	const std::vector <X3DBaseNode*> &
	getSensitiveNodes () const = 0;

	virtual
	bool
	isSensitive () const = 0;

	virtual
	void
	pick (const size_t, const size_t) = 0;

	virtual
	Line3f
	getHitRay () const = 0;

	virtual
	void
	addHit (Hit*) = 0;

	virtual
	const HitArray &
	getHits () const = 0;


protected:

	SFNode <RenderingProperties> renderingProperties;
	SFNode <BrowserProperties>   browserProperties;
	SFNode <BrowserOptions>      browserOptions;
	SFNode <JavaScriptEngine>    javaScriptEngine;

	///  @name Constructor

	X3DBrowserContext ();

	virtual
	void
	initialize ();


private:

	std::shared_ptr <X3DClock> clock;
	Router                     router;
	std::stack <X3DLayerNode*> layers;
	LightStack                 lights;
	TextureIndex               textures;
	Output                     sensors;
	time_type                  changedTime;
	Vector3d                   priorPosition;
	double                     currentSpeed;
	double                     currentFrameRate;

};

} // X3D
} // titania

#endif
