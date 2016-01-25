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

#include "BackgroundEditor.h"

#include "../../Configuration/config.h"

#include <Titania/X3D/Components/EnvironmentalEffects/X3DBackgroundNode.h>

namespace titania {
namespace puck {

BackgroundEditor::BackgroundEditor (X3DBrowserWindow* const browserWindow) :
	            X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DBackgroundEditorInterface (get_ui ("Editors/BackgroundEditor.glade"), gconf_dir ()),
	                         sky (this, getSkyGradientBox (), "skyAngle", "skyColor"),
	                    skyColor (this,
	                              getSkyColorButton (),
	                              getSkyColorAdjustment (),
	                              getSkyColorGrid (),
	                              getAddSkyColorButton (),
	                              getRemoveSkyColorButton (),
	                              getSkyColorsScrolledWindow (),
	                              "skyColor"),
	                      ground (this, getGroundGradientBox (), "groundAngle", "groundColor"),
	                 groundColor (this,
	                              getGroundColorButton (),
	                              getGroundColorAdjustment (),
	                              getGroundColorGrid (),
	                              getAddGroundColorButton (),
	                              getRemoveGroundColorButton (),
	                              getGroundColorsScrolledWindow (),
	                              "groundColor"),
	                transparency (this, getTransparencyAdjustment (), getTransparencyScale (), "transparency"),
	                    changing (false)
{
	sky      .signal_whichChoice_changed () .connect (sigc::mem_fun (this, &BackgroundEditor::on_sky_whichChoice_changed)); 
	skyColor .signal_index_changed ()       .connect (sigc::mem_fun (this, &BackgroundEditor::on_sky_color_index_changed)); 

	ground      .signal_whichChoice_changed () .connect (sigc::mem_fun (this, &BackgroundEditor::on_ground_whichChoice_changed)); 
	groundColor .signal_index_changed ()       .connect (sigc::mem_fun (this, &BackgroundEditor::on_ground_color_index_changed)); 
	
	sky    .addClass ("notebook");
	ground .addClass ("notebook");
	
	skyColor    .setColorsSize (16);
	groundColor .setColorsSize (16);

	setup ();
}

void
BackgroundEditor::initialize ()
{
	X3DBackgroundEditorInterface::initialize ();

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &BackgroundEditor::set_selection);

	set_selection (getBrowserWindow () -> getSelection () -> getChildren ());
}

void
BackgroundEditor::set_selection (const X3D::MFNode & selection)
{
	X3D::X3DPtr <X3D::X3DBackgroundNode> background (selection .empty () ? nullptr : selection .back ());
	const auto nodes = background ? X3D::MFNode ({ background }) : X3D::MFNode ();

	sky          .setNodes (nodes);
	skyColor     .setNodes (nodes);
	ground       .setNodes (nodes);
	groundColor  .setNodes (nodes);
	transparency .setNodes (nodes);
}

void
BackgroundEditor::on_sky_whichChoice_changed ()
{
	__LOG__ << sky .getWhichChoice () << std::endl;

	if (changing)
		return;

	changing = true;

	skyColor .setIndex (sky .getWhichChoice ());

	changing = false;
}

void
BackgroundEditor::on_sky_color_index_changed ()
{
	__LOG__ << skyColor .getIndex () << std::endl;

	if (changing)
		return;

	changing = true;

	sky .setWhichChoice (skyColor .getIndex ());

	changing = false;
}

void
BackgroundEditor::on_ground_whichChoice_changed ()
{
	__LOG__ << ground .getWhichChoice () << std::endl;

	if (changing)
		return;

	changing = true;

	groundColor .setIndex (ground .getWhichChoice ());

	changing = false;
}

void
BackgroundEditor::on_ground_color_index_changed ()
{
	__LOG__ << groundColor .getIndex () << std::endl;

	if (changing)
		return;

	changing = true;

	ground .setWhichChoice (groundColor .getIndex ());

	changing = false;
}

BackgroundEditor::~BackgroundEditor ()
{
	dispose ();
}

} // puck
} // titania
