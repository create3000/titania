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

#ifndef __TITANIA_X3D_TOOLS_NETWORKING_INLINE_TOOL_H__
#define __TITANIA_X3D_TOOLS_NETWORKING_INLINE_TOOL_H__

#include "../Core/X3DChildNodeTool.h"
#include "../Grouping/X3DBoundedObjectTool.h"
#include "../Networking/X3DUrlObjectTool.h"
#include "../ToolColors.h"

#include "../../Components/Networking/Inline.h"

namespace titania {
namespace X3D {

class InlineTool :
	virtual public Inline,
	public X3DChildNodeTool,
	public X3DBoundedObjectTool,
	public X3DUrlObjectTool
{
public:

	///  @name Construction

	InlineTool (X3DBaseNode* const node) :
		         X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		              Inline (node -> getExecutionContext ()),
		         X3DBaseTool (node),
		    X3DChildNodeTool (),
		X3DBoundedObjectTool (ToolColors::WHITE),
		    X3DUrlObjectTool ()
	{
		addType (X3DConstants::InlineTool);
	}

	///  @name Fields

	virtual
	SFBool &
	load ()
	{ return getNode <Inline> () -> load (); }

	virtual
	const SFBool &
	load () const
	{ return getNode <Inline> () -> load (); }

	///  @name Member access

	///  Copies this node and sets the execution context to @a executionContext.
	virtual
	X3DUrlObject*
	copy (X3DExecutionContext* const executionContext, const CopyType type) const final override
	{ return getNode <Inline> () -> copy (executionContext, type); }

	///  @name Member access

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override
	{ X3DChildNodeTool::setExecutionContext (executionContext); }

	virtual
	Box3d
	getBBox () const final override
	{ return X3DBoundedObjectTool::getBBox (); }

	///  @name Operations

	virtual
	void
	requestImmediateLoad () final override
	{ return X3DUrlObjectTool::requestImmediateLoad (); }

	///  @name Root node handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	MFNode &
	getRootNodes () final override
	{ return getNode <Inline> () -> getRootNodes (); }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const MFNode &
	getRootNodes () const final override
	{ return getNode <Inline> () -> getRootNodes (); }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const X3DScenePtr &
	getInternalScene () const final override
	{ return getNode <Inline> () -> getInternalScene (); }

	///  @name Exported node handling

	///  throws Error <INVALID_NAME>, Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	SFNode
	getExportedNode (const std::string & exportedName) const final override
	{ return getNode <Inline> () -> getExportedNode (exportedName); }

	///  throws Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const ExportedNodeIndex &
	getExportedNodes () const final override
	{ return getNode <Inline> () -> getExportedNodes (); }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{
		X3DChildNodeTool::traverse (type, renderObject);
		X3DBoundedObjectTool::traverse (type, renderObject);
		X3DUrlObjectTool::traverse (type, renderObject);
	}

	///  @name Destruction

	virtual
	void
	dispose () final override
	{
		X3DUrlObjectTool::dispose ();
		X3DBoundedObjectTool::dispose ();
		X3DChildNodeTool::dispose ();
	}


private:

	///  @name Construction

	virtual
	void
	initialize () final override
	{
		X3DChildNodeTool::initialize ();
		X3DBoundedObjectTool::initialize ();
		X3DUrlObjectTool::initialize ();
	}

};

} // X3D
} // titania

#endif
