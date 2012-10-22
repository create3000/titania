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

#ifndef __TEST_H__
#define __TEST_H__

#include <chrono>
#include <glibmm/timeval.h>
#include <iostream>
#include <string>

#include "Tests/signal.h"

namespace titania {
namespace Test {

using namespace std::chrono;

template <class Type>
void
benchmark (std::string name, Type function, size_t iterations = 1)
{
	Glib::TimeVal tv;

	tv .assign_current_time ();
	std::cout << std::endl << "### " << tv .as_iso8601 () << ": Starting benchmark '" << name << "' ..." << std::endl;

	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now ();

	while (iterations --)
		function ();

	std::chrono::high_resolution_clock::time_point stopTime = std::chrono::high_resolution_clock::now ();
	std::chrono::duration <double>                 duration =  std::chrono::duration_cast <std::chrono::duration <double>> (stopTime - startTime);

	tv .assign_current_time ();
	std::cout << "### " << tv .as_iso8601 () << ": Benchmark '" << name << "' tooks " << duration .count () << " seconds." << std::endl << std::endl;
}

} // Test
} // titania

#endif
