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

#include "ExportedNode.h"

#include "../Execution/X3DExecutionContext.h"
#include "../Execution/X3DScene.h"

namespace titania {
namespace X3D {

const ComponentType ExportedNode::component      = ComponentType::TITANIA;
const std::string   ExportedNode::typeName       = "ExportedNode";
const std::string   ExportedNode::containerField = "exportedNode";

ExportedNode::ExportedNode (X3DScene* const scene,
                            const std::string & exportedName,
                            const SFNode & node_) :
	 X3DBaseNode (scene -> getBrowser (), scene),
	       scene (scene),
	exportedName (exportedName),
	        node (node_)
{
	addType (X3DConstants::ExportedNode);

	addChildObjects (node);
}

///  throws Error <NOT_SUPPORTED>
[[noreturn]]
X3DBaseNode*
ExportedNode::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating exported nodes is not supported.");
}

///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
ExportedNode*
ExportedNode::copy (X3DExecutionContext* const executionContext, const CopyType) const
{
	throw Error <NOT_SUPPORTED> ("Cloning exported nodes into execution context is not supported.");
}

///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
ExportedNode*
ExportedNode::copy (X3DScene* const scene) const
{
	try
	{
		const auto node = scene -> getNamedNode (getLocalNode () -> getName ());

		return scene -> addExportedNode (exportedName, node) .getValue ();
	}
	catch (const X3DError & error)
	{
		throw Error <INVALID_NAME> ("Bad EXPORT specification in copy: " + std::string (error .what ()));
	}
}

void
ExportedNode::initialize ()
{
	X3DBaseNode::initialize ();

	node .addInterest (&ExportedNode::set_node, this);

	set_node ();
}

///  throws Error <DISPOSED>
SFNode
ExportedNode::getLocalNode () const
{
	if (node)
		return SFNode (node);

	throw Error <DISPOSED> ("ExportedNode::getLocalNode: Node '" + exportedName + "' is already disposed.");
}

void
ExportedNode::set_node ()
{
	if (not node)
		scene -> removeExportedNode (exportedName);
}

///  throws Error <INVALID_NODE>, Error <DISPOSED>
void
ExportedNode::toStream (std::ostream & ostream) const
{
	const std::string & localName = Generator::LocalName (ostream, getLocalNode ());

	if (not getComments () .empty ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & comment : getComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}

		ostream << Generator::TidyBreak;
	}

	ostream
		<< Generator::Indent
		<< "EXPORT"
		<< Generator::Space
		<< localName;

	if (exportedName not_eq localName)
	{
		ostream
			<< Generator::Space
			<< "AS"
			<< Generator::Space
			<< exportedName;
	}
}

///  throws Error <INVALID_NODE>, Error <DISPOSED>
void
ExportedNode::toXMLStream (std::ostream & ostream) const
{
	const std::string & localName = Generator::LocalName (ostream, getLocalNode ());

	ostream
		<< Generator::Indent
		<< "<EXPORT"
		<< Generator::Space
		<< "localDEF='"
		<< XMLEncode (localName)
		<< "'";

	if (exportedName not_eq localName)
	{
		ostream
			<< Generator::Space
			<< "AS='"
			<< XMLEncode (exportedName)
			<< "'";
	}

	ostream << "/>";
}

///  throws Error <INVALID_NODE>, Error <DISPOSED>
void
ExportedNode::toJSONStream (std::ostream & ostream) const
{
	const std::string & localName = Generator::LocalName (ostream, getLocalNode ());

	ostream
		<< '{'
		<< Generator::TidySpace
		<< '"'
		<< "EXPORT"
		<< '"'
		<< ':'
		<< Generator::TidyBreak
		<< Generator::IncIndent
		<< Generator::Indent
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	ostream
		<< Generator::Indent
		<< '"'
		<< "@localDEF"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< SFString (localName);

	if (exportedName not_eq localName)
	{
		ostream
			<< ','
			<< Generator::TidyBreak
			<< Generator::Indent
			<< '"'
			<< "@AS"
			<< '"'
			<< ':'
			<< Generator::TidySpace
			<< SFString (exportedName)
			<< Generator::TidyBreak;
	}
	else
		ostream << Generator::TidyBreak;

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}'
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}';
}

} // X3D
} // titania
