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

#ifndef __TITANIA_EDITORS_PROJECTS_EDITOR_PROJECTS_EDITOR_H__
#define __TITANIA_EDITORS_PROJECTS_EDITOR_PROJECTS_EDITOR_H__

#include "../../UserInterfaces/X3DProjectsEditorInterface.h"

namespace titania {
namespace puck {

class BrowserWindow;
class ScrollFreezer;

class ProjectsEditor :
	virtual public X3DProjectsEditorInterface
{
public:

	///  @name Construction

	ProjectsEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~ProjectsEditor () final override;


private:

	///  Member types

	class FolderElement;

	using FolderElementPtr = std::shared_ptr <FolderElement>;

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	///  @name Event handlers

	virtual
	void
	on_map () final override;

	virtual
	void
	on_unmap () final override;

	virtual
	void
	on_add_project_clicked () final override;

	virtual
	void
	on_remove_project_clicked () final override;

	virtual
	bool
	on_button_press_event (GdkEventButton* event) final override;

	void
	on_open_with_activate (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file);

	virtual
	void
	on_add_new_file_activate () final override;

	virtual
	void
	on_add_new_folder_activate () final override;

	virtual
	void
	on_create_folder_clicked () final override;

	virtual
	bool
	on_folder_name_key_press_event (GdkEventKey* event) final override;

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) final override;

	virtual
	void
	on_selection_changed () final override;

	virtual
	bool
	on_test_expand_row (const Gtk::TreeIter & iter, const Gtk::TreePath & path) final override;

	void
	on_file_changed (const Glib::RefPtr <Gio::File> & file,
	                 const Glib::RefPtr <Gio::File> & other_file,
	                 Gio::FileMonitorEvent event);

	void
	set_execution_context ();

	///  @name Operations

	bool
	selectFile (const Glib::RefPtr <Gio::File> & file);

	bool
	expandTo (const Glib::RefPtr <Gio::File> & file);

	void
	addRootFolder (const std::string & path);

	void
	addFolder (const Gtk::TreeModel::iterator & iter, const Glib::RefPtr <Gio::File> & folder, const bool expander);

	void
	addFolder (const Glib::RefPtr <Gio::File> & folder);

	void
	addChildren (const Gtk::TreeModel::iterator & parent, const Glib::RefPtr <Gio::File> & folder);

	void
	addChild (const Gtk::TreeModel::iterator & iter, const Glib::RefPtr <Gio::File> & file, const std::string & defaultIcon);

	void
	removeRootFolder (const Gtk::TreeModel::iterator & iter);

	void
	removeFolder (const Gtk::TreeModel::iterator & iter);

	void
	removeChildren (const Gtk::TreeIter & iter);

	Gtk::TreeIter
	getIter (const std::string & URL) const;
	
	bool
	getIter (const Gtk::TreeIter & iter, const std::string & URL, Gtk::TreeIter & result) const;

	std::string
	getPath (const Gtk::TreeIter & iter) const;

	void
	launchFile (const std::string & path);

	///  @name Destruction

	void
	restoreExpanded ();

	void
	saveExpanded ();

	void
	getExpanded (const Gtk::TreeModel::Children & children, X3D::MFString & folders);

	virtual
	void
	store () final override;

	///  @name Members

	std::set <std::string>                   projects;
	std::map <std::string, FolderElementPtr> folders;
	std::unique_ptr <ScrollFreezer>          scrollFreezer;

};

} // puck
} // titania

#endif
