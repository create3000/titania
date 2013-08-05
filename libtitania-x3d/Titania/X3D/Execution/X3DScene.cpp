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

#include "X3DScene.h"

#include "../Bits/Error.h"
#include "../Parser/Parser.h"
#include "../Parser/RegEx.h"

namespace titania {
namespace X3D {

X3DScene::X3DScene () :
	X3DExecutionContext (),
	          metadatas (),
	      exportedNodes (),
	      exportedNames ()
{ }

// MetaData handling

void
X3DScene::addMetaData (const std::string & key, const std::string & value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	metadatas .insert (std::make_pair (key, value));
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

const X3DSFNode <ExportedNode> &
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
	exportedNodes .back () .isTainted (true);
	exportedNodes .back () .addParent (this);

	exportedNames [node] = exportedName;

	return exportedNodes .back ();
}

void
X3DScene::removeExportedNode (const std::string & exportedName)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	exportedNodes .erase (exportedName);
}

void
X3DScene::updateExportedNode (const std::string & exportedName,  const SFNode & node)
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

const SFNode &
X3DScene::getExportedNode (const std::string & exportedName) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return exportedNodes .rfind (exportedName) -> getNode ();
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Exported node '" + exportedName + "' not found.");
	}
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

	setupNodes ();
}

void
X3DScene::toStream (std::ostream & ostream) const
{
	bool X3D = (getEncoding () == "X3D");

	Generator::X3DAccessTypes (X3D);
	Generator::X3DFieldNames (X3D);

	ostream
		<< '#'
		<< getEncoding ()
		<< Generator::Space
		<< 'V'
		<< getSpecificationVersion ()
		<< Generator::Space
		<< getCharacterEncoding ();

	if (getComment () .length ())
	{
		ostream
			<< Generator::Space
			<< getComment ();
	}

	ostream << std::endl << std::endl;

	if (getComments () .size ())
	{
		for (const auto & comment : getComments ())
		{
			ostream
				<< Generator::Comment
				<< comment
				<< Generator::Break;
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

	if (getComponents () .size ())
	{
		for (const auto & component : getComponents ())
		{
			ostream
				<< *component
				<< Generator::Break;
		}

		ostream << Generator::TidyBreak;
	}

	if (getMetaDatas () .size ())
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

	Generator::PushContext ();

	X3DExecutionContext::toStream (ostream);

	if (getExportedNodes () .size ())
		ostream << Generator::TidyBreak;

	for (const auto & exportedNode : getExportedNodes ())
		ostream << exportedNode;

	Generator::PopContext ();

	ostream << std::flush;
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
