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
#include "../../Components/Navigation/NavigationInfo.h"
#include "../../Components/Networking/Anchor.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../../Components/Texturing/ImageTexture.h"
#include "../../Components/Texturing/PixelTexture.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Components/Texturing/TextureProperties.h"
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

static constexpr size_t BEZIER_STEPS    = 9;
static constexpr size_t GRADIENT_WIDTH  = 256;
static constexpr size_t GRADIENT_HEIGHT = 256;

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	       X3D::X3DParser (),
	                scene (scene),
	                  uri (uri),
	              istream (istream),
	            xmlParser (new xmlpp::DomParser ()),
	               styles (1),
	        rootTransform (scene -> createNode <X3D::Transform> ()),
	           groupNodes ({ rootTransform }),
	texturePropertiesNode (scene -> createNode <X3D::TextureProperties> ()),
	          namedColors (),
	 whiteSpaceCharacters ()
{
	xmlParser -> set_throw_messages (true);
	xmlParser -> set_validate (false);
	xmlParser -> set_include_default_attributes (true);

	texturePropertiesNode -> generateMipMaps ()     = true;
	texturePropertiesNode -> minificationFilter ()  = "DEFAULT";
	texturePropertiesNode -> magnificationFilter () = "DEFAULT";
	texturePropertiesNode -> boundaryModeS ()       = "CLAMP_TO_EDGE";
	texturePropertiesNode -> boundaryModeT ()       = "CLAMP_TO_EDGE";
	texturePropertiesNode -> boundaryModeR ()       = "CLAMP_TO_EDGE";
	texturePropertiesNode -> textureCompression ()  = "DEFAULT";
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

	// Create navigation info.

	const auto navigationInfo = scene -> createNode <X3D::NavigationInfo> ();

	navigationInfo -> type () = { "PLANE_create3000.de", "EXAMINE", "ANY" };

	scene -> getRootNodes () .emplace_back (navigationInfo);

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

	try
	{
		scene -> updateNamedNode (get_name_from_uri (uri), X3D::SFNode (rootTransform));
	}
	catch (const X3D::X3DError &)
	{ }

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

	const auto transformNode = getTransform (xmlElement, X3D::Vector2d (x, y));

	groupNodes .emplace_back (transformNode);

	try
	{
		// Get named node.
	
		const auto node = scene -> getNamedNode (get_name_from_string (href .fragment ()));

		transformNode -> children () .emplace_back (node);
	}
	catch (const X3D::X3DError &)
	{
		const auto xpath    = "//*[@id='" + href .fragment () + "']";
		const auto xmlNodes = xmlParser -> get_document () -> get_root_node () -> find (xpath);

		for (const auto & xmlNode : xmlNodes)
			element (dynamic_cast <xmlpp::Element*> (xmlNode));
	}

	groupNodes .pop_back ();

	if (not transformNode -> children () .empty ())
		groupNodes .back () -> children () .emplace_back (transformNode);
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

		groupNodes .back () -> children () .emplace_back (node);

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

	const auto transformNode = getTransform (xmlElement);

	// Get child elements.

	groupNodes .emplace_back (transformNode);

	elements (xmlElement);

	groupNodes .pop_back ();
	styles .pop_back ();

	// Add node.

	if (not transformNode -> children () .empty ())
		groupNodes .back () -> children () .emplace_back (transformNode);
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

	const auto transformNode = getTransform (xmlElement);
	const auto switchNode    = scene -> createNode <X3D::Switch> ();

	transformNode -> children () .emplace_back (switchNode);

	switchNode -> whichChoice () = 0;

	// Get child elements.

	groupNodes .emplace_back (switchNode);

	elements (xmlElement);

	groupNodes .pop_back ();
	styles .pop_back ();

	// Add node.

	if (not switchNode -> children () .empty ())
	{
		if (transformNode -> getMatrix () == X3D::Matrix4d ())
			groupNodes .back () -> children () .emplace_back (switchNode);
		else
			groupNodes .back () -> children () .emplace_back (transformNode);
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

	const auto transformNode = getTransform (xmlElement);
	const auto anchorNode    = scene -> createNode <X3D::Anchor> ();

	transformNode -> children () .emplace_back (anchorNode);

	anchorNode -> description () = title;
	anchorNode -> url ()         = { href .str () };

	if (not target .empty ())
		anchorNode -> parameter () = { "target=" + target };

	// Get child elements.

	groupNodes .emplace_back (anchorNode);

	elements (xmlElement);

	groupNodes .pop_back ();
	styles .pop_back ();

	// Add node.

	if (not anchorNode -> children () .empty ())
	{
		if (transformNode -> getMatrix () == X3D::Matrix4d ())
			groupNodes .back () -> children () .emplace_back (anchorNode);
		else
			groupNodes .back () -> children () .emplace_back (transformNode);
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

	const auto size          = X3D::Vector2d (width, height);
	const auto center        = X3D::Vector2d (x + width / 2, y + height / 2);
	const auto bbox          = X3D::Box2d (size, center);
	const auto transformNode = getTransform (xmlElement, center);

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	if (style .fillType not_eq ColorType::NONE)
	{
		const auto shapeNode     = scene -> createNode <X3D::Shape> ();
		const auto rectangleNode = scene -> createNode <X3D::Rectangle2D> ();

		shapeNode -> appearance () = getFillAppearance (style, bbox);
		shapeNode -> geometry ()   = rectangleNode;
		rectangleNode -> solid ()  = false;
		rectangleNode -> size ()   = X3D::Vector2f (size);

		transformNode -> children () .emplace_back (shapeNode);
	}

	if (style .strokeType not_eq ColorType::NONE)
	{
		const auto shapeNode     = scene -> createNode <X3D::Shape> ();
		const auto polylineNode  = scene -> createNode <X3D::Polyline2D> ();
		const auto width1_2      = width / 2;
		const auto height1_2     = height / 2;

		shapeNode -> appearance () = getStrokeAppearance (style);
		shapeNode -> geometry ()   = polylineNode;

		polylineNode -> lineSegments () .emplace_back ( width1_2,  height1_2);
		polylineNode -> lineSegments () .emplace_back (-width1_2,  height1_2);
		polylineNode -> lineSegments () .emplace_back (-width1_2, -height1_2);
		polylineNode -> lineSegments () .emplace_back ( width1_2, -height1_2);
		polylineNode -> lineSegments () .emplace_back ( width1_2,  height1_2);

		transformNode -> children () .emplace_back (shapeNode);
	}

	groupNodes .pop_back ();
	styles     .pop_back ();

	if (not transformNode -> children () .empty ())
		groupNodes .back () -> children () .emplace_back (transformNode);
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

	const auto bbox          = X3D::Box2d (X3D::Vector2d (r * 2, r * 2), X3D::Vector2d (cx, cy));
	const auto transformNode = getTransform (xmlElement, X3D::Vector2d (cx, cy));

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	if (style .fillType not_eq ColorType::NONE)
	{
		const auto shapeNode = scene -> createNode <X3D::Shape> ();
		const auto diskNode  = scene -> createNode <X3D::Disk2D> ();

		shapeNode -> appearance () = getFillAppearance (style, bbox);
		shapeNode -> geometry ()   = diskNode;
		diskNode -> solid ()       = false;
		diskNode -> outerRadius () = r;

		transformNode -> children () .emplace_back (shapeNode);
	}

	if (style .strokeType not_eq ColorType::NONE)
	{
		const auto shapeNode  = scene -> createNode <X3D::Shape> ();
		const auto circleNode = scene -> createNode <X3D::Circle2D> ();

		shapeNode -> appearance () = getStrokeAppearance (style);
		shapeNode -> geometry ()   = circleNode;
		circleNode -> radius ()    = r;

		transformNode -> children () .emplace_back (shapeNode);
	}

	groupNodes .pop_back ();
	styles     .pop_back ();

	if (not transformNode -> children () .empty ())
		groupNodes .back () -> children () .emplace_back (transformNode);
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

	const auto rmin          = std::min (rx, ry);
	const auto bbox          = X3D::Box2d (X3D::Vector2d (rx * 2, ry * 2), X3D::Vector2d (cx, cy));
	const auto transformNode = getTransform (xmlElement, X3D::Vector2d (cx, cy), X3D::Vector2d (rx / rmin, ry / rmin));

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	if (style .fillType not_eq ColorType::NONE)
	{
		const auto shapeNode = scene -> createNode <X3D::Shape> ();
		const auto diskNode  = scene -> createNode <X3D::Disk2D> ();

		shapeNode -> appearance () = getFillAppearance (style, bbox);
		shapeNode -> geometry ()   = diskNode;
		diskNode -> solid ()       = false;
		diskNode -> outerRadius () = rmin;

		transformNode -> children () .emplace_back (shapeNode);
	}

	if (style .strokeType not_eq ColorType::NONE)
	{
		const auto shapeNode  = scene -> createNode <X3D::Shape> ();
		const auto circleNode = scene -> createNode <X3D::Circle2D> ();

		shapeNode -> appearance () = getStrokeAppearance (style);
		shapeNode -> geometry ()   = circleNode;
		circleNode -> radius ()    = rmin;

		transformNode -> children () .emplace_back (shapeNode);
	}

	groupNodes .pop_back ();
	styles     .pop_back ();

	if (not transformNode -> children () .empty ())
		groupNodes .back () -> children () .emplace_back (transformNode);
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

	const auto transformNode = getTransform (xmlElement, X3D::Vector2d (x + width / 2, y + height / 2));

	groupNodes .emplace_back (transformNode);

	// Get href.

	basic::uri href;
	basic::uri abshref;

	urlAttribute (xmlElement -> get_attribute ("href",   "xlink"),    href);
	urlAttribute (xmlElement -> get_attribute ("absref", "sodipodi"), abshref);

	// Create nodes.

	const auto shapeNode      = scene -> createNode <X3D::Shape> ();
	const auto appearanceNode = scene -> createNode <X3D::Appearance> ();
	const auto textureNode    = scene -> createNode <X3D::ImageTexture> ();
	const auto rectangleNode  = scene -> createNode <X3D::Rectangle2D> ();

	shapeNode -> appearance ()          = appearanceNode;
	shapeNode -> geometry ()            = rectangleNode;
	appearanceNode -> texture ()        = textureNode;
	textureNode -> url ()               = { href .str () };
	textureNode -> textureProperties () = texturePropertiesNode;
	rectangleNode -> solid ()           = false;
	rectangleNode -> size ()            = X3D::Vector2f (width, height);

	if (not abshref .empty ())
		textureNode -> url () .emplace_back (abshref .str ());

	transformNode -> children () .emplace_back (shapeNode);

	groupNodes .pop_back ();

	groupNodes .back () -> children () .emplace_back (transformNode);
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

	const auto bbox          = X3D::Box2d (points .begin (), points .end (), math::iterator_type ());
	const auto transformNode = getTransform (xmlElement);

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	const auto coordinateNode = scene -> createNode <X3D::Coordinate> ();

	for (const auto & point : points)
		coordinateNode -> point () .emplace_back (point .x (), -point .y (), 0);

	if (style .fillType not_eq ColorType::NONE)
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
	
			const auto shapeNode    = scene -> createNode <X3D::Shape> ();
			const auto geometryNode = scene -> createNode <X3D::IndexedFaceSet> ();

			try
			{
				const auto texCoordNode  = scene -> createNode <X3D::TextureCoordinate> ();
				const auto invBBoxMatrix = inverse (bbox .matrix ());

				for (const auto & point : points)
				{
					const auto t = (point * invBBoxMatrix + X3D::Vector2d (1, 1)) / 2.0;
	
					texCoordNode -> point () .emplace_back (t .x (), 1 - t .y ());
				}

				geometryNode -> texCoord () = texCoordNode;
			}
			catch (const std::domain_error &)
			{ }

			shapeNode -> appearance ()  = getFillAppearance (style, bbox);
			shapeNode -> geometry ()    = geometryNode;
			geometryNode -> solid ()    = false;
			geometryNode -> coord ()    = coordinateNode;

			for (size_t i = 0, size = triangles .size (); i < size; i += 3)
			{
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 0] .data ()));
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 1] .data ()));
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 2] .data ()));
				geometryNode -> coordIndex () .emplace_back (-1);
			}

			for (const auto & point : points)
				coordinateNode -> point () .emplace_back (point .x (), -point .y (), 0);
	
			transformNode -> children () .emplace_back (shapeNode);
		}
	}

	if (style .strokeType not_eq ColorType::NONE and points .size () > 1)
	{
		// Create geometry.

		const auto shapeNode    = scene -> createNode <X3D::Shape> ();
		const auto geometryNode = scene -> createNode <X3D::IndexedLineSet> ();

		shapeNode -> appearance () = getStrokeAppearance (style);
		shapeNode -> geometry ()   = geometryNode;
		geometryNode -> coord ()   = coordinateNode;

		size_t index = 0;

		for (size_t i = 0, size = points .size (); i < size; ++ i)
			geometryNode -> coordIndex () .emplace_back (index ++);

		geometryNode -> coordIndex () .emplace_back (-1);

		transformNode -> children () .emplace_back (shapeNode);
	}

	groupNodes .pop_back ();
	styles     .pop_back ();

	if (not transformNode -> children () .empty ())
		groupNodes .back () -> children () .emplace_back (transformNode);
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

	const auto bbox          = X3D::Box2d (points .begin (), points .end (), math::iterator_type ());
	const auto transformNode = getTransform (xmlElement);

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	const auto coordinateNode = scene -> createNode <X3D::Coordinate> ();

	for (const auto & point : points)
		coordinateNode -> point () .emplace_back (point .x (), -point .y (), 0);

	if (style .fillType not_eq ColorType::NONE)
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
	
			const auto shapeNode    = scene -> createNode <X3D::Shape> ();
			const auto geometryNode = scene -> createNode <X3D::IndexedFaceSet> ();

			try
			{
				const auto texCoordNode  = scene -> createNode <X3D::TextureCoordinate> ();
				const auto invBBoxMatrix = inverse (bbox .matrix ());
	
				for (const auto & point : points)
				{
					const auto t = (point * invBBoxMatrix + X3D::Vector2d (1, 1)) / 2.0;
	
					texCoordNode -> point () .emplace_back (t .x (), 1 - t .y ());
				}

				geometryNode -> texCoord () = texCoordNode;
			}
			catch (const std::domain_error &)
			{ }
	
			shapeNode -> appearance () = getFillAppearance (style, bbox);
			shapeNode -> geometry ()   = geometryNode;
			geometryNode -> solid ()   = false;
			geometryNode -> coord ()   = coordinateNode;
	
			for (size_t i = 0, size = triangles .size (); i < size; i += 3)
			{
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 0] .data ()));
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 1] .data ()));
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 2] .data ()));
				geometryNode -> coordIndex () .emplace_back (-1);
			}

			for (const auto & point : points)
				coordinateNode -> point () .emplace_back (point .x (), -point .y (), 0);

			transformNode -> children () .emplace_back (shapeNode);
		}
	}

	if (style .strokeType not_eq ColorType::NONE and points .size () > 1)
	{
		// Create geometry.

		const auto shapeNode    = scene -> createNode <X3D::Shape> ();
		const auto geometryNode = scene -> createNode <X3D::IndexedLineSet> ();

		shapeNode -> appearance () = getStrokeAppearance (style);
		shapeNode -> geometry ()   = geometryNode;
		geometryNode -> coord ()   = coordinateNode;

		size_t index = 0;

		for (size_t i = 0, size = points .size (); i < size; ++ i)
			geometryNode -> coordIndex () .emplace_back (index ++);

		geometryNode -> coordIndex () .emplace_back (-1);

		transformNode -> children () .emplace_back (shapeNode);
	}

	groupNodes .pop_back ();
	styles     .pop_back ();

	if (not transformNode -> children () .empty ())
		groupNodes .back () -> children () .emplace_back (transformNode);
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

	auto bbox = X3D::Box2d ();

	for (const auto & contour : contours)
		bbox += X3D::Box2d (contour .begin (), contour .end (), math::iterator_type ());

	const auto transformNode = getTransform (xmlElement);

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	const auto coordinateNode = scene -> createNode <X3D::Coordinate> ();

	for (const auto & contour : contours)
	{
		for (const auto & point : contour)
			coordinateNode -> point () .emplace_back (point .x (), -point .y (), 0);
	}

	if (style .fillType not_eq ColorType::NONE)
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
	
			const auto shapeNode     = scene -> createNode <X3D::Shape> ();
			const auto geometryNode  = scene -> createNode <X3D::IndexedFaceSet> ();

			try
			{
				const auto texCoordNode  = scene -> createNode <X3D::TextureCoordinate> ();
				const auto invBBoxMatrix = inverse (bbox .matrix ());
	
				for (const auto & contour : contours)
				{
					for (const auto & point : contour)
					{
						const auto t = (point * invBBoxMatrix + X3D::Vector2d (1, 1)) / 2.0;
	
						texCoordNode -> point () .emplace_back (t .x (), 1 - t .y ());
					}
				}

				geometryNode -> texCoord () = texCoordNode;
			}
			catch (const std::domain_error &)
			{ }
	
			shapeNode -> appearance ()  = getFillAppearance (style, bbox);
			shapeNode -> geometry ()    = geometryNode;
			geometryNode -> solid ()    = false;
			geometryNode -> coord ()    = coordinateNode;
	
			for (size_t i = 0, size = triangles .size (); i < size; i += 3)
			{
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 0] .data ()));
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 1] .data ()));
				geometryNode -> coordIndex () .emplace_back (std::get <0> (triangles [i + 2] .data ()));
				geometryNode -> coordIndex () .emplace_back (-1);
			}

			for (const auto & contour : contours)
			{
				for (const auto & point : contour)
					coordinateNode -> point () .emplace_back (point .x (), -point .y (), 0);
			}

			transformNode -> children () .emplace_back (shapeNode);
		}
	}

	if (style .strokeType not_eq ColorType::NONE)
	{
		// Create geometry.

		const auto shapeNode    = scene -> createNode <X3D::Shape> ();
		const auto geometryNode = scene -> createNode <X3D::IndexedLineSet> ();

		shapeNode -> appearance () = getStrokeAppearance (style);
		shapeNode -> geometry ()   = geometryNode;
		geometryNode -> coord ()   = coordinateNode;

		size_t index = 0;

		for (const auto & contour : contours)
		{
			if (contour .size () > 1)
			{
				for (size_t i = 0, size = contour .size (); i < size; ++ i)
					geometryNode -> coordIndex () .emplace_back (index ++);
	
				geometryNode -> coordIndex () .emplace_back (-1);
			}
		}

		if (not geometryNode -> coordIndex () .empty ())
			transformNode -> children () .emplace_back (shapeNode);
	}

	groupNodes .pop_back ();
	styles     .pop_back ();

	if (not transformNode -> children () .empty ())
		groupNodes .back () -> children () .emplace_back (transformNode);
}

bool
Parser::paintURL (const basic::uri & url,
                  const X3D::Box2d & bbox,
                  const Cairo::RefPtr <Cairo::Context> & context)
{
	using ElementsFunction = std::function <void (Parser*, xmlpp::Element* const, const X3D::Box2d &, const Cairo::RefPtr <Cairo::Context> &)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("linearGradient", std::mem_fn (&Parser::paintLinearGradientElement)),
		std::make_pair ("radialGradient", std::mem_fn (&Parser::paintRadialGradientElement)),
	};

	try
	{
		const auto xpath    = "//*[@id='" + url .fragment () + "']";
		const auto xmlNodes = xmlParser -> get_document () -> get_root_node () -> find (xpath);

		if (xmlNodes .empty ())
			return false;

		const auto xmlElement = dynamic_cast <xmlpp::Element*> (xmlNodes .back ());

		if (not xmlElement)
			return false;

		elementsIndex .at (xmlElement -> get_name ()) (this, xmlElement, bbox, context);
		return true;
	}
	catch (const std::out_of_range &)
	{
		return false;
	}
}

void
Parser::paintLinearGradientElement (xmlpp::Element* const xmlElement,
                                    const X3D::Box2d & bbox,
                                    const Cairo::RefPtr <Cairo::Context> & context)
{
	Gradient gradient;

	linearGradientElement (xmlElement, gradient);

	const auto linearGradient = Cairo::LinearGradient::create (gradient .x1, gradient .y1, gradient .x2, gradient .y2);

	for (const auto & stop : gradient .stops)
		linearGradient -> add_color_stop_rgba (stop .first, stop .second .r (), stop .second .g (), stop .second .b (), stop .second .a ());

	X3D::Matrix3d m;

	m .scale     (X3D::Vector2d (GRADIENT_WIDTH / 2.0, GRADIENT_HEIGHT / 2.0));
	m .translate (X3D::Vector2d (1, 1));
	m .mult_left (inverse (bbox .matrix ()));
	m .mult_left (gradient .gradientTransform);

	const auto matrix = Cairo::Matrix (m [0] [0], m [1] [0], m [0] [1], m [1] [1], m [2] [0], m [2] [1]);

	context -> set_matrix (matrix);
	context -> set_source (linearGradient);
	context -> set_identity_matrix ();
	context -> rectangle (0, 0, GRADIENT_WIDTH, GRADIENT_HEIGHT);
	context -> fill ();
}

void
Parser::gradientElement (xmlpp::Element* const xmlElement, Gradient & gradient)
{
	using ElementsFunction = std::function <void (Parser*, xmlpp::Element* const, Gradient &)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("linearGradient", std::mem_fn (&Parser::linearGradientElement)),
		std::make_pair ("radialGradient", std::mem_fn (&Parser::radialGradientElement)),
	};

	try
	{
		if (not xmlElement)
			return;

		elementsIndex .at (xmlElement -> get_name ()) (this, xmlElement, gradient);
	}
	catch (const std::out_of_range &)
	{ }
}

void
Parser::linearGradientElement (xmlpp::Element* const xmlElement, Gradient & gradient)
{
	// xlink::href attribute

	basic::uri href;

	if (urlAttribute (xmlElement -> get_attribute ("href", "xlink"), href))
	{
		const auto xpath    = "//*[@id='" + href .fragment () + "']";
		const auto xmlNodes = xmlParser -> get_document () -> get_root_node () -> find (xpath);

		for (const auto & xmlNode : xmlNodes)
			gradientElement (dynamic_cast <xmlpp::Element*> (xmlNode), gradient);
	}

	// Attributes

	lengthAttribute    (xmlElement -> get_attribute ("x1"),                gradient .x1);
	lengthAttribute    (xmlElement -> get_attribute ("y1"),                gradient .y1);
	lengthAttribute    (xmlElement -> get_attribute ("x2"),                gradient .x2);
	lengthAttribute    (xmlElement -> get_attribute ("y2"),                gradient .y2);
	stringAttribute    (xmlElement -> get_attribute ("gradientUnits"),     gradient .gradientUnits);
	transformAttribute (xmlElement -> get_attribute ("gradientTransform"), gradient .gradientTransform);

	// Stops

	for (const auto & xmlNode : xmlElement -> get_children ())
		gradientChild (dynamic_cast <xmlpp::Element*> (xmlNode), gradient);
}

void
Parser::paintRadialGradientElement (xmlpp::Element* const xmlElement,
                                    const X3D::Box2d & bbox,
                                    const Cairo::RefPtr <Cairo::Context> & context)
{
	Gradient gradient;

	radialGradientElement (xmlElement, gradient);

	const auto radialGradient = Cairo::RadialGradient::create (gradient .fx, gradient .fy, 0, gradient .cx, gradient .cy, gradient .r);

	for (const auto & stop : gradient .stops)
		radialGradient -> add_color_stop_rgba (stop .first, stop .second .r (), stop .second .g (), stop .second .b (), stop .second .a ());

	X3D::Matrix3d m;

	m .scale     (X3D::Vector2d (GRADIENT_WIDTH / 2.0, GRADIENT_HEIGHT / 2.0));
	m .translate (X3D::Vector2d (1, 1));
	m .mult_left (inverse (bbox .matrix ()));
	m .mult_left (gradient .gradientTransform);

	const auto matrix = Cairo::Matrix (m [0] [0], m [1] [0], m [0] [1], m [1] [1], m [2] [0], m [2] [1]);

	context -> set_matrix (matrix);
	context -> set_source (radialGradient);
	context -> set_identity_matrix ();
	context -> rectangle (0, 0, GRADIENT_WIDTH, GRADIENT_HEIGHT);
	context -> fill ();
}

void
Parser::radialGradientElement (xmlpp::Element* const xmlElement, Gradient & gradient)
{
	// xlink::href attribute

	basic::uri href;

	if (urlAttribute (xmlElement -> get_attribute ("href", "xlink"), href))
	{
		const auto xpath    = "//*[@id='" + href .fragment () + "']";
		const auto xmlNodes = xmlParser -> get_document () -> get_root_node () -> find (xpath);

		for (const auto & xmlNode : xmlNodes)
			gradientElement (dynamic_cast <xmlpp::Element*> (xmlNode), gradient);
	}

	// Attributes

	lengthAttribute    (xmlElement -> get_attribute ("cx"),                gradient .cx);
	lengthAttribute    (xmlElement -> get_attribute ("cy"),                gradient .cy);
	lengthAttribute    (xmlElement -> get_attribute ("x2"),                gradient .x2);
	lengthAttribute    (xmlElement -> get_attribute ("y2"),                gradient .y2);
	lengthAttribute    (xmlElement -> get_attribute ("r"),                 gradient .r);
	lengthAttribute    (xmlElement -> get_attribute ("fx"),                gradient .fx);
	lengthAttribute    (xmlElement -> get_attribute ("fy"),                gradient .fy);
	stringAttribute    (xmlElement -> get_attribute ("gradientUnits"),     gradient .gradientUnits);
	transformAttribute (xmlElement -> get_attribute ("gradientTransform"), gradient .gradientTransform);
	stringAttribute    (xmlElement -> get_attribute ("spreadMethod"),      gradient .spreadMethod);

	// Stops

	for (const auto & xmlNode : xmlElement -> get_children ())
		gradientChild (dynamic_cast <xmlpp::Element*> (xmlNode), gradient);
}

void
Parser::gradientChild (xmlpp::Element* const xmlElement, Gradient & gradient)
{
	if (not xmlElement)
		return;

	if (xmlElement -> get_name () == "stop")
		return stopElement (xmlElement, gradient);
}

void
Parser::stopElement (xmlpp::Element* const xmlElement, Gradient & gradient)
{
	double offset = 0;
	Style  style;

	percentAttribute (xmlElement -> get_attribute ("offset"), offset);
	styleAttributes  (xmlElement, style);

	gradient .stops .emplace (offset, Color4f (style .stopColor .r (), style .stopColor .g (), style .stopColor .b (), style .stopOpacity));
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

			whiteSpaces (vstream);

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

			whiteSpaces (vstream);

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

			whiteSpaces (vstream);

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, angle))
				{
					bool cwsp = false;

					cwsp |= whiteSpaces (vstream);

					if (Grammar::CloseParenthesis (vstream))
					{
						matrix .rotate (radians (angle));
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
											matrix .rotate (radians (angle));
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

			whiteSpaces (vstream);

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

			whiteSpaces (vstream);

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, angle))
				{
					whiteSpaces (vstream);

					if (Grammar::CloseParenthesis (vstream))
					{
						matrix .mult_left (X3D::Matrix3d (1, std::tan (radians (angle)), 0, 0, 1, 0, 0, 0, 1));
						continue;
					}
				}
			}

			break;
		}
		else if (Grammar::skewY (vstream))
		{
			double angle;

			whiteSpaces (vstream);

			if (Grammar::OpenParenthesis (vstream))
			{
				whiteSpaces (vstream);

				if (Grammar::DoubleValue (vstream, angle))
				{
					whiteSpaces (vstream);

					if (Grammar::CloseParenthesis (vstream))
					{
						matrix .mult_left (X3D::Matrix3d (1, 0, 0, std::tan (radians (angle)), 1, 0, 0, 0, 1));
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
			value /= 1000 * math::pixel <double>;

		return true;
	}

	return false;
}

bool
Parser::percentAttribute (xmlpp::Attribute* const xmlAttribute, double & value)
{
	if (not xmlAttribute)
		return false;

	std::istringstream vstream (xmlAttribute -> get_value ());

	vstream .imbue (std::locale::classic ());

	if (Grammar::DoubleValue (vstream, value))
	{
		// Parse unit

		if (Grammar::PercentSign (vstream))
			value /= 100;

		value = math::clamp (value, 0.0, 1.0);
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
Parser::styleAttributes (xmlpp::Element* const xmlElement, Style & style)
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
		std::make_pair ("stop-color",     std::mem_fn (&Parser::stopColorAttribute)),
		std::make_pair ("stop-opacity",   std::mem_fn (&Parser::stopOpacityAttribute)),
	};

	for (const auto & pair : stylesIndex)
	{
		const auto attribute = xmlElement -> get_attribute (pair .first);

		if (attribute)
			pair .second (this, basic::trim (attribute -> get_value ()), style);

		else
			pair .second (this, "default", style);
	}

	// Style attribute has higher precedence.

	styleAttribute (xmlElement -> get_attribute ("style"), style);
}

bool
Parser::styleAttribute (xmlpp::Attribute* const xmlAttribute, Style & style)
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
		std::make_pair ("stop-color",     std::mem_fn (&Parser::stopColorAttribute)),
		std::make_pair ("stop-opacity",   std::mem_fn (&Parser::stopOpacityAttribute)),
	};

	if (not xmlAttribute)
		return false;

	auto values = std::vector <std::string> ();

	basic::split (std::back_inserter (values), xmlAttribute -> get_value (), ";");

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
			stylesIndex .at (pair [0]) (this, pair [1], style);
		}
		catch (const std::out_of_range &)
		{ }
	}

	return true;
}

void
Parser::displayAttribute (const std::string & value, Style & style)
{
	if (value == "default")
		return;

	if (value == "inherit")
	{
		style .display = styles .back () .display;
		return;
	}

	style .display = value;
}

void
Parser::fillAttribute (const std::string & value, Style & style)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	X3D::Color3f color;

	if (colorValue (vstream, color))
	{
		style .fillType  = ColorType::COLOR;
		style .fillColor = color;
		return;
	}

	basic::uri url;

	if (urlValue (vstream, url))
	{
		style .fillType = ColorType::URL;
		style .fillURL  = url;
		return;
	}

	if (value == "transparent")
	{
		style .fillType = ColorType::NONE;
		return;
	}

	if (value == "none")
	{
		style .fillType = ColorType::NONE;
		return;
	}

	// inherit

	style .fillType  = styles .back () .fillType;
	style .fillColor = styles .back () .fillColor;
	style .fillURL   = styles .back () .fillURL;
}

void
Parser::fillOpacityAttribute (const std::string & value, Style & style)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double fillOpacity;

	if (Grammar::DoubleValue (vstream, fillOpacity))
	{
		style .fillOpacity = math::clamp (fillOpacity, 0.0, 1.0);
		return;
	}

	if (value == "transparent")
	{
		style .fillOpacity = 0;
		return;
	}

	// inherit

	style .fillOpacity = styles .back () .fillOpacity;
}

void
Parser::strokeAttribute (const std::string & value, Style & style)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	X3D::Color3f color;

	if (colorValue (vstream, color))
	{
		style .strokeType  = ColorType::COLOR;
		style .strokeColor = color;
		return;
	}

	basic::uri url;

	if (urlValue (vstream, url))
	{
		style .strokeType = ColorType::URL;
		style .strokeURL  = url;
		return;
	}

	if (value == "transparent")
	{
		style .strokeType = ColorType::NONE;
		return;
	}

	if (value == "none")
	{
		style .strokeType = ColorType::NONE;
		return;
	}

	// inherit

	style .strokeType  = styles .back () .strokeType;
	style .strokeColor = styles .back () .strokeColor;
	style .strokeURL   = styles .back () .strokeURL;
}

void
Parser::strokeOpacityAttribute (const std::string & value, Style & style)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double strokeOpacity;

	if (Grammar::DoubleValue (vstream, strokeOpacity))
	{
		style .strokeOpacity = math::clamp (strokeOpacity, 0.0, 1.0);
		return;
	}

	if (value == "transparent")
	{
		style .strokeOpacity = 0;
		return;
	}

	// inherit

	style .strokeOpacity = styles .back () .strokeOpacity;
}

void
Parser::strokeWidthAttribute (const std::string & value, Style & style)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double strokeWidth;

	if (Grammar::DoubleValue (vstream, strokeWidth))
	{
		style .strokeWidth = strokeWidth;
		return;
	}

	if (value == "none")
	{
		style .strokeWidth = 0;
		return;
	}

	// inherit

	style .strokeWidth = styles .back () .strokeWidth;
}

void
Parser::opacityAttribute (const std::string & value, Style & style)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double opacity;

	if (Grammar::DoubleValue (vstream, opacity))
	{
		style .opacity = math::clamp (opacity, 0.0, 1.0) * styles .back () .opacity;
		return;
	}

	if (value == "transparent")
	{
		style .opacity = 0;
		return;
	}
}

void
Parser::stopColorAttribute (const std::string & value, Style & style)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	X3D::Color3f color;

	if (colorValue (vstream, color))
	{
		style .stopColor = color;
		return;
	}
}

void
Parser::stopOpacityAttribute (const std::string & value, Style & style)
{
	std::istringstream vstream (value);

	vstream .imbue (std::locale::classic ());

	double stopOpacity;

	if (Grammar::DoubleValue (vstream, stopOpacity))
	{
		style .stopOpacity = math::clamp (stopOpacity, 0.0, 1.0);
		return;
	}

	if (value == "transparent")
	{
		style .stopOpacity = 0;
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

				const float b = sc ((number >>= 0) & 0xf) / float (0xff);
				const float g = sc ((number >>= 4) & 0xf) / float (0xff);
				const float r = sc ((number >>= 4) & 0xf) / float (0xff);
	
				color = X3D::Color3f (r, g, b);
				return true;
			}

			const float b = ((number >>= 0) & 0xff) / float (0xff);
			const float g = ((number >>= 8) & 0xff) / float (0xff);
			const float r = ((number >>= 8) & 0xff) / float (0xff);

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
	X3D::Vector2d t, s;
	double        r, so;

	transformAttribute (xmlElement -> get_attribute ("transform"), matrix);

	matrix .translate (translation);
	matrix .scale (scale);

	matrix .get (t, r, s, so);
	matrix .set (X3D::Vector2d (t .x (), -t .y ()), -r, s, -so);

	// Create node.

	const auto transformNode = scene -> createNode <X3D::Transform> ();

	transformNode -> setMatrix (X3D::Matrix4d (matrix [0] [0], matrix [0] [1], 0, 0,
	                                           matrix [1] [0], matrix [1] [1], 0, 0,
	                                           0, 0, 1, 0,
	                                           matrix [2] [0], matrix [2] [1], 0, 1));

	// Set name.

	idAttribute (xmlElement -> get_attribute ("id"), X3D::SFNode (transformNode));

	return transformNode;
}

X3D::X3DPtr <X3D::Appearance>
Parser::getFillAppearance (const Style & style, X3D::Box2d bbox)
{
	const auto appearanceNode = scene -> createNode <X3D::Appearance> ();

	switch (style .fillType)
	{
		case ColorType::NONE:
			return nullptr;

		case ColorType::COLOR:
		{
			const auto materialNode = scene -> createNode <X3D::Material> ();
		
			appearanceNode -> material ()   = materialNode;
			materialNode -> diffuseColor () = style .fillColor;
			materialNode -> transparency () = 1 - style .fillOpacity * style .opacity;

			break;
		}
		case ColorType::URL:
		{
			// Create context

			const auto surface     = Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32, GRADIENT_WIDTH, GRADIENT_HEIGHT);
			const auto context     = Cairo::Context::create (surface);
			const auto textureNode = scene -> createNode <X3D::PixelTexture> ();

			try
			{
				// Get image from url.

				if (not paintURL (style .fillURL, bbox, context))
					return nullptr;
			}
			catch (const std::exception &)
			{
				return nullptr;
			}

			textureNode -> setImage (surface);
	      textureNode -> textureProperties () = texturePropertiesNode;

			try
			{
				scene -> updateNamedNode (get_name_from_string (style .fillURL), X3D::SFNode (textureNode));
			}
			catch (const X3D::X3DError &)
			{ }

			appearanceNode -> texture () = textureNode;
			break;
		}
	}

	return appearanceNode;
}

X3D::X3DPtr <X3D::Appearance>
Parser::getStrokeAppearance (const Style & style)
{
	const auto appearanceNode = scene -> createNode <X3D::Appearance> ();
	const auto materialNode   = scene -> createNode <X3D::Material> ();

	appearanceNode -> material ()    = materialNode;
	materialNode -> emissiveColor () = style .strokeColor;
	materialNode -> transparency ()  = 1 - style .strokeOpacity * style .opacity;

	return appearanceNode;
}

//Matrix3d
//Parser::getTransformationMatrix () const
//{
//	X3D::Matrix4d m;
//	X3D::Vector2d t, s;
//	double        r, so;
//
//	for (const auto & groupNode : basic::make_reverse_range (std::make_pair (groupNodes .begin () + 1, groupNodes .end ())))
//		m *= groupNode -> getMatrix ();
//
//	Matrix3d matrix (m [0] [0], m [0] [1], 0, m [1] [0], m [1] [1], 0, m [3] [0], m [3] [1], 1);
//
//	matrix .get (t, r, s, so);
//	matrix .set (X3D::Vector2d (t .x (), -t .y ()), -r, s, -so);
//
//	return matrix;
//}

Parser::~Parser ()
{ }

} // SVG
} // X3D
} // titania
