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

#include "TransformHandle.h"

#include "../Bits/config.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Grouping/Transform.h"
#include "../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

TransformHandle::TransformHandle (Transform* const node, X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	      Transform (executionContext),
	X3DHandleObject (),
	      transform (node),
	          scene ()
{
	for (auto & field : transform -> getFieldDefinitions ())
		addField (field -> getAccessType (), field -> getName (), *field);

	addChildren (transform, scene);
}

void
TransformHandle::initialize ()
{
	Transform::initialize ();
	X3DHandleObject::initialize ();

	try
	{
		scene = getBrowser () -> createX3DFromURL ({ get_handle ("TransformHandle.wrl") .str () });

		X3DFieldDefinition* field = scene -> getNamedNode ("Handle") -> getField ("transform");

		static_cast <SFNode*> (field) -> setValue (transform .getValue ());
	}
	catch (const X3DError &)
	{
		scene = getBrowser () -> createScene ();
	}
}

Box3f
TransformHandle::getBBox ()
{
	return transform -> getBBox ();
}

void
TransformHandle::removeHandle ()
{
	transform -> removeHandle ();
}

void
TransformHandle::reshape ()
{
	try
	{
		auto handle = scene -> getNamedNode ("Handle");
		auto bbox   = getBBox () * ~transform -> getMatrix ();

		try
		{
			const Matrix4f & cameraSpaceMatrix = getCurrentViewpoint () -> getTransformationMatrix ();

			SFMatrix4f & field = *static_cast <SFMatrix4f*> (handle -> getField ("cameraSpaceMatrix"));

			if (field not_eq cameraSpaceMatrix)
				field = cameraSpaceMatrix;
		}
		catch (const X3DError &)
		{ }

		try
		{
			Matrix4f modelViewMatrix = ModelViewMatrix4f ();

			SFMatrix4f & field = *static_cast <SFMatrix4f*> (handle -> getField ("modelViewMatrix"));

			if (field not_eq modelViewMatrix)
				field = modelViewMatrix;
		}
		catch (const X3DError &)
		{ }

		try
		{
			SFVec3f & field = *static_cast <SFVec3f*> (handle -> getField ("bboxSize"));

			if (field not_eq bbox .size ())
				field = bbox .size ();
		}
		catch (const X3DError &)
		{ }

		try
		{
			SFVec3f & field = *static_cast <SFVec3f*> (handle -> getField ("bboxCenter"));

			if (field not_eq bbox .center ())
				field = bbox .center ();
		}
		catch (const X3DError &)
		{ }
	}
	catch (const X3DError &)
	{
		// catch error from getNamedNode
	}
	catch (const std::domain_error &)
	{ }
}

void
TransformHandle::traverse (TraverseType type)
{
	transform -> traverse (type);

	switch (type)
	{
		case TraverseType::PICKING:
		case TraverseType::CAMERA:
		case TraverseType::NAVIGATION:
		case TraverseType::COLLISION:
		{
			glPushMatrix ();

			glMultMatrixf (transform -> getMatrix () .data ());

			for (const auto & rootNode : scene -> getRootNodes ())
				rootNode -> traverse (type);

			glPopMatrix ();
			break;
		}
		case TraverseType::COLLECT:
		{
			glPushMatrix ();

			glMultMatrixf (transform -> getMatrix () .data ());

			reshape ();

			for (const auto & rootNode : scene -> getRootNodes ())
				rootNode -> traverse (type);

			glPopMatrix ();
			break;
		}
	}
}

void
TransformHandle::dispose ()
{
	transform .dispose ();
	scene     .dispose ();

	X3DHandleObject::dispose ();
	Transform::dispose ();
}

} // X3D
} // titania
