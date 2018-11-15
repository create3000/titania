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

#ifndef __TITANIA_X3D_TOOLS_NAVIGATION_X3DURL_OBJECT_TOOL_H__
#define __TITANIA_X3D_TOOLS_NAVIGATION_X3DURL_OBJECT_TOOL_H__

#include "../Core/X3DBaseTool.h"

#include "../../Components/Networking/X3DUrlObject.h"

namespace titania {
namespace X3D {

class X3DUrlObjectTool :
	virtual public X3DUrlObject,
	virtual public X3DBaseTool
{
public:

	///  @name Construction

	///  Copies this node and sets the execution context to @a executionContext.
	virtual
	X3DUrlObject*
	copy (const CopyType type) const override
	{ return getNode <X3DUrlObject> () -> copy (type); }

	///  Copies this node and sets the execution context to @a executionContext.
	virtual
	X3DUrlObject*
	copy (X3DExecutionContext* const executionContext, const CopyType type) const override
	{ return getNode <X3DUrlObject> () -> copy (executionContext, type); }

	///  @name Fields

	virtual
	MFString &
	url () final override
	{ return getNode <X3DUrlObject> () -> url (); }

	virtual
	const MFString &
	url () const final override
	{ return getNode <X3DUrlObject> () -> url (); }

	///  @name Member access

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override
	{ X3DBaseTool::setExecutionContext (executionContext); }

	///  @name Operations

	virtual
	void
	requestImmediateLoad () override
	{ return getNode <X3DUrlObject> () -> requestImmediateLoad (); }

	///  @name Member access

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const final override
	{ return getNode <X3DUrlObject> () -> checkLoadState (); }

	virtual
	const SFTime &
	file_changed () const final override
	{ return getNode <X3DUrlObject> () -> file_changed (); }

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }


protected:

	///  @name Construction

	X3DUrlObjectTool () :
		X3DUrlObject (),
		 X3DBaseTool ()
	{
		addType (X3DConstants::X3DUrlObjectTool);
	}

	virtual
	void
	initialize () override
	{ }

};

} // X3D
} // titania

#endif
