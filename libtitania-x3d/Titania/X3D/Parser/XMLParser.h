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

#ifndef __TITANIA_X3D_PARSER_XMLPARSER_H__
#define __TITANIA_X3D_PARSER_XMLPARSER_H__

#include "../Execution/X3DScene.h"
#include "../Parser/X3DParser.h"

namespace xmlpp {

class Attribute;
class CdataNode;
class DomParser;
class Element;
class Node;

}

namespace titania {
namespace X3D {

class XMLParser :
	public X3DParser
{
public:

	///  @name Construction

	XMLParser (const X3DScenePtr &, const basic::uri &, std::istream &);

	///  @name Operations

	virtual
	void
	parseIntoScene () final override;

	///  @name Destruction

	virtual
	~XMLParser () final override;


private:

	///  @name Operations

	void
	xmlElement (xmlpp::Element* const xmlElement);

	void
	x3dElement (xmlpp::Element* const xmlElement);

	void
	x3dElementChild (xmlpp::Element* const xmlElement);

	void
	headElement (xmlpp::Element* const xmlElement);
	
	void
	headElementChild (xmlpp::Element* const xmlElement);
	
	void
	componentElement (xmlpp::Element* const xmlElement);
	
	void
	unitElement (xmlpp::Element* const xmlElement);
	
	void
	metaElement (xmlpp::Element* const xmlElement);

	void
	sceneElement (xmlpp::Element* const xmlElement);

	void
	childrenElements (xmlpp::Element* const xmlElement);
	
	bool
	childElement (xmlpp::Element* const xmlElement);

	void
	externProtoDeclareElement (xmlpp::Element* const xmlElement);
	
	void
	protoDeclareElement (xmlpp::Element* const xmlElement);
	
	void
	protoInterfaceElement (xmlpp::Element* const xmlElement);
	
	void
	protoInterfaceElementChild (xmlpp::Element* const xmlElement);
	
	void
	fieldElement (xmlpp::Element* const xmlElement);

	void
	protoBodyElement (xmlpp::Element* const xmlElement);

	void
	isElement (xmlpp::Element* const xmlElement);

	void
	isElementChild (xmlpp::Element* const xmlElement);

	void
	connectElement (xmlpp::Element* const xmlElement);

	void
	protoInstanceElement (xmlpp::Element* const xmlElement);

	void
	fieldValueElement (xmlpp::Element* const xmlElement);

	void
	nodeElement (xmlpp::Element* const xmlElement);

	void
	routeElement (xmlpp::Element* const xmlElement);
	
	void
	importElement (xmlpp::Element* const xmlElement);
	
	void
	exportElement (xmlpp::Element* const xmlElement);

	bool
	cdataNode (xmlpp::CdataNode* const cdataNode);

	bool
	doubleAttribute (xmlpp::Attribute* const xmlAttribute, double & value);

	bool
	longDoubleAttribute (xmlpp::Attribute* const xmlAttribute, long double & value);

	bool
	integerAttribute (xmlpp::Attribute* const xmlAttribute, int32_t & value);

	bool
	stringAttribute (xmlpp::Attribute* const xmlAttribute, std::string & value);
	
	bool
	useAttribute (xmlpp::Element* const xmlElement);
	
	void
	defAttribute (xmlpp::Element* const xmlElement, X3DBaseNode* const node);

	void
	nodeAttributes (xmlpp::Element* const xmlElement, const SFNode & node);

	void
	nodeAttribute (xmlpp::Attribute* const xmlAttribute, const SFNode & node);

	void
	fieldValue (X3DFieldDefinition* const field, const std::string & value);

	void
	sfstringValues (MFString* const field, std::istream & istream);

	void
	addNode (xmlpp::Element* const xmlElement, X3DBaseNode* const node);

	X3DBrowser*
	getBrowser () const
	{ return scene -> getBrowser (); }

	///  @name Static members

	using ElementsFunction = std::function <void (XMLParser*, xmlpp::Element* const)>;
	
	static const std::map <std::string, ElementsFunction> xmlElementIndex;
	static const std::map <std::string, ElementsFunction> x3dElementChildIndex;
	static const std::map <std::string, ElementsFunction> headElementsIndex;
	static const std::map <std::string, ElementsFunction> childElementIndex;

	///  @name Members

	const X3DScenePtr scene;
	const basic::uri  uri;
	std::istream &    istream;

	const std::unique_ptr <xmlpp::DomParser> xmlParser;

	ComponentInfoArray            components;
	std::vector <X3DChildObject*> parents;

};

} // X3D
} // titania

#endif
