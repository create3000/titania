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

#ifndef __TITANIA_X3D_FIELDS_MFNODE_H__
#define __TITANIA_X3D_FIELDS_MFNODE_H__

#include "../Basic/X3DArrayField.h"
#include "../Fields/SFNode.h"
#include "../InputOutput/Generator.h"

#include <Titania/Utility/Adapter.h>

namespace titania {
namespace X3D {

//extern template class Array <SFNode <X3DBaseNode>>;
extern template class X3DField <Array <SFNode <X3DBaseNode>>>;
extern template class X3DArrayField <SFNode <X3DBaseNode>>;

class MFNode :
	public X3DArrayField <SFNode <X3DBaseNode>>
{
private:

	typedef X3DArrayField <SFNode <X3DBaseNode>> ArrayField;


public:

	using X3DArrayField <SFNode <X3DBaseNode>>::addInterest;

	MFNode () :
		ArrayField ()
	{ }

	MFNode (const MFNode & field) :
		ArrayField (field)
	{ }

	explicit
	MFNode (std::initializer_list <SFNode <X3DBaseNode>> initializer_list) :
		ArrayField (initializer_list)
	{ }

	explicit
	MFNode (std::initializer_list <const typename SFNode <X3DBaseNode>::value_type> initializer_list) :
		ArrayField (initializer_list)
	{ }

	template <class InputIterator>
	MFNode (InputIterator first, InputIterator last) :
		ArrayField (first, last)
	{ }

	virtual
	MFNode*
	clone () const final
	{ return new MFNode (*this); }

	virtual
	MFNode*
	clone (X3DExecutionContext* const executionContext) const final;

	virtual
	void
	clone (X3DExecutionContext* const, X3DFieldDefinition*) const final;

	virtual
	X3DConstants::FieldType
	getType () const final
	{ return X3DConstants::MFNode; }

	///  6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (const MFNode &)) const
	{
		addInterest (object, memberFunction, *this);
	}

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const MFNode &)) const
	{
		addInterest (object, memberFunction, *this);
	}

	void
	addInterest (void (* requester) (const MFNode &)) const
	{
		addInterest (requester, *this);
	}

	///  Input/Output

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final;

	virtual
	void
	toStream (std::ostream &) const final;

};

} // X3D

} // titania

#endif
