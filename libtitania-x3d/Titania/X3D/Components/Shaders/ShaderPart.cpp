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

#include "ShaderPart.h"

#include "../../Browser/Shaders/Shader.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../InputOutput/FileLoader.h"

namespace titania {
namespace X3D {

const ComponentType ShaderPart::component      = ComponentType::SHADERS;
const std::string   ShaderPart::typeName       = "ShaderPart";
const std::string   ShaderPart::containerField = "parts";

ShaderPart::Fields::Fields () :
	type (new SFString ("VERTEX"))
{ }

ShaderPart::ShaderPart (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     X3DNode (),
	X3DUrlObject (),
	      fields (),
	      buffer (),
	    shaderId (0),
	       valid (false),
	    openGLES (false),
	      shadow (false)
{
	addType (X3DConstants::ShaderPart);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "type",     type ());
	addField (inputOutput,    "url",      url ());

	addChildObjects (buffer);
}

X3DBaseNode*
ShaderPart::create (X3DExecutionContext* const executionContext) const
{
	return new ShaderPart (executionContext);
}

void
ShaderPart::initialize ()
{
	X3DNode::initialize ();
	X3DUrlObject::initialize ();

	type () .addInterest (&ShaderPart::set_url, this);
	url ()  .addInterest (&ShaderPart::set_url, this);

	buffer .addInterest (&ShaderPart::set_buffer, this);

	set_url ();
}

void
ShaderPart::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DUrlObject::setExecutionContext (executionContext);
	X3DNode::setExecutionContext (executionContext);
}

void
ShaderPart::requestImmediateLoad ()
{
	if (not getBrowser () -> getLoadUrlObjects ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	valid = false;

	for (const auto & URL : basic::make_const_range (url ()))
	{
		try
		{
			FileLoader loader (getExecutionContext ());

			const auto document = loader .loadDocument (URL .raw ());
			const auto source   = Shader::getSource (this, document, loader .getWorldURL (), shadow);
			const auto string   = source .string .c_str ();

			openGLES = Shader::getOpenGLES (source .string);

			if (shaderId)
				glDeleteShader (shaderId);

			shaderId = glCreateShader (Shader::getShaderType (type ()));

			if (shaderId)
			{
				glShaderSource  (shaderId, 1, &string, nullptr);
				glCompileShader (shaderId);

				glGetShaderiv (shaderId, GL_COMPILE_STATUS, &valid);

				Shader::printShaderInfoLog (getBrowser (), getTypeName (), getName (), type (), shaderId, source .uris);

				if (valid)
					break;
			}
		}
		catch (const X3DError & error)
		{
			std::clog << error .what () << std::endl;
		}
	}

	setLoadState (valid ? COMPLETE_STATE : FAILED_STATE);
}

void
ShaderPart::set_url ()
{
	buffer .addEvent ();
}

void
ShaderPart::set_buffer ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();
}

void
ShaderPart::dispose ()
{
	try
	{
		ContextLock lock (getBrowser ());

		if (shaderId)
			glDeleteShader (shaderId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	X3DUrlObject::dispose ();
	X3DNode::dispose ();
}

} // X3D
} // titania
