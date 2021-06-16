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
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"

#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/MetadataBoolean.h>
#include <Titania/X3D/Components/Core/MetadataFloat.h>
#include <Titania/X3D/Components/Core/MetadataDouble.h>
#include <Titania/X3D/Components/Core/MetadataInteger.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>

namespace titania {
namespace puck {

using math::pi;

static const auto X_PLANE_ROTATION = X3D::Rotation4d (0, 0, -1, pi <double> / 2) * X3D::Rotation4d (1, 0, 0, pi <double> / 2);
static const auto Y_PLANE_ROTATION = X3D::Rotation4d ();
static const auto Z_PLANE_ROTATION = X3D::Rotation4d (1, 0, 0, pi <double> / 2);

X3DGridTool::X3DGridTool () :
	 X3DBaseInterface (),
	  X3DEditorObject (),
	          browser (getCurrentBrowser ()),
	          enabled (),
	         undoStep ()
{
	addChildObjects (browser, enabled);
}

void
X3DGridTool::setup ()
{
	X3DBaseInterface::setup ();
	X3DEditorObject::setup ();

	getCurrentScene () .addInterest (&X3DGridTool::set_scene, this);
}

void
X3DGridTool::set_browser ()
{
	const auto   enabled = getEnabled ();
	const auto & value   = enabled ? getCurrentBrowser () : getMasterBrowser ();

	if (not enabled and not browser)
		return;

	if (browser)
		browser -> getActiveLayer () .removeInterest (&X3DGridTool::set_activeLayer, this);

	browser = value;

	if (browser)
	{
		browser -> getActiveLayer () .addInterest (&X3DGridTool::set_activeLayer, this);

		getTool () -> setExecutionContext (browser -> getExecutionContext ());

		set_activeLayer ();
	}
}

void
X3DGridTool::set_scene ()
{
	enabled = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/enabled", X3D::SFBool (false));

	set_browser ();
}

void
X3DGridTool::set_activeLayer ()
{
	enabled = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/enabled", X3D::SFBool (false));

	// Set tool fields from meta data.

	getTool () -> translation () .removeInterest (&X3DGridTool::set_translation, this);
	getTool () -> translation () .addInterest (&X3DGridTool::connectTranslation, this);

	getTool () -> rotation () .removeInterest (&X3DGridTool::set_rotation, this);
	getTool () -> rotation () .addInterest (&X3DGridTool::connectRotation, this);

	getTool () -> scale () .removeInterest (&X3DGridTool::set_scale, this);
	getTool () -> scale () .addInterest (&X3DGridTool::connectScale, this);

	getTool () -> dimension () .removeInterest (&X3DGridTool::set_dimension, this);
	getTool () -> dimension () .addInterest (&X3DGridTool::connectDimension, this);

	getTool () -> majorLineEvery () .removeInterest (&X3DGridTool::set_majorLineEvery, this);
	getTool () -> majorLineEvery () .addInterest (&X3DGridTool::connectMajorLineEvery, this);

	getTool () -> majorLineOffset () .removeInterest (&X3DGridTool::set_majorLineOffset, this);
	getTool () -> majorLineOffset () .addInterest (&X3DGridTool::connectMajorLineOffset, this);

	getTool () -> color () .removeInterest (&X3DGridTool::set_color, this);
	getTool () -> color () .addInterest (&X3DGridTool::connectColor, this);

	getTool () -> lineColor () .removeInterest (&X3DGridTool::set_lineColor, this);
	getTool () -> lineColor () .addInterest (&X3DGridTool::connectLineColor, this);

	getTool () -> majorLineColor () .removeInterest (&X3DGridTool::set_majorLineColor, this);
	getTool () -> majorLineColor () .addInterest (&X3DGridTool::connectMajorLineColor, this);

	getTool () -> snapDistance () .removeInterest (&X3DGridTool::set_snapDistance, this);
	getTool () -> snapDistance () .addInterest (&X3DGridTool::connectSnapDistance, this);

	getTool () -> snapToCenter () .removeInterest (&X3DGridTool::set_snapToCenter, this);
	getTool () -> snapToCenter () .addInterest (&X3DGridTool::connectSnapToCenter, this);

	getTool () -> snapping () .removeInterest (&X3DGridTool::set_snapping, this);
	getTool () -> snapping () .addInterest (&X3DGridTool::connectSnapping, this);

	getTool () -> collision () .removeInterest (&X3DGridTool::set_collision, this);
	getTool () -> collision () .addInterest (&X3DGridTool::connectCollision, this);

	getTool () -> isActive () .addInterest (&X3DGridTool::set_active, this);

	getTool () -> snapping ()     = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/snapping",     X3D::SFBool (true));
	getTool () -> snapDistance () = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/snapDistance", X3D::SFDouble (0.25));
	getTool () -> snapToCenter () = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/snapToCenter", X3D::SFBool (true));
	getTool () -> collision ()    = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/collision",    X3D::SFBool (false));

	fromMetadata ();
}

void
X3DGridTool::setEnabled (const bool value)
{
	if (value == enabled)
		return;

	enabled = value;

	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/enabled", enabled);

	set_browser ();
}

void
X3DGridTool::setPlane (const int32_t index)
{
	const auto & grid = getTool ();

	switch (index)
	{
		case 0:
			grid -> rotation () = X_PLANE_ROTATION;
			break;
		case 1:
			grid -> rotation () = Y_PLANE_ROTATION;
			break;
		case 2:
			grid -> rotation () = Z_PLANE_ROTATION;
			break;
		default:
			break;
	}
}

int32_t
X3DGridTool::getPlane () const
{
	constexpr float EPS  = math::radians (0.1);

	const auto & grid = getTool ();

	if (std::abs ((grid -> rotation () * ~X_PLANE_ROTATION) .angle ()) < EPS)
		return 0;

	else if (std::abs ((grid -> rotation () * ~Y_PLANE_ROTATION) .angle ()) < EPS)
		return 1;

	else if (std::abs ((grid -> rotation () * ~Z_PLANE_ROTATION) .angle ()) < EPS)
		return 2;

	return -1;
}

void
X3DGridTool::set_translation ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/translation", getTool () -> translation ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_rotation ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/rotation", getTool () -> rotation ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_scale ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/scale", getTool () -> scale ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_dimension ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/dimension", getTool () -> dimension ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_majorLineEvery ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/majorLineEvery", getTool () -> majorLineEvery ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_majorLineOffset ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/majorLineOffset", getTool () -> majorLineOffset ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_color ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/color", getTool () -> color ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_lineColor ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/lineColor", getTool () -> lineColor ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_majorLineColor ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/majorLineColor", getTool () -> majorLineColor ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_snapping ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/snapping", getTool () -> snapping ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_snapDistance ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/snapDistance", getTool () -> snapDistance ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_snapToCenter ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/snapToCenter", getTool () -> snapToCenter ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_collision ()
{
	setMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/collision", getTool () -> collision ());

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DGridTool::set_active ()
{
	const X3D::MFNode tool = { getTool () };

	if (getTool () -> isActive ())
	{
		resetUndoGroup ("properties", undoStep);
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
X3DGridTool::connectTranslation (const X3D::SFVec3f & field)
{
	field .removeInterest (&X3DGridTool::connectTranslation, this);
	field .addInterest (&X3DGridTool::set_translation, this);
}

void
X3DGridTool::connectRotation (const X3D::SFRotation & field)
{
	field .removeInterest (&X3DGridTool::connectRotation, this);
	field .addInterest (&X3DGridTool::set_rotation, this);
}

void
X3DGridTool::connectScale (const X3D::SFVec3f & field)
{
	field .removeInterest (&X3DGridTool::connectScale, this);
	field .addInterest (&X3DGridTool::set_scale, this);
}

void
X3DGridTool::connectDimension (const X3D::MFInt32 & field)
{
	field .removeInterest (&X3DGridTool::connectDimension, this);
	field .addInterest (&X3DGridTool::set_dimension, this);
}

void
X3DGridTool::connectMajorLineEvery (const X3D::MFInt32 & field)
{
	field .removeInterest (&X3DGridTool::connectMajorLineEvery, this);
	field .addInterest (&X3DGridTool::set_majorLineEvery, this);
}

void
X3DGridTool::connectMajorLineOffset (const X3D::MFInt32 & field)
{
	field .removeInterest (&X3DGridTool::connectMajorLineOffset, this);
	field .addInterest (&X3DGridTool::set_majorLineOffset, this);
}

void
X3DGridTool::connectColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (&X3DGridTool::connectColor, this);
	field .addInterest (&X3DGridTool::set_color, this);
}

void
X3DGridTool::connectLineColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (&X3DGridTool::connectLineColor, this);
	field .addInterest (&X3DGridTool::set_lineColor, this);
}

void
X3DGridTool::connectMajorLineColor (const X3D::SFColorRGBA & field)
{
	field .removeInterest (&X3DGridTool::connectMajorLineColor, this);
	field .addInterest (&X3DGridTool::set_majorLineColor, this);
}

void
X3DGridTool::connectSnapping (const X3D::SFBool & field)
{
	field .removeInterest (&X3DGridTool::connectSnapping, this);
	field .addInterest (&X3DGridTool::set_snapping, this);
}

void
X3DGridTool::connectSnapDistance (const X3D::SFDouble & field)
{
	field .removeInterest (&X3DGridTool::connectSnapDistance, this);
	field .addInterest (&X3DGridTool::set_snapDistance, this);
}

void
X3DGridTool::connectSnapToCenter (const X3D::SFBool & field)
{
	field .removeInterest (&X3DGridTool::connectSnapToCenter, this);
	field .addInterest (&X3DGridTool::set_snapToCenter, this);
}

void
X3DGridTool::connectCollision (const X3D::SFBool & field)
{
	field .removeInterest (&X3DGridTool::connectCollision, this);
	field .addInterest (&X3DGridTool::set_collision, this);
}

void
X3DGridTool::dispose ()
{
	X3DEditorObject::dispose ();
	X3DBaseInterface::dispose ();
}

X3DGridTool::~X3DGridTool ()
{ }

} // puck
} // titania
