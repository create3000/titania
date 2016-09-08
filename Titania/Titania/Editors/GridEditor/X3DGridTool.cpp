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

#include "X3DGridTool.h"

#include "../../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/MetadataBoolean.h>
#include <Titania/X3D/Components/Core/MetadataFloat.h>
#include <Titania/X3D/Components/Core/MetadataDouble.h>
#include <Titania/X3D/Components/Core/MetadataInteger.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>

namespace titania {
namespace puck {

X3DGridTool::X3DGridTool () :
	 X3DBaseInterface (),
	  X3DEditorObject (),
	          browser (),
	         undoStep ()
{
	addChildren (browser);

	getBrowserWindow () -> isEditor () .addInterest (this, &X3DGridTool::update);
}

void
X3DGridTool::isEnabled (const bool value)
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());

	metadataSet-> createValue <X3D::MetadataBoolean> ("enabled") -> value () = { value };
}

bool
X3DGridTool::isEnabled () const
{
	try
	{
		const auto metadataSet = getMetaData ("/Titania/" + getName ());

		return metadataSet -> getValue <X3D::MetadataBoolean> ("enabled") -> value () .at (0);
	}
	catch (const X3D::X3DError & error)
	{
		return false;
	}
}

void
X3DGridTool::set_browser (const X3D::BrowserPtr & value)
{
	if (browser)
		browser -> getActiveLayer () .removeInterest (this, &X3DGridTool::update);

	browser = value;

	getTool () -> setExecutionContext (browser -> getPrivateScene ());

	browser -> getActiveLayer () .addInterest (this, &X3DGridTool::update);

	try
	{
		const auto metadataSet = getMetaData ("/Titania/" + getName ());

		getTool () -> enabled () .removeInterest (this, &X3DGridTool::set_enabled);
		getTool () -> enabled () .addInterest (this, &X3DGridTool::connectEnabled);

		getTool () -> translation () .removeInterest (this, &X3DGridTool::set_translation);
		getTool () -> translation () .addInterest (this, &X3DGridTool::connectTranslation);

		getTool () -> rotation () .removeInterest (this, &X3DGridTool::set_rotation);
		getTool () -> rotation () .addInterest (this, &X3DGridTool::connectRotation);

		getTool () -> scale () .removeInterest (this, &X3DGridTool::set_scale);
		getTool () -> scale () .addInterest (this, &X3DGridTool::connectScale);

		getTool () -> dimension () .removeInterest (this, &X3DGridTool::set_dimension);
		getTool () -> dimension () .addInterest (this, &X3DGridTool::connectDimension);

		getTool () -> majorLineEvery () .removeInterest (this, &X3DGridTool::set_majorLineEvery);
		getTool () -> majorLineEvery () .addInterest (this, &X3DGridTool::connectMajorLineEvery);

		getTool () -> majorLineOffset () .removeInterest (this, &X3DGridTool::set_majorLineOffset);
		getTool () -> majorLineOffset () .addInterest (this, &X3DGridTool::connectMajorLineOffset);

		getTool () -> color () .removeInterest (this, &X3DGridTool::set_color);
		getTool () -> color () .addInterest (this, &X3DGridTool::connectColor);

		getTool () -> lineColor () .removeInterest (this, &X3DGridTool::set_lineColor);
		getTool () -> lineColor () .addInterest (this, &X3DGridTool::connectLineColor);

		getTool () -> majorLineColor () .removeInterest (this, &X3DGridTool::set_majorLineColor);
		getTool () -> majorLineColor () .addInterest (this, &X3DGridTool::connectMajorLineColor);

		getTool () -> snapDistance () .removeInterest (this, &X3DGridTool::set_snapDistance);
		getTool () -> snapDistance () .addInterest (this, &X3DGridTool::connectSnapDistance);

		getTool () -> snapToCenter () .removeInterest (this, &X3DGridTool::set_snapToCenter);
		getTool () -> snapToCenter () .addInterest (this, &X3DGridTool::connectSnapToCenter);

		getTool () -> isActive () .addInterest (this, &X3DGridTool::set_active);

		try
		{
			const auto & v = metadataSet -> getValue <X3D::MetadataBoolean> ("snap") -> value ();
	
			getTool () -> enabled () = v .at (0);
		}
		catch (...)
		{
			getTool () -> enabled () = true;
		}
	
		try
		{
			const auto & v = metadataSet -> getValue <X3D::MetadataDouble> ("snapDistance") -> value ();
	
			getTool () -> snapDistance () = v .at (0);
		}
		catch (...)
		{
			getTool () -> snapDistance () = 0.25;
		}
	
		try
		{
			const auto & v = metadataSet -> getValue <X3D::MetadataBoolean> ("snapToCenter") -> value ();
	
			getTool () -> snapToCenter () = v .at (0);
		}
		catch (...)
		{
			getTool () -> snapToCenter () = true;
		}

		fromMetadata (metadataSet);
	}
	catch (const X3D::X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DGridTool::update ()
{
	if (isEnabled () and getBrowserWindow () -> isEditor ())
	   enable ();
	else
		disable ();
}

void
X3DGridTool::enable ()
{
	getCurrentBrowser () .addInterest (this, &X3DGridTool::set_browser);
	getCurrentBrowser () -> getActiveLayer () .addInterest (this, &X3DGridTool::update);

	set_browser (getCurrentBrowser ());
}

void
X3DGridTool::disable ()
{
	if (browser)
	{
		getCurrentBrowser () .removeInterest (this, &X3DGridTool::set_browser);
		getCurrentBrowser () -> getActiveLayer () .removeInterest (this, &X3DGridTool::update);

		set_browser (getMasterBrowser ());
	}
}

void
X3DGridTool::set_enabled ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataBoolean> ("snap") -> value ();

	value = { getTool () -> enabled () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_translation ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataFloat> ("translation") -> value ();

	value = { getTool () -> translation () .getX (),
	          getTool () -> translation () .getY (),
	          getTool () -> translation () .getZ () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_rotation ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataFloat> ("rotation") -> value ();

	value = { getTool () -> rotation () .getX (),
	          getTool () -> rotation () .getY (),
	          getTool () -> rotation () .getZ (),
	          getTool () -> rotation () .getAngle () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_scale ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataFloat> ("scale") -> value ();

	value = { getTool () -> scale () .getX (),
	          getTool () -> scale () .getY (),
	          getTool () -> scale () .getZ () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_dimension ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataInteger> ("dimension") -> value ();

	value = getTool () -> dimension ();

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_majorLineEvery ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataInteger> ("majorLineEvery") -> value ();

	value = getTool () -> majorLineEvery ();

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_majorLineOffset ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataInteger> ("majorLineOffset") -> value ();

	value = getTool () -> majorLineOffset ();

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_color ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataFloat> ("color") -> value ();

	value = { getTool () -> color () .getRed (),
	          getTool () -> color () .getGreen (),
	          getTool () -> color () .getBlue (),
	          getTool () -> color () .getAlpha () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_lineColor ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataFloat> ("lineColor") -> value ();

	value = { getTool () -> lineColor () .getRed (),
	          getTool () -> lineColor () .getGreen (),
	          getTool () -> lineColor () .getBlue (),
	          getTool () -> lineColor () .getAlpha () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_majorLineColor ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataFloat> ("majorLineColor") -> value ();

	value = { getTool () -> majorLineColor () .getRed (),
	          getTool () -> majorLineColor () .getGreen (),
	          getTool () -> majorLineColor () .getBlue (),
	          getTool () -> majorLineColor () .getAlpha () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_snapDistance ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataDouble> ("snapDistance") -> value ();

	value = { getTool () -> snapDistance () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_snapToCenter ()
{
	const auto metadataSet = createMetaData ("/Titania/" + getName ());
	auto &     value       = metadataSet -> createValue <X3D::MetadataBoolean> ("snapToCenter") -> value ();

	value = { getTool () -> snapToCenter () };

	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
X3DGridTool::set_active ()
{
	X3D::MFNode tool = { getTool () };

	if (getTool () -> isActive ())
	{
		undoStep .reset ();
		beginUndoGroup ("properties", undoStep);
		addUndoFunction <X3D::SFVec3f>    (tool, "translation", undoStep);
		addUndoFunction <X3D::SFRotation> (tool, "rotation",    undoStep);
		addUndoFunction <X3D::SFVec3f>    (tool, "scale",       undoStep);
		addUndoFunction <X3D::MFInt32>    (tool, "dimension",   undoStep);
		endUndoGroup ("properties", undoStep);
	}
	else
	{
		beginRedoGroup ("properties", undoStep);
		addRedoFunction <X3D::SFVec3f>    (tool, "translation", undoStep);
		addRedoFunction <X3D::SFRotation> (tool, "rotation",    undoStep);
		addRedoFunction <X3D::SFVec3f>    (tool, "scale",       undoStep);
		addRedoFunction <X3D::MFInt32>    (tool, "dimension",   undoStep);
		endRedoGroup ("properties", undoStep);
	}
}

void
X3DGridTool::connectEnabled (const X3D::SFBool & field)
{
	field .removeInterest (this, &X3DGridTool::connectEnabled);
	field .addInterest (this, &X3DGridTool::set_enabled);
}

void
X3DGridTool::connectTranslation (const X3D::SFVec3f & field)
{
	field .removeInterest (this, &X3DGridTool::connectTranslation);
	field .addInterest (this, &X3DGridTool::set_translation);
}

void
X3DGridTool::connectRotation (const X3D::SFRotation & field)
{
	field .removeInterest (this, &X3DGridTool::connectRotation);
	field .addInterest (this, &X3DGridTool::set_rotation);
}

void
X3DGridTool::connectScale (const X3D::SFVec3f & field)
{
	field .removeInterest (this, &X3DGridTool::connectScale);
	field .addInterest (this, &X3DGridTool::set_scale);
}

void
X3DGridTool::connectDimension (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &X3DGridTool::connectDimension);
	field .addInterest (this, &X3DGridTool::set_dimension);
}

void
X3DGridTool::connectMajorLineEvery (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &X3DGridTool::connectMajorLineEvery);
	field .addInterest (this, &X3DGridTool::set_majorLineEvery);
}

void
X3DGridTool::connectMajorLineOffset (const X3D::MFInt32 & field)
{
	field .removeInterest (this, &X3DGridTool::connectMajorLineOffset);
	field .addInterest (this, &X3DGridTool::set_majorLineOffset);
}

void
X3DGridTool::connectColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &X3DGridTool::connectColor);
	field .addInterest (this, &X3DGridTool::set_color);
}

void
X3DGridTool::connectLineColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &X3DGridTool::connectLineColor);
	field .addInterest (this, &X3DGridTool::set_lineColor);
}

void
X3DGridTool::connectMajorLineColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &X3DGridTool::connectMajorLineColor);
	field .addInterest (this, &X3DGridTool::set_majorLineColor);
}

void
X3DGridTool::connectSnapDistance (const X3D::SFDouble & field)
{
	field .removeInterest (this, &X3DGridTool::connectSnapDistance);
	field .addInterest (this, &X3DGridTool::set_snapDistance);
}

void
X3DGridTool::connectSnapToCenter (const X3D::SFBool & field)
{
	field .removeInterest (this, &X3DGridTool::connectSnapToCenter);
	field .addInterest (this, &X3DGridTool::set_snapToCenter);
}

X3DGridTool::~X3DGridTool ()
{
}

} // puck
} // titania
