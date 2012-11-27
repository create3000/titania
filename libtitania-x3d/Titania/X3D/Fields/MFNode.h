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
 ******************************************************************************/

#ifndef __TITANIA_X3D_FIELDS_MFNODE_H__
#define __TITANIA_X3D_FIELDS_MFNODE_H__

#include "../Basic/X3DArrayField.h"
#include "../Fields/SFNode.h"
#include "../InputOutput/Generator.h"

#include <Titania/Utility/Adapter.h>

namespace titania {
namespace X3D {

extern template class Array <SFNode <X3DBasicNode>>;
extern template class X3DField <Array <SFNode <X3DBasicNode>>>;
extern template class X3DArrayField <SFNode <X3DBasicNode>>;

template <class Type>
class MFNode :
	public X3DArrayField <SFNode <Type>>
{
private:

	typedef X3DArrayField <SFNode <Type>> ArrayField;


public:

	using ArrayField::operator =;
	using ArrayField::set;
	using ArrayField::assign;
	using ArrayField::front;
	using ArrayField::back;
	using ArrayField::begin;
	using ArrayField::end;
	using ArrayField::cbegin;
	using ArrayField::cend;
	using ArrayField::size;

	MFNode () :
		ArrayField ()
	{ }

	MFNode (const MFNode & field) :
		ArrayField (field)
	{ }

	template <class Up>
	MFNode (const MFNode <Up> & field) :
		ArrayField (field .begin (), field .end ())
	{ }

	explicit
	MFNode (std::initializer_list <SFNode <Type>> initializer_list) :
		ArrayField (initializer_list)
	{ }

	explicit
	MFNode (std::initializer_list <typename SFNode <Type>::value_type> initializer_list) :
		ArrayField (initializer_list)
	{ }
	
	template <class InputIterator>
	MFNode (InputIterator first, InputIterator last) :
		ArrayField (first, last)
	{ }

	virtual
	MFNode*
	copy () const { return new MFNode (*this); }

	virtual
	MFNode*
	copy (X3DExecutionContext* const executionContext) const
	{
		MFNode* field = new MFNode ();

		for (const auto & node :* this)
		{
			field -> push_back (node .getValue ()
			                    ? node .getValue () -> copy (executionContext)
									  : NULL);
		}

		return field;
	}
	
	template <class Up>
	MFNode &
	operator = (const MFNode <Up> & field)
	{
		assign (field .begin (), field .end ());
		return *this;
	}

	virtual
	void
	write (const X3DObject &);
	
	virtual
	void
	read (std::vector <X3DObject*> &) const;

	virtual
	const FieldType*
	getType () const { return &X3DField <Array <SFNode <X3DBasicNode>>>::type; }

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	void
	toStream (std::ostream &) const;

};

template <class Type>
void
MFNode <Type>::write (const X3DObject & field)
{
	assert (getType () == field .getType ());

	const MFNode* same_type = dynamic_cast <const MFNode*> (&field);
	
	if (same_type)
		set (same_type -> getValue ());

	else
	{
		std::vector <X3DObject*> values;

		field .read (values);

		set (values .begin (), values .end ());
	}
}
	
template <class Type>
void
MFNode <Type>::read (std::vector <X3DObject*> & values) const
{
	values .reserve (size ());
	values .assign (begin (), end ());
}

template <class Type>
void
MFNode <Type>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

template <class Type>
void
MFNode <Type>::toStream (std::ostream & ostream) const
{
	if (size () > 1)
	{
		Generator::PushLevel ();

		ostream
			<< '['
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & value : basic::adapter (cbegin (), cend () - 1))
		{
			ostream
				<< Generator::Indent
				<< value
				<< Generator::TidyBreak;
		}

		ostream
			<< Generator::Indent
			<< back ()
			<< Generator::TidyBreak
			<< Generator::DecIndent
			<< Generator::Indent
			<< ']';

		Generator::PopLevel ();

		return;
	}

	if (size () == 1)
	{
		Generator::PushLevel ();
		ostream << front ();
		Generator::PopLevel ();

		return;
	}

	ostream << Generator::EmptyBrackets;
}

extern template class MFNode <X3DBasicNode>;

} // X3D

extern template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <SFNode <X3D::X3DBasicNode>> &);

} // titania

#endif
