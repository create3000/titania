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

#include "Parser.h"

#include <libxml++/libxml++.h>

#include "../../Components/Geometry2D/Rectangle2D.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../Filter.h"

#include <Titania/InputOutput.h>
#include <Titania/InputOutput/Hex.h>
#include <Titania/InputOutput/Number.h>
#include <Titania/Math/Constants.h>
#include <Titania/String.h>

namespace titania {
namespace X3D {
namespace SVG {

class Grammar
{
public:

	static const io::string matrix;

	static const io::character OpenParenthesis;
	static const io::character CloseParenthesis;
	static const io::character Comma;
	static const io::character NumberSign;

	static const io::string em;
	static const io::string ex;
	static const io::string px;
	static const io::string in;
	static const io::string cm;
	static const io::string mm;
	static const io::string pt;
	static const io::string pc;
	static const io::character Percent;

	static const io::number <double> DoubleValue;
	static const io::hex <int32_t>   HexValue;
};

const io::string Grammar::matrix ("matrix");

const io::character Grammar::OpenParenthesis ('(');
const io::character Grammar::CloseParenthesis (')');
const io::character Grammar::Comma (',');
const io::character Grammar::NumberSign ('#');

const io::string    Grammar::em ("em");
const io::string    Grammar::ex ("ex");
const io::string    Grammar::px ("px");
const io::string    Grammar::in ("in");
const io::string    Grammar::cm ("cm");
const io::string    Grammar::mm ("mm");
const io::string    Grammar::pt ("pt");
const io::string    Grammar::pc ("pc");
const io::character Grammar::Percent ('%');

const io::number <double> Grammar::DoubleValue;
const io::hex <int32_t>   Grammar::HexValue;

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	X3D::X3DParser (),
	         scene (scene),
	           uri (uri),
	       istream (istream),
	     xmlParser (new xmlpp::DomParser ()),
	        groups ({ scene -> createNode <X3D::Transform> () })
{
	xmlParser -> set_throw_messages (true);
	//xmlParser -> set_validate (true);
	xmlParser -> set_include_default_attributes (true);

	scene -> updateNamedNode (get_name_from_uri (uri), X3D::SFNode (groups .back ()));
	scene -> getRootNodes () .emplace_back (groups .back ());
}

void
Parser::parseIntoScene ()
{
	__LOG__ << this << " " << std::endl;

	try
	{
		scene -> setWorldURL (uri);

		xmlParser -> parse_stream (istream);

		const auto xmlDocument = xmlParser -> get_document ();

		if (xmlDocument)
			svgElement (xmlDocument -> get_root_node ());
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		throw;
	}
	catch (const std::exception & error)
	{
		throw X3D::Error <X3D::INVALID_X3D> (error .what ()); 
	}
}

void
Parser::svgElement (xmlpp::Element* const xmlElement)
{
	if (not xmlElement)
		return;

	X3D::Vector4d viewBox;
	double        width  = 0;
	double        height = 0;

	viewBoxAttribute (xmlElement -> get_attribute ("viewBox"), viewBox);
	lengthAttribute  (xmlElement -> get_attribute ("width"),   width);
	lengthAttribute  (xmlElement -> get_attribute ("height"),  height);

	__LOG__ << viewBox << std::endl;
	__LOG__ << width << std::endl;
	__LOG__ << height << std::endl;

	const auto transform   = groups .front ();
	const auto translation = X3D::Vector3d (-viewBox .x (), viewBox .y (), 0);
	const auto scale       = X3D::Vector3d (math::pixel <double> * width / viewBox [2], math::pixel <double> * height / viewBox [3], 1);

	transform -> translation () = translation * scale;
	transform -> scale ()       = scale;

	elements (xmlElement);
}

void
Parser::elements (xmlpp::Element* const xmlElement)
{
	if (not xmlElement)
		return;

	for (const auto & xmlNode : xmlElement -> get_children ())
		element (dynamic_cast <xmlpp::Element*> (xmlNode));
}

void
Parser::element (xmlpp::Element* const xmlElement)
{
	using ElementsFunction = std::function <void (Parser*, xmlpp::Element* const)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("g",       std::mem_fn (&Parser::groupElement)),
		std::make_pair ("rect",    std::mem_fn (&Parser::rectangleElement)),
		std::make_pair ("ellipse", std::mem_fn (&Parser::ellipseElement)),
		std::make_pair ("path",    std::mem_fn (&Parser::pathElement)),
	};

	if (not xmlElement)
		return;

	try
	{
		elementsIndex .at (xmlElement -> get_name ()) (this, xmlElement);
	}
	catch (const std::out_of_range &)
	{ }
}

void
Parser::groupElement (xmlpp::Element* const xmlElement)
{
	__LOG__ << xmlElement -> get_name () << std::endl;

	Matrix3d matrix;

	transformAttribute (xmlElement -> get_attribute ("transform"), matrix);

	__LOG__ << matrix << std::endl;

	const auto transform = scene -> createNode <X3D::Transform> ();

	transform -> setMatrix (X3D::Matrix4d (matrix [0] [0], matrix [0] [1], 0, 0,
	                                       matrix [1] [0], matrix [1] [1], 0, 0,
	                                       0, 0, 1, 0,
	                                       matrix [2] [0], -matrix [2] [1], 0, 1));

	groups .back () -> children () .emplace_back (transform);

	groups .emplace_back (transform);

	elements (xmlElement);

	groups .pop_back ();
}

void
Parser::rectangleElement (xmlpp::Element* const xmlElement)
{
	__LOG__ << xmlElement -> get_name () << std::endl;

	// Determine matrix.	

	X3D::Matrix3d matrix;

	double x      = 0;
	double y      = 0;
	double width  = 0;
	double height = 0;

	transformAttribute (xmlElement -> get_attribute ("transform"), matrix);

	lengthAttribute (xmlElement -> get_attribute ("x"),      x);
	lengthAttribute (xmlElement -> get_attribute ("y"),      y);
	lengthAttribute (xmlElement -> get_attribute ("width"),  width);
	lengthAttribute (xmlElement -> get_attribute ("height"), height);

	matrix .translate (X3D::Vector2d (x + width / 2, y + height / 2));

	// Determine material.

	Style style;

	styleAttribute (xmlElement -> get_attribute ("style"), style);

	__LOG__ << style .fill << std::endl;

	// Create nodes.

	const auto transform  = scene -> createNode <X3D::Transform> ();
	const auto shape      = scene -> createNode <X3D::Shape> ();
	const auto appearance = scene -> createNode <X3D::Appearance> ();
	const auto material   = scene -> createNode <X3D::Material> ();
	const auto rectangle  = scene -> createNode <X3D::Rectangle2D> ();

	transform -> setMatrix (X3D::Matrix4d (matrix [0] [0], matrix [0] [1], 0, 0,
	                                       matrix [1] [0], matrix [1] [1], 0, 0,
	                                       0, 0, 1, 0,
	                                       matrix [2] [0], -matrix [2] [1], 0, 1));

	transform -> children ()    = { shape };
	shape -> appearance ()      = appearance;
	shape -> geometry ()        = rectangle;
	appearance -> material ()   = material;
	material -> diffuseColor () = style .fill;
	rectangle -> size ()        = X3D::Vector2f (width, height);

	groups .back () -> children () .emplace_back (transform);
}

void
Parser::ellipseElement (xmlpp::Element* const xmlElement)
{
	__LOG__ << xmlElement -> get_name () << std::endl;
}

void
Parser::pathElement (xmlpp::Element* const xmlElement)
{
	__LOG__ << xmlElement -> get_name () << std::endl;
}

bool
Parser::viewBoxAttribute (xmlpp::Attribute* const xmlAttribute, X3D::Vector4d & value)
{
	if (not xmlAttribute)
		return false;

	std::istringstream isstream (xmlAttribute -> get_value ());

	isstream .imbue (std::locale::classic ());

	double x, y, width, height;

	if (Grammar::DoubleValue (isstream, x))
	{
		if (Grammar::DoubleValue (isstream, y))
		{
			if (Grammar::DoubleValue (isstream, width))
			{
				if (Grammar::DoubleValue (isstream, height))
				{
					value = X3D::Vector4d (x, y, width, height);
					return true;
				}
			}
		}
	}

	return false;
}

bool
Parser::transformAttribute (xmlpp::Attribute* const xmlAttribute, X3D::Matrix3d & matrix)
{
	if (not xmlAttribute)
		return false;

	const std::string transform = xmlAttribute -> get_value ();
	auto              values    = std::vector <std::string> ();

	basic::split (std::back_inserter (values), transform, ";");

	for (const auto & value : values)
	{
		std::istringstream isstream (value);

		isstream .imbue (std::locale::classic ());

		if (Grammar::matrix (isstream))
		{
			double a, b, c, d, e, f;

			if (Grammar::OpenParenthesis (isstream))
			{
				if (Grammar::DoubleValue (isstream, a))
				{
					if (Grammar::Comma (isstream))
					{
						if (Grammar::DoubleValue (isstream, b))
						{
							if (Grammar::Comma (isstream))
							{
								if (Grammar::DoubleValue (isstream, c))
								{
									if (Grammar::Comma (isstream))
									{
										if (Grammar::DoubleValue (isstream, d))
										{
											if (Grammar::Comma (isstream))
											{
												if (Grammar::DoubleValue (isstream, e))
												{
													if (Grammar::Comma (isstream))
													{
														if (Grammar::DoubleValue (isstream, f))
														{
															if (Grammar::CloseParenthesis (isstream))
															{
																matrix = X3D::Matrix3d (a, b, 0,  c, d, 0,  e, f, 1);
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (false)
		{

		}
	}

	return true;
}

bool
Parser::lengthAttribute (xmlpp::Attribute* const xmlAttribute, double & value)
{
	if (not xmlAttribute)
		return false;

	std::istringstream isstream (xmlAttribute -> get_value ());

	isstream .imbue (std::locale::classic ());

	if (Grammar::DoubleValue (isstream, value))
	{
		// Parse unit

		if (Grammar::mm (isstream))
			value = value / (1000 * math::pixel <double>);

		return true;
	}

	return false;
}

bool
Parser::styleAttribute (xmlpp::Attribute* const xmlAttribute, Style & styleObject)
{
	if (not xmlAttribute)
		return false;

	const std::string styles = xmlAttribute -> get_value ();

	auto values = std::vector <std::string> ();

	basic::split (std::back_inserter (values), styles, ";");

	for (const auto & value : values)
	{
		auto pair = std::vector <std::string> ();
	
		basic::split (std::back_inserter (pair), value, ":");

		if (pair .size () not_eq 2)
			continue;

		basic::trim (pair [0]);
		basic::trim (pair [1]);

		if (pair [0] == "fill")
		{
			std::istringstream isstream (pair [1]);

			isstream .imbue (std::locale::classic ());

			if (Grammar::NumberSign (isstream))
			{
				int32_t color;

				if (Grammar::HexValue (isstream, color))
				{
					float b = (color & 255) / 255.0f;
					float g = ((color >>= 8) & 255) / 255.0f;
					float r = ((color >>= 8) & 255) / 255.0f;

					styleObject .fill = Color3f (r, g, b);
				}
			}
		}
	}

	return true;
}

Parser::~Parser ()
{ }

} // SVG
} // X3D
} // titania
