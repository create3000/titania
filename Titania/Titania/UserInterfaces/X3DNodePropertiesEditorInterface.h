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

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getCellRendererType () const
	{ return m_cellRendererType; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getCellRendererName () const
	{ return m_cellRendererName; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getCellRendererAccessType () const
	{ return m_cellRendererAccessType; }

	Gtk::Menu &
	getAccessTypeMenu () const
	{ return *m_accessTypeMenu; }

	Gtk::MenuItem &
	getInitializeOnlyMenuItem () const
	{ return *m_initializeOnlyMenuItem; }

	Gtk::MenuItem &
	getInputOnlyMenuItem () const
	{ return *m_inputOnlyMenuItem; }

	Gtk::MenuItem &
	getOutputOnlyMenuItem () const
	{ return *m_outputOnlyMenuItem; }

	Gtk::MenuItem &
	getInputOutputMenuItem () const
	{ return *m_inputOutputMenuItem; }

	Gtk::Menu &
	getFieldTypeMenu () const
	{ return *m_fieldTypeMenu; }

	Gtk::MenuItem &
	getSFBoolMenuItem () const
	{ return *m_sFBoolMenuItem; }

	Gtk::MenuItem &
	getSFColorMenuItem () const
	{ return *m_sFColorMenuItem; }

	Gtk::MenuItem &
	getSFColorRGBAMenuItem () const
	{ return *m_sFColorRGBAMenuItem; }

	Gtk::MenuItem &
	getSFDoubleMenuItem () const
	{ return *m_sFDoubleMenuItem; }

	Gtk::MenuItem &
	getSFFloatMenuItem () const
	{ return *m_sFFloatMenuItem; }

	Gtk::MenuItem &
	getSFImageMenuItem () const
	{ return *m_sFImageMenuItem; }

	Gtk::MenuItem &
	getSFInt32MenuItem () const
	{ return *m_sFInt32MenuItem; }

	Gtk::MenuItem &
	getSFMatrix3fMenuItem () const
	{ return *m_sFMatrix3fMenuItem; }

	Gtk::MenuItem &
	getSFMatrix3dMenuItem () const
	{ return *m_sFMatrix3dMenuItem; }

	Gtk::MenuItem &
	getSFMatrix4fMenuItem () const
	{ return *m_sFMatrix4fMenuItem; }

	Gtk::MenuItem &
	getSFMatrix4dMenuItem () const
	{ return *m_sFMatrix4dMenuItem; }

	Gtk::MenuItem &
	getSFNodeMenuItem () const
	{ return *m_sFNodeMenuItem; }

	Gtk::MenuItem &
	getSFRotationMenuItem () const
	{ return *m_sFRotationMenuItem; }

	Gtk::MenuItem &
	getSFStringMenuItem () const
	{ return *m_sFStringMenuItem; }

	Gtk::MenuItem &
	getSFTimeMenuItem () const
	{ return *m_sFTimeMenuItem; }

	Gtk::MenuItem &
	getSFVec2fMenuItem () const
	{ return *m_sFVec2fMenuItem; }

	Gtk::MenuItem &
	getSFVec2dMenuItem () const
	{ return *m_sFVec2dMenuItem; }

	Gtk::MenuItem &
	getSFVec3fMenuItem () const
	{ return *m_sFVec3fMenuItem; }

	Gtk::MenuItem &
	getSFVec3dMenuItem () const
	{ return *m_sFVec3dMenuItem; }

	Gtk::MenuItem &
	getSFVec4fMenuItem () const
	{ return *m_sFVec4fMenuItem; }

	Gtk::MenuItem &
	getSFVec4dMenuItem () const
	{ return *m_sFVec4dMenuItem; }

	Gtk::MenuItem &
	getMFBoolMenuItem () const
	{ return *m_mFBoolMenuItem; }

	Gtk::MenuItem &
	getMFColorMenuItem () const
	{ return *m_mFColorMenuItem; }

	Gtk::MenuItem &
	getMFColorRGBAMenuItem () const
	{ return *m_mFColorRGBAMenuItem; }

	Gtk::MenuItem &
	getMFDoubleMenuItem () const
	{ return *m_mFDoubleMenuItem; }

	Gtk::MenuItem &
	getMFFloatMenuItem () const
	{ return *m_mFFloatMenuItem; }

	Gtk::MenuItem &
	getMFImageMenuItem () const
	{ return *m_mFImageMenuItem; }

	Gtk::MenuItem &
	getMFInt32MenuItem () const
	{ return *m_mFInt32MenuItem; }

	Gtk::MenuItem &
	getMFMatrix3fMenuItem () const
	{ return *m_mFMatrix3fMenuItem; }

	Gtk::MenuItem &
	getMFMatrix3dMenuItem () const
	{ return *m_mFMatrix3dMenuItem; }

	Gtk::MenuItem &
	getMFMatrix4fMenuItem () const
	{ return *m_mFMatrix4fMenuItem; }

	Gtk::MenuItem &
	getMFMatrix4dMenuItem () const
	{ return *m_mFMatrix4dMenuItem; }

	Gtk::MenuItem &
	getMFNodeMenuItem () const
	{ return *m_mFNodeMenuItem; }

	Gtk::MenuItem &
	getMFRotationMenuItem () const
	{ return *m_mFRotationMenuItem; }

	Gtk::MenuItem &
	getMFStringMenuItem () const
	{ return *m_mFStringMenuItem; }

	Gtk::MenuItem &
	getMFTimeMenuItem () const
	{ return *m_mFTimeMenuItem; }

	Gtk::MenuItem &
	getMFVec2fMenuItem () const
	{ return *m_mFVec2fMenuItem; }

	Gtk::MenuItem &
	getMFVec2dMenuItem () const
	{ return *m_mFVec2dMenuItem; }

	Gtk::MenuItem &
	getMFVec3fMenuItem () const
	{ return *m_mFVec3fMenuItem; }

	Gtk::MenuItem &
	getMFVec3dMenuItem () const
	{ return *m_mFVec3dMenuItem; }

	Gtk::MenuItem &
	getMFVec4fMenuItem () const
	{ return *m_mFVec4fMenuItem; }

	Gtk::MenuItem &
	getMFVec4dMenuItem () const
	{ return *m_mFVec4dMenuItem; }

	Gtk::Dialog &
	getWindow () const
	{ return *m_window; }

	Gtk::Button &
	getCancelButton () const
	{ return *m_cancelButton; }

	Gtk::Button &
	getOkButton () const
	{ return *m_okButton; }

	Gtk::Box &
	getWidget () const
	{ return *m_widget; }

	Gtk::Label &
	getHeaderLabel () const
	{ return *m_headerLabel; }

	Gtk::Expander &
	getNodePropertiesExpander () const
	{ return *m_nodePropertiesExpander; }

	Gtk::Label &
	getTypeNameLabel () const
	{ return *m_typeNameLabel; }

	Gtk::Label &
	getNameLabel () const
	{ return *m_nameLabel; }

	Gtk::Entry &
	getTypeNameEntry () const
	{ return *m_typeNameEntry; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_nameEntry; }

	Gtk::Expander &
	getUserDefinedFieldsExpander () const
	{ return *m_userDefinedFieldsExpander; }

	Gtk::TreeView &
	getUserDefinedFieldsTreeView () const
	{ return *m_userDefinedFieldsTreeView; }

	Gtk::Button &
	getAddFieldButton () const
	{ return *m_addFieldButton; }

	Gtk::Button &
	getRemoveFieldButton () const
	{ return *m_removeFieldButton; }

	Gtk::Dialog &
	getAddFieldDialog () const
	{ return *m_addFieldDialog; }

	Gtk::Button &
	getAddFieldCancelButton () const
	{ return *m_addFieldCancelButton; }

	Gtk::Button &
	getAddFieldOkButton () const
	{ return *m_addFieldOkButton; }

	Gtk::MenuButton &
	getAccessTypeMenuButton () const
	{ return *m_accessTypeMenuButton; }

	Gtk::Label &
	getAccessTypeLabel () const
	{ return *m_accessTypeLabel; }

	Gtk::MenuButton &
	getFieldTypeMenuButton () const
	{ return *m_fieldTypeMenuButton; }

	Gtk::Label &
	getFieldTypeLabel () const
	{ return *m_fieldTypeLabel; }

	Gtk::Entry &
	getFieldNameEntry () const
	{ return *m_fieldNameEntry; }

	virtual
	void
	on_cancel () = 0;

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
	on_add_field_clicked () = 0;

	virtual
	void
	on_remove_field_clicked () = 0;

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
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_cellRendererType;
	Glib::RefPtr <Gtk::CellRendererText>   m_cellRendererName;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_cellRendererAccessType;
	Gtk::Menu*                             m_accessTypeMenu;
	Gtk::MenuItem*                         m_initializeOnlyMenuItem;
	Gtk::MenuItem*                         m_inputOnlyMenuItem;
	Gtk::MenuItem*                         m_outputOnlyMenuItem;
	Gtk::MenuItem*                         m_inputOutputMenuItem;
	Gtk::Menu*                             m_fieldTypeMenu;
	Gtk::MenuItem*                         m_sFBoolMenuItem;
	Gtk::MenuItem*                         m_sFColorMenuItem;
	Gtk::MenuItem*                         m_sFColorRGBAMenuItem;
	Gtk::MenuItem*                         m_sFDoubleMenuItem;
	Gtk::MenuItem*                         m_sFFloatMenuItem;
	Gtk::MenuItem*                         m_sFImageMenuItem;
	Gtk::MenuItem*                         m_sFInt32MenuItem;
	Gtk::MenuItem*                         m_sFMatrix3fMenuItem;
	Gtk::MenuItem*                         m_sFMatrix3dMenuItem;
	Gtk::MenuItem*                         m_sFMatrix4fMenuItem;
	Gtk::MenuItem*                         m_sFMatrix4dMenuItem;
	Gtk::MenuItem*                         m_sFNodeMenuItem;
	Gtk::MenuItem*                         m_sFRotationMenuItem;
	Gtk::MenuItem*                         m_sFStringMenuItem;
	Gtk::MenuItem*                         m_sFTimeMenuItem;
	Gtk::MenuItem*                         m_sFVec2fMenuItem;
	Gtk::MenuItem*                         m_sFVec2dMenuItem;
	Gtk::MenuItem*                         m_sFVec3fMenuItem;
	Gtk::MenuItem*                         m_sFVec3dMenuItem;
	Gtk::MenuItem*                         m_sFVec4fMenuItem;
	Gtk::MenuItem*                         m_sFVec4dMenuItem;
	Gtk::MenuItem*                         m_mFBoolMenuItem;
	Gtk::MenuItem*                         m_mFColorMenuItem;
	Gtk::MenuItem*                         m_mFColorRGBAMenuItem;
	Gtk::MenuItem*                         m_mFDoubleMenuItem;
	Gtk::MenuItem*                         m_mFFloatMenuItem;
	Gtk::MenuItem*                         m_mFImageMenuItem;
	Gtk::MenuItem*                         m_mFInt32MenuItem;
	Gtk::MenuItem*                         m_mFMatrix3fMenuItem;
	Gtk::MenuItem*                         m_mFMatrix3dMenuItem;
	Gtk::MenuItem*                         m_mFMatrix4fMenuItem;
	Gtk::MenuItem*                         m_mFMatrix4dMenuItem;
	Gtk::MenuItem*                         m_mFNodeMenuItem;
	Gtk::MenuItem*                         m_mFRotationMenuItem;
	Gtk::MenuItem*                         m_mFStringMenuItem;
	Gtk::MenuItem*                         m_mFTimeMenuItem;
	Gtk::MenuItem*                         m_mFVec2fMenuItem;
	Gtk::MenuItem*                         m_mFVec2dMenuItem;
	Gtk::MenuItem*                         m_mFVec3fMenuItem;
	Gtk::MenuItem*                         m_mFVec3dMenuItem;
	Gtk::MenuItem*                         m_mFVec4fMenuItem;
	Gtk::MenuItem*                         m_mFVec4dMenuItem;
	Gtk::Dialog*                           m_window;
	Gtk::Button*                           m_cancelButton;
	Gtk::Button*                           m_okButton;
	Gtk::Box*                              m_widget;
	Gtk::Label*                            m_headerLabel;
	Gtk::Expander*                         m_nodePropertiesExpander;
	Gtk::Label*                            m_typeNameLabel;
	Gtk::Label*                            m_nameLabel;
	Gtk::Entry*                            m_typeNameEntry;
	Gtk::Entry*                            m_nameEntry;
	Gtk::Expander*                         m_userDefinedFieldsExpander;
	Gtk::TreeView*                         m_userDefinedFieldsTreeView;
	Gtk::Button*                           m_addFieldButton;
	Gtk::Button*                           m_removeFieldButton;
	Gtk::Dialog*                           m_addFieldDialog;
	Gtk::Button*                           m_addFieldCancelButton;
	Gtk::Button*                           m_addFieldOkButton;
	Gtk::MenuButton*                       m_accessTypeMenuButton;
	Gtk::Label*                            m_accessTypeLabel;
	Gtk::MenuButton*                       m_fieldTypeMenuButton;
	Gtk::Label*                            m_fieldTypeLabel;
	Gtk::Entry*                            m_fieldNameEntry;

};

} // puck
} // titania

#endif
