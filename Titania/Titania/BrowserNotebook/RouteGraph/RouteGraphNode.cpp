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

#include "RouteGraphNode.h"

#include "../../Configuration/config.h"

namespace titania {
namespace puck {

RouteGraphNode::RouteGraphNode (Gtk::Fixed* const fixed, const X3D::SFNode & node, const int32_t x, const int32_t y) :
	Gtk::EventBox (),
	        fixed (fixed),
	         node (node),
	       button (0),
	     position (x, y),
	      pointer ()
{
	// Box

	get_style_context () -> add_class ("titania-route-graph-node");

	node -> fields_changed () .addInterest (&RouteGraphNode::build, this);

	build ();

	fixed -> put (*this, position .x (), position .y ());
}

bool
RouteGraphNode::on_button_press_event (GdkEventButton* event)
{
	button  = event -> button;
	pointer = X3D::Vector2d (event -> x, event -> y);

	fixed -> remove (*this);
	fixed -> put (*this, position .x (), position .y ());

	return true;
}

bool
RouteGraphNode::on_button_release_event (GdkEventButton* event)
{
	button = 0;

	return true;
}

bool
RouteGraphNode::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 1)
	{
		position += X3D::Vector2d (event -> x, event -> y) - pointer;
	
		fixed -> move (*this, std::max <int32_t> (0, position .x ()), std::max <int32_t> (0, position .y ()));

		return true;
	}

	return true;
}

void
RouteGraphNode::set_name (Gtk::Label* const name)
{
	name -> set_text (node -> getName ());
}

void
RouteGraphNode::build ()
{
	// Clear

	remove ();

	node -> name_changed () .removeInterest (&RouteGraphNode::set_name, this);

	// Box

	const auto box = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_VERTICAL, 2));

	add (*box);

	get_style_context () -> add_class ("titania-route-graph-node-box");

	// Header

	const auto headerBox   = Gtk::manage (new Gtk::Box ());
	const auto headerLeft  = Gtk::manage (new Gtk::Box ());
	const auto headerRight = Gtk::manage (new Gtk::Box ());

	const auto header   = Gtk::manage (new Gtk::Grid ());
	const auto imageBox = Gtk::manage (new Gtk::Box ());
	const auto image    = Gtk::manage (new Gtk::Image ());
	const auto typeName = Gtk::manage (new Gtk::Label ());
	const auto name     = Gtk::manage (new Gtk::Label ());

	box -> pack_start (*headerBox, true, true);

	headerBox -> pack_start (*headerLeft,  false, true);
	headerBox -> pack_start (*header,      false, true);
	headerBox -> pack_start (*headerRight, false, true);

	header -> attach (*imageBox, 0, 0, 1, 1);
	header -> attach (*typeName, 1, 0, 1, 1);
	header -> attach (*name,     1, 1, 1, 1);

	header -> set_hexpand (true);
	header -> set_hexpand_set (true);

	imageBox -> pack_start (*image, true, true);

	image    -> set (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/X3DBaseNode.svg")));
	typeName -> set_text (node -> getTypeName ());
	name     -> set_text (node -> getName ());

	image    -> set_valign (Gtk::ALIGN_CENTER);
	typeName -> set_halign (Gtk::ALIGN_START);
	name     -> set_halign (Gtk::ALIGN_START);

	headerBox   -> get_style_context () -> add_class ("titania-route-graph-node-element-box");
	headerLeft  -> get_style_context () -> add_class ("titania-route-graph-node-element-left");
	headerRight -> get_style_context () -> add_class ("titania-route-graph-node-element-right");
	header      -> get_style_context () -> add_class ("titania-route-graph-node-header");
	imageBox    -> get_style_context () -> add_class ("titania-route-graph-node-image-box");
	image       -> get_style_context () -> add_class ("titania-route-graph-node-image");
	typeName    -> get_style_context () -> add_class ("titania-route-graph-node-type-name");
	name        -> get_style_context () -> add_class ("titania-route-graph-node-name");

	// Fields

	const auto fieldBox = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_VERTICAL, 2));

	fieldBox -> set_spacing (2);
	fieldBox -> set_homogeneous (true);
	fieldBox -> set_hexpand (true);
	fieldBox -> set_hexpand_set (true);

	box -> pack_start (*fieldBox, true, true);

	for (const auto & fieldDefinition : node -> getFieldDefinitions ())
	{
		if (fieldDefinition -> getAccessType () == X3D::initializeOnly)
			continue;

		// Field

		const auto overlay = Gtk::manage (new Gtk::Overlay ());
		const auto box     = Gtk::manage (new Gtk::Box ());
		const auto left    = Gtk::manage (new Gtk::Box ());
		const auto right   = Gtk::manage (new Gtk::Box ());
		const auto field   = Gtk::manage (new Gtk::Box ());
		const auto image   = Gtk::manage (new Gtk::Image ());
		const auto name    = Gtk::manage (new Gtk::Label ());

		image -> set (Gdk::Pixbuf::create_from_file (get_ui ("icons/FieldType/" + fieldDefinition -> getTypeName () + ".svg")));
		name  -> set_text (fieldDefinition -> getName ());

		fieldBox -> pack_start (*overlay, false, true);
		overlay  -> add (*box);

		box -> pack_start (*left,  false, true);
		box -> pack_start (*field, true,  true);
		box -> pack_start (*right, false, true);

		field -> pack_start (*image, false, true);
		field -> pack_start (*name,  false, true);

		name -> set_halign (Gtk::ALIGN_START);

		box   -> get_style_context () -> add_class ("titania-route-graph-node-element-box");
		left  -> get_style_context () -> add_class ("titania-route-graph-node-element-left");
		right -> get_style_context () -> add_class ("titania-route-graph-node-element-right");
		field -> get_style_context () -> add_class ("titania-route-graph-field");
		image -> get_style_context () -> add_class ("titania-route-graph-field-image");
		name  -> get_style_context () -> add_class ("titania-route-graph-field-name");

		// Connectors

		if (fieldDefinition -> getAccessType () & X3D::inputOnly)
		{
			const auto input  = Gtk::manage (new Gtk::Button ());
	
			input -> set_halign (Gtk::ALIGN_START);
			input -> set_valign (Gtk::ALIGN_CENTER);
	
			overlay -> add_overlay (*input);

			input  -> get_style_context () -> add_class ("titania-route-graph-connector");
			input  -> get_style_context () -> add_class ("titania-route-graph-input");
		}

		if (fieldDefinition -> getAccessType () & X3D::outputOnly)
		{
			const auto output = Gtk::manage (new Gtk::Button ());
	
			output -> set_halign (Gtk::ALIGN_END);
			output -> set_valign (Gtk::ALIGN_CENTER);
	
			overlay -> add_overlay (*output);
	
			output -> get_style_context () -> add_class ("titania-route-graph-connector");
			output -> get_style_context () -> add_class ("titania-route-graph-output");
		}
	}

	// Footer

	const auto footerBox   = Gtk::manage (new Gtk::Box ());
	const auto footerLeft  = Gtk::manage (new Gtk::Box ());
	const auto footerRight = Gtk::manage (new Gtk::Box ());
	const auto footer      = Gtk::manage (new Gtk::Box ());

	footer -> set_hexpand (true);
	footer -> set_hexpand_set (true);

	box       -> pack_start (*footerBox,   true,  true);
	footerBox -> pack_start (*footerLeft,  false, true);
	footerBox -> pack_start (*footer,      false, true);
	footerBox -> pack_start (*footerRight, false, true);

	footerBox   -> get_style_context () -> add_class ("titania-route-graph-node-element-box");
	footerLeft  -> get_style_context () -> add_class ("titania-route-graph-node-footer-left");
	footerLeft  -> get_style_context () -> add_class ("titania-route-graph-node-element-left");
	footerRight -> get_style_context () -> add_class ("titania-route-graph-node-footer-right");
	footerRight -> get_style_context () -> add_class ("titania-route-graph-node-element-right");
	footer      -> get_style_context () -> add_class ("titania-route-graph-node-footer");

	// Node name

	node -> name_changed () .addInterest (&RouteGraphNode::set_name, this, name);

	box -> show_all ();
}

RouteGraphNode::~RouteGraphNode ()
{ }

} // puck
} // titania

