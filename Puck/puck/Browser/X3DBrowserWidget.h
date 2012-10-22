/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#ifndef __PUCK_BROWSER_BASIC_BROWSER_WIDGET_H__
#define __PUCK_BROWSER_BASIC_BROWSER_WIDGET_H__

#include "../UserInterfaces/X3DBrowserUserInterface.h"
#include "../HistoryEditor/HistoryEditor.h"
#include "../MotionBlurEditor/MotionBlurEditor.h"
#include "../OutlineEditor/OutlineEditor.h"
#include "../ViewpointEditor/ViewpointEditor.h"

#include <Titania/X3D/Widgets/DrawingArea.h>

namespace titania {
namespace puck {

class X3DBrowserWidget :
	public X3DBrowserUserInterface
{
public:

	/// @name File

	virtual
	void
	blank ();

	virtual
	void
	home ();

	virtual
	const basic::uri &
	getWorldURL ();
	
	void
	setDescription (const std::string & value)
	throw (X3D::Error <X3D::INVALID_OPERATION_TIMING>,
	       X3D::Error <X3D::DISPOSED>);

	virtual
	void
	loadURL (const X3D::MFString &, const X3D::MFString &)
	throw (X3D::Error <X3D::INVALID_URL>,
	       X3D::Error <X3D::URL_UNAVAILABLE>,
	       X3D::Error <X3D::INVALID_X3D>);

	virtual
	void
	loadURL (const X3D::MFString &)
	throw (X3D::Error <X3D::INVALID_URL>,
	       X3D::Error <X3D::URL_UNAVAILABLE>,
	       X3D::Error <X3D::INVALID_X3D>);

	virtual
	void
	save (const basic::uri &);

	/// @name Destructor

	virtual
	~X3DBrowserWidget ();


protected:

	X3DBrowserWidget (const std::string &, X3DBrowserInterface* const);

	virtual
	void
	initialize ();

	bool
	on_map_event (GdkEventAny*);

	void
	set_initialized ();
	
	void
	restoreSession ();

	/// @name StatusBar

	void
	pushStatusBar (const std::string &);

	void
	popStatusBar ();

	// Statistics

	void
	printStatistics () const;


private:

	// Callbacks

	void
	set_world ();

	void
	update_location ();

	void
	update_icon ();

	void
	update_viewpoints ();

private:

	sigc::connection map_event;
	double loadTime;

};

} // puck
} // titania

#endif
