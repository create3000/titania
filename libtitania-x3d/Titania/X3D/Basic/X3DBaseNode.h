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

#ifndef __TITANIA_X3D_BASIC_X3DBASE_NODE_H__
#define __TITANIA_X3D_BASIC_X3DBASE_NODE_H__

#include "../Base/Output.h"
#include "../Base/X3DParentObject.h"
#include "../Basic/FieldDefinitionArray.h"
#include "../Basic/NodeTypeArray.h"
#include "../Basic/NodeTypeSet.h"
#include "../Basic/TraverseType.h"
#include "../Basic/X3DFieldDefinition.h"
#include "../Fields.h"

#include <map>

namespace titania {
namespace X3D {

typedef std::map <std::string, X3DFieldDefinition*> FieldIndex;

class X3DBrowser;
class X3DScene;
class X3DExecutionContext;
class X3DRenderObject;

/**
 *  Class to represent an object that is the base for all nodes.
 */
class X3DBaseNode :
	public X3DParentObject
{
public:

	using X3DParentObject::addInterest;
	using X3DParentObject::removeInterest;

	///  @name Construction

	///  Initializes this node.
	virtual
	void
	setup ();

	///  Constructs a new node into @a executionContext.
	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const = 0;

	///  Copys this node.  The executionContext will be the same as for this node.
	virtual
	X3DBaseNode*
	copy (const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>)
	{ return copy (executionContext, type); }

	///  Copys this node and sets the execution context to @a executionContext.
	virtual
	X3DBaseNode*
	copy (X3DExecutionContext* const executionContext, const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	///  Assigns @a node. @a node must be of the same type as this node.
	virtual
	void
	assign (const X3DBaseNode* const baseNode, const bool compare = false)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>);

	///  @name Commons members

	///  Sets the name of this object.
	virtual
	void
	setName (const std::string & name) override;

	///  Field that processes its interests when the name has changed.
	virtual
	const SFTime &
	name_changed () const
	{ return nameOutput; }

	///  Returns the current browser time for this frame.
	time_type
	getCurrentTime () const
	throw (Error <DISPOSED>);

	///  Sets the current excecution context to @a executionContext.
	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Returns a pointer to the execution context this node belongs to.
	X3DExecutionContext*
	getExecutionContext () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return executionContext; }

	///  Returns a pointer to the master execution context this node belongs to.
	X3DScene*
	getMasterScene () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Returns a pointer to the root execution context this node belongs to.
	X3DScene*
	getScene () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Returns the component name this node belongs to.
	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) = 0;

	///  Returns a array of types of this node.
	const NodeTypeArray &
	getType () const
	throw (Error <DISPOSED>)
	{ return type; }

	///  Returns true if the node is one of the types in @a types;
	bool
	isType (const NodeTypeSet &) const
	throw (Error <DISPOSED>);

	///  Returns the XML container field for this node type.
	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) = 0;

	///  Returns either a node declaration or the prototype of this node.
	virtual
	const X3DBaseNode*
	getInterfaceDeclaration () const
	throw (Error <DISPOSED>);

	///  Returns for X3DProtoypeInstances the innermost root node.
	virtual
	X3DBaseNode*
	getInnerNode ()
	throw (Error <DISPOSED>)
	{ return this; }

	///  Returns for X3DProtoypeInstances the innermost root node.
	virtual
	const X3DBaseNode*
	getInnerNode () const
	throw (Error <DISPOSED>)
	{ return this; }

	///  @name Field handling

	///  Checks if a field with @a name exists for this node.
	virtual
	bool
	hasField (const std::string & name) const
	throw (Error <DISPOSED>);

	///  Checks whether @a field has the default value for this node type.  The @a field must be of this node.
	virtual
	bool
	isDefaultValue (const X3DFieldDefinition* const field) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Sets the value of the field with @a name.  If the third parameter is true, an event is only generated if the old
	///  value is not the new value.
	template <class FieldType, class ValueType>
	void
	setField (const std::string & name, const ValueType & value, const bool compare = false)
	throw (Error <INVALID_FIELD>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Return the field with @a name.
	template <class Type>
	Type &
	getField (const std::string & name)
	throw (Error <INVALID_FIELD>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Return the field with @a name.
	template <class Type>
	const Type &
	getField (const std::string & name) const
	throw (Error <INVALID_FIELD>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Return the field with @a name.
	virtual
	X3DFieldDefinition*
	getField (const std::string & name) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Returns whether this node can have user defined fields.
	virtual
	bool
	canUserDefinedFields () const
	{ return false; }

	///  Replaces the set of user defined fields of this node with @a userDefinedFields.
	virtual
	void
	setUserDefinedFields (const X3D::FieldDefinitionArray & fields)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>);

	///  Adds @a field to the set of user defined fields of this node.
	virtual
	void
	addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>);

	///  Updates @a field in the set of user defined fields. @a accessType and @a name will be assigned to @a field.
	void
	updateUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>);

	///  Removes the field named @a name from the set of user defined fields of this node.
	virtual
	void
	removeUserDefinedField (const std::string & name)
	throw (Error <DISPOSED>);

	///  Returns an array with all pre defined fields of this node.
	virtual
	FieldDefinitionArray
	getPreDefinedFields () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Returns an array with all user defined fields of this node.
	virtual
	FieldDefinitionArray
	getUserDefinedFields () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Returns an array of all fields with non default value.
	virtual
	FieldDefinitionArray
	getChangedFields () const;

	///  Return all field definition for this node, that is all predefined field and user defined fields.
	virtual
	const FieldDefinitionArray &
	getFieldDefinitions () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return fieldDefinitions; }

	///  Field that processes its interests when the set of fields has changed.
	virtual
	const SFTime &
	fields_changed () const
	{ return fieldsOutput; }

	///  If the node has a XML CDATA field it is returned otherwise a null pointer.
	virtual
	MFString*
	getSourceText ()
	{ return nullptr; }

	///  If the node has a XML CDATA field it is returned otherwise a null pointer.
	virtual
	const MFString*
	getSourceText () const
	{ return nullptr; }

	///  @name Clone handling

	///  Marks this node as private or not.  The default is false.
	virtual
	void
	setPrivate (const bool value);

	///  Returns whether this node is marked private.
	virtual
	bool
	getPrivate () const
	{ return privateState; }

	///  Returns the number of clones of this node used in the scene graph.
	virtual
	size_t
	getCloneCount () const final override
	{ return cloneCount; }

	///  Increments the clone count by @a count.
	virtual
	void
	addCloneCount (const size_t count) final override;

	///  Decrements the clone count by @a count.
	virtual
	void
	removeCloneCount (const size_t count) final override;

	///  Returns the number of clones of this node used in the meta data nodes.
	virtual
	size_t
	getMetaCloneCount () const
	{ return metaCloneCount; }

	///  Increments the meta clone count by @a count.
	virtual
	void
	addMetaCloneCount (const size_t count);

	///  Decrements the meta clone count by @a count.
	virtual
	void
	removeMetaCloneCount (const size_t count);

	///  @name Tool support

	///  Replace the node by its tool if it supports any.
	virtual
	void
	addTool ();

	///  Reverses the effect of addTool.  For some tools @a really must be true.
	virtual
	void
	removeTool (const bool = false)
	{ }

	///  @name Event handling

	///  See beginUpdate and endUpdate.
	virtual
	SFBool &
	isLive ()
	{ return live; }

	///  See beginUpdate and endUpdate.
	virtual
	const SFBool &
	isLive () const
	{ return live; }

	///  After calling beginUpdate the node is able to process events.  Effectivly sets isLive to true.
	virtual
	void
	beginUpdate ()
	throw (Error <DISPOSED>);

	///  Immediately stops event processing of this node.  Effectivly sets isLive to false.
	virtual
	void
	endUpdate ()
	throw (Error <DISPOSED>);

	///  Returns true if any field has a input or output route otherwise false.
	virtual
	bool
	hasRoutes () const;

	///  @name Interest service

	///  Adds an interest to this object.  The @a requester is then notified about a change of this object.
	template <class ValueType>
	void
	addInterest (X3DField <ValueType>* const requester) const
	{ addInterest (&X3DField <ValueType>::addEvent, requester); }

	///  Adds an interest to this object.  The @a requester is then notified about a change of this object.
	template <class ValueType>
	void
	addInterest (X3DField <ValueType> & requester) const
	{ addInterest (&X3DField <ValueType>::addEvent, &requester); }

	///  Removes an interest from this object.  The @a requester will not further notified about a change of this object.
	template <class ValueType>
	void
	removeInterest (X3DField <ValueType>* const requester) const
	{ removeInterest (&X3DField <ValueType>::addEvent, requester); }

	///  Removes an interest from this object.  The @a requester will not further notified about a change of this object.
	template <class ValueType>
	void
	removeInterest (X3DField <ValueType> & requester) const
	{ removeInterest (&X3DField <ValueType>::addEvent, &requester); }

	///  @name Traversal handling

	///  Traverses the scene graph depending on @a type.
	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject)
	{ }

	///  @name Comment handling

	///  Adds comments for this node.  The comments will be printed before the '}'.
	virtual
	void
	addInnerComments (const std::vector <std::string> & value)
	{ comments .insert (comments .end (), value .cbegin (), value .cend ()); }

	///  Returns the comments for this node.
	virtual
	const std::vector <std::string> &
	getInnerComments () const
	{ return comments; }

	///  @name Input/Output

	///  NOT SUPPORTED
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  Inserts this object into @a ostream in VRML Classic Encoding.
	virtual
	void
	toStream (std::ostream & ostream) const override;

	///  Inserts this object into @a ostream in X3D XML Encoding.
	virtual
	void
	toXMLStream (std::ostream & ostream) const override;

	///  Inserts this object into @a ostream in X3D JSON Encoding.
	virtual
	void
	toJSONStream (std::ostream & ostream) const override;

	///  @name Destruction

	///  Disposes this node.  You normally do not need to call this function directly.
	virtual
	void
	dispose () override;

	///  Destructs this node.
	virtual
	~X3DBaseNode () override;


protected:

	///  @name Construction

	///  Constructs new X3DBaseNode.
	X3DBaseNode (X3DBrowser* const = nullptr, X3DExecutionContext* const = nullptr);

	///  Initializes this node.
	virtual
	void
	initialize ();

	///  @name Common members

	///  Adds @a value to the type of this node.
	void
	addType (const X3DConstants::NodeType value)
	{ type .emplace_back (value); }

	///  @name Field handling

	///  Adds @a field to the set of fields of this node.
	virtual
	void
	addField (const AccessType accessType, const std::string & name, X3DFieldDefinition & field)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>);

	///  Adds a field for X3D @a version.
	void
	addField (const SpecificationVersionType specificationVersion, const std::string & alias, const std::string & name)
	throw (Error <INVALID_NAME>,
	       Error <DISPOSED>);

	///  Remove field @a name from the set of fields.
	void
	removeField (const std::string & name)
	throw (Error <DISPOSED>);

	///  @name Tool support

	///  Replaces this node by @a tool.
	void
	addTool (X3DBaseNode* const tool);

	///  Replaces this node by @a node.
	void
	removeTool (X3DBaseNode* const tool);


private:

	///  @name Member types

	struct FlatCopyType { };

	using FieldAliasIndex = std::map <SpecificationVersionType, std::pair <std::map <std::string, std::string>, std::map <std::string, std::string>>>;

	///  @name Construction

	///  Copy or clone this node into @a executionContext.
	X3DBaseNode*
	copy (X3DExecutionContext* const executionContext) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	///  Flat copy this node into @a executionContext.
	X3DBaseNode*
	copy (X3DExecutionContext* const executionContext, const FlatCopyType &) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	///  Replace @a node by this node.
	void
	replace (X3DBaseNode* const baseNode);

	///  @name Field handling

	///  Process sceneGraph_changed event.
	void
	set_sceneGraph ();

	///  Remove field implementation.
	void
	removeField (const FieldIndex::iterator & iter, const bool userDefined, const bool removeParent);

	///  Returns the standard field name for @a alias.
	const std::string &
	getFieldName (const std::string & name) const;

	///  Returns the standard field name for @a alias depending on @a version.
	const std::string &
	getFieldName (const std::string & name, const SpecificationVersionType specificationVersion) const;

	///  @name Input/Output

	///  Inserts @a userDefinedField into @a ostream.
	void
	toStreamUserDefinedField (std::ostream & ostream, X3DFieldDefinition* const field, const size_t, const size_t) const;

	///  Inserts @a predefinedField into @a ostream.
	void
	toStreamField (std::ostream & ostream, X3DFieldDefinition* const field, const size_t, const size_t) const;

	///  @name Members

	X3DExecutionContext* executionContext; // This nodes ExecutionContext

	SFTime        nameOutput;
	NodeTypeArray type;

	FieldDefinitionArray fieldDefinitions;       // Pre-defined and user-defined field definitions
	FieldIndex           fields;                 // Pre-defined and user-defined fields
	FieldAliasIndex      fieldAliases;           // VRML names
	size_t               numUserDefinedFields;   // Number of user defined fields
	SFTime               fieldsOutput;

	bool   privateState;
	size_t cloneCount;
	size_t metaCloneCount;
	SFBool live;

	std::vector <std::string> comments;          // This nodes comments

};

template <class Type, class ValueType>
void
X3DBaseNode::setField (const std::string & name, const ValueType & value, const bool compare)
throw (Error <INVALID_FIELD>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	Type & field = getField <Type> (name);

	if (not compare or field not_eq value)
		field = value;
}

///  Return the field with @a name.
template <class Type>
Type &
X3DBaseNode::getField (const std::string & name)
throw (Error <INVALID_FIELD>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DFieldDefinition* const fieldDefinition = getField (name);

	Type* const field = dynamic_cast <Type*> (fieldDefinition);

	if (field)
		return *field;

	throw Error <INVALID_FIELD> ("Invalid type: Field '" + name + "' has type " + fieldDefinition -> getTypeName () + ".");
}

///  Return the field with @a name.
template <class Type>
const Type &
X3DBaseNode::getField (const std::string & name) const
throw (Error <INVALID_FIELD>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DFieldDefinition* const fieldDefinition = getField (name);

	Type* const field = dynamic_cast <Type*> (fieldDefinition);

	if (field)
		return *field;

	throw Error <INVALID_FIELD> ("Invalid type: Field '" + name + "' has type " + fieldDefinition -> getTypeName () + ".");
}

} // X3D
} // titania

#endif
