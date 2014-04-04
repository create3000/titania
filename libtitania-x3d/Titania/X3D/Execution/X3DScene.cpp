/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "../../X3D.h"

#include "X3DScene.h"

#include "../Bits/Error.h"
#include "../Browser/X3DBrowser.h"
#include "../Execution/ExportedNode.h"
#include "../Parser/Parser.h"
#include "../Parser/RegEx.h"

namespace titania {
namespace X3D {

X3DScene::X3DScene () :
	X3DExecutionContext (),
	         compressed (false),
	          metadatas (),
	      exportedNodes (),
	      exportedNames ()
{ }

void
X3DScene::initialize ()
{
	X3D::pushContext ();
	getBrowser () -> makeCurrent ();

	X3DExecutionContext::initialize ();

	X3D::popContext ();
}

std::string
X3DScene::getTitle () const
{
	try
	{
		return getMetaData ("title");
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		return getWorldURL () .basename ();
	}
}

// MetaData handling

void
X3DScene::addMetaData (const std::string & key, const std::string & value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	metadatas .emplace (key, value);
}

void
X3DScene::setMetaData (const std::string & key, const std::string & value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	metadatas .erase (key);
	addMetaData (key, value);
}

const std::string &
X3DScene::getMetaData (const std::string & key) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto range = metadatas .equal_range (key);

	if (range .first not_eq range .second)
		return (-- range .second) -> second;

	throw Error <INVALID_NAME> ("Unkown meta key '" + key + "'.");
}

const MetaDataIndex &
X3DScene::getMetaDatas () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return metadatas;
}

// Exported nodes handling

const ExportedNodePtr &
X3DScene::addExportedNode (const std::string & exportedName, const SFNode & node)
throw (Error <NODE_IN_USE>,
       Error <INVALID_NAME>,
       Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (exportedName .empty ())
		throw Error <INVALID_NAME> ("Couldn't add exported node: exported node name is empty.");

	if (not node)
		throw Error <INVALID_NODE> ("Couldn't add exported node: node is NULL.");

	exportedNodes .push_back (exportedName, new ExportedNode (this, exportedName, node));

	auto & exportedNode = exportedNodes .back ();

	exportedNode .isTainted (true);
	exportedNode .addParent (this);

	if (isInitialized ())
		exportedNode -> setup ();
	else
		addUninitializedNode (exportedNode);

	exportedNames [node] = exportedName;

	return exportedNode;
}

void
X3DScene::removeExportedNode (const std::string & exportedName)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	exportedNodes .erase (exportedName);
}

void
X3DScene::updateExportedNode (const std::string & exportedName, const SFNode & node)
throw (Error <INVALID_NAME>,
       Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (exportedName .empty ())
		throw Error <INVALID_NAME> ("Couldn't update exported node: exported node name is empty.");

	if (not node)
		throw Error <INVALID_NODE> ("Couldn't update exported node: node is NULL.");

	try
	{
		exportedNodes .erase (exportedNames .at (node));
	}
	catch (const std::out_of_range &)
	{ }

	exportedNodes .erase (exportedName);

	addExportedNode (exportedName, node);
}

SFNode
X3DScene::getExportedNode (const std::string & exportedName) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return exportedNodes .rfind (exportedName) -> getNode ();
	}
	catch (...)
	{
		throw Error <INVALID_NAME> ("Exported node '" + exportedName + "' not found.");
	}
}

// Import handling

void
X3DScene::importMetaData (const X3DScene* const scene)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	for (const auto & metaData : scene -> getMetaDatas ())
		addMetaData (metaData .first, metaData .second);
}

void
X3DScene::importExportedNodes (const X3DScene* const scene)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	for (const auto & exportedNode : scene -> getExportedNodes ())
		exportedNode -> copy (this);
}

// Input/Output

void
X3DScene::fromStream (const basic::uri & worldURL, std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	setWorldURL (worldURL);

	fromStream (istream);
}

void
X3DScene::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	Parser (istream, this) .parseIntoScene ();
}

void
X3DScene::toStream (std::ostream & ostream) const
{
	const auto version = getVersion ();

	Generator::Version (version);

	if (version not_eq VRML_V2_0)
		const_cast <X3DScene*> (this) -> setMetaData ("generator", getBrowser () -> getName () + " V" + getBrowser () -> getVersion () + ", http://titania.create3000.de");

	ostream
		<< '#'
		<< version
		<< Generator::Space
		<< getCharacterEncoding ();

	if (true or getComment () .empty ())
	{
		ostream
			<< Generator::Space
			<< getBrowser () -> getName ()
			<< " V"
			<< getBrowser () -> getVersion ();
	}
	else
	{
		ostream
			<< Generator::Space
			<< getComment ();
	}

	ostream
		<< Generator::ForceBreak
		<< Generator::ForceBreak;

	if (not getComments () .empty ())
	{
		for (const auto & comment : getComments ())
		{
			ostream
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}

		ostream << Generator::TidyBreak;
	}

	if (getProfile ())
	{
		ostream
			<< *getProfile ()
			<< Generator::Break
			<< Generator::TidyBreak;
	}

	if (not getComponents () .empty ())
	{
		for (const auto & component : getComponents ())
		{
			ostream
				<< *component
				<< Generator::Break;
		}

		ostream << Generator::TidyBreak;
	}

	{        // Unit
		bool empty = true;

		for (const auto & unit : getUnits ())
		{
			if (unit .getConversion () not_eq 1)
			{
				empty = false;

				ostream
					<< unit
					<< Generator::Break;
			}
		}

		if (not empty)
			ostream << Generator::TidyBreak;
	}

	if (not getMetaDatas () .empty ())
	{
		for (const auto & metaData : getMetaDatas ())
		{
			ostream
				<< "META"
				<< Generator::Space
				<< SFString (metaData .first)
				<< Generator::Space
				<< SFString (metaData .second)
				<< Generator::Break;
		}

		ostream << Generator::TidyBreak;
	}

	// Scene

	Generator::PushContext ();

	X3DExecutionContext::toStream (ostream);

	if (not getExportedNodes () .empty ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & exportedNode : getExportedNodes ())
		{
			try
			{
				ostream << exportedNode;
			}
			catch (const X3DError &)
			{ }
		}
	}

	Generator::PopContext ();

	// ~Scene

	if (not getInnerComments () .empty ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & comment : getInnerComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}
	}

	ostream << std::flush;
}

void
X3DScene::toXMLStream (std::ostream & ostream) const
{
	auto version = getVersion ();

	if (version == VRML_V2_0)
		version = LATEST_VERSION;

	std::string versionString = XMLEncode (version);

	Generator::Version (version);

	const_cast <X3DScene*> (this) -> setMetaData ("generator", getBrowser () -> getName () + " V" + getBrowser () -> getVersion () + ", http://titania.create3000.de");

	ostream
		<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		<< Generator::ForceBreak;

	ostream
		<< "<!DOCTYPE X3D PUBLIC \"ISO//Web3D//DTD X3D "
		<< versionString
		<< "//EN\" \"http://www.web3d.org/specifications/x3d-"
		<< versionString
		<< ".dtd\">"
		<< Generator::ForceBreak;

	ostream
		<< "<X3D"
		<< Generator::Space
		<< "profile='"
		<< (getProfile () ? getProfile () -> getName () : "Full")
		<< "'"
		<< Generator::Space
		<< "version='"
		<< versionString
		<< "'"
		<< Generator::Space
		<< "xmlns:xsd='http://www.w3.org/2001/XMLSchema-instance'"
		<< Generator::Space
		<< "xsd:noNamespaceSchemaLocation='http://www.web3d.org/specifications/x3d-"
		<< versionString
		<< ".xsd'>"
		<< Generator::Break;

	ostream
		<< Generator::IncIndent
		<< Generator::Indent
		<< "<head>"
		<< Generator::Break
		<< Generator::IncIndent;

	// <head>

	for (const auto & component : getComponents ())
	{
		ostream
			<< XMLEncode (component)
			<< Generator::Break;
	}

	for (const auto & unit : getUnits ())
	{
		if (unit .getConversion () not_eq 1)
		{
			ostream
				<< XMLEncode (unit)
				<< Generator::Break;
		}
	}

	for (const auto & metaData : getMetaDatas ())
	{
		ostream
			<< Generator::Indent
			<< "<meta"
			<< Generator::Space
			<< "name='"
			<< XMLEncode (metaData .first)
			<< "'"
			<< Generator::Space
			<< "content='"
			<< XMLEncode (metaData .second)
			<< "'"
			<< "/>"
			<< Generator::Break;
	}

	// </head>

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< "</head>"
		<< Generator::Break
		<< Generator::Indent
		<< "<Scene>"
		<< Generator::Break
		<< Generator::IncIndent;

	// <Scene>

	Generator::PushContext ();

	X3DExecutionContext::toXMLStream (ostream);

	for (const auto & exportedNode : getExportedNodes ())
	{
		try
		{
			ostream
				<< XMLEncode (exportedNode)
				<< Generator::Break;
		}
		catch (const X3DError &)
		{ }
	}

	Generator::PopContext ();

	// </Scene>

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< "</Scene>"
		<< Generator::Break
		<< Generator::DecIndent
		<< "</X3D>"
		<< Generator::Break
		<< std::flush;
}

void
X3DScene::dispose ()
{
	metadatas     .clear ();
	exportedNodes .clear ();
	exportedNames .clear ();

	X3DExecutionContext::dispose ();
}

} // X3D
} // titania
