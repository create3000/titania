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
 ******************************************************************************/

#include "SupportedNodes.h"

#include "../Components.h"
#include <iostream>

namespace titania {
namespace X3D {

SupportedNodes::SupportedNodes (X3DExecutionContext* const executionContext)
{
	//std::clog << "Creating node index:" << std::endl;

	add (new Anchor                       (executionContext));
	add (new Appearance                   (executionContext));
	add (new Arc2D                        (executionContext));
	add (new ArcClose2D                   (executionContext));
	add (new AudioClip                    (executionContext));
	add (new Background                   (executionContext));
	add (new BallJoint                    (executionContext));
	add (new Billboard                    (executionContext));
	add (new BooleanFilter                (executionContext));
	add (new BooleanSequencer             (executionContext));
	add (new BooleanToggle                (executionContext));
	add (new BooleanTrigger               (executionContext));
	add (new BoundedPhysicsModel          (executionContext));
	add (new Box                          (executionContext));
	add (new CADAssembly                  (executionContext));
	add (new CADFace                      (executionContext));
	add (new CADLayer                     (executionContext));
	add (new CADPart                      (executionContext));
	add (new Circle2D                     (executionContext));
	add (new ClipPlane                    (executionContext));
	add (new CollidableOffset             (executionContext));
	add (new CollidableShape              (executionContext));
	add (new Collision                    (executionContext));
	add (new CollisionCollection          (executionContext));
	add (new CollisionSensor              (executionContext));
	add (new CollisionSpace               (executionContext));
	add (new Color                        (executionContext));
	add (new ColorDamper                  (executionContext));
	add (new ColorInterpolator            (executionContext));
	add (new ColorRGBA                    (executionContext));
	add (new ComposedCubeMapTexture       (executionContext));
	add (new ComposedShader               (executionContext));
	add (new ComposedTexture3D            (executionContext));
	add (new Cone                         (executionContext));
	add (new ConeEmitter                  (executionContext));
	add (new Contact                      (executionContext));
	add (new Contour2D                    (executionContext));
	add (new ContourPolyline2D            (executionContext));
	add (new Coordinate                   (executionContext));
	add (new CoordinateDamper             (executionContext));
	add (new CoordinateDouble             (executionContext));
	add (new CoordinateInterpolator       (executionContext));
	add (new CoordinateInterpolator2D     (executionContext));
	add (new Cylinder                     (executionContext));
	add (new CylinderSensor               (executionContext));
	add (new DISEntityManager             (executionContext));
	add (new DISEntityTypeMapping         (executionContext));
	add (new DirectionalLight             (executionContext));
	add (new Disk2D                       (executionContext));
	add (new DoubleAxisHingeJoint         (executionContext));
	add (new EaseInEaseOut                (executionContext));
	add (new ElevationGrid                (executionContext));
	add (new EspduTransform               (executionContext));
	add (new ExplosionEmitter             (executionContext));
	add (new Extrusion                    (executionContext));
	add (new FillProperties               (executionContext));
	add (new FloatVertexAttribute         (executionContext));
	add (new Fog                          (executionContext));
	add (new FogCoordinate                (executionContext));
	add (new FontStyle                    (executionContext));
	add (new GeneratedCubeMapTexture      (executionContext));
	add (new GeoCoordinate                (executionContext));
	add (new GeoElevationGrid             (executionContext));
	add (new GeoLOD                       (executionContext));
	add (new GeoLocation                  (executionContext));
	add (new GeoMetadata                  (executionContext));
	add (new GeoOrigin                    (executionContext));
	add (new GeoPositionInterpolator      (executionContext));
	add (new GeoProximitySensor           (executionContext));
	add (new GeoTouchSensor               (executionContext));
	add (new GeoTransform                 (executionContext));
	add (new GeoViewpoint                 (executionContext));
	add (new GravityPhysicsModel          (executionContext));
	add (new Group                        (executionContext));
	add (new HAnimDisplacer               (executionContext));
	add (new HAnimHumanoid                (executionContext));
	add (new HAnimJoint                   (executionContext));
	add (new HAnimSegment                 (executionContext));
	add (new HAnimSite                    (executionContext));
	add (new ImageCubeMapTexture          (executionContext));
	add (new ImageTexture                 (executionContext));
	add (new ImageTexture3D               (executionContext));
	add (new IndexedFaceSet               (executionContext));
	add (new IndexedLineSet               (executionContext));
	add (new IndexedQuadSet               (executionContext));
	add (new IndexedTriangleFanSet        (executionContext));
	add (new IndexedTriangleSet           (executionContext));
	add (new IndexedTriangleStripSet      (executionContext));
	add (new Inline                       (executionContext));
	add (new IntegerSequencer             (executionContext));
	add (new IntegerTrigger               (executionContext));
	add (new KeySensor                    (executionContext));
	add (new LOD                          (executionContext));
	add (new Layer                        (executionContext));
	add (new LayerSet                     (executionContext));
	add (new Layout                       (executionContext));
	add (new LayoutGroup                  (executionContext));
	add (new LayoutLayer                  (executionContext));
	add (new LinePickSensor               (executionContext));
	add (new LineProperties               (executionContext));
	add (new LineSet                      (executionContext));
	add (new LoadSensor                   (executionContext));
	add (new LocalFog                     (executionContext));
	add (new Material                     (executionContext));
	add (new Matrix3VertexAttribute       (executionContext));
	add (new Matrix4VertexAttribute       (executionContext));
	add (new MetadataBool                 (executionContext));
	add (new MetadataDouble               (executionContext));
	add (new MetadataFloat                (executionContext));
	add (new MetadataInteger              (executionContext));
	add (new MetadataSet                  (executionContext));
	add (new MetadataString               (executionContext));
	add (new MotorJoint                   (executionContext));
	add (new MovieTexture                 (executionContext));
	add (new MultiTexture                 (executionContext));
	add (new MultiTextureCoordinate       (executionContext));
	add (new MultiTextureTransform        (executionContext));
	add (new NavigationInfo               (executionContext));
	add (new Normal                       (executionContext));
	add (new NormalInterpolator           (executionContext));
	add (new NurbsCurve                   (executionContext));
	add (new NurbsCurve2D                 (executionContext));
	add (new NurbsOrientationInterpolator (executionContext));
	add (new NurbsPatchSurface            (executionContext));
	add (new NurbsPositionInterpolator    (executionContext));
	add (new NurbsSet                     (executionContext));
	add (new NurbsSurfaceInterpolator     (executionContext));
	add (new NurbsSweptSurface            (executionContext));
	add (new NurbsSwungSurface            (executionContext));
	add (new NurbsTextureCoordinate       (executionContext));
	add (new NurbsTrimmedSurface          (executionContext));
	add (new OrientationChaser            (executionContext));
	add (new OrientationDamper            (executionContext));
	add (new OrientationInterpolator      (executionContext));
	add (new OrthoViewpoint               (executionContext));
	add (new PackagedShader               (executionContext));
	add (new ParticleSystem               (executionContext));
	add (new PickableGroup                (executionContext));
	add (new PixelTexture                 (executionContext));
	add (new PixelTexture3D               (executionContext));
	add (new PlaneSensor                  (executionContext));
	add (new PointEmitter                 (executionContext));
	add (new PointLight                   (executionContext));
	add (new PointPickSensor              (executionContext));
	add (new PointSet                     (executionContext));
	add (new Polyline2D                   (executionContext));
	add (new PolylineEmitter              (executionContext));
	add (new Polypoint2D                  (executionContext));
	add (new PositionChaser               (executionContext));
	add (new PositionChaser2D             (executionContext));
	add (new PositionDamper               (executionContext));
	add (new PositionDamper2D             (executionContext));
	add (new PositionInterpolator         (executionContext));
	add (new PositionInterpolator2D       (executionContext));
	add (new PrimitivePickSensor          (executionContext));
	add (new ProgramShader                (executionContext));
	add (new ProximitySensor              (executionContext));
	add (new QuadSet                      (executionContext));
	add (new ReceiverPdu                  (executionContext));
	add (new Rectangle2D                  (executionContext));
	add (new RigidBody                    (executionContext));
	add (new RigidBodyCollection          (executionContext));
	add (new ScalarChaser                 (executionContext));
	add (new ScalarInterpolator           (executionContext));
	add (new ScreenFontStyle              (executionContext));
	add (new ScreenGroup                  (executionContext));
	add (new Script                       (executionContext));
	add (new ShaderPart                   (executionContext));
	add (new ShaderProgram                (executionContext));
	add (new Shape                        (executionContext));
	add (new SignalPdu                    (executionContext));
	add (new SingleAxisHingeJoint         (executionContext));
	add (new SliderJoint                  (executionContext));
	add (new Sound                        (executionContext));
	add (new Sphere                       (executionContext));
	add (new SphereSensor                 (executionContext));
	add (new SplinePositionInterpolator   (executionContext));
	add (new SplinePositionInterpolator2D (executionContext));
	add (new SplineScalarInterpolator     (executionContext));
	add (new SpotLight                    (executionContext));
	add (new SquadOrientationInterpolator (executionContext));
	add (new StaticGroup                  (executionContext));
	add (new StringSensor                 (executionContext));
	add (new SurfaceEmitter               (executionContext));
	add (new Switch                       (executionContext));
	add (new TexCoordDamper2D             (executionContext));
	add (new Text                         (executionContext));
	add (new TextureBackground            (executionContext));
	add (new TextureCoordinate            (executionContext));
	add (new TextureCoordinate3D          (executionContext));
	add (new TextureCoordinate4D          (executionContext));
	add (new TextureCoordinateGenerator   (executionContext));
	add (new TextureProperties            (executionContext));
	add (new TextureTransform             (executionContext));
	add (new TextureTransform3D           (executionContext));
	add (new TextureTransformMatrix3D     (executionContext));
	add (new TimeSensor                   (executionContext));
	add (new TimeTrigger                  (executionContext));
	add (new TouchSensor                  (executionContext));
	add (new Transform                    (executionContext));
	add (new TransformSensor              (executionContext));
	add (new TransmitterPdu               (executionContext));
	add (new TriangleFanSet               (executionContext));
	add (new TriangleSet                  (executionContext));
	add (new TriangleSet2D                (executionContext));
	add (new TriangleStripSet             (executionContext));
	add (new TwoSidedMaterial             (executionContext));
	add (new UniversalJoint               (executionContext));
	add (new Viewpoint                    (executionContext));
	add (new ViewpointGroup               (executionContext));
	add (new Viewport                     (executionContext));
	add (new VisibilitySensor             (executionContext));
	add (new VolumeEmitter                (executionContext));
	add (new VolumePickSensor             (executionContext));
	add (new WindPhysicsModel             (executionContext));
	add (new WorldInfo                    (executionContext));

	//std::clog << "\tDone creating node index." << std::endl;
}

void
SupportedNodes::add (X3DBasicNode* node)
{
	//std::clog << "\tAdding node type " << node -> getTypeName () << ": " << std::flush;

	nodes .push_back (node -> getTypeName (), node);

	//std::clog << "Done." << std::endl;
}

const X3DBasicNode*
SupportedNodes::get (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	try
	{
		return nodes .last (name);
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
