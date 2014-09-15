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

#include "ViewportContainer.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Layering/Viewport.h"

namespace titania {
namespace X3D {

ViewportContainer::ViewportContainer (Viewport* const node) :
	X3DCollectableObject (),
	                node (node),
	            viewport (Viewport4i ())
{ }

void
ViewportContainer::apply ()
{
	const auto viewport = node -> getRectangle ();

	glViewport (viewport [0],
	            viewport [1],
	            viewport [2],
	            viewport [3]);

	glScissor (viewport [0],
	           viewport [1],
	           viewport [2],
	           viewport [3]);
}

void
ViewportContainer::restore ()
{
	glViewport (viewport [0],
	            viewport [1],
	            viewport [2],
	            viewport [3]);

	glScissor (viewport [0],
	           viewport [1],
	           viewport [2],
	           viewport [3]);
}

void
ViewportContainer::enable ()
{
	const auto scissor   = Scissor4i ();
	const auto rectangle = node -> getRectangle ();

	const auto left   = std::max (rectangle [0], scissor [0]);
	const auto bottom = std::max (rectangle [1], scissor [1]);
	const auto right  = std::min (rectangle [0] + rectangle [2], scissor [0] + scissor [2]);
	const auto top    = std::min (rectangle [1] + rectangle [3], scissor [1] + scissor [3]);

	glScissor (left,
	           bottom,
	           std::max (0, right - left),
	           std::max (0, top - bottom));

	glEnable (GL_SCISSOR_TEST);
}

void
ViewportContainer::disable ()
{
	glDisable (GL_SCISSOR_TEST);

	glScissor (viewport [0],
	           viewport [1],
	           viewport [2],
	           viewport [3]);
}

} // X3D
} // titania
