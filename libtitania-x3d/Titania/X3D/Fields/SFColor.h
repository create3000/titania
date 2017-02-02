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

#ifndef __TITANIA_X3D_FIELDS_SFCOLOR_H__
#define __TITANIA_X3D_FIELDS_SFCOLOR_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

template <class ValueType>
class X3DArrayField;

extern template class X3DField <Color3f>;

/**
 *  Class to represent a X3D SFColor field.
 */
class SFColor :
	public X3DField <Color3f>
{
public:

	using value_type = Color3f::value_type;
	using size_type  = Color3f::size_type;

	using X3DField <Color3f>::addInterest;
	using X3DField <Color3f>::setValue;
	using X3DField <Color3f>::getValue;
	using X3DField <Color3f>::operator =;

	///  @name Construction

	SFColor ();

	SFColor (const SFColor &);

	explicit
	SFColor (const Color3f &);

	SFColor (const value_type &, const value_type &, const value_type &);

	virtual
	SFColor*
	create () const final override
	{ return new SFColor (); }

	virtual
	SFColor*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ return new SFColor (*this); }

	///  @name Interest service

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const SFColor &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFColor &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Member access

	void
	setRed (const value_type &);

	value_type
	getRed () const
	{ return getValue () .r (); }

	void
	setGreen (const value_type &);

	value_type
	getGreen () const
	{ return getValue () .g (); }

	void
	setBlue (const value_type &);

	value_type
	getBlue () const
	{ return getValue () .b (); }

	void
	set1Value (const size_type &, const value_type &);

	value_type
	get1Value (const size_type &) const;

	///  @name Operations

	void
	setHSV (const value_type &, const value_type &, const value_type &);

	void
	getHSV (value_type &, value_type &, value_type &) const;

	SFColor*
	lerp (const SFColor &, const value_type &) const;

	///  @name Input/Output

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

	virtual
	void
	toXMLStream (std::ostream &) const final override;

	virtual
	void
	toJSONStream (std::ostream &) const final override;


protected:

	friend class X3DArrayField <SFColor>;

	void
	toJSONStreamValue (std::ostream &) const;


private:

	using X3DField <Color3f>::get;

};

} // X3D
} // titania

#endif
