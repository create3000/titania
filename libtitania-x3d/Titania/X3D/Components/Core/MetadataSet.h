/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_COMPONENTS_CORE_METADATA_SET_H__
#define __TITANIA_X3D_COMPONENTS_CORE_METADATA_SET_H__

#include "../Core/X3DMetadataObject.h"
#include "../Core/X3DNode.h"

namespace titania {
namespace X3D {

class MetadataSet :
	virtual public X3DNode, public X3DMetadataObject
{
public:

	///  @name Construction

	MetadataSet (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Fields

	MFNode &
	value ()
	{ return *fields .value; }

	const MFNode &
	value () const
	{ return *fields .value; }
	
	///  @name Member access

	const X3DPtr <MetadataSet> &
	getSet (const std::string & name, const bool = false)
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	template <class Type>
	void
	setMetaData (const std::string & name, const Type & value)
	throw (Error <DISPOSED>);

	///  Return the field with @a name.
	template <class Type>
	Type &
	getMetaData (const std::string & name, const bool = false)
	throw (Error <INVALID_FIELD>,
	       Error <INVALID_NAME>,
	       Error <DISPOSED>);

	///  Return the field with @a name.
	template <class Type>
	const Type &
	getMetaData (const std::string & name) const
	throw (Error <INVALID_FIELD>,
	       Error <INVALID_NAME>,
	       Error <DISPOSED>);

	X3DFieldDefinition*
	getMetaData (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <DISPOSED>);

	void
	removeMetaData (const std::string &)
	throw (Error <DISPOSED>);

	void
	removeMetaData ()
	throw (Error <DISPOSED>);

	///  @name Destruction

	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	addMetaData (const SFNode &);

	///  @name Event handlers

	void
	set_value ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFNode* const value;
	};

	Fields fields;

	std::map <std::string, X3DPtr <MetadataSet>> setIndex;
	std::map <std::string, X3DFieldDefinition*>  fieldIndex;

};

template <class Type>
void
MetadataSet::setMetaData (const std::string &, const Type &)
throw (Error <DISPOSED>)
{ }

template <>
void
MetadataSet::setMetaData <MFBool> (const std::string &, const MFBool &)
throw (Error <DISPOSED>);

template <>
void
MetadataSet::setMetaData <MFDouble> (const std::string &, const MFDouble &)
throw (Error <DISPOSED>);

template <>
void
MetadataSet::setMetaData <MFFloat> (const std::string &, const MFFloat &)
throw (Error <DISPOSED>);

template <>
void
MetadataSet::setMetaData <MFInt32> (const std::string &, const MFInt32 &)
throw (Error <DISPOSED>);

template <>
void
MetadataSet::setMetaData <MFString> (const std::string &, const MFString &)
throw (Error <DISPOSED>);

///  Return the field with @a name.
template <class Type>
Type &
MetadataSet::getMetaData (const std::string & name, const bool create)
throw (Error <INVALID_FIELD>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	try
	{
		X3DFieldDefinition* const fieldDefinition = getMetaData (name);

		Type* const field = dynamic_cast <Type*> (fieldDefinition);

		if (field)
			return *field;

		throw Error <INVALID_FIELD> ("Invalid type: Field '" + name + "' has type " + fieldDefinition -> getTypeName () + ".");
	}
	catch (const X3DError &)
	{
		if (create)
		{
			setMetaData <Type> (name, Type ());

			return getMetaData <Type> (name);
		}

		throw;
	}
}

///  Return the field with @a name.
template <class Type>
const Type &
MetadataSet::getMetaData (const std::string & name) const
throw (Error <INVALID_FIELD>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	X3DFieldDefinition* const fieldDefinition = getMetaData (name);

	Type* const field = dynamic_cast <Type*> (fieldDefinition);

	if (field)
		return *field;

	throw Error <INVALID_FIELD> ("Invalid type: Field '" + name + "' has type " + fieldDefinition -> getTypeName () + ".");
}

} // X3D
} // titania

#endif
