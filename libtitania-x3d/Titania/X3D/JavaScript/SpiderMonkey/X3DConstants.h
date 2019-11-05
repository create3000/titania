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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DCONSTANTS_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DCONSTANTS_H__

#include "ObjectType.h"

#include <jsapi.h>

namespace titania {
namespace X3D {
namespace spidermonkey {

class X3DConstants
{
public:

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JSObject*
	create (JSContext* const cx);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ return ObjectType::X3DConstants; }


private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	// Event types

	static bool INITIALIZED_EVENT (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SHUTDOWN_EVENT    (JSContext* cx, unsigned argc, JS::Value* vp);

	// Error types

	static bool CONNECTION_ERROR  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool INITIALIZED_ERROR (JSContext* cx, unsigned argc, JS::Value* vp);

	// Load state

	static bool NOT_STARTED_STATE (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IN_PROGRESS_STATE (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool COMPLETE_STATE    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool FAILED_STATE      (JSContext* cx, unsigned argc, JS::Value* vp);

	// Unit categories

	static bool ANGLE  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool FORCE  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LENGTH (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MASS   (JSContext* cx, unsigned argc, JS::Value* vp);

	static bool ACCELERATION (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ANGULAR_RATE (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool AREA         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SPEED        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool VOLUME       (JSContext* cx, unsigned argc, JS::Value* vp);

	// Access types

	static bool initializeOnly (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool inputOnly      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool outputOnly     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool inputOutput    (JSContext* cx, unsigned argc, JS::Value* vp);

	// Field types

	static bool SFBool      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFColor     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFColorRGBA (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFDouble    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFFloat     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFInt32     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFImage     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFMatrix3d  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFMatrix3f  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFMatrix4d  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFMatrix4f  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFNode      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFRotation  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFString    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFTime      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFVec2d     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFVec2f     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFVec3d     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFVec3f     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFVec4d     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SFVec4f     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFBool      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFColor     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFColorRGBA (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFDouble    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFFloat     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFImage     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFInt32     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFMatrix3d  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFMatrix3f  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFMatrix4d  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFMatrix4f  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFNode      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFRotation  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFString    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFTime      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFVec2d     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFVec2f     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFVec3d     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFVec3f     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFVec4d     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MFVec4f     (JSContext* cx, unsigned argc, JS::Value* vp);

	// Base node type

	static bool X3DBaseNode (JSContext* cx, unsigned argc, JS::Value* vp);

	// Basic node types

	static bool Anchor                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool AnnotationLayer                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool AnnotationTarget                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Appearance                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Arc2D                            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ArcClose2D                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool AudioClip                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Background                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool BallJoint                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Billboard                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool BlendedVolumeStyle               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool BooleanFilter                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool BooleanSequencer                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool BooleanToggle                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool BooleanTrigger                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool BoundaryEnhancementVolumeStyle   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool BoundedPhysicsModel              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Box                              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CADAssembly                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CADFace                          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CADLayer                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CADPart                          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CartoonVolumeStyle               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Circle2D                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ClipPlane                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CollidableOffset                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CollidableShape                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Collision                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CollisionCollection              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CollisionSensor                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CollisionSpace                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Color                            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ColorChaser                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ColorDamper                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ColorInterpolator                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ColorRGBA                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ComposedCubeMapTexture           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ComposedShader                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ComposedTexture3D                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ComposedVolumeStyle              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Cone                             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ConeEmitter                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Contact                          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Contour2D                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ContourPolyline2D                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Coordinate                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CoordinateChaser                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CoordinateDamper                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CoordinateDouble                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CoordinateInterpolator           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CoordinateInterpolator2D         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Cylinder                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool CylinderSensor                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool DirectionalLight                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool DISEntityManager                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool DISEntityTypeMapping             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Disk2D                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool DoubleAxisHingeJoint             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool EaseInEaseOut                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool EdgeEnhancementVolumeStyle       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ElevationGrid                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool EspduTransform                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ExplosionEmitter                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Extrusion                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool FillProperties                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool FloatVertexAttribute             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Fog                              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool FogCoordinate                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool FontStyle                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ForcePhysicsModel                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeneratedCubeMapTexture          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoCoordinate                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoElevationGrid                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoLocation                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoLOD                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoMetadata                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoOrigin                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoPositionInterpolator          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoProximitySensor               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoTouchSensor                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoTransform                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GeoViewpoint                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Group                            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool GroupAnnotation                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool HAnimDisplacer                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool HAnimHumanoid                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool HAnimJoint                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool HAnimMotion                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool HAnimSegment                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool HAnimSite                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IconAnnotation                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ImageCubeMapTexture              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ImageTexture                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ImageTexture3D                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IndexedFaceSet                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IndexedLineSet                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IndexedQuadSet                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IndexedTriangleFanSet            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IndexedTriangleSet               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IndexedTriangleStripSet          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Inline                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IntegerSequencer                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IntegerTrigger                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool IsoSurfaceVolumeData             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool KeySensor                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Layer                            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LayerSet                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Layout                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LayoutGroup                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LayoutLayer                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LinePickSensor                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LineProperties                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LineSet                          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LoadSensor                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LocalFog                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool LOD                              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Material                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Matrix3VertexAttribute           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Matrix4VertexAttribute           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MetadataBoolean                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MetadataDouble                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MetadataFloat                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MetadataInteger                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MetadataSet                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MetadataString                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MotorJoint                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MovieTexture                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MultiTexture                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MultiTextureCoordinate           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool MultiTextureTransform            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NavigationInfo                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Normal                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NormalInterpolator               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsCurve                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsCurve2D                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsOrientationInterpolator     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsPatchSurface                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsPositionInterpolator        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsSet                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsSurfaceInterpolator         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsSweptSurface                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsSwungSurface                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsTextureCoordinate           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool NurbsTrimmedSurface              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool OpacityMapVolumeStyle            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool OrientationChaser                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool OrientationDamper                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool OrientationInterpolator          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool OrthoViewpoint                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PackagedShader                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ParticleSystem                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PickableGroup                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PixelTexture                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PixelTexture3D                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PlaneSensor                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PointEmitter                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PointLight                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PointPickSensor                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PointProperties                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PointSet                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Polyline2D                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PolylineEmitter                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Polypoint2D                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PositionChaser                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PositionChaser2D                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PositionDamper                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PositionDamper2D                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PositionInterpolator             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PositionInterpolator2D           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool PrimitivePickSensor              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ProgramShader                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ProjectionVolumeStyle            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ProximitySensor                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool QuadSet                          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ReceiverPdu                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Rectangle2D                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool RigidBody                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool RigidBodyCollection              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ScalarChaser                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ScalarDamper                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ScalarInterpolator               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ScreenFontStyle                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ScreenGroup                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Script                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SegmentedVolumeData              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ShadedVolumeStyle                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ShaderPart                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ShaderProgram                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Shape                            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SignalPdu                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SilhouetteEnhancementVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SingleAxisHingeJoint             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SliderJoint                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Sound                            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Sphere                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SphereSensor                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SplinePositionInterpolator       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SplinePositionInterpolator2D     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SplineScalarInterpolator         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SpotLight                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SquadOrientationInterpolator     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool StaticGroup                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool StringSensor                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool SurfaceEmitter                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Switch                           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TexCoordChaser2D                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TexCoordDamper2D                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Text                             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextAnnotation                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureBackground                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureCoordinate                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureCoordinate3D              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureCoordinate4D              (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureCoordinateGenerator       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureProjectorParallel         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureProjectorPerspective      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureProperties                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureTransform                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureTransform3D               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TextureTransformMatrix3D         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TimeSensor                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TimeTrigger                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ToneMappedVolumeStyle            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TouchGroup                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TouchSensor                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Transform                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TransformMatrix3D                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TransformSensor                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TransmitterPdu                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TriangleFanSet                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TriangleSet                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TriangleSet2D                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TriangleStripSet                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool TwoSidedMaterial                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool UniversalJoint                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool URLAnnotation                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Viewpoint                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool ViewpointGroup                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool Viewport                         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool VisibilitySensor                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool VolumeData                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool VolumeEmitter                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool VolumePickSensor                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool WindPhysicsModel                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool WorldInfo                        (JSContext* cx, unsigned argc, JS::Value* vp);

	// Abstract node types

	static bool X3DAnnotationNode                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DAppearanceChildNode             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DAppearanceNode                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DBackgroundNode                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DBindableNode                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DBoundedObject                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DChaserNode                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DChildNode                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DColorNode                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DComposableVolumeRenderStyleNode (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DComposedGeometryNode            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DCoordinateNode                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DDamperNode                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DDragSensorNode                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DEnvironmentalSensorNode         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DEnvironmentTextureNode          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DFogObject                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DFollowerNode                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DFontStyleNode                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DGeometricPropertyNode           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DGeometryNode                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DGeospatialObject                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DGroupingNode                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DInfoNode                        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DInterpolatorNode                (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DKeyDeviceSensorNode             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DLayerNode                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DLayoutNode                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DLightNode                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DMaterialNode                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DMetadataObject                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DNBodyCollidableNode             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DNBodyCollisionSpaceNode         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DNetworkSensorNode               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DNode                            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DNormalNode                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DNurbsControlCurveNode           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DNurbsSurfaceGeometryNode        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DParametricGeometryNode          (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DParticleEmitterNode             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DParticlePhysicsModelNode        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DPickableObject                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DPickSensorNode                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DPointingDeviceSensorNode        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DProductStructureChildNode       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DProgrammableShaderObject        (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DPrototypeInstance               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DRigidJointNode                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DScriptNode                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DSensorNode                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DSequencerNode                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DShaderNode                      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DShapeNode                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DSoundNode                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DSoundSourceNode                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTexture2DNode                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTexture3DNode                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTextureCoordinateNode           (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTextureNode                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTextureProjectorNode            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTextureTransformNode            (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTimeDependentNode               (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTouchSensorNode                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTransformMatrix3DObject         (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTransformNode                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DTriggerNode                     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DUrlObject                       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DVertexAttributeNode             (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DViewpointNode                   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DViewpointObject                 (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DViewportNode                    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DVolumeDataNode                  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool X3DVolumeRenderStyleNode           (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Non-standard

	static bool BlendMode (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Static Members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

} // spidermonkey
} // X3D
} // titania

#endif
