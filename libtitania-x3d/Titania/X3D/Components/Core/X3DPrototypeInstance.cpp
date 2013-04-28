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

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/Scene.h"
#include "../../Prototype/ExternProto.h"
#include "../../Prototype/Proto.h"

#include <cassert>
#include <iostream>

namespace titania {
namespace X3D {

X3DPrototypeInstance::X3DPrototypeInstance (X3DExecutionContext* const executionContext, Proto* const prototype) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DExecutionContext (),                                                    
	              proto (prototype)                                            
{
	addNodeType (X3DConstants::X3DPrototypeInstance);

	addField (inputOutput, "metadata", metadata ());

	for (const auto & userDefinedField : proto -> getUserDefinedFields ())
	{
		addField (userDefinedField -> getAccessType (),
		          userDefinedField -> getName (),
		          *userDefinedField -> clone ());
	}

	setChildren (proto);
}

X3DPrototypeInstance*
X3DPrototypeInstance::create (X3DExecutionContext* const executionContext) const
{
	return new X3DPrototypeInstance (executionContext, *proto);
}

void
X3DPrototypeInstance::setup ()
{
	assign (*proto);

	X3DExecutionContext::setup ();
}

const std::string &
X3DPrototypeInstance::getTypeName () const
throw (Error <DISPOSED>)
{
	if (proto)
		return proto .getNodeName () [0];

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getTypeName: node is already disposed.");
}

const X3DBaseNode*
X3DPrototypeInstance::getType () const
throw (Error <DISPOSED>)
{
	if (proto)
		return proto .getValue ();

	throw Error <DISPOSED> ("Error: X3DPrototypeInstance::getType: node is already disposed.");
}

X3DBaseNode*
X3DPrototypeInstance::getLocalNode ()
{
	return getRootNode () -> getLocalNode ();
}

X3DBaseNode*
X3DPrototypeInstance::getRootNode ()
{
	//assert (getRootNodes () .size ());

	return getRootNodes () .front ();
}

void
X3DPrototypeInstance::traverse (TraverseType type)
{
	getRootNode () -> traverse (type);
}

void
X3DPrototypeInstance::toStream (std::ostream & ostream) const
{
	X3DBaseNode::toStream (ostream);
}

void
X3DPrototypeInstance::dispose ()
{
	X3DExecutionContext::dispose ();

	proto .dispose ();
}

X3DPrototypeInstance::~X3DPrototypeInstance ()
{
	//removeField ("metadata");

	for (const auto & field : basic::adapter (getFieldDefinitions () .begin () + 1, getFieldDefinitions () .end ()))
		delete field;
}

} // X3D
} // titania
