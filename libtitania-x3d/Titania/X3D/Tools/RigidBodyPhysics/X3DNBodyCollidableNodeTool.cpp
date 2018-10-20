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

#include "X3DNBodyCollidableNodeTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Components/Grouping/Transform.h"
#include "../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

X3DNBodyCollidableNodeTool::X3DNBodyCollidableNodeTool (const Color3f & color) :
	        X3DNBodyCollidableNode (),
	              X3DChildNodeTool (),
	X3DTransformMatrix3DObjectTool (),
	          X3DBoundedObjectTool (color),
                            color (color),
                 startTranslation (),
                    startRotation ()
{
	//addType (X3DConstants::X3DNBodyCollidableNodeTool);
}

void
X3DNBodyCollidableNodeTool::initialize ()
{
	X3DChildNodeTool::initialize ();
	X3DTransformMatrix3DObjectTool::initialize ();

	requestAsyncLoad ({ get_tool ("NBodyCollidableNodeTool.x3dv") .str () });
}

void
X3DNBodyCollidableNodeTool::realize ()
{
	// Configure TransformTool.

	setTransformTool (0, getInlineNode () -> getExportedNode <Transform> ("TransformTool"));

	const auto & transformTool = getTransformTools () [0];

	transformTool -> grouping ()      = false;
	transformTool -> displayCenter () = false;
	transformTool -> tools ()         = { "TRANSLATE", "ROTATE" };
	transformTool -> color ()         = color;

	translation () .addInterest (transformTool -> translation ());
	rotation ()    .addInterest (transformTool -> rotation ());

	transformTool -> translation () .addInterest (translation ());
	transformTool -> rotation ()    .addInterest (rotation ());

	transformTool -> translation () = translation ();
	transformTool -> rotation ()    = rotation ();

	// Enabled

	getNode <X3DNBodyCollidableNode> () -> enabled () .addInterest (&X3DNBodyCollidableNodeTool::set_enabled, this);

	set_enabled (getNode <X3DNBodyCollidableNode> () -> enabled ());
}

void
X3DNBodyCollidableNodeTool::set_enabled (const bool value)
{
	setLinetype (value ? LineType::SOLID : LineType::DOTTED);
}

void
X3DNBodyCollidableNodeTool::beginUndo ()
{
	startTranslation = translation ();
	startRotation    = rotation ();
}

void
X3DNBodyCollidableNodeTool::endUndo (const UndoStepPtr & undoStep)
{
	if (translation () not_eq startTranslation or
	    rotation () not_eq startRotation)
	{
		undoStep -> addUndoFunction (&SFVec3f::setValue, std::ref (translation ()), startTranslation);
		undoStep -> addUndoFunction (&SFRotation::setValue, std::ref (rotation ()), startRotation);
		undoStep -> addUndoFunction (&X3DNBodyCollidableNodeTool::setChanging, X3DPtr <X3DNBodyCollidableNodeTool> (this), 0, true);

		undoStep -> addRedoFunction (&X3DNBodyCollidableNodeTool::setChanging, X3DPtr <X3DNBodyCollidableNodeTool> (this), 0, true);
		undoStep -> addRedoFunction (&SFVec3f::setValue, std::ref (translation ()), translation ());
		undoStep -> addRedoFunction (&SFRotation::setValue, std::ref (rotation ()), rotation ());
	}
}

void
X3DNBodyCollidableNodeTool::reshape (X3DRenderObject* const renderObject)
{
	try
	{
		if (getTransformTools () .empty ())
			return;

		const auto & transformTool = getTransformTools () [0];
		const auto   bbox          = getChildBBox ();

		transformTool -> setField <SFVec3f> ("bboxSize",   bbox .size (),   true);
		transformTool -> setField <SFVec3f> ("bboxCenter", bbox .center (), true);
	}
	catch (const X3DError & error)
	{ }
}

void
X3DNBodyCollidableNodeTool::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	getNode <X3DNBodyCollidableNode> () -> traverse (type, renderObject);
	
	switch (type)
	{
		case TraverseType::POINTER:
		case TraverseType::DISPLAY:
		{
			// Last chance to process events
			reshape (renderObject);
			break;
		}
		default:
			break;
	}

	X3DToolObject::traverse (type, renderObject);
}

void
X3DNBodyCollidableNodeTool::dispose ()
{
	X3DTransformMatrix3DObjectTool::dispose ();
	X3DChildNodeTool::dispose ();
}

X3DNBodyCollidableNodeTool::~X3DNBodyCollidableNodeTool ()
{ }

} // X3D
} // titania
