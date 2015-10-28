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

#include "Random.h"

#include <limits>
#include <random>
#include <chrono>

namespace titania {
namespace X3D {

static std::uniform_int_distribution <int32_t>
uniform_int_distribution (std::numeric_limits <int32_t>::min ());

static std::uniform_real_distribution <float>
uniform_real_distribution (-1, 1);

static std::default_random_engine
random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

int32_t
randomi ()
{
	return uniform_int_distribution (random_engine);
}

float
random1 ()
{
	return uniform_real_distribution (random_engine);
}

float
random1 (const float min, const float max)
{
	return min + ((random1 () + 1) / 2) * (max - min);
}

Vector3f
random_normal ()
{
	const float theta = random1 () * M_PI;
	const float cphi  = random1 ();
	const float phi   = std::acos (cphi);
	const float r     = std::sin (phi);

	return Vector3f (std::sin (theta) * r,
	                 std::cos (theta) * r,
	                 cphi);
}

} // X3D
} // titania
