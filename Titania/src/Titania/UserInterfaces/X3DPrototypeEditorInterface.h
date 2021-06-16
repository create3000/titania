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

#ifndef __TMP_GLAD2CPP_PROTOTYPE_EDITOR_H__
#define __TMP_GLAD2CPP_PROTOTYPE_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for PrototypeEditor.
 */
class X3DPrototypeEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DPrototypeEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DPrototypeEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DPrototypeEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getURLCellRendererText () const
	{ return m_URLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getURLChooserColumn () const
	{ return m_URLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getURLCellrendererPixbuf () const
	{ return m_URLCellrendererPixbuf; }

	Gtk::Menu &
	getSelectPrototypeMenu () const
	{ return *m_SelectPrototypeMenu; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Image &
	getEditPrototypeImage () const
	{ return *m_EditPrototypeImage; }

	Gtk::Button &
	getCreateProtoPopupButton () const
	{ return *m_CreateProtoPopupButton; }

	Gtk::Button &
	getImportExternProtoButton () const
	{ return *m_ImportExternProtoButton; }

	Gtk::Button &
	getCreateInstanceButton () const
	{ return *m_CreateInstanceButton; }

	Gtk::MenuButton &
	getMenuButton () const
	{ return *m_MenuButton; }

	Gtk::Image &
	getPrototypeImage () const
	{ return *m_PrototypeImage; }

	Gtk::Label &
	getPrototypeLabel () const
	{ return *m_PrototypeLabel; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Button &
	getConvertProtoButton () const
	{ return *m_ConvertProtoButton; }

	Gtk::Button &
	getFoldExternProtoBackButton () const
	{ return *m_FoldExternProtoBackButton; }

	Gtk::Notebook &
	getPrototypeNotebook () const
	{ return *m_PrototypeNotebook; }

	Gtk::Expander &
	getInterfaceExpander () const
	{ return *m_InterfaceExpander; }

	Gtk::Box &
	getInterfaceBox () const
	{ return *m_InterfaceBox; }

	Gtk::ScrolledWindow &
	getURLScrolledWindow () const
	{ return *m_URLScrolledWindow; }

	Gtk::TreeView &
	getURLTreeView () const
	{ return *m_URLTreeView; }

	Gtk::Button &
	getURLAddButton () const
	{ return *m_URLAddButton; }

	Gtk::Button &
	getURLRemoveButton () const
	{ return *m_URLRemoveButton; }

	Gtk::Button &
	getURLReloadButton () const
	{ return *m_URLReloadButton; }

	Gtk::Box &
	getInstancesBox () const
	{ return *m_InstancesBox; }

	Gtk::Button &
	getUpdateInstancesButton () const
	{ return *m_UpdateInstancesButton; }

	Gtk::Popover &
	getCreateProtoPopover () const
	{ return *m_CreateProtoPopover; }

	Gtk::Button &
	getNewExternProtoButton () const
	{ return *m_NewExternProtoButton; }

	Gtk::Label &
	getCreateExternProtoButton () const
	{ return *m_CreateExternProtoButton; }

	Gtk::Button &
	getNewProtoButton () const
	{ return *m_NewProtoButton; }

	Gtk::Label &
	getCreatePrototypeButton () const
	{ return *m_CreatePrototypeButton; }

	///  @name Signal handlers

	virtual
	void
	on_create_proto_popup_clicked () = 0;

	virtual
	void
	on_import_extern_proto_clicked () = 0;

	virtual
	void
	on_create_instance_clicked () = 0;

	virtual
	void
	on_convert_prototype_clicked () = 0;

	virtual
	void
	on_fold_extern_proto_back_clicked () = 0;

	virtual
	void
	on_update_instances_clicked () = 0;

	virtual
	void
	on_create_externproto_clicked () = 0;

	virtual
	void
	on_create_proto_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DPrototypeEditorInterface () override;


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
	Glib::RefPtr <Gtk::CellRendererText> m_URLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_URLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_URLCellrendererPixbuf;
	Gtk::Menu* m_SelectPrototypeMenu;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Image* m_EditPrototypeImage;
	Gtk::Button* m_CreateProtoPopupButton;
	Gtk::Button* m_ImportExternProtoButton;
	Gtk::Button* m_CreateInstanceButton;
	Gtk::MenuButton* m_MenuButton;
	Gtk::Image* m_PrototypeImage;
	Gtk::Label* m_PrototypeLabel;
	Gtk::Box* m_NameBox;
	Gtk::Entry* m_NameEntry;
	Gtk::Button* m_RenameButton;
	Gtk::Button* m_ConvertProtoButton;
	Gtk::Button* m_FoldExternProtoBackButton;
	Gtk::Notebook* m_PrototypeNotebook;
	Gtk::Expander* m_InterfaceExpander;
	Gtk::Box* m_InterfaceBox;
	Gtk::ScrolledWindow* m_URLScrolledWindow;
	Gtk::TreeView* m_URLTreeView;
	Gtk::Button* m_URLAddButton;
	Gtk::Button* m_URLRemoveButton;
	Gtk::Button* m_URLReloadButton;
	Gtk::Box* m_InstancesBox;
	Gtk::Button* m_UpdateInstancesButton;
	Gtk::Popover* m_CreateProtoPopover;
	Gtk::Button* m_NewExternProtoButton;
	Gtk::Label* m_CreateExternProtoButton;
	Gtk::Button* m_NewProtoButton;
	Gtk::Label* m_CreatePrototypeButton;

};

} // puck
} // titania

#endif
