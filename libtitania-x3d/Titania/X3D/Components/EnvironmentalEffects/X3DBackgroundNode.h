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

#ifndef __TITANIA_X3D_COMPONENTS_ENVIRONMENTAL_EFFECTS_X3DBACKGROUND_NODE_H__
#define __TITANIA_X3D_COMPONENTS_ENVIRONMENTAL_EFFECTS_X3DBACKGROUND_NODE_H__

#include "../Core/X3DBindableNode.h"
#include "../Texturing/X3DTextureNode.h"

#include "../../Rendering/ClipPlaneContainer.h"

namespace titania {
namespace X3D {

class X3DBackgroundNode :
	public X3DBindableNode
{
public:

	///  @name Fields

	MFFloat &
	groundAngle ()
	{ return *fields .groundAngle; }

	const MFFloat &
	groundAngle () const
	{ return *fields .groundAngle; }

	MFColor &
	groundColor ()
	{ return *fields .groundColor; }

	const MFColor &
	groundColor () const
	{ return *fields .groundColor; }

	MFFloat &
	skyAngle ()
	{ return *fields .skyAngle; }

	const MFFloat &
	skyAngle () const
	{ return *fields .skyAngle; }

	MFColor &
	skyColor ()
	{ return *fields .skyColor; }

	const MFColor &
	skyColor () const
	{ return *fields .skyColor; }

	SFFloat &
	transparency ()
	{ return *fields .transparency; }

	const SFFloat &
	transparency () const
	{ return *fields .transparency; }

	///  @name Member access

	void
	isHidden (const bool);

	bool
	isHidden () const
	{ return hidden; }

	bool
	isTransparent () const;

	const X3DPtr <X3DTextureNode> &
	getFrontTexture () const
	{ return textures [0]; }

	const X3DPtr <X3DTextureNode> &
	getBackTexture () const
	{ return textures [1]; }

	const X3DPtr <X3DTextureNode> &
	getLeftTexture () const
	{ return textures [2]; }

	const X3DPtr <X3DTextureNode> &
	getRightTexture () const
	{ return textures [3]; }

	const X3DPtr <X3DTextureNode> &
	getTopTexture () const
	{ return textures [4]; }

	const X3DPtr <X3DTextureNode> &
	getBottomTexture () const
	{ return textures [5]; }


	///  @name Operations

	void
	transitionStart (X3DBackgroundNode* const)
	{ }

	void
	transitionStop ()
	{ }

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	void
	draw (X3DRenderObject* const renderObject, const Vector4i &);

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBackgroundNode () override;


protected:

	///  @name Construction

	X3DBackgroundNode ();

	virtual
	void
	initialize () override;

	///  @name Event handling

	void
	setFrontTexture (X3DTextureNode* const value)
	{ textures [0] = value; }

	void
	setBackTexture (X3DTextureNode* const value)
	{ textures [1] = value; }

	void
	setLeftTexture (X3DTextureNode* const value)
	{ textures [2] = value; }

	void
	setRightTexture (X3DTextureNode* const value)
	{ textures [3] = value; }

	void
	setTopTexture (X3DTextureNode* const value)
	{ textures [4] = value; }

	void
	setBottomTexture (X3DTextureNode* const value)
	{ textures [5] = value; }


private:

	///  @name Operations

	virtual
	void
	bindToLayer (X3DLayerNode* const) final override;

	virtual
	void
	unbindFromLayer (X3DLayerNode* const) final override;

	virtual
	void
	removeFromLayer (X3DLayerNode* const) final override;

	static Color3f
	getColor (const float, const MFColor &, const MFFloat &);

	void
	buildHalfSphere (const double, const std::vector <double> &, const MFFloat &, const MFColor &, const float, const bool);

	void
	build ();

	void
	drawSphere (X3DRenderObject* const renderObject);

	void
	drawCube (X3DRenderObject* const renderObject);

	///  @name Static members

	static const std::vector <Matrix4d> cubeRotations;
	static const std::vector <Vector4f> cubeTexCoords;
	static const std::vector <Vector3d> cubeVertices;
	static const double                 cubeScale;

	///  @name Fields

	struct Fields
	{
		Fields ();

		MFFloat* const groundAngle;
		MFColor* const groundColor;
		MFFloat* const skyAngle;
		MFColor* const skyColor;
		SFFloat* const transparency;
	};

	Fields fields;

	///  @name Members

	X3DPtrArray <X3DTextureNode> textures;
	bool                         hidden;
	CollectableObjectArray       clipPlanes;
	Matrix4d                     projectionMatrix;
	Matrix4d                     modelMatrix;
	std::vector <Color4f>        sphereColors;
	std::vector <Vector3d>       sphereVertices;
	GLuint                       sphereColorBufferId;
	GLuint                       sphereVertexBufferId;
	GLuint                       cubeTexCoordBufferId;
	GLuint                       cubeVertexBufferId;

};

} // X3D
} // titania

#endif
