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

#include "X3DConstants.h"

#include "Arguments.h"
#include "Error.h"
#include "SlotType.h"

#include "../../Bits/X3DConstants.h"
#include "../../Configuration/UnitCategory.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps X3DConstants::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // enumerate
	nullptr, // newEnumerate
	nullptr, // resolve
	nullptr, // mayResolve
	nullptr, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass X3DConstants::static_class = {
	"X3DConstants",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec X3DConstants::properties [ ] = {

	// Event types

	JS_PSGS ("INITIALIZED_EVENT", INITIALIZED_EVENT, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SHUTDOWN_EVENT",    SHUTDOWN_EVENT,    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Error types

	JS_PSGS ("CONNECTION_ERROR",  CONNECTION_ERROR,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("INITIALIZED_ERROR", INITIALIZED_ERROR, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Load state

	JS_PSGS ("NOT_STARTED_STATE", NOT_STARTED_STATE, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IN_PROGRESS_STATE", IN_PROGRESS_STATE, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("COMPLETE_STATE",    COMPLETE_STATE,    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("FAILED_STATE",      FAILED_STATE,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Unit category

	JS_PSGS ("ANGLE",        ANGLE,        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("FORCE",        FORCE,        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LENGTH",       LENGTH,       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MASS",         MASS,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ACCELERATION", ACCELERATION, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ANGULAR_RATE", ANGULAR_RATE, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("AREA",         AREA,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SPEED",        SPEED,        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("VOLUME",       VOLUME,       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Access types

	JS_PSGS ("initializeOnly", initializeOnly, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("inputOnly",      inputOnly,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("outputOnly",     outputOnly,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("inputOutput",    inputOutput,    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Field types

	JS_PSGS ("SFBool",      SFBool,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFColor",     SFColor,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFColorRGBA", SFColorRGBA, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFDouble",    SFDouble,    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFFloat",     SFFloat,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFInt32",     SFInt32,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFImage",     SFImage,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFMatrix3d",  SFMatrix3d,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFMatrix3f",  SFMatrix3f,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFMatrix4d",  SFMatrix4d,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFMatrix4f",  SFMatrix4f,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFNode",      SFNode,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFRotation",  SFRotation,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFString",    SFString,    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFTime",      SFTime,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFVec2d",     SFVec2d,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFVec2f",     SFVec2f,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFVec3d",     SFVec3d,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFVec3f",     SFVec3f,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFVec4d",     SFVec4d,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SFVec4f",     SFVec4f,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	JS_PSGS ("MFBool",      MFBool,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFColor",     MFColor,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFColorRGBA", MFColorRGBA, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFDouble",    MFDouble,    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFFloat",     MFFloat,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFImage",     MFImage,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFInt32",     MFInt32,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFMatrix3d",  MFMatrix3d,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFMatrix3f",  MFMatrix3f,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFMatrix4d",  MFMatrix4d,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFMatrix4f",  MFMatrix4f,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFNode",      MFNode,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFRotation",  MFRotation,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFString",    MFString,    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFTime",      MFTime,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFVec2d",     MFVec2d,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFVec2f",     MFVec2f,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFVec3d",     MFVec3d,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFVec3f",     MFVec3f,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFVec4d",     MFVec4d,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MFVec4f",     MFVec4f,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Base node type

	JS_PSGS ("X3DBaseNode", X3DBaseNode, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Basic node types

	JS_PSGS ("Anchor",                           Anchor,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Appearance",                       Appearance,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Arc2D",                            Arc2D,                            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ArcClose2D",                       ArcClose2D,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("AudioClip",                        AudioClip,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Background",                       Background,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("BallJoint",                        BallJoint,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Billboard",                        Billboard,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("BlendedVolumeStyle",               BlendedVolumeStyle,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("BooleanFilter",                    BooleanFilter,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("BooleanSequencer",                 BooleanSequencer,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("BooleanToggle",                    BooleanToggle,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("BooleanTrigger",                   BooleanTrigger,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("BoundaryEnhancementVolumeStyle",   BoundaryEnhancementVolumeStyle,   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("BoundedPhysicsModel",              BoundedPhysicsModel,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Box",                              Box,                              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CADAssembly",                      CADAssembly,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CADFace",                          CADFace,                          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CADLayer",                         CADLayer,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CADPart",                          CADPart,                          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CartoonVolumeStyle",               CartoonVolumeStyle,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Circle2D",                         Circle2D,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ClipPlane",                        ClipPlane,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CollidableOffset",                 CollidableOffset,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CollidableShape",                  CollidableShape,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Collision",                        Collision,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CollisionCollection",              CollisionCollection,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CollisionSensor",                  CollisionSensor,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CollisionSpace",                   CollisionSpace,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Color",                            Color,                            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ColorChaser",                      ColorChaser,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ColorDamper",                      ColorDamper,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ColorInterpolator",                ColorInterpolator,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ColorRGBA",                        ColorRGBA,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ComposedCubeMapTexture",           ComposedCubeMapTexture,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ComposedShader",                   ComposedShader,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ComposedTexture3D",                ComposedTexture3D,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ComposedVolumeStyle",              ComposedVolumeStyle,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Cone",                             Cone,                             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ConeEmitter",                      ConeEmitter,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Contact",                          Contact,                          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Contour2D",                        Contour2D,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ContourPolyline2D",                ContourPolyline2D,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Coordinate",                       Coordinate,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CoordinateChaser",                 CoordinateChaser,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CoordinateDamper",                 CoordinateDamper,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CoordinateDouble",                 CoordinateDouble,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CoordinateInterpolator",           CoordinateInterpolator,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CoordinateInterpolator2D",         CoordinateInterpolator2D,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Cylinder",                         Cylinder,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("CylinderSensor",                   CylinderSensor,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("DISEntityManager",                 DISEntityManager,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("DISEntityTypeMapping",             DISEntityTypeMapping,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("DirectionalLight",                 DirectionalLight,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Disk2D",                           Disk2D,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("DoubleAxisHingeJoint",             DoubleAxisHingeJoint,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("EaseInEaseOut",                    EaseInEaseOut,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("EdgeEnhancementVolumeStyle",       EdgeEnhancementVolumeStyle,       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ElevationGrid",                    ElevationGrid,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("EspduTransform",                   EspduTransform,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ExplosionEmitter",                 ExplosionEmitter,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Extrusion",                        Extrusion,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("FillProperties",                   FillProperties,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("FloatVertexAttribute",             FloatVertexAttribute,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Fog",                              Fog,                              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("FogCoordinate",                    FogCoordinate,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("FontStyle",                        FontStyle,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ForcePhysicsModel",                ForcePhysicsModel,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeneratedCubeMapTexture",          GeneratedCubeMapTexture,          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoCoordinate",                    GeoCoordinate,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoElevationGrid",                 GeoElevationGrid,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoLOD",                           GeoLOD,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoLocation",                      GeoLocation,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoMetadata",                      GeoMetadata,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoOrigin",                        GeoOrigin,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoPositionInterpolator",          GeoPositionInterpolator,          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoProximitySensor",               GeoProximitySensor,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoTouchSensor",                   GeoTouchSensor,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoTransform",                     GeoTransform,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("GeoViewpoint",                     GeoViewpoint,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Group",                            Group,                            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("HAnimDisplacer",                   HAnimDisplacer,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("HAnimHumanoid",                    HAnimHumanoid,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("HAnimJoint",                       HAnimJoint,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("HAnimSegment",                     HAnimSegment,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("HAnimSite",                        HAnimSite,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ImageCubeMapTexture",              ImageCubeMapTexture,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ImageTexture",                     ImageTexture,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ImageTexture3D",                   ImageTexture3D,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IndexedFaceSet",                   IndexedFaceSet,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IndexedLineSet",                   IndexedLineSet,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IndexedQuadSet",                   IndexedQuadSet,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IndexedTriangleFanSet",            IndexedTriangleFanSet,            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IndexedTriangleSet",               IndexedTriangleSet,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IndexedTriangleStripSet",          IndexedTriangleStripSet,          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Inline",                           Inline,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IntegerSequencer",                 IntegerSequencer,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IntegerTrigger",                   IntegerTrigger,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("IsoSurfaceVolumeData",             IsoSurfaceVolumeData,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("KeySensor",                        KeySensor,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LOD",                              LOD,                              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Layer",                            Layer,                            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LayerSet",                         LayerSet,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Layout",                           Layout,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LayoutGroup",                      LayoutGroup,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LayoutLayer",                      LayoutLayer,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LinePickSensor",                   LinePickSensor,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LineProperties",                   LineProperties,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LineSet",                          LineSet,                          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LoadSensor",                       LoadSensor,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("LocalFog",                         LocalFog,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Material",                         Material,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Matrix3VertexAttribute",           Matrix3VertexAttribute,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Matrix4VertexAttribute",           Matrix4VertexAttribute,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MetadataBoolean",                  MetadataBoolean,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MetadataDouble",                   MetadataDouble,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MetadataFloat",                    MetadataFloat,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MetadataInteger",                  MetadataInteger,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MetadataSet",                      MetadataSet,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MetadataString",                   MetadataString,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MotorJoint",                       MotorJoint,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MovieTexture",                     MovieTexture,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MultiTexture",                     MultiTexture,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MultiTextureCoordinate",           MultiTextureCoordinate,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("MultiTextureTransform",            MultiTextureTransform,            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NavigationInfo",                   NavigationInfo,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Normal",                           Normal,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NormalInterpolator",               NormalInterpolator,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsCurve",                       NurbsCurve,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsCurve2D",                     NurbsCurve2D,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsOrientationInterpolator",     NurbsOrientationInterpolator,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsPatchSurface",                NurbsPatchSurface,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsPositionInterpolator",        NurbsPositionInterpolator,        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsSet",                         NurbsSet,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsSurfaceInterpolator",         NurbsSurfaceInterpolator,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsSweptSurface",                NurbsSweptSurface,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsSwungSurface",                NurbsSwungSurface,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsTextureCoordinate",           NurbsTextureCoordinate,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("NurbsTrimmedSurface",              NurbsTrimmedSurface,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("OpacityMapVolumeStyle",            OpacityMapVolumeStyle,            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("OrientationChaser",                OrientationChaser,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("OrientationDamper",                OrientationDamper,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("OrientationInterpolator",          OrientationInterpolator,          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("OrthoViewpoint",                   OrthoViewpoint,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PackagedShader",                   PackagedShader,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ParticleSystem",                   ParticleSystem,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PickableGroup",                    PickableGroup,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PixelTexture",                     PixelTexture,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PixelTexture3D",                   PixelTexture3D,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PlaneSensor",                      PlaneSensor,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PointEmitter",                     PointEmitter,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PointLight",                       PointLight,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PointPickSensor",                  PointPickSensor,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PointSet",                         PointSet,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Polyline2D",                       Polyline2D,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PolylineEmitter",                  PolylineEmitter,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Polypoint2D",                      Polypoint2D,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PositionChaser",                   PositionChaser,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PositionChaser2D",                 PositionChaser2D,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PositionDamper",                   PositionDamper,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PositionDamper2D",                 PositionDamper2D,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PositionInterpolator",             PositionInterpolator,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PositionInterpolator2D",           PositionInterpolator2D,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("PrimitivePickSensor",              PrimitivePickSensor,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ProgramShader",                    ProgramShader,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ProjectionVolumeStyle",            ProjectionVolumeStyle,            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ProximitySensor",                  ProximitySensor,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("QuadSet",                          QuadSet,                          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ReceiverPdu",                      ReceiverPdu,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Rectangle2D",                      Rectangle2D,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("RigidBody",                        RigidBody,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("RigidBodyCollection",              RigidBodyCollection,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ScalarChaser",                     ScalarChaser,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ScalarDamper",                     ScalarDamper,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ScalarInterpolator",               ScalarInterpolator,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ScreenFontStyle",                  ScreenFontStyle,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ScreenGroup",                      ScreenGroup,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Script",                           Script,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SegmentedVolumeData",              SegmentedVolumeData,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ShadedVolumeStyle",                ShadedVolumeStyle,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ShaderPart",                       ShaderPart,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ShaderProgram",                    ShaderProgram,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Shape",                            Shape,                            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SignalPdu",                        SignalPdu,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SilhouetteEnhancementVolumeStyle", SilhouetteEnhancementVolumeStyle, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SingleAxisHingeJoint",             SingleAxisHingeJoint,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SliderJoint",                      SliderJoint,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Sound",                            Sound,                            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Sphere",                           Sphere,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SphereSensor",                     SphereSensor,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SplinePositionInterpolator",       SplinePositionInterpolator,       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SplinePositionInterpolator2D",     SplinePositionInterpolator2D,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SplineScalarInterpolator",         SplineScalarInterpolator,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SpotLight",                        SpotLight,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SquadOrientationInterpolator",     SquadOrientationInterpolator,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("StaticGroup",                      StaticGroup,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("StringSensor",                     StringSensor,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("SurfaceEmitter",                   SurfaceEmitter,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Switch",                           Switch,                           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TexCoordChaser2D",                 TexCoordChaser2D,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TexCoordDamper2D",                 TexCoordDamper2D,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Text",                             Text,                             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureBackground",                TextureBackground,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureCoordinate",                TextureCoordinate,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureCoordinate3D",              TextureCoordinate3D,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureCoordinate4D",              TextureCoordinate4D,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureCoordinateGenerator",       TextureCoordinateGenerator,       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureProperties",                TextureProperties,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureTransform",                 TextureTransform,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureTransform3D",               TextureTransform3D,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TextureTransformMatrix3D",         TextureTransformMatrix3D,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TimeSensor",                       TimeSensor,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TimeTrigger",                      TimeTrigger,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ToneMappedVolumeStyle",            ToneMappedVolumeStyle,            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TouchGroup",                       TouchGroup,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TouchSensor",                      TouchSensor,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Transform",                        Transform,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TransformMatrix3D",                TransformMatrix3D,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TransformSensor",                  TransformSensor,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TransmitterPdu",                   TransmitterPdu,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TriangleFanSet",                   TriangleFanSet,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TriangleSet",                      TriangleSet,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TriangleSet2D",                    TriangleSet2D,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TriangleStripSet",                 TriangleStripSet,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("TwoSidedMaterial",                 TwoSidedMaterial,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("UniversalJoint",                   UniversalJoint,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Viewpoint",                        Viewpoint,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("ViewpointGroup",                   ViewpointGroup,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("Viewport",                         Viewport,                         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("VisibilitySensor",                 VisibilitySensor,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("VolumeData",                       VolumeData,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("VolumeEmitter",                    VolumeEmitter,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("VolumePickSensor",                 VolumePickSensor,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("WindPhysicsModel",                 WindPhysicsModel,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("WorldInfo",                        WorldInfo,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Abstract node types

	JS_PSGS ("X3DAppearanceChildNode",             X3DAppearanceChildNode,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DAppearanceNode",                  X3DAppearanceNode,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DBackgroundNode",                  X3DBackgroundNode,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DBindableNode",                    X3DBindableNode,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DBoundedObject",                   X3DBoundedObject,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DChaserNode",                      X3DChaserNode,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DChildNode",                       X3DChildNode,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DColorNode",                       X3DColorNode,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DComposableVolumeRenderStyleNode", X3DComposableVolumeRenderStyleNode, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DComposedGeometryNode",            X3DComposedGeometryNode,            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DCoordinateNode",                  X3DCoordinateNode,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DDamperNode",                      X3DDamperNode,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DDragSensorNode",                  X3DDragSensorNode,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DEnvironmentTextureNode",          X3DEnvironmentTextureNode,          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DEnvironmentalSensorNode",         X3DEnvironmentalSensorNode,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DFogObject",                       X3DFogObject,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DFollowerNode",                    X3DFollowerNode,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DFontStyleNode",                   X3DFontStyleNode,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DGeometricPropertyNode",           X3DGeometricPropertyNode,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DGeometryNode",                    X3DGeometryNode,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DGeospatialObject",                X3DGeospatialObject,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DGroupingNode",                    X3DGroupingNode,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DInfoNode",                        X3DInfoNode,                        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DInterpolatorNode",                X3DInterpolatorNode,                nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DKeyDeviceSensorNode",             X3DKeyDeviceSensorNode,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DLayerNode",                       X3DLayerNode,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DLayoutNode",                      X3DLayoutNode,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DLightNode",                       X3DLightNode,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DMaterialNode",                    X3DMaterialNode,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DMetadataObject",                  X3DMetadataObject,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DNBodyCollidableNode",             X3DNBodyCollidableNode,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DNBodyCollisionSpaceNode",         X3DNBodyCollisionSpaceNode,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DNetworkSensorNode",               X3DNetworkSensorNode,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DNode",                            X3DNode,                            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DNormalNode",                      X3DNormalNode,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DNurbsControlCurveNode",           X3DNurbsControlCurveNode,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DNurbsSurfaceGeometryNode",        X3DNurbsSurfaceGeometryNode,        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DParametricGeometryNode",          X3DParametricGeometryNode,          nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DParticleEmitterNode",             X3DParticleEmitterNode,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DParticlePhysicsModelNode",        X3DParticlePhysicsModelNode,        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DPickSensorNode",                  X3DPickSensorNode,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DPickableObject",                  X3DPickableObject,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DPointingDeviceSensorNode",        X3DPointingDeviceSensorNode,        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DProductStructureChildNode",       X3DProductStructureChildNode,       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DProgrammableShaderObject",        X3DProgrammableShaderObject,        nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DPrototypeInstance",               X3DPrototypeInstance,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DRigidJointNode",                  X3DRigidJointNode,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DScriptNode",                      X3DScriptNode,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DSensorNode",                      X3DSensorNode,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DSequencerNode",                   X3DSequencerNode,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DShaderNode",                      X3DShaderNode,                      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DShapeNode",                       X3DShapeNode,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DSoundNode",                       X3DSoundNode,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DSoundSourceNode",                 X3DSoundSourceNode,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTexture2DNode",                   X3DTexture2DNode,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTexture3DNode",                   X3DTexture3DNode,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTextureCoordinateNode",           X3DTextureCoordinateNode,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTextureNode",                     X3DTextureNode,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTextureTransformNode",            X3DTextureTransformNode,            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTimeDependentNode",               X3DTimeDependentNode,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTouchSensorNode",                 X3DTouchSensorNode,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTransformNode",                   X3DTransformNode,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DTriggerNode",                     X3DTriggerNode,                     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DUrlObject",                       X3DUrlObject,                       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DVertexAttributeNode",             X3DVertexAttributeNode,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DViewpointNode",                   X3DViewpointNode,                   nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DViewpointObject",                 X3DViewpointObject,                 nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DViewportNode",                    X3DViewportNode,                    nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DVolumeDataNode",                  X3DVolumeDataNode,                  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("X3DVolumeRenderStyleNode",           X3DVolumeRenderStyleNode,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	// Non-standard

	JS_PSGS ("BlendMode", BlendMode, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),

	JS_PS_END
};

const JSFunctionSpec X3DConstants::functions [ ] = {
	JS_FS_END
};

JSObject*
X3DConstants::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

bool
X3DConstants::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

JSObject*
X3DConstants::create (JSContext* const cx)
{
	const auto context = getContext (cx);
	const auto obj     = JS_NewObjectWithGivenProto (cx, getClass (), context -> getProto (getId ()));

	return obj;
}

// Event types

bool
X3DConstants::INITIALIZED_EVENT (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::INITIALIZED_EVENT));
	return true;
}

bool
X3DConstants::SHUTDOWN_EVENT (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SHUTDOWN_EVENT));
	return true;
}

// Error types

bool
X3DConstants::CONNECTION_ERROR (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CONNECTION_ERROR));
	return true;
}

bool
X3DConstants::INITIALIZED_ERROR (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::INITIALIZED_ERROR));
	return true;
}

// Load state

bool
X3DConstants::NOT_STARTED_STATE (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NOT_STARTED_STATE));
	return true;
}

bool
X3DConstants::IN_PROGRESS_STATE (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IN_PROGRESS_STATE));
	return true;
}

bool
X3DConstants::COMPLETE_STATE (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::COMPLETE_STATE));
	return true;
}

bool
X3DConstants::FAILED_STATE (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::FAILED_STATE));
	return true;
}

bool
X3DConstants::ANGLE (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::ANGLE));
	return true;
}

bool
X3DConstants::FORCE (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::FORCE));
	return true;
}

bool
X3DConstants::LENGTH (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::LENGTH));
	return true;
}

bool
X3DConstants::MASS (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::MASS));
	return true;
}

bool
X3DConstants::ACCELERATION (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::ACCELERATION));
	return true;
}

bool
X3DConstants::ANGULAR_RATE (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::ANGULAR_RATE));
	return true;
}

bool
X3DConstants::AREA (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::AREA));
	return true;
}

bool
X3DConstants::SPEED (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::SPEED));
	return true;
}

bool
X3DConstants::VOLUME (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::UnitCategory::VOLUME));
	return true;
}

// Access types

bool
X3DConstants::initializeOnly (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::initializeOnly));
	return true;
}

bool
X3DConstants::inputOnly (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::inputOnly));
	return true;
}

bool
X3DConstants::outputOnly (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::outputOnly));
	return true;
}

bool
X3DConstants::inputOutput (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::inputOutput));
	return true;
}

// X3D properties

bool
X3DConstants::SFBool (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFBool));
	return true;
}

bool
X3DConstants::SFColor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFColor));
	return true;
}

bool
X3DConstants::SFColorRGBA (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFColorRGBA));
	return true;
}

bool
X3DConstants::SFDouble (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFDouble));
	return true;
}

bool
X3DConstants::SFFloat (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFFloat));
	return true;
}

bool
X3DConstants::SFInt32 (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFInt32));
	return true;
}

bool
X3DConstants::SFImage (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFImage));
	return true;
}

bool
X3DConstants::SFMatrix3d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFMatrix3d));
	return true;
}

bool
X3DConstants::SFMatrix3f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFMatrix3f));
	return true;
}

bool
X3DConstants::SFMatrix4d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFMatrix4d));
	return true;
}

bool
X3DConstants::SFMatrix4f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFMatrix4f));
	return true;
}

bool
X3DConstants::SFNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFNode));
	return true;
}

bool
X3DConstants::SFRotation (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFRotation));
	return true;
}

bool
X3DConstants::SFString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFString));
	return true;
}

bool
X3DConstants::SFTime (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFTime));
	return true;
}

bool
X3DConstants::SFVec2d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFVec2d));
	return true;
}

bool
X3DConstants::SFVec2f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFVec2f));
	return true;
}

bool
X3DConstants::SFVec3d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFVec3d));
	return true;
}

bool
X3DConstants::SFVec3f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFVec3f));
	return true;
}

bool
X3DConstants::SFVec4d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFVec4d));
	return true;
}

bool
X3DConstants::SFVec4f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SFVec4f));
	return true;
}

bool
X3DConstants::MFBool (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFBool));
	return true;
}

bool
X3DConstants::MFColor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFColor));
	return true;
}

bool
X3DConstants::MFColorRGBA (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFColorRGBA));
	return true;
}

bool
X3DConstants::MFDouble (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFDouble));
	return true;
}

bool
X3DConstants::MFFloat (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFFloat));
	return true;
}

bool
X3DConstants::MFImage (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFImage));
	return true;
}

bool
X3DConstants::MFInt32 (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFInt32));
	return true;
}

bool
X3DConstants::MFMatrix3d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFMatrix3d));
	return true;
}

bool
X3DConstants::MFMatrix3f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFMatrix3f));
	return true;
}

bool
X3DConstants::MFMatrix4d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFMatrix4d));
	return true;
}

bool
X3DConstants::MFMatrix4f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFMatrix4f));
	return true;
}

bool
X3DConstants::MFNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFNode));
	return true;
}

bool
X3DConstants::MFRotation (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFRotation));
	return true;
}

bool
X3DConstants::MFString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFString));
	return true;
}

bool
X3DConstants::MFTime (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFTime));
	return true;
}

bool
X3DConstants::MFVec2d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFVec2d));
	return true;
}

bool
X3DConstants::MFVec2f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFVec2f));
	return true;
}

bool
X3DConstants::MFVec3d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFVec3d));
	return true;
}

bool
X3DConstants::MFVec3f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFVec3f));
	return true;
}

bool
X3DConstants::MFVec4d (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFVec4d));
	return true;
}

bool
X3DConstants::MFVec4f (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MFVec4f));
	return true;
}

// Base node type

bool
X3DConstants::X3DBaseNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DBaseNode));
	return true;
}

// Basic node types

bool
X3DConstants::Anchor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Anchor));
	return true;
}

bool
X3DConstants::Appearance (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Appearance));
	return true;
}

bool
X3DConstants::Arc2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Arc2D));
	return true;
}

bool
X3DConstants::ArcClose2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ArcClose2D));
	return true;
}

bool
X3DConstants::AudioClip (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::AudioClip));
	return true;
}

bool
X3DConstants::Background (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Background));
	return true;
}

bool
X3DConstants::BallJoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BallJoint));
	return true;
}

bool
X3DConstants::Billboard (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Billboard));
	return true;
}

bool
X3DConstants::BlendedVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BlendedVolumeStyle));
	return true;
}

bool
X3DConstants::BooleanFilter (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BooleanFilter));
	return true;
}

bool
X3DConstants::BooleanSequencer (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BooleanSequencer));
	return true;
}

bool
X3DConstants::BooleanToggle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BooleanToggle));
	return true;
}

bool
X3DConstants::BooleanTrigger (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BooleanTrigger));
	return true;
}

bool
X3DConstants::BoundaryEnhancementVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BoundaryEnhancementVolumeStyle));
	return true;
}

bool
X3DConstants::BoundedPhysicsModel (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BoundedPhysicsModel));
	return true;
}

bool
X3DConstants::Box (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Box));
	return true;
}

bool
X3DConstants::CADAssembly (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CADAssembly));
	return true;
}

bool
X3DConstants::CADFace (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CADFace));
	return true;
}

bool
X3DConstants::CADLayer (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CADLayer));
	return true;
}

bool
X3DConstants::CADPart (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CADPart));
	return true;
}

bool
X3DConstants::CartoonVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CartoonVolumeStyle));
	return true;
}

bool
X3DConstants::Circle2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Circle2D));
	return true;
}

bool
X3DConstants::ClipPlane (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ClipPlane));
	return true;
}

bool
X3DConstants::CollidableOffset (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CollidableOffset));
	return true;
}

bool
X3DConstants::CollidableShape (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CollidableShape));
	return true;
}

bool
X3DConstants::Collision (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Collision));
	return true;
}

bool
X3DConstants::CollisionCollection (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CollisionCollection));
	return true;
}

bool
X3DConstants::CollisionSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CollisionSensor));
	return true;
}

bool
X3DConstants::CollisionSpace (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CollisionSpace));
	return true;
}

bool
X3DConstants::Color (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Color));
	return true;
}

bool
X3DConstants::ColorChaser (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ColorChaser));
	return true;
}

bool
X3DConstants::ColorDamper (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ColorDamper));
	return true;
}

bool
X3DConstants::ColorInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ColorInterpolator));
	return true;
}

bool
X3DConstants::ColorRGBA (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ColorRGBA));
	return true;
}

bool
X3DConstants::ComposedCubeMapTexture (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ComposedCubeMapTexture));
	return true;
}

bool
X3DConstants::ComposedShader (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ComposedShader));
	return true;
}

bool
X3DConstants::ComposedTexture3D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ComposedTexture3D));
	return true;
}

bool
X3DConstants::ComposedVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ComposedVolumeStyle));
	return true;
}

bool
X3DConstants::Cone (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Cone));
	return true;
}

bool
X3DConstants::ConeEmitter (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ConeEmitter));
	return true;
}

bool
X3DConstants::Contact (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Contact));
	return true;
}

bool
X3DConstants::Contour2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Contour2D));
	return true;
}

bool
X3DConstants::ContourPolyline2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ContourPolyline2D));
	return true;
}

bool
X3DConstants::Coordinate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Coordinate));
	return true;
}

bool
X3DConstants::CoordinateChaser (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CoordinateChaser));
	return true;
}

bool
X3DConstants::CoordinateDamper (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CoordinateDamper));
	return true;
}

bool
X3DConstants::CoordinateDouble (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CoordinateDouble));
	return true;
}

bool
X3DConstants::CoordinateInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CoordinateInterpolator));
	return true;
}

bool
X3DConstants::CoordinateInterpolator2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CoordinateInterpolator2D));
	return true;
}

bool
X3DConstants::Cylinder (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Cylinder));
	return true;
}

bool
X3DConstants::CylinderSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::CylinderSensor));
	return true;
}

bool
X3DConstants::DISEntityManager (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::DISEntityManager));
	return true;
}

bool
X3DConstants::DISEntityTypeMapping (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::DISEntityTypeMapping));
	return true;
}

bool
X3DConstants::DirectionalLight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::DirectionalLight));
	return true;
}

bool
X3DConstants::Disk2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Disk2D));
	return true;
}

bool
X3DConstants::DoubleAxisHingeJoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::DoubleAxisHingeJoint));
	return true;
}

bool
X3DConstants::EaseInEaseOut (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::EaseInEaseOut));
	return true;
}

bool
X3DConstants::EdgeEnhancementVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::EdgeEnhancementVolumeStyle));
	return true;
}

bool
X3DConstants::ElevationGrid (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ElevationGrid));
	return true;
}

bool
X3DConstants::EspduTransform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::EspduTransform));
	return true;
}

bool
X3DConstants::ExplosionEmitter (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ExplosionEmitter));
	return true;
}

bool
X3DConstants::Extrusion (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Extrusion));
	return true;
}

bool
X3DConstants::FillProperties (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::FillProperties));
	return true;
}

bool
X3DConstants::FloatVertexAttribute (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::FloatVertexAttribute));
	return true;
}

bool
X3DConstants::Fog (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Fog));
	return true;
}

bool
X3DConstants::FogCoordinate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::FogCoordinate));
	return true;
}

bool
X3DConstants::FontStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::FontStyle));
	return true;
}

bool
X3DConstants::ForcePhysicsModel (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ForcePhysicsModel));
	return true;
}

bool
X3DConstants::GeneratedCubeMapTexture (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeneratedCubeMapTexture));
	return true;
}

bool
X3DConstants::GeoCoordinate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoCoordinate));
	return true;
}

bool
X3DConstants::GeoElevationGrid (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoElevationGrid));
	return true;
}

bool
X3DConstants::GeoLOD (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoLOD));
	return true;
}

bool
X3DConstants::GeoLocation (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoLocation));
	return true;
}

bool
X3DConstants::GeoMetadata (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoMetadata));
	return true;
}

bool
X3DConstants::GeoOrigin (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoOrigin));
	return true;
}

bool
X3DConstants::GeoPositionInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoPositionInterpolator));
	return true;
}

bool
X3DConstants::GeoProximitySensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoProximitySensor));
	return true;
}

bool
X3DConstants::GeoTouchSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoTouchSensor));
	return true;
}

bool
X3DConstants::GeoTransform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoTransform));
	return true;
}

bool
X3DConstants::GeoViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::GeoViewpoint));
	return true;
}

bool
X3DConstants::Group (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Group));
	return true;
}

bool
X3DConstants::HAnimDisplacer (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::HAnimDisplacer));
	return true;
}

bool
X3DConstants::HAnimHumanoid (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::HAnimHumanoid));
	return true;
}

bool
X3DConstants::HAnimJoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::HAnimJoint));
	return true;
}

bool
X3DConstants::HAnimMotion (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::HAnimMotion));
	return true;
}

bool
X3DConstants::HAnimSegment (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::HAnimSegment));
	return true;
}

bool
X3DConstants::HAnimSite (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::HAnimSite));
	return true;
}

bool
X3DConstants::ImageCubeMapTexture (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ImageCubeMapTexture));
	return true;
}

bool
X3DConstants::ImageTexture (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ImageTexture));
	return true;
}

bool
X3DConstants::ImageTexture3D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ImageTexture3D));
	return true;
}

bool
X3DConstants::IndexedFaceSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IndexedFaceSet));
	return true;
}

bool
X3DConstants::IndexedLineSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IndexedLineSet));
	return true;
}

bool
X3DConstants::IndexedQuadSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IndexedQuadSet));
	return true;
}

bool
X3DConstants::IndexedTriangleFanSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IndexedTriangleFanSet));
	return true;
}

bool
X3DConstants::IndexedTriangleSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IndexedTriangleSet));
	return true;
}

bool
X3DConstants::IndexedTriangleStripSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IndexedTriangleStripSet));
	return true;
}

bool
X3DConstants::Inline (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Inline));
	return true;
}

bool
X3DConstants::IntegerSequencer (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IntegerSequencer));
	return true;
}

bool
X3DConstants::IntegerTrigger (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IntegerTrigger));
	return true;
}

bool
X3DConstants::IsoSurfaceVolumeData (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::IsoSurfaceVolumeData));
	return true;
}

bool
X3DConstants::KeySensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::KeySensor));
	return true;
}

bool
X3DConstants::LOD (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LOD));
	return true;
}

bool
X3DConstants::Layer (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Layer));
	return true;
}

bool
X3DConstants::LayerSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LayerSet));
	return true;
}

bool
X3DConstants::Layout (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Layout));
	return true;
}

bool
X3DConstants::LayoutGroup (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LayoutGroup));
	return true;
}

bool
X3DConstants::LayoutLayer (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LayoutLayer));
	return true;
}

bool
X3DConstants::LinePickSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LinePickSensor));
	return true;
}

bool
X3DConstants::LineProperties (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LineProperties));
	return true;
}

bool
X3DConstants::LineSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LineSet));
	return true;
}

bool
X3DConstants::LoadSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LoadSensor));
	return true;
}

bool
X3DConstants::LocalFog (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::LocalFog));
	return true;
}

bool
X3DConstants::Material (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Material));
	return true;
}

bool
X3DConstants::Matrix3VertexAttribute (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Matrix3VertexAttribute));
	return true;
}

bool
X3DConstants::Matrix4VertexAttribute (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Matrix4VertexAttribute));
	return true;
}

bool
X3DConstants::MetadataBoolean (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MetadataBoolean));
	return true;
}

bool
X3DConstants::MetadataDouble (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MetadataDouble));
	return true;
}

bool
X3DConstants::MetadataFloat (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MetadataFloat));
	return true;
}

bool
X3DConstants::MetadataInteger (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MetadataInteger));
	return true;
}

bool
X3DConstants::MetadataSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MetadataSet));
	return true;
}

bool
X3DConstants::MetadataString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MetadataString));
	return true;
}

bool
X3DConstants::MotorJoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MotorJoint));
	return true;
}

bool
X3DConstants::MovieTexture (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MovieTexture));
	return true;
}

bool
X3DConstants::MultiTexture (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MultiTexture));
	return true;
}

bool
X3DConstants::MultiTextureCoordinate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MultiTextureCoordinate));
	return true;
}

bool
X3DConstants::MultiTextureTransform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::MultiTextureTransform));
	return true;
}

bool
X3DConstants::NavigationInfo (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NavigationInfo));
	return true;
}

bool
X3DConstants::Normal (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Normal));
	return true;
}

bool
X3DConstants::NormalInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NormalInterpolator));
	return true;
}

bool
X3DConstants::NurbsCurve (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsCurve));
	return true;
}

bool
X3DConstants::NurbsCurve2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsCurve2D));
	return true;
}

bool
X3DConstants::NurbsOrientationInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsOrientationInterpolator));
	return true;
}

bool
X3DConstants::NurbsPatchSurface (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsPatchSurface));
	return true;
}

bool
X3DConstants::NurbsPositionInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsPositionInterpolator));
	return true;
}

bool
X3DConstants::NurbsSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsSet));
	return true;
}

bool
X3DConstants::NurbsSurfaceInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsSurfaceInterpolator));
	return true;
}

bool
X3DConstants::NurbsSweptSurface (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsSweptSurface));
	return true;
}

bool
X3DConstants::NurbsSwungSurface (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsSwungSurface));
	return true;
}

bool
X3DConstants::NurbsTextureCoordinate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsTextureCoordinate));
	return true;
}

bool
X3DConstants::NurbsTrimmedSurface (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::NurbsTrimmedSurface));
	return true;
}

bool
X3DConstants::OpacityMapVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::OpacityMapVolumeStyle));
	return true;
}

bool
X3DConstants::OrientationChaser (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::OrientationChaser));
	return true;
}

bool
X3DConstants::OrientationDamper (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::OrientationDamper));
	return true;
}

bool
X3DConstants::OrientationInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::OrientationInterpolator));
	return true;
}

bool
X3DConstants::OrthoViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::OrthoViewpoint));
	return true;
}

bool
X3DConstants::PackagedShader (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PackagedShader));
	return true;
}

bool
X3DConstants::ParticleSystem (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ParticleSystem));
	return true;
}

bool
X3DConstants::PickableGroup (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PickableGroup));
	return true;
}

bool
X3DConstants::PixelTexture (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PixelTexture));
	return true;
}

bool
X3DConstants::PixelTexture3D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PixelTexture3D));
	return true;
}

bool
X3DConstants::PlaneSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PlaneSensor));
	return true;
}

bool
X3DConstants::PointEmitter (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PointEmitter));
	return true;
}

bool
X3DConstants::PointLight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PointLight));
	return true;
}

bool
X3DConstants::PointPickSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PointPickSensor));
	return true;
}

bool
X3DConstants::PointProperties (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PointProperties));
	return true;
}

bool
X3DConstants::PointSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PointSet));
	return true;
}

bool
X3DConstants::Polyline2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Polyline2D));
	return true;
}

bool
X3DConstants::PolylineEmitter (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PolylineEmitter));
	return true;
}

bool
X3DConstants::Polypoint2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Polypoint2D));
	return true;
}

bool
X3DConstants::PositionChaser (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PositionChaser));
	return true;
}

bool
X3DConstants::PositionChaser2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PositionChaser2D));
	return true;
}

bool
X3DConstants::PositionDamper (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PositionDamper));
	return true;
}

bool
X3DConstants::PositionDamper2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PositionDamper2D));
	return true;
}

bool
X3DConstants::PositionInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PositionInterpolator));
	return true;
}

bool
X3DConstants::PositionInterpolator2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PositionInterpolator2D));
	return true;
}

bool
X3DConstants::PrimitivePickSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::PrimitivePickSensor));
	return true;
}

bool
X3DConstants::ProgramShader (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ProgramShader));
	return true;
}

bool
X3DConstants::ProjectionVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ProjectionVolumeStyle));
	return true;
}

bool
X3DConstants::ProximitySensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ProximitySensor));
	return true;
}

bool
X3DConstants::QuadSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::QuadSet));
	return true;
}

bool
X3DConstants::ReceiverPdu (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ReceiverPdu));
	return true;
}

bool
X3DConstants::Rectangle2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Rectangle2D));
	return true;
}

bool
X3DConstants::RigidBody (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::RigidBody));
	return true;
}

bool
X3DConstants::RigidBodyCollection (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::RigidBodyCollection));
	return true;
}

bool
X3DConstants::ScalarChaser (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ScalarChaser));
	return true;
}

bool
X3DConstants::ScalarDamper (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ScalarDamper));
	return true;
}

bool
X3DConstants::ScalarInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ScalarInterpolator));
	return true;
}

bool
X3DConstants::ScreenFontStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ScreenFontStyle));
	return true;
}

bool
X3DConstants::ScreenGroup (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ScreenGroup));
	return true;
}

bool
X3DConstants::Script (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Script));
	return true;
}

bool
X3DConstants::SegmentedVolumeData (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SegmentedVolumeData));
	return true;
}

bool
X3DConstants::ShadedVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ShadedVolumeStyle));
	return true;
}

bool
X3DConstants::ShaderPart (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ShaderPart));
	return true;
}

bool
X3DConstants::ShaderProgram (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ShaderProgram));
	return true;
}

bool
X3DConstants::Shape (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Shape));
	return true;
}

bool
X3DConstants::SignalPdu (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SignalPdu));
	return true;
}

bool
X3DConstants::SilhouetteEnhancementVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SilhouetteEnhancementVolumeStyle));
	return true;
}

bool
X3DConstants::SingleAxisHingeJoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SingleAxisHingeJoint));
	return true;
}

bool
X3DConstants::SliderJoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SliderJoint));
	return true;
}

bool
X3DConstants::Sound (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Sound));
	return true;
}

bool
X3DConstants::Sphere (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Sphere));
	return true;
}

bool
X3DConstants::SphereSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SphereSensor));
	return true;
}

bool
X3DConstants::SplinePositionInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SplinePositionInterpolator));
	return true;
}

bool
X3DConstants::SplinePositionInterpolator2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SplinePositionInterpolator2D));
	return true;
}

bool
X3DConstants::SplineScalarInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SplineScalarInterpolator));
	return true;
}

bool
X3DConstants::SpotLight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SpotLight));
	return true;
}

bool
X3DConstants::SquadOrientationInterpolator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SquadOrientationInterpolator));
	return true;
}

bool
X3DConstants::StaticGroup (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::StaticGroup));
	return true;
}

bool
X3DConstants::StringSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::StringSensor));
	return true;
}

bool
X3DConstants::SurfaceEmitter (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::SurfaceEmitter));
	return true;
}

bool
X3DConstants::Switch (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Switch));
	return true;
}

bool
X3DConstants::TexCoordChaser2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TexCoordChaser2D));
	return true;
}

bool
X3DConstants::TexCoordDamper2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TexCoordDamper2D));
	return true;
}

bool
X3DConstants::Text (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Text));
	return true;
}

bool
X3DConstants::TextureBackground (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureBackground));
	return true;
}

bool
X3DConstants::TextureCoordinate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureCoordinate));
	return true;
}

bool
X3DConstants::TextureCoordinate3D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureCoordinate3D));
	return true;
}

bool
X3DConstants::TextureCoordinate4D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureCoordinate4D));
	return true;
}

bool
X3DConstants::TextureCoordinateGenerator (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureCoordinateGenerator));
	return true;
}

bool
X3DConstants::TextureProperties (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureProperties));
	return true;
}

bool
X3DConstants::TextureTransform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureTransform));
	return true;
}

bool
X3DConstants::TextureTransform3D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureTransform3D));
	return true;
}

bool
X3DConstants::TextureTransformMatrix3D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TextureTransformMatrix3D));
	return true;
}

bool
X3DConstants::TimeSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TimeSensor));
	return true;
}

bool
X3DConstants::TimeTrigger (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TimeTrigger));
	return true;
}

bool
X3DConstants::ToneMappedVolumeStyle (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ToneMappedVolumeStyle));
	return true;
}

bool
X3DConstants::TouchGroup (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TouchGroup));
	return true;
}

bool
X3DConstants::TouchSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TouchSensor));
	return true;
}

bool
X3DConstants::Transform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Transform));
	return true;
}

bool
X3DConstants::TransformMatrix3D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TransformMatrix3D));
	return true;
}

bool
X3DConstants::TransformSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TransformSensor));
	return true;
}

bool
X3DConstants::TransmitterPdu (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TransmitterPdu));
	return true;
}

bool
X3DConstants::TriangleFanSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TriangleFanSet));
	return true;
}

bool
X3DConstants::TriangleSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TriangleSet));
	return true;
}

bool
X3DConstants::TriangleSet2D (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TriangleSet2D));
	return true;
}

bool
X3DConstants::TriangleStripSet (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TriangleStripSet));
	return true;
}

bool
X3DConstants::TwoSidedMaterial (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::TwoSidedMaterial));
	return true;
}

bool
X3DConstants::UniversalJoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::UniversalJoint));
	return true;
}

bool
X3DConstants::Viewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Viewpoint));
	return true;
}

bool
X3DConstants::ViewpointGroup (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::ViewpointGroup));
	return true;
}

bool
X3DConstants::Viewport (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::Viewport));
	return true;
}

bool
X3DConstants::VisibilitySensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::VisibilitySensor));
	return true;
}

bool
X3DConstants::VolumeData (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::VolumeData));
	return true;
}

bool
X3DConstants::VolumeEmitter (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::VolumeEmitter));
	return true;
}

bool
X3DConstants::VolumePickSensor (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::VolumePickSensor));
	return true;
}

bool
X3DConstants::WindPhysicsModel (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::WindPhysicsModel));
	return true;
}

bool
X3DConstants::WorldInfo (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::WorldInfo));
	return true;
}

// Abstract node types

bool
X3DConstants::X3DAppearanceChildNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DAppearanceChildNode));
	return true;
}

bool
X3DConstants::X3DAppearanceNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DAppearanceNode));
	return true;
}

bool
X3DConstants::X3DBackgroundNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DBackgroundNode));
	return true;
}

bool
X3DConstants::X3DBindableNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DBindableNode));
	return true;
}

bool
X3DConstants::X3DBoundedObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DBoundedObject));
	return true;
}

bool
X3DConstants::X3DChaserNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DChaserNode));
	return true;
}

bool
X3DConstants::X3DChildNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DChildNode));
	return true;
}

bool
X3DConstants::X3DColorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DColorNode));
	return true;
}

bool
X3DConstants::X3DComposableVolumeRenderStyleNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DComposableVolumeRenderStyleNode));
	return true;
}

bool
X3DConstants::X3DComposedGeometryNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DComposedGeometryNode));
	return true;
}

bool
X3DConstants::X3DCoordinateNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DCoordinateNode));
	return true;
}

bool
X3DConstants::X3DDamperNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DDamperNode));
	return true;
}

bool
X3DConstants::X3DDragSensorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DDragSensorNode));
	return true;
}

bool
X3DConstants::X3DEnvironmentTextureNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DEnvironmentTextureNode));
	return true;
}

bool
X3DConstants::X3DEnvironmentalSensorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DEnvironmentalSensorNode));
	return true;
}

bool
X3DConstants::X3DFogObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DFogObject));
	return true;
}

bool
X3DConstants::X3DFollowerNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DFollowerNode));
	return true;
}

bool
X3DConstants::X3DFontStyleNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DFontStyleNode));
	return true;
}

bool
X3DConstants::X3DGeometricPropertyNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DGeometricPropertyNode));
	return true;
}

bool
X3DConstants::X3DGeometryNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DGeometryNode));
	return true;
}

bool
X3DConstants::X3DGeospatialObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DGeospatialObject));
	return true;
}

bool
X3DConstants::X3DGroupingNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DGroupingNode));
	return true;
}

bool
X3DConstants::X3DInfoNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DInfoNode));
	return true;
}

bool
X3DConstants::X3DInterpolatorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DInterpolatorNode));
	return true;
}

bool
X3DConstants::X3DKeyDeviceSensorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DKeyDeviceSensorNode));
	return true;
}

bool
X3DConstants::X3DLayerNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DLayerNode));
	return true;
}

bool
X3DConstants::X3DLayoutNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DLayoutNode));
	return true;
}

bool
X3DConstants::X3DLightNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DLightNode));
	return true;
}

bool
X3DConstants::X3DMaterialNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DMaterialNode));
	return true;
}

bool
X3DConstants::X3DMetadataObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DMetadataObject));
	return true;
}

bool
X3DConstants::X3DNBodyCollidableNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DNBodyCollidableNode));
	return true;
}

bool
X3DConstants::X3DNBodyCollisionSpaceNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DNBodyCollisionSpaceNode));
	return true;
}

bool
X3DConstants::X3DNetworkSensorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DNetworkSensorNode));
	return true;
}

bool
X3DConstants::X3DNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DNode));
	return true;
}

bool
X3DConstants::X3DNormalNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DNormalNode));
	return true;
}

bool
X3DConstants::X3DNurbsControlCurveNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DNurbsControlCurveNode));
	return true;
}

bool
X3DConstants::X3DNurbsSurfaceGeometryNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DNurbsSurfaceGeometryNode));
	return true;
}

bool
X3DConstants::X3DParametricGeometryNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DParametricGeometryNode));
	return true;
}

bool
X3DConstants::X3DParticleEmitterNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DParticleEmitterNode));
	return true;
}

bool
X3DConstants::X3DParticlePhysicsModelNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DParticlePhysicsModelNode));
	return true;
}

bool
X3DConstants::X3DPickSensorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DPickSensorNode));
	return true;
}

bool
X3DConstants::X3DPickableObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DPickableObject));
	return true;
}

bool
X3DConstants::X3DPointingDeviceSensorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DPointingDeviceSensorNode));
	return true;
}

bool
X3DConstants::X3DProductStructureChildNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DProductStructureChildNode));
	return true;
}

bool
X3DConstants::X3DProgrammableShaderObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DProgrammableShaderObject));
	return true;
}

bool
X3DConstants::X3DPrototypeInstance (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DPrototypeInstance));
	return true;
}

bool
X3DConstants::X3DRigidJointNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DRigidJointNode));
	return true;
}

bool
X3DConstants::X3DScriptNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DScriptNode));
	return true;
}

bool
X3DConstants::X3DSensorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DSensorNode));
	return true;
}

bool
X3DConstants::X3DSequencerNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DSequencerNode));
	return true;
}

bool
X3DConstants::X3DShaderNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DShaderNode));
	return true;
}

bool
X3DConstants::X3DShapeNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DShapeNode));
	return true;
}

bool
X3DConstants::X3DSoundNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DSoundNode));
	return true;
}

bool
X3DConstants::X3DSoundSourceNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DSoundSourceNode));
	return true;
}

bool
X3DConstants::X3DTexture2DNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTexture2DNode));
	return true;
}

bool
X3DConstants::X3DTexture3DNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTexture3DNode));
	return true;
}

bool
X3DConstants::X3DTextureCoordinateNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTextureCoordinateNode));
	return true;
}

bool
X3DConstants::X3DTextureNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTextureNode));
	return true;
}

bool
X3DConstants::X3DTextureTransformNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTextureTransformNode));
	return true;
}

bool
X3DConstants::X3DTimeDependentNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTimeDependentNode));
	return true;
}

bool
X3DConstants::X3DTouchSensorNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTouchSensorNode));
	return true;
}

bool
X3DConstants::X3DTransformMatrix3DObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTransformMatrix3DObject));
	return true;
}

bool
X3DConstants::X3DTransformNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTransformNode));
	return true;
}

bool
X3DConstants::X3DTriggerNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DTriggerNode));
	return true;
}

bool
X3DConstants::X3DUrlObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DUrlObject));
	return true;
}

bool
X3DConstants::X3DVertexAttributeNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DVertexAttributeNode));
	return true;
}

bool
X3DConstants::X3DViewpointNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DViewpointNode));
	return true;
}

bool
X3DConstants::X3DViewpointObject (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DViewpointObject));
	return true;
}

bool
X3DConstants::X3DViewportNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DViewportNode));
	return true;
}

bool
X3DConstants::X3DVolumeDataNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DVolumeDataNode));
	return true;
}

bool
X3DConstants::X3DVolumeRenderStyleNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::X3DVolumeRenderStyleNode));
	return true;
}

// Non-standard

bool
X3DConstants::BlendMode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args = JS::CallArgsFromVp (argc, vp);
	args .rval () .setNumber (uint32_t (X3D::X3DConstants::BlendMode));
	return true;
}

} // spidermonkey
} // X3D
} // titania
