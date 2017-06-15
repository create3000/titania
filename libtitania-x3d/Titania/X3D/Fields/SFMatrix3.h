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

#ifndef __TITANIA_X3D_FIELDS_SFMATRIX3_H__
#define __TITANIA_X3D_FIELDS_SFMATRIX3_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"
#include "SFVec2.h"

namespace titania {
namespace X3D {

template <class InternalType>
class X3DArrayField;

extern template class X3DField <Matrix3d>;
extern template class X3DField <Matrix3f>;

/**
 *  Template to represent a 3 x 3 matrix field. This is the base class for SFMatrix3d and SFMatrix3f fields.
 *
 *  Extern instantiations for float and double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  InternalType  Type of the internal value of the field.
 */
template <class InternalType>
class SFMatrix3 :
	public X3DField <InternalType>
{
public:

	using value_type    = typename InternalType::value_type;
	using size_type     = typename InternalType::size_type;
	using vector_type   = SFVec2 <typename InternalType::vector_type>;
	using rotation_type = SFVec2 <typename InternalType::rotation_type>;

	using X3DField <InternalType>::addInterest;
	using X3DField <InternalType>::addEvent;
	using X3DField <InternalType>::setValue;
	using X3DField <InternalType>::getValue;
	using X3DField <InternalType>::operator =;

	///  @name Construction

	SFMatrix3 () :
		X3DField <InternalType> ()
	{ }

	SFMatrix3 (const SFMatrix3 & other) :
		X3DField <InternalType> (other)
	{ }

	explicit
	SFMatrix3 (const InternalType & value) :
		X3DField <InternalType> (value)
	{ }

	SFMatrix3 (const value_type & e11, const value_type & e12, const value_type & e13,
	           const value_type & e21, const value_type & e22, const value_type & e23,
	           const value_type & e31, const value_type & e32, const value_type & e33) :
		X3DField <InternalType> (InternalType (e11, e12, e13, e21, e22, e23, e31, e32, e33))
	{ }

	virtual
	SFMatrix3*
	create () const final override
	{ return new SFMatrix3 (); }

	virtual
	SFMatrix3*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	///  @name Interest service

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFMatrix3 &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFMatrix3 &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Member access

	value_type
	at (const size_type & index) const;

	void
	set1Value (const size_type & index, const value_type & value);

	value_type
	get1Value (const size_type & index) const;

	value_type
	operator [ ] (const size_type & index) const;

	///  @name Arithmetic operations

	void
	setTransform ();

	void
	setTransform (const typename vector_type::internal_type & translation);

	void
	setTransform (const typename vector_type::internal_type & translation,
	              const value_type & rotation);

	void
	setTransform (const typename vector_type::internal_type & translation,
	              const value_type & rotation,
	              const typename vector_type::internal_type  &);

	void
	setTransform (const typename vector_type::internal_type & translation,
	              const value_type & rotation,
	              const typename vector_type::internal_type & scale,
	              const value_type & scaleOrientation);

	void
	setTransform (const typename vector_type::internal_type & translation,
	              const value_type & rotation,
	              const typename vector_type::internal_type & scale,
	              const value_type & scaleOrientation,
	              const typename vector_type::internal_type & center);

	void
	getTransform (typename vector_type::internal_type & translation) const;

	void
	getTransform (typename vector_type::internal_type & translation,
	              value_type & rotation) const;

	void
	getTransform (typename vector_type::internal_type & translation,
	              value_type & rotation,
	              typename vector_type::internal_type & scale) const;

	void
	getTransform (typename vector_type::internal_type & translation,
	              value_type & rotation,
	              typename vector_type::internal_type & scale,
	              value_type & scaleOrientation) const;

	void
	getTransform (typename vector_type::internal_type & translation,
	              value_type & rotation,
	              typename vector_type::internal_type & scale,
	              value_type & scaleOrientation,
	              typename vector_type::internal_type & center) const;

	SFMatrix3 &
	operator *= (const InternalType & matrix);

	value_type
	determinant () const
	{ return getValue () .determinant (); }

	InternalType
	inverse () const;

	InternalType
	transpose () const;

	InternalType
	multLeft (const InternalType & matrix) const;

	InternalType
	multRight (const InternalType & matrix) const;

	typename vector_type::internal_type
	multVecMatrix (const typename vector_type::internal_type & vector) const;

	typename vector_type::internal_type
	multMatrixVec (const typename vector_type::internal_type & vector) const;

	typename vector_type::internal_type
	multDirMatrix (const typename vector_type::internal_type & vector) const;

	typename vector_type::internal_type
	multMatrixDir (const typename vector_type::internal_type & vector) const;

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream & istream)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	virtual
	void
	toStream (std::ostream & ostream) const final override;

	virtual
	void
	toXMLStream (std::ostream & ostream) const final override;

	virtual
	void
	toJSONStream (std::ostream & ostream) const final override;


protected:

	friend class X3DArrayField <SFMatrix3>;

	void
	toJSONStreamValue (std::ostream & ostream) const;


private:

	using X3DField <InternalType>::get;

};

template <class InternalType>
inline
SFMatrix3 <InternalType>*
SFMatrix3 <InternalType>::copy (const CopyType) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	return new SFMatrix3 (*this);
}

template <class InternalType>
inline
typename SFMatrix3 <InternalType>::value_type
SFMatrix3 <InternalType>::at (const size_type & index) const
{
	if (index > std::tuple_size <InternalType>::value)
		throw std::out_of_range ("SFMatrix4::at ");

	return getValue () .data () [index];
}

template <class InternalType>
void
SFMatrix3 <InternalType>::set1Value (const size_type & index, const value_type & value)
{
	get () .data () [index] = value;
	addEvent ();
}

template <class InternalType>
inline
typename SFMatrix3 <InternalType>::value_type
SFMatrix3 <InternalType>::get1Value (const size_type & index) const
{
	return getValue () .data () [index];
}

template <class InternalType>
inline
typename SFMatrix3 <InternalType>::value_type
SFMatrix3 <InternalType>::operator [ ] (const size_type & index) const
{
	return getValue () .data () [index];
}

template <class InternalType>
void
SFMatrix3 <InternalType>::setTransform ()
{
	get () .set ();
	addEvent ();
}

template <class InternalType>
void
SFMatrix3 <InternalType>::setTransform (const typename vector_type::internal_type & translation)
{
	get () .set (translation);
	addEvent ();
}

template <class InternalType>
void
SFMatrix3 <InternalType>::setTransform (const typename vector_type::internal_type & translation,
                                        const value_type & rotation)
{
	get () .set (translation, rotation);
	addEvent ();
}

template <class InternalType>
void
SFMatrix3 <InternalType>::setTransform (const typename vector_type::internal_type & translation,
                                        const value_type & rotation,
                                        const typename vector_type::internal_type & scale)
{
	get () .set (translation, rotation, scale);
	addEvent ();
}

template <class InternalType>
void
SFMatrix3 <InternalType>::setTransform (const typename vector_type::internal_type & translation,
                                        const value_type & rotation,
                                        const typename vector_type::internal_type & scale,
                                        const value_type & scaleOrientation)
{
	get () .set (translation, rotation, scale, scaleOrientation);
	addEvent ();
}

template <class InternalType>
void
SFMatrix3 <InternalType>::setTransform (const typename vector_type::internal_type & translation,
                                        const value_type & rotation,
                                        const typename vector_type::internal_type & scale,
                                        const value_type & scaleOrientation,
                                        const typename vector_type::internal_type & center)
{
	get () .set (translation, rotation, scale, scaleOrientation, center);
	addEvent ();
}

template <class InternalType>
void
SFMatrix3 <InternalType>::getTransform (typename vector_type::internal_type & translation) const
{
	getValue () .get (translation);
}

template <class InternalType>
void
SFMatrix3 <InternalType>::getTransform (typename vector_type::internal_type & translation,
                                        value_type & rotation) const
{
	getValue () .get (translation, rotation);
}

template <class InternalType>
void
SFMatrix3 <InternalType>::getTransform (typename vector_type::internal_type & translation,
                                        value_type & rotation,
                                        typename vector_type::internal_type & scale) const
{
	getValue () .get (translation, rotation, scale);
}

template <class InternalType>
void
SFMatrix3 <InternalType>::getTransform (typename vector_type::internal_type & translation,
                                        value_type & rotation,
                                        typename vector_type::internal_type & scale,
                                        value_type & scaleOrientation) const
{
	getValue () .get (translation, rotation, scale, scaleOrientation);
}

template <class InternalType>
void
SFMatrix3 <InternalType>::getTransform (typename vector_type::internal_type & translation,
                                        value_type & rotation,
                                        typename vector_type::internal_type & scale,
                                        value_type & scaleOrientation,
                                        typename vector_type::internal_type & center) const
{
	getValue () .get (translation, rotation, scale, scaleOrientation, center);
}

template <class InternalType>
SFMatrix3 <InternalType> &
SFMatrix3 <InternalType>::operator *= (const InternalType & matrix)
{
	get () *= matrix;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
InternalType
SFMatrix3 <InternalType>::transpose () const
{
	return math::transpose (getValue ());
}

template <class InternalType>
inline
InternalType
SFMatrix3 <InternalType>::inverse () const
{
	return math::inverse (getValue ());
}

template <class InternalType>
inline
InternalType
SFMatrix3 <InternalType>::multLeft (const InternalType & matrix) const
{
	InternalType result (getValue ());
	result .mult_left (matrix);
	return result;
}

template <class InternalType>
inline
InternalType
SFMatrix3 <InternalType>::multRight (const InternalType & matrix) const
{
	InternalType result (getValue ());
	result .mult_right (matrix);
	return result;
}

template <class InternalType>
inline
typename SFMatrix3 <InternalType>::vector_type::internal_type
SFMatrix3 <InternalType>::multVecMatrix (const typename vector_type::internal_type & vector) const
{
	return getValue () .mult_vec_matrix (vector);
}

template <class InternalType>
inline
typename SFMatrix3 <InternalType>::vector_type::internal_type
SFMatrix3 <InternalType>::multMatrixVec (const typename vector_type::internal_type & vector) const
{
	return getValue () .mult_matrix_vec (vector);
}

template <class InternalType>
inline
typename SFMatrix3 <InternalType>::vector_type::internal_type
SFMatrix3 <InternalType>::multDirMatrix (const typename vector_type::internal_type & vector) const
{
	return getValue () .mult_dir_matrix (vector);
}

template <class InternalType>
inline
typename SFMatrix3 <InternalType>::vector_type::internal_type
SFMatrix3 <InternalType>::multMatrixDir (const typename vector_type::internal_type & vector) const
{
	return getValue () .mult_matrix_dir (vector);
}

template <class InternalType>
inline
void
SFMatrix3 <InternalType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::string whiteSpaces;

	value_type e11, e12, e13;
	value_type e21, e22, e23;
	value_type e31, e32, e33;
	
	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	if (Grammar::Number <value_type> (istream, e11))
	{
		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

		if (Grammar::Number <value_type> (istream, e12))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

			if (Grammar::Number <value_type> (istream, e13))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

				if (Grammar::Number <value_type> (istream, e21))
				{
					Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

					if (Grammar::Number <value_type> (istream, e22))
					{
						Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

						if (Grammar::Number <value_type> (istream, e23))
						{
							Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

							if (Grammar::Number <value_type> (istream, e31))
							{
								Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

								if (Grammar::Number <value_type> (istream, e32))
								{
									Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

									if (Grammar::Number <value_type> (istream, e33))
									{
										setValue (InternalType (e11, e12, e13,
										                     e21, e22, e23,
										                     e31, e32, e33));
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

template <class InternalType>
inline
void
SFMatrix3 <InternalType>::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::SetPrecision <value_type> << getValue ();
}

template <class InternalType>
inline
void
SFMatrix3 <InternalType>::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

template <class InternalType>
inline
void
SFMatrix3 <InternalType>::toJSONStream (std::ostream & ostream) const
{
	ostream
		<< '['
		<< X3DGenerator::TidySpace;

	toJSONStreamValue (ostream);

	ostream
		<< X3DGenerator::TidySpace
		<< ']';
}

template <class InternalType>
inline
void
SFMatrix3 <InternalType>::toJSONStreamValue (std::ostream & ostream) const
{
	ostream
		<< X3DGenerator::SetPrecision <value_type>
		<< getValue () [0] [0]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [0] [1]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [0] [2]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [1] [0]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [1] [1]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [1] [2]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [2] [0]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [2] [1]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [2] [2];
}

// SFMatrix4d and SFMatrix4f
extern template class SFMatrix3 <Matrix3d>;
extern template class SFMatrix3 <Matrix3f>;

typedef SFMatrix3 <Matrix3d> SFMatrix3d;
typedef SFMatrix3 <Matrix3f> SFMatrix3f;

} // X3D
} // titania

#endif
