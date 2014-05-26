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

#include "X3DBindableNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../Layering/X3DLayerNode.h"

#include <iostream>

namespace titania {
namespace X3D {

X3DBindableNode::Fields::Fields () :
	set_bind (new SFBool ()),
	 isBound (new SFBool ()),
	bindTime (new SFTime ())
{ }

X3DBindableNode::X3DBindableNode () :
	X3DChildNode (),
	      fields (),
	      layers (),
	        live (true),
	    wasBound (false)
{
	addType (X3DConstants::X3DBindableNode);
}

void
X3DBindableNode::initialize ()
{
	X3DChildNode::initialize ();

	isLive ()   .addInterest (this, &X3DBindableNode::set_live);
	set_bind () .addInterest (this, &X3DBindableNode::_set_bind);
}

void 
X3DBindableNode::addLayer (X3DLayerNode* const layer)
{
	layers .emplace_back (layer);
	
	layer -> shutdown () .addInterest (this, &X3DBindableNode::removeLayer, layer);
}

void 
X3DBindableNode::removeLayer (X3DLayerNode* const layer)
{
	const auto iter = std::find (layers .begin (), layers .end (), layer);

	if (iter not_eq layers .end ())
		layers .erase (iter);
}

void
X3DBindableNode::set_live ()
{
	if (isLive () == live)
		return;

	live = isLive ();

	if (live)
	{
		if (wasBound)
		{
			for (const auto & layer : layers)
				bindToLayer (layer);
		}
	}
	else
	{
		wasBound = isBound ();

		for (const auto & layer : layers)
			removeFromLayer (layer);
	}
}

void
X3DBindableNode::_set_bind ()
{
	if (set_bind ())
	{
		// Save layers

		for (const auto & layer : layers)
			layer -> shutdown () .removeInterest (this, &X3DBindableNode::removeLayer);

		layers = getLayers ();

		for (const auto & layer : layers)
			layer -> shutdown () .addInterest (this, &X3DBindableNode::removeLayer, layer);

		// Bind

		for (const auto & layer : layers)
			bindToLayer (layer);
	}
	else
	{
		// Unbind

		for (const auto & layer : layers)
			unbindFromLayer (layer);
	}
}

} // X3D
} // titania
