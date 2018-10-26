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

#include <Titania/X3D.h>

#include <btBulletDynamicsCommon.h>

using namespace titania;
using namespace titania::X3D;

using Vector4ui = math::vector4 <uint8_t>;

Vector3f
fract (const Vector3f & vector)
{
	return vector - floor (vector);
}

Vector4f
pack (const float value)
{
	const Vector3f bitShifts = Vector3f (255.0,
	                                     255.0 * 255.0,
	                                     255.0 * 255.0 * 255.0);

	const Vector3f f = fract (value * bitShifts);

	return Vector4f (value, f .x (), f .y (), f .z ());
}

float
unpack (const Vector4f & color)
{
	const Vector3f bitShifts = Vector3f (1.0 / 255.0,
	                                     1.0 / (255.0 * 255.0),
	                                     1.0 / (255.0 * 255.0 * 255.0));

	return color .x () + dot (Vector3f (color .y (), color .z (), color .w ()), bitShifts);
}

Vector4ui
to_int (const Vector4f & color)
{
	return Vector4ui (color * 255.0f);
}

Vector4f
to_float (const Vector4ui & color)
{
	return Vector4f (color) * (1 / 255.0f);
}

int
main (int argc, char** argv)
{
	std::clog << "Starting main ..." << std::endl;
	std::cout << std::boolalpha;
	std::clog << std::boolalpha;
	std::cerr << std::boolalpha;
	std::cout << std::setprecision (std::numeric_limits <float>::digits10);
	std::clog << std::setprecision (std::numeric_limits <float>::digits10);
	std::cerr << std::setprecision (std::numeric_limits <float>::digits10);
	//std::cout << std::setprecision (std::numeric_limits <double>::digits10);
	//std::clog << std::setprecision (std::numeric_limits <double>::digits10);
	//std::cerr << std::setprecision (std::numeric_limits <double>::digits10);
	std::cout .imbue (std::locale (""));
	std::clog .imbue (std::locale (""));
	std::cerr .imbue (std::locale (""));

	std::locale::global (std::locale (""));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	auto b = Box3d (Vector3d (0, 0, 0), Vector3d ());

	__LOG__ << b .matrix () << std::endl;

	b .rotate (Rotation4d (0, 1, 0, pi <double> / 4));

	__LOG__ << b .matrix () << std::endl;

	auto n = b .normals ();
	
	for (const auto & v : n)
		__LOG__ << v << std::endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
