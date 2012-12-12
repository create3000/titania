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

	enum NodeTypes
	{
		X3DAppearanceChildNode,
		X3DAppearanceNode,
		X3DBackgroundNode,
		X3DBindableNode,
		X3DBoundedObject,
		X3DChaserNode,
		X3DChildNode,
		X3DColorNode,
		X3DComposedGeometryNode,
		X3DCoordinateNode,
		X3DDamperNode,
		X3DDragSensorNode,
		X3DEnvironmentalSensorNode,
		X3DEnvironmentTextureNode,
		X3DFogObject,
		X3DFollowerNode,
		X3DFontStyleNode,
		X3DGeometricPropertyNode,
		X3DGeometryNode,
		X3DGroupingNode,
		X3DInfoNode,
		X3DInterpolatorNode,
		X3DKeyDeviceSensorNode,
		X3DLayerNode,
		X3DLayoutNode,
		X3DLightNode,
		X3DMaterialNode,
		X3DMetadataObject,
		X3DNBodyCollidableNode,
		X3DNBodyCollisionSpaceNode,
		X3DNetworkSensorNode,
		X3DNode,
		X3DNormalNode,
		X3DNurbsControlCurveNode,
		X3DNurbsSurfaceGeometryNode,
		X3DParametricGeometryNode,
		X3DParticleEmitterNode,
		X3DParticlePhysicsModelNode,
		X3DPickableObject,
		X3DPickingNode,
		X3DPickSensorNode,
		X3DPointingDeviceSensorNode,
		X3DProductStructureChildNode,
		X3DProgrammableShaderObject,
		X3DPropertyNode,
		X3DPrototypeInstance,
		X3DRigidJointNode,
		X3DScriptNode,
		X3DSensorNode,
		X3DSequencerNode,
		X3DShaderNode,
		X3DShapeNode,
		X3DSoundNode,
		X3DSoundSourceNode,
		X3DTexture2DNode,
		X3DTexture3DNode,
		X3DTextureCoordinateNode,
		X3DTextureNode,
		X3DTextureTransformNode,
		X3DTimeDependentNode,
		X3DTouchSensorNode,
		X3DTriggerNode,
		X3DUrlObject,
		X3DVertexAttributeNode,
		X3DViewpointNode,
		X3DViewportNode

	};

};

} // X3D
} // titania

#endif
