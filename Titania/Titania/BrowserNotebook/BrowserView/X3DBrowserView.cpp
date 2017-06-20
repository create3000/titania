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

#include "X3DBrowserView.h"

#include "../NotebookPage/NotebookPage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Editors/GridEditor/AngleTool.h"
#include "../../Editors/GridEditor/AxonometricGridTool.h"
#include "../../Editors/GridEditor/GridTool.h"

#include <Titania/X3D/Browser/Navigation/PlaneViewer.h>
#include <Titania/X3D/Browser/Selection.h>
#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>
#include <Titania/X3D/Execution/BindableNodeStack.h>

#include <Titania/X3D/Tools/Grids/GridTool.h>
#include <Titania/X3D/Tools/Grids/AngleTool.h>
#include <Titania/X3D/Tools/Grids/AxonometricGridTool.h>

#include <cassert>

namespace titania {
namespace puck {

const std::vector <std::string> X3DBrowserView::names = { "", "Top", "Right", "Front", "Bottom", "Left", "Back" };

const std::vector <X3D::Vector3d> X3DBrowserView::axes = {
	X3D::Vector3f (),
	X3D::Vector3f (0, 1, 0),
	X3D::Vector3f (1, 0, 0),
	X3D::Vector3f (0, 0, 1),
	X3D::Vector3f (0, -1, 0),
	X3D::Vector3f (-1, 0, 0),
	X3D::Vector3f (0, 0, -1)
};

const std::vector <X3D::Vector3d> X3DBrowserView::positions = {
	X3D::Vector3f (),
	X3D::Vector3f (0, 10, 0),
	X3D::Vector3f (10, 0, 0),
	X3D::Vector3f (0, 0, 10),
	X3D::Vector3f (0, -10, 0),
	X3D::Vector3f (-10, 0, 0),
	X3D::Vector3f (0, 0, -10)
};

const std::vector <X3D::Rotation4d> X3DBrowserView::orientations = {
	X3D::Rotation4f (),
	X3D::Rotation4f (1, 0, 0, -math::pi <float> / 2),
	X3D::Rotation4f (0, 1, 0, math::pi <float> / 2),
	X3D::Rotation4f (),
	X3D::Rotation4f (1, 0, 0, math::pi <float> / 2) * X3D::Rotation4f (0, 1, 0, math::pi <float>),
	X3D::Rotation4f (0, 1, 0, -math::pi <float> / 2),
	X3D::Rotation4f (0, 1, 0, math::pi <float>)
};

X3DBrowserView::X3DBrowserView (NotebookPage* const page, const BrowserViewType type) :
	X3DBrowserViewInterface (),
	                   page (page),
	                   type (type),
	                browser (page -> getMainBrowser ()),
	            activeLayer (),
	              viewpoint (),
	          gridTransform (),
	             gridSwitch (),
	                   grid ()
{
	assert (page);

	addChildObjects (browser,
	                 activeLayer,
	                 viewpoint,
	                 gridTransform,
	                 gridSwitch,
	                 grid);

}

X3DBrowserView::X3DBrowserView () :
	X3DBrowserView (nullptr, BrowserViewType::MAIN)
{ }

void
X3DBrowserView::initialize ()
{
	X3DBrowserViewInterface::initialize ();

	__LOG__ << getConfig () -> get <int32_t> ("type", type) << std::endl;

	setType (BrowserViewType (math::clamp <int32_t> (getConfig () -> get <int32_t> ("type", type), BrowserViewType::MAIN, BrowserViewType::BACK)));
}

X3D::BrowserPtr
X3DBrowserView::createBrowser (const BrowserViewType type) const
{
	if (type == BrowserViewType::MAIN)
		return page -> getMainBrowser ();

	return X3D::createBrowser (page -> getMainBrowser (), { get_ui ("BrowserView.x3dv") });
}

void
X3DBrowserView::setType (const BrowserViewType value)
{
	__LOG__ << int (value) << std::endl;

	type = value;

	getConfig () -> set <int32_t> ("type", type);

	switch (type)
	{
		case BrowserViewType::MAIN:
			getDescriptionMenuItem () .set_label (_ ("Main View"));  
			break;
		case BrowserViewType::TOP:
			getDescriptionMenuItem () .set_label (_ ("Top View"));  
			break;
		case BrowserViewType::BOTTOM:
			getDescriptionMenuItem () .set_label (_ ("Bottom View"));  
			break;
		case BrowserViewType::LEFT:
			getDescriptionMenuItem () .set_label(_ ("Left View"));  
			break;
		case BrowserViewType::RIGHT:
			getDescriptionMenuItem () .set_label(_ ("Right View"));  
			break;
		case BrowserViewType::FRONT:
			getDescriptionMenuItem () .set_label (_ ("Front View"));  
			break;
		case BrowserViewType::BACK:
			getDescriptionMenuItem () .set_label (_ ("Back View"));  
			break;
	}

	setLocalBrowser (createBrowser (type));
}

BrowserViewType
X3DBrowserView::getType () const
{
	return type;
}

void
X3DBrowserView::setLocalBrowser (const X3D::BrowserPtr & value)
{
	if (browser)
		getBrowserBox () .remove (*browser);

	browser = value;

	if (type not_eq BrowserViewType::MAIN)
	{
		browser -> setName (names [type]);
		browser -> initialized () .addInterest (&X3DBrowserView::set_dependent_browser, this);
		browser -> set_opacity (0);
	}

	if (getWidget () .get_mapped ())
		X3DBrowserView::on_map ();

	browser -> signal_focus_out_event () .connect (sigc::mem_fun ((X3DViewInterface*) this, &X3DBrowserView::on_focus_out_event));
	browser -> signal_focus_in_event ()  .connect (sigc::mem_fun ((X3DViewInterface*) this, &X3DBrowserView::on_focus_in_event));
	browser -> setAntialiasing (4);
	browser -> show ();

	getBrowserBox () .pack_start (*browser, true, true, 0);
}

int32_t
X3DBrowserView::getPlane () const
{
	switch (type)
	{
		case BrowserViewType::MAIN:
			return -1;
		case BrowserViewType::TOP:
		case BrowserViewType::BOTTOM:
			return 1;
		case BrowserViewType::RIGHT:
		case BrowserViewType::LEFT:
			return 0;
		case BrowserViewType::FRONT:
		case BrowserViewType::BACK:
			return 2;
	}

	return -1;
}

void
X3DBrowserView::set_dependent_browser ()
{
	try
	{
		// Setup dependent browser.

		browser -> initialized () .removeInterest (&X3DBrowserView::set_dependent_browser, this);	
		browser -> setSelection (page -> getMainBrowser () -> getSelection ());
		browser -> setFrameRate (30);
		browser -> set_opacity (1);

		// Setup scene.

		const auto   worldInfo   = createWorldInfo (page -> getScene ());
		const auto & activeLayer = browser -> getWorld () -> getLayerSet () -> getActiveLayer ();

		const auto & gridTool            = getBrowserWindow () -> getGridTool ()            -> getTool ();
		const auto & angleGridTool       = getBrowserWindow () -> getAngleTool ()           -> getTool ();
		const auto & axonometricGridTool = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();

		getBrowserWindow () -> getGridTool ()            -> getTool () -> addInterest (&X3DBrowserView::set_grid, this);
		getBrowserWindow () -> getAngleTool ()           -> getTool () -> addInterest (&X3DBrowserView::set_grid, this);
		getBrowserWindow () -> getAxonometricGridTool () -> getTool () -> addInterest (&X3DBrowserView::set_grid, this);

		viewpoint     = browser -> getExecutionContext () -> getNamedNode <X3D::OrthoViewpoint> ("OrthoViewpoint");
		gridTransform = browser -> getExecutionContext () -> getNamedNode <X3D::Transform> ("GridTransform");
		gridSwitch    = browser -> getExecutionContext () -> getNamedNode <X3D::Switch> ("GridSwitch");
		grid          = browser -> getExecutionContext () -> getNamedNode ("Grid");

		activeLayer -> getNavigationInfoStack () -> setLock (true);
		activeLayer -> getViewpointStack ()      -> setLock (true);
		activeLayer -> getBackgroundStack ()     -> setLock (true);
		activeLayer -> getFogStack ()            -> setLock (true);

		gridSwitch -> children () .emplace_back (gridTool            -> getTool ());
		gridSwitch -> children () .emplace_back (angleGridTool       -> getTool ());
		gridSwitch -> children () .emplace_back (axonometricGridTool -> getTool ());

		viewpoint -> position ()               .addInterest (&X3DBrowserView::set_viewpoint, this);
		viewpoint -> positionOffset ()         .addInterest (&X3DBrowserView::set_viewpoint, this);
		viewpoint -> orientation ()            .addInterest (&X3DBrowserView::set_viewpoint, this);
		viewpoint -> orientationOffset ()      .addInterest (&X3DBrowserView::set_viewpoint, this);
		viewpoint -> centerOfRotation ()       .addInterest (&X3DBrowserView::set_viewpoint, this);
		viewpoint -> centerOfRotationOffset () .addInterest (&X3DBrowserView::set_viewpoint, this);
		viewpoint -> fieldOfViewScale ()       .addInterest (&X3DBrowserView::set_viewpoint, this);

		viewpoint -> position ()         .set (worldInfo -> getMetaData ("/Titania/" + names [type] + "Viewpoint/position", positions [type]));
		viewpoint -> orientation ()      .set (worldInfo -> getMetaData ("/Titania/" + names [type] + "Viewpoint/orientation", orientations [type]));
		viewpoint -> centerOfRotation () .set (worldInfo -> getMetaData ("/Titania/" + names [type] + "Viewpoint/centerOfRotation", X3D::Vector3d ()));
		viewpoint -> fieldOfViewScale () .set (worldInfo -> getMetaData ("/Titania/" + names [type] + "Viewpoint/fieldOfViewScale", 1.0));

		grid -> setField <X3D::SFRotation> ("rotation", X3D::Rotation4d (1, 0, 0, math::pi <double> / 2) * orientations [type]);
	
		// Connect to active layer.
	
		page -> getMainBrowser () -> getActiveLayer () .addInterest (&X3DBrowserView::set_activeLayer, this);
	
		set_activeLayer ();
		set_grid ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserView::set_activeLayer ()
{
	try
	{
		if (activeLayer)
		{
			const auto group = browser -> getExecutionContext () -> getNamedNode <X3D::Group> ("Group");

			activeLayer -> children () .removeInterest (group -> children ());
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	activeLayer = page -> getMainBrowser () -> getActiveLayer ();

	try
	{
		const auto group = browser -> getExecutionContext () -> getNamedNode <X3D::Group> ("Group");

		group -> setPrivate (true);

		if (activeLayer)
		{
			activeLayer -> children () .addInterest (group -> children ());
			group -> children () = activeLayer -> children ();
		}
		else
			group -> children () .clear ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserView::set_viewpoint ()
{
	const auto worldInfo = createWorldInfo (page -> getScene ());

	worldInfo -> setMetaData ("/Titania/" + names [type] + "Viewpoint/position",         viewpoint -> getUserPosition ());
	worldInfo -> setMetaData ("/Titania/" + names [type] + "Viewpoint/orientation",      viewpoint -> getUserOrientation ());
	worldInfo -> setMetaData ("/Titania/" + names [type] + "Viewpoint/centerOfRotation", viewpoint -> getUserCenterOfRotation ());
	worldInfo -> setMetaData ("/Titania/" + names [type] + "Viewpoint/fieldOfViewScale", viewpoint -> fieldOfViewScale ());

	page -> setModified (true);

	gridTransform -> translation () = viewpoint -> getUserPosition () * axes [type] - X3D::Vector3d (0, 0, 10) * viewpoint -> getUserOrientation ();
}

void
X3DBrowserView::set_grid ()
{
	// <plane, type> = <x, y>
	static const std::map <std::pair <int32_t, BrowserViewType>, X3D::Vector2i> mappings = {
		// x-plane
		std::make_pair (std::make_pair (0, BrowserViewType::TOP),   X3D::Vector2i (1, 0)),
		std::make_pair (std::make_pair (0, BrowserViewType::RIGHT), X3D::Vector2i (0, 2)),
		std::make_pair (std::make_pair (0, BrowserViewType::FRONT), X3D::Vector2i (1, 2)),
		// y-plane
		std::make_pair (std::make_pair (1, BrowserViewType::TOP),   X3D::Vector2i (0, 2)),
		std::make_pair (std::make_pair (1, BrowserViewType::RIGHT), X3D::Vector2i (2, 1)),
		std::make_pair (std::make_pair (1, BrowserViewType::FRONT), X3D::Vector2i (0, 1)),
		// z-plane
		std::make_pair (std::make_pair (2, BrowserViewType::TOP),   X3D::Vector2i (0, 1)),
		std::make_pair (std::make_pair (2, BrowserViewType::RIGHT), X3D::Vector2i (1, 2)),
		std::make_pair (std::make_pair (2, BrowserViewType::FRONT), X3D::Vector2i (0, 2)),
		// x-plane
		std::make_pair (std::make_pair (0, BrowserViewType::BOTTOM), X3D::Vector2i (1, 0)),
		std::make_pair (std::make_pair (0, BrowserViewType::LEFT),   X3D::Vector2i (0, 2)),
		std::make_pair (std::make_pair (0, BrowserViewType::BACK),   X3D::Vector2i (1, 2)),
		// y-plane
		std::make_pair (std::make_pair (1, BrowserViewType::BOTTOM), X3D::Vector2i (0, 2)),
		std::make_pair (std::make_pair (1, BrowserViewType::LEFT),   X3D::Vector2i (2, 1)),
		std::make_pair (std::make_pair (1, BrowserViewType::BACK),   X3D::Vector2i (0, 1)),
		// z-plane
		std::make_pair (std::make_pair (2, BrowserViewType::BOTTOM), X3D::Vector2i (0, 1)),
		std::make_pair (std::make_pair (2, BrowserViewType::LEFT),   X3D::Vector2i (1, 2)),
		std::make_pair (std::make_pair (2, BrowserViewType::BACK),   X3D::Vector2i (0, 2)),
	};

	if (page -> getMainBrowser () -> getExecutionContext () not_eq getCurrentContext ())
		return;

	try
	{
		if (getBrowserWindow () -> getGridTool () -> getEnabled ())
		{
			const auto plane = getBrowserWindow () -> getGridTool () -> getPlane ();
	
			if (plane == getPlane ())
			{
				gridSwitch -> whichChoice () = 1;
			}
			else
			{
				const auto & tool    = getBrowserWindow () -> getGridTool () -> getTool ();
				const auto   mapping = mappings .at (std::make_pair (plane, type));
				const auto   one     = X3D::Vector3f (1, 1, 1) - X3D::Vector3f (max (axes [type], -axes [type]));

				gridSwitch -> whichChoice () = 0;

				grid -> setField <X3D::SFVec3f> ("translation", tool -> translation () * one);
				grid -> setField <X3D::SFVec3f> ("scale",       X3D::Vector3f (one .x () ? tool -> scale () .getX () : 1, one .y () ? tool -> scale () .getY () : 1, one .z () ? tool -> scale () .getZ () : 1));
				grid -> setField <X3D::MFInt32> ("dimension",   X3D::MFInt32 ({ tool -> dimension () .at (mapping .x ()), 0, tool -> dimension () .at (mapping .y ()) }));
	
				for (size_t i = 0; i < tool -> majorLineEvery () .size (); i += 3)
				{
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 0, tool -> majorLineEvery () .at (i + mapping .x ()));
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 2, tool -> majorLineEvery () .at (i + mapping .y ()));
				}
	
				for (size_t i = 0; i < tool -> majorLineOffset () .size (); i += 3)
				{
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 0, tool -> majorLineOffset () .at (i + mapping .x ()));
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 2, tool -> majorLineOffset () .at (i + mapping .y ()));
				}
	
				grid -> setField <X3D::SFColor> ("color",          X3D::Color3f (tool -> color ()          .getRed (), tool -> color ()          .getGreen (), tool -> color ()          .getBlue ()));
				grid -> setField <X3D::SFColor> ("lineColor",      X3D::Color3f (tool -> lineColor ()      .getRed (), tool -> lineColor ()      .getGreen (), tool -> lineColor ()      .getBlue ()));
				grid -> setField <X3D::SFColor> ("majorLineColor", X3D::Color3f (tool -> majorLineColor () .getRed (), tool -> majorLineColor () .getGreen (), tool -> majorLineColor () .getBlue ()));
	
				grid -> setField <X3D::SFFloat> ("transparency",          1 - tool -> color ()          .getAlpha ());
				grid -> setField <X3D::SFFloat> ("lineTransparency",      1 - tool -> lineColor ()      .getAlpha ());
				grid -> setField <X3D::SFFloat> ("majorLineTransparency", 1 - tool -> majorLineColor () .getAlpha ());
			}
		}
		else if (getBrowserWindow () -> getAngleTool () -> getEnabled ())
		{
			const auto plane = getBrowserWindow () -> getAngleTool () -> getPlane ();

			if (plane == getPlane ())
			{
				gridSwitch -> whichChoice () = 2;
			}
			else
			{
				const auto & tool = getBrowserWindow () -> getAngleTool () -> getTool ();
				const auto   one  = X3D::Vector3f (1, 1, 1) - X3D::Vector3f (max (axes [type], -axes [type]));

				gridSwitch -> whichChoice () = 0;
	
				grid -> setField <X3D::SFVec3f> ("translation", tool -> translation () * one);
				grid -> setField <X3D::SFVec3f> ("scale",       X3D::Vector3f (one .x () ? tool -> scale () .getX () : 1, one .y () ? tool -> scale () .getY () : 1, one .z () ? tool -> scale () .getZ () : 1));
				grid -> setField <X3D::MFInt32> ("dimension",   X3D::MFInt32 ({ 2, 0, tool -> dimension () .at (2) }));
	
				for (size_t i = 0; i < tool -> majorLineEvery () .size (); i += 3)
				{
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 0, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 2, tool -> majorLineEvery () .at (i + 2));
				}
	
				for (size_t i = 0; i < tool -> majorLineOffset () .size (); i += 3)
				{
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 0, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 2, tool -> majorLineOffset () .at (i + 2));
				}
	
				grid -> setField <X3D::SFColor> ("color",          X3D::Color3f (tool -> color ()          .getRed (), tool -> color ()          .getGreen (), tool -> color ()          .getBlue ()));
				grid -> setField <X3D::SFColor> ("lineColor",      X3D::Color3f (tool -> lineColor ()      .getRed (), tool -> lineColor ()      .getGreen (), tool -> lineColor ()      .getBlue ()));
				grid -> setField <X3D::SFColor> ("majorLineColor", X3D::Color3f (tool -> majorLineColor () .getRed (), tool -> majorLineColor () .getGreen (), tool -> majorLineColor () .getBlue ()));
	
				grid -> setField <X3D::SFFloat> ("transparency",          1 - tool -> color ()          .getAlpha ());
				grid -> setField <X3D::SFFloat> ("lineTransparency",      1 - tool -> lineColor ()      .getAlpha ());
				grid -> setField <X3D::SFFloat> ("majorLineTransparency", 1 - tool -> majorLineColor () .getAlpha ());
			}
		}
		else if (getBrowserWindow () -> getAxonometricGridTool () -> getEnabled ())
		{
			const auto plane = getBrowserWindow () -> getAxonometricGridTool () -> getPlane ();

			if (plane == getPlane ())
			{
				gridSwitch -> whichChoice () = 3;
			}
			else
			{
				const auto & tool = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
				const auto   one  = X3D::Vector3f (1, 1, 1) - X3D::Vector3f (max (axes [type], -axes [type]));

				gridSwitch -> whichChoice () = 0;
	
				grid -> setField <X3D::SFVec3f> ("translation", tool -> translation () * one);
				grid -> setField <X3D::SFVec3f> ("scale",       X3D::Vector3f (one .x () ? tool -> scale () .getX () : 1, one .y () ? tool -> scale () .getY () : 1, one .z () ? tool -> scale () .getZ () : 1));
				grid -> setField <X3D::MFInt32> ("dimension",   X3D::MFInt32 ({ 2, 0, tool -> dimension () .at (1) }));
	
				for (size_t i = 0; i < tool -> majorLineEvery () .size () / 4; ++ i)
				{
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i * 3 + 0, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i * 3 + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i * 3 + 2, tool -> majorLineEvery () .at (i * 4 + 3));
				}
	
				for (size_t i = 0; i < tool -> majorLineOffset () .size () / 4; ++ i)
				{
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i * 3 + 0, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i * 3 + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i * 3 + 2, tool -> majorLineOffset () .at (i * 4 + 3));
				}
	
				grid -> setField <X3D::SFColor> ("color",          X3D::Color3f (tool -> color ()          .getRed (), tool -> color ()          .getGreen (), tool -> color ()          .getBlue ()));
				grid -> setField <X3D::SFColor> ("lineColor",      X3D::Color3f (tool -> lineColor ()      .getRed (), tool -> lineColor ()      .getGreen (), tool -> lineColor ()      .getBlue ()));
				grid -> setField <X3D::SFColor> ("majorLineColor", X3D::Color3f (tool -> majorLineColor () .getRed (), tool -> majorLineColor () .getGreen (), tool -> majorLineColor () .getBlue ()));
	
				grid -> setField <X3D::SFFloat> ("transparency",          1 - tool -> color ()          .getAlpha ());
				grid -> setField <X3D::SFFloat> ("lineTransparency",      1 - tool -> lineColor ()      .getAlpha ());
				grid -> setField <X3D::SFFloat> ("majorLineTransparency", 1 - tool -> majorLineColor () .getAlpha ());
			}
		}
		else
		{
			gridSwitch -> whichChoice () = -1;
		}
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;

		gridSwitch -> whichChoice () = -1;
	}
}

void
X3DBrowserView::on_map ()
{
	if (type == BrowserViewType::MAIN)
		return;

	page -> getMainBrowser () -> changed () .addInterest (&X3D::Browser::addEvent, browser .getValue ());
}

void
X3DBrowserView::on_unmap ()
{
	if (type == BrowserViewType::MAIN)
		return;

	page -> getMainBrowser () -> changed () .removeInterest (&X3D::Browser::addEvent, browser .getValue ());
}

void
X3DBrowserView::dispose ()
{
	X3DBrowserViewInterface::dispose ();
}

X3DBrowserView::~X3DBrowserView ()
{ }

} // puck
} // titania
