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

#include "X3DPrototypeInstance.h"

#include "../../Bits/Traverse.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/Scene.h"
#include "../../Prototype/ExternProto.h"
#include "../Networking/X3DUrlObject.h"

#include "../../Tools/Core/X3DToolObject.h"

#include <cassert>
#include <iostream>

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

const std::string X3DPrototypeInstance::componentName  = "Core";
const std::string X3DPrototypeInstance::containerField = "children";

X3DPrototypeInstance::X3DPrototypeInstance (X3DExecutionContext* const executionContext, const X3DProtoObjectPtr & _protoObject) :
//throw (Error <INVALID_NAME>,
//       Error <INVALID_OPERATION_TIMING>,
//       Error <DISPOSED>) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	            X3DNode (),
	X3DExecutionContext (),
	        protoObject (_protoObject)
{
	addType (X3DConstants::X3DPrototypeInstance);

	addField (inputOutput, "metadata", metadata ());

	Proto* const prototype = protoObject -> getProtoDeclaration ();

	for (const auto & userDefinedField : prototype -> getUserDefinedFields ())
	{
		addField (userDefinedField -> getAccessType (),
		          userDefinedField -> getName (),
		          *userDefinedField -> clone ());
	}

	addChildren (protoObject);

	// Assign protos and root nodes

	setEncoding             (prototype -> getEncoding ());
	setSpecificationVersion (prototype -> getSpecificationVersion ());
	setCharacterEncoding    (prototype -> getCharacterEncoding ());
	setComment              (prototype -> getComment ());

	setProfile (prototype -> getProfile ());

	for (const auto & component : prototype -> getComponents ())
		addComponent (component);

	setUnits (prototype -> getUnits ());

	try
	{
		importExternProtos (prototype, CloneType { });
		importProtos (prototype, CloneType { });
		importRootNodes (prototype);
	}
	catch (const X3DError &)
	{ }

	setExtendedEventHandling (false);

	getRootNodes () .setAccessType (initializeOnly);
}

X3DPrototypeInstance*
X3DPrototypeInstance::create (X3DExecutionContext* const executionContext) const
{
	return new X3DPrototypeInstance (executionContext, protoObject);
}

void
X3DPrototypeInstance::initialize ()
{
	getExecutionContext () -> isLive () .addInterest (isLive ());

	isLive () = getExecutionContext () -> isLive ();

	try
	{
		// Defer assigning imports and routes until now

		Proto* const prototype = protoObject -> getProtoDeclaration ();

		importImportedNodes (prototype);
		importRoutes (prototype);
	}
	catch (const X3DError &)
	{ }

	X3DNode::initialize ();
	X3DExecutionContext::initialize ();
}

const std::string &
X3DPrototypeInstance::getTypeName () const
throw (Error <DISPOSED>)
{
	if (protoObject)
		return protoObject -> getName ();

	backtrace_fn ();

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getTypeName: node is already disposed.");
}

const X3DBaseNode*
X3DPrototypeInstance::getDeclaration () const
throw (Error <DISPOSED>)
{
	if (protoObject)
		return protoObject .getValue ();

	backtrace_fn ();

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getType: node is already disposed.");
}

X3DBaseNode*
X3DPrototypeInstance::getInnerNode ()
throw (Error <DISPOSED>)
{
	return getRootNode () -> getInnerNode ();
}

const X3DBaseNode*
X3DPrototypeInstance::getInnerNode () const
throw (Error <DISPOSED>)
{
	return getRootNode () -> getInnerNode ();
}

X3DBaseNode*
X3DPrototypeInstance::getRootNode () const
throw (Error <DISPOSED>)
{
	if (getRootNodes () .empty () or not getRootNodes () .front ())
		throw Error <DISPOSED> ("Root node not available.");

	return getRootNodes () .front ();
}

void
X3DPrototypeInstance::traverse (const TraverseType type)
{
	try
	{
		getRootNode () -> traverse (type);
	}
	catch (const X3DError &)
	{ }
}

void
X3DPrototypeInstance::addTool ()
{
	try
	{
		getInnerNode () -> addTool ();
	}
	catch (const X3DError &)
	{ }
}

void
X3DPrototypeInstance::removeTool (const bool really)
{
	try
	{
		getInnerNode () -> removeTool (really);
	}
	catch (const X3DError &)
	{ }
}

void
X3DPrototypeInstance::toStream (std::ostream & ostream) const
//throw (Error <DISPOSED>)
{
	X3DNode::toStream (ostream);
}

void
X3DPrototypeInstance::toXMLStream (std::ostream & ostream) const
//throw (Error <DISPOSED>)
{
	ostream .imbue (std::locale::classic ());

	if (Generator::IsSharedNode (this))
	{
		ostream
			<< Generator::Indent
			<< "<!-- NULL -->";

		return;
	}

	Generator::PushContext ();

	const std::string name = Generator::GetName (this);

	if (not name .empty ())
	{
		if (Generator::ExistsNode (this))
		{
			ostream
				<< Generator::Indent
				<< "<ProtoInstance"
				<< Generator::Space
				<< "USE='"
				<< XMLEncode (name)
				<< "'";

			const auto containerField = Generator::GetContainerField ();

			if (containerField)
			{
				if (containerField -> getName () not_eq getContainerField ())
				{
					ostream
						<< Generator::Space
						<< "containerField='"
						<< XMLEncode (containerField -> getName ())
						<< "'";
				}
			}

			ostream << "/>";

			Generator::PopContext ();

			return;
		}
	}

	ostream
		<< Generator::Indent
		<< "<ProtoInstance"
		<< Generator::Space
		<< "name='"
		<< XMLEncode (getTypeName ())
		<< "'";

	if (not name .empty ())
	{
		Generator::AddNode (this);

		ostream
			<< Generator::Space
			<< "DEF='"
			<< XMLEncode (name)
			<< "'";
	}

	const auto containerField = Generator::GetContainerField ();

	if (containerField)
	{
		if (containerField -> getName () not_eq getContainerField ())
		{
			ostream
				<< Generator::Space
				<< "containerField='"
				<< XMLEncode (containerField -> getName ())
				<< "'";
		}
	}

	const FieldDefinitionArray fields = getChangedFields ();

	if (fields .empty ())
	{
		ostream << "/>";
	}
	else
	{
		ostream
			<< ">"
			<< Generator::Break
			<< Generator::IncIndent;

		FieldDefinitionArray references;

		for (const auto & field : fields)
		{
			// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
			// for this field.

			bool mustOutputValue = false;

			if (field -> getAccessType () == inputOutput and not field -> getReferences () .empty ())
			{
				bool initializeableReference = false;

				for (const auto & reference : field -> getReferences ())
					initializeableReference |= reference -> isInitializeable ();

				try
				{
					if (not initializeableReference)
						mustOutputValue = not isDefaultValue (field);
				}
				catch (const X3DError &)
				{
					mustOutputValue = false;
				}
			}

			if (field -> getReferences () .empty () or mustOutputValue)
			{
				if (mustOutputValue)
					references .emplace_back (field);

				switch (field -> getType ())
				{
					case X3DConstants::MFNode:
					{
						static const MFNode _empty;

						ostream
							<< Generator::Indent
							<< "<fieldValue"
							<< Generator::Space
							<< "name='"
							<< XMLEncode (field -> getName ())
							<< "'";
						
						if (*field == _empty)
						{
							ostream
								<< "/>"
								<< Generator::Break;
						}
						else
						{
							ostream
								<< ">"
								<< Generator::Break
								<< Generator::IncIndent
								<< XMLEncode (field)
								<< Generator::Break
								<< Generator::DecIndent
								<< Generator::Indent
								<< "</fieldValue>"
								<< Generator::Break;
						}

						break;
					}
					case X3DConstants::SFNode:
					{
						static const SFNode _null;

						if (*field not_eq _null)
						{
							ostream
								<< Generator::Indent
								<< "<fieldValue"
								<< Generator::Space
								<< "name='"
								<< XMLEncode (field -> getName ())
								<< "'"
								<< ">"
								<< Generator::Break
								<< Generator::IncIndent
								<< XMLEncode (field)
								<< Generator::Break
								<< Generator::DecIndent
								<< Generator::Indent
								<< "</fieldValue>"
								<< Generator::Break;

							break;
						}

						// Proceed with next case.
					}
					default:
					{
						ostream
							<< Generator::Indent
							<< "<fieldValue"
							<< Generator::Space
							<< "name='"
							<< XMLEncode (field -> getName ())
							<< "'"
							<< Generator::Space
							<< "value='"
							<< XMLEncode (field)
							<< "'"
							<< "/>"
							<< Generator::Break;

						break;
					}
				}
			}
			else
			{
				references .emplace_back (field);
			}
		}

		if (not references .empty ())
		{
			ostream
				<< Generator::Indent
				<< "<IS>"
				<< Generator::Break
				<< Generator::IncIndent;

			for (const auto & field : references)
			{
				for (const auto & reference : field -> getReferences ())
				{
					ostream
						<< Generator::Indent
						<< "<connect"
						<< Generator::Space
						<< "nodeField='"
						<< XMLEncode (field -> getName ())
						<< "'"
						<< Generator::Space
						<< "protoField='"
						<< XMLEncode (reference -> getName ())
						<< "'"
						<< "/>"
						<< Generator::Break;
				}
			}

			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< "</IS>"
				<< Generator::Break;
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< "</ProtoInstance>";
	}

	Generator::PopContext ();
}

void
X3DPrototypeInstance::dispose ()
{
	X3DExecutionContext::dispose ();
	X3DNode::dispose ();
}

} // X3D
} // titania
