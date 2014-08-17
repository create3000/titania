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

#include "LightEditor.h"

#include "../../Browser/BrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"

namespace titania {
namespace puck {

LightEditor::LightEditor (BrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	  X3DLightEditorInterface (get_ui ("Dialogs/LightEditor.xml"), gconf_dir ()),
	X3DDirectionalLightEditor (),
	      X3DPointLightEditor (),
	       X3DSpotLightEditor (),
	                 nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                   global (getBrowserWindow (), getGlobalCheckButton (), "global"),
	                       on (getBrowserWindow (), getOnCheckButton (), "on"),
	                    color (getBrowserWindow (), getColorButton (), getColorAdjustment (), getColorBox (), "color"),
	                intensity (getBrowserWindow (), getIntensityAdjustment (), getIntensityBox (), "intensity"),
	         ambientIntensity (getBrowserWindow (), getAmbientIntensityAdjustment (), getAmbientIntensityBox (), "ambientIntensity")
{
	setup ();
}

void
LightEditor::initialize ()
{
	X3DDirectionalLightEditor::initialize ();
	X3DPointLightEditor::initialize ();
	X3DSpotLightEditor::initialize ();

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &LightEditor::set_selection);

	set_selection (getBrowser () -> getSelection () -> getChildren ());
}

void
LightEditor::set_selection (const X3D::MFNode & selection)
{
	X3D::X3DPtr <X3D::X3DLightNode> lightNode (selection .empty () ? nullptr : selection .back ());

	setDirectionalLight (lightNode);
	setPointLight (lightNode);
	setSpotLight (lightNode);

	const auto lightNodes = lightNode ? X3D::MFNode ({ lightNode }) : X3D::MFNode ();

	nodeName         .setNode  (X3D::SFNode (lightNode));
	global           .setNodes (lightNodes);
	on               .setNodes (lightNodes);
	color            .setNodes (lightNodes);
	intensity        .setNodes (lightNodes);
	ambientIntensity .setNodes (lightNodes);

	getLightBox () .set_sensitive (lightNode);
	getWindow () .resize (getWindow () .get_width (), 1);
}

void
LightEditor::on_index_clicked ()
{
	const auto nodeIndex = std::dynamic_pointer_cast <NodeIndex> (addDialog ("NodeIndex"));
	nodeIndex -> setTypeNames ({ "DirectionalLight", "PointLight", "SpotLight" });
	nodeIndex -> refresh ();
}

LightEditor::~LightEditor ()
{
	dispose ();
}

} // puck
} // titania
