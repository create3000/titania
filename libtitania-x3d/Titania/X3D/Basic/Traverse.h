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

#ifndef __TITANIA_X3D_BITS_TRAVERSE_H__
#define __TITANIA_X3D_BITS_TRAVERSE_H__

#include "../Base/X3DChildObject.h"
#include "../Components/Core/X3DNode.h"
#include "../Execution/X3DScene.h"

#include "../Fields/MFNode.h"
#include "../Fields/SFNode.h"

#include <vector>

namespace titania {
namespace X3D {

constexpr int32_t TRAVERSE_EXTERNPROTO_DECLARATIONS       = 1;
constexpr int32_t TRAVERSE_EXTERNPROTO_PROTO_DECLARATIONS = 1 << 1;
constexpr int32_t TRAVERSE_PROTO_DECLARATIONS             = 1 << 2;
constexpr int32_t TRAVERSE_ROOT_NODES                     = 1 << 3;
constexpr int32_t TRAVERSE_PROTOTYPE_INSTANCES            = 1 << 4;
constexpr int32_t TRAVERSE_IMPORTED_NODES                 = 1 << 5;
constexpr int32_t TRAVERSE_EXPORTED_NODES                 = 1 << 6;
constexpr int32_t TRAVERSE_INLINE_NODES                   = 1 << 7;
constexpr int32_t TRAVERSE_TOOL_OBJECTS                   = 1 << 8;
constexpr int32_t TRAVERSE_VISIBLE_NODES                  = 1 << 9;
constexpr int32_t TRAVERSE_CLONED_NODES                   = 1 << 10;
constexpr int32_t TRAVERSE_META_DATA                      = 1 << 11;

typedef std::function <bool (X3D::SFNode &)> TraverseCallback;

bool
traverse (X3DExecutionContext* const, const TraverseCallback &, const bool = true, const int32_t = TRAVERSE_ROOT_NODES);

inline
bool
traverse (const X3DScenePtr & scene, const TraverseCallback & callback, const bool distinct = true, const int32_t flags = TRAVERSE_ROOT_NODES)
{ return traverse (scene .getValue (), callback, distinct, flags); }

inline
bool
traverse (const X3DExecutionContextPtr & executionContext, const TraverseCallback & callback, const bool distinct = true, const int32_t flags = TRAVERSE_ROOT_NODES)
{ return traverse (executionContext .getValue (), callback, distinct, flags); }

bool
traverse (MFNode &, const TraverseCallback &, const bool = true, const int32_t = TRAVERSE_ROOT_NODES);

bool
traverse (SFNode &, const TraverseCallback &, const bool = true, const int32_t = TRAVERSE_ROOT_NODES);

std::vector <X3DChildObject*>
find (X3DScene* const, X3DChildObject* const, const int32_t = TRAVERSE_ROOT_NODES);

inline
std::vector <X3DChildObject*>
find (const X3DScenePtr & scene, X3DChildObject* const object, const int32_t flags = TRAVERSE_ROOT_NODES)
{ return find (scene .getValue (), object, flags); }

std::vector <X3DChildObject*>
find (X3DExecutionContext* const, X3DChildObject* const, const int32_t = TRAVERSE_ROOT_NODES);

inline
std::vector <X3DChildObject*>
find (const X3DExecutionContextPtr & executionContext, X3DChildObject* const object, const int32_t flags = TRAVERSE_ROOT_NODES)
{ return find (executionContext .getValue (), object, flags); }

std::vector <X3DChildObject*>
find (const X3D::MFNode &, X3DChildObject* const, const int32_t = TRAVERSE_ROOT_NODES);

std::vector <X3DChildObject*>
find (const X3D::SFNode &, X3DChildObject* const, const int32_t = TRAVERSE_ROOT_NODES);

template <class Type>
void
findParents (X3DChildObject* const object, std::vector <Type*> & parents, const int32_t flags, ChildObjectSet & seen)
{
	if (not seen .emplace (object) .second)
		return;

	const auto node = dynamic_cast <X3DBaseNode*> (object);

	if (node)
	{
		for (const auto & type : basic::make_reverse_range (node -> getType ()))
		{
			switch (type)
			{
				case X3DConstants::X3DMetadataObject:
				case X3DConstants::X3DProgrammableShaderObject:
				case X3DConstants::X3DProtoDeclarationNode:
				case X3DConstants::X3DScriptNode:
				{
					if (flags & TRAVERSE_VISIBLE_NODES)
						return;

					break;
				}
				case X3DConstants::X3DNode:
					break;
				case X3DConstants::LayerSet:
				case X3DConstants::X3DBaseNode:
					return;
				default:
					continue;
			}

			break;
		}
	}

	const auto parent = dynamic_cast <Type*> (object);

	if (parent)
	{
		parents .emplace_back (parent);
		return;
	}

	for (const auto & parent : object -> getParents ())
		findParents <Type> (parent, parents, flags, seen);
}

template <class Type>
std::vector <Type*>
findParents (const X3DChildObject* const object, const int32_t flags = 0)
{
	std::vector <Type*> parents;
	ChildObjectSet      seen;

	for (const auto & parent : object -> getParents ())
		findParents <Type> (parent, parents, flags, seen);

	return parents;
}

} // X3D
} // titania

#endif
