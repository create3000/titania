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
#include "../Base/X3DChildObject.h"
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
#include "../Types/Time.h"

#include <map>

namespace titania {
namespace X3D {

typedef std::map <std::string, X3DFieldDefinition*> FieldIndex;

class X3DBrowser;
class X3DExecutionContext;

struct FlattCopyType { };
struct DeepCopyType { };

class X3DBaseNode :
	public X3DChildObject
{
public:

	using X3DChildObject::addInterest;
	using X3DChildObject::removeInterest;

	///  @name Construction

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const = 0;

	virtual
	X3DBaseNode*
	clone (X3DExecutionContext* const) const
	throw (Error <INVALID_NAME>,
	       Error <NODE_NOT_AVAILABLE>,
	       Error <NOT_SUPPORTED>);

	virtual
	X3DBaseNode*
	copy (X3DExecutionContext* const) const
	throw (Error <INVALID_NAME>,
	       Error <NODE_NOT_AVAILABLE>,
	       Error <NOT_SUPPORTED>);

	X3DBaseNode*
	copy (X3DExecutionContext* const, const FlattCopyType &) const
	throw (Error <NOT_SUPPORTED>);

	X3DBaseNode*
	copy (X3DExecutionContext* const, const DeepCopyType &) const
	throw (Error <NOT_SUPPORTED>);

	void
	assign (const X3DBaseNode* const)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>);

	virtual
	void
	setup ();

	bool
	isInitialized () const
	{ return initialized; }

	///  @name Commons members

	time_type
	getCurrentTime () const;

	virtual
	X3DBrowser*
	getBrowser () const
	{ return browser; }

	X3DExecutionContext*
	getExecutionContext () const
	{ return executionContext; }

	virtual
	const std::string &
	getComponentName () const = 0;

	virtual
	const X3DBaseNode*
	getType () const
	throw (Error <DISPOSED>);

	const NodeTypeArray &
	getNodeType () const
	{ return nodeType; }

	virtual
	const std::string &
	getContainerField () const = 0;

	virtual
	X3DBaseNode*
	getInnerNode ()
	{ return this; }

	///  @name Field handling

	bool
	existsField (X3DFieldDefinition* const) const;

	template <class FieldType, class ValueType>
	void
	setField (const std::string &, const ValueType &, const bool = false)
	throw (Error <INVALID_FIELD>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	template <class Type>
	Type &
	getField (const std::string & name)
	throw (Error <INVALID_FIELD>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{
		X3DFieldDefinition* const field = getField (name);
		Type* const               type  = dynamic_cast <Type*> (field);

		if (type)
			return *type;

		throw Error <INVALID_FIELD> ("Invalid type: Field '" + name + "' has type " + field -> getTypeName () + ".");
	}

	template <class Type>
	const Type &
	getField (const std::string & name) const
	throw (Error <INVALID_FIELD>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{
		X3DFieldDefinition* const field = getField (name);
		Type* const               type  = dynamic_cast <Type*> (field);

		if (type)
			return *type;

		throw Error <INVALID_FIELD> ("Invalid type: Field '" + name + "' has type " + field -> getTypeName () + ".");
	}

	X3DFieldDefinition*
	getField (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const FieldDefinitionArray &
	getFieldDefinitions () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return fieldDefinitions; }

	virtual
	MFString*
	getCData ()
	{ return nullptr; }

	virtual
	const MFString*
	getCData () const
	{ return nullptr; }

	virtual
	void
	addUserDefinedField (const AccessType, const std::string &, X3DFieldDefinition* const);

	virtual
	void
	removeUserDefinedField (X3DFieldDefinition* const);

	FieldDefinitionArray
	getPreDefinedFields () const;

	FieldDefinitionArray
	getUserDefinedFields () const;

	virtual
	bool
	hasUserDefinedFields () const
	{ return false; }

	bool
	isDefaultValue (const X3DFieldDefinition* const) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Special functions

	size_t
	getNumClones () const;

	bool
	hasRoutes () const;

	bool
	isInternal () const
	{ return internal; }

	void
	isInternal (const bool);

	bool
	isSaved () const
	{ return saved; }

	virtual
	void
	saveState ();

	virtual
	void
	restoreState ();

	///  @name Tool handling

	virtual
	void
	addTool ()
	{ }

	virtual
	void
	removeTool (const bool = false)
	{ }

	///  @name Event handling

	void
	addInterest (X3DBaseNode* const object) const
	{ addInterest (object, (void (X3DBaseNode::*)()) & X3DBaseNode::addEvent); }

	template <class ValueType>
	void
	addInterest (X3DField <ValueType>* const object) const
	{ addInterest (object, (void (X3DField <ValueType>::*) ()) & X3DField <ValueType>::addEvent); }

	template <class ValueType>
	void
	addInterest (X3DField <ValueType> & object) const
	{ addInterest (&object); }

	void
	removeInterest (X3DBaseNode* const object) const
	{ removeInterest (object, (void (X3DBaseNode::*)()) & X3DBaseNode::addEvent); }

	template <class ValueType>
	void
	removeInterest (X3DField <ValueType>* const object) const
	{ removeInterest (object, (void (X3DField <ValueType>::*) ()) & X3DField <ValueType>::addEvent); }

	template <class ValueType>
	void
	removeInterest (X3DField <ValueType> & object) const
	{ removeInterest (&object); }

	///  @name Event handling

	virtual
	void
	addEvent (X3DChildObject* const) override;

	virtual
	void
	addEvent (X3DChildObject* const, const EventPtr &) override;

	virtual
	void
	addEvent () override;

	void
	eventsProcessed ();

	///  @name Traversal handling

	virtual
	void
	traverse (const TraverseType)
	{ }

	///  @name Comment handling

	void
	addInnerComments (const std::vector <std::string> & value)
	{ comments .insert (comments .end (), value .begin (), value .end ()); }

	const std::vector <std::string> &
	getInnerComments () const
	{ return comments; }

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	virtual
	void
	toStream (std::ostream &) const override;

	virtual
	void
	toXMLStream (std::ostream &) const override;

	///  @name Destruction

	virtual
	const Output &
	shutdown () const
	{ return shutdownOutput; }

	virtual
	void
	dispose () override;

	virtual
	~X3DBaseNode ();


protected:

	///  @name Construction

	X3DBaseNode (X3DBrowser* const = nullptr, X3DExecutionContext* const = nullptr);

	///  @name Misc

	void
	addNodeType (const X3DConstants::NodeType value)
	{ nodeType .push_back (value); }

	void
	addField (const AccessType, const std::string &, X3DFieldDefinition &);

	void
	addField (const VersionType, const std::string &, const std::string &);

	void
	removeField (const std::string &);

	FieldDefinitionArray
	getChangedFields () const;

	virtual
	void
	addChild (X3DChildObject &) final override;

	virtual
	void
	removeChild (X3DChildObject &) final override;

	void
	setExtendedEventHandling (const bool value)
	{ extendedEventHandling = value; }

	bool
	getExtendedEventHandling () const
	{ return extendedEventHandling; }

	virtual
	void
	initialize ()
	{ }

	void
	addTool (X3DBaseNode* const);

	void
	removeTool (X3DBaseNode* const);


private:

	using FieldAliasIndex = std::map <VersionType, std::pair <std::map <std::string, std::string>, std::map <std::string, std::string>>  >;

	///  @name Misc

	void
	replace (X3DBaseNode* const, const std::set <const X3DFieldDefinition*> & = { });

	void
	removeField (const FieldIndex::iterator &, const bool);

	const std::string &
	getFieldName (const std::string &) const;

	const std::string &
	getFieldName (const std::string &, const VersionType) const;

	void
	removeEvents ();

	///  @name Input/Output

	void
	toStreamUserDefinedField (std::ostream &, X3DFieldDefinition* const, const size_t, const size_t) const;

	void
	toStreamField (std::ostream &, X3DFieldDefinition* const, const size_t, const size_t) const;

	///  @name Members

	X3DBrowser* const          browser;          // This nodes Browser
	X3DExecutionContext* const executionContext; // This nodes ExecutionContext

	NodeTypeArray nodeType;

	FieldDefinitionArray fieldDefinitions;       // Pre-defined and user-defined field definitions
	FieldIndex           fields;                 // Pre-defined and user-defined fields
	FieldAliasIndex      fieldAliases;           // VRML names
	size_t               numUserDefinedFields;   // Number of user defined fields
	ChildObjectSet       children;               // Internal used fields

	bool                  internal;              // Is this node interally used
	bool                  saved;                 // Is this node hidden and saved
	bool                  extendedEventHandling; // Tool initializeOnlys as input events
	NodeId                nodeId;                // Router eventsProcessed id
	std::vector <EventId> events;

	std::vector <std::string> comments;          // This nodes comments

	bool   initialized;
	Output shutdownOutput;                       // Shutdown service

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

	if (compare)
	{
		if (field not_eq value)
			field = value;
	}
	else
		field = value;
}

} // X3D
} // titania

#endif
