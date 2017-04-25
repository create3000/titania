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

#ifndef __TITANIA_X3D_INPUT_OUTPUT_X3DBASE_GENERATOR_H__
#define __TITANIA_X3D_INPUT_OUTPUT_X3DBASE_GENERATOR_H__

#include "../Basic/X3DConstants.h"

#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <ostream>
#include <string>

namespace titania {
namespace X3D {

class X3DGenerator
{
public:

	///  @name Member types

	enum class StyleType :
		uint8_t
	{
		SMALLEST,
		SMALL,
		COMPACT,
		NICEST
	};

	struct X3DAccessType { const AccessType accessType; };
	
	struct VrmlAccessType { const AccessType accessType; };

	///  @name Output style handling

	static
	void
	Style (std::ostream & ostream, const std::string &);

	static
	StyleType
	Style (std::ostream & ostream)
	{ return get (ostream) -> style; }

	static
	std::ostream &
	SmallestStyle (std::ostream & ostream)
	{
		get (ostream) -> SmallestStyle ();
		return ostream;
	}

	static
	std::ostream &
	SmallStyle (std::ostream & ostream)
	{
		get (ostream) -> SmallStyle ();
		return ostream;
	}

	static
	std::ostream &
	CompactStyle (std::ostream & ostream)
	{
		get (ostream) -> CompactStyle ();
		return ostream;
	}

	static
	std::ostream &
	NicestStyle (std::ostream & ostream)
	{
		get (ostream) -> NicestStyle ();
		return ostream;
	}

	///  @name Operataions

	static
	std::ostream &
	Comment (std::ostream & ostream)
	{ return ostream << get (ostream) -> comment; }

	static
	std::ostream &
	Space (std::ostream & ostream)
	{ return ostream << get (ostream) -> space; }

	static
	std::ostream &
	TidySpace (std::ostream & ostream)
	{ return ostream << get (ostream) -> tidySpace; }

	static
	std::ostream &
	ForceBreak (std::ostream & ostream)
	{ return ostream << get (ostream) -> forceBreak; }

	static
	std::ostream &
	Break (std::ostream & ostream)
	{ return ostream << get (ostream) -> endl; }

	static
	std::ostream &
	TidyBreak (std::ostream & ostream)
	{ return ostream << get (ostream) -> tidyBreak; }

	static
	std::ostream &
	ListBreak (std::ostream & ostream)
	{ return ostream << get (ostream) -> listBreak; }

	static
	std::ostream &
	Comma (std::ostream & ostream)
	{ return ostream << get (ostream) -> comma; }

	static
	bool
	ListSpace (std::ostream & ostream)
	{ return get (ostream) -> listSpace; }

	static
	bool
	HasListBreak (std::ostream & ostream)
	{ return get (ostream) -> hasListBreak; }

	static
	bool
	HasBreak (std::ostream & ostream)
	{ return get (ostream) -> hasBreak; }

	static
	std::ostream &
	Indent (std::ostream & ostream)
	{ return ostream << get (ostream) -> indent; }

	static
	std::ostream &
	IncIndent (std::ostream & ostream)
	{
		get (ostream) -> indent += get (ostream) -> indentChar;
		return ostream;
	}

	static
	std::ostream &
	DecIndent (std::ostream & ostream)
	{
		const auto size = get (ostream) -> indent .size () - get (ostream) -> indentChar .size ();

		get (ostream) -> indent .resize (size);
		return ostream;
	}

	static
	std::ostream &
	OpenBracket (std::ostream &);

	static
	std::ostream &
	CloseBracket (std::ostream &);

	static
	std::ostream &
	EmptyBrackets (std::ostream &);

	template <class T>
	static
	std::ostream &
	SetPrecision (std::ostream & ostream)
	{ return ostream << std::setprecision (NumericLimits <T>::digits10); }

	template <class T>
	static
	void
	Precision (std::ostream & ostream, const size_t value)
	{ NumericLimits <T>::digits10 = value; }

	template <class T>
	static
	size_t
	Precision (std::ostream & ostream)
	{ return NumericLimits <T>::digits10; }

	///  @name Destruction

	virtual
	~X3DGenerator ();


protected:

	///  @name Construction

	X3DGenerator (std::ostream & ostream);

private:

	///  @name Construction

	static
	X3DGenerator*
	get (std::ostream & ostream);

	///  @name Style type handling

	void
	SmallestStyle ();

	void
	SmallStyle ();

	void
	CompactStyle ();

	void
	NicestStyle ();

	///  @name Member types

	template <class T>
	struct NumericLimits
	{
		static size_t digits10;
	};

	///  @name Members

	StyleType   style;
	std::string comment;
	std::string space;
	std::string tidySpace;
	std::string forceBreak;
	std::string endl;
	std::string tidyBreak;
	std::string listBreak;
	std::string comma;
	bool        listSpace;
	bool        hasListBreak;
	bool        hasBreak;

	std::string indent;
	std::string indentChar;

};

template <class T>
size_t X3DGenerator::NumericLimits <T>::digits10 = std::numeric_limits <T>::digits10;

} // X3D
} // titania

#endif
