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

#ifndef __TITANIA_X3D_TOOLS_PROJECTIVE_TEXTURE_MAPPING_X3DTEXTURE_PROJECTOR_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_PROJECTIVE_TEXTURE_MAPPING_X3DTEXTURE_PROJECTOR_NODE_TOOL_H__

#include "../Core/X3DChildNodeTool.h"

#include "../../Components/Grouping/X3DBoundedObject.h"
#include "../../Components/ProjectiveTextureMapping/X3DTextureProjectorNode.h"

namespace titania {
namespace X3D {

class X3DTextureProjectorNodeTool :
	virtual public X3DTextureProjectorNode,
	virtual public X3DChildNodeTool,
	public X3DBoundedObject
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

	///  @name Fields

	virtual
	SFString &
	description () final override
	{ return getNode <X3DTextureProjectorNode> () -> description (); }

	virtual
	const SFString &
	description () const final override
	{ return getNode <X3DTextureProjectorNode> () -> description (); }

	virtual
	SFBool &
	on () final override
	{ return getNode <X3DTextureProjectorNode> () -> on (); }

	virtual
	const SFBool &
	on () const final override
	{ return getNode <X3DTextureProjectorNode> () -> on (); }

	virtual
	SFBool &
	global () final override
	{ return getNode <X3DTextureProjectorNode> () -> global (); }

	virtual
	const SFBool &
	global () const final override
	{ return getNode <X3DTextureProjectorNode> () -> global (); }

	virtual
	SFVec3f &
	location () final override
	{ return getNode <X3DTextureProjectorNode> () -> location (); }

	virtual
	const SFVec3f &
	location () const final override
	{ return getNode <X3DTextureProjectorNode> () -> location (); }

	virtual
	SFVec3f &
	direction () final override
	{ return getNode <X3DTextureProjectorNode> () -> direction (); }

	virtual
	const SFVec3f &
	direction () const final override
	{ return getNode <X3DTextureProjectorNode> () -> direction (); }

	virtual
	SFVec3f &
	upVector () final override
	{ return getNode <X3DTextureProjectorNode> () -> upVector (); }

	virtual
	const SFVec3f &
	upVector () const final override
	{ return getNode <X3DTextureProjectorNode> () -> upVector (); }

	virtual
	SFFloat &
	nearDistance () final override
	{ return getNode <X3DTextureProjectorNode> () -> nearDistance (); }

	virtual
	const SFFloat &
	nearDistance () const final override
	{ return getNode <X3DTextureProjectorNode> () -> nearDistance (); }

	virtual
	SFFloat &
	farDistance () final override
	{ return getNode <X3DTextureProjectorNode> () -> farDistance (); }

	virtual
	const SFFloat &
	farDistance () const final override
	{ return getNode <X3DTextureProjectorNode> () -> farDistance (); }

	virtual
	SFFloat &
	aspectRatio () final override
	{ return getNode <X3DTextureProjectorNode> () -> aspectRatio (); }

	virtual
	const SFFloat &
	aspectRatio () const final override
	{ return getNode <X3DTextureProjectorNode> () -> aspectRatio (); }

	virtual
	SFNode &
	texture () final override
	{ return getNode <X3DTextureProjectorNode> () -> texture (); }

	virtual
	const SFNode &
	texture () const final override
	{ return getNode <X3DTextureProjectorNode> () -> texture (); }

	///  @name Member access

	virtual
	const X3DPtr <X3DTexture2DNode> &
	getTexture () const final override
	{ return getNode <X3DTextureProjectorNode> () -> getTexture (); }

	///  @name Operations

	virtual
	Box3d
	getBBox () const final override;

	virtual
	void
	push (X3DRenderObject* const renderObject) final override
	{ getNode <X3DTextureProjectorNode> () -> push (renderObject); }

	virtual
	void
	pop (X3DRenderObject* const renderObject) final override
	{ getNode  <X3DTextureProjectorNode>() -> pop (renderObject); }

	virtual
	void
	setGlobalVariables (X3DRenderObject* const renderObject, ProjectiveTextureContainer* const container) override
	{ getNode <X3DTextureProjectorNode> () -> setGlobalVariables (renderObject, container); }

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	virtual
	void
	addTool () final override;

	virtual
	void
	removeTool (const bool really) final override;

	virtual
	void
	beginUndo () final override;

	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DTextureProjectorNodeTool () override;


protected:

	///  @name Construction

	X3DTextureProjectorNodeTool ();

	virtual
	void
	initialize () override;

	virtual
	void
	realize () override;


private:

	///  @name Member access

	Vector3f startLocation;
	Vector3f startDirection;
	Vector3f startUpVector;

};

} // X3D
} // titania

#endif
