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

#include "../../Basic/Traverse.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DScene.h"
#include "../../Prototype/ExternProtoDeclaration.h"
#include "../Networking/X3DUrlObject.h"

#include "../../Tools/Core/X3DToolObject.h"

#include <cassert>
#include <iostream>

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

const ComponentType X3DPrototypeInstance::component      = ComponentType::CORE;
const std::string   X3DPrototypeInstance::containerField = "children";

X3DPrototypeInstance::X3DPrototypeInstance (X3DExecutionContext* const executionContext, const X3DProtoDeclarationNodePtr & protoNode_) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	            X3DNode (),
	X3DExecutionContext (),
	          protoNode (protoNode_),
	               live (true)
{
	addType (X3DConstants::X3DPrototypeInstance);

	addField (inputOutput, "metadata", metadata ());

	addChildren (getRootNodes (), protoNode, live);

	protoNode -> addInstance (this);

	for (const auto & userDefinedField : protoNode -> getUserDefinedFields ())
	{
		addField (userDefinedField -> getAccessType (),
		          userDefinedField -> getName (),
		          *userDefinedField -> copy (FLAT_COPY));
	}

	if (protoNode -> isExternproto ())
	{
		if (protoNode -> checkLoadState () == COMPLETE_STATE)
			construct ();
	}
	else
	{
		ProtoDeclaration* const proto = protoNode -> getProtoDeclaration ();

		metadata () = proto -> metadata ();

		importExternProtos (proto);
		importProtos (proto);
		copyRootNodes (proto);

		setExtendedEventHandling (false);
	}

	getRootNodes () .setAccessType (initializeOnly);
}

X3DPrototypeInstance*
X3DPrototypeInstance::create (X3DExecutionContext* const executionContext) const
{
	try
	{
		return executionContext -> findProtoDeclaration (getTypeName (), AvailableType { }) -> createInstance (executionContext);
	}
	catch (const X3DError & error)
	{
		throw Error <INVALID_NAME> (error .what ());
	}
}

void
X3DPrototypeInstance::construct ()
{
	try
	{
		setExtendedEventHandling (false);

		if (protoNode -> checkLoadState () not_eq COMPLETE_STATE)
			return;

		protoNode -> checkLoadState () .removeInterest (this, &X3DPrototypeInstance::construct);

		// Interface

		ProtoDeclaration* const proto = protoNode -> getProtoDeclaration ();

		metadata () = proto -> metadata ();

		const auto & fieldDefinitions = proto -> getFieldDefinitions ();

		for (const auto fieldDefinition : fieldDefinitions)
		{
			try
			{
				const auto protoField = proto -> getField (fieldDefinition -> getName ());
				const auto field      = getField (fieldDefinition -> getName ());

				// Return if something is wrong.
				if (field -> getAccessType () not_eq protoField -> getAccessType ())
					continue;

				// Return if something is wrong.
				if (field -> getType () not_eq protoField -> getType ())
					continue;

				// Continue if field is eventIn or eventOut.
				if (not (field -> getAccessType () & initializeOnly))
					continue;

				// Is set during parse or an event occured.
				if (field -> isSet ())
					continue;

				// Has IS references.
				if (not field -> getReferences () .empty ()) // hasIsReferences
					continue;

				field -> set (*protoField);
			}
			catch (const X3DError &)
			{
				// Definition exists in proto but does not exist in extern proto.

				addField (fieldDefinition -> getAccessType (),
				          fieldDefinition -> getName (),
				          *fieldDefinition -> copy (FLAT_COPY));
			}
		}

		// Assign protos and root nodes

		importExternProtos (proto); // XXX: deletable if all get/set are virtual
		importProtos (proto);       // XXX: deletable if all get/set are virtual
		copyRootNodes (proto);

		if (isInitialized ())
		{
			copyImportedNodes (proto);
			copyRoutes (proto);

			X3DNode::initialize ();
			X3DExecutionContext::initialize ();
			X3DChildObject::addEvent ();
		}
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DPrototypeInstance::update ()
{
	X3DExecutionContext::dispose ();

	try
	{
		const auto proto = protoNode -> getProtoDeclaration ();

		for (const auto & fieldDefinition : getFieldDefinitions ())
		{
			try
			{
				const auto protoField = proto -> getField (fieldDefinition -> getName ());

				if (fieldDefinition -> getType () not_eq protoField -> getType ())
					removeField (fieldDefinition -> getName ());

				else
					fieldDefinition -> setAccessType (protoField -> getAccessType ());
			}
			catch (const X3DError & error)
			{
				removeField (fieldDefinition -> getName ());
			}
		}		
	}
	catch (const X3DError & error)
	{
		getBrowser () -> print (error .what ());
	}

	construct ();

	const_cast <SFTime &> (fields_changed ()) = getCurrentTime ();
}

void
X3DPrototypeInstance::initialize ()
{
	try
	{
		switch (protoNode -> checkLoadState ())
		{
			case NOT_STARTED_STATE:
				protoNode -> requestAsyncLoad ();
				// Procceed with next case:
			case IN_PROGRESS_STATE:
				protoNode -> checkLoadState () .addInterest (this, &X3DPrototypeInstance::construct);
				break;

			case COMPLETE_STATE:
			{
				if (protoNode -> isExternproto () and getExtendedEventHandling ())
					construct ();

				ProtoDeclaration* const proto = protoNode -> getProtoDeclaration ();

				copyImportedNodes (proto);
				copyRoutes (proto);
				break;
			}
			case FAILED_STATE:
				break;
		}

		getExecutionContext () -> isLive () .addInterest (this, &X3DPrototypeInstance::set_live);
		X3DBaseNode::isLive () .addInterest (this, &X3DPrototypeInstance::set_live);

		set_live ();

		// Now initialize bases.

		X3DNode::initialize ();
		X3DExecutionContext::initialize ();
	}
	catch (const X3DError &)
	{ }
}

void
X3DPrototypeInstance::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getExecutionContext () -> isLive () .removeInterest (this, &X3DPrototypeInstance::set_live);

	X3DExecutionContext::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (this, &X3DPrototypeInstance::set_live);

		set_live ();
	}
}

const std::string &
X3DPrototypeInstance::getTypeName () const
throw (Error <DISPOSED>)
{
	if (protoNode)
		return protoNode -> getName ();

	//backtrace_fn ();

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getTypeName: node is already disposed.");
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

X3DProtoDeclarationNode*
X3DPrototypeInstance::findProtoDeclaration (const std::string & name, const AvailableType & available) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (protoNode)
		return protoNode -> getProtoDeclaration () -> findProtoDeclaration (name, available);

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getType: node is already disposed.");
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
X3DPrototypeInstance::set_live ()
{
	const bool value = getExecutionContext () -> isLive () and X3DBaseNode::isLive ();

	if (value not_eq live)
		live = value;
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

	Generator::EnterScope ();

	const std::string & name = Generator::Name (this);

	if (not name .empty ())
	{
		if (Generator::ExistsNode (this))
		{
			ostream
				<< Generator::Indent
				<< "<ProtoInstance"
				<< Generator::Space
				<< "name='"
				<< XMLEncode (getTypeName ())
				<< "'"
				<< Generator::Space
				<< "USE='"
				<< XMLEncode (name)
				<< "'";

			const auto containerField = Generator::ContainerField ();

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

			Generator::LeaveScope ();

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

	const auto containerField = Generator::ContainerField ();

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

			if (Generator::ExecutionContext () and not Generator::IsSharedNode (this))
			{
				if (field -> getAccessType () == inputOutput and not field -> getReferences () .empty ())
				{
					bool initializableReference = false;

					for (const auto & reference : field -> getReferences ())
						initializableReference |= reference -> isInitializable ();

					try
					{
						if (not initializableReference)
							mustOutputValue = not isDefaultValue (field);
					}
					catch (const X3DError &)
					{
						mustOutputValue = false;
					}
				}
			}

			// If we have no execution context we are not in a proto and must not generate IS references the same is true
			// if the node is a shared node as the node does not belong to the execution context.

			if ((field -> getReferences () .empty () or not Generator::ExecutionContext () or Generator::IsSharedNode (this)) or mustOutputValue)
			{
				if (mustOutputValue)
					references .emplace_back (field);

				switch (field -> getType ())
				{
					case X3DConstants::MFNode:
					{
						ostream
							<< Generator::Indent
							<< "<fieldValue"
							<< Generator::Space
							<< "name='"
							<< XMLEncode (field -> getName ())
							<< "'";

						if (static_cast <MFNode*> (field) -> empty ())
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
						static const SFNode null_;

						if (*field not_eq null_)
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

	Generator::LeaveScope ();
}

void
X3DPrototypeInstance::dispose ()
{
	try
	{
		protoNode -> removeInstance (this);
	}
	catch (const X3D::X3DError &)
	{ }

	X3DExecutionContext::dispose ();
	X3DNode::dispose ();

	removeChildren (getRootNodes ());
}

} // X3D
} // titania
