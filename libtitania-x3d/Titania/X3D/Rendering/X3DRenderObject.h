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
#include "../Rendering/LightContainer.h"
#include "../Rendering/ShapeContainer.h"
#include "../Rendering/ViewVolumeStack.h"
#include "../Rendering/X3DCollectableObject.h"
#include "../Types/MatrixStack.h"

#include <memory>
#include <stack>

namespace titania {
namespace X3D {

class GeneratedCubeMapTexture;
class FrameBuffer;
class LocalFog;
class X3DFogObject;
class X3DGroupingNode;
class X3DShaderNode;

using LocalFogStack              = std::stack <LocalFog*, std::vector <LocalFog*>>;
using GeneratedCubeMapTextureSet = std::set <GeneratedCubeMapTexture*>;
using ShaderSet                  = std::set <X3DShaderNode*>;

class X3DRenderObject :
	virtual public X3DBaseNode
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Layer handling

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
	X3DFogObject*
	getFog () const = 0;

	///  @name Matrix stacks

	Matrix4dStack &
	getProjectionMatrix ()
	{ return projectionMatrix; }

	const Matrix4dStack &
	getProjectionMatrix () const
	{ return projectionMatrix; }

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
	getModelViewMatrix ()
	{ return modelViewMatrix; }

	const Matrix4dStack &
	getModelViewMatrix () const
	{ return modelViewMatrix; }

	ViewVolumeStack &
	getViewVolumes ()
	{ return viewVolumeStack; }

	///  @name Node stacks

	LightContainerArray &
	getGlobalLights ()
	{ return globalLights; }

	CollectableObjectArray &
	getLocalObjects ()
	{ return localObjects; }

	ClipPlaneContainerArray &
	getClipPlanes ()
	{ return clipPlanes; }

	LocalFogStack &
	getLocalFogs ()
	{ return localFogs; }

	const LocalFogStack &
	getLocalFogs () const
	{ return localFogs; }

	LightContainerArray &
	getLocalLights ()
	{ return localLights; }

	LightContainerArray &
	getLights ()
	{ return lights; }

	CollisionArray &
	getCollisions ()
	{ return collisions; }

	GeneratedCubeMapTextureSet &
	getGeneratedCubeMapTextures ()
	{ return generatedCubeMapTextures; }

	ShaderSet &
	getShaders ()
	{ return shaders; }

	///  @name Observer

	size_t
	getNumOpaqueShapes () const
	{ return numOpaqueDisplayShapes; }

	const ShapeContainerArray &
	getOpaqueShapes () const
	{ return opaqueDisplayShapes; }

	size_t
	getNumTransparentShapes () const
	{ return numTransparentDisplayShapes; }

	const ShapeContainerArray &
	getTransparentShapes () const
	{ return transparentDisplayShapes; }

	///  @name Operations

	Vector3d
	constrainTranslation (const Vector3d & translation) const;

	void
	render (const TraverseType type, const TraverseFunction & traverse);

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DRenderObject ();


protected:

	///  @name Friends

	friend class ParticleSystem;
	friend class Shape;
	friend class X3DLightNode;

	///  @name Construction

	X3DRenderObject ();

	virtual
	void
	initialize () override;

	///  @name Member acess

	const std::shared_ptr <LightContainer> &
	getLight () const;

	void
	addCollisionShape (X3DShapeNode* const shape);

	void
	addDepthShape (X3DShapeNode* const shape);

	void
	addDrawShape (X3DShapeNode* const shape);

	void
	addDisplayShape (X3DShapeNode* const shape);


private:

	///  @name Operations

	void
	addShape (X3DShapeNode* const shape,
	          ShapeContainerArray & opaqueShapes,
	          size_t & numOpaqueShapes,
	          ShapeContainerArray & transparentShapes,
	          size_t & numTransparentShapes);

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
	display (const TraverseFunction & traverse);

	void
	draw (ShapeContainerArray & opaqueShapes,
	      size_t & numOpaqueShapes,
	      ShapeContainerArray & transparentShapes,
	      size_t & numTransparentShapes);

	///  @name Members

	Matrix4dStack projectionMatrix;
	Matrix4dStack cameraSpaceMatrix;
	Matrix4dStack invCameraSpaceMatrix;
	Matrix4dStack modelViewMatrix;

	ViewVolumeStack            viewVolumeStack;
	LightContainerArray        globalLights;
	CollectableObjectArray     localObjects;
	ClipPlaneContainerArray    clipPlanes;
	LocalFogStack              localFogs;
	LightContainerArray        localLights;
	LightContainerArray        lights;
	size_t                     lightId;
	CollisionArray             collisions;
	GeneratedCubeMapTextureSet generatedCubeMapTextures;
	ShaderSet                  shaders;

	ShapeContainerArray      opaqueDrawShapes;
	ShapeContainerArray      transparentDrawShapes;
	ShapeContainerArray      opaqueDisplayShapes;
	ShapeContainerArray      transparentDisplayShapes;
	CollisionContainerArray  collisionShapes;
	std::vector <Collision*> activeCollisions;
	CollisionContainerArray  depthShapes;

	std::unique_ptr <FrameBuffer> depthBuffer;
	double                        speed;

	size_t numOpaqueDrawShapes;
	size_t numTransparentDrawShapes;
	size_t numOpaqueDisplayShapes;
	size_t numTransparentDisplayShapes;
	size_t numCollisionShapes;
	size_t numDepthShapes;

};

} // X3D
} // titania

#endif
