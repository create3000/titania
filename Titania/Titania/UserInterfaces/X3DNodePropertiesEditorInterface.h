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
#ifndef __TMP_GLAD2CPP_NODE_PROPERTIES_EDITOR_H__
#define __TMP_GLAD2CPP_NODE_PROPERTIES_EDITOR_H__

#include "../Base/X3DDialogInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DNodePropertiesEditorInterface :
	public X3DDialogInterface
{
public:

	template <class ... Arguments>
	X3DNodePropertiesEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DDialogInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const std::string & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		widget -> set_name (name);
		return widget;
	}

	const Glib::RefPtr <Gtk::TreeSelection> &
	getUserDefinedFieldsTreeviewSelection () const
	{ return m_UserDefinedFieldsTreeviewSelection; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getCellRendererType () const
	{ return m_CellRendererType; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getCellRendererName () const
	{ return m_CellRendererName; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getCellRendererAccessType () const
	{ return m_CellRendererAccessType; }

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

	Gtk::Dialog &
	getWindow () const
	{ return *m_Window; }

	Gtk::Button &
	getCancelButton () const
	{ return *m_CancelButton; }

	Gtk::Button &
	getCancelButton1 () const
	{ return *m_CancelButton1; }

	Gtk::Button &
	getOkButton () const
	{ return *m_OkButton; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Label &
	getHeaderLabel () const
	{ return *m_HeaderLabel; }

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

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Expander &
	getCommonPropertiesExpander () const
	{ return *m_CommonPropertiesExpander; }

	Gtk::Entry &
	getComponentEntry () const
	{ return *m_ComponentEntry; }

	Gtk::Entry &
	getContainerFieldEntry () const
	{ return *m_ContainerFieldEntry; }

	Gtk::Expander &
	getUserDefinedFieldsExpander () const
	{ return *m_UserDefinedFieldsExpander; }

	Gtk::TreeView &
	getUserDefinedFieldsTreeView () const
	{ return *m_UserDefinedFieldsTreeView; }

	Gtk::Button &
	getAddFieldButton () const
	{ return *m_AddFieldButton; }

	Gtk::Button &
	getRemoveFieldButton () const
	{ return *m_RemoveFieldButton; }

	Gtk::Expander &
	getCDataFieldExpander () const
	{ return *m_CDataFieldExpander; }

	Gtk::Button &
	getEditCDataButton () const
	{ return *m_EditCDataButton; }

	Gtk::Dialog &
	getAddFieldDialog () const
	{ return *m_AddFieldDialog; }

	Gtk::Button &
	getAddFieldCancelButton () const
	{ return *m_AddFieldCancelButton; }

	Gtk::Button &
	getAddFieldOkButton () const
	{ return *m_AddFieldOkButton; }

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

	virtual
	void
	on_cancel () = 0;

	virtual
	void
	on_apply () = 0;

	virtual
	void
	on_ok () = 0;

	virtual
	void
	on_type_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_type_name_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_name_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_user_defined_field_activated (const TreeModel::Path & path, TreeViewColumn* column) = 0;

	virtual
	void
	on_user_defined_field_changed () = 0;

	virtual
	void
	on_add_field_clicked () = 0;

	virtual
	void
	on_remove_field_clicked () = 0;

	virtual
	void
	on_edit_cdata_clicked () = 0;

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

	virtual
	~X3DNodePropertiesEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	Glib::RefPtr <Gtk::TreeSelection>      m_UserDefinedFieldsTreeviewSelection;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_CellRendererType;
	Glib::RefPtr <Gtk::CellRendererText>   m_CellRendererName;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_CellRendererAccessType;
	Gtk::Menu*                             m_AccessTypeMenu;
	Gtk::MenuItem*                         m_InitializeOnlyMenuItem;
	Gtk::MenuItem*                         m_InputOnlyMenuItem;
	Gtk::MenuItem*                         m_OutputOnlyMenuItem;
	Gtk::MenuItem*                         m_InputOutputMenuItem;
	Gtk::Menu*                             m_FieldTypeMenu;
	Gtk::MenuItem*                         m_SFBoolMenuItem;
	Gtk::MenuItem*                         m_SFColorMenuItem;
	Gtk::MenuItem*                         m_SFColorRGBAMenuItem;
	Gtk::MenuItem*                         m_SFDoubleMenuItem;
	Gtk::MenuItem*                         m_SFFloatMenuItem;
	Gtk::MenuItem*                         m_SFImageMenuItem;
	Gtk::MenuItem*                         m_SFInt32MenuItem;
	Gtk::MenuItem*                         m_SFMatrix3dMenuItem;
	Gtk::MenuItem*                         m_SFMatrix3fMenuItem;
	Gtk::MenuItem*                         m_SFMatrix4dMenuItem;
	Gtk::MenuItem*                         m_SFMatrix4fMenuItem;
	Gtk::MenuItem*                         m_SFNodeMenuItem;
	Gtk::MenuItem*                         m_SFRotationMenuItem;
	Gtk::MenuItem*                         m_SFStringMenuItem;
	Gtk::MenuItem*                         m_SFTimeMenuItem;
	Gtk::MenuItem*                         m_SFVec2dMenuItem;
	Gtk::MenuItem*                         m_SFVec2fMenuItem;
	Gtk::MenuItem*                         m_SFVec3dMenuItem;
	Gtk::MenuItem*                         m_SFVec3fMenuItem;
	Gtk::MenuItem*                         m_SFVec4dMenuItem;
	Gtk::MenuItem*                         m_SFVec4fMenuItem;
	Gtk::MenuItem*                         m_MFBoolMenuItem;
	Gtk::MenuItem*                         m_MFColorMenuItem;
	Gtk::MenuItem*                         m_MFColorRGBAMenuItem;
	Gtk::MenuItem*                         m_MFDoubleMenuItem;
	Gtk::MenuItem*                         m_MFFloatMenuItem;
	Gtk::MenuItem*                         m_MFImageMenuItem;
	Gtk::MenuItem*                         m_MFInt32MenuItem;
	Gtk::MenuItem*                         m_MFMatrix3dMenuItem;
	Gtk::MenuItem*                         m_MFMatrix3fMenuItem;
	Gtk::MenuItem*                         m_MFMatrix4dMenuItem;
	Gtk::MenuItem*                         m_MFMatrix4fMenuItem;
	Gtk::MenuItem*                         m_MFNodeMenuItem;
	Gtk::MenuItem*                         m_MFRotationMenuItem;
	Gtk::MenuItem*                         m_MFStringMenuItem;
	Gtk::MenuItem*                         m_MFTimeMenuItem;
	Gtk::MenuItem*                         m_MFVec2dMenuItem;
	Gtk::MenuItem*                         m_MFVec2fMenuItem;
	Gtk::MenuItem*                         m_MFVec3dMenuItem;
	Gtk::MenuItem*                         m_MFVec3fMenuItem;
	Gtk::MenuItem*                         m_MFVec4dMenuItem;
	Gtk::MenuItem*                         m_MFVec4fMenuItem;
	Gtk::Dialog*                           m_Window;
	Gtk::Button*                           m_CancelButton;
	Gtk::Button*                           m_CancelButton1;
	Gtk::Button*                           m_OkButton;
	Gtk::Box*                              m_Widget;
	Gtk::Label*                            m_HeaderLabel;
	Gtk::Expander*                         m_NodePropertiesExpander;
	Gtk::Label*                            m_TypeNameLabel;
	Gtk::Label*                            m_NameLabel;
	Gtk::Entry*                            m_TypeNameEntry;
	Gtk::Entry*                            m_NameEntry;
	Gtk::Expander*                         m_CommonPropertiesExpander;
	Gtk::Entry*                            m_ComponentEntry;
	Gtk::Entry*                            m_ContainerFieldEntry;
	Gtk::Expander*                         m_UserDefinedFieldsExpander;
	Gtk::TreeView*                         m_UserDefinedFieldsTreeView;
	Gtk::Button*                           m_AddFieldButton;
	Gtk::Button*                           m_RemoveFieldButton;
	Gtk::Expander*                         m_CDataFieldExpander;
	Gtk::Button*                           m_EditCDataButton;
	Gtk::Dialog*                           m_AddFieldDialog;
	Gtk::Button*                           m_AddFieldCancelButton;
	Gtk::Button*                           m_AddFieldOkButton;
	Gtk::MenuButton*                       m_AccessTypeMenuButton;
	Gtk::Label*                            m_AccessTypeLabel;
	Gtk::MenuButton*                       m_FieldTypeMenuButton;
	Gtk::Label*                            m_FieldTypeLabel;
	Gtk::Entry*                            m_FieldNameEntry;

};

} // puck
} // titania

#endif
