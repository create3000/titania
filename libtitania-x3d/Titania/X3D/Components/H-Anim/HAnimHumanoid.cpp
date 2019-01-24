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

#include "HAnimHumanoid.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"

#include "../Grouping/Group.h"
#include "../Grouping/Transform.h"

namespace titania {
namespace X3D {

const std::string HAnimHumanoid::componentName  = "H-Anim";
const std::string HAnimHumanoid::typeName       = "HAnimHumanoid";
const std::string HAnimHumanoid::containerField = "children";

HAnimHumanoid::Fields::Fields () :
	            name (new SFString ()),
	         version (new SFString ()),
	            info (new MFString ()),
	     translation (new SFVec3f ()),
	        rotation (new SFRotation ()),
	           scale (new SFVec3f (1, 1, 1)),
	scaleOrientation (new SFRotation ()),
	          center (new SFVec3f ()),
	      viewpoints (new MFNode ()),
	      skinNormal (new SFNode ()),
	       skinCoord (new SFNode ()),
	            skin (new MFNode ()),
	           sites (new MFNode ()),
	          joints (new MFNode ()),
	        segments (new MFNode ()),
	        skeleton (new MFNode ())
{ }

HAnimHumanoid::HAnimHumanoid (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	              X3DChildNode (),
	          X3DBoundedObject (),
	                    fields (),
	            viewpointsNode (new Group (executionContext)),
	                  skinNode (new Group (executionContext)),
	              skeletonNode (new Group (executionContext)),
	             transformNode (new Transform (executionContext))
{
	addType (X3DConstants::HAnimHumanoid);

	addField (inputOutput,    "metadata",         metadata ());
	addField (inputOutput,    "name",             name ());
	addField (inputOutput,    "version",          version ());
	addField (inputOutput,    "info",             info ());

	addField (inputOutput,    "translation",      translation ());
	addField (inputOutput,    "rotation",         rotation ());
	addField (inputOutput,    "scale",            scale ());
	addField (inputOutput,    "scaleOrientation", scaleOrientation ());
	addField (inputOutput,    "center",           center ());
	addField (initializeOnly, "bboxSize",         bboxSize ());
	addField (initializeOnly, "bboxCenter",       bboxCenter ());

	addField (inputOutput,    "viewpoints",       viewpoints ());
	addField (inputOutput,    "skinNormal",       skinNormal ());
	addField (inputOutput,    "skinCoord",        skinCoord ());
	addField (inputOutput,    "skin",             skin ());
	addField (inputOutput,    "sites",            sites ());
	addField (inputOutput,    "joints",           joints ());
	addField (inputOutput,    "segments",         segments ());
	addField (inputOutput,    "skeleton",         skeleton ());

	addChildObjects (transformNode,
	                 skinNode,
	                 viewpointsNode,
	                 skeletonNode);
}

X3DBaseNode*
HAnimHumanoid::create (X3DExecutionContext* const executionContext) const
{
	return new HAnimHumanoid (executionContext);
}

void
HAnimHumanoid::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	// Groups

	viewpoints () .addInterest (viewpointsNode -> children ());
	skin ()       .addInterest (skinNode       -> children ());
	skeleton ()   .addInterest (skeletonNode   -> children ());

	viewpointsNode -> children () = viewpoints ();
	skinNode       -> children () = skin ();
	skeletonNode   -> children () = skeleton ();

	// Transform

	translation ()      .addInterest (transformNode -> translation ());
	rotation ()         .addInterest (transformNode -> rotation ());
	scale ()            .addInterest (transformNode -> scale ());
	scaleOrientation () .addInterest (transformNode -> scaleOrientation ());
	center ()           .addInterest (transformNode -> center ());
	bboxSize ()         .addInterest (transformNode -> bboxSize ());
	bboxCenter ()       .addInterest (transformNode -> bboxCenter ());

	transformNode -> translation ()      .addInterest (translation ());
	transformNode -> rotation ()         .addInterest (rotation ());
	transformNode -> scale ()            .addInterest (scale ());
	transformNode -> scaleOrientation () .addInterest (scaleOrientation ());
	transformNode -> center ()           .addInterest (center ());
	transformNode -> bboxSize ()         .addInterest (bboxSize ());
	transformNode -> bboxCenter ()       .addInterest (bboxCenter ());

	transformNode -> translation ()      = translation ();
	transformNode -> rotation ()         = rotation ();
	transformNode -> scale ()            = scale ();
	transformNode -> scaleOrientation () = scaleOrientation ();
	transformNode -> center ()           = center ();
	transformNode -> bboxSize ()         = bboxSize ();
	transformNode -> bboxCenter ()       = bboxCenter ();
	transformNode -> children ()         = { viewpointsNode, skinNode, skeletonNode };

	transformNode -> isCameraObject () .addInterest (&HAnimHumanoid::setCameraObject, static_cast <X3DChildNode*> (this));

	// Setup

	viewpointsNode -> setup ();
	skinNode       -> setup ();
	skeletonNode   -> setup ();
	transformNode  -> setup ();
}

Box3d
HAnimHumanoid::getBBox () const
{
	return transformNode -> getBBox ();
}

void
HAnimHumanoid::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	transformNode -> traverse (type, renderObject);
}

void
HAnimHumanoid::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

HAnimHumanoid::~HAnimHumanoid ()
{ }

} // X3D
} // titania
