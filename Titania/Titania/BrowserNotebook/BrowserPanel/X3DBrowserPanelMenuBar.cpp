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

#include "X3DBrowserPanelMenuBar.h"

#include "ViewpointObserver.h"
#include "../NotebookPage/NotebookPage.h"

#include <Titania/X3D/Browser/Core/BrowserOptions.h>
#include <Titania/X3D/Components/EnvironmentalEffects/X3DBackgroundNode.h>
#include <Titania/X3D/Components/EnvironmentalEffects/X3DFogObject.h>
#include <Titania/X3D/Components/EnvironmentalSensor/TransformSensor.h>
#include <Titania/X3D/Components/EnvironmentalSensor/VisibilitySensor.h>
#include <Titania/X3D/Components/Lighting/X3DLightNode.h>
#include <Titania/X3D/Components/Navigation/LOD.h>
#include <Titania/X3D/Components/Navigation/X3DViewpointNode.h>
#include <Titania/X3D/Components/Sound/Sound.h>
#include <Titania/X3D/Parser/Filter.h>

namespace titania {
namespace puck {

X3DBrowserPanelMenuBar::X3DBrowserPanelMenuBar () :
	   X3DBrowserPanel (),
	           browser (getPage () -> getMainBrowser ()),
	 viewpointObserver (),
	          changing (false)
{
	addChildObjects (browser);
}

void
X3DBrowserPanelMenuBar::initialize ()
{
	hasFocus () .addInterest (&X3DBrowserPanelMenuBar::set_focus, this);

	getBrowserWindow () -> getMenubarAction () -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelMenuBar::on_menubar_toggled));

	getPage () -> getMainBrowser () -> signal_hierarchy_changed () .connect (sigc::mem_fun (this, &X3DBrowserPanelMenuBar::on_main_browser_hierarchy_changed));

	getPage () -> getMainBrowser () -> getExecutionContext () .addInterest (&X3DBrowserPanelMenuBar::set_scene, this);

	getPage () -> getMainBrowser () -> getLightTools ()            .addInterest (&X3DBrowserPanelMenuBar::set_lightTools,            this);
	getPage () -> getMainBrowser () -> getProximitySensorTools ()  .addInterest (&X3DBrowserPanelMenuBar::set_proximitySensorTools,  this);
	getPage () -> getMainBrowser () -> getSoundTools ()            .addInterest (&X3DBrowserPanelMenuBar::set_soundTools,            this);
	getPage () -> getMainBrowser () -> getTransformSensorTools ()  .addInterest (&X3DBrowserPanelMenuBar::set_transformSensorTools,  this);
	getPage () -> getMainBrowser () -> getVisibilitySensorTools () .addInterest (&X3DBrowserPanelMenuBar::set_visibilitySensorTools, this);
	getPage () -> getMainBrowser () -> getViewpointTools ()        .addInterest (&X3DBrowserPanelMenuBar::set_viewpointTools,        this);

	on_menubar_toggled ();
	on_main_browser_hierarchy_changed (nullptr);

	getStraightenHorizonMenuItem () .set_active (getConfig () -> getItem <bool> ("straightenHorizon"));

	set_lightTools            (getPage () -> getMainBrowser () -> getLightTools ());
	set_proximitySensorTools  (getPage () -> getMainBrowser () -> getProximitySensorTools ());
	set_soundTools            (getPage () -> getMainBrowser () -> getSoundTools ());
	set_transformSensorTools  (getPage () -> getMainBrowser () -> getTransformSensorTools ());
	set_visibilitySensorTools (getPage () -> getMainBrowser () -> getVisibilitySensorTools ());
	set_viewpointTools        (getPage () -> getMainBrowser () -> getViewpointTools ());
}

void
X3DBrowserPanelMenuBar::setLocalBrowser (const X3D::BrowserPtr & value)
{
	browser -> getBrowserOptions () -> Shading () .removeInterest (&X3DBrowserPanelMenuBar::set_shading, this);
	browser -> initialized ()          .removeInterest (&X3DBrowserPanelMenuBar::set_initialized,        this);
	browser -> getViewer ()            .removeInterest (&X3DBrowserPanelMenuBar::set_viewer,             this);
	browser -> getStraightenHorizon () .removeInterest (&X3DBrowserPanelMenuBar::set_straighten_horizon, this);
	browser -> getTexturing ()         .removeInterest (&X3DBrowserPanelMenuBar::set_texturing,          this);
	browser -> getShaders ()           .removeInterest (&X3DBrowserPanelMenuBar::set_shaders,            this);

	browser = value;

	browser -> getBrowserOptions () -> Shading () .addInterest (&X3DBrowserPanelMenuBar::set_shading, this);
	browser -> initialized ()          .addInterest (&X3DBrowserPanelMenuBar::set_initialized,       this);
	browser -> getViewer ()            .addInterest (&X3DBrowserPanelMenuBar::set_viewer,             this);
	browser -> getStraightenHorizon () .addInterest (&X3DBrowserPanelMenuBar::set_straighten_horizon, this);
	browser -> getTexturing ()         .addInterest (&X3DBrowserPanelMenuBar::set_texturing,          this);
	browser -> getShaders ()           .addInterest (&X3DBrowserPanelMenuBar::set_shaders,            this);

	viewpointObserver .reset (new ViewpointObserver (getBrowserWindow (), browser));
	viewpointObserver -> getUndoHistory () -> addInterest (&X3DBrowserPanelMenuBar::set_undoHistory, this);

	set_undoHistory ();
	set_initialized ();
	set_shading (browser -> getBrowserOptions () -> Shading ());
	set_viewer ();
	set_straighten_horizon ();
	set_texturing ();
	set_shaders ();

	if (browser not_eq getPage () -> getMainBrowser ())
	{
		getShowHideEnvironmentalEffectsMenuItem () .set_visible (false);
		getBackgroundsMenuItem ()                  .set_visible (false);
		getFogsMenuItem ()                         .set_visible (false);
	}
}

void
X3DBrowserPanelMenuBar::on_menubar_toggled ()
{
	//getMenuBar () .set_visible (getBrowserWindow () -> getMenubarAction () -> get_active ());
}

void
X3DBrowserPanelMenuBar::set_initialized ()
{
	changing = true;

	const auto shading = getShading (getId ());

	if (shading == "PHONG")
		getPhongMenuItem () .set_active (true);
	else if (shading == "GOURAUD")
		getGouraudMenuItem () .set_active (true);
	else if (shading == "FLAT")
		getFlatMenuItem () .set_active (true);
	else if (shading == "WIREFRAME")
		getWireframeMenuItem () .set_active (true);
	else if (shading == "POINTSET")
		getPointsetMenuItem () .set_active (true);

	changing = false;
}

void
X3DBrowserPanelMenuBar::set_focus (const bool focus)
{
	if (focus)
		getBrowserWindow () -> getWindow () .add_accel_group (getAccelGroup ());
	else
		getBrowserWindow () -> getWindow () .remove_accel_group (getAccelGroup ());
}

void
X3DBrowserPanelMenuBar::set_scene ()
{
	// View Menu

	changing = true;

	getBackgroundsMenuItem () .set_active (true);
	getFogsMenuItem ()        .set_active (true);

	changing = false;
}

void
X3DBrowserPanelMenuBar::on_main_browser_hierarchy_changed (Gtk::Widget* previous_toplevel)
{
	const bool visible = getPage () -> getMainBrowser () -> get_parent ();

	getMainViewMenuItem ()          .set_sensitive (not visible);
	getMainViewSeparatorMenuItem () .set_sensitive (not visible);
}

/*
 *  View menu
 */

void
X3DBrowserPanelMenuBar::set_undoHistory ()
{
	const auto & undoHistory = viewpointObserver -> getUndoHistory () ;

	getUndoViewMenuItem () .set_sensitive (undoHistory -> hasUndo ());
	getRedoViewMenuItem () .set_sensitive (undoHistory -> hasRedo ());
}

void
X3DBrowserPanelMenuBar::undo_view_activate ()
{
	viewpointObserver -> getUndoHistory () -> undo ();
}

void
X3DBrowserPanelMenuBar::redo_view_activate ()
{
	viewpointObserver -> getUndoHistory () -> redo ();
}

void
X3DBrowserPanelMenuBar::set_viewer ()
{
	getStraightenHorizonMenuItem ()          .set_visible (getLocalBrowser () -> getCurrentViewer () == X3D::X3DConstants::ExamineViewer);
	getStraightenHorizonSeparatorMenuItem () .set_visible (getLocalBrowser () -> getCurrentViewer () == X3D::X3DConstants::ExamineViewer);
}

void
X3DBrowserPanelMenuBar::set_straighten_horizon ()
{
	changing = true;

	getStraightenHorizonMenuItem () .set_active (getLocalBrowser () -> getStraightenHorizon ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::on_straighten_horizon_toggled ()
{
	if (changing)
		return;

	getConfig () -> setItem <bool> ("straightenHorizon", getStraightenHorizonMenuItem () .get_active ());
	getLocalBrowser () -> setStraightenHorizon (getStraightenHorizonMenuItem () .get_active ());

	if (getStraightenHorizonMenuItem () .get_active ())
		on_straighten_clicked ();
}

void
X3DBrowserPanelMenuBar::on_straighten_clicked ()
{
	const auto & activeLayer = getLocalBrowser () -> getActiveLayer ();

	if (not activeLayer)
		return;

	activeLayer -> getViewpoint () -> straighten (getLocalBrowser () -> getCurrentViewer () == X3D::X3DConstants::ExamineViewer);
}

void
X3DBrowserPanelMenuBar::on_look_at_selection_activate ()
{
	getLocalBrowser () -> lookAtSelection ();
}

void
X3DBrowserPanelMenuBar::on_look_at_all_activate ()
{
	getLocalBrowser () -> lookAtAllObjectsInActiveLayer ();
}

void
X3DBrowserPanelMenuBar::on_reset_user_offsets_activate ()
{
	const auto & activeLayer = getLocalBrowser () -> getActiveLayer ();

	if (not activeLayer)
		return;

	const auto viewpoint = activeLayer -> getViewpoint ();

	viewpoint -> setAnimate (true); // VRML
	viewpoint -> transitionStart (viewpoint);
}

/*
 *  Shading menu
 */

void
X3DBrowserPanelMenuBar::on_phong_toggled ()
{
	if (getPhongMenuItem () .get_active ())
		on_shading_changed ("PHONG");
}

void
X3DBrowserPanelMenuBar::on_gouraud_toggled ()
{
	if (getGouraudMenuItem () .get_active ())
		on_shading_changed ("GOURAUD");
}

void
X3DBrowserPanelMenuBar::on_flat_toggled ()
{
	if (getFlatMenuItem () .get_active ())
		on_shading_changed ("FLAT");
}

void
X3DBrowserPanelMenuBar::on_wireframe_toggled ()
{
	if (getWireframeMenuItem () .get_active ())
		on_shading_changed ("WIREFRAME");
}

void
X3DBrowserPanelMenuBar::on_pointset_toggled ()
{
	if (getPointsetMenuItem () .get_active ())
		on_shading_changed ("POINTSET");
}

void
X3DBrowserPanelMenuBar::on_shading_changed (const std::string & value)
{
	if (changing)
		return;

	setShading (getId (), value);

	getLocalBrowser () -> getBrowserOptions () -> Shading () = value;
}

void
X3DBrowserPanelMenuBar::set_shading (const X3D::SFString & value)
{
	const auto shading = getShading (getId ());

	if (value != shading)
		getLocalBrowser () -> getBrowserOptions () -> Shading () = shading;
}

void
X3DBrowserPanelMenuBar::set_texturing ()
{
	changing = true;

	getTexturesMenuItem () .set_active (getLocalBrowser () -> getTexturing ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::on_textures_toggled ()
{
	if (changing)
		return;

	getLocalBrowser () -> setTexturing (getTexturesMenuItem () .get_active ());
}

void
X3DBrowserPanelMenuBar::set_shaders ()
{
	changing = true;

	getShadersMenuItem () .set_active (getLocalBrowser () -> getShaders ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::on_shaders_toggled ()
{
	if (changing)
		return;

	getLocalBrowser () -> setShaders (getShadersMenuItem () .get_active ());
}

/*
 *  Show menu
 */

void
X3DBrowserPanelMenuBar::set_lightTools (const X3D::X3DWeakPtrArray <X3D::X3DLightNodeTool> & tools)
{
	changing = tools .empty ();

	getLightsMenuItem () .set_active (tools .size ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::set_proximitySensorTools (const X3D::X3DWeakPtrArray <X3D::ProximitySensorTool> & tools)
{
	changing = tools .empty ();

	getProximitySensorsMenuItem () .set_active (tools .size ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::set_soundTools (const X3D::X3DWeakPtrArray <X3D::SoundTool> & tools)
{
	changing = tools .empty ();

	getSoundsMenuItem () .set_active (tools .size ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::set_transformSensorTools (const X3D::X3DWeakPtrArray <X3D::TransformSensorTool> & tools)
{
	changing = tools .empty ();

	getTransformSensorsMenuItem () .set_active (tools .size ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::set_visibilitySensorTools (const X3D::X3DWeakPtrArray <X3D::VisibilitySensorTool> & tools)
{
	changing = tools .empty ();

	getVisibilitySensorsMenuItem () .set_active (tools .size ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::set_viewpointTools (const X3D::X3DWeakPtrArray <X3D::X3DViewpointNodeTool> & tools)
{
	changing = tools .empty ();

	getViewpointsMenuItem () .set_active (tools .size ());

	changing = false;
}

// Object Icons

void
X3DBrowserPanelMenuBar::on_backgrounds_toggled ()
{
	if (changing)
		return;

	const bool hidden = not getBackgroundsMenuItem () .get_active ();

	X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [&hidden] (X3D::SFNode & node)
	{
		const auto background = dynamic_cast <X3D::X3DBackgroundNode*> (node .getValue ());
		
		if (background)
			background -> isHidden (hidden);
		
		return true;
	});
}

void
X3DBrowserPanelMenuBar::on_fogs_toggled ()
{
	if (changing)
		return;

	const bool hidden = not getFogsMenuItem () .get_active ();

	X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [&hidden] (X3D::SFNode & node)
	{
		const auto fog = dynamic_cast <X3D::X3DFogObject*> (node .getValue ());
		
		if (fog)
			fog -> isHidden (hidden);
		
		return true;
	});
}

// Object Icons

void
X3DBrowserPanelMenuBar::on_lights_toggled ()
{
	if (changing)
		return;

	if (getLightsMenuItem () .get_active ())
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::X3DLightNode*> (node .getValue ()))
				node -> addTool ();
			
			return true;
		});
	}
	else
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::X3DLightNode*> (node .getValue ()))
				node -> removeTool (true);
			
			return true;
		});
	}
}

void
X3DBrowserPanelMenuBar::on_proximity_sensors_toggled ()
{
	static const std::set <X3D::X3DConstants::NodeType> proximitySensors = {
		X3D::X3DConstants::ProximitySensor,
		X3D::X3DConstants::GeoProximitySensor,
		X3D::X3DConstants::ViewpointGroup
	};

	if (changing)
		return;

	if (getProximitySensorsMenuItem () .get_active ())
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		{
			if (node -> isType (proximitySensors))
				node -> addTool ();
			
			return true;
		});
	}
	else
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		{
			if (node -> isType (proximitySensors))
				node -> removeTool (true);
			
			return true;
		});
	}
}

void
X3DBrowserPanelMenuBar::on_sounds_toggled ()
{
	if (changing)
		return;

	if (getSoundsMenuItem () .get_active ())
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::Sound*> (node .getValue ()))
				node -> addTool ();
			
			return true;
		});
	}
	else
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::Sound*> (node .getValue ()))
				node -> removeTool (true);
			
			return true;
		});
	}
}

void
X3DBrowserPanelMenuBar::on_transform_sensors_toggled ()
{
	if (changing)
		return;

	if (getTransformSensorsMenuItem () .get_active ())
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::TransformSensor*> (node .getValue ()))
				node -> addTool ();
			
			return true;
		});
	}
	else
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::TransformSensor*> (node .getValue ()))
				node -> removeTool (true);
			
			return true;
		});
	}
}

void
X3DBrowserPanelMenuBar::on_visibility_sensors_toggled ()
{
	if (changing)
		return;

	if (getVisibilitySensorsMenuItem () .get_active ())
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::VisibilitySensor*> (node .getValue ()))
				node -> addTool ();
			
			return true;
		});
	}
	else
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::VisibilitySensor*> (node .getValue ()))
				node -> removeTool (true);
			
			return true;
		});
	}
}

void
X3DBrowserPanelMenuBar::on_viewpoints_toggled ()
{
	if (changing)
		return;

	if (getViewpointsMenuItem () .get_active ())
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::X3DViewpointNode*> (node .getValue ()))
				node -> addTool ();
			
			return true;
		});
	}
	else
	{
		X3D::traverse (getPage () -> getMainBrowser () -> getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		{
			if (dynamic_cast <X3D::X3DViewpointNode*> (node .getValue ()))
				node -> removeTool (true);
			
			return true;
		});
	}
}

void
X3DBrowserPanelMenuBar::on_hide_all_iconic_objects_activated ()
{
	if (getLightsMenuItem () .get_active ())
		getLightsMenuItem () .set_active (false);

	if (getProximitySensorsMenuItem () .get_active ())
		getProximitySensorsMenuItem () .set_active (false);

	if (getSoundsMenuItem () .get_active ())
		getSoundsMenuItem () .set_active (false);

	if (getTransformSensorsMenuItem () .get_active ())
		getTransformSensorsMenuItem () .set_active (false);

	if (getVisibilitySensorsMenuItem () .get_active ())
		getVisibilitySensorsMenuItem () .set_active (false);

	if (getViewpointsMenuItem () .get_active ())
		getViewpointsMenuItem () .set_active (false);
}

/*
 *  Layers menu
 */

void
X3DBrowserPanelMenuBar::on_layers_activate ()
{
	// Remove all menu items.

	for (auto & widget : getLayersMenu () .get_children ())
	   getLayersMenu () .remove (*widget);
	
	// Rebuild menu.

	const auto & layerSet = getPage () -> getMainBrowser () -> getWorld () -> getLayerSet ();

	on_layers_activate (layerSet -> getLayer0 (), 0);

	size_t layerNumber = 1;

	for (const auto & node : layerSet -> layers ())
	{
		const X3D::X3DPtr <X3D::X3DLayerNode> layerNode (node);

		if (layerNode)
			on_layers_activate (layerNode, layerNumber);

	   ++ layerNumber;
	}

	getLayersMenu () .show_all ();
}

void
X3DBrowserPanelMenuBar::on_layers_activate (const X3D::X3DPtr <X3D::X3DLayerNode> & layer, const size_t layerNumber)
{
	const auto & layerSet = getPage () -> getMainBrowser () -> getWorld () -> getLayerSet ();
	const auto   menuItem = Gtk::manage (new Gtk::MenuItem ());
	auto         name     = std::string ();

	if (layer == layerSet -> getLayer0 ())
		name = _ ("Default Layer");
	else if (layer -> getName () .empty ())
		name = std::to_string (layerNumber);
	else
		name = X3D::GetDisplayName (layer);

	menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &X3DBrowserPanelMenuBar::on_layer_activate), layerNumber));
	menuItem -> set_label (layer -> getTypeName () + " »" +  name  + "«");

	if (layer == getLayer ())
	   menuItem -> get_style_context () -> add_class ("titania-menu-item-selected");

	getLayersMenu () .append (*menuItem);
}

void
X3DBrowserPanelMenuBar::on_layer_activate (const size_t layerNumber)
{
	const auto & layerSet = getPage () -> getMainBrowser () -> getWorld () -> getLayerSet ();

	layerSet -> privateActiveLayer () = layerNumber;
}

/*
 *  Camera menu
 */

void
X3DBrowserPanelMenuBar::on_main_view_activate ()
{
	setType (BrowserPanelType::MAIN_VIEW);
}

void
X3DBrowserPanelMenuBar::on_perspective_view_activate ()
{
	setType (BrowserPanelType::PERSPECTIVE_VIEW);
}

void
X3DBrowserPanelMenuBar::on_top_view_activate ()
{
	setType (BrowserPanelType::TOP_VIEW);
}

void
X3DBrowserPanelMenuBar::on_right_view_activate ()
{
	setType (BrowserPanelType::RIGHT_VIEW);
}

void
X3DBrowserPanelMenuBar::on_front_view_activate ()
{
	setType (BrowserPanelType::FRONT_VIEW);
}

void
X3DBrowserPanelMenuBar::on_bottom_view_activate ()
{
	setType (BrowserPanelType::BOTTOM_VIEW);
}

void
X3DBrowserPanelMenuBar::on_left_view_activate ()
{
	setType (BrowserPanelType::LEFT_VIEW);
}

void
X3DBrowserPanelMenuBar::on_back_view_activate ()
{
	setType (BrowserPanelType::BACK_VIEW);
}

/*
 *  Destruction menu
 */

void
X3DBrowserPanelMenuBar::dispose ()
{
	viewpointObserver .reset ();
}

X3DBrowserPanelMenuBar::~X3DBrowserPanelMenuBar ()
{ }

} // puck
} // titania
