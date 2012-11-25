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
 ******************************************************************************/

#ifndef __TITANIA_X3D_FIELDS_X3DSCALAR_H__
#define __TITANIA_X3D_FIELDS_X3DSCALAR_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

template <>
void
X3DField <Bool>::toStream (std::ostream & ostream) const;

template <>
void
X3DField <Double>::toStream (std::ostream & ostream) const;

template <>
void
X3DField <Float>::toStream (std::ostream & ostream) const;

extern template class X3DField <Bool>;
extern template class X3DField <Double>;
extern template class X3DField <Float>;
extern template class X3DField <Int32>;

template <class ValueType>
class X3DScalar :
	public X3DField <ValueType>
{
public:

	typedef ValueType scalar_type;

	using X3DField <ValueType>::operator =;
	using X3DField <ValueType>::getValue;

	X3DScalar () :
		X3DField <ValueType> () { }

	X3DScalar (const X3DScalar & field) :
		X3DField <ValueType> (field) { }

	explicit
	X3DScalar (const ValueType & value) :
		X3DField <ValueType> (value) { }

	virtual
	X3DScalar*
	copy () const { return new X3DScalar <ValueType> (*this); }

};

extern template class X3DScalar <Bool>;
extern template class X3DScalar <Double>;
extern template class X3DScalar <Float>;
extern template class X3DScalar <Int32>;

typedef X3DScalar <Bool>   SFBool;
typedef X3DScalar <Double> SFDouble;
typedef X3DScalar <Float>  SFFloat;
typedef X3DScalar <Int32>  SFInt32;

} // X3D
} // titania

#endif
