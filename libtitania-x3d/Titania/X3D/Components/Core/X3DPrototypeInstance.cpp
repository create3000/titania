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
#include "../../Prototype/Proto.h"
#include "../Networking/X3DUrlObject.h"

#include <cassert>
#include <iostream>

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

const std::string X3DPrototypeInstance::componentName  = "Core";
const std::string X3DPrototypeInstance::containerField = "children";

X3DPrototypeInstance::X3DPrototypeInstance (X3DExecutionContext* const executionContext, const X3DSFNode <X3DProto> & prototype) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DExecutionContext (),
	   protoDeclaration (prototype),
	      savedChildren ()
{
	addNodeType (X3DConstants::X3DPrototypeInstance);

	addField (inputOutput, "metadata", metadata ());

	Proto* const proto = protoDeclaration -> getProto ();

	for (const auto & userDefinedField : proto -> getUserDefinedFields ())
	{
		addField (userDefinedField -> getAccessType (),
		          userDefinedField -> getName (),
		          *userDefinedField -> clone ());
	}

	addChildren (protoDeclaration, savedChildren);

	// Assign protos and root nodes

	if (not executionContext -> isProto ())
	{
		setEncoding             (proto -> getEncoding ());
		setSpecificationVersion (proto -> getSpecificationVersion ());
		setCharacterEncoding    (proto -> getCharacterEncoding ());
		setComment              (proto -> getComment ());

		setWorldURL (proto -> getWorldURL ());

		setProfile (proto -> getProfile ());

		for (const auto & component : proto -> getComponents ())
			addComponent (component);

		setUnits (proto -> getUnits ());

		cloneExternProtos (proto);
		cloneProtos (proto);
		copyRootNodes (proto);
	}

	setExtendedEventHandling (false);

	getRootNodes () .setAccessType (initializeOnly);
}

X3DPrototypeInstance*
X3DPrototypeInstance::create (X3DExecutionContext* const executionContext) const
{
	if (executionContext -> isScene ())
	{
		X3DProto* const protoDeclaration = executionContext -> findProtoDeclaration (getTypeName ());

		protoDeclaration -> requestImmediateLoad ();

		return new X3DPrototypeInstance (executionContext, protoDeclaration);
	}

	return new X3DPrototypeInstance (executionContext, protoDeclaration);
}

X3DPrototypeInstance*
X3DPrototypeInstance::copy (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DPrototypeInstance* copy = dynamic_cast <X3DPrototypeInstance*> (X3DBaseNode::copy (executionContext));

	try
	{
		X3DFieldDefinition* const field = copy -> getField ("url");

		if (field -> getType () == X3DConstants::MFString)
			X3DUrlObject::transform (*static_cast <MFString*> (field), getExecutionContext (), executionContext);
	}
	catch (const Error <INVALID_NAME> &)
	{ }

	return copy;
}

void
X3DPrototypeInstance::initialize ()
{
	X3DExecutionContext::initialize ();

	if (not getExecutionContext () -> isProto ())
	{
		// Defer assigning imports and routes until now

		Proto* const proto = protoDeclaration -> getProto ();

		copyImportedNodes (proto);
		copyRoutes (proto);
	}
}

const std::string &
X3DPrototypeInstance::getTypeName () const
throw (Error <DISPOSED>)
{
	if (protoDeclaration)
		return protoDeclaration -> getName ();

	backtrace_fn ();

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getTypeName: node is already disposed.");
}

const X3DBaseNode*
X3DPrototypeInstance::getType () const
throw (Error <DISPOSED>)
{
	if (protoDeclaration)
		return protoDeclaration .getValue ();

	backtrace_fn ();

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getType: node is already disposed.");
}

X3DBaseNode*
X3DPrototypeInstance::getInnerNode ()
{
	return getRootNode () -> getInnerNode ();
}

X3DBaseNode*
X3DPrototypeInstance::getRootNode () const
{
	//assert (getRootNodes () .size ());

	return getRootNodes () .front ();
}

void
X3DPrototypeInstance::saveState ()
{
	if (isSaved ())
		return;

	X3DExecutionContext::saveState ();

	// Delete children of node if not in scene

	std::set <X3D::SFNode> children;

	// Collect children

	X3D::traverse (getRootNodes (), [&children] (X3D::SFNode & child)
	               {
	                  children .emplace (child);
	                  return true;
						});

	children .erase (this);

	// Filter out scene nodes

	X3D::traverse (getScene () -> getRootNodes (), [&children] (X3D::SFNode & node)
	               {
	                  // If scene node in children, remove from children.
	                  children .erase (node);
	                  return true;
						});

	// Save state of rest, this are only nodes not in scene

	for (const auto & child : children)
	{
		child -> saveState ();

		savedChildren .emplace_back (child);
	}
}

void
X3DPrototypeInstance::restoreState ()
{
	if (not isSaved ())
		return;

	X3DExecutionContext::restoreState ();

	for (const auto & child : savedChildren)
		child -> restoreState ();

	savedChildren .clear ();
}

void
X3DPrototypeInstance::traverse (const TraverseType type)
{
	getRootNode () -> traverse (type);
}

void
X3DPrototypeInstance::toStream (std::ostream & ostream) const
{
	X3DBaseNode::toStream (ostream);
}

void
X3DPrototypeInstance::toXMLStream (std::ostream & ostream) const
{
	X3DBaseNode::toXMLStream (ostream);
}

} // X3D
} // titania
