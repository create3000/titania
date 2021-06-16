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

#ifndef __TITANIA_WIDGETS_NOTEBOOK_PAGE_NOTEBOOK_PAGE_H__
#define __TITANIA_WIDGETS_NOTEBOOK_PAGE_NOTEBOOK_PAGE_H__

#include "X3DNotebookPage.h"

#include "PanelType.h"

namespace titania {
namespace puck {

class X3DPanelInterface;

class NotebookPage :
	virtual public X3DNotebookPageInterface,
	public X3DNotebookPage
{
public:

	///  @name Member types

	using PanelPtr   = std::shared_ptr <X3DPanelInterface>;
	using PanelArray = std::vector <PanelPtr>;

	///  @name Construction

	NotebookPage (X3DBrowserWindow* const browserWindow, const basic::uri & startUrl);

	///  @name Member access

	const PanelPtr &
	getActivePanel () const
	{ return panels [activeView]; }

	const PanelArray &
	getPanels () const
	{ return panels; }

	void
	setMultiView (const bool value, const bool modify);

	const X3D::SFBool &
	getMultiView () const
	{ return multiView; }

	void
	setBrowserRatioSet (const bool value);

	bool
	getBrowserRatioSet () const;

	void
	setBrowserRatio (const double value);

	double
	getBrowserRatio () const;

	///  @name Operations

	void
	lookAtSelection ();

	void
	lookAtAll ();

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~NotebookPage () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	loaded () final override;

	///  @name Event handlers

	void
	set_scene ();

	virtual
	bool
	on_key_release_event (GdkEventKey* event);

	void
	setPanelType (const size_t id, const PanelType panelType);
	
	PanelType
	getPanelType (const size_t id) const;

	void
	setPanel (const size_t id, const PanelType panelType, Gtk::Viewport & box);

	void
	set_panel (const size_t id, const PanelType panelType, Gtk::Viewport & box);

	void
	set_focus (const size_t id);

	void
	setActiveView (const size_t value, const bool modify);

	size_t
	getActiveView () const
	{ return activeView; }

	void
	set_browser_ratio ();

	///  @name Members

	std::vector <Gtk::Widget*> boxes;
	PanelArray                 panels;
	size_t                     activeView;
	X3D::SFBool                multiView;

};

} // puck
} // titania

#endif
