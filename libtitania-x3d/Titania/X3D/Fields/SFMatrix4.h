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

#ifndef __TITANIA_X3D_FIELDS_SFMATRIX4_H__
#define __TITANIA_X3D_FIELDS_SFMATRIX4_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"
#include "SFRotation4.h"
#include "SFVec3.h"

namespace titania {
namespace X3D {

extern template class X3DField <Matrix4d>;
extern template class X3DField <Matrix4f>;

/**
 *  Template to represent a 4 x 4 matrix field. This is the base class for SFMatrix4d and SFMatrix4f fields.
 *
 *  Extern instantiations for float and double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  ValueType  Type of the internal value of the field.
 */
template <class ValueType>
class SFMatrix4 :
	public X3DField <ValueType>
{
public:

	typedef typename ValueType::value_type                   value_type;
	typedef SFVec3 <typename ValueType::point_type>          vector3_type;
	typedef SFRotation4 <typename ValueType::rotation_type>  rotation4_type;
	typedef SFRotation4 <Rotation4f>                         rotation4f_type;
	typedef typename ValueType::size_type                    size_type;

	using X3DField <ValueType>::addInterest;
	using X3DField <ValueType>::addEvent;
	using X3DField <ValueType>::setValue;
	using X3DField <ValueType>::getValue;
	using X3DField <ValueType>::operator =;

	///  @name Construction

	SFMatrix4 () :
		X3DField <ValueType> ()
	{ }

	SFMatrix4 (const SFMatrix4 & field) :
		X3DField <ValueType> (field)
	{ }

	explicit
	SFMatrix4 (const ValueType & value) :
		X3DField <ValueType> (value)
	{ }

	SFMatrix4 (const value_type & e11, const value_type & e12, const value_type & e13, const value_type & e14,
	           const value_type & e21, const value_type & e22, const value_type & e23, const value_type & e24,
	           const value_type & e31, const value_type & e32, const value_type & e33, const value_type & e34,
	           const value_type & e41, const value_type & e42, const value_type & e43, const value_type & e44) :
		X3DField <ValueType> (ValueType (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44))
	{ }

	virtual
	SFMatrix4*
	create () const override
	{ return new SFMatrix4 (); }

	virtual
	SFMatrix4*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) override;

	///  @name Interest service

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const SFMatrix4 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFMatrix4 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Member access

	void
	set1Value (const size_type &, const value_type &);

	value_type
	get1Value (const size_type &) const;

	void
	setValue (const value_type &, const value_type &, const value_type &, const value_type &,
	          const value_type &, const value_type &, const value_type &, const value_type &,
	          const value_type &, const value_type &, const value_type &, const value_type &,
	          const value_type &, const value_type &, const value_type &, const value_type &);

	void
	getValue (value_type &, value_type &, value_type &, value_type &,
	          value_type &, value_type &, value_type &, value_type &,
	          value_type &, value_type &, value_type &, value_type &,
	          value_type &, value_type &, value_type &, value_type &) const;

	///  @name Arithmetic operations

	void
	setTransform ();

	void
	setTransform (const vector3_type &);

	void
	setTransform (const vector3_type &,
	              const rotation4f_type &);

	void
	setTransform (const vector3_type &,
	              const rotation4f_type &,
	              const vector3_type &);

	void
	setTransform (const vector3_type &,
	              const rotation4f_type &,
	              const vector3_type &,
	              const rotation4f_type &);

	void
	setTransform (const vector3_type &,
	              const rotation4f_type &,
	              const vector3_type &,
	              const rotation4f_type &,
	              const vector3_type &);

	void
	getTransform (vector3_type &) const;

	void
	getTransform (vector3_type &,
	              rotation4f_type &) const;

	void
	getTransform (vector3_type &,
	              rotation4f_type &,
	              vector3_type &) const;

	void
	getTransform (vector3_type &,
	              rotation4f_type &,
	              vector3_type &,
	              rotation4f_type &) const;

	void
	getTransform (vector3_type &,
	              rotation4f_type &,
	              vector3_type &,
	              rotation4f_type &,
	              const vector3_type &) const;

	value_type
	determinant3 () const
	{ return getValue () .determinant3 (); }

	value_type
	determinant () const
	{ return getValue () .determinant (); }

	SFMatrix4*
	inverse () const;

	SFMatrix4*
	transpose () const;

	SFMatrix4*
	multLeft (const SFMatrix4 &) const;

	SFMatrix4*
	multRight (const SFMatrix4 &) const;

	vector3_type*
	multVecMatrix (const vector3_type &) const;

	vector3_type*
	multMatrixVec (const vector3_type &) const;

	vector3_type*
	multDirMatrix (const vector3_type &) const;

	vector3_type*
	multMatrixDir (const vector3_type &) const;

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
SFMatrix4 <ValueType>*
SFMatrix4 <ValueType>::copy (const CopyType) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	return new SFMatrix4 (*this);
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::set1Value (const size_type & index, const value_type & value)
{
	get () .data () [index] = value;
	addEvent ();
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::value_type
SFMatrix4 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () .data () [index];
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::setValue (const value_type & e11, const value_type & e12, const value_type & e13, const value_type & e14,
                                 const value_type & e21, const value_type & e22, const value_type & e23, const value_type & e24,
                                 const value_type & e31, const value_type & e32, const value_type & e33, const value_type & e34,
                                 const value_type & e41, const value_type & e42, const value_type & e43, const value_type & e44)
{
	setValue (ValueType (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44));
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::getValue (value_type & e11, value_type & e12, value_type & e13, value_type & e14,
                                 value_type & e21, value_type & e22, value_type & e23, value_type & e24,
                                 value_type & e31, value_type & e32, value_type & e33, value_type & e34,
                                 value_type & e41, value_type & e42, value_type & e43, value_type & e44) const
{
	const auto & data = getValue () .data ();

	e11 = data [ 0];
	e12 = data [ 1];
	e13 = data [ 2];
	e14 = data [ 3];

	e21 = data [ 4];
	e22 = data [ 5];
	e23 = data [ 6];
	e24 = data [ 7];

	e31 = data [ 8];
	e32 = data [ 9];
	e33 = data [10];
	e34 = data [11];

	e41 = data [12];
	e42 = data [13];
	e43 = data [14];
	e44 = data [15];
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform ()
{
	get () .set ();
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation)
{
	get () .set (translation);
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation,
                                     const rotation4f_type & rotation)
{
	get () .set (translation, rotation .getValue ());
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation,
                                     const rotation4f_type & rotation,
                                     const vector3_type & scale)
{
	get () .set (translation, rotation .getValue (), scale);
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation,
                                     const rotation4f_type & rotation,
                                     const vector3_type & scale,
                                     const rotation4f_type & scaleOrientation)
{
	get () .set (translation, rotation .getValue (), scale, scaleOrientation .getValue ());
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation,
                                     const rotation4f_type & rotation,
                                     const vector3_type & scale,
                                     const rotation4f_type & scaleOrientation,
                                     const vector3_type & center)
{
	get () .set (translation, rotation .getValue (), scale, scaleOrientation .getValue (), center);
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector3_type & translation) const
{
	typename vector3_type::internal_type t;

	getValue () .get (t);

	translation = t;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector3_type & translation,
                                     rotation4f_type & rotation) const
{
	typename vector3_type::internal_type t;
	typename rotation4_type::internal_type r;

	getValue () .get (t, r);

	translation = t;
	rotation    = r;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector3_type & translation,
                                     rotation4f_type & rotation,
                                     vector3_type & scale) const
{
	typename vector3_type::internal_type t, s;
	typename rotation4_type::internal_type r;

	getValue () .get (t, r, s);

	translation = t;
	rotation    = r;
	scale       = s;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector3_type & translation,
                                     rotation4f_type & rotation,
                                     vector3_type & scale,
                                     rotation4f_type & scaleOrientation) const
{
	typename vector3_type::internal_type t, s;
	typename rotation4_type::internal_type r, so;

	getValue () .get (t, r, s, so);

	translation      = t;
	rotation         = r;
	scale            = s;
	scaleOrientation = so;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector3_type & translation,
                                     rotation4f_type & rotation,
                                     vector3_type & scale,
                                     rotation4f_type & scaleOrientation,
                                     const vector3_type & center) const
{
	typename vector3_type::internal_type t, s, c;
	typename rotation4_type::internal_type r, so;

	getValue () .get (t, r, s, so, c);

	translation      = t;
	rotation         = r;
	scale            = s;
	scaleOrientation = so;
}

template <class ValueType>
inline
SFMatrix4 <ValueType>*
SFMatrix4 <ValueType>::transpose () const
{
	return new SFMatrix4 (! getValue ());
}

template <class ValueType>
inline
SFMatrix4 <ValueType>*
SFMatrix4 <ValueType>::inverse () const
{
	return new SFMatrix4 (~getValue ());
}

template <class ValueType>
inline
SFMatrix4 <ValueType>*
SFMatrix4 <ValueType>::multLeft (const SFMatrix4 & value) const
{
	ValueType result (getValue ());
	result .mult_left (value .getValue ());
	return new SFMatrix4 (result);
}

template <class ValueType>
inline
SFMatrix4 <ValueType>*
SFMatrix4 <ValueType>::multRight (const SFMatrix4 & value) const
{
	ValueType result (getValue ());
	result .mult_right (value .getValue ());
	return new SFMatrix4 (result);
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector3_type *
SFMatrix4 <ValueType>::multVecMatrix (const vector3_type &value) const
{
	return new vector3_type (getValue () .mult_vec_matrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector3_type *
SFMatrix4 <ValueType>::multMatrixVec (const vector3_type &value) const
{
	return new vector3_type (getValue () .mult_matrix_vec (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector3_type *
SFMatrix4 <ValueType>::multDirMatrix (const vector3_type &value) const
{
	return new vector3_type (getValue () .mult_dir_matrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector3_type *
SFMatrix4 <ValueType>::multMatrixDir (const vector3_type &value) const
{
	return new vector3_type (getValue () .mult_matrix_dir (value .getValue ()));
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::fromStream (std::istream & istream)
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
SFMatrix4 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::Precision <value_type> << getValue ();
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

// SFMatrix4d and SFMatrix4f
extern template class SFMatrix4 <Matrix4d>;
extern template class SFMatrix4 <Matrix4f>;

typedef SFMatrix4 <Matrix4d> SFMatrix4d;
typedef SFMatrix4 <Matrix4f> SFMatrix4f;

} // X3D
} // titania

#endif
