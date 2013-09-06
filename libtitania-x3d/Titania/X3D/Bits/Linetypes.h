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

#ifndef __TITANIA_X3D_BITS_CAST_H__
#define __TITANIA_X3D_BITS_CAST_H__

#include "../Rendering/OpenGL.h"

#include <Titania/Math/Utility/StrToL.h>

namespace titania {
namespace X3D {

static std::vector <GLushort> linetypes = {
	math::strtol ("0000000000000000", 2), // 0 None
	math::strtol ("1111111111111111", 2), // 1 Solid
	math::strtol ("1111111110000000", 2), // 2 Dashed
	math::strtol ("1000100010001000", 2), // 3 Dotted
	math::strtol ("1111111110001000", 2), // 4 Dashed-dotted
	math::strtol ("1111100010001000", 2), // 5 Dash-dot-dot

	math::strtol ("1111111111111111", 2), // 6 (single arrow)
	math::strtol ("1111111111111111", 2), // 7 (single dot)
	math::strtol ("1111111111111111", 2), // 8 (double arrow)

	math::strtol ("1111111100000000", 2), // 9 (stitch line)
	math::strtol ("1111111000111000", 2), // 10 (chain line)
	math::strtol ("1111111110011100", 2), // 11 (center line)
	math::strtol ("1111111111100000", 2), // 12 (hidden line)
	math::strtol ("1111111011101110", 2)  // 13 (phantom line)

	// 14 (break line - style 1)
	// 15 (break line - style 2)
	// 16 User - specified dash pattern

};

} // X3D
} // titania

#endif
