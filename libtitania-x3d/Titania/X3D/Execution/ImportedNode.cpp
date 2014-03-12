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

#include "ImportedNode.h"

#include "../Bits/Cast.h"
#include "../Components/Networking/Inline.h"
#include "../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string ImportedNode::componentName  = "Browser";
const std::string ImportedNode::typeName       = "ImportedNode";
const std::string ImportedNode::containerField = "importedNode";

ImportedNode::ImportedNode (X3DExecutionContext* const executionContext,
                            const X3DSFNode <Inline> & inlineNode,
                            const std::string & exportedName,
                            const std::string & importedName) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  inlineNode (inlineNode),
	exportedName (exportedName),
	importedName (importedName)
{
	addChildren (this -> inlineNode);

	setup ();
}

X3DBaseNode*
ImportedNode::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating imported nodes is not supported.");
}

ImportedNode*
ImportedNode::clone (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("Cloning imported nodes is not supported.");
}

ImportedNode*
ImportedNode::copy (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	try
	{
		const auto localInlineNode = x3d_cast <Inline*> (executionContext -> getNamedNode (inlineNode -> getName ()));

		return executionContext -> addImportedNode (localInlineNode, exportedName, importedName) .getValue ();
	}
	catch (const X3DError & error)
	{
		throw Error <INVALID_NAME> ("Bad IMPORT specification in copy: " + std::string (error .what ()));
	}
}

const SFNode &
ImportedNode::getExportedNode () const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return inlineNode -> getExportedNode (exportedName);
}

void
ImportedNode::toStream (std::ostream & ostream) const
{
	try
	{
		if (Generator::ExistsNode (inlineNode))
		{
			Generator::AddImportedNode (getExportedNode (), importedName);

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
				<< "IMPORT"
				<< Generator::Space
				<< inlineNode -> getName ()
				<< '.'
				<< exportedName;

			if (importedName not_eq exportedName)
			{
				ostream
					<< Generator::Space
					<< "AS"
					<< Generator::Space
					<< importedName;
			}

			ostream << Generator::Break;
		}
	}
	catch (const X3DError &)
	{ }
}

void
ImportedNode::dispose ()
{
	inlineNode .dispose ();

	X3DBaseNode::dispose ();
}

} // X3D
} // titania
