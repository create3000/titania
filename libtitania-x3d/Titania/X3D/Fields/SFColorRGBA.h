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

#ifndef __TITANIA_X3D_FIELDS_SFCOLOR_RGBA_H__
#define __TITANIA_X3D_FIELDS_SFCOLOR_RGBA_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

extern template class X3DField <Color4f>;

class SFColorRGBA :
	public X3DField <Color4f>
{
public:

	typedef Color3f::value_type scalar_type;
	typedef size_t              size_type;

	using X3DField <Color4f>::setValue;
	using X3DField <Color4f>::getValue;
	using X3DField <Color4f>::operator =;

	SFColorRGBA ();

	SFColorRGBA (const SFColorRGBA &);

	explicit
	SFColorRGBA (const Color4f &);

	SFColorRGBA (const scalar_type &, const scalar_type &, const scalar_type &, const scalar_type &);

	virtual
	SFColorRGBA*
	clone () const;

	void
	setR (const scalar_type &);

	scalar_type
	getR () const;

	void
	setG (const scalar_type &);

	scalar_type
	getG () const;

	void
	setB (const scalar_type &);

	scalar_type
	getB () const;

	void
	setA (const scalar_type &);

	scalar_type
	getA () const;

	void
	set1Value (const size_type &, const scalar_type &);

	scalar_type
	get1Value (const size_type &) const;

	void
	setValue (const scalar_type &, const scalar_type &, const scalar_type &, const scalar_type &);

	void
	getValue (scalar_type &, scalar_type &, scalar_type &, scalar_type &) const;

	void
	setHSV (const scalar_type &, const scalar_type &, const scalar_type &);

	void
	getHSV (scalar_type &, scalar_type &, scalar_type &) const;

	///  Output operator.
	virtual
	void
	toStream (std::ostream &) const;


private:

	using X3DField <Color4f>::get;

};

} // X3D
} // titania

#endif
