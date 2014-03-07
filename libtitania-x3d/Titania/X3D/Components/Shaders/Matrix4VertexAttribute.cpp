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

#include "Matrix4VertexAttribute.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string Matrix4VertexAttribute::componentName  = "Shaders";
const std::string Matrix4VertexAttribute::typeName       = "Matrix4VertexAttribute";
const std::string Matrix4VertexAttribute::containerField = "attrib";

Matrix4VertexAttribute::Fields::Fields () :
	value (new MFMatrix4f ())
{ }

Matrix4VertexAttribute::Matrix4VertexAttribute (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DVertexAttributeNode (),
	                fields ()
{
	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "name",     name ());
	addField (inputOutput,    "value",    value ());
}

X3DBaseNode*
Matrix4VertexAttribute::create (X3DExecutionContext* const executionContext) const
{
	return new Matrix4VertexAttribute (executionContext);
}

void
Matrix4VertexAttribute::addValue (std::vector <float> & values, const size_t index) const
{
	if (index < value () .size ())
	{
		for (size_t i = 0; i < 16; ++ i)
			values .emplace_back (value () [index] .get1Value (i));
	}
	else
		values .resize (value () .size () + 16);
}

void
Matrix4VertexAttribute::enable (const GLint program, const GLuint buffer) const
{
//	glBindBuffer (GL_ARRAY_BUFFER, buffer);
//
//	glEnableVertexAttribArray (i + 0);
//	glEnableVertexAttribArray (i + 1);
//	glEnableVertexAttribArray (i + 2);
//	glEnableVertexAttribArray (i + 3);
//
//	glVertexAttribPointer (i + 0, 4, GL_FLOAT, false, sizeof (float) * 16, (void*) (sizeof (float) * 0));
//	glVertexAttribPointer (i + 1, 4, GL_FLOAT, false, sizeof (float) * 16, (void*) (sizeof (float) * 4));
//	glVertexAttribPointer (i + 2, 4, GL_FLOAT, false, sizeof (float) * 16, (void*) (sizeof (float) * 8));
//	glVertexAttribPointer (i + 3, 4, GL_FLOAT, false, sizeof (float) * 16, (void*) (sizeof (float) * 12));
//
//	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
Matrix4VertexAttribute::disable (const GLint program) const
{
//	glDisableVertexAttribArray (i ++);
//	glDisableVertexAttribArray (i ++);
//	glDisableVertexAttribArray (i ++);
//	glDisableVertexAttribArray (i ++);
}

} // X3D
} // titania
