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
#include "../Miscellaneous/MediaStream.h"
#include "../Parser/Filter.h"
#include "../Parser/Wavefront/Parser.h"

#include <Titania/OS.h>
#include <Titania/Physics/Constants.h>
#include <Titania/String/to_string.h>

#include <Magick++.h>
#include <giomm.h>
#include <pcrecpp.h>

//

namespace titania {
namespace X3D {

static const pcrecpp::RE Name        ("__NAME__");
static const pcrecpp::RE Description ("__DESCRIPTION__");
static const pcrecpp::RE Width       ("__WIDTH__");
static const pcrecpp::RE Height      ("__HEIGHT__");
static const pcrecpp::RE URL         ("__URL__");

static
void
golden_x3dv (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream && goldenstream)
{
	scene -> fromStream (uri, goldenstream);
}

static
std::string
golden_pipe (const std::string & program, const std::string & input)
//throw (Error <URL_UNAVAILABLE>)
{
	constexpr size_t BUFFER_SIZE = 1024;

	int    stdin     = 0;
	int    stdout    = 0;
	int    stderr    = 0;
	size_t bytesRead = 0;

	std::vector <char> buffer (BUFFER_SIZE);

	// Open pipe.

	if (os::popen3 (program, &stdin, &stdout, &stderr) <= 0)
		throw Error <URL_UNAVAILABLE> ("Couldn't open program '" + program + "'.");

	// Write to pipe.

	if (write (stdin, input .c_str (), input .size ()) not_eq (int) input .size ())
		throw Error <URL_UNAVAILABLE> ("Write to pipe failed.");

	close (stdin);

	// Read from pipe.

	std::string output;

	while ((bytesRead = read (stdout, buffer .data (), sizeof (char) * BUFFER_SIZE)) > 0)
		output .append (buffer .data (), bytesRead);

	close (stdout);

	// Read error from pipe.

	while ((bytesRead = read (stderr, buffer .data (), sizeof (char) * BUFFER_SIZE)) > 0)
		std::clog .write (buffer .data (), bytesRead);

	close (stderr);

	return output;
}

static
void
golden_x3d (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream && istream)
{
	static const std::string x3d2vrml = "x3d2vrml";

	// Parse into stream.

	basic::ifilestream goldenstream (golden_pipe (x3d2vrml, basic::to_string (istream)));

	scene -> fromStream (uri, goldenstream);
}

static
void
golden_obj (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream && istream)
{
	Wavefront::Parser (scene, uri, istream) .parseIntoScene ();

//	const std::string obj2wrl = "perl '" + os::find_data_file ("titania/goldengate/obj2wrl.pl") + "'";
//
//	// Parse into stream.
//
//	basic::ifilestream goldenstream (golden_pipe (obj2wrl, basic::to_string (istream)));
//
//	scene -> fromStream (uri, goldenstream);
}

static
void
golden_text (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream && istream)
{
	static const std::string X3D_XML = "<X3D ";

	// Save current state of stream.

	const auto state = istream .rdstate ();
	const auto pos   = istream .tellg ();

	// Read indentifer.

	char data [X3D_XML .size ()];

	istream .read (data, X3D_XML .size ());

	// Reset stream.

	istream .clear (state);
	istream .seekg (pos - istream .tellg (), std::ios_base::cur);

	// Test

	if (std::string (data, istream .gcount ()) == "<X3D ")
		return golden_x3d (scene, uri, std::move (istream));

	return golden_x3dv (scene, uri, std::move (istream));
}

static
void
golden_image (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream && istream)
{
	Magick::Image image;
	image .read (uri);
	image .resolutionUnits (Magick::PixelsPerInchResolution);

	const float width  = float (image .size () .width  ()) / float (image .density () .width  ()) * M_INCH;
	const float height = float (image .size () .height ()) / float (image .density () .height ()) * M_INCH;

	std::string file = os::load_file (os::find_data_file ("titania/goldengate/image.wrl"));

	Name   .GlobalReplace (get_name_from_uri (uri), &file);
	Width  .GlobalReplace (basic::to_string (width), &file);
	Height .GlobalReplace (basic::to_string (height), &file);
	URL    .GlobalReplace ("[ " + SFString (uri .basename ()) .toString () + ", " + SFString (uri .str ()) .toString () + " ]", &file);

	// Parse into scene.

	basic::ifilestream goldenstream (file);
	
	scene -> fromStream (uri, goldenstream);
}

static
void
golden_audio (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream && istream)
{
	std::string file = os::load_file (os::find_data_file ("titania/goldengate/audio.wrl"));

	Name        .GlobalReplace (get_name_from_uri (uri), &file);
	Description .GlobalReplace (SFString (uri .basename (false)) .toString (), &file);
	URL         .GlobalReplace ("[ " + SFString (uri .basename ()) .toString () + ", " + SFString (uri .str ()) .toString () + " ]", &file);

	// Parse into scene.

	basic::ifilestream goldenstream (file);
	
	scene -> fromStream (uri, goldenstream);
}

static
void
golden_video (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream && istream)
{
	MediaStream mediaStream;

	mediaStream .setup ();
	mediaStream .setUri (uri);
	mediaStream .sync ();

	std::string file = os::load_file (os::find_data_file ("titania/goldengate/video.wrl"));

	float width  = 1;
	float height = 1;

	if (mediaStream .getVideoSink () -> get_last_buffer ())
	{
		width  = mediaStream .getVideoSink () -> get_width  () / 72.0 * M_INCH;
		height = mediaStream .getVideoSink () -> get_height () / 72.0 * M_INCH;
	}

	Name        .GlobalReplace (get_name_from_uri (uri), &file);
	Description .GlobalReplace (SFString (uri .basename (false)) .toString (), &file);
	Width       .GlobalReplace (basic::to_string (width),  &file);
	Height      .GlobalReplace (basic::to_string (height), &file);
	URL         .GlobalReplace ("[ " + SFString (uri .basename ()) .toString () + ", " + SFString (uri .str ()) .toString () + " ]", &file);

	// Parse into scene.

	basic::ifilestream goldenstream (file);
	
	scene -> fromStream (uri, goldenstream);
}

void
golden_gate (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream && istream)
{
	using GoldenFunction = std::function <void (const X3DScenePtr &, const basic::uri &, basic::ifilestream &&)>;

	static const std::map <std::string, GoldenFunction> contentTypes = {
		std::make_pair ("model/vrml",      &golden_x3dv),
		std::make_pair ("x-world/x-vrml",  &golden_x3dv),
		std::make_pair ("model/x3d+vrml",  &golden_x3dv),
		std::make_pair ("model/x3d+xml",   &golden_x3d),
		std::make_pair ("application/xml", &golden_x3d),
		std::make_pair ("application/vnd.hzn-3d-crossword", &golden_x3d),
		std::make_pair ("application/ogg", &golden_video)
	};

	static const std::map <std::string, GoldenFunction> suffixes = {
		std::make_pair (".obj", &golden_obj)
	};

	try
	{
		const std::string contentType = istream .response_headers () .at ("Content-Type");
	
		//__LOG__ << contentType << " : " << uri << std::endl;

		try
		{
			return contentTypes .at (contentType) (scene, uri, std::move (istream));
		}
		catch (const std::out_of_range &)
		{
			try
			{
				return suffixes .at (uri .suffix ()) (scene, uri, std::move (istream));
			}
			catch (const std::out_of_range &)
			{
				if (Gio::content_type_is_a (contentType, "image/*"))
					return golden_image (scene, uri, std::move (istream));

				if (Gio::content_type_is_a (contentType, "audio/*"))
					return golden_audio (scene, uri, std::move (istream));

				if (Gio::content_type_is_a (contentType, "video/*"))
					return golden_video (scene, uri, std::move (istream));
			}
		}
	}
	catch (const std::out_of_range &)
	{ }

	golden_text (scene, uri, std::move (istream));
}

} // X3D
} // titania
