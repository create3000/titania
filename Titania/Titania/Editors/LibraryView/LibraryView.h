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

#ifndef __TITANIA_EDITORS_LIBRARY_VIEW_LIBRARY_VIEW_H__
#define __TITANIA_EDITORS_LIBRARY_VIEW_LIBRARY_VIEW_H__

#include "../../UserInterfaces/X3DLibraryViewInterface.h"

namespace titania {
namespace puck {

class ScrollFreezer;

class LibraryView :
	virtual public X3DLibraryViewInterface
{
public:

	///  @name Construction

	LibraryView (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~LibraryView () override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	///  @name Destruction

	virtual
	void
	store () final override;


private:

	///  @name Selection handling

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) final override;

	///  @name Folder handling handling

	void
	setRootFolder (const Glib::RefPtr <Gio::File> & folder);

	void
	addFolder (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & folder);

	void
	addChildren (const Gtk::TreeIter & parentIter, const Glib::RefPtr <Gio::File> & folder);

	void
	addChild (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & file, const std::string & defaultIcon);

	Gtk::TreeIter
	getIter (const std::string & URL) const;

	bool
	getIter (const Gtk::TreeIter & iter, const std::string & URL, Gtk::TreeIter & result) const;

	std::string
	getPath (const Gtk::TreeIter & iter) const;

	///  @name Expanded handling

	void
	restoreExpanded ();

	void
	saveExpanded ();

	void
	getExpanded (const Gtk::TreeModel::Children & children, X3D::MFString & folders);

	// Members

	std::unique_ptr <ScrollFreezer> scrollFreezer;

};

} // puck
} // titania

#endif
