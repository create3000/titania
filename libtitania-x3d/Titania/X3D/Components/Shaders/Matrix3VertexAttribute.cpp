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

#include "Matrix3VertexAttribute.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType Matrix3VertexAttribute::component      = ComponentType::SHADERS;
const std::string   Matrix3VertexAttribute::typeName       = "Matrix3VertexAttribute";
const std::string   Matrix3VertexAttribute::containerField = "attrib";

Matrix3VertexAttribute::Fields::Fields () :
	value (new MFMatrix3f ())
{ }

Matrix3VertexAttribute::Matrix3VertexAttribute (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DVertexAttributeNode (),
	                fields ()
{
	addType (X3DConstants::Matrix3VertexAttribute);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "name",     name ());
	addField (inputOutput,    "value",    value ());
}

X3DBaseNode*
Matrix3VertexAttribute::create (X3DExecutionContext* const executionContext) const
{
	return new Matrix3VertexAttribute (executionContext);
}

void
Matrix3VertexAttribute::addValue (std::vector <float> & array, const size_t index) const
{
	if (index < value () .size ())
	{
		const Matrix3f & mat3 = value () [index];

		array .insert (array .end (), mat3 .data (), mat3 .data () + mat3 .size ());
	}
	else
	{
		static const Matrix3f mat3;

		array .insert (array .end (), mat3 .data (), mat3 .data () + mat3 .size ());
	}
}

void
Matrix3VertexAttribute::enable (const GLint program, const GLuint buffer) const
{
	const GLint location = glGetAttribLocation (program, name () .c_str ());

	if (location == -1)
		return;

	glEnableVertexAttribArray (location + 0);
	glEnableVertexAttribArray (location + 1);
	glEnableVertexAttribArray (location + 2);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);

	glVertexAttribPointer (location + 0, 3, GL_FLOAT, false, sizeof (Matrix3f), (void*) (sizeof (Vector3f) * 0));
	glVertexAttribPointer (location + 1, 3, GL_FLOAT, false, sizeof (Matrix3f), (void*) (sizeof (Vector3f) * 1));
	glVertexAttribPointer (location + 2, 3, GL_FLOAT, false, sizeof (Matrix3f), (void*) (sizeof (Vector3f) * 2));
}

void
Matrix3VertexAttribute::disable (const GLint program) const
{
	const GLint location = glGetAttribLocation (program, name () .c_str ());

	if (location == -1)
		return;

	glDisableVertexAttribArray (location + 0);
	glDisableVertexAttribArray (location + 1);
	glDisableVertexAttribArray (location + 2);
}

} // X3D
} // titania
