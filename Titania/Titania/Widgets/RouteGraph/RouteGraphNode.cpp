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

#include <Titania/X3D/Execution/X3DExecutionContext.h>
#include <Titania/X3D/Parser/Filter.h>

namespace titania {
namespace puck {

RouteGraphNode::RouteGraphNode (const X3D::SFNode & node) :
	             Gtk::Box (),
	        X3D::X3DInput (),
	                 node (node),
	          headerInput (nullptr),
	         headerOutput (nullptr),
	       fieldsRevealer (nullptr),
	               inputs (),
	              outputs (),
	             selected (false),
	             expanded (true),
	     headerConnectors (false),
	  connectorsSensitive (true),
	     selectNodeSignal (),
	 inputConnectorSignal (),
	outputConnectorSignal (),
	        changedSignal ()
{
	// Box

	node -> fields_changed () .addInterest (&RouteGraphNode::build, this);

	get_style_context () -> add_class ("titania-route-graph-node");

	build ();
}

void
RouteGraphNode::setSelected (const bool value)
{
	selected = value;

	if (selected)
		get_style_context () -> add_class ("titania-route-graph-node-selected");
	else
		get_style_context () -> remove_class ("titania-route-graph-node-selected");
}

void
RouteGraphNode::setExpanded (const bool value)
{
	expanded = value;

	fieldsRevealer -> set_reveal_child (expanded);
}

void
RouteGraphNode::setConnectorsSensitive (const bool value)
{
	connectorsSensitive = value;

	for (const auto & pair : inputs)
		pair .second -> set_sensitive (connectorsSensitive);

	for (const auto & pair : outputs)
		pair .second -> set_sensitive (connectorsSensitive);
}

///  throws std::out_of_range
X3D::Vector2i
RouteGraphNode::getInputPosition (X3D::X3DFieldDefinition* const field) const
{
	const auto button = headerConnectors ? headerInput : inputs .at (field);
	const auto width  = button -> get_width ();
	const auto height = button -> get_height ();

	int x, y;

	button -> translate_coordinates (*const_cast <RouteGraphNode*> (this), width / 2, height / 2, x, y);

	return X3D::Vector2i (x, y);
}

///  throws std::out_of_range
X3D::Vector2i
RouteGraphNode::getOutputPosition (X3D::X3DFieldDefinition* const field) const
{
	const auto button = headerConnectors ? headerOutput : outputs .at (field);
	const auto width  = button -> get_width ();
	const auto height = button -> get_height ();

	int x, y;

	button -> translate_coordinates (*const_cast <RouteGraphNode*> (this), width / 2, height / 2, x, y);

	return X3D::Vector2i (x, y);
}

void
RouteGraphNode::disableInputConnectors (const X3D::X3DExecutionContextPtr & executionContext,
                                        const X3D::SFNode & sourceNode,
                                        X3D::X3DFieldDefinition* const sourceField)
{
	const bool disableInputs = node == sourceNode or (executionContext not_eq node -> getExecutionContext () and not executionContext -> isImportedNode (node));

	for (const auto & input : inputs)
	{
		const auto field  = input .first;
		const auto button = input .second;

		if (field -> getType () not_eq sourceField -> getType () or disableInputs)
			button-> set_visible (false);
	}

	for (const auto & output : outputs)
	{
		const auto field  = output .first;
		const auto button = output .second;

		if (field == sourceField)
			button -> set_sensitive (false);
		else
			button -> set_visible (false);
	}
}

void
RouteGraphNode::disableOutputConnectors (const X3D::X3DExecutionContextPtr & executionContext,
                                         const X3D::SFNode & destinationNode,
                                         X3D::X3DFieldDefinition* const destinationField)
{
	const bool disableOutputs = node == destinationNode or (executionContext not_eq node -> getExecutionContext () and not executionContext -> isImportedNode (node));

	for (const auto & input : inputs)
	{
		const auto field  = input .first;
		const auto button = input .second;

		if (field == destinationField)
			button-> set_sensitive (false);
		else
			button -> set_visible (false);
	}

	for (const auto & output : outputs)
	{
		const auto field  = output .first;
		const auto button = output .second;

		if (field -> getType () not_eq destinationField -> getType () or disableOutputs)
			button -> set_visible (false);
	}
}

void
RouteGraphNode::enableConnectors ()
{
	for (const auto & input : inputs)
	{
		input .second -> set_sensitive (true);
		input .second -> set_visible (true);
	}

	for (const auto & output : outputs)
	{
		output .second -> set_sensitive (true);
		output .second -> set_visible (true);
	}
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

	node -> name_changed () .removeInterest (&RouteGraphNode::set_name, this);

	inputs  .clear ();
	outputs .clear ();

	for (const auto & widget : get_children ())
		remove (*widget);

	// Box

	const auto box = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_VERTICAL, 0));

	pack_start (*box, true, true);

	get_style_context () -> add_class ("titania-route-graph-node-box");

	// Header

	const auto displayName = X3D::GetDisplayName (node);

	const auto headerOverlay = Gtk::manage (new Gtk::Overlay ());
	const auto headerBox     = Gtk::manage (new Gtk::Box ());
	const auto headerLeft    = Gtk::manage (new Gtk::Box ());
	const auto headerRight   = Gtk::manage (new Gtk::Box ());

	const auto header      = Gtk::manage (new Gtk::Grid ());
	const auto imageBox    = Gtk::manage (new Gtk::Box ());
	const auto image       = Gtk::manage (new Gtk::Image ());
	const auto typeName    = Gtk::manage (new Gtk::Label ());
	const auto name        = Gtk::manage (new Gtk::Label ());
	const auto buttonBox   = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_VERTICAL, 0));
	const auto arrowButton = Gtk::manage (new Gtk::Button ());
	const auto arrow       = Gtk::manage (new Gtk::Image ());

	arrowButton -> signal_clicked () .connect (sigc::mem_fun (this, &RouteGraphNode::on_select_node_clicked));

	box -> pack_start (*headerOverlay, true, true);
	headerOverlay -> add (*headerBox);

	headerBox -> set_hexpand (true);
	headerBox -> set_hexpand_set (true);

	headerBox -> pack_start (*headerLeft,  false, true);
	headerBox -> pack_start (*header,      false, true);
	headerBox -> pack_start (*headerRight, false, true);

	header -> attach (*imageBox,  0, 0, 1, 1);
	header -> attach (*name,      1, 0, 1, 1);
	header -> attach (*typeName,  1, 1, 1, 1);
	header -> attach (*buttonBox, 2, 0, 1, 2);

	header   -> set_hexpand (true);
	header   -> set_hexpand_set (true);
	name     -> set_hexpand (true);
	name     -> set_hexpand_set (true);
	typeName -> set_hexpand (true);
	typeName -> set_hexpand_set (true);

	imageBox    -> pack_start (*image,       true,  true);
	buttonBox   -> pack_start (*arrowButton, false, true);

	image       -> set (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/X3DBaseNode.svg")));
	typeName    -> set_text (node -> getTypeName ());
	name        -> set_text (displayName .empty () ? _ ("<unnamed>") : displayName);
	arrowButton -> set_always_show_image (true);
	arrowButton -> set_image (*arrow);
	arrowButton -> set_tooltip_text (_ ("Select node."));
	arrow       -> set (Gtk::StockID ("Arrow"), Gtk::ICON_SIZE_MENU);

	image    -> set_valign (Gtk::ALIGN_CENTER);
	typeName -> set_halign (Gtk::ALIGN_START);
	name     -> set_halign (Gtk::ALIGN_START);
	arrow    -> set_valign (Gtk::ALIGN_START);

	headerBox   -> get_style_context () -> add_class ("titania-route-graph-node-element-box");
	headerLeft  -> get_style_context () -> add_class ("titania-route-graph-node-element-left");
	headerRight -> get_style_context () -> add_class ("titania-route-graph-node-element-right");
	header      -> get_style_context () -> add_class ("titania-route-graph-node-header");
	imageBox    -> get_style_context () -> add_class ("titania-route-graph-node-image-box");
	image       -> get_style_context () -> add_class ("titania-route-graph-node-image");
	typeName    -> get_style_context () -> add_class ("titania-route-graph-node-type-name");
	name        -> get_style_context () -> add_class ("titania-route-graph-node-name");
	arrowButton -> get_style_context () -> add_class ("titania-route-graph-node-arrow");

	// Fields

	fieldsRevealer = Gtk::manage (new Gtk::Revealer ());

	const auto fieldsBox = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_VERTICAL, 0));

	fieldsRevealer -> property_reveal_child ()   .signal_changed () .connect (sigc::mem_fun (this, &RouteGraphNode::on_reveal_fields));
	fieldsRevealer -> property_child_revealed () .signal_changed () .connect (sigc::mem_fun (this, &RouteGraphNode::on_fields_revealed));

	fieldsBox -> set_homogeneous (true);
	fieldsBox -> set_hexpand (true);
	fieldsBox -> set_hexpand_set (true);

	box -> pack_start (*fieldsRevealer, true, true);
	fieldsRevealer -> add (*fieldsBox);

	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (field -> getAccessType () == X3D::initializeOnly)
			continue;

		// Field

		const auto overlay  = Gtk::manage (new Gtk::Overlay ());
		const auto box      = Gtk::manage (new Gtk::Box ());
		const auto left     = Gtk::manage (new Gtk::Box ());
		const auto right    = Gtk::manage (new Gtk::Box ());
		const auto fieldBox = Gtk::manage (new Gtk::Box ());
		const auto image    = Gtk::manage (new Gtk::Image ());
		const auto name     = Gtk::manage (new Gtk::Label ());

		image -> set (Gdk::Pixbuf::create_from_file (get_ui ("icons/FieldType/" + field -> getTypeName () + ".svg")));
		name  -> set_text (field -> getName ());

		fieldsBox -> pack_start (*overlay, false, true);
		overlay  -> add (*box);

		box -> pack_start (*left,     false, true);
		box -> pack_start (*fieldBox, true,  true);
		box -> pack_start (*right,    false, true);

		fieldBox -> set_hexpand (true);
		fieldBox -> set_hexpand_set (true);

		fieldBox -> pack_start (*image, false, true);
		fieldBox -> pack_start (*name,  false, true);

		name -> set_halign (Gtk::ALIGN_START);

		box      -> get_style_context () -> add_class ("titania-route-graph-node-element-box");
		left     -> get_style_context () -> add_class ("titania-route-graph-node-element-left");
		right    -> get_style_context () -> add_class ("titania-route-graph-node-element-right");
		fieldBox -> get_style_context () -> add_class ("titania-route-graph-field");
		image    -> get_style_context () -> add_class ("titania-route-graph-field-image");
		name     -> get_style_context () -> add_class ("titania-route-graph-field-name");

		// Connectors

		if (field -> getAccessType () & X3D::inputOnly)
		{
			const auto input = Gtk::manage (new Gtk::Button ());

			input -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &RouteGraphNode::on_input_connector_clicked), field));

			input -> set_halign (Gtk::ALIGN_START);
			input -> set_valign (Gtk::ALIGN_CENTER);

			overlay -> add_overlay (*input);

			input -> get_style_context () -> add_class ("titania-route-graph-connector");
			input -> get_style_context () -> add_class ("titania-route-graph-input");

			inputs .emplace (field, input);
		}

		if (field -> getAccessType () & X3D::outputOnly)
		{
			const auto output = Gtk::manage (new Gtk::Button ());

			output -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &RouteGraphNode::on_output_connector_clicked), field));

			output -> set_halign (Gtk::ALIGN_END);
			output -> set_valign (Gtk::ALIGN_CENTER);

			overlay -> add_overlay (*output);

			output -> get_style_context () -> add_class ("titania-route-graph-connector");
			output -> get_style_context () -> add_class ("titania-route-graph-output");

			outputs .emplace (field, output);
		}
	}

	// Footer

	const auto footerBox   = Gtk::manage (new Gtk::Box ());
	const auto footerLeft  = Gtk::manage (new Gtk::Box ());
	const auto footerRight = Gtk::manage (new Gtk::Box ());
	const auto footer      = Gtk::manage (new Gtk::Button ());

	footer -> signal_clicked () .connect (sigc::mem_fun (this, &RouteGraphNode::on_footer_clicked));

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

	// Header Connectors

	headerInput  = Gtk::manage (new Gtk::Button ());
	headerOutput = Gtk::manage (new Gtk::Button ());

	headerInput  -> set_visible (headerConnectors);
	headerOutput -> set_visible (headerConnectors);
	headerInput  -> set_sensitive (false);
	headerOutput -> set_sensitive (false);

	headerInput  -> set_halign (Gtk::ALIGN_START);
	headerInput  -> set_valign (Gtk::ALIGN_CENTER);
	headerOutput -> set_halign (Gtk::ALIGN_END);
	headerOutput -> set_valign (Gtk::ALIGN_CENTER);

	headerOverlay -> add_overlay (*headerInput);
	headerOverlay -> add_overlay (*headerOutput);

	headerInput  -> get_style_context () -> add_class ("titania-route-graph-connector");
	headerInput  -> get_style_context () -> add_class ("titania-route-graph-input");
	headerOutput -> get_style_context () -> add_class ("titania-route-graph-connector");
	headerOutput -> get_style_context () -> add_class ("titania-route-graph-output");

	// Reveal at end, when all widget are created.

	fieldsRevealer -> set_reveal_child (expanded);
}

void
RouteGraphNode::on_select_node_clicked ()
{
	selectNodeSignal .emit ();
}

void
RouteGraphNode::on_input_connector_clicked (X3D::X3DFieldDefinition* const field)
{
	inputConnectorSignal .emit (field);
}

void
RouteGraphNode::on_output_connector_clicked (X3D::X3DFieldDefinition* const field)
{
	outputConnectorSignal .emit (field);
}

void
RouteGraphNode::on_footer_clicked ()
{
	setExpanded (not expanded);
}

void
RouteGraphNode::on_reveal_fields ()
{
	if (fieldsRevealer -> get_reveal_child ())
		fieldsRevealer -> set_visible (true);

	if (not fieldsRevealer -> get_reveal_child ())
	{
		headerConnectors = true;

		headerInput   -> set_visible (headerConnectors);
		headerOutput  -> set_visible (headerConnectors);
	}

	changedSignal .emit ();
}

void
RouteGraphNode::on_fields_revealed ()
{
	if (not fieldsRevealer -> get_reveal_child ())
		fieldsRevealer -> set_visible (false);

	if (fieldsRevealer -> get_reveal_child ())
	{
		headerConnectors = false;

		headerInput   -> set_visible (headerConnectors);
		headerOutput  -> set_visible (headerConnectors);
	}

	changedSignal .emit ();
}

RouteGraphNode::~RouteGraphNode ()
{ }

} // puck
} // titania

