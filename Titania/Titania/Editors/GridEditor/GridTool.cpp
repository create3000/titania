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
	getTool () -> translation () .removeInterest (this, &GridTool::set_translation);
	getTool () -> translation () .addInterest (this, &GridTool::connectTranslation);

	getTool () -> rotation () .removeInterest (this, &GridTool::set_rotation);
	getTool () -> rotation () .addInterest (this, &GridTool::connectRotation);

	getTool () -> dimension () .removeInterest (this, &GridTool::set_dimension);
	getTool () -> dimension () .addInterest (this, &GridTool::connectDimension);

	getTool () -> spacing () .removeInterest (this, &GridTool::set_spacing);
	getTool () -> spacing () .addInterest (this, &GridTool::connectSpacing);

	getTool () -> color () .removeInterest (this, &GridTool::set_color);
	getTool () -> color () .addInterest (this, &GridTool::connectColor);

	getTool () -> transparency () .removeInterest (this, &GridTool::set_transparency);
	getTool () -> transparency () .addInterest (this, &GridTool::connectTransparency);

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

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> ("/Titania/GridTool/dimension");

		getTool () -> dimension () = v;
		getTool () -> dimension () .resize (3, X3D::SFInt32 (10));
	}
	catch (...)
	{
		getTool () -> dimension () = { 10, 10, 10 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/GridTool/spacing");

		getTool () -> spacing () = v;
		getTool () -> spacing () .resize (3, X3D::SFFloat (1));
	}
	catch (...)
	{
		getTool () -> spacing () = { 1, 1, 1 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/GridTool/color");

		getTool () -> color () = X3D::Color3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> color () = X3D::Color3f (1, 0.5, 0);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/GridTool/transparency");

		getTool () -> transparency () = v .at (0);
	}
	catch (...)
	{
		getTool () -> transparency () = 0.8;
	}
}

void
GridTool::set_translation ()
{
	getWorldInfo (true) -> setMetaData <X3D::Vector3f> ("/Titania/GridTool/translation", getTool () -> translation ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_rotation ()
{
	getWorldInfo (true) -> setMetaData <X3D::Rotation4f> ("/Titania/GridTool/rotation", getTool () -> rotation ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_dimension ()
{
	getWorldInfo (true) -> setMetaData ("/Titania/GridTool/dimension", getTool () -> dimension ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_spacing ()
{
	getWorldInfo (true) -> setMetaData ("/Titania/GridTool/spacing", getTool () -> spacing ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_color ()
{
	getWorldInfo (true) -> setMetaData <X3D::Color3f> ("/Titania/GridTool/color", getTool () -> color ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_transparency ()
{
	getWorldInfo (true) -> setMetaData <float> ("/Titania/GridTool/transparency", getTool () -> transparency ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::connectTranslation (const X3D::SFVec3f & field)
{
	field .removeInterest (this, &GridTool::connectTranslation);
	field .addInterest (this, &GridTool::set_translation);
}

void
GridTool::connectRotation (const X3D::SFRotation & field)
{
	field .removeInterest (this, &GridTool::connectRotation);
	field .addInterest (this, &GridTool::set_rotation);
}

void
GridTool::connectDimension (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &GridTool::connectDimension);
	field .addInterest (this, &GridTool::set_dimension);
}

void
GridTool::connectSpacing (const X3D::MFFloat & field)
{
	field .removeInterest (this, &GridTool::connectSpacing);
	field .addInterest (this, &GridTool::set_spacing);
}

void
GridTool::connectColor (const X3D::SFColor & field)
{
	field .removeInterest (this, &GridTool::connectColor);
	field .addInterest (this, &GridTool::set_color);
}

void
GridTool::connectTransparency (const X3D::SFFloat & field)
{
	field .removeInterest (this, &GridTool::connectTransparency);
	field .addInterest (this, &GridTool::set_transparency);
}

GridTool::~GridTool ()
{
	dispose ();
}

} // puck
} // titania
