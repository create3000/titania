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

#include "X3DRenderPanel.h"

#include "../NotebookPage/NotebookPage.h"

namespace titania {
namespace puck {

X3DRenderPanel::X3DRenderPanel () :
	X3DRenderPanelInterface ()
{ }

void
X3DRenderPanel::setFilename (const size_t id, const basic::uri & filename)
{
	auto filenameArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/RenderPanel/filename");

	filenameArray .resize (4);

	if (filenameArray [id] not_eq filename .str ())
	{
		filenameArray [id] = filename .str ();
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/filename", filenameArray);
	
		getPage () -> setModified (true);
	}
}

basic::uri
X3DRenderPanel::getFilename (const size_t id, const basic::uri & filename) const
{
	try
	{
		const auto filenameArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/RenderPanel/filename");

		if (filenameArray .at (id) .empty ())
			return filename;

		return filenameArray .at (id) .raw ();
	}
	catch (const std::exception &)
	{
		return filename;
	}
}

void
X3DRenderPanel::setCodec (const size_t id, const std::string & codec)
{
	auto codecArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/RenderPanel/codec");

	codecArray .resize (4);

	if (codecArray [id] not_eq codec)
	{
		codecArray [id] = codec;
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/codec", codecArray);
	
		getPage () -> setModified (true);
	}
}

std::string
X3DRenderPanel::getCodec (const size_t id, const std::string & codec) const
{
	try
	{
		const auto codecArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/RenderPanel/codec");

		if (codecArray .at (id) .empty ())
			return codec;

		return codecArray .at (id);
	}
	catch (const std::exception &)
	{
		return codec;
	}
}

void
X3DRenderPanel::setDuration (const size_t id, const int32_t duration)
{
	auto durationArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/duration");

	durationArray .resize (4, X3D::SFInt32 (-1));

	if (durationArray [id] not_eq duration)
	{
		durationArray [id] = duration;
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/duration", durationArray);
	
		getPage () -> setModified (true);
	}
}

size_t
X3DRenderPanel::getDuration (const size_t id, const int32_t duration) const
{
	try
	{
		const auto durationArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/duration");

		if (durationArray .at (id) <= 0)
			return duration;

		return durationArray .at (id);
	}
	catch (const std::exception &)
	{
		return duration;
	}
}

void
X3DRenderPanel::setFrameRate (const size_t id, const int32_t frameRate)
{
	auto frameRateArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/frameRate");

	frameRateArray .resize (4, X3D::SFInt32 (-1));

	if (frameRateArray [id] not_eq frameRate)
	{
		frameRateArray [id] = frameRate;
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/frameRate", frameRateArray);
	
		getPage () -> setModified (true);
	}
}

size_t
X3DRenderPanel::getFrameRate (const size_t id, const int32_t frameRate) const
{
	try
	{
		const auto frameRateArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/frameRate");

		if (frameRateArray .at (id) <= 0)
			return frameRate;

		return frameRateArray .at (id);
	}
	catch (const std::exception &)
	{
		return frameRate;
	}
}

void
X3DRenderPanel::setWidth (const size_t id, const int32_t width)
{
	auto widthArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/width");

	widthArray .resize (4, X3D::SFInt32 (-1));

	if (widthArray [id] not_eq width)
	{
		widthArray [id] = width;
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/width", widthArray);
	
		getPage () -> setModified (true);
	}
}

size_t
X3DRenderPanel::getWidth (const size_t id, const int32_t width) const
{
	try
	{
		const auto widthArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/width");

		if (widthArray .at (id) <= 0)
			return width;

		return widthArray .at (id);
	}
	catch (const std::exception &)
	{
		return width;
	}
}

void
X3DRenderPanel::setHeight (const size_t id, const int32_t height)
{
	auto heightArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/height");

	heightArray .resize (4, X3D::SFInt32 (-1));

	if (heightArray [id] not_eq height)
	{
		heightArray [id] = height;
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/height", heightArray);
	
		getPage () -> setModified (true);
	}
}

size_t
X3DRenderPanel::getHeight (const size_t id, const int32_t height) const
{
	try
	{
		const auto heightArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/height");

		if (heightArray .at (id) <= 0)
			return height;

		return heightArray .at (id);
	}
	catch (const std::exception &)
	{
		return height;
	}
}

void
X3DRenderPanel::setAntialiasing (const size_t id, const int32_t antialiasing)
{
	auto antialiasingArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/antialiasing");

	antialiasingArray .resize (4, X3D::SFInt32 (-1));

	if (antialiasingArray [id] not_eq antialiasing)
	{
		antialiasingArray [id] = antialiasing;
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/antialiasing", antialiasingArray);
	
		getPage () -> setModified (true);
	}
}

size_t
X3DRenderPanel::getAntialiasing (const size_t id, const int32_t antialiasing) const
{
	try
	{
		const auto antialiasingArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFInt32> ("/Titania/RenderPanel/antialiasing");

		if (antialiasingArray .at (id) < 0)
			return antialiasing;

		return antialiasingArray .at (id);
	}
	catch (const std::exception &)
	{
		return antialiasing;
	}
}

void
X3DRenderPanel::setViewpoint (const size_t id, const std::string & viewpoint)
{
	auto viewpointArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/RenderPanel/viewpoint");

	viewpointArray .resize (4);

	if (viewpointArray [id] not_eq viewpoint)
	{
		viewpointArray [id] = viewpoint;
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/viewpoint", viewpointArray);
	
		getPage () -> setModified (true);
	}
}

void
X3DRenderPanel::setShading (const size_t id, const std::string & shading)
{
	auto shadingArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/RenderPanel/shading");

	shadingArray .resize (4);

	if (shadingArray [id] not_eq shading)
	{
		shadingArray [id] = shading;
	
		createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/RenderPanel/shading", shadingArray);
	
		getPage () -> setModified (true);
	}
}

std::string
X3DRenderPanel::getShading (const size_t id, const std::string & shading) const
{
	try
	{
		const auto shadingArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/RenderPanel/shading");

		if (shadingArray .at (id) .empty ())
			return shading;

		return shadingArray .at (id);
	}
	catch (const std::exception &)
	{
		return shading;
	}
}

std::string
X3DRenderPanel::getViewpoint (const size_t id, const std::string & viewpoint) const
{
	try
	{
		const auto viewpointArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/RenderPanel/viewpoint");

		return viewpointArray .at (id);
	}
	catch (const std::exception &)
	{
		return viewpoint;
	}
}

X3DRenderPanel::~X3DRenderPanel ()
{ }

} // puck
} // titania
