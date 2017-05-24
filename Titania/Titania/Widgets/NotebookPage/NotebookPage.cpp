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

#include "NotebookPage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>

namespace titania {
namespace puck {

NotebookPage::NotebookPage (X3DBrowserWindow* const browserWindow, const basic::uri & startUrl) :
	        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DNotebookPageInterface (get_ui ("Widgets/NotebookPage.glade")),
	             mainBrowser (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser ())),
	              topBrowser (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser (), { get_ui ("Camera.x3dv") })),
	            rightBrowser (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser (), { get_ui ("Camera.x3dv") })),
	            frontBrowser (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser (), { get_ui ("Camera.x3dv") })),
	                browsers ({ topBrowser, mainBrowser, rightBrowser,  frontBrowser }),
	             activeLayer (),
	             initialized (0),
	                     url (startUrl),
	          browserHistory (mainBrowser),
	             undoHistory (),
	                modified (false),
	           saveConfirmed (false),
	            fileMonitors (),
	                 widgets ({ &getBox1 (), &getBox2 (), &getBox3 (), &getBox4 () }),
	               multiView (false),
	               positions ({ X3D::Vector3f (0, 10, 0), X3D::Vector3f (), X3D::Vector3f (10, 0, 0), X3D::Vector3f (0, 0, 10) }),
	            orientations ({ X3D::Rotation4f (1, 0, 0, -math::pi <float> / 2), X3D::Rotation4f (), X3D::Rotation4f (0, 1, 0, math::pi <float> / 2), X3D::Rotation4f () })
{
	addChildObjects (mainBrowser,
	                 topBrowser,
	                 rightBrowser,
	                 frontBrowser,
	                 browsers,
	                 activeLayer);

	unparent (getWidget ());

	mainBrowser -> setNotifyOnLoad (true);
	mainBrowser -> isStrict (false);

	for (size_t i = 0, size = browsers .size (); i < size; ++ i)
	{
		const auto & browser = browsers [i];
	
		browser -> signal_focus_out_event () .connect (sigc::bind (sigc::mem_fun (this, &NotebookPage::on_focus_out_event), i));
		browser -> signal_focus_in_event () .connect (sigc::bind (sigc::mem_fun (this, &NotebookPage::on_focus_in_event), i));
		browser -> initialized () .addInterest (&NotebookPage::set_initialized, this, i);
		browser -> setAntialiasing (4);
		browser -> show ();
	}

	getBox1 () .pack_start (*topBrowser,   true, true, 0);
	getBox2 () .pack_start (*mainBrowser,  true, true, 0);
	getBox3 () .pack_start (*rightBrowser, true, true, 0);
	getBox4 () .pack_start (*frontBrowser, true, true, 0);
	getBox2 () .set_visible (true);

	setup ();
}

void
NotebookPage::initialize ()
{
	X3DNotebookPageInterface::initialize ();
}

void
NotebookPage::set_initialized (const size_t index)
{
	const auto & browser = browsers [index];

	browser -> initialized () .removeInterest (&NotebookPage::set_initialized, this);
	
	if (++ initialized < browsers .size ())
		return;

	// Set up viewpoint.

	for (size_t i = 0, size = browsers .size (); i < size; ++ i)
	{
		try
		{
			const auto & browser = browsers [i];
	
			if (browser == mainBrowser)
				continue;
	
			mainBrowser -> changed () .addInterest (&X3D::Browser::addEvent, browser .getValue ());

			const auto viewpoint = browser -> getExecutionContext () -> getNamedNode <X3D::OrthoViewpoint> ("OrthoViewpoint");
			const auto grid      = browser -> getExecutionContext () -> getNamedNode ("Grid");

			viewpoint -> position ()    = positions [i];
			viewpoint -> orientation () = orientations [i];

			grid -> setField <X3D::SFRotation> ("rotation", X3D::Rotation4d (1, 0, 0, math::pi <double> / 2) * orientations [i]);
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	// Connect to active layer.

	mainBrowser -> getActiveLayer () .addInterest (&NotebookPage::set_activeLayer, this);

	set_activeLayer ();
}

void
NotebookPage::set_activeLayer ()
{
	if (activeLayer)
	{
		for (const auto & browser : browsers)
		{
			try
			{
				if (browser == mainBrowser)
					continue;
		
				auto & children = browser -> getExecutionContext () -> getNamedNode <X3D::Group> ("Group") -> children ();
		
				activeLayer -> children () .removeInterest (children);
			}
			catch (const X3D::X3DError & error)
			{
				__LOG__ << error .what () << std::endl;
			}
		}
	}

	activeLayer = mainBrowser -> getActiveLayer ();

	for (const auto & browser : browsers)
	{
		try
		{
			if (browser == mainBrowser)
				continue;
	
			auto & children = browser -> getExecutionContext () -> getNamedNode <X3D::Group> ("Group") -> children ();
		
			if (activeLayer)
			{
				activeLayer -> children () .addInterest (children);
				children = activeLayer -> children ();
			}
			else
				children .clear ();
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}
}

bool
NotebookPage::on_focus_out_event (GdkEventFocus* event, const size_t index)
{
	widgets [index] -> get_style_context () -> remove_class ("titania-widget-box-selected");
	return false;
}

bool
NotebookPage::on_focus_in_event (GdkEventFocus* event, const size_t index)
{
	widgets [index] -> get_style_context () -> add_class ("titania-widget-box-selected");
	return false;
}

int32_t
NotebookPage::getPageNumber () const
{
	return getBrowserWindow () -> getBrowserNotebook () .page_num (getWidget ());
}

const basic::uri &
NotebookPage::getWorldURL () const
{
	if (mainBrowser -> isInitialized ())
	   return mainBrowser -> getWorldURL ();

	return url;
}

void
NotebookPage::setModified (const bool value)
{
	modified = value;
}

bool
NotebookPage::getModified () const
{
	return undoHistory .getModified () or modified;
}

void
NotebookPage::addFileMonitor (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::FileMonitor> & fileMonitor)
{
	fileMonitors .emplace_back (file, fileMonitor);
}

void
NotebookPage::reset ()
{
	undoHistory .clear ();

	modified      = false;
	saveConfirmed = false;

	for (const auto & fileMonitor : fileMonitors)
	{
		fileMonitor .second -> cancel ();
		fileMonitor .first -> remove ();
	}

	fileMonitors .clear ();
}

bool
NotebookPage::on_box1_key_release_event (GdkEventKey* event)
{
	return on_box_key_release_event (event, 0);
}

bool
NotebookPage::on_box2_key_release_event (GdkEventKey* event)
{
	return on_box_key_release_event (event, 1);
}

bool
NotebookPage::on_box3_key_release_event (GdkEventKey* event)
{
	return on_box_key_release_event (event, 2);
}

bool
NotebookPage::on_box4_key_release_event (GdkEventKey* event)
{
	return on_box_key_release_event (event, 3);
}

bool
NotebookPage::on_box_key_release_event (GdkEventKey* event, const size_t index)
{
	switch (event -> keyval)
	{
		case GDK_KEY_space:
		{
			multiView = not multiView;

			for (size_t i = 0, size = widgets .size (); i < size; ++ i)
			{
				widgets [i] -> set_visible (multiView or i == index);
			}

			return true;
		}
		default:
			return false;
	}

	return false;
}

NotebookPage::~NotebookPage ()
{
	dispose ();
}

} // puck
} // titania
