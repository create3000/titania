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

template <class ValueType>
class SFMatrix4 :
	public X3DField <ValueType>
{
public:

	typedef typename ValueType::value_type                   scalar_type;
	typedef SFVec3 <typename ValueType::vector3_type>        vector3_type;
	typedef SFRotation4 <typename ValueType::rotation4_type> rotation4_type;
	typedef SFRotation4 <Rotation4f>                         rotation4f_type;
	typedef typename ValueType::size_type                    size_type;

	using X3DField <ValueType>::addInterest;
	using X3DField <ValueType>::setValue;
	using X3DField <ValueType>::getValue;
	using X3DField <ValueType>::operator =;

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

	SFMatrix4 (const scalar_type & e11, const scalar_type & e12, const scalar_type & e13, const scalar_type & e14,
	           const scalar_type & e21, const scalar_type & e22, const scalar_type & e23, const scalar_type & e24,
	           const scalar_type & e31, const scalar_type & e32, const scalar_type & e33, const scalar_type & e34,
	           const scalar_type & e41, const scalar_type & e42, const scalar_type & e43, const scalar_type & e44) :
		X3DField <ValueType> (ValueType (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44))
	{ }

	virtual
	SFMatrix4*
	clone () const final;

	///  6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (const SFMatrix4 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFMatrix4 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  Functions

	void
	set1Value (const size_type &, const scalar_type &);

	scalar_type
	get1Value (const size_type &) const;

	void
	setValue (const scalar_type &, const scalar_type &, const scalar_type &, const scalar_type &,
	          const scalar_type &, const scalar_type &, const scalar_type &, const scalar_type &,
	          const scalar_type &, const scalar_type &, const scalar_type &, const scalar_type &,
	          const scalar_type &, const scalar_type &, const scalar_type &, const scalar_type &);

	void
	getValue (scalar_type &, scalar_type &, scalar_type &, scalar_type &,
	          scalar_type &, scalar_type &, scalar_type &, scalar_type &,
	          scalar_type &, scalar_type &, scalar_type &, scalar_type &,
	          scalar_type &, scalar_type &, scalar_type &, scalar_type &) const;

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
	              vector3_type &) const;

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

	///  @name Input operator.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final;

	///  @name Output operator.
	virtual
	void
	toStream (std::ostream &) const final;


private:

	using X3DField <ValueType>::notifyParents;
	using X3DField <ValueType>::get;

};

template <class ValueType>
inline
SFMatrix4 <ValueType>*
SFMatrix4 <ValueType>::clone () const
{
	return new SFMatrix4 (*this);
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::set1Value (const size_type & index, const scalar_type & value)
{
	get () .data () [index] = value;
	notifyParents ();
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::scalar_type
SFMatrix4 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () .data () [index];
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::setValue (const scalar_type & e11, const scalar_type & e12, const scalar_type & e13, const scalar_type & e14,
                                 const scalar_type & e21, const scalar_type & e22, const scalar_type & e23, const scalar_type & e24,
                                 const scalar_type & e31, const scalar_type & e32, const scalar_type & e33, const scalar_type & e34,
                                 const scalar_type & e41, const scalar_type & e42, const scalar_type & e43, const scalar_type & e44)
{
	setValue (ValueType (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44));
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::getValue (scalar_type & e11, scalar_type & e12, scalar_type & e13, scalar_type & e14,
                                 scalar_type & e21, scalar_type & e22, scalar_type & e23, scalar_type & e24,
                                 scalar_type & e31, scalar_type & e32, scalar_type & e33, scalar_type & e34,
                                 scalar_type & e41, scalar_type & e42, scalar_type & e43, scalar_type & e44) const
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
	notifyParents ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation)
{
	get () .set (translation);
	notifyParents ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation,
                                     const rotation4f_type & rotation)
{
	get () .set (translation, rotation .getValue ());
	notifyParents ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation,
                                     const rotation4f_type & rotation,
                                     const vector3_type & scale)
{
	get () .set (translation, rotation .getValue (), scale);
	notifyParents ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector3_type & translation,
                                     const rotation4f_type & rotation,
                                     const vector3_type & scale,
                                     const rotation4f_type & scaleOrientation)
{
	get () .set (translation, rotation .getValue (), scale, scaleOrientation .getValue ());
	notifyParents ();
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
	notifyParents ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector3_type & translation) const
{
	typename vector3_type::value_type t;

	getValue () .get (t);

	translation = t;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector3_type & translation,
                                     rotation4f_type & rotation) const
{
	typename vector3_type::value_type t;
	typename rotation4_type::value_type r;

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
	typename vector3_type::value_type t, s;
	typename rotation4_type::value_type r;

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
	typename vector3_type::value_type t, s;
	typename rotation4_type::value_type r, so;

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
                                     vector3_type & center) const
{
	typename vector3_type::value_type t, s, c;
	typename rotation4_type::value_type r, so;

	getValue () .get (t, r, s, so, c);

	translation      = t;
	rotation         = r;
	scale            = s;
	scaleOrientation = so;
	center           = c;
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
	return new SFMatrix4 (ValueType (getValue ()) .multLeft (value .getValue ()));
}

template <class ValueType>
inline
SFMatrix4 <ValueType>*
SFMatrix4 <ValueType>::multRight (const SFMatrix4 & value) const
{
	return new SFMatrix4 (ValueType (getValue ()) .multRight (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector3_type *
SFMatrix4 <ValueType>::multVecMatrix (const vector3_type &value) const
{
	return new vector3_type (getValue () .multVecMatrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector3_type *
SFMatrix4 <ValueType>::multMatrixVec (const vector3_type &value) const
{
	return new vector3_type (getValue () .multMatrixVec (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector3_type *
SFMatrix4 <ValueType>::multDirMatrix (const vector3_type &value) const
{
	return new vector3_type (getValue () .multDirMatrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector3_type *
SFMatrix4 <ValueType>::multMatrixDir (const vector3_type &value) const
{
	return new vector3_type (getValue () .multMatrixDir (value .getValue ()));
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << Generator::Precision <scalar_type> << getValue ();
}

//extern template class X3DField <Matrix4d>;
//extern template class X3DField <Matrix4f>;

// SFMatrix4d and SFMatrix4f
extern template class SFMatrix4 <Matrix4d>;
extern template class SFMatrix4 <Matrix4f>;

typedef SFMatrix4 <Matrix4d> SFMatrix4d;
typedef SFMatrix4 <Matrix4f> SFMatrix4f;

} // X3D
} // titania

#endif
