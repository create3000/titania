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

#ifndef __TITANIA_X3D_PARSER_SVG_COLORS_H__
#define __TITANIA_X3D_PARSER_SVG_COLORS_H__

#include "../../Types/Numbers.h"

#include <map>

namespace titania {
namespace X3D {
namespace SVG {

class Colors
{
public:

	Colors ();

	const Color3f &
	at (const std::string & name) const
	{ return namedColors .at (name); }


private:

	std::map <std::string, Color3f> namedColors;

};

inline
Colors::Colors () :
	namedColors ()
{
	// JavaScript CSS3 Colors
	// http://www.w3.org/TR/2010/PR-css3-color-20101028/#svg-color

	namedColors .emplace ("aliceblue",            make_rgb <float> (240, 248, 255));
	namedColors .emplace ("antiquewhite",         make_rgb <float> (250, 235, 215));
	namedColors .emplace ("aqua",                 make_rgb <float> (0, 255, 255));
	namedColors .emplace ("aquamarine",           make_rgb <float> (127, 255, 212));
	namedColors .emplace ("azure",                make_rgb <float> (240, 255, 255));
	namedColors .emplace ("beige",                make_rgb <float> (245, 245, 220));
	namedColors .emplace ("bisque",               make_rgb <float> (255, 228, 196));
	namedColors .emplace ("black",                make_rgb <float> (0, 0, 0));
	namedColors .emplace ("blanchedalmond",       make_rgb <float> (255, 235, 205));
	namedColors .emplace ("blue",                 make_rgb <float> (0, 0, 255));
	namedColors .emplace ("blueviolet",           make_rgb <float> (138, 43, 226));
	namedColors .emplace ("brown",                make_rgb <float> (165, 42, 42));
	namedColors .emplace ("burlywood",            make_rgb <float> (222, 184, 135));
	namedColors .emplace ("cadetblue",            make_rgb <float> (95, 158, 160));
	namedColors .emplace ("chartreuse",           make_rgb <float> (127, 255, 0));
	namedColors .emplace ("chocolate",            make_rgb <float> (210, 105, 30));
	namedColors .emplace ("coral",                make_rgb <float> (255, 127, 80));
	namedColors .emplace ("cornflowerblue",       make_rgb <float> (100, 149, 237));
	namedColors .emplace ("cornsilk",             make_rgb <float> (255, 248, 220));
	namedColors .emplace ("crimson",              make_rgb <float> (220, 20, 60));
	namedColors .emplace ("cyan",                 make_rgb <float> (0, 255, 255));
	namedColors .emplace ("darkblue",             make_rgb <float> (0, 0, 139));
	namedColors .emplace ("darkcyan",             make_rgb <float> (0, 139, 139));
	namedColors .emplace ("darkgoldenrod",        make_rgb <float> (184, 134, 11));
	namedColors .emplace ("darkgray",             make_rgb <float> (169, 169, 169));
	namedColors .emplace ("darkgrey",             make_rgb <float> (169, 169, 169));
	namedColors .emplace ("darkgreen",            make_rgb <float> (0, 100, 0));
	namedColors .emplace ("darkkhaki",            make_rgb <float> (189, 183, 107));
	namedColors .emplace ("darkmagenta",          make_rgb <float> (139, 0, 139));
	namedColors .emplace ("darkolivegreen",       make_rgb <float> (85, 107, 47));
	namedColors .emplace ("darkorange",           make_rgb <float> (255, 140, 0));
	namedColors .emplace ("darkorchid",           make_rgb <float> (153, 50, 204));
	namedColors .emplace ("darkred",              make_rgb <float> (139, 0, 0));
	namedColors .emplace ("darksalmon",           make_rgb <float> (233, 150, 122));
	namedColors .emplace ("darkseagreen",         make_rgb <float> (143, 188, 143));
	namedColors .emplace ("darkslateblue",        make_rgb <float> (72, 61, 139));
	namedColors .emplace ("darkslategray",        make_rgb <float> (47, 79, 79));
	namedColors .emplace ("darkslategrey",        make_rgb <float> (47, 79, 79));
	namedColors .emplace ("darkturquoise",        make_rgb <float> (0, 206, 209));
	namedColors .emplace ("darkviolet",           make_rgb <float> (148, 0, 211));
	namedColors .emplace ("deeppink",             make_rgb <float> (255, 20, 147));
	namedColors .emplace ("deepskyblue",          make_rgb <float> (0, 191, 255));
	namedColors .emplace ("dimgray",              make_rgb <float> (105, 105, 105));
	namedColors .emplace ("dimgrey",              make_rgb <float> (105, 105, 105));
	namedColors .emplace ("dodgerblue",           make_rgb <float> (30, 144, 255));
	namedColors .emplace ("firebrick",            make_rgb <float> (178, 34, 34));
	namedColors .emplace ("floralwhite",          make_rgb <float> (255, 250, 240));
	namedColors .emplace ("forestgreen",          make_rgb <float> (34, 139, 34));
	namedColors .emplace ("fuchsia",              make_rgb <float> (255, 0, 255));
	namedColors .emplace ("gainsboro",            make_rgb <float> (220, 220, 220));
	namedColors .emplace ("ghostwhite",           make_rgb <float> (248, 248, 255));
	namedColors .emplace ("gold",                 make_rgb <float> (255, 215, 0));
	namedColors .emplace ("goldenrod",            make_rgb <float> (218, 165, 32));
	namedColors .emplace ("gray",                 make_rgb <float> (128, 128, 128));
	namedColors .emplace ("grey",                 make_rgb <float> (128, 128, 128));
	namedColors .emplace ("green",                make_rgb <float> (0, 128, 0));
	namedColors .emplace ("greenyellow",          make_rgb <float> (173, 255, 47));
	namedColors .emplace ("honeydew",             make_rgb <float> (240, 255, 240));
	namedColors .emplace ("hotpink",              make_rgb <float> (255, 105, 180));
	namedColors .emplace ("indianred",            make_rgb <float> (205, 92, 92));
	namedColors .emplace ("indigo",               make_rgb <float> (75, 0, 130));
	namedColors .emplace ("ivory",                make_rgb <float> (255, 255, 240));
	namedColors .emplace ("khaki",                make_rgb <float> (240, 230, 140));
	namedColors .emplace ("lavender",             make_rgb <float> (230, 230, 250));
	namedColors .emplace ("lavenderblush",        make_rgb <float> (255, 240, 245));
	namedColors .emplace ("lawngreen",            make_rgb <float> (124, 252, 0));
	namedColors .emplace ("lemonchiffon",         make_rgb <float> (255, 250, 205));
	namedColors .emplace ("lightblue",            make_rgb <float> (173, 216, 230));
	namedColors .emplace ("lightcoral",           make_rgb <float> (240, 128, 128));
	namedColors .emplace ("lightcyan",            make_rgb <float> (224, 255, 255));
	namedColors .emplace ("lightgoldenrodyellow", make_rgb <float> (250, 250, 210));
	namedColors .emplace ("lightgray",            make_rgb <float> (211, 211, 211));
	namedColors .emplace ("lightgrey",            make_rgb <float> (211, 211, 211));
	namedColors .emplace ("lightgreen",           make_rgb <float> (144, 238, 144));
	namedColors .emplace ("lightpink",            make_rgb <float> (255, 182, 193));
	namedColors .emplace ("lightsalmon",          make_rgb <float> (255, 160, 122));
	namedColors .emplace ("lightseagreen",        make_rgb <float> (32, 178, 170));
	namedColors .emplace ("lightskyblue",         make_rgb <float> (135, 206, 250));
	namedColors .emplace ("lightslategray",       make_rgb <float> (119, 136, 153));
	namedColors .emplace ("lightslategrey",       make_rgb <float> (119, 136, 153));
	namedColors .emplace ("lightsteelblue",       make_rgb <float> (176, 196, 222));
	namedColors .emplace ("lightyellow",          make_rgb <float> (255, 255, 224));
	namedColors .emplace ("lime",                 make_rgb <float> (0, 255, 0));
	namedColors .emplace ("limegreen",            make_rgb <float> (50, 205, 50));
	namedColors .emplace ("linen",                make_rgb <float> (250, 240, 230));
	namedColors .emplace ("magenta",              make_rgb <float> (255, 0, 255));
	namedColors .emplace ("maroon",               make_rgb <float> (128, 0, 0));
	namedColors .emplace ("mediumaquamarine",     make_rgb <float> (102, 205, 170));
	namedColors .emplace ("mediumblue",           make_rgb <float> (0, 0, 205));
	namedColors .emplace ("mediumorchid",         make_rgb <float> (186, 85, 211));
	namedColors .emplace ("mediumpurple",         make_rgb <float> (147, 112, 219));
	namedColors .emplace ("mediumseagreen",       make_rgb <float> (60, 179, 113));
	namedColors .emplace ("mediumslateblue",      make_rgb <float> (123, 104, 238));
	namedColors .emplace ("mediumspringgreen",    make_rgb <float> (0, 250, 154));
	namedColors .emplace ("mediumturquoise",      make_rgb <float> (72, 209, 204));
	namedColors .emplace ("mediumvioletred",      make_rgb <float> (199, 21, 133));
	namedColors .emplace ("midnightblue",         make_rgb <float> (25, 25, 112));
	namedColors .emplace ("mintcream",            make_rgb <float> (245, 255, 250));
	namedColors .emplace ("mistyrose",            make_rgb <float> (255, 228, 225));
	namedColors .emplace ("moccasin",             make_rgb <float> (255, 228, 181));
	namedColors .emplace ("navajowhite",          make_rgb <float> (255, 222, 173));
	namedColors .emplace ("navy",                 make_rgb <float> (0, 0, 128));
	namedColors .emplace ("oldlace",              make_rgb <float> (253, 245, 230));
	namedColors .emplace ("olive",                make_rgb <float> (128, 128, 0));
	namedColors .emplace ("olivedrab",            make_rgb <float> (107, 142, 35));
	namedColors .emplace ("orange",               make_rgb <float> (255, 165, 0));
	namedColors .emplace ("orangered",            make_rgb <float> (255, 69, 0));
	namedColors .emplace ("orchid",               make_rgb <float> (218, 112, 214));
	namedColors .emplace ("palegoldenrod",        make_rgb <float> (238, 232, 170));
	namedColors .emplace ("palegreen",            make_rgb <float> (152, 251, 152));
	namedColors .emplace ("paleturquoise",        make_rgb <float> (175, 238, 238));
	namedColors .emplace ("palevioletred",        make_rgb <float> (219, 112, 147));
	namedColors .emplace ("papayawhip",           make_rgb <float> (255, 239, 213));
	namedColors .emplace ("peachpuff",            make_rgb <float> (255, 218, 185));
	namedColors .emplace ("peru",                 make_rgb <float> (205, 133, 63));
	namedColors .emplace ("pink",                 make_rgb <float> (255, 192, 203));
	namedColors .emplace ("plum",                 make_rgb <float> (221, 160, 221));
	namedColors .emplace ("powderblue",           make_rgb <float> (176, 224, 230));
	namedColors .emplace ("purple",               make_rgb <float> (128, 0, 128));
	namedColors .emplace ("red",                  make_rgb <float> (255, 0, 0));
	namedColors .emplace ("rosybrown",            make_rgb <float> (188, 143, 143));
	namedColors .emplace ("royalblue",            make_rgb <float> (65, 105, 225));
	namedColors .emplace ("saddlebrown",          make_rgb <float> (139, 69, 19));
	namedColors .emplace ("salmon",               make_rgb <float> (250, 128, 114));
	namedColors .emplace ("sandybrown",           make_rgb <float> (244, 164, 96));
	namedColors .emplace ("seagreen",             make_rgb <float> (46, 139, 87));
	namedColors .emplace ("seashell",             make_rgb <float> (255, 245, 238));
	namedColors .emplace ("sienna",               make_rgb <float> (160, 82, 45));
	namedColors .emplace ("silver",               make_rgb <float> (192, 192, 192));
	namedColors .emplace ("skyblue",              make_rgb <float> (135, 206, 235));
	namedColors .emplace ("slateblue",            make_rgb <float> (106, 90, 205));
	namedColors .emplace ("slategray",            make_rgb <float> (112, 128, 144));
	namedColors .emplace ("slategrey",            make_rgb <float> (112, 128, 144));
	namedColors .emplace ("snow",                 make_rgb <float> (255, 250, 250));
	namedColors .emplace ("springgreen",          make_rgb <float> (0, 255, 127));
	namedColors .emplace ("steelblue",            make_rgb <float> (70, 130, 180));
	namedColors .emplace ("tan",                  make_rgb <float> (210, 180, 140));
	namedColors .emplace ("teal",                 make_rgb <float> (0, 128, 128));
	namedColors .emplace ("thistle",              make_rgb <float> (216, 191, 216));
	namedColors .emplace ("tomato",               make_rgb <float> (255, 99, 71));
	namedColors .emplace ("turquoise",            make_rgb <float> (64, 224, 208));
	namedColors .emplace ("violet",               make_rgb <float> (238, 130, 238));
	namedColors .emplace ("wheat",                make_rgb <float> (245, 222, 179));
	namedColors .emplace ("white",                make_rgb <float> (255, 255, 255));
	namedColors .emplace ("whitesmoke",           make_rgb <float> (245, 245, 245));
	namedColors .emplace ("yellow",               make_rgb <float> (255, 255, 0));
	namedColors .emplace ("yellowgreen",          make_rgb <float> (154, 205, 50));
}

} // SVG
} // X3D
} // titania

#endif
