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

#ifndef __TITANIA_X3D_FIELDS_SFMATRIX4_H__
#define __TITANIA_X3D_FIELDS_SFMATRIX4_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"
#include "SFRotation4.h"
#include "SFVec3.h"

namespace titania {
namespace X3D {

template <class ValueType>
class X3DArrayField;

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

	using value_type    = typename ValueType::value_type;
	using size_type     = typename ValueType::size_type;
	using vector_type   = SFVec3 <typename ValueType::vector_type>;
	using rotation_type = SFRotation;

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
	addInterest (void (Class::* memberFunction) (const SFMatrix4 &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFMatrix4 &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Member access

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
	setTransform (const vector_type & translation);

	void
	setTransform (const vector_type & translation,
	              const rotation_type & rotation);

	void
	setTransform (const vector_type & translation,
	              const rotation_type & rotation,
	              const vector_type & scale);

	void
	setTransform (const vector_type & translation,
	              const rotation_type & rotation,
	              const vector_type & scale,
	              const rotation_type & scaleOrientation);

	void
	setTransform (const vector_type & translation,
	              const rotation_type & rotation,
	              const vector_type & scale,
	              const rotation_type & scaleOrientation,
	              const vector_type & center);

	void
	getTransform (vector_type & translation) const;

	void
	getTransform (vector_type & translation,
	              rotation_type & rotation) const;

	void
	getTransform (vector_type & translation,
	              rotation_type & rotation,
	              vector_type & scale) const;

	void
	getTransform (vector_type & translation,
	              rotation_type & rotation,
	              vector_type & scale,
	              rotation_type & scaleOrientation) const;

	void
	getTransform (vector_type & translation,
	              rotation_type & rotation,
	              vector_type & scale,
	              rotation_type & scaleOrientation,
	              const vector_type & center) const;

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

	vector_type*
	multVecMatrix (const vector_type &) const;

	vector_type*
	multMatrixVec (const vector_type &) const;

	vector_type*
	multDirMatrix (const vector_type &) const;

	vector_type*
	multMatrixDir (const vector_type &) const;

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

	friend class X3DArrayField <SFMatrix4>;

	void
	toJSONStreamValue (std::ostream &) const;


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
	get () .front () .data () [index] = value;
	addEvent ();
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::value_type
SFMatrix4 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () .front () .data () [index];
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::value_type
SFMatrix4 <ValueType>::operator [ ] (const size_type & index) const
{
	return getValue () .front () .data () [index];
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
SFMatrix4 <ValueType>::setTransform (const vector_type & translation)
{
	get () .set (translation);
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector_type & translation,
                                     const rotation_type & rotation)
{
	get () .set (translation, rotation .getValue ());
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector_type & translation,
                                     const rotation_type & rotation,
                                     const vector_type & scale)
{
	get () .set (translation, rotation .getValue (), scale);
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector_type & translation,
                                     const rotation_type & rotation,
                                     const vector_type & scale,
                                     const rotation_type & scaleOrientation)
{
	get () .set (translation, rotation .getValue (), scale, scaleOrientation .getValue ());
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::setTransform (const vector_type & translation,
                                     const rotation_type & rotation,
                                     const vector_type & scale,
                                     const rotation_type & scaleOrientation,
                                     const vector_type & center)
{
	get () .set (translation, rotation .getValue (), scale, scaleOrientation .getValue (), center);
	addEvent ();
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector_type & translation) const
{
	typename vector_type::internal_type t;

	getValue () .get (t);

	translation = t;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector_type & translation,
                                     rotation_type & rotation) const
{
	typename vector_type::internal_type t;
	typename rotation_type::internal_type r;

	getValue () .get (t, r);

	translation = t;
	rotation    = r;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector_type & translation,
                                     rotation_type & rotation,
                                     vector_type & scale) const
{
	typename vector_type::internal_type t, s;
	typename rotation_type::internal_type r;

	getValue () .get (t, r, s);

	translation = t;
	rotation    = r;
	scale       = s;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector_type & translation,
                                     rotation_type & rotation,
                                     vector_type & scale,
                                     rotation_type & scaleOrientation) const
{
	typename vector_type::internal_type t, s;
	typename rotation_type::internal_type r, so;

	getValue () .get (t, r, s, so);

	translation      = t;
	rotation         = r;
	scale            = s;
	scaleOrientation = so;
}

template <class ValueType>
void
SFMatrix4 <ValueType>::getTransform (vector_type & translation,
                                     rotation_type & rotation,
                                     vector_type & scale,
                                     rotation_type & scaleOrientation,
                                     const vector_type & center) const
{
	typename vector_type::internal_type t, s, c;
	typename rotation_type::internal_type r, so;

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
	return new SFMatrix4 (math::inverse (getValue ()));
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
typename SFMatrix4 <ValueType>::vector_type *
SFMatrix4 <ValueType>::multVecMatrix (const vector_type &value) const
{
	return new vector_type (getValue () .mult_vec_matrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector_type *
SFMatrix4 <ValueType>::multMatrixVec (const vector_type &value) const
{
	return new vector_type (getValue () .mult_matrix_vec (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector_type *
SFMatrix4 <ValueType>::multDirMatrix (const vector_type &value) const
{
	return new vector_type (getValue () .mult_dir_matrix (value .getValue ()));
}

template <class ValueType>
inline
typename SFMatrix4 <ValueType>::vector_type *
SFMatrix4 <ValueType>::multMatrixDir (const vector_type &value) const
{
	return new vector_type (getValue () .mult_matrix_dir (value .getValue ()));
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

	value_type e11, e12, e13, e14;
	value_type e21, e22, e23, e24;
	value_type e31, e32, e33, e34;
	value_type e41, e42, e43, e44;
	
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

				if (Grammar::Number <value_type> (istream, e14))
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

								if (Grammar::Number <value_type> (istream, e24))
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
												Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

												if (Grammar::Number <value_type> (istream, e34))
												{
													Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

													if (Grammar::Number <value_type> (istream, e41))
													{
														Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

														if (Grammar::Number <value_type> (istream, e42))
														{
															Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

															if (Grammar::Number <value_type> (istream, e43))
															{
																Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

																if (Grammar::Number <value_type> (istream, e44))
																{
																	setValue (ValueType (e11, e12, e13, e14,
																	                     e21, e22, e23, e24,
																	                     e31, e32, e33, e34,
																	                     e41, e42, e43, e44));
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
						}
					}
				}
			}
		}
	}
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::SetPrecision <value_type> << getValue ();
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::toJSONStream (std::ostream & ostream) const
{
	ostream
		<< '['
		<< X3DGenerator::TidySpace;

	toJSONStreamValue (ostream);

	ostream
		<< X3DGenerator::TidySpace
		<< ']';
}

template <class ValueType>
inline
void
SFMatrix4 <ValueType>::toJSONStreamValue (std::ostream & ostream) const
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
		<< getValue () [0] [3]
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
		<< getValue () [1] [3]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [2] [0]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [2] [1]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [2] [2]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [2] [3]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [3] [0]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [3] [1]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [3] [2]
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () [3] [3];
}

// SFMatrix4d and SFMatrix4f
extern template class SFMatrix4 <Matrix4d>;
extern template class SFMatrix4 <Matrix4f>;

typedef SFMatrix4 <Matrix4d> SFMatrix4d;
typedef SFMatrix4 <Matrix4f> SFMatrix4f;

} // X3D
} // titania

#endif
