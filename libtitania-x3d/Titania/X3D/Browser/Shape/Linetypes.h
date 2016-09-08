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

#ifndef __TITANIA_X3D_BITS_LINETYPES_H__
#define __TITANIA_X3D_BITS_LINETYPES_H__

#include "../../Rendering/OpenGL.h"

namespace titania {
namespace X3D {

enum class LineType :
	uint8_t
{
	NONE,
	SOLID,
	DASHED,
	DOTTED,
	DASHED_DOTTED,
	DASH_DOT_DOT,

	SINGLE_ARROW,
	SINGLE_DOT,
	DOUBLE_ARROW,

	STITCH_LINE,
	CHAIN_LINE,
	CENTER_LINE,
	HIDDEN_LINE,
	PHANTOM_LINE,

	BREAK_LINE_1,
	BREAK_LINE_2

};

static const std::vector <GLushort> linetypes = {
	0b0000000000000000, // 0 None
	0b1111111111111111, // 1 Solid
	0b1111111110000000, // 2 Dashed
	0b1100110011001100, // 3 Dotted
	0b1111111110001000, // 4 Dashed-dotted
	0b1111100010001000, // 5 Dash-dot-dot

	0b1111111111111111, // 6 (single arrow)
	0b1111111111111111, // 7 (single dot)
	0b1111111111111111, // 8 (double arrow)

	0b1111111100000000, // 9 (stitch line)
	0b1111111000111000, // 10 (chain line)
	0b1111111110011100, // 11 (center line)
	0b1111111111100000, // 12 (hidden line)
	0b1111111011101110, // 13 (phantom line)

	0b1111111111111111, // 14 (break line - style 1)
	0b1111111111111111, // 15 (break line - style 2)
	0b1111111111111111  // 16 User - specified dash pattern

};

} // X3D
} // titania

#endif
