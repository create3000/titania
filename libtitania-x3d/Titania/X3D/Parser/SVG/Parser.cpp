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
#include "../../Components/Grouping/Switch.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Navigation/OrthoViewpoint.h"
#include "../../Components/Networking/Anchor.h"
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
#include <Titania/InputOutput/InverseCharacter.h>
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
	static const io::string url;

	static const io::character         OpenParenthesis;
	static const io::character         CloseParenthesis;
	static const io::inverse_character iCloseParenthesis;
	static const io::character         Comma;
	static const io::character         NumberSign;

	static const io::string em;
	static const io::string ex;
	static const io::string px;
	static const io::string in;
	static const io::string cm;
	static const io::string mm;
	static const io::string pt;
	static const io::string pc;
	static const io::character PercentSign;

	static const io::number <double>   DoubleValue;
	static const io::number <int32_t>  IntegerValue;
	static const io::hex <int32_t>     HexValue;
	static const io::sequence          NamedColor;
};

const io::sequence Grammar::WhiteSpaces ("\r\n \t");

const io::string Grammar::matrix ("matrix");
const io::string Grammar::translate ("translate");
const io::string Grammar::rotate ("rotate");
const io::string Grammar::scale ("scale");
const io::string Grammar::skewX ("skewX");
const io::string Grammar::skewY ("skewY");
const io::string Grammar::rgb ("rgb", io::CASE_INSENSITIVE);
const io::string Grammar::url ("url", io::CASE_INSENSITIVE);

const io::character         Grammar::OpenParenthesis ('(');
const io::character         Grammar::CloseParenthesis (')');
const io::inverse_character Grammar::iCloseParenthesis (')');
const io::character         Grammar::Comma (',');
const io::character         Grammar::NumberSign ('#');

const io::string    Grammar::em ("em");
const io::string    Grammar::ex ("ex");
const io::string    Grammar::px ("px");
const io::string    Grammar::in ("in");
const io::string    Grammar::cm ("cm");
const io::string    Grammar::mm ("mm");
const io::string    Grammar::pt ("pt");
const io::string    Grammar::pc ("pc");
const io::character Grammar::PercentSign ('%');

const io::number <double>   Grammar::DoubleValue;
const io::number <int32_t>  Grammar::IntegerValue;
const io::hex <int32_t>     Grammar::HexValue;
const io::sequence          Grammar::NamedColor ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

// Parser

static constexpr size_t BEZIER_STEPS = 9;

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	      X3D::X3DParser (),
	               scene (scene),
	                 uri (uri),
	             istream (istream),
	           xmlParser (new xmlpp::DomParser ()),
	              styles (),
	       rootTransform (scene -> createNode <X3D::Transform> ()),
	              groups ({ rootTransform }),
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

	const auto translation = X3D::Vector3d (-viewBox .x (), viewBox .y (), 0);
	const auto scale       = X3D::Vector3d (math::pixel <double> * width / viewBox [2], math::pixel <double> * height / viewBox [3], 1);

	rootTransform -> translation () = translation * scale;
	rootTransform -> scale ()       = scale;

	scene -> updateNamedNode (get_name_from_uri (uri), X3D::SFNode (rootTransform));
	scene -> getRootNodes () .emplace_back (rootTransform);

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
		std::make_pair ("use",      std::mem_fn (&Parser::useElement)),
		std::make_pair ("g",        std::mem_fn (&Parser::groupElement)),
		std::make_pair ("switch",   std::mem_fn (&Parser::switchElement)),
		std::make_pair ("a",        std::mem_fn (&Parser::aElement)),
		std::make_pair ("rect",     std::mem_fn (&Parser::rectangleElement)),
		std::make_pair ("circle",   std::mem_fn (&Parser::circleElement)),
		std::make_pair ("ellipse",  std::mem_fn (&Parser::ellipseElement)),
		std::make_pair ("polygon",  std::mem_fn (&Parser::polygonElement)),
		std::make_pair ("text",     std::mem_fn (&Parser::textElement)),
		std::make_pair ("image",    std::mem_fn (&Parser::imageElement)),
		std::make_pair ("polyline", std::mem_fn (&Parser::polylineElement)),
		std::make_pair ("path",     std::mem_fn (&Parser::pathElement)),
	};

	try
	{
		if (not xmlElement)
			return;

		if (isUsed (xmlElement))
			return;

		elementsIndex .at (xmlElement -> get_name ()) (this, xmlElement);
	}
	catch (const std::out_of_range &)
	{ }
}

void
Parser::useElement (xmlpp::Element* const xmlElement)
{
	// Get href.

	basic::uri href;

	urlAttribute (xmlElement -> get_attribute ("href", "xlink"), href);

	// Get transform.	

	double x      = 0;
	double y      = 0;
	double width  = 0;
	double height = 0;

	lengthAttribute (xmlElement -> get_attribute ("x"),      x);
	lengthAttribute (xmlElement -> get_attribute ("y"),      y);
	lengthAttribute (xmlElement -> get_attribute ("width"),  width);
	lengthAttribute (xmlElement -> get_attribute ("height"), height);

	const auto transform = getTransform (xmlElement, X3D::Vector2d (x, y));

	groups .emplace_back (transform);

	try
	{
		// Get named node.
	
		const auto node = scene -> getNamedNode (get_name_from_string (href .fragment ()));

		transform -> children () .emplace_back (node);
	}
	catch (const X3D::X3DError &)
	{
		const auto xpath    = "//*[@id='" + href .fragment () + "']";
		const auto xmlNodes = xmlParser -> get_document () -> get_root_node () -> find (xpath);

		for (const auto & xmlNode : xmlNodes)
			element (dynamic_cast <xmlpp::Element*> (xmlNode));
	}

	groups .pop_back ();

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);
}

bool
Parser::isUsed (xmlpp::Element* const xmlElement)
{
	try
	{
		// Get id.
	
		std::string id;

		stringAttribute (xmlElement -> get_attribute ("id"), id);

		// Get named node.
	
		const auto node = scene -> getNamedNode (get_name_from_string (id));

		groups .back () -> children () .emplace_back (node);

		return true;
	}
	catch (const X3D::X3DError &)
	{
		return false;
	}
}

void
Parser::groupElement (xmlpp::Element* const xmlElement)
{
	// Determine style.

	Style style;

	styleAttributes (xmlElement, style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Get transform.	

	const auto transform = getTransform (xmlElement);

	// Get child elements.

	groups .emplace_back (transform);

	elements (xmlElement);

	groups .pop_back ();
	styles .pop_back ();

	// Add node.

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);
}

void
Parser::switchElement (xmlpp::Element* const xmlElement)
{
	// Determine style.

	Style style;

	styleAttributes (xmlElement, style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Create node.

	const auto transform  = getTransform (xmlElement);
	const auto switchNode = scene -> createNode <X3D::Switch> ();

	transform -> children () .emplace_back (switchNode);

	switchNode -> whichChoice () = 0;

	// Get child elements.

	groups .emplace_back (switchNode);

	elements (xmlElement);

	groups .pop_back ();
	styles .pop_back ();

	// Add node.

	if (not switchNode -> children () .empty ())
	{
		if (transform -> getMatrix () == X3D::Matrix4d ())
			groups .back () -> children () .emplace_back (switchNode);
		else
			groups .back () -> children () .emplace_back (transform);
	}
}

void
Parser::aElement (xmlpp::Element* const xmlElement)
{
	// Determine style.

	Style style;

	styleAttributes (xmlElement, style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Get attributes.

	basic::uri  href;
	std::string title;
	std::string target;

	urlAttribute    (xmlElement -> get_attribute ("href",  "xlink"), href);
	stringAttribute (xmlElement -> get_attribute ("title", "xlink"), title);
	stringAttribute (xmlElement -> get_attribute ("target"),         target);

	// Create node.

	const auto transform = getTransform (xmlElement);
	const auto anchor    = scene -> createNode <X3D::Anchor> ();

	transform -> children () .emplace_back (anchor);

	anchor -> description () = title;
	anchor -> url ()         = { href .str () };

	if (not target .empty ())
		anchor -> parameter () = { "target=" + target };

	// Get child elements.

	groups .emplace_back (anchor);

	elements (xmlElement);

	groups .pop_back ();
	styles .pop_back ();

	// Add node.

	if (not anchor -> children () .empty ())
	{
		if (transform -> getMatrix () == X3D::Matrix4d ())
			groups .back () -> children () .emplace_back (anchor);
		else
			groups .back () -> children () .emplace_back (transform);
	}
}

void
Parser::rectangleElement (xmlpp::Element* const xmlElement)
{
	// Determine style.

	Style style;

	styleAttributes (xmlElement, style);

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

	if (getFillType () not_eq ColorType::NONE)
	{
		const auto shape     = scene -> createNode <X3D::Shape> ();
		const auto rectangle = scene -> createNode <X3D::Rectangle2D> ();

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getFillAppearance ();
		shape -> geometry ()   = rectangle;
		rectangle -> solid ()  = false;
		rectangle -> size ()   = X3D::Vector2f (width, height);
	}

	if (getStrokeType () not_eq ColorType::NONE)
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

	styleAttributes (xmlElement, style);

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

	if (getFillType () not_eq ColorType::NONE)
	{
		const auto shape = scene -> createNode <X3D::Shape> ();
		const auto disk  = scene -> createNode <X3D::Disk2D> ();

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getFillAppearance ();
		shape -> geometry ()   = disk;
		disk -> solid ()       = false;
		disk -> outerRadius () = r;
	}

	if (getStrokeType () not_eq ColorType::NONE)
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

	styleAttributes (xmlElement, style);

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

	if (getFillType () not_eq ColorType::NONE)
	{
		const auto shape = scene -> createNode <X3D::Shape> ();
		const auto disk  = scene -> createNode <X3D::Disk2D> ();

		transform -> children () .emplace_back (shape);

		shape -> appearance () = getFillAppearance ();
		shape -> geometry ()   = disk;
		disk -> solid ()       = false;
		disk -> outerRadius () = rmin;
	}

	if (getStrokeType () not_eq ColorType::NONE)
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

	basic::uri href;
	basic::uri abshref;

	urlAttribute (xmlElement -> get_attribute ("href",   "xlink"),    href);
	urlAttribute (xmlElement -> get_attribute ("absref", "sodipodi"), abshref);

	// Create nodes.

	const auto shape      = scene -> createNode <X3D::Shape> ();
	const auto appearance = scene -> createNode <X3D::Appearance> ();
	const auto texture    = scene -> createNode <X3D::ImageTexture> ();
	const auto rectangle  = scene -> createNode <X3D::Rectangle2D> ();

	transform -> children () .emplace_back (shape);

	shape -> appearance ()   = appearance;
	shape -> geometry ()     = rectangle;
	appearance -> texture () = texture;
	texture -> url ()        = { href .str () };
	rectangle -> solid ()    = false;
	rectangle -> size ()     = X3D::Vector2f (width, height);

	if (not abshref .empty ())
		texture -> url () .emplace_back (abshref .str ());

	groups .back () -> children () .emplace_back (transform);
}

void
Parser::polylineElement (xmlpp::Element* const xmlElement)
{
	using Tesselator = math::tessellator <double, size_t>;

	// Get path points.

	std::vector <X3D::Vector2d> points;

	if (not pointsAttribute (xmlElement -> get_attribute ("points"), points))
		return;

	// Determine style.

	Style style;

	styleAttributes (xmlElement, style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Get transform.	

	const auto transform = getTransform (xmlElement);

	// Create nodes.

	const auto coordinate = scene -> createNode <X3D::Coordinate> ();

	for (const auto & point : points)
		coordinate -> point () .emplace_back (point .x (), -point .y (), 0);

	if (getFillType () not_eq ColorType::NONE)
	{
		// Tesselate contours

		Tesselator tessellator;

		tessellator .property (GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

		tessellator .property (GLU_TESS_TOLERANCE, 0);
		tessellator .normal (Vector3d (0, 0, 1));
		tessellator .begin_polygon ();
		tessellator .begin_contour ();

		size_t index = 0;

		for (const auto & point : points)
		{
			tessellator .add_vertex (X3D::Vector3d (point .x (), point .y (), 0), index ++);
		}

		tessellator .end_contour ();
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

			for (const auto & point : points)
				coordinate -> point () .emplace_back (point .x (), -point .y (), 0);
		}
	}

	if (getStrokeType () not_eq ColorType::NONE and points .size () > 1)
	{
		// Create geometry.

		const auto shape    = scene -> createNode <X3D::Shape> ();
		const auto geometry = scene -> createNode <X3D::IndexedLineSet> ();

		shape -> appearance () = getStrokeAppearance ();
		shape -> geometry ()   = geometry;
		geometry -> coord ()   = coordinate;

		size_t index = 0;

		for (size_t i = 0, size = points .size (); i < size; ++ i)
			geometry -> coordIndex () .emplace_back (index ++);

		geometry -> coordIndex () .emplace_back (-1);

		transform -> children () .emplace_back (shape);
	}

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);

	styles .pop_back ();
}

void
Parser::polygonElement (xmlpp::Element* const xmlElement)
{
	using Tesselator = math::tessellator <double, size_t>;

	// Get path points.

	std::vector <X3D::Vector2d> points;

	if (not pointsAttribute (xmlElement -> get_attribute ("points"), points))
		return;

	if (not points .empty ())
		points .emplace_back (points .front ());

	// Determine style.

	Style style;

	styleAttributes (xmlElement, style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Get transform.	

	const auto transform = getTransform (xmlElement);

	// Create nodes.

	const auto coordinate = scene -> createNode <X3D::Coordinate> ();

	for (const auto & point : points)
		coordinate -> point () .emplace_back (point .x (), -point .y (), 0);

	if (getFillType () not_eq ColorType::NONE)
	{
		// Tesselate contours

		Tesselator tessellator;

		tessellator .property (GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

		tessellator .property (GLU_TESS_TOLERANCE, 0);
		tessellator .normal (Vector3d (0, 0, 1));
		tessellator .begin_polygon ();
		tessellator .begin_contour ();

		size_t index = 0;

		for (const auto & point : points)
		{
			tessellator .add_vertex (X3D::Vector3d (point .x (), point .y (), 0), index ++);
		}

		tessellator .end_contour ();
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

			for (const auto & point : points)
				coordinate -> point () .emplace_back (point .x (), -point .y (), 0);
		}
	}

	if (getStrokeType () not_eq ColorType::NONE and points .size () > 1)
	{
		// Create geometry.

		const auto shape    = scene -> createNode <X3D::Shape> ();
		const auto geometry = scene -> createNode <X3D::IndexedLineSet> ();

		shape -> appearance () = getStrokeAppearance ();
		shape -> geometry ()   = geometry;
		geometry -> coord ()   = coordinate;

		size_t index = 0;

		for (size_t i = 0, size = points .size (); i < size; ++ i)
			geometry -> coordIndex () .emplace_back (index ++);

		geometry -> coordIndex () .emplace_back (-1);

		transform -> children () .emplace_back (shape);
	}

	if (not transform -> children () .empty ())
		groups .back () -> children () .emplace_back (transform);

	styles .pop_back ();
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

	styleAttributes (xmlElement, style);

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

	if (getFillType () not_eq ColorType::NONE)
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

	if (getStrokeType () not_eq ColorType::NONE)
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
				for (size_t i = 0, size = contour .size (); i < size; ++ i)
					geometry -> coordIndex () .emplace_back (index ++);
	
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

	return true;
}

bool
Parser::urlAttribute (xmlpp::Attribute* const xmlAttribute, basic::uri & value)
{
	if (not xmlAttribute)
		return false;

	value = xmlAttribute -> get_value ();

	return true;
}

bool
Parser::pointsAttribute (xmlpp::Attribute* const xmlAttribute, std::vector <X3D::Vector2d> & points)
{
	if (not xmlAttribute)
		return false;

	std::istringstream vstream (xmlAttribute -> get_value ());

	vstream .imbue (std::locale::classic ());

	whiteSpaces (vstream);

	while (vstream)
	{
		double x, y;

		if (Grammar::DoubleValue (vstream, x))
		{
			if (commaWhiteSpaces (vstream))
			{
				if (Grammar::DoubleValue (vstream, y))
				{
					points .emplace_back (x, y);

					if (commaWhiteSpaces (vstream))
						continue;
				}
			}
		}

		break;
	}

	return true;
}

bool
Parser::dAttribute (xmlpp::Attribute* const xmlAttribute, Contours & contours)
{
	if (not xmlAttribute)
		return false;

	std::istringstream vstream (xmlAttribute -> get_value ());

	vstream .imbue (std::locale::classic ());

	Contour contour;

	double ax = 0, ay = 0, px = 0, py = 0;

	std::istringstream::int_type peek = -1, previous = -1;

	whiteSpaces (vstream);

	while (vstream)
	{
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

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x, y;
		
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

								if (commaWhiteSpaces (vstream))
									continue;
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

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x, y;
		
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

								if (commaWhiteSpaces (vstream))
									continue;
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

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x;
		
					if (Grammar::DoubleValue (vstream, x))
					{
						if (relative)
							x += ax;

						contour .emplace_back (x, ay);

						ax = x;

						if (commaWhiteSpaces (vstream))
							continue;
					}
	
					break;
				}
	
				continue;
			}
			case 'V':
			case 'v':
			{
				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double y;
		
					if (Grammar::DoubleValue (vstream, y))
					{
						if (relative)
							y += ay;

						contour .emplace_back (ax, y);

						ay = y;

						if (commaWhiteSpaces (vstream))
							continue;
					}
	
					break;
				}
	
				continue;
			}
			case 'Q':
			case 'q':
			{
				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x1, y1, x, y;
		
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

												if (commaWhiteSpaces (vstream))
													continue;
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

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x1, y1, x, y;
		
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

								if (commaWhiteSpaces (vstream))
									continue;
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

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x1, y1, x2, y2, x, y;
		
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

																if (commaWhiteSpaces (vstream))
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
	
					break;
				}
	
				continue;
			}
			case 'S':
			case 's':
			{
				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x1, y1, x2, y2, x, y;
		
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

												if (commaWhiteSpaces (vstream))
													continue;
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
					if (contour .front () not_eq contour .back ())
						contour .emplace_back (contour .front ());

					ax = contour .back () .x ();
					ay = contour .back () .y ();

					contours .emplace_back (std::move (contour));
				}

				if (commaWhiteSpaces (vstream))
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

void
Parser::styleAttributes (xmlpp::Element* const xmlElement, Style & styleObject)
{
	using StyleFunction = std::function <void (Parser*, const std::string &, Style &)>;

	static const std::map <std::string, StyleFunction> stylesIndex =
	{
		std::make_pair ("display",        std::mem_fn (&Parser::displayAttribute)),
		std::make_pair ("fill",           std::mem_fn (&Parser::fillAttribute)),
		std::make_pair ("fill-opacity",   std::mem_fn (&Parser::fillOpacityAttribute)),
		std::make_pair ("stroke",         std::mem_fn (&Parser::strokeAttribute)),
		std::make_pair ("stroke-opacity", std::mem_fn (&Parser::strokeOpacityAttribute)),
		std::make_pair ("stroke-width",   std::mem_fn (&Parser::strokeWidthAttribute)),
		std::make_pair ("opacity",        std::mem_fn (&Parser::opacityAttribute)),
	};

	for (const auto & attribute : xmlElement -> get_attributes ())
	{
		try
		{
			stylesIndex .at (attribute -> get_name ()) (this, basic::trim (attribute -> get_value ()), styleObject);
		}
		catch (const std::out_of_range &)
		{ }
	}

	// Style attribute has higher precedence.

	styleAttribute (xmlElement -> get_attribute ("style"), styleObject);
}

bool
Parser::styleAttribute (xmlpp::Attribute* const xmlAttribute, Style & styleObject)
{
	using StyleFunction = std::function <void (Parser*, const std::string &, Style &)>;

	static const std::map <std::string, StyleFunction> stylesIndex =
	{
		std::make_pair ("display",        std::mem_fn (&Parser::displayAttribute)),
		std::make_pair ("fill",           std::mem_fn (&Parser::fillAttribute)),
		std::make_pair ("fill-opacity",   std::mem_fn (&Parser::fillOpacityAttribute)),
		std::make_pair ("stroke",         std::mem_fn (&Parser::strokeAttribute)),
		std::make_pair ("stroke-opacity", std::mem_fn (&Parser::strokeOpacityAttribute)),
		std::make_pair ("stroke-width",   std::mem_fn (&Parser::strokeWidthAttribute)),
		std::make_pair ("opacity",        std::mem_fn (&Parser::opacityAttribute)),
	};

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

		try
		{
			stylesIndex .at (pair [0]) (this, pair [1], styleObject);
		}
		catch (const std::out_of_range &)
		{ }
	}

	return true;
}

void
Parser::displayAttribute (const std::string & value, Style & styleObject)
{
	styleObject .display = value;
}

void
Parser::fillAttribute (const std::string & value, Style & styleObject)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	X3D::Color3f color;

	if (colorValue (vstream, color))
	{
		styleObject .fillType  = ColorType::COLOR;
		styleObject .fillColor = color;
		return;
	}

	basic::uri url;

	if (urlValue (vstream, url))
	{
		styleObject .fillType = ColorType::URL;
		styleObject .fillURL  = url;
		return;
	}

	if (value == "inherit")
	{
		return;
	}
}

void
Parser::fillOpacityAttribute (const std::string & value, Style & styleObject)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double fillOpacity;

	if (Grammar::DoubleValue (vstream, fillOpacity))
	{
		styleObject .fillOpacity = fillOpacity;
		return;
	}

	if (value == "transparent")
	{
		styleObject .fillOpacity = 0;
		return;
	}

	if (value == "inherit")
	{
		return;
	}
}

void
Parser::strokeAttribute (const std::string & value, Style & styleObject)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	X3D::Color3f color;

	if (colorValue (vstream, color))
	{
		styleObject .strokeType  = ColorType::COLOR;
		styleObject .strokeColor = color;
		return;
	}

	basic::uri url;

	if (urlValue (vstream, url))
	{
		styleObject .strokeType = ColorType::URL;
		styleObject .strokeURL  = url;
		return;
	}

	if (value == "inherit")
	{
		return;
	}
}

void
Parser::strokeOpacityAttribute (const std::string & value, Style & styleObject)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double strokeOpacity;

	if (Grammar::DoubleValue (vstream, strokeOpacity))
	{
		styleObject .strokeOpacity = strokeOpacity;
		return;
	}

	if (value == "transparent")
	{
		styleObject .strokeOpacity = 0;
		return;
	}

	if (value == "inherit")
	{
		return;
	}
}

void
Parser::strokeWidthAttribute (const std::string & value, Style & styleObject)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double strokeWidth;

	if (Grammar::DoubleValue (vstream, strokeWidth))
	{
		styleObject .strokeWidth = strokeWidth;
		return;
	}

	if (value == "none")
	{
		styleObject .strokeWidth = 0;
		return;
	}

	if (value == "inherit")
	{
		return;
	}
}

void
Parser::opacityAttribute (const std::string & value, Style & styleObject)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double opacity;

	if (Grammar::DoubleValue (vstream, opacity))
	{
		styleObject .opacity = opacity;
		return;
	}

	if (value == "transparent")
	{
		styleObject .opacity = 0;
		return;
	}

	if (value == "inherit")
	{
		return;
	}
}

bool
Parser::colorValue (std::istream & istream, X3D::Color3f & color)
{
	if (Grammar::NumberSign (istream))
	{
		const auto pos = istream .tellg ();

		int32_t number;

		if (Grammar::HexValue (istream, number))
		{
			istream .clear ();

			if (istream .tellg () - pos == 3)
			{
				// Shorthand hex color

				static const auto sc = [ ] (const uint32_t c) { return c << 4 | c; };

				float b = sc ((number >>= 0) & 0xf) / float (0xff);
				float g = sc ((number >>= 4) & 0xf) / float (0xff);
				float r = sc ((number >>= 4) & 0xf) / float (0xff);
	
				color = X3D::Color3f (r, g, b);
				return true;
			}

			float b = ((number >>= 0) & 0xff) / float (0xff);
			float g = ((number >>= 8) & 0xff) / float (0xff);
			float r = ((number >>= 8) & 0xff) / float (0xff);

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
		{
			for (size_t i = 0, size = colorName .size (); i < size; ++ i)
				istream .unget ();
		}
	}

	return false;
}

bool
Parser::urlValue (std::istream & istream, basic::uri & url)
{
	if (Grammar::url (istream))
	{
		if (Grammar::OpenParenthesis (istream))
		{
			std::string value;

			if (Grammar::iCloseParenthesis (istream, value))
			{
				url = basic::trim (value);
				return true;
			}
		}

		return false;
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

	switch (getFillType ())
	{
		case ColorType::NONE:
			return nullptr;

		case ColorType::COLOR:
		{
			const auto material = scene -> createNode <X3D::Material> ();
		
			appearance -> material ()   = material;
			material -> diffuseColor () = getFillColor ();
			material -> transparency () = 1 - getFillOpacity ();

			break;
		}
		case ColorType::URL:
		{
			break;
		}
	}

	return appearance;
}

Parser::ColorType
Parser::getFillType () const
{
	for (const auto & style : styles)
	{
		if (style .fillType not_eq ColorType::NONE)
			return style .fillType;
	}

	return ColorType::NONE;
}

X3D::Color3f
Parser::getFillColor () const
{
	for (const auto & style : styles)
	{
		if (style .fillType not_eq ColorType::NONE)
			return style .fillColor;
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

		if (style .fillType not_eq ColorType::NONE)
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
	material -> emissiveColor () = getStrokeColor ();
	material -> transparency ()  = 1 - getStrokeOpacity ();

	return appearance;
}

Parser::ColorType
Parser::getStrokeType () const
{
	for (const auto & style : styles)
	{
		if (style .strokeType not_eq ColorType::NONE)
			return style .strokeType;
	}

	return ColorType::NONE;
}

X3D::Color3f
Parser::getStrokeColor () const
{
	for (const auto & style : styles)
	{
		if (style .strokeType not_eq ColorType::NONE)
			return style .strokeColor;
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

		if (style .strokeType not_eq ColorType::NONE)
			return style .strokeOpacity * opacity;
	}

	return 0;
}

double
Parser::getStrokeWidth () const
{
	for (const auto & style : styles)
	{
		if (style .strokeType not_eq ColorType::NONE)
			return style .strokeWidth;
	}

	return 1;
}

Parser::~Parser ()
{ }

} // SVG
} // X3D
} // titania
