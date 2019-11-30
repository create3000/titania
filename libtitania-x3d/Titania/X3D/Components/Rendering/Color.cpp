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

#include "Color.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   Color::component      = Component ("Rendering", 1);
const std::string Color::typeName       = "Color";
const std::string Color::containerField = "color";

Color::Fields::Fields () :
	color (new MFColor ())
{ }

Color::Color (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DColorNode (),
	      fields ()
{
	addType (X3DConstants::Color);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "color",    color ());
}

X3DBaseNode*
Color::create (X3DExecutionContext* const executionContext) const
{
	return new Color (executionContext);
}

void
Color::set1Color (const size_t index, const Color4f & value)
{
	color () .set1Value (index, Color3f (value .r (), value .g (), value .b ()));
}

Color4f
Color::get1Color (const size_t index) const
{
	if (index < color () .size ())
	{
		const auto & value = color () [index];
		return Color4f (value .r (), value .g (), value .b (), 1);
	}
	else if (color () .size ())
	{
		const auto & value = color () .back ();
		return Color4f (value .r (), value .g (), value .b (), 1);
	}

	return Color4f (1, 1, 1, 1);
}

void
Color::addColor (std::vector <Color4f> & colors, const size_t index) const
{
	const auto size = color () .size ();

	if (index < size)
	{
		const Color3f & color3 = color () [index];

		colors .emplace_back (color3 .r (), color3 .g (), color3 .b (), 1);
	}
	else if (size)
	{
		const Color3f & color3 = color () .back ();

		colors .emplace_back (color3 .r (), color3 .g (), color3 .b (), 1);
	}
	else
		colors .emplace_back (1, 1, 1, 1);
}

void
Color::getHSVA (std::vector <Vector4f> & colors) const
{
	colors .reserve (color () .size ());

	for (const Color3f & color3 : color ())
	{
		const auto hsv = color3 .hsv ();

		colors .emplace_back (hsv [0], hsv [1], hsv [2], 1);
	}
}

void
Color::assignColors (const std::vector <Color4f> & colors)
{
	color () .clear ();

	for (const auto & c : colors)
		color () .emplace_back (c .r (), c .g (), c .b ());
}

void
Color::eraseColor (const size_t index)
{
	if (index < color () .size ())
		color () .erase (color () .begin () + index);
}

} // X3D
} // titania
