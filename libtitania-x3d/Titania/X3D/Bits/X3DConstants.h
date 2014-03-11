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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_X3D_BITS_X3DCONSTANTS_H__
#define __TITANIA_X3D_BITS_X3DCONSTANTS_H__

namespace titania {
namespace X3D {

namespace X3DConstants {
enum EventType
{
	INITIALIZED_EVENT,
	SHUTDOWN_EVENT

};

enum ErrorType
{
	BROWSER_UNAVAILABLE,
	CONNECTION_ERROR,
	DISPOSED,
	IMPORTED_NODE,
	INITIALIZED_ERROR,
	INSUFFICIENT_CAPABILITIES,
	INVALID_ACCESS_TYPE,
	INVALID_BROWSER,
	INVALID_DOCUMENT,
	INVALID_EXECUTION_CONTEXT,
	INVALID_FIELD,
	INVALID_NAME,
	INVALID_NODE,
	INVALID_OPERATION_TIMING,
	INVALID_SCENE,
	INVALID_URL,
	INVALID_X3D,
	NODE_IN_USE,
	NODE_NOT_AVAILABLE,
	NOT_SHARED,
	NOT_SUPPORTED,
	URL_UNAVAILABLE

};

enum LoadState
{
	NOT_STARTED_STATE,
	IN_PROGRESS_STATE,
	COMPLETE_STATE,
	FAILED_STATE

};

enum AccessType
{
	initializeOnly = 1, // 0b001
	inputOnly      = 2, // 0b010
	outputOnly     = 4, // 0b100
	inputOutput    = 7  // 0b111

};

enum FieldType
{
	SFBool,
	SFColor,
	SFColorRGBA,
	SFDouble,
	SFFloat,
	SFImage,
	SFInt32,
	SFMatrix3d,
	SFMatrix3f,
	SFMatrix4d,
	SFMatrix4f,
	SFNode,
	SFRotation,
	SFString,
	SFTime,
	SFVec2d,
	SFVec2f,
	SFVec3d,
	SFVec3f,
	SFVec4d,
	SFVec4f,

	MFBool,
	MFColor,
	MFColorRGBA,
	MFDouble,
	MFFloat,
	MFImage,
	MFInt32,
	MFMatrix3d,
	MFMatrix3f,
	MFMatrix4d,
	MFMatrix4f,
	MFNode,
	MFRotation,
	MFString,
	MFTime,
	MFVec2d,
	MFVec2f,
	MFVec3d,
	MFVec3f,
	MFVec4d,
	MFVec4f

};

enum NodeType
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
	X3DViewpointObject,
	X3DViewportNode

};

};

using X3DConstants::LoadState;
using X3DConstants::NOT_STARTED_STATE;
using X3DConstants::IN_PROGRESS_STATE;
using X3DConstants::COMPLETE_STATE;
using X3DConstants::FAILED_STATE;

using X3DConstants::ErrorType;
using X3DConstants::BROWSER_UNAVAILABLE;
using X3DConstants::CONNECTION_ERROR;
using X3DConstants::DISPOSED;
using X3DConstants::IMPORTED_NODE;
using X3DConstants::INITIALIZED_ERROR;
using X3DConstants::INSUFFICIENT_CAPABILITIES;
using X3DConstants::INVALID_ACCESS_TYPE;
using X3DConstants::INVALID_BROWSER;
using X3DConstants::INVALID_DOCUMENT;
using X3DConstants::INVALID_EXECUTION_CONTEXT;
using X3DConstants::INVALID_FIELD;
using X3DConstants::INVALID_NAME;
using X3DConstants::INVALID_NODE;
using X3DConstants::INVALID_OPERATION_TIMING;
using X3DConstants::INVALID_SCENE;
using X3DConstants::INVALID_URL;
using X3DConstants::INVALID_X3D;
using X3DConstants::NODE_IN_USE;
using X3DConstants::NODE_NOT_AVAILABLE;
using X3DConstants::NOT_SHARED;
using X3DConstants::NOT_SUPPORTED;
using X3DConstants::URL_UNAVAILABLE;

using X3DConstants::AccessType;
using X3DConstants::initializeOnly;
using X3DConstants::inputOnly;
using X3DConstants::outputOnly;
using X3DConstants::inputOutput;

} // X3D
} // titania

#endif
