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

#include "Traverse.h"

#include "../Basic/NodeSet.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DMetadataObject.h"
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
traverse (SFNode & node, const TraverseCallback & callback, const int32_t flags, NodeSet & seen);

static
bool
traverse (X3DExecutionContext* const executionContext, const TraverseCallback & callback, const int32_t flags, NodeSet & seen)
{
	if (not executionContext)
		return true;

//	if (executionContext -> getPrivate ())
//		return true;

	if (flags & TRAVERSE_EXTERNPROTO_DECLARATIONS)
	{
		for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
		{
			SFNode node (externProto);

			if (traverse (node, callback, flags, seen))
				continue;

			return false;
		}
	}

	if (flags & TRAVERSE_PROTO_DECLARATIONS)
	{
		for (const auto & prototype : executionContext -> getProtoDeclarations ())
		{
			SFNode node (prototype);

			if (traverse (node, callback, flags, seen))
				continue;

			return false;
		}
	}

	if (flags & TRAVERSE_ROOT_NODES)
	{
		if (flags & TRAVERSE_VISIBLE_NODES and executionContext -> isType ({ X3DConstants::X3DPrototypeInstance }))
		{
			if (not executionContext -> getRootNodes () .empty ())
			{
				if (not traverse (executionContext -> getRootNodes () .front (), callback, flags, seen))
					return false;
			}
		}
		else
		{
			for (auto & node : executionContext -> getRootNodes ())
			{
				if (traverse (node, callback, flags, seen))
					continue;

				return false;
			}
		}
	}

	if (flags & TRAVERSE_IMPORTED_NODES)
	{
		for (const auto & importedNode : executionContext -> getImportedNodes ())
		{
			try
			{
				SFNode exportedNode (importedNode .second -> getExportedNode ());

				if (traverse (exportedNode, callback, flags, seen))
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
traverse (X3DExecutionContext* const executionContext, const TraverseCallback & callback, const int32_t flags)
{
	NodeSet seen;

	return traverse (executionContext, callback, flags, seen);
}

static
bool
traverse (SFNode & node, const TraverseCallback & callback, const int32_t flags, NodeSet & seen)
{
	if (not node)
		return true;

	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		switch (type)
		{
			case X3DConstants::RigidBody:
			case X3DConstants::RigidBodyCollection:
			case X3DConstants::Script:
			{
				if (flags & TRAVERSE_VISIBLE_NODES)
					return true;

				break;
			}
			case X3DConstants::X3DMetadataObject:
			{
				if (flags & TRAVERSE_META_DATA)
					break;

				return true;
			}
			default:
				continue;
		}

		break;
	}

	if (seen .emplace (node) .second)
	{
		if (not node -> isType ({ X3DConstants::ExternProtoDeclaration }))
		{
			for (const auto & field : node -> getFieldDefinitions ())
			{
				switch (field -> getType ())
				{
					case X3DConstants::SFNode :
					{
						const auto sfnode = static_cast <SFNode*> (field);

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
									default:
										break;
								}
							}
						}

						if (traverse (*sfnode, callback, flags, seen))
							continue;

						return false;
					}
					case X3DConstants::MFNode:
					{
						const auto mfnode = static_cast <MFNode*> (field);

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
											if (traverse (switchNode -> children () [switchNode -> getWhichChoice ()], callback, flags, seen))
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
										if (lod -> children () .size ())
										{
											const int32_t index = std::min <int32_t> (lod -> level_changed (), lod -> children () .size () - 1);

											if (index >= 0)
											{
												if (traverse (lod -> children () [index], callback, flags, seen))
													continue;

												return false;
											}
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
							if (traverse (value, callback, flags, seen))
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
	}
	else
	{
		if (not (flags & TRAVERSE_INDISTINCT))
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
					if (flags & TRAVERSE_EXTERNPROTO_DECLARATION_SCENE)
					{
						const auto externProto = dynamic_cast <ExternProtoDeclaration*> (node .getValue ());

						if (traverse (externProto -> getInternalScene (), callback, flags, seen))
							continue;

						return false;
					}

					break;
				}
				case X3DConstants::ProtoDeclaration:
				{
					if (flags & TRAVERSE_PROTO_DECLARATION_BODY)
					{
						if (traverse (dynamic_cast <X3DExecutionContext*> (node .getValue ()), callback, flags, seen))
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
							if (traverse (rootNode, callback, flags, seen))
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
							if (traverse (value, callback, flags, seen))
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
traverse (MFNode & nodes, const TraverseCallback & callback, const int32_t flags)
{
	NodeSet seen;

	for (auto & node : nodes)
	{
		if (traverse (node, callback, flags, seen))
			continue;

		return false;
	}

	return true;
}

bool
traverse (SFNode & node, const TraverseCallback & callback, const int32_t flags)
{
	NodeSet seen;

	return traverse (node, callback, flags, seen);
}

static
void
find (X3DBaseNode* const, X3DChildObject* const, const int32_t, Hierarchies & hierarchies, Hierarchy &, NodeSet &);

static
void
find (X3DExecutionContext* const executionContext, X3DChildObject* const object, const int32_t flags, Hierarchies & hierarchies, Hierarchy & hierarchy, NodeSet & seen)
{
	if (not executionContext)
		return;

	if (executionContext -> getPrivate ())
		return;

	hierarchy .emplace_back (executionContext);

	if (executionContext == object)
	{
		hierarchies .emplace_back (hierarchy);
	}
	else
	{
		if (flags & TRAVERSE_EXTERNPROTO_DECLARATIONS)
		{
			for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
			{
				find (static_cast <X3DBaseNode*> (externProto), object, flags, hierarchies, hierarchy, seen);
			}
		}

		if (flags & TRAVERSE_PROTO_DECLARATIONS)
		{
			for (const auto & prototype : executionContext -> getProtoDeclarations ())
			{
				find (static_cast <X3DBaseNode*> (prototype), object, flags, hierarchies, hierarchy, seen);
			}
		}

		if (flags & TRAVERSE_ROOT_NODES)
		{
			if (flags & TRAVERSE_ROOT_NODES)
			{
				if (flags & TRAVERSE_VISIBLE_NODES and executionContext -> isType ({ X3DConstants::X3DPrototypeInstance }))
				{
					if (not executionContext -> getRootNodes () .empty ())
						find (executionContext -> getRootNodes () .front (), object, flags, hierarchies, hierarchy, seen);
				}
				else
				{
					for (const auto & node : executionContext -> getRootNodes ())
					{
						find (node, object, flags, hierarchies, hierarchy, seen);
					}
				}
			}
		}

		if (flags & TRAVERSE_IMPORTED_NODES)
		{
			for (const auto & importedNode : executionContext -> getImportedNodes ())
			{
				try
				{
					const auto exportedNode = importedNode .second -> getExportedNode ();

					find (exportedNode, object, flags, hierarchies, hierarchy, seen);
				}
				catch (...)
				{ }
			}
		}
	}

	hierarchy .pop_back ();
}

static
void
find (X3DScene* const scene, X3DChildObject* const object, const int32_t flags, Hierarchies & hierarchies, Hierarchy & hierarchy, NodeSet & seen)
{
	find (static_cast <X3DExecutionContext*> (scene), object, flags, hierarchies, hierarchy, seen);
}

static
void
find (X3DBaseNode* const node, X3DChildObject* const object, const int32_t flags, Hierarchies & hierarchies, Hierarchy & hierarchy, NodeSet & seen)
{
	if (not node)
		return;

	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		switch (type)
		{
			case X3DConstants::RigidBody:
			case X3DConstants::RigidBodyCollection:
			case X3DConstants::Script:
			{
				if (flags & TRAVERSE_VISIBLE_NODES)
					return;

				break;
			}
			case X3DConstants::X3DMetadataObject:
			{
				if (flags & TRAVERSE_META_DATA)
					break;

				return;
			}
			default:
				continue;
		}

		break;
	}

	if (not seen .emplace (node) .second)
		return;

	hierarchy .emplace_back (node);

	if (node == object)
	{
		hierarchies .emplace_back (hierarchy);
	}
	else if (not node -> isType ({ X3DConstants::ExternProtoDeclaration }))
	{
		for (const auto & field : node -> getFieldDefinitions ())
		{
			hierarchy .emplace_back (field);

			if (field == object)
			{
				hierarchies .emplace_back (hierarchy);
			}
			else
			{
				switch (field -> getType ())
				{
					case X3DConstants::SFNode:
					{
						const auto sfnode = static_cast <SFNode*> (field);

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

						find (*sfnode, object, flags, hierarchies, hierarchy, seen);

						break;
					}
					case X3DConstants::MFNode:
					{
						const auto mfnode = static_cast <MFNode*> (field);

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
											find (switchNode -> children () [switchNode -> getWhichChoice ()], object, flags, hierarchies, hierarchy, seen);
										}

										goto CONTINUE;
									}

									break;
								}
								case X3DConstants::LOD:
								case X3DConstants::LODTool:
								{
									const auto lod = dynamic_cast <LOD*> (node);

									if (mfnode == &lod -> children ())
									{
										if (lod -> children () .size ())
										{
											const int32_t index = std::min <int32_t> (lod -> level_changed (), lod -> children () .size () - 1);

											if (index >= 0)
											{
												find (lod -> children () [index], object, flags, hierarchies, hierarchy, seen);
											}
										}

										goto CONTINUE;
									}

									break;
								}
								default:
									break;
							}
						}

						// Normal case:

						for (const auto & value : *mfnode)
						{
							find (value, object, flags, hierarchies, hierarchy, seen);
						}

						break;
					}
					default:
						break;
				}

CONTINUE:;

				hierarchy .pop_back ();
			}
		}

		if (flags & ~TRAVERSE_ROOT_NODES)
		{
			for (const auto & type : basic::make_reverse_range (node -> getType ()))
			{
				switch (type)
				{
					case X3DConstants::ExternProtoDeclaration:
					{
						if (flags & TRAVERSE_EXTERNPROTO_DECLARATION_SCENE)
						{
						   try
						   {
								const auto externProto = dynamic_cast <ExternProtoDeclaration*> (node);

								find (static_cast <X3DExecutionContext*> (externProto -> getInternalScene ()), object, flags, hierarchies, hierarchy, seen);
							}
							catch (const X3DError &)
							{ }
						}

						break;
					}
					case X3DConstants::ProtoDeclaration:
					{
						if (flags & TRAVERSE_PROTO_DECLARATION_BODY)
						{
							find (dynamic_cast <X3DExecutionContext*> (node), object, flags, hierarchies, hierarchy, seen);
						}

						break;
					}
					case X3DConstants::X3DPrototypeInstance:
					{
						if (flags & TRAVERSE_PROTOTYPE_INSTANCES)
						{
							find (dynamic_cast <X3DExecutionContext*> (node), object, flags, hierarchies, hierarchy, seen);
						}

						break;
					}
					case X3DConstants::Inline:
					{
						if (flags & TRAVERSE_INLINE_NODES)
						{
							const auto inlineNode = dynamic_cast <Inline*> (node);

							find (static_cast <X3DExecutionContext*> (inlineNode -> getInternalScene ()), object, flags, hierarchies, hierarchy, seen);
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
								find (rootNode, object, flags, hierarchies, hierarchy, seen);
							}
						}

						break;
					}
					default:
						break;
				}
			}
		}
	}

	hierarchy .pop_back ();

	seen .erase (node);
}

Hierarchies
find (X3DScene* const scene, X3DChildObject* const object, const int32_t flags)
{
	Hierarchies hierarchies;
	Hierarchy   hierarchy;
	NodeSet     seen;

	find (scene, object, flags, hierarchies, hierarchy, seen);

	return hierarchies;
}

Hierarchies
find (X3DExecutionContext* const executionContext, X3DChildObject* const object, const int32_t flags)
{
	Hierarchies hierarchies;
	Hierarchy   hierarchy;
	NodeSet     seen;

	find (executionContext, object, flags, hierarchies, hierarchy, seen);

	return hierarchies;
}

Hierarchies
find (const MFNode & nodes, X3DChildObject* const object, const int32_t flags)
{
	Hierarchies hierarchies;
	Hierarchy   hierarchy;
	NodeSet     seen;

	for (const auto & node : nodes)
	{
		find (node, object, flags, hierarchies, hierarchy, seen);
	}

	return hierarchies;
}

Hierarchies
find (const SFNode & node, X3DChildObject* const object, const int32_t flags)
{
	Hierarchies hierarchies;
	Hierarchy   hierarchy;
	NodeSet     seen;

	find (node, object, flags, hierarchies, hierarchy, seen);

	return hierarchies;
}

} // X3D
} // titania
