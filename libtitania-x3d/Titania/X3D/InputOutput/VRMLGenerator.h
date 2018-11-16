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

#ifndef __TITANIA_X3D_INPUT_OUTPUT_VRMLGENERATOR_H__
#define __TITANIA_X3D_INPUT_OUTPUT_VRMLGENERATOR_H__

#include "../InputOutput/Generator.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

class VRMLGenerator
{
public:

	// VRML

	static
	void
	Encode (std::ostream & ostream, const bool value, const UnitCategory unitCategory);

	static
	void
	Encode (std::ostream & ostream, const Color3f & value, const UnitCategory unitCategory);

	static
	void
	Encode (std::ostream & ostream, const Color4f & value, const UnitCategory unitCategory);

	static
	void
	Encode (std::ostream & ostream, const double value, const UnitCategory unitCategory);

	static
	void
	Encode (std::ostream & ostream, const float value, const UnitCategory unitCategory);

	static
	void
	Encode (std::ostream & ostream, const int32_t value, const UnitCategory unitCategory);

	template <class Type>
	static
	void
	Encode (std::ostream & ostream, const matrix3 <Type> & value, const UnitCategory unitCategory)
	{ ostream << X3DGenerator::SetPrecision <Type> << value; }

	template <class Type>
	static
	void
	Encode (std::ostream & ostream, const matrix4 <Type> & value, const UnitCategory unitCategory)
	{ ostream << X3DGenerator::SetPrecision <Type> << value; }

	template <class Type>
	static
	void
	Encode (std::ostream & ostream, const rotation4 <Type> & value, const UnitCategory unitCategory)
	{
		ostream
			<< Generator::SetPrecision <Type>
			<< value .x ()
			<< Generator::Space
			<< value .y ()
			<< Generator::Space
			<< value .z ()
			<< Generator::Space
			<< Generator::ToUnit (ostream, UnitCategory::ANGLE, value .angle ());
	}

	static
	void
	Encode (std::ostream & ostream, const String & value, const UnitCategory unitCategory);

	template <class Type>
	static
	void
	Encode (std::ostream & ostream, const vector2 <Type> & value, const UnitCategory unitCategory)
	{
		const auto unit = Generator::Unit (ostream, unitCategory);

		ostream
			<< Generator::SetPrecision <Type>
			<< Generator::ToUnit (ostream, unit, value .x ())
			<< Generator::Space
			<< Generator::ToUnit (ostream, unit, value .y ());
	}

	template <class Type>
	static
	void
	Encode (std::ostream & ostream, const vector3 <Type> & value, const UnitCategory unitCategory)
	{
		const auto unit = Generator::Unit (ostream, unitCategory);

		ostream
			<< Generator::SetPrecision <Type>
			<< Generator::ToUnit (ostream, unit, value .x ())
			<< Generator::Space
			<< Generator::ToUnit (ostream, unit, value .y ())
			<< Generator::Space
			<< Generator::ToUnit (ostream, unit, value .z ());
	}

	template <class Type>
	static
	void
	Encode (std::ostream & ostream, const vector4 <Type> & value, const UnitCategory unitCategory)
	{
		const auto unit = Generator::Unit (ostream, unitCategory);

		ostream
			<< Generator::SetPrecision <Type>
			<< Generator::ToUnit (ostream, unit, value .x ())
			<< Generator::Space
			<< Generator::ToUnit (ostream, unit, value .y ())
			<< Generator::Space
			<< Generator::ToUnit (ostream, unit, value .z ())
			<< Generator::Space
			<< Generator::ToUnit (ostream, unit, value .w ());
	}

	template <class Type>
	static
	std::enable_if_t <std::is_enum_v <Type>, void>
	Encode (std::ostream & ostream, const Type value, const UnitCategory unitCategory)
	{ Encode (ostream, int32_t (value), unitCategory); }

};

} // X3D
} // titania

#endif
