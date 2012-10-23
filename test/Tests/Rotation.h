/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#include <Titania/Basic/Rotation4.h>
#include <Titania/Math.h>

#include "Obsolete/Rotation.h"

#include <cassert>
#include <chrono>

namespace titania {
namespace Test {

inline
float
frand (float min = -1, float max = 1)
{
	return min + ((max - min) * ((float) std::rand () / (float) RAND_MAX));
}

using namespace titania::basic;

class TestRotation
{
public:

	TestRotation ()
	{
		test1 ();
		test2 ();
		test3 ();
		test4 ();
		test5 ();
		test6 ();
		test7 ();
		test8 ();
		test9 ();
		test10 ();
	}

	void
	test1 ()
	{
		constexpr Quaternion <float> q;

		assert (q .x () == 0);
		assert (q .y () == 0);
		assert (q .z () == 0);
		assert (q .w () == 0);
	}

	void
	test2 ()
	{
		constexpr Quaternion <float> q (1, 2, 3, 4);

		assert (q .x () == 1);
		assert (q .y () == 2);
		assert (q .z () == 3);
		assert (q .w () == 4);
	}

	void
	test3 ()
	{
		Rotation4 <float> r;

		float x, y, z, angle;
		r .get (x, y, z, angle);

		assert (x == 0);
		assert (y == 0);
		assert (z == 1);
		assert (angle == 0);
	}

	void
	test4 ()
	{
		Rotation4 <float> r (1, 2, 3, 4);
		Rotation4 <float> v = r * Rotation4 <float> ();

		std::clog << Rotation4 <float> () << std::endl;
		std::clog << r << std::endl;
		std::clog << v << std::endl;

		assert (v == r);
	}

	void
	test5 ()
	{
		Quaternion <float> a (1, 2, 3, 0);
		Quaternion <float> b (1, 2, 3, 0);
		Quaternion <float> v = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << v << std::endl;

		assert (v .x () == 0);
		assert (v .y () == 0);
		assert (v .z () == 0);
		assert (v .w () == -14);

		a = Quaternion <float> (1, 0, 0, 0);
		b = Quaternion <float> (1, 0, 0, 0);
		v = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << v << std::endl;

		assert (v .x () == 0);
		assert (v .y () == 0);
		assert (v .z () == 0);
		assert (v .w () == -1);

		a = Quaternion <float> (0, 1, 0, 0);
		b = Quaternion <float> (0, 1, 0, 0);
		v = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << v << std::endl;

		assert (v .x () == 0);
		assert (v .y () == 0);
		assert (v .z () == 0);
		assert (v .w () == -1);

		a = Quaternion <float> (0, 0, 1, 0);
		b = Quaternion <float> (0, 0, 1, 0);
		v = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << v << std::endl;

		assert (v .x () == 0);
		assert (v .y () == 0);
		assert (v .z () == 0);
		assert (v .w () == -1);

		a = Quaternion <float> (0, 0, 0, 1);
		b = Quaternion <float> (0, 0, 0, 1);
		v = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << v << std::endl;

		assert (v .x () == 0);
		assert (v .y () == 0);
		assert (v .z () == 0);
		assert (v .w () == 1);

		a = Quaternion <float> (1, 0, 0, 0);
		b = Quaternion <float> (0, 1, 0, 0);
		v = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << v << std::endl;

		assert (v .x () == 0);
		assert (v .y () == 0);
		assert (v .z () == 1);
		assert (v .w () == 0);

		a = Quaternion <float> (1, 0, 0, 0);
		b = Quaternion <float> (0, 0, 1, 0);
		v = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << v << std::endl;

		assert (v .x () == 0);
		assert (v .y () == -1);
		assert (v .z () == 0);
		assert (v .w () == 0);

		a = Quaternion <float> (0, 0, 0, 1);
		b = Quaternion <float> (0, 0, 1, 0);
		v = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << v << std::endl;

		assert (v .x () == 0);
		assert (v .y () == 0);
		assert (v .z () == 1);
		assert (v .w () == 0);

		Vector3 <float> v1 (1, 0, 0);
		Vector3 <float> v2 (0, 0, 1);
		Vector3 <float> ve = cross (v1, v2);

		std::clog << v1 << std::endl;
		std::clog << v2 << std::endl;
		std::clog << ve << std::endl;
	}

	void
	test6 ()
	{
		constexpr Quaternion <float> a (0, 1, 2, 3);
		constexpr Quaternion <float> b (0, 1, 2, 3);
		constexpr float              d = dot (a, b);

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << d << std::endl;

		assert (d == 14);

		Quaternion <float> m = a * 2.0f;
		std::clog << m << std::endl;
	}

	void
	test7 ()
	{
		constexpr Quaternion <float> a (1, 3, 2, 9);
		constexpr Quaternion <float> b (3, 1, 4, 7);
		Quaternion <float>           s1 = math::slerp (normalize (a), normalize (b), 0.5f);
		Quaternion <float>           s2 = math::slerp (normalize (a), normalize (b), 0.5f);

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << "slerp " << s1 << std::endl;
		std::clog << "slerp " << s2 << std::endl;

		std::clog << Quaternion <float> (1, 0, 0, 0) * Quaternion <float> () << std::endl;
		std::clog << Quaternion <float> (0, 1, 0, 0) * Quaternion <float> () << std::endl;
		std::clog << Quaternion <float> (0, 0, 1, 0) * Quaternion <float> () << std::endl;
		std::clog << Quaternion <float> (0, 0, 0, 1) * Quaternion <float> () << std::endl;

		std::clog << Quaternion <float> (10, 0, 0, 0) * ~Quaternion <float> (0,  2, 0, 0) << std::endl;
		std::clog << Quaternion <float> (10, 0, 0, 0) * ~Quaternion <float> (0,  4, 0, 0) << std::endl;
		std::clog << Quaternion <float> (10, 0, 0, 0) * ~Quaternion <float> (0,  8, 0, 0) << std::endl;
		std::clog << Quaternion <float> (10, 0, 0, 0) * ~Quaternion <float> (0, 10, 0, 0) << std::endl;

		std::clog << Quaternion <float> (10, 0, 0, 0) * Quaternion <float> (0,  2, 0, 0) << std::endl;
		std::clog << Quaternion <float> (10, 0, 0, 0) * Quaternion <float> (0,  4, 0, 0) << std::endl;
		std::clog << Quaternion <float> (10, 0, 0, 0) * Quaternion <float> (0,  8, 0, 0) << std::endl;
		std::clog << Quaternion <float> (10, 0, 0, 0) * Quaternion <float> (0, 10, 0, 0) << std::endl;
	}

	void
	test8 ()
	{
		constexpr Quaternion <float> a (0, 0, 1, 0);
		constexpr Vector3 <float>    b (0, 1, 0);
		constexpr auto               d = a * b;

		std::clog << a << std::endl;
		std::clog << b << std::endl;
		std::clog << d << std::endl;

		assert (d == Vector3 <float> (0, -1, 0));
	}

	void
	test9 ()
	{
		constexpr Vector3 <float> a (1,  0, 0);
		constexpr Vector3 <float> b (0,  1, 0);
		constexpr Vector3 <float> c (-1,  0, 0);
		constexpr Vector3 <float> d (0, -1, 0);
		constexpr Vector3 <float> e (1,  0, 0);

		std::clog << a << std::endl;
		std::clog << math::slerp (a, b, 0.25f) << std::endl;
		std::clog << math::slerp (a, b, 0.50f) << std::endl;
		std::clog << math::slerp (a, b, 0.75f) << std::endl;
		std::clog << b << std::endl;
		std::clog << math::slerp (b, c, 0.25f) << std::endl;
		std::clog << math::slerp (b, c, 0.50f) << std::endl;
		std::clog << math::slerp (b, c, 0.75f) << std::endl;
		std::clog << c << std::endl;
		std::clog << math::slerp (c, d, 0.25f) << std::endl;
		std::clog << math::slerp (c, d, 0.50f) << std::endl;
		std::clog << math::slerp (c, d, 0.75f) << std::endl;
		std::clog << d << std::endl;
		std::clog << math::slerp (d, e, 0.25f) << std::endl;
		std::clog << math::slerp (d, e, 0.50f) << std::endl;
		std::clog << math::slerp (d, e, 0.75f) << std::endl;
		std::clog << e << std::endl;

	}

	//	void
	//	test9 ()
	//	{
	//		constexpr auto a = Quaternion <float> (9, 7, 2, 7) * ~Quaternion <float> (1, 3, 1, 2);
	//		constexpr auto b = Quaternion <float> (9, 7, 2, 7) / Quaternion <float> (1, 3, 1, 2);
	//
	//		std::clog << a << std::endl;
	//		std::clog << b << std::endl;
	//
	//		assert (a == b);
	//
	//		constexpr auto c = ~Quaternion <float> (9, 7, 2, 7) * Quaternion <float> (1, 3, 1, 2);
	//		constexpr auto d = ~(~Quaternion <float> (1, 3, 1, 2) * Quaternion <float> (9, 7, 2, 7));
	//
	//		std::clog << c << std::endl;
	//		std::clog << d << std::endl;
	//
	//		assert (c == d);
	//	}

	void
	test10 ()
	{
		std::srand (std::chrono::duration_cast <std::chrono::duration <unsigned>> (std::chrono::system_clock::now () .time_since_epoch ()) .count ());
		std::clog << "########################################" << std::endl;

		typedef titania::Obsolete::Rotation4 <float> ORotation;
		//		ORotation  a;
		//		Rotation4f b;

		assert (cmp (ORotation (), Rotation4f ()));
		assert (cmp (ORotation (0, 0, 1, 0), Rotation4f (0, 0, 1, 0)));
		assert (cmp (ORotation (1, 2, 3, 4), Rotation4f (1, 2, 3, 4)));

		{
			float
			   r1 = frand (), r2 = frand (), r3 = frand (), r4 = frand ();

			ORotation  a = ORotation  (r1, r2, r3, r4);
			Rotation4f b = Rotation4f (r1, r2, r3, r4);

			std::clog << a << std::endl;
			std::clog << b << std::endl;
			assert (cmp (a, b));

			std::clog << a * a << std::endl;
			std::clog << b * b << std::endl;
			assert (cmp (a * a, b * b));

			std::clog << ~a << std::endl;
			std::clog << ~b << std::endl;
			assert (cmp (~a, ~b));

			//			std::clog << a .getAxis () << std::endl;
			//			std::clog << b .axis ()    << std::endl;
			//			assert (a .getAxis ()  == b .axis ());
			//
			//			std::clog << a .getAngle () << std::endl;
			//			std::clog << b .angle ()    << std::endl;
			//			assert (a .getAngle () == b .angle ());
		}

		{
			float
			   r1 = frand (), r2 = frand (), r3 = frand (), r4 = frand ();

			ORotation  a = ORotation  (Vector3f (r1, r2, r3), r4);
			Rotation4f b = Rotation4f (Vector3f (r1, r2, r3), r4);

			std::clog << a << std::endl;
			std::clog << b << std::endl;
			assert (cmp (a, b));
		}

		{
			float
			   f1 = frand (), f2 = frand (), f3 = frand (),
			   t1 = frand (), t2 = frand (), t3 = frand ();

			ORotation  a = ORotation  (Vector3f (f1, f2, f3), Vector3f (t1, t2, t3));
			Rotation4f b = Rotation4f (Vector3f (f1, f2, f3), Vector3f (t1, t2, t3));

			std::clog << a << std::endl;
			std::clog << b << std::endl;
			assert (cmp (a, b));
		}

		{
			float
			   f1 = frand (), f2 = frand (), f3 = frand (),
			   t1 = frand (), t2 = frand (), t3 = frand ();

			ORotation  a = ORotation  (Vector3f (f1, f2, f3), Vector3f (t1, t2, t3));
			Rotation4f b = Rotation4f (Vector3f (f1, f2, f3), Vector3f (t1, t2, t3));

			Vector3f v (frand (), frand (), frand ());

			std::clog << a * v << std::endl;
			std::clog << b * v << std::endl;
			assert (a * v == b * v);
		}
	}

	bool
	cmp (const titania::Obsolete::Rotation4 <float> & a, const Rotation4f & b)
	{
		return
		   a .quaternion .x == b .quaternion () .x () and
		   a .quaternion .y == b .quaternion () .y () and
		   a .quaternion .z == b .quaternion () .z () and
		   a .quaternion .w == b .quaternion () .w ();
	}

};

} // Test
} // titania
