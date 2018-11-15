/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_BROWSER_X3DBROWSER_WIDGET_H__
#define __TITANIA_BROWSER_X3DBROWSER_WIDGET_H__

#include "../UserInterfaces/X3DBrowserWindowInterface.h"
#include "../Editors/HistoryEditor/History.h"

namespace titania {
namespace puck {

class BrowserUserData;
class History;
class IconFactory;
class NotebookPage;
class RecentView;
class UserData;

using NotebookPagePtr      = std::shared_ptr <NotebookPage>;
using NotebookPagePtrArray = std::deque <NotebookPagePtr>;

class X3DBrowserNotebook :
	virtual public X3DBrowserWindowInterface
{
public:

	///  @name Member access

	const NotebookPagePtrArray &
	getPages () const
	{ return pages; }

	const NotebookPagePtrArray &
	getRecentPages () const
	{ return recentPages; }

	NotebookPagePtr
	getCurrentPage () const
	{ return page; }

	///  throws std::out_of_range
	NotebookPagePtr
	getPage (const basic::uri &) const;

	virtual
	const X3D::BrowserPtr &
	getMasterBrowser () const final override
	{ return masterBrowser; }

	virtual
	const X3D::BrowserPtr &
	getCurrentBrowser () const final override
	{ return browser; }

	virtual
	const X3D::X3DScenePtr &
	getCurrentScene () const final override
	{ return scene; }

	virtual
	void
	setCurrentContext (const X3D::X3DExecutionContextPtr &) override;

	virtual
	const X3D::X3DExecutionContextPtr &
	getCurrentContext () const final override
	{ return executionContext; }
	
	const X3D::Output &
	worldURL_changed () const
	{ return worldURLOutput; }

	virtual
	void
	isLive (const bool);

	bool
	isLive () const;

	std::string
	getOutputStyle (const X3D::X3DScenePtr & scene) const;

	///  @name Operations

	void
	open (const basic::uri & uri);

	void
	openRecent ();

	virtual
	void
	load (const basic::uri & uri);

	NotebookPagePtr
	append (const basic::uri & URL);

	bool
	save (const X3D::X3DScenePtr & scene, const basic::uri & worldURL, const std::string & outputStyle, const bool copy);

	virtual
	void
	reload ();

	virtual
	void
	close (const NotebookPagePtr page);

	virtual
	bool
	quit () override;

	const std::unique_ptr <IconFactory> &
	getIconFactory () const
	{ return iconFactory; }

	const std::unique_ptr <RecentView> &
	getRecentView () const
	{ return recentView; }

	const std::unique_ptr <History> &
	getHistory () const
	{ return history; }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBrowserNotebook ();


protected:

	///  @name Construction

	X3DBrowserNotebook (const X3D::BrowserPtr &);

	virtual
	void
	configure () override;

	virtual
	void
	initialize () override;

	virtual
	void
	on_fullscreen (const bool) override;

	///  @name Destruction

	virtual
	void
	store () override;

	///  @name Operations

	virtual
	void
	setPage (const NotebookPagePtr & value);

	bool
	getShowTabs () const;


private:

	///  @name Event handlers

	void
	set_masterBrowser ();

	void
	on_tab_close_clicked (NotebookPage* const page);

	virtual
	void
	on_switch_page (Gtk::Widget*, guint pageNumber) final override;

	virtual
	void
	on_page_reordered (Gtk::Widget* widget, guint pageNumber) final override;

	void
	set_shutdown ();

	void
	set_executionContext ();

	void
	set_history ();

	void
	set_urlError ();

	///  @name Operations

	void
	setOutputStyle (const X3D::X3DScenePtr & scene, const std::string & outputStyle);

	///  @name Members

	NotebookPagePtr                 page;
	NotebookPagePtrArray            pages;
	NotebookPagePtrArray            recentPages;
	X3D::BrowserPtr                 masterBrowser;
	X3D::BrowserPtr                 browser;
	X3D::X3DScenePtr                scene;
	X3D::X3DExecutionContextPtr     executionContext;
	X3D::Output                     worldURLOutput;
	std::unique_ptr <IconFactory>   iconFactory;
	std::unique_ptr <RecentView>    recentView;
	std::unique_ptr <History>       history;

};

} // puck
} // titania

#endif
