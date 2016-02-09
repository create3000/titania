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

#ifndef __TITANIA_X3D_TOOLS_TOOL_COLORS_H__
#define __TITANIA_X3D_TOOLS_TOOL_COLORS_H__

#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

namespace ToolColors {

static constexpr Color3f GREEN  (0.35, 1, 0.7);   // Group
static constexpr Color3f YELLOW (1, 1, 0.35);     // Switch
static constexpr Color3f LILA   (0.7, 0.35, 1);   // Anchor
static constexpr Color3f PINK   (1, 0.35, 0.7);   // Billboard
static constexpr Color3f RED    (1, 0.35, 0.35);  // Collision
static constexpr Color3f CYAN   (0.35, 1, 1);     // LOD

static constexpr Color3f WHITE  (1, 1, 1);        // Inline
static constexpr Color3f ORANGE (1, 0.7, 0.35);   // Shape
static constexpr Color3f BLUE   (0.7, 0.85, 1);   // Normals, Edges
static constexpr Color3f LIME   (0.35, 1, 0.35);  // ScreenGroup

static constexpr Color3f DARK_GREEN  (0.175, 0.5, 0.35);    // CADAssembly
static constexpr Color3f DARK_YELLOW (0.7, 0.7, 0.35);      // CADLayer
static constexpr Color3f BROWN       (0.75, 0.525, 0.2625); // CADFace
static constexpr Color3f DARK_CYAN   (0.35, 0.75, 0.75);    // GeoLOD
static constexpr Color3f DARK_BLUE   (0.35, 0.35, 1);       // 

static constexpr Color4f GREEN_RGBA  (0.35, 1, 0.7, 1);   // Group
static constexpr Color4f YELLOW_RGBA (1, 1, 0.35, 1);     // Switch
static constexpr Color4f LILA_RGBA   (0.7, 0.35, 1, 1);   // Anchor
static constexpr Color4f PINK_RGBA   (1, 0.35, 0.7, 1);   // Billboard
static constexpr Color4f RED_RGBA    (1, 0.35, 0.35, 1);  // Collision
static constexpr Color4f CYAN_RGBA   (0.35, 1, 1, 1);     // LOD

static constexpr Color4f WHITE_RGBA  (1, 1, 1, 1);        // Inline
static constexpr Color4f ORANGE_RGBA (1, 0.7, 0.35, 1);   // Shape
static constexpr Color4f BLUE_RGBA   (0.7, 0.85, 1, 1);   // Normals, Edges
static constexpr Color4f LIME_RGBA   (0.35, 1, 0.35, 1);  // ScreenGroup

static constexpr Color4f DARK_GREEN_RGBA  (0.175, 0.5, 0.35, 1);    // CADAssembly
static constexpr Color4f DARK_YELLOW_RGBA (0.7, 0.7, 0.35, 1);      // CADLayer
static constexpr Color4f BROWN_RGBA       (0.75, 0.525, 0.2625, 1); // CADFace
static constexpr Color4f DARK_CYAN_RGBA   (0.35, 0.75, 0.75, 1);    // GeoLOD
static constexpr Color4f DARK_BLUE_RGBA   (0.35, 0.35, 1, 1);       // 

}

} // X3D
} // titania

#endif
