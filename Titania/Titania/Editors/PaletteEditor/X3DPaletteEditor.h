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

#include "../../Bits/File.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"

#include <Titania/X3D.h>
#include <Titania/X3D/Components/Geometry3D/Box.h>
#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/PointingDeviceSensor/TouchSensor.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Material.h>
#include <Titania/X3D/Components/Shape/Shape.h>
#include <Titania/X3D/Parser/Filter.h>

#include <Titania/String.h>

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

	virtual
	void
	configure () override;

	void
	setBoxTransparency (const float value)
	{ boxTransparency = value; }

	float
	getBoxTransparency () const
	{ return boxTransparency; }

	///  @name Virtual functions

	virtual
	X3D::SFNode
	getObject (const basic::uri &) = 0;

	virtual
	void
	setTouchTime (const basic::uri &) = 0;

	virtual
	bool
	createScene (const X3D::X3DScenePtr & scene, const std::string & name, const size_t position) = 0;

	void
	updateNamedNode (const X3D::X3DScenePtr & scene, const std::string & name, const size_t position, const X3D::SFNode & node) const;

	///  @name Member access

	const X3D::BrowserPtr &
	getPreview () const
	{ return preview; }

	///  @name Destruction

	virtual
	void
	store () override;


private:

	///  @name Construction

	void
	set_browser ();

	X3D::SFNode
	getBox () const;

	///  @name Operations

	bool
	checkSelection () const;

	void
	refreshPalette ();

	void
	addLibrary (const basic::uri & URL, const bool includeEmptyFolders);

	void
	setCurrentFolder (const size_t);

	void
	addObject (const size_t position, const basic::uri & URL);

	X3D::Vector3f
	getTranslation (const size_t position) const;

	void
	setSelection (const size_t position);

	void
	enable ();

	///  @name Event handlers

	void
	set_over (const bool over, const size_t position);

	void
	set_touchTime (const size_t position);

	void
	on_drag_begin (const Glib::RefPtr <Gdk::DragContext> & context);

	void
	on_drag_data_get (const Glib::RefPtr <Gdk::DragContext> &, Gtk::SelectionData & selection_data, guint info, guint time);

	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y,
	                       const Gtk::SelectionData & selection_data, guint info, guint time);

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
	on_palette_button_press_event (GdkEventButton* event) final override;
	
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
	on_update_object_in_palette_activate () final override;
	
	void
	on_save_object_to_folder (const size_t number);

	virtual
	void
	on_remove_object_from_palette_activate () final override;

	virtual
	void
	on_show_default_palettes_toggled () final override;

	///  @name Members

	X3D::BrowserPtr              preview;
	X3D::X3DPtr <X3D::Group>     group;
	X3D::X3DPtr <X3D::Switch>    selectionSwitch;
	X3D::X3DPtr <X3D::Transform> selectionRectangle;
	X3D::SFNode                  box;
	float                        boxTransparency;
	const std::string            libraryFolder;
	std::vector <basic::uri>     folders;
	std::vector <basic::uri>     files;
	size_t                       numDefaultPalettes;
	bool                         over;
	size_t                       overIndex;
	size_t                       selectedIndex;
	size_t                       dragIndex;
	bool                         changing;

};

template <class Type>
X3DPaletteEditor <Type>::X3DPaletteEditor (const std::string & libraryFolder) :
	              Type (),
	           preview (X3D::createBrowser (this -> getMasterBrowser (), { get_ui ("Editors/Palette.x3dv") })),
	             group (),
	   selectionSwitch (),
	selectionRectangle (),
	               box (),
	   boxTransparency (0.9),
	     libraryFolder (libraryFolder),
	           folders (),
	             files (),
	numDefaultPalettes (0),
	              over (false),
	         overIndex (-1),
	     selectedIndex (-1),
	         dragIndex (-1),
	          changing (true)
{
	this -> addChildObjects (preview, group, selectionSwitch, selectionRectangle, box);
}

template <class Type>
void
X3DPaletteEditor <Type>::initialize ()
{
	// Show browser.

	preview -> initialized () .addInterest (&X3DPaletteEditor::set_browser, this);
	preview -> setFixedPipeline (false);
	preview -> setAntialiasing (4);
	preview -> show ();

	this -> getPalettePreviewBox () .pack_start (*preview, true, true, 0);

//	// Drag n drop
//
//	std::vector <Gtk::TargetEntry> listTargets;
//	
//	listTargets .emplace_back (Gtk::TargetEntry ("STRING"));
//	listTargets .emplace_back (Gtk::TargetEntry ("text/plain"));
//
//	preview -> drag_source_set (listTargets);
//	preview -> drag_dest_set (listTargets);
//
//	preview -> signal_drag_begin ()         .connect (sigc::mem_fun (this, &X3DPaletteEditor::on_drag_begin));
//	preview -> signal_drag_data_get ()      .connect (sigc::mem_fun (this, &X3DPaletteEditor::on_drag_data_get));
//	preview -> signal_drag_data_received () .connect (sigc::mem_fun (this, &X3DPaletteEditor::on_drag_data_received));
}

template <class Type>
void
X3DPaletteEditor <Type>::configure ()
{
	changing = true;

	this -> getShowDefaultPalettesMenuItem () .set_active (this -> getConfig () -> template getItem <bool> ("showDefaultPalettes", true));

	changing = false;
}

template <class Type>
void
X3DPaletteEditor <Type>::set_browser ()
{
	try
	{
		// Disconnect.

		preview -> initialized () .removeInterest (&X3DPaletteEditor::set_browser, this);
	
		// Get exported nodes.

		const auto scene = preview -> getExecutionContext () -> getScene ();

		group              = scene -> getExportedNode <X3D::Group>     ("Items");
		selectionSwitch    = scene -> getExportedNode <X3D::Switch>    ("SelectionSwitch");
		selectionRectangle = scene -> getExportedNode <X3D::Transform> ("SelectionRectangle");
		box                = getBox ();

		// Refresh palette.

		refreshPalette ();
	
		const size_t paletteIndex = this -> getConfig () -> template getItem <int32_t> ("palette");
	
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
X3D::SFNode
X3DPaletteEditor <Type>::getBox () const
{
	const auto shape      = preview -> getExecutionContext () -> createNode <X3D::Shape> ();
	const auto appearance = preview -> getExecutionContext () -> createNode <X3D::Appearance> ();
	const auto material   = preview -> getExecutionContext () -> createNode <X3D::Material> ();
	const auto box        = preview -> getExecutionContext () -> createNode <X3D::Box> ();

	material -> transparency () = boxTransparency;
	appearance -> material ()   = material;
	shape -> appearance ()      = appearance;
	shape -> geometry ()        = box;

	return shape;
}

template <class Type>
bool
X3DPaletteEditor <Type>::checkSelection () const
{
	return not this -> getBrowserWindow () -> getSelection () -> getNodes () .empty ();
}

template <class Type>
void
X3DPaletteEditor <Type>::refreshPalette ()
{
	// Find materials in folders.

	folders .clear ();

	this -> getPaletteComboBoxText () .remove_all ();

	if (this -> getShowDefaultPalettesMenuItem () .get_active ())
		addLibrary (find_data_file ("Library/" + libraryFolder), false);

	numDefaultPalettes = folders .size ();

	addLibrary (config_dir (libraryFolder), true);

	enable ();
}

template <class Type>
void
X3DPaletteEditor <Type>::addLibrary (const basic::uri & libraryPath, const bool includeEmptyFolders)
{
	try
	{
		const auto folder = Gio::File::create_for_path (libraryPath);

		for (const auto & fileInfo : File::getChildren (folder))
		{
			if (fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY)
			{
				const auto child = folder -> get_child (fileInfo -> get_name ());
				const auto URL   = child -> get_uri ();

				if (not includeEmptyFolders and not File::hasChildren (child))
					continue;

				folders .emplace_back (URL);

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

		for (const auto & fileInfo : File::getChildren (folder))
		{
			if (fileInfo -> get_file_type () == Gio::FILE_TYPE_REGULAR)
				addObject (files .size (), Glib::uri_unescape_string (folder -> get_child (fileInfo -> get_name ()) -> get_uri ()));

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
X3DPaletteEditor <Type>::addObject (const size_t position, const basic::uri & URL)
{
	const auto node        = getObject (URL);
	const auto transform   = preview -> getExecutionContext () -> createNode <X3D::Transform> ();
	const auto touchSensor = preview -> getExecutionContext () -> createNode <X3D::TouchSensor> ();

	touchSensor -> isOver ()    .addInterest (&X3DPaletteEditor::set_over, this, std::cref (touchSensor -> isOver ()), position);
	touchSensor -> touchTime () .addInterest (&X3DPaletteEditor::set_touchTime, this, position);

	transform -> translation () = getTranslation (position);

	transform -> children () .emplace_back (touchSensor); // Must be the first node.
	transform -> children () .emplace_back (node);
	transform -> children () .emplace_back (box);

	group -> children () .resize (std::max (position + 1, group -> children () .size ()));
	group -> children () [position] = transform;

	files .resize (std::max (position + 1, files .size ()));
	files [position] = URL;
}

//template <class Type>
//X3D::X3DScenePtr
//X3DPaletteEditor <Type>::createScene (const size_t position)
//{
//	const auto paletteIndex       = this -> getPaletteComboBoxText () .get_active_row_number ();
//	const auto folder             = Gio::File::create_for_uri (folders .at (paletteIndex));
//	const auto positionCharacters = basic::to_string (position + 1, std::locale::classic ());
//	const auto file               = folder -> get_child (folder -> get_basename () + positionCharacters + ".x3dv");
//	const auto scene              = this -> getCurrentBrowser () -> createScene ();
//
//	scene -> setWorldURL (Glib::uri_unescape_string (file -> get_uri ()));
//
//	if (not createScene (scene))
//		return;
//
//	scene -> addStandardMetaData ();
//
//	return scene;
//}

template <class Type>
void
X3DPaletteEditor <Type>::updateNamedNode (const X3D::X3DScenePtr & scene, const std::string & name, const size_t position, const X3D::SFNode & node) const
{
	const auto x3dName = X3D::GetNameFromString (name + basic::to_string (position, std::locale::classic ()));

	if (x3dName .empty ())
		scene -> removeNamedNode (node -> getName ());
	else
		scene -> updateNamedNode (x3dName, node);
}

template <class Type>
X3D::Vector3f
X3DPaletteEditor <Type>::getTranslation (const size_t position) const
{
	const int column = position % COLUMNS;
	const int row    = position / COLUMNS;

	return X3D::Vector3f (column * DISTANCE, -row * DISTANCE, 0);
}

template <class Type>
void
X3DPaletteEditor <Type>::setSelection (const size_t position)
{
	selectedIndex = position;

	if (position < PAGE_SIZE)
	{
		selectionSwitch -> whichChoice ()    = true;
		selectionRectangle -> translation () = getTranslation (position);
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
X3DPaletteEditor <Type>::set_over (const bool value, const size_t position)
{
	over      = value;
	overIndex = position;

	if (over)
		this -> getPalettePreviewBox () .set_tooltip_text (files [position] .stem ());
	else
		this -> getPalettePreviewBox () .set_tooltip_text ("");
}

template <class Type>
void
X3DPaletteEditor <Type>::set_touchTime (const size_t position)
{
	// Display and place selection rectangle.

	setSelection (position);

	// Do something with selection.

	setTouchTime (files [position]);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_drag_begin (const Glib::RefPtr <Gdk::DragContext> & context)
{
__LOG__ << over << std::endl;
__LOG__ << context << std::endl;

	if (not over)
	{
		dragIndex = -1;
		return;
	}

	dragIndex = overIndex;
}

template <class Type>
void
X3DPaletteEditor <Type>::on_drag_data_get (const Glib::RefPtr <Gdk::DragContext> & context, Gtk::SelectionData & selection_data, guint info, guint time)
{
	selection_data .set (selection_data .get_target (),
	                     8 /* 8 bits format */,
	                     (const guchar*) "I'm Data!",
	                     9 /* the length of I'm Data! in bytes */);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y,
                                                const Gtk::SelectionData & selection_data, guint, guint time)
{
__LOG__ << over << std::endl;

	const int length = selection_data .get_length ();

	if (length and selection_data .get_format () == 8)
	{
		__LOG__ << "Received '" << selection_data .get_data_as_string () << "' in label." << std::endl;
	}

	context -> drag_finish (false, false, time);
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

	#ifdef TITANIA_DEBUG
	const bool customPalette = folders .size ();
	#else
	const size_t paletteIndex  = this -> getPaletteComboBoxText () .get_active_row_number ();
	const bool   customPalette = paletteIndex >= numDefaultPalettes and folders .size ();
	#endif

	this -> getRemovePaletteMenuItem () .set_sensitive (customPalette);
	this -> getEditPaletteMenuItem ()   .set_sensitive (customPalette);

	this -> getAddObjectToPaletteMenuItem ()      .set_sensitive (customPalette and files .size () < PAGE_SIZE and checkSelection ());
	this -> getUpdateObjectInPaletteMenuItem ()   .set_sensitive (customPalette and selectedIndex < PAGE_SIZE and files .size () and over);
	this -> getRemoveObjectFromPaletteMenuItem () .set_sensitive (customPalette and selectedIndex < PAGE_SIZE and files .size () and over);

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
		const auto dialog       = this -> template createDialog <MessageDialog> ("MessageDialog");
	
		dialog -> setType (Gtk::MESSAGE_QUESTION);
		dialog -> setMessage (basic::sprintf (_ ("Do you really want to remove palette »%s«?"), folder -> get_basename () .c_str ()));
		dialog -> setText ("All item will be irrevocably deleted!");

		if (dialog -> run () not_eq Gtk::RESPONSE_OK)
			return;

		for (const auto & fileInfo : File::getChildren (folder))
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
	on_save_object_to_folder (files .size ());
}

template <class Type>
void
X3DPaletteEditor <Type>::on_update_object_in_palette_activate ()
{
	const auto dialog = this -> template createDialog <MessageDialog> ("MessageDialog");

	dialog -> setType (Gtk::MESSAGE_QUESTION);
	dialog -> setMessage (_ ("Do you realy want to update the selected item in this palette?"));
	dialog -> setText (_ ("This will irrevocably replace the selected item."));

	if (dialog -> run () not_eq Gtk::RESPONSE_OK)
		return;

	on_save_object_to_folder (selectedIndex);
}

template <class Type>
void
X3DPaletteEditor <Type>::on_save_object_to_folder (const size_t position)
{
	try
	{
		const auto paletteIndex       = this -> getPaletteComboBoxText () .get_active_row_number ();
		const auto folder             = Gio::File::create_for_uri (folders .at (paletteIndex));
		const auto positionCharacters = basic::to_string (position + 1, std::locale::classic ());
		const auto basename           = folder -> get_basename ();
		const auto file               = folder -> get_child (folder -> get_basename () + positionCharacters + ".x3dv");
		const auto scene              = this -> getCurrentBrowser () -> createScene ();
	
		scene -> setWorldURL (file -> get_uri ());

		if (not createScene (scene, basename, position + 1))
			return;

		scene -> addStandardMetaData ();
	
		// Print scene.
	
		std::ostringstream osstream;
	
		osstream << X3D::Generator::NicestStyle << scene << std::endl;
	
		// Create file.
	
		std::string etag;
	
		file -> replace_contents (osstream .str (), "", etag, false, Gio::FILE_CREATE_REPLACE_DESTINATION);
	
		// Append material to palette preview.
	
		addObject (position, Glib::uri_unescape_string (file -> get_uri ()));
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
	try
	{
		const auto dialog = this -> template createDialog <MessageDialog> ("MessageDialog");
	
		dialog -> setType (Gtk::MESSAGE_QUESTION);
		dialog -> setMessage (_ ("Do you realy want to delete the selected item from this palette?"));
		dialog -> setText (_ ("This will irrevocably delete the selected item."));
	
		if (dialog -> run () not_eq Gtk::RESPONSE_OK)
			return;

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
	
			for (size_t position = selectedIndex, size = group -> children () .size (); position < size; ++ position)
			{
				const auto & transform   = group -> children () [position];
				const auto & touchSensor = transform -> getField <X3D::MFNode> ("children") .at (0);

				transform -> getField <X3D::SFVec3f> ("translation") = getTranslation (position);
	
				touchSensor -> getField <X3D::SFBool> ("isOver")    .removeInterest (&X3DPaletteEditor::set_over, this);
				touchSensor -> getField <X3D::SFTime> ("touchTime") .removeInterest (&X3DPaletteEditor::set_touchTime, this);
	
				touchSensor -> getField <X3D::SFBool> ("isOver")    .addInterest (&X3DPaletteEditor::set_over, this, std::cref (touchSensor -> getField <X3D::SFBool> ("isOver")), position);
				touchSensor -> getField <X3D::SFTime> ("touchTime") .addInterest (&X3DPaletteEditor::set_touchTime, this, position);
			}
	
			// Handle selection
	
			if (selectedIndex >= files .size ())
				setSelection (-1);
		}
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}


template <class Type>
void
X3DPaletteEditor <Type>::on_show_default_palettes_toggled ()
{
	if (changing)
		return;

	refreshPalette ();

	this -> getPaletteComboBoxText () .set_active (-1);
}

template <class Type>
void
X3DPaletteEditor <Type>::store ()
{
	this -> getConfig () -> template setItem <bool> ("showDefaultPalettes", this -> getShowDefaultPalettesMenuItem () .get_active ());
}

template <class Type>
X3DPaletteEditor <Type>::~X3DPaletteEditor ()
{ }

} // puck
} // titania

#endif
