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

#include "XMLParser.h"

#include <libxml++/libxml++.h>

#include "../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

class XMLGrammar
{
public:

	static const io::number <double>  DoubleValue;
	static const io::number <int32_t> IntegerValue;

};

const io::number <double>  XMLGrammar::DoubleValue;
const io::number <int32_t> XMLGrammar::IntegerValue;

XMLParser::XMLParser (const X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	X3DParser (),
	    scene (scene),
	      uri (uri),
	  istream (istream),
	xmlParser (new xmlpp::DomParser ())
{
	xmlParser -> set_throw_messages (true);
	xmlParser -> set_validate (false);
	xmlParser -> set_include_default_attributes (true);
}

void
XMLParser::parseIntoScene ()
{
	//__LOG__ << this << " " << std::endl;

	try
	{
		scene -> setWorldURL (uri);
		scene -> setEncoding (EncodingType::XML);

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
}

void
XMLParser::xmlElement (xmlpp::Element* const xmlElement)
{
	using ElementsFunction = std::function <void (XMLParser*, xmlpp::Element* const)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("X3D", std::mem_fn (&XMLParser::x3dElement)),
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
XMLParser::x3dElement (xmlpp::Element* const xmlElement)
{
	std::string profileCharacters;
	std::string specificationVersionCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("profile"), profileCharacters))
	{
		try
		{
			scene -> setProfile (getBrowser () -> getProfile (profileCharacters));
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (stringAttribute (xmlElement -> get_attribute ("version"), specificationVersionCharacters))
	{
		try
		{
			scene -> setSpecificationVersion (specificationVersionCharacters);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Parse children.

	for (const auto & xmlNode : xmlElement -> get_children ())
		x3dChild (dynamic_cast <xmlpp::Element*> (xmlNode));
}

void
XMLParser::x3dChild (xmlpp::Element* const xmlElement)
{
	using ElementsFunction = std::function <void (XMLParser*, xmlpp::Element* const)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("head",  std::mem_fn (&XMLParser::headElement)),
		std::make_pair ("Scene", std::mem_fn (&XMLParser::sceneElement)),
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
XMLParser::headElement (xmlpp::Element* const xmlElement)
{
	for (const auto & xmlNode : xmlElement -> get_children ())
		headChild (dynamic_cast <xmlpp::Element*> (xmlNode));
}

void
XMLParser::headChild (xmlpp::Element* const xmlElement)
{
	using ElementsFunction = std::function <void (XMLParser*, xmlpp::Element* const)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("component", std::mem_fn (&XMLParser::componentElement)),
		std::make_pair ("unit",      std::mem_fn (&XMLParser::unitElement)),
		std::make_pair ("meta",      std::mem_fn (&XMLParser::metaElement)),
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
XMLParser::componentElement (xmlpp::Element* const xmlElement)
{
	std::string componentNameCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("name"), componentNameCharacters))
	{
		int32_t componentSupportLevel;

		if (integerAttribute (xmlElement -> get_attribute ("level"), componentSupportLevel))
		{
			try
			{
				const auto component = getBrowser () -> getComponent (componentNameCharacters, componentSupportLevel);

				scene -> updateComponent (component);
				return;
			}
			catch (const X3D::X3DError & error)
			{
				getBrowser () -> println (error .what ());
				return;
			}
		}
		else
			getBrowser () -> println ("Expected a component support level.");
	}
	else
		getBrowser () -> println ("Expected a component name.");
}

void
XMLParser::unitElement (xmlpp::Element* const xmlElement)
{
	std::string categoryNameCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("category"), categoryNameCharacters))
	{
		std::string unitNameCharacters;

		if (stringAttribute (xmlElement -> get_attribute ("name"), unitNameCharacters))
		{
			double unitConversionFactor;

			if (floatAttribute (xmlElement -> get_attribute ("conversionFactor"), unitConversionFactor))
			{
				try
				{
					scene -> updateUnit (categoryNameCharacters, unitNameCharacters, unitConversionFactor);
					return;
				}
				catch (const X3DError & error)
				{
					getBrowser () -> println (error .what ());
					return;
				}
			}
			else
				getBrowser () -> println ("Expected unit conversion factor.");
		}
		else
			getBrowser () -> println ("Expected unit name identificator.");
	}
	else
		getBrowser () -> println ("Expected category name identificator after UNIT statement.");
}

void
XMLParser::metaElement (xmlpp::Element* const xmlElement)
{
	std::string metakeyCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("name"), metakeyCharacters))
	{
		std::string metavalueCharacters;

		if (stringAttribute (xmlElement -> get_attribute ("content"), metavalueCharacters))
		{
			scene -> addMetaData (metakeyCharacters, metavalueCharacters);
			return;
		}
		else
			getBrowser () -> println ("Expected metadata value.");
	}
	else
		getBrowser () -> println ("Expected metadata key.");
}

void
XMLParser::sceneElement (xmlpp::Element* const xmlElement)
{

}

bool
XMLParser::floatAttribute (xmlpp::Attribute* const xmlAttribute, double & value)
{
	if (not xmlAttribute)
		return false;

	// TODO: Trim and check for count?

	std::istringstream istream (xmlAttribute -> get_value ());

	istream .imbue (std::locale::classic ());

	return XMLGrammar::DoubleValue (istream, value);
}

bool
XMLParser::integerAttribute (xmlpp::Attribute* const xmlAttribute, int32_t & value)
{
	if (not xmlAttribute)
		return false;

	// TODO: Trim and check for count?

	std::istringstream istream (xmlAttribute -> get_value ());

	istream .imbue (std::locale::classic ());

	return XMLGrammar::IntegerValue (istream, value);
}

bool
XMLParser::stringAttribute (xmlpp::Attribute* const xmlAttribute, std::string & value)
{
	if (not xmlAttribute)
		return false;

	// TODO: Trim? but not meta!

	value = xmlAttribute -> get_value ();

	return true;
}

XMLParser::~XMLParser ()
{ }

} // X3D
} // titania
