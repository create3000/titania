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

#ifndef __TITANIA_EDITORS_PALETTE_EDITOR_X3DPALETTE_EDITOR_H__
#define __TITANIA_EDITORS_PALETTE_EDITOR_X3DPALETTE_EDITOR_H__

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"
#include "../../Editors/LibraryView/X3DLibraryView.h"

#include <Titania/X3D.h>
#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/PointingDeviceSensor/TouchSensor.h>

#include <Titania/String/to_string.h>

namespace titania {
namespace puck {

static constexpr size_t COLUMNS   = 6;
static constexpr size_t ROWS      = 6;
static constexpr size_t PAGE_SIZE = COLUMNS * ROWS;
static constexpr double DISTANCE  = 2.5;

template <class Type>
class X3DPaletteEditor:
	virtual public Type
{
public:

	///  @name Destruction

	virtual
	~X3DPaletteEditor () override;


protected:

	///  @name Construction

	X3DPaletteEditor (const std::string &);

	virtual
	void
	initialize () override;

	///  @name Virtual functions

	virtual
	void
	addObject (const std::string &) = 0;

	virtual
	void
	setTouchTime (const std::string &) = 0;

	virtual
	bool
	createScene (const X3D::X3DScenePtr &) = 0;

	///  @name Operations

	void
	addObject (const std::string &, const X3D::X3DPtr <X3D::Transform> &);

	///  @name Member access

	const X3D::BrowserPtr &
	getPreview () const
	{ return preview; }


private:

	///  @name Construction

	void
	set_browser ();

	///  @name Operations

	bool
	checkSelection () const;

	void
	refreshPalette ();

	void
	addLibrary (const std::string &, const bool);

	void
	setCurrentFolder (const size_t);

	X3D::Vector3f
	getPosition (const size_t) const;

	void
	setSelection (const size_t);

	void
	enable ();

	///  @name Event handlers

	void
	set_over (const bool, const size_t);

	void
	set_touchTime (const size_t);

	virtual
	void
	on_palette_previous_clicked () final override;
	
	virtual
	void
	on_palette_next_clicked () final override;
	
	virtual
	void
	on_palette_changed () final override;
	
	virtual
	bool
	on_palette_button_press_event (GdkEventButton*) final override;
	
	virtual
	void
	on_add_palette_activate () final override;
	
	virtual
	void
	on_remove_palette_activate () final override;
	
	virtual
	void
	on_edit_palette_activate () final override;
	
	virtual
	void
	on_edit_palette_ok_clicked () final override;
	
	virtual
	void
	on_edit_palette_cancel_clicked () final override;

	virtual
	void
	on_palette_name_insert_text (const Glib::ustring &, int*) final override;
	
	virtual
	void
	on_palette_name_delete_text (int, int) final override;
	
	virtual
	void
	on_palette_name_changed () final override;
	
	virtual
	void
	on_add_object_to_palette_activate () final override;

	virtual
	void
	on_remove_object_from_palette_activate () final override;

	///  @name Members

	X3D::BrowserPtr              preview;
	X3D::X3DPtr <X3D::Group>     group;
	X3D::X3DPtr <X3D::Switch>    selectionSwitch;
	X3D::X3DPtr <X3D::Transform> selectionRectangle;
	const std::string            libraryFolder;
	std::vector <std::string>    folders;
	std::vector <std::string>    files;
	size_t                       numDefaultPalettes;
	bool                         over;
	size_t                       overIndex;
	size_t                       selectedIndex;

};

template <class Type>
X3DPaletteEditor <Type>::X3DPaletteEditor (const std::string & libraryFolder) :
	              Type (),
	           preview (X3D::createBrowser (this -> getBrowserWindow () -> getMasterBrowser (), { get_ui ("Editors/Palette.x3dv") })),
	             group (),
	   selectionSwitch (),
	selectionRectangle (),
	     libraryFolder (libraryFolder),
	           folders (),
	             files (),
	numDefaultPalettes (0),
	              over (false),
	         overIndex (-1),
	     selectedIndex (-1)
{
	this -> addChildren (preview, group, selectionSwitch, selectionRectangle);
}

template <class Type>
void
X3DPaletteEditor <Type>::initialize ()
{
	// Show browser.

	preview -> initialized () .addInterest (this, &X3DPaletteEditor::set_browser);
	preview -> setAntialiasing (4);
	preview -> show ();

	this -> getPalettePreviewBox () .pack_start (*preview, true, true, 0);
}

template <class Type>
void
X3DPaletteEditor <Type>::set_browser ()
{
	try
	{
		// Disconnect.

		preview -> initialized () .removeInterest (this, &X3DPaletteEditor::set_browser);
	
		// Get exported nodes.

		const auto scene = preview -> getExecutionContext () -> getScene ();

		group              = scene -> getExportedNode <X3D::Group>     ("Items");
		selectionSwitch    = scene -> getExportedNode <X3D::Switch>    ("SelectionSwitch");
		selectionRectangle = scene -> getExportedNode <X3D::Transform> ("SelectionRectangle");
	
		// Refresh palette.

		refreshPalette ();
	
		const size_t paletteIndex = this -> getConfig () -> getInteger ("palette");
	
		if (paletteIndex < folders .size ())
			this -> getPaletteComboBoxText () .set_active (paletteIndex);
		else
			this -> getPaletteComboBoxText () .set_active (0);
	}
	catch (const X3D::X3DError &)
	{
		this -> getPaletteBox () .set_sensitive (false);
	}
}

template <class Type>
bool
X3DPaletteEditor <Type>::checkSelection () const
{
	return not this -> getBrowserWindow () -> getSelection () -> getChildren () .empty ();
}

template <class Type>
void
X3DPaletteEditor <Type>::refreshPalette ()
{
	// Find materials in folders.

	folders .clear ();

	this -> getPaletteComboBoxText () .remove_all ();

	addLibrary (find_data_file ("Library/" + libraryFolder), false);

	numDefaultPalettes = folders .size ();

	addLibrary (config_dir (libraryFolder), true);

	enable ();
}

template <class Type>
void
X3DPaletteEditor <Type>::addLibrary (const std::string & libraryPath, const bool includeEmptyFolders)
{
	try
	{
		const auto folder = Gio::File::create_for_path (libraryPath);

		for (const auto & fileInfo : X3DLibraryView::getChildren (folder))
		{
			if (fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY)
			{
				const auto child = folder -> get_child (fileInfo -> get_name ());
				const auto uri   = child -> get_uri ();

				if (not includeEmptyFolders and not X3DLibraryView::containsFiles (child))
					continue;

				folders .emplace_back (uri);
				this -> getPaletteComboBoxText () .append (fileInfo -> get_name ());
			}
		}
	}
	catch (...)
	{ }
}

template <class Type>
void
X3DPaletteEditor <Type>::setCurrentFolder (const size_t paletteIndex)
{
	this -> getConfig () -> setItem ("palette", (int) paletteIndex);

	this -> getPalettePreviousButton () .set_sensitive (paletteIndex > 0);
	this -> getPaletteNextButton ()     .set_sensitive (paletteIndex + 1 < folders .size ());

	setSelection (-1);

	try
	{
		files .clear ();

		group -> children () .clear ();

		const auto folder = Gio::File::create_for_uri (folders .at (paletteIndex));

		for (const auto & fileInfo : X3DLibraryView::getChildren (folder))
		{
			if (fileInfo -> get_file_type () == Gio::FILE_TYPE_REGULAR)
				addObject (Glib::uri_unescape_string (folder -> get_child (fileInfo -> get_name ()) -> get_uri ()));

			if (files .size () < PAGE_SIZE)
				continue;

			break;
		}

		enable ();
	}
	catch (...)
	{ }
}

template <class Type>
void
X3DPaletteEditor <Type>::addObject (const std::string & uri, const X3D::X3DPtr <X3D::Transform> & transform)
{
	const auto i = files .size ();

	const auto touchSensor = preview -> getExecutionContext () -> createNode <X3D::TouchSensor> ();

	touchSensor -> isOver ()    .addInterest (this, &X3DPaletteEditor::set_over, std::cref (touchSensor -> isOver ()), i);
	touchSensor -> touchTime () .addInterest (this, &X3DPaletteEditor::set_touchTime, i);

	transform -> translation () = getPosition (i);

	transform -> children () .emplace_back (touchSensor);

	group -> children () .emplace_back (transform);
	preview -> getExecutionContext () -> realize ();

	files .emplace_back (uri);
}

template <class Type>
X3D::Vector3f
X3DPaletteEditor <Type>::getPosition (const size_t i) const
{
	const int column = i % COLUMNS;
	const int row    = i / COLUMNS;

	return X3D::Vector3f (column * DISTANCE, -row * DISTANCE, 0);
}

template <class Type>
void
X3DPaletteEditor <Type>::setSelection (const size_t i)
{
	selectedIndex = i;

	if (i < PAGE_SIZE)
	{
		selectionSwitch -> whichChoice ()    = true;
		selectionRectangle -> translation () = getPosition (i);
	}
	else
		selectionSwitch -> whichChoice () = false;
}

template <class Type>
void
X3DPaletteEditor <Type>::enable ()
{
	this -> getChangePaletteBox () .set_sensitive (folders .size ());
}

template <class Type>
void
X3DPaletteEditor <Type>::set_over (const bool value, const size_t i)
{
	over      = value;
	overIndex = i;
}

template <class Type>
void
X3DPaletteEditor <Type>::set_touchTime (const size_t i)
{
	// Display and place selection rectangle.

	setSelection (i);

	// Do something with selection.

	setTouchTime (files [i]);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_palette_previous_clicked ()
{
	this -> getPaletteComboBoxText () .set_active (this -> getPaletteComboBoxText () .get_active_row_number () - 1);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_palette_next_clicked ()
{
	this -> getPaletteComboBoxText () .set_active (this -> getPaletteComboBoxText () .get_active_row_number () + 1);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_palette_changed ()
{
	setCurrentFolder (this -> getPaletteComboBoxText () .get_active_row_number ());
}

template <class Type>
bool
X3DPaletteEditor <Type>::on_palette_button_press_event (GdkEventButton* event)
{
	if (event -> button not_eq 3)
		return false;

	// Display and place selection rectangle.

	if (over)
		setSelection (overIndex);

	// Display menu.

	const size_t paletteIndex  = this -> getPaletteComboBoxText () .get_active_row_number ();
	const bool   customPalette = paletteIndex >= numDefaultPalettes and folders .size ();

	this -> getRemovePaletteMenuItem () .set_sensitive (customPalette);
	this -> getEditPaletteMenuItem ()   .set_sensitive (customPalette);

	this -> getAddObjectToPaletteMenuItem ()      .set_sensitive (customPalette and files .size () < PAGE_SIZE and checkSelection ());
	this -> getRemoveObjectFromPaletteMenuItem () .set_sensitive (customPalette and selectedIndex < PAGE_SIZE and files .size ());

	this -> getPaletteMenu () .popup (event -> button, event -> time);
	return true;
}

template <class Type>
void
X3DPaletteEditor <Type>::on_add_palette_activate ()
{
	this -> getPaletteNameEntry () .set_text ("");

	this -> getEditPaletteDialog () .set_title (_ ("Add New Palette"));
	this -> getEditPaletteDialog () .present ();
}

template <class Type>
void
X3DPaletteEditor <Type>::on_remove_palette_activate ()
{
	try
	{
		const auto paletteIndex = this -> getPaletteComboBoxText () .get_active_row_number ();
		const auto folder       = Gio::File::create_for_uri (folders .at (paletteIndex));
		const auto dialog       = std::dynamic_pointer_cast <MessageDialog> (this -> addDialog ("MessageDialog", false));
	
		dialog -> setType (Gtk::MESSAGE_QUESTION);
		dialog -> setMessage (basic::sprintf (_ ("Do you really want to remove palette »%s«?"), folder -> get_basename () .c_str ()));
		dialog -> setText ("All item will be irrevocably deleted!");

		if (dialog -> run () not_eq Gtk::RESPONSE_OK)
			return;

		for (const auto & fileInfo : X3DLibraryView::getChildren (folder))
		{
			if (fileInfo -> get_file_type () == Gio::FILE_TYPE_REGULAR)
				folder -> get_child (fileInfo -> get_name ()) -> remove ();
		}

		folder -> remove ();

		refreshPalette ();

		this -> getPaletteComboBoxText () .set_active (std::min <size_t> (folders .size () - 1, paletteIndex));
	}
	catch (...)
	{ }
}

template <class Type>
void
X3DPaletteEditor <Type>::on_edit_palette_activate ()
{
	__LOG__ << std::endl;
}

template <class Type>
void
X3DPaletteEditor <Type>::on_edit_palette_ok_clicked ()
{
	this -> getEditPaletteDialog () .hide ();

	const auto paletteIndex       = this -> getPaletteComboBoxText () .get_active_row_number ();
	const std::string paletteName = this -> getPaletteNameEntry () .get_text ();
	const auto        folder      = Gio::File::create_for_path (config_dir (libraryFolder + "/" + paletteName));

	folder -> make_directory_with_parents ();

	refreshPalette ();

	const auto iter = std::find (folders .begin () + numDefaultPalettes, folders .end (), folder -> get_uri ());

	if (iter not_eq folders .end ())
		this -> getPaletteComboBoxText () .set_active (iter - folders .begin ());
	else
		this -> getPaletteComboBoxText () .set_active (paletteIndex);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_edit_palette_cancel_clicked ()
{
	this -> getEditPaletteDialog () .hide ();
}

template <class Type>
void
X3DPaletteEditor <Type>::on_palette_name_insert_text (const Glib::ustring & text, int* position)
{
	this -> validateFolderOnInsert (this -> getPaletteNameEntry (), text, *position);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_palette_name_delete_text (int start_pos, int end_pos)
{
	this -> validateFolderOnDelete (this -> getPaletteNameEntry (), start_pos, end_pos);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_palette_name_changed ()
{
	const std::string paletteName = this -> getPaletteNameEntry () .get_text ();

	this -> getEditPaletteOkButton () .set_sensitive (not paletteName .empty () /*and not exists paletteName*/);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_add_object_to_palette_activate ()
{
	try
	{
		const auto paletteIndex = this -> getPaletteComboBoxText () .get_active_row_number ();
		const auto folder       = Gio::File::create_for_uri (folders .at (paletteIndex));
		const auto number       = basic::to_string (files .size () + 1, std::locale::classic ());
		const auto file         = folder -> get_child (folder -> get_basename () + number + ".x3dv");
		const auto scene        = this -> getCurrentBrowser () -> createScene ();
	
		scene -> setWorldURL (file -> get_uri ());

		if (not createScene (scene))
			return;

		scene -> setup ();
		scene -> addStandardMetaData ();
	
		// Print scene.
	
		std::ostringstream osstream;
	
		osstream << X3D::Generator::NicestStyle << scene << std::endl;
	
		// Create file.
	
		std::string etag;
	
		file -> replace_contents (osstream .str (), "", etag, false, Gio::FILE_CREATE_REPLACE_DESTINATION);
	
		// Append material to palette preview.
	
		addObject (Glib::uri_unescape_string (file -> get_uri ()));
	}
	catch (...)
	{
		__LOG__ << std::endl;
	}
}

template <class Type>
void
X3DPaletteEditor <Type>::on_remove_object_from_palette_activate ()
{
	if (selectedIndex < files .size ())
	{
		// Remove and move files

		Gio::File::create_for_uri (files [selectedIndex]) -> remove ();

		for (size_t i = selectedIndex + 1, size = files .size (); i < size; ++ i)
		{
			const auto file = Gio::File::create_for_uri (files [i]);
			
			file -> move (Gio::File::create_for_uri (files [i - 1]), Gio::FILE_COPY_OVERWRITE);
		}

		files .pop_back ();

		// Move items

		group -> children () .erase (group -> children () .begin () + selectedIndex);

		for (size_t i = selectedIndex, size = group -> children () .size (); i < size; ++ i)
		{
			const auto & transform   = group -> children () [i];
			const auto & touchSensor = transform -> getField <X3D::MFNode> ("children") .back ();
	
			transform -> getField <X3D::SFVec3f> ("translation") = getPosition (i);

			touchSensor -> getField <X3D::SFBool> ("isOver")    .removeInterest (this, &X3DPaletteEditor::set_over);
			touchSensor -> getField <X3D::SFTime> ("touchTime") .removeInterest (this, &X3DPaletteEditor::set_touchTime);

			touchSensor -> getField <X3D::SFBool> ("isOver")    .addInterest (this, &X3DPaletteEditor::set_over, std::cref (touchSensor -> getField <X3D::SFBool> ("isOver")), i);
			touchSensor -> getField <X3D::SFTime> ("touchTime") .addInterest (this, &X3DPaletteEditor::set_touchTime, i);
		}

		// Handle selection

		if (selectedIndex >= files .size ())
			setSelection (-1);
	}
}

template <class Type>
X3DPaletteEditor <Type>::~X3DPaletteEditor ()
{ }

} // puck
} // titania

#endif
