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

#ifndef __TITANIA_VIEWPOINT_LIST_VIEWPOINT_LIST_H__
#define __TITANIA_VIEWPOINT_LIST_VIEWPOINT_LIST_H__

#include "../../UserInterfaces/X3DViewpointListInterface.h"

#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Execution/BindableNodeStack.h>

namespace titania {
namespace puck {

class AdjustmentObject;

class ViewpointList :
	public X3DViewpointListInterface
{
public:

	///  @name Construction

	ViewpointList (X3DBrowserWindow* const);

	///  @name Member acccess

	void
	isEditor (const bool);

	bool
	isEditor () const
	{ return editor; }

	///  @name Destruction

	virtual
	~ViewpointList ();


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Member acccess

	const X3D::ViewpointStackPtr &
	getStack ();

	const X3D::ViewpointListPtr &
	getList () const;

	///  @name Event handlers

	void
	set_browser (const X3D::BrowserPtr &);

	void
	set_activeLayer ();

	void
	set_viewpoints ();

	void
	set_currentViewpoint ();

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) final override;

	virtual
	bool
	on_button_release_event (GdkEventButton*) final override;

	void
	on_bind_toggled (const Gtk::TreePath &);

	static
	void
	set_adjustment (const Glib::RefPtr <Gtk::Adjustment> &, const double);

	///  @name Members

	X3D::BrowserPtr      browser;
	X3D::X3DLayerNodePtr activeLayer;
	bool                 editor;

	std::unique_ptr <AdjustmentObject> hadjustment;
	std::unique_ptr <AdjustmentObject> vadjustment;

};

} // puck
} // titania

#endif
