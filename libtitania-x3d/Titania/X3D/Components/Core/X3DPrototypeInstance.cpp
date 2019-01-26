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

#include "X3DPrototypeInstance.h"

#include "../../Bits/Traverse.h"
#include "../../Browser/PointingDeviceSensor/HierarchyGuard.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DScene.h"
#include "../../Prototype/ExternProtoDeclaration.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../Networking/X3DUrlObject.h"

#include "../../Tools/Core/X3DToolObject.h"

#include <cassert>
#include <iostream>

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

const Component   X3DPrototypeInstance::component      = Component ("Core", 2);
const std::string X3DPrototypeInstance::containerField = "children";

X3DPrototypeInstance::X3DPrototypeInstance (X3DExecutionContext* const executionContext, const X3DProtoDeclarationNodePtr & p_protoNode) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	            X3DNode (),
	X3DExecutionContext (),
	          protoNode (p_protoNode),
	     typeNameOutput (),
	               live (true),
	      fieldMappings ()
{
	addType (X3DConstants::X3DPrototypeInstance);

	addField (inputOutput, "metadata", metadata ());

	addChildObjects (getRootNodes (), protoNode, typeNameOutput, live);

	protoNode -> addInstance (this);
	protoNode -> name_changed () .addInterest (typeNameOutput);

	for (const auto & protoField : protoNode -> getUserDefinedFields ())
	{
		const auto field = protoField -> copy (FLAT_COPY);

		addField (protoField -> getAccessType (),
		          protoField -> getName (),
		          *field);

		fieldMappings .emplace (protoField, field);
	}

	if (protoNode -> isExternproto ())
	{
		if (protoNode -> checkLoadState () == COMPLETE_STATE)
			construct ();
		else
			protoNode -> checkLoadState () .addInterest (&X3DPrototypeInstance::construct, this);
	}
	else
	{
		ProtoDeclaration* const proto = protoNode -> getProtoDeclaration ();

		metadata () = proto -> metadata ();

		importExternProtos (proto, COPY_OR_CLONE);
		importProtos       (proto, COPY_OR_CLONE);
		copyRootNodes      (proto, COPY_OR_CLONE);
	}

	getRootNodes () .setAccessType (initializeOnly);

	setExtendedEventHandling (false);
}

SFNode &
X3DPrototypeInstance::metadata ()
{
	try
	{
		return getField <SFNode> ("metadata");
	}
	catch (const X3DError &)
	{
		return X3DNode::metadata ();
	}
}

const SFNode &
X3DPrototypeInstance::metadata () const
{
	try
	{
		return getField <SFNode> ("metadata");
	}
	catch (const X3DError &)
	{
		return X3DNode::metadata ();
	}
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
		if (protoNode -> checkLoadState () == FAILED_STATE)
		{
			if (isInitialized ())
				getBrowser () -> removeLoadCount (this);

			return;
		}

		if (protoNode -> checkLoadState () not_eq COMPLETE_STATE)
			return;

		if (protoNode -> isExternproto ())
		{
			protoNode -> checkLoadState () .removeInterest (&X3DPrototypeInstance::construct, this);
			protoNode -> checkLoadState () .addInterest (&X3DPrototypeInstance::update, this);
		}

		// Interface

		fieldMappings .clear ();

		ProtoDeclaration* const proto = protoNode -> getProtoDeclaration ();

		if (not metadata () .isSet ())
			metadata () = proto -> metadata ();

		for (const auto & protoField : proto -> getFieldDefinitions ())
		{
			try
			{
				const auto field = getField (protoField -> getName ());

				fieldMappings .emplace (protoField, field);

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

				if (field -> equals (*protoField))
					continue;

				// If default value of protoField is different from field update default value for field.
				*field = *protoField;
			}
			catch (const X3DError &)
			{
				// Definition exists in proto but does not exist in extern proto.

				const auto field = protoField -> copy (FLAT_COPY);

				addField (protoField -> getAccessType (),
				          protoField -> getName (),
				          *field);

				fieldMappings .emplace (protoField, field);
			}
		}

		// Assign protos and root nodes

		importExternProtos (proto, COPY_OR_CLONE);
		importProtos       (proto, COPY_OR_CLONE);
		copyRootNodes      (proto, COPY_OR_CLONE);

		if (isInitialized ())
		{
			getBrowser () -> removeLoadCount (this);

			copyImportedNodes (proto, COPY_OR_CLONE);
			copyRoutes (proto, COPY_OR_CLONE);

			X3DNode::initialize ();
			X3DExecutionContext::initialize ();
			X3DChildObject::addEvent ();
		}
	}
	catch (const X3DError & error)
	{
		__LOG__ << getTypeName () << " : " << error .what () << std::endl;
	}
}

void
X3DPrototypeInstance::update ()
{
	try
	{
		using addRoute = const RoutePtr &(X3DExecutionContext::*) (const SFNode &, const std::string &, const SFNode &, const std::string &);

		if (protoNode -> checkLoadState () not_eq COMPLETE_STATE)
			return;

		const auto tool = hasTool ();

		X3DExecutionContext::dispose ();

		const auto proto  = protoNode -> getProtoDeclaration ();
		const auto fields = FieldArray (getFieldDefinitions () .cbegin (), getFieldDefinitions () .cend ());
		const auto map    = fieldMappings;
		auto       routes = std::vector <std::function <void ()>> ();

		for (const auto & fieldPtr : fields)
			removeField (fieldPtr .getValue () -> getName ());

		fieldMappings .clear ();

		for (const auto & protoField : proto -> getFieldDefinitions ())
		{
			const auto iter = map .find (protoField);

			// Reuse old field.

			if (iter not_eq map .end ())
			{
				const auto field = iter -> second;

				addField (protoField -> getAccessType (),
				          protoField -> getName (),
				          *field);

				fieldMappings .emplace (protoField, field);
				continue;
			}

			// Otherwise create new field.

			const auto field = protoField -> copy (FLAT_COPY);

			// Find suitable field and assign value. Probably access type has changed.

			for (const auto & pair : map)
			{
				const auto currentField = pair .second;
				const auto inputRoutes  = currentField -> getInputRoutes ();
				const auto outputRoutes = currentField -> getOutputRoutes ();

				for (const auto inputRoute : inputRoutes)
				{
					routes .emplace_back (std::bind ((addRoute) &X3DExecutionContext::addRoute,
					                                 inputRoute -> getExecutionContext (),
					                                 inputRoute -> getSourceNode (),
					                                 inputRoute -> getSourceField (),
					                                 inputRoute -> getDestinationNode (),
					                                 inputRoute -> getDestinationField ()));
					
					inputRoute -> getExecutionContext () -> deleteRoute (inputRoute);
				}

				for (const auto outputRoute : outputRoutes)
				{
					routes .emplace_back (std::bind ((addRoute) &X3DExecutionContext::addRoute,
					                                 outputRoute -> getExecutionContext (),
					                                 outputRoute -> getSourceNode (),
					                                 outputRoute -> getSourceField (),
					                                 outputRoute -> getDestinationNode (),
					                                 outputRoute -> getDestinationField ()));
					
					outputRoute -> getExecutionContext () -> deleteRoute (outputRoute);
				}

				if (protoField -> getName () == currentField -> getName ())
				{
					if (protoField -> getType () == currentField -> getType ())
					{
						field -> set (*currentField);
						field -> isSet (true);
						break;
					}
					else
					{
						switch (protoField -> getType ())
						{
							case X3DConstants::SFNode:
							{
								if (currentField -> getType () == X3DConstants::MFNode)
								{
									const auto mfnode = static_cast <MFNode*> (currentField);
	
									if (not mfnode -> empty ())
										field -> set (mfnode -> front ());
	
									field -> isSet (true);
								}
	
								break;
							}
							case X3DConstants::MFNode:
							{
								if (currentField -> getType () == X3DConstants::SFNode)
								{
									const auto sfnode = static_cast <SFNode*> (currentField);
	
									field -> set (MFNode ({ *sfnode }));
									field -> isSet (true);
								}
	
								break;
							}
							default:
							{
								break;
							}
						}
					}
				}
			}

			addField (protoField -> getAccessType (),
			          protoField -> getName (),
			          *field);

			fieldMappings .emplace (protoField, field);
		}

		for (const auto & route : routes)
		{
			try
			{
				route ();
			}
			catch (const X3DError & error)
			{ }
		}

		construct ();

		if (tool)
			addTool ();
	}
	catch (const X3DError & error)
	{
		// If extern proto isn't loaded.
		__LOG__ << getTypeName () << " : " << error .what () << std::endl;
	}
}

void
X3DPrototypeInstance::initialize ()
{
	try
	{
		switch (protoNode -> checkLoadState ())
		{
			case NOT_STARTED_STATE:
			{
				protoNode -> requestAsyncLoad ();

				if (protoNode -> isExternproto ())
					getBrowser () -> addLoadCount (this);

				break;
			}
			case IN_PROGRESS_STATE:
			{
				if (protoNode -> isExternproto ())
					getBrowser () -> addLoadCount (this);

				break;
			}
			case COMPLETE_STATE:
			{
				if (not protoNode -> isExternproto ())
				{
					ProtoDeclaration* const proto = protoNode -> getProtoDeclaration ();
	
					copyImportedNodes (proto, COPY_OR_CLONE);
					copyRoutes (proto, COPY_OR_CLONE);
				}

				break;
			}
			case FAILED_STATE:
				break;
		}

		getExecutionContext () -> isLive () .addInterest (&X3DPrototypeInstance::set_live, this);
		X3DBaseNode::isLive () .addInterest (&X3DPrototypeInstance::set_live, this);
		getRootNodes () .addInterest ([&] () { X3DChildObject::addEvent (); });

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
{
	getExecutionContext () -> isLive () .removeInterest (&X3DPrototypeInstance::set_live, this);

	X3DExecutionContext::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (&X3DPrototypeInstance::set_live, this);

		set_live ();
	}
}

///  throws Error <DISPOSED>
const std::string &
X3DPrototypeInstance::getTypeName () const
{
	if (protoNode)
		return protoNode -> getName ();

	//backtrace_fn ();

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getTypeName: node is already disposed.");
}

void
X3DPrototypeInstance::setProtoDeclarationNode (const X3DProtoDeclarationNodePtr & value)
{
	protoNode -> removeInstance (this);
	protoNode -> name_changed () .removeInterest (typeNameOutput);

	protoNode = value;

	protoNode -> addInstance (this);
	protoNode -> name_changed () .addInterest (typeNameOutput);

	update ();

	typeNameOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DProtoDeclarationNode*
X3DPrototypeInstance::findProtoDeclaration (const std::string & name, const AvailableType & available) const
{
	if (protoNode)
		return protoNode -> getProtoDeclaration () -> findProtoDeclaration (name, available);

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getType: node is already disposed.");
}

///  throws Error <DISPOSED>
X3DBaseNode*
X3DPrototypeInstance::getInnerNode ()
{
	return getRootNode () -> getInnerNode ();
}

///  throws Error <DISPOSED>
const X3DBaseNode*
X3DPrototypeInstance::getInnerNode () const
{
	return getRootNode () -> getInnerNode ();
}

///  throws Error <DISPOSED>
X3DBaseNode*
X3DPrototypeInstance::getRootNode () const
{
	if (getRootNodes () .empty () or not getRootNodes () .front ())
		throw Error <DISPOSED> ("Root node not available.");

	return getRootNodes () .front ();
}

void
X3DPrototypeInstance::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	// This function is normally not called, as the root node is optimized and there traverse is called.

	try
	{
		getRootNode () -> traverse (type, renderObject);
	}
	catch (const X3DError &)
	{ }
}

bool
X3DPrototypeInstance::hasTool () const
{
	try
	{
		return getInnerNode () -> isType ({ X3DConstants::X3DToolObject });
	}
	catch (const X3DError & error)
	{
		return false;
	}
}

void
X3DPrototypeInstance::addTool ()
{
	try
	{
		getInnerNode () -> addTool ();
	}
	catch (const X3DError & error)
	{ }
}

void
X3DPrototypeInstance::removeTool (const bool really)
{
	try
	{
		getInnerNode () -> removeTool (really);
	}
	catch (const X3DError & error)
	{ }
}

void
X3DPrototypeInstance::set_live ()
{
	const bool value = getExecutionContext () -> isLive () and X3DBaseNode::isLive ();

	if (value == live)
		return;

	live = value;
}

///  throws Error <DISPOSED>
void
X3DPrototypeInstance::toStream (std::ostream & ostream) const
{
	X3DNode::toStream (ostream);
}

///  throws Error <DISPOSED>
void
X3DPrototypeInstance::toXMLStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	if (Generator::IsSharedNode (ostream, this))
	{
		ostream
			<< Generator::Indent
			<< "<!-- NULL -->";

		return;
	}

	Generator::EnterScope (ostream);

	const std::string & name = Generator::Name (ostream, this);

	if (not name .empty ())
	{
		if (Generator::ExistsNode (ostream, this))
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

			const auto containerField = Generator::ContainerField (ostream);

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

			Generator::LeaveScope (ostream);

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
		Generator::AddNode (ostream, this);

		ostream
			<< Generator::Space
			<< "DEF='"
			<< XMLEncode (name)
			<< "'";
	}

	const auto containerField = Generator::ContainerField (ostream);

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

	FieldDefinitionArray fields   = getChangedFields ();

	if (fields .empty ())
	{
		ostream << "/>";
	}
	else
	{
		ostream
			<< ">"
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		if (not fields .empty ())
		{
			FieldDefinitionArray references;
	
			for (const auto & field : fields)
			{
				// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
				// for this field.
	
				bool mustOutputValue = false;
	
				if (Generator::ExecutionContext (ostream))
				{
					if (field -> getAccessType () == inputOutput and not field -> getReferences () .empty ())
					{
						bool initializableReference = false;
	
						for (const auto & reference : field -> getReferences ())
							initializableReference |= reference -> isInitializable ();
	
						if (not initializableReference)
							mustOutputValue = true;
					}
				}
	
				// If we have no execution context we are not in a proto and must not generate IS references the same is true
				// if the node is a shared node as the node does not belong to the execution context.
	
				if (field -> getReferences () .empty () or not Generator::ExecutionContext (ostream) or mustOutputValue)
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
									<< Generator::TidyBreak;
							}
							else
							{
								Generator::PushContainerField (ostream, field);

								ostream
									<< ">"
									<< Generator::TidyBreak
									<< Generator::IncIndent
									<< XMLEncode (field)
									<< Generator::TidyBreak
									<< Generator::DecIndent
									<< Generator::Indent
									<< "</fieldValue>"
									<< Generator::TidyBreak;

								Generator::PopContainerField (ostream);
							}
	
							break;
						}
						case X3DConstants::SFNode:
						{
							static const SFNode null_;
	
							if (not field -> equals (null_))
							{
								Generator::PushContainerField (ostream, field);

								ostream
									<< Generator::Indent
									<< "<fieldValue"
									<< Generator::Space
									<< "name='"
									<< XMLEncode (field -> getName ())
									<< "'"
									<< ">"
									<< Generator::TidyBreak
									<< Generator::IncIndent
									<< XMLEncode (field)
									<< Generator::TidyBreak
									<< Generator::DecIndent
									<< Generator::Indent
									<< "</fieldValue>"
									<< Generator::TidyBreak;

								Generator::PopContainerField (ostream);
	
								break;
							}
	
							[[fallthrough]];
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
								<< Generator::TidyBreak;
	
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
					<< Generator::TidyBreak
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
							<< Generator::TidyBreak;
					}
				}
	
				ostream
					<< Generator::DecIndent
					<< Generator::Indent
					<< "</IS>"
					<< Generator::TidyBreak;
			}
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< "</ProtoInstance>";
	}

	Generator::LeaveScope (ostream);
}

///  throws Error <DISPOSED>
void
X3DPrototypeInstance::toJSONStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	if (Generator::IsSharedNode (ostream, this))
	{
		ostream << "null";
		return;
	}

	Generator::EnterScope (ostream);

	const std::string & name = Generator::Name (ostream, this);

	// USE name

	if (not name .empty ())
	{
		if (Generator::ExistsNode (ostream, this))
		{
			ostream
				<< '{'
				<< Generator::TidySpace
				<< '"'
				<< "ProtoInstance"
				<< '"'
				<< ':'
				<< Generator::TidyBreak
				<< Generator::IncIndent
				<< Generator::Indent
				<< '{'
				<< Generator::TidyBreak
				<< Generator::IncIndent

				<< Generator::Indent
				<< '"'
				<< "@name"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '"'
				<< getTypeName ()
				<< '"'
				<< ','
				<< Generator::TidyBreak

				<< Generator::Indent
				<< '"'
				<< "@USE"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< JSONEncode (SFString (name))
				<< Generator::TidyBreak

				<< Generator::DecIndent
				<< Generator::Indent
				<< '}'
				<< Generator::TidyBreak
				<< Generator::DecIndent
				<< Generator::Indent
				<< '}';

			Generator::LeaveScope (ostream);
			return;
		}
	}

	bool lastProperty = false;

	// Type name

	ostream
		<< '{'
		<< Generator::TidySpace
		<< '"'
		<< "ProtoInstance"
		<< '"'
		<< ':'
		<< Generator::TidyBreak
		<< Generator::IncIndent
		<< Generator::Indent
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent;


	// DEF name

	if (not name .empty ())
	{
		Generator::AddNode (ostream, this);

		ostream
			<< Generator::Indent
			<< '"'
			<< "@DEF"
			<< '"'
			<< ':'
			<< Generator::TidySpace
			<< JSONEncode (SFString (name));

		lastProperty = true;
	}


	// Type name

	if (lastProperty)
	{
		ostream
			<< ','
			<< Generator::TidyBreak;
	}

	ostream
		<< Generator::Indent
		<< '"'
		<< "@name"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< SFString (getTypeName ());

	lastProperty = true;


	// Fields

	FieldDefinitionArray fields = getChangedFields ();
	FieldDefinitionArray references;

	if (fields .empty ())
		;
	else
	{
		if (lastProperty)
		{
			ostream
				<< ','
				<< Generator::TidyBreak;
		}

		if (not fields .empty ())
		{
			ostream
				<< Generator::Indent
				<< '"'
				<< "fieldValue"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '['
				<< Generator::TidyBreak
				<< Generator::IncIndent;
	
			FieldDefinitionArray outputFields;
	
			for (const auto & field : fields)
			{
				// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
				// for this field.
		
				bool mustOutputValue = false;
		
				if (Generator::ExecutionContext (ostream))
				{
					if (field -> getAccessType () == inputOutput and not field -> getReferences () .empty ())
					{
						bool initializableReference = false;
		
						for (const auto & reference : field -> getReferences ())
							initializableReference |= reference -> isInitializable ();
		
						if (not initializableReference)
							mustOutputValue = true;
					}
				}
		
				// If we have no execution context we are not in a proto and must not generate IS references the same is true
				// if the node is a shared node as the node does not belong to the execution context.
		
				if (field -> getReferences () .empty () or not Generator::ExecutionContext (ostream) or mustOutputValue)
				{
					if (mustOutputValue)
						references .emplace_back (field);
	
					outputFields .emplace_back (field);
				}
				else
				{
					references .emplace_back (field);
				}
			}
	
			for (const auto & field : outputFields)
			{
				switch (field -> getType ())
				{
					case X3DConstants::MFNode:
					{
						ostream
							<< Generator::Indent
							<< '{'
							<< Generator::TidyBreak
							<< Generator::IncIndent
							<< Generator::Indent
							<< '"'
							<< "@name"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< SFString (field -> getName ())
							<< ','
							<< Generator::TidyBreak
							<< Generator::Indent
							<< '"'
							<< "-children"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< JSONEncode (field)
							<< Generator::TidyBreak
							<< Generator::DecIndent
							<< Generator::Indent
							<< '}';
	
						break;
					}
					case X3DConstants::SFNode:
					{
						ostream
							<< Generator::Indent
							<< '{'
							<< Generator::TidyBreak
							<< Generator::IncIndent
							<< Generator::Indent
							<< '"'
							<< "@name"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< SFString (field -> getName ())
							<< ','
							<< Generator::TidyBreak
							<< Generator::Indent
							<< '"'
							<< "-children"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< '['
							<< Generator::TidyBreak
							<< Generator::IncIndent
							<< Generator::Indent
							<< JSONEncode (field)
							<< Generator::TidyBreak
							<< Generator::DecIndent
							<< Generator::Indent
							<< ']'
							<< Generator::TidyBreak
							<< Generator::DecIndent
							<< Generator::Indent
							<< '}';
		
						break;
					}
					default:
					{
						ostream
							<< Generator::Indent
							<< '{'
							<< Generator::TidyBreak
							<< Generator::IncIndent
							<< Generator::Indent
							<< '"'
							<< "@name"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< SFString (field -> getName ())
							<< ','
							<< Generator::TidyBreak
							<< Generator::Indent
							<< '"'
							<< "@value"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< JSONEncode (field)
							<< Generator::TidyBreak
							<< Generator::DecIndent
							<< Generator::Indent
							<< '}';
	
						break;
					}
				}
	
				if (field not_eq outputFields .back ())
				{
					ostream
						<< ','
						<< Generator::TidyBreak;
				}
			}
	
			ostream
				<< Generator::TidyBreak
				<< Generator::DecIndent
				<< Generator::Indent
				<< ']';
	
			lastProperty = true;
		}
	
	
		// IS references
	
		if (not references .empty ())
		{
			if (lastProperty)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
	
			ostream
				<< Generator::Indent
				<< '"'
				<< "IS"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '{'
				<< Generator::TidyBreak
				<< Generator::IncIndent
				<< Generator::Indent
				<< '"'
				<< "connect"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '['
				<< Generator::TidyBreak
				<< Generator::IncIndent;
	
			for (const auto & field : references)
			{
				for (const auto & reference : field -> getReferences ())
				{
					ostream
						<< Generator::Indent
						<< '{'
						<< Generator::TidyBreak
						<< Generator::IncIndent;
	
					ostream
						<< Generator::Indent
						<< '"'
						<< "@nodeField"
						<< '"'
						<< ':'
						<< Generator::TidySpace
						<< SFString (field -> getName ())
						<< ','
						<< Generator::TidyBreak;
	
					ostream
						<< Generator::Indent
						<< '"'
						<< "@protoField"
						<< '"'
						<< ':'
						<< Generator::TidySpace
						<< SFString (reference -> getName ())
						<< Generator::TidyBreak;
	
					ostream
						<< Generator::DecIndent
						<< Generator::Indent
						<< '}';
	
					if (field == references .back () and reference == *--field -> getReferences () .end ())
						;
					else
					{
						ostream << ',';
					}
	
					ostream << Generator::TidyBreak;
				}
			}

			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< ']'
				<< Generator::TidyBreak
				<< Generator::DecIndent
				<< Generator::Indent
				<< '}';
		}

		lastProperty = true;
	}


	// End

	if (lastProperty)
		ostream << Generator::TidyBreak;

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}'
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}';

	Generator::LeaveScope (ostream);
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

	removeChildObjects (getRootNodes ());
}

} // X3D
} // titania
