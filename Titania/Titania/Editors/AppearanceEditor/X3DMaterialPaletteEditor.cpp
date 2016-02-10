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

#include "X3DMaterialPaletteEditor.h"

#include "../../Browser/MagicImport.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Widgets/LibraryView/LibraryView.h"

#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Components/PointingDeviceSensor/TouchSensor.h>

namespace titania {
namespace puck {

static constexpr size_t COLUMNS   = 6;
static constexpr size_t ROWS      = 6;
static constexpr size_t PAGE_SIZE = COLUMNS * ROWS;
static constexpr double DISTANCE  = 2.5;

X3DMaterialPaletteEditor::X3DMaterialPaletteEditor () :
	X3DAppearanceEditorInterface (),
	                     preview (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser ())),
	                     folders (),
	                       files (),
	               frontMaterial (true)
{
	preview -> setAntialiasing (4);
}

void
X3DMaterialPaletteEditor::configure ()
{
	getPaletteFaceCombo () .set_active (getConfig () -> getInteger ("paletteFace"));
}

void
X3DMaterialPaletteEditor::initialize ()
{
	// Find material folders.

	try
	{
		const auto folder = Gio::File::create_for_path (find_data_file ("Library/Materials"));

		for (const auto & fileInfo : LibraryView::getChildren (folder))
		{
			if (fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY)
			{
				folders .emplace_back (folder -> get_child (fileInfo -> get_name ()) -> get_uri ());
				getPaletteComboBoxText () .append (fileInfo -> get_name ());
			}
		}
	}
	catch (...)
	{ }

	// Show browser.

	getPalettePreviewBox () .pack_start (*preview, true, true, 0);

	preview -> show ();
	preview -> initialized () .addInterest (this, &X3DMaterialPaletteEditor::set_browser);
}

void
X3DMaterialPaletteEditor::set_browser ()
{
	preview -> initialized () .removeInterest (this, &X3DMaterialPaletteEditor::set_browser);

	if (folders .empty ())
	{
		disable ();
		return;
	}

	const size_t index = getConfig () -> getInteger ("palette");

	if (index < folders .size ())
		getPaletteComboBoxText () .set_active (index);
	else
		getPaletteComboBoxText () .set_active (0);
}

void
X3DMaterialPaletteEditor::setCurrentFolder (const size_t index)
{
	preview -> initialized () .removeInterest (this, &X3DMaterialPaletteEditor::set_initialized);
	preview -> initialized () .addInterest (this, &X3DMaterialPaletteEditor::set_initialized, index);

	preview -> loadURL ({ get_ui ("Editors/Palette.x3dv") }, { });
}

void
X3DMaterialPaletteEditor::set_initialized (const size_t index)
{
	getConfig () -> setItem ("palette", (int) index);

	getPalettePreviousButton () .set_sensitive (index > 0);
	getPaletteNextButton ()     .set_sensitive (index + 1 < folders .size ());

	try
	{
		files .clear ();

		const auto folder = Gio::File::create_for_uri (folders .at (index));

		for (const auto & fileInfo : LibraryView::getChildren (folder))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_REGULAR       :
				case Gio::FILE_TYPE_SYMBOLIC_LINK :
					{
						const std::string uri = Glib::uri_unescape_string (folder -> get_child (fileInfo -> get_name ()) -> get_uri ());
						addMaterial (files .size (), uri);
						files .emplace_back (uri);
						break;
					}
				default:
					break;
			}

			if (files .size () < PAGE_SIZE)
				continue;

			break;
		}
	}
	catch (...)
	{
		disable ();
	}
}

void
X3DMaterialPaletteEditor::addMaterial (const size_t i, const std::string & uri)
{
	const int column = i % COLUMNS;
	const int row    = i / COLUMNS;

	const auto inlineNode  = preview -> getExecutionContext () -> createNode <X3D::Inline> ();
	const auto touchSensor = preview -> getExecutionContext () -> createNode <X3D::TouchSensor> ();
	const auto transform   = preview -> getExecutionContext () -> createNode <X3D::Transform> ();

	touchSensor -> touchTime () .addInterest (this, &X3DMaterialPaletteEditor::set_touchTime, i);

	inlineNode -> url ()        = { uri };
	transform -> translation () = X3D::Vector3f (column * DISTANCE, -row * DISTANCE, 0);
	transform -> children ()    = { inlineNode, touchSensor };

	preview -> getExecutionContext () -> getRootNodes () .emplace_back (transform);
	preview -> getExecutionContext () -> realize ();
}

void
X3DMaterialPaletteEditor::disable ()
{
	getPaletteBox () .set_sensitive (false);
}

void
X3DMaterialPaletteEditor::on_palette_face_changed ()
{
	frontMaterial = getPaletteFaceCombo () .get_active_row_number () == 0;
}

void
X3DMaterialPaletteEditor::on_palette_previous_clicked ()
{
	getPaletteComboBoxText () .set_active (getPaletteComboBoxText () .get_active_row_number () - 1);
}

void
X3DMaterialPaletteEditor::on_palette_next_clicked ()
{
	getPaletteComboBoxText () .set_active (getPaletteComboBoxText () .get_active_row_number () + 1);
}

void
X3DMaterialPaletteEditor::on_palette_changed ()
{
	setCurrentFolder (getPaletteComboBoxText () .get_active_row_number ());
}

void
X3DMaterialPaletteEditor::set_touchTime (const size_t i)
{
	try
	{
		auto selection = getBrowserWindow () -> getSelection () -> getChildren ();

		if (selection .empty ())
			return;

		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Apply Material From Palette"));
		const auto scene    = getCurrentBrowser () -> createX3DFromURL ({ files [i] });

		MagicImport magicImport (getBrowserWindow ());

		magicImport .setFrontMaterial (frontMaterial);

		if (magicImport .import (getCurrentContext (), selection, scene, undoStep))
			getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DMaterialPaletteEditor::store ()
{
	getConfig () -> setItem ("paletteFace", getPaletteFaceCombo () .get_active_row_number ());
}

X3DMaterialPaletteEditor::~X3DMaterialPaletteEditor ()
{ }

} // puck
} // titania
