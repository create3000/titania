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

#include "../../Components/Geometry2D/Disk2D.h"
#include "../../Components/Geometry2D/Rectangle2D.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../Filter.h"

#include <Titania/Math/Algorithms/Bezier.h>
#include <Titania/Math/Mesh/Tessellator.h>
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

	static const io::sequence WhiteSpaces;
	static const io::sequence CommaWhiteSpaces;

	static const io::string matrix;

	static const io::character OpenParenthesis;
	static const io::character CloseParenthesis;
	static const io::character Comma;
	static const io::character NumberSign;

	static const io::character M;
	static const io::character m;
	static const io::character L;
	static const io::character l;
	static const io::character H;
	static const io::character h;
	static const io::character V;
	static const io::character v;
	static const io::character C;
	static const io::character c;
	static const io::character S;
	static const io::character s;
	static const io::character Z;
	static const io::character z;

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

const io::sequence Grammar::WhiteSpaces ("\r\n \t");
const io::sequence Grammar::CommaWhiteSpaces ("\r\n \t,");

const io::string Grammar::matrix ("matrix");

const io::character Grammar::OpenParenthesis ('(');
const io::character Grammar::CloseParenthesis (')');
const io::character Grammar::Comma (',');
const io::character Grammar::NumberSign ('#');

const io::character Grammar::M ('M');
const io::character Grammar::m ('m');
const io::character Grammar::L ('L');
const io::character Grammar::l ('l');
const io::character Grammar::H ('H');
const io::character Grammar::h ('h');
const io::character Grammar::V ('V');
const io::character Grammar::v ('v');
const io::character Grammar::C ('C');
const io::character Grammar::c ('c');
const io::character Grammar::S ('S');
const io::character Grammar::s ('s');
const io::character Grammar::Z ('Z');
const io::character Grammar::z ('z');

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
	        styles (),
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
		std::make_pair ("circle",  std::mem_fn (&Parser::circleElement)),
		std::make_pair ("ellipse", std::mem_fn (&Parser::ellipseElement)),
		std::make_pair ("polygon", std::mem_fn (&Parser::polygonElement)),
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
	// Determine style.

	Style style;

	styleAttribute (xmlElement -> get_attribute ("style"), style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Determine matrix.	

	const auto transform = getTransform (xmlElement);

	groups .back () -> children () .emplace_back (transform);

	groups .emplace_back (transform);

	elements (xmlElement);

	groups .pop_back ();

	styles .pop_back ();
}

void
Parser::rectangleElement (xmlpp::Element* const xmlElement)
{
	// Determine style.

	Style style;

	styleAttribute (xmlElement -> get_attribute ("style"), style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Determine matrix.	

	double x      = 0;
	double y      = 0;
	double width  = 0;
	double height = 0;

	lengthAttribute (xmlElement -> get_attribute ("x"),      x);
	lengthAttribute (xmlElement -> get_attribute ("y"),      y);
	lengthAttribute (xmlElement -> get_attribute ("width"),  width);
	lengthAttribute (xmlElement -> get_attribute ("height"), height);

	const auto transform = getTransform (xmlElement, X3D::Vector2d (x + width / 2, y + height / 2));

	// Create nodes.

	if (getFillSet ())
	{
		const auto shape     = scene -> createNode <X3D::Shape> ();
		const auto rectangle = scene -> createNode <X3D::Rectangle2D> ();

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getFillAppearance ();
		shape -> geometry ()   = rectangle;
		rectangle -> solid ()  = false;
		rectangle -> size ()   = X3D::Vector2f (width, height);
	}

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);

	styles .pop_back ();
}

void
Parser::circleElement (xmlpp::Element* const xmlElement)
{
	// Determine style.

	Style style;

	styleAttribute (xmlElement -> get_attribute ("style"), style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Determine matrix.	

	double cx = 0;
	double cy = 0;
	double r  = 0;

	lengthAttribute (xmlElement -> get_attribute ("cx"), cx);
	lengthAttribute (xmlElement -> get_attribute ("cy"), cy);
	lengthAttribute (xmlElement -> get_attribute ("r"),  r);

	const auto transform = getTransform (xmlElement, X3D::Vector2d (cx, cy));

	// Create nodes.

	if (getFillSet ())
	{
		const auto shape = scene -> createNode <X3D::Shape> ();
		const auto disk  = scene -> createNode <X3D::Disk2D> ();

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getFillAppearance ();
		shape -> geometry ()   = disk;
		disk -> solid ()       = false;
		disk -> outerRadius () = r;
	}

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);

	styles .pop_back ();
}

void
Parser::ellipseElement (xmlpp::Element* const xmlElement)
{
	// Determine style.

	Style style;

	styleAttribute (xmlElement -> get_attribute ("style"), style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Determine matrix.	

	double cx = 0;
	double cy = 0;
	double rx = 0;
	double ry = 0;

	lengthAttribute (xmlElement -> get_attribute ("cx"), cx);
	lengthAttribute (xmlElement -> get_attribute ("cy"), cy);
	lengthAttribute (xmlElement -> get_attribute ("rx"), rx);
	lengthAttribute (xmlElement -> get_attribute ("ry"), ry);

	const auto rmin      = std::min (rx, ry);
	const auto transform = getTransform (xmlElement, X3D::Vector2d (cx, cy), X3D::Vector2d (rx / rmin, ry / rmin));

	// Create nodes.

	if (getFillSet ())
	{
		const auto shape = scene -> createNode <X3D::Shape> ();
		const auto disk  = scene -> createNode <X3D::Disk2D> ();

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getFillAppearance ();
		shape -> geometry ()   = disk;
		disk -> solid ()       = false;
		disk -> outerRadius () = rmin;
	}

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);

	styles .pop_back ();
}

void
Parser::polygonElement (xmlpp::Element* const xmlElement)
{
	//__LOG__ << xmlElement -> get_name () << std::endl;
}

void
Parser::pathElement (xmlpp::Element* const xmlElement)
{
	using Tesselator = math::tessellator <double, size_t>;

	// Get path points.

	Contours contours;

	if (not dAttribute (xmlElement -> get_attribute ("d"), contours))
		return;

	// Determine style.

	Style style;

	styleAttribute (xmlElement -> get_attribute ("style"), style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Determine matrix.	

	const auto transform = getTransform (xmlElement);

	// Create nodes.

	if (getFillSet ())
	{
		// Tesselate contours

		Tesselator tessellator;

		tessellator .property (GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

		tessellator .property (GLU_TESS_TOLERANCE, 0);
		tessellator .normal (Vector3d (0, 0, 1));
		tessellator .begin_polygon ();

		size_t index = 0;

		for (const auto & contour : contours)
		{
			tessellator .begin_contour ();

			for (const auto & point : contour)
			{
				tessellator .add_vertex (X3D::Vector3d (point .x (), point .y (), 0), index ++);
			}

			tessellator .end_contour ();
		}

		tessellator .end_polygon ();

		const auto triangles = tessellator .triangles ();

		if (triangles .size () >= 3)
		{
			// Create geometry.
	
			const auto shape      = scene -> createNode <X3D::Shape> ();
			const auto geometry   = scene -> createNode <X3D::IndexedFaceSet> ();
			const auto coordinate = scene -> createNode <X3D::Coordinate> ();
	
			transform -> children () .emplace_back (shape);
	
			shape -> appearance () = getFillAppearance ();
			shape -> geometry ()   = geometry;
			geometry -> solid ()   = false;
			geometry -> coord ()   = coordinate;
	
			for (size_t i = 0, size = triangles .size (); i < size; i += 3)
			{
				geometry -> coordIndex () .emplace_back (std::get <0> (triangles [i + 0] .data ()));
				geometry -> coordIndex () .emplace_back (std::get <0> (triangles [i + 1] .data ()));
				geometry -> coordIndex () .emplace_back (std::get <0> (triangles [i + 2] .data ()));
				geometry -> coordIndex () .emplace_back (-1);
			}

			for (const auto & contour : contours)
			{
				for (const auto & point : contour)
					coordinate -> point () .emplace_back (point .x (), -point .y (), 0);
			}
		}
	}

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);

	styles .pop_back ();
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
Parser::dAttribute (xmlpp::Attribute* const xmlAttribute, Contours & contours)
{
	if (not xmlAttribute)
		return false;

	std::istringstream isstream (xmlAttribute -> get_value ());

	isstream .imbue (std::locale::classic ());

	Contour     contour;
	std::string whiteSpaces;

	double ax = 0, ay = 0;

	while (isstream)
	{
		Grammar::CommaWhiteSpaces (isstream, whiteSpaces);

		if (Grammar::M (isstream))
		{
			if (not contour .empty ())
				contours .emplace_back (std::move (contour));

			bool first = true;

			while (isstream)
			{
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, ax))
					{
						if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
						{
							if (Grammar::DoubleValue (isstream, ay))
							{
								contour .emplace_back (ax, ay);
								continue;
							}
						}
					}
				}

				break;
			}

			continue;
		}
		else if (Grammar::m (isstream))
		{
			if (not contour .empty ())
				contours .emplace_back (std::move (contour));

			bool first = true;

			while (isstream)
			{
				double x, y;
	
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, x))
					{
						if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
						{
							if (Grammar::DoubleValue (isstream, y))
							{
								ax += x;
								ay += y;

								contour .emplace_back (ax, ay);
								continue;
							}
						}
					}
				}

				break;
			}

			continue;
		}
		else if (Grammar::L (isstream))
		{
			bool first = true;

			while (isstream)
			{
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, ax))
					{
						if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
						{
							if (Grammar::DoubleValue (isstream, ay))
							{
								contour .emplace_back (ax, ay);
								continue;
							}
						}
					}
				}

				break;
			}

			continue;
		}
		else if (Grammar::l (isstream))
		{
			bool first = true;

			while (isstream)
			{
				double x, y;
	
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, x))
					{
						if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
						{
							if (Grammar::DoubleValue (isstream, y))
							{
								ax += x;
								ay += y;

								contour .emplace_back (ax, ay);
								continue;
							}
						}
					}
				}

				break;
			}

			continue;
		}
		else if (Grammar::H (isstream))
		{
			bool first = true;

			while (isstream)
			{
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, ax))
					{
						contour .emplace_back (ax, ay);
						continue;
					}
				}

				break;
			}

			continue;
		}
		else if (Grammar::h (isstream))
		{
			bool first = true;

			while (isstream)
			{
				double x;
	
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, x))
					{
						ax += x;

						contour .emplace_back (ax, ay);
						continue;
					}
				}

				break;
			}

			continue;
		}
		else if (Grammar::V (isstream))
		{
			bool first = true;

			while (isstream)
			{
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, ay))
					{
						contour .emplace_back (ax, ay);
						continue;
					}
				}

				break;
			}

			continue;
		}
		else if (Grammar::v (isstream))
		{
			bool first = true;

			while (isstream)
			{
				double y;
	
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, y))
					{
						ay += y;

						contour .emplace_back (ax, ay);
						continue;
					}
				}

				break;
			}

			continue;
		}
		else if (Grammar::C (isstream))
		{
			bool first = true;

			while (isstream)
			{
				double x1, y1, x2, y2, x, y;
	
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, x1))
					{
						if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
						{
							if (Grammar::DoubleValue (isstream, y1))
							{
								if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
								{
									if (Grammar::DoubleValue (isstream, x2))
									{
										if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
										{
											if (Grammar::DoubleValue (isstream, y2))
											{
												if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
												{
													if (Grammar::DoubleValue (isstream, x))
													{
														if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
														{
															if (Grammar::DoubleValue (isstream, y))
															{
																math::bezier::cubic_curve (X3D::Vector2d (ax, ay), X3D::Vector2d (x1, y1), X3D::Vector2d (x2, y2), X3D::Vector2d (x, y), 9, contour);

																ax = x;
																ay = y;
																continue;
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

				break;
			}

			continue;
		}
		else if (Grammar::c (isstream))
		{
			bool first = true;

			while (isstream)
			{
				double x1, y1, x2, y2, x, y;
	
				if (first || Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
				{
					first = false;

					if (Grammar::DoubleValue (isstream, x1))
					{
						if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
						{
							if (Grammar::DoubleValue (isstream, y1))
							{
								if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
								{
									if (Grammar::DoubleValue (isstream, x2))
									{
										if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
										{
											if (Grammar::DoubleValue (isstream, y2))
											{
												if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
												{
													if (Grammar::DoubleValue (isstream, x))
													{
														if (Grammar::CommaWhiteSpaces (isstream, whiteSpaces))
														{
															if (Grammar::DoubleValue (isstream, y))
															{
																x1 += ax;
																y1 += ay;
																x2 += ax;
																y2 += ay;
																x  += ax;
																y  += ay;

																math::bezier::cubic_curve (X3D::Vector2d (ax, ay), X3D::Vector2d (x1, y1), X3D::Vector2d (x2, y2), X3D::Vector2d (x, y), 9, contour);

																ax = x;
																ay = y;
																continue;
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

				break;
			}

			continue;
		}
		else if (Grammar::Z (isstream) or Grammar::z (isstream))
		{
			if (not contour .empty ())
			{
				ax = contour .front () .x ();
				ay = contour .front () .y ();

				contours .emplace_back (std::move (contour));
			}

			continue;
		}

		break;
	}

	if (not contour .empty ())
		contours .emplace_back (std::move (contour));

	return true;
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

		std::istringstream vstream (pair [1]);

		vstream .imbue (std::locale::classic ());

		if (pair [0] == "display")
		{
			styleObject .display = pair [1];
		}
		else if (pair [0] == "fill")
		{
			X3D::Color3f color;

			if (colorValue (vstream, color))
			{
				styleObject .fillSet = true;
				styleObject .fill    = color;
			}
		}
		else if (pair [0] == "fill-opacity")
		{
			double fillOpacity;

			if (Grammar::DoubleValue (vstream, fillOpacity))
			{
				styleObject .fillOpacity = fillOpacity;
			}
		}
		else if (pair [0] == "opacity")
		{
			double opacity;

			if (Grammar::DoubleValue (vstream, opacity))
			{
				styleObject .opacity = opacity;
			}
		}
	}

	return true;
}

bool
Parser::colorValue (std::istream & istream, X3D::Color3f & color)
{
	if (Grammar::NumberSign (istream))
	{
		int32_t number;

		if (Grammar::HexValue (istream, number))
		{
			float b = (number & 255) / 255.0f;
			float g = ((number >>= 8) & 255) / 255.0f;
			float r = ((number >>= 8) & 255) / 255.0f;

			color = X3D::Color3f (r, g, b);
			return true;
		}
	}

	return false;
}

X3D::X3DPtr <X3D::Transform>
Parser::getTransform (xmlpp::Element* const xmlElement, const X3D::Vector2d & translation, const X3D::Vector2d & scale)
{
	X3D::Matrix3d matrix;

	transformAttribute (xmlElement -> get_attribute ("transform"), matrix);

	matrix .translate (translation);
	matrix .scale (scale);

	const auto transform = scene -> createNode <X3D::Transform> ();

	transform -> setMatrix (X3D::Matrix4d (matrix [0] [0], matrix [0] [1], 0, 0,
	                                       matrix [1] [0], matrix [1] [1], 0, 0,
	                                       0, 0, 1, 0,
	                                       matrix [2] [0], -matrix [2] [1], 0, 1));

	return transform;
}

X3D::X3DPtr <X3D::Appearance>
Parser::getFillAppearance ()
{
	const auto appearance = scene -> createNode <X3D::Appearance> ();
	const auto material   = scene -> createNode <X3D::Material> ();

	appearance -> material ()   = material;
	material -> diffuseColor () = getFill ();
	material -> transparency () = 1 - getFillOpacity ();

	return appearance;
}

bool
Parser::getFillSet () const
{
	for (const auto & style : styles)
	{
		if (style .fillSet)
			return true;
	}

	return false;
}

X3D::Color3f
Parser::getFill () const
{
	for (const auto & style : styles)
	{
		if (style .fillSet)
			return style .fill;
	}

	return X3D::Color3f ();
}

double
Parser::getFillOpacity () const
{
	double opacity = 1;

	for (const auto & style : styles)
	{
		opacity *= style .opacity;

		if (style .fillSet)
			return style .fillOpacity * opacity;
	}

	return 0;
}

Parser::~Parser ()
{ }

} // SVG
} // X3D
} // titania
