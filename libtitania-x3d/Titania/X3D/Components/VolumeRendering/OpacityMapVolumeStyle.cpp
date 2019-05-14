/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "OpacityMapVolumeStyle.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"
#include "../Texturing/PixelTexture.h"
#include "../Texturing/X3DTexture2DNode.h"
#include "../Texturing3D/X3DTexture3DNode.h"

namespace titania {
namespace X3D {

const Component   OpacityMapVolumeStyle::component      = Component ("VolumeRendering", 2);
const std::string OpacityMapVolumeStyle::typeName       = "OpacityMapVolumeStyle";
const std::string OpacityMapVolumeStyle::containerField = "renderStyle";

OpacityMapVolumeStyle::Fields::Fields () :
	transferFunction (new SFNode ())
{ }

OpacityMapVolumeStyle::OpacityMapVolumeStyle (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposableVolumeRenderStyleNode (),
	                            fields (),
	              transferFunctionNode (),
			 transferFunctionTextureUnit (0)
{
	addType (X3DConstants::OpacityMapVolumeStyle);

	addField (inputOutput, "enabled", enabled ());
	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "transferFunction", transferFunction ());

	addChildObjects (transferFunctionNode);
}

X3DBaseNode*
OpacityMapVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new OpacityMapVolumeStyle (executionContext);
}

void
OpacityMapVolumeStyle::initialize ()
{
	X3DComposableVolumeRenderStyleNode::initialize ();

	getShader () -> enabled ()  .addInterest (&OpacityMapVolumeStyle::set_enabled,  this);
	getShader () -> disabled () .addInterest (&OpacityMapVolumeStyle::set_disabled, this);

	transferFunction () .addInterest (&OpacityMapVolumeStyle::set_transferFunction, this);

	set_transferFunction ();
}

const X3DPtr <ComposedShader> &
OpacityMapVolumeStyle::getShader () const
{
	return getBrowser () -> getOpacityMapVolumeStyleShader ();
}

void
OpacityMapVolumeStyle::set_transferFunction ()
{
	transferFunctionNode = x3d_cast <X3DTexture2DNode*> (transferFunction ());

	//if (not transferFunctionNode)
	//	transferFunctionNode = x3d_cast <X3DTexture3DNode*> (transferFunction ());

	if (not transferFunctionNode)
		transferFunctionNode = getBrowser () -> getDefaultTransferFunction ();
}

void
OpacityMapVolumeStyle::set_enabled ()
{
	if (getBrowser () -> getCombinedTextureUnits () .empty ())
	{
		transferFunctionTextureUnit = 0;
	}
	else
	{
		const auto program                         = getShader () -> getProgramId ();
		const auto transferFunctionUniformLocation = glGetUniformLocation (program, "transferFunction");

		transferFunctionTextureUnit = getBrowser () -> getCombinedTextureUnits () .top ();
		getBrowser () -> getCombinedTextureUnits () .pop ();

		glActiveTexture (GL_TEXTURE0 + transferFunctionTextureUnit);
		glBindTexture (transferFunctionNode -> getTarget (), transferFunctionNode -> getTextureId ());
		glUniform1i (transferFunctionUniformLocation, transferFunctionTextureUnit);
	}
}

void
OpacityMapVolumeStyle::set_disabled ()
{
	if (transferFunctionTextureUnit)
		getBrowser () -> getCombinedTextureUnits () .push (transferFunctionTextureUnit);
}

OpacityMapVolumeStyle::~OpacityMapVolumeStyle ()
{ }

} // X3D
} // titania
