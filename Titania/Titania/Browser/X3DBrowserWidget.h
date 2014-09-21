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

#ifndef __TITANIA_BROWSER_X3DBROWSER_WIDGET_H__
#define __TITANIA_BROWSER_X3DBROWSER_WIDGET_H__

#include "../UserInterfaces/X3DBrowserWindowInterface.h"
#include "../Undo/UndoStep.h"

#include <gtkmm.h>
#include <memory>

namespace titania {
namespace puck {

class AboutTab;
class BrowserUserData;

class X3DBrowserWidget :
	virtual public X3DBrowserWindowInterface
{
public:

	///  @name Member access

	const X3D::X3DPtrArray <X3D::Browser> &
	getBrowsers () const
	{ return browsers; }

	virtual
	const X3D::BrowserPtr &
	getBrowser () const final override
	{ return browser; }

	const X3D::BrowserPtr &
	getMasterBrowser () const
	{ return masterBrowser; }

	static
	std::shared_ptr <BrowserUserData>
	getUserData (const X3D::BrowserPtr &);

	virtual
	const X3D::X3DScenePtr &
	getScene () const final override
	{ return scene; }

	virtual
	void
	setExecutionContext (const X3D::X3DExecutionContextPtr &) final override;

	virtual
	const X3D::X3DExecutionContextPtr &
	getExecutionContext () const final override
	{ return executionContext; }

	const X3D::Output &
	worldURL_changed () const
	{ return worldURLOutput; }

	virtual
	void
	isLive (const bool);

	bool
	isLive () const;

	virtual
	X3D::WorldInfoPtr
	getWorldInfo () const final override;

	///  @name Operations

	virtual
	void
	open (const basic::uri &, const bool = true);

	void
	openRecent ();

	virtual
	void
	load (const X3D::BrowserPtr &, const basic::uri &);

	void
	append (const X3D::BrowserPtr &, const basic::uri &, const bool = true);

	void
	loadIcon (const basic::uri &, const std::string &);

	std::string
	getIcon (const basic::uri &, const Gtk::IconSize &);

	virtual
	bool
	save (const basic::uri &, const bool);

	static
	bool
	transform (const basic::uri &, const basic::uri &, const UndoStepPtr &, X3D::SFNode &);

	virtual
	void
	reload ();

	virtual
	void
	close (const X3D::BrowserPtr &);

	virtual
	bool
	quit () override;

	///  @name Destruction

	virtual
	~X3DBrowserWidget ();


protected:

	///  @name Construction

	X3DBrowserWidget (const X3D::BrowserPtr &);

	virtual
	void
	initialize () override;

	virtual
	void
	restoreSession () override;

	///  @name Destruction

	virtual
	void
	saveSession () override;

	///  @name Operations

	virtual
	void
	setBrowser (const X3D::BrowserPtr &);

	void
	setTitle () const;

	void
	setTransparent (const bool);


private:

	X3D::X3DPtrArray <X3D::Browser>::const_iterator
	getBrowser (const basic::uri &) const;

	///  @name Event handlers

	void
	set_initialized ();

	void
	set_splashScreen (const X3D::BrowserPtr &, const basic::uri &);

	virtual
	void
	on_switch_browser (Gtk::Widget*, guint) final override;

	virtual
	void
	on_browser_reordered (Widget* page, guint page_num) final override;

	void
	set_executionContext ();

	void
	set_scene ();

	///  @name Operations
	
	void
	setWorldURL (const X3D::X3DExecutionContextPtr &, const basic::uri &, const UndoStepPtr &);

	void
	loadIcon ();

	bool
	statistics ();

	///  @name Members

	X3D::BrowserPtr                 masterBrowser;
	X3D::BrowserPtr                 browser;
	X3D::X3DPtrArray <X3D::Browser> browsers;
	X3D::X3DScenePtr                scene;
	X3D::X3DExecutionContextPtr     executionContext;
	X3D::Output                     worldURLOutput;
	std::unique_ptr <AboutTab>      aboutTab;
	double                          loadTime;
	sigc::connection                timeout;

};

} // puck
} // titania

#endif
