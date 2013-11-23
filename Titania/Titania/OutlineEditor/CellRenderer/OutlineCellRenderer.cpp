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

#include "OutlineCellRenderer.h"

#include "../../Browser/BrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Undo/UndoStep.h"
#include "../OutlineTreeModel.h"
#include "../X3DOutlineTreeView.h"

namespace titania {
namespace puck {

constexpr double M_PI3_2 = 1.5 * M_PI;

constexpr int    NAME_X_PAD        = 1;
constexpr int    ACCESS_TYPE_X_PAD = 8;
constexpr int    ROUTE_WIDTH       = 16;
constexpr int    ROUTE_CURVE_WIDTH = 16;
constexpr double ROUTE_RADIUS      = 8.5;
constexpr int    RIGHT_PAD         = 8;
constexpr double ROUTE_INPUT_PAD   = 14;
constexpr double ROUTE_Y_PAD       = 3.5; // Depends on image

OutlineCellRenderer::OutlineCellRenderer (X3D::X3DBrowser* const browser, X3DOutlineTreeView* const treeView) :
	             Glib::ObjectBase (typeid (OutlineCellRenderer)),
	        Gtk::CellRendererText (),
	                     treeView (treeView),
	                data_property (*this, "tree-data", nullptr),
	            cellrenderer_icon (),
	cellrenderer_access_type_icon (),
	                    noneImage (),
	                baseNodeImage (),
	              fieldTypeImages (),
	             accessTypeImages (),
	                   accessType (),
	                     textview ()
{
	// Images

	noneImage     = Gdk::Pixbuf::create_from_file (puck::get_icon ("none.png"));
	baseNodeImage = Gdk::Pixbuf::create_from_file (puck::get_icon ("Node.png"));

	for (const auto & field : browser -> getSupportedFields ())
		fieldTypeImages [field -> getType ()] = Gdk::Pixbuf::create_from_file (puck::get_icon (field -> getTypeName () + ".png"));

	accessTypeImages [X3D::initializeOnly] .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("initializeOnly.png")));
	accessTypeImages [X3D::inputOnly]      .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOnly.0.png")));
	accessTypeImages [X3D::inputOnly]      .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOnly.1.png")));
	accessTypeImages [X3D::inputOnly]      .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOnly.2.png")));
	accessTypeImages [X3D::outputOnly]     .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("outputOnly.0.png")));
	accessTypeImages [X3D::outputOnly]     .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("outputOnly.1.png")));
	accessTypeImages [X3D::outputOnly]     .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("outputOnly.2.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.0.0.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.0.1.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.0.2.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (accessTypeImages [X3D::inputOutput] .back ());
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.1.0.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.1.1.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.1.2.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (accessTypeImages [X3D::inputOutput] .back ());
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.2.0.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.2.1.png")));
	accessTypeImages [X3D::inputOutput]    .emplace_back (Gdk::Pixbuf::create_from_file (puck::get_icon ("inputOutput.2.2.png")));

	// CellRendererPixbuf

	cellrenderer_icon             .set_alignment (0, 0.5);
	cellrenderer_access_type_icon .set_alignment (0, 0.5);

	// Signals

	data_property .get_proxy () .signal_changed () .connect (sigc::mem_fun (this, &OutlineCellRenderer::on_data));
}

void
OutlineCellRenderer::on_data ()
{
	cellrenderer_icon .property_pixbuf () = get_icon ();

	switch (get_data_type ())
	{
		case OutlineIterType::X3DInputRoute:
		{
			auto route = static_cast <X3D::Route*> (get_object ());

			const std::string name = route -> getSourceNode () -> getName () .size ()
			                         ? route -> getSourceNode () -> getName ()
											 : _ ("<unnamed>");

			property_editable ()                              = false;
			property_markup ()                                = _ ("Route from ") + name + "." + route -> getSourceField ();
			cellrenderer_access_type_icon .property_pixbuf () = accessTypeImages [X3D::inputOnly] [1];
			accessType                                        = X3D::inputOnly;
			set_alignment (0, 0.5);
			break;
		}
		case OutlineIterType::X3DOutputRoute:
		{
			auto route = static_cast <X3D::Route*> (get_object ());

			const std::string name = route -> getDestinationNode () -> getName () .size ()
			                         ? route -> getDestinationNode () -> getName ()
											 : _ ("<unnamed>");

			property_editable ()                              = false;
			property_markup ()                                = _ ("Route to ") + name + "." + route -> getDestinationField ();
			cellrenderer_access_type_icon .property_pixbuf () = accessTypeImages [X3D::outputOnly] [1];
			accessType                                        = X3D::outputOnly;
			set_alignment (0, 0.5);
			break;
		}
		case OutlineIterType::X3DFieldValue:
		{
			property_editable () = true;
			property_text ()     = get_field_value ();
			set_alignment (0, 0);
			break;
		}
		case OutlineIterType::X3DField:
		{
			property_editable ()                              = false;
			property_text ()                                  = get_object () -> getName ();
			cellrenderer_access_type_icon .property_pixbuf () = get_access_type_icon (accessType);
			set_alignment (0, 0.5);
			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			property_editable () = false;
			property_markup ()   = get_node_name ();
			set_alignment (0, 0.5);
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
OutlineCellRenderer::get_expanded () const
{
	auto userData = property_data () .get_value () -> get_user_data ();

	return userData -> expanded;
}

bool
OutlineCellRenderer::get_all_expanded () const
{
	auto userData = property_data () .get_value () -> get_user_data ();

	return userData -> all_expanded;
}

const Glib::RefPtr <Gdk::Pixbuf> &
OutlineCellRenderer::get_icon () const
{
	switch (get_data_type ())
	{
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::X3DFieldValue:
		{
			return noneImage;
		}
		case OutlineIterType::X3DField:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (get_object ());
			auto iter  = fieldTypeImages .find (field -> getType ());

			if (iter not_eq fieldTypeImages .end ())
				return iter -> second;

			return noneImage;
		}
		case OutlineIterType::X3DBaseNode:
		{
			return baseNodeImage;
		}
	}

	return noneImage;
}

const Glib::RefPtr <Gdk::Pixbuf> &
OutlineCellRenderer::get_access_type_icon (X3D::AccessType & accessType) const
{
	auto   field = static_cast <X3D::X3DFieldDefinition*> (get_object ());
	auto   iter  = accessTypeImages .find (field -> getAccessType ());
	size_t index = 0;

	size_t inputRoutes  = treeView -> get_model () -> get_input_routes (field);
	size_t outputRoutes = treeView -> get_model () -> get_output_routes (field);

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
OutlineCellRenderer::get_node_name () const
{
	auto sfnode = static_cast <X3D::SFNode*> (get_object ());

	if (*sfnode)
	{
		X3D::X3DBaseNode* node = sfnode -> getValue ();

		std::string typeName  = Glib::Markup::escape_text (node -> getTypeName ());
		std::string name      = Glib::Markup::escape_text (node -> getName ());
		size_t      numClones = node -> getNumClones ();

		X3D::RegEx::_LastNumber .Replace ("", &name);

		std::string string = "<b>" + typeName + "</b> " + name;

		if (numClones > 1)
			string += " [" + std::to_string (numClones) + "]";

		return string;
	}

	return "<b>NULL</b>";
}

template <class Type>
static
std::string
array_to_string (const Type & array)
{
	std::ostringstream stream;

	if (array .empty ())
		return stream .str ();

	for (const auto & value : basic::adapter (array .begin (), array .end () - 1))
	{
		value .toStream (stream);
		stream << std::endl;
	}

	array .back () .toStream (stream);

	return stream .str ();
}

std::string
OutlineCellRenderer::get_field_value () const
{
	auto field = static_cast <X3D::X3DFieldDefinition*> (get_object ());

	X3D::Generator::NicestStyle ();

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFNode:
			return "";

		case X3D::X3DConstants::MFBool:
			return array_to_string (*static_cast <X3D::MFBool*> (field));

		case X3D::X3DConstants::MFColor:
			return array_to_string (*static_cast <X3D::MFColor*> (field));

		case X3D::X3DConstants::MFColorRGBA:
			return array_to_string (*static_cast <X3D::MFColorRGBA*> (field));

		case X3D::X3DConstants::MFDouble:
			return array_to_string (*static_cast <X3D::MFDouble*> (field));

		case X3D::X3DConstants::MFFloat:
			return array_to_string (*static_cast <X3D::MFFloat*> (field));

		case X3D::X3DConstants::MFImage:
			return array_to_string (*static_cast <X3D::MFImage*> (field));

		case X3D::X3DConstants::MFInt32:
			return array_to_string (*static_cast <X3D::MFInt32*> (field));

		case X3D::X3DConstants::MFMatrix3d:
			return array_to_string (*static_cast <X3D::MFMatrix3d*> (field));

		case X3D::X3DConstants::MFMatrix3f:
			return array_to_string (*static_cast <X3D::MFMatrix3f*> (field));

		case X3D::X3DConstants::MFMatrix4d:
			return array_to_string (*static_cast <X3D::MFMatrix4d*> (field));

		case X3D::X3DConstants::MFMatrix4f:
			return array_to_string (*static_cast <X3D::MFMatrix4f*> (field));

		case X3D::X3DConstants::MFNode:
			return "";

		case X3D::X3DConstants::MFRotation:
			return array_to_string (*static_cast <X3D::MFRotation*> (field));

		case X3D::X3DConstants::MFString:
			return array_to_string (*static_cast <X3D::MFString*> (field));

		case X3D::X3DConstants::MFTime:
			return array_to_string (*static_cast <X3D::MFTime*> (field));

		case X3D::X3DConstants::MFVec2d:
			return array_to_string (*static_cast <X3D::MFVec2d*> (field));

		case X3D::X3DConstants::MFVec2f:
			return array_to_string (*static_cast <X3D::MFVec2f*> (field));

		case X3D::X3DConstants::MFVec3d:
			return array_to_string (*static_cast <X3D::MFVec3d*> (field));

		case X3D::X3DConstants::MFVec3f:
			return array_to_string (*static_cast <X3D::MFVec3f*> (field));

		case X3D::X3DConstants::MFVec4d:
			return array_to_string (*static_cast <X3D::MFVec4d*> (field));

		case X3D::X3DConstants::MFVec4f:
			return array_to_string (*static_cast <X3D::MFVec4f*> (field));

		default:
			return field -> toString ();
	}
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
			if (get_all_expanded () and get_expanded ())
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
	minimum_height = 0;
	natural_height = 0;

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
			if (get_all_expanded () and get_expanded ())
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
	minimum_height = 0;
	natural_height = 0;

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
			if (get_all_expanded () and get_expanded ())
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
			if (get_all_expanded () and get_expanded ())
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
	int icon_width    = 0;
	int natural_width = 0;

	cellrenderer_icon .get_preferred_width (widget, icon_width, natural_width);

	int x_pad = icon_width + NAME_X_PAD + property_xpad ();

	switch (get_data_type ())
	{
		case OutlineIterType::X3DFieldValue:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (get_object ());

			textview .reset (new TextViewEditable (property_data (), path, field -> isArray () or dynamic_cast <X3D::SFString*> (field)));
			textview -> set_text (property_text ());
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

	OutlineTreeData* data = textview -> get_data ();

	switch (data -> get_type ())
	{
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::X3DField:
			break;
		case OutlineIterType::X3DFieldValue:
		{
			std::string string = textview -> get_text ();

			if (set_field_value (data -> get_object (), string, data -> get_path ()))
			{
				textview -> set_validated (true);
				textview -> remove_widget ();
				edited (textview -> get_path (), string);
			}

			break;
		}
		case OutlineIterType::X3DBaseNode:
			break;
	}
}

bool
OutlineCellRenderer::set_field_value (X3D::X3DChildObject* const object, const std::string & string, Gtk::TreeModel::Path path)
{
	path .up ();
	path .up ();

	auto parent = treeView -> get_model () -> get_iter (path);
	auto node   = static_cast <X3D::SFNode*> (treeView -> get_object (parent));
	auto field  = static_cast <X3D::X3DFieldDefinition*> (object);

	if (field -> isArray ())
		return set_field_value (field, "[" + string + "]", *node);

	return set_field_value (field, string, *node);
}

bool
OutlineCellRenderer::set_field_value (X3D::X3DFieldDefinition* const field, const std::string & string, const X3D::SFNode & node)
{
	std::string value = field -> toString ();

	if (field -> fromString (string))
	{
		if (field -> toString () not_eq value)
		{
			auto undoStep = std::make_shared <UndoStep> (_ ("Edit Field Value"));

			undoStep -> addVariables (node);

			undoStep -> addUndoFunction (std::mem_fn (&X3D::X3DFieldDefinition::fromString), field, value);
			undoStep -> addRedoFunction (std::mem_fn (&X3D::X3DFieldDefinition::fromString), field, string);

			treeView -> getBrowserWindow () -> addUndoStep (undoStep);
		}

		return true;
	}

	return false;
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

	// Icon

	{
		cellrenderer_icon .render (context, widget, background_area, cell_area, flags);
		cellrenderer_icon .get_preferred_width (widget, minimum_width, natural_width);

		x     += minimum_width;
		width -= minimum_width;
	}

	// Name or value

	{
		x     += NAME_X_PAD;
		width -= NAME_X_PAD;

		Gdk::Rectangle cell_area (x, y, width, height);
		Gtk::CellRendererText::render_vfunc (context, widget, background_area, cell_area, flags);
		Gtk::CellRendererText::get_preferred_width_vfunc (widget, minimum_width, natural_width);

		x     += minimum_width;
		width -= minimum_width;
	}

	// Access type and routes

	switch (get_data_type ())
	{
		case OutlineIterType::X3DField:
		{
			if (get_all_expanded () and get_expanded ())
				break;
		}
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		{
			x     += ACCESS_TYPE_X_PAD;
			width -= ACCESS_TYPE_X_PAD;

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

	// Routes

	{
		cellrenderer_access_type_icon .get_preferred_height (widget, minimum_height, natural_height);

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
	int x      = cell_area .get_x ();
	int y      = cell_area .get_y ();
	int width  = cell_area .get_width () - ROUTE_CURVE_WIDTH;
	int height = cell_area .get_height ();
	int y_pad  = (height - minimum_height) / 2;

	double input_x  = x;
	double input_y  = y + y_pad + ROUTE_Y_PAD;
	double input_w  = width;
	double output_x = x;
	double output_y = y + y_pad + minimum_height - ROUTE_Y_PAD;
	double output_w = width;
	
	double radius      = std::min (y_pad + ROUTE_Y_PAD, ROUTE_RADIUS);
	double connector_x = x + width + radius;

	if (accessType == X3D::inputOutput)
	{
		input_x -= ROUTE_INPUT_PAD;
		input_w += ROUTE_INPUT_PAD;
	}

	//	context -> set_source_rgb (0.9, 0.9, 0.9);
	//	context -> rectangle (x, y + 1, width, height - 2);
	//	context -> fill ();

	context -> reset_clip ();
	context -> set_operator (Cairo::OPERATOR_OVER);
	context -> set_source_rgb (0, 0, 0);
	context -> set_line_width (1);

	auto data = property_data () .get_value ();

	if (not data -> get_inputs_above () .empty () or not data -> get_inputs_below () .empty ())
	{
		context -> move_to (input_x, input_y);
		context -> line_to (input_x + input_w, input_y);
		
		// Arc up
		
		if (not data -> get_inputs_above () .empty ())
		{
			context -> begin_new_sub_path ();
			context -> arc (input_x + input_w, input_y - radius, radius, 0, M_PI1_2);
			
			context -> move_to (connector_x, y);
			context -> line_to (connector_x, input_y - radius);
		}

		// Arc down

		if (not data -> get_inputs_below () .empty ())
		{
			context -> begin_new_sub_path ();
			context -> arc (input_x + input_w, input_y + radius, radius, M_PI3_2, M_PI2);
			
			context -> move_to (connector_x, input_y + radius);
			context -> line_to (connector_x, y + height);
		}
	}

	if (not data -> get_outputs_above () .empty () or not data -> get_outputs_below () .empty ())
	{
		context -> move_to (output_x, output_y);
		context -> line_to (output_x + output_w, output_y);
		
		// Arc up
		
		if (not data -> get_outputs_above () .empty ())
		{
			context -> begin_new_sub_path ();
			context -> arc (output_x + output_w, output_y - radius, radius, 0, M_PI1_2);
			
			context -> move_to (connector_x, y);
			context -> line_to (connector_x, output_y - radius);
		}

		// Arc down

		if (not data -> get_outputs_below () .empty ())
		{
			context -> begin_new_sub_path ();
			context -> arc (output_x + output_w, output_y + radius, radius, M_PI3_2, M_PI2);
			
			context -> move_to (connector_x, output_y + radius);
			context -> line_to (connector_x, y + height);
		}
	}

	if (data -> get_self_connection ())
	{
		double cy     = (input_y + output_y) / 2;
		double radius = (input_y + output_y) / 2 - input_y;

		context -> move_to (input_x, input_y);
		context -> line_to (output_x, input_y);

		context -> begin_new_sub_path ();
		context -> arc (output_x, cy, radius, M_PI3_2, M_PI1_2);
	}

	// Connections

	if (not data -> get_connections () .empty ())
	{
		context -> move_to (connector_x, y);
		context -> line_to (connector_x, y + height);
	}

	context -> stroke ();
}

} // puck
} // titania
