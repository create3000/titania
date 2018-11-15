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

#include "X3DScalar.h"

#include "../Parser/MiniParser.h"

namespace titania {
namespace X3D {

template <>
const std::string X3DField <bool>::typeName = "SFBool";

template <>
const std::string X3DField <double>::typeName = "SFDouble";

template <>
const std::string X3DField <float>::typeName = "SFFloat";

template <>
const std::string X3DField <int32_t>::typeName = "SFInt32";

template <>
const FieldType X3DField <bool>::type = X3DConstants::SFBool;

template <>
const FieldType X3DField <double>::type = X3DConstants::SFDouble;

template <>
const FieldType X3DField <float>::type = X3DConstants::SFFloat;

template <>
const FieldType X3DField <int32_t>::type = X3DConstants::SFInt32;

template <>
void
X3DScalar <bool>::fromStream (std::istream & istream)
{
	bool value = false;

	if (MiniParser::Decode (istream, value))
		setValue (value);
}

template <>
void
X3DScalar <double>::fromStream (std::istream & istream)
{
	double value = 0;

	if (MiniParser::Decode (istream, value))
		setValue (value);
}

template <>
void
X3DScalar <float>::fromStream (std::istream & istream)
{
	float value = 0;

	if (MiniParser::Decode (istream, value))
		setValue (value);
}

template <>
void
X3DScalar <int32_t>::fromStream (std::istream & istream)
{
	int32_t value = 0;

	if (MiniParser::Decode (istream, value))
		setValue (value);
}

template class X3DField <bool>;
template class X3DField <double>;
template class X3DField <float>;
template class X3DField <int32_t>;

// SFBool, SFDouble, SFFloat and SFInt32
template class X3DScalar <bool>;
template class X3DScalar <double>;
template class X3DScalar <float>;
template class X3DScalar <int32_t>;

} // X3D
} // titania
