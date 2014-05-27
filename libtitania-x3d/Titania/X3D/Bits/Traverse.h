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

#ifndef __TITANIA_X3D_BITS_TRAVERSE_H__
#define __TITANIA_X3D_BITS_TRAVERSE_H__

#include "../Base/X3DChildObject.h"
#include "../Components/Core/X3DNode.h"
#include "../Execution/Scene.h"

#include "../Fields/MFNode.h"
#include "../Fields/SFNode.h"

#include <vector>

namespace titania {
namespace X3D {

constexpr int TRAVERSE_EXTERN_PROTO_DECLARATIONS = 1;
constexpr int TRAVERSE_PROTO_DECLARATIONS        = 1 << 1;
constexpr int TRAVERSE_ROOT_NODES                = 1 << 2;
constexpr int TRAVERSE_PROTOTYPE_INSTANCES       = 1 << 3;
constexpr int TRAVERSE_IMPORTED_NODES            = 1 << 4;
constexpr int TRAVERSE_EXPORTED_NODES            = 1 << 5;
constexpr int TRAVERSE_INLINE_NODES              = 1 << 6;
constexpr int TRAVERSE_TOOL_OBJECTS              = 1 << 7;

typedef std::function <bool (X3D::SFNode &)> TraverseCallback;

bool
traverse (MFNode &, const TraverseCallback &, const bool = true, const int = 0);

bool
traverse (SFNode &, const TraverseCallback &, const bool = true, const int = 0);

std::vector <X3DChildObject*>
find (X3DScene* const, X3DChildObject* const, const int = TRAVERSE_ROOT_NODES);

std::vector <X3DChildObject*>
find (X3DExecutionContext* const, X3DChildObject* const, const int = TRAVERSE_ROOT_NODES);

std::vector <X3DChildObject*>
find (const X3D::MFNode &, X3DChildObject* const, const int = TRAVERSE_ROOT_NODES);

std::vector <X3DChildObject*>
find (const X3D::SFNode &, X3DChildObject* const, const int = TRAVERSE_ROOT_NODES);

template <class Type>
void
findParents (X3DChildObject* const self, std::vector <Type*> & parents, ChildObjectSet & seen)
{
	if (not seen .emplace (self) .second)
		return;

	const auto node = dynamic_cast <X3DBaseNode*> (self);

	if (node)
	{
		for (const auto & type : basic::reverse_adapter (node -> getType ()))
		{
			switch (type)
			{
				case X3DConstants::X3DProtoObject:
				case X3DConstants::X3DScriptNode:
				case X3DConstants::X3DProgrammableShaderObject:
				case X3DConstants::X3DBaseNode:
					return;
				case X3DConstants::X3DNode:
					goto NEXT;
				default:
					break;
			}
		}
	}

NEXT:

	const auto parent = dynamic_cast <Type*> (self);

	if (parent)
	{
		parents .emplace_back (parent);
		return;
	}

	for (const auto & object : self -> getParents ())
		findParents <Type> (object, parents, seen);
}

template <class Type>
std::vector <Type*>
findParents (const X3DNode* const self)
{
	std::vector <Type*> parents;
	ChildObjectSet      seen;

	for (const auto & object : self -> getParents ())
		findParents <Type> (object, parents, seen);

	return parents;
}

} // X3D
} // titania

#endif
