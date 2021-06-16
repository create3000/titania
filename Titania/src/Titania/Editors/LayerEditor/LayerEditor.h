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

#ifndef __TITANIA_EDITORS_LAYER_EDITOR_LAYER_EDITOR_H__
#define __TITANIA_EDITORS_LAYER_EDITOR_LAYER_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DLayerEditorInterface.h"

namespace titania {
namespace puck {

class X3DBrowserWidget;

class LayerEditor :
	public X3DLayerEditorInterface
{
public:

	///  @name Construction

	LayerEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~LayerEditor () final override;


private:

	///  @name Member types

	struct Columns;
	struct Weight;

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_executionContext ();

	void
	set_layersSet ();

	void
	set_layers ();

	void
	set_order (const X3D::UndoStepPtr &);

	void
	set_treeView ();

	void
	set_name (X3D::X3DLayerNode* const layer);

	void
	add_layer (const X3D::SFNode &, const X3D::X3DPtr <X3D::X3DLayerNode> &, const int32_t);

	void
	connectPrivateActiveLayer ();

	void
	connectActiveLayer ();

	void
	connectOrder ();

	void
	connectLayers ();

	void
	connectIsPickable (const X3D::X3DPtr <X3D::X3DLayerNode> &);

	void
	disconnectPrivateActiveLayer ();

	void
	disconnectActiveLayer ();

	void
	disconnectOrder ();

	void
	disconnectLayers ();

	void
	disconnectIsPickable (const X3D::X3DPtr <X3D::X3DLayerNode> &);

	virtual
	void
	on_index_clicked () final override;

	virtual
	void
	on_layer_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) final override;

	virtual
	bool
	on_layers_button_release_event (GdkEventButton*) final override;

	void
	on_visibility_toggled (const Gtk::TreePath &);

	void
	on_pickable_toggled (const Gtk::TreePath &);

	void
	on_active_layer_toggled (const Gtk::TreePath &);

	virtual
	void
	on_layer_selection_changed () final override;

	virtual
	void
	on_new_layer_set_button_clicked () final override;

	virtual
	void
	on_remove_layer_set_clicked () final override;

	virtual
	void
	on_new_layer_popup_clicked () final override;

	virtual
	void
	on_new_layer_clicked () final override;

	virtual
	void
	on_new_layout_layer_clicked () final override;

	virtual
	void
	on_remove_layer_button_clicked () final override;

	virtual
	void
	on_top_clicked () final override;

	virtual
	void
	on_up_clicked () final override;

	virtual
	void
	on_down_clicked () final override;

	virtual
	void
	on_bottom_clicked () final override;

	///  @name Members

	X3D::WorldPtr                        world;
	X3D::X3DPtr <X3D::LayerSet>          layerSet;
	X3D::X3DPtrArray <X3D::X3DLayerNode> layers;
	NameEntry                            nodeName;

};

} // puck
} // titania

#endif
