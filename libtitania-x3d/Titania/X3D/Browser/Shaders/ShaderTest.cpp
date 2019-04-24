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

#include "ShaderTest.h"

#include "../../Components/Shaders/ComposedShader.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/OpenGL.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

bool
ShaderTest::verify (X3DBrowser* const browser, X3DPtr <ComposedShader> & shaderNode)
{
	static const std::vector <float> normals = {
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
	};

	static const std::vector <double> vertices = {
		 2,  2, 0,
		-2,  2, 0,
		-2, -2, 0,
		 2,  2, 0,
		-2, -2, 0,
		 2, -2, 0,
	};

	FrameBuffer frameBuffer (browser, 16, 16, 0);

	frameBuffer .setup ();
	frameBuffer .bind ();

	GLuint normalBufferId = 0;
	GLuint vertexBufferId = 0;

	glGenBuffers (1, &normalBufferId);
	glGenBuffers (1, &vertexBufferId);

	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (double) * vertices .size (), vertices .data (), GL_STATIC_COPY);
	glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (float) * normals .size (), normals .data (), GL_STATIC_COPY);

	shaderNode -> enable ();
	shaderNode -> setClipPlanes (browser, { });

	glUniformMatrix4fv (shaderNode -> getProjectionMatrixUniformLocation (), 1, false, camera <float>::ortho (-1, 1, -1, 1, -1, 1) .front () .data ());
	glUniformMatrix4fv (shaderNode -> getModelViewMatrixUniformLocation (),  1, false, Matrix4f () .front () .data ());
	glUniformMatrix3fv (shaderNode -> getNormalMatrixUniformLocation (),     1, false, Matrix3f () .front () .data ());

	glUniform1i (shaderNode -> getFogTypeUniformLocation (),               0);
	glUniform1i (shaderNode -> getFillPropertiesFilledUniformLocation (),  true);
	glUniform1i (shaderNode -> getFillPropertiesHatchedUniformLocation (), false);
	glUniform1i (shaderNode -> getColorMaterialUniformLocation (),         false);
	glUniform1i (shaderNode -> getLightingUniformLocation (),              true);
	glUniform1i (shaderNode -> getNumLightsUniformLocation (),             0);
	glUniform1i (shaderNode -> getNumTexturesUniformLocation (),           0);

	glUniform1i (shaderNode -> getSeparateBackColorUniformLocation (), false);
	glUniform1f (shaderNode -> getAmbientIntensityUniformLocation (),  0);
	glUniform3f (shaderNode -> getDiffuseColorUniformLocation (),      1, 0, 0);
	glUniform3f (shaderNode -> getSpecularColorUniformLocation (),     1, 0, 0);
	glUniform3f (shaderNode -> getEmissiveColorUniformLocation (),     1, 0, 0);
	glUniform1f (shaderNode -> getShininessUniformLocation (),         0);
	glUniform1f (shaderNode -> getTransparencyUniformLocation (),      0);

	glClearColor (0, 0, 0, 0);
	glClear (GL_COLOR_BUFFER_BIT);

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_BLEND);
	glFrontFace (GL_CCW);
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);

	shaderNode -> enableNormalAttrib (normalBufferId, GL_FLOAT,  0, nullptr);
	shaderNode -> enableVertexAttrib (vertexBufferId, GL_DOUBLE, 0, nullptr);

	glDrawArrays (GL_TRIANGLES, 0, 6);

	shaderNode -> disableNormalAttrib ();
	shaderNode -> disableVertexAttrib ();
	shaderNode -> disable ();

	glDeleteBuffers (1, &normalBufferId);
	glDeleteBuffers (1, &vertexBufferId);

	const auto & data = frameBuffer .readPixels ();

	frameBuffer .unbind ();

//	for (const auto p : data)
//		std::clog << int (p) << " ";
//	std::clog << std::endl;

	return data [0] == 255 and data [1] == 0 and data [2] == 0 and data [3] == 255;
}

} // X3D
} // titania
