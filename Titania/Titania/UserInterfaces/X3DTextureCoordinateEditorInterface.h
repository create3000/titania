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
#ifndef __TMP_GLAD2CPP_TEXTURE_COORDINATE_EDITOR_H__
#define __TMP_GLAD2CPP_TEXTURE_COORDINATE_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DTextureCoordinateEditorInterface :
	public X3DEditorInterface
{
public:

	X3DTextureCoordinateEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DTextureCoordinateEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const Glib::ustring & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	void
	updateWidgets (const std::vector <Glib::ustring> & names) const
	{ getBuilder () -> add_from_file (filename, names); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureStageAdjustment () const
	{ return m_TextureStageAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::ImageMenuItem &
	getUndoMenuItem () const
	{ return *m_UndoMenuItem; }

	Gtk::ImageMenuItem &
	getRedoMenuItem () const
	{ return *m_RedoMenuItem; }

	Gtk::MenuItem &
	getSelectionMenuItem () const
	{ return *m_SelectionMenuItem; }

	Gtk::ImageMenuItem &
	getSelectAllMenuItem () const
	{ return *m_SelectAllMenuItem; }

	Gtk::MenuItem &
	getDeselectAllMenuItem () const
	{ return *m_DeselectAllMenuItem; }

	Gtk::Box &
	getLeftBox () const
	{ return *m_LeftBox; }

	Gtk::RadioToolButton &
	getLeftHandButton () const
	{ return *m_LeftHandButton; }

	Gtk::RadioToolButton &
	getLeftArrowButton () const
	{ return *m_LeftArrowButton; }

	Gtk::ToolButton &
	getLeftLookAtButton () const
	{ return *m_LeftLookAtButton; }

	Gtk::Box &
	getRightBox () const
	{ return *m_RightBox; }

	Gtk::RadioToolButton &
	getRightHandButton () const
	{ return *m_RightHandButton; }

	Gtk::RadioToolButton &
	getRightArrowButton () const
	{ return *m_RightArrowButton; }

	Gtk::ToolButton &
	getRightLookAtAllButton () const
	{ return *m_RightLookAtAllButton; }

	Gtk::ToggleToolButton &
	getRightLookAtButton () const
	{ return *m_RightLookAtButton; }

	Gtk::Button &
	getRemoveButton () const
	{ return *m_RemoveButton; }

	Gtk::Button &
	getApplyButton () const
	{ return *m_ApplyButton; }

	virtual
	void
	on_texture_stage_changed () = 0;

	virtual
	void
	on_undo_activate () = 0;

	virtual
	void
	on_redo_activate () = 0;

	virtual
	void
	on_select_all_activate () = 0;

	virtual
	void
	on_deselect_all_activate () = 0;

	virtual
	void
	on_left_hand_toggled () = 0;

	virtual
	void
	on_left_arrow_toggled () = 0;

	virtual
	void
	on_left_look_at_all_clicked () = 0;

	virtual
	void
	on_right_hand_toggled () = 0;

	virtual
	void
	on_right_arrow_toggled () = 0;

	virtual
	void
	on_right_look_at_all_clicked () = 0;

	virtual
	void
	on_right_look_at_toggled () = 0;

	virtual
	void
	on_remove_clicked () = 0;

	virtual
	void
	on_apply_clicked () = 0;

	virtual
	~X3DTextureCoordinateEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_TextureStageAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::ImageMenuItem*            m_UndoMenuItem;
	Gtk::ImageMenuItem*            m_RedoMenuItem;
	Gtk::MenuItem*                 m_SelectionMenuItem;
	Gtk::ImageMenuItem*            m_SelectAllMenuItem;
	Gtk::MenuItem*                 m_DeselectAllMenuItem;
	Gtk::Box*                      m_LeftBox;
	Gtk::RadioToolButton*          m_LeftHandButton;
	Gtk::RadioToolButton*          m_LeftArrowButton;
	Gtk::ToolButton*               m_LeftLookAtButton;
	Gtk::Box*                      m_RightBox;
	Gtk::RadioToolButton*          m_RightHandButton;
	Gtk::RadioToolButton*          m_RightArrowButton;
	Gtk::ToolButton*               m_RightLookAtAllButton;
	Gtk::ToggleToolButton*         m_RightLookAtButton;
	Gtk::Button*                   m_RemoveButton;
	Gtk::Button*                   m_ApplyButton;

};

} // puck
} // titania

#endif
