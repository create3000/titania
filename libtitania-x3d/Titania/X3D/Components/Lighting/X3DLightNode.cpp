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

#include "X3DLightNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/LightContainer.h"

#include "../Layering/X3DLayerNode.h"

namespace titania {
namespace X3D {

X3DLightNode::Fields::Fields () :
	          global (new SFBool (true)),
	              on (new SFBool (true)),
	           color (new SFColor (1, 1, 1)),
	       intensity (new SFFloat (1)),
	ambientIntensity (new SFFloat ()),

	     shadowColor (new SFColor (0, 0, 0)), // Color of shadow.
	 shadowIntensity (new SFFloat (0)),       // Intensity of shadow color in the range (0, 1).
	 shadowDiffusion (new SFFloat (0)),       // Diffusion of the shadow in length units in the range (0, inf).
	   shadowMapSize (new SFInt32 (1024))     // Size of the shadow map in pixels in the range (0, inf).
{ }

X3DLightNode::X3DLightNode () :
	           X3DChildNode (),
	                 fields ()
{
	addType (X3DConstants::X3DLightNode);

	shadowDiffusion () .setUnit (UnitCategory::LENGTH);
}

const Color3f &
X3DLightNode::getColor () const
{
	return color () .getValue ();
}

float
X3DLightNode::getAmbientIntensity () const
{
	return math::clamp <float> (ambientIntensity (), 0, 1);
}

float
X3DLightNode::getIntensity () const
{
	return math::clamp <float> (intensity (), 0, 1);
}

const Color3f &
X3DLightNode::getShadowColor () const
{
	return shadowColor () .getValue ();
}

float
X3DLightNode::getShadowIntensity () const
{
	return math::clamp <float> (shadowIntensity (), 0, 1);
}

float
X3DLightNode::getShadowDiffusion () const
{
	return std::max <float> (shadowDiffusion (), 0);
}

size_t
X3DLightNode::getShadowMapSize () const
{
	return std::min <int32_t> (shadowMapSize (), getBrowser () -> getMaxTextureSize ());
}

void
X3DLightNode::push (X3DGroupingNode* const group)
{
	if (on ())
	{
		if (global ())
		{
			const auto lightContainer = std::make_shared <LightContainer> (this, getCurrentLayer () -> getGroup ());

			getCurrentLayer () -> getGlobalLights () .emplace_back (lightContainer);
			getCurrentLayer () -> getLights ()       .emplace_back (lightContainer);
		}
		else
		{
			const auto lightContainer = std::make_shared <LightContainer> (this, group);

			getCurrentLayer () -> getLocalLights () .emplace_back (lightContainer);
			getCurrentLayer () -> getLights ()      .emplace_back (lightContainer);
		}
	}
}

void
X3DLightNode::pop ()
{
	if (on () and not global ())
		getCurrentLayer () -> getLocalLights () .pop_back ();
}

} // X3D
} // titania
