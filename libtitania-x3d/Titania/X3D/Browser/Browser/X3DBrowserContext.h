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

#ifndef __TITANIA_X3D_BROWSER_BROWSER_X3DBROWSER_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_BROWSER_X3DBROWSER_CONTEXT_H__

#include "../../Execution/X3DExecutionContext.h"

#include "../../Rendering/OpenGL.h"
#include "../../Routing/X3DRouterObject.h"
#include "../../Types/Pointer.h"
#include "../Core/X3DCoreContext.h"
#include "../KeyDeviceSensor/X3DKeyDeviceSensorContext.h"
#include "../Layering/X3DLayeringContext.h"
#include "../Layout/X3DLayoutContext.h"
#include "../Lighting/X3DLightingContext.h"
#include "../Navigation/X3DNavigationContext.h"
#include "../Networking/X3DNetworkingContext.h"
#include "../ParticleSystems/X3DParticleSystemsContext.h"
#include "../PointingDeviceSensor/X3DPointingDeviceSensorContext.h"
#include "../Rendering/X3DRenderingContext.h"
#include "../Scripting/X3DScriptingContext.h"
#include "../Shape/X3DShapeContext.h"
#include "../Text/X3DTextContext.h"
#include "../Texturing2D/X3DTexturing2DContext.h"
#include "../Time/X3DTimeContext.h"

namespace titania {
namespace X3D {

class X3DBrowserContext :
	virtual public X3DBaseNode,
	public X3DExecutionContext,
	public X3DRouterObject,
	public X3DCoreContext,
	public X3DKeyDeviceSensorContext,
	public X3DLayeringContext,
	public X3DLayoutContext,
	public X3DLightingContext,
	public X3DNavigationContext,
	public X3DNetworkingContext,
	public X3DParticleSystemsContext,
	public X3DPointingDeviceSensorContext,
	public X3DRenderingContext,
	public X3DShapeContext,
	public X3DScriptingContext,
	public X3DTextContext,
	public X3DTexturing2DContext,
	public X3DTimeContext
{
public:

	///  @name Outputs

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

	virtual
	void
	addEvent () final override;

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

	Output reshapedOutput;
	Output sensorsOutput;
	Output prepareEventsOutput;
	Output displayedOutput;
	Output finishedOutput;
	Output changedOutput;

	time_type triggerTime;

	SelectionPtr    selection;
	NotificationPtr notification;
	ConsolePtr      console;

};

} // X3D
} // titania

#endif
