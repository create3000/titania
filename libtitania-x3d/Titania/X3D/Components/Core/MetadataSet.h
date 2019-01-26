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

#ifndef __TITANIA_X3D_COMPONENTS_CORE_METADATA_SET_H__
#define __TITANIA_X3D_COMPONENTS_CORE_METADATA_SET_H__

#include "../../Execution/X3DExecutionContext.h"
#include "../Core/X3DMetadataObject.h"
#include "../Core/X3DNode.h"

namespace titania {
namespace X3D {

class MetadataSet :
	virtual public X3DNode, public X3DMetadataObject
{
public:

	///  @name Construction

	MetadataSet (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	MFNode &
	value ()
	{ return *fields .value; }

	const MFNode &
	value () const
	{ return *fields .value; }
	
	///  @name Member access

	///  Return the metadata with where name is @a name if it exists otherwise create it.
	///  throws Error <INVALID_NAME>, Error <DISPOSED>
	template <class Type>
	X3DPtr <Type>
	createValue (const std::string & name)
	{ return X3DPtr <Type> (getValue <Type> (name, false)); }

	///  Return the metadata with where name is @a name if it exists otherwise throw an exception.
	///  throws Error <INVALID_NODE>, Error <INVALID_NAME>, Error <DISPOSED>
	template <class Type>
	X3DPtr <Type>
	getValue (const std::string & name)
	{ return X3DPtr <Type> (getValue <Type> (name, true)); }

	///  throws Error <DISPOSED>
	void
	removeValue (const std::string & name);

	///  @name Destruction

	virtual
	void
	dispose () final override;


protected:

	// @name Friends

	friend class X3DNode;

	///  Return the metadata with where name is @a name.
	///  throws Error <INVALID_NODE>, Error <INVALID_NAME>, Error <DISPOSED>
	template <class Type>
	Type*
	getValue (const std::string & name, const bool throw_);


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	addValue (const SFNode &);

	///  throws Error <INVALID_NAME>, Error <DISPOSED>
	X3DMetadataObject*
	getObject (const std::string & name) const;

	void
	setValue (const std::string & name, X3DMetadataObject* const metadataObject);

	///  throws Error <DISPOSED>
	void
	removeValues ();

	///  @name Event handlers

	void
	set_value ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFNode* const value;
	};

	Fields fields;

	std::map <std::string, X3DMetadataObject*> metadataIndex;
	MFNode                                     nodes;

};

///  Return the field with @a name.
///  throws Error <INVALID_NODE>, Error <INVALID_NAME>, Error <DISPOSED>
template <class Type>
Type*
MetadataSet::getValue (const std::string & name, const bool throw_)
{
	try
	{
	   const auto metadataObject = dynamic_cast <Type*> (getObject (name));

		if (metadataObject)
			return metadataObject;
		
	   if (throw_)
	      throw Error <INVALID_NODE> ("MetaData " + name + " has invalid type.");

		removeValue (name);
	}
	catch (const Error <INVALID_NAME> &)
	{
	   if (throw_)
	      throw;
	}
	catch (...)
	{
		throw;
	}

	const auto metadataObject = getExecutionContext () -> createNode <Type> ();

	value () .emplace_back (metadataObject);

	setValue (name, metadataObject);

	return metadataObject;
}

} // X3D
} // titania

#endif
