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

#include "X3DFontStyleNode.h"

#include <fontconfig/fontconfig.h>

namespace titania {
namespace X3D {

X3DFontStyleNode::X3DFontStyleNode () :
	X3DPropertyNode (),                                    
	         family ({ "SERIF" }),                         // MFString [ ] family       "SERIF"
	          style ("PLAIN"),                             // SFString [ ] style        "PLAIN"        ["PLAIN"|"BOLD"|"ITALIC"|"BOLDITALIC"|""]
	        spacing (1),                                   // SFFloat  [ ] spacing      1.0            [0,?)
	     horizontal (true),                                // SFBool   [ ] horizontal   TRUE
	        justify ({ "BEGIN" }),                         // MFString [ ] justify      "BEGIN"        ["BEGIN","END","FIRST","MIDDLE",""]
	    topToBottom (true),                                // SFBool   [ ] topToBottom  TRUE
	    leftToRight (true),                                // SFBool   [ ] leftToRight  TRUE
	       language (),                                    // SFString [ ] language     ""
	     alignments { Alignment::BEGIN, Alignment::BEGIN } 
{
	addNodeType (X3DConstants::X3DFontStyleNode);
}

void
X3DFontStyleNode::initialize ()
{
	X3DPropertyNode::initialize ();

	justify .addInterest (this, &X3DFontStyleNode::set_justify);

	set_justify ();
}

void
X3DFontStyleNode::set_justify ()
{
	alignments [0] = justify .size () > 0
	                 ? getAlignment (0)
						  : Alignment::BEGIN;

	alignments [1] = justify .size () > 1
	                 ? getAlignment (1)
						  : Alignment::FIRST;
}

X3DFontStyleNode::Alignment
X3DFontStyleNode::getAlignment (const size_t index) const
{
	if (justify [index] == "BEGIN")
		return Alignment::BEGIN;

	else if (justify [index] == "FIRST")
		return Alignment::FIRST;

	else if (justify [index] == "MIDDLE")
		return Alignment::MIDDLE;

	else if (justify [index] == "END")
		return Alignment::END;

	return index ? Alignment::FIRST : Alignment::BEGIN;
}

X3DFontStyleNode::Alignment
X3DFontStyleNode::getMajorAlignment () const
{
	return alignments [0];
}

X3DFontStyleNode::Alignment
X3DFontStyleNode::getMinorAlignment () const
{
	return alignments [1];
}

std::string
X3DFontStyleNode::getFilename () const
{
	FcPattern* pattern = family .size ()
	                     ? FcNameParse ((FcChar8*) (family [0] .getValue () .c_str ()))
								: FcPatternCreate ();

	FcPatternAddString (pattern, "style", (FcChar8*) (style == "BOLDITALIC" ? "bold italic" : style .getValue () .c_str ()));

	FcConfigSubstitute (NULL, pattern, FcMatchPattern);
	FcDefaultSubstitute (pattern);

	FcResult   result;
	FcPattern* match = FcFontMatch (NULL, pattern, &result);

	FcChar8* file;

	FcPatternGetString (match, FC_FILE, 0, &file);

	FcPatternDestroy (pattern);

	__LOG__ << file << std::endl;
	return std::string ((char*) file);
}

} // X3D
} // titania
