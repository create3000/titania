/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 * Copyright (c) 2008 Sean Morrison <learner@brlcad.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __TITANIA_FTGL_POINT_H__
#define __TITANIA_FTGL_POINT_H__

namespace titania {
namespace FTGL {

/**
 * Vector3d class is a basic 3-dimensional point or vector.
 */
class Vector3d
{
public:

	/**
	 * Default constructor. Vector3d is set to zero.
	 */
	inline
	Vector3d ()
	{
		values [0] = 0;
		values [1] = 0;
		values [2] = 0;
	}

	/**
	 * Constructor. Z coordinate is set to zero if unspecified.
	 *
	 * @param x First component
	 * @param y Second component
	 * @param z Third component
	 */
	inline
	Vector3d (const double x, const double y,
	          const double z = 0)
	{
		values [0] = x;
		values [1] = y;
		values [2] = z;
	}

	/**
	 * Operator += In Place Addition.
	 *
	 * @param point
	 * @return this plus point.
	 */
	inline Vector3d &
	operator += (const Vector3d & point)
	{
		values [0] += point .values [0];
		values [1] += point .values [1];
		values [2] += point .values [2];

		return *this;
	}

	/**
	 * Operator +
	 *
	 * @param point
	 * @return this plus point.
	 */
	inline Vector3d
	operator + (const Vector3d & point) const
	{
		Vector3d temp;

		temp .values [0] = values [0] + point .values [0];
		temp .values [1] = values [1] + point .values [1];
		temp .values [2] = values [2] + point .values [2];

		return temp;
	}

	/**
	 * Operator -= In Place Substraction.
	 *
	 * @param point
	 * @return this minus point.
	 */
	inline Vector3d &
	operator -= (const Vector3d & point)
	{
		values [0] -= point .values [0];
		values [1] -= point .values [1];
		values [2] -= point .values [2];

		return *this;
	}

	/**
	 * Operator -
	 *
	 * @param point
	 * @return this minus point.
	 */
	inline Vector3d
	operator - (const Vector3d & point) const
	{
		Vector3d temp;

		temp .values [0] = values [0] - point .values [0];
		temp .values [1] = values [1] - point .values [1];
		temp .values [2] = values [2] - point .values [2];

		return temp;
	}

	/**
	 * Operator *  Scalar multiplication
	 *
	 * @param multiplier
	 * @return <code>this</code> multiplied by <code>multiplier</code>.
	 */
	inline Vector3d
	operator * (double multiplier) const
	{
		Vector3d temp;

		temp .values [0] = values [0] * multiplier;
		temp .values [1] = values [1] * multiplier;
		temp .values [2] = values [2] * multiplier;

		return temp;
	}

	/**
	 * Operator *  Scalar multiplication
	 *
	 * @param point
	 * @param multiplier
	 * @return <code>multiplier</code> multiplied by <code>point</code>.
	 */
	inline friend Vector3d
	operator * (double multiplier, Vector3d & point)
	{
		return point * multiplier;
	}

	/**
	 * Operator == Tests for equality
	 *
	 * @param a
	 * @param b
	 * @return true if a & b are equal
	 */
	friend bool
	operator == (const Vector3d & a, const Vector3d & b);

	/**
	 * Operator not_eq Tests for non equality
	 *
	 * @param a
	 * @param b
	 * @return true if a & b are not equal
	 */
	friend bool
	operator not_eq (const Vector3d & a, const Vector3d & b);

	/**
	 * Cast to double*
	 */
	inline
	const double*
	data () const
	{
		return values;
	}

	/**
	 * Setters
	 */
	inline void
	x (double x) { values [0] = x; }

	inline void
	y (double y) { values [1] = y; }

	inline void
	z (double z) { values [2] = z; }

	/**
	 * Getters
	 */
	inline double
	x () const { return values [0]; }

	inline double
	y () const { return values [1]; }

	inline double
	z () const { return values [2]; }


private:

	/**
	 * The point data
	 */
	double values [3];

};

/**
 * Normalise a point's coordinates. If the coordinates are zero,
 * the point is left untouched.
 *
 * @return A vector of norm one.
 */
Vector3d
normalize (const Vector3d &);

} // FTGL
} // titania

#endif  //  __FTPoint__
