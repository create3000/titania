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

#include "X3DBaseTool.h"

namespace titania {
namespace X3D {

X3DBaseTool::X3DBaseTool () :
   X3DBaseNode (),
          node (nullptr)
{
   throw Error <NOT_SUPPORTED> ("X3DBaseTool::X3DBaseTool");
}
 
X3DBaseTool::X3DBaseTool (X3DBaseNode* node) :
	  X3DBaseNode (),
	X3DToolObject (),
	         node (node)
{
	setPrivate (node -> getPrivate ());

	node -> addParent (this);
}

void
X3DBaseTool::initialize ()
{
	X3DBaseNode::initialize ();
	X3DToolObject::initialize ();

	if (not node -> isInitialized ())
		node -> setup ();
}

void
X3DBaseTool::setExecutionContext (X3DExecutionContext* const value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	node -> setExecutionContext (value);

	X3DToolObject::setExecutionContext (value);
	X3DBaseNode::setExecutionContext (value);
}

X3DFieldDefinition*
X3DBaseTool::getField (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
	   return node -> getField (name);
	}
	catch (const Error <INVALID_NAME> &)
	{
		return X3DBaseNode::getField (name);
	}
}

//addInterest must not be overloaded is base node does a setValue in replace.

void
X3DBaseTool::addEvent ()
{
	node -> addEvent ();
	X3DBaseNode::addEvent ();
}

void
X3DBaseTool::removeTool (const bool really)
{
	X3DBaseNode::removeTool (node);
}

void
X3DBaseTool::dispose ()
{
	node -> setPrivate (getPrivate ());
	node -> removeParent (this);

	X3DToolObject::dispose ();
	X3DBaseNode::dispose ();
}

} // X3D
} // titania
