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
#include <Titania/X3D/Components/Layering/Layer.h>
#include <Titania/X3D/Components/Layering/LayerSet.h>
#include <Titania/X3D/Components/Layout/LayoutLayer.h>

namespace titania {
namespace puck {

namespace Columns {

static constexpr int VISIBLE            = 0;
static constexpr int TYPE_NAME          = 1;
static constexpr int NAME               = 2;
static constexpr int WEIGHT             = 3;
static constexpr int STYLE              = 4;
static constexpr int INDEX              = 5;
static constexpr int PICKABLE           = 6;
static constexpr int EYE                = 7;
static constexpr int ACTIVE_LAYER       = 8;
static constexpr int ACTIVE_LAYER_IMAGE = 9;

};

namespace Weight {

static constexpr int NORMAL = 400;
static constexpr int BOLD   = 700;

};


LayerEditor::LayerEditor (X3DBrowserWindow* const browserWindow) :
	       X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DLayerEditorInterface (get_ui ("Editors/LayerEditor.glade"), gconf_dir ()),
	                  world (),
	               layerSet (),
	                 layers ()
{
	addChildren (world, layerSet, layers);

	setup ();
}

void
LayerEditor::initialize ()
{
	X3DLayerEditorInterface::initialize ();

	getCurrentContext () .addInterest (this, &LayerEditor::set_executionContext);

	set_executionContext ();
}

void
LayerEditor::on_index_clicked ()
{
}

void
LayerEditor::set_executionContext ()
{
	if (world)
	   world -> getLayerSet () .removeInterest (this, &LayerEditor::set_layersSet);

	world = getCurrentWorld ();

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
		layerSet -> privateActiveLayer () .removeInterest (this, &LayerEditor::set_treeView);
		layerSet -> activeLayer ()        .removeInterest (this, &LayerEditor::set_treeView);
		layerSet -> order ()              .removeInterest (this, &LayerEditor::set_treeView);
		layerSet -> layers ()             .removeInterest (this, &LayerEditor::set_layers);
	}

	layerSet = world -> getLayerSet ();

	if (layerSet)
	{
		layerSet -> privateActiveLayer () .addInterest (this, &LayerEditor::set_treeView);
		layerSet -> activeLayer ()        .addInterest (this, &LayerEditor::set_treeView);
		layerSet -> order ()              .addInterest (this, &LayerEditor::set_treeView);
		layerSet -> layers ()             .addInterest (this, &LayerEditor::set_layers);

		set_layers ();

		const bool isDefaultLayerSet = (layerSet == world -> getDefaultLayerSet ());

		getNewLayerSetButton ()    .set_sensitive (isDefaultLayerSet);
		getRemoveLayerSetButton () .set_sensitive (not isDefaultLayerSet);
		getLayerActionBox ()       .set_sensitive (not isDefaultLayerSet);

		getLayerSetLabel () .set_text (isDefaultLayerSet ? _ ("Default LayerSet") : _ ("LayerSet"));
	}

	getWidget () .set_sensitive (not inPrototypeInstance ());
}

void
LayerEditor::set_order (const X3D::UndoStepPtr & undoStep)
{
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

	undoStep -> addObjects (layerSet);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (layerSet -> order ()), layerSet -> order ());

	disconnectOrder ();
	layerSet -> order () = std::move (order);

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (layerSet -> order ()), layerSet -> order ());
}

void
LayerEditor::set_layers ()
{
	for (const auto & layer : layers)
	{
	   if (layer)
	      layer -> isPickable () .removeInterest (this, &LayerEditor::set_treeView);
	}

	layers .assign (layerSet -> layers () .begin (), layerSet -> layers () .end ());

	for (const auto & layer : layers)
	{
	   if (layer)
	      layer -> isPickable () .addInterest (this, &LayerEditor::set_treeView);
	}

	set_treeView ();
}

void
LayerEditor::set_treeView ()
{
	getLayerTreeView () .unset_model ();
	getLayerListStore () -> clear ();

	// Layer0

	add_layer (X3D::SFNode (layerSet -> getLayer0 ()), layerSet -> getLayer0 (), 0);

	// Layers

	int32_t index = 1;

	for (const auto & node : layerSet -> layers ())
	{
		add_layer (node, layers [index - 1], index);

		++ index;
	}

	getLayerTreeView () .set_model (getLayerListStore ());
}

void
LayerEditor::add_layer (const X3D::SFNode & node, const X3D::X3DLayerNodePtr & layer, const int32_t index)
{
	const auto   row         = getLayerListStore () -> append ();
	const auto & order       = layerSet -> order ();
	const bool   visible     = std::find (order .begin (), order .end (), index) not_eq order .end ();
	const bool   activeLayer = (index == layerSet -> activeLayer ());

	row -> set_value (Columns::INDEX,              index);
	row -> set_value (Columns::VISIBLE,            visible);
	row -> set_value (Columns::EYE,                std::string (visible ? "EyeOpen" : "EyeClosed"));
	row -> set_value (Columns::PICKABLE,           std::string (layer and layer -> isPickable () ? "Hand" : "gtk-stop"));
	row -> set_value (Columns::TYPE_NAME,          node -> getTypeName ());
	row -> set_value (Columns::NAME,               node -> getName ());
	row -> set_value (Columns::ACTIVE_LAYER,       activeLayer);
	row -> set_value (Columns::ACTIVE_LAYER_IMAGE, std::string (activeLayer ? "WalkViewer" : "gtk-stop"));
		
	if (index == layerSet -> getActiveLayerIndex ())
	{
		row -> set_value (Columns::WEIGHT, Weight::BOLD);
		row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
	}
	else
	{
		row -> set_value (Columns::WEIGHT, Weight::NORMAL);
		row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
	}
}

void
LayerEditor::connectPrivateActiveLayer ()
{
	layerSet -> privateActiveLayer () .removeInterest (this, &LayerEditor::connectPrivateActiveLayer);
	layerSet -> privateActiveLayer () .addInterest (this, &LayerEditor::set_treeView);
}

void
LayerEditor::connectActiveLayer ()
{
	layerSet -> activeLayer () .removeInterest (this, &LayerEditor::connectActiveLayer);
	layerSet -> activeLayer () .addInterest (this, &LayerEditor::set_treeView);
}

void
LayerEditor::connectOrder ()
{
	layerSet -> order () .removeInterest (this, &LayerEditor::connectOrder);
	layerSet -> order () .addInterest (this, &LayerEditor::set_treeView);
}

void
LayerEditor::connectLayers ()
{
	layerSet -> layers () .removeInterest (this, &LayerEditor::connectLayers);
	layerSet -> layers () .addInterest (this, &LayerEditor::set_layers);
}

void
LayerEditor::connectIsPickable (const X3D::X3DLayerNodePtr & layer)
{
	layer -> isPickable () .removeInterest (this, &LayerEditor::connectIsPickable);
	layer -> isPickable () .addInterest (this, &LayerEditor::set_treeView);
}

void
LayerEditor::disconnectPrivateActiveLayer ()
{
	layerSet -> privateActiveLayer () .removeInterest (this, &LayerEditor::set_treeView);
	layerSet -> privateActiveLayer () .addInterest (this, &LayerEditor::connectPrivateActiveLayer);
}

void
LayerEditor::disconnectActiveLayer ()
{
	layerSet -> activeLayer () .removeInterest (this, &LayerEditor::set_treeView);
	layerSet -> activeLayer () .addInterest (this, &LayerEditor::connectActiveLayer);
}

void
LayerEditor::disconnectOrder ()
{
	layerSet -> order () .removeInterest (this, &LayerEditor::set_treeView);
	layerSet -> order () .addInterest (this, &LayerEditor::connectOrder);
}

void
LayerEditor::disconnectLayers ()
{
	layerSet -> layers () .removeInterest (this, &LayerEditor::set_layers);
	layerSet -> layers () .addInterest (this, &LayerEditor::connectLayers);
}

void
LayerEditor::disconnectIsPickable (const X3D::X3DLayerNodePtr & layer)
{
	layer -> isPickable () .removeInterest (this, &LayerEditor::set_treeView);
	layer -> isPickable () .addInterest (this, &LayerEditor::connectIsPickable, layer);
}

bool
LayerEditor::on_layers_button_release_event (GdkEventButton* event)
{
	Gtk::TreePath        path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	getLayerTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	const auto row = getLayerListStore () -> get_iter (path);

	if (getLayerListStore () -> iter_is_valid (row))
	{
		if (column == getPickableColumn () .operator -> ())
		{
			on_pickable_toggled (path);
			return true;
		}

		if (column == getVisibilityColumn () .operator -> ())
		{
			on_visibility_toggled (path);
			return true;
		}

		if (column == getActiveLayerColumn () .operator -> ())
		{
			on_active_layer_toggled (path);
			return true;
		}
	}

	return false;
}

void
LayerEditor::on_visibility_toggled (const Gtk::TreePath & path)
{
	// Toggle row

	const auto row     = getLayerListStore () -> get_iter (path);
	bool       visible = false;

	row -> get_value (Columns::VISIBLE, visible);

	visible = not visible;

	row -> set_value (Columns::VISIBLE, visible);
	row -> set_value (Columns::EYE,     std::string (visible ? "EyeOpen" : "EyeClosed"));

	// Set order

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Visibility Of Layer"));

	set_order (undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
LayerEditor::on_pickable_toggled (const Gtk::TreePath & path)
{
	size_t index = path .back () - 1;

	if (index >= layerSet -> layers () .size ())
	   return;

	const auto & layer = layers [index];

	if (not layer)
	   return;

	// Toggle isPickable

	disconnectIsPickable (layer);

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Layer isPickable"));

	undoStep -> addObjects (layer);
	undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (layer -> isPickable ()), layer -> isPickable ());

	layer -> isPickable () = not layer -> isPickable ();

	undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (layer -> isPickable ()), layer -> isPickable ());
	getBrowserWindow () -> addUndoStep (undoStep);

	// Toggle row

	const auto row = getLayerListStore () -> get_iter (path);

	row -> set_value (Columns::PICKABLE, std::string (layer -> isPickable () ? "Hand" : "gtk-stop"));
}

void
LayerEditor::on_active_layer_toggled (const Gtk::TreePath & path)
{
	if (layerSet == getCurrentWorld () -> getDefaultLayerSet ())
		return;

	int32_t index = path .back ();
	int32_t last  = -1;

	const auto children = getLayerListStore () -> children ();
		
	// Find last active layer

	for (const auto & row : children)
	{
		bool active = false;

		row -> get_value (Columns::ACTIVE_LAYER, active);

		if (not active)
		   continue;

		row -> get_value (Columns::INDEX, last);
		break;
	}

	// Toggle last to false

	if (last >= 0)
	{
		const auto row = children [last];
		row -> set_value (Columns::ACTIVE_LAYER,       false);
		row -> set_value (Columns::ACTIVE_LAYER_IMAGE, std::string ("gtk-stop"));
	}

	// Remove active layer

	if (layerSet -> getActiveLayerIndex () >= 0 and layerSet -> getActiveLayerIndex () < int32_t (children .size ()))
	{
		const auto row = children [layerSet -> getActiveLayerIndex ()];
		row -> set_value (Columns::WEIGHT, Weight::NORMAL);
		row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
	}

	// Toggle current selection

	bool active = false;

	if (index not_eq last)
	{
		const auto row = getLayerListStore () -> get_iter (path);

		row -> get_value (Columns::ACTIVE_LAYER, active);

		active = not active;

		row -> set_value (Columns::ACTIVE_LAYER, active);
		row -> set_value (Columns::ACTIVE_LAYER_IMAGE, std::string (active ? "WalkViewer" : "gtk-stop"));
	}

	// Set value in LayerSet

	disconnectActiveLayer ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Active Layer"));

	undoStep -> addObjects (layerSet);
	undoStep -> addUndoFunction (&X3D::SFInt32::setValue, std::ref (layerSet -> activeLayer ()), layerSet -> activeLayer ());

	layerSet -> activeLayer () = active ? index : -1;

	undoStep -> addRedoFunction (&X3D::SFInt32::setValue, std::ref (layerSet -> activeLayer ()), layerSet -> activeLayer ());
	getBrowserWindow () -> addUndoStep (undoStep);

	// Set active layer

	if (layerSet -> getActiveLayerIndex () >= 0 and layerSet -> getActiveLayerIndex () < int32_t (children .size ()))
	{
		const auto row = children [layerSet -> getActiveLayerIndex ()];
		row -> set_value (Columns::WEIGHT, Weight::BOLD);
		row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
	}
}

void
LayerEditor::on_layer_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	// Deactivate layer

	const int32_t activeLayer = layerSet -> getActiveLayerIndex ();

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

	disconnectPrivateActiveLayer ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Active Layer"));

	undoStep -> addObjects (layerSet);
	undoStep -> addUndoFunction (&X3D::SFInt32::setValue, std::ref (layerSet -> privateActiveLayer ()), layerSet -> privateActiveLayer ());

	const auto row = getLayerListStore () -> get_iter (path);

	int32_t index = 0;

	row -> get_value (Columns::INDEX, index);

	if (index not_eq layerSet -> getActiveLayerIndex ())
	{
		layerSet -> privateActiveLayer () = index;

		row -> set_value (Columns::WEIGHT, Weight::BOLD);
		row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
	}
	else 
	{
		layerSet -> privateActiveLayer () = -1;

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
	}

	undoStep -> addRedoFunction (&X3D::SFInt32::setValue, std::ref (layerSet -> privateActiveLayer ()), layerSet -> privateActiveLayer ());
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
LayerEditor::on_layer_selection_changed ()
{
	const bool haveSelection = not getLayerSelection () -> get_selected_rows () .empty ();

	int32_t selectedIndex = -1;

	if (haveSelection)
		getLayerSelection () -> get_selected () -> get_value (Columns::INDEX, selectedIndex);
	
	const bool isFirstRow = selectedIndex == 1;
	const bool isLastRow  = selectedIndex == int32_t (getLayerListStore () -> children () .size ()) - 1;

	// Move box

	getRemoveLayerButton () .set_sensitive (haveSelection and selectedIndex not_eq 0);
	getMoveLayerBox ()      .set_sensitive (haveSelection and selectedIndex not_eq 0);

	getTopButton ()    .set_sensitive (not isFirstRow);
	getUpButton ()     .set_sensitive (not isFirstRow);
	getDownButton ()   .set_sensitive (not isLastRow);
	getBottomButton () .set_sensitive (not isLastRow);
}

void
LayerEditor::on_new_layer_set_button_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New LayerSet"));
	const X3D::SFNode node (getCurrentContext () -> createNode <X3D::LayerSet> ());

	getCurrentContext () -> realize ();
	getBrowserWindow () -> pushBackIntoArray (X3D::SFNode (getCurrentContext ()), getCurrentContext () -> getRootNodes (), node, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
LayerEditor::on_remove_layer_set_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove LayerSet"));

	getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), { layerSet }, true, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
LayerEditor::on_new_layer_activated ()
{
	// Add to LayerSet

	disconnectLayers ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New Layer"));
	const X3D::SFNode node (getCurrentContext () -> createNode <X3D::Layer> ());

	getCurrentContext () -> realize ();
	getBrowserWindow () -> pushBackIntoArray (X3D::SFNode (layerSet), layerSet -> layers (), node, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	// Add to tree view

	layers .emplace_back (layerSet -> layers () .back ());

	add_layer (layerSet -> layers () .back (), layers .back (), layers .size ());
}

void
LayerEditor::on_new_layout_layer_activated ()
{
	// Add to LayerSet

	disconnectLayers ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New LayoutLayer"));
	const X3D::SFNode node (getCurrentContext () -> createNode <X3D::LayoutLayer> ());

	getCurrentContext () -> realize ();
	getBrowserWindow () -> pushBackIntoArray (X3D::SFNode (layerSet), layerSet -> layers (), node, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	// Add to tree view

	layers .emplace_back (layerSet -> layers () .back ());

	add_layer (layerSet -> layers () .back (), layers .back (), layers .size ());
}

void
LayerEditor::on_remove_layer_button_clicked ()
{
	const bool haveSelection = not getLayerSelection () -> get_selected_rows () .empty ();

	if (not haveSelection)
	   return;

	int32_t selectedIndex = -1;

	getLayerSelection () -> get_selected () -> get_value (Columns::INDEX, selectedIndex);

	if (selectedIndex == 0)
	   return;

	-- selectedIndex;

	// Remove from tree view

	getLayerListStore () -> erase (getLayerSelection () -> get_selected ());

	// Update index
	{
		int32_t index = 0;

		for (const auto & row : getLayerListStore () -> children ())
		   row -> set_value (Columns::INDEX, index ++);
	}

	// Remove from  LayerSet

	disconnectLayers ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Layer"));
	getBrowserWindow () -> removeNode (getCurrentContext (), X3D::SFNode (layerSet), layerSet -> layers (), selectedIndex, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
LayerEditor::on_top_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Layer To Top"));

	disconnectLayers ();

	// Move to top
	{
		const auto selected = getLayerSelection () -> get_selected ();

		int32_t selectedIndex = 0;

		selected -> get_value (Columns::INDEX, selectedIndex);

		if (selectedIndex == 0)
			return;

		// Move row
		{
			Gtk::TreePath destination;

			destination .push_back (1);

			getLayerListStore () -> move (selected, getLayerListStore () -> get_iter (destination));
		}

		// Update index
		{
			int32_t index = 0;

			for (const auto & row : getLayerListStore () -> children ())
			   row -> set_value (Columns::INDEX, index ++);
		}

		// Move layer
		{
			-- selectedIndex;

			getBrowserWindow () -> moveValueWithinArray (X3D::SFNode (layerSet), layerSet -> layers (), selectedIndex, 0, undoStep);
		}
	}

	set_order (undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);

	on_layer_selection_changed ();
}

void
LayerEditor::on_up_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Layer Up"));

	undoStep -> addObjects (layerSet);

	disconnectLayers ();

	// Move to top
	{
		const auto selected = getLayerSelection () -> get_selected ();

		int32_t selectedIndex = 0;

		selected -> get_value (Columns::INDEX, selectedIndex);

		if (selectedIndex == 0)
			return;

		if (selectedIndex == 1)
			return;

		// Move row
		{
			Gtk::TreePath destination;

			destination .push_back (selectedIndex - 1);

			getLayerListStore () -> move (selected, getLayerListStore () -> get_iter (destination));
		}

		// Update index
		{
			int32_t index = 0;

			for (const auto & row : getLayerListStore () -> children ())
			   row -> set_value (Columns::INDEX, index ++);
		}

		// Move layer
		{
			-- selectedIndex;

			getBrowserWindow () -> moveValueWithinArray (X3D::SFNode (layerSet), layerSet -> layers (), selectedIndex, selectedIndex - 1, undoStep);
		}
	}

	set_order (undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);

	on_layer_selection_changed ();
}

void
LayerEditor::on_down_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Layer Down"));

	disconnectLayers ();

	// Move to top
	{
		const auto selected = getLayerSelection () -> get_selected ();
		const auto children = getLayerListStore () -> children ();

		int32_t selectedIndex = 0;

		selected -> get_value (Columns::INDEX, selectedIndex);

		if (selectedIndex == 0)
			return;

		if (selectedIndex > int32_t (children .size () - 1))
			return;

		// Move row
		{
			Gtk::TreePath destination;

			destination .push_back (selectedIndex + 2);

			if (destination .back () < int32_t (children .size ()))
				getLayerListStore () -> move (selected, getLayerListStore () -> get_iter (destination));
			else
				getLayerListStore () -> move (selected, children .end ());
		}

		// Update index
		{
			int32_t index = 0;

			for (const auto & row : getLayerListStore () -> children ())
			   row -> set_value (Columns::INDEX, index ++);
		}

		// Move layer
		{
			-- selectedIndex;

			getBrowserWindow () -> moveValueWithinArray (X3D::SFNode (layerSet), layerSet -> layers (), selectedIndex, selectedIndex + 2, undoStep);
		}
	}

	set_order (undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);

	on_layer_selection_changed ();
}

void
LayerEditor::on_bottom_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Layer To Bottom"));

	disconnectLayers ();

	// Move to bottom
	{
		const auto selected = getLayerSelection () -> get_selected ();

		int32_t selectedIndex = 0;

		selected -> get_value (Columns::INDEX, selectedIndex);

		if (selectedIndex == 0)
			return;
		
		// Move row
		getLayerListStore () -> move (selected, getLayerListStore () -> children () .end ());
	
		// Update index
		{
			int32_t index = 0;

			for (const auto & row : getLayerListStore () -> children ())
			   row -> set_value (Columns::INDEX, index ++);
		}

		// Move layer
		{
			-- selectedIndex;

			getBrowserWindow () -> moveValueWithinArray (X3D::SFNode (layerSet), layerSet -> layers (), selectedIndex, layerSet -> layers () .size (), undoStep);
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);

	on_layer_selection_changed ();
}

LayerEditor::~LayerEditor ()
{
	dispose ();
}

} // puck
} // titania
