/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layering/Layer.h"
#include "LayerSet.h"

namespace titania {
namespace X3D {

LayerSet::LayerSet (X3DExecutionContext* const executionContext) :
	X3DBasicNode (executionContext -> getBrowser (), executionContext),
	     X3DNode (),                                 
	 activeLayer (),                                 // SFInt32 [in,out] activeLayer  0          (-∞,∞)
	       order ({ 0 }),                            // MFInt32 [in,out] order        [0]        (0,∞)
	      layers (),                                 // MFNode  [in,out] layers       [ ]        [X3DLayerNode]
	    children ({ new Layer (executionContext) })
{
	setComponent ("Layering");
	setTypeName ("LayerSet");

	appendField (inputOutput, "metadata",    metadata);
	appendField (inputOutput, "activeLayer", activeLayer);
	appendField (inputOutput, "order",       order);
	appendField (inputOutput, "layers",      layers);

	setChildren (children);

	children [0] -> getActiveBackground () -> transparency = 0; // XXX
}

X3DBasicNode*
LayerSet::create (X3DExecutionContext* const executionContext) const
{
	return new LayerSet (executionContext);
}

Box3f
LayerSet::getBBox ()
{
	return getActiveLayer () -> getBBox ();                     // XXX get bbox from all layers;
}

const SFNode <X3DLayerNode>
LayerSet::getActiveLayer () const
{
	if (activeLayer >= 0 and activeLayer < (int32_t) children .size ())
		return children [activeLayer];

	return children [0];
}

const MFNode <X3DLayerNode> &
LayerSet::getLayers ()
{
	return children;
}

void
LayerSet::initialize ()
{
	X3DNode::initialize ();

	children [0] -> setup ();

	activeLayer .addInterest (this, &LayerSet::set_activeLayer);
	layers      .addInterest (this, &LayerSet::set_layers);

	set_activeLayer ();
	set_layers ();
}

void
LayerSet::set_activeLayer ()
{ }

void
LayerSet::set_layers ()
{
	children .resize (1 + layers .size ());
	std::copy (layers .cbegin (), layers .cend (), children .begin () + 1);
}

void
LayerSet::intersect ()
{
	for (const auto & index : order)
	{
		if (index >= 0  and index < (int32_t) children .size ())
		{
			X3DLayerNode* currentLayer = *children .at (index);

			setCurrentLayer (currentLayer);

			currentLayer -> select ();
		}
	}
}

void
LayerSet::draw ()
{
	for (const auto & index : order)
	{
		if (index >= 0  and index < (int32_t) children .size ())
		{
			X3DLayerNode* currentLayer = *children .at (index);

			setCurrentLayer (currentLayer);

			currentLayer -> display ();
		}
	}
}

void
LayerSet::dispose ()
{
	X3DNode::dispose ();
}

} // X3D
} // titania
