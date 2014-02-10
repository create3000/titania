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

#ifndef __TITANIA_X3D_FIELDS_X3DMFNODE_H__
#define __TITANIA_X3D_FIELDS_X3DMFNODE_H__

#include "../Basic/X3DArrayField.h"
#include "../Fields/X3DSFNode.h"
#include "../InputOutput/Generator.h"

#include <Titania/Utility/Adapter.h>

namespace titania {
namespace X3D {

template <class ValueType>
class X3DMFNode :
	public X3DArrayField <X3DSFNode <ValueType>> 
{
private:

	typedef X3DArrayField <X3DSFNode <ValueType>>  ArrayField;


public:

	using X3DArrayField <X3DSFNode <ValueType>> ::operator =;
	using X3DArrayField <X3DSFNode <ValueType>> ::addInterest;
	using X3DArrayField <X3DSFNode <ValueType>> ::cbegin;
	using X3DArrayField <X3DSFNode <ValueType>> ::front;
	using X3DArrayField <X3DSFNode <ValueType>> ::back;
	using X3DArrayField <X3DSFNode <ValueType>> ::cend;
	using X3DArrayField <X3DSFNode <ValueType>> ::size;

	X3DMFNode () :
		ArrayField ()
	{ }

	X3DMFNode (const X3DMFNode & field) :
		ArrayField (field)
	{ }

	X3DMFNode (X3DMFNode && field) :
		ArrayField (std::move (field))
	{ }

	X3DMFNode (std::initializer_list <X3DSFNode <ValueType>>  initializer_list) :
		ArrayField (initializer_list)
	{ }

	X3DMFNode (std::initializer_list <const typename X3DSFNode <ValueType>::internal_type> initializer_list) :
		ArrayField (initializer_list)
	{ }

	template <class InputIterator>
	X3DMFNode (InputIterator first, InputIterator last) :
		ArrayField (first, last)
	{ }

	virtual
	X3DMFNode*
	clone () const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ return new X3DMFNode (*this); }

	virtual
	X3DMFNode*
	clone (X3DExecutionContext* const executionContext) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	virtual
	void
	clone (X3DExecutionContext* const, X3DFieldDefinition*) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	X3DMFNode &
	operator = (const X3DMFNode & field)
	{
		ArrayField::operator = (field);
		return *this;
	}

	X3DMFNode &
	operator = (X3DMFNode && field)
	{
		ArrayField::operator = (std::move (field));
		return *this;
	}

	virtual
	X3DConstants::FieldType
	getType () const final override
	{ return X3DConstants::MFNode; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	///  6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DMFNode &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DMFNode &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  Input/Output

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	virtual
	void
	toStream (std::ostream &) const final override;


private:

	///  TypeName identifer for X3DFields.
	static const std::string typeName;

};

template <class ValueType>
const std::string X3DMFNode <ValueType>::typeName ("MFNode");

template <class ValueType>
X3DMFNode <ValueType>*
X3DMFNode <ValueType>::clone (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DMFNode* field = new X3DMFNode ();

	clone (executionContext, field);

	return field;
}

template <class ValueType>
void
X3DMFNode <ValueType>::clone (X3DExecutionContext* const executionContext, X3DFieldDefinition* fieldDefinition) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DMFNode* field = static_cast <X3DMFNode*> (fieldDefinition);

	for (const auto & value : *this)
	{
		if (value)
			field -> emplace_back (value -> clone (executionContext));

		else
			field -> emplace_back ();
	}
}

template <class ValueType>
void
X3DMFNode <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

template <class ValueType>
void
X3DMFNode <ValueType>::toStream (std::ostream & ostream) const
{
	switch (size ())
	{
		case 0:
		{
			ostream << Generator::EmptyBrackets;
			return;
		}
		case 1:
		{
			Generator::PushContext ();
			ostream << front ();
			Generator::PopContext ();

			return;
		}
		default:
		{
			Generator::PushContext ();

			ostream
				<< '['
				<< Generator::TidyBreak
				<< Generator::IncIndent;

			for (const auto & field : basic::adapter (cbegin (), cend () - 1))
			{
				ostream
					<< Generator::Indent
					<< field
					<< Generator::TidyBreak;
			}

			ostream
				<< Generator::Indent
				<< back ()
				<< Generator::TidyBreak
				<< Generator::DecIndent
				<< Generator::Indent
				<< ']';

			Generator::PopContext ();

			return;
		}
	}
}

} // X3D
} // titania

#endif
