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

#include "SupportedNodes.h"

#include "../Components.h"

namespace titania {
namespace X3D {

SupportedNodes::SupportedNodes (X3DExecutionContext* const executionContext)
{
	//std::clog << "Creating node index:" << std::endl;
	
	// Specification version 3.2
	
__LOG__ << std::endl;
	add ("MetadataBool", new MetadataBoolean (executionContext));

	// Specification version 3.3
	
__LOG__ << std::endl;
	add ("Anchor",                       new Anchor                       (executionContext));
__LOG__ << std::endl;
	add ("Appearance",                   new Appearance                   (executionContext));
__LOG__ << std::endl;
	add ("Arc2D",                        new Arc2D                        (executionContext));
__LOG__ << std::endl;
	add ("ArcClose2D",                   new ArcClose2D                   (executionContext));
__LOG__ << std::endl;
	add ("AudioClip",                    new AudioClip                    (executionContext));
__LOG__ << std::endl;
	add ("Background",                   new Background                   (executionContext));
__LOG__ << std::endl;
	add ("BallJoint",                    new BallJoint                    (executionContext));
__LOG__ << std::endl;
	add ("Billboard",                    new Billboard                    (executionContext));
__LOG__ << std::endl;
	add ("BooleanFilter",                new BooleanFilter                (executionContext));
__LOG__ << std::endl;
	add ("BooleanSequencer",             new BooleanSequencer             (executionContext));
__LOG__ << std::endl;
	add ("BooleanToggle",                new BooleanToggle                (executionContext));
__LOG__ << std::endl;
	add ("BooleanTrigger",               new BooleanTrigger               (executionContext));
__LOG__ << std::endl;
	add ("BoundedPhysicsModel",          new BoundedPhysicsModel          (executionContext));
__LOG__ << std::endl;
	add ("Box",                          new Box                          (executionContext));
__LOG__ << std::endl;
	add ("CADAssembly",                  new CADAssembly                  (executionContext));
__LOG__ << std::endl;
	add ("CADFace",                      new CADFace                      (executionContext));
__LOG__ << std::endl;
	add ("CADLayer",                     new CADLayer                     (executionContext));
__LOG__ << std::endl;
	add ("CADPart",                      new CADPart                      (executionContext));
__LOG__ << std::endl;
	add ("Circle2D",                     new Circle2D                     (executionContext));
__LOG__ << std::endl;
	add ("ClipPlane",                    new ClipPlane                    (executionContext));
__LOG__ << std::endl;
	add ("CollidableOffset",             new CollidableOffset             (executionContext));
__LOG__ << std::endl;
	add ("CollidableShape",              new CollidableShape              (executionContext));
__LOG__ << std::endl;
	add ("Collision",                    new Collision                    (executionContext));
__LOG__ << std::endl;
	add ("CollisionCollection",          new CollisionCollection          (executionContext));
__LOG__ << std::endl;
	add ("CollisionSensor",              new CollisionSensor              (executionContext));
__LOG__ << std::endl;
	add ("CollisionSpace",               new CollisionSpace               (executionContext));
__LOG__ << std::endl;
	add ("Color",                        new Color                        (executionContext));
__LOG__ << std::endl;
	add ("ColorChaser",                  new ColorChaser                  (executionContext));
__LOG__ << std::endl;
	add ("ColorDamper",                  new ColorDamper                  (executionContext));
__LOG__ << std::endl;
	add ("ColorInterpolator",            new ColorInterpolator            (executionContext));
__LOG__ << std::endl;
	add ("ColorRGBA",                    new ColorRGBA                    (executionContext));
__LOG__ << std::endl;
	add ("ComposedCubeMapTexture",       new ComposedCubeMapTexture       (executionContext));
__LOG__ << std::endl;
	add ("ComposedShader",               new ComposedShader               (executionContext));
__LOG__ << std::endl;
	add ("ComposedTexture3D",            new ComposedTexture3D            (executionContext));
__LOG__ << std::endl;
	add ("Cone",                         new Cone                         (executionContext));
__LOG__ << std::endl;
	add ("ConeEmitter",                  new ConeEmitter                  (executionContext));
__LOG__ << std::endl;
	add ("Contact",                      new Contact                      (executionContext));
__LOG__ << std::endl;
	add ("Contour2D",                    new Contour2D                    (executionContext));
__LOG__ << std::endl;
	add ("ContourPolyline2D",            new ContourPolyline2D            (executionContext));
__LOG__ << std::endl;
	add ("Coordinate",                   new Coordinate                   (executionContext));
__LOG__ << std::endl;
	add ("CoordinateChaser",             new CoordinateChaser             (executionContext));
__LOG__ << std::endl;
	add ("CoordinateDamper",             new CoordinateDamper             (executionContext));
__LOG__ << std::endl;
	add ("CoordinateDouble",             new CoordinateDouble             (executionContext));
__LOG__ << std::endl;
	add ("CoordinateInterpolator",       new CoordinateInterpolator       (executionContext));
__LOG__ << std::endl;
	add ("CoordinateInterpolator2D",     new CoordinateInterpolator2D     (executionContext));
__LOG__ << std::endl;
	add ("Cylinder",                     new Cylinder                     (executionContext));
__LOG__ << std::endl;
	add ("CylinderSensor",               new CylinderSensor               (executionContext));
__LOG__ << std::endl;
	add ("DISEntityManager",             new DISEntityManager             (executionContext));
__LOG__ << std::endl;
	add ("DISEntityTypeMapping",         new DISEntityTypeMapping         (executionContext));
__LOG__ << std::endl;
	add ("DirectionalLight",             new DirectionalLight             (executionContext));
__LOG__ << std::endl;
	add ("Disk2D",                       new Disk2D                       (executionContext));
__LOG__ << std::endl;
	add ("DoubleAxisHingeJoint",         new DoubleAxisHingeJoint         (executionContext));
__LOG__ << std::endl;
	add ("EaseInEaseOut",                new EaseInEaseOut                (executionContext));
__LOG__ << std::endl;
	add ("ElevationGrid",                new ElevationGrid                (executionContext));
__LOG__ << std::endl;
	add ("EspduTransform",               new EspduTransform               (executionContext));
__LOG__ << std::endl;
	add ("ExplosionEmitter",             new ExplosionEmitter             (executionContext));
__LOG__ << std::endl;
	add ("Extrusion",                    new Extrusion                    (executionContext));
__LOG__ << std::endl;
	add ("FillProperties",               new FillProperties               (executionContext));
__LOG__ << std::endl;
	add ("FloatVertexAttribute",         new FloatVertexAttribute         (executionContext));
__LOG__ << std::endl;
	add ("Fog",                          new Fog                          (executionContext));
__LOG__ << std::endl;
	add ("FogCoordinate",                new FogCoordinate                (executionContext));
__LOG__ << std::endl;
	add ("FontStyle",                    new FontStyle                    (executionContext));
__LOG__ << std::endl;
	add ("GeneratedCubeMapTexture",      new GeneratedCubeMapTexture      (executionContext));
__LOG__ << std::endl;
	add ("GeoCoordinate",                new GeoCoordinate                (executionContext));
__LOG__ << std::endl;
	add ("GeoElevationGrid",             new GeoElevationGrid             (executionContext));
__LOG__ << std::endl;
	add ("GeoLOD",                       new GeoLOD                       (executionContext));
__LOG__ << std::endl;
	add ("GeoLocation",                  new GeoLocation                  (executionContext));
__LOG__ << std::endl;
	add ("GeoMetadata",                  new GeoMetadata                  (executionContext));
__LOG__ << std::endl;
	add ("GeoOrigin",                    new GeoOrigin                    (executionContext)); // GeoOrigin node is not allowed in X3D scenes having version 3.3 or higher.
__LOG__ << std::endl;
	add ("GeoPositionInterpolator",      new GeoPositionInterpolator      (executionContext));
__LOG__ << std::endl;
	add ("GeoProximitySensor",           new GeoProximitySensor           (executionContext));
__LOG__ << std::endl;
	add ("GeoTouchSensor",               new GeoTouchSensor               (executionContext));
__LOG__ << std::endl;
	add ("GeoTransform",                 new GeoTransform                 (executionContext));
__LOG__ << std::endl;
	add ("GeoViewpoint",                 new GeoViewpoint                 (executionContext));
__LOG__ << std::endl;
	add ("ForcePhysicsModel",            new ForcePhysicsModel            (executionContext));
__LOG__ << std::endl;
	add ("Group",                        new Group                        (executionContext));
__LOG__ << std::endl;
	add ("HAnimDisplacer",               new HAnimDisplacer               (executionContext));
__LOG__ << std::endl;
	add ("HAnimHumanoid",                new HAnimHumanoid                (executionContext));
__LOG__ << std::endl;
	add ("HAnimJoint",                   new HAnimJoint                   (executionContext));
__LOG__ << std::endl;
	add ("HAnimSegment",                 new HAnimSegment                 (executionContext));
__LOG__ << std::endl;
	add ("HAnimSite",                    new HAnimSite                    (executionContext));
__LOG__ << std::endl;
	add ("ImageCubeMapTexture",          new ImageCubeMapTexture          (executionContext));
__LOG__ << std::endl;
	add ("ImageTexture",                 new ImageTexture                 (executionContext));
__LOG__ << std::endl;
	add ("ImageTexture3D",               new ImageTexture3D               (executionContext));
__LOG__ << std::endl;
	add ("IndexedFaceSet",               new IndexedFaceSet               (executionContext));
__LOG__ << std::endl;
	add ("IndexedLineSet",               new IndexedLineSet               (executionContext));
__LOG__ << std::endl;
	add ("IndexedQuadSet",               new IndexedQuadSet               (executionContext));
__LOG__ << std::endl;
	add ("IndexedTriangleFanSet",        new IndexedTriangleFanSet        (executionContext));
__LOG__ << std::endl;
	add ("IndexedTriangleSet",           new IndexedTriangleSet           (executionContext));
__LOG__ << std::endl;
	add ("IndexedTriangleStripSet",      new IndexedTriangleStripSet      (executionContext));
__LOG__ << std::endl;
	add ("Inline",                       new Inline                       (executionContext));
__LOG__ << std::endl;
	add ("IntegerSequencer",             new IntegerSequencer             (executionContext));
__LOG__ << std::endl;
	add ("IntegerTrigger",               new IntegerTrigger               (executionContext));
__LOG__ << std::endl;
	add ("KeySensor",                    new KeySensor                    (executionContext));
__LOG__ << std::endl;
	add ("LOD",                          new LOD                          (executionContext));
__LOG__ << std::endl;
	add ("Layer",                        new Layer                        (executionContext));
__LOG__ << std::endl;
	add ("LayerSet",                     new LayerSet                     (executionContext));
__LOG__ << std::endl;
	add ("Layout",                       new Layout                       (executionContext));
__LOG__ << std::endl;
	add ("LayoutGroup",                  new LayoutGroup                  (executionContext));
__LOG__ << std::endl;
	add ("LayoutLayer",                  new LayoutLayer                  (executionContext));
__LOG__ << std::endl;
	add ("LinePickSensor",               new LinePickSensor               (executionContext));
__LOG__ << std::endl;
	add ("LineProperties",               new LineProperties               (executionContext));
__LOG__ << std::endl;
	add ("LineSet",                      new LineSet                      (executionContext));
__LOG__ << std::endl;
	add ("LoadSensor",                   new LoadSensor                   (executionContext));
__LOG__ << std::endl;
	add ("LocalFog",                     new LocalFog                     (executionContext));
__LOG__ << std::endl;
	add ("Material",                     new Material                     (executionContext));
__LOG__ << std::endl;
	add ("Matrix3VertexAttribute",       new Matrix3VertexAttribute       (executionContext));
__LOG__ << std::endl;
	add ("Matrix4VertexAttribute",       new Matrix4VertexAttribute       (executionContext));
__LOG__ << std::endl;
	add ("MetadataBoolean",              new MetadataBoolean              (executionContext));
__LOG__ << std::endl;
	add ("MetadataDouble",               new MetadataDouble               (executionContext));
__LOG__ << std::endl;
	add ("MetadataFloat",                new MetadataFloat                (executionContext));
__LOG__ << std::endl;
	add ("MetadataInteger",              new MetadataInteger              (executionContext));
__LOG__ << std::endl;
	add ("MetadataSet",                  new MetadataSet                  (executionContext));
__LOG__ << std::endl;
	add ("MetadataString",               new MetadataString               (executionContext));
__LOG__ << std::endl;
	add ("MotorJoint",                   new MotorJoint                   (executionContext));
__LOG__ << std::endl;
	add ("MovieTexture",                 new MovieTexture                 (executionContext));
__LOG__ << std::endl;
	add ("MultiTexture",                 new MultiTexture                 (executionContext));
__LOG__ << std::endl;
	add ("MultiTextureCoordinate",       new MultiTextureCoordinate       (executionContext));
__LOG__ << std::endl;
	add ("MultiTextureTransform",        new MultiTextureTransform        (executionContext));
__LOG__ << std::endl;
	add ("NavigationInfo",               new NavigationInfo               (executionContext));
__LOG__ << std::endl;
	add ("Normal",                       new Normal                       (executionContext));
__LOG__ << std::endl;
	add ("NormalInterpolator",           new NormalInterpolator           (executionContext));
__LOG__ << std::endl;
	add ("NurbsCurve",                   new NurbsCurve                   (executionContext));
__LOG__ << std::endl;
	add ("NurbsCurve2D",                 new NurbsCurve2D                 (executionContext));
__LOG__ << std::endl;
	add ("NurbsOrientationInterpolator", new NurbsOrientationInterpolator (executionContext));
__LOG__ << std::endl;
	add ("NurbsPatchSurface",            new NurbsPatchSurface            (executionContext));
__LOG__ << std::endl;
	add ("NurbsPositionInterpolator",    new NurbsPositionInterpolator    (executionContext));
__LOG__ << std::endl;
	add ("NurbsSet",                     new NurbsSet                     (executionContext));
__LOG__ << std::endl;
	add ("NurbsSurfaceInterpolator",     new NurbsSurfaceInterpolator     (executionContext));
__LOG__ << std::endl;
	add ("NurbsSweptSurface",            new NurbsSweptSurface            (executionContext));
__LOG__ << std::endl;
	add ("NurbsSwungSurface",            new NurbsSwungSurface            (executionContext));
__LOG__ << std::endl;
	add ("NurbsTextureCoordinate",       new NurbsTextureCoordinate       (executionContext));
__LOG__ << std::endl;
	add ("NurbsTrimmedSurface",          new NurbsTrimmedSurface          (executionContext));
__LOG__ << std::endl;
	add ("OrientationChaser",            new OrientationChaser            (executionContext));
__LOG__ << std::endl;
	add ("OrientationDamper",            new OrientationDamper            (executionContext));
__LOG__ << std::endl;
	add ("OrientationInterpolator",      new OrientationInterpolator      (executionContext));
__LOG__ << std::endl;
	add ("OrthoViewpoint",               new OrthoViewpoint               (executionContext));
__LOG__ << std::endl;
	add ("PackagedShader",               new PackagedShader               (executionContext));
__LOG__ << std::endl;
	add ("ParticleSystem",               new ParticleSystem               (executionContext));
__LOG__ << std::endl;
	add ("PickableGroup",                new PickableGroup                (executionContext));
__LOG__ << std::endl;
	add ("PixelTexture",                 new PixelTexture                 (executionContext));
__LOG__ << std::endl;
	add ("PixelTexture3D",               new PixelTexture3D               (executionContext));
__LOG__ << std::endl;
	add ("PlaneSensor",                  new PlaneSensor                  (executionContext));
__LOG__ << std::endl;
	add ("PointEmitter",                 new PointEmitter                 (executionContext));
__LOG__ << std::endl;
	add ("PointLight",                   new PointLight                   (executionContext));
__LOG__ << std::endl;
	add ("PointPickSensor",              new PointPickSensor              (executionContext));
__LOG__ << std::endl;
	add ("PointSet",                     new PointSet                     (executionContext));
__LOG__ << std::endl;
	add ("Polyline2D",                   new Polyline2D                   (executionContext));
__LOG__ << std::endl;
	add ("PolylineEmitter",              new PolylineEmitter              (executionContext));
__LOG__ << std::endl;
	add ("Polypoint2D",                  new Polypoint2D                  (executionContext));
__LOG__ << std::endl;
	add ("PositionChaser",               new PositionChaser               (executionContext));
__LOG__ << std::endl;
	add ("PositionChaser2D",             new PositionChaser2D             (executionContext));
__LOG__ << std::endl;
	add ("PositionDamper",               new PositionDamper               (executionContext));
__LOG__ << std::endl;
	add ("PositionDamper2D",             new PositionDamper2D             (executionContext));
__LOG__ << std::endl;
	add ("PositionInterpolator",         new PositionInterpolator         (executionContext));
__LOG__ << std::endl;
	add ("PositionInterpolator2D",       new PositionInterpolator2D       (executionContext));
__LOG__ << std::endl;
	add ("PrimitivePickSensor",          new PrimitivePickSensor          (executionContext));
__LOG__ << std::endl;
	add ("ProgramShader",                new ProgramShader                (executionContext));
__LOG__ << std::endl;
	add ("ProximitySensor",              new ProximitySensor              (executionContext));
__LOG__ << std::endl;
	add ("QuadSet",                      new QuadSet                      (executionContext));
__LOG__ << std::endl;
	add ("ReceiverPdu",                  new ReceiverPdu                  (executionContext));
__LOG__ << std::endl;
	add ("Rectangle2D",                  new Rectangle2D                  (executionContext));
__LOG__ << std::endl;
	add ("RigidBody",                    new RigidBody                    (executionContext));
__LOG__ << std::endl;
	add ("RigidBodyCollection",          new RigidBodyCollection          (executionContext));
__LOG__ << std::endl;
	add ("ScalarChaser",                 new ScalarChaser                 (executionContext));
__LOG__ << std::endl;
	add ("ScalarDamper",                 new ScalarDamper                 (executionContext));
__LOG__ << std::endl;
	add ("ScalarInterpolator",           new ScalarInterpolator           (executionContext));
__LOG__ << std::endl;
	add ("ScreenFontStyle",              new ScreenFontStyle              (executionContext));
__LOG__ << std::endl;
	add ("ScreenGroup",                  new ScreenGroup                  (executionContext));
__LOG__ << std::endl;
	add ("Script",                       new Script                       (executionContext));
__LOG__ << std::endl;
	add ("ShaderPart",                   new ShaderPart                   (executionContext));
__LOG__ << std::endl;
	add ("ShaderProgram",                new ShaderProgram                (executionContext));
__LOG__ << std::endl;
	add ("Shape",                        new Shape                        (executionContext));
__LOG__ << std::endl;
	add ("SignalPdu",                    new SignalPdu                    (executionContext));
__LOG__ << std::endl;
	add ("SingleAxisHingeJoint",         new SingleAxisHingeJoint         (executionContext));
__LOG__ << std::endl;
	add ("SliderJoint",                  new SliderJoint                  (executionContext));
__LOG__ << std::endl;
	add ("Sound",                        new Sound                        (executionContext));
__LOG__ << std::endl;
	add ("Sphere",                       new Sphere                       (executionContext));
__LOG__ << std::endl;
	add ("SphereSensor",                 new SphereSensor                 (executionContext));
__LOG__ << std::endl;
	add ("SplinePositionInterpolator",   new SplinePositionInterpolator   (executionContext));
__LOG__ << std::endl;
	add ("SplinePositionInterpolator2D", new SplinePositionInterpolator2D (executionContext));
__LOG__ << std::endl;
	add ("SplineScalarInterpolator",     new SplineScalarInterpolator     (executionContext));
__LOG__ << std::endl;
	add ("SpotLight",                    new SpotLight                    (executionContext));
__LOG__ << std::endl;
	add ("SquadOrientationInterpolator", new SquadOrientationInterpolator (executionContext));
__LOG__ << std::endl;
	add ("StaticGroup",                  new StaticGroup                  (executionContext));
__LOG__ << std::endl;
	add ("StringSensor",                 new StringSensor                 (executionContext));
__LOG__ << std::endl;
	add ("SurfaceEmitter",               new SurfaceEmitter               (executionContext));
__LOG__ << std::endl;
	add ("Switch",                       new Switch                       (executionContext));
__LOG__ << std::endl;
	add ("TexCoordChaser2D",             new TexCoordChaser2D             (executionContext));
__LOG__ << std::endl;
	add ("TexCoordDamper2D",             new TexCoordDamper2D             (executionContext));
__LOG__ << std::endl;
	add ("Text",                         new Text                         (executionContext));
__LOG__ << std::endl;
	add ("TextureBackground",            new TextureBackground            (executionContext));
__LOG__ << std::endl;
	add ("TextureCoordinate",            new TextureCoordinate            (executionContext));
__LOG__ << std::endl;
	add ("TextureCoordinate3D",          new TextureCoordinate3D          (executionContext));
__LOG__ << std::endl;
	add ("TextureCoordinate4D",          new TextureCoordinate4D          (executionContext));
__LOG__ << std::endl;
	add ("TextureCoordinateGenerator",   new TextureCoordinateGenerator   (executionContext));
__LOG__ << std::endl;
	add ("TextureProperties",            new TextureProperties            (executionContext));
__LOG__ << std::endl;
	add ("TextureTransform",             new TextureTransform             (executionContext));
__LOG__ << std::endl;
	add ("TextureTransform3D",           new TextureTransform3D           (executionContext));
__LOG__ << std::endl;
	add ("TextureTransformMatrix3D",     new TextureTransformMatrix3D     (executionContext));
__LOG__ << std::endl;
	add ("TimeSensor",                   new TimeSensor                   (executionContext));
__LOG__ << std::endl;
	add ("TimeTrigger",                  new TimeTrigger                  (executionContext));
__LOG__ << std::endl;
	add ("TouchSensor",                  new TouchSensor                  (executionContext));
__LOG__ << std::endl;
	add ("Transform",                    new Transform                    (executionContext));
__LOG__ << std::endl;
	add ("TransformSensor",              new TransformSensor              (executionContext));
__LOG__ << std::endl;
	add ("TransmitterPdu",               new TransmitterPdu               (executionContext));
__LOG__ << std::endl;
	add ("TriangleFanSet",               new TriangleFanSet               (executionContext));
__LOG__ << std::endl;
	add ("TriangleSet",                  new TriangleSet                  (executionContext));
__LOG__ << std::endl;
	add ("TriangleSet2D",                new TriangleSet2D                (executionContext));
__LOG__ << std::endl;
	add ("TriangleStripSet",             new TriangleStripSet             (executionContext));
__LOG__ << std::endl;
	add ("TwoSidedMaterial",             new TwoSidedMaterial             (executionContext));
__LOG__ << std::endl;
	add ("UniversalJoint",               new UniversalJoint               (executionContext));
__LOG__ << std::endl;
	add ("Viewpoint",                    new Viewpoint                    (executionContext));
__LOG__ << std::endl;
	add ("ViewpointGroup",               new ViewpointGroup               (executionContext));
__LOG__ << std::endl;
	add ("Viewport",                     new Viewport                     (executionContext));
__LOG__ << std::endl;
	add ("VisibilitySensor",             new VisibilitySensor             (executionContext));
__LOG__ << std::endl;
	add ("VolumeEmitter",                new VolumeEmitter                (executionContext));
__LOG__ << std::endl;
	add ("VolumePickSensor",             new VolumePickSensor             (executionContext));
__LOG__ << std::endl;
	add ("WindPhysicsModel",             new WindPhysicsModel             (executionContext));
__LOG__ << std::endl;
	add ("WorldInfo",                    new WorldInfo                    (executionContext));
__LOG__ << std::endl;

	//  Non standard: Component »Titania«

	add ("TransformMatrix3D",            new TransformMatrix3D            (executionContext));
__LOG__ << std::endl;
	add ("TouchGroup",                   new TouchGroup                   (executionContext));
__LOG__ << std::endl;
	add ("PolygonOffsetGroup",           new PolygonOffsetGroup           (executionContext));
__LOG__ << std::endl;
	add ("DepthBuffer",                  new DepthBuffer                  (executionContext));
__LOG__ << std::endl;

	//std::clog << "\tDone creating node index." << std::endl;
}

void
SupportedNodes::add (const std::string & name, X3DBaseNode* const node)
{
	//std::clog << "\tAdding node type " << name << ": " << std::flush;

	node -> setName (name);

	nodes .push_back (name, node);

	//std::clog << "Done." << std::endl;
}

const X3DBaseNode*
SupportedNodes::get (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	try
	{
		return nodes .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Unknown node type '" + name + "'.");
	}
}

const BaseNodeArray &
SupportedNodes::get () const
{
	return nodes;
}

void
SupportedNodes::dispose ()
{
	for (const auto & node : nodes)
		delete node;

	nodes .clear ();
}

SupportedNodes::~SupportedNodes ()
{ }

} // X3D
} // titania
