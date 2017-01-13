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

#include "../../Components/Geometry2D/Circle2D.h"
#include "../../Components/Geometry2D/Disk2D.h"
#include "../../Components/Geometry2D/Rectangle2D.h"
#include "../../Components/Geometry2D/Polyline2D.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Navigation/OrthoViewpoint.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../../Components/Texturing/ImageTexture.h"
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

	static const io::string matrix;
	static const io::string translate;
	static const io::string rotate;
	static const io::string scale;
	static const io::string skewX;
	static const io::string skewY;
	static const io::string rgb;

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
	static const io::character PercentSign;

	static const io::number <double>  DoubleValue;
	static const io::number <int32_t> IntegerValue;
	static const io::hex <int32_t>    HexValue;
	static const io::sequence         NamedColor;
};

const io::sequence Grammar::WhiteSpaces ("\r\n \t");

const io::string Grammar::matrix ("matrix");
const io::string Grammar::translate ("translate");
const io::string Grammar::rotate ("rotate");
const io::string Grammar::scale ("scale");
const io::string Grammar::skewX ("skewX");
const io::string Grammar::skewY ("skewY");
const io::string Grammar::rgb ("rgb", io::CASE_INSENSITIVE);

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
const io::character Grammar::PercentSign ('%');

const io::number <double>  Grammar::DoubleValue;
const io::number <int32_t> Grammar::IntegerValue;
const io::hex <int32_t>    Grammar::HexValue;
const io::sequence         Grammar::NamedColor ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

// Parser

static constexpr size_t BEZIER_STEPS = 9;

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	      X3D::X3DParser (),
	               scene (scene),
	                 uri (uri),
	             istream (istream),
	           xmlParser (new xmlpp::DomParser ()),
	              styles (),
	              groups ({ scene -> createNode <X3D::Transform> () }),
	         namedColors (),
	whiteSpaceCharacters ()
{
	xmlParser -> set_throw_messages (true);
	xmlParser -> set_validate (false);
	xmlParser -> set_include_default_attributes (true);
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

	// Get attributes of svg element.

	double        width  = 0;
	double        height = 0;
	X3D::Vector4d viewBox;

	if (not lengthAttribute  (xmlElement -> get_attribute ("width"), width))
		width = 100;

	if (not lengthAttribute  (xmlElement -> get_attribute ("height"), height))
		height = 100;

	if (not viewBoxAttribute (xmlElement -> get_attribute ("viewBox"), viewBox))
		viewBox = X3D::Vector4d (0, 0, width, height);

	// Create viewpoint.

	const auto viewpoint = scene -> createNode <X3D::OrthoViewpoint> ();

	viewpoint -> description ()      = uri .basename ();
	viewpoint -> position ()         = X3D::Vector3f ((viewBox .x () + width / 2) * math::pixel <double>, -(viewBox .y () + height / 2) * math::pixel <double>, 10);
	viewpoint -> centerOfRotation () = X3D::Vector3f ((viewBox .x () + width / 2) * math::pixel <double>, -(viewBox .y () + height / 2) * math::pixel <double>, 0);

	viewpoint -> fieldOfView () = {
		-width  / 2 * math::pixel <double>,
		-height / 2 * math::pixel <double>,
		 width  / 2 * math::pixel <double>,
		 height / 2 * math::pixel <double>,
	};

	scene -> getRootNodes () .emplace_back (viewpoint);

	// Create root Transform.

	const auto transform   = groups .front ();
	const auto translation = X3D::Vector3d (-viewBox .x (), viewBox .y (), 0);
	const auto scale       = X3D::Vector3d (math::pixel <double> * width / viewBox [2], math::pixel <double> * height / viewBox [3], 1);

	transform -> translation () = translation * scale;
	transform -> scale ()       = scale;

	scene -> updateNamedNode (get_name_from_uri (uri), X3D::SFNode (transform));
	scene -> getRootNodes () .emplace_back (transform);

	// Parse elements.

	elements (xmlElement);
}

bool
Parser::whiteSpaces (std::istream & istream)
{
	return Grammar::WhiteSpaces (istream, whiteSpaceCharacters);
}

bool
Parser::commaWhiteSpaces (std::istream & istream)
{
	bool result = false;

	result |= Grammar::WhiteSpaces (istream, whiteSpaceCharacters);
	result |= Grammar::Comma (istream);
	result |= Grammar::WhiteSpaces (istream, whiteSpaceCharacters);

	return result;
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
		std::make_pair ("a",       std::mem_fn (&Parser::aElement)),
		std::make_pair ("rect",    std::mem_fn (&Parser::rectangleElement)),
		std::make_pair ("circle",  std::mem_fn (&Parser::circleElement)),
		std::make_pair ("ellipse", std::mem_fn (&Parser::ellipseElement)),
		std::make_pair ("polygon", std::mem_fn (&Parser::polygonElement)),
		std::make_pair ("text",    std::mem_fn (&Parser::textElement)),
		std::make_pair ("image",   std::mem_fn (&Parser::imageElement)),
		std::make_pair ("path",    std::mem_fn (&Parser::pathElement)),
	};

	try
	{
		if (not xmlElement)
			return;

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

	// Get transform.	

	const auto transform = getTransform (xmlElement);

	groups .emplace_back (transform);

	elements (xmlElement);

	groups .pop_back ();

	styles .pop_back ();

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);
}

void
Parser::aElement (xmlpp::Element* const xmlElement)
{
	groupElement (xmlElement);
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

	// Get transform.	

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

	if (getStrokeSet ())
	{
		const auto shape     = scene -> createNode <X3D::Shape> ();
		const auto polyline  = scene -> createNode <X3D::Polyline2D> ();
		const auto width1_2  = width / 2;
		const auto height1_2 = height / 2;

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getStrokeAppearance ();
		shape -> geometry ()   = polyline;

		polyline -> lineSegments () .emplace_back ( width1_2,  height1_2);
		polyline -> lineSegments () .emplace_back (-width1_2,  height1_2);
		polyline -> lineSegments () .emplace_back (-width1_2, -height1_2);
		polyline -> lineSegments () .emplace_back ( width1_2, -height1_2);
		polyline -> lineSegments () .emplace_back ( width1_2,  height1_2);
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

	// Get transform.	

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

	if (getStrokeSet ())
	{
		const auto shape  = scene -> createNode <X3D::Shape> ();
		const auto circle = scene -> createNode <X3D::Circle2D> ();

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getStrokeAppearance ();
		shape -> geometry ()   = circle;
		circle -> radius ()    = r;
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

	// Get transform.	

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

	if (getStrokeSet ())
	{
		const auto shape  = scene -> createNode <X3D::Shape> ();
		const auto circle = scene -> createNode <X3D::Circle2D> ();

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getStrokeAppearance ();
		shape -> geometry ()   = circle;
		circle -> radius ()    = rmin;
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
Parser::textElement (xmlpp::Element* const xmlElement)
{
	//__LOG__ << xmlElement -> get_name () << std::endl;
}

void
Parser::imageElement (xmlpp::Element* const xmlElement)
{
	// Get transform.	

	double x      = 0;
	double y      = 0;
	double width  = 0;
	double height = 0;

	lengthAttribute (xmlElement -> get_attribute ("x"),      x);
	lengthAttribute (xmlElement -> get_attribute ("y"),      y);
	lengthAttribute (xmlElement -> get_attribute ("width"),  width);
	lengthAttribute (xmlElement -> get_attribute ("height"), height);

	const auto transform = getTransform (xmlElement, X3D::Vector2d (x + width / 2, y + height / 2));

	// Get href.

	std::string href;

	stringAttribute (xmlElement -> get_attribute ("href", "xlink"), href);

	// Create nodes.

	const auto shape      = scene -> createNode <X3D::Shape> ();
	const auto appearance = scene -> createNode <X3D::Appearance> ();
	const auto texture    = scene -> createNode <X3D::ImageTexture> ();
	const auto rectangle  = scene -> createNode <X3D::Rectangle2D> ();

	transform -> children () .emplace_back (shape);

	shape -> appearance ()   = appearance;
	shape -> geometry ()     = rectangle;
	appearance -> texture () = texture;
	texture -> url ()        = { href };
	rectangle -> solid ()    = false;
	rectangle -> size ()     = X3D::Vector2f (width, height);

	groups .back () -> children () .emplace_back (transform);
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

	// Get transform.	

	const auto transform = getTransform (xmlElement);

	// Create nodes.

	const auto coordinate = scene -> createNode <X3D::Coordinate> ();

	for (const auto & contour : contours)
	{
		for (const auto & point : contour)
			coordinate -> point () .emplace_back (point .x (), -point .y (), 0);
	}

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
	
			const auto shape    = scene -> createNode <X3D::Shape> ();
			const auto geometry = scene -> createNode <X3D::IndexedFaceSet> ();
	
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

	if (getStrokeSet ())
	{
		// Create geometry.

		const auto shape    = scene -> createNode <X3D::Shape> ();
		const auto geometry = scene -> createNode <X3D::IndexedLineSet> ();

		shape -> appearance () = getStrokeAppearance ();
		shape -> geometry ()   = geometry;
		geometry -> coord ()   = coordinate;

		size_t index = 0;

		for (const auto & contour : contours)
		{
			if (contour .size () > 1)
			{
				const auto first = index;
	
				for (size_t i = 0, size = contour .size (); i < size; ++ i)
					geometry -> coordIndex () .emplace_back (index ++);

				if (getFillSet () and not contour .empty ())
					geometry -> coordIndex () .emplace_back (first);
	
				geometry -> coordIndex () .emplace_back (-1);
			}
		}

		if (not geometry -> coordIndex () .empty ())
			transform -> children () .emplace_back (shape);
	}

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);

	styles .pop_back ();
}

void
Parser::idAttribute (xmlpp::Attribute* const attribute, const X3D::SFNode & node)
{
	try
	{
		if (not attribute)
			return;

		scene -> updateNamedNode (get_name_from_string (attribute -> get_value ()), node);
	}
	catch (const X3D::X3DError &)
	{ }
}

bool
Parser::viewBoxAttribute (xmlpp::Attribute* const xmlAttribute, X3D::Vector4d & value)
{
	if (not xmlAttribute)
		return false;

	std::istringstream vstream (xmlAttribute -> get_value ());

	vstream .imbue (std::locale::classic ());

	double x, y, width, height;

	if (Grammar::DoubleValue (vstream, x))
	{
		if (Grammar::DoubleValue (vstream, y))
		{
			if (Grammar::DoubleValue (vstream, width))
			{
				if (Grammar::DoubleValue (vstream, height))
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

	std::istringstream vstream (xmlAttribute -> get_value ());

	vstream .imbue (std::locale::classic ());

	while (vstream)
	{
		whiteSpaces (vstream);

		if (Grammar::matrix (vstream))
		{
			double a, b, c, d, e, f;

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, a))
				{
					if (commaWhiteSpaces (vstream))
					{
						if (Grammar::DoubleValue (vstream, b))
						{
							if (commaWhiteSpaces (vstream))
							{
								if (Grammar::DoubleValue (vstream, c))
								{
									if (commaWhiteSpaces (vstream))
									{
										if (Grammar::DoubleValue (vstream, d))
										{
											if (commaWhiteSpaces (vstream))
											{
												if (Grammar::DoubleValue (vstream, e))
												{
													if (commaWhiteSpaces (vstream))
													{
														if (Grammar::DoubleValue (vstream, f))
														{
															whiteSpaces (vstream);

															if (Grammar::CloseParenthesis (vstream))
															{
																matrix .mult_left (X3D::Matrix3d (a, b, 0,  c, d, 0,  e, f, 1));
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
			}

			break;
		}
		else if (Grammar::translate (vstream))
		{
			double tx, ty;

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, tx))
				{
					if (commaWhiteSpaces (vstream))
					{
						if (Grammar::DoubleValue (vstream, ty))
						{
							whiteSpaces (vstream);

							if (Grammar::CloseParenthesis (vstream))
							{
								matrix .translate (X3D::Vector2d (tx, ty));
								continue;
							}
						}
					}
				}
			}

			break;
		}
		else if (Grammar::rotate (vstream))
		{
			double angle;

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, angle))
				{
					bool cwsp = false;

					cwsp |= whiteSpaces (vstream);

					if (Grammar::CloseParenthesis (vstream))
					{
						matrix .rotate (angle);
						continue;
					}
					else
					{
						double cx, cy;

						cwsp |= Grammar::Comma (vstream);
						cwsp |= whiteSpaces (vstream);

						if (cwsp)
						{
							if (Grammar::DoubleValue (vstream, cx))
							{
								if (commaWhiteSpaces (vstream))
								{
									if (Grammar::DoubleValue (vstream, cy))
									{
										whiteSpaces (vstream);
			
										if (Grammar::CloseParenthesis (vstream))
										{
											matrix .translate (X3D::Vector2d (cx, cy));
											matrix .rotate (angle);
											matrix .translate(X3D::Vector2d (-cx, -cy));
											continue;
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
		else if (Grammar::scale (vstream))
		{
			double sx, sy;

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, sx))
				{
					if (commaWhiteSpaces (vstream))
					{
						if (Grammar::DoubleValue (vstream, sy))
						{
							whiteSpaces (vstream);

							if (Grammar::CloseParenthesis (vstream))
							{
								matrix .scale (X3D::Vector2d (sx, sy));
								continue;
							}
						}
					}
				}
			}

			break;
		}
		else if (Grammar::skewX (vstream))
		{
			double angle;

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, angle))
				{
					whiteSpaces (vstream);

					if (Grammar::CloseParenthesis (vstream))
					{
						matrix .mult_left (X3D::Matrix3d (1, std::tan (angle), 0, 0, 1, 0, 0, 0, 1));
						continue;
					}
				}
			}

			break;
		}
		else if (Grammar::skewY (vstream))
		{
			double angle;

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, angle))
				{
					whiteSpaces (vstream);

					if (Grammar::CloseParenthesis (vstream))
					{
						matrix .mult_left (X3D::Matrix3d (1, 0, 0, std::tan (angle), 1, 0, 0, 0, 1));
						continue;
					}
				}
			}

			break;
		}

		break;
	}

	return true;
}

bool
Parser::lengthAttribute (xmlpp::Attribute* const xmlAttribute, double & value)
{
	if (not xmlAttribute)
		return false;

	std::istringstream vstream (xmlAttribute -> get_value ());

	vstream .imbue (std::locale::classic ());

	if (Grammar::DoubleValue (vstream, value))
	{
		// Parse unit

		if (Grammar::mm (vstream))
			value = value / (1000 * math::pixel <double>);

		return true;
	}

	return false;
}

bool
Parser::stringAttribute (xmlpp::Attribute* const xmlAttribute, std::string & value)
{
	if (not xmlAttribute)
		return false;

	value = xmlAttribute -> get_value ();

	return false;
}

bool
Parser::dAttribute (xmlpp::Attribute* const xmlAttribute, Contours & contours)
{
	if (not xmlAttribute)
		return false;

	std::istringstream vstream (xmlAttribute -> get_value ());

	vstream .imbue (std::locale::classic ());

	Contour     contour;

	double ax = 0, ay = 0, px = 0, py = 0;

	std::istringstream::int_type peek = -1, previous = -1;

	while (vstream)
	{
		commaWhiteSpaces (vstream);

		previous = peek;
		peek     = vstream .peek ();

		const auto relative = std::islower ((char) peek, std::locale::classic ());

		switch (peek)
		{
			case 'M':
			case 'm':
			{
				vstream .get ();

				if (not contour .empty ())
					contours .emplace_back (std::move (contour));
	
				bool first = true;
	
				while (vstream)
				{
					double x, y;
		
					if (first or commaWhiteSpaces (vstream))
					{
						first = false;
	
						if (Grammar::DoubleValue (vstream, x))
						{
							if (commaWhiteSpaces (vstream))
							{
								if (Grammar::DoubleValue (vstream, y))
								{
									if (relative)
									{
										x += ax;
										y += ay;
									}
	
									contour .emplace_back (x, y);
	
									ax = x;
									ay = y;
									continue;
								}
							}
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'L':
			case 'l':
			{
				vstream .get ();

				bool first = true;
	
				while (vstream)
				{
					double x, y;
		
					if (first or commaWhiteSpaces (vstream))
					{
						first = false;
	
						if (Grammar::DoubleValue (vstream, x))
						{
							if (commaWhiteSpaces (vstream))
							{
								if (Grammar::DoubleValue (vstream, y))
								{
									if (relative)
									{
										x += ax;
										y += ay;
									}
	
									contour .emplace_back (x, y);
	
									ax = x;
									ay = y;
									continue;
								}
							}
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'H':
			case 'h':
			{
				vstream .get ();

				bool first = true;
	
				while (vstream)
				{
					double x;
		
					if (first or commaWhiteSpaces (vstream))
					{
						first = false;
	
						if (Grammar::DoubleValue (vstream, x))
						{
							if (relative)
								x += ax;
	
							contour .emplace_back (x, ay);
	
							ax = x;
							continue;
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'V':
			case 'v':
			{
				vstream .get ();

				bool first = true;
	
				while (vstream)
				{
					double y;
		
					if (first or commaWhiteSpaces (vstream))
					{
						first = false;
	
						if (Grammar::DoubleValue (vstream, y))
						{
							if (relative)
								y += ay;
	
							contour .emplace_back (ax, y);
	
							ay = y;
							continue;
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'Q':
			case 'q':
			{
				vstream .get ();

				bool first = true;
	
				while (vstream)
				{
					double x1, y1, x, y;
		
					if (first or commaWhiteSpaces (vstream))
					{
						first = false;
	
						if (Grammar::DoubleValue (vstream, x1))
						{
							if (commaWhiteSpaces (vstream))
							{
								if (Grammar::DoubleValue (vstream, y1))
								{
									if (commaWhiteSpaces (vstream))
									{
										if (Grammar::DoubleValue (vstream, x))
										{
											if (commaWhiteSpaces (vstream))
											{
												if (Grammar::DoubleValue (vstream, y))
												{
													if (relative)
													{
														x1 += ax;
														y1 += ay;
														x  += ax;
														y  += ay;
													}

													math::bezier::quadratic_curve (X3D::Vector2d (ax, ay), X3D::Vector2d (x1, y1), X3D::Vector2d (x, y), BEZIER_STEPS, contour);

													ax = x;
													ay = y;
													px = x1;
													py = y1;
													continue;
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
			case 'T':
			case 't':
			{
				vstream .get ();

				bool first = true;
	
				while (vstream)
				{
					double x1, y1, x, y;
		
					if (first or commaWhiteSpaces (vstream))
					{
						first = false;
	
						if (Grammar::DoubleValue (vstream, x))
						{
							if (commaWhiteSpaces (vstream))
							{
								if (Grammar::DoubleValue (vstream, y))
								{
									if (relative)
									{
										x += ax;
										y += ay;
									}

									switch (previous)
									{
										case 'Q':
										case 'q':
										case 'T':
										case 't':
										{
											x1 = ax + (ax - px);
											y1 = ay + (ay - py);
											break;
										}
										default:
										{
											x1 = ax;
											y1 = ay;
											break;
										}
									}

									math::bezier::quadratic_curve (X3D::Vector2d (ax, ay), X3D::Vector2d (x1, y1), X3D::Vector2d (x, y), BEZIER_STEPS, contour);

									ax = x;
									ay = y;
									continue;
								}
							}
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'C':
			case 'c':
			{
				vstream .get ();

				bool first = true;
	
				while (vstream)
				{
					double x1, y1, x2, y2, x, y;
		
					if (first or commaWhiteSpaces (vstream))
					{
						first = false;
	
						if (Grammar::DoubleValue (vstream, x1))
						{
							if (commaWhiteSpaces (vstream))
							{
								if (Grammar::DoubleValue (vstream, y1))
								{
									if (commaWhiteSpaces (vstream))
									{
										if (Grammar::DoubleValue (vstream, x2))
										{
											if (commaWhiteSpaces (vstream))
											{
												if (Grammar::DoubleValue (vstream, y2))
												{
													if (commaWhiteSpaces (vstream))
													{
														if (Grammar::DoubleValue (vstream, x))
														{
															if (commaWhiteSpaces (vstream))
															{
																if (Grammar::DoubleValue (vstream, y))
																{
																	if (relative)
																	{
																		x1 += ax;
																		y1 += ay;
																		x2 += ax;
																		y2 += ay;
																		x  += ax;
																		y  += ay;
																	}
	
																	math::bezier::cubic_curve (X3D::Vector2d (ax, ay), X3D::Vector2d (x1, y1), X3D::Vector2d (x2, y2), X3D::Vector2d (x, y), BEZIER_STEPS, contour);
	
																	ax = x;
																	ay = y;
																	px = x2;
																	py = y2;
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
			case 'S':
			case 's':
			{
				vstream .get ();

				bool first = true;
	
				while (vstream)
				{
					double x1, y1, x2, y2, x, y;
		
					if (first or commaWhiteSpaces (vstream))
					{
						first = false;
	
						if (Grammar::DoubleValue (vstream, x2))
						{
							if (commaWhiteSpaces (vstream))
							{
								if (Grammar::DoubleValue (vstream, y2))
								{
									if (commaWhiteSpaces (vstream))
									{
										if (Grammar::DoubleValue (vstream, x))
										{
											if (commaWhiteSpaces (vstream))
											{
												if (Grammar::DoubleValue (vstream, y))
												{
													if (relative)
													{
														x2 += ax;
														y2 += ay;
														x  += ax;
														y  += ay;
													}

													switch (previous)
													{
														case 'C':
														case 'c':
														case 'S':
														case 's':
														{
															x1 = ax + (ax - px);
															y1 = ay + (ay - py);
															break;
														}
														default:
														{
															x1 = ax;
															y1 = ay;
															break;
														}
													}

													math::bezier::cubic_curve (X3D::Vector2d (ax, ay), X3D::Vector2d (x1, y1), X3D::Vector2d (x2, y2), X3D::Vector2d (x, y), BEZIER_STEPS, contour);
	
													ax = x;
													ay = y;
													px = x2;
													py = y2;
													continue;
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
			case 'Z':
			case 'z':
			{
				vstream .get ();

				if (not contour .empty ())
				{
					ax = contour .front () .x ();
					ay = contour .front () .y ();
	
					contours .emplace_back (std::move (contour));
				}
	
				continue;
			}
			default:
				break;
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

		pair [0] = basic::trim (pair [0]);
		pair [1] = basic::trim (pair [1]);

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
			else if (pair [1] == "inherit")
			{
			}
		}
		else if (pair [0] == "fill-opacity")
		{
			double fillOpacity;

			if (Grammar::DoubleValue (vstream, fillOpacity))
			{
				styleObject .fillOpacity = fillOpacity;
			}
			else if (pair [1] == "transparent")
			{
				styleObject .fillOpacity = 0;
			}
			else if (pair [1] == "inherit")
			{
			}
		}
		else if (pair [0] == "stroke")
		{
			X3D::Color3f color;

			if (colorValue (vstream, color))
			{
				styleObject .strokeSet = true;
				styleObject .stroke    = color;
			}
			else if (pair [1] == "inherit")
			{
			}
		}
		else if (pair [0] == "stroke-opacity")
		{
			double strokeOpacity;

			if (Grammar::DoubleValue (vstream, strokeOpacity))
			{
				styleObject .strokeOpacity = strokeOpacity;
			}
			else if (pair [1] == "transparent")
			{
				styleObject .strokeOpacity = 0;
			}
			else if (pair [1] == "inherit")
			{
			}
		}
		else if (pair [0] == "stroke-width")
		{
			double strokeWidth;

			if (Grammar::DoubleValue (vstream, strokeWidth))
			{
				styleObject .strokeWidth = strokeWidth;
			}
			else if (pair [1] == "none")
			{
				styleObject .strokeWidth = 0;
			}
			else if (pair [1] == "inherit")
			{
			}
		}
		else if (pair [0] == "opacity")
		{
			double opacity;

			if (Grammar::DoubleValue (vstream, opacity))
			{
				styleObject .opacity = opacity;
			}
			else if (pair [1] == "transparent")
			{
				styleObject .opacity = 0;
			}
			else if (pair [1] == "inherit")
			{
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

		return false;
	}

	if (Grammar::rgb (istream))
	{
		int32_t r, g, b;

		if (Grammar::OpenParenthesis (istream))
		{
			whiteSpaces (istream);

			if (Grammar::IntegerValue (istream, r))
			{
				whiteSpaces (istream);

				bool rp = Grammar::PercentSign (istream);
				
				if (commaWhiteSpaces (istream))
				{
					if (Grammar::IntegerValue (istream, g))
					{
						whiteSpaces (istream);

						bool gp = Grammar::PercentSign (istream);
						
						if (commaWhiteSpaces (istream))
						{
							if (Grammar::IntegerValue (istream, b))
							{
								whiteSpaces (istream);

								bool bp = Grammar::PercentSign (istream);
								
								whiteSpaces (istream);

								if (Grammar::CloseParenthesis (istream))
								{
									color = make_rgb <float> (rp ? r / 100.0 * 255.0 : r,
									                          gp ? g / 100.0 * 255.0 : g,
									                          bp ? b / 100.0 * 255.0 : b);

									return true;
								}
							}
						}
					}
				}
			}
		}

		return false;
	}

	std::string colorName;

	if (Grammar::NamedColor (istream, colorName))
	{
		try
		{
			std::transform (colorName .begin (), colorName .end (), colorName .begin (), [ ] (const char c) { return std::tolower (c, std::locale::classic ()); });

			color = namedColors .at (colorName);
			return true;
		}
		catch (const std::out_of_range &)
		{ }
	}

	return false;
}

X3D::X3DPtr <X3D::Transform>
Parser::getTransform (xmlpp::Element* const xmlElement, const X3D::Vector2d & translation, const X3D::Vector2d & scale)
{
	// Determine matrix.

	X3D::Matrix3d matrix;

	transformAttribute (xmlElement -> get_attribute ("transform"), matrix);

	matrix .translate (translation);
	matrix .scale (scale);

	// Create node.

	const auto transform = scene -> createNode <X3D::Transform> ();

	transform -> setMatrix (X3D::Matrix4d (matrix [0] [0], matrix [0] [1], 0, 0,
	                                       matrix [1] [0], matrix [1] [1], 0, 0,
	                                       0, 0, 1, 0,
	                                       matrix [2] [0], -matrix [2] [1], 0, 1));

	// Set name.

	idAttribute (xmlElement -> get_attribute ("id"), X3D::SFNode (transform));

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

X3D::X3DPtr <X3D::Appearance>
Parser::getStrokeAppearance ()
{
	const auto appearance = scene -> createNode <X3D::Appearance> ();
	const auto material   = scene -> createNode <X3D::Material> ();

	appearance -> material ()    = material;
	material -> emissiveColor () = getStroke ();
	material -> transparency ()  = 1 - getStrokeOpacity ();

	return appearance;
}

bool
Parser::getStrokeSet () const
{
	for (const auto & style : styles)
	{
		if (style .strokeSet)
			return true;
	}

	return false;
}

X3D::Color3f
Parser::getStroke () const
{
	for (const auto & style : styles)
	{
		if (style .strokeSet)
			return style .stroke;
	}

	return X3D::Color3f ();
}

double
Parser::getStrokeOpacity () const
{
	double opacity = 1;

	for (const auto & style : styles)
	{
		opacity *= style .opacity;

		if (style .strokeSet)
			return style .strokeOpacity * opacity;
	}

	return 0;
}

double
Parser::getStrokeWidth () const
{
	for (const auto & style : styles)
	{
		if (style .strokeSet)
			return style .strokeWidth;
	}

	return 1;
}

Parser::~Parser ()
{ }

} // SVG
} // X3D
} // titania
