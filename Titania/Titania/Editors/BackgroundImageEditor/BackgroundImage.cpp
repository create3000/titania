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

#include "BackgroundImage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/X3DNotebookPage.h"

#include <Titania/X3D/Components/Layering/Layer.h>
#include <Titania/X3D/Components/Texturing/ImageTexture.h>
#include <Titania/X3D/Components/Texturing/TextureProperties.h>

namespace titania {
namespace puck {

BackgroundImage::BackgroundImage (X3DNotebookPage* const page) :
	 X3DBaseInterface (page -> getBrowserWindow (), page -> getMainBrowser ()),
	             page (page),
	              url (),
	     transparency (0),
	          texture (page -> getMainBrowser () -> createNode <X3D::ImageTexture> ()),
	textureProperties (page -> getMainBrowser () -> createNode <X3D::TextureProperties> ())
{
	addChildObjects (url,
	                 transparency,
	                 texture,
	                 textureProperties);

	page -> getMainBrowser () -> getActiveLayer () .addInterest (&BackgroundImage::set_activeLayer, this);
	addInterest (&BackgroundImage::eventsProcessed, this);

	textureProperties -> generateMipMaps ()     = true;
	textureProperties -> minificationFilter ()  = "LINEAR";
	textureProperties -> magnificationFilter () = "LINEAR";
	textureProperties -> boundaryModeS ()       = "CLAMP_TO_EDGE";
	textureProperties -> boundaryModeT ()       = "CLAMP_TO_EDGE";
	textureProperties -> boundaryModeR ()       = "CLAMP_TO_EDGE";
	textureProperties -> textureCompression ()  = "DEFAULT";

	texture -> textureProperties () = textureProperties;

	setup ();
}

void
BackgroundImage::setUrl (const X3D::MFString & value)
{
	url = value;

	X3D::MFString absoluteUrl;

	for (const auto & URL : basic::make_const_range (url))
		absoluteUrl .emplace_back (page -> getMainBrowser () -> getExecutionContext () -> getWorldURL () .transform (URL .raw ()));

	texture -> url () = absoluteUrl;
}

void
BackgroundImage::set_activeLayer ()
{
	removeInterest (&BackgroundImage::eventsProcessed, this);
	addInterest (&BackgroundImage::connectEventsProcessed, this);

	setUrl          (getMetaData <X3D::MFString> (page -> getMainBrowser (), "/Titania/BackgroundImage/url"));
	setTransparency (getMetaData <float>         (page -> getMainBrowser (), "/Titania/BackgroundImage/transparency"));
}

void
BackgroundImage::eventsProcessed ()
{
	setMetaData (page -> getMainBrowser (), "/Titania/BackgroundImage/url",          getUrl ());
	setMetaData (page -> getMainBrowser (), "/Titania/BackgroundImage/transparency", getTransparency ());

	page -> setModified (true);
}

void
BackgroundImage::connectEventsProcessed ()
{
	removeInterest (&BackgroundImage::connectEventsProcessed, this);
	addInterest (&BackgroundImage::eventsProcessed, this);
}

BackgroundImage::~BackgroundImage ()
{
	dispose ();
}

} // puck
} // titania
