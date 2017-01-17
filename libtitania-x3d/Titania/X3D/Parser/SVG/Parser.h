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

#ifndef __TITANIA_X3D_PARSER_SVG_PARSER_H__
#define __TITANIA_X3D_PARSER_SVG_PARSER_H__

#include "../../Execution/X3DScene.h"
#include "../../Parser/X3DParser.h"
#include "../../Types/Geometry.h"

#include "Colors.h"

#include <cairomm/cairomm.h>

namespace xmlpp {

class Attribute;
class DomParser;
class Element;
class Node;

}

namespace titania {
namespace X3D {
namespace SVG {

class Parser :
	public X3D::X3DParser
{
public:

	///  @name Construction

	Parser (const X3D::X3DScenePtr &, const basic::uri &, std::istream &);

	///  @name Operations

	virtual
	void
	parseIntoScene () final override;

	///  @name Destruction

	virtual
	~Parser ();


private:

	using Contour  = std::vector <X3D::Vector2d>;
	using Contours = std::vector <Contour>;

	struct Gradient
	{
		Gradient () :
			               x1 (0),
			               y1 (0),
			               x2 (0),
			               y2 (0),
			    gradientUnits (),
			gradientTransform (),
			            stops ()
			{ }

		double                          x1;
		double                          y1;
		double                          x2;
		double                          y2;
		std::string                     gradientUnits;
		X3D::Matrix3d                   gradientTransform;
		std::map <double, X3D::Color4f> stops;
	};

	enum class ColorType
	{
		NONE,
		COLOR,
		URL,
	};

	struct Style {

		Style () :
			      display ("inline"),
			     fillType (ColorType::COLOR),
			    fillColor (),
			      fillURL (),
			  fillOpacity (1),
			   strokeType (ColorType::COLOR),
			  strokeColor (),
			    strokeURL (),
			strokeOpacity (1),
			  strokeWidth (1),
			      opacity (1),
			    stopColor (),
			  stopOpacity (1)
			{ }

		std::string  display;
		ColorType    fillType;
		X3D::Color3f fillColor;
		basic::uri   fillURL;
		double       fillOpacity;
		ColorType    strokeType;
		X3D::Color3f strokeColor;
		basic::uri   strokeURL;
		double       strokeOpacity;
		double       strokeWidth;
		double       opacity;
		Color3f      stopColor;
		double       stopOpacity;

	};

	///  @name Operations

	void
	svgElement (xmlpp::Element* const xmlElement);

	bool
	whiteSpaces (std::istream & istream);

	bool
	commaWhiteSpaces (std::istream & istream);

	void
	elements (xmlpp::Element* const xmlElement);

	void
	element (xmlpp::Element* const xmlElement);

	void
	useElement (xmlpp::Element* const xmlElement);

	bool
	isUsed (xmlpp::Element* const xmlElement);

	void
	groupElement (xmlpp::Element* const xmlElement);

	void
	switchElement (xmlpp::Element* const xmlElement);

	void
	aElement (xmlpp::Element* const xmlElement);

	void
	rectangleElement (xmlpp::Element* const xmlElement);

	void
	circleElement (xmlpp::Element* const xmlElement);

	void
	ellipseElement (xmlpp::Element* const xmlElement);

	void
	textElement (xmlpp::Element* const xmlElement);

	void
	imageElement (xmlpp::Element* const xmlElement);

	void
	polylineElement (xmlpp::Element* const xmlElement);

	void
	polygonElement (xmlpp::Element* const xmlElement);

	void
	pathElement (xmlpp::Element* const xmlElement);
	
	bool
	paintURL (const basic::uri & url,
	          const X3D::Box2d & bbox,
	          const Cairo::RefPtr <Cairo::Context> & context);

	void
	paintLinearGradientElement (xmlpp::Element* const xmlElement,
	                            const X3D::Box2d & bbox,
	                            const Cairo::RefPtr <Cairo::Context> & context);

	void
	linearGradientElement (xmlpp::Element* const xmlElement, Gradient & linearGradient);

	void
	gradientChild (xmlpp::Element* const xmlElement, Gradient & gradient);
	
	void
	stopElement (xmlpp::Element* const xmlElement, Gradient & gradient);

	void
	idAttribute (xmlpp::Attribute* const attribute, const X3D::SFNode & node);

	bool
	viewBoxAttribute (xmlpp::Attribute* const xmlAttribute, X3D::Vector4d & value);

	bool
	transformAttribute (xmlpp::Attribute* const xmlAttribute, X3D::Matrix3d & matrix);

	bool
	lengthAttribute (xmlpp::Attribute* const xmlAttribute, double & value);

	bool
	percentAttribute (xmlpp::Attribute* const xmlAttribute, double & value);

	bool
	stringAttribute (xmlpp::Attribute* const xmlAttribute, std::string & value);

	bool
	urlAttribute (xmlpp::Attribute* const xmlAttribute, basic::uri & value);

	bool
	pointsAttribute (xmlpp::Attribute* const xmlAttribute, std::vector <X3D::Vector2d> & points);

	bool
	dAttribute (xmlpp::Attribute* const xmlAttribute, Contours & contours);

	void
	styleAttributes (xmlpp::Element* const xmlElement, Style & style);

	bool
	styleAttribute (xmlpp::Attribute* const xmlAttribute, Style & value);

	void
	displayAttribute (const std::string & value, Style & style);
	
	void
	fillAttribute (const std::string & value, Style & style);
	
	void
	fillOpacityAttribute (const std::string & value, Style & style);
	
	void
	strokeAttribute (const std::string & value, Style & style);
	
	void
	strokeOpacityAttribute (const std::string & value, Style & style);
	
	void
	strokeWidthAttribute (const std::string & value, Style & style);
	
	void
	opacityAttribute (const std::string & value, Style & style);
	
	void
	stopColorAttribute (const std::string & value, Style & style);
	
	void
	stopOpacityAttribute (const std::string & value, Style & style);

	bool
	colorValue (std::istream & istream, X3D::Color3f & color);

	bool
	urlValue (std::istream & istream, basic::uri & url);

	X3D::X3DPtr <X3D::Transform>
	getTransform (xmlpp::Element* const xmlElement,
	              const X3D::Vector2d & translation = X3D::Vector2d (),
	              const X3D::Vector2d & scale = X3D::Vector2d (1, 1));

	X3D::X3DPtr <X3D::Appearance>
	getFillAppearance (const Style & fillStyle, X3D::Box2d bbox);

	X3D::X3DPtr <X3D::Appearance>
	getStrokeAppearance (const Style & strokeStyle);

//	Matrix3d
//	getTransformationMatrix () const;

	///  @name Members

	const X3D::X3DScenePtr scene;
	const basic::uri       uri;
	std::istream &         istream;

	std::unique_ptr <xmlpp::DomParser> xmlParser;

	std::vector <Style>                     styles;
	X3D::X3DPtr <X3D::Transform>            rootTransform;
	X3D::X3DPtrArray <X3D::X3DGroupingNode> groupNodes;

	Colors      namedColors;
	std::string whiteSpaceCharacters;

};

} // SVG
} // X3D
} // titania

#endif
