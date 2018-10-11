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

#include "String.h"

#include <Titania/X3D/Components/CubeMapTexturing/X3DEnvironmentTextureNode.h>
#include <Titania/X3D/Components/Texturing/X3DTexture2DNode.h>
#include <Titania/X3D/Components/Texturing3D/X3DTexture3DNode.h>

#include <cmath>
#include <iomanip>
#include <sstream>

namespace titania {
namespace puck {

std::string
strfframes (const size_t value, const size_t framesPerSecond)
{
	size_t time = value;

	const size_t frames = time % framesPerSecond;
	time /= framesPerSecond;

	const size_t seconds = time % 60;
	time /= 60;

	const size_t minutes = time % 60;
	time /= 60;

	const size_t hours = time;

	std::ostringstream osstream;
	
	osstream
		<< std::setfill ('0')
		<< std::setw (2) << hours << ":"
		<< std::setw (2) << minutes << ":" 
		<< std::setw (2) << seconds << ":" 
		<< std::setw (2) << frames;

	return osstream .str ();
}

std::string
strftime (const long double value, const size_t fractions)
{
	const size_t framesPerSecond = std::pow (10, fractions);

	size_t time = std::round (value * framesPerSecond);

	const size_t milli = time % framesPerSecond;
	time /= framesPerSecond;

	const size_t seconds = time % 60;
	time /= 60;

	const size_t minutes = time % 60;
	time /= 60;

	const size_t hours = time;

	std::ostringstream osstream;
	
	osstream
		<< std::setfill ('0')
		<< std::setw (2) << hours << ":"
		<< std::setw (2) << minutes << ":" 
		<< std::setw (2) << seconds << "." 
		<< std::setw (fractions) << milli;

	return osstream .str ();
}

std::string
textureInfo (const X3D::X3DPtr <X3D::X3DTextureNode> & textureNode)
{
	const X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode (textureNode);

	if (texture2DNode)
	{
		std::string components;
	
		switch (texture2DNode -> components ())
		{
			case 1: components = _ ("GRAY");       break;
			case 2: components = _ ("GRAY ALPHA"); break;
			case 3: components = _ ("RGB");        break;
			case 4: components = _ ("RGBA");       break;
			default:
				break;
		}
	
		return std::to_string (texture2DNode -> width ()) +
		       " × " +
		       std::to_string (texture2DNode -> height ()) +
		       " (" +
		       components +
		       ")";
	}

	const X3D::X3DPtr <X3D::X3DTexture3DNode> texture3DNode (textureNode);

	if (texture3DNode)
	{
		std::string components;
	
		switch (texture3DNode -> components ())
		{
			case 1: components = _ ("GRAY");       break;
			case 2: components = _ ("GRAY ALPHA"); break;
			case 3: components = _ ("RGB");        break;
			case 4: components = _ ("RGBA");       break;
			default:
				break;
		}
	
		return std::to_string (texture3DNode -> width ()) +
		       " × " +
		       std::to_string (texture3DNode -> height ()) +
		       " × " +
		       std::to_string (texture3DNode -> depth ()) +
		       " (" +
		       components +
		       ")";
	}

	const X3D::X3DPtr <X3D::X3DEnvironmentTextureNode> environmentTexture (textureNode);

	if (environmentTexture)
	{
		std::string components;
	
		switch (environmentTexture -> getComponents ())
		{
			case 1: components = _ ("GRAY");       break;
			case 2: components = _ ("GRAY ALPHA"); break;
			case 3: components = _ ("RGB");        break;
			case 4: components = _ ("RGBA");       break;
			default:
				break;
		}

		return std::to_string (environmentTexture -> getWidth ()) +
		       " × " +
		       std::to_string (environmentTexture -> getHeight ()) +
		       " × 6 " +
		       " (" +
		       components +
		       ")";
	}

	return "";
}

std::string
loadStateInfo (const X3D::LoadState value)
{
	switch (value)
	{
		case X3D::NOT_STARTED_STATE:
		{
			return _ ("NOT STARTED");
		}
		case X3D::IN_PROGRESS_STATE:
		{
			return _ ("IN PROGRESS");
		}
		case X3D::COMPLETE_STATE:
		{
			return _ ("COMPLETE");
		}
		case X3D::FAILED_STATE:
		{
			return _ ("FAILED");
		}
	}

	return _ ("NOT STARTED");
}

} // puck
} // titania
