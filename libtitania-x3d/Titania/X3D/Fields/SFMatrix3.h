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

#ifndef __TITANIA_X3D_FIELDS_SFMATRIX3_H__
#define __TITANIA_X3D_FIELDS_SFMATRIX3_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"
#include "SFVec2.h"

namespace titania {
namespace X3D {

extern template class X3DField <Matrix3d>;
extern template class X3DField <Matrix3f>;

template <class ValueType>
class SFMatrix3 :
	public X3DField <ValueType>
{
public:

	typedef typename ValueType::value_type            scalar_type;
	typedef SFVec2 <typename ValueType::vector2_type> vector2_type;
	typedef typename ValueType::size_type             size_type;

	using X3DField <ValueType>::addInterest;
	using X3DField <ValueType>::setValue;
	using X3DField <ValueType>::getValue;
	using X3DField <ValueType>::operator =;

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

	SFMatrix3 (const scalar_type & e11, const scalar_type & e12, const scalar_type & e13,
	           const scalar_type & e21, const scalar_type & e22, const scalar_type & e23,
	           const scalar_type & e31, const scalar_type & e32, const scalar_type & e33) :
		X3DField <ValueType> (ValueType (e11, e12, e13, e21, e22, e23, e31, e32, e33))
	{ }

	virtual
	SFMatrix3*
	clone () const final;

	///  6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (const SFMatrix3 &)) const
	{
		addInterest (object, memberFunction, *this);
	}

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFMatrix3 &)) const
	{
		addInterest (object, memberFunction, *this);
	}

	void
	addInterest (void (* requester) (const SFMatrix3 &)) const
	{
		addInterest (requester, *this);
	}

	///  Functions

	void
	set1Value (const size_type &, const scalar_type &);

	scalar_type
	get1Value (const size_type &) const;

	void
	setValue (const scalar_type &, const scalar_type &, const scalar_type &,
	          const scalar_type &, const scalar_type &, const scalar_type &,
	          const scalar_type &, const scalar_type &, const scalar_type &);

	void
	getValue (scalar_type &, scalar_type &, scalar_type &,
	          scalar_type &, scalar_type &, scalar_type &,
	          scalar_type &, scalar_type &, scalar_type &) const;

	void
	setTransform ();

	void
	setTransform (const vector2_type &);

	void
	setTransform (const vector2_type &,
	              const scalar_type &);

	void
	setTransform (const vector2_type &,
	              const scalar_type &,
	              const vector2_type  &);

	void
	setTransform (const vector2_type &,
	              const scalar_type &,
	              const vector2_type &,
	              const scalar_type &);

	void
	setTransform (const vector2_type &,
	              const scalar_type &,
	              const vector2_type &,
	              const scalar_type &,
	              const vector2_type  &);

	void
	getTransform (vector2_type &,
	              scalar_type &,
	              vector2_type &) const;

	void
	getTransform (vector2_type &,
	              scalar_type &, vector2_type &,
	              scalar_type &) const;

	void
	getTransform (vector2_type &,
	              scalar_type &, vector2_type &,
	              scalar_type &,
	              vector2_type &) const;

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
SFMatrix3 <ValueType>*
SFMatrix3 <ValueType>::clone () const
{
	return new SFMatrix3 (*this);
}

template <class ValueType>
void
SFMatrix3 <ValueType>::set1Value (const size_type & index, const scalar_type & value)
{
	get () .data () [index] = value;
	notifyParents ();
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::scalar_type
SFMatrix3 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () .data () [index];
}

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::setValue (const scalar_type & e11, const scalar_type & e12, const scalar_type & e13,
                                 const scalar_type & e21, const scalar_type & e22, const scalar_type & e23,
                                 const scalar_type & e31, const scalar_type & e32, const scalar_type & e33)
{
	setValue (ValueType (e11, e12, e13, e21, e22, e23, e31, e32, e33));
}

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::getValue (scalar_type & e11, scalar_type & e12, scalar_type & e13,
                                 scalar_type & e21, scalar_type & e22, scalar_type & e23,
                                 scalar_type & e31, scalar_type & e32, scalar_type & e33) const
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
	notifyParents ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation)
{
	get () .set (translation);
	notifyParents ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation,
                                     const scalar_type & rotation)
{
	get () .set (translation, rotation);
	notifyParents ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation,
                                     const scalar_type & rotation,
                                     const vector2_type & scale)
{
	get () .set (translation, rotation, scale);
	notifyParents ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation,
                                     const scalar_type & rotation,
                                     const vector2_type & scale,
                                     const scalar_type & scaleOrientation)
{
	get () .set (translation, rotation, scale, scaleOrientation);
	notifyParents ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::setTransform (const vector2_type & translation,
                                     const scalar_type & rotation,
                                     const vector2_type & scale,
                                     const scalar_type & scaleOrientation,
                                     const vector2_type & center)
{
	get () .set (translation, rotation, scale, scaleOrientation, center);
	notifyParents ();
}

template <class ValueType>
void
SFMatrix3 <ValueType>::getTransform (vector2_type & translation,
                                     scalar_type & rotation,
                                     vector2_type & scale) const
{
	typename vector2_type::value_type t, s;

	getValue () .get (t, rotation, s);

	translation = t;
	scale       = s;
}

template <class ValueType>
void
SFMatrix3 <ValueType>::getTransform (vector2_type & translation,
                                     scalar_type & rotation,
                                     vector2_type & scale,
                                     scalar_type & scaleOrientation) const
{
	typename vector2_type::value_type t, s;

	getValue () .get (t, rotation, s, scaleOrientation);

	translation = t;
	scale       = s;
}

template <class ValueType>
void
SFMatrix3 <ValueType>::getTransform (vector2_type & translation,
                                     scalar_type & rotation,
                                     vector2_type & scale,
                                     scalar_type & scaleOrientation,
                                     vector2_type & center) const
{
	typename vector2_type::value_type t, s, c;

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
	return new SFMatrix3 (ValueType (getValue ()) .multLeft (value .getValue ()));
}

template <class ValueType>
inline
SFMatrix3 <ValueType>*
SFMatrix3 <ValueType>::multRight (const SFMatrix3 & value) const
{
	return new SFMatrix3 (ValueType (getValue ()) .multRight (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::vector2_type *
SFMatrix3 <ValueType>::multVecMatrix (const vector2_type &value) const
{
	return new vector2_type (getValue () .multVecMatrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::vector2_type *
SFMatrix3 <ValueType>::multMatrixVec (const vector2_type &value) const
{
	return new vector2_type (getValue () .multMatrixVec (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::vector2_type *
SFMatrix3 <ValueType>::multDirMatrix (const vector2_type &value) const
{
	return new vector2_type (getValue () .multDirMatrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix3 <ValueType>::vector2_type *
SFMatrix3 <ValueType>::multMatrixDir (const vector2_type &value) const
{
	return new vector2_type (getValue () .multMatrixDir (value .getValue ()));
}

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

template <class ValueType>
inline
void
SFMatrix3 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << Generator::Precision <scalar_type><< getValue ();
}

//extern template class X3DField <Matrix3d>;
//extern template class X3DField <Matrix3f>;

// SFMatrix4d and SFMatrix4f
extern template class SFMatrix3 <Matrix3d>;
extern template class SFMatrix3 <Matrix3f>;

typedef SFMatrix3 <Matrix3d> SFMatrix3d;
typedef SFMatrix3 <Matrix3f> SFMatrix3f;

} // X3D
} // titania

#endif
