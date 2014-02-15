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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DCONSTANTS_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DCONSTANTS_H__

#include <iostream>
#include <jsapi.h>

namespace titania {
namespace X3D {

class jsX3DConstants
{
public:

	static
	void
	init (JSContext* const, JSObject* const);

	static
	JSClass*
	getClass () { return &static_class; }


private:

	enum Property
	{
		// Event types

		_INITIALIZED_EVENT,
		_SHUTDOWN_EVENT,

		// Error types

		_CONNECTION_ERROR,
		_INITIALIZED_ERROR,

		// Load state

		_NOT_STARTED_STATE,
		_IN_PROGRESS_STATE,
		_COMPLETE_STATE,
		_FAILED_STATE,

		// Access types

		INITIALIZE_ONLY,
		INPUT_ONLY,
		OUTPUT_ONLY,
		INPUT_OUTPUT,

		// Field types

		SFBOOL,
		SFCOLOR,
		SFCOLORRGBA,
		SFDOUBLE,
		SFFLOAT,
		SFINT32,
		SFIMAGE,
		SFMATRIX3D,
		SFMATRIX3F,
		SFMATRIX4D,
		SFMATRIX4F,
		SFNODE,
		SFROTATION,
		SFSTRING,
		SFTIME,
		SFVEC2D,
		SFVEC2F,
		SFVEC3D,
		SFVEC3F,
		SFVEC4D,
		SFVEC4F,

		MFBOOL,
		MFCOLOR,
		MFCOLORRGBA,
		MFDOUBLE,
		MFFLOAT,
		MFIMAGE,
		MFINT32,
		MFMATRIX3D,
		MFMATRIX3F,
		MFMATRIX4D,
		MFMATRIX4F,
		MFNODE,
		MFROTATION,
		MFSTRING,
		MFTIME,
		MFVEC2D,
		MFVEC2F,
		MFVEC3D,
		MFVEC3F,
		MFVEC4D,
		MFVEC4F,

		// Node types

		X3D_APPEARANCECHILD_NODE,
		X3D_APPEARANCE_NODE,
		X3D_BACKGROUND_NODE,
		X3D_BINDABLE_NODE,
		X3D_BOUNDED_OBJECT,
		X3D_CHASER_NODE,
		X3D_CHILD_NODE,
		X3D_COLOR_NODE,
		X3D_COMPOSEDGEOMETRY_NODE,
		X3D_COORDINATE_NODE,
		X3D_DAMPER_NODE,
		X3D_DRAGSENSOR_NODE,
		X3D_ENVIRONMENTALSENSOR_NODE,
		X3D_ENVIRONMENTTEXTURE_NODE,
		X3D_FOG_OBJECT,
		X3D_FOLLOWER_NODE,
		X3D_FONTSTYLE_NODE,
		X3D_GEOMETRICPROPERTY_NODE,
		X3D_GEOMETRY_NODE,
		X3D_GROUPING_NODE,
		X3D_INFO_NODE,
		X3D_INTERPOLATOR_NODE,
		X3D_KEYDEVICESENSOR_NODE,
		X3D_LAYER_NODE,
		X3D_LAYOUT_NODE,
		X3D_LIGHT_NODE,
		X3D_MATERIAL_NODE,
		X3D_METADATA_OBJECT,
		X3D_NBODYCOLLIDABLE_NODE,
		X3D_NBODYCOLLISIONSPACE_NODE,
		X3D_NETWORKSENSOR_NODE,
		X3D_NODE,
		X3D_NORMAL_NODE,
		X3D_NURBSCONTROLCURVE_NODE,
		X3D_NURBSSURFACEGEOMETRY_NODE,
		X3D_PARAMETRICGEOMETRY_NODE,
		X3D_PARTICLEEMITTER_NODE,
		X3D_PARTICLEPHYSICSMODEL_NODE,
		X3D_PICKABLE_OBJECT,
		X3D_PICKING_NODE,
		X3D_PICKSENSOR_NODE,
		X3D_POINTINGDEVICESENSOR_NODE,
		X3D_PRODUCTSTRUCTURECHILD_NODE,
		X3D_PROGRAMMABLESHADER_OBJECT,
		X3D_PROPERTY_NODE,
		X3D_PROTOTYPE_INSTANCE,
		X3D_RIGIDJOINT_NODE,
		X3D_SCRIPT_NODE,
		X3D_SENSOR_NODE,
		X3D_SEQUENCER_NODE,
		X3D_SHADER_NODE,
		X3D_SHAPE_NODE,
		X3D_SOUND_NODE,
		X3D_SOUNDSOURCE_NODE,
		X3D_TEXTURE2D_NODE,
		X3D_TEXTURE3D_NODE,
		X3D_TEXTURECOORDINATE_NODE,
		X3D_TEXTURE_NODE,
		X3D_TEXTURETRANSFORM_NODE,
		X3D_TIMEDEPENDENT_NODE,
		X3D_TOUCHSENSOR_NODE,
		X3D_TRANSFORM_NODE,
		X3D_TRIGGER_NODE,
		X3D_URL_OBJECT,
		X3D_VERTEXATTRIBUTE_NODE,
		X3D_VIEWPOINT_NODE,
		X3D_VIEWPOINT_OBJECT,
		X3D_VIEWPORT_NODE
	};

	// Event types

	static JSBool INITIALIZED_EVENT (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SHUTDOWN_EVENT    (JSContext *, JSObject *, jsid, jsval*);

	// Error types

	static JSBool CONNECTION_ERROR  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool INITIALIZED_ERROR (JSContext *, JSObject *, jsid, jsval*);

	// Load state

	static JSBool NOT_STARTED_STATE (JSContext *, JSObject *, jsid, jsval*);
	static JSBool IN_PROGRESS_STATE (JSContext *, JSObject *, jsid, jsval*);
	static JSBool COMPLETE_STATE    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool FAILED_STATE      (JSContext *, JSObject *, jsid, jsval*);

	// Access types

	static JSBool initializeOnly (JSContext *, JSObject *, jsid, jsval*);
	static JSBool inputOnly      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool outputOnly     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool inputOutput    (JSContext *, JSObject *, jsid, jsval*);

	// Field types

	static JSBool SFBool      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFColor     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFColorRGBA (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFDouble    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFFloat     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFInt32     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFImage     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFMatrix3d  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFMatrix3f  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFMatrix4d  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFMatrix4f  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFNode      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFRotation  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFString    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFTime      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec2d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec2f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec3d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec3f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec4d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec4f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFBool      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFColor     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFColorRGBA (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFDouble    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFFloat     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFImage     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFInt32     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFMatrix3d  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFMatrix3f  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFMatrix4d  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFMatrix4f  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFNode      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFRotation  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFString    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFTime      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec2d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec2f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec3d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec3f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec4d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec4f     (JSContext *, JSObject *, jsid, jsval*);

	// Node types

	static JSBool X3DAppearanceChildNode       (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DAppearanceNode            (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DBackgroundNode            (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DBindableNode              (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DBoundedObject             (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DChaserNode                (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DChildNode                 (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DColorNode                 (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DComposedGeometryNode      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DCoordinateNode            (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DDamperNode                (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DDragSensorNode            (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DEnvironmentalSensorNode   (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DEnvironmentTextureNode    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DFogObject                 (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DFollowerNode              (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DFontStyleNode             (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DGeometricPropertyNode     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DGeometryNode              (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DGroupingNode              (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DInfoNode                  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DInterpolatorNode          (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DKeyDeviceSensorNode       (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DLayerNode                 (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DLayoutNode                (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DLightNode                 (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DMaterialNode              (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DMetadataObject            (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DNBodyCollidableNode       (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DNBodyCollisionSpaceNode   (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DNetworkSensorNode         (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DNode                      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DNormalNode                (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DNurbsControlCurveNode     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DNurbsSurfaceGeometryNode  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DParametricGeometryNode    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DParticleEmitterNode       (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DParticlePhysicsModelNode  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DPickableObject            (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DPickingNode               (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DPickSensorNode            (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DPointingDeviceSensorNode  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DProductStructureChildNode (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DProgrammableShaderObject  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DPrototypeInstance         (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DRigidJointNode            (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DScriptNode                (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DSensorNode                (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DSequencerNode             (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DShaderNode                (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DShapeNode                 (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DSoundNode                 (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DSoundSourceNode           (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTexture2DNode             (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTexture3DNode             (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTextureCoordinateNode     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTextureNode               (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTextureTransformNode      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTimeDependentNode         (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTouchSensorNode           (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTransformNode             (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DTriggerNode               (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DUrlObject                 (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DVertexAttributeNode       (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DViewpointNode             (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DViewpointObject           (JSContext *, JSObject *, jsid, jsval*);
	static JSBool X3DViewportNode              (JSContext *, JSObject *, jsid, jsval*);

	// Members

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

} // X3D
} // titania

#endif
