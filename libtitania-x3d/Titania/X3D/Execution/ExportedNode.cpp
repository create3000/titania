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

#include "ExportedNode.h"

#include "../Execution/X3DExecutionContext.h"
#include "../Execution/X3DScene.h"

namespace titania {
namespace X3D {

const std::string ExportedNode::componentName  = "Browser";
const std::string ExportedNode::typeName       = "ExportedNode";
const std::string ExportedNode::containerField = "exportedNode";

ExportedNode::ExportedNode (X3DScene* const scene,
                            const std::string & exportedName,
                            const SFNode & _node) :
	 X3DBaseNode (scene -> getBrowser (), scene),
	       scene (scene),
	exportedName (exportedName),
	        node (_node)
{
	addChildren (node);
}

X3DBaseNode*
ExportedNode::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating exported nodes is not supported.");
}

ExportedNode*
ExportedNode::clone (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("Cloning exported nodes to execution context is not supported.");
}

ExportedNode*
ExportedNode::copy (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("Copying exported nodes to execution context is not supported.");
}

ExportedNode*
ExportedNode::copy (X3DScene* const scene) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	try
	{
		const auto node = scene -> getNamedNode (getNode () -> getName ());

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

	node .addInterest (this, &ExportedNode::set_node);

	set_node ();
}

SFNode
ExportedNode::getNode () const
throw (Error <DISPOSED>)
{
	if (node and node -> getReferenceCount ())
		return SFNode (node);

	throw Error <DISPOSED> ("ExportedNode: Node '" + exportedName + "' is already disposed.");
}

void
ExportedNode::set_node ()
{
	if (not node)
		scene -> removeExportedNode (exportedName);
}

void
ExportedNode::toStream (std::ostream & ostream) const
//throw (Error <INVALID_NODE>,
//       Error <DISPOSED>)
{
	const std::string localName = Generator::GetLocalName (getNode ());

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

	ostream << Generator::Break;
}

void
ExportedNode::toXMLStream (std::ostream & ostream) const
//throw (Error <INVALID_NODE>,
//       Error <DISPOSED>)
{
	const std::string localName = Generator::GetLocalName (getNode ());

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

} // X3D
} // titania
