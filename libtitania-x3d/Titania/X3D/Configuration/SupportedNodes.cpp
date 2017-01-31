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
	executionContext (executionContext),
	       functions (),
	           nodes ()
{
	//std::clog << "Creating node index:" << std::endl;
	
	// Specification version 3.2
	
	add ("MetadataBool",                 [ ] (X3DExecutionContext* const executionContext) { return new MetadataBoolean              (executionContext); });

	// Specification version 3.3
	
	add ("Anchor",                       [ ] (X3DExecutionContext* const executionContext) { return new Anchor                       (executionContext); });
	add ("Appearance",                   [ ] (X3DExecutionContext* const executionContext) { return new Appearance                   (executionContext); });
	add ("Arc2D",                        [ ] (X3DExecutionContext* const executionContext) { return new Arc2D                        (executionContext); });
	add ("ArcClose2D",                   [ ] (X3DExecutionContext* const executionContext) { return new ArcClose2D                   (executionContext); });
	add ("AudioClip",                    [ ] (X3DExecutionContext* const executionContext) { return new AudioClip                    (executionContext); });
	add ("Background",                   [ ] (X3DExecutionContext* const executionContext) { return new Background                   (executionContext); });
	add ("BallJoint",                    [ ] (X3DExecutionContext* const executionContext) { return new BallJoint                    (executionContext); });
	add ("Billboard",                    [ ] (X3DExecutionContext* const executionContext) { return new Billboard                    (executionContext); });
	add ("BooleanFilter",                [ ] (X3DExecutionContext* const executionContext) { return new BooleanFilter                (executionContext); });
	add ("BooleanSequencer",             [ ] (X3DExecutionContext* const executionContext) { return new BooleanSequencer             (executionContext); });
	add ("BooleanToggle",                [ ] (X3DExecutionContext* const executionContext) { return new BooleanToggle                (executionContext); });
	add ("BooleanTrigger",               [ ] (X3DExecutionContext* const executionContext) { return new BooleanTrigger               (executionContext); });
	add ("BoundedPhysicsModel",          [ ] (X3DExecutionContext* const executionContext) { return new BoundedPhysicsModel          (executionContext); });
	add ("Box",                          [ ] (X3DExecutionContext* const executionContext) { return new Box                          (executionContext); });
	add ("CADAssembly",                  [ ] (X3DExecutionContext* const executionContext) { return new CADAssembly                  (executionContext); });
	add ("CADFace",                      [ ] (X3DExecutionContext* const executionContext) { return new CADFace                      (executionContext); });
	add ("CADLayer",                     [ ] (X3DExecutionContext* const executionContext) { return new CADLayer                     (executionContext); });
	add ("CADPart",                      [ ] (X3DExecutionContext* const executionContext) { return new CADPart                      (executionContext); });
	add ("Circle2D",                     [ ] (X3DExecutionContext* const executionContext) { return new Circle2D                     (executionContext); });
	add ("ClipPlane",                    [ ] (X3DExecutionContext* const executionContext) { return new ClipPlane                    (executionContext); });
	add ("CollidableOffset",             [ ] (X3DExecutionContext* const executionContext) { return new CollidableOffset             (executionContext); });
	add ("CollidableShape",              [ ] (X3DExecutionContext* const executionContext) { return new CollidableShape              (executionContext); });
	add ("Collision",                    [ ] (X3DExecutionContext* const executionContext) { return new Collision                    (executionContext); });
	add ("CollisionCollection",          [ ] (X3DExecutionContext* const executionContext) { return new CollisionCollection          (executionContext); });
	add ("CollisionSensor",              [ ] (X3DExecutionContext* const executionContext) { return new CollisionSensor              (executionContext); });
	add ("CollisionSpace",               [ ] (X3DExecutionContext* const executionContext) { return new CollisionSpace               (executionContext); });
	add ("Color",                        [ ] (X3DExecutionContext* const executionContext) { return new Color                        (executionContext); });
	add ("ColorChaser",                  [ ] (X3DExecutionContext* const executionContext) { return new ColorChaser                  (executionContext); });
	add ("ColorDamper",                  [ ] (X3DExecutionContext* const executionContext) { return new ColorDamper                  (executionContext); });
	add ("ColorInterpolator",            [ ] (X3DExecutionContext* const executionContext) { return new ColorInterpolator            (executionContext); });
	add ("ColorRGBA",                    [ ] (X3DExecutionContext* const executionContext) { return new ColorRGBA                    (executionContext); });
	add ("ComposedCubeMapTexture",       [ ] (X3DExecutionContext* const executionContext) { return new ComposedCubeMapTexture       (executionContext); });
	add ("ComposedShader",               [ ] (X3DExecutionContext* const executionContext) { return new ComposedShader               (executionContext); });
	add ("ComposedTexture3D",            [ ] (X3DExecutionContext* const executionContext) { return new ComposedTexture3D            (executionContext); });
	add ("Cone",                         [ ] (X3DExecutionContext* const executionContext) { return new Cone                         (executionContext); });
	add ("ConeEmitter",                  [ ] (X3DExecutionContext* const executionContext) { return new ConeEmitter                  (executionContext); });
	add ("Contact",                      [ ] (X3DExecutionContext* const executionContext) { return new Contact                      (executionContext); });
	add ("Contour2D",                    [ ] (X3DExecutionContext* const executionContext) { return new Contour2D                    (executionContext); });
	add ("ContourPolyline2D",            [ ] (X3DExecutionContext* const executionContext) { return new ContourPolyline2D            (executionContext); });
	add ("Coordinate",                   [ ] (X3DExecutionContext* const executionContext) { return new Coordinate                   (executionContext); });
	add ("CoordinateChaser",             [ ] (X3DExecutionContext* const executionContext) { return new CoordinateChaser             (executionContext); });
	add ("CoordinateDamper",             [ ] (X3DExecutionContext* const executionContext) { return new CoordinateDamper             (executionContext); });
	add ("CoordinateDouble",             [ ] (X3DExecutionContext* const executionContext) { return new CoordinateDouble             (executionContext); });
	add ("CoordinateInterpolator",       [ ] (X3DExecutionContext* const executionContext) { return new CoordinateInterpolator       (executionContext); });
	add ("CoordinateInterpolator2D",     [ ] (X3DExecutionContext* const executionContext) { return new CoordinateInterpolator2D     (executionContext); });
	add ("Cylinder",                     [ ] (X3DExecutionContext* const executionContext) { return new Cylinder                     (executionContext); });
	add ("CylinderSensor",               [ ] (X3DExecutionContext* const executionContext) { return new CylinderSensor               (executionContext); });
	add ("DISEntityManager",             [ ] (X3DExecutionContext* const executionContext) { return new DISEntityManager             (executionContext); });
	add ("DISEntityTypeMapping",         [ ] (X3DExecutionContext* const executionContext) { return new DISEntityTypeMapping         (executionContext); });
	add ("DirectionalLight",             [ ] (X3DExecutionContext* const executionContext) { return new DirectionalLight             (executionContext); });
	add ("Disk2D",                       [ ] (X3DExecutionContext* const executionContext) { return new Disk2D                       (executionContext); });
	add ("DoubleAxisHingeJoint",         [ ] (X3DExecutionContext* const executionContext) { return new DoubleAxisHingeJoint         (executionContext); });
	add ("EaseInEaseOut",                [ ] (X3DExecutionContext* const executionContext) { return new EaseInEaseOut                (executionContext); });
	add ("ElevationGrid",                [ ] (X3DExecutionContext* const executionContext) { return new ElevationGrid                (executionContext); });
	add ("EspduTransform",               [ ] (X3DExecutionContext* const executionContext) { return new EspduTransform               (executionContext); });
	add ("ExplosionEmitter",             [ ] (X3DExecutionContext* const executionContext) { return new ExplosionEmitter             (executionContext); });
	add ("Extrusion",                    [ ] (X3DExecutionContext* const executionContext) { return new Extrusion                    (executionContext); });
	add ("FillProperties",               [ ] (X3DExecutionContext* const executionContext) { return new FillProperties               (executionContext); });
	add ("FloatVertexAttribute",         [ ] (X3DExecutionContext* const executionContext) { return new FloatVertexAttribute         (executionContext); });
	add ("Fog",                          [ ] (X3DExecutionContext* const executionContext) { return new Fog                          (executionContext); });
	add ("FogCoordinate",                [ ] (X3DExecutionContext* const executionContext) { return new FogCoordinate                (executionContext); });
	add ("FontStyle",                    [ ] (X3DExecutionContext* const executionContext) { return new FontStyle                    (executionContext); });
	add ("GeneratedCubeMapTexture",      [ ] (X3DExecutionContext* const executionContext) { return new GeneratedCubeMapTexture      (executionContext); });
	add ("GeoCoordinate",                [ ] (X3DExecutionContext* const executionContext) { return new GeoCoordinate                (executionContext); });
	add ("GeoElevationGrid",             [ ] (X3DExecutionContext* const executionContext) { return new GeoElevationGrid             (executionContext); });
	add ("GeoLOD",                       [ ] (X3DExecutionContext* const executionContext) { return new GeoLOD                       (executionContext); });
	add ("GeoLocation",                  [ ] (X3DExecutionContext* const executionContext) { return new GeoLocation                  (executionContext); });
	add ("GeoMetadata",                  [ ] (X3DExecutionContext* const executionContext) { return new GeoMetadata                  (executionContext); });
	add ("GeoOrigin",                    [ ] (X3DExecutionContext* const executionContext) { return new GeoOrigin                    (executionContext); }); // GeoOrigin node is not allowed in X3D scenes having version 3.3 or higher.
	add ("GeoPositionInterpolator",      [ ] (X3DExecutionContext* const executionContext) { return new GeoPositionInterpolator      (executionContext); });
	add ("GeoProximitySensor",           [ ] (X3DExecutionContext* const executionContext) { return new GeoProximitySensor           (executionContext); });
	add ("GeoTouchSensor",               [ ] (X3DExecutionContext* const executionContext) { return new GeoTouchSensor               (executionContext); });
	add ("GeoTransform",                 [ ] (X3DExecutionContext* const executionContext) { return new GeoTransform                 (executionContext); });
	add ("GeoViewpoint",                 [ ] (X3DExecutionContext* const executionContext) { return new GeoViewpoint                 (executionContext); });
	add ("ForcePhysicsModel",            [ ] (X3DExecutionContext* const executionContext) { return new ForcePhysicsModel            (executionContext); });
	add ("Group",                        [ ] (X3DExecutionContext* const executionContext) { return new Group                        (executionContext); });
	add ("HAnimDisplacer",               [ ] (X3DExecutionContext* const executionContext) { return new HAnimDisplacer               (executionContext); });
	add ("HAnimHumanoid",                [ ] (X3DExecutionContext* const executionContext) { return new HAnimHumanoid                (executionContext); });
	add ("HAnimJoint",                   [ ] (X3DExecutionContext* const executionContext) { return new HAnimJoint                   (executionContext); });
	add ("HAnimSegment",                 [ ] (X3DExecutionContext* const executionContext) { return new HAnimSegment                 (executionContext); });
	add ("HAnimSite",                    [ ] (X3DExecutionContext* const executionContext) { return new HAnimSite                    (executionContext); });
	add ("ImageCubeMapTexture",          [ ] (X3DExecutionContext* const executionContext) { return new ImageCubeMapTexture          (executionContext); });
	add ("ImageTexture",                 [ ] (X3DExecutionContext* const executionContext) { return new ImageTexture                 (executionContext); });
	add ("ImageTexture3D",               [ ] (X3DExecutionContext* const executionContext) { return new ImageTexture3D               (executionContext); });
	add ("IndexedFaceSet",               [ ] (X3DExecutionContext* const executionContext) { return new IndexedFaceSet               (executionContext); });
	add ("IndexedLineSet",               [ ] (X3DExecutionContext* const executionContext) { return new IndexedLineSet               (executionContext); });
	add ("IndexedQuadSet",               [ ] (X3DExecutionContext* const executionContext) { return new IndexedQuadSet               (executionContext); });
	add ("IndexedTriangleFanSet",        [ ] (X3DExecutionContext* const executionContext) { return new IndexedTriangleFanSet        (executionContext); });
	add ("IndexedTriangleSet",           [ ] (X3DExecutionContext* const executionContext) { return new IndexedTriangleSet           (executionContext); });
	add ("IndexedTriangleStripSet",      [ ] (X3DExecutionContext* const executionContext) { return new IndexedTriangleStripSet      (executionContext); });
	add ("Inline",                       [ ] (X3DExecutionContext* const executionContext) { return new Inline                       (executionContext); });
	add ("IntegerSequencer",             [ ] (X3DExecutionContext* const executionContext) { return new IntegerSequencer             (executionContext); });
	add ("IntegerTrigger",               [ ] (X3DExecutionContext* const executionContext) { return new IntegerTrigger               (executionContext); });
	add ("KeySensor",                    [ ] (X3DExecutionContext* const executionContext) { return new KeySensor                    (executionContext); });
	add ("LOD",                          [ ] (X3DExecutionContext* const executionContext) { return new LOD                          (executionContext); });
	add ("Layer",                        [ ] (X3DExecutionContext* const executionContext) { return new Layer                        (executionContext); });
	add ("LayerSet",                     [ ] (X3DExecutionContext* const executionContext) { return new LayerSet                     (executionContext); });
	add ("Layout",                       [ ] (X3DExecutionContext* const executionContext) { return new Layout                       (executionContext); });
	add ("LayoutGroup",                  [ ] (X3DExecutionContext* const executionContext) { return new LayoutGroup                  (executionContext); });
	add ("LayoutLayer",                  [ ] (X3DExecutionContext* const executionContext) { return new LayoutLayer                  (executionContext); });
	add ("LinePickSensor",               [ ] (X3DExecutionContext* const executionContext) { return new LinePickSensor               (executionContext); });
	add ("LineProperties",               [ ] (X3DExecutionContext* const executionContext) { return new LineProperties               (executionContext); });
	add ("LineSet",                      [ ] (X3DExecutionContext* const executionContext) { return new LineSet                      (executionContext); });
	add ("LoadSensor",                   [ ] (X3DExecutionContext* const executionContext) { return new LoadSensor                   (executionContext); });
	add ("LocalFog",                     [ ] (X3DExecutionContext* const executionContext) { return new LocalFog                     (executionContext); });
	add ("Material",                     [ ] (X3DExecutionContext* const executionContext) { return new Material                     (executionContext); });
	add ("Matrix3VertexAttribute",       [ ] (X3DExecutionContext* const executionContext) { return new Matrix3VertexAttribute       (executionContext); });
	add ("Matrix4VertexAttribute",       [ ] (X3DExecutionContext* const executionContext) { return new Matrix4VertexAttribute       (executionContext); });
	add ("MetadataBoolean",              [ ] (X3DExecutionContext* const executionContext) { return new MetadataBoolean              (executionContext); });
	add ("MetadataDouble",               [ ] (X3DExecutionContext* const executionContext) { return new MetadataDouble               (executionContext); });
	add ("MetadataFloat",                [ ] (X3DExecutionContext* const executionContext) { return new MetadataFloat                (executionContext); });
	add ("MetadataInteger",              [ ] (X3DExecutionContext* const executionContext) { return new MetadataInteger              (executionContext); });
	add ("MetadataSet",                  [ ] (X3DExecutionContext* const executionContext) { return new MetadataSet                  (executionContext); });
	add ("MetadataString",               [ ] (X3DExecutionContext* const executionContext) { return new MetadataString               (executionContext); });
	add ("MotorJoint",                   [ ] (X3DExecutionContext* const executionContext) { return new MotorJoint                   (executionContext); });
	add ("MovieTexture",                 [ ] (X3DExecutionContext* const executionContext) { return new MovieTexture                 (executionContext); });
	add ("MultiTexture",                 [ ] (X3DExecutionContext* const executionContext) { return new MultiTexture                 (executionContext); });
	add ("MultiTextureCoordinate",       [ ] (X3DExecutionContext* const executionContext) { return new MultiTextureCoordinate       (executionContext); });
	add ("MultiTextureTransform",        [ ] (X3DExecutionContext* const executionContext) { return new MultiTextureTransform        (executionContext); });
	add ("NavigationInfo",               [ ] (X3DExecutionContext* const executionContext) { return new NavigationInfo               (executionContext); });
	add ("Normal",                       [ ] (X3DExecutionContext* const executionContext) { return new Normal                       (executionContext); });
	add ("NormalInterpolator",           [ ] (X3DExecutionContext* const executionContext) { return new NormalInterpolator           (executionContext); });
	add ("NurbsCurve",                   [ ] (X3DExecutionContext* const executionContext) { return new NurbsCurve                   (executionContext); });
	add ("NurbsCurve2D",                 [ ] (X3DExecutionContext* const executionContext) { return new NurbsCurve2D                 (executionContext); });
	add ("NurbsOrientationInterpolator", [ ] (X3DExecutionContext* const executionContext) { return new NurbsOrientationInterpolator (executionContext); });
	add ("NurbsPatchSurface",            [ ] (X3DExecutionContext* const executionContext) { return new NurbsPatchSurface            (executionContext); });
	add ("NurbsPositionInterpolator",    [ ] (X3DExecutionContext* const executionContext) { return new NurbsPositionInterpolator    (executionContext); });
	add ("NurbsSet",                     [ ] (X3DExecutionContext* const executionContext) { return new NurbsSet                     (executionContext); });
	add ("NurbsSurfaceInterpolator",     [ ] (X3DExecutionContext* const executionContext) { return new NurbsSurfaceInterpolator     (executionContext); });
	add ("NurbsSweptSurface",            [ ] (X3DExecutionContext* const executionContext) { return new NurbsSweptSurface            (executionContext); });
	add ("NurbsSwungSurface",            [ ] (X3DExecutionContext* const executionContext) { return new NurbsSwungSurface            (executionContext); });
	add ("NurbsTextureCoordinate",       [ ] (X3DExecutionContext* const executionContext) { return new NurbsTextureCoordinate       (executionContext); });
	add ("NurbsTrimmedSurface",          [ ] (X3DExecutionContext* const executionContext) { return new NurbsTrimmedSurface          (executionContext); });
	add ("OrientationChaser",            [ ] (X3DExecutionContext* const executionContext) { return new OrientationChaser            (executionContext); });
	add ("OrientationDamper",            [ ] (X3DExecutionContext* const executionContext) { return new OrientationDamper            (executionContext); });
	add ("OrientationInterpolator",      [ ] (X3DExecutionContext* const executionContext) { return new OrientationInterpolator      (executionContext); });
	add ("OrthoViewpoint",               [ ] (X3DExecutionContext* const executionContext) { return new OrthoViewpoint               (executionContext); });
	add ("PackagedShader",               [ ] (X3DExecutionContext* const executionContext) { return new PackagedShader               (executionContext); });
	add ("ParticleSystem",               [ ] (X3DExecutionContext* const executionContext) { return new ParticleSystem               (executionContext); });
	add ("PickableGroup",                [ ] (X3DExecutionContext* const executionContext) { return new PickableGroup                (executionContext); });
	add ("PixelTexture",                 [ ] (X3DExecutionContext* const executionContext) { return new PixelTexture                 (executionContext); });
	add ("PixelTexture3D",               [ ] (X3DExecutionContext* const executionContext) { return new PixelTexture3D               (executionContext); });
	add ("PlaneSensor",                  [ ] (X3DExecutionContext* const executionContext) { return new PlaneSensor                  (executionContext); });
	add ("PointEmitter",                 [ ] (X3DExecutionContext* const executionContext) { return new PointEmitter                 (executionContext); });
	add ("PointLight",                   [ ] (X3DExecutionContext* const executionContext) { return new PointLight                   (executionContext); });
	add ("PointPickSensor",              [ ] (X3DExecutionContext* const executionContext) { return new PointPickSensor              (executionContext); });
	add ("PointSet",                     [ ] (X3DExecutionContext* const executionContext) { return new PointSet                     (executionContext); });
	add ("Polyline2D",                   [ ] (X3DExecutionContext* const executionContext) { return new Polyline2D                   (executionContext); });
	add ("PolylineEmitter",              [ ] (X3DExecutionContext* const executionContext) { return new PolylineEmitter              (executionContext); });
	add ("Polypoint2D",                  [ ] (X3DExecutionContext* const executionContext) { return new Polypoint2D                  (executionContext); });
	add ("PositionChaser",               [ ] (X3DExecutionContext* const executionContext) { return new PositionChaser               (executionContext); });
	add ("PositionChaser2D",             [ ] (X3DExecutionContext* const executionContext) { return new PositionChaser2D             (executionContext); });
	add ("PositionDamper",               [ ] (X3DExecutionContext* const executionContext) { return new PositionDamper               (executionContext); });
	add ("PositionDamper2D",             [ ] (X3DExecutionContext* const executionContext) { return new PositionDamper2D             (executionContext); });
	add ("PositionInterpolator",         [ ] (X3DExecutionContext* const executionContext) { return new PositionInterpolator         (executionContext); });
	add ("PositionInterpolator2D",       [ ] (X3DExecutionContext* const executionContext) { return new PositionInterpolator2D       (executionContext); });
	add ("PrimitivePickSensor",          [ ] (X3DExecutionContext* const executionContext) { return new PrimitivePickSensor          (executionContext); });
	add ("ProgramShader",                [ ] (X3DExecutionContext* const executionContext) { return new ProgramShader                (executionContext); });
	add ("ProximitySensor",              [ ] (X3DExecutionContext* const executionContext) { return new ProximitySensor              (executionContext); });
	add ("QuadSet",                      [ ] (X3DExecutionContext* const executionContext) { return new QuadSet                      (executionContext); });
	add ("ReceiverPdu",                  [ ] (X3DExecutionContext* const executionContext) { return new ReceiverPdu                  (executionContext); });
	add ("Rectangle2D",                  [ ] (X3DExecutionContext* const executionContext) { return new Rectangle2D                  (executionContext); });
	add ("RigidBody",                    [ ] (X3DExecutionContext* const executionContext) { return new RigidBody                    (executionContext); });
	add ("RigidBodyCollection",          [ ] (X3DExecutionContext* const executionContext) { return new RigidBodyCollection          (executionContext); });
	add ("ScalarChaser",                 [ ] (X3DExecutionContext* const executionContext) { return new ScalarChaser                 (executionContext); });
	add ("ScalarDamper",                 [ ] (X3DExecutionContext* const executionContext) { return new ScalarDamper                 (executionContext); });
	add ("ScalarInterpolator",           [ ] (X3DExecutionContext* const executionContext) { return new ScalarInterpolator           (executionContext); });
	add ("ScreenFontStyle",              [ ] (X3DExecutionContext* const executionContext) { return new ScreenFontStyle              (executionContext); });
	add ("ScreenGroup",                  [ ] (X3DExecutionContext* const executionContext) { return new ScreenGroup                  (executionContext); });
	add ("Script",                       [ ] (X3DExecutionContext* const executionContext) { return new Script                       (executionContext); });
	add ("ShaderPart",                   [ ] (X3DExecutionContext* const executionContext) { return new ShaderPart                   (executionContext); });
	add ("ShaderProgram",                [ ] (X3DExecutionContext* const executionContext) { return new ShaderProgram                (executionContext); });
	add ("Shape",                        [ ] (X3DExecutionContext* const executionContext) { return new Shape                        (executionContext); });
	add ("SignalPdu",                    [ ] (X3DExecutionContext* const executionContext) { return new SignalPdu                    (executionContext); });
	add ("SingleAxisHingeJoint",         [ ] (X3DExecutionContext* const executionContext) { return new SingleAxisHingeJoint         (executionContext); });
	add ("SliderJoint",                  [ ] (X3DExecutionContext* const executionContext) { return new SliderJoint                  (executionContext); });
	add ("Sound",                        [ ] (X3DExecutionContext* const executionContext) { return new Sound                        (executionContext); });
	add ("Sphere",                       [ ] (X3DExecutionContext* const executionContext) { return new Sphere                       (executionContext); });
	add ("SphereSensor",                 [ ] (X3DExecutionContext* const executionContext) { return new SphereSensor                 (executionContext); });
	add ("SplinePositionInterpolator",   [ ] (X3DExecutionContext* const executionContext) { return new SplinePositionInterpolator   (executionContext); });
	add ("SplinePositionInterpolator2D", [ ] (X3DExecutionContext* const executionContext) { return new SplinePositionInterpolator2D (executionContext); });
	add ("SplineScalarInterpolator",     [ ] (X3DExecutionContext* const executionContext) { return new SplineScalarInterpolator     (executionContext); });
	add ("SpotLight",                    [ ] (X3DExecutionContext* const executionContext) { return new SpotLight                    (executionContext); });
	add ("SquadOrientationInterpolator", [ ] (X3DExecutionContext* const executionContext) { return new SquadOrientationInterpolator (executionContext); });
	add ("StaticGroup",                  [ ] (X3DExecutionContext* const executionContext) { return new StaticGroup                  (executionContext); });
	add ("StringSensor",                 [ ] (X3DExecutionContext* const executionContext) { return new StringSensor                 (executionContext); });
	add ("SurfaceEmitter",               [ ] (X3DExecutionContext* const executionContext) { return new SurfaceEmitter               (executionContext); });
	add ("Switch",                       [ ] (X3DExecutionContext* const executionContext) { return new Switch                       (executionContext); });
	add ("TexCoordChaser2D",             [ ] (X3DExecutionContext* const executionContext) { return new TexCoordChaser2D             (executionContext); });
	add ("TexCoordDamper2D",             [ ] (X3DExecutionContext* const executionContext) { return new TexCoordDamper2D             (executionContext); });
	add ("Text",                         [ ] (X3DExecutionContext* const executionContext) { return new Text                         (executionContext); });
	add ("TextureBackground",            [ ] (X3DExecutionContext* const executionContext) { return new TextureBackground            (executionContext); });
	add ("TextureCoordinate",            [ ] (X3DExecutionContext* const executionContext) { return new TextureCoordinate            (executionContext); });
	add ("TextureCoordinate3D",          [ ] (X3DExecutionContext* const executionContext) { return new TextureCoordinate3D          (executionContext); });
	add ("TextureCoordinate4D",          [ ] (X3DExecutionContext* const executionContext) { return new TextureCoordinate4D          (executionContext); });
	add ("TextureCoordinateGenerator",   [ ] (X3DExecutionContext* const executionContext) { return new TextureCoordinateGenerator   (executionContext); });
	add ("TextureProperties",            [ ] (X3DExecutionContext* const executionContext) { return new TextureProperties            (executionContext); });
	add ("TextureTransform",             [ ] (X3DExecutionContext* const executionContext) { return new TextureTransform             (executionContext); });
	add ("TextureTransform3D",           [ ] (X3DExecutionContext* const executionContext) { return new TextureTransform3D           (executionContext); });
	add ("TextureTransformMatrix3D",     [ ] (X3DExecutionContext* const executionContext) { return new TextureTransformMatrix3D     (executionContext); });
	add ("TimeSensor",                   [ ] (X3DExecutionContext* const executionContext) { return new TimeSensor                   (executionContext); });
	add ("TimeTrigger",                  [ ] (X3DExecutionContext* const executionContext) { return new TimeTrigger                  (executionContext); });
	add ("TouchSensor",                  [ ] (X3DExecutionContext* const executionContext) { return new TouchSensor                  (executionContext); });
	add ("Transform",                    [ ] (X3DExecutionContext* const executionContext) { return new Transform                    (executionContext); });
	add ("TransformSensor",              [ ] (X3DExecutionContext* const executionContext) { return new TransformSensor              (executionContext); });
	add ("TransmitterPdu",               [ ] (X3DExecutionContext* const executionContext) { return new TransmitterPdu               (executionContext); });
	add ("TriangleFanSet",               [ ] (X3DExecutionContext* const executionContext) { return new TriangleFanSet               (executionContext); });
	add ("TriangleSet",                  [ ] (X3DExecutionContext* const executionContext) { return new TriangleSet                  (executionContext); });
	add ("TriangleSet2D",                [ ] (X3DExecutionContext* const executionContext) { return new TriangleSet2D                (executionContext); });
	add ("TriangleStripSet",             [ ] (X3DExecutionContext* const executionContext) { return new TriangleStripSet             (executionContext); });
	add ("TwoSidedMaterial",             [ ] (X3DExecutionContext* const executionContext) { return new TwoSidedMaterial             (executionContext); });
	add ("UniversalJoint",               [ ] (X3DExecutionContext* const executionContext) { return new UniversalJoint               (executionContext); });
	add ("Viewpoint",                    [ ] (X3DExecutionContext* const executionContext) { return new Viewpoint                    (executionContext); });
	add ("ViewpointGroup",               [ ] (X3DExecutionContext* const executionContext) { return new ViewpointGroup               (executionContext); });
	add ("Viewport",                     [ ] (X3DExecutionContext* const executionContext) { return new Viewport                     (executionContext); });
	add ("VisibilitySensor",             [ ] (X3DExecutionContext* const executionContext) { return new VisibilitySensor             (executionContext); });
	add ("VolumeEmitter",                [ ] (X3DExecutionContext* const executionContext) { return new VolumeEmitter                (executionContext); });
	add ("VolumePickSensor",             [ ] (X3DExecutionContext* const executionContext) { return new VolumePickSensor             (executionContext); });
	add ("WindPhysicsModel",             [ ] (X3DExecutionContext* const executionContext) { return new WindPhysicsModel             (executionContext); });
	add ("WorldInfo",                    [ ] (X3DExecutionContext* const executionContext) { return new WorldInfo                    (executionContext); });

	//  Non standard: Component »Titania«

	add ("TransformMatrix3D",            [ ] (X3DExecutionContext* const executionContext) { return new TransformMatrix3D            (executionContext); });
	add ("TouchGroup",                   [ ] (X3DExecutionContext* const executionContext) { return new TouchGroup                   (executionContext); });
	add ("PolygonOffsetGroup",           [ ] (X3DExecutionContext* const executionContext) { return new PolygonOffsetGroup           (executionContext); });
	add ("DepthBuffer",                  [ ] (X3DExecutionContext* const executionContext) { return new DepthBuffer                  (executionContext); });

	//std::clog << "\tDone creating node index." << std::endl;
}

void
SupportedNodes::add (const std::string & typeName, const Function & function)
{
	//std::clog << "\tAdding node type " << name << ": " << std::flush;

	functions .emplace (typeName, function);

	//std::clog << "Done." << std::endl;
}

const X3DBaseNode*
SupportedNodes::get (const std::string & typeName) const
throw (Error <INVALID_NAME>)
{
	try
	{
		return nodes .at (typeName);
	}
	catch (const std::out_of_range &)
	{
		try
		{
			const auto function = functions .at (typeName);
			const auto node     = function (executionContext);

			const_cast <SupportedNodes*> (this) -> nodes .emplace (typeName, node);

			return node;
		}
		catch (const std::out_of_range &)
		{
			throw Error <INVALID_NAME> ("Unknown node type '" + typeName + "'.");
		}
	}
}

const BaseNodeArray &
SupportedNodes::get () const
{
	for (const auto & pair : functions)
	{
		auto & node = const_cast <SupportedNodes*> (this) -> nodes [pair .first];

		if (not node)
			node = pair .second (executionContext);
	}

	return nodes;
}

void
SupportedNodes::dispose ()
{
	for (const auto & pair : nodes)
		delete pair .second;

	nodes .clear ();
}

SupportedNodes::~SupportedNodes ()
{ }

} // X3D
} // titania
