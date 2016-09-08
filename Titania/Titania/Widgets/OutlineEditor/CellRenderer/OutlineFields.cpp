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

#include "OutlineFields.h"

#include <sstream>

namespace titania {
namespace puck {

static constexpr int32_t VALUES_MAX = 64;

static constexpr X3D::Image::size_type WIDTH_MAX  = 16;
static constexpr X3D::Image::size_type HEIGHT_MAX = 16;

static
std::string
get_field_value (const X3D::X3DScene* const, const X3D::X3DFieldDefinition* const, const X3D::X3DFieldDefinition* const, const bool, const bool);

static
std::string
get_field_value (const X3D::X3DScene* const,
                 const X3D::SFImage & field,
                 const bool ellipsize,
                 const bool)
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

static
std::string
get_field_value (const X3D::X3DScene* const scene,
	              const X3D::X3DFieldDefinition* const parent,
	              const X3D::SFString* const fieldDefinition,
	              const bool ellipsize,
	              const bool useLocale)
{
	return useLocale ? fieldDefinition -> toLocaleString () : fieldDefinition -> toString ();
}

template <class Type>
static
std::string
array_to_string (const X3D::X3DScene* const scene,
	              const Type & array,
	              const bool ellipsize,
	              const bool useLocale)
{
	std::ostringstream ostream;

	if (array .empty ())
		return ostream .str ();

	const size_t lines = ellipsize ? std::min <size_t> (VALUES_MAX, array .size ()) : array .size ();

	for (const auto & value : basic::make_range (array .begin (), lines - 1))
	{
		ostream
			<< get_field_value (scene, &array, &value, false, useLocale)
			<< X3D::Generator::ForceBreak;
	}

	ostream << get_field_value (scene, &array, &array [lines - 1], false, useLocale);

	if (lines < array .size ())
	{
		ostream
			<< X3D::Generator::ForceBreak
			<< '[' << array .size () << "]  …­";
	}
	else
	{
		if (ellipsize)
		{
			ostream
				<< X3D::Generator::ForceBreak
				<< '[' << array .size () << "]";
		}
	}

	return ostream .str ();
}

static
std::string
array_to_string (const X3D::X3DScene* const scene,
	              const X3D::MFImage & array,
	              const bool ellipsize,
	              const bool useLocale)
{
	if (ellipsize)
	{
		std::ostringstream ostream;

		if (array .empty ())
			return ostream .str ();

		const size_t lines = ellipsize ? std::min <size_t> (VALUES_MAX, array .size ()) : array .size ();

		for (const auto & value : basic::make_range (array .begin (), lines - 1))
		{
			ostream
				<< get_field_value (scene, value, true, useLocale)
				<< X3D::Generator::ForceBreak;
		}

		ostream << get_field_value (scene, array [lines - 1], true, useLocale);

		if (lines < array .size ())
		{
			ostream
				<< X3D::Generator::ForceBreak
				<< '[' << array .size () << "] …";
		}

		return ostream .str ();
	}

	return array_to_string <X3D::MFImage> (scene, array, false, useLocale);
}

static
std::string
get_field_value (const X3D::X3DScene* const scene,
	              const X3D::X3DFieldDefinition* const parent,
	              const X3D::X3DFieldDefinition* const fieldDefinition,
	              const bool ellipsize,
	              const bool useLocale)
{
	X3D::Generator::NicestStyle ();

	switch (fieldDefinition -> getType ())
	{
		case X3D::X3DConstants::SFNode:
			return "";

		case X3D::X3DConstants::SFImage:
			return get_field_value (scene, *static_cast <const X3D::SFImage*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::SFString:
			return static_cast <const X3D::SFString*> (fieldDefinition) -> getValue ();

		case X3D::X3DConstants::MFBool:
			return array_to_string (scene, *static_cast <const X3D::MFBool*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFColor:
			return array_to_string (scene, *static_cast <const X3D::MFColor*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFColorRGBA:
			return array_to_string (scene, *static_cast <const X3D::MFColorRGBA*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFDouble:
			return array_to_string (scene, *static_cast <const X3D::MFDouble*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFFloat:
			return array_to_string (scene, *static_cast <const X3D::MFFloat*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFImage:
			return array_to_string (scene, *static_cast <const X3D::MFImage*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFInt32:
			return array_to_string (scene, *static_cast <const X3D::MFInt32*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFMatrix3d:
			return array_to_string (scene, *static_cast <const X3D::MFMatrix3d*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFMatrix3f:
			return array_to_string (scene, *static_cast <const X3D::MFMatrix3f*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFMatrix4d:
			return array_to_string (scene, *static_cast <const X3D::MFMatrix4d*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFMatrix4f:
			return array_to_string (scene, *static_cast <const X3D::MFMatrix4f*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFNode:
			return "";

		case X3D::X3DConstants::MFRotation:
			return array_to_string (scene, *static_cast <const X3D::MFRotation*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFString:
			return array_to_string (scene, *static_cast <const X3D::MFString*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFTime:
			return array_to_string (scene, *static_cast <const X3D::MFTime*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFVec2d:
			return array_to_string (scene, *static_cast <const X3D::MFVec2d*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFVec2f:
			return array_to_string (scene, *static_cast <const X3D::MFVec2f*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFVec3d:
			return array_to_string (scene, *static_cast <const X3D::MFVec3d*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFVec3f:
			return array_to_string (scene, *static_cast <const X3D::MFVec3f*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFVec4d:
			return array_to_string (scene, *static_cast <const X3D::MFVec4d*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::MFVec4f:
			return array_to_string (scene, *static_cast <const X3D::MFVec4f*> (fieldDefinition), ellipsize, useLocale);

		case X3D::X3DConstants::SFDouble:
		{
			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFDouble::value_type>
				<< scene -> toUnit (parent -> getUnit (), *static_cast <const X3D::SFDouble*> (fieldDefinition));

			return osstream .str ();
		}

		case X3D::X3DConstants::SFFloat:
		{
			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFFloat::value_type>
				<< scene -> toUnit (parent -> getUnit (), *static_cast <const X3D::SFFloat*> (fieldDefinition));

			return osstream .str ();
		}

		case X3D::X3DConstants::SFRotation:
		{
			X3D::SFRotation::value_type x, y, z, angle;

			static_cast <const X3D::SFRotation*> (fieldDefinition) -> getValue (x, y, z, angle);

			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFRotation::value_type>
				<< x
				<< " "
				<< y
				<< " "
				<< z
				<< " "
				<< scene -> toUnit (X3D::UnitCategory::ANGLE, angle);

			return osstream .str ();
		}

		case X3D::X3DConstants::SFVec2d:
		{
			const auto & field = *static_cast <const X3D::SFVec2d*> (fieldDefinition);
			const auto   unit  = parent -> getUnit ();

			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFVec2d::value_type>
				<< scene -> toUnit (unit, field .getX ())
				<< " "
				<< scene -> toUnit (unit, field .getY ());

			return osstream .str ();
		}

		case X3D::X3DConstants::SFVec2f:
		{
			const auto & field = *static_cast <const X3D::SFVec2f*> (fieldDefinition);
			const auto   unit  = parent -> getUnit ();

			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFVec2f::value_type>
				<< scene -> toUnit (unit, field .getX ())
				<< " "
				<< scene -> toUnit (unit, field .getY ());

			return osstream .str ();
		}

		case X3D::X3DConstants::SFVec3d:
		{
			const auto & field = *static_cast <const X3D::SFVec3d*> (fieldDefinition);
			const auto   unit  = parent -> getUnit ();
			const auto   geo   = parent -> isGeospatial ();

			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFVec3d::value_type>
				<< (geo ? field .getX () : scene -> toUnit (unit, field .getX ()))
				<< " "
				<< (geo ? field .getY () : scene -> toUnit (unit, field .getY ()))
				<< " "
				<< scene -> toUnit (unit, field .getZ ());

			return osstream .str ();
		}

		case X3D::X3DConstants::SFVec3f:
		{
			const auto & field = *static_cast <const X3D::SFVec3f*> (fieldDefinition);
			const auto   unit  = parent -> getUnit ();

			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFVec3f::value_type>
				<< scene -> toUnit (unit, field .getX ())
				<< " "
				<< scene -> toUnit (unit, field .getY ())
				<< " "
				<< scene -> toUnit (unit, field .getZ ());

			return osstream .str ();
		}

		case X3D::X3DConstants::SFVec4d:
		{
			const auto & field = *static_cast <const X3D::SFVec4d*> (fieldDefinition);
			const auto   unit  = parent -> getUnit ();

			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFVec4d::value_type>
				<< scene -> toUnit (unit, field .getX ())
				<< " "
				<< scene -> toUnit (unit, field .getY ())
				<< " "
				<< scene -> toUnit (unit, field .getZ ())
				<< " "
				<< scene -> toUnit (unit, field .getW ());

			return osstream .str ();
		}

		case X3D::X3DConstants::SFVec4f:
		{
			const auto & field = *static_cast <const X3D::SFVec4f*> (fieldDefinition);
			const auto   unit  = parent -> getUnit ();

			std::ostringstream osstream;

			if (not useLocale)
				osstream .imbue (std::locale::classic ());

			osstream
				<< X3D::Generator::Precision <X3D::SFVec4f::value_type>
				<< scene -> toUnit (unit, field .getX ())
				<< " "
				<< scene -> toUnit (unit, field .getY ())
				<< " "
				<< scene -> toUnit (unit, field .getZ ())
				<< " "
				<< scene -> toUnit (unit, field .getW ());

			return osstream .str ();
		}

		default:
			return useLocale ? fieldDefinition -> toLocaleString () : fieldDefinition -> toString ();
	}
}

std::string
get_field_value (const X3D::X3DScene* const scene,
	              const X3D::X3DFieldDefinition* const fieldDefinition,
	              const bool ellipsize,
	              const bool useLocale)
{ return get_field_value (scene, fieldDefinition, fieldDefinition, ellipsize, useLocale); }

/*
 * set_field_value
 */


static
void
set_field_value (const X3D::X3DScene* const, const X3D::X3DFieldDefinition* const, X3D::X3DFieldDefinition* const);

template <class Type>
static
void
set_array (const X3D::X3DScene* const scene, Type & array)
{
	for (auto & value : array)
	   set_field_value (scene, &array, &value);
}

static
void
set_field_value (const X3D::X3DScene* const scene,
	              const X3D::X3DFieldDefinition* const parent,
	              X3D::X3DFieldDefinition* const fieldDefinition)
{
	switch (fieldDefinition -> getType ())
	{
		case X3D::X3DConstants::MFDouble:
			return set_array (scene, *static_cast <X3D::MFDouble*> (fieldDefinition));

		case X3D::X3DConstants::MFFloat:
			return set_array (scene, *static_cast <X3D::MFFloat*> (fieldDefinition));

		case X3D::X3DConstants::MFVec2d:
			return set_array (scene, *static_cast <X3D::MFVec2d*> (fieldDefinition));

		case X3D::X3DConstants::MFVec2f:
			return set_array (scene, *static_cast <X3D::MFVec2f*> (fieldDefinition));

		case X3D::X3DConstants::MFVec3d:
			return set_array (scene, *static_cast <X3D::MFVec3d*> (fieldDefinition));

		case X3D::X3DConstants::MFVec3f:
			return set_array (scene, *static_cast <X3D::MFVec3f*> (fieldDefinition));

		case X3D::X3DConstants::MFVec4d:
			return set_array (scene, *static_cast <X3D::MFVec4d*> (fieldDefinition));

		case X3D::X3DConstants::MFVec4f:
			return set_array (scene, *static_cast <X3D::MFVec4f*> (fieldDefinition));

		case X3D::X3DConstants::SFDouble:
		{
			auto & field = *static_cast <X3D::SFDouble*> (fieldDefinition);

			field = scene -> fromUnit (parent -> getUnit (), field);
			return;
		}

		case X3D::X3DConstants::SFFloat:
		{
			auto & field = *static_cast <X3D::SFFloat*> (fieldDefinition);

			field = scene -> fromUnit (parent -> getUnit (), field);
			return;
		}

		case X3D::X3DConstants::SFVec2d:
		{
			auto &     field = *static_cast <X3D::SFVec2d*> (fieldDefinition);
			const auto unit  = parent -> getUnit ();

			field .setX (scene -> fromUnit (unit, field .getX ()));
			field .setY (scene -> fromUnit (unit, field .getY ()));
			return;
		}

		case X3D::X3DConstants::SFVec2f:
		{
			auto &     field = *static_cast <X3D::SFVec2f*> (fieldDefinition);
			const auto unit  = parent -> getUnit ();

			field .setX (scene -> fromUnit (unit, field .getX ()));
			field .setY (scene -> fromUnit (unit, field .getY ()));
			return;
		}

		case X3D::X3DConstants::SFVec3d:
		{
			auto &     field = *static_cast <X3D::SFVec3d*> (fieldDefinition);
			const auto unit  = parent -> getUnit ();

			if (not parent -> isGeospatial ())
			{
				field .setX (scene -> fromUnit (unit, field .getX ()));
				field .setY (scene -> fromUnit (unit, field .getY ()));
			}

			field .setZ (scene -> fromUnit (unit, field .getZ ()));
			return;
		}

		case X3D::X3DConstants::SFVec3f:
		{
			auto &     field = *static_cast <X3D::SFVec3f*> (fieldDefinition);
			const auto unit  = parent -> getUnit ();

			field .setX (scene -> fromUnit (unit, field .getX ()));
			field .setY (scene -> fromUnit (unit, field .getY ()));
			field .setZ (scene -> fromUnit (unit, field .getZ ()));
			return;
		}

		case X3D::X3DConstants::SFVec4d:
		{
			auto &     field = *static_cast <X3D::SFVec4d*> (fieldDefinition);
			const auto unit  = parent -> getUnit ();

			field .setX (scene -> fromUnit (unit, field .getX ()));
			field .setY (scene -> fromUnit (unit, field .getY ()));
			field .setZ (scene -> fromUnit (unit, field .getZ ()));
			field .setW (scene -> fromUnit (unit, field .getW ()));
			return;
		}

		case X3D::X3DConstants::SFVec4f:
		{
			auto &     field = *static_cast <X3D::SFVec4f*> (fieldDefinition);
			const auto unit  = parent -> getUnit ();

			field .setX (scene -> fromUnit (unit, field .getX ()));
			field .setY (scene -> fromUnit (unit, field .getY ()));
			field .setZ (scene -> fromUnit (unit, field .getZ ()));
			field .setW (scene -> fromUnit (unit, field .getW ()));
			return;
		}

		default:
			return;
	}
}

bool
set_field_value_from_string (const X3D::X3DScene* const scene,
	                          X3D::X3DFieldDefinition* const fieldDefinition,
	                          const std::string & string,
	                          const bool useLocale)
{
	const auto locale = useLocale ? std::locale () : std::locale::classic ();

	switch (fieldDefinition -> getType ())
	{
		case X3D::X3DConstants::SFRotation:
		{
			X3D::SFVec4d vector;

			if (not vector .fromLocaleString (string, locale))
				return false;

			auto & field = *static_cast <X3D::SFRotation*> (fieldDefinition);

			field = X3D::Rotation4d (vector .getX (),
			                         vector .getY (),
			                         vector .getZ (),
			                         scene -> fromUnit (X3D::UnitCategory::ANGLE, vector .getW ()));
		   return true;
		}
		case X3D::X3DConstants::MFRotation:
		{
			X3D::MFVec4d array;

			if (not array .fromLocaleString (string, locale))
				return false;

			auto & field = *static_cast <X3D::MFRotation*> (fieldDefinition);

			field .resize (array .size ());

			size_t i = 0;

			for (const auto & vector : array)
			{
				field [i] = X3D::Rotation4d (vector .getX (),
				                             vector .getY (),
				                             vector .getZ (),
				                             scene -> fromUnit (X3D::UnitCategory::ANGLE, vector .getW ()));
			
			   ++ i;
			}
		   
			return true;
		}
	   default:
		{
			if (not fieldDefinition -> fromLocaleString (string, locale))
				return false;

			set_field_value (scene, fieldDefinition, fieldDefinition);
			return true;
		}
	}

	return true;
}

} // puck
} // titania
