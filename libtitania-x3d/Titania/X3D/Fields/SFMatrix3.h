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

#ifndef __TITANIA_X3D_FIELDS_SFMATRIX3_H__
#define __TITANIA_X3D_FIELDS_SFMATRIX3_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"
#include "SFVec2.h"

namespace titania {
namespace X3D {

extern template class X3DField <Matrix3d>;
extern template class X3DField <Matrix3f>;

/**
 *  Template to represent a 3 x 3 matrix field. This is the base class for SFMatrix3d and SFMatrix3f fields.
 *
 *  Extern instantiations for float and double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  ValueType  Type of the internal value of the field.
 */
template <class ValueType>
class SFMatrix3 :
	public X3DField <ValueType>
{
public:

	typedef typename ValueType::value_type          value_type;
	typedef SFVec2 <typename ValueType::point_type> vector2_type;
	typedef typename ValueType::size_type           size_type;

	using X3DField <ValueType>::addInterest;
	using X3DField <ValueType>::addEvent;
	using X3DField <ValueType>::setValue;
	using X3DField <ValueType>::getValue;
	using X3DField <ValueType>::operator =;

	///  @name Construction

	SFMatrix3 () :
		X3DField <ValueType> ()
	{ }

	SFMatrix3 (const SFMatrix3 & field) :
		X3DField <ValueType> (field)
	{ }

	explicit
	SFMatrix3 (const ValueType & value) :
		X3DField <ValueType> (value)
	{ }

	SFMatrix3 (const value_type & e11, const value_type & e12, const value_type & e13,
	           const value_type & e21, const value_type & e22, const value_type & e23,
	           const value_type & e31, const value_type & e32, const value_type & e33) :
		X3DField <ValueType> (ValueType (e11, e12, e13, e21, e22, e23, e31, e32, e33))
	{ }

	virtual
	SFMatrix3*
	create () const final override
	{ return new SFMatrix3 (); }

	virtual
	SFMatrix3*
	clone () const
	throw (Error <NOT_SUPPORTED>) final override;

	///  @name Interest service

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const SFMatrix3 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFMatrix3 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Member access

	void
	set1Value (const size_type &, const value_type &);

	value_type
	get1Value (const size_type &) const;

	void
	setValue (const value_type &, const value_type &, const value_type &,
	          const value_type &, const value_type &, const value_type &,
	          const value_type &, const value_type &, const value_type &);

	void
	getValue (value_type &, value_type &, value_type &,
	          value_type &, value_type &, value_type &,
	          value_type &, value_type &, value_type &) const;

	///  @name Arithmetic operations

	void
	setTransform ();

	void
	setTransform (const vector2_type &);

	void
	setTransform (const vector2_type &,
	              const value_type &);

	void
	setTransform (const vector2_type &,
	              const value_type &,
	              const vector2_type  &);

	void
	setTransform (const vector2_type &,
	              const value_type &,
	              const vector2_type &,
	              const value_type &);

	void
	setTransform (const vector2_type &,
	              const value_type &,
	              const vector2_type &,
	              const value_type &,
	              const vector2_type  &);

	void
	getTransform (vector2_type &) const;

	void
	getTransform (vector2_type &,
	              value_type &) const;

	void
	getTransform (vector2_type &,
	              value_type &,
	              vector2_type &) const;

	void
	getTransform (vector2_type &,
	              value_type &, vector2_type &,
	              value_type &) const;

	void
	getTransform (vector2_type &,
	              value_type &, vector2_type &,
	              value_type &,
	              vector2_type &) const;

	value_type
	determinant2 () const
	{ return getValue () .determinant2 (); }

	value_type
	determinant () const
	{ return getValue () .determinant (); }

	SFMatrix3*
	inverse () const;

	SFMatrix3*
	transpose () const;

	SFMatrix3*
	multLeft (const SFMatrix3 &) const;

	SFMatrix3*
	multRight (const SFMatrix3 &) const;

	vector2_type*
	multVecMatrix (const vector2_type &) const;

	vector2_type*
	multMatrixVec (const vector2_type &) const;

	vector2_type*
	multDirMatrix (const vector2_type &) const;

	vector2_type*
	multMatrixDir (const vector2_type &) const;

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


private:

	using X3DField <ValueType>::get;

};

template <class ValueType>
inline
SFMatrix3 <ValueType>*
SFMatrix3 <ValueType>::clone () const
throw (Error <NOT_SUPPORTED>)
{
	return new SFMatrix3 (*this);
}

template <class ValueType>
void
SFMatrix3 <ValueType>::set1Value (const size_type & index, const value_type & value)
{
	get () .data () [index] = value;
	addEvent ();
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::value_type
SFMatrix3 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () .data () [index];
}

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::setValue (const value_type & e11, const value_type & e12, const value_type & e13,
                                 const value_type & e21, const value_type & e22, const value_type & e23,
                                 const value_type & e31, const value_type & e32, const value_type & e33)
{
	setValue (ValueType (e11, e12, e13, e21, e22, e23, e31, e32, e33));
}

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::getValue (value_type & e11, value_type & e12, value_type & e13,
                                 value_type & e21, value_type & e22, value_type & e23,
                                 value_type & e31, value_type & e32, value_type & e33) const
{
	const auto & data = getValue () .data ();

	e11 = data [0];
	e12 = data [1];
	e13 = data [2];

	e21 = data [3];
	e22 = data [4];
	e23 = data [5];

	e31 = data [6];
	e32 = data [7];
	e33 = data [8];
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform ()
{
	get () .set ();
	addEvent ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation)
{
	get () .set (translation);
	addEvent ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation,
                                     const value_type & rotation)
{
	get () .set (translation, rotation);
	addEvent ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation,
                                     const value_type & rotation,
                                     const vector2_type & scale)
{
	get () .set (translation, rotation, scale);
	addEvent ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation,
                                     const value_type & rotation,
                                     const vector2_type & scale,
                                     const value_type & scaleOrientation)
{
	get () .set (translation, rotation, scale, scaleOrientation);
	addEvent ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation,
                                     const value_type & rotation,
                                     const vector2_type & scale,
                                     const value_type & scaleOrientation,
                                     const vector2_type & center)
{
	get () .set (translation, rotation, scale, scaleOrientation, center);
	addEvent ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::getTransform (vector2_type & translation) const
{
	typename vector2_type::internal_type t;

	getValue () .get (t);

	translation = t;
}

template <class ValueType>
void
SFMatrix3 <ValueType>::getTransform (vector2_type & translation,
                                     value_type & rotation) const
{
	typename vector2_type::internal_type t;

	getValue () .get (t, rotation);

	translation = t;
}

template <class ValueType>
void
SFMatrix3 <ValueType>::getTransform (vector2_type & translation,
                                     value_type & rotation,
                                     vector2_type & scale) const
{
	typename vector2_type::internal_type t, s;

	getValue () .get (t, rotation, s);

	translation = t;
	scale       = s;
}

template <class ValueType>
void
SFMatrix3 <ValueType>::getTransform (vector2_type & translation,
                                     value_type & rotation,
                                     vector2_type & scale,
                                     value_type & scaleOrientation) const
{
	typename vector2_type::internal_type t, s;

	getValue () .get (t, rotation, s, scaleOrientation);

	translation = t;
	scale       = s;
}

template <class ValueType>
void
SFMatrix3 <ValueType>::getTransform (vector2_type & translation,
                                     value_type & rotation,
                                     vector2_type & scale,
                                     value_type & scaleOrientation,
                                     vector2_type & center) const
{
	typename vector2_type::internal_type t, s, c;

	getValue () .get (t, rotation, s, scaleOrientation, c);

	translation = t;
	scale       = s;
	center      = c;
}

template <class ValueType>
inline
SFMatrix3 <ValueType>*
SFMatrix3 <ValueType>::transpose () const
{
	return new SFMatrix3 (! getValue ());
}

template <class ValueType>
inline
SFMatrix3 <ValueType>*
SFMatrix3 <ValueType>::inverse () const
{
	return new SFMatrix3 (~getValue ());
}

template <class ValueType>
inline
SFMatrix3 <ValueType>*
SFMatrix3 <ValueType>::multLeft (const SFMatrix3 & value) const
{
	ValueType result (getValue ());
	result .mult_left (value .getValue ());
	return new SFMatrix3 (result);
}

template <class ValueType>
inline
SFMatrix3 <ValueType>*
SFMatrix3 <ValueType>::multRight (const SFMatrix3 & value) const
{
	ValueType result (getValue ());
	result .mult_right (value .getValue ());
	return new SFMatrix3 (result);
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::vector2_type *
SFMatrix3 <ValueType>::multVecMatrix (const vector2_type &value) const
{
	return new vector2_type (getValue () .mult_vec_matrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::vector2_type *
SFMatrix3 <ValueType>::multMatrixVec (const vector2_type &value) const
{
	return new vector2_type (getValue () .mult_matrix_vec (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::vector2_type *
SFMatrix3 <ValueType>::multDirMatrix (const vector2_type &value) const
{
	return new vector2_type (getValue () .mult_dir_matrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::vector2_type *
SFMatrix3 <ValueType>::multMatrixDir (const vector2_type &value) const
{
	return new vector2_type (getValue () .mult_matrix_dir (value .getValue ()));
}

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::string whiteSpaces;
	
	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	if (istream >> get ())
		addEvent ();
}

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::Precision <value_type> << getValue ();
}

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

// SFMatrix4d and SFMatrix4f
extern template class SFMatrix3 <Matrix3d>;
extern template class SFMatrix3 <Matrix3f>;

typedef SFMatrix3 <Matrix3d> SFMatrix3d;
typedef SFMatrix3 <Matrix3f> SFMatrix3f;

} // X3D
} // titania

#endif
