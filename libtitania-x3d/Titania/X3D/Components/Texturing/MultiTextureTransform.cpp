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

#include "MultiTextureTransform.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const Component   MultiTextureTransform::component      = Component ("Texturing", 2);
const std::string MultiTextureTransform::typeName       = "MultiTextureTransform";
const std::string MultiTextureTransform::containerField = "textureTransform";

MultiTextureTransform::Fields::Fields () :
	textureTransform (new MFNode ())
{ }

MultiTextureTransform::MultiTextureTransform (X3DExecutionContext* const executionContext) :
	            X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTextureTransformNode (),
	                 fields (),
	  textureTransformNodes ()
{
	addType (X3DConstants::MultiTextureTransform);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "textureTransform", textureTransform ());

	addChildObjects (textureTransformNodes);
}

X3DBaseNode*
MultiTextureTransform::create (X3DExecutionContext* const executionContext) const
{
	return new MultiTextureTransform (executionContext);
}

void
MultiTextureTransform::initialize ()
{
	X3DTextureTransformNode::initialize ();
	
	textureTransform () .addInterest (&MultiTextureTransform::set_textureTransform, this);
}

void
MultiTextureTransform::set_textureTransform ()
{
	for (const auto & node : textureTransformNodes)
		node -> removeInterest (this);

	std::vector <X3DTextureTransformNode*> value;

	for (const auto & node : textureTransform ())
	{
		if (x3d_cast <MultiTextureTransform*> (node))
			continue;

		const auto textureTransformNode = x3d_cast <X3DTextureTransformNode*> (node);
		
		if (textureTransformNode)
			value .emplace_back (textureTransformNode);
	}

	textureTransformNodes .set (value .cbegin (), value .cend ());

	for (const auto & node : textureTransformNodes)
		node -> addInterest (this);
}

void
MultiTextureTransform::draw (X3DRenderObject* const renderObject)
{
	X3DBrowser* const              browser                 = renderObject -> getBrowser ();
	X3DTextureTransformNode* const defaultTextureTransform = browser -> getDefaultTextureTransform ();
	X3DTextureTransformNode*       last                    = defaultTextureTransform;
	size_t                         channel                 = 0;
	const size_t                   size                    = browser -> getTextureStages () .size ();

	for (const auto & textureTransform : textureTransformNodes)
	{
		const int32_t unit = channel < size ? browser -> getTextureStages () [channel] : 0;

		if (unit >= 0)
			textureTransform -> draw (renderObject, unit);

		last = textureTransform;
		++ channel;

		if (channel >= size)
			break;
	}

	for ( ; channel < size; ++ channel)
	{
		const int32_t unit = browser -> getTextureStages () [channel];

		if (unit >= 0)
			last -> draw (renderObject, unit);
	}
}

void
MultiTextureTransform::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject) const
{

}

} // X3D
} // titania
