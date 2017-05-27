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

#include "BrowserView.h"

#include "../NotebookPage/NotebookPage.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Browser/Navigation/PlaneViewer.h>
#include <Titania/X3D/Browser/Selection.h>
#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>
#include <Titania/X3D/Execution/BindableNodeStack.h>

namespace titania {
namespace puck {

BrowserView::BrowserView (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const BrowserViewType type) :
	       X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DBrowserViewInterface (get_ui ("Views/BrowserView.glade")),
	                   page (page),
	                   type (type),
	                browser (createBrowser (type)),
	            activeLayer (),
	              positions ({ X3D::Vector3f (), X3D::Vector3f (0, 10, 0), X3D::Vector3f (10, 0, 0), X3D::Vector3f (0, 0, 10) }),
	           orientations ({ X3D::Rotation4f (), X3D::Rotation4f (1, 0, 0, -math::pi <float> / 2), X3D::Rotation4f (0, 1, 0, math::pi <float> / 2), X3D::Rotation4f () })
{
	if (type not_eq BrowserViewType::MAIN)
		browser -> initialized () .addInterest (&BrowserView::set_dependent_browser, this);

	browser -> signal_focus_out_event () .connect (sigc::mem_fun (this, &BrowserView::on_focus_out_event));
	browser -> signal_focus_in_event ()  .connect (sigc::mem_fun (this, &BrowserView::on_focus_in_event));
	browser -> setAntialiasing (4);
	browser -> show ();

	getBrowserBox () .pack_start (*browser, true, true, 0);

	setup ();
}

X3D::BrowserPtr
BrowserView::createBrowser (const BrowserViewType type) const
{
	if (type == BrowserViewType::MAIN)
		return page -> getMainBrowser ();

	return X3D::createBrowser (getBrowserWindow () -> getMasterBrowser (), { get_ui ("BrowserView.x3dv") });
}

void
BrowserView::set_dependent_browser ()
{
	try
	{
		browser -> initialized () .removeInterest (&BrowserView::set_dependent_browser, this);	

		// Setup dependent browser.

		page -> getMainBrowser () -> changed () .addInterest (&X3D::Browser::addEvent, browser .getValue ());
		browser -> setSelection (page -> getMainBrowser () -> getSelection ());
		browser -> signal_draw () .connect (sigc::mem_fun (this, &BrowserView::on_draw));

		// Setup scene.

		const auto & activeLayer = browser -> getWorld () -> getLayerSet () -> getActiveLayer ();
		const auto   viewpoint   = browser -> getExecutionContext () -> getNamedNode <X3D::OrthoViewpoint> ("OrthoViewpoint");
		const auto   grid        = browser -> getExecutionContext () -> getNamedNode ("Grid");

		activeLayer -> getNavigationInfoStack () -> setLock (true);
		activeLayer -> getViewpointStack ()      -> setLock (true);
		activeLayer -> getBackgroundStack ()     -> setLock (true);
		activeLayer -> getFogStack ()            -> setLock (true);

		viewpoint -> position ()    = positions [type];
		viewpoint -> orientation () = orientations [type];

		grid -> setField <X3D::SFRotation> ("rotation", X3D::Rotation4d (1, 0, 0, math::pi <double> / 2) * orientations [type]);
	
		// Connect to active layer.
	
		page -> getMainBrowser () -> getActiveLayer () .addInterest (&BrowserView::set_activeLayer, this);
	
		set_activeLayer ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
BrowserView::set_activeLayer ()
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

bool
BrowserView::on_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	cairo -> set_font_size (10);
	cairo -> move_to (10, 20);
	cairo -> set_source_rgb (0, 0, 0); 

	cairo -> select_font_face ("sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);

	switch (type)
	{
		case BrowserViewType::MAIN:
			cairo -> show_text ("Perspective View");  
			break;
		case BrowserViewType::TOP:
			cairo -> show_text ("Top View");  
			break;
		case BrowserViewType::RIGHT:
			cairo -> show_text ("Right View");  
			break;
		case BrowserViewType::FRONT:
			cairo -> show_text ("Front View");  
			break;
	}

	return false;
}

BrowserView::~BrowserView ()
{
	dispose ();
}

} // puck
} // titania
