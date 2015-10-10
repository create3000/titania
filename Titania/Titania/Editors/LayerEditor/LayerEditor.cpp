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

#include "LayerEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Execution/World.h>
#include <Titania/X3D/Components/Layering/LayerSet.h>

namespace titania {
namespace puck {

namespace Columns {

static constexpr int VISIBLE   = 0;
static constexpr int TYPE_NAME = 1;
static constexpr int NAME      = 2;
static constexpr int WEIGHT    = 3;
static constexpr int STYLE     = 4;
static constexpr int INDEX     = 5;

};

namespace Weight {

static constexpr int NORMAL = 400;
static constexpr int BOLD   = 700;

};


LayerEditor::LayerEditor (X3DBrowserWindow* const browserWindow) :
	       X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DLayerEditorInterface (get_ui ("Editors/LayerEditor.xml"), gconf_dir ()),
	                  world (),
	               layerSet ()
{
	setup ();
}

void
LayerEditor::initialize ()
{
	X3DLayerEditorInterface::initialize ();

	getExecutionContext () .addInterest (this, &LayerEditor::set_executionContext);

	set_executionContext ();
}

void
LayerEditor::set_executionContext ()
{
	if (world)
	   world -> getLayerSet () .removeInterest (this, &LayerEditor::set_layersSet);

	world = getWorld ();

	if (world)
	{
		world -> getLayerSet () .addInterest (this, &LayerEditor::set_layersSet);

		set_layersSet ();
	}
}

void
LayerEditor::set_layersSet ()
{
	if (layerSet)
	{
		layerSet -> activeLayer () .removeInterest (this, &LayerEditor::set_layers);
		layerSet -> order ()       .removeInterest (this, &LayerEditor::set_layers);
	}

	layerSet = world -> getLayerSet ();

	if (layerSet)
	{
		layerSet -> activeLayer () .addInterest (this, &LayerEditor::set_layers);
		layerSet -> order ()       .addInterest (this, &LayerEditor::set_layers);

		set_layers ();
	}
}

void
LayerEditor::set_layers ()
{
	getLayerTreeView () .unset_model ();
	getLayerListStore () -> clear ();

	const auto & layerSet = getWorld () -> getLayerSet ();
	const auto & order    = layerSet -> order ();

	// Layer0

	const auto row     = getLayerListStore () -> append ();
	const bool visible = std::find (order .begin (), order .end (), 0) not_eq order  .end ();

	row -> set_value (Columns::INDEX,     0);
	row -> set_value (Columns::VISIBLE,   visible);
	row -> set_value (Columns::TYPE_NAME, std::string ("Layer"));
	row -> set_value (Columns::NAME,      std::string ("Layer 0"));
		
	if (0 == layerSet -> activeLayer ())
	{
		row -> set_value (Columns::WEIGHT, Weight::BOLD);
		row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
	}
	else
	{
		row -> set_value (Columns::WEIGHT, Weight::NORMAL);
		row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
	}

	// Layers

	int32_t index = 1;

	for (const auto & layer : layerSet -> layers ())
	{
		const auto row     = getLayerListStore () -> append ();
		const bool visible = std::find (order .begin (), order .end (), index) not_eq order .end ();

		row -> set_value (Columns::INDEX,     index);
		row -> set_value (Columns::VISIBLE,   visible);
		row -> set_value (Columns::TYPE_NAME, layer -> getTypeName ());
		row -> set_value (Columns::NAME,      layer -> getName ());
			
		if (index == layerSet -> activeLayer ())
		{
			row -> set_value (Columns::WEIGHT, Weight::BOLD);
			row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
		}
		else
		{
			row -> set_value (Columns::WEIGHT, Weight::NORMAL);
			row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
		}

		++ index;
	}

	getLayerTreeView () .set_model (getLayerListStore ());
}

void
LayerEditor::connectOrder ()
{
	layerSet -> order () .removeInterest (this, &LayerEditor::connectOrder);
	layerSet -> order () .addInterest (this, &LayerEditor::set_layers);
}

void
LayerEditor::connectActiveLayer ()
{
	layerSet -> activeLayer () .removeInterest (this, &LayerEditor::connectOrder);
	layerSet -> activeLayer () .addInterest (this, &LayerEditor::set_layers);
}

void
LayerEditor::on_visibility_toggled (const Glib::ustring & path)
{
	// Toggle row

	const auto & layerSet = getWorld () -> getLayerSet ();
	const auto   row      = getLayerListStore () -> get_iter (path);
	const bool   visible  = not getVisibilityCellRenderer () -> get_active ();

	row -> set_value (Columns::VISIBLE, visible);

	// Set order

	X3D::MFInt32 order;

	for (const auto & row : getLayerListStore () -> children ())
	{
		int32_t index   = 0;
		bool    visible = false;

		row -> get_value (Columns::INDEX,   index);
		row -> get_value (Columns::VISIBLE, visible);
	   
	   if (visible)
			order .emplace_back (index);
	}

	// Set order

	layerSet -> order () .removeInterest (this, &LayerEditor::set_layers);
	layerSet -> order () .addInterest (this, &LayerEditor::connectOrder);

	layerSet -> order () = std::move (order);
}

void
LayerEditor::on_layer_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	// Deactivate layer

	const int32_t activeLayer = layerSet -> activeLayer ();

	if (activeLayer not_eq -1)
	{
	   Gtk::TreePath path;

	   path .push_back (activeLayer);
		
		const auto row = getLayerListStore () -> get_iter (path);

		if (getLayerListStore () -> iter_is_valid (row))
		{
			row -> set_value (Columns::WEIGHT, Weight::NORMAL);
			row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
		}
	}

	// Activate layer

	layerSet -> activeLayer () .removeInterest (this, &LayerEditor::set_layers);
	layerSet -> activeLayer () .addInterest (this, &LayerEditor::connectActiveLayer);

	const auto row = getLayerListStore () -> get_iter (path);

	int32_t index = 0;

	row -> get_value (Columns::INDEX, index);

	if (index not_eq layerSet -> activeLayer ())
	{
		layerSet -> activeLayer () = index;

		row -> set_value (Columns::WEIGHT, Weight::BOLD);
		row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
	}
	else
	{
		layerSet -> activeLayer () = -1;

		row -> set_value (Columns::WEIGHT, Weight::NORMAL);
		row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
	}
}

void
LayerEditor::on_index_clicked ()
{
}

void
LayerEditor::on_top_clicked ()
{
}

void
LayerEditor::on_up_clicked ()
{
}

void
LayerEditor::on_down_clicked ()
{
}

void
LayerEditor::on_bottom_clicked ()
{
}

LayerEditor::~LayerEditor ()
{
	dispose ();
}

} // puck
} // titania
