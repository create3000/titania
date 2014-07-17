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

#ifndef __TITANIA_X3D_DEBUG_H__
#define __TITANIA_X3D_DEBUG_H__

#include "Fields.h"
#include "Rendering/OpenGL.h"

namespace titania {
namespace X3D {

#define GL_ERROR  debug_gl_error (std::string (__FILE__) + ":" + std::to_string (__LINE__) + ": in function '" + __func__)

inline
void
debug_gl_error (const std::string & identifer)
{
	GLenum errorNum = glGetError ();

	if (errorNum not_eq GL_NO_ERROR)
		std::clog << "OpenGL Error at " << identifer << ": " << gluErrorString (errorNum) << std::endl;
}

inline
void
debug_roots (X3DChildObject* node, std::set <X3DChildObject*> & seen)
{
	if (not seen .emplace (node) .second)
		return;

	static int i = 0;

	__LOG__ << std::string (i, '\t') << node -> getParents () .size () << " : " << node -> getTypeName () << " : " << node -> getName () << " : " << node << std::endl;

	if (node -> getParents () .size ())
	{
		++ i;

		for (auto & child : node -> getParents ())
			debug_roots (child, seen);

		-- i;

		return;
	}

	__LOG__ << node -> getTypeName () << " : " << node -> getName () << std::endl;
}

inline
void
debug_roots (X3DChildObject* node)
{
	std::set <X3DChildObject*> seen;
	debug_roots (node, seen);
}

// prints all parent nodes of a nodes parent fields
inline
void
debug_print_parent_nodes (const X3D::X3DChildObject* object)
{
	static int i = 0;

	for (auto & parent : object -> getParents ())
	{
		auto node = dynamic_cast <X3D::X3DBaseNode*> (parent);

		if (node)
			__LOG__ << std::string (i, '\t') << node -> getName () << " : " << node -> getTypeName () << " : " << node << std::endl;

		else
		{
			++ i;

			debug_print_parent_nodes (parent);

			-- i;
		}
	}
}

} // X3D
} // titania

#endif
