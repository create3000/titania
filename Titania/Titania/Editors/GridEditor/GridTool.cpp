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

#include "GridTool.h"

#include "../../Browser/X3DBrowserWindow.h"

namespace titania {
namespace puck {

GridTool::GridTool (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	            tool (),
	         enabled (false)
{
	getScene () .addInterest (this, &GridTool::set_scene);
	setup ();
}

void
GridTool::realize ()
{
	this -> tool = isEnabled ()
	               ? X3D::createNode <X3D::GridTool> (getBrowser ())
	               : X3D::createNode <X3D::GridTool> (getBrowserWindow () -> getMasterBrowser ());

	configure ();

	tool -> getExecutionContext () -> realize ();
}

void
GridTool::isEnabled (const bool value, const bool metadata)
{
	enabled = value;

	if (enabled)
	{
		getBrowser () .addInterest (this, &GridTool::set_browser);
		set_browser (getBrowser ());
	}
	else
	{
		if (tool)
		{
			getBrowser () .removeInterest (this, &GridTool::set_browser);
			set_browser (getBrowserWindow () -> getMasterBrowser ());
		}
	}

	if (metadata)
	{
		getWorldInfo (true) -> setMetaData <bool> ("/Titania/GridTool/enabled", enabled);
		getBrowserWindow () -> isModified (getBrowser (), true);
	}
}

const X3D::X3DPtr <X3D::GridTool> &
GridTool::getTool () const
{
	if (not tool)
		const_cast <GridTool*> (this) -> realize ();

	return tool;
}

void
GridTool::set_browser (const X3D::BrowserPtr & browser)
{
	getTool () -> setExecutionContext (browser);
}

void
GridTool::set_scene ()
{
	try
	{
		isEnabled (getWorldInfo () -> getMetaData <X3D::MFBool> ("/Titania/GridTool/enabled") .at (0), false);
	}
	catch (...)
	{
		isEnabled (false, false);
	}

	if (tool)
		configure ();
}

void
GridTool::configure ()
{
	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/GridTool/translation");

		getTool () -> translation () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> translation () = X3D::Vector3f ();
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/GridTool/rotation");

		getTool () -> rotation () = X3D::Rotation4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> rotation () = X3D::Rotation4f ();
	}
}

GridTool::~GridTool ()
{
	dispose ();
}

} // puck
} // titania
