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

#include "X3DGenerator.h"

#include "Generator.h"

#include <Titania/String/toupper.h>

namespace titania {
namespace X3D {

X3DGenerator::X3DGenerator (std::ostream & ostream) :
	       style (StyleType::NICEST),
	     comment ("#"),
	       space (" "),
	   tidySpace (" "),
	  forceBreak ("\n"),
	        endl ("\n"),
	   tidyBreak ("\n"),
	   listBreak ("\n"),
	       comma (","),
	   listSpace (true),
	hasListBreak (true),
	    hasBreak (true),
	      indent (""),
	  indentChar ("  ")
{ }

X3DGenerator*
X3DGenerator::get (std::ostream & ostream)
{
	return Generator::get (ostream);
}

void
X3DGenerator::Style (std::ostream & ostream, const std::string & value)
{
	const std::string style = basic::toupper (value, std::locale::classic ());

	if (style == "SMALLEST")
		SmallestStyle (ostream);

	else if (style == "SMALL")
		SmallStyle (ostream);

	else if (style == "COMPACT")
		CompactStyle (ostream);

	else
		NicestStyle (ostream);
}

void
X3DGenerator::SmallestStyle ()
{
	if (style == StyleType::SMALLEST)
		return;

	style = StyleType::SMALLEST;

	space     = " ";
	tidySpace = "";
	endl      = " ";
	tidyBreak = "";
	listBreak = "";
	comma     = " ";

	indent     = "";
	indentChar = "";

	listSpace    = false;
	hasListBreak = false;
	hasBreak     = false;
}

void
X3DGenerator::SmallStyle ()
{
	if (style == StyleType::SMALL)
		return;

	style = StyleType::SMALL;

	space     = " ";
	tidySpace = "";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = "";
	comma     = ",";

	indent     = "";
	indentChar = "";

	listSpace    = false;
	hasListBreak = false;
	hasBreak     = true;
}

void
X3DGenerator::CompactStyle ()
{
	if (style == StyleType::COMPACT)
		return;

	style = StyleType::COMPACT;

	space     = " ";
	tidySpace = " ";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = " ";
	comma     = ",";

	indent     = "";
	indentChar = "  ";

	listSpace    = true;
	hasListBreak = false;
	hasBreak     = true;
}

void
X3DGenerator::NicestStyle ()
{
	if (style == StyleType::NICEST)
		return;

	style = StyleType::NICEST;

	space     = " ";
	tidySpace = " ";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = "\n";
	comma     = ",";

	indent     = "";
	indentChar = "  ";

	listSpace    = true;
	hasListBreak = true;
	hasBreak     = true;
}

std::ostream &
X3DGenerator::OpenBracket (std::ostream & ostream)
{
	ostream .put (ostream .widen ('['));

	if (ListSpace (ostream))
		ostream << ListBreak;

	ostream << IncIndent;

	if (HasListBreak (ostream))
		ostream << Indent;

	return ostream;
}

std::ostream &
X3DGenerator::CloseBracket (std::ostream & ostream)
{
	if (ListSpace (ostream))
		ostream << ListBreak;

	ostream << DecIndent;

	if (HasListBreak (ostream))
		ostream << Indent;

	ostream .put (ostream .widen (']'));
	return ostream;
}

std::ostream &
X3DGenerator::EmptyBrackets (std::ostream & ostream)
{
	ostream .put (ostream .widen ('['));
	ostream << TidySpace;
	ostream .put (ostream .widen (']'));
	return ostream;
}

X3DGenerator::~X3DGenerator ()
{ }

} // basic
} // titania
