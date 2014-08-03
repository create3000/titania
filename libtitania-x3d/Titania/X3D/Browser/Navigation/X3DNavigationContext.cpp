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

#include "X3DNavigationContext.h"

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Execution/World.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../X3DBrowser.h"

// First include X3DExecutionContext
#include "../../Execution/BindableNodeStack.h"

namespace titania {
namespace X3D {

X3DNavigationContext::X3DNavigationContext () :
	               X3DBaseNode (),
	               activeLayer (),
	      activeNavigationInfo (nullptr),
	activeNavigationInfoOutput (),
	                    viewer (ViewerType::NONE),
	          availableViewers (),
	     activeViewpointOutput ()
{
	addChildren (activeLayer,
	             activeNavigationInfoOutput,
	             viewer,
	             availableViewers,
	             activeViewpointOutput);
}

void
X3DNavigationContext::initialize ()
{
	getBrowser () -> initialized () .addInterest (this, &X3DNavigationContext::set_initialized);
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
		activeNavigationInfo -> disposed () .removeInterest (this, &X3DNavigationContext::remove_navigationInfo);
		activeNavigationInfo -> type ()     .removeInterest (this, &X3DNavigationContext::set_navigationInfo_type);
	}

	activeNavigationInfo       = activeLayer ? activeLayer -> getNavigationInfo () : nullptr;
	activeNavigationInfoOutput = getCurrentTime ();

	if (activeNavigationInfo)
	{
		activeNavigationInfo -> disposed () .addInterest (this, &X3DNavigationContext::remove_navigationInfo);
		activeNavigationInfo -> type ()     .addInterest (this, &X3DNavigationContext::set_navigationInfo_type);
	}

	set_navigationInfo_type ();
}

void
X3DNavigationContext::remove_navigationInfo ()
{
	activeNavigationInfo       = nullptr;
	activeNavigationInfoOutput = getCurrentTime ();
	set_navigationInfo_type ();
}

void
X3DNavigationContext::set_viewpoint ()
{
	activeViewpointOutput = getCurrentTime ();
}

void
X3DNavigationContext::set_navigationInfo_type ()
{
	availableViewers .clear ();

	bool examineViewer = false;
	bool walkViewer    = false;
	bool flyViewer     = false;
	bool planeViewer   = false;
	bool noneViewer    = false;
	bool lookAt        = false;

	if (activeNavigationInfo)
	{
		static const std::map <std::string, ViewerType> viewerTypes = {
			std::make_pair ("EXAMINE",             ViewerType::EXAMINE),
			std::make_pair ("WALK",                ViewerType::WALK),
			std::make_pair ("FLY",                 ViewerType::FLY),
			std::make_pair ("PLANE_create3000.de", ViewerType::PLANE),
			std::make_pair ("NONE",                ViewerType::NONE),
			std::make_pair ("LOOKAT",              ViewerType::LOOKAT)
		};

		// Determine active viewer.

		viewer = ViewerType::EXAMINE;

		for (const auto & string : activeNavigationInfo -> type ())
		{
			try
			{
				const auto viewerType = viewerTypes .at (string);

				switch (viewerType)
				{
					case ViewerType::LOOKAT:
						// Continue with next type.
						continue;
					default:
						viewer = viewerType;
						break;
				}

				// Leave for loop.
				break;
			}
			catch (const std::out_of_range &)
			{
				continue;
			}
		}

		// Determine available viewers.

		if (activeNavigationInfo -> type () .empty ())
		{
			examineViewer = true;
			walkViewer    = true;
			flyViewer     = true;
			planeViewer   = true;
			noneViewer    = true;
			lookAt        = true;
		}
		else
		{
			for (const auto & string : activeNavigationInfo -> type ())
			{
				const auto viewerType = viewerTypes .find (string);

				if (viewerType not_eq viewerTypes .end ())
				{
					switch (viewerType -> second)
					{
						case ViewerType::EXAMINE:
							examineViewer = true;
							continue;
						case ViewerType::WALK:
							walkViewer = true;
							continue;
						case ViewerType::FLY:
							flyViewer = true;
							continue;
						case ViewerType::PLANE:
							planeViewer = true;
							continue;
						case ViewerType::NONE:
							noneViewer = true;
							continue;
						case ViewerType::LOOKAT:
							lookAt = true;
							continue;
					}

					// All cases handled continue.
				}

				if (string == "ANY")
				{
					examineViewer = true;
					walkViewer    = true;
					flyViewer     = true;
					planeViewer   = true;
					noneViewer    = true;
					lookAt        = true;

					// Leave for loop.
					break;
				}

				// Some string defaults to EXAMINE.
				examineViewer = true;
			}
		}
	}
	else
	{
		viewer     = ViewerType::NONE;
		noneViewer = true;
	}

	if (examineViewer)
		availableViewers .emplace_back (ViewerType::EXAMINE);

	if (walkViewer)
		availableViewers .emplace_back (ViewerType::WALK);

	if (flyViewer)
		availableViewers .emplace_back (ViewerType::FLY);

	if (planeViewer)
		availableViewers .emplace_back (ViewerType::PLANE);

	if (noneViewer)
		availableViewers .emplace_back (ViewerType::NONE);

	if (lookAt)
	{
		if (availableViewers .empty ())
		{
			viewer = ViewerType::NONE;
			availableViewers .emplace_back (ViewerType::NONE);
		}

		availableViewers .emplace_back (ViewerType::LOOKAT);
	}
}

X3DNavigationContext::~X3DNavigationContext ()
{ }

} // X3D
} // titania
