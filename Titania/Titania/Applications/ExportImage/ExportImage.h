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

#ifndef __TITANIA_COMMANDS_EXPORT_IMAGE_H__
#define __TITANIA_COMMANDS_EXPORT_IMAGE_H__

#include "../../Applications/main/CommandOptions.h"

#include <Titania/X3D.h>
#include <Titania/X3D/Execution/World.h>
#include <Titania/X3D/InputOutput/FileGenerator.h>

#include <gtkmm/main.h>

namespace titania {
namespace puck {

class ExportImage :
	public X3D::X3DInput
{
public:

	void
	set_loadCount (X3D::X3DBrowser* const browser)
	{
		std::clog << "*** Loading " << browser -> getLoadCount () << " files." << std::endl;
	}

	void
	set_snapshot (X3D::X3DBrowser* const browser, const CommandOptions & options, const basic::uri & outputFilename)
	{
		// Constrain options.

		size_t width        = options .width;
		size_t height       = options .height;
		size_t antialiasing = options .antialiasing;

		if (width > browser -> getMaxRenderBufferSize ())
		{
			width = browser -> getMaxRenderBufferSize ();

			std::clog << "*** Image width to high, using max width of " << browser -> getMaxRenderBufferSize () << " pixels." << std::endl;
		}

		if (height > browser -> getMaxRenderBufferSize ())
		{
			height = browser -> getMaxRenderBufferSize ();

			std::clog << "*** Image height to high, using max height of " << browser -> getMaxRenderBufferSize () << " pixels." << std::endl;
		}

		if (antialiasing > browser -> getMaxSamples ())
		{
			antialiasing = browser -> getMaxSamples ();

			std::clog << "*** Antialiasing samples to high, using max antialiasing of " << browser -> getMaxSamples () << " samples." << std::endl;
		}

		try
		{
			// Make snapshot and save image.
	
			auto image = browser -> getSnapshot (width,
			                                     height,
			                                     options .alphaChannel,
			                                     antialiasing);
	
			image .quality (100);
			image .write (outputFilename .path ());
		}
		catch (const std::exception & error)
		{
			std::clog << "*** Couldn't save image!" << std::endl;
			std::clog << error .what () << std::endl;
		}

		Gtk::Main::quit ();
	}

	int
	main (const CommandOptions & options)
	{
      Gtk::Main kit (0, nullptr);

 		if (options .filenames .empty ())
			throw std::runtime_error ("Expected a filename.");

		basic::uri inputFilename (options .filenames .front ());
		basic::uri outputFilename (options .imageFilename);

		if (inputFilename .is_relative ())
			inputFilename = basic::uri ("file://" + basic::path (Glib::get_current_dir ()) .escape () + "/") .transform (inputFilename);

		if (outputFilename .is_relative ())
			outputFilename = basic::uri ("file://" + basic::path (Glib::get_current_dir ()) .escape () + "/") .transform (outputFilename);

		const auto browser = X3D::createBrowser ({ inputFilename .str () });
		//browser -> set_size_request (options .width, options. height);
		browser -> setup ();

		browser -> getLoadCount () .addInterest (&ExportImage::set_loadCount, this, browser .getValue ());
		browser -> initialized ()  .addInterest (&ExportImage::set_snapshot, this, browser .getValue (), std::ref (options), outputFilename);

		kit .run ();
		return 0;
	}
};

} // puck
} // titania

#endif
