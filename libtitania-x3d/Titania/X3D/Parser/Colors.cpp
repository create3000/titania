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

#include "Colors.h"

namespace titania {
namespace X3D {

const std::map <std::string, Color3f> Colors::colors = Colors::create ();

std::map <std::string, Color3f>
Colors::create ()
{
	// JavaScript CSS3 Colors
	// http://www.w3.org/TR/2010/PR-css3-color-20101028/#svg-color

	std::map <std::string, Color3f> colors;

	colors .emplace ("aliceblue",            make_rgb <float> (240, 248, 255));
	colors .emplace ("antiquewhite",         make_rgb <float> (250, 235, 215));
	colors .emplace ("aqua",                 make_rgb <float> (0, 255, 255));
	colors .emplace ("aquamarine",           make_rgb <float> (127, 255, 212));
	colors .emplace ("azure",                make_rgb <float> (240, 255, 255));
	colors .emplace ("beige",                make_rgb <float> (245, 245, 220));
	colors .emplace ("bisque",               make_rgb <float> (255, 228, 196));
	colors .emplace ("black",                make_rgb <float> (0, 0, 0));
	colors .emplace ("blanchedalmond",       make_rgb <float> (255, 235, 205));
	colors .emplace ("blue",                 make_rgb <float> (0, 0, 255));
	colors .emplace ("blueviolet",           make_rgb <float> (138, 43, 226));
	colors .emplace ("brown",                make_rgb <float> (165, 42, 42));
	colors .emplace ("burlywood",            make_rgb <float> (222, 184, 135));
	colors .emplace ("cadetblue",            make_rgb <float> (95, 158, 160));
	colors .emplace ("chartreuse",           make_rgb <float> (127, 255, 0));
	colors .emplace ("chocolate",            make_rgb <float> (210, 105, 30));
	colors .emplace ("coral",                make_rgb <float> (255, 127, 80));
	colors .emplace ("cornflowerblue",       make_rgb <float> (100, 149, 237));
	colors .emplace ("cornsilk",             make_rgb <float> (255, 248, 220));
	colors .emplace ("crimson",              make_rgb <float> (220, 20, 60));
	colors .emplace ("cyan",                 make_rgb <float> (0, 255, 255));
	colors .emplace ("darkblue",             make_rgb <float> (0, 0, 139));
	colors .emplace ("darkcyan",             make_rgb <float> (0, 139, 139));
	colors .emplace ("darkgoldenrod",        make_rgb <float> (184, 134, 11));
	colors .emplace ("darkgray",             make_rgb <float> (169, 169, 169));
	colors .emplace ("darkgrey",             make_rgb <float> (169, 169, 169));
	colors .emplace ("darkgreen",            make_rgb <float> (0, 100, 0));
	colors .emplace ("darkkhaki",            make_rgb <float> (189, 183, 107));
	colors .emplace ("darkmagenta",          make_rgb <float> (139, 0, 139));
	colors .emplace ("darkolivegreen",       make_rgb <float> (85, 107, 47));
	colors .emplace ("darkorange",           make_rgb <float> (255, 140, 0));
	colors .emplace ("darkorchid",           make_rgb <float> (153, 50, 204));
	colors .emplace ("darkred",              make_rgb <float> (139, 0, 0));
	colors .emplace ("darksalmon",           make_rgb <float> (233, 150, 122));
	colors .emplace ("darkseagreen",         make_rgb <float> (143, 188, 143));
	colors .emplace ("darkslateblue",        make_rgb <float> (72, 61, 139));
	colors .emplace ("darkslategray",        make_rgb <float> (47, 79, 79));
	colors .emplace ("darkslategrey",        make_rgb <float> (47, 79, 79));
	colors .emplace ("darkturquoise",        make_rgb <float> (0, 206, 209));
	colors .emplace ("darkviolet",           make_rgb <float> (148, 0, 211));
	colors .emplace ("deeppink",             make_rgb <float> (255, 20, 147));
	colors .emplace ("deepskyblue",          make_rgb <float> (0, 191, 255));
	colors .emplace ("dimgray",              make_rgb <float> (105, 105, 105));
	colors .emplace ("dimgrey",              make_rgb <float> (105, 105, 105));
	colors .emplace ("dodgerblue",           make_rgb <float> (30, 144, 255));
	colors .emplace ("firebrick",            make_rgb <float> (178, 34, 34));
	colors .emplace ("floralwhite",          make_rgb <float> (255, 250, 240));
	colors .emplace ("forestgreen",          make_rgb <float> (34, 139, 34));
	colors .emplace ("fuchsia",              make_rgb <float> (255, 0, 255));
	colors .emplace ("gainsboro",            make_rgb <float> (220, 220, 220));
	colors .emplace ("ghostwhite",           make_rgb <float> (248, 248, 255));
	colors .emplace ("gold",                 make_rgb <float> (255, 215, 0));
	colors .emplace ("goldenrod",            make_rgb <float> (218, 165, 32));
	colors .emplace ("gray",                 make_rgb <float> (128, 128, 128));
	colors .emplace ("grey",                 make_rgb <float> (128, 128, 128));
	colors .emplace ("green",                make_rgb <float> (0, 128, 0));
	colors .emplace ("greenyellow",          make_rgb <float> (173, 255, 47));
	colors .emplace ("honeydew",             make_rgb <float> (240, 255, 240));
	colors .emplace ("hotpink",              make_rgb <float> (255, 105, 180));
	colors .emplace ("indianred",            make_rgb <float> (205, 92, 92));
	colors .emplace ("indigo",               make_rgb <float> (75, 0, 130));
	colors .emplace ("ivory",                make_rgb <float> (255, 255, 240));
	colors .emplace ("khaki",                make_rgb <float> (240, 230, 140));
	colors .emplace ("lavender",             make_rgb <float> (230, 230, 250));
	colors .emplace ("lavenderblush",        make_rgb <float> (255, 240, 245));
	colors .emplace ("lawngreen",            make_rgb <float> (124, 252, 0));
	colors .emplace ("lemonchiffon",         make_rgb <float> (255, 250, 205));
	colors .emplace ("lightblue",            make_rgb <float> (173, 216, 230));
	colors .emplace ("lightcoral",           make_rgb <float> (240, 128, 128));
	colors .emplace ("lightcyan",            make_rgb <float> (224, 255, 255));
	colors .emplace ("lightgoldenrodyellow", make_rgb <float> (250, 250, 210));
	colors .emplace ("lightgray",            make_rgb <float> (211, 211, 211));
	colors .emplace ("lightgrey",            make_rgb <float> (211, 211, 211));
	colors .emplace ("lightgreen",           make_rgb <float> (144, 238, 144));
	colors .emplace ("lightpink",            make_rgb <float> (255, 182, 193));
	colors .emplace ("lightsalmon",          make_rgb <float> (255, 160, 122));
	colors .emplace ("lightseagreen",        make_rgb <float> (32, 178, 170));
	colors .emplace ("lightskyblue",         make_rgb <float> (135, 206, 250));
	colors .emplace ("lightslategray",       make_rgb <float> (119, 136, 153));
	colors .emplace ("lightslategrey",       make_rgb <float> (119, 136, 153));
	colors .emplace ("lightsteelblue",       make_rgb <float> (176, 196, 222));
	colors .emplace ("lightyellow",          make_rgb <float> (255, 255, 224));
	colors .emplace ("lime",                 make_rgb <float> (0, 255, 0));
	colors .emplace ("limegreen",            make_rgb <float> (50, 205, 50));
	colors .emplace ("linen",                make_rgb <float> (250, 240, 230));
	colors .emplace ("magenta",              make_rgb <float> (255, 0, 255));
	colors .emplace ("maroon",               make_rgb <float> (128, 0, 0));
	colors .emplace ("mediumaquamarine",     make_rgb <float> (102, 205, 170));
	colors .emplace ("mediumblue",           make_rgb <float> (0, 0, 205));
	colors .emplace ("mediumorchid",         make_rgb <float> (186, 85, 211));
	colors .emplace ("mediumpurple",         make_rgb <float> (147, 112, 219));
	colors .emplace ("mediumseagreen",       make_rgb <float> (60, 179, 113));
	colors .emplace ("mediumslateblue",      make_rgb <float> (123, 104, 238));
	colors .emplace ("mediumspringgreen",    make_rgb <float> (0, 250, 154));
	colors .emplace ("mediumturquoise",      make_rgb <float> (72, 209, 204));
	colors .emplace ("mediumvioletred",      make_rgb <float> (199, 21, 133));
	colors .emplace ("midnightblue",         make_rgb <float> (25, 25, 112));
	colors .emplace ("mintcream",            make_rgb <float> (245, 255, 250));
	colors .emplace ("mistyrose",            make_rgb <float> (255, 228, 225));
	colors .emplace ("moccasin",             make_rgb <float> (255, 228, 181));
	colors .emplace ("navajowhite",          make_rgb <float> (255, 222, 173));
	colors .emplace ("navy",                 make_rgb <float> (0, 0, 128));
	colors .emplace ("oldlace",              make_rgb <float> (253, 245, 230));
	colors .emplace ("olive",                make_rgb <float> (128, 128, 0));
	colors .emplace ("olivedrab",            make_rgb <float> (107, 142, 35));
	colors .emplace ("orange",               make_rgb <float> (255, 165, 0));
	colors .emplace ("orangered",            make_rgb <float> (255, 69, 0));
	colors .emplace ("orchid",               make_rgb <float> (218, 112, 214));
	colors .emplace ("palegoldenrod",        make_rgb <float> (238, 232, 170));
	colors .emplace ("palegreen",            make_rgb <float> (152, 251, 152));
	colors .emplace ("paleturquoise",        make_rgb <float> (175, 238, 238));
	colors .emplace ("palevioletred",        make_rgb <float> (219, 112, 147));
	colors .emplace ("papayawhip",           make_rgb <float> (255, 239, 213));
	colors .emplace ("peachpuff",            make_rgb <float> (255, 218, 185));
	colors .emplace ("peru",                 make_rgb <float> (205, 133, 63));
	colors .emplace ("pink",                 make_rgb <float> (255, 192, 203));
	colors .emplace ("plum",                 make_rgb <float> (221, 160, 221));
	colors .emplace ("powderblue",           make_rgb <float> (176, 224, 230));
	colors .emplace ("purple",               make_rgb <float> (128, 0, 128));
	colors .emplace ("red",                  make_rgb <float> (255, 0, 0));
	colors .emplace ("rosybrown",            make_rgb <float> (188, 143, 143));
	colors .emplace ("royalblue",            make_rgb <float> (65, 105, 225));
	colors .emplace ("saddlebrown",          make_rgb <float> (139, 69, 19));
	colors .emplace ("salmon",               make_rgb <float> (250, 128, 114));
	colors .emplace ("sandybrown",           make_rgb <float> (244, 164, 96));
	colors .emplace ("seagreen",             make_rgb <float> (46, 139, 87));
	colors .emplace ("seashell",             make_rgb <float> (255, 245, 238));
	colors .emplace ("sienna",               make_rgb <float> (160, 82, 45));
	colors .emplace ("silver",               make_rgb <float> (192, 192, 192));
	colors .emplace ("skyblue",              make_rgb <float> (135, 206, 235));
	colors .emplace ("slateblue",            make_rgb <float> (106, 90, 205));
	colors .emplace ("slategray",            make_rgb <float> (112, 128, 144));
	colors .emplace ("slategrey",            make_rgb <float> (112, 128, 144));
	colors .emplace ("snow",                 make_rgb <float> (255, 250, 250));
	colors .emplace ("springgreen",          make_rgb <float> (0, 255, 127));
	colors .emplace ("steelblue",            make_rgb <float> (70, 130, 180));
	colors .emplace ("tan",                  make_rgb <float> (210, 180, 140));
	colors .emplace ("teal",                 make_rgb <float> (0, 128, 128));
	colors .emplace ("thistle",              make_rgb <float> (216, 191, 216));
	colors .emplace ("tomato",               make_rgb <float> (255, 99, 71));
	colors .emplace ("turquoise",            make_rgb <float> (64, 224, 208));
	colors .emplace ("violet",               make_rgb <float> (238, 130, 238));
	colors .emplace ("wheat",                make_rgb <float> (245, 222, 179));
	colors .emplace ("white",                make_rgb <float> (255, 255, 255));
	colors .emplace ("whitesmoke",           make_rgb <float> (245, 245, 245));
	colors .emplace ("yellow",               make_rgb <float> (255, 255, 0));
	colors .emplace ("yellowgreen",          make_rgb <float> (154, 205, 50));

	return colors;
}


} // X3D
} // titania
