/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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
 ******************************************************************************/

#include "../Basic/FieldType.h"

#ifndef __TITANIA_X3D_BITS_X3DCONSTANTS_H__
#define __TITANIA_X3D_BITS_X3DCONSTANTS_H__

namespace titania {
namespace X3D {

class X3DConstants
{
public:

	enum X3DAccessType
	{
		initializeOnly = 1, // 0b001
		inputOnly      = 2, // 0b010
		outputOnly     = 4, // 0b100
		inputOutput    = 7  // 0b111

	};

	static const FieldType* const SFBool;
	static const FieldType* const SFColor;
	static const FieldType* const SFColorRGBA;
	static const FieldType* const SFDouble;
	static const FieldType* const SFFloat;
	static const FieldType* const SFImage;
	static const FieldType* const SFInt32;
	static const FieldType* const SFMatrix3d;
	static const FieldType* const SFMatrix3f;
	static const FieldType* const SFMatrix4d;
	static const FieldType* const SFMatrix4f;
	static const FieldType* const SFNode;
	static const FieldType* const SFRotation;
	static const FieldType* const SFString;
	static const FieldType* const SFTime;
	static const FieldType* const SFVec2d;
	static const FieldType* const SFVec2f;
	static const FieldType* const SFVec3d;
	static const FieldType* const SFVec3f;
	static const FieldType* const SFVec4d;
	static const FieldType* const SFVec4f;

	static const FieldType* const MFBool;
	static const FieldType* const MFColor;
	static const FieldType* const MFColorRGBA;
	static const FieldType* const MFDouble;
	static const FieldType* const MFFloat;
	static const FieldType* const MFImage;
	static const FieldType* const MFInt32;
	static const FieldType* const MFMatrix3d;
	static const FieldType* const MFMatrix3f;
	static const FieldType* const MFMatrix4d;
	static const FieldType* const MFMatrix4f;
	static const FieldType* const MFNode;
	static const FieldType* const MFRotation;
	static const FieldType* const MFString;
	static const FieldType* const MFTime;
	static const FieldType* const MFVec2d;
	static const FieldType* const MFVec2f;
	static const FieldType* const MFVec3d;
	static const FieldType* const MFVec3f;
	static const FieldType* const MFVec4d;
	static const FieldType* const MFVec4f;

};

enum NodeTypes
{
	X3DAppearanceChildNodeType,
	X3DAppearanceNodeType,
	X3DBackgroundNodeType,
	X3DBindableNodeType,
	X3DBoundedObjectType,
	X3DChaserNodeType,
	X3DChildNodeType,
	X3DColorNodeType,
	X3DComposedGeometryNodeType,
	X3DCoordinateNodeType,
	X3DDamperNodeType,
	X3DDragSensorNodeType,
	X3DEnvironmentalSensorNodeType,
	X3DEnvironmentTextureNodeType,
	X3DFogObjectType,
	X3DFollowerNodeType,
	X3DFontStyleNodeType,
	X3DGeometricPropertyNodeType,
	X3DGeometryNodeType,
	X3DGroupingNodeType,
	X3DInfoNodeType,
	X3DInterpolatorNodeType,
	X3DKeyDeviceSensorNodeType,
	X3DLayerNodeType,
	X3DLayoutNodeType,
	X3DLightNodeType,
	X3DMaterialNodeType,
	X3DMetadataObjectType,
	X3DNBodyCollidableNodeType,
	X3DNBodyCollisionSpaceNodeType,
	X3DNetworkSensorNodeType,
	X3DNodeType,
	X3DNormalNodeType,
	X3DNurbsControlCurveNodeType,
	X3DNurbsSurfaceGeometryNodeType,
	X3DParametricGeometryNodeType,
	X3DParticleEmitterNodeType,
	X3DParticlePhysicsModelNodeType,
	X3DPickableObjectType,
	X3DPickingNodeType,
	X3DPickSensorNodeType,
	X3DPointingDeviceSensorNodeType,
	X3DProductStructureChildNodeType,
	X3DProgrammableShaderObjectType,
	X3DPropertyNodeType,
	X3DPrototypeInstanceType,
	X3DRigidJointNodeType,
	X3DScriptNodeType,
	X3DSensorNodeType,
	X3DSequencerNodeType,
	X3DShaderNodeType,
	X3DShapeNodeType,
	X3DSoundNodeType,
	X3DSoundSourceNodeType,
	X3DTexture2DNodeType,
	X3DTexture3DNodeType,
	X3DTextureCoordinateNodeType,
	X3DTextureNodeType,
	X3DTextureTransformNodeType,
	X3DTimeDependentNodeType,
	X3DTouchSensorNodeType,
	X3DTriggerNodeType,
	X3DUrlObjectType,
	X3DVertexAttributeNodeType,
	X3DViewpointNodeType,
	X3DViewportNodeType

};

} // X3D
} // titania

#endif
