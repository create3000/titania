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

#ifndef __TMP_GLAD2CPP_LAYER_EDITOR_H__
#define __TMP_GLAD2CPP_LAYER_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for LayerEditor.
 */
class X3DLayerEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DLayerEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DLayerEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DLayerEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::ListStore> &
	getLayerListStore () const
	{ return m_LayerListStore; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getLayerSelection () const
	{ return m_LayerSelection; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getVisibilityColumn () const
	{ return m_VisibilityColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getPickableColumn () const
	{ return m_PickableColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getTypeNameColumn () const
	{ return m_TypeNameColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getNameColumn () const
	{ return m_NameColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getSpacerColumn () const
	{ return m_SpacerColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getActiveLayerColumn () const
	{ return m_ActiveLayerColumn; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Button &
	getNewLayerSetButton () const
	{ return *m_NewLayerSetButton; }

	Gtk::Button &
	getRemoveLayerSetButton () const
	{ return *m_RemoveLayerSetButton; }

	Gtk::Button &
	getIndexButton () const
	{ return *m_IndexButton; }

	Gtk::Expander &
	getLayerSetExpander () const
	{ return *m_LayerSetExpander; }

	Gtk::ScrolledWindow &
	getLayerScrolledWindow () const
	{ return *m_LayerScrolledWindow; }

	Gtk::TreeView &
	getLayerTreeView () const
	{ return *m_LayerTreeView; }

	Gtk::Box &
	getLayerNameBox () const
	{ return *m_LayerNameBox; }

	Gtk::Entry &
	getLayerNameEntry () const
	{ return *m_LayerNameEntry; }

	Gtk::Button &
	getLayerRenameButton () const
	{ return *m_LayerRenameButton; }

	Gtk::Box &
	getLayerActionBox () const
	{ return *m_LayerActionBox; }

	Gtk::Button &
	getNewLayerPopupButton () const
	{ return *m_NewLayerPopupButton; }

	Gtk::Button &
	getRemoveLayerButton () const
	{ return *m_RemoveLayerButton; }

	Gtk::Box &
	getMoveLayerBox () const
	{ return *m_MoveLayerBox; }

	Gtk::Button &
	getTopButton () const
	{ return *m_TopButton; }

	Gtk::Button &
	getUpButton () const
	{ return *m_UpButton; }

	Gtk::Button &
	getDownButton () const
	{ return *m_DownButton; }

	Gtk::Button &
	getBottomButton () const
	{ return *m_BottomButton; }

	Gtk::Label &
	getLayerSetLabel () const
	{ return *m_LayerSetLabel; }

	Gtk::Popover &
	getNewLayerPopover () const
	{ return *m_NewLayerPopover; }

	Gtk::Button &
	getNewLayerButton () const
	{ return *m_NewLayerButton; }

	Gtk::Button &
	getNewLayoutLayerButton () const
	{ return *m_NewLayoutLayerButton; }

	///  @name Signal handlers

	virtual
	void
	on_new_layer_set_button_clicked () = 0;

	virtual
	void
	on_remove_layer_set_clicked () = 0;

	virtual
	void
	on_index_clicked () = 0;

	virtual
	bool
	on_layers_button_release_event (GdkEventButton* release_event) = 0;

	virtual
	void
	on_layer_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) = 0;

	virtual
	void
	on_layer_selection_changed () = 0;

	virtual
	void
	on_new_layer_popup_clicked () = 0;

	virtual
	void
	on_remove_layer_button_clicked () = 0;

	virtual
	void
	on_top_clicked () = 0;

	virtual
	void
	on_up_clicked () = 0;

	virtual
	void
	on_down_clicked () = 0;

	virtual
	void
	on_bottom_clicked () = 0;

	virtual
	void
	on_new_layer_clicked () = 0;

	virtual
	void
	on_new_layout_layer_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DLayerEditorInterface () override;


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
	Glib::RefPtr <Gtk::ListStore> m_LayerListStore;
	Glib::RefPtr <Gtk::TreeSelection> m_LayerSelection;
	Glib::RefPtr <Gtk::TreeViewColumn> m_VisibilityColumn;
	Glib::RefPtr <Gtk::TreeViewColumn> m_PickableColumn;
	Glib::RefPtr <Gtk::TreeViewColumn> m_TypeNameColumn;
	Glib::RefPtr <Gtk::TreeViewColumn> m_NameColumn;
	Glib::RefPtr <Gtk::TreeViewColumn> m_SpacerColumn;
	Glib::RefPtr <Gtk::TreeViewColumn> m_ActiveLayerColumn;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Button* m_NewLayerSetButton;
	Gtk::Button* m_RemoveLayerSetButton;
	Gtk::Button* m_IndexButton;
	Gtk::Expander* m_LayerSetExpander;
	Gtk::ScrolledWindow* m_LayerScrolledWindow;
	Gtk::TreeView* m_LayerTreeView;
	Gtk::Box* m_LayerNameBox;
	Gtk::Entry* m_LayerNameEntry;
	Gtk::Button* m_LayerRenameButton;
	Gtk::Box* m_LayerActionBox;
	Gtk::Button* m_NewLayerPopupButton;
	Gtk::Button* m_RemoveLayerButton;
	Gtk::Box* m_MoveLayerBox;
	Gtk::Button* m_TopButton;
	Gtk::Button* m_UpButton;
	Gtk::Button* m_DownButton;
	Gtk::Button* m_BottomButton;
	Gtk::Label* m_LayerSetLabel;
	Gtk::Popover* m_NewLayerPopover;
	Gtk::Button* m_NewLayerButton;
	Gtk::Button* m_NewLayoutLayerButton;

};

} // puck
} // titania

#endif
