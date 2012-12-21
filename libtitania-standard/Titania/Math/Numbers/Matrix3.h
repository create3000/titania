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
 ******************************************************************************/

#ifndef __TITANIA_MATH_NUMBERS_MATRIX3_H__
#define __TITANIA_MATH_NUMBERS_MATRIX3_H__

#include <cfloat>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "Vector2.h"
#include "Vector3.h"

#define INDEX3(c, r) ((c) * 3 + (r))

namespace titania {
namespace math {

template <typename Type>
class matrix3;

template <typename Type>
inline
matrix3 <Type>
operator ! (const matrix3 <Type> & matrix);

template <typename Type>
inline
matrix3 <Type>
transpose (const matrix3 <Type> & matrix);

template <typename Type>
inline
matrix3 <Type>
operator ~ (const matrix3 <Type> & matrix);

template <typename Type>
inline
matrix3 <Type>
inverse (const matrix3 <Type> & matrix);

template <typename Type>
inline
matrix3 <Type>
operator * (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs);

template <typename Type>
class matrix3
{
private:

	static constexpr size_t Order = 3;
	static constexpr size_t Size  = Order * Order;


public:

	///  Size typedef.  Used for size and indices.
	typedef size_t size_type;

	///  Value typedef.
	typedef Type value_type;

	///  Vector2 typedef.
	typedef vector2 <Type> vector2_type;

	///  Vector3 typedef.
	typedef vector3 <Type> vector3_type;

	///  Vector typedef.
	typedef vector3 <vector3_type> vector_type;

	///  Array typedef.
	typedef Type array_type [Size];

	// XXX
	typedef Type Matrix [9];

	///  Default constructor. A new matrix initialized with the identity matrix is created and returned.
	matrix3 ();

	///  Copy constructor.
	matrix3 (const matrix3 &);

	///  Copy constructor.
	template <typename Up>
	matrix3 (const matrix3 <Up> &);

	///  Value constructor.
	explicit
	matrix3 (const Matrix);

	///  Element constructor.
	matrix3 (const Type &, const Type &, const Type &,
	         const Type &, const Type &, const Type &,
	         const Type &, const Type &, const Type &);

	template <typename T>
	matrix3 (const vector2 <T> &);

	explicit
	matrix3 (const Type &);

	matrix3 &
	operator = (const Matrix);

	template <typename T>
	matrix3 &
	operator = (const matrix3 <T> &);

	matrix3 &
	operator = (const matrix3 &);

	void
	set (const Matrix);

	void get (Matrix);

	void
	set (const Type &, const Type &, const Type &,
	     const Type &, const Type &, const Type &,
	     const Type &, const Type &, const Type &);

	template <typename T>
	void
	get (T &, T &, T &,
	     T &, T &, T &,
	     T &, T &, T &) const;

	template <typename T>
	void
	setRow (const size_type, const vector3 <T> &);

	vector3 <Type>
	getRow (const size_type) const;

	template <typename T>
	void
	setTranslation (const vector2 <T> &);

	vector2 <Type>
	getTranslation () const;

	void
	setRotation (const Type &);

	Type
	getRotation () const;

	template <typename S>
	void
	setScale (const vector2 <S> &);

	vector2 <Type>
	getScale () const;

	void
	translate (const vector2 <Type> &);

	void
	rotate (const Type &);

	void
	scale (const vector2 <Type> &);

	void
	set ();

	void
	set (const vector2 <Type> &);

	void
	set (const vector2 <Type> &, const Type &);

	void
	set (const vector2 <Type> &, const Type &, const vector2 <Type> &);

	void
	set (const vector2 <Type> &, const Type &, const vector2 <Type> &, const Type &);

	void
	set (const vector2 <Type> &, const Type &, const vector2 <Type> &, const Type &, const vector2 <Type> &);

	void
	get (vector2 <Type> &, Type &, vector2 <Type> &) const;

	void
	get (vector2 <Type> &, Type &, vector2 <Type> &, Type &) const;

	void
	get (vector2 <Type> &, Type &, vector2 <Type> &, Type &, vector2 <Type> &) const;

	Type
	determinant () const;

	matrix3 &
	transpose ();

	matrix3 &
	inverse ();

	matrix3 &
	operator *= (const Type &);

	template <typename T>
	matrix3 &
	operator *= (const matrix3 <T> &);

	matrix3 &
	operator /= (const Type &);

	template <typename T>
	matrix3 &
	multLeft (const matrix3 <T> &);

	template <typename T>
	matrix3 &
	multRight (const matrix3 <T> &);

	template <typename T>
	vector2 <Type>
	multVecMatrix (const vector2 <T> &) const;

	template <typename T>
	vector2 <Type>
	multMatrixVec (const vector2 <T> &) const;

	template <typename T>
	vector2 <Type>
	multDirMatrix (const vector2 <T> &) const;

	Type &
	operator [ ] (const size_type);

	const Type &
	operator [ ] (const size_type) const;

	static const Matrix Identity;

	Matrix value;


private:

	Type
	det2 () const;

	template <typename T, typename S>
	bool
	factor (matrix3 &, vector2 <S> &, matrix3 &, vector2 <T> &, matrix3 &) const;

	void jacobi2 (Type [2], vector2 <Type> [2], int &) const;

};

template <typename Type>
const typename matrix3 <Type>::Matrix matrix3 <Type>::Identity = {
	1, 0, 0,
	0, 1, 0,
	0, 0, 1

};

template <typename Type>
matrix3 <Type>::matrix3 ()
{
	set (Identity);
}

template <typename Type>
matrix3 <Type>::matrix3 (const matrix3 <Type> & matrix)
{
	set (matrix .value);
}

template <typename Type>
template <typename T>
matrix3 <Type>::matrix3 (const matrix3 <T> & matrix)
{
	*this = matrix;
}

template <typename Type>
matrix3 <Type>::matrix3 (const Matrix value)
{
	set (value);
}

template <typename Type>
matrix3 <Type>::matrix3 (const Type & e11, const Type & e12, const Type & e13,
                         const Type & e21, const Type & e22, const Type & e23,
                         const Type & e31, const Type & e32, const Type & e33)
{
	set (e11, e12, e13, e21, e22, e23, e31, e32, e33);
}

template <typename Type>
template <typename T>
matrix3 <Type>::matrix3 (const vector2 <T> & t)
{
	setTranslation (t);
}

template <typename Type>
matrix3 <Type>::matrix3 (const Type & r)
{
	setRotation (r);
}

template <typename Type>
matrix3 <Type> &
matrix3 <Type>::operator = (const Matrix matrix)
{
	set (matrix);
	return *this;
}

template <typename Type>
template <typename T>
matrix3 <Type> &
matrix3 <Type>::operator = (const matrix3 <T> & matrix)
{
	value [ 0] = matrix .value [ 0];
	value [ 1] = matrix .value [ 1];
	value [ 2] = matrix .value [ 2];
	value [ 3] = matrix .value [ 3];
	value [ 4] = matrix .value [ 4];
	value [ 5] = matrix .value [ 5];
	value [ 6] = matrix .value [ 6];
	value [ 7] = matrix .value [ 7];
	value [ 8] = matrix .value [ 8];
	return *this;
}

template <typename Type>
matrix3 <Type> &
matrix3 <Type>::operator = (const matrix3 <Type> & matrix)
{
	set (matrix .value);
	return *this;
}

template <typename Type>
void
matrix3 <Type>::set (const Matrix value)
{
	(void) memmove (this -> value, value, sizeof (Type) * 9);
}

template <typename Type>
void
matrix3 <Type>::get (Matrix value)
{
	(void) memmove (value, this -> value, sizeof (Type) * 9);
}

template <typename Type>
void
matrix3 <Type>::set (const Type & e11, const Type & e12, const Type & e13,
                     const Type & e21, const Type & e22, const Type & e23,
                     const Type & e31, const Type & e32, const Type & e33)
{
	value [0] = e11; value [1] = e12; value [2] = e13;
	value [3] = e21; value [4] = e22; value [5] = e23;
	value [6] = e31; value [7] = e32; value [8] = e33;
}

template <typename Type>
template <typename T>
void
matrix3 <Type>::get (T & e11, T & e12, T & e13,
                     T & e21, T & e22, T & e23,
                     T & e31, T & e32, T & e33) const
{
	e11 = value [0]; e12 = value [1]; e13 = value [2];
	e21 = value [3]; e22 = value [4]; e23 = value [5];
	e31 = value [6]; e32 = value [7]; e33 = value [8];
}

template <typename Type>
template <typename T>
void
matrix3 <Type>::setRow (const size_type row, const vector3 <T> & vector)
{
	switch (row)
	{
		case 0:
			value [0] = vector .x;
			value [1] = vector .y;
			value [2] = vector .z;
			return;
		case 1:
			value [3] = vector .x;
			value [4] = vector .y;
			value [5] = vector .z;
			return;
		case 2:
			value [6] = vector .x;
			value [7] = vector .y;
			value [8] = vector .z;
			return;
	}

	throw std::out_of_range ("index out of range");
}

template <typename Type>
vector3 <Type>
matrix3 <Type>::getRow (const size_type row) const
{
	switch (row)
	{
		case 0: return vector3 <Type> (value [0], value [1], value [2]);
		case 1: return vector3 <Type> (value [3], value [4], value [5]);
		case 2: return vector3 <Type> (value [6], value [7], value [8]);
	}

	throw std::out_of_range ("index out of range");
}

template <typename Type>
template <typename T>
void
matrix3 <Type>::setTranslation (const vector2 <T> & t)
{
	*this     = Identity;
	value [6] = t .x ();
	value [7] = t .y ();
}

template <typename Type>
vector2 <Type>
matrix3 <Type>::getTranslation () const
{
	return vector2 <Type> (value [6],
	                       value [7]);
}

template <typename Type>
void
matrix3 <Type>::setRotation (const Type & rotation)
{
	Type sinAngle = std::sin (rotation);
	Type cosAngle = std::cos (rotation);

	*this = Identity;

	value [0] = cosAngle;
	value [1] = sinAngle;
	value [3] = -sinAngle;
	value [4] = cosAngle;
}

template <typename Type>
Type
matrix3 <Type>::getRotation () const
{
	return std::atan2 (value [1], value [0]);
}

template <typename Type>
template <typename S>
void
matrix3 <Type>::setScale (const vector2 <S> & s)
{
	*this     = Identity;
	value [0] = s .x ();
	value [4] = s .y ();
}

template <typename Type>
vector2 <Type>
matrix3 <Type>::getScale () const
{
	return vector2 <Type> (value [0],
	                       value [4]);
}

template <typename Type>
void
matrix3 <Type>::translate (const vector2 <Type> & translation)
{
	#define TRANSLATE(i) \
	   (value [INDEX3 (0, i)] * translation .x ()   \
	    + value [INDEX3 (1, i)] * translation .y ())

	value [INDEX3 (2, 0)] += TRANSLATE (0);
	value [INDEX3 (2, 1)] += TRANSLATE (1);

	#undef TRANSLATE
}

template <typename Type>
void
matrix3 <Type>::rotate (const Type & rotation)
{
	multLeft (matrix3 <Type> (rotation));
}

template <typename Type>
void
matrix3 <Type>::scale (const vector2 <Type> & scaleFactor)
{
	value [INDEX3 (0, 0)] *= scaleFactor .x ();
	value [INDEX3 (1, 0)] *= scaleFactor .y ();

	value [INDEX3 (0, 1)] *= scaleFactor .x ();
	value [INDEX3 (1, 1)] *= scaleFactor .y ();

	//value[INDEX3(0,2)] *= scaleFactor .x;
	//value[INDEX3(1,2)] *= scaleFactor .y;
}

template <typename Type>
void
matrix3 <Type>::set ()
{
	set (Identity);
}

template <typename Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation)
{
	setTranslation (translation);
}

template <typename Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation,
                     const Type & rotation)
{
	setTranslation (translation);

	if (rotation not_eq 0)
		rotate (rotation);
}

template <typename Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation,
                     const Type & rotation,
                     const vector2 <Type> & scaleFactor)
{
	setTranslation (translation);

	if (rotation not_eq 0)
		rotate (rotation);

	if (scaleFactor not_eq vector2 <Type> (1, 1))
		scale (scaleFactor);
}

template <typename Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation,
                     const Type & rotation,
                     const vector2 <Type> & scaleFactor,
                     const Type & scaleOrientation)
{
	setTranslation (translation);

	if (rotation not_eq 0)
		rotate (rotation);

	if (scaleFactor not_eq vector2 <Type> (1, 1))
	{
		if (scaleOrientation not_eq 0)
			rotate (scaleOrientation);

		scale (scaleFactor);

		if (scaleOrientation not_eq 0)
			rotate (-scaleOrientation);
	}
}

template <typename Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation,
                     const Type & rotation,
                     const vector2 <Type> & scaleFactor,
                     const Type & scaleOrientation,
                     const vector2 <Type> & center)
{
	setTranslation (translation);

	if (center not_eq vector2 <Type> ())
		translate (center);

	if (rotation not_eq 0)
		rotate (rotation);

	if (scaleFactor not_eq vector2 <Type> (1, 1))
	{
		if (scaleOrientation not_eq 0)
			rotate (scaleOrientation);

		scale (scaleFactor);

		if (scaleOrientation not_eq 0)
			rotate (-scaleOrientation);
	}

	if (center not_eq vector2 <Type> ())
		translate (-center);
}

template <typename Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation,
                     vector2 <Type> & scaleFactor) const
{
	matrix3 <Type> so, rot, proj;
	factor (so, scaleFactor, rot, translation, proj);
	rotation = rot .getRotation ();
}

template <typename Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation,
                     vector2 <Type> & scaleFactor,
                     Type & scaleOrientation) const
{
	matrix3 <Type> so, rot, proj;
	factor (so, scaleFactor, rot, translation, proj);
	rotation         = rot .getRotation ();
	scaleOrientation = so .getRotation ();
}

template <typename Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation,
                     vector2 <Type> & scaleFactor,
                     Type & scaleOrientation,
                     vector2 <Type> & center) const
{
	matrix3 <Type> m, c;
	m .setTranslation (-center);
	m .multLeft (*this);
	c .setTranslation (center);
	m .multLeft (c);

	m .get (translation, rotation, scaleFactor, scaleOrientation);
}

template <typename Type>
template <typename T, typename S>
bool
matrix3 <Type>::factor (matrix3 & r, vector2 <S> & s, matrix3 & u, vector2 <T> & t, matrix3 & proj) const
{
	matrix3 a (*this);

	for (int i = 0; i < 2; i ++)
	{
		t [i]                    = value [INDEX3 (2, i)];
		a .value [INDEX3 (2, i)] = a .value [INDEX3 (i, 2)] = 0;
	}

	a .value [INDEX3 (2, 2)] = 1;

	/* (3) Compute det A. If negative, set sign = -1, else sign = 1 */
	double det      = a .det2 ();
	double det_sign = (det < 0 ? -1 : 1);

	if (det_sign * det < 1e-12)
		return false;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         // singular

	/* (4) B = A * !A  (here !A means A transpose) */
	matrix3 b = a * !a;

	Type           evalues [2];
	vector2 <Type> evectors [2];
	int            junk;
	b .jacobi2 (evalues, evectors, junk);

	// find min / max eigenvalues and do ratio test to determine singularity

	r = matrix3 (evectors [0] .x (), evectors [0] .y (), 0,
	             evectors [1] .x (), evectors [1] .y (), 0,
	             0, 0, 1);

	/* Compute s = sqrt(evalues), with sign. Set si = s-inverse */
	matrix3 si;

	for (int i = 0; i < 2; i ++)
	{
		s [i]                     = det_sign * std::sqrt (evalues [i]);
		si .value [INDEX3 (i, i)] = 1 / s [i];
	}

	/* (5) Compute U = R^ S! R A. */
	u = r * si * !r * a;
	
	r .transpose ();

	return true;
}

#define SB_JACOBI_RANK  2
template <typename Type>
void
matrix3 <Type>::jacobi2 (Type evalues [SB_JACOBI_RANK], vector2 <Type> evectors [SB_JACOBI_RANK], int & rots) const
{
	double sm;                 // smallest entry
	double theta;              // angle for Jacobi rotation
	double c, s, t;            // cosine, sine, tangent of theta
	double tau;                // sine / (1 + cos)
	double h, g;               // two scrap values
	double thresh;             // threshold below which no rotation done
	double b [SB_JACOBI_RANK]; // more scratch
	double z [SB_JACOBI_RANK]; // more scratch
	int    p, q, i, j;
	double a [SB_JACOBI_RANK] [SB_JACOBI_RANK];

	// initializations
	for (i = 0; i < SB_JACOBI_RANK; i ++)
	{
		b [i] = evalues [i] = value [INDEX3 (i, i)];
		z [i] = 0;

		for (j = 0; j < SB_JACOBI_RANK; j ++)
		{
			evectors [i] [j] = (i == j) ? 1 : 0;
			a [i] [j]        = value [INDEX3 (i, j)];
		}
	}

	rots = 0;

	// Why 50? I don't know--it's the way the folks who wrote the
	// algorithm did it:
	for (i = 0; i < 50; i ++)
	{
		sm = 0;

		for (p = 0; p < SB_JACOBI_RANK - 1; p ++)
			for (q = p + 1; q < SB_JACOBI_RANK; q ++)
				sm += std::abs (a [p] [q]);

		if (sm == 0)
			return;

		thresh = i < 3 ?
		         .2 * sm / (SB_JACOBI_RANK * SB_JACOBI_RANK) :
		         0;

		for (p = 0; p < SB_JACOBI_RANK - 1; p ++)
		{
			for (q = p + 1; q < SB_JACOBI_RANK; q ++)
			{
				g = 100 * std::abs (a [p] [q]);

				if (i > 3 and (std::abs (evalues [p]) + g == std::abs (evalues [p])) and (std::abs (evalues [q]) + g == std::abs (evalues [q])))
					a [p] [q] = 0;

				else if (std::abs (a [p] [q]) > thresh)
				{
					h = evalues [q] - evalues [p];

					if (std::abs (h) + g == std::abs (h))
						t = a [p] [q] / h;
					else
					{
						theta = .5 * h / a [p] [q];
						t     = 1 / (std::abs (theta) + std::sqrt (1 + theta * theta));

						if (theta < 0)
							t = -t;
					}

					// End of computing tangent of rotation angle

					c            = 1 / std::sqrt (1 + t * t);
					s            = t * c;
					tau          = s / (1 + c);
					h            = t * a [p] [q];
					z [p]       -= h;
					z [q]       += h;
					evalues [p] -= h;
					evalues [q] += h;
					a [p] [q]    = 0;

					for (j = 0; j < p; j ++)
					{
						g         = a [j] [p];
						h         = a [j] [q];
						a [j] [p] = g - s * (h + g * tau);
						a [j] [q] = h + s * (g - h * tau);
					}

					for (j = p + 1; j < q; j ++)
					{
						g         = a [p] [j];
						h         = a [j] [q];
						a [p] [j] = g - s * (h + g * tau);
						a [j] [q] = h + s * (g - h * tau);
					}

					for (j = q + 1; j < SB_JACOBI_RANK; j ++)
					{
						g         = a [p] [j];
						h         = a [q] [j];
						a [p] [j] = g - s * (h + g * tau);
						a [q] [j] = h + s * (g - h * tau);
					}

					for (j = 0; j < SB_JACOBI_RANK; j ++)
					{
						g                = evectors [j] [p];
						h                = evectors [j] [q];
						evectors [j] [p] = g - s * (h + g * tau);
						evectors [j] [q] = h + s * (g - h * tau);
					}
				}

				rots ++;
			}
		}

		for (p = 0; p < SB_JACOBI_RANK; p ++)
		{
			evalues [p] = b [p] += z [p];
			z [p]       = 0;
		}
	}
}

#undef SB_JACOBI_RANK

template <typename Type>
Type
matrix3 <Type>::det2 () const
{
	return
	   value [0] * value [4]
	   - value [1] * value [3];
}

template <typename Type>
Type
matrix3 <Type>::determinant () const
{
	return value [0] * (value [4] * value [8] - value [5] * value [7])
	       - value [1] * (value [3] * value [8] - value [5] * value [6])
	       + value [2] * (value [3] * value [7] - value [4] * value [6]);
}

template <typename Type>
matrix3 <Type> &
matrix3 <Type>::transpose ()
{
	return *this = matrix3 <Type> (value [0], value [3], value [6],
	                               value [1], value [4], value [7],
	                               value [2], value [5], value [8]);
}

template <typename Type>
matrix3 <Type> &
matrix3 <Type>::inverse ()
{
	Type m00 = value [0];
	Type m01 = value [1];
	Type m02 = value [2];

	Type m10 = value [3];
	Type m11 = value [4];
	Type m12 = value [5];

	Type m20 = value [6];
	Type m21 = value [7];
	Type m22 = value [8];

	Type t4  = m00 * m11;
	Type t6  = m00 * m21;
	Type t8  = m10 * m01;
	Type t10 = m10 * m21;
	Type t12 = m20 * m01;
	Type t14 = m20 * m11;

	Type d = (t4 * m22 - t6 * m12 - t8 * m22 + t10 * m02 + t12 * m12 - t14 * m02);

	if (d == 0)
		throw std::domain_error ("matrix3::inverse: determinant is 0.");

	return *this = matrix3 <Type> ((m11 * m22 - m21 * m12) / d,
	                       -(m01 * m22 - m21 * m02) / d,
	                       (m01 * m12 - m11 * m02) / d,

	                       -(m10 * m22 - m20 * m12) / d,
	                       (m00 * m22 - m20 * m02) / d,
	                       -(m00 * m12 - m10 * m02) / d,

	                       (t10 - t14) / d,
	                       -(t6 - t12) / d,
	                       (t4 - t8) / d);
}

template <typename Type>
template <typename T>
matrix3 <Type> &
matrix3 <Type>::multLeft (const matrix3 <T> & matrix)
{
	#define MULT_LEFT(i, j) \
	   (value [0 * 3 + j] * matrix .value [i * 3 + 0]   \
	    + value [1 * 3 + j] * matrix .value [i * 3 + 1]   \
	    + value [2 * 3 + j] * matrix .value [i * 3 + 2])

	return *this = matrix3 <Type> (MULT_LEFT (0, 0),
	                               MULT_LEFT (0, 1),
	                               MULT_LEFT (0, 2),

	                               MULT_LEFT (1, 0),
	                               MULT_LEFT (1, 1),
	                               MULT_LEFT (1, 2),

	                               MULT_LEFT (2, 0),
	                               MULT_LEFT (2, 1),
	                               MULT_LEFT (2, 2));

	#undef MULT_LEFT
}

template <typename Type>
template <typename T>
matrix3 <Type> &
matrix3 <Type>::multRight (const matrix3 <T> & matrix)
{
	#define MULT_RIGHT(i, j) \
	   (value [i * 3 + 0] * matrix .value [0 * 3 + j]   \
	    + value [i * 3 + 1] * matrix .value [1 * 3 + j]   \
	    + value [i * 3 + 2] * matrix .value [2 * 3 + j])

	return *this = matrix3 <Type> (MULT_RIGHT (0, 0),
	                               MULT_RIGHT (0, 1),
	                               MULT_RIGHT (0, 2),

	                               MULT_RIGHT (1, 0),
	                               MULT_RIGHT (1, 1),
	                               MULT_RIGHT (1, 2),

	                               MULT_RIGHT (2, 0),
	                               MULT_RIGHT (2, 1),
	                               MULT_RIGHT (2, 2));

	#undef MULT_RIGHT
}

template <typename Type>
template <typename T>
vector2 <Type>
matrix3 <Type>::multVecMatrix (const vector2 <T> & vector) const
{
	Type w = vector .x () * value [2] + vector .y () * value [5] + value [8];

	return vector2 <Type> ((vector .x () * value [0] + vector .y () * value [3] + value [6]) / w,
	                       (vector .x () * value [1] + vector .y () * value [4] + value [7]) / w);
}

template <typename Type>
template <typename T>
vector2 <Type>
matrix3 <Type>::multMatrixVec (const vector2 <T> & vector) const
{
	Type w = vector .x () * value [6] + vector .y () * value [7] + value [8];

	return vector2 <Type> ((vector .x () * value [0] + vector .y () * value [1] + value [2]) / w,
	                       (vector .x () * value [3] + vector .y () * value [4] + value [5]) / w);
}

template <typename Type>
template <typename T>
vector2 <Type>
matrix3 <Type>::multDirMatrix (const vector2 <T> & vector) const
{
	return vector2 <Type> (vector .x () * value [0] + vector .y () * value [3],
	                       vector .x () * value [1] + vector .y () * value [4]);
}

template <typename Type>
matrix3 <Type> &
matrix3 <Type>::operator *= (const Type & v)
{
	value [0] *= v;
	value [1] *= v;
	value [2] *= v;

	value [3] *= v;
	value [4] *= v;
	value [5] *= v;

	value [6] *= v;
	value [7] *= v;
	value [8] *= v;

	return *this;
}

template <typename Type>
template <typename T>
matrix3 <Type> &
matrix3 <Type>::operator *= (const matrix3 <T> & matrix)
{
	return multRight (matrix);
}

template <typename Type>
matrix3 <Type> &
matrix3 <Type>::operator /= (const Type & v)
{
	value [0] /= v;
	value [1] /= v;
	value [2] /= v;

	value [3] /= v;
	value [4] /= v;
	value [5] /= v;

	value [6] /= v;
	value [7] /= v;
	value [8] /= v;

	return *this;
}

template <typename Type>
Type &
matrix3 <Type>::operator [ ] (const size_type index)
{
	return value [index];
}

template <typename Type>
const
Type &
matrix3 <Type>::operator [ ] (const size_type index) const
{
	return value [index];
}

template <typename Type>
bool
operator == (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return
	   lhs .value [ 0] == rhs .value [ 0] and
	   lhs .value [ 1] == rhs .value [ 1] and
	   lhs .value [ 2] == rhs .value [ 2] and
	   lhs .value [ 3] == rhs .value [ 3] and
	   lhs .value [ 4] == rhs .value [ 4] and
	   lhs .value [ 5] == rhs .value [ 5] and
	   lhs .value [ 6] == rhs .value [ 6] and
	   lhs .value [ 7] == rhs .value [ 7] and
	   lhs .value [ 8] == rhs .value [ 8];
}

template <typename Type>
inline
bool
operator not_eq (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return not (lhs == rhs);
}

template <typename Type>
inline
matrix3 <Type>
operator ! (const matrix3 <Type> & matrix)
{
	return matrix3 <Type> (matrix) .transpose ();
}

template <typename Type>
inline
matrix3 <Type>
transpose (const matrix3 <Type> & matrix)
{
	return matrix3 <Type> (matrix) .transpose ();
}

template <typename Type>
inline
matrix3 <Type>
operator ~ (const matrix3 <Type> & matrix)
{
	return matrix3 <Type> (matrix) .inverse ();
}

template <typename Type>
inline
matrix3 <Type>
inverse (const matrix3 <Type> & matrix)
{
	return matrix3 <Type> (matrix) .inverse ();
}

template <typename Type>
inline
matrix3 <Type>
operator * (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return matrix3 <Type> (lhs) .multRight (rhs);
}

template <class Type>
inline
vector2 <Type>
operator * (const matrix3 <Type> & lhs, const vector2 <Type> & rhs)
{
	return lhs .multVecMatrix (rhs);
}

template <class Type>
inline
vector2 <Type>
operator * (const vector2 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return rhs .multMatrixVec (lhs);
}

///  Extraction operator for vector values.
template <typename CharT, class Traits, typename Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, matrix3 <Type> & matrix)
{
	typename matrix3 <Type>::Matrix value;

	istream
		>> value [0]
		>> value [1]
		>> value [2]
		>> value [3]
		>> value [4]
		>> value [5]
		>> value [6]
		>> value [7]
		>> value [8];

	if (istream)
		matrix .set (value);

	return istream;
}

///  Insertion operator for vector values.
template <typename CharT, class Traits, typename Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const matrix3 <Type> & matrix)
{
	return ostream
	       << matrix [0] << ' '
	       << matrix [1] << ' '
	       << matrix [2] << ' '
	       << matrix [3] << ' '
	       << matrix [4] << ' '
	       << matrix [5] << ' '
	       << matrix [6] << ' '
	       << matrix [7] << ' '
	       << matrix [8];
}

extern template class matrix3 <float>;
extern template class matrix3 <double>;
extern template class matrix3 <long double>;

//
extern template std::istream & operator >> (std::istream &, matrix3 <float> &);
extern template std::istream & operator >> (std::istream &, matrix3 <double> &);
extern template std::istream & operator >> (std::istream &, matrix3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const matrix3 <float> &);
extern template std::ostream & operator << (std::ostream &, const matrix3 <double> &);
extern template std::ostream & operator << (std::ostream &, const matrix3 <long double> &);

} // math
} // titania

#undef INDEX3

#endif
