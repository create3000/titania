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

#ifndef __TITANIA_COMPOSED_WIDGETS_CAIRO_H__
#define __TITANIA_COMPOSED_WIDGETS_CAIRO_H__

#include "../Base/X3DEditorObject.h"

namespace titania {
namespace puck {

inline
void
draw_checker_board (const Cairo::RefPtr <Cairo::Context> & context,
                    const double x, const double y,
                    const double width, const double height,
                    const double w, const double h,
                    const X3D::Color4f & color1,
                    const X3D::Color4f & color2)
{
	const size_t columns = std::ceil (width / w);
	const size_t rows    = std::ceil (height / h);

	for (size_t r = 0; r < rows; ++ r)
	{
		for (size_t c = 0; c < columns; ++ c)
		{
			if (r % 2 - c % 2)
				context -> set_source_rgba (color2 .r (), color2 .g (), color2 .b (), color2 .a ());
			else
				context -> set_source_rgba (color1 .r (), color1 .g (), color1 .b (), color1 .a ());

			const auto sx = x + c * w;
			const auto sy = y + r * h;

			context -> rectangle (sx, sy, w, h);
			context -> fill ();
		}
	}
}

} // puck
} // titania

#endif
