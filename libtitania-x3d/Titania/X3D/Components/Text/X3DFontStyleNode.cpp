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

#include "X3DFontStyleNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Browser/Networking/config.h"

namespace titania {
namespace X3D {

X3DFontStyleNode::Fields::Fields () :
	   language (new SFString ()),
	     family (new MFString ({ "SERIF" })),
	      style (new SFString ("PLAIN")),
	    spacing (new SFFloat (1)),
	 horizontal (new SFBool (true)),
	leftToRight (new SFBool (true)),
	topToBottom (new SFBool (true)),
	    justify (new MFString ({ "BEGIN" }))
{ }

X3DFontStyleNode::X3DFontStyleNode () :
	   X3DNode (),
	    fields (),
	    italic (false),
	      bold (false),
	alignments ({ Alignment::BEGIN, Alignment::FIRST }),
	  tempfile ()
{
	addType (X3DConstants::X3DFontStyleNode);
}

void
X3DFontStyleNode::initialize ()
{
	X3DNode::initialize ();

	style ()       .addInterest (&X3DFontStyleNode::set_style,   this);
	horizontal ()  .addInterest (&X3DFontStyleNode::set_justify, this);
	leftToRight () .addInterest (&X3DFontStyleNode::set_justify, this);
	topToBottom () .addInterest (&X3DFontStyleNode::set_justify, this);
	justify ()     .addInterest (&X3DFontStyleNode::set_justify, this);

	set_style ();
	set_justify ();
}

X3DFontStyleNode::Alignment
X3DFontStyleNode::getAlignment (const size_t index, const bool normal) const
{
	if (normal)
	{
		// Return for west-european normal alignment.

		static const std::map <std::string, Alignment> alignments = {
			std::pair ("FIRST",  Alignment::FIRST),
			std::pair ("BEGIN",  Alignment::BEGIN),
			std::pair ("MIDDLE", Alignment::MIDDLE),
			std::pair ("END",    Alignment::END)
		};

		try
		{
			return alignments .at (justify () [index]);
		}
		catch (const std::out_of_range &)
		{ }
	}
	else
	{
		// Return appropriate alignment if topToBottom or leftToRight are FALSE.

		static const std::map <std::string, Alignment> ralignments = {
			std::pair ("FIRST",  Alignment::END),
			std::pair ("BEGIN",  Alignment::END),
			std::pair ("MIDDLE", Alignment::MIDDLE),
			std::pair ("END",    Alignment::BEGIN)
		};

		try
		{
			return ralignments .at (justify () [index]);
		}
		catch (const std::out_of_range &)
		{ }
	}

	return index ? Alignment::FIRST : Alignment::BEGIN;
}

Font
X3DFontStyleNode::createFont () const
{
	bool isExactMatch = false;

	if (getBrowser () -> getLoadUrlObjects ())
	{
		for (const auto & familyName : family ())
		{
			const auto font = createFont (familyName .empty () ? "SERIF" : familyName, isExactMatch);

			if (isExactMatch)
				return font;
		}
	}

	return createFont ("SERIF", isExactMatch);
}

Font
X3DFontStyleNode::createFont (const String & rawFamilyName, bool & isExactMatch) const
{
	// Test if familyName is a valid path local path.
	// TODO: add support for network paths.

	static const std::map <std::tuple <std::string, bool, bool>, std::string> defaultFonts = {
		std::pair (std::make_tuple ("SERIF",      false, false), get_font ("DroidSerif-Regular.ttf")    .str ()),
		std::pair (std::make_tuple ("SERIF",      false, true),  get_font ("DroidSerif-Italic.ttf")     .str ()),
		std::pair (std::make_tuple ("SERIF",      true,  false), get_font ("DroidSerif-Bold.ttf")       .str ()),
		std::pair (std::make_tuple ("SERIF",      true,  true),  get_font ("DroidSerif-BoldItalic.ttf") .str ()),

		std::pair (std::make_tuple ("SANS",       false, false), get_font ("Ubuntu-R.ttf")              .str ()),
		std::pair (std::make_tuple ("SANS",       false, true),  get_font ("Ubuntu-RI.ttf")             .str ()),
		std::pair (std::make_tuple ("SANS",       true,  false), get_font ("Ubuntu-B.ttf")              .str ()),
		std::pair (std::make_tuple ("SANS",       true,  true),  get_font ("Ubuntu-BI.ttf")             .str ()),

		std::pair (std::make_tuple ("TYPEWRITER", false, false), get_font ("UbuntuMono-R.ttf")          .str ()),
		std::pair (std::make_tuple ("TYPEWRITER", false, true),  get_font ("UbuntuMono-RI.ttf")         .str ()),
		std::pair (std::make_tuple ("TYPEWRITER", true,  false), get_font ("UbuntuMono-B.ttf")          .str ()),
		std::pair (std::make_tuple ("TYPEWRITER", true,  true),  get_font ("UbuntuMono-BI.ttf")         .str ()),
	};

	const auto iter       = defaultFonts .find (std::make_tuple (rawFamilyName, bold, italic));
	const auto familyName = iter == defaultFonts .end () ? rawFamilyName .raw () : iter -> second;

	try
	{
		if (tempfile and tempfile -> query_exists ())
			tempfile -> remove ();

		const auto  fontData     = FileLoader (getExecutionContext ()) .loadDocument (familyName);
		const auto  uri          = basic::uri (familyName);
		const auto  extension    = uri .extension ();
		std::string tempFilename = "/tmp/titania-XXXXXX" + extension;

		::close (Glib::mkstemp (tempFilename));

		const_cast <X3DFontStyleNode*> (this) -> tempfile = Gio::File::create_for_path (tempFilename);

		std::ofstream ostream (tempFilename);

		ostream << fontData;

		isExactMatch = true;

		Font font;
		font .setFilename (tempFilename);
		font .substitute ();
		return font;
	}
	catch (const Glib::Exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	Font font;
	font .setFamilyName (familyName == "TYPEWRITER" ? "monospace" : familyName);
	font .setWeight (isBold ()   ? Font::Weight::BOLD  : Font::Weight::NORMAL);
	font .setSlant  (isItalic () ? Font::Slant::ITALIC : Font::Slant::ROMAN);
	font .setScalable (true);
	font .substitute ();

	const Font match = font .getMatch ();

	isExactMatch = (font == match);

	return match;
}

void
X3DFontStyleNode::set_style ()
{
	italic = (style () == "ITALIC" or style () == "BOLDITALIC");
	bold   = (style () == "BOLD"   or style () == "BOLDITALIC");
}

void
X3DFontStyleNode::set_justify ()
{
	const bool majorNormal = horizontal () ? leftToRight () : topToBottom ();

	alignments [0] = justify () .size () > 0
	                 ? getAlignment (0, majorNormal)
						  : majorNormal ? Alignment::BEGIN : Alignment::END;

	const bool minorNormal = horizontal () ? topToBottom () : leftToRight ();

	alignments [1] = justify () .size () > 1
	                 ? getAlignment (1, minorNormal)
						  : minorNormal ? Alignment::FIRST : Alignment::END;
}

void
X3DFontStyleNode::transform (MFString & url, const basic::uri & oldWorldURL, const basic::uri & newWorldURL)
{
	MFString transformed = url;

	for (MFString::reference value : transformed)
	{
		const basic::uri URL (value .get ());

		if (URL .stem () == URL)
			continue;

		if (URL .is_relative () and not URL .filename (true) .empty ())
		{
			const auto transformed = oldWorldURL .transform (URL);

			value .set (newWorldURL .relative_path (transformed) .str ());
		}
	}

	transformed .erase (std::unique (url .begin (), url .end ()), url .end ());

	url .set (transformed);
}

void
X3DFontStyleNode::dispose ()
{
	try
	{
		if (tempfile and tempfile -> query_exists ())
			tempfile -> remove ();
	}
	catch (const Glib::Exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	X3DNode::dispose ();
}

X3DFontStyleNode::~X3DFontStyleNode ()
{ }

} // X3D
} // titania
