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

#ifndef __TITANIA_X3D_BROWSER_X3DBROWSER_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_X3DBROWSER_CONTEXT_H__

#include "../Execution/X3DExecutionContext.h"

#include "../Browser/Core/X3DCoreContext.h"
#include "../Browser/Layout/X3DLayoutContext.h"
#include "../Browser/Navigation/X3DNavigationContext.h"
#include "../Browser/Networking/X3DNetworkingContext.h"
#include "../Browser/PointingDeviceSensor/X3DPointingDeviceSensorContext.h"
#include "../Components/KeyDeviceSensor/X3DKeyDeviceSensorNode.h"
#include "../Components/Layout/X3DLayoutNode.h"
#include "../Types/Pointer.h"

#include "../Routing/Router.h"
#include "../Types/Speed.h"

#include <Titania/Chrono/ClockBase.h>

#include <memory>
#include <stack>

namespace titania {
namespace X3D {

using X3DClock         = chrono::clock_base <time_type>;
using LayerStack       = std::stack <X3DLayerNode*>;
using LightStack       = std::stack <GLenum>;
using ClipPlaneStack   = std::stack <GLenum>;
using TextureUnitStack = std::stack <int32_t>;
using TextureArray     = std::vector <int32_t>;

class X3DBrowserContext :
	virtual public X3DBaseNode,
	public X3DExecutionContext,
	public X3DCoreContext,
	public X3DLayoutContext,
	public X3DNavigationContext,
	public X3DNetworkingContext,
	public X3DPointingDeviceSensorContext
{
public:

	///  @name Outputs

	virtual
	SFTime &
	initialized ()
	{ return initializedOutput; }

	virtual
	const SFTime &
	initialized () const
	{ return initializedOutput; }

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

	const Vector4i
	getViewport () const
	{ return viewport; }

	///  @name Event handling

	Router &
	getRouter ()
	{ return router; }

	///  @name JavaScript handling

	const X3DJavaScriptEnginePtr &
	getJavaScriptEngine () const
	{ return javaScriptEngine; }

	///  @name Layer handling

	LayerStack &
	getLayers ()
	{ return layers; }

	///  @name Light stack handling

	LightStack &
	getLights ()
	{ return lights; }

	ClipPlaneStack &
	getClipPlanes ()
	{ return clipPlanes; }

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

	///  @name Key device handling

	void
	setKeyDeviceSensorNode (X3DKeyDeviceSensorNode* const);

	X3DKeyDeviceSensorNode*
	getKeyDeviceSensorNode () const
	{ return keyDeviceSensorNode; }

	///  @name Children

	const SelectionPtr &
	getSelection ()
	{ return selection; }

	const NotificationPtr &
	getNotification () const
	{ return notification; }

	const ConsolePtr &
	getConsole () const
	{ return console; }

	///  @name Event handling

	void
	addEvent ();

	///  @name Rendering

	virtual
	void
	reshape ();

	void
	update ();

	virtual
	void
	traverse (const TraverseType type) final override
	{ update (); }

	virtual
	bool
	makeCurrent () const
	{ return true; }

	virtual
	void
	swapBuffers () const
	{ }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBrowserContext ();


protected:

	RenderingPropertiesPtr renderingProperties;
	BrowserPropertiesPtr   browserProperties;
	BrowserOptionsPtr      browserOptions;
	X3DJavaScriptEnginePtr javaScriptEngine;

	///  @name Constructor

	X3DBrowserContext ();

	virtual
	void
	initialize () override;

	virtual
	const WorldPtr &
	getWorld () const = 0;


private:

	// Members

	SFTime initializedOutput;
	Output pickedOutput;
	Output reshapedOutput;
	Output sensorsOutput;
	Output prepareEventsOutput;
	Output displayedOutput;
	Output finishedOutput;
	Output changedOutput;

	std::unique_ptr <X3DClock> clock;

	Router           router;
	Vector4i         viewport;
	LayerStack       layers;
	LightStack       lights;
	ClipPlaneStack   clipPlanes;
	TextureUnitStack textureUnits;
	TextureUnitStack combinedTextureUnits;
	TextureArray     textureStages;
	bool             texture;

	X3DKeyDeviceSensorNode* keyDeviceSensorNode;
	SFTime                  keyDeviceSensorNodeOutput;

	time_type      changedTime;
	Speed <double> currentSpeed;
	double         currentFrameRate;

	SelectionPtr    selection;
	NotificationPtr notification;
	ConsolePtr      console;

};

} // X3D
} // titania

#endif
