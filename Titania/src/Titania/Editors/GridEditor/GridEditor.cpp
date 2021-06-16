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

#include "GridEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

GridEditor::GridEditor (X3DBrowserWindow* const browserWindow) :
	        X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	  X3DGridEditorInterface (get_ui ("Editors/GridEditor.glade")),
	           X3DGridEditor (),
	          X3DAngleEditor (),
	X3DAxonometricGridEditor ()
{
	setup ();
}

void
GridEditor::initialize ()
{
	X3DGridEditorInterface::initialize ();
	X3DGridEditor::initialize ();
	X3DAngleEditor::initialize ();
	X3DAxonometricGridEditor::initialize ();
}

void
GridEditor::configure ()
{
	X3DGridEditorInterface::configure ();
	X3DGridEditor::configure ();
	X3DAngleEditor::configure ();
	X3DAxonometricGridEditor::configure ();

	getGridNotebook () .set_current_page (getConfig () -> getItem <int32_t> ("currentPage"));
}

void
GridEditor::store ()
{
	getConfig () -> setItem ("currentPage", getGridNotebook () .get_current_page ());

	X3DAxonometricGridEditor::store ();
	X3DAngleEditor::store ();
	X3DGridEditor::store ();
	X3DGridEditorInterface::store ();
}

GridEditor::~GridEditor ()
{
	dispose ();
}

} // puck
} // titania
