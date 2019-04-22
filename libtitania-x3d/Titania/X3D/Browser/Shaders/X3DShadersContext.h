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

#ifndef __TITANIA_X3D_BROWSER_SHADING_X3DSHADING_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_SHADING_X3DSHADING_CONTEXT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../Shaders/ShadingType.h"

namespace titania {
namespace X3D {

class ComposedShader;
class X3DShaderNode;

class X3DShadersContext :
	virtual public X3DBaseNode
{
public:

	///  @name Member access

	void
	setShaders (const bool value)
	{ shaders = value; }

	const SFBool &
	getShaders () const
	{ return shaders; }

	float
	getShadingLanguageVersion () const
	{ return shadingLanguageVersion; }

	int32_t
	getMaxVertexUniformVectors () const
	{ return maxVertexUniformVectors; }

	int32_t
	getMaxFragmentUniformVectors () const
	{ return maxFragmentUniformVectors; }

	int32_t
	getMaxVertexAttributes () const
	{ return maxVertexAttributes; }

	const X3DPtr <ComposedShader> &
	getPointShader () const
	{ return pointShader; }

	const X3DPtr <ComposedShader> &
	getWireframeShader () const
	{ return wireframeShader; }

	const X3DPtr <ComposedShader> &
	getGouraudShader () const
	{ return gouraudShader; }

	const X3DPtr <ComposedShader> &
	getPhongShader () const
	{ return phongShader; }

	const X3DPtr <ComposedShader> &
	getDefaultShader () const
	{ return defaultShader; }

	const X3DPtr <ComposedShader> &
	getShadowShader () const
	{ return shadowShader; }

	bool
	getMultiTexturing () const
	{ return multiTexturing; }

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }

	virtual
	~X3DShadersContext () override;


protected:

	///  @name Friends

	friend class X3DEnvironmentalEffectsContext;

	///  @name Construction

	X3DShadersContext ();

	virtual
	void
	initialize () override;

	///  Operations

	X3DPtr <ComposedShader>
	createShader (const std::string & name, const MFString & vertexUrl, const MFString & fragmentUrl, const bool shadow = false);


private:

	///  @name Event handlers

	void
	set_gouraud_shader_valid ();

	void
	set_fallback_shader_valid ();

	void
	set_phong_shader_valid ();

	void
	set_shadow_shader_valid ();

	void
	set_shading ();

	///  @name Members

	SFBool                  shaders;
	float                   shadingLanguageVersion;
	int32_t                 maxVertexUniformVectors;
	int32_t                 maxFragmentUniformVectors;
	int32_t                 maxVertexAttributes;
	X3DPtr <ComposedShader> pointShader;
	X3DPtr <ComposedShader> wireframeShader;
	X3DPtr <ComposedShader> gouraudShader;
	X3DPtr <ComposedShader> phongShader;
	X3DPtr <ComposedShader> defaultShader;
	X3DPtr <ComposedShader> shadowShader;
	bool                    multiTexturing;


};

} // X3D
} // titania

#endif
