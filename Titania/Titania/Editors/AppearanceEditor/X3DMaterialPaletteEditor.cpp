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

#include <Titania/OS.h>
#include <Titania/X3D/Components/Geometry3D/Sphere.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Components/PointingDeviceSensor/TouchSensor.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Shape.h>

#include <Titania/Backtrace.h>

namespace titania {
namespace puck {

static constexpr size_t COLUMNS   = 6;
static constexpr size_t ROWS      = 6;
static constexpr size_t PAGE_SIZE = COLUMNS * ROWS;
static constexpr double DISTANCE  = 2.5;

X3DMaterialPaletteEditor::X3DMaterialPaletteEditor () :
	X3DAppearanceEditorInterface (),
	                     preview (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser (), { get_ui ("Editors/Palette.x3dv") })),
	                       group (),
	                     folders (),
	                       files (),
	          numDefaultPalettes (0),
	               frontMaterial (true),
	                        over (false),
	               materialIndex (-1)
{
	addChildren (preview, group);
}

void
X3DMaterialPaletteEditor::configure ()
{
	getPaletteFaceCombo () .set_active (getConfig () -> getInteger ("paletteFace"));
}

void
X3DMaterialPaletteEditor::initialize ()
{
	// Show browser.

	preview -> initialized () .addInterest (this, &X3DMaterialPaletteEditor::set_browser);
	preview -> setAntialiasing (4);
	preview -> show ();

	getPalettePreviewBox () .pack_start (*preview, true, true, 0);
}

void
X3DMaterialPaletteEditor::set_browser ()
{
	try
	{
		preview -> initialized () .removeInterest (this, &X3DMaterialPaletteEditor::set_browser);
	
		group = preview -> getExecutionContext () -> getScene () -> getExportedNode <X3D::Group> ("Items");
	
		refreshPalette ();
	
		const size_t paletteIndex = getConfig () -> getInteger ("palette");
	
		if (paletteIndex < folders .size ())
			getPaletteComboBoxText () .set_active (paletteIndex);
		else
			getPaletteComboBoxText () .set_active (0);
	}
	catch (const X3D::X3DError &)
	{
		disable ();
	}
}

void
X3DMaterialPaletteEditor::refreshPalette ()
{
	try
	{
		// Find materials in folders.
	
		folders .clear ();
		getPaletteComboBoxText () .remove_all ();

		addLibrary (find_data_file ("Library/Materials"));

		numDefaultPalettes = folders .size ();

		addLibrary (config_dir ("Materials"));
	
		if (folders .empty ())
			disable ();
		else
			enable ();
	}
	catch (...)
	{
		disable ();
	}
}

void
X3DMaterialPaletteEditor::addLibrary (const std::string & libraryPath)
{
	try
	{
		const auto folder = Gio::File::create_for_path (libraryPath);

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
}

void
X3DMaterialPaletteEditor::setCurrentFolder (const size_t paletteIndex)
{
	const bool customPalette = paletteIndex >= numDefaultPalettes;

	getConfig () -> setItem ("palette", (int) paletteIndex);

	getPalettePreviousButton () .set_sensitive (paletteIndex > 0);
	getPaletteNextButton ()     .set_sensitive (paletteIndex + 1 < folders .size ());

	getRemovePaletteMenuItem () .set_sensitive (customPalette);
	getEditPaletteMenuItem ()   .set_sensitive (customPalette);

	getRemoveMaterialMenuItem () .set_sensitive (false);

	try
	{
		files .clear ();

		group -> children () .clear ();

		const auto folder = Gio::File::create_for_uri (folders .at (paletteIndex));

		for (const auto & fileInfo : LibraryView::getChildren (folder))
		{
			if (fileInfo -> get_file_type () == Gio::FILE_TYPE_REGULAR)
				addMaterial (Glib::uri_unescape_string (folder -> get_child (fileInfo -> get_name ()) -> get_uri ()));

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
X3DMaterialPaletteEditor::addMaterial (const std::string & uri)
{
	const auto i      = files .size ();
	const int  column = i % COLUMNS;
	const int  row    = i / COLUMNS;

	const auto inlineNode  = preview -> getExecutionContext () -> createNode <X3D::Inline> ();
	const auto touchSensor = preview -> getExecutionContext () -> createNode <X3D::TouchSensor> ();
	const auto transform   = preview -> getExecutionContext () -> createNode <X3D::Transform> ();

	touchSensor -> isOver ()    .addInterest (this, &X3DMaterialPaletteEditor::set_over, std::cref (touchSensor -> isOver ()), i);
	touchSensor -> touchTime () .addInterest (this, &X3DMaterialPaletteEditor::set_touchTime, i);

	inlineNode -> url ()        = { uri };
	transform -> translation () = X3D::Vector3f (column * DISTANCE, -row * DISTANCE, 0);
	transform -> children ()    = { inlineNode, touchSensor };

	group -> children () .emplace_back (transform);
	preview -> getExecutionContext () -> realize ();

	files .emplace_back (uri);

	//

	const size_t paletteIndex  = getPaletteComboBoxText () .get_active_row_number ();
	const bool   customPalette = paletteIndex >= numDefaultPalettes;

	getAddMaterialMenuItem () .set_sensitive (customPalette and files .size () < PAGE_SIZE);
}

void
X3DMaterialPaletteEditor::enable ()
{
	getPaletteBox () .set_sensitive (true);
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
X3DMaterialPaletteEditor::set_over (const bool value, const size_t i)
{
	over          = value;
	materialIndex = i;
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

bool
X3DMaterialPaletteEditor::on_palette_button_press_event (GdkEventButton* event)
{
	if (event -> button not_eq 3)
		return false;

	const size_t paletteIndex  = getPaletteComboBoxText () .get_active_row_number ();
	const bool   customPalette = paletteIndex >= numDefaultPalettes;

	getRemoveMaterialMenuItem () .set_sensitive (customPalette and over);

	getPaletteMenu () .popup (event -> button, event -> time);
	return true;
}

void
X3DMaterialPaletteEditor::on_add_palette_activate ()
{
	getPaletteNameEntry () .set_text ("");

	getEditPaletteDialog () .set_title (_ ("Add New Palette"));
	getEditPaletteDialog () .present ();
}

void
X3DMaterialPaletteEditor::on_remove_palette_activate ()
{
	try
	{
		const auto paletteIndex = getPaletteComboBoxText () .get_active_row_number ();
		const auto folder       = Gio::File::create_for_uri (folders .at (paletteIndex));

		for (const auto & fileInfo : LibraryView::getChildren (folder))
		{
			if (fileInfo -> get_file_type () == Gio::FILE_TYPE_REGULAR)
				folder -> get_child (fileInfo -> get_name ()) -> remove ();
		}

		folder -> remove ();

		refreshPalette ();

		getPaletteComboBoxText () .set_active (std::min <size_t> (folders .size () - 1, paletteIndex));
	}
	catch (...)
	{ }
}

void
X3DMaterialPaletteEditor::on_edit_palette_activate ()
{
	__LOG__ << std::endl;
}

void
X3DMaterialPaletteEditor::on_edit_palette_ok_clicked ()
{
	getEditPaletteDialog () .hide ();

	const std::string paletteName = getPaletteNameEntry () .get_text ();
	const auto        folder      = Gio::File::create_for_path (config_dir ("Materials/" + paletteName));

	folder -> make_directory_with_parents ();

	refreshPalette ();

	const auto iter = std::find (folders .begin () + numDefaultPalettes, folders .end (), folder -> get_uri ());

	if (iter == folders .end ())
		return; // Should never happen.

	getPaletteComboBoxText () .set_active (iter - folders .begin ());
}

void
X3DMaterialPaletteEditor::on_edit_palette_cancel_clicked ()
{
	getEditPaletteDialog () .hide ();
}

void
X3DMaterialPaletteEditor::on_palette_name_insert_text (const Glib::ustring & text, int* position)
{
	validateFolderOnInsert (getPaletteNameEntry (), text, *position);
}

void
X3DMaterialPaletteEditor::on_palette_name_delete_text (int start_pos, int end_pos)
{
	validateFolderOnDelete (getPaletteNameEntry (), start_pos, end_pos);
}

void
X3DMaterialPaletteEditor::on_palette_name_changed ()
{
	const std::string paletteName = getPaletteNameEntry () .get_text ();

	getEditPaletteOkButton () .set_sensitive (not paletteName .empty () /*and not exists paletteName*/);
}

void
X3DMaterialPaletteEditor::on_add_material_activate ()
{
	try
	{
		const auto scene      = getCurrentBrowser () -> createScene ();
		const auto transform  = scene -> createNode <X3D::Transform> ();
		const auto shape      = scene -> createNode <X3D::Shape> ();
		const auto appearance = scene -> createNode <X3D::Appearance> ();
		const auto sphere     = scene -> createNode <X3D::Sphere> ();
	
		scene -> getRootNodes ()  = { transform };
		transform -> children ()  = { shape };
		shape -> appearance ()    = appearance;
		shape -> geometry ()      = sphere;
		appearance -> material () = getMaterial () -> copy (scene, X3D::FLAT_COPY);
	
		scene -> setup ();
		scene -> addStandardMetaData ();
		scene -> setMetaData ("titania magic", "Material");

		std::ostringstream osstream;

		osstream << X3D::NicestStyle << scene << std::endl;

		const auto paletteIndex = getPaletteComboBoxText () .get_active_row_number ();
		const auto folder       = Gio::File::create_for_uri (folders .at (paletteIndex));
		const auto number       = basic::to_string (files .size () + 1, std::locale::classic ());
		const auto file         = folder -> get_child (folder -> get_basename () + number + ".x3dv");

		std::string etag;

		file -> replace_contents (osstream .str (), "", etag);

		addMaterial (Glib::uri_unescape_string (file -> get_uri ()));
	}
	catch (...)
	{ }
}

void
X3DMaterialPaletteEditor::on_remove_material_activate ()
{
	if (materialIndex < files .size ())
		Gio::File::create_for_uri (files [materialIndex]) -> remove ();

	for (size_t i = materialIndex + 1, size = files .size (); i < size; ++ i)
	{
		const auto file = Gio::File::create_for_uri (files [i]);
		
		file -> move (Gio::File::create_for_uri (files [i - 1]), Gio::FILE_COPY_OVERWRITE);
	}

	setCurrentFolder (getPaletteComboBoxText () .get_active_row_number ());
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
