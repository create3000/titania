/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TMP_GLAD2CPP_NODE_PROPERTIES_EDITOR_H__
#define __TMP_GLAD2CPP_NODE_PROPERTIES_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for NodePropertiesEditor.
 */
class X3DNodePropertiesEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DNodePropertiesEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DNodePropertiesEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DNodePropertiesEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::ListStore> &
	getExportedNodesListStore () const
	{ return m_ExportedNodesListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getFieldNameListStore () const
	{ return m_FieldNameListStore; }

	const Glib::RefPtr <Gtk::EntryCompletion> &
	getFieldNameCompletion () const
	{ return m_FieldNameCompletion; }

	const Glib::RefPtr <Gtk::ListStore> &
	getImportedNodesListStore () const
	{ return m_ImportedNodesListStore; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getUserDefinedFieldsTreeSelection () const
	{ return m_UserDefinedFieldsTreeSelection; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getCellRendererType () const
	{ return m_CellRendererType; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getCellRendererName () const
	{ return m_CellRendererName; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getCellRendererAccessType () const
	{ return m_CellRendererAccessType; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getImportedNodesTreeSelection () const
	{ return m_ImportedNodesTreeSelection; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getImportedNodesImportedTreeviewColumn () const
	{ return m_ImportedNodesImportedTreeviewColumn; }

	const Glib::RefPtr <Gtk::CellRendererToggle> &
	getImportedNodesImportedCellRendererToggle () const
	{ return m_ImportedNodesImportedCellRendererToggle; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getImportedNodesTypeNameCellRendererText () const
	{ return m_ImportedNodesTypeNameCellRendererText; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getImportedNodesExportedNameCellRendererText () const
	{ return m_ImportedNodesExportedNameCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getImportedNodesImportedNameTreeviewColumn () const
	{ return m_ImportedNodesImportedNameTreeviewColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getImportedNodesImportedNameCellRendererText () const
	{ return m_ImportedNodesImportedNameCellRendererText; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getExportedNodesTreeSelection () const
	{ return m_ExportedNodesTreeSelection; }

	Gtk::Menu &
	getAccessTypeMenu () const
	{ return *m_AccessTypeMenu; }

	Gtk::MenuItem &
	getInitializeOnlyMenuItem () const
	{ return *m_InitializeOnlyMenuItem; }

	Gtk::MenuItem &
	getInputOnlyMenuItem () const
	{ return *m_InputOnlyMenuItem; }

	Gtk::MenuItem &
	getOutputOnlyMenuItem () const
	{ return *m_OutputOnlyMenuItem; }

	Gtk::MenuItem &
	getInputOutputMenuItem () const
	{ return *m_InputOutputMenuItem; }

	Gtk::Menu &
	getFieldTypeMenu () const
	{ return *m_FieldTypeMenu; }

	Gtk::MenuItem &
	getSFBoolMenuItem () const
	{ return *m_SFBoolMenuItem; }

	Gtk::MenuItem &
	getSFColorMenuItem () const
	{ return *m_SFColorMenuItem; }

	Gtk::MenuItem &
	getSFColorRGBAMenuItem () const
	{ return *m_SFColorRGBAMenuItem; }

	Gtk::MenuItem &
	getSFDoubleMenuItem () const
	{ return *m_SFDoubleMenuItem; }

	Gtk::MenuItem &
	getSFFloatMenuItem () const
	{ return *m_SFFloatMenuItem; }

	Gtk::MenuItem &
	getSFImageMenuItem () const
	{ return *m_SFImageMenuItem; }

	Gtk::MenuItem &
	getSFInt32MenuItem () const
	{ return *m_SFInt32MenuItem; }

	Gtk::MenuItem &
	getSFMatrix3dMenuItem () const
	{ return *m_SFMatrix3dMenuItem; }

	Gtk::MenuItem &
	getSFMatrix3fMenuItem () const
	{ return *m_SFMatrix3fMenuItem; }

	Gtk::MenuItem &
	getSFMatrix4dMenuItem () const
	{ return *m_SFMatrix4dMenuItem; }

	Gtk::MenuItem &
	getSFMatrix4fMenuItem () const
	{ return *m_SFMatrix4fMenuItem; }

	Gtk::MenuItem &
	getSFNodeMenuItem () const
	{ return *m_SFNodeMenuItem; }

	Gtk::MenuItem &
	getSFRotationMenuItem () const
	{ return *m_SFRotationMenuItem; }

	Gtk::MenuItem &
	getSFStringMenuItem () const
	{ return *m_SFStringMenuItem; }

	Gtk::MenuItem &
	getSFTimeMenuItem () const
	{ return *m_SFTimeMenuItem; }

	Gtk::MenuItem &
	getSFVec2dMenuItem () const
	{ return *m_SFVec2dMenuItem; }

	Gtk::MenuItem &
	getSFVec2fMenuItem () const
	{ return *m_SFVec2fMenuItem; }

	Gtk::MenuItem &
	getSFVec3dMenuItem () const
	{ return *m_SFVec3dMenuItem; }

	Gtk::MenuItem &
	getSFVec3fMenuItem () const
	{ return *m_SFVec3fMenuItem; }

	Gtk::MenuItem &
	getSFVec4dMenuItem () const
	{ return *m_SFVec4dMenuItem; }

	Gtk::MenuItem &
	getSFVec4fMenuItem () const
	{ return *m_SFVec4fMenuItem; }

	Gtk::MenuItem &
	getMFBoolMenuItem () const
	{ return *m_MFBoolMenuItem; }

	Gtk::MenuItem &
	getMFColorMenuItem () const
	{ return *m_MFColorMenuItem; }

	Gtk::MenuItem &
	getMFColorRGBAMenuItem () const
	{ return *m_MFColorRGBAMenuItem; }

	Gtk::MenuItem &
	getMFDoubleMenuItem () const
	{ return *m_MFDoubleMenuItem; }

	Gtk::MenuItem &
	getMFFloatMenuItem () const
	{ return *m_MFFloatMenuItem; }

	Gtk::MenuItem &
	getMFImageMenuItem () const
	{ return *m_MFImageMenuItem; }

	Gtk::MenuItem &
	getMFInt32MenuItem () const
	{ return *m_MFInt32MenuItem; }

	Gtk::MenuItem &
	getMFMatrix3dMenuItem () const
	{ return *m_MFMatrix3dMenuItem; }

	Gtk::MenuItem &
	getMFMatrix3fMenuItem () const
	{ return *m_MFMatrix3fMenuItem; }

	Gtk::MenuItem &
	getMFMatrix4dMenuItem () const
	{ return *m_MFMatrix4dMenuItem; }

	Gtk::MenuItem &
	getMFMatrix4fMenuItem () const
	{ return *m_MFMatrix4fMenuItem; }

	Gtk::MenuItem &
	getMFNodeMenuItem () const
	{ return *m_MFNodeMenuItem; }

	Gtk::MenuItem &
	getMFRotationMenuItem () const
	{ return *m_MFRotationMenuItem; }

	Gtk::MenuItem &
	getMFStringMenuItem () const
	{ return *m_MFStringMenuItem; }

	Gtk::MenuItem &
	getMFTimeMenuItem () const
	{ return *m_MFTimeMenuItem; }

	Gtk::MenuItem &
	getMFVec2dMenuItem () const
	{ return *m_MFVec2dMenuItem; }

	Gtk::MenuItem &
	getMFVec2fMenuItem () const
	{ return *m_MFVec2fMenuItem; }

	Gtk::MenuItem &
	getMFVec3dMenuItem () const
	{ return *m_MFVec3dMenuItem; }

	Gtk::MenuItem &
	getMFVec3fMenuItem () const
	{ return *m_MFVec3fMenuItem; }

	Gtk::MenuItem &
	getMFVec4dMenuItem () const
	{ return *m_MFVec4dMenuItem; }

	Gtk::MenuItem &
	getMFVec4fMenuItem () const
	{ return *m_MFVec4fMenuItem; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Notebook &
	getNodeChildNotebook () const
	{ return *m_NodeChildNotebook; }

	Gtk::Expander &
	getNodePropertiesExpander () const
	{ return *m_NodePropertiesExpander; }

	Gtk::Label &
	getTypeNameLabel () const
	{ return *m_TypeNameLabel; }

	Gtk::Label &
	getNameLabel () const
	{ return *m_NameLabel; }

	Gtk::Entry &
	getTypeNameEntry () const
	{ return *m_TypeNameEntry; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Expander &
	getCommonPropertiesExpander () const
	{ return *m_CommonPropertiesExpander; }

	Gtk::Entry &
	getComponentEntry () const
	{ return *m_ComponentEntry; }

	Gtk::Entry &
	getContainerFieldEntry () const
	{ return *m_ContainerFieldEntry; }

	Gtk::Box &
	getUserDefinedFieldsBox () const
	{ return *m_UserDefinedFieldsBox; }

	Gtk::Expander &
	getUserDefinedFieldsExpander () const
	{ return *m_UserDefinedFieldsExpander; }

	Gtk::Box &
	getUserDefinedFieldsWidget () const
	{ return *m_UserDefinedFieldsWidget; }

	Gtk::TreeView &
	getUserDefinedFieldsTreeView () const
	{ return *m_UserDefinedFieldsTreeView; }

	Gtk::ButtonBox &
	getUserDefinedFieldsActionBox () const
	{ return *m_UserDefinedFieldsActionBox; }

	Gtk::Button &
	getAddUserDefinedFieldButton () const
	{ return *m_AddUserDefinedFieldButton; }

	Gtk::Button &
	getRemoveUserDefinedFieldButton () const
	{ return *m_RemoveUserDefinedFieldButton; }

	Gtk::Box &
	getImportedNodesBox () const
	{ return *m_ImportedNodesBox; }

	Gtk::Expander &
	getImportedNodesExpander () const
	{ return *m_ImportedNodesExpander; }

	Gtk::TreeView &
	getImportedNodesTreeView () const
	{ return *m_ImportedNodesTreeView; }

	Gtk::Box &
	getExportedNodesBox () const
	{ return *m_ExportedNodesBox; }

	Gtk::Expander &
	getExportedNodesExpander () const
	{ return *m_ExportedNodesExpander; }

	Gtk::TreeView &
	getExportedNodesTreeView () const
	{ return *m_ExportedNodesTreeView; }

	Gtk::Button &
	getAddExportedNodeButton () const
	{ return *m_AddExportedNodeButton; }

	Gtk::Button &
	getRemoveExportedNodeButton () const
	{ return *m_RemoveExportedNodeButton; }

	Gtk::Dialog &
	getExportedNodeDialog () const
	{ return *m_ExportedNodeDialog; }

	Gtk::Button &
	getExportedNodeCancelButton () const
	{ return *m_ExportedNodeCancelButton; }

	Gtk::Button &
	getExportedNodeOkButton () const
	{ return *m_ExportedNodeOkButton; }

	Gtk::Entry &
	getExportedNameEntry () const
	{ return *m_ExportedNameEntry; }

	Gtk::Dialog &
	getUserDefinedFieldDialog () const
	{ return *m_UserDefinedFieldDialog; }

	Gtk::Button &
	getUserDefinedFieldCancelButton () const
	{ return *m_UserDefinedFieldCancelButton; }

	Gtk::Button &
	getAddUserDefinedFieldOkButton () const
	{ return *m_AddUserDefinedFieldOkButton; }

	Gtk::MenuButton &
	getAccessTypeMenuButton () const
	{ return *m_AccessTypeMenuButton; }

	Gtk::Label &
	getAccessTypeLabel () const
	{ return *m_AccessTypeLabel; }

	Gtk::MenuButton &
	getFieldTypeMenuButton () const
	{ return *m_FieldTypeMenuButton; }

	Gtk::Label &
	getFieldTypeLabel () const
	{ return *m_FieldTypeLabel; }

	Gtk::Entry &
	getFieldNameEntry () const
	{ return *m_FieldNameEntry; }

	///  @name Signal handlers

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	bool
	on_user_defined_field_focus_in_event (GdkEventFocus* focus_event) = 0;

	virtual
	bool
	on_user_defined_field_focus_out_event (GdkEventFocus* gdk_event) = 0;

	virtual
	bool
	on_user_defined_field_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_user_defined_field_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) = 0;

	virtual
	void
	on_user_defined_field_changed () = 0;

	virtual
	void
	on_add_user_defined_field_clicked () = 0;

	virtual
	void
	on_remove_user_defined_field_clicked () = 0;

	virtual
	void
	on_imported_toggled (const Glib::ustring & path) = 0;

	virtual
	void
	on_imported_name_edited (const Glib::ustring & path, const Glib::ustring & new_text) = 0;

	virtual
	void
	on_exported_node_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) = 0;

	virtual
	void
	on_exported_node_changed () = 0;

	virtual
	void
	on_add_exported_node () = 0;

	virtual
	void
	on_remove_exported_node () = 0;

	virtual
	void
	on_exported_node_cancel_clicked () = 0;

	virtual
	void
	on_exported_node_ok_clicked () = 0;

	virtual
	void
	on_exported_name_changed () = 0;

	virtual
	void
	on_exported_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_exported_name_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_add_field_cancel_clicked () = 0;

	virtual
	void
	on_add_field_ok_clicked () = 0;

	virtual
	void
	on_field_name_changed () = 0;

	virtual
	void
	on_field_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_field_name_insert_text (const Glib::ustring & text, int* position) = 0;

	///  @name Destruction

	virtual
	~X3DNodePropertiesEditorInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::ListStore> m_ExportedNodesListStore;
	Glib::RefPtr <Gtk::ListStore> m_FieldNameListStore;
	Glib::RefPtr <Gtk::EntryCompletion> m_FieldNameCompletion;
	Glib::RefPtr <Gtk::ListStore> m_ImportedNodesListStore;
	Glib::RefPtr <Gtk::TreeSelection> m_UserDefinedFieldsTreeSelection;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_CellRendererType;
	Glib::RefPtr <Gtk::CellRendererText> m_CellRendererName;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_CellRendererAccessType;
	Glib::RefPtr <Gtk::TreeSelection> m_ImportedNodesTreeSelection;
	Glib::RefPtr <Gtk::TreeViewColumn> m_ImportedNodesImportedTreeviewColumn;
	Glib::RefPtr <Gtk::CellRendererToggle> m_ImportedNodesImportedCellRendererToggle;
	Glib::RefPtr <Gtk::CellRendererText> m_ImportedNodesTypeNameCellRendererText;
	Glib::RefPtr <Gtk::CellRendererText> m_ImportedNodesExportedNameCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_ImportedNodesImportedNameTreeviewColumn;
	Glib::RefPtr <Gtk::CellRendererText> m_ImportedNodesImportedNameCellRendererText;
	Glib::RefPtr <Gtk::TreeSelection> m_ExportedNodesTreeSelection;
	Gtk::Menu* m_AccessTypeMenu;
	Gtk::MenuItem* m_InitializeOnlyMenuItem;
	Gtk::MenuItem* m_InputOnlyMenuItem;
	Gtk::MenuItem* m_OutputOnlyMenuItem;
	Gtk::MenuItem* m_InputOutputMenuItem;
	Gtk::Menu* m_FieldTypeMenu;
	Gtk::MenuItem* m_SFBoolMenuItem;
	Gtk::MenuItem* m_SFColorMenuItem;
	Gtk::MenuItem* m_SFColorRGBAMenuItem;
	Gtk::MenuItem* m_SFDoubleMenuItem;
	Gtk::MenuItem* m_SFFloatMenuItem;
	Gtk::MenuItem* m_SFImageMenuItem;
	Gtk::MenuItem* m_SFInt32MenuItem;
	Gtk::MenuItem* m_SFMatrix3dMenuItem;
	Gtk::MenuItem* m_SFMatrix3fMenuItem;
	Gtk::MenuItem* m_SFMatrix4dMenuItem;
	Gtk::MenuItem* m_SFMatrix4fMenuItem;
	Gtk::MenuItem* m_SFNodeMenuItem;
	Gtk::MenuItem* m_SFRotationMenuItem;
	Gtk::MenuItem* m_SFStringMenuItem;
	Gtk::MenuItem* m_SFTimeMenuItem;
	Gtk::MenuItem* m_SFVec2dMenuItem;
	Gtk::MenuItem* m_SFVec2fMenuItem;
	Gtk::MenuItem* m_SFVec3dMenuItem;
	Gtk::MenuItem* m_SFVec3fMenuItem;
	Gtk::MenuItem* m_SFVec4dMenuItem;
	Gtk::MenuItem* m_SFVec4fMenuItem;
	Gtk::MenuItem* m_MFBoolMenuItem;
	Gtk::MenuItem* m_MFColorMenuItem;
	Gtk::MenuItem* m_MFColorRGBAMenuItem;
	Gtk::MenuItem* m_MFDoubleMenuItem;
	Gtk::MenuItem* m_MFFloatMenuItem;
	Gtk::MenuItem* m_MFImageMenuItem;
	Gtk::MenuItem* m_MFInt32MenuItem;
	Gtk::MenuItem* m_MFMatrix3dMenuItem;
	Gtk::MenuItem* m_MFMatrix3fMenuItem;
	Gtk::MenuItem* m_MFMatrix4dMenuItem;
	Gtk::MenuItem* m_MFMatrix4fMenuItem;
	Gtk::MenuItem* m_MFNodeMenuItem;
	Gtk::MenuItem* m_MFRotationMenuItem;
	Gtk::MenuItem* m_MFStringMenuItem;
	Gtk::MenuItem* m_MFTimeMenuItem;
	Gtk::MenuItem* m_MFVec2dMenuItem;
	Gtk::MenuItem* m_MFVec2fMenuItem;
	Gtk::MenuItem* m_MFVec3dMenuItem;
	Gtk::MenuItem* m_MFVec3fMenuItem;
	Gtk::MenuItem* m_MFVec4dMenuItem;
	Gtk::MenuItem* m_MFVec4fMenuItem;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Notebook* m_NodeChildNotebook;
	Gtk::Expander* m_NodePropertiesExpander;
	Gtk::Label* m_TypeNameLabel;
	Gtk::Label* m_NameLabel;
	Gtk::Entry* m_TypeNameEntry;
	Gtk::Box* m_NameBox;
	Gtk::Entry* m_NameEntry;
	Gtk::Button* m_RenameButton;
	Gtk::Expander* m_CommonPropertiesExpander;
	Gtk::Entry* m_ComponentEntry;
	Gtk::Entry* m_ContainerFieldEntry;
	Gtk::Box* m_UserDefinedFieldsBox;
	Gtk::Expander* m_UserDefinedFieldsExpander;
	Gtk::Box* m_UserDefinedFieldsWidget;
	Gtk::TreeView* m_UserDefinedFieldsTreeView;
	Gtk::ButtonBox* m_UserDefinedFieldsActionBox;
	Gtk::Button* m_AddUserDefinedFieldButton;
	Gtk::Button* m_RemoveUserDefinedFieldButton;
	Gtk::Box* m_ImportedNodesBox;
	Gtk::Expander* m_ImportedNodesExpander;
	Gtk::TreeView* m_ImportedNodesTreeView;
	Gtk::Box* m_ExportedNodesBox;
	Gtk::Expander* m_ExportedNodesExpander;
	Gtk::TreeView* m_ExportedNodesTreeView;
	Gtk::Button* m_AddExportedNodeButton;
	Gtk::Button* m_RemoveExportedNodeButton;
	Gtk::Dialog* m_ExportedNodeDialog;
	Gtk::Button* m_ExportedNodeCancelButton;
	Gtk::Button* m_ExportedNodeOkButton;
	Gtk::Entry* m_ExportedNameEntry;
	Gtk::Dialog* m_UserDefinedFieldDialog;
	Gtk::Button* m_UserDefinedFieldCancelButton;
	Gtk::Button* m_AddUserDefinedFieldOkButton;
	Gtk::MenuButton* m_AccessTypeMenuButton;
	Gtk::Label* m_AccessTypeLabel;
	Gtk::MenuButton* m_FieldTypeMenuButton;
	Gtk::Label* m_FieldTypeLabel;
	Gtk::Entry* m_FieldNameEntry;

};

} // puck
} // titania

#endif
