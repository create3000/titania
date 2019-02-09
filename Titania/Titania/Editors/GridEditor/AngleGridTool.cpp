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

#include "AngleGridTool.h"

#include "../../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Browser/Tools/TransformToolOptions.h>
#include <Titania/X3D/Tools/Grids/AngleGridTool.h>

#include <Titania/X3D/Components/Core/MetadataBoolean.h>
#include <Titania/X3D/Components/Core/MetadataFloat.h>
#include <Titania/X3D/Components/Core/MetadataInteger.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>

namespace titania {
namespace puck {

using math::pi;

AngleGridTool::AngleGridTool (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	     X3DGridTool (),
	            tool ()
{
	setName ("AngleGrid");

	addChildObjects (tool);

	setup ();
}

void
AngleGridTool::realize ()
{
	tool = getMasterBrowser () -> getExecutionContext () -> createNode <X3D::AngleGridTool> ();
}

const X3D::X3DPtr <X3D::X3DGridTool> &
AngleGridTool::getTool () const
{
	if (not tool)
		const_cast <AngleGridTool*> (this) -> realize ();

	return tool;
}

void
AngleGridTool::fromMetadata ()
{
	getTool () -> translation ()     = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/translation",     X3D::SFVec3f ());
	getTool () -> rotation ()        = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/rotation",        X3D::SFRotation ());
	getTool () -> scale ()           = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/scale",           X3D::SFVec3f (1, 1, 1));
	getTool () -> dimension ()       = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/dimension",       X3D::MFInt32 ({ 5, 16, 10 }));
	getTool () -> majorLineEvery ()  = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/majorLineEvery",  X3D::MFInt32 ({ 5, 2, 5 }));
	getTool () -> majorLineOffset () = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/majorLineOffset", X3D::MFInt32 ({ 0, 0, 0 }));
	getTool () -> color ()           = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/color",           X3D::SFColorRGBA (0.5, 0.5, 0.5, 0.2));
	getTool () -> lineColor ()       = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/lineColor",       X3D::SFColorRGBA (1, 0.7, 0.7, 0.2));
	getTool () -> majorLineColor ()  = getMetaData (getCurrentBrowser (), "/Titania/" + getName () + "/majorLineColor",  X3D::SFColorRGBA (1, 0.7, 0.7, 0.4));

	getCurrentBrowser () -> getTransformToolOptions () -> snapAngle () = 2 * pi <double> / getTool () -> dimension () [1];
}

void
AngleGridTool::set_dimension ()
{
	X3DGridTool::set_dimension ();

	getCurrentBrowser () -> getTransformToolOptions () -> snapAngle () = 2 * pi <double> / getTool () -> dimension () .at (1);
}

AngleGridTool::~AngleGridTool ()
{
	dispose ();
}

} // puck
} // titania
