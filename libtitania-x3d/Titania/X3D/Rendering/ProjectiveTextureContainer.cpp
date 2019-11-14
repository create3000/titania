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

#include "ProjectiveTextureContainer.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/ProjectiveTextureMapping/X3DTextureProjectorNode.h"
#include "../Components/Shaders/X3DProgrammableShaderObject.h"
#include "../Components/Texturing/X3DTexture2DNode.h"
#include "../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

ProjectiveTextureContainer::ProjectiveTextureContainer (X3DBrowser* const browser, X3DTextureProjectorNode* const node, const Matrix4d & modelViewMatrix) :
	                  browser (browser),
	                     node (node),
	          modelViewMatrix ({ modelViewMatrix }),
	  projectiveTextureMatrix (),
	projectiveTextureLocation ()
{
}

void
ProjectiveTextureContainer::setGlobalVariables (X3DRenderObject* const renderObject)
{
	node -> setGlobalVariables (renderObject, this);
}

void
ProjectiveTextureContainer::setShaderUniforms (X3DRenderObject* const renderObject, X3DProgrammableShaderObject* const shaderObject, const size_t i)
{
	if (shaderObject -> hasProjectiveTexture (i, this))
		return;

	glActiveTexture (GL_TEXTURE0 + browser -> getProjectiveTextureUnits () [i]);
	glBindTexture (GL_TEXTURE_2D, node -> getTexture () -> getTextureId ());
	glActiveTexture (GL_TEXTURE0);

	if (shaderObject -> isExtensionGPUShaderFP64Available ())
		glUniformMatrix4dv (shaderObject -> getProjectiveTextureMatrixUniformLocation () [i], 1, false, (projectiveTextureMatrix) .front () .data ());
	else
		glUniformMatrix4fv (shaderObject -> getProjectiveTextureMatrixUniformLocation () [i], 1, false, Matrix4f (projectiveTextureMatrix) .front () .data ());

	glUniform3fv (shaderObject -> getProjectiveTextureLocationUniformLocation () [i], 1, projectiveTextureLocation .data ());
}

ProjectiveTextureContainer::~ProjectiveTextureContainer ()
{ }

} // X3D
} // titania
