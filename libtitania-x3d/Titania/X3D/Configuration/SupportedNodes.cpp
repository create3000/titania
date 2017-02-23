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
	
	addNode ("MetadataBool",                 [ ] (X3DExecutionContext* const executionContext) { return new MetadataBoolean              (executionContext); });

	// Specification version 3.3
	
	addNode ("Anchor",                       [ ] (X3DExecutionContext* const executionContext) { return new Anchor                       (executionContext); });
	addNode ("Appearance",                   [ ] (X3DExecutionContext* const executionContext) { return new Appearance                   (executionContext); });
	addNode ("Arc2D",                        [ ] (X3DExecutionContext* const executionContext) { return new Arc2D                        (executionContext); });
	addNode ("ArcClose2D",                   [ ] (X3DExecutionContext* const executionContext) { return new ArcClose2D                   (executionContext); });
	addNode ("AudioClip",                    [ ] (X3DExecutionContext* const executionContext) { return new AudioClip                    (executionContext); });
	addNode ("Background",                   [ ] (X3DExecutionContext* const executionContext) { return new Background                   (executionContext); });
	addNode ("BallJoint",                    [ ] (X3DExecutionContext* const executionContext) { return new BallJoint                    (executionContext); });
	addNode ("Billboard",                    [ ] (X3DExecutionContext* const executionContext) { return new Billboard                    (executionContext); });
	addNode ("BooleanFilter",                [ ] (X3DExecutionContext* const executionContext) { return new BooleanFilter                (executionContext); });
	addNode ("BooleanSequencer",             [ ] (X3DExecutionContext* const executionContext) { return new BooleanSequencer             (executionContext); });
	addNode ("BooleanToggle",                [ ] (X3DExecutionContext* const executionContext) { return new BooleanToggle                (executionContext); });
	addNode ("BooleanTrigger",               [ ] (X3DExecutionContext* const executionContext) { return new BooleanTrigger               (executionContext); });
	addNode ("BoundedPhysicsModel",          [ ] (X3DExecutionContext* const executionContext) { return new BoundedPhysicsModel          (executionContext); });
	addNode ("Box",                          [ ] (X3DExecutionContext* const executionContext) { return new Box                          (executionContext); });
	addNode ("CADAssembly",                  [ ] (X3DExecutionContext* const executionContext) { return new CADAssembly                  (executionContext); });
	addNode ("CADFace",                      [ ] (X3DExecutionContext* const executionContext) { return new CADFace                      (executionContext); });
	addNode ("CADLayer",                     [ ] (X3DExecutionContext* const executionContext) { return new CADLayer                     (executionContext); });
	addNode ("CADPart",                      [ ] (X3DExecutionContext* const executionContext) { return new CADPart                      (executionContext); });
	addNode ("Circle2D",                     [ ] (X3DExecutionContext* const executionContext) { return new Circle2D                     (executionContext); });
	addNode ("ClipPlane",                    [ ] (X3DExecutionContext* const executionContext) { return new ClipPlane                    (executionContext); });
	addNode ("CollidableOffset",             [ ] (X3DExecutionContext* const executionContext) { return new CollidableOffset             (executionContext); });
	addNode ("CollidableShape",              [ ] (X3DExecutionContext* const executionContext) { return new CollidableShape              (executionContext); });
	addNode ("Collision",                    [ ] (X3DExecutionContext* const executionContext) { return new Collision                    (executionContext); });
	addNode ("CollisionCollection",          [ ] (X3DExecutionContext* const executionContext) { return new CollisionCollection          (executionContext); });
	addNode ("CollisionSensor",              [ ] (X3DExecutionContext* const executionContext) { return new CollisionSensor              (executionContext); });
	addNode ("CollisionSpace",               [ ] (X3DExecutionContext* const executionContext) { return new CollisionSpace               (executionContext); });
	addNode ("Color",                        [ ] (X3DExecutionContext* const executionContext) { return new Color                        (executionContext); });
	addNode ("ColorChaser",                  [ ] (X3DExecutionContext* const executionContext) { return new ColorChaser                  (executionContext); });
	addNode ("ColorDamper",                  [ ] (X3DExecutionContext* const executionContext) { return new ColorDamper                  (executionContext); });
	addNode ("ColorInterpolator",            [ ] (X3DExecutionContext* const executionContext) { return new ColorInterpolator            (executionContext); });
	addNode ("ColorRGBA",                    [ ] (X3DExecutionContext* const executionContext) { return new ColorRGBA                    (executionContext); });
	addNode ("ComposedCubeMapTexture",       [ ] (X3DExecutionContext* const executionContext) { return new ComposedCubeMapTexture       (executionContext); });
	addNode ("ComposedShader",               [ ] (X3DExecutionContext* const executionContext) { return new ComposedShader               (executionContext); });
	addNode ("ComposedTexture3D",            [ ] (X3DExecutionContext* const executionContext) { return new ComposedTexture3D            (executionContext); });
	addNode ("Cone",                         [ ] (X3DExecutionContext* const executionContext) { return new Cone                         (executionContext); });
	addNode ("ConeEmitter",                  [ ] (X3DExecutionContext* const executionContext) { return new ConeEmitter                  (executionContext); });
	addNode ("Contact",                      [ ] (X3DExecutionContext* const executionContext) { return new Contact                      (executionContext); });
	addNode ("Contour2D",                    [ ] (X3DExecutionContext* const executionContext) { return new Contour2D                    (executionContext); });
	addNode ("ContourPolyline2D",            [ ] (X3DExecutionContext* const executionContext) { return new ContourPolyline2D            (executionContext); });
	addNode ("Coordinate",                   [ ] (X3DExecutionContext* const executionContext) { return new Coordinate                   (executionContext); });
	addNode ("CoordinateChaser",             [ ] (X3DExecutionContext* const executionContext) { return new CoordinateChaser             (executionContext); });
	addNode ("CoordinateDamper",             [ ] (X3DExecutionContext* const executionContext) { return new CoordinateDamper             (executionContext); });
	addNode ("CoordinateDouble",             [ ] (X3DExecutionContext* const executionContext) { return new CoordinateDouble             (executionContext); });
	addNode ("CoordinateInterpolator",       [ ] (X3DExecutionContext* const executionContext) { return new CoordinateInterpolator       (executionContext); });
	addNode ("CoordinateInterpolator2D",     [ ] (X3DExecutionContext* const executionContext) { return new CoordinateInterpolator2D     (executionContext); });
	addNode ("Cylinder",                     [ ] (X3DExecutionContext* const executionContext) { return new Cylinder                     (executionContext); });
	addNode ("CylinderSensor",               [ ] (X3DExecutionContext* const executionContext) { return new CylinderSensor               (executionContext); });
	addNode ("DISEntityManager",             [ ] (X3DExecutionContext* const executionContext) { return new DISEntityManager             (executionContext); });
	addNode ("DISEntityTypeMapping",         [ ] (X3DExecutionContext* const executionContext) { return new DISEntityTypeMapping         (executionContext); });
	addNode ("DirectionalLight",             [ ] (X3DExecutionContext* const executionContext) { return new DirectionalLight             (executionContext); });
	addNode ("Disk2D",                       [ ] (X3DExecutionContext* const executionContext) { return new Disk2D                       (executionContext); });
	addNode ("DoubleAxisHingeJoint",         [ ] (X3DExecutionContext* const executionContext) { return new DoubleAxisHingeJoint         (executionContext); });
	addNode ("EaseInEaseOut",                [ ] (X3DExecutionContext* const executionContext) { return new EaseInEaseOut                (executionContext); });
	addNode ("ElevationGrid",                [ ] (X3DExecutionContext* const executionContext) { return new ElevationGrid                (executionContext); });
	addNode ("EspduTransform",               [ ] (X3DExecutionContext* const executionContext) { return new EspduTransform               (executionContext); });
	addNode ("ExplosionEmitter",             [ ] (X3DExecutionContext* const executionContext) { return new ExplosionEmitter             (executionContext); });
	addNode ("Extrusion",                    [ ] (X3DExecutionContext* const executionContext) { return new Extrusion                    (executionContext); });
	addNode ("FillProperties",               [ ] (X3DExecutionContext* const executionContext) { return new FillProperties               (executionContext); });
	addNode ("FloatVertexAttribute",         [ ] (X3DExecutionContext* const executionContext) { return new FloatVertexAttribute         (executionContext); });
	addNode ("Fog",                          [ ] (X3DExecutionContext* const executionContext) { return new Fog                          (executionContext); });
	addNode ("FogCoordinate",                [ ] (X3DExecutionContext* const executionContext) { return new FogCoordinate                (executionContext); });
	addNode ("FontStyle",                    [ ] (X3DExecutionContext* const executionContext) { return new FontStyle                    (executionContext); });
	addNode ("GeneratedCubeMapTexture",      [ ] (X3DExecutionContext* const executionContext) { return new GeneratedCubeMapTexture      (executionContext); });
	addNode ("GeoCoordinate",                [ ] (X3DExecutionContext* const executionContext) { return new GeoCoordinate                (executionContext); });
	addNode ("GeoElevationGrid",             [ ] (X3DExecutionContext* const executionContext) { return new GeoElevationGrid             (executionContext); });
	addNode ("GeoLOD",                       [ ] (X3DExecutionContext* const executionContext) { return new GeoLOD                       (executionContext); });
	addNode ("GeoLocation",                  [ ] (X3DExecutionContext* const executionContext) { return new GeoLocation                  (executionContext); });
	addNode ("GeoMetadata",                  [ ] (X3DExecutionContext* const executionContext) { return new GeoMetadata                  (executionContext); });
	addNode ("GeoOrigin",                    [ ] (X3DExecutionContext* const executionContext) { return new GeoOrigin                    (executionContext); }); // GeoOrigin node is not allowed in X3D scenes having version 3.3 or higher.
	addNode ("GeoPositionInterpolator",      [ ] (X3DExecutionContext* const executionContext) { return new GeoPositionInterpolator      (executionContext); });
	addNode ("GeoProximitySensor",           [ ] (X3DExecutionContext* const executionContext) { return new GeoProximitySensor           (executionContext); });
	addNode ("GeoTouchSensor",               [ ] (X3DExecutionContext* const executionContext) { return new GeoTouchSensor               (executionContext); });
	addNode ("GeoTransform",                 [ ] (X3DExecutionContext* const executionContext) { return new GeoTransform                 (executionContext); });
	addNode ("GeoViewpoint",                 [ ] (X3DExecutionContext* const executionContext) { return new GeoViewpoint                 (executionContext); });
	addNode ("ForcePhysicsModel",            [ ] (X3DExecutionContext* const executionContext) { return new ForcePhysicsModel            (executionContext); });
	addNode ("Group",                        [ ] (X3DExecutionContext* const executionContext) { return new Group                        (executionContext); });
	addNode ("HAnimDisplacer",               [ ] (X3DExecutionContext* const executionContext) { return new HAnimDisplacer               (executionContext); });
	addNode ("HAnimHumanoid",                [ ] (X3DExecutionContext* const executionContext) { return new HAnimHumanoid                (executionContext); });
	addNode ("HAnimJoint",                   [ ] (X3DExecutionContext* const executionContext) { return new HAnimJoint                   (executionContext); });
	addNode ("HAnimSegment",                 [ ] (X3DExecutionContext* const executionContext) { return new HAnimSegment                 (executionContext); });
	addNode ("HAnimSite",                    [ ] (X3DExecutionContext* const executionContext) { return new HAnimSite                    (executionContext); });
	addNode ("ImageCubeMapTexture",          [ ] (X3DExecutionContext* const executionContext) { return new ImageCubeMapTexture          (executionContext); });
	addNode ("ImageTexture",                 [ ] (X3DExecutionContext* const executionContext) { return new ImageTexture                 (executionContext); });
	addNode ("ImageTexture3D",               [ ] (X3DExecutionContext* const executionContext) { return new ImageTexture3D               (executionContext); });
	addNode ("IndexedFaceSet",               [ ] (X3DExecutionContext* const executionContext) { return new IndexedFaceSet               (executionContext); });
	addNode ("IndexedLineSet",               [ ] (X3DExecutionContext* const executionContext) { return new IndexedLineSet               (executionContext); });
	addNode ("IndexedQuadSet",               [ ] (X3DExecutionContext* const executionContext) { return new IndexedQuadSet               (executionContext); });
	addNode ("IndexedTriangleFanSet",        [ ] (X3DExecutionContext* const executionContext) { return new IndexedTriangleFanSet        (executionContext); });
	addNode ("IndexedTriangleSet",           [ ] (X3DExecutionContext* const executionContext) { return new IndexedTriangleSet           (executionContext); });
	addNode ("IndexedTriangleStripSet",      [ ] (X3DExecutionContext* const executionContext) { return new IndexedTriangleStripSet      (executionContext); });
	addNode ("Inline",                       [ ] (X3DExecutionContext* const executionContext) { return new Inline                       (executionContext); });
	addNode ("IntegerSequencer",             [ ] (X3DExecutionContext* const executionContext) { return new IntegerSequencer             (executionContext); });
	addNode ("IntegerTrigger",               [ ] (X3DExecutionContext* const executionContext) { return new IntegerTrigger               (executionContext); });
	addNode ("KeySensor",                    [ ] (X3DExecutionContext* const executionContext) { return new KeySensor                    (executionContext); });
	addNode ("LOD",                          [ ] (X3DExecutionContext* const executionContext) { return new LOD                          (executionContext); });
	addNode ("Layer",                        [ ] (X3DExecutionContext* const executionContext) { return new Layer                        (executionContext); });
	addNode ("LayerSet",                     [ ] (X3DExecutionContext* const executionContext) { return new LayerSet                     (executionContext); });
	addNode ("Layout",                       [ ] (X3DExecutionContext* const executionContext) { return new Layout                       (executionContext); });
	addNode ("LayoutGroup",                  [ ] (X3DExecutionContext* const executionContext) { return new LayoutGroup                  (executionContext); });
	addNode ("LayoutLayer",                  [ ] (X3DExecutionContext* const executionContext) { return new LayoutLayer                  (executionContext); });
	addNode ("LinePickSensor",               [ ] (X3DExecutionContext* const executionContext) { return new LinePickSensor               (executionContext); });
	addNode ("LineProperties",               [ ] (X3DExecutionContext* const executionContext) { return new LineProperties               (executionContext); });
	addNode ("LineSet",                      [ ] (X3DExecutionContext* const executionContext) { return new LineSet                      (executionContext); });
	addNode ("LoadSensor",                   [ ] (X3DExecutionContext* const executionContext) { return new LoadSensor                   (executionContext); });
	addNode ("LocalFog",                     [ ] (X3DExecutionContext* const executionContext) { return new LocalFog                     (executionContext); });
	addNode ("Material",                     [ ] (X3DExecutionContext* const executionContext) { return new Material                     (executionContext); });
	addNode ("Matrix3VertexAttribute",       [ ] (X3DExecutionContext* const executionContext) { return new Matrix3VertexAttribute       (executionContext); });
	addNode ("Matrix4VertexAttribute",       [ ] (X3DExecutionContext* const executionContext) { return new Matrix4VertexAttribute       (executionContext); });
	addNode ("MetadataBoolean",              [ ] (X3DExecutionContext* const executionContext) { return new MetadataBoolean              (executionContext); });
	addNode ("MetadataDouble",               [ ] (X3DExecutionContext* const executionContext) { return new MetadataDouble               (executionContext); });
	addNode ("MetadataFloat",                [ ] (X3DExecutionContext* const executionContext) { return new MetadataFloat                (executionContext); });
	addNode ("MetadataInteger",              [ ] (X3DExecutionContext* const executionContext) { return new MetadataInteger              (executionContext); });
	addNode ("MetadataSet",                  [ ] (X3DExecutionContext* const executionContext) { return new MetadataSet                  (executionContext); });
	addNode ("MetadataString",               [ ] (X3DExecutionContext* const executionContext) { return new MetadataString               (executionContext); });
	addNode ("MotorJoint",                   [ ] (X3DExecutionContext* const executionContext) { return new MotorJoint                   (executionContext); });
	addNode ("MovieTexture",                 [ ] (X3DExecutionContext* const executionContext) { return new MovieTexture                 (executionContext); });
	addNode ("MultiTexture",                 [ ] (X3DExecutionContext* const executionContext) { return new MultiTexture                 (executionContext); });
	addNode ("MultiTextureCoordinate",       [ ] (X3DExecutionContext* const executionContext) { return new MultiTextureCoordinate       (executionContext); });
	addNode ("MultiTextureTransform",        [ ] (X3DExecutionContext* const executionContext) { return new MultiTextureTransform        (executionContext); });
	addNode ("NavigationInfo",               [ ] (X3DExecutionContext* const executionContext) { return new NavigationInfo               (executionContext); });
	addNode ("Normal",                       [ ] (X3DExecutionContext* const executionContext) { return new Normal                       (executionContext); });
	addNode ("NormalInterpolator",           [ ] (X3DExecutionContext* const executionContext) { return new NormalInterpolator           (executionContext); });
	addNode ("NurbsCurve",                   [ ] (X3DExecutionContext* const executionContext) { return new NurbsCurve                   (executionContext); });
	addNode ("NurbsCurve2D",                 [ ] (X3DExecutionContext* const executionContext) { return new NurbsCurve2D                 (executionContext); });
	addNode ("NurbsOrientationInterpolator", [ ] (X3DExecutionContext* const executionContext) { return new NurbsOrientationInterpolator (executionContext); });
	addNode ("NurbsPatchSurface",            [ ] (X3DExecutionContext* const executionContext) { return new NurbsPatchSurface            (executionContext); });
	addNode ("NurbsPositionInterpolator",    [ ] (X3DExecutionContext* const executionContext) { return new NurbsPositionInterpolator    (executionContext); });
	addNode ("NurbsSet",                     [ ] (X3DExecutionContext* const executionContext) { return new NurbsSet                     (executionContext); });
	addNode ("NurbsSurfaceInterpolator",     [ ] (X3DExecutionContext* const executionContext) { return new NurbsSurfaceInterpolator     (executionContext); });
	addNode ("NurbsSweptSurface",            [ ] (X3DExecutionContext* const executionContext) { return new NurbsSweptSurface            (executionContext); });
	addNode ("NurbsSwungSurface",            [ ] (X3DExecutionContext* const executionContext) { return new NurbsSwungSurface            (executionContext); });
	addNode ("NurbsTextureCoordinate",       [ ] (X3DExecutionContext* const executionContext) { return new NurbsTextureCoordinate       (executionContext); });
	addNode ("NurbsTrimmedSurface",          [ ] (X3DExecutionContext* const executionContext) { return new NurbsTrimmedSurface          (executionContext); });
	addNode ("OrientationChaser",            [ ] (X3DExecutionContext* const executionContext) { return new OrientationChaser            (executionContext); });
	addNode ("OrientationDamper",            [ ] (X3DExecutionContext* const executionContext) { return new OrientationDamper            (executionContext); });
	addNode ("OrientationInterpolator",      [ ] (X3DExecutionContext* const executionContext) { return new OrientationInterpolator      (executionContext); });
	addNode ("OrthoViewpoint",               [ ] (X3DExecutionContext* const executionContext) { return new OrthoViewpoint               (executionContext); });
	addNode ("PackagedShader",               [ ] (X3DExecutionContext* const executionContext) { return new PackagedShader               (executionContext); });
	addNode ("ParticleSystem",               [ ] (X3DExecutionContext* const executionContext) { return new ParticleSystem               (executionContext); });
	addNode ("PickableGroup",                [ ] (X3DExecutionContext* const executionContext) { return new PickableGroup                (executionContext); });
	addNode ("PixelTexture",                 [ ] (X3DExecutionContext* const executionContext) { return new PixelTexture                 (executionContext); });
	addNode ("PixelTexture3D",               [ ] (X3DExecutionContext* const executionContext) { return new PixelTexture3D               (executionContext); });
	addNode ("PlaneSensor",                  [ ] (X3DExecutionContext* const executionContext) { return new PlaneSensor                  (executionContext); });
	addNode ("PointEmitter",                 [ ] (X3DExecutionContext* const executionContext) { return new PointEmitter                 (executionContext); });
	addNode ("PointLight",                   [ ] (X3DExecutionContext* const executionContext) { return new PointLight                   (executionContext); });
	addNode ("PointPickSensor",              [ ] (X3DExecutionContext* const executionContext) { return new PointPickSensor              (executionContext); });
	addNode ("PointSet",                     [ ] (X3DExecutionContext* const executionContext) { return new PointSet                     (executionContext); });
	addNode ("Polyline2D",                   [ ] (X3DExecutionContext* const executionContext) { return new Polyline2D                   (executionContext); });
	addNode ("PolylineEmitter",              [ ] (X3DExecutionContext* const executionContext) { return new PolylineEmitter              (executionContext); });
	addNode ("Polypoint2D",                  [ ] (X3DExecutionContext* const executionContext) { return new Polypoint2D                  (executionContext); });
	addNode ("PositionChaser",               [ ] (X3DExecutionContext* const executionContext) { return new PositionChaser               (executionContext); });
	addNode ("PositionChaser2D",             [ ] (X3DExecutionContext* const executionContext) { return new PositionChaser2D             (executionContext); });
	addNode ("PositionDamper",               [ ] (X3DExecutionContext* const executionContext) { return new PositionDamper               (executionContext); });
	addNode ("PositionDamper2D",             [ ] (X3DExecutionContext* const executionContext) { return new PositionDamper2D             (executionContext); });
	addNode ("PositionInterpolator",         [ ] (X3DExecutionContext* const executionContext) { return new PositionInterpolator         (executionContext); });
	addNode ("PositionInterpolator2D",       [ ] (X3DExecutionContext* const executionContext) { return new PositionInterpolator2D       (executionContext); });
	addNode ("PrimitivePickSensor",          [ ] (X3DExecutionContext* const executionContext) { return new PrimitivePickSensor          (executionContext); });
	addNode ("ProgramShader",                [ ] (X3DExecutionContext* const executionContext) { return new ProgramShader                (executionContext); });
	addNode ("ProximitySensor",              [ ] (X3DExecutionContext* const executionContext) { return new ProximitySensor              (executionContext); });
	addNode ("QuadSet",                      [ ] (X3DExecutionContext* const executionContext) { return new QuadSet                      (executionContext); });
	addNode ("ReceiverPdu",                  [ ] (X3DExecutionContext* const executionContext) { return new ReceiverPdu                  (executionContext); });
	addNode ("Rectangle2D",                  [ ] (X3DExecutionContext* const executionContext) { return new Rectangle2D                  (executionContext); });
	addNode ("RigidBody",                    [ ] (X3DExecutionContext* const executionContext) { return new RigidBody                    (executionContext); });
	addNode ("RigidBodyCollection",          [ ] (X3DExecutionContext* const executionContext) { return new RigidBodyCollection          (executionContext); });
	addNode ("ScalarChaser",                 [ ] (X3DExecutionContext* const executionContext) { return new ScalarChaser                 (executionContext); });
	addNode ("ScalarDamper",                 [ ] (X3DExecutionContext* const executionContext) { return new ScalarDamper                 (executionContext); });
	addNode ("ScalarInterpolator",           [ ] (X3DExecutionContext* const executionContext) { return new ScalarInterpolator           (executionContext); });
	addNode ("ScreenFontStyle",              [ ] (X3DExecutionContext* const executionContext) { return new ScreenFontStyle              (executionContext); });
	addNode ("ScreenGroup",                  [ ] (X3DExecutionContext* const executionContext) { return new ScreenGroup                  (executionContext); });
	addNode ("Script",                       [ ] (X3DExecutionContext* const executionContext) { return new Script                       (executionContext); });
	addNode ("ShaderPart",                   [ ] (X3DExecutionContext* const executionContext) { return new ShaderPart                   (executionContext); });
	addNode ("ShaderProgram",                [ ] (X3DExecutionContext* const executionContext) { return new ShaderProgram                (executionContext); });
	addNode ("Shape",                        [ ] (X3DExecutionContext* const executionContext) { return new Shape                        (executionContext); });
	addNode ("SignalPdu",                    [ ] (X3DExecutionContext* const executionContext) { return new SignalPdu                    (executionContext); });
	addNode ("SingleAxisHingeJoint",         [ ] (X3DExecutionContext* const executionContext) { return new SingleAxisHingeJoint         (executionContext); });
	addNode ("SliderJoint",                  [ ] (X3DExecutionContext* const executionContext) { return new SliderJoint                  (executionContext); });
	addNode ("Sound",                        [ ] (X3DExecutionContext* const executionContext) { return new Sound                        (executionContext); });
	addNode ("Sphere",                       [ ] (X3DExecutionContext* const executionContext) { return new Sphere                       (executionContext); });
	addNode ("SphereSensor",                 [ ] (X3DExecutionContext* const executionContext) { return new SphereSensor                 (executionContext); });
	addNode ("SplinePositionInterpolator",   [ ] (X3DExecutionContext* const executionContext) { return new SplinePositionInterpolator   (executionContext); });
	addNode ("SplinePositionInterpolator2D", [ ] (X3DExecutionContext* const executionContext) { return new SplinePositionInterpolator2D (executionContext); });
	addNode ("SplineScalarInterpolator",     [ ] (X3DExecutionContext* const executionContext) { return new SplineScalarInterpolator     (executionContext); });
	addNode ("SpotLight",                    [ ] (X3DExecutionContext* const executionContext) { return new SpotLight                    (executionContext); });
	addNode ("SquadOrientationInterpolator", [ ] (X3DExecutionContext* const executionContext) { return new SquadOrientationInterpolator (executionContext); });
	addNode ("StaticGroup",                  [ ] (X3DExecutionContext* const executionContext) { return new StaticGroup                  (executionContext); });
	addNode ("StringSensor",                 [ ] (X3DExecutionContext* const executionContext) { return new StringSensor                 (executionContext); });
	addNode ("SurfaceEmitter",               [ ] (X3DExecutionContext* const executionContext) { return new SurfaceEmitter               (executionContext); });
	addNode ("Switch",                       [ ] (X3DExecutionContext* const executionContext) { return new Switch                       (executionContext); });
	addNode ("TexCoordChaser2D",             [ ] (X3DExecutionContext* const executionContext) { return new TexCoordChaser2D             (executionContext); });
	addNode ("TexCoordDamper2D",             [ ] (X3DExecutionContext* const executionContext) { return new TexCoordDamper2D             (executionContext); });
	addNode ("Text",                         [ ] (X3DExecutionContext* const executionContext) { return new Text                         (executionContext); });
	addNode ("TextureBackground",            [ ] (X3DExecutionContext* const executionContext) { return new TextureBackground            (executionContext); });
	addNode ("TextureCoordinate",            [ ] (X3DExecutionContext* const executionContext) { return new TextureCoordinate            (executionContext); });
	addNode ("TextureCoordinate3D",          [ ] (X3DExecutionContext* const executionContext) { return new TextureCoordinate3D          (executionContext); });
	addNode ("TextureCoordinate4D",          [ ] (X3DExecutionContext* const executionContext) { return new TextureCoordinate4D          (executionContext); });
	addNode ("TextureCoordinateGenerator",   [ ] (X3DExecutionContext* const executionContext) { return new TextureCoordinateGenerator   (executionContext); });
	addNode ("TextureProperties",            [ ] (X3DExecutionContext* const executionContext) { return new TextureProperties            (executionContext); });
	addNode ("TextureTransform",             [ ] (X3DExecutionContext* const executionContext) { return new TextureTransform             (executionContext); });
	addNode ("TextureTransform3D",           [ ] (X3DExecutionContext* const executionContext) { return new TextureTransform3D           (executionContext); });
	addNode ("TextureTransformMatrix3D",     [ ] (X3DExecutionContext* const executionContext) { return new TextureTransformMatrix3D     (executionContext); });
	addNode ("TimeSensor",                   [ ] (X3DExecutionContext* const executionContext) { return new TimeSensor                   (executionContext); });
	addNode ("TimeTrigger",                  [ ] (X3DExecutionContext* const executionContext) { return new TimeTrigger                  (executionContext); });
	addNode ("TouchSensor",                  [ ] (X3DExecutionContext* const executionContext) { return new TouchSensor                  (executionContext); });
	addNode ("Transform",                    [ ] (X3DExecutionContext* const executionContext) { return new Transform                    (executionContext); });
	addNode ("TransformSensor",              [ ] (X3DExecutionContext* const executionContext) { return new TransformSensor              (executionContext); });
	addNode ("TransmitterPdu",               [ ] (X3DExecutionContext* const executionContext) { return new TransmitterPdu               (executionContext); });
	addNode ("TriangleFanSet",               [ ] (X3DExecutionContext* const executionContext) { return new TriangleFanSet               (executionContext); });
	addNode ("TriangleSet",                  [ ] (X3DExecutionContext* const executionContext) { return new TriangleSet                  (executionContext); });
	addNode ("TriangleSet2D",                [ ] (X3DExecutionContext* const executionContext) { return new TriangleSet2D                (executionContext); });
	addNode ("TriangleStripSet",             [ ] (X3DExecutionContext* const executionContext) { return new TriangleStripSet             (executionContext); });
	addNode ("TwoSidedMaterial",             [ ] (X3DExecutionContext* const executionContext) { return new TwoSidedMaterial             (executionContext); });
	addNode ("UniversalJoint",               [ ] (X3DExecutionContext* const executionContext) { return new UniversalJoint               (executionContext); });
	addNode ("Viewpoint",                    [ ] (X3DExecutionContext* const executionContext) { return new Viewpoint                    (executionContext); });
	addNode ("ViewpointGroup",               [ ] (X3DExecutionContext* const executionContext) { return new ViewpointGroup               (executionContext); });
	addNode ("Viewport",                     [ ] (X3DExecutionContext* const executionContext) { return new Viewport                     (executionContext); });
	addNode ("VisibilitySensor",             [ ] (X3DExecutionContext* const executionContext) { return new VisibilitySensor             (executionContext); });
	addNode ("VolumeEmitter",                [ ] (X3DExecutionContext* const executionContext) { return new VolumeEmitter                (executionContext); });
	addNode ("VolumePickSensor",             [ ] (X3DExecutionContext* const executionContext) { return new VolumePickSensor             (executionContext); });
	addNode ("WindPhysicsModel",             [ ] (X3DExecutionContext* const executionContext) { return new WindPhysicsModel             (executionContext); });
	addNode ("WorldInfo",                    [ ] (X3DExecutionContext* const executionContext) { return new WorldInfo                    (executionContext); });

	//  Non standard: Component »Titania«

	addNode ("TransformMatrix3D",            [ ] (X3DExecutionContext* const executionContext) { return new TransformMatrix3D            (executionContext); });
	addNode ("TouchGroup",                   [ ] (X3DExecutionContext* const executionContext) { return new TouchGroup                   (executionContext); });
	addNode ("PolygonOffset",                [ ] (X3DExecutionContext* const executionContext) { return new PolygonOffset                (executionContext); });
	addNode ("DepthBuffer",                  [ ] (X3DExecutionContext* const executionContext) { return new DepthBuffer                  (executionContext); });

	//std::clog << "\tDone creating node index." << std::endl;
}

void
SupportedNodes::addNode (const std::string & typeName, const Function & function)
throw (Error <INVALID_NAME>)
{
	//__LOG__ << "\tAdding node type " << typeName << ": " << std::endl;

	if (not functions .emplace (typeName, function) .second)
		throw Error <INVALID_NAME> ("Node type '" + typeName + "' already exists.");
}

const X3DBaseNode*
SupportedNodes::getNode (const std::string & typeName) const
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
SupportedNodes::getNodes () const
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
