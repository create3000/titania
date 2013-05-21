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
#include <iostream>

namespace titania {
namespace X3D {

SupportedNodes::SupportedNodes (X3DExecutionContext* const executionContext)
{
	//std::clog << "Creating node index:" << std::endl;

	add ("Anchor",                       new Anchor                       (executionContext));
	add ("Appearance",                   new Appearance                   (executionContext));
	add ("Arc2D",                        new Arc2D                        (executionContext));
	add ("ArcClose2D",                   new ArcClose2D                   (executionContext));
	add ("AudioClip",                    new AudioClip                    (executionContext));
	add ("Background",                   new Background                   (executionContext));
	add ("BallJoint",                    new BallJoint                    (executionContext));
	add ("Billboard",                    new Billboard                    (executionContext));
	add ("BooleanFilter",                new BooleanFilter                (executionContext));
	add ("BooleanSequencer",             new BooleanSequencer             (executionContext));
	add ("BooleanToggle",                new BooleanToggle                (executionContext));
	add ("BooleanTrigger",               new BooleanTrigger               (executionContext));
	add ("BoundedPhysicsModel",          new BoundedPhysicsModel          (executionContext));
	add ("Box",                          new Box                          (executionContext));
	add ("CADAssembly",                  new CADAssembly                  (executionContext));
	add ("CADFace",                      new CADFace                      (executionContext));
	add ("CADLayer",                     new CADLayer                     (executionContext));
	add ("CADPart",                      new CADPart                      (executionContext));
	add ("Circle2D",                     new Circle2D                     (executionContext));
	add ("ClipPlane",                    new ClipPlane                    (executionContext));
	add ("CollidableOffset",             new CollidableOffset             (executionContext));
	add ("CollidableShape",              new CollidableShape              (executionContext));
	add ("Collision",                    new Collision                    (executionContext));
	add ("CollisionCollection",          new CollisionCollection          (executionContext));
	add ("CollisionSensor",              new CollisionSensor              (executionContext));
	add ("CollisionSpace",               new CollisionSpace               (executionContext));
	add ("Color",                        new Color                        (executionContext));
	add ("ColorDamper",                  new ColorDamper                  (executionContext));
	add ("ColorInterpolator",            new ColorInterpolator            (executionContext));
	add ("ColorRGBA",                    new ColorRGBA                    (executionContext));
	add ("ComposedCubeMapTexture",       new ComposedCubeMapTexture       (executionContext));
	add ("ComposedShader",               new ComposedShader               (executionContext));
	add ("ComposedTexture3D",            new ComposedTexture3D            (executionContext));
	add ("Cone",                         new Cone                         (executionContext));
	add ("ConeEmitter",                  new ConeEmitter                  (executionContext));
	add ("Contact",                      new Contact                      (executionContext));
	add ("Contour2D",                    new Contour2D                    (executionContext));
	add ("ContourPolyline2D",            new ContourPolyline2D            (executionContext));
	add ("Coordinate",                   new Coordinate                   (executionContext));
	add ("CoordinateDamper",             new CoordinateDamper             (executionContext));
	add ("CoordinateDouble",             new CoordinateDouble             (executionContext));
	add ("CoordinateInterpolator",       new CoordinateInterpolator       (executionContext));
	add ("CoordinateInterpolator2D",     new CoordinateInterpolator2D     (executionContext));
	add ("Cylinder",                     new Cylinder                     (executionContext));
	add ("CylinderSensor",               new CylinderSensor               (executionContext));
	add ("DISEntityManager",             new DISEntityManager             (executionContext));
	add ("DISEntityTypeMapping",         new DISEntityTypeMapping         (executionContext));
	add ("DirectionalLight",             new DirectionalLight             (executionContext));
	add ("Disk2D",                       new Disk2D                       (executionContext));
	add ("DoubleAxisHingeJoint",         new DoubleAxisHingeJoint         (executionContext));
	add ("EaseInEaseOut",                new EaseInEaseOut                (executionContext));
	add ("ElevationGrid",                new ElevationGrid                (executionContext));
	add ("EspduTransform",               new EspduTransform               (executionContext));
	add ("ExplosionEmitter",             new ExplosionEmitter             (executionContext));
	add ("Extrusion",                    new Extrusion                    (executionContext));
	add ("FillProperties",               new FillProperties               (executionContext));
	add ("FloatVertexAttribute",         new FloatVertexAttribute         (executionContext));
	add ("Fog",                          new Fog                          (executionContext));
	add ("FogCoordinate",                new FogCoordinate                (executionContext));
	add ("FontStyle",                    new FontStyle                    (executionContext));
	add ("GeneratedCubeMapTexture",      new GeneratedCubeMapTexture      (executionContext));
	add ("GeoCoordinate",                new GeoCoordinate                (executionContext));
	add ("GeoElevationGrid",             new GeoElevationGrid             (executionContext));
	add ("GeoLOD",                       new GeoLOD                       (executionContext));
	add ("GeoLocation",                  new GeoLocation                  (executionContext));
	add ("GeoMetadata",                  new GeoMetadata                  (executionContext));
	add ("GeoOrigin",                    new GeoOrigin                    (executionContext));
	add ("GeoPositionInterpolator",      new GeoPositionInterpolator      (executionContext));
	add ("GeoProximitySensor",           new GeoProximitySensor           (executionContext));
	add ("GeoTouchSensor",               new GeoTouchSensor               (executionContext));
	add ("GeoTransform",                 new GeoTransform                 (executionContext));
	add ("GeoViewpoint",                 new GeoViewpoint                 (executionContext));
	add ("GravityPhysicsModel",          new GravityPhysicsModel          (executionContext));
	add ("Group",                        new Group                        (executionContext));
	add ("HAnimDisplacer",               new HAnimDisplacer               (executionContext));
	add ("HAnimHumanoid",                new HAnimHumanoid                (executionContext));
	add ("HAnimJoint",                   new HAnimJoint                   (executionContext));
	add ("HAnimSegment",                 new HAnimSegment                 (executionContext));
	add ("HAnimSite",                    new HAnimSite                    (executionContext));
	add ("ImageCubeMapTexture",          new ImageCubeMapTexture          (executionContext));
	add ("ImageTexture",                 new ImageTexture                 (executionContext));
	add ("ImageTexture3D",               new ImageTexture3D               (executionContext));
	add ("IndexedFaceSet",               new IndexedFaceSet               (executionContext));
	add ("IndexedLineSet",               new IndexedLineSet               (executionContext));
	add ("IndexedQuadSet",               new IndexedQuadSet               (executionContext));
	add ("IndexedTriangleFanSet",        new IndexedTriangleFanSet        (executionContext));
	add ("IndexedTriangleSet",           new IndexedTriangleSet           (executionContext));
	add ("IndexedTriangleStripSet",      new IndexedTriangleStripSet      (executionContext));
	add ("Inline",                       new Inline                       (executionContext));
	add ("IntegerSequencer",             new IntegerSequencer             (executionContext));
	add ("IntegerTrigger",               new IntegerTrigger               (executionContext));
	add ("KeySensor",                    new KeySensor                    (executionContext));
	add ("LOD",                          new LOD                          (executionContext));
	add ("Layer",                        new Layer                        (executionContext));
	add ("LayerSet",                     new LayerSet                     (executionContext));
	add ("Layout",                       new Layout                       (executionContext));
	add ("LayoutGroup",                  new LayoutGroup                  (executionContext));
	add ("LayoutLayer",                  new LayoutLayer                  (executionContext));
	add ("LinePickSensor",               new LinePickSensor               (executionContext));
	add ("LineProperties",               new LineProperties               (executionContext));
	add ("LineSet",                      new LineSet                      (executionContext));
	add ("LoadSensor",                   new LoadSensor                   (executionContext));
	add ("LocalFog",                     new LocalFog                     (executionContext));
	add ("Material",                     new Material                     (executionContext));
	add ("Matrix3VertexAttribute",       new Matrix3VertexAttribute       (executionContext));
	add ("Matrix4VertexAttribute",       new Matrix4VertexAttribute       (executionContext));
	add ("MetadataBool",                 new MetadataBool                 (executionContext));
	add ("MetadataDouble",               new MetadataDouble               (executionContext));
	add ("MetadataFloat",                new MetadataFloat                (executionContext));
	add ("MetadataInteger",              new MetadataInteger              (executionContext));
	add ("MetadataSet",                  new MetadataSet                  (executionContext));
	add ("MetadataString",               new MetadataString               (executionContext));
	add ("MotorJoint",                   new MotorJoint                   (executionContext));
	add ("MovieTexture",                 new MovieTexture                 (executionContext));
	add ("MultiTexture",                 new MultiTexture                 (executionContext));
	add ("MultiTextureCoordinate",       new MultiTextureCoordinate       (executionContext));
	add ("MultiTextureTransform",        new MultiTextureTransform        (executionContext));
	add ("NavigationInfo",               new NavigationInfo               (executionContext));
	add ("Normal",                       new Normal                       (executionContext));
	add ("NormalInterpolator",           new NormalInterpolator           (executionContext));
	add ("NurbsCurve",                   new NurbsCurve                   (executionContext));
	add ("NurbsCurve2D",                 new NurbsCurve2D                 (executionContext));
	add ("NurbsOrientationInterpolator", new NurbsOrientationInterpolator (executionContext));
	add ("NurbsPatchSurface",            new NurbsPatchSurface            (executionContext));
	add ("NurbsPositionInterpolator",    new NurbsPositionInterpolator    (executionContext));
	add ("NurbsSet",                     new NurbsSet                     (executionContext));
	add ("NurbsSurfaceInterpolator",     new NurbsSurfaceInterpolator     (executionContext));
	add ("NurbsSweptSurface",            new NurbsSweptSurface            (executionContext));
	add ("NurbsSwungSurface",            new NurbsSwungSurface            (executionContext));
	add ("NurbsTextureCoordinate",       new NurbsTextureCoordinate       (executionContext));
	add ("NurbsTrimmedSurface",          new NurbsTrimmedSurface          (executionContext));
	add ("OrientationChaser",            new OrientationChaser            (executionContext));
	add ("OrientationDamper",            new OrientationDamper            (executionContext));
	add ("OrientationInterpolator",      new OrientationInterpolator      (executionContext));
	add ("OrthoViewpoint",               new OrthoViewpoint               (executionContext));
	add ("PackagedShader",               new PackagedShader               (executionContext));
	add ("ParticleSystem",               new ParticleSystem               (executionContext));
	add ("PickableGroup",                new PickableGroup                (executionContext));
	add ("PixelTexture",                 new PixelTexture                 (executionContext));
	add ("PixelTexture3D",               new PixelTexture3D               (executionContext));
	add ("PlaneSensor",                  new PlaneSensor                  (executionContext));
	add ("PointEmitter",                 new PointEmitter                 (executionContext));
	add ("PointLight",                   new PointLight                   (executionContext));
	add ("PointPickSensor",              new PointPickSensor              (executionContext));
	add ("PointSet",                     new PointSet                     (executionContext));
	add ("Polyline2D",                   new Polyline2D                   (executionContext));
	add ("PolylineEmitter",              new PolylineEmitter              (executionContext));
	add ("Polypoint2D",                  new Polypoint2D                  (executionContext));
	add ("PositionChaser",               new PositionChaser               (executionContext));
	add ("PositionChaser2D",             new PositionChaser2D             (executionContext));
	add ("PositionDamper",               new PositionDamper               (executionContext));
	add ("PositionDamper2D",             new PositionDamper2D             (executionContext));
	add ("PositionInterpolator",         new PositionInterpolator         (executionContext));
	add ("PositionInterpolator2D",       new PositionInterpolator2D       (executionContext));
	add ("PrimitivePickSensor",          new PrimitivePickSensor          (executionContext));
	add ("ProgramShader",                new ProgramShader                (executionContext));
	add ("ProximitySensor",              new ProximitySensor              (executionContext));
	add ("QuadSet",                      new QuadSet                      (executionContext));
	add ("ReceiverPdu",                  new ReceiverPdu                  (executionContext));
	add ("Rectangle2D",                  new Rectangle2D                  (executionContext));
	add ("RigidBody",                    new RigidBody                    (executionContext));
	add ("RigidBodyCollection",          new RigidBodyCollection          (executionContext));
	add ("ScalarChaser",                 new ScalarChaser                 (executionContext));
	add ("ScalarInterpolator",           new ScalarInterpolator           (executionContext));
	add ("ScreenFontStyle",              new ScreenFontStyle              (executionContext));
	add ("ScreenGroup",                  new ScreenGroup                  (executionContext));
	add ("Script",                       new Script                       (executionContext));
	add ("ShaderPart",                   new ShaderPart                   (executionContext));
	add ("ShaderProgram",                new ShaderProgram                (executionContext));
	add ("Shape",                        new Shape                        (executionContext));
	add ("SignalPdu",                    new SignalPdu                    (executionContext));
	add ("SingleAxisHingeJoint",         new SingleAxisHingeJoint         (executionContext));
	add ("SliderJoint",                  new SliderJoint                  (executionContext));
	add ("Sound",                        new Sound                        (executionContext));
	add ("Sphere",                       new Sphere                       (executionContext));
	add ("SphereSensor",                 new SphereSensor                 (executionContext));
	add ("SplinePositionInterpolator",   new SplinePositionInterpolator   (executionContext));
	add ("SplinePositionInterpolator2D", new SplinePositionInterpolator2D (executionContext));
	add ("SplineScalarInterpolator",     new SplineScalarInterpolator     (executionContext));
	add ("SpotLight",                    new SpotLight                    (executionContext));
	add ("SquadOrientationInterpolator", new SquadOrientationInterpolator (executionContext));
	add ("StaticGroup",                  new StaticGroup                  (executionContext));
	add ("StringSensor",                 new StringSensor                 (executionContext));
	add ("SurfaceEmitter",               new SurfaceEmitter               (executionContext));
	add ("Switch",                       new Switch                       (executionContext));
	add ("TexCoordDamper2D",             new TexCoordDamper2D             (executionContext));
	add ("Text",                         new Text                         (executionContext));
	add ("TextureBackground",            new TextureBackground            (executionContext));
	add ("TextureCoordinate",            new TextureCoordinate            (executionContext));
	add ("TextureCoordinate3D",          new TextureCoordinate3D          (executionContext));
	add ("TextureCoordinate4D",          new TextureCoordinate4D          (executionContext));
	add ("TextureCoordinateGenerator",   new TextureCoordinateGenerator   (executionContext));
	add ("TextureProperties",            new TextureProperties            (executionContext));
	add ("TextureTransform",             new TextureTransform             (executionContext));
	add ("TextureTransform3D",           new TextureTransform3D           (executionContext));
	add ("TextureTransformMatrix3D",     new TextureTransformMatrix3D     (executionContext));
	add ("TimeSensor",                   new TimeSensor                   (executionContext));
	add ("TimeTrigger",                  new TimeTrigger                  (executionContext));
	add ("TouchSensor",                  new TouchSensor                  (executionContext));
	add ("Transform",                    new Transform                    (executionContext));
	add ("TransformSensor",              new TransformSensor              (executionContext));
	add ("TransmitterPdu",               new TransmitterPdu               (executionContext));
	add ("TriangleFanSet",               new TriangleFanSet               (executionContext));
	add ("TriangleSet",                  new TriangleSet                  (executionContext));
	add ("TriangleSet2D",                new TriangleSet2D                (executionContext));
	add ("TriangleStripSet",             new TriangleStripSet             (executionContext));
	add ("TwoSidedMaterial",             new TwoSidedMaterial             (executionContext));
	add ("UniversalJoint",               new UniversalJoint               (executionContext));
	add ("Viewpoint",                    new Viewpoint                    (executionContext));
	add ("ViewpointGroup",               new ViewpointGroup               (executionContext));
	add ("Viewport",                     new Viewport                     (executionContext));
	add ("VisibilitySensor",             new VisibilitySensor             (executionContext));
	add ("VolumeEmitter",                new VolumeEmitter                (executionContext));
	add ("VolumePickSensor",             new VolumePickSensor             (executionContext));
	add ("WindPhysicsModel",             new WindPhysicsModel             (executionContext));
	add ("WorldInfo",                    new WorldInfo                    (executionContext));

	//std::clog << "\tDone creating node index." << std::endl;
}

void
SupportedNodes::add (const std::string & name, X3DBaseNode* node)
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

const BasicNodeArray &
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
