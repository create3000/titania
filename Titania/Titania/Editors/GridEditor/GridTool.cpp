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

#include <Titania/X3D/Tools/Grids/GridTool.h>

#include <Titania/X3D/Components/Core/MetadataBoolean.h>
#include <Titania/X3D/Components/Core/MetadataFloat.h>
#include <Titania/X3D/Components/Core/MetadataInteger.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>

namespace titania {
namespace puck {

GridTool::GridTool (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	     X3DGridTool (),
	            tool ()
{
	setName ("Grid");

	setup ();
}

void
GridTool::realize ()
{
	const auto & browser = isEnabled () ? getCurrentBrowser () : getMasterBrowser ();

	tool = browser -> getExecutionContext () -> createNode <X3D::GridTool> ();

	tool -> getExecutionContext () -> realize ();
}

const X3D::X3DPtr <X3D::X3DGridTool> &
GridTool::getTool () const
{
	if (not tool)
		const_cast <GridTool*> (this) -> realize ();

	return tool;
}

void
GridTool::configure (const X3D::X3DPtr <X3D::MetadataSet> & metadataSet)
{
	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataFloat> ("translation") -> value ();

		getTool () -> translation () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> translation () = X3D::Vector3f ();
	}

	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataFloat> ("rotation") -> value ();

		getTool () -> rotation () = X3D::Rotation4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> rotation () = X3D::Rotation4f ();
	}

	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataFloat> ("scale") -> value ();

		getTool () -> scale () = X3D::Vector3f (v .at (0), v .at (1), v .at (2));
	}
	catch (...)
	{
		getTool () -> scale () = X3D::Vector3f (1, 1, 1);
	}

	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataInteger> ("dimension") -> value ();

		getTool () -> dimension () = v;
		getTool () -> dimension () .resize (3, X3D::SFInt32 (10));
	}
	catch (...)
	{
		getTool () -> dimension () = { 10, 10, 10 };
	}

	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataInteger> ("majorLineEvery") -> value ();

		getTool () -> majorLineEvery () = v;
	}
	catch (...)
	{
		getTool () -> majorLineEvery () = { 5, 5, 5 };
	}

	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataInteger> ("majorLineOffset") -> value ();

		getTool () -> majorLineOffset () = v;
	}
	catch (...)
	{
		getTool () -> majorLineOffset () = { 0, 0, 0 };
	}

	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataFloat> ("color") -> value ();

		getTool () -> color () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> color () = X3D::Color4f (0.5, 0.5, 0.5, 0.2);
	}

	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataFloat> ("lineColor") -> value ();

		getTool () -> lineColor () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> lineColor () = X3D::Color4f (1, 0.7, 0.7, 0.2);
	}

	try
	{
		const auto & v = metadataSet -> getValue <X3D::MetadataFloat> ("majorLineColor") -> value ();

		getTool () -> majorLineColor () = X3D::Color4f (v .at (0), v .at (1), v .at (2), v .at (3));
	}
	catch (...)
	{
		getTool () -> majorLineColor () = X3D::Color4f (1, 0.7, 0.7, 0.4);
	}
}

GridTool::~GridTool ()
{
	dispose ();
}

} // puck
} // titania
