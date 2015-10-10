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

#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>

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
	               ? getBrowser () -> getPrivateScene () -> createNode <X3D::AngleTool> ()
						: getMasterBrowser () -> getPrivateScene () -> createNode <X3D::AngleTool> ();

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
			set_browser (getMasterBrowser ());
		}
	}

	if (metadata)
	{
		const X3D::SFNode metadataSet (getWorldInfo () -> createMetaData <X3D::MetadataSet> (".titania.angleGrid"));
		const auto executionContext = metadataSet -> getExecutionContext ();
	
		executionContext -> addNamedNode (executionContext -> getUniqueName ("AngleLayoutTool"), metadataSet);
		createWorldInfo () -> setMetaData <bool> (".titania.angleGrid.enabled", enabled);
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
	getTool () -> setExecutionContext (browser -> getPrivateScene ());
}

void
AngleTool::set_scene ()
{
	try
	{
		isEnabled (getWorldInfo () -> getMetaData <X3D::MFBool> (".titania.angleGrid.enabled", false) .at (0), false);
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
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> (".titania.angleGrid.translation", false);

		getTool () -> translation () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> translation () = X3D::Vector3f ();
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> (".titania.angleGrid.rotation", false);

		getTool () -> rotation () = X3D::Rotation4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> rotation () = X3D::Rotation4f ();
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> (".titania.angleGrid.scale", false);

		getTool () -> scale () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> scale () = X3D::Vector3f (1, 1, 1);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> (".titania.angleGrid.dimension", false);

		getTool () -> dimension () = v;

		if (v .size () < 1)
			getTool () -> dimension () .resize (1, X3D::SFInt32 (5));

		if (v .size () < 2)
			getTool () -> dimension () .resize (2, X3D::SFInt32 (16));

		if (v .size () < 3)
			getTool () -> dimension () .resize (3, X3D::SFInt32 (10));
	}
	catch (...)
	{
		getTool () -> dimension () = { 5, 16, 10 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> (".titania.angleGrid.majorLineEvery", false);

		getTool () -> majorLineEvery () = v;
	}
	catch (...)
	{
		getTool () -> majorLineEvery () = { 5, 2, 5 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFInt32> (".titania.angleGrid.majorLineOffset", false);

		getTool () -> majorLineOffset () = v;
	}
	catch (...)
	{
		getTool () -> majorLineOffset () = { 0, 0, 0 };
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> (".titania.angleGrid.color", false);

		getTool () -> color () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> color () = X3D::Color4f (0.5, 0.5, 0.5, 0.2);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> (".titania.angleGrid.lineColor", false);

		getTool () -> lineColor () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> lineColor () = X3D::Color4f (1, 0.7, 0.7, 0.2);
	}

	try
	{
		const auto & v = getWorldInfo () -> getMetaData <X3D::MFFloat> (".titania.angleGrid.majorLineColor", false);

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
	createWorldInfo () -> setMetaData <X3D::Vector3f> (".titania.angleGrid.translation", getTool () -> translation ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_rotation ()
{
	createWorldInfo () -> setMetaData <X3D::Rotation4f> (".titania.angleGrid.rotation", getTool () -> rotation ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_scale ()
{
	createWorldInfo () -> setMetaData <X3D::Vector3f> (".titania.angleGrid.scale", getTool () -> scale ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_dimension ()
{
	createWorldInfo () -> setMetaData (".titania.angleGrid.dimension", getTool () -> dimension ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_majorLineEvery ()
{
	createWorldInfo () -> setMetaData (".titania.angleGrid.majorLineEvery", getTool () -> majorLineEvery ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_majorLineOffset ()
{
	createWorldInfo () -> setMetaData (".titania.angleGrid.majorLineOffset", getTool () -> majorLineOffset ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_color ()
{
	createWorldInfo () -> setMetaData <X3D::Color4f> (".titania.angleGrid.color", getTool () -> color ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_lineColor ()
{
	createWorldInfo () -> setMetaData <X3D::Color4f> (".titania.angleGrid.lineColor", getTool () -> lineColor ());
	getBrowserWindow () -> isModified (getBrowser (), true);
}

void
AngleTool::set_majorLineColor ()
{
	createWorldInfo () -> setMetaData <X3D::Color4f> (".titania.angleGrid.majorLineColor", getTool () -> majorLineColor ());
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
