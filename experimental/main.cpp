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
#include <Titania/X3D/Prototype/ProtoDeclaration.h>

#include <Titania/X3D/Prototype/ProtoDeclaration.h>

using namespace titania;
using namespace titania::X3D;

namespace titania {
namespace X3D {

template class X3DNativeArrayField <int32_t>;

}
}

template <class Type>
void
f ()
{
	static constexpr size_t N = 10'000'000;

	const auto t0 = chrono::now ();

	Type a (N);

	std::fill (a .begin (), a .end (), typename Type::value_type (1));

	auto y = a [1];
	auto z = a [1];

	std::swap (y, z);
	

	a [1] .swap (a [2]);
	std::swap (a [1], a [2]);

	std::clog << (chrono::now () - t0) << std::endl;
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

	auto a = [ ] (bool a, bool g) { return g ? false : not a; };
	auto b = [ ] (bool a, bool g) { return not (a or g); };

	__LOG__ << (a (false, false) == b (false, false)) << std::endl; 
	__LOG__ << (a (false, true)  == b (false, true))  << std::endl; 
	__LOG__ << (a (true,  false) == b (true,  false)) << std::endl; 
	__LOG__ << (a (true,  true)  == b (true,  true))  << std::endl; 

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
