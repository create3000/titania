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

#ifndef __TITANIA_X3D_RENDERING_X3DRENDERER_H__
#define __TITANIA_X3D_RENDERING_X3DRENDERER_H__

#include "../Base/Output.h"
#include "../Basic/X3DBaseNode.h"
#include "../Rendering/ClipPlaneContainer.h"
#include "../Rendering/CollisionArray.h"
#include "../Rendering/CollisionContainer.h"
#include "../Rendering/FogContainer.h"
#include "../Rendering/LightContainer.h"
#include "../Rendering/ProjectiveTextureContainer.h"
#include "../Rendering/ShapeContainer.h"
#include "../Rendering/ViewVolumeStack.h"
#include "../Rendering/X3DCollectableObject.h"
#include "../Types/MatrixStack.h"

#include <memory>
#include <stack>

namespace titania {
namespace X3D {

class GeneratedCubeMapTexture;
class Fog;
class FrameBuffer;
class LocalFog;
class X3DFogObject;
class X3DGroupingNode;
class X3DLayoutNode;
class X3DShaderNode;

using ShadowStack                = std::stack <bool>;
using LocalFogStack              = std::vector <FogContainerPtr>;
using LayoutStack                = std::vector <X3DLayoutNode*>;
using GeneratedCubeMapTextureSet = std::set <GeneratedCubeMapTexture*>;
using ShaderSet                  = std::set <X3DShaderNode*>;

using BlendStack       = std::stack <bool, std::vector <bool>>;
using DepthOffsetStack = std::stack <double, std::vector <double>>;

class X3DRenderObject :
	virtual public X3DBaseNode
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Layer handling

	virtual
	bool
	isIndependent () const
	{ return true; }

	virtual
	X3DLayerNode*
	getLayer () const = 0;

	virtual
	NavigationInfo*
	getNavigationInfo () const = 0;

	virtual
	X3DViewpointNode*
	getViewpoint () const = 0;

	virtual
	X3DBackgroundNode*
	getBackground () const = 0;

	virtual
	Fog*
	getFog () const = 0;

	///  @name Flags

	BlendStack &
	getBlend ()
	{ return blend; }

	DepthOffsetStack &
	getDepthOffset ()
	{ return depthOffset; }

	///  @name Matrix stacks

	Matrix4dStack &
	getCameraSpaceMatrix ()
	{ return cameraSpaceMatrix; }

	const Matrix4dStack &
	getCameraSpaceMatrix () const
	{ return cameraSpaceMatrix; }

	Matrix4dStack &
	getInverseCameraSpaceMatrix ()
	{ return invCameraSpaceMatrix; }

	const Matrix4dStack &
	getInverseCameraSpaceMatrix () const
	{ return invCameraSpaceMatrix; }

	Matrix4dStack &
	getProjectionMatrix ()
	{ return projectionMatrix; }

	const Matrix4dStack &
	getProjectionMatrix () const
	{ return projectionMatrix; }

	Matrix4dStack &
	getModelViewMatrix ()
	{ return modelViewMatrix; }

	const Matrix4dStack &
	getModelViewMatrix () const
	{ return modelViewMatrix; }

	ViewVolumeStack &
	getViewVolumes ()
	{ return viewVolumeStack; }

	///  @name Node stacks

	CollectableObjectArray &
	getGlobalObjects ()
	{ return globalObjects; }

	CollectableObjectArray &
	getLocalObjects ()
	{ return localObjects; }

	LocalFogStack &
	getLocalFogs ()
	{ return localFogs; }

	const LocalFogStack &
	getLocalFogs () const
	{ return localFogs; }

	LightContainerArray &
	getLights ()
	{ return lights; }

	virtual
	const std::shared_ptr <LightContainer> &
	getLightContainer () const;

	void
	setLightIndex (const size_t value)
	{ lightIndex = value; }

	void
	pushShadow (const bool value)
	{ return shadow .push (shadow .top () or value); }

	void
	popShadow ()
	{ return shadow .pop (); }

	bool
	getShadow () const
	{ return shadow .top (); }

	LayoutStack &
	getLayouts ()
	{ return layouts; }

	X3DLayoutNode*
	getParentLayout ()
	{ return layouts .empty () ? nullptr : layouts .back (); }

	ProjectiveTextureContainerArray &
	getProjectiveTextures ()
	{ return projectiveTextures; }

	virtual
	const std::shared_ptr <ProjectiveTextureContainer> &
	getProjectiveTextureContainer () const;

	GeneratedCubeMapTextureSet &
	getGeneratedCubeMapTextures ()
	{ return generatedCubeMapTextures; }

	ShaderSet &
	getShaders ()
	{ return shaders; }

	CollisionArray &
	getCollisions ()
	{ return collisions; }

	///  @name Shape handling

	bool
	addCollisionShape (X3DShapeNode* const shape);

	bool
	addDepthShape (X3DShapeNode* const shape);

	bool
	addDisplayShape (X3DShapeNode* const shape);

	///  @name Observer

	size_t
	getNumOpaqueShapes () const
	{ return numOpaqueShapes; }

	const ShapeContainerArray &
	getOpaqueShapes () const
	{ return opaqueShapes; }

	size_t
	getNumTransparentShapes () const
	{ return numTransparentShapes; }

	const ShapeContainerArray &
	getTransparentShapes () const
	{ return transparentShapes; }

	///  @name Operations

	Vector3d
	constrainTranslation (const Vector3d & translation, const bool stepBack) const;

	virtual
	void
	render (const TraverseType type, const TraverseFunction & traverse);

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DRenderObject () override;


protected:

	///  @name Construction

	X3DRenderObject ();

	virtual
	void
	initialize () override;


private:

	///  @name Operations

	void
	setGlobalFog (Fog* const fog);

	double
	getDistance (const Vector3d &) const;

	double
	getDepth (const Matrix4d &) const;

	void
	collide ();

	void
	gravite ();

	void
	depth (const CollisionContainerArray &, const size_t);

	void
	draw (const TraverseFunction & traverse);

	///  @name Members

	BlendStack       blend;
	DepthOffsetStack depthOffset;

	Matrix4dStack cameraSpaceMatrix;
	Matrix4dStack invCameraSpaceMatrix;
	Matrix4dStack projectionMatrix;
	Matrix4dStack modelViewMatrix;

	ViewVolumeStack                 viewVolumeStack;
	CollectableObjectArray          globalObjects;
	CollectableObjectArray          localObjects;
	LocalFogStack                   localFogs;
	LightContainerArray             lights;
	size_t                          lightIndex;
	ShadowStack                     shadow;
	LayoutStack                     layouts;
	ProjectiveTextureContainerArray projectiveTextures;
	size_t                          projectiveTextureIndex;
	GeneratedCubeMapTextureSet      generatedCubeMapTextures;
	ShaderSet                       shaders;
	CollisionArray                  collisions;

	ShapeContainerArray      opaqueShapes;
	ShapeContainerArray      transparentShapes;
	CollisionContainerArray  collisionShapes;
	std::vector <Collision*> activeCollisions;
	CollisionContainerArray  depthShapes;

	std::unique_ptr <FrameBuffer> depthBuffer;
	double                        speed;

	size_t numOpaqueShapes;
	size_t numTransparentShapes;
	size_t numCollisionShapes;
	size_t numDepthShapes;

};

} // X3D
} // titania

#endif
