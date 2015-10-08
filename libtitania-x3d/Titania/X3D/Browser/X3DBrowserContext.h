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
#include "../Browser/Scripting/X3DScriptingContext.h"
#include "../Browser/Shading/X3DShadingContext.h"
#include "../Browser/Shape/X3DShapeContext.h"
#include "../Browser/Text/X3DTextContext.h"
#include "../Browser/Texturing/X3DTexturingContext.h"
#include "../Browser/Time/X3DTimeContext.h"

#include "../Routing/X3DRouterObject.h"

#include "../Types/Pointer.h"

#include <Magick++.h>

namespace titania {
namespace X3D {

class X3DBrowserContext :
	public X3DScene,
	public X3DCoreContext,
	public X3DShadingContext,
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
	public X3DScriptingContext,
	public X3DShapeContext,
	public X3DTextContext,
	public X3DTexturingContext,
	public X3DTimeContext,
	public X3DRouterObject
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
	
	///  @name Member access

	const WorldPtr &
	getWorld () const
	throw (Error <DISPOSED>);

	const SelectionPtr &
	getSelection ()
	throw (Error <DISPOSED>);

	const NotificationPtr &
	getNotification () const
	throw (Error <DISPOSED>);

	const ConsolePtr &
	getConsole () const
	throw (Error <DISPOSED>);

	///  @name Operations

	std::shared_ptr <Magick::Image>
	getSnapshot (const size_t, const size_t, const bool, const size_t) const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>,
	       std::runtime_error);

	///  @name Event handling

	virtual
	void
	addEvent () final override;

	void
	beginUpdateForFrame ();

	void
	endUpdateForFrame ();

	///  @name Rendering

	virtual
	void
	reshape ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	update ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	void
	traverse (const TraverseType) final override
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

	///  @name Constructor

	X3DBrowserContext ();

	virtual
	void
	initialize () override;

	void
	setWorld (World* const value)
	{ world = value; }


private:

	///  @name Members

	SFTime initializedOutput;
	Output pickedOutput;
	Output reshapedOutput;
	Output prepareEventsOutput;
	Output sensorsOutput;
	Output displayedOutput;
	Output finishedOutput;
	Output changedOutput;

	time_type changedTime;
	time_type freezedTime;

	WorldPtr        world;
	SelectionPtr    selection;
	NotificationPtr notification;
	ConsolePtr      console;

};

} // X3D
} // titania

#endif
