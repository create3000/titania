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
#include "../Basic/X3DContext.h"
#include "../Basic/X3DFieldDefinition.h"
#include "../Bits/Error.h"
#include "../Bits/X3DConstants.h"
#include "../Fields/SFTime.h"
#include "../Types/Time.h"

#include <map>
#include <set>

namespace titania {
namespace X3D {

typedef std::map <std::string, X3DFieldDefinition*> FieldsMap;

class X3DBrowser;
class X3DExecutionContext;

class X3DBaseNode :
	public X3DChildObject
{
public:

	SFTime initialized;
	Output shutdown;

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const = 0;

	virtual
	X3DBaseNode*
	clone (X3DExecutionContext* const) const;

	virtual
	X3DBaseNode*
	copy (X3DExecutionContext* const) const;

	virtual
	time_type
	getCurrentTime () const;

	virtual
	X3DBrowser*
	getBrowser () const
	{ return browser; }

	virtual
	X3DExecutionContext*
	getExecutionContext () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return executionContext; }

	size_t
	getNumClones () const;

	const std::string &
	getComponentName () const;

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) override;

	virtual
	const X3DBaseNode*
	getType () const
	throw (Error <DISPOSED>);

	const NodeTypeArray &
	getNodeType () const;

	virtual
	X3DBaseNode*
	getLocalNode ();

	X3DFieldDefinition*
	getField (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const FieldDefinitionArray &
	getFieldDefinitions () const;

	void
	addUserDefinedField (const AccessType, const basic::id &, X3DFieldDefinition* const);

	FieldDefinitionArray
	getUserDefinedFields () const;

	virtual
	void
	setup ();

	virtual
	void
	processEvents (ChildObjectSet &) final;

	virtual
	void
	processInterests () final;

	virtual
	void
	eventsProcessed ();

	///  @name Traversal handling

	virtual
	void
	pick () { }

	virtual
	void
	traverse () { }

	virtual
	void
	display () { }

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

	///  @name Dispose

	virtual
	void
	dispose () override;

	///  @name Destructor

	virtual
	~X3DBaseNode ();


protected:

	X3DBaseNode (X3DBrowser* const = nullptr, X3DExecutionContext* const = nullptr);

	void
	setComponent (const std::string &);

	void
	setTypeName (const std::string &);

	void
	addNodeType (const X3DConstants::NodeType);

	void
	addField (const AccessType, const basic::id &, X3DFieldDefinition &);

	void
	removeField (const basic::id &);

	void
	addFieldAlias (const std::string &, const std::string &);

	const FieldsMap &
	getFields () const;

	virtual
	void
	initialize () { }


private:

	typedef std::map <std::string, std::string> FieldAliasesMap;

	const std::string &
	getFieldName (const std::string &) const;

	FieldDefinitionArray
	getInitializeableFields (const bool = false) const;

	virtual
	void
	registerEvent (X3DChildObject* const) final;

	virtual
	void
	registerInterest (X3DChildObject* const) final;

	X3DBrowser* const          browser;
	X3DExecutionContext* const executionContext;

	std::string   component;
	std::string   typeName;
	NodeTypeArray nodeType;

	FieldDefinitionArray fieldDefinitions;      // pre-defined and user-defined field definitions
	FieldsMap            fields;                // pre-defined and user-defined fields
	FieldAliasesMap      fieldAliases;
	size_t               numUserDefinedFields;

	ChildObjectSet events;
	ChildObjectSet interests;

};

template <class CharT, class Traits>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, X3DBaseNode* node)
{
	//	auto node = executionContext -> createNode ("typeId")
	//
	//	node -> fromStream (istream);
	//
	return istream;
}

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const X3DBaseNode* node)
{
	if (node)
		node -> toStream (ostream);
	else
		ostream << "NULL";

	return ostream;
}

extern template std::istream & operator >> (std::istream &, X3DBaseNode*);
extern template std::ostream & operator << (std::ostream &, const X3DBaseNode*);

} // X3D
} // titania

#endif
