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

#include "NodeEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include "../../Editors/AppearanceEditor/AppearanceEditor.h"
#include "../../Editors/BindableNodeEditor/BindableNodeEditor.h"
#include "../../Editors/GeometryPropertiesEditor/GeometryPropertiesEditor.h"
#include "../../Editors/InlineEditor/InlineEditor.h"
#include "../../Editors/LayerEditor/LayerEditor.h"
#include "../../Editors/LightEditor/LightEditor.h"
#include "../../Editors/NodePropertiesEditor/NodePropertiesEditor.h"
#include "../../Editors/PrecisionPlacementPanel/PrecisionPlacementPanel.h"
#include "../../Editors/SoundEditor/SoundEditor.h"
#include "../../Editors/TextEditor/TextEditor.h"
#include "../../Editors/TextureEditor/TextureEditor.h"

namespace titania {
namespace puck {

NodeEditor::NodeEditor (X3DBrowserWindow* const browserWindow) :
	                    X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	              X3DNodeEditorInterface (get_ui ("Editors/NodeEditor.glade")),
	X3DNotebook <X3DNodeEditorInterface> ()
{
	addPage ("NodePropertiesEditor",     getNodePropertiesEditorBox     ());

	addPage ("AppearanceEditor",         getAppearanceEditorBox         ());
	addPage ("TextureEditor",            getTextureEditorBox            ());
	addPage ("GeometryPropertiesEditor", getGeometryPropertiesEditorBox ());
	addPage ("TextEditor",               getTextEditorBox               ());

	addPage ("LayerEditor",              getLayerEditorBox              ());
	addPage ("BindableNodeEditor",       getBindableNodeEditorBox       ());
	addPage ("LightEditor",              getLightEditorBox              ());
	addPage ("SoundEditor",              getSoundEditorBox              ());
	addPage ("InlineEditor",             getInlineEditorBox             ());
	addPage ("PrecisionPlacementPanel",  getPrecisionPlacementPanelBox  ());

	addPage ("SculpToolEditor",          getSculpToolEditorBox          ());
	addPage ("ModelsPalette",            getModelsPaletteBox            ());

	setup ();
}

void
NodeEditor::initialize ()
{
	X3DNodeEditorInterface::initialize ();
	X3DNotebook <X3DNodeEditorInterface>::initialize ();
}

Gtk::HeaderBar &
NodeEditor::getHeaderBar () const
{
	return getCurrentPage <X3DEditorInterface> () -> getHeaderBar ();
}

void
NodeEditor::on_map_window ()
{
	//getNotebook () .set_tab_pos (Gtk::POS_LEFT);
}

void
NodeEditor::on_switch_page (Gtk::Widget* widget, guint pageNumber)
{
	getCurrentPage <X3DEditorInterface> () -> headerBar_changed () .removeInterest (&NodeEditor::on_headerBar_changed, this);

	X3DNotebook <X3DNodeEditorInterface>::on_switch_page (widget, pageNumber);

	getCurrentPage <X3DEditorInterface> () -> headerBar_changed () .addInterest (&NodeEditor::on_headerBar_changed, this);

	on_headerBar_changed ();
}

void
NodeEditor::on_headerBar_changed ()
{
	if (getWindow () .get_visible ())
		setTitleBar (getHeaderBar ());
}

NodeEditor::~NodeEditor ()
{
	X3DNotebook <X3DNodeEditorInterface>::dispose ();
	X3DNodeEditorInterface::dispose ();
}

} // puck
} // titania
