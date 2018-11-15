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

#ifndef __TITANIA_X3D_BROWSER_SHADING_SHADER_H__
#define __TITANIA_X3D_BROWSER_SHADING_SHADER_H__

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/OpenGL.h"

namespace titania {
namespace X3D {

struct ShaderSource
{
	std::string              string;
	std::vector <basic::uri> uris;

};

class Shader
{
public:

	static
	bool
	getOpenGLES (const std::string & source);

	///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
	static
	ShaderSource
	getSource (X3DBaseNode* const node, const std::string & source, const basic::uri & worldURL, const bool shadow = false);

	static
	GLenum
	getShaderType (const std::string & type);

	static
	GLint
	getProgramStageBit (const std::string & type);

	static
	void
	printShaderInfoLog (X3DBrowser* const browser,
	                    const std::string & typeName,
	                    const std::string & name,
	                    const std::string & type,
	                    const GLint shaderId,
	                    const std::vector <basic::uri> & sources);

	static
	void
	printProgramInfoLog (X3DBrowser* const browser,
	                     const std::string & typeName,
	                     const std::string & name,
	                     const GLint programId,
	                     const std::vector <basic::uri> & sources);


private:

	///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
	static
	std::string
	getSource (X3DBaseNode* const node,
	           const std::string & source,
	           const basic::uri & worldURL,
	           std::vector <basic::uri> & sources,
	           const size_t level,
	           std::set <basic::uri> & files);

	static
	std::string
	addDefinitions (X3DBrowser* const browser, std::string source, const bool shadow);

	static
	void
	depreciatedWarning (X3DBrowser* const browser, const std::string & source, const std::string & depreciated, const std::string & current);

};

} // X3D
} // titania

#endif
