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

#ifndef __TITANIA_X3D_COMPONENTS_SHAPE_APPEARANCE_H__
#define __TITANIA_X3D_COMPONENTS_SHAPE_APPEARANCE_H__

#include "../Shape/X3DAppearanceNode.h"

namespace titania {
namespace X3D {

class X3DMaterialNode;
class X3DTextureNode;
class X3DTextureTransformNode;
class X3DShaderNode;

class Appearance :
	public X3DAppearanceNode
{
public:

	///  @name Construction

	Appearance (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

	///  @name Fields

	SFNode &
	lineProperties ()
	{ return *fields .lineProperties; }

	const SFNode &
	lineProperties () const
	{ return *fields .lineProperties; }

	SFNode &
	fillProperties ()
	{ return *fields .fillProperties; }

	const SFNode &
	fillProperties () const
	{ return *fields .fillProperties; }

	SFNode &
	material ()
	{ return *fields .material; }

	const SFNode &
	material () const
	{ return *fields .material; }

	SFNode &
	texture ()
	{ return *fields .texture; }

	const SFNode &
	texture () const
	{ return *fields .texture; }

	SFNode &
	textureTransform ()
	{ return *fields .textureTransform; }

	const SFNode &
	textureTransform () const
	{ return *fields .textureTransform; }

	MFNode &
	shaders ()
	{ return *fields .shaders; }

	const MFNode &
	shaders () const
	{ return *fields .shaders; }

	SFNode &
	blendMode ()
	{ return *fields .blendMode; }

	const SFNode &
	blendMode () const
	{ return *fields .blendMode; }

	///  @name MemberAccess

	const X3DPtr <FillProperties> &
	getFillProperties () const
	{ return fillPropertiesNode; }

	const X3DPtr <LineProperties> &
	getLineProperties () const
	{ return linePropertiesNode; }

	const X3DPtr <X3DMaterialNode> &
	getMaterial () const
	{ return materialNode; }

	const X3DPtr <X3DTextureNode> &
	getTexture () const
	{ return textureNode; }

	const X3DPtr <X3DTextureTransformNode> &
	getTextureTransform () const
	{ return textureTransformNode; }

	const X3DPtrArray <X3DShaderNode> &
	getShaders () const
	{ return shaderNodes; }

	const X3DPtr <X3DShaderNode> &
	getShader () const
	{ return shaderNode; }

	const X3DPtr <BlendMode> &
	getBlendMode () const
	{ return blendModeNode; }

	///  @name Tests

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	virtual
	void
	enable (ShapeContainer* const context) final override;

	virtual
	void
	disable (ShapeContainer* const context) final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_lineProperties ();

	void
	set_fillProperties ();

	void
	set_material ();

	void
	set_texture ();

	void
	set_textureTransform ();

	void
	set_shaders ();

	void
	set_shader ();

	void
	set_blendMode ();

	void
	set_transparent ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const fillProperties;
		SFNode* const lineProperties;
		SFNode* const material;
		SFNode* const texture;
		SFNode* const textureTransform;
		MFNode* const shaders;
		SFNode* const blendMode;
	};

	Fields fields;

	X3DPtr <FillProperties>          fillPropertiesNode;
	X3DPtr <LineProperties>          linePropertiesNode;
	X3DPtr <X3DMaterialNode>         materialNode;
	X3DPtr <X3DTextureNode>          textureNode;
	X3DPtr <X3DTextureTransformNode> textureTransformNode;
	X3DPtrArray <X3DShaderNode>      shaderNodes;
	X3DPtr <X3DShaderNode>           shaderNode;
	X3DPtr <BlendMode>               blendModeNode;

};

} // X3D
} // titania

#endif
