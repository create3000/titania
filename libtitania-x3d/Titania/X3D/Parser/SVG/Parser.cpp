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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <libxml++/libxml++.h>
#pragma GCC diagnostic pop

#include "../../Components/EnvironmentalEffects/Background.h"
#include "../../Components/Geometry2D/Circle2D.h"
#include "../../Components/Geometry2D/Disk2D.h"
#include "../../Components/Geometry2D/Rectangle2D.h"
#include "../../Components/Geometry2D/Polyline2D.h"
#include "../../Components/Grouping/Switch.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Layering/Layer.h"
#include "../../Components/Layering/LayerSet.h"
#include "../../Components/Navigation/OrthoViewpoint.h"
#include "../../Components/Navigation/NavigationInfo.h"
#include "../../Components/Networking/Anchor.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Components/Rendering/IndexedTriangleSet.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/LineProperties.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../../Components/Texturing/ImageTexture.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Components/Texturing/TextureProperties.h"
#include "../../Parser/Colors.h"
#include "../../Parser/Filter.h"

#include "Parser.h"

#include <Titania/Math/Algorithms/Bezier.h>
#include <Titania/InputOutput.h>
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

static constexpr double BEZIER_TOLERANCE = 1e-4;
static constexpr size_t BEZIER_STEPS     = 16;
static constexpr size_t CIRCLE_DIMENSION = 64;
static constexpr size_t GRADIENT_WIDTH   = 256;
static constexpr size_t GRADIENT_HEIGHT  = 256;

const std::map <std::string, Parser::ElementsFunction> Parser::xmlElementsIndex = {
	std::make_pair ("svg", std::mem_fn (&Parser::svgElement)),
};

const std::map <std::string, Parser::ElementsFunction> Parser::elementsIndex = {
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

const std::map <std::string, Parser::PaintGradientFunction> Parser::paintGradientIndex = {
	std::make_pair ("linearGradient", std::mem_fn (&Parser::paintLinearGradientElement)),
	std::make_pair ("radialGradient", std::mem_fn (&Parser::paintRadialGradientElement)),
};

const std::map <std::string, Parser::GradientFunction> Parser::gradientIndex = {
	std::make_pair ("linearGradient", std::mem_fn (&Parser::linearGradientElement)),
	std::make_pair ("radialGradient", std::mem_fn (&Parser::radialGradientElement)),
};

const std::map <std::string, Parser::StyleFunction> Parser::styleIndex =
{
	std::make_pair ("display",        std::mem_fn (&Parser::displayStyle)),
	std::make_pair ("fill",           std::mem_fn (&Parser::fillStyle)),
	std::make_pair ("fill-opacity",   std::mem_fn (&Parser::fillOpacityStyle)),
	std::make_pair ("fill-rule",      std::mem_fn (&Parser::fillRuleStyle)),
	std::make_pair ("stroke",         std::mem_fn (&Parser::strokeStyle)),
	std::make_pair ("stroke-opacity", std::mem_fn (&Parser::strokeOpacityStyle)),
	std::make_pair ("stroke-width",   std::mem_fn (&Parser::strokeWidthStyle)),
	std::make_pair ("opacity",        std::mem_fn (&Parser::opacityStyle)),
	std::make_pair ("stop-color",     std::mem_fn (&Parser::stopColorStyle)),
	std::make_pair ("stop-opacity",   std::mem_fn (&Parser::stopOpacityStyle)),
};

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	       X3D::X3DParser (),
	                scene (scene),
	                  uri (uri),
	              istream (istream),
	            xmlParser (new xmlpp::DomParser ()),
	               styles (1),
	             layerSet (scene -> createNode <X3D::LayerSet> ()),
	        rootTransform (scene -> createNode <X3D::Transform> ()),
	           groupNodes ({ rootTransform }),
	            viewpoint (scene -> createNode <X3D::OrthoViewpoint> ()),
	texturePropertiesNode (scene -> createNode <X3D::TextureProperties> ()),
	           viewMatrix (),
	 whiteSpaceCharacters ()
{
	xmlParser -> set_throw_messages (true);
	xmlParser -> set_validate (false);
	xmlParser -> set_include_default_attributes (true);

	texturePropertiesNode -> generateMipMaps ()     = true;
	texturePropertiesNode -> minificationFilter ()  = "NEAREST_PIXEL_AVG_MIPMAP";
	texturePropertiesNode -> magnificationFilter () = "DEFAULT";
	texturePropertiesNode -> boundaryModeS ()       = "CLAMP_TO_EDGE";
	texturePropertiesNode -> boundaryModeT ()       = "CLAMP_TO_EDGE";
	texturePropertiesNode -> boundaryModeR ()       = "CLAMP_TO_EDGE";
	texturePropertiesNode -> textureCompression ()  = "DEFAULT";
}

void
Parser::parseIntoScene ()
{
	//__LOG__ << this << " " << std::endl;

	try
	{
		scene -> setWorldURL (uri);
		scene -> setEncoding (EncodingType::XML);
		scene -> setProfile (getBrowser () -> getProfile ("Full"));

		xmlParser -> parse_stream (istream);

		const auto xmlDocument = xmlParser -> get_document ();

		if (xmlDocument)
			xmlElement (xmlDocument -> get_root_node ());
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		throw;
	}
	catch (const std::exception & error)
	{
		throw Error <X3D::INVALID_X3D> (error .what ()); 
	}
	catch (...)
	{
		throw Error <INVALID_X3D> ("Unkown parser error.");
	}
}

void
Parser::xmlElement (xmlpp::Element* const xmlElement)
{
	try
	{
		if (not xmlElement)
			return;

		xmlElementsIndex .at (xmlElement -> get_name ()) (this, xmlElement);
	}
	catch (const std::out_of_range &)
	{ }
}

void
Parser::svgElement (xmlpp::Element* const xmlElement)
{
	// Get attributes of svg element.

	double        width  = 0;
	double        height = 0;
	X3D::Vector4d viewBox;

	if (not viewBoxAttribute (xmlElement -> get_attribute ("viewBox"), viewBox))
		viewBox = X3D::Vector4d (0, 0, 100, 100);

	if (not lengthAttribute  (xmlElement -> get_attribute ("width"), width))
		width = viewBox [2];

	if (not lengthAttribute  (xmlElement -> get_attribute ("height"), height))
		height = viewBox [3];

	// Create background.

	const auto background = scene -> createNode <X3D::Background> ();

	background -> skyColor () = { Color3f (1, 1, 1) };

	scene -> getRootNodes () .emplace_back (background);

	// Create navigation info.

	const auto navigationInfo = scene -> createNode <X3D::NavigationInfo> ();

	navigationInfo -> type () = { "PLANE_create3000.de", "EXAMINE", "ANY" };

	scene -> getRootNodes () .emplace_back (navigationInfo);

	// Create viewpoint.

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

	// Create first layer.

	const auto scale       = X3D::Vector3d (math::pixel <double> * width / viewBox [2], -math::pixel <double> * height / viewBox [3], 1);
	const auto translation = X3D::Vector3d (-viewBox .x (), viewBox .y (), 0) * scale;

	viewMatrix .set (translation, Rotation4d (), scale);

__LOG__ << viewBox << std::endl;
__LOG__ << scale << std::endl;

	rootTransform -> setMatrix (viewMatrix);

	// Parse elements.

	elements (xmlElement);

	if (rootTransform)
	{
		scene -> updateNamedNode (scene -> getUniqueName (GetNameFromURI (uri)), X3D::SFNode (rootTransform));
		scene -> getRootNodes () .emplace_back (rootTransform);
	}
	else
	{
		scene -> updateNamedNode (scene -> getUniqueName (GetNameFromURI (uri)), X3D::SFNode (layerSet));
		scene -> getRootNodes () .emplace_back (layerSet);

		layerSet -> activeLayer () = 0;

		for (size_t i = 1, size = layerSet -> layers () .size () + 1; i < size; ++ i)
			layerSet -> order () .emplace_back (i);
	}
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
	
		const auto node = scene -> getNamedNode (GetNameFromString (href .fragment ()));

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
	
		const auto node = scene -> getNamedNode (GetNameFromString (id));

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
	std::string groupmode;
	std::string label;

	styleAttributes (xmlElement, style);

	if (style .display == "none")
		return;

	styles .emplace_back (style);

	// Get transform.	
	
	const auto transformNode = getTransform (xmlElement);

	// Layer handling.

	stringAttribute (xmlElement -> get_attribute ("groupmode", "inkscape"), groupmode);
	stringAttribute (xmlElement -> get_attribute ("label",     "inkscape"), label);

	if (groupmode == "layer")
	{
		// Add first root Transform to a layer if need.

		if (rootTransform and not rootTransform -> children () .empty ())
		{
			const auto layer = scene -> createNode <X3D::Layer> ();

			layerSet -> layers () .emplace_back (layer);
			layer -> children ()  .emplace_back (rootTransform);
		}
		else
		{
			transformNode -> setMatrix (transformNode -> getMatrix () * viewMatrix);
		}
		// Clear.

		rootTransform = nullptr;

		groupNodes .clear ();

		// Create a new layer with a new root Transform.

		const auto layer = scene -> createNode <X3D::Layer> ();

		scene -> updateNamedNode (scene -> getUniqueName (GetNameFromString (label)), layer);

		layerSet -> layers () .emplace_back (layer);
		layer -> children ()  .emplace_back (viewpoint);
		layer -> children ()  .emplace_back (transformNode);
	}

	// Get child elements.

	groupNodes .emplace_back (transformNode);

	elements (xmlElement);

	styles .pop_back ();

	// Add node.

	if (groupmode not_eq "layer")
	{
		groupNodes .pop_back ();

		if (not transformNode -> children () .empty ())
			groupNodes .back () -> children () .emplace_back (transformNode);
	}
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

	const auto transformNode = getTransform (xmlElement, X3D::Vector2d (x + width / 2, y + height / 2), X3D::Vector2d (1, -1));

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
	using namespace std::placeholders;

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

	const auto bbox          = X3D::Box2d (points .cbegin (), points .cend (), math::iterator_type ());
	const auto transformNode = getTransform (xmlElement);

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	const auto coordinateNode = scene -> createNode <X3D::Coordinate> ();

	for (const auto & point : points)
		coordinateNode -> point () .emplace_back (point .x (), point .y (), 0);

	if (style .fillType not_eq ColorType::NONE)
	{
		// Tesselate contours

		Tesselator tessellator;

		tessellator .property (GLU_TESS_WINDING_RULE, getFillRule (style));
		tessellator .property (GLU_TESS_TOLERANCE, 0);
		tessellator .normal (Vector3d (0, 0, 1));
		tessellator .combine (std::bind (&Parser::combine, this, coordinateNode, _1));

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
			const auto geometryNode = scene -> createNode <X3D::IndexedTriangleSet> ();

			try
			{
				const auto texCoordNode  = scene -> createNode <X3D::TextureCoordinate> ();
				const auto invBBoxMatrix = inverse (bbox .matrix ());

				for (const auto & point : basic::make_const_range (coordinateNode -> point ()))
					texCoordNode -> point () .emplace_back ((Vector2d (point .x (), point .y ()) * invBBoxMatrix + X3D::Vector2d (1, 1)) / 2.0);

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
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 0] .data ()));
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 1] .data ()));
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 2] .data ()));
			}
	
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
	using namespace std::placeholders;

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

	const auto bbox          = X3D::Box2d (points .cbegin (), points .cend (), math::iterator_type ());
	const auto transformNode = getTransform (xmlElement);

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	const auto coordinateNode = scene -> createNode <X3D::Coordinate> ();

	for (const auto & point : points)
		coordinateNode -> point () .emplace_back (point .x (), point .y (), 0);

	if (style .fillType not_eq ColorType::NONE)
	{
		// Tesselate contours

		Tesselator tessellator;

		tessellator .property (GLU_TESS_WINDING_RULE, getFillRule (style));
		tessellator .property (GLU_TESS_TOLERANCE, 0);
		tessellator .normal (Vector3d (0, 0, 1));
		tessellator .combine (std::bind (&Parser::combine, this, coordinateNode, _1));

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
			const auto geometryNode = scene -> createNode <X3D::IndexedTriangleSet> ();

			try
			{
				const auto texCoordNode  = scene -> createNode <X3D::TextureCoordinate> ();
				const auto invBBoxMatrix = inverse (bbox .matrix ());
	
				for (const auto & point : basic::make_const_range (coordinateNode -> point ()))
					texCoordNode -> point () .emplace_back ((Vector2d (point .x (), point .y ()) * invBBoxMatrix + X3D::Vector2d (1, 1)) / 2.0);

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
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 0] .data ()));
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 1] .data ()));
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 2] .data ()));
			}

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
	using namespace std::placeholders;

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
		bbox += X3D::Box2d (contour -> curve -> points .cbegin (), contour -> curve -> points .cend (), math::iterator_type ());

	const auto transformNode = getTransform (xmlElement);

	groupNodes .emplace_back (transformNode);

	// Create nodes.

	const auto coordinateNode = scene -> createNode <X3D::Coordinate> ();

	size_t index = 0;

	for (const auto & contour : contours)
	{
		contour -> index = index;

		for (const auto & point : contour -> curve -> points)
			coordinateNode -> point () .emplace_back (point .x (), point .y (), 0);

		index += contour -> curve -> points .size ();
	}

	if (style .fillType not_eq ColorType::NONE)
	{
		// Tesselate contours

		Tesselator tessellator;

		tessellator .property (GLU_TESS_WINDING_RULE, getFillRule (style));
		tessellator .property (GLU_TESS_TOLERANCE, 0);
		tessellator .normal (Vector3d (0, 0, 1));
		tessellator .combine (std::bind (&Parser::combine, this, coordinateNode, _1));

		tessellator .begin_polygon ();

		for (const auto & contour : contours)
		{
			tessellator .begin_contour ();

			size_t index = contour -> index;

			for (const auto & point : contour -> curve -> points)
			{
				tessellator .add_vertex (X3D::Vector3d (point .x (), point .y (), 0), index ++);
			}

			tessellator .end_contour ();
		}

		tessellator .end_polygon ();

		const auto triangles = tessellator .triangles ();

		if (triangles .size ())
		{
			// Create geometry.
	
			const auto shapeNode     = scene -> createNode <X3D::Shape> ();
			const auto geometryNode  = scene -> createNode <X3D::IndexedTriangleSet> ();

			try
			{
				const auto texCoordNode  = scene -> createNode <X3D::TextureCoordinate> ();
				const auto invBBoxMatrix = inverse (bbox .matrix ());
	
				for (const auto & point : basic::make_const_range (coordinateNode -> point ()))
					texCoordNode -> point () .emplace_back ((Vector2d (point .x (), point .y ()) * invBBoxMatrix + X3D::Vector2d (1, 1)) / 2.0);

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
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 0] .data ()));
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 1] .data ()));
				geometryNode -> index () .emplace_back (std::get <0> (triangles [i + 2] .data ()));
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

		for (const auto & contour : contours)
		{
			if (contour -> curve -> points .size () > 1)
			{
				size_t index = contour -> index;

				for (size_t i = 0, size = contour -> curve -> points .size (); i < size; ++ i)
					geometryNode -> coordIndex () .emplace_back (index ++);

				if (contour -> curve -> closed)
					geometryNode -> coordIndex () .emplace_back (contour -> index);

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

Parser::Tesselator::data_type
Parser::combine (const X3D::X3DPtr <X3D::Coordinate> & coordinateNode, const Vector3d & point) const
{
	const auto index = coordinateNode -> point () .size ();

	coordinateNode -> point () .emplace_back (point);

	return Tesselator::data_type (index);
}

bool
Parser::paintURL (const basic::uri & url,
                  const X3D::Box2d & bbox,
                  const Cairo::RefPtr <Cairo::Context> & context)
{
	try
	{
		const auto xpath    = "//*[@id='" + url .fragment () + "']";
		const auto xmlNodes = xmlParser -> get_document () -> get_root_node () -> find (xpath);

		if (xmlNodes .empty ())
			return false;

		const auto xmlElement = dynamic_cast <xmlpp::Element*> (xmlNodes .back ());

		if (not xmlElement)
			return false;

		paintGradientIndex .at (xmlElement -> get_name ()) (this, xmlElement, bbox, context);
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

	paintGradient (linearGradient, gradient, bbox, context);
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

	paintGradient (radialGradient, gradient, bbox, context);
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

	lengthAttribute (xmlElement -> get_attribute ("cx"), gradient .cx);
	lengthAttribute (xmlElement -> get_attribute ("cy"), gradient .cy);

	lengthAttribute (xmlElement -> get_attribute ("x2"), gradient .x2);
	lengthAttribute (xmlElement -> get_attribute ("y2"), gradient .y2);

	if (not lengthAttribute (xmlElement -> get_attribute ("r"),  gradient .r))
		 gradient .r = gradient .cx;

	if (not lengthAttribute (xmlElement -> get_attribute ("fx"), gradient .fx))
		 gradient .fx = gradient .cx;

	if (not lengthAttribute (xmlElement -> get_attribute ("fy"), gradient .fy))
		 gradient .fy =  gradient .cy;

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
Parser::gradientElement (xmlpp::Element* const xmlElement, Gradient & gradient)
{
	try
	{
		if (not xmlElement)
			return;

		gradientIndex .at (xmlElement -> get_name ()) (this, xmlElement, gradient);
	}
	catch (const std::out_of_range &)
	{ }
}

void
Parser::paintGradient (const Cairo::RefPtr <Cairo::Gradient> & cairoGradient,
                       const Gradient & gradient,
                       const X3D::Box2d & bbox,
                       const Cairo::RefPtr <Cairo::Context> & context)
{
	// Determine gradient transform.

	X3D::Matrix3d m;

	m .scale     (X3D::Vector2d (GRADIENT_WIDTH / 2.0, GRADIENT_HEIGHT / 2.0));
	m .translate (X3D::Vector2d (1, 1));
	m .scale     (X3D::Vector2d (1, -1));
	m .mult_left (inverse (bbox .matrix ()));
	m .mult_left (gradient .gradientTransform);

	const auto gradientTransform = Cairo::Matrix (m [0] [0], m [0] [1], m [1] [0], m [1] [1], m [2] [0], m [2] [1]);

	// Add colors.

	for (const auto & stop : gradient .stops)
		cairoGradient -> add_color_stop_rgba (stop .first, stop .second .r (), stop .second .g (), stop .second .b (), stop .second .a ());

	// Paint.

	context -> set_matrix (gradientTransform);
	context -> set_source (cairoGradient);
	context -> set_identity_matrix ();
	context -> rectangle (0, 0, GRADIENT_WIDTH, GRADIENT_HEIGHT);
	context -> fill ();
}

void
Parser::idAttribute (xmlpp::Attribute* const attribute, const X3D::SFNode & node)
{
	try
	{
		if (not attribute)
			return;

		scene -> updateNamedNode (scene -> getUniqueName (GetNameFromString (attribute -> get_value ())), node);
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
						matrix .skew_x (radians (angle));
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
						matrix .skew_y (radians (angle));
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

		if (Grammar::cm (vstream))
			value /= 100 * math::pixel <double>;

		if (Grammar::in (vstream))
			value *= math::inch <double> / math::pixel <double>;

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

	Points points;

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
				// moveto

				vstream .get ();

				if (not points .empty ())
					contours .emplace_back (std::make_shared <Contour> (std::move (points), false));

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x, y;
		
					if (Grammar::DoubleValue (vstream, x))
					{
						commaWhiteSpaces (vstream);

						if (Grammar::DoubleValue (vstream, y))
						{
							if (relative)
							{
								x += ax;
								y += ay;
							}

							points .emplace_back (x, y);

							ax = x;
							ay = y;

							commaWhiteSpaces (vstream);
							continue;
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'L':
			case 'l':
			{
				// lineto

				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x, y;
		
					if (Grammar::DoubleValue (vstream, x))
					{
						commaWhiteSpaces (vstream);

						if (Grammar::DoubleValue (vstream, y))
						{
							if (relative)
							{
								x += ax;
								y += ay;
							}

							points .emplace_back (x, y);

							ax = x;
							ay = y;

							commaWhiteSpaces (vstream);
							continue;
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'H':
			case 'h':
			{
				// horizontal lineto

				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x;
		
					if (Grammar::DoubleValue (vstream, x))
					{
						if (relative)
							x += ax;

						points .emplace_back (x, ay);

						ax = x;

						commaWhiteSpaces (vstream);
						continue;
					}
	
					break;
				}
	
				continue;
			}
			case 'V':
			case 'v':
			{
				// vertical lineto

				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double y;
		
					if (Grammar::DoubleValue (vstream, y))
					{
						if (relative)
							y += ay;

						points .emplace_back (ax, y);

						ay = y;

						commaWhiteSpaces (vstream);
						continue;
					}
	
					break;
				}
	
				continue;
			}
			case 'Q':
			case 'q':
			{
				// quadratic Bézier curveto

				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x1, y1, x, y;
		
					if (Grammar::DoubleValue (vstream, x1))
					{
						commaWhiteSpaces (vstream);

						if (Grammar::DoubleValue (vstream, y1))
						{
							commaWhiteSpaces (vstream);

							if (Grammar::DoubleValue (vstream, x))
							{
								commaWhiteSpaces (vstream);

								if (Grammar::DoubleValue (vstream, y))
								{
									if (relative)
									{
										x1 += ax;
										y1 += ay;
										x  += ax;
										y  += ay;
									}

									math::bezier::quadratic_curve (X3D::Vector2d (ax, ay), X3D::Vector2d (x1, y1), X3D::Vector2d (x, y), BEZIER_STEPS, BEZIER_TOLERANCE, points);

									ax = x;
									ay = y;
									px = x1;
									py = y1;

									commaWhiteSpaces (vstream);
									continue;
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
				// Shorthand/smooth quadratic Bézier curveto

				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x1, y1, x, y;
		
					if (Grammar::DoubleValue (vstream, x))
					{
						commaWhiteSpaces (vstream);

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

							math::bezier::quadratic_curve (X3D::Vector2d (ax, ay), X3D::Vector2d (x1, y1), X3D::Vector2d (x, y), BEZIER_STEPS, BEZIER_TOLERANCE, points);

							ax = x;
							ay = y;

							commaWhiteSpaces (vstream);
							continue;
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'C':
			case 'c':
			{
				// curveto, cubic Bézier curve

				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x1, y1, x2, y2, x, y;
		
					if (Grammar::DoubleValue (vstream, x1))
					{
						commaWhiteSpaces (vstream);

						if (Grammar::DoubleValue (vstream, y1))
						{
							commaWhiteSpaces (vstream);

							if (Grammar::DoubleValue (vstream, x2))
							{
								commaWhiteSpaces (vstream);

								if (Grammar::DoubleValue (vstream, y2))
								{
									commaWhiteSpaces (vstream);

									if (Grammar::DoubleValue (vstream, x))
									{
										commaWhiteSpaces (vstream);

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

											math::bezier::cubic_curve (X3D::Vector2d (ax, ay),
											                           X3D::Vector2d (x1, y1),
											                           X3D::Vector2d (x2, y2),
											                           X3D::Vector2d (x, y),
											                           BEZIER_STEPS,
											                           BEZIER_TOLERANCE, points);

											ax = x;
											ay = y;
											px = x2;
											py = y2;

											commaWhiteSpaces (vstream);
											continue;
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
				// shorthand/smooth curveto, cubic Bézier curve

				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double x1, y1, x2, y2, x, y;
		
					if (Grammar::DoubleValue (vstream, x2))
					{
						commaWhiteSpaces (vstream);

						if (Grammar::DoubleValue (vstream, y2))
						{
							commaWhiteSpaces (vstream);

							if (Grammar::DoubleValue (vstream, x))
							{
								commaWhiteSpaces (vstream);

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

									math::bezier::cubic_curve (X3D::Vector2d (ax, ay),
									                           X3D::Vector2d (x1, y1),
									                           X3D::Vector2d (x2, y2),
									                           X3D::Vector2d (x, y),
									                           BEZIER_STEPS,
									                           BEZIER_TOLERANCE, points);

									ax = x;
									ay = y;
									px = x2;
									py = y2;

									commaWhiteSpaces (vstream);
									continue;
								}
							}
						}
					}
	
					break;
				}
	
				continue;
			}
			case 'A':
			case 'a':
			{
				// elliptical arc

				vstream .get ();

				whiteSpaces (vstream);
	
				while (vstream)
				{
					double rx, ry, xAxisRotation, x, y;
					int32_t largeArcFlag, sweepFlag;
		
					if (Grammar::DoubleValue (vstream, rx))
					{
						commaWhiteSpaces (vstream);

						if (Grammar::DoubleValue (vstream, ry))
						{
							commaWhiteSpaces (vstream);

							if (Grammar::DoubleValue (vstream, xAxisRotation))
							{
								commaWhiteSpaces (vstream);

								if (Grammar::IntegerValue (vstream, largeArcFlag))
								{
									commaWhiteSpaces (vstream);
	
									if (Grammar::IntegerValue (vstream, sweepFlag))
									{
										commaWhiteSpaces (vstream);

										if (Grammar::DoubleValue (vstream, x))
										{
											commaWhiteSpaces (vstream);
			
											if (Grammar::DoubleValue (vstream, y))
											{
												if (relative)
												{
													x += ax;
													y += ay;
												}

												const auto start = X3D::Vector2d (ax, ay);
												const auto end   = X3D::Vector2d (x, y);

												bezier::arc_curve (start, rx, ry, radians (xAxisRotation), largeArcFlag, sweepFlag, end, CIRCLE_DIMENSION, BEZIER_TOLERANCE, points);

												ax = x;
												ay = y;
			
												commaWhiteSpaces (vstream);
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
				// closepath

				vstream .get ();

				if (not points .empty ())
				{
					ax = points .front () .x ();
					ay = points .front () .y ();

					contours .emplace_back (std::make_shared <Contour> (std::move (points), true));
				}

				commaWhiteSpaces (vstream);
				continue;
			}
			default:
				break;
		}

		break;
	}

	if (not points .empty ())
		contours .emplace_back (std::make_shared <Contour> (std::move (points), false));

	return true;
}

void
Parser::styleAttributes (xmlpp::Element* const xmlElement, Style & style)
{
	for (const auto & pair : styleIndex)
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
			styleIndex .at (pair [0]) (this, pair [1], style);
		}
		catch (const std::out_of_range &)
		{ }
	}

	return true;
}

void
Parser::displayStyle (const std::string & value, Style & style)
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
Parser::fillStyle (const std::string & value, Style & style)
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
Parser::fillOpacityStyle (const std::string & value, Style & style)
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
Parser::fillRuleStyle (const std::string & value, Style & style)
{
	style .fillRule = value;
}

void
Parser::strokeStyle (const std::string & value, Style & style)
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
Parser::strokeOpacityStyle (const std::string & value, Style & style)
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
Parser::strokeWidthStyle (const std::string & value, Style & style)
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
Parser::opacityStyle (const std::string & value, Style & style)
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
Parser::stopColorStyle (const std::string & value, Style & style)
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
Parser::stopOpacityStyle (const std::string & value, Style & style)
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
	// Parse HTML hex colors and shorthand hex colors.

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

	// Parse RGB colors.

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

	// Parse named colors.

	{
		std::string colorName;
	
		if (Grammar::NamedColor (istream, colorName))
		{
			try
			{
				color = X3D::Colors::get (basic::tolower (colorName, std::locale::classic ()));
				return true;
			}
			catch (const std::out_of_range &)
			{
				for (size_t i = 0, size = colorName .size (); i < size; ++ i)
					istream .unget ();
			}
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
			const auto textureNode = scene -> createNode <X3D::ImageTexture> ();

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

			textureNode -> setUrl (surface);
	      textureNode -> textureProperties () = texturePropertiesNode;

			try
			{
				scene -> updateNamedNode (scene -> getUniqueName (GetNameFromString (style .fillURL)), X3D::SFNode (textureNode));
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

	if (style .strokeWidth not_eq 1)
	{
		const auto lineProperties = scene -> createNode <X3D::LineProperties> ();

		appearanceNode -> lineProperties ()       = lineProperties;
		lineProperties -> linewidthScaleFactor () = style .strokeWidth;
	}

	return appearanceNode;
}

GLenum
Parser::getFillRule (const Style & style)
{
	if (style .fillRule == "evenodd")
		return GLU_TESS_WINDING_ODD;

	return GLU_TESS_WINDING_NONZERO;
}

Parser::~Parser ()
{ }

} // SVG
} // X3D
} // titania
