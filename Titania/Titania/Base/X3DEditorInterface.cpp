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

#include "X3DEditorInterface.h"

#include "../Browser/X3DBrowserWindow.h"
#include "../Browser/BrowserSelection.h"

namespace titania {
namespace puck {

///  @name Construction

X3DEditorInterface::X3DEditorInterface () :
	X3DDialogInterface (),
	   X3DEditorObject (),
	          titleBar (),
	   headerBarOutput ()
{ }

void
X3DEditorInterface::setup ()
{
	X3DDialogInterface::setup ();
	X3DEditorObject::setup ();
}

void
X3DEditorInterface::initialize ()
{
	X3DDialogInterface::initialize ();
	X3DEditorObject::initialize ();
}

void
X3DEditorInterface::present ()
{
	setTitleBar (getHeaderBar ());

	X3DDialogInterface::present ();
}

void
X3DEditorInterface::setTitleBar (Gtk::HeaderBar & headerBar)
{
	headerBar .set_show_close_button (true);

	const auto children = titleBar .get_children ();

	if (children .empty ())
	{
		titleBar .show ();
		X3DDialogInterface::setTitleBar (getWindow (), titleBar);
	}

	for (const auto widget : children)
		titleBar .remove (*widget);

	const auto container = headerBar .get_parent ();

	if (container)
	   container -> remove (headerBar);

	titleBar .pack_start (headerBar, true, true);
}

///  @name Destruction

void
X3DEditorInterface::dispose ()
{
	X3DEditorObject::dispose ();
	X3DDialogInterface::dispose ();
}

X3DEditorInterface::~X3DEditorInterface ()
{ }

} // puck
} // titania
