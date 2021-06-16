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

#ifndef __TMP_GLAD2CPP_ROUTE_GRAPH_H__
#define __TMP_GLAD2CPP_ROUTE_GRAPH_H__

#include "../Base/X3DUserInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for RouteGraph.
 */
class X3DRouteGraphInterface :
	public X3DUserInterface
{
public:

	///  @name Construction

	X3DRouteGraphInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DRouteGraphInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DRouteGraphInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getHAdjustment () const
	{ return m_HAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getVAdjustment () const
	{ return m_VAdjustment; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::MenuBar &
	getMenuBar () const
	{ return *m_MenuBar; }

	Gtk::MenuItem &
	getRouteGraphMenuItem () const
	{ return *m_RouteGraphMenuItem; }

	Gtk::ImageMenuItem &
	getNewPageMenuItem () const
	{ return *m_NewPageMenuItem; }

	Gtk::ImageMenuItem &
	getRenamePageMenuItem () const
	{ return *m_RenamePageMenuItem; }

	Gtk::CheckMenuItem &
	getAddConnectedNodesMenuItem () const
	{ return *m_AddConnectedNodesMenuItem; }

	Gtk::MenuItem &
	getAlignToGridMenuItem () const
	{ return *m_AlignToGridMenuItem; }

	Gtk::ImageMenuItem &
	getExportPageMenuItem () const
	{ return *m_ExportPageMenuItem; }

	Gtk::ImageMenuItem &
	getClosePageMenuItem () const
	{ return *m_ClosePageMenuItem; }

	Gtk::MenuItem &
	getEditMenuItem () const
	{ return *m_EditMenuItem; }

	Gtk::ImageMenuItem &
	getDeleteMenuItem () const
	{ return *m_DeleteMenuItem; }

	Gtk::ImageMenuItem &
	getSelectAllMenuItem () const
	{ return *m_SelectAllMenuItem; }

	Gtk::MenuItem &
	getDeselectAllMenuItem () const
	{ return *m_DeselectAllMenuItem; }

	Gtk::Toolbar &
	getEditToolBar () const
	{ return *m_EditToolBar; }

	Gtk::ToolButton &
	getNewLogicButton () const
	{ return *m_NewLogicButton; }

	Gtk::ToolButton &
	getRenamePageButton () const
	{ return *m_RenamePageButton; }

	Gtk::ToolButton &
	getDeleteButton () const
	{ return *m_DeleteButton; }

	Gtk::ToolButton &
	getClosePageButton () const
	{ return *m_ClosePageButton; }

	Gtk::Notebook &
	getNotebook () const
	{ return *m_Notebook; }

	///  @name Signal handlers

	virtual
	void
	on_new_page_activate () = 0;

	virtual
	void
	on_rename_page_activate () = 0;

	virtual
	void
	on_align_to_grid_activate () = 0;

	virtual
	void
	on_export_page_activate () = 0;

	virtual
	void
	on_close_page_activate () = 0;

	virtual
	void
	on_delete_activate () = 0;

	virtual
	void
	on_select_all_activate () = 0;

	virtual
	void
	on_deselect_all_activate () = 0;

	virtual
	void
	on_new_page_clicked () = 0;

	virtual
	void
	on_rename_page_clicked () = 0;

	virtual
	void
	on_delete_clicked () = 0;

	virtual
	void
	on_close_page_clicked () = 0;

	virtual
	void
	on_page_reordered (Gtk::Widget* page, guint page_num) = 0;

	virtual
	void
	on_switch_page (Gtk::Widget* page, guint page_number) = 0;

	///  @name Destruction

	virtual
	~X3DRouteGraphInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_HAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_VAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::MenuBar* m_MenuBar;
	Gtk::MenuItem* m_RouteGraphMenuItem;
	Gtk::ImageMenuItem* m_NewPageMenuItem;
	Gtk::ImageMenuItem* m_RenamePageMenuItem;
	Gtk::CheckMenuItem* m_AddConnectedNodesMenuItem;
	Gtk::MenuItem* m_AlignToGridMenuItem;
	Gtk::ImageMenuItem* m_ExportPageMenuItem;
	Gtk::ImageMenuItem* m_ClosePageMenuItem;
	Gtk::MenuItem* m_EditMenuItem;
	Gtk::ImageMenuItem* m_DeleteMenuItem;
	Gtk::ImageMenuItem* m_SelectAllMenuItem;
	Gtk::MenuItem* m_DeselectAllMenuItem;
	Gtk::Toolbar* m_EditToolBar;
	Gtk::ToolButton* m_NewLogicButton;
	Gtk::ToolButton* m_RenamePageButton;
	Gtk::ToolButton* m_DeleteButton;
	Gtk::ToolButton* m_ClosePageButton;
	Gtk::Notebook* m_Notebook;

};

} // puck
} // titania

#endif
