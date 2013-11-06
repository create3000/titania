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

#include "../Execution/X3DExecutionContext.h"

#include "../Browser/Console.h"
#include "../Browser/Notification.h"
#include "../Browser/Picking/HitArray.h"
#include "../Browser/Picking/Selection.h"
#include "../Browser/Properties/BrowserOptions.h"
#include "../Browser/Properties/BrowserProperties.h"
#include "../Browser/Properties/RenderingProperties.h"
#include "../Browser/Viewer/ViewerType.h"
#include "../Components/KeyDeviceSensor/X3DKeyDeviceSensorNode.h"
#include "../Components/Layout/X3DLayoutNode.h"
#include "../Execution/BindableNodeStack.h"
#include "../Execution/World.h"
#include "../JavaScript/X3DJavaScriptEngine.h"

#include "../Rendering/X3DRenderer.h"
#include "../Routing/Router.h"
#include "../Types/Speed.h"

#include <Titania/Chrono/ClockBase.h>

#include <memory>
#include <mutex>
#include <stack>

namespace titania {
namespace X3D {

typedef chrono::clock_base <time_type> X3DClock;
typedef std::stack <X3DRenderer*>      RendererStack;
typedef std::stack <X3DLayerNode*>     LayerStack;
typedef std::stack <X3DLayoutNode*>    LayoutStack;
typedef std::stack <GLenum>            LightStack;
typedef std::stack <size_t>            TextureUnitStack;
typedef std::vector <size_t>           TextureArray;

class X3DBrowserContext :
	public X3DExecutionContext
{
public:

	///  @name Outputs

	SFTime &
	initialized ()
	{ return initializedOutput; }

	const SFTime &
	initialized () const
	{ return initializedOutput; }

	const Output &
	picked () const
	{ return pickedOutput; }

	const Output &
	reshaped () const
	{ return reshapedOutput; }

	const Output &
	sensors () const
	{ return sensorsOutput; }

	const Output &
	prepareEvents () const
	{ return prepareEventsOutput; }

	const Output &
	displayed () const
	{ return displayedOutput; }

	const Output &
	finished () const
	{ return finishedOutput; }

	const Output &
	changed () const
	{ return changedOutput; }

	SFTime &
	keyDeviceSensorNodeEvent ()
	{ return keyDeviceSensorNodeOutput; }

	const SFTime &
	keyDeviceSensorNodeEvent () const
	{ return keyDeviceSensorNodeOutput; }

	///  @name Time handling

	void
	advanceClock ();

	virtual
	time_type
	getCurrentTime () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	double
	getCurrentSpeed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	double
	getCurrentFrameRate () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Layer handling

	const X3DSFNode <X3DLayerNode> &
	getActiveLayer () const
	{ return activeLayer; }

	const SFTime &
	getActiveNavigationInfoChanged () const
	{ return activeNavigationInfoChanged; }

	const SFTime &
	getActiveViewpointChanged () const
	{ return activeViewpointChanged; }

	///  @name Event handling

	Router &
	getRouter ()
	{ return router; }

	///  @name JavaScript handling

	const X3DSFNode <X3DJavaScriptEngine> &
	getJavaScriptEngine () const
	{ return javaScriptEngine; }

	///  @name Renderer handling

	RendererStack &
	getRenderers ()
	{ return renderers; }

	///  @name Layer handling

	LayerStack &
	getLayers ()
	{ return layers; }

	LayoutStack &
	getLayouts ()
	{ return layouts; }

	///  @name Navigation handling

	virtual
	void
	setViewer (ViewerType value)
	{ viewer = value; }

	const X3DScalar <ViewerType> &
	getViewer () const
	{ return viewer; }

	const SFBool &
	getExamineViewer () const
	{ return examineViewer; }

	const SFBool &
	getWalkViewer () const
	{ return walkViewer; }

	const SFBool &
	getFlyViewer () const
	{ return flyViewer; }

	const SFBool &
	getNoneViewer () const
	{ return noneViewer; }

	const SFBool &
	getLookAt () const
	{ return lookAt; }

	///  @name Light stack handling

	LightStack &
	getLights ()
	{ return lights; }

	///  @name Texture unit stack handling

	TextureUnitStack &
	getTextureUnits ()
	{ return textureUnits; }

	TextureUnitStack &
	getCombinedTextureUnits ()
	{ return combinedTextureUnits; }

	TextureArray &
	getTextureStages ()
	{ return textureStages; }

	void
	isEnabledTexture (bool value)
	{ texture = value; }

	bool
	isEnabledTexture () const
	{ return texture; }

	///  @name Thread handling

	std::mutex &
	getDownloadMutex ();

	///  @name Key device handling

	void
	setKeyDeviceSensorNode (X3DKeyDeviceSensorNode* const);

	X3DKeyDeviceSensorNode*
	getKeyDeviceSensorNode () const
	{ return keyDeviceSensorNode; }

	///  @name Picking

	void
	pick (const double, const double);

	std::deque <NodeSet> &
	getSensors ()
	{ return enabledSensors; }

	void
	updateHitRay ()
	{ hitRay = getHitRay (); }

	Line3d
	getHitRay () const;

	void
	addHit (const Matrix4d &, const IntersectionPtr &, X3DBaseNode* const);

	const HitArray &
	getHits () const
	{ return hits; }

	void
	motionNotifyEvent ();

	void
	buttonPressEvent ();

	void
	buttonReleaseEvent ();

	const X3DSFNode <Selection> &
	getSelection ()
	{ return selection; }

	///  @name Notification handling

	const X3DSFNode <Notification> &
	getNotification () const
	{ return notification; }

	///  @name Console handling

	const X3DSFNode <Console> &
	getConsole () const
	{ return console; }

	///  @name Event handling

	void
	addEvent ();

	///  @name Rendering

	void
	update ();

	virtual
	bool
	makeCurrent () const
	{ return true; }

	virtual
	void
	swapBuffers () const
	{ }

	///  @name Destruction

	void
	dispose ();

	~X3DBrowserContext ();


protected:

	X3DSFNode <RenderingProperties> renderingProperties;
	X3DSFNode <BrowserProperties>   browserProperties;
	X3DSFNode <BrowserOptions>      browserOptions;
	X3DSFNode <X3DJavaScriptEngine> javaScriptEngine;

	///  @name Constructor

	X3DBrowserContext ();

	virtual
	void
	initialize () override;

	void
	setEnabled (bool value)
	{ enabled = value; }

	virtual
	const X3DSFNode <World> &
	getWorld () const = 0;

	NavigationInfo*
	getActiveNavigationInfo () const
	{ return activeNavigationInfo; }


private:

	void
	set_initialized ();

	void
	set_shutdown ();

	void
	set_activeLayer ();

	void
	set_navigationInfo ();

	void
	remove_navigationInfo ();

	void
	set_viewpoint ();

	void
	set_navigationInfo_type ();

	// Members

	bool enabled;

	SFTime initializedOutput;
	Output pickedOutput;
	Output reshapedOutput;
	Output sensorsOutput;
	Output prepareEventsOutput;
	Output displayedOutput;
	Output finishedOutput;
	Output changedOutput;

	std::shared_ptr <X3DClock> clock;

	Router           router;
	RendererStack    renderers;
	LayerStack       layers;
	LayoutStack      layouts;
	LightStack       lights;
	TextureUnitStack textureUnits;
	TextureUnitStack combinedTextureUnits;
	TextureArray     textureStages;
	bool             texture;

	X3DSFNode <X3DLayerNode> activeLayer;
	NavigationInfo*          activeNavigationInfo;
	SFTime                   activeNavigationInfoChanged;
	X3DScalar <ViewerType>   viewer;
	SFBool                   examineViewer;
	SFBool                   walkViewer;
	SFBool                   flyViewer;
	SFBool                   noneViewer;
	SFBool                   lookAt;
	SFTime                   activeViewpointChanged;

	X3DKeyDeviceSensorNode* keyDeviceSensorNode;
	SFTime                  keyDeviceSensorNodeOutput;

	double                x;
	double                y;
	Line3d                hitRay;
	HitArray              hits;
	HitComp               hitComp;
	std::deque <NodeSet>  enabledSensors;
	MFNode                overSensors;
	MFNode                activeSensors;
	X3DSFNode <Selection> selection;

	time_type      changedTime;
	Speed <double> currentSpeed;
	double         currentFrameRate;

	size_t                  downloadMutexIndex;
	std::deque <std::mutex> downloadMutexes;
	std::mutex              downloadMutex;

	X3DSFNode <Notification> notification;
	X3DSFNode <Console>      console;

};

} // X3D
} // titania

#endif
