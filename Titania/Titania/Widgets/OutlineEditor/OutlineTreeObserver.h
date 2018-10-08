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

#ifndef __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_TREE_OBSERVER_H__
#define __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_TREE_OBSERVER_H__

#include <gtkmm.h>

#include <Titania/X3D.h>

namespace titania {
namespace puck {

class X3DOutlineTreeView;

class OutlineTreeObserver :
	public X3D::X3DInput
{
public:

	///  @name Construction

	OutlineTreeObserver (X3DOutlineTreeView* const);

	///  @name Operations

	void
	watch (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	void
	watch_child (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	void
	unwatch_tree (const Gtk::TreeModel::iterator &, const bool = true);

	virtual
	~OutlineTreeObserver () final override;


private:

	///  @name Operations

	void
	watch_children (const Gtk::TreeModel::iterator &);

	void
	unwatch_child (const Gtk::TreeModel::iterator &, const bool);

	void
	clear_open_path (const Gtk::TreeModel::iterator &);

	void
	on_row_has_child_toggled (const Gtk::TreeModel::Path &, const bool);

	void
	on_field_value_changed (const Gtk::TreeModel::Path &);

	void
	on_row_changed (const Gtk::TreeModel::Path &);

	void
	toggle_path (const Gtk::TreeModel::Path &);

//	bool
//	getToggle (const Gtk::TreeModel::iterator &) const;

	///  @name Members

	X3DOutlineTreeView* const treeView;

};

} // puck
} // titania

#endif
