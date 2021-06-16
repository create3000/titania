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

#ifndef __TITANIA_X3D_FIELDS_SFIMAGE_H__
#define __TITANIA_X3D_FIELDS_SFIMAGE_H__

#include "../Basic/X3DField.h"
#include "../Types/Image.h"

namespace titania {
namespace X3D {

template <class ValueType>
class X3DArrayField;

#ifdef __APPLE__
template <>
const std::string X3DField <Image>::typeName;

template <>
const FieldType X3DField <Image>::type;
#endif

extern template class X3DField <Image>;

/**
 *  Class to represent a X3D SFImage field.
 */
class SFImage :
	public X3DField <Image>
{
public:

	///  @name Member types

	using value_type = Image::array_type::value_type;
	using size_type  = Image::size_type;

	using X3DField <Image>::addInterest;
	using X3DField <Image>::copy;
	using X3DField <Image>::setValue;
	using X3DField <Image>::getValue;
	using X3DField <Image>::operator =;

	///  @name Construction

	SFImage ();

	SFImage (const SFImage & other);

	SFImage (SFImage && other);

	explicit
	SFImage (const Image & value);

	explicit
	SFImage (Image && value);

	SFImage (const size_type width, const size_type height, const size_type components, const MFInt32 & array);

	SFImage (const size_type width, const size_type height, const size_type components, MFInt32 && array);

	virtual
	SFImage*
	create () const final override
	{ return new SFImage (); }

	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	SFImage*
	copy (const CopyType) const final override
	{ return new SFImage (*this); }

	///  @name Assignment operators

	SFImage &
	operator = (const SFImage & other);

	SFImage &
	operator = (SFImage && other);

	SFImage &
	operator = (Image && value);

	///  @name Member access

	void
	setWidth (const size_type value);

	size_type
	getWidth () const;

	void
	setHeight (const size_type value);

	size_type
	getHeight () const;

	void
	setComponents (const size_type value);

	size_type
	getComponents () const;

	void
	setArray (const MFInt32 & value);

	MFInt32 &
	getArray ();

	const MFInt32 &
	getArray () const;

	///  @name Interest service

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFImage &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFImage &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream & istream) final override;

	virtual
	void
	toStream (std::ostream & ostream) const final override;

	virtual
	void
	toXMLStream (std::ostream & ostream) const final override;

	virtual
	void
	toJSONStream (std::ostream & ostream) const final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;


protected:

	using X3DField <Image>::get;

	friend class X3DArrayField <SFImage>;

	void
	toJSONStreamValue (std::ostream & ostream) const;

};

} // X3D
} // titania

#endif
