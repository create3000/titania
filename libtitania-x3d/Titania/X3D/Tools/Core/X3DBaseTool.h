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

#ifndef __TITANIA_X3D_TOOLS_CORE_X3DBASE_TOOL_H__
#define __TITANIA_X3D_TOOLS_CORE_X3DBASE_TOOL_H__

#include "../Core/X3DToolObject.h"
#include "../../Basic/X3DBaseNode.h"

namespace titania {
namespace X3D {

class X3DBaseTool :
	virtual public X3DBaseNode,
	public X3DToolObject
{
public:

	///  @name X3DOutput

	virtual
	bool
	checkInterest (const void* const object, const void* const memberFunction) const final override
	{ return node -> checkInterest (object, memberFunction); }

	virtual
	bool
	insertInterest (const Requester & function, const void* const object, const void* const memberFunction) const final override
	{ return node -> insertInterest (function, object, memberFunction); }

	virtual
	void
	insertInput (const X3DInput* const input, const void* const memberFunction) const final override
	{ node -> insertInput (input, memberFunction); }

	virtual
	void
	eraseInterest (const void* const object, const void* const memberFunction) const final override
	{ node -> eraseInterest (object, memberFunction); }

	virtual
	void
	eraseInput (const X3DInput* const input, void* const memberFunction) const final override
	{ node -> eraseInput (input, memberFunction); }

	///  @name X3DObject

	virtual
	size_t
	getId () const final override
	{ return node -> getId (); }

	virtual
	void
	setName (const std::string & value) final override
	{ node -> setName (value); }

	virtual
	const std::string &
	getName () const final override
	{ return node -> getName (); }

	virtual
	const SFTime &
	name_changed () const final override
	{ return node -> name_changed (); }

	virtual
	void
	setUserData (const UserDataPtr & value) final override
	{ node -> setUserData (value); }

	virtual
	const UserDataPtr &
	getUserData () const final override
	{ return node -> getUserData (); }

	///  @name X3DBaseNode

	///  Copys this node and sets the execution context to @a executionContext.
	virtual
	X3DBaseNode*
	copy (const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) override
	{ return node -> copy (type); }

	///  Copys this node and sets the execution context to @a executionContext.
	virtual
	X3DBaseNode*
	copy (X3DExecutionContext* const executionContext, const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) override
	{ return node -> copy (executionContext, type); }

	virtual
	void
	assign (const X3DBaseNode* const other, const bool compare = false)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>) final override
	{ node -> assign (other, compare); }

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Field handling

	///  Checks if a field with @a name exists for this node.
	virtual
	bool
	hasField (const std::string & name) const
	throw (Error <DISPOSED>) final override
	{ return node -> hasField (name); }

	///  Checks whether @a field has the default value for this node type.  The @a field must be of this node.
	virtual
	bool
	isDefaultValue (const X3DFieldDefinition* const field) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return node -> isDefaultValue (field); }

	///  Return the field with @a name.
	virtual
	X3DFieldDefinition*
	getField (const std::string & name) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  Returns whether this node can have user defined fields.
	virtual
	bool
	canUserDefinedFields () const final override
	{ return node -> canUserDefinedFields (); }

	///  Replaces the set of user defined fields of this node with @a userDefinedFields.
	virtual
	void
	setUserDefinedFields (const X3D::FieldDefinitionArray & fieldDefinitions)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>) final override
	{ node -> setUserDefinedFields (fieldDefinitions); }

	///  Adds @a field to the set of user defined fields of this node.
	virtual
	void
	addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>) final override
	{ node -> addUserDefinedField (accessType, name, field); }

	///  Removes the field named @a name from the set of user defined fields of this node.
	virtual
	void
	removeUserDefinedField (const std::string & name)
	throw (Error <DISPOSED>) final override
	{ node -> removeUserDefinedField (name); }

	///  Returns an array with all pre defined fields of this node.
	virtual
	FieldDefinitionArray
	getPreDefinedFields () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return node -> getPreDefinedFields (); }

	///  Returns an array with all user defined fields of this node.
	virtual
	FieldDefinitionArray
	getUserDefinedFields () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return node -> getUserDefinedFields (); }

	///  Returns an array of all fields with non default value.
	virtual
	FieldDefinitionArray
	getChangedFields () const final override
	{ return node -> getChangedFields (); }

	///  Return all field definition for this node, that is all predefined field and user defined fields.
	virtual
	const FieldDefinitionArray &
	getFieldDefinitions () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return node -> getFieldDefinitions (); }

	///  Field that processes its interests when the set of fields has changed.
	virtual
	const SFTime &
	fields_changed () const final override
	{ return node -> fields_changed (); }

	///  If the node has a XML CDATA field it is returned otherwise a null pointer.
	virtual
	MFString*
	getSourceText () final override
	{ return node -> getSourceText (); }

	///  If the node has a XML CDATA field it is returned otherwise a null pointer.
	virtual
	const MFString*
	getSourceText () const final override
	{ return node -> getSourceText (); }

	///  @name Clone handling

	// Clone handling must not be overloaded!

	///  Returns the number of clones of this node used in the meta data nodes.
	virtual
	size_t
	getMetaCloneCount () const final override
	{ return node -> getMetaCloneCount (); }

	///  Increments the meta clone count by @a count.
	virtual
	void
	addMetaCloneCount (const size_t count) final override
	{ return node -> addMetaCloneCount (count); }

	///  Decrements the meta clone count by @a count.
	virtual
	void
	removeMetaCloneCount (const size_t count) final override
	{ return node -> removeMetaCloneCount (count); }

	///  @name Tool support

	virtual
	void
	addTool () override
	{ }

	virtual
	void
	removeTool (const bool = false) override;

	///  @name Event handling

	virtual
	SFBool &
	isLive () final override
	{ return node -> isLive (); }

	virtual
	const SFBool &
	isLive () const final override
	{ return node -> isLive (); }

	virtual
	void
	beginUpdate ()
	throw (Error <DISPOSED>) final override
	{ return node -> beginUpdate (); }

	virtual
	void
	endUpdate ()
	throw (Error <DISPOSED>) final override
	{ return node -> endUpdate (); }

	///  Returns true if any field has a input or output route otherwise false.
	virtual
	bool
	hasRoutes () const final override
	{ return node -> hasRoutes (); }

	virtual
	void
	addEvent () final override;
	
	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override
	{ }

	///  @name Comment handling

	///  Adds comments for this node.  The comments will be printed before the '}'.
	virtual
	void
	addInnerComments (const std::vector <std::string> & value) final override
	{ node -> addInnerComments (value); }

	///  Returns the comments for this node.
	virtual
	const std::vector <std::string> &
	getInnerComments () const final override
	{ return node -> getInnerComments (); }

	///  @name Input/Output

	// Input/Output must not be overloaded!

	///  @name Destruction

	virtual
	const Output &
	shutdown () const final override
	{ return node -> shutdown (); }

	virtual
	const Output &
	disposed () const final override
	{ return node -> disposed (); }

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	X3DBaseTool ();

	X3DBaseTool (X3DBaseNode* const);

	virtual
	void
	initialize () override;

	///  @name Member access

	template <class Type>
	Type*
	getNode ()
	{ return dynamic_cast <Type*> (node); }

	template <class Type>
	const Type*
	getNode () const
	{ return dynamic_cast <const Type*> (node); }


private:

	///  @name Members

	X3DBaseNode* const node;

};

} // X3D
} // titania

#endif
