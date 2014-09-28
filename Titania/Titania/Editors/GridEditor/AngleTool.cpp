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

#include "AngleTool.h"

#include "../../Browser/X3DBrowserWindow.h"

namespace titania {
namespace puck {

AngleTool::AngleTool (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	            tool (),
	         enabled (false)
{
	getBrowserWindow () -> isEditor () .addInterest (this, &AngleTool::set_enabled);
	getScene () .addInterest (this, &AngleTool::set_scene);
	setup ();
}

void
AngleTool::realize ()
{
	this -> tool = isEnabled ()
	               ? X3D::createNode <X3D::AngleTool> (getBrowser ())
						: X3D::createNode <X3D::AngleTool> (getBrowserWindow () -> getMasterBrowser ());

	configure ();

	tool -> getExecutionContext () -> realize ();
}

void
AngleTool::isEnabled (const bool value, const bool metadata)
{
	enabled = value;

	if (isEnabled ())
	{
		getBrowser () .addInterest (this, &AngleTool::set_browser);
		set_browser (getBrowser ());
	}
	else
	{
		if (tool)
		{
			getBrowser () .removeInterest (this, &AngleTool::set_browser);
			set_browser (getBrowserWindow () -> getMasterBrowser ());
		}
	}

	if (metadata)
	{
		getWorldInfo (true) -> setMetaData <bool> ("/Titania/AngleGrid/enabled", enabled);
		getBrowserWindow () -> isModified (getBrowser (), true);
	}
}

bool
AngleTool::isEnabled () const
{
	return enabled and getBrowserWindow () -> isEditor ();
}

const X3D::X3DPtr <X3D::AngleTool> &
AngleTool::getTool () const
{
	if (not tool)
		const_cast <AngleTool*> (this) -> realize ();

	return tool;
}

void
AngleTool::set_enabled ()
{
	isEnabled (enabled, false);
}

void
AngleTool::set_browser (const X3D::BrowserPtr & browser)
{
	getTool () -> setExecutionContext (browser);
}

void
AngleTool::set_scene ()
{
	try
	{
		isEnabled (getWorldInfo () -> getMetaData <X3D::MFBool> ("/Titania/AngleGrid/enabled") .at (0), false);
	}
	catch (...)
	{
		isEnabled (false, false);
	}

	if (tool)
		configure ();
}

void
AngleTool::configure ()
{
	getTool () -> translation () .removeInterest (this, &AngleTool::set_translation);
	getTool () -> translation () .addInterest (this, &AngleTool::connectTranslation);

	getTool () -> rotation () .removeInterest (this, &AngleTool::set_rotation);
	getTool () -> rotation () .addInterest (this, &AngleTool::connectRotation);

	getTool () -> scale () .removeInterest (this, &AngleTool::set_scale);
	getTool () -> scale () .addInterest (this, &AngleTool::connectScale);

	getTool () -> dimension () .removeInterest (this, &AngleTool::set_dimension);
	getTool () -> dimension () .addInterest (this, &AngleTool::connectDimension);

	getTool () -> majorLineEvery () .removeInterest (this, &AngleTool::set_majorLineEvery);
	getTool () -> majorLineEvery () .addInterest (this, &AngleTool::connectMajorLineEvery);

	getTool () -> majorLineOffset () .removeInterest (this, &AngleTool::set_majorLineOffset);
	getTool () -> majorLineOffset () .addInterest (this, &AngleTool::connectMajorLineOffset);

	getTool () -> color () .removeInterest (this, &AngleTool::set_color);
	getTool () -> color () .addInterest (this, &AngleTool::connectColor);

	getTool () -> lineColor () .removeInterest (this, &AngleTool::set_lineColor);
	getTool () -> lineColor () .addInterest (this, &AngleTool::connectLineColor);

	getTool () -> majorLineColor () .removeInterest (this, &AngleTool::set_majorLineColor);
	getTool () -> majorLineColor () .addInterest (this, &AngleTool::connectMajorLineColor);

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/AngleGrid/translation");

		getTool () -> translation () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> translation () = X3D::Vector3f ();
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/AngleGrid/rotation");

		getTool () -> rotation () = X3D::Rotation4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> rotation () = X3D::Rotation4f ();
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/AngleGrid/scale");

		getTool () -> scale () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> scale () = X3D::Vector3f (1, 1, 1);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> ("/Titania/AngleGrid/dimension");

		getTool () -> dimension () = v;

		if (v .size () < 1)
			getTool () -> dimension () .resize (1, X3D::SFInt32 (5));

		if (v .size () < 2)
			getTool () -> dimension () .resize (2, X3D::SFInt32 (16));
	}
	catch (...)
	{
		getTool () -> dimension () = { 5, 16 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> ("/Titania/AngleGrid/majorLineEvery");

		getTool () -> majorLineEvery () = v;
	}
	catch (...)
	{
		getTool () -> majorLineEvery () = { 5, 2 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> ("/Titania/AngleGrid/majorLineOffset");

		getTool () -> majorLineOffset () = v;
	}
	catch (...)
	{
		getTool () -> majorLineOffset () = { 0, 0 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/AngleGrid/color");

		getTool () -> color () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> color () = X3D::Color4f (0.5, 0.5, 0.5, 0.2);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/AngleGrid/lineColor");

		getTool () -> lineColor () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> lineColor () = X3D::Color4f (1, 0.7, 0.7, 0.2);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/AngleGrid/majorLineColor");

		getTool () -> majorLineColor () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> majorLineColor () = X3D::Color4f (1, 0.7, 0.7, 0.4);
	}
}

void
AngleTool::set_translation ()
{
	getWorldInfo (true) -> setMetaData <X3D::Vector3f> ("/Titania/AngleGrid/translation", getTool () -> translation ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_rotation ()
{
	getWorldInfo (true) -> setMetaData <X3D::Rotation4f> ("/Titania/AngleGrid/rotation", getTool () -> rotation ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_scale ()
{
	getWorldInfo (true) -> setMetaData <X3D::Vector3f> ("/Titania/AngleGrid/scale", getTool () -> scale ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_dimension ()
{
	getWorldInfo (true) -> setMetaData ("/Titania/AngleGrid/dimension", getTool () -> dimension ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_majorLineEvery ()
{
	getWorldInfo (true) -> setMetaData ("/Titania/AngleGrid/majorLineEvery", getTool () -> majorLineEvery ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_majorLineOffset ()
{
	getWorldInfo (true) -> setMetaData ("/Titania/AngleGrid/majorLineOffset", getTool () -> majorLineOffset ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_color ()
{
	getWorldInfo (true) -> setMetaData <X3D::Color4f> ("/Titania/AngleGrid/color", getTool () -> color ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_lineColor ()
{
	getWorldInfo (true) -> setMetaData <X3D::Color4f> ("/Titania/AngleGrid/lineColor", getTool () -> lineColor ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_majorLineColor ()
{
	getWorldInfo (true) -> setMetaData <X3D::Color4f> ("/Titania/AngleGrid/majorLineColor", getTool () -> majorLineColor ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::connectTranslation (const X3D::SFVec3f & field)
{
	field .removeInterest (this, &AngleTool::connectTranslation);
	field .addInterest (this, &AngleTool::set_translation);
}

void
AngleTool::connectRotation (const X3D::SFRotation & field)
{
	field .removeInterest (this, &AngleTool::connectRotation);
	field .addInterest (this, &AngleTool::set_rotation);
}

void
AngleTool::connectScale (const X3D::SFVec3f & field)
{
	field .removeInterest (this, &AngleTool::connectScale);
	field .addInterest (this, &AngleTool::set_scale);
}

void
AngleTool::connectDimension (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &AngleTool::connectDimension);
	field .addInterest (this, &AngleTool::set_dimension);
}

void
AngleTool::connectMajorLineEvery (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &AngleTool::connectMajorLineEvery);
	field .addInterest (this, &AngleTool::set_majorLineEvery);
}

void
AngleTool::connectMajorLineOffset (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &AngleTool::connectMajorLineOffset);
	field .addInterest (this, &AngleTool::set_majorLineOffset);
}

void
AngleTool::connectColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &AngleTool::connectColor);
	field .addInterest (this, &AngleTool::set_color);
}

void
AngleTool::connectLineColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &AngleTool::connectLineColor);
	field .addInterest (this, &AngleTool::set_lineColor);
}

void
AngleTool::connectMajorLineColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &AngleTool::connectMajorLineColor);
	field .addInterest (this, &AngleTool::set_majorLineColor);
}

AngleTool::~AngleTool ()
{
	dispose ();
}

} // puck
} // titania
