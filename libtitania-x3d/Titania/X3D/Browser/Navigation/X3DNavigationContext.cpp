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

#include "X3DNavigationContext.h"

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Components/Lighting/DirectionalLight.h"
#include "../../Execution/World.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../X3DBrowser.h"

// First include X3DExecutionContext
#include "../../Execution/BindableNodeStack.h"

namespace titania {
namespace X3D {

X3DNavigationContext::X3DNavigationContext () :
	               X3DBaseNode (),
	                 headLight (new DirectionalLight (getExecutionContext ())),
	               activeLayer (),
	      activeNavigationInfo (nullptr),
	activeNavigationInfoOutput (),
	                    viewer (X3DConstants::NoneViewer),
	             privateViewer (X3DConstants::X3DBaseNode),
	          availableViewers (),
	     activeViewpointOutput (),
	          activeCollisions (),
	         straightenHorizon ()
{
	addChildren (headLight,
	             activeLayer,
	             activeNavigationInfoOutput,
	             viewer,
	             privateViewer,
	             availableViewers,
	             activeViewpointOutput,
	             straightenHorizon);
}

void
X3DNavigationContext::initialize ()
{
	headLight -> setup ();

	getBrowser () -> initialized () .addInterest (this, &X3DNavigationContext::set_initialized);
}

X3D::X3DConstants::NodeType
X3DNavigationContext::getCurrentViewer () const
{
	if (getPrivateViewer () not_eq X3DConstants::X3DBaseNode)
	   return getPrivateViewer ();

	return getViewerType ();
}

void
X3DNavigationContext::set_initialized ()
{
	getWorld () -> getActiveLayer () .addInterest (this, &X3DNavigationContext::set_activeLayer);

	set_activeLayer ();
}

void
X3DNavigationContext::set_activeLayer ()
{
	if (activeLayer not_eq getWorld () -> getActiveLayer ())
	{
		if (activeLayer)
		{
			activeLayer -> getNavigationInfoStack () -> removeInterest (this, &X3DNavigationContext::set_navigationInfo);
			activeLayer -> getViewpointStack ()      -> removeInterest (this, &X3DNavigationContext::set_viewpoint);
		}

		activeLayer = getWorld () -> getActiveLayer ();

		if (activeLayer)
		{
			activeLayer -> getNavigationInfoStack () -> addInterest (this, &X3DNavigationContext::set_navigationInfo);
			activeLayer -> getViewpointStack ()      -> addInterest (this, &X3DNavigationContext::set_viewpoint);
		}

		set_navigationInfo ();
		set_viewpoint ();
	}
}

void
X3DNavigationContext::set_navigationInfo ()
{
	if (activeNavigationInfo)
	{
		activeNavigationInfo -> disposed ()            .removeInterest (this, &X3DNavigationContext::remove_navigationInfo);
		activeNavigationInfo -> getViewer ()           .removeInterest (viewer);
		activeNavigationInfo -> getAvailableViewers () .removeInterest (availableViewers);
	}

	activeNavigationInfo       = activeLayer ? activeLayer -> getNavigationInfo () : nullptr;
	activeNavigationInfoOutput = getCurrentTime ();

	if (activeNavigationInfo)
	{
		activeNavigationInfo -> disposed ()            .addInterest (this, &X3DNavigationContext::remove_navigationInfo);
		activeNavigationInfo -> getViewer ()           .addInterest (viewer);
		activeNavigationInfo -> getAvailableViewers () .addInterest (availableViewers);

		viewer           = activeNavigationInfo -> getViewer ();
		availableViewers = activeNavigationInfo -> getAvailableViewers ();
	}
	else
	{
		viewer = X3DConstants::NoneViewer;
		availableViewers .clear ();
	}
}

void
X3DNavigationContext::remove_navigationInfo ()
{
	activeNavigationInfo       = nullptr;
	activeNavigationInfoOutput = getCurrentTime ();

	viewer = X3DConstants::NoneViewer;
	availableViewers .clear ();
}

void
X3DNavigationContext::set_viewpoint ()
{
	activeViewpointOutput = getCurrentTime ();
}

X3DNavigationContext::~X3DNavigationContext ()
{ }

} // X3D
} // titania
