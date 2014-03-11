/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "jsX3DConstants.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Scripting/Script.h"
#include "../../InputOutput/Generator.h"
#include "Fields/jsMFNode.h"
#include "Fields/jsMFString.h"
#include "Fields/jsSFNode.h"
#include "jsComponentInfoArray.h"
#include "jsContext.h"
#include "jsProfileInfoArray.h"
#include "jsString.h"
#include "jsX3DExecutionContext.h"
#include "jsX3DScene.h"

namespace titania {
namespace X3D {

JSClass jsX3DConstants::static_class = {
	"X3DConstants", 0,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DConstants::properties [ ] = {
	// Event types

	{ "INITIALIZED_EVENT", JS_INITIALIZED_EVENT, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, INITIALIZED_EVENT, NULL },
	{ "SHUTDOWN_EVENT",    JS_SHUTDOWN_EVENT,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SHUTDOWN_EVENT,    NULL },

	// Error types

	{ "CONNECTION_ERROR",  JS_CONNECTION_ERROR,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CONNECTION_ERROR,  NULL },
	{ "INITIALIZED_ERROR", JS_INITIALIZED_ERROR, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, INITIALIZED_ERROR, NULL },

	// Load state

	{ "NOT_STARTED_STATE", JS_NOT_STARTED_STATE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NOT_STARTED_STATE, NULL },
	{ "IN_PROGRESS_STATE", JS_IN_PROGRESS_STATE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IN_PROGRESS_STATE, NULL },
	{ "COMPLETE_STATE",    JS_COMPLETE_STATE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, COMPLETE_STATE,    NULL },
	{ "FAILED_STATE",      JS_FAILED_STATE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FAILED_STATE,      NULL },

	// Access types

	{ "initializeOnly", JS_INITIALIZE_ONLY, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, initializeOnly, NULL },
	{ "inputOnly",      JS_INPUT_ONLY,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, inputOnly,      NULL },
	{ "outputOnly",     JS_OUTPUT_ONLY,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, outputOnly,     NULL },
	{ "inputOutput",    JS_INPUT_OUTPUT,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, inputOutput,    NULL },

	// Field types

	{ "SFBool",      JS_SFBOOL,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFBool,      NULL },
	{ "SFColor",     JS_SFCOLOR,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFColor,     NULL },
	{ "SFColorRGBA", JS_SFCOLORRGBA, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFColorRGBA, NULL },
	{ "SFDouble",    JS_SFDOUBLE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFDouble,    NULL },
	{ "SFFloat",     JS_SFFLOAT,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFFloat,     NULL },
	{ "SFInt32",     JS_SFINT32,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFInt32,     NULL },
	{ "SFImage",     JS_SFIMAGE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFImage,     NULL },
	{ "SFMatrix3d",  JS_SFMATRIX3D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix3d,  NULL },
	{ "SFMatrix3f",  JS_SFMATRIX3F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix3f,  NULL },
	{ "SFMatrix4d",  JS_SFMATRIX4D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix4d,  NULL },
	{ "SFMatrix4f",  JS_SFMATRIX4F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix4f,  NULL },
	{ "SFNode",      JS_SFNODE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFNode,      NULL },
	{ "SFRotation",  JS_SFROTATION,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFRotation,  NULL },
	{ "SFString",    JS_SFSTRING,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFString,    NULL },
	{ "SFTime",      JS_SFTIME,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFTime,      NULL },
	{ "SFVec2d",     JS_SFVEC2D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec2d,     NULL },
	{ "SFVec2f",     JS_SFVEC2F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec2f,     NULL },
	{ "SFVec3d",     JS_SFVEC3D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec3d,     NULL },
	{ "SFVec3f",     JS_SFVEC3F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec3f,     NULL },
	{ "SFVec4d",     JS_SFVEC4D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec4d,     NULL },
	{ "SFVec4f",     JS_SFVEC4F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec4f,     NULL },

	{ "MFBool",      JS_MFBOOL,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFBool,      NULL },
	{ "MFColor",     JS_MFCOLOR,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFColor,     NULL },
	{ "MFColorRGBA", JS_MFCOLORRGBA, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFColorRGBA, NULL },
	{ "MFDouble",    JS_MFDOUBLE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFDouble,    NULL },
	{ "MFFloat",     JS_MFFLOAT,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFFloat,     NULL },
	{ "MFImage",     JS_MFIMAGE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFImage,     NULL },
	{ "MFInt32",     JS_MFINT32,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFInt32,     NULL },
	{ "MFMatrix3d",  JS_MFMATRIX3D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix3d,  NULL },
	{ "MFMatrix3f",  JS_MFMATRIX3F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix3f,  NULL },
	{ "MFMatrix4d",  JS_MFMATRIX4D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix4d,  NULL },
	{ "MFMatrix4f",  JS_MFMATRIX4F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix4f,  NULL },
	{ "MFNode",      JS_MFNODE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFNode,      NULL },
	{ "MFRotation",  JS_MFROTATION,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFRotation,  NULL },
	{ "MFString",    JS_MFSTRING,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFString,    NULL },
	{ "MFTime",      JS_MFTIME,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFTime,      NULL },
	{ "MFVec2d",     JS_MFVEC2D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec2d,     NULL },
	{ "MFVec2f",     JS_MFVEC2F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec2f,     NULL },
	{ "MFVec3d",     JS_MFVEC3D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec3d,     NULL },
	{ "MFVec3f",     JS_MFVEC3F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec3f,     NULL },
	{ "MFVec4d",     JS_MFVEC4D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec4d,     NULL },
	{ "MFVec4f",     JS_MFVEC4F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec4f,     NULL },

	// Node types

	{ "X3DAppearanceChildNode",       JS_X3D_APPEARANCECHILD_NODE,       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DAppearanceChildNode,       NULL },
	{ "X3DAppearanceNode",            JS_X3D_APPEARANCE_NODE,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DAppearanceNode,            NULL },
	{ "X3DBackgroundNode",            JS_X3D_BACKGROUND_NODE,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBackgroundNode,            NULL },
	{ "X3DBindableNode",              JS_X3D_BINDABLE_NODE,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBindableNode,              NULL },
	{ "X3DBoundedObject",             JS_X3D_BOUNDED_OBJECT,             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBoundedObject,             NULL },
	{ "X3DChaserNode",                JS_X3D_CHASER_NODE,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DChaserNode,                NULL },
	{ "X3DChildNode",                 JS_X3D_CHILD_NODE,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DChildNode,                 NULL },
	{ "X3DColorNode",                 JS_X3D_COLOR_NODE,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DColorNode,                 NULL },
	{ "X3DComposedGeometryNode",      JS_X3D_COMPOSEDGEOMETRY_NODE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DComposedGeometryNode,      NULL },
	{ "X3DCoordinateNode",            JS_X3D_COORDINATE_NODE,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DCoordinateNode,            NULL },
	{ "X3DDamperNode",                JS_X3D_DAMPER_NODE,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DDamperNode,                NULL },
	{ "X3DDragSensorNode",            JS_X3D_DRAGSENSOR_NODE,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DDragSensorNode,            NULL },
	{ "X3DEnvironmentalSensorNode",   JS_X3D_ENVIRONMENTALSENSOR_NODE,   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DEnvironmentalSensorNode,   NULL },
	{ "X3DEnvironmentTextureNode",    JS_X3D_ENVIRONMENTTEXTURE_NODE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DEnvironmentTextureNode,    NULL },
	{ "X3DFogObject",                 JS_X3D_FOG_OBJECT,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFogObject,                 NULL },
	{ "X3DFollowerNode",              JS_X3D_FOLLOWER_NODE,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFollowerNode,              NULL },
	{ "X3DFontStyleNode",             JS_X3D_FONTSTYLE_NODE,             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFontStyleNode,             NULL },
	{ "X3DGeometricPropertyNode",     JS_X3D_GEOMETRICPROPERTY_NODE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGeometricPropertyNode,     NULL },
	{ "X3DGeometryNode",              JS_X3D_GEOMETRY_NODE,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGeometryNode,              NULL },
	{ "X3DGroupingNode",              JS_X3D_GROUPING_NODE,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGroupingNode,              NULL },
	{ "X3DInfoNode",                  JS_X3D_INFO_NODE,                  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DInfoNode,                  NULL },
	{ "X3DInterpolatorNode",          JS_X3D_INTERPOLATOR_NODE,          JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DInterpolatorNode,          NULL },
	{ "X3DKeyDeviceSensorNode",       JS_X3D_KEYDEVICESENSOR_NODE,       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DKeyDeviceSensorNode,       NULL },
	{ "X3DLayerNode",                 JS_X3D_LAYER_NODE,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLayerNode,                 NULL },
	{ "X3DLayoutNode",                JS_X3D_LAYOUT_NODE,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLayoutNode,                NULL },
	{ "X3DLightNode",                 JS_X3D_LIGHT_NODE,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLightNode,                 NULL },
	{ "X3DMaterialNode",              JS_X3D_MATERIAL_NODE,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DMaterialNode,              NULL },
	{ "X3DMetadataObject",            JS_X3D_METADATA_OBJECT,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DMetadataObject,            NULL },
	{ "X3DNBodyCollidableNode",       JS_X3D_NBODYCOLLIDABLE_NODE,       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNBodyCollidableNode,       NULL },
	{ "X3DNBodyCollisionSpaceNode",   JS_X3D_NBODYCOLLISIONSPACE_NODE,   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNBodyCollisionSpaceNode,   NULL },
	{ "X3DNetworkSensorNode",         JS_X3D_NETWORKSENSOR_NODE,         JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNetworkSensorNode,         NULL },
	{ "X3DNode",                      JS_X3D_NODE,                       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNode,                      NULL },
	{ "X3DNormalNode",                JS_X3D_NORMAL_NODE,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNormalNode,                NULL },
	{ "X3DNurbsControlCurveNode",     JS_X3D_NURBSCONTROLCURVE_NODE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNurbsControlCurveNode,     NULL },
	{ "X3DNurbsSurfaceGeometryNode",  JS_X3D_NURBSSURFACEGEOMETRY_NODE,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNurbsSurfaceGeometryNode,  NULL },
	{ "X3DParametricGeometryNode",    JS_X3D_PARAMETRICGEOMETRY_NODE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParametricGeometryNode,    NULL },
	{ "X3DParticleEmitterNode",       JS_X3D_PARTICLEEMITTER_NODE,       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParticleEmitterNode,       NULL },
	{ "X3DParticlePhysicsModelNode",  JS_X3D_PARTICLEPHYSICSMODEL_NODE,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParticlePhysicsModelNode,  NULL },
	{ "X3DPickableObject",            JS_X3D_PICKABLE_OBJECT,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPickableObject,            NULL },
	{ "X3DPickingNode",               JS_X3D_PICKING_NODE,               JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPickingNode,               NULL },
	{ "X3DPickSensorNode",            JS_X3D_PICKSENSOR_NODE,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPickSensorNode,            NULL },
	{ "X3DPointingDeviceSensorNode",  JS_X3D_POINTINGDEVICESENSOR_NODE,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPointingDeviceSensorNode,  NULL },
	{ "X3DProductStructureChildNode", JS_X3D_PRODUCTSTRUCTURECHILD_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DProductStructureChildNode, NULL },
	{ "X3DProgrammableShaderObject",  JS_X3D_PROGRAMMABLESHADER_OBJECT,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DProgrammableShaderObject,  NULL },
	{ "X3DPrototypeInstance",         JS_X3D_PROTOTYPE_INSTANCE,         JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPrototypeInstance,         NULL },
	{ "X3DRigidJointNode",            JS_X3D_RIGIDJOINT_NODE,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DRigidJointNode,            NULL },
	{ "X3DScriptNode",                JS_X3D_SCRIPT_NODE,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DScriptNode,                NULL },
	{ "X3DSensorNode",                JS_X3D_SENSOR_NODE,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSensorNode,                NULL },
	{ "X3DSequencerNode",             JS_X3D_SEQUENCER_NODE,             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSequencerNode,             NULL },
	{ "X3DShaderNode",                JS_X3D_SHADER_NODE,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DShaderNode,                NULL },
	{ "X3DShapeNode",                 JS_X3D_SHAPE_NODE,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DShapeNode,                 NULL },
	{ "X3DSoundNode",                 JS_X3D_SOUND_NODE,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSoundNode,                 NULL },
	{ "X3DSoundSourceNode",           JS_X3D_SOUNDSOURCE_NODE,           JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSoundSourceNode,           NULL },
	{ "X3DTexture2DNode",             JS_X3D_TEXTURE2D_NODE,             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTexture2DNode,             NULL },
	{ "X3DTexture3DNode",             JS_X3D_TEXTURE3D_NODE,             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTexture3DNode,             NULL },
	{ "X3DTextureCoordinateNode",     JS_X3D_TEXTURECOORDINATE_NODE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureCoordinateNode,     NULL },
	{ "X3DTextureNode",               JS_X3D_TEXTURE_NODE,               JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureNode,               NULL },
	{ "X3DTextureTransformNode",      JS_X3D_TEXTURETRANSFORM_NODE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureTransformNode,      NULL },
	{ "X3DTimeDependentNode",         JS_X3D_TIMEDEPENDENT_NODE,         JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTimeDependentNode,         NULL },
	{ "X3DTouchSensorNode",           JS_X3D_TOUCHSENSOR_NODE,           JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTouchSensorNode,           NULL },
	{ "X3DTriggerNode",               JS_X3D_TRIGGER_NODE,               JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTriggerNode,               NULL },
	{ "X3DUrlObject",                 JS_X3D_URL_OBJECT,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DUrlObject,                 NULL },
	{ "X3DVertexAttributeNode",       JS_X3D_VERTEXATTRIBUTE_NODE,       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DVertexAttributeNode,       NULL },
	{ "X3DViewpointNode",             JS_X3D_VIEWPOINT_NODE,             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewpointNode,             NULL },
	{ "X3DViewpointObject",           JS_X3D_VIEWPOINT_OBJECT,           JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewpointObject,           NULL },
	{ "X3DViewportNode",              JS_X3D_VIEWPORT_NODE,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewportNode,              NULL },

	{ 0 }

};

JSFunctionSpec jsX3DConstants::functions [ ] = {
	{ 0, 0, 0, 0 }

};

void
jsX3DConstants::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, NULL,
	              0, properties, functions, NULL, NULL);

	JS_DefineObject (context, global, "X3DConstants", &static_class, NULL,
	                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT);
}

// Event types

JSBool
jsX3DConstants::INITIALIZED_EVENT (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::INITIALIZED_EVENT, vp);
}

JSBool
jsX3DConstants::SHUTDOWN_EVENT (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SHUTDOWN_EVENT, vp);
}

// Error types

JSBool
jsX3DConstants::CONNECTION_ERROR (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CONNECTION_ERROR, vp);
}

JSBool
jsX3DConstants::INITIALIZED_ERROR (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::INITIALIZED_ERROR, vp);
}

// Load state

JSBool
jsX3DConstants::NOT_STARTED_STATE (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NOT_STARTED_STATE, vp);
}

JSBool
jsX3DConstants::IN_PROGRESS_STATE (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IN_PROGRESS_STATE, vp);
}

JSBool
jsX3DConstants::COMPLETE_STATE (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::COMPLETE_STATE, vp);
}

JSBool
jsX3DConstants::FAILED_STATE (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::FAILED_STATE, vp);
}

// Access types

JSBool
jsX3DConstants::initializeOnly (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::initializeOnly, vp);
}

JSBool
jsX3DConstants::inputOnly (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::inputOnly, vp);
}

JSBool
jsX3DConstants::outputOnly (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::outputOnly, vp);
}

JSBool
jsX3DConstants::inputOutput (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::inputOutput, vp);
}

// X3D properties

JSBool
jsX3DConstants::SFBool (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFBool, vp);
}

JSBool
jsX3DConstants::SFColor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFColor, vp);
}

JSBool
jsX3DConstants::SFColorRGBA (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFColorRGBA, vp);
}

JSBool
jsX3DConstants::SFDouble (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFDouble, vp);
}

JSBool
jsX3DConstants::SFFloat (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFFloat, vp);
}

JSBool
jsX3DConstants::SFInt32 (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFInt32, vp);
}

JSBool
jsX3DConstants::SFImage (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFImage, vp);
}

JSBool
jsX3DConstants::SFMatrix3d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFMatrix3d, vp);
}

JSBool
jsX3DConstants::SFMatrix3f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFMatrix3f, vp);
}

JSBool
jsX3DConstants::SFMatrix4d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFMatrix4d, vp);
}

JSBool
jsX3DConstants::SFMatrix4f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFMatrix4f, vp);
}

JSBool
jsX3DConstants::SFNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFNode, vp);
}

JSBool
jsX3DConstants::SFRotation (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFRotation, vp);
}

JSBool
jsX3DConstants::SFString (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFString, vp);
}

JSBool
jsX3DConstants::SFTime (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFTime, vp);
}

JSBool
jsX3DConstants::SFVec2d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec2d, vp);
}

JSBool
jsX3DConstants::SFVec2f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec2f, vp);
}

JSBool
jsX3DConstants::SFVec3d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec3d, vp);
}

JSBool
jsX3DConstants::SFVec3f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec3f, vp);
}

JSBool
jsX3DConstants::SFVec4d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec4d, vp);
}

JSBool
jsX3DConstants::SFVec4f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec4f, vp);
}

JSBool
jsX3DConstants::MFBool (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFBool, vp);
}

JSBool
jsX3DConstants::MFColor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFColor, vp);
}

JSBool
jsX3DConstants::MFColorRGBA (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFColorRGBA, vp);
}

JSBool
jsX3DConstants::MFDouble (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFDouble, vp);
}

JSBool
jsX3DConstants::MFFloat (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFFloat, vp);
}

JSBool
jsX3DConstants::MFImage (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFImage, vp);
}

JSBool
jsX3DConstants::MFInt32 (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFInt32, vp);
}

JSBool
jsX3DConstants::MFMatrix3d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFMatrix3d, vp);
}

JSBool
jsX3DConstants::MFMatrix3f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFMatrix3f, vp);
}

JSBool
jsX3DConstants::MFMatrix4d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFMatrix4d, vp);
}

JSBool
jsX3DConstants::MFMatrix4f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFMatrix4f, vp);
}

JSBool
jsX3DConstants::MFNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFNode, vp);
}

JSBool
jsX3DConstants::MFRotation (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFRotation, vp);
}

JSBool
jsX3DConstants::MFString (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFString, vp);
}

JSBool
jsX3DConstants::MFTime (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFTime, vp);
}

JSBool
jsX3DConstants::MFVec2d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec2d, vp);
}

JSBool
jsX3DConstants::MFVec2f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec2f, vp);
}

JSBool
jsX3DConstants::MFVec3d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec3d, vp);
}

JSBool
jsX3DConstants::MFVec3f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec3f, vp);
}

JSBool
jsX3DConstants::MFVec4d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec4d, vp);
}

JSBool
jsX3DConstants::MFVec4f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec4f, vp);
}

JSBool
jsX3DConstants::X3DAppearanceChildNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DAppearanceChildNode, vp);
}

// Node types

JSBool
jsX3DConstants::X3DAppearanceNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DAppearanceNode, vp);
}

JSBool
jsX3DConstants::X3DBackgroundNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DBackgroundNode, vp);
}

JSBool
jsX3DConstants::X3DBindableNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DBindableNode, vp);
}

JSBool
jsX3DConstants::X3DBoundedObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DBoundedObject, vp);
}

JSBool
jsX3DConstants::X3DChaserNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DChaserNode, vp);
}

JSBool
jsX3DConstants::X3DChildNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DChildNode, vp);
}

JSBool
jsX3DConstants::X3DColorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DColorNode, vp);
}

JSBool
jsX3DConstants::X3DComposedGeometryNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DComposedGeometryNode, vp);
}

JSBool
jsX3DConstants::X3DCoordinateNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DCoordinateNode, vp);
}

JSBool
jsX3DConstants::X3DDamperNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DDamperNode, vp);
}

JSBool
jsX3DConstants::X3DDragSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DDragSensorNode, vp);
}

JSBool
jsX3DConstants::X3DEnvironmentalSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DEnvironmentalSensorNode, vp);
}

JSBool
jsX3DConstants::X3DEnvironmentTextureNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DEnvironmentTextureNode, vp);
}

JSBool
jsX3DConstants::X3DFogObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DFogObject, vp);
}

JSBool
jsX3DConstants::X3DFollowerNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DFollowerNode, vp);
}

JSBool
jsX3DConstants::X3DFontStyleNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DFontStyleNode, vp);
}

JSBool
jsX3DConstants::X3DGeometricPropertyNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DGeometricPropertyNode, vp);
}

JSBool
jsX3DConstants::X3DGeometryNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DGeometryNode, vp);
}

JSBool
jsX3DConstants::X3DGroupingNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DGroupingNode, vp);
}

JSBool
jsX3DConstants::X3DInfoNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DInfoNode, vp);
}

JSBool
jsX3DConstants::X3DInterpolatorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DInterpolatorNode, vp);
}

JSBool
jsX3DConstants::X3DKeyDeviceSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DKeyDeviceSensorNode, vp);
}

JSBool
jsX3DConstants::X3DLayerNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DLayerNode, vp);
}

JSBool
jsX3DConstants::X3DLayoutNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DLayoutNode, vp);
}

JSBool
jsX3DConstants::X3DLightNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DLightNode, vp);
}

JSBool
jsX3DConstants::X3DMaterialNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DMaterialNode, vp);
}

JSBool
jsX3DConstants::X3DMetadataObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DMetadataObject, vp);
}

JSBool
jsX3DConstants::X3DNBodyCollidableNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNBodyCollidableNode, vp);
}

JSBool
jsX3DConstants::X3DNBodyCollisionSpaceNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNBodyCollisionSpaceNode, vp);
}

JSBool
jsX3DConstants::X3DNetworkSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNetworkSensorNode, vp);
}

JSBool
jsX3DConstants::X3DNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNode, vp);
}

JSBool
jsX3DConstants::X3DNormalNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNormalNode, vp);
}

JSBool
jsX3DConstants::X3DNurbsControlCurveNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNurbsControlCurveNode, vp);
}

JSBool
jsX3DConstants::X3DNurbsSurfaceGeometryNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNurbsSurfaceGeometryNode, vp);
}

JSBool
jsX3DConstants::X3DParametricGeometryNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DParametricGeometryNode, vp);
}

JSBool
jsX3DConstants::X3DParticleEmitterNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DParticleEmitterNode, vp);
}

JSBool
jsX3DConstants::X3DParticlePhysicsModelNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DParticlePhysicsModelNode, vp);
}

JSBool
jsX3DConstants::X3DPickableObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPickableObject, vp);
}

JSBool
jsX3DConstants::X3DPickingNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPickingNode, vp);
}

JSBool
jsX3DConstants::X3DPickSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPickSensorNode, vp);
}

JSBool
jsX3DConstants::X3DPointingDeviceSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPointingDeviceSensorNode, vp);
}

JSBool
jsX3DConstants::X3DProductStructureChildNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DProductStructureChildNode, vp);
}

JSBool
jsX3DConstants::X3DProgrammableShaderObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DProgrammableShaderObject, vp);
}

JSBool
jsX3DConstants::X3DPrototypeInstance (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPrototypeInstance, vp);
}

JSBool
jsX3DConstants::X3DRigidJointNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DRigidJointNode, vp);
}

JSBool
jsX3DConstants::X3DScriptNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DScriptNode, vp);
}

JSBool
jsX3DConstants::X3DSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DSensorNode, vp);
}

JSBool
jsX3DConstants::X3DSequencerNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DSequencerNode, vp);
}

JSBool
jsX3DConstants::X3DShaderNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DShaderNode, vp);
}

JSBool
jsX3DConstants::X3DShapeNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DShapeNode, vp);
}

JSBool
jsX3DConstants::X3DSoundNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DSoundNode, vp);
}

JSBool
jsX3DConstants::X3DSoundSourceNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DSoundSourceNode, vp);
}

JSBool
jsX3DConstants::X3DTexture2DNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTexture2DNode, vp);
}

JSBool
jsX3DConstants::X3DTexture3DNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTexture3DNode, vp);
}

JSBool
jsX3DConstants::X3DTextureCoordinateNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTextureCoordinateNode, vp);
}

JSBool
jsX3DConstants::X3DTextureNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTextureNode, vp);
}

JSBool
jsX3DConstants::X3DTextureTransformNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTextureTransformNode, vp);
}

JSBool
jsX3DConstants::X3DTimeDependentNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTimeDependentNode, vp);
}

JSBool
jsX3DConstants::X3DTouchSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTouchSensorNode, vp);
}

JSBool
jsX3DConstants::X3DTriggerNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTriggerNode, vp);
}

JSBool
jsX3DConstants::X3DUrlObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DUrlObject, vp);
}

JSBool
jsX3DConstants::X3DVertexAttributeNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DVertexAttributeNode, vp);
}

JSBool
jsX3DConstants::X3DViewpointNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DViewpointNode, vp);
}

JSBool
jsX3DConstants::X3DViewpointObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DViewpointObject, vp);
}

JSBool
jsX3DConstants::X3DViewportNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DViewportNode, vp);
}

} // X3D
} // titania
