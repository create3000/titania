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

#include "../Navigation/ExamineViewer.h"
#include "../Navigation/FlyViewer.h"
#include "../Navigation/LookAtViewer.h"
#include "../Navigation/NoneViewer.h"
#include "../Navigation/PlaneViewer.h"
#include "../Navigation/WalkViewer.h"
#include "../Navigation/X3DViewer.h"
#include "../Tools/LightSaber.h"
#include "../Tools/LassoSelection.h"
#include "../Tools/RectangleSelection.h"

#include "../Selection.h"
#include "../X3DBrowser.h"

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Components/Lighting/DirectionalLight.h"
#include "../../Components/Navigation/NavigationInfo.h"
#include "../../Components/Navigation/X3DViewpointNode.h"
#include "../../Components/Rendering/X3DGeometryNode.h"

#include "../../Editing/X3DEditor.h"
#include "../../Execution/World.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/LightContainer.h"

// First include X3DExecutionContext
#include "../../Execution/BindableNodeStack.h"

namespace titania {
namespace X3D {

X3DNavigationContext::X3DNavigationContext () :
	               X3DBaseNode (),
	             headlightNode (new DirectionalLight (getExecutionContext ())),
	        headlightContainer (std::make_shared <LightContainer> (getBrowser (), headlightNode, nullptr, Matrix4d ())),
	               activeLayer (),
	      activeNavigationInfo (),
	                    viewer (X3DConstants::NoneViewer),
	             privateViewer (X3DConstants::DefaultViewer),
	          availableViewers (),
	               viewerNode  (new NoneViewer (getBrowser ())),
	           activeViewpoint (),
	         straightenHorizon (),
	          activeCollisions ()
{
	addChildObjects (headlightNode,
	                 activeLayer,
	                 activeNavigationInfo,
	                 viewer,
	                 privateViewer,
	                 availableViewers,
	                 viewerNode,
	                 activeViewpoint,
	                 straightenHorizon);
}

void
X3DNavigationContext::initialize ()
{
	getBrowser () -> initialized () .addInterest (&X3DNavigationContext::set_initialized,      this);
	getActiveNavigationInfo () .addInterest (&X3DNavigationContext::set_active_navigationInfo, this);
	getViewerType ()           .addInterest (&X3DNavigationContext::set_viewer,                this);
	getPrivateViewer ()        .addInterest (&X3DNavigationContext::set_viewer,                this);

	headlightNode -> setup ();
	viewerNode    -> setup ();
}

X3D::X3DConstants::NodeType
X3DNavigationContext::getCurrentViewer () const
{
	if (getPrivateViewer () not_eq X3DConstants::DefaultViewer)
	   return getPrivateViewer ();

	return getViewerType ();
}

void
X3DNavigationContext::lookAtSelection ()
{
	if (not getActiveLayer ())
		return;

	const auto & selection = getBrowser () -> getSelection () -> getNodes ();

	if (selection .empty ())
		return;

	X3D::Box3d bbox;

	for (const auto & node : selection)
	{
		const auto modelMatrix   = X3D::X3DEditor::getModelMatrix (getBrowser () -> getExecutionContext (), node);
		const auto boundedObject = X3D::x3d_cast <X3D::X3DBoundedObject*> (node);

		if (boundedObject)
			bbox += boundedObject -> getBBox () * modelMatrix;

		const auto geometryNode = X3D::x3d_cast <X3D::X3DGeometryNode*> (node);

		if (geometryNode)
			bbox += geometryNode -> getBBox () * modelMatrix;
	}

	getActiveLayer () -> getViewpoint () -> lookAt (bbox);
}

void
X3DNavigationContext::lookAtAllObjectsInActiveLayer ()
{
	if (getActiveLayer ())
		getActiveLayer () -> lookAt ();
}

void
X3DNavigationContext::set_initialized ()
{
	getBrowser () -> getWorld () -> getActiveLayer () .addInterest (&X3DNavigationContext::set_activeLayer, this);

	set_activeLayer ();
}

void
X3DNavigationContext::set_activeLayer ()
{
	if (activeLayer not_eq getBrowser () -> getWorld () -> getActiveLayer ())
	{
		if (activeLayer)
		{
			activeLayer -> getNavigationInfoStack () -> removeInterest (&X3DNavigationContext::set_navigationInfo, this);
			activeLayer -> getViewpointStack ()      -> removeInterest (&X3DNavigationContext::set_viewpoint, this);
		}

		activeLayer = getBrowser () -> getWorld () -> getActiveLayer ();

		if (activeLayer)
		{
			activeLayer -> getNavigationInfoStack () -> addInterest (&X3DNavigationContext::set_navigationInfo, this);
			activeLayer -> getViewpointStack ()      -> addInterest (&X3DNavigationContext::set_viewpoint, this);
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
		activeNavigationInfo -> getViewer ()           .removeInterest (viewer);
		activeNavigationInfo -> getAvailableViewers () .removeInterest (availableViewers);
	}

	if (activeLayer)
		activeNavigationInfo .setValue (activeLayer -> getNavigationInfo ());
	else
		activeNavigationInfo = nullptr;

	if (activeNavigationInfo)
	{
		activeNavigationInfo -> getViewer ()           .addInterest (viewer);
		activeNavigationInfo -> getAvailableViewers () .addInterest (availableViewers);

		viewer           = activeNavigationInfo -> getViewer ();
		availableViewers = activeNavigationInfo -> getAvailableViewers ();
	}
}

void
X3DNavigationContext::set_active_navigationInfo ()
{
	if (activeNavigationInfo)
		return;

	viewer           = X3DConstants::NoneViewer;
	availableViewers = { X3DConstants::NoneViewer };
}

void
X3DNavigationContext::set_viewpoint ()
{
	if (activeLayer)
		activeViewpoint .setValue (activeLayer -> getViewpoint ());
	else
		activeViewpoint = nullptr;
}

void
X3DNavigationContext::set_viewer ()
{
	const auto type = getCurrentViewer ();

	getBrowser () -> setCursor ("ARROW");

	switch (type)
	{
		case X3DConstants::NoneViewer:
		{
			viewerNode .setValue (new NoneViewer (getBrowser ()));
			break;
		}
		case X3DConstants::ExamineViewer:
		{
			viewerNode .setValue (new ExamineViewer (getBrowser ()));
			break;
		}
		case X3DConstants::WalkViewer:
		{
			viewerNode .setValue (new WalkViewer (getBrowser ()));
			break;
		}
		case X3DConstants::FlyViewer:
		{
			viewerNode .setValue (new FlyViewer (getBrowser ()));
			break;
		}
		case X3DConstants::PlaneViewer:
		{
			viewerNode .setValue (new PlaneViewer (getBrowser (), X3DConstants::PlaneViewer));
			break;
		}
		case X3DConstants::PlaneViewer3D:
		{
			viewerNode .setValue (new PlaneViewer (getBrowser (), X3DConstants::PlaneViewer3D));
			break;
		}
		case X3DConstants::LookAtViewer:
		{
			viewerNode .setValue (new LookAtViewer (getBrowser ()));
			break;
		}
		case X3DConstants::RectangleSelection:
		{
			viewerNode .setValue (new RectangleSelection (getBrowser ()));
			break;
		}
		case X3DConstants::LassoSelection:
		{
			viewerNode .setValue (new LassoSelection (getBrowser ()));
			break;
		}
		case X3DConstants::LightSaber:
		{
			viewerNode .setValue (new LightSaber (getBrowser ()));
			break;
		}
		default:
			viewerNode .setValue (new NoneViewer (getBrowser ()));
			break;
	}

	viewerNode -> setup ();
}

X3DNavigationContext::~X3DNavigationContext ()
{ }

} // X3D
} // titania
