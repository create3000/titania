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

#ifndef __TITANIA_X3D_FIELDS_SFCOLOR_RGBA_H__
#define __TITANIA_X3D_FIELDS_SFCOLOR_RGBA_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

template <class ValueType>
class X3DArrayField;

extern template class X3DField <Color4f>;

/**
 *  Class to represent a X3D SFColorRGBA field.
 */
class SFColorRGBA :
	public X3DField <Color4f>
{
public:

	using value_type = Color4f::value_type;
	using size_type  = size_t;

	using X3DField <Color4f>::addInterest;
	using X3DField <Color4f>::setValue;
	using X3DField <Color4f>::getValue;
	using X3DField <Color4f>::operator =;

	///  @name Construction

	SFColorRGBA ();

	SFColorRGBA (const SFColorRGBA &);

	explicit
	SFColorRGBA (const internal_type &);

	SFColorRGBA (const value_type &, const value_type &, const value_type &, const value_type &);

	virtual
	SFColorRGBA*
	create () const final override
	{ return new SFColorRGBA (); }

	virtual
	SFColorRGBA*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ return new SFColorRGBA (*this); }

	///  @name Interest service

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFColorRGBA &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFColorRGBA &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Member access

	void
	setRed (const value_type & value);

	value_type
	getRed () const
	{ return getValue () .r (); }

	void
	setGreen (const value_type & value);

	value_type
	getGreen () const
	{ return getValue () .g (); }

	void
	setBlue (const value_type & value);

	value_type
	getBlue () const
	{ return getValue () .b (); }

	void
	setAlpha (const value_type & value);

	value_type
	getAlpha () const
	{ return getValue () .a (); }

	void
	set1Value (const size_type & index, const value_type & value);

	value_type
	get1Value (const size_type & index) const;

	value_type
	operator [ ] (const size_type & index) const;

	///  @name Operations

	void
	setHSVA (const vector4 <value_type> & hsva);

	vector4 <value_type>
	getHSVA () const;

	internal_type
	lerp (const internal_type & toColor, const value_type & t) const;

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

	friend class X3DArrayField <SFColorRGBA>;

	void
	toJSONStreamValue (std::ostream &) const;


private:

	using X3DField <Color4f>::get;

};

} // X3D
} // titania

#endif
