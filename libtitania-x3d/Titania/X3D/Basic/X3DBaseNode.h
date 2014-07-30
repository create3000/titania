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

#ifndef __TITANIA_X3D_BASIC_X3DBASE_NODE_H__
#define __TITANIA_X3D_BASIC_X3DBASE_NODE_H__

#include "../Base/Output.h"
#include "../Base/X3DReferenceObject.h"
#include "../Basic/FieldDefinitionArray.h"
#include "../Basic/NodeTypeArray.h"
#include "../Basic/X3DFieldDefinition.h"
#include "../Bits/Error.h"
#include "../Bits/TraverseType.h"
#include "../Bits/X3DConstants.h"
#include "../Fields/ArrayFields.h"
#include "../Fields/SFTime.h"
#include "../Fields/X3DScalar.h"
#include "../Routing/EventList.h"
#include "../Routing/NodeList.h"
#include "../Types/Struct.h"
#include "../Types/Time.h"

#include <map>

namespace titania {
namespace X3D {

typedef std::map <std::string, X3DFieldDefinition*> FieldIndex;

class X3DBrowser;
class X3DExecutionContext;

/**
 *  Class to represent an object that is the base for all nodes.
 */
class X3DBaseNode :
	public X3DReferenceObject
{
public:

	using X3DReferenceObject::addInterest;
	using X3DReferenceObject::removeInterest;

	/***
	 *  @name Construction
	 */

	///  Initializes this node.
	virtual
	void
	setup ();

	///  Returns whether this node is initialized.
	bool
	isInitialized () const
	{ return initialized; }

	///  Constructs a new node into @a executionContext.
	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const = 0;

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
	copy (X3DExecutionContext* const, const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	///  Assigns @a node. @a node must be of the same type as this node.
	void
	assign (const X3DBaseNode* const, const bool = false)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>);

	/***
	 *  @name Commons members
	 */

	///  Returns the current browser time for this frame.
	time_type
	getCurrentTime () const;

	///  Returns a pointer to the browser this node belongs to.
	X3DBrowser*
	getBrowser () const
	{ return browser; }

	///  Returns a pointer to the execution context this node belongs to.
	X3DExecutionContext*
	getExecutionContext () const
	{ return executionContext; }

	///  Returns a pointer to the root execution context this node belongs to.
	X3DExecutionContext*
	getRootContext () const;

	///  Returns the component name this node belongs to.
	virtual
	const std::string &
	getComponentName () const = 0;

	///  Returns a array of types of this node.
	const NodeTypeArray &
	getType () const
	throw (Error <DISPOSED>)
	{ return type; }

	///  Returns either a node declaration or the prototype of this node.
	virtual
	const X3DBaseNode*
	getDeclaration () const
	throw (Error <DISPOSED>);

	///  Returns the XML container field for this node type.
	virtual
	const std::string &
	getContainerField () const = 0;

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

	/***
	 *  @name Field handling
	 */

	///  Checks if a field with @a name exists for this node.
	bool
	hasField (const std::string &) const
	throw (Error <DISPOSED>);

	///  Checks whether @a field has the default value for this node type.  The @a field must be of this node.
	bool
	isDefaultValue (const X3DFieldDefinition* const) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Sets the value of the field with @a name.  If the third parameter is true, an event is only generated if the old
	///  value is not the new value.
	template <class FieldType, class ValueType>
	void
	setField (const std::string &, const ValueType &, const bool = false)
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
	getField (const std::string & name) const
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
	X3DFieldDefinition*
	getField (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Returns whether this node can have user defined fields.
	virtual
	bool
	canUserDefinedFields () const
	{ return false; }

	///  Adds @a field to the set of user defined fields of this node.
	virtual
	void
	addUserDefinedField (const AccessType, const std::string &, X3DFieldDefinition* const)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>);

	///  Replaces the field @a name by @a field. @a accessType and @a name will be assigned to @a field.
	virtual
	void
	updateUserDefinedField (const AccessType, const std::string &, X3DFieldDefinition* const)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>);

	///  Removes the field named @a name from the set of user defined fields of this node.
	virtual
	void
	removeUserDefinedField (const std::string &)
	throw (Error <DISPOSED>);

	///  Returns an array with all pre defined fields of this node.
	FieldDefinitionArray
	getPreDefinedFields () const;

	///  Returns an array with all user defined fields of this node.
	FieldDefinitionArray
	getUserDefinedFields () const;

	///  Return all field definition for this node, that is all predefined field and user defined fields.
	const FieldDefinitionArray &
	getFieldDefinitions () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return fieldDefinitions; }

	///  If the node has a XML CDATA field it is returned otherwise a null pointer.
	virtual
	MFString*
	getCDATA ()
	{ return nullptr; }

	///  If the node has a XML CDATA field it is returned otherwise a null pointer.
	virtual
	const MFString*
	getCDATA () const
	{ return nullptr; }

	/***
	 *  @name Clone handling
	 */

	///  Marks this node as private or not.  The default is false.
	void
	isPrivate (const bool);

	///  Returns whether this node is marked private.
	bool
	isPrivate () const
	{ return private_; }

	///  Returns the number of clones of this node used in the scene graph.
	virtual
	size_t
	getCloneCount () const final override
	{ return cloneCount; }

	///  Increments the clone count by @a count.
	virtual
	void
	addClones (const size_t count) final override
	{ cloneCount += count; }

	///  Decrements the clone count by @a count.
	virtual
	void
	removeClones (const size_t count) final override
	{ cloneCount -= count; }

	/***
	 *  @name Tool support
	 */

	///  Replace the node by its tool if it supports any.
	virtual
	void
	addTool ()
	{ }

	///  Reverses the effect of addTool.  For some tools @a really must be true.
	virtual
	void
	removeTool (const bool = false)
	{ }

	/***
	 *  @name Event handling
	 */

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

	///  Marks this node as tainted, i.e. all interests of this node will be processed later. 
	virtual
	void
	addEvent () override;

	///  This function is called by the router when all events are processed.  You normally do not need to call this
	///  function directly.
	void
	eventsProcessed ();

	///  Returns true if any field has a input or output route otherwise false.
	bool
	hasRoutes () const;

	/***
	 *  @name Interest service
	 */

	///  Adds an interest to this object.  The @a requester is then notified about a change of this object.  This version
	///  of the function effectivly calls addEvent on @a requester.
	void
	addInterest (X3DBaseNode* const requester) const
	{ addInterest (requester, (void (X3DBaseNode::*)()) & X3DBaseNode::addEvent); }

	///  Adds an interest to this object.  The @a requester is then notified about a change of this object.  This version
	///  of the function effectivly calls addEvent on @a requester.
	template <class ValueType>
	void
	addInterest (X3DField <ValueType>* const requester) const
	{ addInterest (requester, (void (X3DField <ValueType>::*) ()) & X3DField <ValueType>::addEvent); }

	///  Adds an interest to this object.  The @a requester is then notified about a change of this object.
	template <class ValueType>
	void
	addInterest (X3DField <ValueType> & requester) const
	{ addInterest (&requester, (void (X3DField <ValueType>::*) ()) & X3DField <ValueType>::addEvent); }

	///  Removes an interest from this object.  The @a requester will not further notified about a change of this object.
	void
	removeInterest (X3DBaseNode* const requester) const
	{ removeInterest (requester, (void (X3DBaseNode::*)()) & X3DBaseNode::addEvent); }

	///  Removes an interest from this object.  The @a requester will not further notified about a change of this object.
	template <class ValueType>
	void
	removeInterest (X3DField <ValueType>* const requester) const
	{ removeInterest (requester, (void (X3DField <ValueType>::*) ()) & X3DField <ValueType>::addEvent); }

	///  Removes an interest from this object.  The @a requester will not further notified about a change of this object.
	template <class ValueType>
	void
	removeInterest (X3DField <ValueType> & requester) const
	{ removeInterest (&requester, (void (X3DField <ValueType>::*) ()) & X3DField <ValueType>::addEvent); }

	/***
	 *  @name Traversal handling
	 */

	///  Traverses the scene graph depending on @a type.
	virtual
	void
	traverse (const TraverseType)
	{ }

	/***
	 *  @name Comment handling
	 */

	///  Adds comments for this node.  The comments will be printed before the '}'.
	void
	addInnerComments (const std::vector <std::string> & value)
	{ comments .insert (comments .end (), value .begin (), value .end ()); }

	///  Returns the comments for this node.
	const std::vector <std::string> &
	getInnerComments () const
	{ return comments; }

	/***
	 *  @name Input/Output
	 */

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
	toStream (std::ostream &) const override;

	///  Inserts this object into @a ostream in X3D XML Encoding.
	virtual
	void
	toXMLStream (std::ostream &) const override;

	/***
	 *  @name Destruction
	 */

	///  Disposes this node.  You normally do not need to call this function directly.
	virtual
	void
	dispose () override;

	///  Destructs this node.
	virtual
	~X3DBaseNode ();


protected:

	/***
	 *  @name Construction
	 */

	///  Constructs new X3DBaseNode.
	X3DBaseNode (X3DBrowser* const = nullptr, X3DExecutionContext* const = nullptr);

	///  Initializes this node.
	virtual
	void
	initialize ()
	{ }

	/***
	 *  @name Common members
	 */

	///  Adds @a value to the type of this node.
	void
	addType (const X3DConstants::NodeType value)
	{ type .emplace_back (value); }

	/***
	 *  @name Field handling
	 */

	///  Adds @a field to the set of fields of this node.
	void
	addField (const AccessType, const std::string &, X3DFieldDefinition &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>);

	///  Adds a field for X3D @a version.
	void
	addField (const VersionType, const std::string &, const std::string &)
	throw (Error <INVALID_NAME>,
	       Error <DISPOSED>);

	///  Returns an array of all fields with non default value.
	FieldDefinitionArray
	getChangedFields () const;

	///  Adds a private field to this node.
	virtual
	void
	addChild (X3DChildObject &) final override;

	///  Removes a private field to this node.  If the reference count of @a object becomes 0 the object will be disposed.
	virtual
	void
	removeChild (X3DChildObject &) final override;

	/***
	 *  @name Tool support
	 */

	///  Replaces this node by @a tool.
	void
	addTool (X3DBaseNode* const);

	///  Replaces this node by @a node.
	void
	removeTool (X3DBaseNode* const);

	/***
	 *  @name Event handling
	 */

	///  If extended event handling is set to true, initializeOnly field behave like inputOutput fields.  Otherwise
	///  initializeOnly fields will not process any events.  The default is true.
	void
	setExtendedEventHandling (const bool value)
	{ extendedEventHandling = value; }

	///  Returns whether extended event handling is enabled.
	bool
	getExtendedEventHandling () const
	{ return extendedEventHandling; }

	///  Handler that is called when a child (field) of this node should be marked tainted.
	virtual
	void
	addEvent (X3DChildObject* const) override;

	///  Handler that is called when a child (field) of this node should be added to the event queue.
	virtual
	void
	addEvent (X3DChildObject* const, const EventPtr &) override;


private:

	/***
	 *  @name Member types
	 */

	struct FlatCopyType { };

	using FieldAliasIndex = std::map <VersionType, std::pair <std::map <std::string, std::string>, std::map <std::string, std::string>>>;

	/***
	 *  @name Construction
	 */

	///  Copy or clone this node into @a executionContext.
	X3DBaseNode*
	copy (X3DExecutionContext* const) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	///  Flat copy this node into @a executionContext.
	X3DBaseNode*
	copy (X3DExecutionContext* const executionContext, const FlatCopyType &) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	/***
	 *  @name Field handling
	 */

	///  Replace @a node by this node.
	void
	replace (X3DBaseNode* const);

	///  Remove field @a name from the set of fields.
	void
	removeField (const std::string &)
	throw (Error <DISPOSED>);

	///  Remove field implementation.
	void
	removeField (const FieldIndex::iterator &, const bool, const bool);

	///  Returns the standard field name for @a alias.
	const std::string &
	getFieldName (const std::string &) const;

	///  Returns the standard field name for @a alias depending on @a version.
	const std::string &
	getFieldName (const std::string &, const VersionType) const;

	/***
	 *  @name Event handling
	 */

	///  Removes all fields from the event queue.
	void
	removeEvents ();

	/***
	 *  @name Input/Output
	 */

	///  Inserts @a userDefinedField into @a ostream.
	void
	toStreamUserDefinedField (std::ostream &, X3DFieldDefinition* const, const size_t, const size_t) const;

	///  Inserts @a predefinedField into @a ostream.
	void
	toStreamField (std::ostream &, X3DFieldDefinition* const, const size_t, const size_t) const;

	/***
	 *  @name Members
	 */

	X3DBrowser* const          browser;          // This nodes Browser
	X3DExecutionContext* const executionContext; // This nodes ExecutionContext

	NodeTypeArray type;

	FieldDefinitionArray fieldDefinitions;       // Pre-defined and user-defined field definitions
	FieldIndex           fields;                 // Pre-defined and user-defined fields
	FieldAliasIndex      fieldAliases;           // VRML names
	size_t               numUserDefinedFields;   // Number of user defined fields
	ChildObjectSet       children;               // Internal used fields

	bool                  initialized;
	bool                  private_;
	size_t                cloneCount;
	SFBool                live;
	bool                  extendedEventHandling; // Handle initializeOnlys as input events
	NodeId                nodeId;                // Router eventsProcessed id
	std::vector <EventId> events;

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

} // X3D
} // titania

#endif
