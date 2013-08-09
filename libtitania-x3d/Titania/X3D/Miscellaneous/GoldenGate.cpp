/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "GoldenGate.h"

#include "../Fields.h"
#include "../Parser/Filter.h"
#include "../Miscellaneous/MediaStream.h"

#include <Titania/OS.h>
#include <Titania/Physics/Constants.h>

#include <Magick++.h>
#include <giomm.h>
#include <pcrecpp.h>

namespace titania {
namespace X3D {

static const pcrecpp::RE Name        ("__NAME__");
static const pcrecpp::RE Description ("__DESCRIPTION__");
static const pcrecpp::RE Width       ("__WIDTH__");
static const pcrecpp::RE Height      ("__HEIGHT__");
static const pcrecpp::RE URL         ("__URL__");

static
std::string
get_name (const basic::uri & uri)
{
	static const pcrecpp::RE Spaces ("\\s+");

	std::string name = uri .basename (false);

	Spaces .GlobalReplace ("_", &name);

	filter_non_id_characters (name);

	if (name .size ())
		return name;

	return "unnamed";
}

static
basic::ifilestream
golden_image (const basic::uri & uri)
{
	auto locale = std::locale::global (std::locale::classic ());

	try
	{
		Magick::Image image;
		image .read (uri);
		image .resolutionUnits (Magick::PixelsPerInchResolution);

		float width  = (float) image .size () .width  () / (float) image .density () .width  () * M_INCH;
		float height = (float) image .size () .height () / (float) image .density () .height () * M_INCH;

		std::string file = os::load_file (os::find_data_file ("titania/filetype/image.wrl"));

		Name   .GlobalReplace (get_name (uri), &file);
		Width  .GlobalReplace (std::to_string (width), &file);
		Height .GlobalReplace (std::to_string (height), &file);
		URL    .GlobalReplace (MFString ({ uri .basename (), uri .str () }) .toString (), &file);

		std::locale::global (locale);

		return basic::ifilestream (basic::http::GET, "data:model/vrml;charset=UTF-8," + file);
	}
	catch (...)
	{
		std::locale::global (locale);
		throw;
	}
}

static
basic::ifilestream
golden_audio (const basic::uri & uri)
{
	std::string file = os::load_file (os::find_data_file ("titania/filetype/audio.wrl"));

	Name        .GlobalReplace (get_name (uri), &file);
	Description .GlobalReplace (SFString (uri .basename (false)) .toString (), &file);
	URL         .GlobalReplace (MFString ({ uri .basename (), uri .str () }) .toString (), &file);

	return basic::ifilestream (basic::http::GET, "data:model/vrml;charset=UTF-8," + file);
}

static
basic::ifilestream
golden_video (const basic::uri & uri)
{
	MediaStream mediaStream;

	mediaStream .setUri (uri);
	mediaStream .sync ();

	std::string file = os::load_file (os::find_data_file ("titania/filetype/video.wrl"));

	float width  = 1;
	float height = 1;

	if (mediaStream .getVideoSink () -> get_last_buffer ())
	{
		width  = (float) mediaStream .getVideoSink () -> get_width  () / 72.0f * M_INCH;
		height = (float) mediaStream .getVideoSink () -> get_height () / 72.0f * M_INCH;
	}

	auto locale = std::locale::global (std::locale::classic ());

	Name        .GlobalReplace (get_name (uri), &file);
	Description .GlobalReplace (SFString (uri .basename (false)) .toString (), &file);
	Width       .GlobalReplace (std::to_string (width),  &file);
	Height      .GlobalReplace (std::to_string (height), &file);
	URL         .GlobalReplace (MFString ({ uri .basename (), uri .str () }) .toString (), &file);

	std::locale::global (locale);

	return basic::ifilestream (basic::http::GET, "data:model/vrml;charset=UTF-8," + file);
}

basic::ifilestream
golden_gate (const basic::uri & uri, basic::ifilestream && istream)
{
	try
	{
		if (Gio::content_type_is_a (istream .response_headers () .at ("Content-Type"), "image/*"))
			return golden_image (uri);

		else if (Gio::content_type_is_a (istream .response_headers () .at ("Content-Type"), "audio/*"))
			return golden_audio (uri);

		else if (Gio::content_type_is_a (istream .response_headers () .at ("Content-Type"), "video/*"))
			return golden_video (uri);
	}
	catch (...)
	{ }

	return std::move (istream);
}

	

} // X3D
} // titania
