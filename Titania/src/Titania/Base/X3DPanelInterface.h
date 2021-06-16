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

#ifndef __TITANIA_BASE_X3DPANEL_INTERFACE_H__
#define __TITANIA_BASE_X3DPANEL_INTERFACE_H__

#include "../Base/X3DUserInterface.h"

#include "../BrowserNotebook/NotebookPage/PanelType.h"

namespace titania {
namespace puck {

class NotebookPage;
class PanelMenu;

class X3DPanelInterface :
	public X3DUserInterface
{
public:

	/// @name Member access

	const X3D::SFEnum <PanelType> &
	getPanelType () const;

	virtual
	Gtk::MenuItem &
	getPanelsMenuItem () const = 0;

	const std::unique_ptr <PanelMenu> &
	getPanelMenu () const
	{ return panelMenu; }

	void
	setFocus (const bool value);

	const X3D::SFBool &
	hasFocus () const
	{ return focus; }

	virtual
	void
	lookAtSelection () = 0;

	virtual
	void
	lookAtAll () = 0;

	/// @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DPanelInterface () override;


protected:

	/// @name Construction

	X3DPanelInterface (NotebookPage* const page, const PanelType panelType, const size_t id);

	X3DPanelInterface ();

	virtual
	void
	initialize () override;

	/// @name Member access

	NotebookPage*
	getPage () const
	{ return page; }

	size_t
	getId () const
	{ return id; }

	void
	addFocusWidget (Gtk::Widget* const widget);

	void
	removeFocusWidget (Gtk::Widget* const widget);


private:

	/// @name Event handlers

	void
	set_focus ();

	bool
	on_button_press_event (GdkEventButton* event);

	///  @name Member types

	using FocusWidgets = std::map <Gtk::Widget*, sigc::connection>;

	///  @name Members

	NotebookPage* const         page;
	const size_t                id;
	std::unique_ptr <PanelMenu> panelMenu;
	X3D::SFBool                 focus;
	FocusWidgets                focusWidgets;

};

} // puck
} // titania

#endif
