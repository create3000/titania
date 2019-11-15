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

#ifndef __TITANIA_X3D_TOOLS_LIGHTING_X3DLIGHT_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_LIGHTING_X3DLIGHT_NODE_TOOL_H__

#include "../Core/X3DChildNodeTool.h"

#include "../../Components/Grouping/X3DBoundedObject.h"
#include "../../Components/Lighting/X3DLightNode.h"

namespace titania {
namespace X3D {

class X3DLightNodeTool :
	virtual public X3DLightNode,
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
	SFBool &
	global () final override
	{ return getNode <X3DLightNode> () -> global (); }

	virtual
	const SFBool &
	global () const final override
	{ return getNode <X3DLightNode> () -> global (); }

	virtual
	SFBool &
	on () final override
	{ return getNode  <X3DLightNode>() -> on (); }

	virtual
	const SFBool &
	on () const final override
	{ return getNode <X3DLightNode> () -> on (); }

	virtual
	SFColor &
	color () final override
	{ return getNode <X3DLightNode> () -> color (); }

	virtual
	const SFColor &
	color () const final override
	{ return getNode <X3DLightNode> () -> color (); }

	virtual
	SFFloat &
	intensity () final override
	{ return getNode <X3DLightNode> () -> intensity (); }

	virtual
	const SFFloat &
	intensity () const final override
	{ return getNode <X3DLightNode> () -> intensity (); }

	virtual
	SFFloat &
	ambientIntensity () final override
	{ return getNode <X3DLightNode> () -> ambientIntensity (); }

	virtual
	const SFFloat &
	ambientIntensity () const final override
	{ return getNode <X3DLightNode> () -> ambientIntensity (); }

	///  @name Experimental

	virtual
	SFColor &
	shadowColor () final override
	{ return getNode <X3DLightNode> () -> shadowColor (); }

	virtual
	const SFColor &
	shadowColor () const final override
	{ return getNode <X3DLightNode> () -> shadowColor (); }

	virtual
	SFFloat &
	shadowIntensity () final override
	{ return getNode <X3DLightNode> () -> shadowIntensity (); }

	virtual
	const SFFloat &
	shadowIntensity () const final override
	{ return getNode <X3DLightNode> () -> shadowIntensity (); }

	virtual
	SFFloat &
	shadowBias () final override
	{ return getNode <X3DLightNode> () -> shadowBias (); }

	virtual
	const SFFloat &
	shadowBias () const final override
	{ return getNode <X3DLightNode> () -> shadowBias (); }

	virtual
	SFInt32 &
	shadowMapSize () final override
	{ return getNode <X3DLightNode> () -> shadowMapSize (); }

	virtual
	const SFInt32 &
	shadowMapSize () const final override
	{ return getNode <X3DLightNode> () -> shadowMapSize (); }

	///  @name Member access

	virtual
	const Color3f &
	getColor () const final override
	{ return getNode <X3DLightNode> () -> getColor (); }

	virtual
	float
	getAmbientIntensity () const final override
	{ return getNode <X3DLightNode> () -> getAmbientIntensity (); }

	virtual
	float
	getIntensity () const final override
	{ return getNode <X3DLightNode> () -> getIntensity (); }

	virtual
	const Color3f &
	getShadowColor () const final override
	{ return getNode <X3DLightNode> () -> getShadowColor (); }

	virtual
	float
	getShadowIntensity () const final override
	{ return getNode <X3DLightNode> () -> getShadowIntensity (); }

	virtual
	float
	getShadowBias () const final override
	{ return getNode <X3DLightNode> () -> getShadowBias (); }

	virtual
	size_t
	getShadowMapSize () const override
	{ return getNode <X3DLightNode> () -> getShadowMapSize (); }

	///  @name Operations

	virtual
	void
	push (X3DRenderObject* const renderObject, X3DGroupingNode* const group) final override
	{ getNode <X3DLightNode> () -> push (renderObject, group); }

	virtual
	void
	pop (X3DRenderObject* const renderObject) final override
	{ getNode  <X3DLightNode>() -> pop (renderObject); }

	virtual
	void
	draw (const GLenum lightId) override
	{ getNode <X3DLightNode> () -> draw (lightId); }

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	virtual
	void
	addTool () final override;

	virtual
	void
	removeTool (const bool really) final override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	X3DLightNodeTool ();

	virtual
	void
	initialize () override;

	virtual
	void
	realize () override;

};

} // X3D
} // titania

#endif
