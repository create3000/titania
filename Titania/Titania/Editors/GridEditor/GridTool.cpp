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
	getBrowserWindow () -> isEditor () .addInterest (this, &GridTool::set_enabled);
	getScene () .addInterest (this, &GridTool::set_scene);
	setup ();
}

void
GridTool::realize ()
{
	this -> tool = isEnabled ()
	               ? X3D::createNode <X3D::GridTool> (getBrowser () -> getPrivateScene ())
	               : X3D::createNode <X3D::GridTool> (getBrowserWindow () -> getMasterBrowser () -> getPrivateScene ());

	configure ();

	tool -> getExecutionContext () -> realize ();
}

void
GridTool::isEnabled (const bool value, const bool metadata)
{
	enabled = value;

	if (isEnabled ())
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
		getWorldInfo (true) -> setMetaData <bool> ("/Titania/Grid/enabled", enabled);
		getBrowserWindow () -> isModified (getBrowser (), true);
	}
}

bool
GridTool::isEnabled () const
{
	return enabled and getBrowserWindow () -> isEditor ();
}

const X3D::X3DPtr <X3D::GridTool> &
GridTool::getTool () const
{
	if (not tool)
		const_cast <GridTool*> (this) -> realize ();

	return tool;
}

void
GridTool::set_enabled ()
{
	isEnabled (enabled, false);
}

void
GridTool::set_browser (const X3D::BrowserPtr & browser)
{
	getTool () -> setExecutionContext (browser -> getPrivateScene ());
}

void
GridTool::set_scene ()
{
	try
	{
		isEnabled (getWorldInfo () -> getMetaData <X3D::MFBool> ("/Titania/Grid/enabled") .at (0), false);
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

	getTool () -> scale () .removeInterest (this, &GridTool::set_scale);
	getTool () -> scale () .addInterest (this, &GridTool::connectScale);

	getTool () -> dimension () .removeInterest (this, &GridTool::set_dimension);
	getTool () -> dimension () .addInterest (this, &GridTool::connectDimension);

	getTool () -> majorLineEvery () .removeInterest (this, &GridTool::set_majorLineEvery);
	getTool () -> majorLineEvery () .addInterest (this, &GridTool::connectMajorLineEvery);

	getTool () -> majorLineOffset () .removeInterest (this, &GridTool::set_majorLineOffset);
	getTool () -> majorLineOffset () .addInterest (this, &GridTool::connectMajorLineOffset);

	getTool () -> color () .removeInterest (this, &GridTool::set_color);
	getTool () -> color () .addInterest (this, &GridTool::connectColor);

	getTool () -> lineColor () .removeInterest (this, &GridTool::set_lineColor);
	getTool () -> lineColor () .addInterest (this, &GridTool::connectLineColor);

	getTool () -> majorLineColor () .removeInterest (this, &GridTool::set_majorLineColor);
	getTool () -> majorLineColor () .addInterest (this, &GridTool::connectMajorLineColor);

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/Grid/translation");

		getTool () -> translation () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> translation () = X3D::Vector3f ();
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/Grid/rotation");

		getTool () -> rotation () = X3D::Rotation4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> rotation () = X3D::Rotation4f ();
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/Grid/scale");

		getTool () -> scale () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> scale () = X3D::Vector3f (1, 1, 1);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> ("/Titania/Grid/dimension");

		getTool () -> dimension () = v;
		getTool () -> dimension () .resize (3, X3D::SFInt32 (10));
	}
	catch (...)
	{
		getTool () -> dimension () = { 10, 10, 10 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> ("/Titania/Grid/majorLineEvery");

		getTool () -> majorLineEvery () = v;
	}
	catch (...)
	{
		getTool () -> majorLineEvery () = { 5, 5, 5 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> ("/Titania/Grid/majorLineOffset");

		getTool () -> majorLineOffset () = v;
	}
	catch (...)
	{
		getTool () -> majorLineOffset () = { 0, 0, 0 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/Grid/color");

		getTool () -> color () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> color () = X3D::Color4f (0.5, 0.5, 0.5, 0.2);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/Grid/lineColor");

		getTool () -> lineColor () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> lineColor () = X3D::Color4f (1, 0.7, 0.7, 0.2);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> ("/Titania/Grid/majorLineColor");

		getTool () -> majorLineColor () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> majorLineColor () = X3D::Color4f (1, 0.7, 0.7, 0.4);
	}
}

void
GridTool::set_translation ()
{
	getWorldInfo (true) -> setMetaData <X3D::Vector3f> ("/Titania/Grid/translation", getTool () -> translation ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_rotation ()
{
	getWorldInfo (true) -> setMetaData <X3D::Rotation4f> ("/Titania/Grid/rotation", getTool () -> rotation ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_scale ()
{
	getWorldInfo (true) -> setMetaData <X3D::Vector3f> ("/Titania/Grid/scale", getTool () -> scale ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_dimension ()
{
	getWorldInfo (true) -> setMetaData ("/Titania/Grid/dimension", getTool () -> dimension ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_majorLineEvery ()
{
	getWorldInfo (true) -> setMetaData ("/Titania/Grid/majorLineEvery", getTool () -> majorLineEvery ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_majorLineOffset ()
{
	getWorldInfo (true) -> setMetaData ("/Titania/Grid/majorLineOffset", getTool () -> majorLineOffset ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_color ()
{
	getWorldInfo (true) -> setMetaData <X3D::Color4f> ("/Titania/Grid/color", getTool () -> color ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_lineColor ()
{
	getWorldInfo (true) -> setMetaData <X3D::Color4f> ("/Titania/Grid/lineColor", getTool () -> lineColor ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
GridTool::set_majorLineColor ()
{
	getWorldInfo (true) -> setMetaData <X3D::Color4f> ("/Titania/Grid/majorLineColor", getTool () -> majorLineColor ());
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
GridTool::connectScale (const X3D::SFVec3f & field)
{
	field .removeInterest (this, &GridTool::connectScale);
	field .addInterest (this, &GridTool::set_scale);
}

void
GridTool::connectDimension (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &GridTool::connectDimension);
	field .addInterest (this, &GridTool::set_dimension);
}

void
GridTool::connectMajorLineEvery (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &GridTool::connectMajorLineEvery);
	field .addInterest (this, &GridTool::set_majorLineEvery);
}

void
GridTool::connectMajorLineOffset (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &GridTool::connectMajorLineOffset);
	field .addInterest (this, &GridTool::set_majorLineOffset);
}

void
GridTool::connectColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &GridTool::connectColor);
	field .addInterest (this, &GridTool::set_color);
}

void
GridTool::connectLineColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &GridTool::connectLineColor);
	field .addInterest (this, &GridTool::set_lineColor);
}

void
GridTool::connectMajorLineColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &GridTool::connectMajorLineColor);
	field .addInterest (this, &GridTool::set_majorLineColor);
}

GridTool::~GridTool ()
{
	dispose ();
}

} // puck
} // titania
