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

#include "X3DGeometricPropertyNodeTool.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/ShapeContainer.h"

namespace titania {
namespace X3D {

X3DGeometricPropertyNodeTool::Fields::Fields () :
	           load (new SFBool (true)),
	modelViewMatrix (new SFMatrix4f ())
{ }

X3DGeometricPropertyNodeTool::X3DGeometricPropertyNodeTool () :
	X3DActiveLayerTool (),
	          enabled (false)
{
	//addType (X3DConstants::X3DGeometricPropertyNodeTool);
}

void
X3DGeometricPropertyNodeTool::initialize ()
{
	X3DActiveLayerTool::initialize ();

	getActiveLayer () .addInterest (this, &X3DGeometricPropertyNodeTool::set_activeLayer);
	load () .addInterest (getInlineNode () -> load ());

	getInlineNode () -> load () = load ();
}

void
X3DGeometricPropertyNodeTool::realize ()
{
	X3DActiveLayerTool::realize ();

	try
	{
		getBrowser () -> prepareEvents () .addInterest (this, &X3DGeometricPropertyNodeTool::prepareEvent);

		auto & set_modelViewMatrix = getInlineNode () -> getExportedNode ("TransformMatrix3D") -> getField <SFMatrix4f> ("matrix");
		modelViewMatrix () .addInterest (set_modelViewMatrix);
		set_modelViewMatrix = modelViewMatrix ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DGeometricPropertyNodeTool::prepareEvent ()
{
	try
	{
		getInlineNode () -> getExportedNode ("Switch") -> setField <SFInt32> ("whichChoice", int32_t (enabled), true);
	}
	catch (const X3DError & error)
	{ }

	enabled = false;
}

void
X3DGeometricPropertyNodeTool::set_activeLayer ()
{
	try
	{
		getInlineNode () -> getExportedNode ("Switch") -> setField <SFInt32> ("whichChoice", 0, true);
	}
	catch (const X3DError & error)
	{ }
}

void
X3DGeometricPropertyNodeTool::draw (const ShapeContainer* const container)
{
	if (getCurrentLayer () != getActiveLayer ())
		return;

	enabled = true;

	const auto mvm = container -> getModelViewMatrix () * getCurrentViewpoint () -> getCameraSpaceMatrix ();

	if (mvm != modelViewMatrix ())
		modelViewMatrix () = mvm;
}

X3DGeometricPropertyNodeTool::~X3DGeometricPropertyNodeTool ()
{ }

} // X3D
} // titania
