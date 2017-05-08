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
#ifndef __TMP_GLAD2CPP_SCENE_PROPERTIES_EDITOR_H__
#define __TMP_GLAD2CPP_SCENE_PROPERTIES_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for ScenePropertiesEditor.
 */
class X3DScenePropertiesEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DScenePropertiesEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DScenePropertiesEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DScenePropertiesEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::ListStore> &
	getMetaDataListStore () const
	{ return m_MetaDataListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getUnitAngleAdjustment () const
	{ return m_UnitAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getUnitForceAdjustment () const
	{ return m_UnitForceAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getUnitLengthAdjustment () const
	{ return m_UnitLengthAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getUnitMassAdjustment () const
	{ return m_UnitMassAdjustment; }

	const Glib::RefPtr <Gtk::TextBuffer> &
	getWorldInfoInfoTextBuffer () const
	{ return m_WorldInfoInfoTextBuffer; }

	const Glib::RefPtr <Gtk::TextBuffer> &
	getWorldInfoInfoTitleTextBuffer () const
	{ return m_WorldInfoInfoTitleTextBuffer; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getMetaDataTreeSelection () const
	{ return m_MetaDataTreeSelection; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getMetaDataNameColumn () const
	{ return m_MetaDataNameColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getCellRendererMetaDataName () const
	{ return m_CellRendererMetaDataName; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getMetaDataContentColumn () const
	{ return m_MetaDataContentColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getCellRendererMetaDataContent () const
	{ return m_CellRendererMetaDataContent; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Notebook &
	getNotebook () const
	{ return *m_Notebook; }

	Gtk::Expander &
	getUnitsExpander () const
	{ return *m_UnitsExpander; }

	Gtk::ComboBoxText &
	getUnitMassCombo () const
	{ return *m_UnitMassCombo; }

	Gtk::Entry &
	getUnitMassEntry () const
	{ return *m_UnitMassEntry; }

	Gtk::ComboBoxText &
	getUnitLengthCombo () const
	{ return *m_UnitLengthCombo; }

	Gtk::Entry &
	getUnitLengthEntry () const
	{ return *m_UnitLengthEntry; }

	Gtk::ComboBoxText &
	getUnitForceCombo () const
	{ return *m_UnitForceCombo; }

	Gtk::Entry &
	getUnitForceEntry () const
	{ return *m_UnitForceEntry; }

	Gtk::ComboBoxText &
	getUnitAngleCombo () const
	{ return *m_UnitAngleCombo; }

	Gtk::Entry &
	getUnitAngleEntry () const
	{ return *m_UnitAngleEntry; }

	Gtk::Expander &
	getMetaDataExpander () const
	{ return *m_MetaDataExpander; }

	Gtk::TreeView &
	getMetaDataTreeView () const
	{ return *m_MetaDataTreeView; }

	Gtk::ButtonBox &
	getUserDefinedFieldsActionBox () const
	{ return *m_UserDefinedFieldsActionBox; }

	Gtk::Button &
	getAddMetaDataButton () const
	{ return *m_AddMetaDataButton; }

	Gtk::Button &
	getRemoveMetaDataButton () const
	{ return *m_RemoveMetaDataButton; }

	Gtk::Switch &
	getAddStandardMetaDataButton () const
	{ return *m_AddStandardMetaDataButton; }

	Gtk::Box &
	getWorldInfoBox () const
	{ return *m_WorldInfoBox; }

	Gtk::Expander &
	getWorldInfoExpander () const
	{ return *m_WorldInfoExpander; }

	Gtk::TextView &
	getWorldInfoTitleTextView () const
	{ return *m_WorldInfoTitleTextView; }

	Gtk::TextView &
	getWorldInfoInfoTextView () const
	{ return *m_WorldInfoInfoTextView; }

	Gtk::Switch &
	getAddWorldInfoButton () const
	{ return *m_AddWorldInfoButton; }

	Gtk::Dialog &
	getMetaDataDialog () const
	{ return *m_MetaDataDialog; }

	Gtk::Button &
	getMetaDataCancelButton () const
	{ return *m_MetaDataCancelButton; }

	Gtk::Button &
	getMetaDataOkButton () const
	{ return *m_MetaDataOkButton; }

	Gtk::Entry &
	getMetaDataNameEntry () const
	{ return *m_MetaDataNameEntry; }

	Gtk::Entry &
	getMetaDataContentEntry () const
	{ return *m_MetaDataContentEntry; }

	///  @name Signal handlers

	virtual
	void
	on_unit_angle_changed () = 0;

	virtual
	void
	on_unit_force_changed () = 0;

	virtual
	void
	on_unit_length_changed () = 0;

	virtual
	void
	on_unit_mass_changed () = 0;

	virtual
	void
	on_unit_mass_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_unit_mass_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_unit_length_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_unit_length_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_unit_force_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_unit_force_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_unit_angle_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_unit_angle_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	bool
	on_meta_data_focus_in_event (GdkEventFocus* focus_event) = 0;

	virtual
	bool
	on_meta_data_focus_out_event (GdkEventFocus* gdk_event) = 0;

	virtual
	bool
	on_meta_data_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_meta_data_changed () = 0;

	virtual
	void
	on_meta_data_name_edited (const Glib::ustring & path, const Glib::ustring & new_text) = 0;

	virtual
	void
	on_meta_data_content_edited (const Glib::ustring & path, const Glib::ustring & new_text) = 0;

	virtual
	void
	on_add_meta_data_clicked () = 0;

	virtual
	void
	on_remove_meta_data_clicked () = 0;

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_add_meta_data_cancel_clicked () = 0;

	virtual
	void
	on_add_meta_data_ok_clicked () = 0;

	virtual
	void
	on_meta_data_name_changed () = 0;

	///  @name Destruction

	virtual
	~X3DScenePropertiesEditorInterface () override;


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ X3DEditorInterface::construct (); }

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::ListStore> m_MetaDataListStore;
	Glib::RefPtr <Gtk::Adjustment> m_UnitAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_UnitForceAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_UnitLengthAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_UnitMassAdjustment;
	Glib::RefPtr <Gtk::TextBuffer> m_WorldInfoInfoTextBuffer;
	Glib::RefPtr <Gtk::TextBuffer> m_WorldInfoInfoTitleTextBuffer;
	Glib::RefPtr <Gtk::TreeSelection> m_MetaDataTreeSelection;
	Glib::RefPtr <Gtk::TreeViewColumn> m_MetaDataNameColumn;
	Glib::RefPtr <Gtk::CellRendererText> m_CellRendererMetaDataName;
	Glib::RefPtr <Gtk::TreeViewColumn> m_MetaDataContentColumn;
	Glib::RefPtr <Gtk::CellRendererText> m_CellRendererMetaDataContent;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Notebook* m_Notebook;
	Gtk::Expander* m_UnitsExpander;
	Gtk::ComboBoxText* m_UnitMassCombo;
	Gtk::Entry* m_UnitMassEntry;
	Gtk::ComboBoxText* m_UnitLengthCombo;
	Gtk::Entry* m_UnitLengthEntry;
	Gtk::ComboBoxText* m_UnitForceCombo;
	Gtk::Entry* m_UnitForceEntry;
	Gtk::ComboBoxText* m_UnitAngleCombo;
	Gtk::Entry* m_UnitAngleEntry;
	Gtk::Expander* m_MetaDataExpander;
	Gtk::TreeView* m_MetaDataTreeView;
	Gtk::ButtonBox* m_UserDefinedFieldsActionBox;
	Gtk::Button* m_AddMetaDataButton;
	Gtk::Button* m_RemoveMetaDataButton;
	Gtk::Switch* m_AddStandardMetaDataButton;
	Gtk::Box* m_WorldInfoBox;
	Gtk::Expander* m_WorldInfoExpander;
	Gtk::TextView* m_WorldInfoTitleTextView;
	Gtk::TextView* m_WorldInfoInfoTextView;
	Gtk::Switch* m_AddWorldInfoButton;
	Gtk::Dialog* m_MetaDataDialog;
	Gtk::Button* m_MetaDataCancelButton;
	Gtk::Button* m_MetaDataOkButton;
	Gtk::Entry* m_MetaDataNameEntry;
	Gtk::Entry* m_MetaDataContentEntry;

};

} // puck
} // titania

#endif
