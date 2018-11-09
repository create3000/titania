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

#include "NavigationInfo.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/LightContainer.h"
#include "../Layering/X3DLayerNode.h"
#include "../Lighting/DirectionalLight.h"
#include "../Navigation/X3DViewpointNode.h"

#include <limits>

namespace titania {
namespace X3D {

const ComponentType NavigationInfo::component      = ComponentType::NAVIGATION;
const std::string   NavigationInfo::typeName       = "NavigationInfo";
const std::string   NavigationInfo::containerField = "children";

NavigationInfo::Fields::Fields () :
	              type (new MFString ({ "EXAMINE", "ANY" })),
	        avatarSize (new MFFloat ({ 0.25, 1.6, 0.75 })),
	             speed (new SFFloat (1)),
	         headlight (new SFBool (true)),
	   visibilityLimit (new SFFloat ()),
	    transitionType (new MFString ({ "LINEAR" })),
	    transitionTime (new SFTime (1)),
	transitionComplete (new SFBool ()),
	   transitionStart (),
	  transitionActive ()
{ }

NavigationInfo::NavigationInfo (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	 X3DBindableNode (),
	          fields (),
	          viewer (X3DConstants::ExamineViewer),
	availableViewers ()
{
	addType (X3DConstants::NavigationInfo);

	addField (inputOutput, "metadata",           metadata ());
	addField (inputOnly,   "set_bind",           set_bind ());
	addField (inputOutput, "type",               type ());
	addField (inputOutput, "avatarSize",         avatarSize ());
	addField (inputOutput, "speed",              speed ());
	addField (inputOutput, "headlight",          headlight ());
	addField (inputOutput, "visibilityLimit",    visibilityLimit ());
	addField (inputOutput, "transitionType",     transitionType ());
	addField (inputOutput, "transitionTime",     transitionTime ());
	addField (outputOnly,  "transitionComplete", transitionComplete ());
	addField (outputOnly,  "isBound",            isBound ());
	addField (outputOnly,  "bindTime",           bindTime ());

	addChildObjects (transitionStart (),
	                 transitionActive (),
	                 viewer,
	                 availableViewers);

	avatarSize ()      .setUnit (UnitCategory::LENGTH);
	speed ()           .setUnit (UnitCategory::SPEED);
	visibilityLimit () .setUnit (UnitCategory::SPEED);

}

X3DBaseNode*
NavigationInfo::create (X3DExecutionContext* const executionContext) const
{
	return new NavigationInfo (executionContext);
}

void
NavigationInfo::initialize ()
{
	X3DBindableNode::initialize ();

	type ()               .addInterest (&NavigationInfo::set_type,               this);
	transitionStart ()    .addInterest (&NavigationInfo::set_transitionStart,    this);
	transitionComplete () .addInterest (&NavigationInfo::set_transitionComplete, this);
	isBound ()            .addInterest (&NavigationInfo::set_isBound,            this);

	set_type ();
}
double
NavigationInfo::getCollisionRadius () const
{
	if (avatarSize () .size () > 0)
	{
		if (avatarSize () [0] > 0)
			return avatarSize () [0];
	}

	return 0.25;
}

double
NavigationInfo::getAvatarHeight () const
{
	if (avatarSize () .size () > 1)
		return avatarSize () [1];

	return 1.6;
}

double
NavigationInfo::getStepHeight () const
{
	if (avatarSize () .size () > 2)
		return avatarSize () [2];

	return 0.75;
}

double
NavigationInfo::getNearValue () const
{
	const double nearValue = getCollisionRadius ();

	if (nearValue == 0)
		return std::numeric_limits <float>::epsilon () * 100;

	else
		return nearValue / 2;
}

double
NavigationInfo::getFarValue (const X3DViewpointNode* const viewpoint) const
{
	return visibilityLimit () ? visibilityLimit () : viewpoint -> getMaxFarValue ();
}

TransitionType
NavigationInfo::getTransitionType () const
{
	static const std::map <std::string, TransitionType> transitionTypes = {
		std::make_pair ("TELEPORT", TransitionType::TELEPORT),
		std::make_pair ("LINEAR",   TransitionType::LINEAR),
		std::make_pair ("ANIMATE",  TransitionType::ANIMATE)
	};

	for (const auto & type : transitionType ())
	{
		try
		{
			return transitionTypes .at (type);
		}
		catch (const std::out_of_range &)
		{ }
	}

	return TransitionType::LINEAR;
}

void
NavigationInfo::set_type ()
{
	static const std::map <std::string, X3DConstants::NodeType> viewerTypes = {
		std::make_pair ("EXAMINE",               X3DConstants::ExamineViewer),
		std::make_pair ("WALK",                  X3DConstants::WalkViewer),
		std::make_pair ("FLY",                   X3DConstants::FlyViewer),
		std::make_pair ("PLANE",                 X3DConstants::PlaneViewer),
		std::make_pair ("PLANE_create3000.de",   X3DConstants::PlaneViewer),
		std::make_pair ("PLANE3D",               X3DConstants::PlaneViewer3D),
		std::make_pair ("PLANE3D_create3000.de", X3DConstants::PlaneViewer3D),
		std::make_pair ("NONE",                  X3DConstants::NoneViewer),
		std::make_pair ("LOOKAT",                X3DConstants::LookAtViewer)
	};

	availableViewers .clear ();

	bool examineViewer = false;
	bool walkViewer    = false;
	bool flyViewer     = false;
	bool planeViewer   = false;
	bool planeViewer3D = false;
	bool noneViewer    = false;
	bool lookAt        = false;

	// Determine active viewer.

	viewer = X3DConstants::ExamineViewer;

	for (const auto & string : type ())
	{
		try
		{
			const auto viewerType = viewerTypes .at (string .get ());

			switch (viewer)
			{
				case X3DConstants::NodeType::LookAtViewer:
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

	if (type () .empty ())
	{
		examineViewer = true;
		walkViewer    = true;
		flyViewer     = true;
		planeViewer   = true;
		planeViewer3D = true;
		noneViewer    = true;
		lookAt        = true;
	}
	else
	{
		for (const auto & string : type ())
		{
			const auto viewer = viewerTypes .find (string .get ());

			if (viewer not_eq viewerTypes .end ())
			{
				switch (viewer -> second)
				{
					case X3DConstants::ExamineViewer:
						examineViewer = true;
						continue;
					case X3DConstants::WalkViewer:
						walkViewer = true;
						continue;
					case X3DConstants::FlyViewer:
						flyViewer = true;
						continue;
					case X3DConstants::PlaneViewer:
						planeViewer = true;
						continue;
					case X3DConstants::PlaneViewer3D:
						planeViewer3D = true;
						continue;
					case X3DConstants::NoneViewer:
						noneViewer = true;
						continue;
					case X3DConstants::LookAtViewer:
						lookAt = true;
						continue;
					default:
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
				planeViewer3D = true;
				noneViewer    = true;
				lookAt        = true;

				// Leave for loop.
				break;
			}

			// Some string defaults to EXAMINE.
			examineViewer = true;
		}
	}

	if (examineViewer)
		availableViewers .emplace_back (X3DConstants::ExamineViewer);

	if (walkViewer)
		availableViewers .emplace_back (X3DConstants::WalkViewer);

	if (flyViewer)
		availableViewers .emplace_back (X3DConstants::FlyViewer);

	if (planeViewer)
		availableViewers .emplace_back (X3DConstants::PlaneViewer);

	if (planeViewer3D)
		availableViewers .emplace_back (X3DConstants::PlaneViewer3D);

	if (noneViewer)
		availableViewers .emplace_back (X3DConstants::NoneViewer);

	if (lookAt)
	{
		if (availableViewers .empty ())
		{
			viewer = X3DConstants::NoneViewer;

			availableViewers .emplace_back (X3DConstants::NoneViewer);
		}

		availableViewers .emplace_back (X3DConstants::LookAtViewer);
	}
}

void
NavigationInfo::set_transitionStart ()
{
	if (not transitionActive ())
		transitionActive () = true;
}

void
NavigationInfo::set_transitionComplete ()
{
	if (transitionActive ())
		transitionActive () = false;
}

void
NavigationInfo::set_isBound ()
{
	if (isBound ())
		return;

	if (transitionActive ())
		transitionActive () = false;
}

void
NavigationInfo::bindToLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfoStack () -> pushOnTop (this);
}

void
NavigationInfo::unbindFromLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfoStack () -> pop (this);
}

void
NavigationInfo::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfoStack () -> erase (this);
}

void
NavigationInfo::enable (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (type not_eq TraverseType::DISPLAY)
		return;

	if (headlight ())
	{
		const auto & lightContainer = renderObject -> getBrowser () -> getHeadlight ();

		renderObject -> getGlobalLights () .emplace_back (lightContainer);
	}
}

void
NavigationInfo::disable ()
{ }

void
NavigationInfo::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::CAMERA:
		{
			renderObject -> getLayer () -> getNavigationInfos () -> pushBack (this);
			break;
		}
		default:
			break;
	}
}

NavigationInfo::~NavigationInfo ()
{ }

} // X3D
} // titania
