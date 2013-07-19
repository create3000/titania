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

#include "../../Configuration/config.h"

namespace titania {
namespace puck {

constexpr int NAME_PAD_X        = 1;
constexpr int ACCESS_TYPE_PAD_X = 8;

OutlineCellRenderer::OutlineCellRenderer (const X3D::X3DSFNode <X3D::Browser> & browser) :
	             Glib::ObjectBase (typeid (OutlineCellRenderer)),
	        Gtk::CellRendererText (),
	                data_property (*this, "tree-data", nullptr),
	            cellrenderer_icon (),
	cellrenderer_access_type_icon (),
	                    noneImage (),
	                baseNodeImage (),
	              fieldTypeImages (),
	             accessTypeImages (),
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

	data_property .get_proxy () .signal_changed () .connect (sigc::mem_fun (this, &OutlineCellRenderer::set_data));
}

void
OutlineCellRenderer::set_data ()
{
	cellrenderer_icon .property_pixbuf () = get_icon ();

	switch (get_data_type ())
	{
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
			cellrenderer_access_type_icon .property_pixbuf () = get_access_type_icon ();
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

const Glib::RefPtr <Gdk::Pixbuf> &
OutlineCellRenderer::get_icon () const
{
	switch (get_data_type ())
	{
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
OutlineCellRenderer::get_access_type_icon () const
{
	auto   field = static_cast <X3D::X3DFieldDefinition*> (get_object ());
	auto   iter  = accessTypeImages .find (field -> getAccessType ());
	size_t index = 0;

	switch (field -> getAccessType ())
	{
		case X3D::initializeOnly:
			break;
		case X3D::inputOnly:
			index = std::min <size_t> (field -> getInputRoutes () .size (), 2);
			break;
		case X3D::outputOnly:
			index = std::min <size_t> (field -> getOutputRoutes () .size (), 2);
			break;
		case X3D::inputOutput:
			index = (std::min <size_t> (field -> getInputRoutes () .size (), 2) << 2) | (std::min <size_t> (field -> getOutputRoutes () .size (), 2));
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

		X3D::RegEx::_Number .Replace ("", &name);

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
		Gtk::CellRendererText::get_preferred_width_vfunc (widget, minimum, natural);
		minimum_width += minimum;
		natural_width += natural;
	}

	// Access type and routes

	if (get_data_type () == OutlineIterType::X3DField)
	{
		cellrenderer_access_type_icon .get_preferred_width (widget, minimum, natural);
		minimum_width += minimum;
		natural_width += natural;
	}

	// Padding

	minimum_width += NAME_PAD_X;
	natural_width += NAME_PAD_X;

	minimum_width += ACCESS_TYPE_PAD_X;
	natural_width += ACCESS_TYPE_PAD_X;
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

	if (get_data_type () == OutlineIterType::X3DField)
	{
		cellrenderer_access_type_icon .get_preferred_height_for_width (widget, width, minimum, natural);
		minimum_height = std::max (minimum, minimum_height);
		natural_height = std::max (natural, natural_height);
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

	if (get_data_type () == OutlineIterType::X3DField)
	{
		cellrenderer_access_type_icon .get_preferred_height (widget, minimum, natural);
		minimum_height = std::max (minimum, minimum_height);
		natural_height = std::max (natural, natural_height);
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
		Gtk::CellRendererText::get_preferred_width_for_height_vfunc (widget, height, minimum, natural);
		minimum_width += minimum;
		natural_width += natural;
	}

	// Access type and routes

	if (get_data_type () == OutlineIterType::X3DField)
	{
		cellrenderer_access_type_icon .get_preferred_width_for_height (widget, height, minimum, natural);
		minimum_width += minimum;
		natural_width += natural;
	}

	// Padding

	minimum_width += NAME_PAD_X;
	natural_width += NAME_PAD_X;

	minimum_width += ACCESS_TYPE_PAD_X;
	natural_width += ACCESS_TYPE_PAD_X;
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

	int x_pad = icon_width + NAME_PAD_X + property_xpad ();
	
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
	if (textview -> property_editing_canceled ())
		return;

	OutlineTreeData* data = textview -> get_data ();
	
	switch (data -> get_type ())
	{
		case OutlineIterType::X3DFieldValue:
		{
			std::string string = textview -> get_text ();
			
			if (set_field_value (data -> get_object (), string))
			{
				textview -> remove_widget ();
				edited (textview -> get_path (), string);
			}

			break;
		}
		case OutlineIterType::X3DField:
		case OutlineIterType::X3DBaseNode:
			break;
	}
}

bool
OutlineCellRenderer::set_field_value (X3D::X3DChildObject* const object, const std::string & string)
{
	auto field = static_cast <X3D::X3DFieldDefinition*> (object);

	if (field -> isArray ())
		return field -> fromString ("[" + string + "]");

	return field -> fromString (string);
}

void
OutlineCellRenderer::render_vfunc (const Cairo::RefPtr <Cairo::Context> & context,
                                   Gtk::Widget & widget,
                                   const Gdk::Rectangle & background_area,
                                   const Gdk::Rectangle & cell_area,
                                   Gtk::CellRendererState flags)
{
	int x             = cell_area .get_x ();
	int y             = cell_area .get_y ();
	int width         = cell_area .get_width ();
	int height        = cell_area .get_height ();
	int minimum_width = 0;
	int natural_width = 0;

	// Icon

	{
		cellrenderer_icon .render (context, widget, background_area, cell_area, flags);
		cellrenderer_icon .get_preferred_width (widget, minimum_width, natural_width);

		x     += minimum_width;
		width -= minimum_width;
	}

	// Name or value

	{
		x     += NAME_PAD_X;
		width -= NAME_PAD_X;

		Gdk::Rectangle cell_area (x, y, width, height);
		Gtk::CellRendererText::render_vfunc (context, widget, background_area, cell_area, flags);
		Gtk::CellRendererText::get_preferred_width_vfunc (widget, minimum_width, natural_width);

		x     += minimum_width;
		width -= minimum_width;
	}

	// Access type and routes

	if (get_data_type () == OutlineIterType::X3DField)
	{
		x     += ACCESS_TYPE_PAD_X;
		width -= ACCESS_TYPE_PAD_X;

		Gdk::Rectangle cell_area (x, y, width, height);
		cellrenderer_access_type_icon .render (context, widget, background_area, cell_area, flags);
	}
}

} // puck
} // titania
