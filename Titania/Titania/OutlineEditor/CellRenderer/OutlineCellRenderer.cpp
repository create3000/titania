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

#include "OutlineCellRenderer.h"

#include "../../Browser/BrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Undo/UndoStep.h"
#include "../OutlineTreeModel.h"
#include "../X3DOutlineTreeView.h"
#include "OutlineFields.h"

#include <Titania/String/to_string.h>
#include <Titania/String/sprintf.h>

namespace titania {
namespace puck {

using math::M_PI1_2;
using math::M_PI2;
using math::M_PI3_2;

static constexpr double ICON_X_PAD         = 3;
static constexpr double NAME_X_PAD         = 1;
static constexpr double ACCESS_TYPE_X_PAD  = 6;
static constexpr double ACCESS_TYPE_RADIUS = 5.5;// Depends on image

static constexpr double INPUT_WIDTH      = 13;   // Depends on image
static constexpr double OUTPUT_WIDTH     = 14;   // Depends on image
static constexpr double FIELD_WIDTH      = 10;   // Depends on image
static constexpr double FIELD_PAD        = 1;    // Depends on image
static constexpr double INPUT_PAD        = 8;    // Depends on image (one pixel less)
static constexpr double OUTPUT_PAD       = 7;    // Depends on image (one pixel less)
static constexpr double INPUT_OUTPUT_PAD = 9;    // Depends on image (one pixel less)
static constexpr double CONNECTOR_WIDTH  = 6;    // Depends on image (two pixel more)

static constexpr double ROUTE_WIDTH       = 16;
static constexpr double ROUTE_CURVE_WIDTH = 16;
static constexpr double ROUTE_RADIUS      = 8.5;
static constexpr double ROUTE_INPUT_PAD   = 14;
static constexpr double ROUTE_Y_PAD       = 3.5; // Depends on image
static constexpr double RIGHT_PAD         = 8;

OutlineCellRenderer::OutlineCellRenderer (X3D::X3DBrowser* const browser, X3DOutlineTreeView* const treeView) :
	             Glib::ObjectBase (typeid (OutlineCellRenderer)),
	        Gtk::CellRendererText (),
	                     treeView (treeView),
	                data_property (*this, "tree-data", nullptr),
	            cellrenderer_icon (),
	cellrenderer_access_type_icon (),
	                    noneImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/FieldType/none.png"))),
	        executionContextImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/X3DExecutionContext.svg"))),
	                baseNodeImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/X3DBaseNode.svg"))),
	               baseNodeUImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/X3DBaseNode.u.svg"))),
	               prototypeImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/Prototype.svg"))),
	             externProtoImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/ExternProto.svg"))),
	            importedNodeImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/ImportedNode.svg"))),
	            exportedNodeImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/ExportedNode.svg"))),
	              sharedNodeImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/SharedNode.svg"))),
	             sharedNodeUImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/SharedNode.u.svg"))),
	                   routeImage (Gdk::Pixbuf::create_from_file (get_ui ("icons/Node/Route.svg"))),
	              fieldTypeImages (),
	             accessTypeImages (),
	                   accessType (),
	                       routes (),
	                     textview ()
{
	// Images

	for (const auto & field : browser -> getSupportedFields ())
		fieldTypeImages [field -> getType ()] = Gdk::Pixbuf::create_from_file (get_ui ("icons/FieldType/" + field -> getTypeName () + ".svg"));

	accessTypeImages [X3D::initializeOnly] .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/initializeOnly.png")));
	accessTypeImages [X3D::inputOnly]      .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOnly.0.png")));
	accessTypeImages [X3D::inputOnly]      .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOnly.1.png")));
	accessTypeImages [X3D::inputOnly]      .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOnly.2.png")));
	accessTypeImages [X3D::outputOnly]     .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/outputOnly.0.png")));
	accessTypeImages [X3D::outputOnly]     .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/outputOnly.1.png")));
	accessTypeImages [X3D::outputOnly]     .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/outputOnly.2.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.0.0.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.0.1.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.0.2.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (noneImage);
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.1.0.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.1.1.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.1.2.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (noneImage);
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.2.0.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.2.1.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (get_ui ("icons/AccessType/inputOutput.2.2.png")));

	// CellRendererPixbuf

	cellrenderer_icon             .set_alignment (0, 0.5);
	cellrenderer_access_type_icon .set_alignment (0, 0.5);

	// Signals

	data_property .get_proxy () .signal_changed () .connect (sigc::mem_fun (this, &OutlineCellRenderer::on_data));
}

void
OutlineCellRenderer::on_data ()
{
	property_editable () = false;
	set_alignment (0, 0.5);

	cellrenderer_icon .property_pixbuf () = get_icon ();

	switch (get_data_type ())
	{
		case OutlineIterType::X3DInputRoute:
		{
			try
			{
				const auto route  = static_cast <X3D::Route*> (get_object ());
				const auto source = route -> getSourceNode ();

				const std::string name = source -> getName () .empty ()
				                         ? _ ("<unnamed>")
												 : source -> getName ();

				property_text () = _ ("Route from ") + name + "." + route -> getSourceField ();
			}
			catch (const X3D::X3DError &)
			{
				property_text () = "";
			}

			cellrenderer_access_type_icon .property_pixbuf () = accessTypeImages [X3D::inputOnly] [1];
			accessType                                        = X3D::inputOnly;
			break;
		}
		case OutlineIterType::X3DOutputRoute:
		{
			try
			{
				const auto route       = static_cast <X3D::Route*> (get_object ());
				const auto destination = route -> getDestinationNode ();

				const std::string name = destination -> getName () .empty ()
				                         ? _ ("<unnamed>")
												 : destination -> getName ();

				property_text () = _ ("Route to ") + name + "." + route -> getDestinationField ();
			}
			catch (...)
			{
				property_text () = "";
			}

			cellrenderer_access_type_icon .property_pixbuf () = accessTypeImages [X3D::outputOnly] [1];
			accessType                                        = X3D::outputOnly;
			break;
		}
		case OutlineIterType::X3DFieldValue:
		{
			property_editable () = true;
			set_alignment (0, 0);

			property_text () = get_field_value (static_cast <X3D::X3DFieldDefinition*> (get_object ()), true);
			break;
		}
		case OutlineIterType::X3DField:
		{
			const auto userData = property_data () .get_value () -> get_user_data ();

			if (userData -> selected & OUTLINE_SPECIAL)
				property_markup () = "<i><b>" + Glib::Markup::escape_text (get_object () -> getName ()) + "</b></i>";

			else if (not static_cast <X3D::X3DFieldDefinition*> (get_object ()) -> getReferences () .empty ())
				property_markup () = "<b>" + Glib::Markup::escape_text (get_object () -> getName ()) + "</b>";

			else
				property_text () = get_object () -> getName ();

			cellrenderer_access_type_icon .property_pixbuf () = get_access_type_icon (accessType);
			break;
		}
		case OutlineIterType::X3DExecutionContext:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (get_object ());
			const auto   scene  = dynamic_cast <X3D::Scene*> (sfnode .getValue ());

			if (scene)
				property_markup () = "<i><b>Scene</b> »" + Glib::Markup::escape_text (scene -> getWorldURL () .basename ()) + "«</i>";

			else if (dynamic_cast <X3D::X3DProtoObject*> (sfnode .getValue ()))
				property_markup () = "<i><b>Body</b></i>";

			else // X3DPrototypeInstance
				property_markup () = "<i><b>Scene</b> " + Glib::Markup::escape_text (sfnode -> getTypeName ()) + "</i>";

			break;
		}
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		{
			property_markup () = get_node_name (*static_cast <X3D::SFNode*> (get_object ()), "");
			break;
		}
		case OutlineIterType::ImportedNode:
		{
			try
			{
				const auto & sfnode       = *static_cast <X3D::SFNode*> (get_object ());
				const auto   importedNode = dynamic_cast <X3D::ImportedNode*> (sfnode .getValue ());
				const auto   exportedNode = importedNode -> getExportedNode ();

				property_markup () = get_node_name (exportedNode, importedNode -> getImportedName ());
			}
			catch (...)
			{
				property_text () = "";
			}
			break;
		}
		case OutlineIterType::ExportedNode:
		{
			try
			{
				const auto & sfnode       = *static_cast <X3D::SFNode*> (get_object ());
				const auto   exportedNode = dynamic_cast <X3D::ExportedNode*> (sfnode .getValue ());
				const auto   localNode    = exportedNode -> getLocalNode ();

				property_markup () = get_node_name (localNode, exportedNode -> getExportedName ());
			}
			catch (...)
			{
				property_text () = "";
			}
			break;
		}
		case OutlineIterType::Separator:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (get_object ());

			set_alignment (0, math::M_PHI - 1);
			property_markup () = "<b><small>" + Glib::Markup::escape_text (sfnode -> getName ()) + "</small></b>";
			break;
		}
	}
}

OutlineIterType
OutlineCellRenderer::get_data_type () const
{
	return property_data () .get_value () -> get_type ();
}

X3D::X3DChildObject*
OutlineCellRenderer::get_object () const
{
	return property_data () .get_value () -> get_object ();
}

bool
OutlineCellRenderer::is_expanded () const
{
	const auto userData = property_data () .get_value () -> get_user_data ();

	return userData -> expanded;
}

bool
OutlineCellRenderer::is_full_expanded () const
{
	const auto userData = property_data () .get_value () -> get_user_data ();

	return userData -> full_expanded;
}

const Glib::RefPtr <Gdk::Pixbuf> &
OutlineCellRenderer::get_icon () const
{
	switch (get_data_type ())
	{
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
			return routeImage;

		case OutlineIterType::X3DFieldValue:
			return noneImage;

		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (get_object ());
			const auto iter  = fieldTypeImages .find (field -> getType ());

			if (iter not_eq fieldTypeImages .end ())
				return iter -> second;

			return noneImage;
		}
		case OutlineIterType::X3DExecutionContext:
			return executionContextImage;

		case OutlineIterType::X3DBaseNode:
		{
			const auto sfnode = static_cast <X3D::SFNode*> (get_object ());
			const auto node   = sfnode -> getValue ();

			if (not node)
				return baseNodeImage;

			if (node -> getExecutionContext () == treeView -> get_model () -> get_execution_context ())
			{
				if (node -> isInitialized () and not node -> getExecutionContext () -> isProtoDeclaration ())
					return baseNodeImage;
				
				return baseNodeUImage;
			}

			if (treeView -> get_model () -> get_execution_context () -> isImportedNode (*sfnode))
				return importedNodeImage;

			if (node -> isInitialized () and not node -> getExecutionContext () -> isProtoDeclaration ())
				return sharedNodeImage;

			return sharedNodeUImage;
		}
		case OutlineIterType::ExternProtoDeclaration:
			return externProtoImage;

		case OutlineIterType::ProtoDeclaration:
			return prototypeImage;

		case OutlineIterType::ImportedNode:
			return importedNodeImage;

		case OutlineIterType::ExportedNode:
			return exportedNodeImage;

		case OutlineIterType::Separator:
			return noneImage;
	}

	return noneImage;
}

const Glib::RefPtr <Gdk::Pixbuf> &
OutlineCellRenderer::get_access_type_icon (X3D::AccessType & accessType) const
{
	const auto field = static_cast <X3D::X3DFieldDefinition*> (get_object ());
	const auto iter  = accessTypeImages .find (field -> getAccessType ());
	size_t     index = 0;

	const size_t inputRoutes  = treeView -> get_model () -> get_input_routes_size (field);
	const size_t outputRoutes = treeView -> get_model () -> get_output_routes_size (field);

	accessType = field -> getAccessType ();

	switch (accessType)
	{
		case X3D::initializeOnly:
			break;
		case X3D::inputOnly:
			index = std::min <size_t> (inputRoutes, 2);
			break;
		case X3D::outputOnly:
			index = std::min <size_t> (outputRoutes, 2);
			break;
		case X3D::inputOutput:
			index = (std::min <size_t> (inputRoutes, 2) << 2) | (std::min <size_t> (outputRoutes, 2));
			break;
	}

	if (iter not_eq accessTypeImages .end ())
		return iter -> second [index];

	return noneImage;
}

std::string
OutlineCellRenderer::get_node_name (const X3D::SFNode & sfnode, std::string name) const
{
	if (sfnode)
	{
		const X3D::X3DBaseNode* node = sfnode .getValue ();

		// Get name.

		if (name .empty ())
			name = node -> getName ();

		X3D::RegEx::LastNumber_ .Replace ("", &name);

		// Add typeName and name.

		std::string string = "<b>" + Glib::Markup::escape_text (node -> getTypeName ()) + "</b> " + Glib::Markup::escape_text (name);

		// Add clone count if any.

		const auto numClones = node -> getCloneCount ();

		if (numClones > 1)
			string += " [" + basic::to_string (numClones) + "]";

		return string;
	}

	return "<b>NULL</b>";
}

bool
OutlineCellRenderer::is_array () const
{
	return static_cast <X3D::X3DFieldDefinition*> (get_object ()) -> isArray ();
}

void
OutlineCellRenderer::get_preferred_width_vfunc (Gtk::Widget & widget, int & minimum_width, int & natural_width) const
{
	minimum_width = 0;
	natural_width = 0;

	int minimum = 0;
	int natural = 0;

	// Icon

	{
		minimum_width += ICON_X_PAD;
		natural_width += ICON_X_PAD;

		cellrenderer_icon .get_preferred_width (widget, minimum, natural);
		minimum_width += minimum;
		natural_width += natural;
	}

	// Name or value

	{
		minimum_width += NAME_X_PAD;
		natural_width += NAME_X_PAD;

		Gtk::CellRendererText::get_preferred_width_vfunc (widget, minimum, natural);
		minimum_width += minimum;
		natural_width += natural;
	}

	// Access type and routes

	switch (get_data_type ())
	{
		case OutlineIterType::X3DField:
		{
			if (is_full_expanded () and is_expanded ())
				break;
		}
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		{
			minimum_width += ACCESS_TYPE_X_PAD;
			natural_width += ACCESS_TYPE_X_PAD;

			cellrenderer_access_type_icon .get_preferred_width (widget, minimum, natural);
			minimum_width += minimum;
			natural_width += natural;

			break;
		}
		default:
			break;
	}

	minimum_width += ROUTE_WIDTH + ROUTE_CURVE_WIDTH + RIGHT_PAD;
	natural_width += ROUTE_WIDTH + ROUTE_CURVE_WIDTH + RIGHT_PAD;
}

void
OutlineCellRenderer::get_preferred_height_for_width_vfunc (Gtk::Widget & widget, int width, int & minimum_height, int & natural_height) const
{
	const float diameter = 2 * ROUTE_RADIUS;

	minimum_height = diameter;
	natural_height = diameter;

	int minimum = 0;
	int natural = 0;

	// Icon

	{
		cellrenderer_access_type_icon .get_preferred_height_for_width (widget, width, minimum, natural);
		minimum_height = std::max (minimum, minimum_height);
		natural_height = std::max (natural, natural_height);
	}

	// Name or value

	{
		Gtk::CellRendererText::get_preferred_height_for_width_vfunc (widget, width, minimum, natural);
		minimum_height = std::max (minimum, minimum_height);
		natural_height = std::max (natural, natural_height);
	}

	// Access type and routes

	switch (get_data_type ())
	{
		case OutlineIterType::X3DField:
		{
			if (is_full_expanded () and is_expanded ())
				break;
		}
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		{
			cellrenderer_access_type_icon .get_preferred_height_for_width (widget, width, minimum, natural);
			minimum_height = std::max (minimum, minimum_height);
			natural_height = std::max (natural, natural_height);

			break;
		}
		default:
			break;
	}
}

void
OutlineCellRenderer::get_preferred_height_vfunc (Gtk::Widget & widget, int & minimum_height, int & natural_height) const
{
	const float diameter = 2 * ROUTE_RADIUS;

	minimum_height = diameter;
	natural_height = diameter;

	int minimum = 0;
	int natural = 0;

	// Icon

	{
		cellrenderer_icon .get_preferred_height (widget, minimum, natural);
		minimum_height = std::max (minimum, minimum_height);
		natural_height = std::max (natural, natural_height);
	}

	// Name or value

	{
		Gtk::CellRendererText::get_preferred_height_vfunc (widget, minimum, natural);
		minimum_height = std::max (minimum, minimum_height);
		natural_height = std::max (natural, natural_height);
	}

	// Access type and routes

	switch (get_data_type ())
	{
		case OutlineIterType::X3DField:
		{
			if (is_full_expanded () and is_expanded ())
				break;
		}
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		{
			cellrenderer_access_type_icon .get_preferred_height (widget, minimum, natural);
			minimum_height = std::max (minimum, minimum_height);
			natural_height = std::max (natural, natural_height);

			break;
		}
		default:
			break;
	}

}

void
OutlineCellRenderer::get_preferred_width_for_height_vfunc (Gtk::Widget & widget, int height, int & minimum_width, int & natural_width) const
{
	minimum_width = 0;
	natural_width = 0;

	int minimum = 0;
	int natural = 0;

	// Icon

	{
		minimum_width += ICON_X_PAD;
		natural_width += ICON_X_PAD;

		cellrenderer_icon .get_preferred_width_for_height (widget, height, minimum, natural);
		minimum_width += minimum;
		natural_width += natural;
	}

	// Name or value

	{
		minimum_width += NAME_X_PAD;
		natural_width += NAME_X_PAD;

		Gtk::CellRendererText::get_preferred_width_for_height_vfunc (widget, height, minimum, natural);
		minimum_width += minimum;
		natural_width += natural;
	}

	// Access type and routes

	switch (get_data_type ())
	{
		case OutlineIterType::X3DField:
		{
			if (is_full_expanded () and is_expanded ())
				break;
		}
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		{
			minimum_width += ACCESS_TYPE_X_PAD;
			natural_width += ACCESS_TYPE_X_PAD;

			cellrenderer_access_type_icon .get_preferred_width_for_height (widget, height, minimum, natural);
			minimum_width += minimum;
			natural_width += natural;

			break;
		}
		default:
			break;
	}

	minimum_width += ROUTE_WIDTH + ROUTE_CURVE_WIDTH + RIGHT_PAD;
	natural_width += ROUTE_WIDTH + ROUTE_CURVE_WIDTH + RIGHT_PAD;
}

Gtk::CellEditable*
OutlineCellRenderer::start_editing_vfunc (GdkEvent* event,
                                          Gtk::Widget & widget,
                                          const Glib::ustring & path,
                                          const Gdk::Rectangle & background_area,
                                          const Gdk::Rectangle & cell_area,
                                          Gtk::CellRendererState flags)
{
	switch (get_data_type ())
	{
		case OutlineIterType::X3DFieldValue:
		{
			int icon_width    = 0;
			int natural_width = 0;

			cellrenderer_icon .get_preferred_width (widget, icon_width, natural_width);

			Gtk::TreePath parentPath (path);
			parentPath .up ();
			parentPath .up ();

			const auto parent = treeView -> get_model () -> get_iter (parentPath);
			const auto node   = static_cast <X3D::SFNode*> (treeView -> get_object (parent));
			const auto field  = static_cast <X3D::X3DFieldDefinition*> (get_object ());
			const int  x_pad  = ICON_X_PAD + icon_width + NAME_X_PAD + property_xpad ();

			textview .reset (new TextViewEditable (*node, field, path, field -> isArray () or dynamic_cast <X3D::SFString*> (field)));
			textview -> set_text (get_field_value (field, false));
			textview -> set_margin_left (x_pad);
			textview -> set_margin_top (property_ypad ());
			textview -> set_margin_bottom (property_ypad ());
			textview -> set_size_request (cell_area .get_width () - x_pad, cell_area .get_height ());

			textview -> signal_editing_done () .connect (sigc::mem_fun (this, &OutlineCellRenderer::on_editing_done));

			return textview .get ();
		}
		default:
			break;
	}

	return nullptr;
}

void
OutlineCellRenderer::on_editing_done ()
{
	if (textview -> property_editing_canceled () or textview -> get_validated ())
		return;

	const std::string string = textview -> get_text ();

	if (set_field_value (textview -> get_node (), textview -> get_field (), string))
	{
		textview -> set_validated (true);
		textview -> remove_widget ();
		edited (textview -> get_path (), string);
	}
}

bool
OutlineCellRenderer::set_field_value (const X3D::SFNode & node, X3D::X3DFieldDefinition* const field, const std::string & string)
{
	if (field -> isArray ())
		return set_field_value (node, field, "[" + string + "]", true);

	return set_field_value (node, field, string, true);
}

bool
OutlineCellRenderer::set_field_value (const X3D::SFNode & node, X3D::X3DFieldDefinition* const field, const std::string & string, const bool)
{
	X3D::Generator::SmallestStyle ();

	if (field -> getType () == X3D::X3DConstants::SFString)
	{
		const auto        sfstring     = static_cast <X3D::SFString*> (field);
		const std::string currentValue = sfstring -> getValue ();

		sfstring -> setValue (string);

		if (string not_eq currentValue)
		{
			const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Edit Field »%s«"), field -> getName () .c_str ()));

			undoStep -> addVariables (node);

			undoStep -> addUndoFunction (&X3D::SFString::setValue, sfstring, currentValue);
			undoStep -> addRedoFunction (&X3D::SFString::setValue, sfstring, string);

			treeView -> getBrowserWindow () -> addUndoStep (undoStep);
		}

		return true;
	}

	const auto currentValue = field -> toString ();
	const auto locale       = std::locale ();

	if (field -> fromLocaleString (string, locale))
	{
		const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Edit Field »%s«"), field -> getName () .c_str ()));

		const X3D::InlinePtr inlineNode (node);
		
		if (inlineNode and (
		    (field -> getName () == "load" and field -> toString () == "FALSE") or 
		     field -> getName () == "url"))
		{
			treeView -> getBrowserWindow () -> removeImportedNodes (inlineNode, undoStep);

			if (not undoStep -> isEmpty ())
				undoStep -> addUndoFunction (&X3D::X3DBrowser::update, inlineNode -> getBrowser ());
		}

		if (field -> toString () not_eq currentValue or not undoStep -> isEmpty ())
		{
			undoStep -> addVariables (node);

			undoStep -> addUndoFunction (&X3D::X3DFieldDefinition::fromString, field, currentValue);
			undoStep -> addRedoFunction (&X3D::X3DFieldDefinition::fromLocaleString, field, string, locale);

			treeView -> getBrowserWindow () -> addUndoStep (undoStep);
		}

		return true;
	}

	return false;
}

void
OutlineCellRenderer::row_changed (const Glib::RefPtr <OutlineTreeModel> & model, const Glib::ustring & string_path)
{
	const Gtk::TreeModel::Path path (string_path);
	const auto iter = model -> get_iter (path);

	if (model -> iter_is_valid (iter))
		model -> row_changed (path, iter);
}

OutlineCellContent
OutlineCellRenderer::pick (Gtk::Widget & widget,
                           const Gdk::Rectangle & cell_area,
                           double point_x, double point_y)
{
	X3D::Vector2f point (point_x, point_y);

	int x              = cell_area .get_x ();
	int y              = cell_area .get_y ();
	int height         = cell_area .get_height ();
	int minimum_width  = 0;
	int natural_width  = 0;
	int minimum_height = 0;
	int natural_height = 0;

	// Icon

	{
		x += ICON_X_PAD;

		cellrenderer_icon .get_preferred_width  (widget, minimum_width,  natural_width);
		cellrenderer_icon .get_preferred_height (widget, minimum_height, natural_height);

		const X3D::Box2f box (X3D::Vector2f (minimum_width, minimum_height), X3D::Vector2f (x + minimum_width / 2, y + height / 2));

		if (box .intersect (point))
			return OutlineCellContent::ICON;

		x += minimum_width;
	}

	// Name or value

	{
		x += NAME_X_PAD;

		Gtk::CellRendererText::get_preferred_width_vfunc (widget, minimum_width, natural_width);

		const X3D::Box2f box (X3D::Vector2f (minimum_width, minimum_height), X3D::Vector2f (x + minimum_width / 2, y + height / 2));

		if (box .intersect (point))
			return OutlineCellContent::NAME;

		x += minimum_width;
	}

	// Access type and routes

	x += ACCESS_TYPE_X_PAD;
	cellrenderer_access_type_icon .get_preferred_height (widget, minimum_height, natural_height);

	switch (get_data_type ())
	{
		case OutlineIterType::X3DField:
		{
			if (is_full_expanded () and is_expanded ())
			{
				return OutlineCellContent::NONE;
			}

			const auto   field        = static_cast <X3D::X3DFieldDefinition*> (get_object ());
			const size_t inputRoutes  = treeView -> get_model () -> get_input_routes_size (field);
			const size_t outputRoutes = treeView -> get_model () -> get_output_routes_size (field);

			switch (field -> getAccessType ())
			{
				case X3D::initializeOnly:
				{
					return OutlineCellContent::NONE;
				}
				case X3D::inputOnly:
				{
					const X3D::Box2f box (X3D::Vector2f (INPUT_WIDTH, minimum_height), X3D::Vector2f (x + INPUT_WIDTH / 2, y + height / 2));

					if (box .intersect (point))
						return OutlineCellContent::INPUT;

					x += INPUT_WIDTH;

					if (inputRoutes)
					{
						x += INPUT_PAD;

						const X3D::Box2f box (X3D::Vector2f (CONNECTOR_WIDTH, minimum_height), X3D::Vector2f (x + CONNECTOR_WIDTH / 2, y + height / 2));

						if (box .intersect (point))
							return OutlineCellContent::INPUT_CONNECTOR;
					}

					break;
				}
				case X3D::outputOnly:
				{
					const X3D::Box2f box (X3D::Vector2f (OUTPUT_WIDTH, minimum_height), X3D::Vector2f (x + OUTPUT_WIDTH / 2, y + height / 2));

					if (box .intersect (point))
						return OutlineCellContent::OUTPUT;

					x += OUTPUT_WIDTH;

					if (outputRoutes)
					{
						x += OUTPUT_PAD;

						X3D::Box2f box (X3D::Vector2f (CONNECTOR_WIDTH, minimum_height), X3D::Vector2f (x + CONNECTOR_WIDTH / 2, y + height / 2));

						if (box .intersect (point))
							return OutlineCellContent::OUTPUT_CONNECTOR;
					}

					break;
				}
				case X3D::inputOutput:
				{
					{
						const X3D::Box2f box (X3D::Vector2f (INPUT_WIDTH, minimum_height), X3D::Vector2f (x + INPUT_WIDTH / 2, y + height / 2));

						if (box .intersect (point))
							return OutlineCellContent::INPUT;

						x += INPUT_WIDTH;
					}
					x += FIELD_PAD;
					{
						const X3D::Box2f box (X3D::Vector2f (OUTPUT_WIDTH, minimum_height), X3D::Vector2f (x + OUTPUT_WIDTH / 2, y + height / 2));

						if (box .intersect (point))
							return OutlineCellContent::OUTPUT;

						x += OUTPUT_WIDTH;
					}

					bool input = false;

					if (inputRoutes)
					{
						input = true;

						x += INPUT_PAD;

						const X3D::Box2f box (X3D::Vector2f (CONNECTOR_WIDTH, minimum_height), X3D::Vector2f (x + CONNECTOR_WIDTH / 2, y + height / 2));

						if (box .intersect (point))
							return OutlineCellContent::INPUT_CONNECTOR;

						x += CONNECTOR_WIDTH;
					}

					if (outputRoutes)
					{
						x += input ? INPUT_OUTPUT_PAD : OUTPUT_PAD;

						const X3D::Box2f box (X3D::Vector2f (CONNECTOR_WIDTH, minimum_height), X3D::Vector2f (x + CONNECTOR_WIDTH / 2, y + height / 2));

						if (box .intersect (point))
							return OutlineCellContent::OUTPUT_CONNECTOR;

						x += CONNECTOR_WIDTH;
					}

					break;
				}
			}

			break;
		}
		case OutlineIterType::X3DInputRoute:
		{
			const X3D::Box2f box (X3D::Vector2f (INPUT_WIDTH, minimum_height), X3D::Vector2f (x + INPUT_WIDTH / 2, y + height / 2));

			if (box .intersect (point))
				return OutlineCellContent::INPUT;

			x += INPUT_WIDTH;

			// Connector
			{
				x += INPUT_PAD;

				const X3D::Box2f box (X3D::Vector2f (CONNECTOR_WIDTH, minimum_height), X3D::Vector2f (x + CONNECTOR_WIDTH / 2, y + height / 2));

				if (box .intersect (point))
					return OutlineCellContent::INPUT_CONNECTOR;
			}

			break;
		}
		case OutlineIterType::X3DOutputRoute:
		{
			const X3D::Box2f box (X3D::Vector2f (OUTPUT_WIDTH, minimum_height), X3D::Vector2f (x + OUTPUT_WIDTH / 2, y + height / 2));

			if (box .intersect (point))
				return OutlineCellContent::OUTPUT;

			x += OUTPUT_WIDTH;

			// Connector
			{
				x += OUTPUT_PAD;

				const X3D::Box2f box (X3D::Vector2f (CONNECTOR_WIDTH, minimum_height), X3D::Vector2f (x + CONNECTOR_WIDTH / 2, y + height / 2));

				if (box .intersect (point))
					return OutlineCellContent::OUTPUT_CONNECTOR;
			}

			break;
		}
		default:
			break;
	}

	return OutlineCellContent::NONE;
}

void
OutlineCellRenderer::render_vfunc (const Cairo::RefPtr <Cairo::Context> & context,
                                   Gtk::Widget & widget,
                                   const Gdk::Rectangle & background_area,
                                   const Gdk::Rectangle & cell_area,
                                   Gtk::CellRendererState flags)
{
	int x              = cell_area .get_x ();
	int y              = cell_area .get_y ();
	int width          = cell_area .get_width ();
	int height         = cell_area .get_height ();
	int minimum_width  = 0;
	int natural_width  = 0;
	int minimum_height = 0;
	int natural_height = 0;

	const auto data_type = get_data_type ();

	if (data_type == OutlineIterType::Separator)
	{
		// Separator

		Gtk::CellRendererText::render_vfunc (context, widget, background_area, cell_area, flags);

		x += ICON_X_PAD;
	}
	else
	{
		// Icon

		{
			x += ICON_X_PAD;

			const Gdk::Rectangle cell_area (x, y, width, height);
			cellrenderer_icon .render (context, widget, background_area, cell_area, flags);
			cellrenderer_icon .get_preferred_width (widget, minimum_width, natural_width);

			x     += minimum_width;
			width -= minimum_width;
		}

		// Name or value

		{
			x     += NAME_X_PAD;
			width -= NAME_X_PAD;

			const Gdk::Rectangle cell_area (x, y, width, height);
			Gtk::CellRendererText::render_vfunc (context, widget, background_area, cell_area, flags);
			Gtk::CellRendererText::get_preferred_width_vfunc (widget, minimum_width, natural_width);

			x     += minimum_width;
			width -= minimum_width;
		}

		// Access type padding

		switch (data_type)
		{
			case OutlineIterType::X3DField:
			{
				if (is_full_expanded () and is_expanded ())
					break;
			}
			case OutlineIterType::X3DInputRoute:
			case OutlineIterType::X3DOutputRoute:
			{
				x     += ACCESS_TYPE_X_PAD;
				width -= ACCESS_TYPE_X_PAD;
				break;
			}
			default:
				break;
		}

		// Access type background

		cellrenderer_access_type_icon .get_preferred_height (widget, minimum_height, natural_height);

		double       field_x      = x;
		const double field_y      = y + (height - minimum_height) / 2;
		const double field_height = minimum_height;

		const auto      data     = property_data () .get_value ();
		const int       selected = data -> get_user_data () -> selected | property_cell_background_set ();
		const Gdk::RGBA color    = selected & OUTLINE_SELECTED ? property_foreground_rgba () : property_cell_background_rgba ();

		context -> reset_clip ();
		context -> set_operator (Cairo::OPERATOR_OVER);
		context -> set_source_rgba (color .get_red (), color .get_green (), color .get_blue (), color .get_alpha ());

		constexpr double radius = ACCESS_TYPE_RADIUS;

		switch (data_type)
		{
			case OutlineIterType::X3DField:
			{
				if (is_full_expanded () and is_expanded ())
					break;

				if (selected & (OUTLINE_OVER_INPUT | OUTLINE_SELECTED_INPUT))
				{
					context -> begin_new_sub_path ();
					context -> arc_negative (field_x + radius, field_y + radius, radius, M_PI3_2, M_PI1_2);
					context -> rel_line_to (FIELD_WIDTH - radius, 0);
					context -> rel_line_to (0, -field_height + 1);
		
					//context -> rectangle (field_x, field_y, FIELD_WIDTH, field_height);
					context -> fill ();
				}

				if (accessType == X3D::inputOutput)
				{
					field_x += INPUT_WIDTH + FIELD_PAD;
				}

				if (selected & (OUTLINE_OVER_OUTPUT | OUTLINE_SELECTED_OUTPUT))
				{
					context -> begin_new_sub_path ();
					context -> arc_negative (field_x + FIELD_WIDTH - radius - 1, field_y + radius, radius, M_PI1_2, M_PI3_2);
					context -> rel_line_to (-(FIELD_WIDTH - radius - 1), 0);
					context -> rel_line_to (0, field_height);
		
					//context -> rectangle (field_x, field_y, FIELD_WIDTH, field_height);
					context -> fill ();
				}

				break;
			}
			case OutlineIterType::X3DInputRoute:
			{
				if (selected & (OUTLINE_OVER_INPUT | OUTLINE_SELECTED_INPUT))
				{
					context -> begin_new_sub_path ();
					context -> arc_negative (field_x + radius, field_y + radius, radius, M_PI3_2, M_PI1_2);
					context -> rel_line_to (FIELD_WIDTH - radius, 0);
					context -> rel_line_to (0, -field_height + 1);
		
					//context -> rectangle (field_x, field_y, FIELD_WIDTH, field_height);
					context -> fill ();
				}

				break;
			}
			case OutlineIterType::X3DOutputRoute:
			{
				if (selected & (OUTLINE_OVER_OUTPUT | OUTLINE_SELECTED_OUTPUT))
				{
					context -> begin_new_sub_path ();
					context -> arc_negative (field_x + FIELD_WIDTH - radius - 1, field_y + radius, radius, M_PI1_2, M_PI3_2);
					context -> rel_line_to (-(FIELD_WIDTH - radius - 1), 0);
					context -> rel_line_to (0, field_height);
		
					//context -> rectangle (field_x, field_y, FIELD_WIDTH, field_height);
					context -> fill ();
				}

				break;
			}
			default:
				break;
		}

		// Access type

		switch (data_type)
		{
			case OutlineIterType::X3DField:
			{
				if (is_full_expanded () and is_expanded ())
					break;
			}
			case OutlineIterType::X3DInputRoute:
			case OutlineIterType::X3DOutputRoute:
			{
				Gdk::Rectangle cell_area (x, y, width, height);
				cellrenderer_access_type_icon .render (context, widget, background_area, cell_area, flags);
				cellrenderer_access_type_icon .get_preferred_width (widget, minimum_width, natural_width);

				x     += minimum_width;
				width -= minimum_width;
				break;
			}
			default:
				break;
		}
	}

	// Routes

	{
		Gdk::Rectangle cell_area (x, y, width - RIGHT_PAD, height);
		render_routes (context, widget, background_area, cell_area, minimum_height, flags);
	}
}

void
OutlineCellRenderer::render_routes (const Cairo::RefPtr <Cairo::Context> & context,
                                    Gtk::Widget & widget,
                                    const Gdk::Rectangle & background_area,
                                    const Gdk::Rectangle & cell_area,
                                    int minimum_height,
                                    Gtk::CellRendererState flags)
{
	const int x      = cell_area .get_x ();
	const int y      = background_area .get_y ();
	const int width  = cell_area .get_width () - ROUTE_CURVE_WIDTH;
	const int height = background_area .get_height ();
	const int y_pad  = (height - minimum_height) / 2;

	double       input_x  = x;
	const double input_y  = y + y_pad + ROUTE_Y_PAD;
	double       input_w  = width;
	const double output_x = x;
	const double output_y = y + y_pad + minimum_height - ROUTE_Y_PAD;
	const double output_w = width;

	const double radius      = ROUTE_RADIUS;
	const double connector_x = x + width + radius;

	if (accessType == X3D::inputOutput)
	{
		input_x -= ROUTE_INPUT_PAD;
		input_w += ROUTE_INPUT_PAD;
	}

	const auto data = property_data () .get_value ();

	const auto foregroundColor         = widget .get_style_context () -> get_color (Gtk::STATE_FLAG_NORMAL);
	const auto selectedForegroundColor = widget .get_style_context () -> get_color (Gtk::STATE_FLAG_SELECTED);

	const int  selected      = data -> get_user_data () -> selected | property_cell_background_set ();
	const auto color         = selected & OUTLINE_SELECTED ? selectedForegroundColor : foregroundColor;
	const auto selectedColor = selected & OUTLINE_SELECTED ? selectedForegroundColor : property_cell_background_rgba () .get_value ();

	//	context -> set_source_rgb (0.9, 0.9, 0.9);
	//	context -> rectangle (x, y , width, height);
	//	context -> fill ();

	context -> reset_clip ();
	context -> set_operator (Cairo::OPERATOR_OVER);
	context -> set_line_width (1);

	// Input

	if (not data -> get_inputs_above () .empty () or not data -> get_inputs_below () .empty ())
	{
		const auto selected_above = have_selected_routes (data -> get_inputs_above ());
		const auto selected_below = have_selected_routes (data -> get_inputs_below ());

		const auto c = selected_above or selected_below ? selectedColor : foregroundColor;
		context -> set_source_rgba (c .get_red (), c .get_green (), c .get_blue (), c .get_alpha ());

		context -> move_to (input_x, input_y);
		context -> line_to (input_x + input_w, input_y);
		context -> stroke ();

		// Arc up

		if (not data -> get_inputs_above () .empty ())
		{
			const auto c = selected_above ? selectedColor : foregroundColor;
			context -> set_source_rgba (c .get_red (), c .get_green (), c .get_blue (), c .get_alpha ());

			context -> begin_new_sub_path ();
			context -> arc (input_x + input_w, input_y - radius, radius, 0, M_PI1_2);

			context -> move_to (connector_x, y);
			context -> line_to (connector_x, input_y - radius);
			context -> stroke ();
		}

		// Arc down

		if (not data -> get_inputs_below () .empty ())
		{
			const auto c = selected_below ? selectedColor : foregroundColor;
			context -> set_source_rgba (c .get_red (), c .get_green (), c .get_blue (), c .get_alpha ());

			context -> begin_new_sub_path ();
			context -> arc (input_x + input_w, input_y + radius, radius, M_PI3_2, M_PI2);

			context -> move_to (connector_x, input_y + radius);
			context -> line_to (connector_x, y + height);
			context -> stroke ();
		}
	}

	// Output

	if (not data -> get_outputs_above () .empty () or not data -> get_outputs_below () .empty ())
	{
		const auto selected_above = have_selected_routes (data -> get_outputs_above ());
		const auto selected_below = have_selected_routes (data -> get_outputs_below ());

		const auto c = selected_above or selected_below ? selectedColor : foregroundColor;
		context -> set_source_rgba (c .get_red (), c .get_green (), c .get_blue (), c .get_alpha ());

		context -> move_to (output_x, output_y);
		context -> line_to (output_x + output_w, output_y);
		context -> stroke ();

		// Arc up

		if (not data -> get_outputs_above () .empty ())
		{
			const auto c = selected_above ? selectedColor : foregroundColor;
			context -> set_source_rgba (c .get_red (), c .get_green (), c .get_blue (), c .get_alpha ());

			context -> begin_new_sub_path ();
			context -> arc (output_x + output_w, output_y - radius, radius, 0, M_PI1_2);

			context -> move_to (connector_x, y);
			context -> line_to (connector_x, output_y - radius);
			context -> stroke ();
		}

		// Arc down

		if (not data -> get_outputs_below () .empty ())
		{
			const auto c = selected_below ? selectedColor : foregroundColor;
			context -> set_source_rgba (c .get_red (), c .get_green (), c .get_blue (), c .get_alpha ());

			context -> begin_new_sub_path ();
			context -> arc (output_x + output_w, output_y + radius, radius, M_PI3_2, M_PI2);

			context -> move_to (connector_x, output_y + radius);
			context -> line_to (connector_x, y + height);
			context -> stroke ();
		}
	}

	// Self Connection

	if (data -> get_self_connection ())
	{
		const double cy     = (input_y + output_y) / 2;
		const double radius = (input_y + output_y) / 2 - input_y;

		context -> set_source_rgba (color .get_red (), color .get_green (), color .get_blue (), color .get_alpha ());

		context -> move_to (input_x, input_y);
		context -> line_to (output_x, input_y);

		context -> begin_new_sub_path ();
		context -> arc (output_x, cy, radius, M_PI3_2, M_PI1_2);
		context -> stroke ();
	}

	// Connections

	if (not data -> get_connections () .empty ())
	{
		const auto c = have_selected_routes (data -> get_connections ()) ? selectedColor : foregroundColor;
		context -> set_source_rgba (c .get_red (), c .get_green (), c .get_blue (), c .get_alpha ());

		context -> move_to (connector_x, y);
		context -> line_to (connector_x, y + height);
		context -> stroke ();
	}
}

bool
OutlineCellRenderer::have_selected_routes (const OutlineRoutes & set)
{
	for (const auto & route : routes)
	{
		if (set .count (route))
			return true;
	}

	return false;
}

} // puck
} // titania
