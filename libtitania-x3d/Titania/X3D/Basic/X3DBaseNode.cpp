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

#include "X3DBaseNode.h"

#include "../Basic/X3DConstants.h"
#include "../Browser/X3DBrowser.h"
#include "../Execution/X3DScene.h"
#include "../Fields/ArrayFields.h"
#include "../Fields/MFNode.h"
#include "../Fields/SFNode.h"
#include "../Parser/Filter.h"

#include <Titania/Utility/Range.h>

#include <cassert>
#include <iomanip>
#include <iostream>

namespace titania {
namespace X3D {

/*
 * Creation phase
 *    The node is first instantiated by the browser internals with all field values set to defaults
 *
 *    Permitted field access capabilities
 *       initializeOnly	None
 *       inputOnly		None
 *       outputOnly		None
 *       inputOutput		None
 *
 *
 * Setup phase
 *    Field values are changed from the default value where required
 *
 *    Permitted field access capabilities
 *       initializeOnly	readable/writable
 *       inputOnly		None
 *       outputOnly		None
 *       inputOutput		readable/writable
 *
 *    setBrowser (SAIBrowserRef)
 *    setFields  (SAIField(s))
 *    initialize ()
 *
 *
 * Realized phase
 *    The node is participating in the scene graph and/or scripting
 *
 *    Permitted field access capabilities
 *       initializeOnly	None
 *       inputOnly		writable
 *       outputOnly		readable
 *       inputOutput		readable/writable
 *
 *    prepareEvents ()
 *    eventsProcessed ()
 *
 *
 * Disposed phase
 *    The node is no longer part of a scene graph and no remaining references to it exist at the scripting level.
 *
 *    Permitted field access capabilities
 *       initializeOnly	None
 *       inputOnly		None
 *       outputOnly		None
 *       inputOutput		None
 *
 *    shutdown ()
 */

/***
 *  Contructs a new node into @a browser and @a executionContext.
 *
 *  Important: a in this way newly created node must be setuped manually. On setup there must be a
 *  OpenGL context available.
 *
 *  You can use:
 *
 *      auto node = new Transform (executionContext);
 *      executionContext -> addUninitalizedNode (node);
 *      executionContext -> realize ();
 */
X3DBaseNode::X3DBaseNode (X3DBrowser* const browser, X3DExecutionContext* const executionContext) :
	     X3DParentObject (browser),
	    executionContext (executionContext),
	          nameOutput (),
	                type ({ X3DConstants::X3DBaseNode }),
	    fieldDefinitions (),
	              fields (),
	        fieldAliases (),
	numUserDefinedFields (0),
	        fieldsOutput (),
	         initialized (false),
	            private_ (false),
	          cloneCount (0),
	                live (true),
	            comments ()
{
	assert (executionContext);

	addChildren (nameOutput, fieldsOutput, live);
}

/***
 *  Initializes a node: after setup all fields can process events.
 */
void
X3DBaseNode::setup ()
{
	X3DParentObject::setup ();

	executionContext -> addParent (this);

	for (const auto & field : fieldDefinitions)
	{
		field -> updateIsReferences ();
		field -> isTainted (false);
	}

	initialized = true;
	initialize ();
}

/***
 *  Creates copy of this node as specified by @a type.
 *
 *  The node must be created and setuped like this:
 *
 *      auto clone = node -> copy (executionContext, FLAT_COPY);
 *      executionContext -> realize ();
 */
X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext, const CopyType type) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	switch (type)
	{
		case CLONE:
		{
			return const_cast <X3DBaseNode*> (this);
		}
		case COPY_OR_CLONE:
		{
			if (getName () .empty ())
				return copy (executionContext);

			try
			{
				return executionContext -> getNamedNode (getName ());
			}
			catch (const X3DError &)
			{
				return copy (executionContext);
			}
		}
		case FLAT_COPY:
		{
			return copy (executionContext, FlatCopyType { });
		}
	}

	throw Error <NOT_SUPPORTED> ("Not supported.");
}

/***
 *  Creates a copy of this node and all of its child nodes into @a executionContext. If a named node of one of this
 *  node children already exists in @a executionContext then only a clone is created.
 */
X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	const SFNode copy = create (executionContext); // Never try to optimize this.

	if (not getName () .empty ())
		executionContext -> updateNamedNode (getName (), copy);

	for (const auto & fieldDefinition : fieldDefinitions)
	{
		try
		{
			// Default fields

			X3DFieldDefinition* const field = copy -> getField (fieldDefinition -> getName ());

			if (field -> getAccessType () == fieldDefinition -> getAccessType () and field -> getType () == fieldDefinition -> getType ())
			{
				field -> isSet (fieldDefinition -> isSet ());

				if (fieldDefinition -> getIsReferences () .empty ())
				{
					if (fieldDefinition -> isInitializable ())
					{
						fieldDefinition -> copy (executionContext, field, COPY_OR_CLONE);
					}
				}
				else
				{
					// IS relationship

					for (const auto & originalReference : fieldDefinition -> getIsReferences ())
					{
						try
						{
							field -> addIsReference (executionContext -> getField (originalReference -> getName ()));
						}
						catch (const Error <INVALID_NAME> &)
						{
							throw Error <INVALID_NAME> ("No such event or field '" + originalReference -> getName () + ".");
						}
					}
				}
			}
			else
				throw Error <INVALID_NAME> ("");
		}
		catch (const Error <INVALID_NAME> &)
		{
			// User defined fields from Script and Shader

			if (fieldDefinition -> getIsReferences () .empty ())
			{
				const auto field = fieldDefinition -> copy (executionContext, COPY_OR_CLONE);

				field -> isSet (fieldDefinition -> isSet ());

				copy -> addUserDefinedField (fieldDefinition -> getAccessType (),
				                             fieldDefinition -> getName (),
				                             field);
			}
			else
			{
				// IS relationship

				X3DFieldDefinition* const field = fieldDefinition -> copy (COPY_OR_CLONE);

				field -> isSet (fieldDefinition -> isSet ());

				copy -> addUserDefinedField (fieldDefinition -> getAccessType (),
				                             fieldDefinition -> getName (),
				                             field);

				for (const auto & originalReference : fieldDefinition -> getIsReferences ())
				{
					try
					{
						field -> addIsReference (executionContext -> getField (originalReference -> getName ()));
					}
					catch (const Error <INVALID_NAME> &)
					{
						throw Error <INVALID_NAME> ("No reference '" + originalReference -> getName () + "' inside execution context " + executionContext -> getTypeName () + " '" + executionContext -> getName () + "'.");
					}
				}
			}
		}
	}

	executionContext -> addUninitializedNode (copy);

	return copy;
}

/***
 *  Creates a flat copy of this node into @a executionContext.
 */
X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext, const FlatCopyType &) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	const SFNode copy = create (executionContext);

	if (not getName () .empty ())
		executionContext -> updateNamedNode (executionContext -> getUniqueName (getName ()), copy);

	for (const auto & field : std::make_pair (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end ()))
		copy -> addUserDefinedField (field -> getAccessType (), field -> getName (), field -> create ());

	copy -> assign (this);

	executionContext -> addUninitializedNode (copy);

	return copy;
}

/***
 *  Assigns all fields from @a node to this node.  @a node must be of the same type and must have exacly the same field
 *  definitions.  If @a compare is true the fields are only assigned if they are not equal to prevent event generation.
 */
void
X3DBaseNode::assign (const X3DBaseNode* const node, const bool compare)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>)
{
	for (size_t i = 0, size = fieldDefinitions .size (); i < size; ++ i)
	{
		auto & lhs = *fieldDefinitions [i];

		if (lhs .getAccessType () & initializeOnly)
		{
			auto & rhs = *node -> getFieldDefinitions () [i];

			if (not compare or lhs not_eq rhs)
				lhs = rhs;
		}
	}
}

/***
 *  Completely replaces this node in all parent X3DPtr and X3DWeakPtr by @a node.
 *
 *  This function should be used with caution.  It is currently only useful with with addTool/removeTool.
 *  @a node requires a special implementation as it should return from getId the same id as this node as
 *  this node could be replaced in a std::set <X3DPtr> like container.
 */
void
X3DBaseNode::replace (X3DBaseNode* const node)
{
	std::vector <X3DChildObject*> parents;

	for (const auto & parent : node -> getParents ())
	{
		if (dynamic_cast <X3DPtrBase*> (parent))
			parents .emplace_back (parent);
	}

	// We must hold a reference to both nodes until the replace is done to avoid disposal,
	// as there are pointer and weak pointer.

	const SFNode previous (node);
	const SFNode replacement (this);

	for (const auto & parent : parents)
	{
		parent -> set (replacement);
	}
}

/***
 *  Sets the name of this object and processes the name_changed interests.
 */
void
X3DBaseNode::setName (const std::string & value)
{
	X3DParentObject::setName (value);

	nameOutput = chrono::now ();
}

/***
 *  Returns the current browser time for the current frame.
 */
time_type
X3DBaseNode::getCurrentTime () const
throw (Error <DISPOSED>)
{
	return getBrowser () -> getClock () -> cycle ();
}

/***
 *  Sets the current excecution context to @a executionContext.
 */
void
X3DBaseNode::setExecutionContext (X3DExecutionContext* const value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (value not_eq executionContext)
	{
		executionContext -> removeParent (this);

		executionContext = value;
		executionContext -> addParent (this);
	}

	// Replace browser as last step!

	setBrowser (executionContext -> getBrowser ());
}

/***
 *  Returns the master execution context for this node.  This is normaly the initial scene the node belongs to.
 */
X3DScene*
X3DBaseNode::getMasterScene () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DScene* scene = getExecutionContext () -> getScene ();

	while (not scene -> isMasterScene ())
		scene = scene -> getScene ();

	return scene;
}

/***
 *  Returns the root execution context for this node.  This is normaly the scene the node belongs to.
 */
X3DScene*
X3DBaseNode::getScene () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DExecutionContext* executionContext = getExecutionContext ();

	while (not executionContext -> isScene ())
		executionContext = executionContext -> getExecutionContext ();

	return dynamic_cast <X3DScene*> (executionContext);
}

/***
 *  Returns either a node declaration or the prototype of this node.
 */
const X3DBaseNode*
X3DBaseNode::getInterfaceDeclaration () const
throw (Error <DISPOSED>)
{
	try
	{
		return getBrowser () -> getSupportedNode (getTypeName ());
	}
	catch (const X3DError &)
	{
		return this;
	}
}

/***
 *  Returns true if field is in the set of fields either pre or user defined.
 */
bool
X3DBaseNode::hasField (const std::string & name) const
throw (Error <DISPOSED>)
{
	try
	{
		getField (name);
		return true;
	}
	catch (const X3D::X3DError &)
	{
		return false;
	}
}

/***
 *  Adds @a field to the set of pre defined fields. @a accessType and @a name will be assigned to @a field.
 */
void
X3DBaseNode::addField (const AccessType accessType, const std::string & name, X3DFieldDefinition & field)
throw (Error <INVALID_NAME>,
       Error <INVALID_FIELD>,
       Error <DISPOSED>)
{
	const auto iter = fields .find (name);

	if (iter not_eq fields .end ())
		removeField (iter, false, iter -> second not_eq &field);

	if (not isInitialized ())
		field .isTainted (true);

	field .addParent (this);
	field .setAccessType (accessType);
	field .setName (name);

	if (not private_)
		field .addClones (1);

	fieldDefinitions .emplace_back (&field);
	fields .emplace (name, &field);
}

/***
 *  Adds a @a alias for a field named @a name.
 */
void
X3DBaseNode::addField (const SpecificationVersionType version, const std::string & alias, const std::string & name)
throw (Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto & fieldAlias = fieldAliases [version];

	fieldAlias .first [alias] = name;
	fieldAlias .second [name] = alias;
}

/***
 *  Removes @a field from the set of fields.
 */
void
X3DBaseNode::removeField (const std::string & name)
throw (Error <DISPOSED>)
{
	removeField (fields .find (name), false, true);
}

/***
 *  Removes @a field from the set fields.
 */
void
X3DBaseNode::removeField (const FieldIndex::iterator & field, const bool userDefined, const bool removeParent)
{
	if (field == fields .end ())
		return;

	const auto iter = userDefined
	                  ? std::find (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end (), field -> second)
							: std::find (fieldDefinitions .begin (), fieldDefinitions .end (), field -> second);

	if (iter == fieldDefinitions .end ())
		return;

	if (fieldDefinitions .end () - iter <= FieldDefinitionArray::difference_type (numUserDefinedFields))
	{
		-- numUserDefinedFields;

		//  If the field is added twice we must not remove the parent to prevent dispose.
		if (removeParent)
			field -> second -> removeParent (this);
	}
	else
	{
		//  If field is a pre defined field, we defer dispose.
		X3DParentObject::disposed () .addInterest (field -> second, &X3DFieldDefinition::removeParent, this);
	}

	if (not private_)
		field -> second -> removeClones (1);

	fieldDefinitions .erase (iter);
	fields .erase (field);
}

/***
 *  The getField service returns a field identifier so that operations can be performed on the node properties.  If the
 *  field requested is an inputOutput field, either the field name or the set_ and _changed modifiers may be used to
 *  access the appropriate form of the node as required..
 */
X3DFieldDefinition*
X3DBaseNode::getField (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto field = fields .find (getFieldName (name));

	if (field not_eq fields .end ())
		return field -> second;

	if (name .substr (0, 4) == "set_")
	{
		const auto field = fields .find (getFieldName (name .substr (4)));

		if (field not_eq fields .end () and field -> second -> getAccessType () == inputOutput)
			return field -> second;
	}

	if (name .size () > 8 and name .substr (name .size () - 8) == "_changed")
	{
		const auto field = fields .find (getFieldName (name .substr (0, name .size () - 8)));

		if (field not_eq fields .end () and field -> second -> getAccessType () == inputOutput)
			return field -> second;
	}

	throw Error <INVALID_NAME> ("No such field '" + name + "' in node " + getTypeName () + ".");
}

/***
 *  Returns the standard field name for @a alias.
 */
const std::string &
X3DBaseNode::getFieldName (const std::string & alias) const
{
	for (const auto & version : fieldAliases)
	{
		const auto fieldAlias = version .second .first .find (alias);

		if (fieldAlias not_eq version .second .first .end ())
			return fieldAlias -> second;
	}

	return alias;
}

/***
 *  Returns the standard field name for @a alias depending on @a version.
 */
const std::string &
X3DBaseNode::getFieldName (const std::string & name, const SpecificationVersionType version) const
{
	if (not fieldAliases .empty ())
	{
		const auto fieldAlias = fieldAliases .find (version);

		if (fieldAlias not_eq fieldAliases .end ())
		{
			const auto alias = fieldAlias -> second .second .find (name);

			if (alias not_eq fieldAlias -> second .second  .end ())
				return alias -> second;
		}
	}

	return name;
}

/***
 *  Replaces the set of user defined fields of this node with @a userDefinedFields.
 */
void
X3DBaseNode::setUserDefinedFields (const X3D::FieldDefinitionArray & userDefinedFields)
throw (Error <INVALID_NAME>,
       Error <INVALID_FIELD>,
       Error <DISPOSED>)
{
	// Unfortunately this has to be done field by field as long as derived classes do not override this function.

	std::vector <FieldPtr> roots;

	for (const auto & field : getUserDefinedFields ())
	{
		roots .emplace_back (field);
		removeUserDefinedField (field -> getName ());
	}

	for (const auto & field : userDefinedFields)
		addUserDefinedField (field -> getAccessType (), field -> getName (), field);
}

/***
 *  Adds @a field to the set of user defined fields. @a accessType and @a name will be assigned to @a field.
 */
void
X3DBaseNode::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
throw (Error <INVALID_NAME>,
       Error <INVALID_FIELD>,
       Error <DISPOSED>)
{
	addField (accessType, name, *field);

	++ numUserDefinedFields;

	fieldsOutput = chrono::now ();
}

/***
 *  Removes @a field from the set of user defined fields.  You are self responsible to remove all routes of this field
 *  from and to this node before you remove the field.
 */
void
X3DBaseNode::removeUserDefinedField (const std::string & name)
throw (Error <DISPOSED>)
{
	removeField (fields .find (name), true, true);

	fieldsOutput = chrono::now ();
}

/***
 *  Returns all predefined fields.
 */
FieldDefinitionArray
X3DBaseNode::getPreDefinedFields () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	FieldDefinitionArray predefinedFields;

	for (const auto & field : std::make_pair (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields))
	{
		if (field -> isHidden ())
			continue;

		try
		{
			getInterfaceDeclaration () -> getField (field -> getName ());

			predefinedFields .emplace_back (field);
		}
		catch (const X3D::Error <INVALID_NAME> &)
		{
			// Field is not defined in ExternProto but in Proto.
		}
		catch (const X3D::Error <DISPOSED> &)
		{
			// Field is not defined in ExternProto but in Proto.

			predefinedFields .emplace_back (field);
		}
	}

	return predefinedFields;
}

/***
 *  Returns all user defined fields.
 */
FieldDefinitionArray
X3DBaseNode::getUserDefinedFields () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return FieldDefinitionArray (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end ());
}

/***
 *  Returns all fields that have a non default value.
 */
FieldDefinitionArray
X3DBaseNode::getChangedFields () const
{
	FieldDefinitionArray changedFields;

	for (const auto & field : std::make_pair (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields))
	{
		if (field -> getIsReferences () .empty ())
		{
			if (not field -> isInitializable ())
				continue;

			try
			{
				if (isDefaultValue (field))
					continue;
			}
			catch (const Error <DISPOSED> &)
			{
				// This can happen if a ExternProto is not loaded.
			}
			catch (const Error <INVALID_NAME> &)
			{
				// This can happen if a ExternProto has less fields than the prototype.
				continue;
			}
		}

		changedFields .emplace_back (field);
	}

	return changedFields;
}

/***
 *  Returns true if @a field has the default value, @a field must be a field of this node.
 */
bool
X3DBaseNode::isDefaultValue (const X3DFieldDefinition* const field) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		const X3DFieldDefinition* const declarationField = getInterfaceDeclaration () -> getField (field -> getName ());

		return *field == *declarationField;
	}
	catch (const Error <DISPOSED> &)
	{
	   // Externproto could not be loaded.

		return not field -> isSet ();
	}
}

/***
 *  Marks this node as a node for internal use only. Such they do not increment the clone count of its child nodes and
 *  are not fully printable.
 */
void
X3DBaseNode::isPrivate (const bool value)
{
	if (value == private_)
		return;

	private_ = value;

	if (private_)
	{
		for (const auto & field : fieldDefinitions)
			field -> removeClones (1);
	}
	else
	{
		for (const auto & field : fieldDefinitions)
			field -> addClones (1);
	}
}

/***
 *  Increments the clone count by @a count.
 */
void
X3DBaseNode::addClones (const size_t count)
{
	if (count == 0)
		return;

	cloneCount += count;

	// Works, but the assumption that the scene graph changes when the clone count changeds is not fully correct as shared node are not handled.
	if (executionContext not_eq this)
		const_cast <SFTime &> (executionContext -> sceneGraph_changed ()) = chrono::now ();
}

/***
 *  Decrements the clone count by @a count.
 */
void
X3DBaseNode::removeClones (const size_t count)
{
	if (count == 0)
		return;

	cloneCount -= count;

	// Works, but the assumption that the scene graph changes when the clone count changeds is not fully correct as shared node are not handled.
	if (executionContext not_eq this)
		const_cast <SFTime &> (executionContext -> sceneGraph_changed ()) = chrono::now ();
}

/***
 *  Replaces this node by @a tool and calls setup on tool.
 */
void
X3DBaseNode::addTool (X3DBaseNode* const tool)
{
	tool -> replace (this);
	tool -> setup ();
}

/***
 *  Replaces this tool node by @a node.
 */
void
X3DBaseNode::removeTool (X3DBaseNode* const node)
{
	node -> replace (this);
}

/***
 *  Enables this node if possible, ie. the execution context of this node must be enabled too.
 */
void
X3DBaseNode::beginUpdate ()
throw (Error <DISPOSED>)
{
	if (not live)
		live = true;
}

/***
 *  Definitely disables this node. No further event processing is done.
 */
void
X3DBaseNode::endUpdate ()
throw (Error <DISPOSED>)
{
	if (live)
		live = false;
}

/***
 *  Returns true if there are any routes from or to fields of this node otherwise false.
 */
bool
X3DBaseNode::hasRoutes () const
{
	for (const auto & field : fieldDefinitions)
	{
		if (field -> getInputRoutes () .empty () and field -> getOutputRoutes () .empty ())
			continue;

		return true;
	}

	return false;
}

/***
 *  NOT SUPPORTED.
 */
void
X3DBaseNode::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	//	return istream >> value;
}

/***
 *  Inserts this node into @a ostream.
 */
void
X3DBaseNode::toStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	if (Generator::IsSharedNode (this))
	{
		ostream << "NULL";
		return;
	}

	Generator::EnterScope ();

	const std::string & name = Generator::Name (this);

	if (not name .empty ())
	{
		if (Generator::ExistsNode (this))
		{
			ostream
				<< "USE"
				<< Generator::Space
				<< name;

			Generator::LeaveScope ();

			return;
		}
	}

	if (not getComments () .empty ())
	{
		ostream
			<< Generator::Comment
			<< getComments () .front ()
			<< Generator::ForceBreak;

		for (const auto & comment : std::make_pair (getComments () .begin () + 1, getComments (). end ()))
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}

		ostream << Generator::Indent;
	}

	if (not name .empty ())
	{
		Generator::AddNode (this);

		ostream
			<< "DEF"
			<< Generator::Space
			<< name
			<< Generator::Space;
	}

	ostream
		<< getTypeName ()
		<< Generator::TidySpace
		<< '{';

	size_t fieldTypeLength  = 0;
	size_t accessTypeLength = 0;

	if (canUserDefinedFields ())
	{
		const auto userDefinedFields = std::make_pair (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end ());

		switch (Generator::Style ())
		{
			case Generator::SMALLEST:
			case Generator::SMALL:
			{
				break;
			}
			default:
			{
				for (const auto & field : userDefinedFields)
				{
					fieldTypeLength  = std::max (fieldTypeLength, field -> getTypeName () .length ());
					accessTypeLength = std::max (accessTypeLength, Generator::AccessTypes [field] .length ());
				}

				break;
			}
		}

		if (numUserDefinedFields)
		{
			ostream
				<< Generator::TidyBreak
				<< Generator::IncIndent;

			for (const auto & field : std::make_pair (userDefinedFields .first, userDefinedFields .second - 1))
			{
				toStreamUserDefinedField (ostream, field, fieldTypeLength, accessTypeLength);
				ostream << Generator::Break;
			}

			toStreamUserDefinedField (ostream, *(userDefinedFields .second - 1), fieldTypeLength, accessTypeLength);
			ostream << Generator::Break;

			ostream
				<< Generator::DecIndent
				<< Generator::TidyBreak;
		}
	}

	const FieldDefinitionArray fields = getChangedFields ();

	if (fields .empty ())
	{
		if (numUserDefinedFields)
			ostream << Generator::Indent;
		else
			ostream << Generator::TidySpace;
	}
	else
	{
		if (numUserDefinedFields == 0)
			ostream << Generator::TidyBreak;

		ostream << Generator::IncIndent;

		for (const auto & field : std::make_pair (fields .begin (), fields .end () - 1))
		{
			toStreamField (ostream, field, fieldTypeLength, accessTypeLength);
			ostream << Generator::Break;
		}

		toStreamField (ostream, fields .back (), fieldTypeLength, accessTypeLength);
		ostream << Generator::TidyBreak;

		ostream
			<< Generator::DecIndent
			<< Generator::Indent;
	}

	if (not getInnerComments () .empty ())
	{
		ostream
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & comment : getInnerComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent;
	}

	ostream << '}';

	Generator::LeaveScope ();
}

/***
 *  Inserts this @a field into @a ostream.
 */
void
X3DBaseNode::toStreamField (std::ostream & ostream, X3DFieldDefinition* const field, const size_t fieldTypeLength, const size_t accessTypeLength) const
{
	for (const auto & comment : field -> getComments ())
	{
		ostream
			<< Generator::Indent
			<< Generator::Comment
			<< comment
			<< Generator::ForceBreak;
	}

	if (field -> getIsReferences () .empty () or not Generator::ExecutionContext () or Generator::IsSharedNode (this))
	{
		// Output build in field

		ostream << Generator::Indent;

		ostream << getFieldName (field -> getName (), Generator::SpecificationVersion ());

		ostream
			<< Generator::Space
			<< *field;
	}
	else
	{
		size_t index                  = 0;
		bool   initializableReference = false;

		for (const auto & reference : field -> getIsReferences ())
		{
			initializableReference |= reference -> isInitializable ();

			// Output build in reference

			ostream << Generator::Indent;

			ostream << getFieldName (field -> getName (), Generator::SpecificationVersion ());

			ostream
				<< Generator::Space
				<< "IS"
				<< Generator::Space
				<< reference -> getName ();

			++ index;

			if (index not_eq field -> getIsReferences () .size ())
				ostream << Generator::Break;
		}

		try
		{
			// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
			// for this field.

			if (field -> getAccessType () == inputOutput and not initializableReference and not isDefaultValue (field))
			{
				// Output build in field

				ostream
					<< Generator::Break
					<< Generator::Indent;

				ostream << getFieldName (field -> getName (), Generator::SpecificationVersion ());

				ostream
					<< Generator::Space
					<< *field;
			}
		}
		catch (const X3DError &)
		{
			// Catch isDefaultValue, this can happen if the Externproto has less fields than the proto but should not happen here
			// as the field has references.
		}
	}
}

/***
 *  Inserts this @a field into @a ostream.
 */
void
X3DBaseNode::toStreamUserDefinedField (std::ostream & ostream, X3DFieldDefinition* const field, const size_t fieldTypeLength, const size_t accessTypeLength) const
{
	for (const auto & comment : field -> getComments ())
	{
		ostream
			<< Generator::Indent
			<< Generator::Comment
			<< comment
			<< Generator::ForceBreak;
	}

	// If we have no execution context we are not in a proto and must not generate IS references the same is true
	// if the node is a shared node as the node does not belong to the execution context.

	if (field -> getIsReferences () .empty () or not Generator::ExecutionContext () or Generator::IsSharedNode (this))
	{
		// Output user defined field

		ostream
			<< Generator::Indent
			<< std::setiosflags (std::ios::left)
			<< std::setw (accessTypeLength);

		ostream << Generator::AccessTypes [field];

		ostream
			<< Generator::Space
			<< std::setiosflags (std::ios::left) << std::setw (fieldTypeLength) << field -> getTypeName ()
			<< Generator::Space;

		ostream << field -> getName ();

		if (field -> isInitializable ())
		{
			ostream
				<< Generator::Space
				<< *field;
		}
	}
	else
	{
		size_t index                  = 0;
		bool   initializableReference = false;

		for (const auto & reference : field -> getIsReferences ())
		{
			initializableReference |= reference -> isInitializable ();

			// Output user defined reference field

			ostream
				<< Generator::Indent
				<< std::setiosflags (std::ios::left)
				<< std::setw (accessTypeLength);

			ostream << Generator::AccessTypes [field];

			ostream
				<< Generator::Space
				<< std::setiosflags (std::ios::left) << std::setw (fieldTypeLength) << field -> getTypeName ()
				<< Generator::Space;

			ostream << field -> getName ();

			ostream
				<< Generator::Space
				<< "IS"
				<< Generator::Space
				<< reference -> getName ();

			++ index;

			if (index not_eq field -> getIsReferences () .size ())
				ostream << Generator::Break;
		}

		// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
		// for this field.

		if (field -> getAccessType () == inputOutput and not initializableReference and not field -> isDefaultValue ())
		{
			// Output user defined field

			ostream
				<< Generator::Break
				<< Generator::Indent
				<< std::setiosflags (std::ios::left)
				<< std::setw (accessTypeLength);

			ostream << Generator::AccessTypes [field];

			ostream
				<< Generator::Space
				<< std::setiosflags (std::ios::left) << std::setw (fieldTypeLength) << field -> getTypeName ()
				<< Generator::Space;

			ostream << field -> getName ();

			if (field -> isInitializable ())
			{
				ostream
					<< Generator::Space
					<< *field;
			}
		}
	}
}

/***
 *  Inserts this node into @a ostream in XML output format.
 */
void
X3DBaseNode::toXMLStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	if (Generator::IsSharedNode (this))
	{
		ostream
			<< Generator::Indent
			<< "<!-- NULL -->";

		return;
	}

	Generator::EnterScope ();

	const std::string & name = Generator::Name (this);

	if (not name .empty ())
	{
		if (Generator::ExistsNode (this))
		{
			ostream
				<< Generator::Indent
				<< "<"
				<< getTypeName ()
				<< Generator::Space
				<< "USE='"
				<< XMLEncode (name)
				<< "'";

			const auto containerField = Generator::ContainerField ();

			if (containerField)
			{
				if (containerField -> getName () not_eq getContainerField ())
				{
					ostream
						<< Generator::Space
						<< "containerField='"
						<< XMLEncode (containerField -> getName ())
						<< "'";
				}
			}

			ostream << "/>";

			Generator::LeaveScope ();

			return;
		}
	}

	ostream
		<< Generator::Indent
		<< "<"
		<< getTypeName ();

	if (not name .empty ())
	{
		Generator::AddNode (this);

		ostream
			<< Generator::Space
			<< "DEF='"
			<< XMLEncode (name)
			<< "'";
	}

	const auto containerField = Generator::ContainerField ();

	if (containerField)
	{
		if (containerField -> getName () not_eq getContainerField ())
		{
			ostream
				<< Generator::Space
				<< "containerField='"
				<< XMLEncode (containerField -> getName ())
				<< "'";
		}
	}

	const FieldDefinitionArray fields            = getChangedFields ();
	const auto                 userDefinedFields = std::make_pair (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end ());

	FieldDefinitionArray references;
	FieldDefinitionArray childNodes;

	const MFString* cdata = getCDATA ();

	if (cdata and cdata -> empty ())
		cdata = nullptr;

	ostream
		<< Generator::IncIndent
		<< Generator::IncIndent;

	for (const auto & field : fields)
	{
		// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
		// for this field.

		bool mustOutputValue = false;

		if (Generator::ExecutionContext () and not Generator::IsSharedNode (this))
		{
			if (field -> getAccessType () == inputOutput and not field -> getIsReferences () .empty ())
			{
				bool initializableReference = false;

				for (const auto & reference : field -> getIsReferences ())
					initializableReference |= reference -> isInitializable ();

				try
				{
					if (not initializableReference)
						mustOutputValue = not isDefaultValue (field);
				}
				catch (const X3DError &)
				{
					mustOutputValue = false;
				}
			}
		}

		// If we have no execution context we are not in a proto and must not generate IS references the same is true
		// if the node is a shared node as the node does not belong to the execution context.

		if ((field -> getIsReferences () .empty () or not Generator::ExecutionContext () or Generator::IsSharedNode (this)) or mustOutputValue)
		{
			if (mustOutputValue)
				references .emplace_back (field);

			switch (field -> getType ())
			{
				case X3DConstants::SFNode:
				case X3DConstants::MFNode:
				{
					childNodes .emplace_back (field);
					break;
				}
				default:
				{
					if (field == cdata)
						break;

					ostream
						<< Generator::Break
						<< Generator::Indent
						<< getFieldName (field -> getName (), Generator::SpecificationVersion ())
						<< "='"
						<< XMLEncode (field)
						<< "'";

					break;
				}
			}
		}
		else
		{
			references .emplace_back (field);
		}
	}

	ostream
		<< Generator::DecIndent
		<< Generator::DecIndent;

	if ((not canUserDefinedFields () or numUserDefinedFields == 0) and references .empty () and childNodes .empty () and not cdata)
	{
		ostream << "/>";
	}
	else
	{
		ostream
			<< ">"
			<< Generator::Break
			<< Generator::IncIndent;

		if (canUserDefinedFields ())
		{
			for (const auto & field : userDefinedFields)
			{
				ostream
					<< Generator::Indent
					<< "<field"
					<< Generator::Space
					<< "accessType='"
					<< Generator::X3DAccessTypes [field]
					<< "'"
					<< Generator::Space
					<< "type='"
					<< field -> getTypeName ()
					<< "'"
					<< Generator::Space
					<< "name='"
					<< XMLEncode (field -> getName ())
					<< "'";

				// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
				// for this field.

				bool mustOutputValue = false;

				if (field -> getAccessType () == inputOutput and not field -> getIsReferences () .empty ())
				{
					bool initializableReference = false;

					for (const auto & reference : field -> getIsReferences ())
						initializableReference |= reference -> isInitializable ();

					if (not initializableReference)
						mustOutputValue = not field -> isDefaultValue ();
				}

				if ((field -> getIsReferences () .empty () or not Generator::ExecutionContext ()) or mustOutputValue)
				{
					if (mustOutputValue and Generator::ExecutionContext ())
						references .emplace_back (field);

					if (not field -> isInitializable () or field -> isDefaultValue ())
					{
						ostream
							<< "/>"
							<< Generator::Break;
					}
					else
					{
						// Output value

						switch (field -> getType ())
						{
							case X3DConstants::SFNode:
							case X3DConstants::MFNode:
							{
								Generator::PushContainerField (nullptr);

								ostream
									<< ">"
									<< Generator::Break
									<< Generator::IncIndent
									<< XMLEncode (field)
									<< Generator::Break
									<< Generator::DecIndent
									<< Generator::Indent
									<< "</field>"
									<< Generator::Break;

								Generator::PopContainerField ();

								break;
							}
							default:
							{
								ostream
									<< Generator::Space
									<< "value='"
									<< XMLEncode (field)
									<< "'"
									<< "/>"
									<< Generator::Break;

								break;
							}
						}
					}
				}
				else
				{
					if (Generator::ExecutionContext ())
						references .emplace_back (field);

					ostream
						<< "/>"
						<< Generator::Break;
				}
			}
		}

		if (not references .empty ())
		{
			ostream
				<< Generator::Indent
				<< "<IS>"
				<< Generator::Break
				<< Generator::IncIndent;

			for (const auto & field : references)
			{
				for (const auto & reference : field -> getIsReferences ())
				{
					ostream
						<< Generator::Indent
						<< "<connect"
						<< Generator::Space
						<< "nodeField='"
						<< XMLEncode (field -> getName ())
						<< "'"
						<< Generator::Space
						<< "protoField='"
						<< XMLEncode (reference -> getName ())
						<< "'"
						<< "/>"
						<< Generator::Break;
				}
			}

			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< "</IS>"
				<< Generator::Break;
		}

		for (const auto & field : childNodes)
		{
			Generator::PushContainerField (field);

			ostream
				<< XMLEncode (field)
				<< Generator::Break;

			Generator::PopContainerField ();
		}

		if (cdata)
		{
			for (std::string value : *cdata)
			{
				ostream
					<< "<![CDATA["
					<< escape_cdata (value)
					<< "]]> "
					<< Generator::Break;
			}
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< "</"
			<< getTypeName ()
			<< ">";
	}

	Generator::LeaveScope ();
}

/***
 *  Disposes this node.  Note: it is normaly not needed to call this function.
 */
void
X3DBaseNode::dispose ()
{
	X3DParentObject::dispose ();

	for (const auto & field : fieldDefinitions)
	{
		if (not private_)
			field -> removeClones (1);

		field -> removeParent (this);
	}

	fields .clear ();
	fieldDefinitions .clear ();

	executionContext -> removeParent (this);
}

/***
 *  Destructs this node.
 */
X3DBaseNode::~X3DBaseNode ()
{ }

} // X3D
} // titania
