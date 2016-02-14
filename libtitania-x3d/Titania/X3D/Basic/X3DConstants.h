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

#ifndef __TITANIA_X3D_BITS_X3DCONSTANTS_H__
#define __TITANIA_X3D_BITS_X3DCONSTANTS_H__

#include <cstdint>
#include <iostream>

namespace titania {
namespace X3D {

// Official Constants

namespace X3DConstants {

enum EventType : uint8_t
{
	INITIALIZED_EVENT,
	SHUTDOWN_EVENT

};

enum ErrorType : uint8_t
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

enum LoadState : uint8_t
{
	NOT_STARTED_STATE,
	IN_PROGRESS_STATE,
	COMPLETE_STATE,
	FAILED_STATE

};

enum AccessType : uint8_t
{
	initializeOnly = 1, // 0b0001
	inputOnly      = 2, // 0b0010
	outputOnly     = 4, // 0b0100
	inputOutput    = 7  // 0b0111

};

enum FieldType : uint8_t
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

enum NodeType : uint16_t
{
	// Base node type

	X3DBaseNode,

	// Basic node types

	Anchor,
	Appearance,
	Arc2D,
	ArcClose2D,
	AudioClip,
	Background,
	BallJoint,
	Billboard,
	BooleanFilter,
	BooleanSequencer,
	BooleanToggle,
	BooleanTrigger,
	BoundedPhysicsModel,
	Box,
	CADAssembly,
	CADFace,
	CADLayer,
	CADPart,
	Circle2D,
	ClipPlane,
	CollidableOffset,
	CollidableShape,
	Collision,
	CollisionCollection,
	CollisionSensor,
	CollisionSpace,
	Color,
	ColorChaser,
	ColorDamper,
	ColorInterpolator,
	ColorRGBA,
	ComposedCubeMapTexture,
	ComposedShader,
	ComposedTexture3D,
	Cone,
	ConeEmitter,
	Contact,
	Contour2D,
	ContourPolyline2D,
	Coordinate,
	CoordinateChaser,
	CoordinateDamper,
	CoordinateDouble,
	CoordinateInterpolator,
	CoordinateInterpolator2D,
	Cylinder,
	CylinderSensor,
	DISEntityManager,
	DISEntityTypeMapping,
	DirectionalLight,
	Disk2D,
	DoubleAxisHingeJoint,
	EaseInEaseOut,
	ElevationGrid,
	EspduTransform,
	ExplosionEmitter,
	Extrusion,
	FillProperties,
	FloatVertexAttribute,
	Fog,
	FogCoordinate,
	FontStyle,
	ForcePhysicsModel,
	GeneratedCubeMapTexture,
	GeoCoordinate,
	GeoElevationGrid,
	GeoLOD,
	GeoLocation,
	GeoMetadata,
	GeoOrigin,
	GeoPositionInterpolator,
	GeoProximitySensor,
	GeoTouchSensor,
	GeoTransform,
	GeoViewpoint,
	Group,
	HAnimDisplacer,
	HAnimHumanoid,
	HAnimJoint,
	HAnimSegment,
	HAnimSite,
	ImageCubeMapTexture,
	ImageTexture,
	ImageTexture3D,
	IndexedFaceSet,
	IndexedLineSet,
	IndexedQuadSet,
	IndexedTriangleFanSet,
	IndexedTriangleSet,
	IndexedTriangleStripSet,
	Inline,
	IntegerSequencer,
	IntegerTrigger,
	KeySensor,
	LOD,
	Layer,
	LayerSet,
	Layout,
	LayoutGroup,
	LayoutLayer,
	LinePickSensor,
	LineProperties,
	LineSet,
	LoadSensor,
	LocalFog,
	Material,
	Matrix3VertexAttribute,
	Matrix4VertexAttribute,
	MetadataBoolean,
	MetadataDouble,
	MetadataFloat,
	MetadataInteger,
	MetadataSet,
	MetadataString,
	MotorJoint,
	MovieTexture,
	MultiTexture,
	MultiTextureCoordinate,
	MultiTextureTransform,
	NavigationInfo,
	Normal,
	NormalInterpolator,
	NurbsCurve,
	NurbsCurve2D,
	NurbsOrientationInterpolator,
	NurbsPatchSurface,
	NurbsPositionInterpolator,
	NurbsSet,
	NurbsSurfaceInterpolator,
	NurbsSweptSurface,
	NurbsSwungSurface,
	NurbsTextureCoordinate,
	NurbsTrimmedSurface,
	OrientationChaser,
	OrientationDamper,
	OrientationInterpolator,
	OrthoViewpoint,
	PackagedShader,
	ParticleSystem,
	PickableGroup,
	PixelTexture,
	PixelTexture3D,
	PlaneSensor,
	PointEmitter,
	PointLight,
	PointPickSensor,
	PointSet,
	Polyline2D,
	PolylineEmitter,
	Polypoint2D,
	PositionChaser,
	PositionChaser2D,
	PositionDamper,
	PositionDamper2D,
	PositionInterpolator,
	PositionInterpolator2D,
	PrimitivePickSensor,
	ProgramShader,
	ProximitySensor,
	QuadSet,
	ReceiverPdu,
	Rectangle2D,
	RigidBody,
	RigidBodyCollection,
	ScalarChaser,
	ScalarDamper,
	ScalarInterpolator,
	ScreenFontStyle,
	ScreenGroup,
	Script,
	ShaderPart,
	ShaderProgram,
	Shape,
	SignalPdu,
	SingleAxisHingeJoint,
	SliderJoint,
	Sound,
	Sphere,
	SphereSensor,
	SplinePositionInterpolator,
	SplinePositionInterpolator2D,
	SplineScalarInterpolator,
	SpotLight,
	SquadOrientationInterpolator,
	StaticGroup,
	StringSensor,
	SurfaceEmitter,
	Switch,
	TexCoordChaser2D,
	TexCoordDamper2D,
	Text,
	TextureBackground,
	TextureCoordinate,
	TextureCoordinate3D,
	TextureCoordinate4D,
	TextureCoordinateGenerator,
	TextureProperties,
	TextureTransform,
	TextureTransform3D,
	TextureTransformMatrix3D,
	TimeSensor,
	TimeTrigger,
	TouchSensor,
	Transform,
	TransformSensor,
	TransmitterPdu,
	TriangleFanSet,
	TriangleSet,
	TriangleSet2D,
	TriangleStripSet,
	TwoSidedMaterial,
	UniversalJoint,
	Viewpoint,
	ViewpointGroup,
	Viewport,
	VisibilitySensor,
	VolumeEmitter,
	VolumePickSensor,
	WindPhysicsModel,
	WorldInfo,

	// Abstract node types

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
	X3DEnvironmentTextureNode,
	X3DEnvironmentalSensorNode,
	X3DFogObject,
	X3DFollowerNode,
	X3DFontStyleNode,
	X3DGeometricPropertyNode,
	X3DGeometryNode,
	X3DGeospatialObject,
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
	X3DPickSensorNode,
	X3DPickableObject,
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
	X3DTransformMatrix4DNode,
	X3DTransformNode,
	X3DTriggerNode,
	X3DUrlObject,
	X3DVertexAttributeNode,
	X3DViewpointNode,
	X3DViewpointObject,
	X3DViewportNode,

	// Non statndard

	Clipboard,
	FaceSelection,
	TouchGroup,
	TransformMatrix3D,
	X3DFaceSelection,

	// Browser node types

	Browser,
	BrowserApplication,
	BrowserOptions,
	BrowserProperties,
	Console,
	ExportedNode,
	ExternProtoDeclaration,
	FieldSet,
	ImportedNode,
	MotionBlur,
	NamedNode,
	Notification,
	Parser,
	ProtoDeclaration,
	RenderingProperties,
	Route,
	Scene,
	Selection,
	World,
	X3DBrowser,
	X3DBrowserContext,
	X3DExecutionContext,
	X3DJavaScriptContext,
	X3DJavaScriptEngine,
	X3DParser,
	X3DProtoDeclarationNode,
	X3DRenderObject,
	X3DRouterObject,
	X3DScene,

	// Viewer

	DefaultViewer,
	ExamineViewer,
	FlyViewer,
	LookAtViewer,
	NoneViewer,
	PlaneViewer,
	WalkViewer,
	RectangleSelection,
	LassoSelection,
	X3DExamineViewer,
	X3DSelector,

	// Tool nodes

	AnchorTool,
	AngleTool,
	BillboardTool,
	CADAssemblyTool,
	CADFaceTool,
	CADLayerTool,
	CADPartTool,
	CollisionTool,
	CoordinateTool,
	DirectionalLightTool,
	GeoLocationTool,
	GeoLODTool,
	GeoTransformTool,
	GeoViewpointTool,
	GridTool,
	GroupTool,
	IndexedFaceSetTool,
	InlineTool,
	LayoutGroupTool,
	LODTool,
	NormalTool,
	OrthoViewpointTool,
	ParticleSystemTool,
	PickableGroupTool,
	PointLightTool,
	ProximitySensorTool,
	ScreenGroupTool,
	ShapeTool,
	SpotLightTool,
	SwitchTool,
	Tool,
	TouchGroupTool,
	TransformTool,
	TransformSensorTool,
	ViewpointTool,
	VisibilitySensorTool,
	X3DActiveLayerTool,
	X3DBindableNodeTool,
	X3DBoundedObjectTool,
	X3DChildNodeTool,
	X3DComposedGeometryNodeTool,
	X3DCoordinateNodeTool,
	X3DGeometricPropertyNodeTool,
	X3DEnvironmentalSensorNodeTool,
	X3DGeometryNodeTool,
	X3DGeospatialObjectTool,
	X3DGridTool,
	X3DGroupingNodeTool,
	X3DLightNodeTool,
	X3DNodeTool,
	X3DNormalNodeTool,
	X3DProductStructureChildNodeTool,
	X3DSensorNodeTool,
	X3DShapeNodeTool,
	X3DToolObject,
	X3DTransformMatrix4DNodeTool,
	X3DTransformNodeTool,
	X3DUrlObjectTool,
	X3DViewpointNodeTool


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

using X3DConstants::NodeType;

enum class ComponentType : uint8_t
{
	CAD_GEOMETRY,
	CORE,
	CUBE_MAP_TEXTURING,
	DIS,
	ENVIRONMENTAL_EFFECTS,
	ENVIRONMENTAL_SENSOR,
	EVENT_UTILITIES,
	FOLLOWERS,
	GEOMETRY_2D,
	GEOMETRY_3D,
	GEOSPATIAL,
	GROUPING,
	H_ANIM,
	INTERPOLATION,
	KEY_DEVICE_SENSOR,
	LAYERING,
	LAYOUT,
	LIGHTING,
	NAVIGATION,
	NETWORKING,
	NURBS,
	PARTICLE_SYSTEMS,
	PICKING,
	POINTING_DEVICE_SENSOR,
	RENDERING,
	RIGID_BODY_PHYSICS,
	SCRIPTING,
	SHADERS,
	SHAPE,
	SOUND,
	TEXT,
	TEXTURING,
	TEXTURING_3D,
	TIME,
	VOLUME_RENDERING,

	TITANIA

};

enum SpecificationVersionType : uint8_t
{
	VRML_V2_0,
	X3D_V3_0,
	X3D_V3_1,
	X3D_V3_2,
	X3D_V3_3

};

static constexpr SpecificationVersionType LATEST_VERSION = SpecificationVersionType::X3D_V3_3;

inline
std::ostream &
operator << (std::ostream & ostream, const SpecificationVersionType specificationVersion)
{
	switch (specificationVersion)
	{
		case VRML_V2_0: return ostream << "VRML V2.0";
		case X3D_V3_0:  return ostream << "X3D V3.0";
		case X3D_V3_1:  return ostream << "X3D V3.1";
		case X3D_V3_2:  return ostream << "X3D V3.2";
		case X3D_V3_3:
		default:
			return ostream << "X3D V3.3";
	}
}

/// XMLEncode

inline
std::string
XMLEncode (const SpecificationVersionType specificationVersion)
{
	switch (specificationVersion)
	{
		case X3D_V3_0: return "3.0";
		case X3D_V3_1: return "3.1";
		case X3D_V3_2: return "3.2";
		case X3D_V3_3:
		default:
			return "3.3";
	}
}

enum CopyType : uint8_t
{
	CLONE,
	COPY_OR_CLONE,
	FLAT_COPY

};

} // X3D
} // titania

#endif
