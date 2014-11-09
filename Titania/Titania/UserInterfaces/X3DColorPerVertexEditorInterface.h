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
#ifndef __TMP_GLAD2CPP_COLOR_PER_VERTEX_EDITOR_H__
#define __TMP_GLAD2CPP_COLOR_PER_VERTEX_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DColorPerVertexEditorInterface :
	public X3DEditorInterface
{
public:

	X3DColorPerVertexEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DColorPerVertexEditorInterface (const std::string & filename, const Arguments & ... arguments) :
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
	getColorAdjustment () const
	{ return m_ColorAdjustment; }

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

	Gtk::Box &
	getPreviewBox () const
	{ return *m_PreviewBox; }

	Gtk::MenuToolButton &
	getShadingButton () const
	{ return *m_ShadingButton; }

	Gtk::ToolButton &
	getLookAtButton () const
	{ return *m_LookAtButton; }

	Gtk::ToggleButton &
	getSelectColorButton () const
	{ return *m_SelectColorButton; }

	Gtk::Button &
	getColorButton () const
	{ return *m_ColorButton; }

	Gtk::Button &
	getAddColorButton () const
	{ return *m_AddColorButton; }

	Gtk::Button &
	getRemoveColorButton () const
	{ return *m_RemoveColorButton; }

	Gtk::RadioButton &
	getSingleVertexButton () const
	{ return *m_SingleVertexButton; }

	Gtk::RadioButton &
	getAdjacentVerticesButton () const
	{ return *m_AdjacentVerticesButton; }

	Gtk::RadioButton &
	getSingleFaceButton () const
	{ return *m_SingleFaceButton; }

	Gtk::RadioButton &
	getWholeObjectButton () const
	{ return *m_WholeObjectButton; }

	Gtk::ScrolledWindow &
	getColorsScrolledWindow () const
	{ return *m_ColorsScrolledWindow; }

	Gtk::Menu &
	getShadingMenu () const
	{ return *m_ShadingMenu; }

	Gtk::RadioMenuItem &
	getPhongMenuItem () const
	{ return *m_PhongMenuItem; }

	Gtk::RadioMenuItem &
	getGouraudMenuItem () const
	{ return *m_GouraudMenuItem; }

	Gtk::RadioMenuItem &
	getFlatMenuItem () const
	{ return *m_FlatMenuItem; }

	Gtk::RadioMenuItem &
	getWireFrameMenuItem () const
	{ return *m_WireFrameMenuItem; }

	Gtk::RadioMenuItem &
	getPointSetMenuItem () const
	{ return *m_PointSetMenuItem; }

	virtual
	void
	on_undo_activate () = 0;

	virtual
	void
	on_redo_activate () = 0;

	virtual
	void
	on_look_at_all_clicked () = 0;

	virtual
	void
	on_single_vertex_clicked () = 0;

	virtual
	void
	on_adjacent_vertices_clicked () = 0;

	virtual
	void
	on_single_face_clicked () = 0;

	virtual
	void
	on_whole_object_clicked () = 0;

	virtual
	void
	on_phong_activate () = 0;

	virtual
	void
	on_gouraud_activate () = 0;

	virtual
	void
	on_flat_activate () = 0;

	virtual
	void
	on_wireframe_activate () = 0;

	virtual
	void
	on_pointset_activate () = 0;

	virtual
	~X3DColorPerVertexEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_ColorAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::ImageMenuItem*            m_UndoMenuItem;
	Gtk::ImageMenuItem*            m_RedoMenuItem;
	Gtk::Box*                      m_PreviewBox;
	Gtk::MenuToolButton*           m_ShadingButton;
	Gtk::ToolButton*               m_LookAtButton;
	Gtk::ToggleButton*             m_SelectColorButton;
	Gtk::Button*                   m_ColorButton;
	Gtk::Button*                   m_AddColorButton;
	Gtk::Button*                   m_RemoveColorButton;
	Gtk::RadioButton*              m_SingleVertexButton;
	Gtk::RadioButton*              m_AdjacentVerticesButton;
	Gtk::RadioButton*              m_SingleFaceButton;
	Gtk::RadioButton*              m_WholeObjectButton;
	Gtk::ScrolledWindow*           m_ColorsScrolledWindow;
	Gtk::Menu*                     m_ShadingMenu;
	Gtk::RadioMenuItem*            m_PhongMenuItem;
	Gtk::RadioMenuItem*            m_GouraudMenuItem;
	Gtk::RadioMenuItem*            m_FlatMenuItem;
	Gtk::RadioMenuItem*            m_WireFrameMenuItem;
	Gtk::RadioMenuItem*            m_PointSetMenuItem;

};

} // puck
} // titania

#endif
