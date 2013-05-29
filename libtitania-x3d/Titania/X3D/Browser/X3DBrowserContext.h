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

#ifndef __TITANIA_X3D_BROWSER_X3DBROWSER_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_X3DBROWSER_CONTEXT_H__

#include "../Browser/Console.h"
#include "../Browser/Properties/BrowserOptions.h"
#include "../Browser/Properties/BrowserProperties.h"
#include "../Browser/Properties/RenderingProperties.h"
#include "../Browser/Selection.h"
#include "../Browser/Viewer/ViewerType.h"
#include "../Components/Core/X3DSensorNode.h"
#include "../Components/Navigation/NavigationInfo.h"
#include "../Components/Navigation/Viewpoint.h"
#include "../Execution/Scene.h"
#include "../Execution/X3DExecutionContext.h"
#include "../JavaScript/JavaScriptEngine.h"

#include "../Browser/HitArray.h"
#include "../Rendering/X3DRenderer.h"
#include "../Routing/Router.h"
#include "../Types/Speed.h"

#include <Titania/Chrono/ClockBase.h>

#include <memory>
#include <stack>

namespace titania {
namespace X3D {

typedef chrono::clock_base <time_type> X3DClock;
typedef std::stack <X3DRenderer*>      RendererStack;
typedef std::stack <X3DLayerNode*>     LayerStack;
typedef std::stack <GLenum>            LightStack;
typedef std::stack <size_t>            TextureUnitStack;

class X3DBrowserContext :
	public X3DExecutionContext
{
public:

	Output sensors;
	Output reshaped;
	Output prepareEvents;
	Output displayed;
	Output finished;
	Output changed;
	SFBool select;

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
	const SFNode <Scene> &
	getExecutionContext () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	///  @name Event handling

	Router &
	getRouter ();

	///  @name Renderer handling

	RendererStack &
	getRenderers ()
	{ return renderers; }

	///  @name Layer handling

	LayerStack &
	getLayers ()
	{ return layers; }

	///  @name Light stack handling

	virtual
	ViewerType
	getViewerType () const = 0;

	///  @name Light stack handling

	LightStack &
	getLights ()
	{ return lights; }

	///  @name Texture unit stack handling

	TextureUnitStack &
	getTextureUnits ()
	{ return textureUnits; }

	///  @name NavigationInfo handling

	NavigationInfo*
	getActiveNavigationInfo () const;

	///  @name Viewpoint handling

	X3DViewpointNode*
	getActiveViewpoint () const;

	///  @name Console handling

	const SFNode <Console> &
	getConsole () const
	{ return console; }

	///  @name Picking

	void
	pick (const double, const double);

	std::deque <NodeSet> &
	getSensors ()
	{ return enabledSensors; }

	Line3f
	getHitRay () const;

	void
	addHit (const Matrix4f &, const std::shared_ptr <Intersection> &, X3DBaseNode* const);

	const HitArray &
	getHits () const
	{ return hits; }

	void
	motionNotifyEvent ();

	void
	buttonPressEvent ();

	void
	buttonReleaseEvent ();

	const SFNode <Selection> &
	getSelection ()
	{ return selection; }

	///  @name Event handling

	void
	notify ();

	///  @name Event handling

	virtual
	void
	swapBuffers ()
	{ }

	///  @name Dispose

	void
	dispose ();


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

	virtual
	void
	update ();

	std::shared_ptr <X3DClock> clock;


private:

	void
	set_initialized ();

	Router           router;
	RendererStack    renderers;
	LayerStack       layers;
	LightStack       lights;
	TextureUnitStack textureUnits;

	double               x;
	double               y;
	Line3f               hitRay;
	HitArray             hits;
	HitComp              hitComp;
	std::deque <NodeSet> enabledSensors;
	NodeSet              overSensors;
	NodeSet              activeSensors;
	SFNode <Selection>   selection;

	time_type        changedTime;
	Speed <double>   currentSpeed;
	double           currentFrameRate;
	SFNode <Console> console;

};

} // X3D
} // titania

#endif
