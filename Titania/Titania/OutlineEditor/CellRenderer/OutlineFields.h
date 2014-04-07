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

#ifndef __TITANIA_OUTLINE_EDITOR_CELL_RENDERER_OUTLINE_FIELDS_H__
#define __TITANIA_OUTLINE_EDITOR_CELL_RENDERER_OUTLINE_FIELDS_H__

#include <Titania/X3D.h>

namespace titania {
namespace puck {

static constexpr int VALUES_MAX = 64;

static constexpr X3D::Image::size_type WIDTH_MAX  = 16;
static constexpr X3D::Image::size_type HEIGHT_MAX = 16;

static
std::string
get_field_value (const X3D::SFImage & field, const bool ellipsize)
{
	if (ellipsize)
	{
		const X3D::Image & image = field .getValue ();

		if (image .width () > WIDTH_MAX or image .height () > HEIGHT_MAX)
		{
			std::ostringstream ostream;

			ostream
				<< image .width ()
				<< X3D::Generator::Space
				<< image .height ()
				<< X3D::Generator::Space
				<< image .components ()
				<< X3D::Generator::ForceBreak
				<< "…";

			return ostream .str ();
		}
	}

	return field .toString ();
}

template <class Type>
static
std::string
array_to_string (const Type & array, const bool ellipsize)
{
	std::ostringstream ostream;

	if (array .empty ())
		return ostream .str ();

	const size_t lines = ellipsize ? std::min <size_t> (VALUES_MAX, array .size ()) : array .size ();

	for (const auto & value : basic::adapter (array .begin (), array .begin () + lines - 1))
	{
		ostream
			<< value
			<< X3D::Generator::ForceBreak;
	}

	ostream << array [lines - 1];

	if (lines < array .size ())
	{
		ostream
			<< X3D::Generator::ForceBreak
			<< "…";
	}

	return ostream .str ();
}

static
std::string
array_to_string (const X3D::MFImage & array, const bool ellipsize)
{
	if (ellipsize)
	{
		std::ostringstream ostream;

		if (array .empty ())
			return ostream .str ();

		const size_t lines = ellipsize ? std::min <size_t> (VALUES_MAX, array .size ()) : array .size ();

		for (const auto & value : basic::adapter (array .begin (), array .begin () + lines - 1))
		{
			ostream
				<< get_field_value (value, true)
				<< X3D::Generator::ForceBreak;
		}

		ostream << array [lines - 1];

		if (lines < array .size ())
		{
			ostream
				<< X3D::Generator::ForceBreak
				<< "…";
		}

		return ostream .str ();
	}

	return array_to_string <X3D::MFImage> (array, false);
}

static
std::string
get_field_value (X3D::X3DFieldDefinition* const field, const bool ellipsize)
{
	X3D::Generator::NicestStyle ();

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFNode:
			return "";

		case X3D::X3DConstants::SFImage:
			return get_field_value (*static_cast <X3D::SFImage*> (field), ellipsize);

		case X3D::X3DConstants::SFString:
			return static_cast <X3D::SFString*> (field) -> getValue ();

		case X3D::X3DConstants::MFBool:
			return array_to_string (*static_cast <X3D::MFBool*> (field), ellipsize);

		case X3D::X3DConstants::MFColor:
			return array_to_string (*static_cast <X3D::MFColor*> (field), ellipsize);

		case X3D::X3DConstants::MFColorRGBA:
			return array_to_string (*static_cast <X3D::MFColorRGBA*> (field), ellipsize);

		case X3D::X3DConstants::MFDouble:
			return array_to_string (*static_cast <X3D::MFDouble*> (field), ellipsize);

		case X3D::X3DConstants::MFFloat:
			return array_to_string (*static_cast <X3D::MFFloat*> (field), ellipsize);

		case X3D::X3DConstants::MFImage:
			return array_to_string (*static_cast <X3D::MFImage*> (field), ellipsize);

		case X3D::X3DConstants::MFInt32:
			return array_to_string (*static_cast <X3D::MFInt32*> (field), ellipsize);

		case X3D::X3DConstants::MFMatrix3d:
			return array_to_string (*static_cast <X3D::MFMatrix3d*> (field), ellipsize);

		case X3D::X3DConstants::MFMatrix3f:
			return array_to_string (*static_cast <X3D::MFMatrix3f*> (field), ellipsize);

		case X3D::X3DConstants::MFMatrix4d:
			return array_to_string (*static_cast <X3D::MFMatrix4d*> (field), ellipsize);

		case X3D::X3DConstants::MFMatrix4f:
			return array_to_string (*static_cast <X3D::MFMatrix4f*> (field), ellipsize);

		case X3D::X3DConstants::MFNode:
			return "";

		case X3D::X3DConstants::MFRotation:
			return array_to_string (*static_cast <X3D::MFRotation*> (field), ellipsize);

		case X3D::X3DConstants::MFString:
			return array_to_string (*static_cast <X3D::MFString*> (field), ellipsize);

		case X3D::X3DConstants::MFTime:
			return array_to_string (*static_cast <X3D::MFTime*> (field), ellipsize);

		case X3D::X3DConstants::MFVec2d:
			return array_to_string (*static_cast <X3D::MFVec2d*> (field), ellipsize);

		case X3D::X3DConstants::MFVec2f:
			return array_to_string (*static_cast <X3D::MFVec2f*> (field), ellipsize);

		case X3D::X3DConstants::MFVec3d:
			return array_to_string (*static_cast <X3D::MFVec3d*> (field), ellipsize);

		case X3D::X3DConstants::MFVec3f:
			return array_to_string (*static_cast <X3D::MFVec3f*> (field), ellipsize);

		case X3D::X3DConstants::MFVec4d:
			return array_to_string (*static_cast <X3D::MFVec4d*> (field), ellipsize);

		case X3D::X3DConstants::MFVec4f:
			return array_to_string (*static_cast <X3D::MFVec4f*> (field), ellipsize);

		default:
			return field -> toLocaleString (std::locale ());
	}
}

} // puck
} // titania

#endif
