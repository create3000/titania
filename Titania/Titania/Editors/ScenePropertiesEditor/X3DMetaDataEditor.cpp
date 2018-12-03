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

#include "X3DMetaDataEditor.h"

#include <Titania/X3D/Components/Core/WorldInfo.h>

namespace titania {
namespace puck {

struct X3DMetaDataEditor::Columns
{
	static constexpr int32_t NAME    = 0;
	static constexpr int32_t CONTENT = 1;
};

X3DMetaDataEditor::X3DMetaDataEditor () :
	X3DScenePropertiesEditorInterface (),
	                            scene ()
{
	addChildObjects (scene);

	getAddStandardMetaDataButton () .property_active () .signal_changed () .connect (sigc::mem_fun (this, &X3DMetaDataEditor::on_add_standard_meta_data_activate));
}

void
X3DMetaDataEditor::initialize ()
{
	set_current_scene ();
}

void
X3DMetaDataEditor::configure ()
{
	getCurrentScene () .addInterest (&X3DMetaDataEditor::set_current_scene, this);

	getAddStandardMetaDataButton () .set_state (getBrowserWindow () -> getConfig () -> getItem <bool> ("addStandardMetaData"));
	//getAddStandardMetaDataButton () .set_state (getAddStandardMetadata (scene));
}

void
X3DMetaDataEditor::set_current_scene ()
{
	if (scene)
		scene -> metaData_changed () .removeInterest (&X3DMetaDataEditor::set_meta_data, this);

	scene = getCurrentScene ();

	scene -> metaData_changed () .addInterest (&X3DMetaDataEditor::set_meta_data, this);

	set_meta_data ();
}

void
X3DMetaDataEditor::set_meta_data ()
{
	getMetaDataListStore () -> clear ();

	for (const auto & metaData : scene -> getMetaDatas ())
	{
		const auto iter = getMetaDataListStore () -> append ();

		iter -> set_value (Columns::NAME,    metaData .first);
		iter -> set_value (Columns::CONTENT, metaData .second);
	}
}

bool
X3DMetaDataEditor::on_meta_data_focus_in_event (GdkEventFocus* event)
{
	getBrowserWindow () -> setAccelerators (false);
	return false;
}

bool
X3DMetaDataEditor::on_meta_data_focus_out_event (GdkEventFocus* event)
{
	getBrowserWindow () -> setAccelerators (true);
	return false;
}

bool
X3DMetaDataEditor::on_meta_data_key_press_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_BackSpace or event -> keyval == GDK_KEY_Delete)
	{
		if (getMetaDataTreeView () .get_selection () -> get_selected_rows () .empty ())
			return false;

		on_remove_meta_data_clicked ();
		return true;
	}

	return false;
}

void
X3DMetaDataEditor::on_meta_data_changed ()
{
	getRemoveMetaDataButton () .set_sensitive (not getMetaDataTreeView () .get_selection () -> get_selected_rows () .empty ());
}

void
X3DMetaDataEditor::on_add_meta_data_clicked ()
{
	getMetaDataNameEntry ()    .set_text ("");
	getMetaDataContentEntry () .set_text ("");

	getMetaDataDialog () .present ();
}

void
X3DMetaDataEditor::on_meta_data_name_changed ()
{
	getMetaDataOkButton () .set_sensitive (not getMetaDataNameEntry () .get_text () .empty ());
}

void
X3DMetaDataEditor::on_add_meta_data_ok_clicked ()
{
	getMetaDataDialog () .hide ();

	const auto name    = getMetaDataNameEntry ()    .get_text ();
	const auto content = getMetaDataContentEntry () .get_text ();

	const auto iter = getMetaDataListStore () -> append ();

	iter -> set_value (Columns::NAME,    name);
	iter -> set_value (Columns::CONTENT, content);

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Add Meta Data »%s«"), name .c_str ()));

	undoStep -> addUndoFunction (&X3D::X3DScene::removeMetaData, scene, name);
	undoStep -> addRedoFunction (&X3D::X3DScene::setMetaData, scene, name, content);

	addUndoStep (undoStep);

	scene -> setMetaData (name, content);

	scene -> metaData_changed () .removeInterest (&X3DMetaDataEditor::set_meta_data, this);
	scene -> metaData_changed () .addInterest (&X3DMetaDataEditor::connectMetaData, this);
}

void
X3DMetaDataEditor::on_add_meta_data_cancel_clicked ()
{
	getMetaDataDialog () .hide ();
}

void
X3DMetaDataEditor::on_remove_meta_data_clicked ()
{
	const auto selected = getMetaDataTreeView () .get_selection () -> get_selected ();

	std::string name;
	std::string content;

	selected -> get_value (Columns::NAME, name);
	selected -> get_value (Columns::CONTENT, content);

	getMetaDataListStore () -> erase (selected);

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove Meta Data »%s«"), name .c_str ()));

	undoStep -> addUndoFunction (&X3D::X3DScene::setMetaData, scene, name, content);
	undoStep -> addRedoFunction (&X3D::X3DScene::removeMetaData, scene, name);

	addUndoStep (undoStep);

	scene -> removeMetaData (name);

	scene -> metaData_changed () .removeInterest (&X3DMetaDataEditor::set_meta_data, this);
	scene -> metaData_changed () .addInterest (&X3DMetaDataEditor::connectMetaData, this);
}

void
X3DMetaDataEditor::on_meta_data_name_edited (const Glib::ustring & path, const Glib::ustring & new_text)
{
	if (new_text .empty ())
		return;

	std::string name;
	std::string content;

	const auto iter = getMetaDataListStore () -> get_iter (path);

	iter -> get_value (Columns::NAME,    name);
	iter -> get_value (Columns::CONTENT, content);
	iter -> set_value (Columns::NAME,    new_text);

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Change Meta Data »%s«"), name .c_str ()));

	undoStep -> addUndoFunction (&X3D::X3DScene::setMetaData, scene, name, content);
	undoStep -> addUndoFunction (&X3D::X3DScene::removeMetaData, scene, new_text);
	undoStep -> addRedoFunction (&X3D::X3DScene::removeMetaData, scene, name);
	undoStep -> addRedoFunction (&X3D::X3DScene::setMetaData, scene, new_text, content);

	addUndoStep (undoStep);

	scene -> removeMetaData (name);
	scene -> setMetaData (new_text, content);

	scene -> metaData_changed () .removeInterest (&X3DMetaDataEditor::set_meta_data, this);
	scene -> metaData_changed () .addInterest (&X3DMetaDataEditor::connectMetaData, this);
}

void
X3DMetaDataEditor::on_meta_data_content_edited (const Glib::ustring & path, const Glib::ustring & new_text)
{
	std::string name;
	std::string content;

	const auto iter = getMetaDataListStore () -> get_iter (path);

	iter -> get_value (Columns::NAME,    name);
	iter -> get_value (Columns::CONTENT, content);
	iter -> set_value (Columns::CONTENT, new_text);

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Change Meta Data »%s«"), name .c_str ()));

	undoStep -> addUndoFunction (&X3D::X3DScene::setMetaData, scene, name, content);
	undoStep -> addRedoFunction (&X3D::X3DScene::setMetaData, scene, name, new_text);

	addUndoStep (undoStep);

	scene -> setMetaData (name, new_text);

	scene -> metaData_changed () .removeInterest (&X3DMetaDataEditor::set_meta_data, this);
	scene -> metaData_changed () .addInterest (&X3DMetaDataEditor::connectMetaData, this);
}

void
X3DMetaDataEditor::connectMetaData (const X3D::SFTime & field)
{
	field .removeInterest (&X3DMetaDataEditor::connectMetaData, this);
	field .addInterest (&X3DMetaDataEditor::set_meta_data, this);
}

void
X3DMetaDataEditor::on_add_standard_meta_data_activate ()
{
	getBrowserWindow () -> getConfig () -> setItem ("addStandardMetaData", getAddStandardMetaDataButton () .get_active ());
	//setAddStandardMetadata (scene, getAddStandardMetaDataButton () .get_active ());
}

void
X3DMetaDataEditor::store ()
{
	getCurrentScene () .removeInterest (&X3DMetaDataEditor::set_current_scene, this);

	if (getMetaDataTreeView () .has_focus ())
		getCurrentBrowser () -> grab_focus ();
}

X3DMetaDataEditor::~X3DMetaDataEditor ()
{ }

} // puck
} // titania
