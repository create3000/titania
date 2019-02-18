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

#ifndef __TITANIA_X3D_TYPES_POINTER_H__
#define __TITANIA_X3D_TYPES_POINTER_H__

#include "../Fields/X3DPtr.h"

namespace titania {
namespace X3D {

// Basic node types

class Anchor;
class Appearance;
class Arc2D;
class ArcClose2D;
class AudioClip;
class Background;
class BallJoint;
class Billboard;
class BooleanFilter;
class BooleanSequencer;
class BooleanToggle;
class BooleanTrigger;
class BoundedPhysicsModel;
class Box;
class CADAssembly;
class CADFace;
class CADLayer;
class CADPart;
class Circle2D;
class ClipPlane;
class CollidableOffset;
class CollidableShape;
class Collision;
class CollisionCollection;
class CollisionSensor;
class CollisionSpace;
class Color;
class ColorChaser;
class ColorDamper;
class ColorInterpolator;
class ColorRGBA;
class ComposedCubeMapTexture;
class ComposedShader;
class ComposedTexture3D;
class Cone;
class ConeEmitter;
class Contact;
class Contour2D;
class ContourPolyline2D;
class Coordinate;
class CoordinateChaser;
class CoordinateDamper;
class CoordinateDouble;
class CoordinateInterpolator;
class CoordinateInterpolator2D;
class Cylinder;
class CylinderSensor;
class DISEntityManager;
class DISEntityTypeMapping;
class DirectionalLight;
class Disk2D;
class DoubleAxisHingeJoint;
class EaseInEaseOut;
class ElevationGrid;
class EspduTransform;
class ExplosionEmitter;
class Extrusion;
class FillProperties;
class FloatVertexAttribute;
class Fog;
class FogCoordinate;
class FontStyle;
class ForcePhysicsModel;
class GeneratedCubeMapTexture;
class GeoCoordinate;
class GeoElevationGrid;
class GeoLOD;
class GeoLocation;
class GeoMetadata;
class GeoOrigin;
class GeoPositionInterpolator;
class GeoProximitySensor;
class GeoTouchSensor;
class GeoTransform;
class GeoViewpoint;
class Group;
class HAnimDisplacer;
class HAnimHumanoid;
class HAnimJoint;
class HAnimSegment;
class HAnimSite;
class ImageCubeMapTexture;
class ImageTexture;
class ImageTexture3D;
class IndexedFaceSet;
class IndexedLineSet;
class IndexedQuadSet;
class IndexedTriangleFanSet;
class IndexedTriangleSet;
class IndexedTriangleStripSet;
class Inline;
class IntegerSequencer;
class IntegerTrigger;
class KeySensor;
class LOD;
class Layer;
class LayerSet;
class Layout;
class LayoutGroup;
class LayoutLayer;
class LinePickSensor;
class LineProperties;
class LineSet;
class LoadSensor;
class LocalFog;
class Material;
class Matrix3VertexAttribute;
class Matrix4VertexAttribute;
class MetadataBoolean;
class MetadataDouble;
class MetadataFloat;
class MetadataInteger;
class MetadataSet;
class MetadataString;
class MotorJoint;
class MovieTexture;
class MultiTexture;
class MultiTextureCoordinate;
class MultiTextureTransform;
class NavigationInfo;
class Normal;
class NormalInterpolator;
class NurbsCurve;
class NurbsCurve2D;
class NurbsOrientationInterpolator;
class NurbsPatchSurface;
class NurbsPositionInterpolator;
class NurbsSet;
class NurbsSurfaceInterpolator;
class NurbsSweptSurface;
class NurbsSwungSurface;
class NurbsTextureCoordinate;
class NurbsTrimmedSurface;
class OrientationChaser;
class OrientationDamper;
class OrientationInterpolator;
class OrthoViewpoint;
class PackagedShader;
class ParticleSystem;
class PickableGroup;
class PixelTexture;
class PixelTexture3D;
class PlaneSensor;
class PointEmitter;
class PointLight;
class PointPickSensor;
class PointSet;
class Polyline2D;
class PolylineEmitter;
class Polypoint2D;
class PositionChaser;
class PositionChaser2D;
class PositionDamper;
class PositionDamper2D;
class PositionInterpolator;
class PositionInterpolator2D;
class PrimitivePickSensor;
class ProgramShader;
class ProximitySensor;
class QuadSet;
class ReceiverPdu;
class Rectangle2D;
class RigidBody;
class RigidBodyCollection;
class ScalarChaser;
class ScalarDamper;
class ScalarInterpolator;
class ScreenFontStyle;
class ScreenGroup;
class Script;
class ShaderPart;
class ShaderProgram;
class Shape;
class SignalPdu;
class SingleAxisHingeJoint;
class SliderJoint;
class Sound;
class Sphere;
class SphereSensor;
class SplinePositionInterpolator;
class SplinePositionInterpolator2D;
class SplineScalarInterpolator;
class SpotLight;
class SquadOrientationInterpolator;
class StaticGroup;
class StringSensor;
class SurfaceEmitter;
class Switch;
class TexCoordChaser2D;
class TexCoordDamper2D;
class Text;
class TextureBackground;
class TextureCoordinate;
class TextureCoordinate3D;
class TextureCoordinate4D;
class TextureCoordinateGenerator;
class TextureProperties;
class TextureTransform;
class TextureTransform3D;
class TextureTransformMatrix3D;
class TimeSensor;
class TimeTrigger;
class TouchSensor;
class Transform;
class TransformSensor;
class TransmitterPdu;
class TriangleFanSet;
class TriangleSet;
class TriangleSet2D;
class TriangleStripSet;
class TwoSidedMaterial;
class UniversalJoint;
class Viewpoint;
class ViewpointGroup;
class Viewport;
class VisibilitySensor;
class VolumeEmitter;
class VolumePickSensor;
class WindPhysicsModel;
class WorldInfo;

// Abstract node types

class X3DAppearanceChildNode;
class X3DAppearanceNode;
class X3DBackgroundNode;
class X3DBindableNode;
class X3DBoundedObject;
class X3DChaserNode;
class X3DChildNode;
class X3DColorNode;
class X3DComposedGeometryNode;
class X3DCoordinateNode;
class X3DDamperNode;
class X3DDragSensorNode;
class X3DEnvironmentTextureNode;
class X3DEnvironmentalSensorNode;
class X3DFogObject;
class X3DFollowerNode;
class X3DFontStyleNode;
class X3DGeometricPropertyNode;
class X3DGeometryNode;
class X3DGeospatialObject;
class X3DGroupingNode;
class X3DInfoNode;
class X3DInterpolatorNode;
class X3DKeyDeviceSensorNode;
class X3DLayerNode;
class X3DLayoutNode;
class X3DLightNode;
class X3DMaterialNode;
class X3DMetadataObject;
class X3DNBodyCollidableNode;
class X3DNBodyCollisionSpaceNode;
class X3DNetworkSensorNode;
class X3DNode;
class X3DNormalNode;
class X3DNurbsControlCurveNode;
class X3DNurbsSurfaceGeometryNode;
class X3DParametricGeometryNode;
class X3DParticleEmitterNode;
class X3DParticlePhysicsModelNode;
class X3DPickSensorNode;
class X3DPickableObject;
class X3DPointingDeviceSensorNode;
class X3DProductStructureChildNode;
class X3DProgrammableShaderObject;
class X3DPrototypeInstance;
class X3DRigidJointNode;
class X3DScriptNode;
class X3DSensorNode;
class X3DSequencerNode;
class X3DShaderNode;
class X3DShapeNode;
class X3DSoundNode;
class X3DSoundSourceNode;
class X3DTexture2DNode;
class X3DTexture3DNode;
class X3DTextureCoordinateNode;
class X3DTextureNode;
class X3DTextureTransformNode;
class X3DTimeDependentNode;
class X3DTouchSensorNode;
class X3DTransformNode;
class X3DTriggerNode;
class X3DUrlObject;
class X3DVertexAttributeNode;
class X3DViewpointNode;
class X3DViewpointObject;
class X3DViewportNode;

// Non statndard

class BlendMode;
class Clipboard;
class DepthBuffer;
class FaceSelection;
class PolygonOffset;
class TouchGroup;
class TransformMatrix3D;
class X3DFaceSelection;

// Option nodes

class Arc2DOptions;
class ArcClose2DOptions;
class Circle2DOptions;
class Disk2DOptions;
class Rectangle2DOptions;

class BoxOptions;
class ConeOptions;
class CylinderOptions;
class OctahedronOptions;
class QuadSphereOptions;
class TetrahedronOptions;
class X3DSphereOptionsNode;

class LSystem;

// Browser node types

class Browser;
class BrowserApplication;
class BrowserOptions;
class BrowserProperties;
class Console;
class ExportedNode;
class ExternProtoDeclaration;
class FieldSet;
class ImportedNode;
class MotionBlur;
class NamedNode;
class Notification;
class Parser;
class ProtoDeclaration;
class RenderingProperties;
class Route;
class Router;
class Scene;
class Selection;
class SupportedComponents;
class SupportedFields;
class SupportedNodes;
class SupportedProfiles;
class World;
class X3DBrowser;
class X3DBrowserContext;
class X3DExecutionContext;
class X3DJavaScriptContext;
class X3DJavaScriptEngine;
class X3DParser;
class X3DProtoDeclarationNode;
class X3DRenderObject;
class X3DScene;
class X3DBaseNode;

// Devices

class KeyDevice;
class PointingDevice;

// Viewer

class DefaultViewer;
class ExamineViewer;
class FlyViewer;
class LookAtViewer;
class NoneViewer;
class PlaneViewer;
class WalkViewer;
class X3DExamineViewer;
class X3DViewer;

// Viewer tools

class LightSaber;
class RectangleSelection;
class LassoSelection;
class X3DSelector;

// Tool nodes

class UndoStepContainer;

class AnchorTool;
class AngleGridTool;
class BillboardTool;
class CADAssemblyTool;
class CADFaceTool;
class CADLayerTool;
class CADPartTool;
class CollisionTool;
class CoordinateTool;
class DirectionalLightTool;
class GeoLocationTool;
class GeoLODTool;
class GeoTransformTool;
class GeoViewpointTool;
class GridTool;
class GroupTool;
class IndexedFaceSetTool;
class InlineTool;
class LayoutGroupTool;
class LODTool;
class NormalTool;
class OrthoViewpointTool;
class ParticleSystemTool;
class PickableGroupTool;
class PointLightTool;
class ProximitySensorTool;
class ScreenGroupTool;
class ShapeTool;
class SnapSourceTool;
class SnapTargetTool;
class SoundTool;
class SpotLightTool;
class SwitchTool;
class TouchGroupTool;
class TransformTool;
class TransformSensorTool;
class ViewpointTool;
class VisibilitySensorTool;
class X3DActiveLayerTool;
class X3DBindableNodeTool;
class X3DBoundedObjectTool;
class X3DChildNodeTool;
class X3DComposedGeometryNodeTool;
class X3DCoordinateNodeTool;
class X3DGeometricPropertyNodeTool;
class X3DEnvironmentalSensorNodeTool;
class X3DGeometryNodeTool;
class X3DGeospatialObjectTool;
class X3DGridTool;
class X3DGroupingNodeTool;
class X3DIndexedFaceSetCutObject;
class X3DIndexedFaceSetKnifeObject;
class X3DIndexedFaceSetOperationsObject;
class X3DIndexedFaceSetSelectionObject;
class X3DIndexedFaceSetTool;
class X3DIndexedFaceSetTransformObject;
class X3DLightNodeTool;
class X3DNodeTool;
class X3DNormalNodeTool;
class X3DProductStructureChildNodeTool;
class X3DSensorNodeTool;
class X3DShapeNodeTool;
class X3DSnapTool;
class X3DSoundNodeTool;
class X3DToolObject;
class X3DTransformMatrix3DNodeTool;
class X3DTransformNodeTool;
class X3DUrlObjectTool;
class X3DViewpointNodeTool;

///  Futures

class X3DFuture;
class SceneFuture;

// Bindable stacks and lists

template <class Type>
class X3DBindableNodeStack;

template <class Type>
class X3DBindableNodeList;

using NavigationInfoStack = X3DBindableNodeStack <NavigationInfo>;
using BackgroundStack     = X3DBindableNodeStack <X3DBackgroundNode>;
using FogStack            = X3DBindableNodeStack <Fog>;
using ViewpointStack      = X3DBindableNodeStack <X3DViewpointNode>;

using NavigationInfoList = X3DBindableNodeList <NavigationInfo>;
using BackgroundList     = X3DBindableNodeList <X3DBackgroundNode>;
using FogList            = X3DBindableNodeList <Fog>;
using ViewpointList      = X3DBindableNodeList <X3DViewpointNode>;

// typedef's

using BrowserApplicationPtr  = X3DPtr <BrowserApplication>;
using BrowserPtr             = X3DPtr <Browser>;
using X3DBrowserPtr          = X3DPtr <X3DBrowser>;
using X3DBrowserContextPtr   = X3DPtr <X3DBrowserContext>;
using RenderingPropertiesPtr = X3DPtr <RenderingProperties>;
using BrowserPropertiesPtr   = X3DPtr <BrowserProperties>;
using BrowserOptionsPtr      = X3DPtr <BrowserOptions>;
using SelectionPtr           = X3DPtr <Selection>;
using NotificationPtr        = X3DPtr <Notification>;
using ConsolePtr             = X3DPtr <Console>;
using ClipboardPtr           = X3DPtr <Clipboard>;

using WorldPtr                   = X3DPtr <World>;
using ScenePtr                   = X3DPtr <Scene>;
using X3DScenePtr                = X3DPtr <X3DScene>;
using X3DExecutionContextPtr     = X3DPtr <X3DExecutionContext>;
using ProtoDeclarationPtr        = X3DPtr <ProtoDeclaration>;
using X3DProtoDeclarationNodePtr = X3DPtr <X3DProtoDeclarationNode>;
using X3DPrototypeInstancePtr    = X3DPtr <X3DPrototypeInstance>;
using ExternProtoDeclarationPtr  = X3DPtr <ExternProtoDeclaration>;
using NamedNodePtr               = X3DPtr <NamedNode>;
using ImportedNodePtr            = X3DPtr <ImportedNode>;
using RoutePtr                   = X3DPtr <Route>;
using ExportedNodePtr            = X3DPtr <ExportedNode>;

using NavigationInfoStackPtr = X3DPtr <NavigationInfoStack>;
using BackgroundStackPtr     = X3DPtr <BackgroundStack>;
using FogStackPtr            = X3DPtr <FogStack>;
using ViewpointStackPtr      = X3DPtr <ViewpointStack>;
using NavigationInfoListPtr  = X3DPtr <NavigationInfoList>;
using BackgroundListPtr      = X3DPtr <BackgroundList>;
using FogListPtr             = X3DPtr <FogList>;
using ViewpointListPtr       = X3DPtr <ViewpointList>;

using SceneFuturePtr      = X3DPtr <SceneFuture>;
using SceneFutureCallback = std::function <void (X3DScenePtr &&)>;

class UndoStepContainer;

using UndoStepContainerPtr = X3DPtr <UndoStepContainer>;

} // X3D
} // titania

#endif
