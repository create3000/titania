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

#include "ComposedVolumeStyle.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"

namespace titania {
namespace X3D {

const Component   ComposedVolumeStyle::component      = Component ("VolumeRendering", 2);
const std::string ComposedVolumeStyle::typeName       = "ComposedVolumeStyle";
const std::string ComposedVolumeStyle::containerField = "renderStyle";

ComposedVolumeStyle::Fields::Fields () :
	renderStyle (new MFNode ())
{ }

ComposedVolumeStyle::ComposedVolumeStyle (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposableVolumeRenderStyleNode (),
	                            fields (),
	                  renderStyleNodes ()
{
	addType (X3DConstants::ComposedVolumeStyle);

	addField (inputOutput, "enabled", enabled ());
	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "renderStyle", renderStyle ());

	addChildObjects (renderStyleNodes);
}

X3DBaseNode*
ComposedVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new ComposedVolumeStyle (executionContext);
}

void
ComposedVolumeStyle::initialize ()
{
	X3DComposableVolumeRenderStyleNode::initialize ();

	renderStyle () .addInterest (&ComposedVolumeStyle::set_renderStyle, this);

	set_renderStyle ();
}

void
ComposedVolumeStyle::set_renderStyle ()
{
	for (const auto & renderStyleNode : renderStyleNodes)
	{
		renderStyleNode -> removeInterest (&ComposedVolumeStyle::addEvent, this);

		for (const auto & volumeData : getVolumeData ())
			renderStyleNode -> removeVolumeData (volumeData);
	}

	renderStyleNodes .clear ();

	for (const auto & node : renderStyle ())
	{
		const auto renderStyleNode = x3d_cast <X3DComposableVolumeRenderStyleNode*> (node);

		if (renderStyleNode)
			renderStyleNodes .emplace_back (renderStyleNode);
	}

	for (const auto & renderStyleNode : renderStyleNodes)
	{
		renderStyleNode -> addInterest (&ComposedVolumeStyle::addEvent, this);

		for (const auto & volumeData : getVolumeData ())
			renderStyleNode -> addVolumeData (volumeData);
	}
}

void
ComposedVolumeStyle::addVolumeData (X3DVolumeDataNode* const volumeDataNode)
{
	X3DComposableVolumeRenderStyleNode::addVolumeData (volumeDataNode);

	for (const auto & renderStyleNode : renderStyleNodes)
		renderStyleNode -> addVolumeData (volumeDataNode);
}

void
ComposedVolumeStyle::removeVolumeData (X3DVolumeDataNode* const volumeDataNode)
{
	X3DComposableVolumeRenderStyleNode::removeVolumeData (volumeDataNode);

	for (const auto & renderStyleNode : renderStyleNodes)
		renderStyleNode -> removeVolumeData (volumeDataNode);
}

void
ComposedVolumeStyle::addShaderFields (const X3DPtr <ComposedShader> & shaderNode) const
{
	if (not enabled ())
		return;

	for (const auto & renderStyleNode : renderStyleNodes)
		renderStyleNode -> addShaderFields (shaderNode);
}

std::string
ComposedVolumeStyle::getUniformsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	for (const auto & renderStyleNode : renderStyleNodes)
		string += renderStyleNode -> getUniformsText ();

	string += "\n";
	string += "vec4\n";
	string += "getComposedStyle_" + getStyleId () + " (in vec4 textureColor, in vec3 texCoord)\n";
	string += "{\n";

	for (const auto & renderStyleNode : renderStyleNodes)
		string += renderStyleNode -> getFunctionsText ();

	string += "\n";
	string += "	return textureColor;\n";
	string += "}\n";

	return string;
}

std::string
ComposedVolumeStyle::getFunctionsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "	// ComposedVolumeStyle\n";
	string += "\n";
	string += "	textureColor = getComposedStyle_" + getStyleId () + " (textureColor, texCoord);\n";

	return string;
}

void
ComposedVolumeStyle::shutdown ()
{
	for (const auto & renderStyleNode : renderStyleNodes)
	{
		for (const auto & volumeData : getVolumeData ())
			renderStyleNode -> removeVolumeData (volumeData);
	}

	X3DComposableVolumeRenderStyleNode::shutdown ();
}

ComposedVolumeStyle::~ComposedVolumeStyle ()
{ }

} // X3D
} // titania
