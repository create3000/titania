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

#ifndef __TITANIA_X3D_BROWSER_X3DBROWSER_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_X3DBROWSER_CONTEXT_H__

#include "../RenderingSurface/X3DRenderingSurface.h"

#include "../Execution/X3DScene.h"

#include "../Browser/Core/X3DCoreContext.h"
#include "../Browser/Geometry2D/X3DGeometry2DContext.h"
#include "../Browser/Geometry3D/X3DGeometry3DContext.h"
#include "../Browser/KeyDeviceSensor/X3DKeyDeviceSensorContext.h"
#include "../Browser/Layering/X3DLayeringContext.h"
#include "../Browser/Layout/X3DLayoutContext.h"
#include "../Browser/Lighting/X3DLightingContext.h"
#include "../Browser/Navigation/X3DNavigationContext.h"
#include "../Browser/Networking/X3DNetworkingContext.h"
#include "../Browser/ParticleSystems/X3DParticleSystemsContext.h"
#include "../Browser/PointingDeviceSensor/X3DPointingDeviceSensorContext.h"
#include "../Browser/Rendering/X3DRenderingContext.h"
#include "../Browser/RigidBodyPhysics/X3DRigidBodyPhysicsContext.h"
#include "../Browser/Scripting/X3DScriptingContext.h"
#include "../Browser/Shaders/X3DShadersContext.h"
#include "../Browser/Shape/X3DShapeContext.h"
#include "../Browser/Text/X3DTextContext.h"
#include "../Browser/Texturing/X3DTexturingContext.h"
#include "../Browser/CubeMapTexturing/X3DCubeMapTexturingContext.h"
#include "../Browser/Time/X3DTimeContext.h"
#include "../Browser/Tools/X3DToolContext.h"

#include "../Types/Pointer.h"

#include <Magick++.h>

namespace titania {
namespace X3D {

class X3DBrowserContext :
	public X3DRenderingSurface,
	public X3DScene,
	public X3DCoreContext,
	public X3DShadersContext,
	public X3DGeometry2DContext,
	public X3DGeometry3DContext,
	public X3DKeyDeviceSensorContext,
	public X3DLayeringContext,
	public X3DLayoutContext,
	public X3DLightingContext,
	public X3DNavigationContext,
	public X3DNetworkingContext,
	public X3DParticleSystemsContext,
	public X3DPointingDeviceSensorContext,
	public X3DRenderingContext,
	public X3DRigidBodyPhysicsContext,
	public X3DScriptingContext,
	public X3DShapeContext,
	public X3DTextContext,
	public X3DTexturingContext,
	public X3DCubeMapTexturingContext,
	public X3DTimeContext,
	public X3DToolContext
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
	
	///  @name Member access

	const X3DBrowserContextPtr &
	getSharedContext () const
	{ return sharedContext; }

	const WorldPtr &
	getWorld () const
	{ return world; }

	const X3DPtr <LayerSet> &
	getHeadUpDisplay () const;

	void
	setSelection (const SelectionPtr & value);

	const SelectionPtr &
	getSelection () const
	{ return selection; }

	const NotificationPtr &
	getNotification () const
	{ return notification; }

	const ConsolePtr &
	getConsole () const
	{ return console; }

	///  @name Operations

	Magick::Image
	getSnapshot (const size_t width, const size_t height, const bool alphaChannel, const size_t antialiasing)
	throw (Error <INSUFFICIENT_CAPABILITIES>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Event handling

	void
	setRouter (const std::shared_ptr <Router> & value);

	const std::shared_ptr <Router> &
	getRouter () const
	{ return router; }

	virtual
	void
	addEvent () final override;

	void
	beginUpdateForFrame ();

	void
	endUpdateForFrame ();

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBrowserContext () override;


protected:

	///  @name Friends

	friend class ContextLock;

	///  @name Constructor

	X3DBrowserContext (const X3DBrowserContextPtr & sharedContext);

	virtual
	void
	initialize () override;

	///  @name Member access

	void
	setWorld (World* const value)
	{ world = value; }

	///  @name Operations

	void
	update ()
	noexcept (true);

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{ update (); }


private:

	///  @name Members


	SFTime initializedOutput;
	Output pickedOutput;
	Output prepareEventsOutput;
	Output sensorsOutput;
	Output displayedOutput;
	Output finishedOutput;
	Output changedOutput;

	time_type changedTime;
	time_type freezedTime;

	std::shared_ptr <Router> router;
	X3DBrowserContextPtr     sharedContext;
	WorldPtr                 world;
	WorldPtr                 headUpDisplay;
	SelectionPtr             selection;
	NotificationPtr          notification;
	ConsolePtr               console;

};

} // X3D
} // titania

#endif
