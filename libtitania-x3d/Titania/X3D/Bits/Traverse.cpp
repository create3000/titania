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

#include "Traverse.h"

#include "../Basic/NodeSet.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Grouping/Switch.h"
#include "../Components/Navigation/Collision.h"
#include "../Components/Navigation/LOD.h"
#include "../Components/Networking/Inline.h"
#include "../Execution/ImportedNode.h"
#include "../Prototype/ExternProtoDeclaration.h"
#include "../Prototype/ProtoDeclaration.h"
#include "../Tools/Core/X3DToolObject.h"

namespace titania {
namespace X3D {

static
bool
traverse (X3D::SFNode & node, const TraverseCallback & callback, const bool distinct, const int flags, NodeSet & seen);

static
bool
traverse (X3DExecutionContext* const executionContext, const TraverseCallback & callback, const bool distinct, const int flags, NodeSet & seen)
{
	if (not executionContext)
		return true;

	if (flags & TRAVERSE_EXTERNPROTO_DECLARATIONS)
	{
		for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
		{
			SFNode node (externProto);

			if (traverse (node, callback, distinct, flags, seen))
				continue;

			return false;
		}
	}

	if (flags & TRAVERSE_PROTO_DECLARATIONS)
	{
		for (const auto & prototype : executionContext -> getProtoDeclarations ())
		{
			SFNode node (prototype);

			if (traverse (node, callback, distinct, flags, seen))
				continue;

			return false;
		}
	}

	if (flags & TRAVERSE_ROOT_NODES)
	{
		for (auto & node : executionContext -> getRootNodes ())
		{
			if (traverse (node, callback, distinct, flags, seen))
				continue;

			return false;
		}
	}

	if (flags & TRAVERSE_IMPORTED_NODES)
	{
		for (const auto & importedNode : executionContext -> getImportedNodes ())
		{
			try
			{
				SFNode exportedNode (importedNode .second -> getExportedNode ());

				if (traverse (exportedNode, callback, distinct, flags, seen))
				   continue;

			   return false;
			}
			catch (...)
			{ }
		}
	}

	return true;
}

bool
traverse (X3DExecutionContext* const executionContext, const TraverseCallback & callback, const bool distinct, const int flags)
{
	NodeSet seen;

	return traverse (executionContext, callback, distinct, flags, seen);
}

static
bool
traverse (X3D::SFNode & node, const TraverseCallback & callback, const bool distinct, const int flags, NodeSet & seen)
{
	if (not node)
		return true;

	if (flags & TRAVERSE_VISIBLE_NODES)
	{
		switch (node -> getType () .back ())
		{
			case X3DConstants::Script:
				return true;
			default:
				break;
		}
	}

	if (seen .emplace (node) .second)
	{
		for (const auto & field : node -> getFieldDefinitions ())
		{
			switch (field -> getType ())
			{
				case X3DConstants::SFNode :
				{
					const auto sfnode = static_cast <X3D::SFNode*> (field);

					if (flags & TRAVERSE_VISIBLE_NODES)
					{
						for (const auto & type : basic::make_reverse_range (node -> getType ()))
						{
							switch (type)
							{
								case X3DConstants::Collision:
								{
									if (sfnode == &dynamic_cast <Collision*> (node .getValue ()) -> proxy ())
										goto CONTINUE;		
									break;
								}
								case X3DConstants::X3DNode:
								{
									if (sfnode == &dynamic_cast <X3DNode*> (node .getValue ()) -> metadata ())
										goto CONTINUE;		
									break;
								}
								default:
									break;
							}
						}
					}

					if (traverse (*sfnode, callback, distinct, flags, seen))
						continue;

					return false;
				}
				case X3DConstants::MFNode:
				{
					const auto mfnode = static_cast <X3D::MFNode*> (field);
	
					if (flags & TRAVERSE_VISIBLE_NODES)
					{
						switch (node -> getType () .back ())
						{
							case X3DConstants::SwitchTool:
							case X3DConstants::Switch:
							{
								const auto switchNode = dynamic_cast <Switch*> (node .getValue ());
									
								if (mfnode == &switchNode -> children ())
								{
									if (switchNode -> getWhichChoice () >= 0 and switchNode -> getWhichChoice () < (int32_t) switchNode -> children () .size ())
									{
										if (traverse (switchNode -> children () [switchNode -> getWhichChoice ()], callback, distinct, flags, seen))
											continue;
										
										return false;
									}

									continue;
								}

								break;
							}
							case X3DConstants::LODTool:
							case X3DConstants::LOD:
							{
								const auto lod = dynamic_cast <LOD*> (node .getValue ());
									
								if (mfnode == &lod -> children ())
								{
									if (lod -> level_changed () >= 0 and lod -> level_changed () < (int32_t) lod -> children () .size ())
									{
										if (traverse (lod -> children () [lod -> level_changed ()], callback, distinct, flags, seen))
											continue;
										
										return false;
									}

									continue;
								}

								break;
							}
							default:
								break;
						}
					}

					for (auto & value : *mfnode)
					{
						if (traverse (value, callback, distinct, flags, seen))
							continue;

						return false;
					}

					break;
				}
				default:
					break;
			}

CONTINUE:;
		}
	}
	else
	{
		if (distinct)
			return true;
	}

	if (flags & ~TRAVERSE_ROOT_NODES)
	{
		for (const auto & type : basic::make_reverse_range (node -> getType ()))
		{
			switch (type)
			{
				case X3DConstants::ExternProtoDeclaration:
				{
					if (flags & TRAVERSE_EXTERNPROTO_PROTO_DECLARATIONS)
					{
						SFNode externProto (node);

						if (traverse (externProto, callback, distinct, flags, seen))
							continue;

						return false;
					}

					break;
				}
				case X3DConstants::ProtoDeclaration:
				{
					if (flags & TRAVERSE_PROTO_DECLARATIONS)
					{
						if (traverse (dynamic_cast <X3DExecutionContext*> (node .getValue ()), callback, distinct, flags, seen))
							continue;

						return false;
					}

					break;
				}
				case X3DConstants::X3DPrototypeInstance:
				{
					if (flags & TRAVERSE_PROTOTYPE_INSTANCES)
					{
						const auto instance = dynamic_cast <X3DPrototypeInstance*> (node .getValue ());

						for (auto & rootNode : instance -> getRootNodes ())
						{
							if (traverse (rootNode, callback, distinct, flags, seen))
								continue;

							return false;
						}
					}

					break;
				}
				case X3DConstants::Inline:
				{
					if (flags & TRAVERSE_INLINE_NODES)
					{
						const auto inlineNode = dynamic_cast <Inline*> (node .getValue ());

						for (auto & value : inlineNode -> getRootNodes ())
						{
							if (traverse (value, callback, distinct, flags, seen))
								continue;

							return false;
						}
					}

					break;
				}
				default:
					break;
			}
		}
	}

	if (callback (node))
	{
		if (flags & TRAVERSE_CLONED_NODES)
			seen .erase (node);

		return true;
	}

	return false;
}

bool
traverse (X3D::MFNode & nodes, const TraverseCallback & callback, const bool distinct, const int flags)
{
	NodeSet seen;

	for (auto & node : nodes)
	{
		if (traverse (node, callback, distinct, flags, seen))
			continue;

		return false;
	}

	return true;
}

bool
traverse (X3D::SFNode & node, const TraverseCallback & callback, const bool distinct, const int flags)
{
	NodeSet seen;

	return traverse (node, callback, distinct, flags, seen);
}

static
bool
find (X3DBaseNode* const, X3DChildObject* const, const int, std::vector <X3DChildObject*> &, NodeSet &);

static
bool
find (X3DExecutionContext* const executionContext, X3DChildObject* const object, const int flags, std::vector <X3DChildObject*> & hierarchy, NodeSet & seen)
{
	if (not executionContext)
		return false;

	hierarchy .emplace_back (executionContext);

	if (executionContext == object)
		return true;

	if (flags & TRAVERSE_EXTERNPROTO_DECLARATIONS)
	{
		for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
		{
			if (find (static_cast <X3DBaseNode*> (externProto), object, flags, hierarchy, seen))
				return true;
		}
	}

	if (flags & TRAVERSE_PROTO_DECLARATIONS)
	{
		for (const auto & prototype : executionContext -> getProtoDeclarations ())
		{
			if (find (static_cast <X3DBaseNode*> (prototype), object, flags, hierarchy, seen))
				return true;
		}
	}

	if (flags & TRAVERSE_ROOT_NODES)
	{
		for (const auto & node : executionContext -> getRootNodes ())
		{
			if (find (node, object, flags, hierarchy, seen))
				return true;
		}
	}

	if (flags & TRAVERSE_IMPORTED_NODES)
	{
		for (const auto & importedNode : executionContext -> getImportedNodes ())
		{
			try
			{
				const auto exportedNode = importedNode .second -> getExportedNode ();

				if (find (exportedNode, object, flags, hierarchy, seen))
					return true;
			}
			catch (...)
			{ }
		}
	}

	hierarchy .pop_back ();
	return false;
}

static
bool
find (X3DScene* const scene, X3DChildObject* const object, const int flags, std::vector <X3DChildObject*> & hierarchy, NodeSet & seen)
{
	return find (static_cast <X3DExecutionContext*> (scene), object, flags, hierarchy, seen);
}

static
bool
find (X3DBaseNode* const node, X3DChildObject* const object, const int flags, std::vector <X3DChildObject*> & hierarchy, NodeSet & seen)
{
	if (not node)
		return false;

	if (not seen .emplace (node) .second)
		return false;

	hierarchy .emplace_back (node);

	if (node == object)
		return true;

	if (flags & TRAVERSE_VISIBLE_NODES)
	{
		switch (node -> getType () .back ())
		{
			case X3DConstants::Script:
				goto END;
			default:
				break;
		}
	}

	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (field == object)
		{
			hierarchy .emplace_back (field);
			return true;
		}

		switch (field -> getType ())
		{
			case X3DConstants::SFNode:
			{
				const auto sfnode = static_cast <X3D::SFNode*> (field);

				if (flags & TRAVERSE_VISIBLE_NODES)
				{
					for (const auto & type : basic::make_reverse_range (node -> getType ()))
					{
						switch (type)
						{
							case X3DConstants::Collision:
							{
								if (sfnode == &dynamic_cast <Collision*> (node) -> proxy ())
									goto CONTINUE;
								break;
							}
							case X3DConstants::X3DNode:
							{
								if (sfnode == &dynamic_cast <X3DNode*> (node) -> metadata ())
									goto CONTINUE;
								break;
							}
							default:
								break;
						}
					}
				}

				// Normal case:

				hierarchy .emplace_back (field);

				if (find (*sfnode, object, flags, hierarchy, seen))
					return true;

				hierarchy .pop_back ();
				break;
			}
			case X3DConstants::MFNode:
			{
				const auto mfnode = static_cast <X3D::MFNode*> (field);

				if (flags & TRAVERSE_VISIBLE_NODES)
				{
					switch (node -> getType () .back ())
					{
						case X3DConstants::Switch:
						case X3DConstants::SwitchTool:
						{
							const auto switchNode = dynamic_cast <Switch*> (node);

							if (mfnode == &switchNode -> children ())
							{
								if (switchNode -> getWhichChoice () >= 0 and switchNode -> getWhichChoice () < (int32_t) switchNode -> children () .size ())
								{
									hierarchy .emplace_back (field);

									if (find (switchNode -> children () [switchNode -> getWhichChoice ()], object, flags, hierarchy, seen))
										return true;

									hierarchy .pop_back ();
								}
								
								continue;
							}

							break;
						}
						case X3DConstants::LOD:
						case X3DConstants::LODTool:
						{
							const auto lod = dynamic_cast <LOD*> (node);
								
							if (mfnode == &lod -> children ())
							{
								if (lod -> level_changed () >= 0 and lod -> level_changed () < (int32_t) lod -> children () .size ())
								{
									hierarchy .emplace_back (field);

									if (find (lod -> children () [lod -> level_changed ()], object, flags, hierarchy, seen))
										return true;

									hierarchy .pop_back ();
								}
								
								continue;
							}

							break;
						}
						default:
							break;
					}
				}

				// Normal case:

				hierarchy .emplace_back (field);

				for (const auto & value : *mfnode)
				{
					if (find (value, object, flags, hierarchy, seen))
						return true;
				}

				hierarchy .pop_back ();
				break;
			}
			default:
				break;
		}

CONTINUE:;
	}

	if (flags & ~TRAVERSE_ROOT_NODES)
	{
		for (const auto & type : basic::make_reverse_range (node -> getType ()))
		{
			switch (type)
			{
				case X3DConstants::ExternProtoDeclaration:
				{
					if (flags & TRAVERSE_EXTERNPROTO_PROTO_DECLARATIONS)
					{
					   try
					   {
							const auto externProto = dynamic_cast <ExternProtoDeclaration*> (node);

							if (find (static_cast <X3DBaseNode*> (externProto -> getProtoDeclaration ()), object, flags, hierarchy, seen))
								return true;
						}
						catch (const X3DError &)
						{ }
					}

					break;
				}
				case X3DConstants::ProtoDeclaration:
				{
					if (flags & TRAVERSE_PROTO_DECLARATIONS)
					{
						if (find (dynamic_cast <X3DExecutionContext*> (node), object, flags, hierarchy, seen))
							return true;
					}

					break;
				}
				case X3DConstants::X3DPrototypeInstance:
				{
					if (flags & TRAVERSE_PROTOTYPE_INSTANCES)
					{
						if (find (dynamic_cast <X3DExecutionContext*> (node), object, flags, hierarchy, seen))
							return true;
					}
					
					break;
				}
				case X3DConstants::Inline:
				{
					if (flags & TRAVERSE_INLINE_NODES)
					{
						const auto inlineNode = dynamic_cast <Inline*> (node);

						if (find (static_cast <X3DExecutionContext*> (inlineNode -> getInternalScene ()), object, flags, hierarchy, seen))
							return true;
					}

					break;
				}
				case X3DConstants::X3DToolObject:
				{
					if (flags & TRAVERSE_TOOL_OBJECTS)
					{
						const auto tool = dynamic_cast <X3DToolObject*> (node);

						for (const auto & rootNode : tool -> getInlineNode () -> getRootNodes ())
						{
							if (find (rootNode, object, flags, hierarchy, seen))
								return true;
						}
					}

					break;
				}
				default:
					break;
			}
		}
	}

END:

	hierarchy .pop_back ();
	return false;
}

std::vector <X3DChildObject*>
find (X3DScene* const scene, X3DChildObject* const object, const int flags)
{
	std::vector <X3DChildObject*> hierarchy;
	NodeSet                       seen;

	find (scene, object, flags, hierarchy, seen);

	return hierarchy;
}

std::vector <X3DChildObject*>
find (X3DExecutionContext* const executionContext, X3DChildObject* const object, const int flags)
{
	std::vector <X3DChildObject*> hierarchy;
	NodeSet                       seen;

	find (executionContext, object, flags, hierarchy, seen);

	return hierarchy;
}

std::vector <X3DChildObject*>
find (const X3D::MFNode & nodes, X3DChildObject* const object, const int flags)
{
	std::vector <X3DChildObject*> hierarchy;
	NodeSet                       seen;

	for (const auto & node : nodes)
	{
		if (find (node, object, flags, hierarchy, seen))
			return hierarchy;
	}

	return hierarchy;
}

std::vector <X3DChildObject*>
find (const X3D::SFNode & node, X3DChildObject* const object, const int flags)
{
	std::vector <X3DChildObject*> hierarchy;
	NodeSet                       seen;

	find (node, object, flags, hierarchy, seen);

	return hierarchy;
}

} // X3D
} // titania
