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

#include "X3DToolContext.h"

#include "TransformToolOptions.h"

#include "../X3DBrowser.h"

#include "../../Tools/EnvironmentalSensor/ProximitySensorTool.h"
#include "../../Tools/EnvironmentalSensor/TransformSensorTool.h"
#include "../../Tools/EnvironmentalSensor/VisibilitySensorTool.h"
#include "../../Tools/Grouping/X3DTransformNodeTool.h"
#include "../../Tools/Lighting/X3DLightNodeTool.h"
#include "../../Tools/Navigation/X3DViewpointNodeTool.h"
#include "../../Tools/SnapTool/SnapTargetTool.h"
#include "../../Tools/SnapTool/SnapSourceTool.h"
#include "../../Tools/Sound/SoundTool.h"

namespace titania {
namespace X3D {

X3DToolContext::X3DToolContext () :
	          X3DBaseNode (),
	       supportedTools (),
	         displayTools ({ true }),
	 transformToolOptions (new TransformToolOptions (getExecutionContext ())),
	       transformTools (),
	           lightTools (),
	 proximitySensorTools (),
	           soundTools (),
	 transformSensorTools (),
	visibilitySensorTools (),
	       viewpointTools (),
	           snapTarget (),
	           snapSource (),
	              cutLine ()
{
	addChildObjects (transformToolOptions,
	                 transformTools,
	                 lightTools,
	                 proximitySensorTools,
	                 soundTools,
	                 transformSensorTools,
	                 visibilitySensorTools,
	                 viewpointTools,
	                 snapTarget,
	                 snapSource);
}

void
X3DToolContext::initialize ()
{
	transformToolOptions -> setup ();
}

const SupportedTools::Function &
X3DToolContext::getSupportedTool (const std::string & typeName) const
throw (Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	return supportedTools .getTool (typeName);
}

void
X3DToolContext::addTransformTool (X3DTransformNodeTool* const node)
{
	transformTools .emplace_back (node);
}

void
X3DToolContext::removeTransformTool (X3DTransformNodeTool* const node)
{
	transformTools .remove (X3DWeakPtr <X3DTransformNodeTool> (node));
	transformTools .remove (nullptr);
}

void
X3DToolContext::addLightTool (X3DLightNodeTool* const node)
{
	lightTools .emplace_back (node);
}

void
X3DToolContext::removeLightTool (X3DLightNodeTool* const node)
{
	lightTools .remove (X3DWeakPtr <X3DLightNodeTool> (node));
	lightTools .remove (nullptr);
}

void
X3DToolContext::addProximitySensorTool (ProximitySensorTool* const node)
{
	proximitySensorTools .emplace_back (node);
}

void
X3DToolContext::removeProximitySensorTool (ProximitySensorTool* const node)
{
	proximitySensorTools .remove (X3DWeakPtr <ProximitySensorTool> (node));
	proximitySensorTools .remove (nullptr);
}

void
X3DToolContext::addSoundTool (SoundTool* const node)
{
	soundTools .emplace_back (node);
}

void
X3DToolContext::removeSoundTool (SoundTool* const node)
{
	soundTools .remove (X3DWeakPtr <SoundTool> (node));
	soundTools .remove (nullptr);
}

void
X3DToolContext::addTransformSensorTool (TransformSensorTool* const node)
{
	transformSensorTools .emplace_back (node);
}

void
X3DToolContext::removeTransformSensorTool (TransformSensorTool* const node)
{
	transformSensorTools .remove (X3DWeakPtr <TransformSensorTool> (node));
	transformSensorTools .remove (nullptr);
}

void
X3DToolContext::addVisibilitySensorTool (VisibilitySensorTool* const node)
{
	visibilitySensorTools .emplace_back (node);
}

void
X3DToolContext::removeVisibilitySensorTool (VisibilitySensorTool* const node)
{
	visibilitySensorTools .remove (X3DWeakPtr <VisibilitySensorTool> (node));
	visibilitySensorTools .remove (nullptr);
}

void
X3DToolContext::addViewpointTool (X3DViewpointNodeTool* const node)
{
	viewpointTools .emplace_back (node);
}

void
X3DToolContext::removeViewpointTool (X3DViewpointNodeTool* const node)
{
	viewpointTools .remove (X3DWeakPtr <X3DViewpointNodeTool> (node));
	viewpointTools .remove (nullptr);
}

const X3DPtr <SnapTargetTool> &
X3DToolContext::getSnapTarget () const
{
	if (not snapTarget)
	{
		const_cast <X3DToolContext*> (this) -> snapTarget .setValue (new SnapTargetTool (getExecutionContext ()));

		snapTarget -> enabled () = false;
		snapTarget -> setup ();
	}

	return snapTarget;
}

const X3DPtr <SnapSourceTool> &
X3DToolContext::getSnapSource () const
{
	if (not snapSource)
	{
		const_cast <X3DToolContext*> (this) -> snapSource .setValue (new SnapSourceTool (getExecutionContext ()));

		snapSource -> enabled () = false;
		snapSource -> setup ();
	}

	return snapSource;
}

void
X3DToolContext::dispose ()
{ }

X3DToolContext::~X3DToolContext ()
{ }

} // X3D
} // titania
