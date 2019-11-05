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

#include "SupportedNodes.h"

#include "../Components.h"

namespace titania {
namespace X3D {

SupportedNodes::SupportedNodes (X3DExecutionContext* const executionContext) :
	nodes ()
{
	//std::clog << "Creating node index:" << std::endl;

	// Specification version 3.2

	addNode ("MetadataBool",                     new MetadataBoolean                  (executionContext));

	// Specification version 3.3

	addNode ("Anchor",                           new Anchor                           (executionContext));
	addNode ("AnnotationLayer",                  new AnnotationLayer                  (executionContext));
	addNode ("AnnotationTarget",                 new AnnotationTarget                 (executionContext));
	addNode ("Appearance",                       new Appearance                       (executionContext));
	addNode ("Arc2D",                            new Arc2D                            (executionContext));
	addNode ("ArcClose2D",                       new ArcClose2D                       (executionContext));
	addNode ("AudioClip",                        new AudioClip                        (executionContext));
	addNode ("Background",                       new Background                       (executionContext));
	addNode ("BallJoint",                        new BallJoint                        (executionContext));
	addNode ("Billboard",                        new Billboard                        (executionContext));
	addNode ("BlendedVolumeStyle",               new BlendedVolumeStyle               (executionContext));
	addNode ("BooleanFilter",                    new BooleanFilter                    (executionContext));
	addNode ("BooleanSequencer",                 new BooleanSequencer                 (executionContext));
	addNode ("BooleanToggle",                    new BooleanToggle                    (executionContext));
	addNode ("BooleanTrigger",                   new BooleanTrigger                   (executionContext));
	addNode ("BoundaryEnhancementVolumeStyle",   new BoundaryEnhancementVolumeStyle   (executionContext));
	addNode ("BoundedPhysicsModel",              new BoundedPhysicsModel              (executionContext));
	addNode ("Box",                              new Box                              (executionContext));
	addNode ("CADAssembly",                      new CADAssembly                      (executionContext));
	addNode ("CADFace",                          new CADFace                          (executionContext));
	addNode ("CADLayer",                         new CADLayer                         (executionContext));
	addNode ("CADPart",                          new CADPart                          (executionContext));
	addNode ("CartoonVolumeStyle",               new CartoonVolumeStyle               (executionContext));
	addNode ("Circle2D",                         new Circle2D                         (executionContext));
	addNode ("ClipPlane",                        new ClipPlane                        (executionContext));
	addNode ("CollidableOffset",                 new CollidableOffset                 (executionContext));
	addNode ("CollidableShape",                  new CollidableShape                  (executionContext));
	addNode ("Collision",                        new Collision                        (executionContext));
	addNode ("CollisionCollection",              new CollisionCollection              (executionContext));
	addNode ("CollisionSensor",                  new CollisionSensor                  (executionContext));
	addNode ("CollisionSpace",                   new CollisionSpace                   (executionContext));
	addNode ("Color",                            new Color                            (executionContext));
	addNode ("ColorChaser",                      new ColorChaser                      (executionContext));
	addNode ("ColorDamper",                      new ColorDamper                      (executionContext));
	addNode ("ColorInterpolator",                new ColorInterpolator                (executionContext));
	addNode ("ColorRGBA",                        new ColorRGBA                        (executionContext));
	addNode ("ComposedCubeMapTexture",           new ComposedCubeMapTexture           (executionContext));
	addNode ("ComposedShader",                   new ComposedShader                   (executionContext));
	addNode ("ComposedTexture3D",                new ComposedTexture3D                (executionContext));
	addNode ("ComposedVolumeStyle",              new ComposedVolumeStyle              (executionContext));
	addNode ("Cone",                             new Cone                             (executionContext));
	addNode ("ConeEmitter",                      new ConeEmitter                      (executionContext));
	addNode ("Contact",                          new Contact                          (executionContext));
	addNode ("Contour2D",                        new Contour2D                        (executionContext));
	addNode ("ContourPolyline2D",                new ContourPolyline2D                (executionContext));
	addNode ("Coordinate",                       new Coordinate                       (executionContext));
	addNode ("CoordinateChaser",                 new CoordinateChaser                 (executionContext));
	addNode ("CoordinateDamper",                 new CoordinateDamper                 (executionContext));
	addNode ("CoordinateDouble",                 new CoordinateDouble                 (executionContext));
	addNode ("CoordinateInterpolator",           new CoordinateInterpolator           (executionContext));
	addNode ("CoordinateInterpolator2D",         new CoordinateInterpolator2D         (executionContext));
	addNode ("Cylinder",                         new Cylinder                         (executionContext));
	addNode ("CylinderSensor",                   new CylinderSensor                   (executionContext));
	addNode ("DirectionalLight",                 new DirectionalLight                 (executionContext));
	addNode ("DISEntityManager",                 new DISEntityManager                 (executionContext));
	addNode ("DISEntityTypeMapping",             new DISEntityTypeMapping             (executionContext));
	addNode ("Disk2D",                           new Disk2D                           (executionContext));
	addNode ("DoubleAxisHingeJoint",             new DoubleAxisHingeJoint             (executionContext));
	addNode ("EaseInEaseOut",                    new EaseInEaseOut                    (executionContext));
	addNode ("EdgeEnhancementVolumeStyle",       new EdgeEnhancementVolumeStyle       (executionContext));
	addNode ("ElevationGrid",                    new ElevationGrid                    (executionContext));
	addNode ("EspduTransform",                   new EspduTransform                   (executionContext));
	addNode ("ExplosionEmitter",                 new ExplosionEmitter                 (executionContext));
	addNode ("Extrusion",                        new Extrusion                        (executionContext));
	addNode ("FillProperties",                   new FillProperties                   (executionContext));
	addNode ("FloatVertexAttribute",             new FloatVertexAttribute             (executionContext));
	addNode ("Fog",                              new Fog                              (executionContext));
	addNode ("FogCoordinate",                    new FogCoordinate                    (executionContext));
	addNode ("FontStyle",                        new FontStyle                        (executionContext));
	addNode ("ForcePhysicsModel",                new ForcePhysicsModel                (executionContext));
	addNode ("GeneratedCubeMapTexture",          new GeneratedCubeMapTexture          (executionContext));
	addNode ("GeoCoordinate",                    new GeoCoordinate                    (executionContext));
	addNode ("GeoElevationGrid",                 new GeoElevationGrid                 (executionContext));
	addNode ("GeoLocation",                      new GeoLocation                      (executionContext));
	addNode ("GeoLOD",                           new GeoLOD                           (executionContext));
	addNode ("GeoMetadata",                      new GeoMetadata                      (executionContext));
	addNode ("GeoOrigin",                        new GeoOrigin                        (executionContext)); // GeoOrigin node is not allowed in X3D scenes having version 3.3 or higher.
	addNode ("GeoPositionInterpolator",          new GeoPositionInterpolator          (executionContext));
	addNode ("GeoProximitySensor",               new GeoProximitySensor               (executionContext));
	addNode ("GeoTouchSensor",                   new GeoTouchSensor                   (executionContext));
	addNode ("GeoTransform",                     new GeoTransform                     (executionContext));
	addNode ("GeoViewpoint",                     new GeoViewpoint                     (executionContext));
	addNode ("Group",                            new Group                            (executionContext));
	addNode ("GroupAnnotation",                  new GroupAnnotation                  (executionContext));
	addNode ("HAnimDisplacer",                   new HAnimDisplacer                   (executionContext));
	addNode ("HAnimHumanoid",                    new HAnimHumanoid                    (executionContext));
	addNode ("HAnimJoint",                       new HAnimJoint                       (executionContext));
	addNode ("HAnimMotion",                      new HAnimMotion                      (executionContext));
	addNode ("HAnimSegment",                     new HAnimSegment                     (executionContext));
	addNode ("HAnimSite",                        new HAnimSite                        (executionContext));
	addNode ("IconAnnotation",                   new IconAnnotation                   (executionContext));
	addNode ("ImageCubeMapTexture",              new ImageCubeMapTexture              (executionContext));
	addNode ("ImageTexture",                     new ImageTexture                     (executionContext));
	addNode ("ImageTexture3D",                   new ImageTexture3D                   (executionContext));
	addNode ("IndexedFaceSet",                   new IndexedFaceSet                   (executionContext));
	addNode ("IndexedLineSet",                   new IndexedLineSet                   (executionContext));
	addNode ("IndexedQuadSet",                   new IndexedQuadSet                   (executionContext));
	addNode ("IndexedTriangleFanSet",            new IndexedTriangleFanSet            (executionContext));
	addNode ("IndexedTriangleSet",               new IndexedTriangleSet               (executionContext));
	addNode ("IndexedTriangleStripSet",          new IndexedTriangleStripSet          (executionContext));
	addNode ("Inline",                           new Inline                           (executionContext));
	addNode ("IntegerSequencer",                 new IntegerSequencer                 (executionContext));
	addNode ("IntegerTrigger",                   new IntegerTrigger                   (executionContext));
	addNode ("IsoSurfaceVolumeData",             new IsoSurfaceVolumeData             (executionContext));
	addNode ("KeySensor",                        new KeySensor                        (executionContext));
	addNode ("Layer",                            new Layer                            (executionContext));
	addNode ("LayerSet",                         new LayerSet                         (executionContext));
	addNode ("Layout",                           new Layout                           (executionContext));
	addNode ("LayoutGroup",                      new LayoutGroup                      (executionContext));
	addNode ("LayoutLayer",                      new LayoutLayer                      (executionContext));
	addNode ("LinePickSensor",                   new LinePickSensor                   (executionContext));
	addNode ("LineProperties",                   new LineProperties                   (executionContext));
	addNode ("LineSet",                          new LineSet                          (executionContext));
	addNode ("LoadSensor",                       new LoadSensor                       (executionContext));
	addNode ("LocalFog",                         new LocalFog                         (executionContext));
	addNode ("LOD",                              new LOD                              (executionContext));
	addNode ("Material",                         new Material                         (executionContext));
	addNode ("Matrix3VertexAttribute",           new Matrix3VertexAttribute           (executionContext));
	addNode ("Matrix4VertexAttribute",           new Matrix4VertexAttribute           (executionContext));
	addNode ("MetadataBoolean",                  new MetadataBoolean                  (executionContext));
	addNode ("MetadataDouble",                   new MetadataDouble                   (executionContext));
	addNode ("MetadataFloat",                    new MetadataFloat                    (executionContext));
	addNode ("MetadataInteger",                  new MetadataInteger                  (executionContext));
	addNode ("MetadataSet",                      new MetadataSet                      (executionContext));
	addNode ("MetadataString",                   new MetadataString                   (executionContext));
	addNode ("MotorJoint",                       new MotorJoint                       (executionContext));
	addNode ("MovieTexture",                     new MovieTexture                     (executionContext));
	addNode ("MultiTexture",                     new MultiTexture                     (executionContext));
	addNode ("MultiTextureCoordinate",           new MultiTextureCoordinate           (executionContext));
	addNode ("MultiTextureTransform",            new MultiTextureTransform            (executionContext));
	addNode ("NavigationInfo",                   new NavigationInfo                   (executionContext));
	addNode ("Normal",                           new Normal                           (executionContext));
	addNode ("NormalInterpolator",               new NormalInterpolator               (executionContext));
	addNode ("NurbsCurve",                       new NurbsCurve                       (executionContext));
	addNode ("NurbsCurve2D",                     new NurbsCurve2D                     (executionContext));
	addNode ("NurbsOrientationInterpolator",     new NurbsOrientationInterpolator     (executionContext));
	addNode ("NurbsPatchSurface",                new NurbsPatchSurface                (executionContext));
	addNode ("NurbsPositionInterpolator",        new NurbsPositionInterpolator        (executionContext));
	addNode ("NurbsSet",                         new NurbsSet                         (executionContext));
	addNode ("NurbsSurfaceInterpolator",         new NurbsSurfaceInterpolator         (executionContext));
	addNode ("NurbsSweptSurface",                new NurbsSweptSurface                (executionContext));
	addNode ("NurbsSwungSurface",                new NurbsSwungSurface                (executionContext));
	addNode ("NurbsTextureCoordinate",           new NurbsTextureCoordinate           (executionContext));
	addNode ("NurbsTrimmedSurface",              new NurbsTrimmedSurface              (executionContext));
	addNode ("OpacityMapVolumeStyle",            new OpacityMapVolumeStyle            (executionContext));
	addNode ("OrientationChaser",                new OrientationChaser                (executionContext));
	addNode ("OrientationDamper",                new OrientationDamper                (executionContext));
	addNode ("OrientationInterpolator",          new OrientationInterpolator          (executionContext));
	addNode ("OrthoViewpoint",                   new OrthoViewpoint                   (executionContext));
	addNode ("PackagedShader",                   new PackagedShader                   (executionContext));
	addNode ("ParticleSystem",                   new ParticleSystem                   (executionContext));
	addNode ("PickableGroup",                    new PickableGroup                    (executionContext));
	addNode ("PixelTexture",                     new PixelTexture                     (executionContext));
	addNode ("PixelTexture3D",                   new PixelTexture3D                   (executionContext));
	addNode ("PlaneSensor",                      new PlaneSensor                      (executionContext));
	addNode ("PointEmitter",                     new PointEmitter                     (executionContext));
	addNode ("PointLight",                       new PointLight                       (executionContext));
	addNode ("PointPickSensor",                  new PointPickSensor                  (executionContext));
	addNode ("PointProperties",                  new PointProperties                  (executionContext));
	addNode ("PointSet",                         new PointSet                         (executionContext));
	addNode ("Polyline2D",                       new Polyline2D                       (executionContext));
	addNode ("PolylineEmitter",                  new PolylineEmitter                  (executionContext));
	addNode ("Polypoint2D",                      new Polypoint2D                      (executionContext));
	addNode ("PositionChaser",                   new PositionChaser                   (executionContext));
	addNode ("PositionChaser2D",                 new PositionChaser2D                 (executionContext));
	addNode ("PositionDamper",                   new PositionDamper                   (executionContext));
	addNode ("PositionDamper2D",                 new PositionDamper2D                 (executionContext));
	addNode ("PositionInterpolator",             new PositionInterpolator             (executionContext));
	addNode ("PositionInterpolator2D",           new PositionInterpolator2D           (executionContext));
	addNode ("PrimitivePickSensor",              new PrimitivePickSensor              (executionContext));
	addNode ("ProgramShader",                    new ProgramShader                    (executionContext));
	addNode ("ProjectionVolumeStyle",            new ProjectionVolumeStyle            (executionContext));
	addNode ("ProximitySensor",                  new ProximitySensor                  (executionContext));
	addNode ("QuadSet",                          new QuadSet                          (executionContext));
	addNode ("ReceiverPdu",                      new ReceiverPdu                      (executionContext));
	addNode ("Rectangle2D",                      new Rectangle2D                      (executionContext));
	addNode ("RigidBody",                        new RigidBody                        (executionContext));
	addNode ("RigidBodyCollection",              new RigidBodyCollection              (executionContext));
	addNode ("ScalarChaser",                     new ScalarChaser                     (executionContext));
	addNode ("ScalarDamper",                     new ScalarDamper                     (executionContext));
	addNode ("ScalarInterpolator",               new ScalarInterpolator               (executionContext));
	addNode ("ScreenFontStyle",                  new ScreenFontStyle                  (executionContext));
	addNode ("ScreenGroup",                      new ScreenGroup                      (executionContext));
	addNode ("Script",                           new Script                           (executionContext));
	addNode ("SegmentedVolumeData",              new SegmentedVolumeData              (executionContext));
	addNode ("ShadedVolumeStyle",                new ShadedVolumeStyle                (executionContext));
	addNode ("ShaderPart",                       new ShaderPart                       (executionContext));
	addNode ("ShaderProgram",                    new ShaderProgram                    (executionContext));
	addNode ("Shape",                            new Shape                            (executionContext));
	addNode ("SignalPdu",                        new SignalPdu                        (executionContext));
	addNode ("SilhouetteEnhancementVolumeStyle", new SilhouetteEnhancementVolumeStyle (executionContext));
	addNode ("SingleAxisHingeJoint",             new SingleAxisHingeJoint             (executionContext));
	addNode ("SliderJoint",                      new SliderJoint                      (executionContext));
	addNode ("Sound",                            new Sound                            (executionContext));
	addNode ("Sphere",                           new Sphere                           (executionContext));
	addNode ("SphereSensor",                     new SphereSensor                     (executionContext));
	addNode ("SplinePositionInterpolator",       new SplinePositionInterpolator       (executionContext));
	addNode ("SplinePositionInterpolator2D",     new SplinePositionInterpolator2D     (executionContext));
	addNode ("SplineScalarInterpolator",         new SplineScalarInterpolator         (executionContext));
	addNode ("SpotLight",                        new SpotLight                        (executionContext));
	addNode ("SquadOrientationInterpolator",     new SquadOrientationInterpolator     (executionContext));
	addNode ("StaticGroup",                      new StaticGroup                      (executionContext));
	addNode ("StringSensor",                     new StringSensor                     (executionContext));
	addNode ("SurfaceEmitter",                   new SurfaceEmitter                   (executionContext));
	addNode ("Switch",                           new Switch                           (executionContext));
	addNode ("TexCoordChaser2D",                 new TexCoordChaser2D                 (executionContext));
	addNode ("TexCoordDamper2D",                 new TexCoordDamper2D                 (executionContext));
	addNode ("Text",                             new Text                             (executionContext));
	addNode ("TextAnnotation",                   new TextAnnotation                   (executionContext));
	addNode ("TextureBackground",                new TextureBackground                (executionContext));
	addNode ("TextureCoordinate",                new TextureCoordinate                (executionContext));
	addNode ("TextureCoordinate3D",              new TextureCoordinate3D              (executionContext));
	addNode ("TextureCoordinate4D",              new TextureCoordinate4D              (executionContext));
	addNode ("TextureCoordinateGenerator",       new TextureCoordinateGenerator       (executionContext));
	addNode ("TextureProjectorParallel",         new TextureProjectorParallel         (executionContext));
	addNode ("TextureProjectorPerspective",      new TextureProjectorPerspective      (executionContext));
	addNode ("TextureProperties",                new TextureProperties                (executionContext));
	addNode ("TextureTransform",                 new TextureTransform                 (executionContext));
	addNode ("TextureTransform3D",               new TextureTransform3D               (executionContext));
	addNode ("TextureTransformMatrix3D",         new TextureTransformMatrix3D         (executionContext));
	addNode ("TimeSensor",                       new TimeSensor                       (executionContext));
	addNode ("TimeTrigger",                      new TimeTrigger                      (executionContext));
	addNode ("ToneMappedVolumeStyle",            new ToneMappedVolumeStyle            (executionContext));
	addNode ("TouchSensor",                      new TouchSensor                      (executionContext));
	addNode ("Transform",                        new Transform                        (executionContext));
	addNode ("TransformSensor",                  new TransformSensor                  (executionContext));
	addNode ("TransmitterPdu",                   new TransmitterPdu                   (executionContext));
	addNode ("TriangleFanSet",                   new TriangleFanSet                   (executionContext));
	addNode ("TriangleSet",                      new TriangleSet                      (executionContext));
	addNode ("TriangleSet2D",                    new TriangleSet2D                    (executionContext));
	addNode ("TriangleStripSet",                 new TriangleStripSet                 (executionContext));
	addNode ("TwoSidedMaterial",                 new TwoSidedMaterial                 (executionContext));
	addNode ("UniversalJoint",                   new UniversalJoint                   (executionContext));
	addNode ("URLAnnotation",                    new URLAnnotation                    (executionContext));
	addNode ("Viewpoint",                        new Viewpoint                        (executionContext));
	addNode ("ViewpointGroup",                   new ViewpointGroup                   (executionContext));
	addNode ("Viewport",                         new Viewport                         (executionContext));
	addNode ("VisibilitySensor",                 new VisibilitySensor                 (executionContext));
	addNode ("VolumeData",                       new VolumeData                       (executionContext));
	addNode ("VolumeEmitter",                    new VolumeEmitter                    (executionContext));
	addNode ("VolumePickSensor",                 new VolumePickSensor                 (executionContext));
	addNode ("WindPhysicsModel",                 new WindPhysicsModel                 (executionContext));
	addNode ("WorldInfo",                        new WorldInfo                        (executionContext));

	//  Non standard: Component »Titania«

	addNode ("BlendMode",                        new BlendMode                        (executionContext));
	addNode ("DepthBuffer",                      new DepthBuffer                      (executionContext));
	addNode ("PolygonMode",                      new PolygonMode                      (executionContext));
	addNode ("PolygonOffset",                    new PolygonOffset                    (executionContext));
	addNode ("TouchGroup",                       new TouchGroup                       (executionContext));
	addNode ("TransformMatrix3D",                new TransformMatrix3D                (executionContext));

	//std::clog << "\tDone creating node index." << std::endl;
}

///  throws Error <INVALID_NAME>
void
SupportedNodes::addNode (const std::string & typeName, const X3DBaseNode* const node)
{
	//__LOG__ << "\tAdding node type " << typeName << ": " << std::endl;

	if (nodes .emplace (typeName, node) .second)
		return;

	throw Error <INVALID_NAME> ("Node type '" + typeName + "' already exists.");
}

///  throws Error <NOT_SUPPORTED>
const X3DBaseNode*
SupportedNodes::getNode (const std::string & typeName) const
{
	try
	{
		return nodes .at (typeName);
	}
	catch (const std::out_of_range &)
	{
		throw Error <NOT_SUPPORTED> ("Unknown node type '" + typeName + "'.");
	}
}

const SupportedNodesArray &
SupportedNodes::getNodes () const
{
	return nodes;
}

SupportedNodes::~SupportedNodes ()
{
	for (const auto & pair : nodes)
		delete pair .second;
}

} // X3D
} // titania
