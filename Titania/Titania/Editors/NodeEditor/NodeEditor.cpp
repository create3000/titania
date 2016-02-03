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

#include "NodeEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

NodeEditor::NodeEditor (X3DBrowserWindow* const browserWindow) :
	        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	  X3DNodeEditorInterface (get_ui ("Editors/NodeEditor.glade"), gconf_dir ()),
	    nodePropertiesEditor (new NodePropertiesEditor (browserWindow)),
	        appearanceEditor (new AppearanceEditor (browserWindow)),
	           textureEditor (new TextureEditor (browserWindow)),
	geometryPropertiesEditor (new GeometryPropertiesEditor (browserWindow)),
	              textEditor (new TextEditor (browserWindow)),
	             layerEditor (new LayerEditor (browserWindow)),
	        backgroundEditor (new BackgroundEditor (browserWindow)),
	    navigationInfoEditor (new NavigationInfoEditor (browserWindow)),
	         viewpointEditor (new ViewpointEditor (browserWindow)),
	             lightEditor (new LightEditor (browserWindow)),
	            inlineEditor (new InlineEditor (browserWindow)),
	 precisionPlacementPanel (new PrecisionPlacementPanel (browserWindow)),
	                 editors ()
{
	const auto currentPage = getConfig () .getInteger ("currentPage");

	getNotebook () .set_current_page (currentPage);

	nodePropertiesEditor     -> reparent (getNodePropertiesEditorBox (),     getWindow ());
	appearanceEditor         -> reparent (getAppearanceEditorBox (),         getWindow ());
	textureEditor            -> reparent (getTextureEditorBox (),            getWindow ());
	geometryPropertiesEditor -> reparent (getGeometryPropertiesEditorBox (), getWindow ());
	textEditor               -> reparent (getTextEditorBox (),               getWindow ());
	layerEditor              -> reparent (getLayerEditorBox (),              getWindow ());
	backgroundEditor         -> reparent (getBackgroundEditorBox (),         getWindow ());
	navigationInfoEditor     -> reparent (getNavigationInfoEditorBox (),         getWindow ());
	viewpointEditor          -> reparent (getViewpointEditorBox (),         getWindow ());
	lightEditor              -> reparent (getLightEditorBox (),         getWindow ());
	inlineEditor             -> reparent (getInlineEditorBox (),         getWindow ());
	precisionPlacementPanel  -> reparent (getPrecisionPlacementPanelBox (), getWindow ());

	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (nodePropertiesEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (appearanceEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (textureEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (geometryPropertiesEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (textEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (layerEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (backgroundEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (navigationInfoEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (viewpointEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (lightEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (inlineEditor));
	editors .emplace_back (std::static_pointer_cast <X3DUserInterface> (precisionPlacementPanel));

	for (const auto & editor : editors)
		editor -> getWidget () .set_visible (false);

	if (editors [currentPage])
		editors [currentPage] -> getWidget () .set_visible (true);

	setup ();
}

void
NodeEditor::initialize ()
{
	X3DNodeEditorInterface::initialize ();

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &NodeEditor::set_selection);

	set_selection (getBrowserWindow () -> getSelection () -> getChildren ());
}

void
NodeEditor::set_selection (const X3D::MFNode & selection)
{
}

void
NodeEditor::on_switch_page (Gtk::Widget*, guint pageNumber)
{
	const size_t currentPage = getConfig () .getInteger ("currentPage");

	if (currentPage < editors .size ())
		editors [currentPage] -> getWidget () .set_visible (false);

	if (pageNumber < editors .size ())
		editors [pageNumber] -> getWidget () .set_visible (true);

	getConfig () .setItem ("currentPage", int (pageNumber));
}

NodeEditor::~NodeEditor ()
{
	dispose ();
}

} // puck
} // titania
