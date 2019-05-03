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

#include "GoldenGate.h"

#include "../Fields.h"
#include "../Browser/Sound/MediaStream.h"
#include "../Browser/X3DBrowser.h"
#include "../Parser/JSONParser.h"
#include "../Parser/XMLParser.h"
#include "../Parser/Filter.h"
#include "../Parser/Autodesk/Parser.h"
#include "../Parser/glTF/Parser.h"
#include "../Parser/PDF/Parser.h"
#include "../Parser/SVG/Parser.h"
#include "../Parser/VRML1/Parser.h"
#include "../Parser/Wavefront/Parser.h"

#include <Titania/Physics/Constants.h>
#include <Titania/InputOutput/String.h>
#include <Titania/OS/find_data_file.h>
#include <Titania/String/to_string.h>

#include <Magick++.h>
#include <giomm.h>
#include <sys/wait.h>

namespace titania {
namespace X3D {

const std::regex GoldenGate::Name        ("__NAME__");
const std::regex GoldenGate::Description ("__DESCRIPTION__");
const std::regex GoldenGate::Width       ("__WIDTH__");
const std::regex GoldenGate::Height      ("__HEIGHT__");
const std::regex GoldenGate::URL         ("__URL__");

class GoldenGate::GoldenParser
{
public:

	template <class Type>
	static
	void
	parse (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
	{
		Type (scene, uri, istream) .parseIntoScene ();
	}

};

void
GoldenGate::read (basic::ifilestream & istream, const X3DScenePtr & scene, const basic::uri & uri)
{
	try
	{
		static const auto contentTypes = getContentTypeFunctions ();
		static const auto extensions   = getExtensions ();

		try
		{
			const auto contentType = istream .response_headers () .at ("Content-Type");

			//__LOG__ << contentType << " : " << uri << std::endl;

			try
			{
				return contentTypes .at (contentType) (scene, uri, istream);
			}
			catch (const std::out_of_range &)
			{
				try
				{
					return extensions .at (uri .extension ()) (scene, uri, istream);
				}
				catch (const std::out_of_range &)
				{
					if (Gio::content_type_is_a (contentType, "image/*"))
						return image (scene, uri, istream);

					if (Gio::content_type_is_a (contentType, "audio/*"))
						return audio (scene, uri, istream);

					if (Gio::content_type_is_a (contentType, "video/*"))
						return video (scene, uri, istream);
				}
			}
		}
		catch (const std::out_of_range &)
		{ }

		text (scene, uri, istream);
	}
	catch (const X3DError & error)
	{
		throw;
	}
	catch (const std::exception & error)
	{
		throw Error <INVALID_X3D> (error .what ());
	}
	catch (...)
	{
		throw Error <INVALID_X3D> ("Unkown error while loading '" + uri .str () + "'.");
	}
}

std::vector <std::string>
GoldenGate::getContentTypes ()
{
	std::vector <std::string> contentTypes;

	for (const auto & pair : getContentTypeFunctions ())
		contentTypes .emplace_back (pair .first);

	contentTypes .emplace_back ("image/*");
	contentTypes .emplace_back ("audio/*");
	contentTypes .emplace_back ("video/*");

	return contentTypes;
}

std::map <std::string, GoldenGate::GoldenFunction>
GoldenGate::getContentTypeFunctions ()
{
	std::map <std::string, GoldenFunction> contentTypes;

	contentTypes .emplace ("text/plain",                       &text);
	contentTypes .emplace ("model/vrml",                       &vrml);
	contentTypes .emplace ("x-world/x-vrml",                   &vrml);
	contentTypes .emplace ("model/x3d+vrml",                   &vrml);
	contentTypes .emplace ("model/x3d+xml",                    &GoldenParser::parse <XMLParser>);
	contentTypes .emplace ("application/xml",                  &GoldenParser::parse <XMLParser>);
	contentTypes .emplace ("application/vnd.hzn-3d-crossword", &GoldenParser::parse <XMLParser>);
	contentTypes .emplace ("model/x3d+json",                   &GoldenParser::parse <JSONParser>);
	contentTypes .emplace ("model/gltf+json",                  &GoldenParser::parse <glTF::Parser>);
	contentTypes .emplace ("application/x-3ds",                &GoldenParser::parse <Autodesk::Parser>);
	contentTypes .emplace ("image/x-3ds",                      &GoldenParser::parse <Autodesk::Parser>);

	if (Glib::find_program_in_path ("inkscape") .size ())
	{
		contentTypes .emplace ("application/pdf",     &GoldenParser::parse <PDF::Parser>);
		contentTypes .emplace ("application/x-pdf",   &GoldenParser::parse <PDF::Parser>);
		contentTypes .emplace ("application/x-gzpdf", &GoldenParser::parse <PDF::Parser>);
	}

	contentTypes .emplace ("image/svg+xml",   &GoldenParser::parse <SVG::Parser>);
	contentTypes .emplace ("application/ogg", &video);

	return contentTypes;
}

std::map <std::string, GoldenGate::GoldenFunction>
GoldenGate::getExtensions ()
{
	std::map <std::string, GoldenFunction> extensions;

	// VRML
	extensions .emplace (".wrl",    &vrml);
	extensions .emplace (".wrz",    &vrml);
	extensions .emplace (".wrl.gz", &vrml); /// Todo: does not work with URI::extension
	extensions .emplace (".vrml",   &vrml);
	extensions .emplace (".vrm",    &vrml);

	// X3D Vrml Classic Encoding
	extensions .emplace (".x3dv",    &vrml);
	extensions .emplace (".x3dvz",   &vrml);
	extensions .emplace (".x3dv.gz", &vrml); /// Todo: does not work with URI::extension

	// X3D XML Encoding
	extensions .emplace (".x3d",    &GoldenParser::parse <XMLParser>);
	extensions .emplace (".x3dz",   &GoldenParser::parse <XMLParser>);
	extensions .emplace (".x3d.gz", &GoldenParser::parse <XMLParser>); /// Todo: does not work with URI::extension
	extensions .emplace (".xml",    &GoldenParser::parse <XMLParser>);

	// X3D XML Encoding
	extensions .emplace (".x3dj", &GoldenParser::parse <JSONParser>);

	// GLTF
	extensions .emplace (".gltf", &GoldenParser::parse <glTF::Parser>);

	// Autodesk 3DS Max
	extensions .emplace (".3ds", &GoldenParser::parse <Autodesk::Parser>);

	// Wavefront OBJ
	extensions .emplace (".obj", &GoldenParser::parse <Wavefront::Parser>);

	if (Glib::find_program_in_path ("inkscape") .size ())
	{
		// PDF
		extensions .emplace (".pdf", &GoldenParser::parse <PDF::Parser>);
	}

	// SVG
	extensions .emplace (".svg",  &GoldenParser::parse <SVG::Parser>);
	extensions .emplace (".svgz", &GoldenParser::parse <SVG::Parser>);

	return extensions;
}

void
GoldenGate::vrml (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	if (isVRML1 (istream))
		VRML1::Parser (scene, uri, istream) .parseIntoScene ();

	else
		scene -> fromStream (uri, istream);
}

void
GoldenGate::text (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	// Text

	std::string errorString;

	basic::ifilestream::iostate  state;
	basic::ifilestream::pos_type pos;

	state = istream .rdstate ();
	pos   = istream .tellg ();

	try
	{
		return vrml (scene, uri, istream);
	}
	catch (const X3DError & error)
	{
		errorString += error .what ();
		errorString += '\n';
	}

	istream .clear (state);

	for (size_t i = 0, size = istream .tellg () - pos; i < size; ++ i)
		istream .unget ();

	state = istream .rdstate ();
	pos   = istream .tellg ();

	try
	{
		return GoldenParser::parse <XMLParser> (scene, uri, istream);
	}
	catch (const X3DError & error)
	{
		errorString += error .what ();
		errorString += '\n';
	}

	istream .clear (state);

	for (size_t i = 0, size = istream .tellg () - pos; i < size; ++ i)
		istream .unget ();

	try
	{
		return GoldenParser::parse <JSONParser> (scene, uri, istream);
	}
	catch (const X3DError & error)
	{
		errorString += error .what ();
		errorString += '\n';
	}

	throw Error <INVALID_X3D> (errorString);
}

void
GoldenGate::image (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	Magick::Image image;
	image .read (uri);
	image .resolutionUnits (Magick::PixelsPerInchResolution);

	const auto width  = double (image .size () .width  ()) / double (image .density () .width  ()) * inch <double>;
	const auto height = double (image .size () .height ()) / double (image .density () .height ()) * inch <double>;

	std::string file = Glib::file_get_contents (os::find_data_file ("titania/goldengate/image.x3dv"));

	file = std::regex_replace (file, Name,   GetNameFromURI (uri));
	file = std::regex_replace (file, Width,  basic::to_string (width,  std::locale::classic ()));
	file = std::regex_replace (file, Height, basic::to_string (height, std::locale::classic ()));
	file = std::regex_replace (file, URL,    "[ " + SFString (uri .escape () .basename ()) .toString () + ", " + SFString (uri .escape () .str ()) .toString () + " ]");

	// Parse into scene.

	basic::ifilestream goldenstream (file);

	scene -> fromStream (uri, goldenstream);
}

void
GoldenGate::audio (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	std::string file = Glib::file_get_contents (os::find_data_file ("titania/goldengate/audio.x3dv"));

	file = std::regex_replace (file, Name,        GetNameFromURI (uri));
	file = std::regex_replace (file, Description, SFString (uri .stem ()) .toString ());
	file = std::regex_replace (file, URL,         "[ " + SFString (uri .escape ()  .basename ()) .toString () + ", " + SFString (uri .escape ()  .str ()) .toString () + " ]");

	// Parse into scene.

	basic::ifilestream goldenstream (file);

	scene -> fromStream (uri, goldenstream);
}

void
GoldenGate::video (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	MediaStream mediaStream (true);

	mediaStream .setup ();
	mediaStream .setUri (uri);
	mediaStream .sync ();

	std::string file = Glib::file_get_contents (os::find_data_file ("titania/goldengate/video.x3dv"));

	const auto width  = mediaStream .getWidth  () ? mediaStream .getWidth  () / 72.0 * inch <double> : 1.0;
	const auto height = mediaStream .getHeight () ? mediaStream .getHeight () / 72.0 * inch <double> : 1.0;

	file = std::regex_replace (file, Name,        GetNameFromURI (uri));
	file = std::regex_replace (file, Description, SFString (uri .stem ()) .toString ());
	file = std::regex_replace (file, Width,       basic::to_string (width,  std::locale::classic ()));
	file = std::regex_replace (file, Height,      basic::to_string (height, std::locale::classic ()));
	file = std::regex_replace (file, URL,         "[ " + SFString (uri .escape ()  .basename ()) .toString () + ", " + SFString (uri .escape ()  .str ()) .toString () + " ]");

	// Parse into scene.

	basic::ifilestream goldenstream (file);

	scene -> fromStream (uri, goldenstream);
}

bool
GoldenGate::isVRML1 (std::istream & istream)
{
	static const io::string VRML1_0 ("#VRML V1.0");
	static const io::string VRML1_1 ("#VRML V1.1");

	return VRML1_0 .lookahead (istream) or VRML1_1 .lookahead (istream);
}

} // X3D
} // titania
