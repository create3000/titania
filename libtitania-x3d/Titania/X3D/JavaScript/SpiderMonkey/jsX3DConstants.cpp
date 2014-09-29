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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "jsX3DConstants.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Scripting/Script.h"
#include "../../InputOutput/Generator.h"
#include "Fields/jsMFNode.h"
#include "Fields/jsMFString.h"
#include "Fields/jsSFNode.h"
#include "jsComponentInfoArray.h"
#include "jsContext.h"
#include "jsProfileInfoArray.h"
#include "jsString.h"
#include "jsX3DExecutionContext.h"
#include "jsX3DScene.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsX3DConstants::static_class = {
	"X3DConstants", 0,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DConstants::properties [ ] = {
	// Event types

	{ "INITIALIZED_EVENT", JS_INITIALIZED_EVENT, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, INITIALIZED_EVENT, NULL },
	{ "SHUTDOWN_EVENT",    JS_SHUTDOWN_EVENT,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SHUTDOWN_EVENT,    NULL },

	// Error types

	{ "CONNECTION_ERROR",  JS_CONNECTION_ERROR,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CONNECTION_ERROR,  NULL },
	{ "INITIALIZED_ERROR", JS_INITIALIZED_ERROR, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, INITIALIZED_ERROR, NULL },

	// Load state

	{ "NOT_STARTED_STATE", JS_NOT_STARTED_STATE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NOT_STARTED_STATE, NULL },
	{ "IN_PROGRESS_STATE", JS_IN_PROGRESS_STATE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IN_PROGRESS_STATE, NULL },
	{ "COMPLETE_STATE",    JS_COMPLETE_STATE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, COMPLETE_STATE,    NULL },
	{ "FAILED_STATE",      JS_FAILED_STATE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FAILED_STATE,      NULL },

	// Access types

	{ "initializeOnly", JS_INITIALIZE_ONLY, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, initializeOnly, NULL },
	{ "inputOnly",      JS_INPUT_ONLY,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, inputOnly,      NULL },
	{ "outputOnly",     JS_OUTPUT_ONLY,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, outputOnly,     NULL },
	{ "inputOutput",    JS_INPUT_OUTPUT,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, inputOutput,    NULL },

	// Field types

	{ "SFBool",      JS_SFBOOL,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFBool,      NULL },
	{ "SFColor",     JS_SFCOLOR,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFColor,     NULL },
	{ "SFColorRGBA", JS_SFCOLORRGBA, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFColorRGBA, NULL },
	{ "SFDouble",    JS_SFDOUBLE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFDouble,    NULL },
	{ "SFFloat",     JS_SFFLOAT,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFFloat,     NULL },
	{ "SFInt32",     JS_SFINT32,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFInt32,     NULL },
	{ "SFImage",     JS_SFIMAGE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFImage,     NULL },
	{ "SFMatrix3d",  JS_SFMATRIX3D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix3d,  NULL },
	{ "SFMatrix3f",  JS_SFMATRIX3F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix3f,  NULL },
	{ "SFMatrix4d",  JS_SFMATRIX4D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix4d,  NULL },
	{ "SFMatrix4f",  JS_SFMATRIX4F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix4f,  NULL },
	{ "SFNode",      JS_SFNODE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFNode,      NULL },
	{ "SFRotation",  JS_SFROTATION,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFRotation,  NULL },
	{ "SFString",    JS_SFSTRING,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFString,    NULL },
	{ "SFTime",      JS_SFTIME,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFTime,      NULL },
	{ "SFVec2d",     JS_SFVEC2D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec2d,     NULL },
	{ "SFVec2f",     JS_SFVEC2F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec2f,     NULL },
	{ "SFVec3d",     JS_SFVEC3D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec3d,     NULL },
	{ "SFVec3f",     JS_SFVEC3F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec3f,     NULL },
	{ "SFVec4d",     JS_SFVEC4D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec4d,     NULL },
	{ "SFVec4f",     JS_SFVEC4F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec4f,     NULL },

	{ "MFBool",      JS_MFBOOL,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFBool,      NULL },
	{ "MFColor",     JS_MFCOLOR,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFColor,     NULL },
	{ "MFColorRGBA", JS_MFCOLORRGBA, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFColorRGBA, NULL },
	{ "MFDouble",    JS_MFDOUBLE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFDouble,    NULL },
	{ "MFFloat",     JS_MFFLOAT,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFFloat,     NULL },
	{ "MFImage",     JS_MFIMAGE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFImage,     NULL },
	{ "MFInt32",     JS_MFINT32,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFInt32,     NULL },
	{ "MFMatrix3d",  JS_MFMATRIX3D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix3d,  NULL },
	{ "MFMatrix3f",  JS_MFMATRIX3F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix3f,  NULL },
	{ "MFMatrix4d",  JS_MFMATRIX4D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix4d,  NULL },
	{ "MFMatrix4f",  JS_MFMATRIX4F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix4f,  NULL },
	{ "MFNode",      JS_MFNODE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFNode,      NULL },
	{ "MFRotation",  JS_MFROTATION,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFRotation,  NULL },
	{ "MFString",    JS_MFSTRING,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFString,    NULL },
	{ "MFTime",      JS_MFTIME,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFTime,      NULL },
	{ "MFVec2d",     JS_MFVEC2D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec2d,     NULL },
	{ "MFVec2f",     JS_MFVEC2F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec2f,     NULL },
	{ "MFVec3d",     JS_MFVEC3D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec3d,     NULL },
	{ "MFVec3f",     JS_MFVEC3F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec3f,     NULL },
	{ "MFVec4d",     JS_MFVEC4D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec4d,     NULL },
	{ "MFVec4f",     JS_MFVEC4F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec4f,     NULL },

	// Base node type

	{ "X3DBaseNode", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBaseNode, NULL },

	// Basic node types

	{ "Anchor",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Anchor,                       NULL },
	{ "Appearance",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Appearance,                   NULL },
	{ "Arc2D",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Arc2D,                        NULL },
	{ "ArcClose2D",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ArcClose2D,                   NULL },
	{ "AudioClip",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, AudioClip,                    NULL },
	{ "Background",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Background,                   NULL },
	{ "BallJoint",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BallJoint,                    NULL },
	{ "Billboard",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Billboard,                    NULL },
	{ "BooleanFilter",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BooleanFilter,                NULL },
	{ "BooleanSequencer",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BooleanSequencer,             NULL },
	{ "BooleanToggle",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BooleanToggle,                NULL },
	{ "BooleanTrigger",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BooleanTrigger,               NULL },
	{ "BoundedPhysicsModel",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BoundedPhysicsModel,          NULL },
	{ "Box",                          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Box,                          NULL },
	{ "CADAssembly",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CADAssembly,                  NULL },
	{ "CADFace",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CADFace,                      NULL },
	{ "CADLayer",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CADLayer,                     NULL },
	{ "CADPart",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CADPart,                      NULL },
	{ "Circle2D",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Circle2D,                     NULL },
	{ "ClipPlane",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ClipPlane,                    NULL },
	{ "CollidableOffset",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollidableOffset,             NULL },
	{ "CollidableShape",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollidableShape,              NULL },
	{ "Collision",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Collision,                    NULL },
	{ "CollisionCollection",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollisionCollection,          NULL },
	{ "CollisionSensor",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollisionSensor,              NULL },
	{ "CollisionSpace",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollisionSpace,               NULL },
	{ "Color",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Color,                        NULL },
	{ "ColorChaser",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ColorChaser,                  NULL },
	{ "ColorDamper",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ColorDamper,                  NULL },
	{ "ColorInterpolator",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ColorInterpolator,            NULL },
	{ "ColorRGBA",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ColorRGBA,                    NULL },
	{ "ComposedCubeMapTexture",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ComposedCubeMapTexture,       NULL },
	{ "ComposedShader",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ComposedShader,               NULL },
	{ "ComposedTexture3D",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ComposedTexture3D,            NULL },
	{ "Cone",                         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Cone,                         NULL },
	{ "ConeEmitter",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ConeEmitter,                  NULL },
	{ "Contact",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Contact,                      NULL },
	{ "Contour2D",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Contour2D,                    NULL },
	{ "ContourPolyline2D",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ContourPolyline2D,            NULL },
	{ "Coordinate",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Coordinate,                   NULL },
	{ "CoordinateChaser",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateChaser,             NULL },
	{ "CoordinateDamper",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateDamper,             NULL },
	{ "CoordinateDouble",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateDouble,             NULL },
	{ "CoordinateInterpolator",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateInterpolator,       NULL },
	{ "CoordinateInterpolator2D",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateInterpolator2D,     NULL },
	{ "Cylinder",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Cylinder,                     NULL },
	{ "CylinderSensor",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CylinderSensor,               NULL },
	{ "DISEntityManager",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, DISEntityManager,             NULL },
	{ "DISEntityTypeMapping",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, DISEntityTypeMapping,         NULL },
	{ "DirectionalLight",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, DirectionalLight,             NULL },
	{ "Disk2D",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Disk2D,                       NULL },
	{ "DoubleAxisHingeJoint",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, DoubleAxisHingeJoint,         NULL },
	{ "EaseInEaseOut",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, EaseInEaseOut,                NULL },
	{ "ElevationGrid",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ElevationGrid,                NULL },
	{ "EspduTransform",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, EspduTransform,               NULL },
	{ "ExplosionEmitter",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ExplosionEmitter,             NULL },
	{ "Extrusion",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Extrusion,                    NULL },
	{ "FillProperties",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FillProperties,               NULL },
	{ "FloatVertexAttribute",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FloatVertexAttribute,         NULL },
	{ "Fog",                          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Fog,                          NULL },
	{ "FogCoordinate",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FogCoordinate,                NULL },
	{ "FontStyle",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FontStyle,                    NULL },
	{ "ForcePhysicsModel",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ForcePhysicsModel,            NULL },
	{ "GeneratedCubeMapTexture",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeneratedCubeMapTexture,      NULL },
	{ "GeoCoordinate",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoCoordinate,                NULL },
	{ "GeoElevationGrid",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoElevationGrid,             NULL },
	{ "GeoLOD",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoLOD,                       NULL },
	{ "GeoLocation",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoLocation,                  NULL },
	{ "GeoMetadata",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoMetadata,                  NULL },
	{ "GeoOrigin",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoOrigin,                    NULL },
	{ "GeoPositionInterpolator",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoPositionInterpolator,      NULL },
	{ "GeoProximitySensor",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoProximitySensor,           NULL },
	{ "GeoTouchSensor",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoTouchSensor,               NULL },
	{ "GeoTransform",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoTransform,                 NULL },
	{ "GeoViewpoint",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoViewpoint,                 NULL },
	{ "Group",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Group,                        NULL },
	{ "HAnimDisplacer",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimDisplacer,               NULL },
	{ "HAnimHumanoid",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimHumanoid,                NULL },
	{ "HAnimJoint",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimJoint,                   NULL },
	{ "HAnimSegment",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimSegment,                 NULL },
	{ "HAnimSite",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimSite,                    NULL },
	{ "ImageCubeMapTexture",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ImageCubeMapTexture,          NULL },
	{ "ImageTexture",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ImageTexture,                 NULL },
	{ "ImageTexture3D",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ImageTexture3D,               NULL },
	{ "IndexedFaceSet",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedFaceSet,               NULL },
	{ "IndexedLineSet",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedLineSet,               NULL },
	{ "IndexedQuadSet",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedQuadSet,               NULL },
	{ "IndexedTriangleFanSet",        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedTriangleFanSet,        NULL },
	{ "IndexedTriangleSet",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedTriangleSet,           NULL },
	{ "IndexedTriangleStripSet",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedTriangleStripSet,      NULL },
	{ "Inline",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Inline,                       NULL },
	{ "IntegerSequencer",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IntegerSequencer,             NULL },
	{ "IntegerTrigger",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IntegerTrigger,               NULL },
	{ "KeySensor",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, KeySensor,                    NULL },
	{ "LOD",                          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LOD,                          NULL },
	{ "Layer",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Layer,                        NULL },
	{ "LayerSet",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LayerSet,                     NULL },
	{ "Layout",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Layout,                       NULL },
	{ "LayoutGroup",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LayoutGroup,                  NULL },
	{ "LayoutLayer",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LayoutLayer,                  NULL },
	{ "LinePickSensor",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LinePickSensor,               NULL },
	{ "LineProperties",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LineProperties,               NULL },
	{ "LineSet",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LineSet,                      NULL },
	{ "LoadSensor",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LoadSensor,                   NULL },
	{ "LocalFog",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LocalFog,                     NULL },
	{ "Material",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Material,                     NULL },
	{ "Matrix3VertexAttribute",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Matrix3VertexAttribute,       NULL },
	{ "Matrix4VertexAttribute",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Matrix4VertexAttribute,       NULL },
	{ "MetadataBoolean",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataBoolean,              NULL },
	{ "MetadataDouble",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataDouble,               NULL },
	{ "MetadataFloat",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataFloat,                NULL },
	{ "MetadataInteger",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataInteger,              NULL },
	{ "MetadataSet",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataSet,                  NULL },
	{ "MetadataString",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataString,               NULL },
	{ "MotorJoint",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MotorJoint,                   NULL },
	{ "MovieTexture",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MovieTexture,                 NULL },
	{ "MultiTexture",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MultiTexture,                 NULL },
	{ "MultiTextureCoordinate",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MultiTextureCoordinate,       NULL },
	{ "MultiTextureTransform",        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MultiTextureTransform,        NULL },
	{ "NavigationInfo",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NavigationInfo,               NULL },
	{ "Normal",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Normal,                       NULL },
	{ "NormalInterpolator",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NormalInterpolator,           NULL },
	{ "NurbsCurve",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsCurve,                   NULL },
	{ "NurbsCurve2D",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsCurve2D,                 NULL },
	{ "NurbsOrientationInterpolator", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsOrientationInterpolator, NULL },
	{ "NurbsPatchSurface",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsPatchSurface,            NULL },
	{ "NurbsPositionInterpolator",    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsPositionInterpolator,    NULL },
	{ "NurbsSet",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsSet,                     NULL },
	{ "NurbsSurfaceInterpolator",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsSurfaceInterpolator,     NULL },
	{ "NurbsSweptSurface",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsSweptSurface,            NULL },
	{ "NurbsSwungSurface",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsSwungSurface,            NULL },
	{ "NurbsTextureCoordinate",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsTextureCoordinate,       NULL },
	{ "NurbsTrimmedSurface",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsTrimmedSurface,          NULL },
	{ "OrientationChaser",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, OrientationChaser,            NULL },
	{ "OrientationDamper",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, OrientationDamper,            NULL },
	{ "OrientationInterpolator",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, OrientationInterpolator,      NULL },
	{ "OrthoViewpoint",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, OrthoViewpoint,               NULL },
	{ "PackagedShader",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PackagedShader,               NULL },
	{ "ParticleSystem",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ParticleSystem,               NULL },
	{ "PickableGroup",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PickableGroup,                NULL },
	{ "PixelTexture",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PixelTexture,                 NULL },
	{ "PixelTexture3D",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PixelTexture3D,               NULL },
	{ "PlaneSensor",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PlaneSensor,                  NULL },
	{ "PointEmitter",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PointEmitter,                 NULL },
	{ "PointLight",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PointLight,                   NULL },
	{ "PointPickSensor",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PointPickSensor,              NULL },
	{ "PointSet",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PointSet,                     NULL },
	{ "Polyline2D",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Polyline2D,                   NULL },
	{ "PolylineEmitter",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PolylineEmitter,              NULL },
	{ "Polypoint2D",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Polypoint2D,                  NULL },
	{ "PositionChaser",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionChaser,               NULL },
	{ "PositionChaser2D",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionChaser2D,             NULL },
	{ "PositionDamper",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionDamper,               NULL },
	{ "PositionDamper2D",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionDamper2D,             NULL },
	{ "PositionInterpolator",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionInterpolator,         NULL },
	{ "PositionInterpolator2D",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionInterpolator2D,       NULL },
	{ "PrimitivePickSensor",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PrimitivePickSensor,          NULL },
	{ "ProgramShader",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ProgramShader,                NULL },
	{ "ProximitySensor",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ProximitySensor,              NULL },
	{ "QuadSet",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, QuadSet,                      NULL },
	{ "ReceiverPdu",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ReceiverPdu,                  NULL },
	{ "Rectangle2D",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Rectangle2D,                  NULL },
	{ "RigidBody",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, RigidBody,                    NULL },
	{ "RigidBodyCollection",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, RigidBodyCollection,          NULL },
	{ "ScalarChaser",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScalarChaser,                 NULL },
	{ "ScalarDamper",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScalarDamper,                 NULL },
	{ "ScalarInterpolator",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScalarInterpolator,           NULL },
	{ "ScreenFontStyle",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScreenFontStyle,              NULL },
	{ "ScreenGroup",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScreenGroup,                  NULL },
	{ "Script",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Script,                       NULL },
	{ "ShaderPart",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ShaderPart,                   NULL },
	{ "ShaderProgram",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ShaderProgram,                NULL },
	{ "Shape",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Shape,                        NULL },
	{ "SignalPdu",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SignalPdu,                    NULL },
	{ "SingleAxisHingeJoint",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SingleAxisHingeJoint,         NULL },
	{ "SliderJoint",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SliderJoint,                  NULL },
	{ "Sound",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Sound,                        NULL },
	{ "Sphere",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Sphere,                       NULL },
	{ "SphereSensor",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SphereSensor,                 NULL },
	{ "SplinePositionInterpolator",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SplinePositionInterpolator,   NULL },
	{ "SplinePositionInterpolator2D", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SplinePositionInterpolator2D, NULL },
	{ "SplineScalarInterpolator",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SplineScalarInterpolator,     NULL },
	{ "SpotLight",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SpotLight,                    NULL },
	{ "SquadOrientationInterpolator", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SquadOrientationInterpolator, NULL },
	{ "StaticGroup",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, StaticGroup,                  NULL },
	{ "StringSensor",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, StringSensor,                 NULL },
	{ "SurfaceEmitter",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SurfaceEmitter,               NULL },
	{ "Switch",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Switch,                       NULL },
	{ "TexCoordChaser2D",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TexCoordChaser2D,             NULL },
	{ "TexCoordDamper2D",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TexCoordDamper2D,             NULL },
	{ "Text",                         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Text,                         NULL },
	{ "TextureBackground",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureBackground,            NULL },
	{ "TextureCoordinate",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureCoordinate,            NULL },
	{ "TextureCoordinate3D",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureCoordinate3D,          NULL },
	{ "TextureCoordinate4D",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureCoordinate4D,          NULL },
	{ "TextureCoordinateGenerator",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureCoordinateGenerator,   NULL },
	{ "TextureProperties",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureProperties,            NULL },
	{ "TextureTransform",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureTransform,             NULL },
	{ "TextureTransform3D",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureTransform3D,           NULL },
	{ "TextureTransformMatrix3D",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureTransformMatrix3D,     NULL },
	{ "TimeSensor",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TimeSensor,                   NULL },
	{ "TimeTrigger",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TimeTrigger,                  NULL },
	{ "TouchSensor",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TouchSensor,                  NULL },
	{ "Transform",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Transform,                    NULL },
	{ "TransformSensor",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TransformSensor,              NULL },
	{ "TransmitterPdu",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TransmitterPdu,               NULL },
	{ "TriangleFanSet",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TriangleFanSet,               NULL },
	{ "TriangleSet",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TriangleSet,                  NULL },
	{ "TriangleSet2D",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TriangleSet2D,                NULL },
	{ "TriangleStripSet",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TriangleStripSet,             NULL },
	{ "TwoSidedMaterial",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TwoSidedMaterial,             NULL },
	{ "UniversalJoint",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, UniversalJoint,               NULL },
	{ "Viewpoint",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Viewpoint,                    NULL },
	{ "ViewpointGroup",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ViewpointGroup,               NULL },
	{ "Viewport",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Viewport,                     NULL },
	{ "VisibilitySensor",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, VisibilitySensor,             NULL },
	{ "VolumeEmitter",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, VolumeEmitter,                NULL },
	{ "VolumePickSensor",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, VolumePickSensor,             NULL },
	{ "WindPhysicsModel",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, WindPhysicsModel,             NULL },
	{ "WorldInfo",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, WorldInfo,                    NULL },

	// Abstract node types

	{ "X3DAppearanceChildNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DAppearanceChildNode,       NULL },
	{ "X3DAppearanceNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DAppearanceNode,            NULL },
	{ "X3DBackgroundNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBackgroundNode,            NULL },
	{ "X3DBindableNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBindableNode,              NULL },
	{ "X3DBoundedObject",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBoundedObject,             NULL },
	{ "X3DChaserNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DChaserNode,                NULL },
	{ "X3DChildNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DChildNode,                 NULL },
	{ "X3DColorNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DColorNode,                 NULL },
	{ "X3DComposedGeometryNode",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DComposedGeometryNode,      NULL },
	{ "X3DCoordinateNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DCoordinateNode,            NULL },
	{ "X3DDamperNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DDamperNode,                NULL },
	{ "X3DDragSensorNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DDragSensorNode,            NULL },
	{ "X3DEnvironmentTextureNode",    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DEnvironmentTextureNode,    NULL },
	{ "X3DEnvironmentalSensorNode",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DEnvironmentalSensorNode,   NULL },
	{ "X3DFogObject",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFogObject,                 NULL },
	{ "X3DFollowerNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFollowerNode,              NULL },
	{ "X3DFontStyleNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFontStyleNode,             NULL },
	{ "X3DGeometricPropertyNode",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGeometricPropertyNode,     NULL },
	{ "X3DGeometryNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGeometryNode,              NULL },
	{ "X3DGeospatialObject",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGeospatialObject,          NULL },
	{ "X3DGroupingNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGroupingNode,              NULL },
	{ "X3DInfoNode",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DInfoNode,                  NULL },
	{ "X3DInterpolatorNode",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DInterpolatorNode,          NULL },
	{ "X3DKeyDeviceSensorNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DKeyDeviceSensorNode,       NULL },
	{ "X3DLayerNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLayerNode,                 NULL },
	{ "X3DLayoutNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLayoutNode,                NULL },
	{ "X3DLightNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLightNode,                 NULL },
	{ "X3DMaterialNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DMaterialNode,              NULL },
	{ "X3DMetadataObject",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DMetadataObject,            NULL },
	{ "X3DNBodyCollidableNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNBodyCollidableNode,       NULL },
	{ "X3DNBodyCollisionSpaceNode",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNBodyCollisionSpaceNode,   NULL },
	{ "X3DNetworkSensorNode",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNetworkSensorNode,         NULL },
	{ "X3DNode",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNode,                      NULL },
	{ "X3DNormalNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNormalNode,                NULL },
	{ "X3DNurbsControlCurveNode",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNurbsControlCurveNode,     NULL },
	{ "X3DNurbsSurfaceGeometryNode",  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNurbsSurfaceGeometryNode,  NULL },
	{ "X3DParametricGeometryNode",    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParametricGeometryNode,    NULL },
	{ "X3DParticleEmitterNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParticleEmitterNode,       NULL },
	{ "X3DParticlePhysicsModelNode",  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParticlePhysicsModelNode,  NULL },
	{ "X3DPickSensorNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPickSensorNode,            NULL },
	{ "X3DPickableObject",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPickableObject,            NULL },
	{ "X3DPointingDeviceSensorNode",  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPointingDeviceSensorNode,  NULL },
	{ "X3DProductStructureChildNode", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DProductStructureChildNode, NULL },
	{ "X3DProgrammableShaderObject",  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DProgrammableShaderObject,  NULL },
	{ "X3DPrototypeInstance",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPrototypeInstance,         NULL },
	{ "X3DRigidJointNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DRigidJointNode,            NULL },
	{ "X3DScriptNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DScriptNode,                NULL },
	{ "X3DSensorNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSensorNode,                NULL },
	{ "X3DSequencerNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSequencerNode,             NULL },
	{ "X3DShaderNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DShaderNode,                NULL },
	{ "X3DShapeNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DShapeNode,                 NULL },
	{ "X3DSoundNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSoundNode,                 NULL },
	{ "X3DSoundSourceNode",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSoundSourceNode,           NULL },
	{ "X3DTexture2DNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTexture2DNode,             NULL },
	{ "X3DTexture3DNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTexture3DNode,             NULL },
	{ "X3DTextureCoordinateNode",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureCoordinateNode,     NULL },
	{ "X3DTextureNode",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureNode,               NULL },
	{ "X3DTextureTransformNode",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureTransformNode,      NULL },
	{ "X3DTimeDependentNode",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTimeDependentNode,         NULL },
	{ "X3DTouchSensorNode",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTouchSensorNode,           NULL },
	{ "X3DTransformMatrix4DNode",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTransformMatrix4DNode,     NULL },
	{ "X3DTransformNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTransformNode,             NULL },
	{ "X3DTriggerNode",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTriggerNode,               NULL },
	{ "X3DUrlObject",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DUrlObject,                 NULL },
	{ "X3DVertexAttributeNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DVertexAttributeNode,       NULL },
	{ "X3DViewpointNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewpointNode,             NULL },
	{ "X3DViewpointObject",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewpointObject,           NULL },
	{ "X3DViewportNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewportNode,              NULL },

	// Tools

	{ "AngleTool",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, AngleTool,   NULL },
	{ "GridTool",    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GridTool,    NULL },
	{ "X3DGridTool", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGridTool, NULL },

	{ 0 }

};

JSFunctionSpec jsX3DConstants::functions [ ] = {
	{ 0, 0, 0, 0 }

};

void
jsX3DConstants::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, NULL,
	              0, properties, functions, NULL, NULL);

	JS_DefineObject (context, global, "X3DConstants", &static_class, NULL,
	                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT);
}

// Event types

JSBool
jsX3DConstants::INITIALIZED_EVENT (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::INITIALIZED_EVENT, vp);
}

JSBool
jsX3DConstants::SHUTDOWN_EVENT (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SHUTDOWN_EVENT, vp);
}

// Error types

JSBool
jsX3DConstants::CONNECTION_ERROR (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CONNECTION_ERROR, vp);
}

JSBool
jsX3DConstants::INITIALIZED_ERROR (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::INITIALIZED_ERROR, vp);
}

// Load state

JSBool
jsX3DConstants::NOT_STARTED_STATE (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NOT_STARTED_STATE, vp);
}

JSBool
jsX3DConstants::IN_PROGRESS_STATE (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IN_PROGRESS_STATE, vp);
}

JSBool
jsX3DConstants::COMPLETE_STATE (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::COMPLETE_STATE, vp);
}

JSBool
jsX3DConstants::FAILED_STATE (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::FAILED_STATE, vp);
}

// Access types

JSBool
jsX3DConstants::initializeOnly (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::initializeOnly, vp);
}

JSBool
jsX3DConstants::inputOnly (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::inputOnly, vp);
}

JSBool
jsX3DConstants::outputOnly (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::outputOnly, vp);
}

JSBool
jsX3DConstants::inputOutput (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::inputOutput, vp);
}

// X3D properties

JSBool
jsX3DConstants::SFBool (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFBool, vp);
}

JSBool
jsX3DConstants::SFColor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFColor, vp);
}

JSBool
jsX3DConstants::SFColorRGBA (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFColorRGBA, vp);
}

JSBool
jsX3DConstants::SFDouble (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFDouble, vp);
}

JSBool
jsX3DConstants::SFFloat (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFFloat, vp);
}

JSBool
jsX3DConstants::SFInt32 (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFInt32, vp);
}

JSBool
jsX3DConstants::SFImage (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFImage, vp);
}

JSBool
jsX3DConstants::SFMatrix3d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFMatrix3d, vp);
}

JSBool
jsX3DConstants::SFMatrix3f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFMatrix3f, vp);
}

JSBool
jsX3DConstants::SFMatrix4d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFMatrix4d, vp);
}

JSBool
jsX3DConstants::SFMatrix4f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFMatrix4f, vp);
}

JSBool
jsX3DConstants::SFNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFNode, vp);
}

JSBool
jsX3DConstants::SFRotation (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFRotation, vp);
}

JSBool
jsX3DConstants::SFString (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFString, vp);
}

JSBool
jsX3DConstants::SFTime (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFTime, vp);
}

JSBool
jsX3DConstants::SFVec2d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec2d, vp);
}

JSBool
jsX3DConstants::SFVec2f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec2f, vp);
}

JSBool
jsX3DConstants::SFVec3d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec3d, vp);
}

JSBool
jsX3DConstants::SFVec3f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec3f, vp);
}

JSBool
jsX3DConstants::SFVec4d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec4d, vp);
}

JSBool
jsX3DConstants::SFVec4f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SFVec4f, vp);
}

JSBool
jsX3DConstants::MFBool (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFBool, vp);
}

JSBool
jsX3DConstants::MFColor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFColor, vp);
}

JSBool
jsX3DConstants::MFColorRGBA (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFColorRGBA, vp);
}

JSBool
jsX3DConstants::MFDouble (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFDouble, vp);
}

JSBool
jsX3DConstants::MFFloat (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFFloat, vp);
}

JSBool
jsX3DConstants::MFImage (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFImage, vp);
}

JSBool
jsX3DConstants::MFInt32 (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFInt32, vp);
}

JSBool
jsX3DConstants::MFMatrix3d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFMatrix3d, vp);
}

JSBool
jsX3DConstants::MFMatrix3f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFMatrix3f, vp);
}

JSBool
jsX3DConstants::MFMatrix4d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFMatrix4d, vp);
}

JSBool
jsX3DConstants::MFMatrix4f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFMatrix4f, vp);
}

JSBool
jsX3DConstants::MFNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFNode, vp);
}

JSBool
jsX3DConstants::MFRotation (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFRotation, vp);
}

JSBool
jsX3DConstants::MFString (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFString, vp);
}

JSBool
jsX3DConstants::MFTime (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFTime, vp);
}

JSBool
jsX3DConstants::MFVec2d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec2d, vp);
}

JSBool
jsX3DConstants::MFVec2f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec2f, vp);
}

JSBool
jsX3DConstants::MFVec3d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec3d, vp);
}

JSBool
jsX3DConstants::MFVec3f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec3f, vp);
}

JSBool
jsX3DConstants::MFVec4d (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec4d, vp);
}

JSBool
jsX3DConstants::MFVec4f (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MFVec4f, vp);
}

// Base node type

JSBool
jsX3DConstants::X3DBaseNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DBaseNode, vp);
}

// Basic node types

JSBool
jsX3DConstants::Anchor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Anchor, vp);
}

JSBool
jsX3DConstants::Appearance (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Appearance, vp);
}

JSBool
jsX3DConstants::Arc2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Arc2D, vp);
}

JSBool
jsX3DConstants::ArcClose2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ArcClose2D, vp);
}

JSBool
jsX3DConstants::AudioClip (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::AudioClip, vp);
}

JSBool
jsX3DConstants::Background (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Background, vp);
}

JSBool
jsX3DConstants::BallJoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::BallJoint, vp);
}

JSBool
jsX3DConstants::Billboard (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Billboard, vp);
}

JSBool
jsX3DConstants::BooleanFilter (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::BooleanFilter, vp);
}

JSBool
jsX3DConstants::BooleanSequencer (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::BooleanSequencer, vp);
}

JSBool
jsX3DConstants::BooleanToggle (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::BooleanToggle, vp);
}

JSBool
jsX3DConstants::BooleanTrigger (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::BooleanTrigger, vp);
}

JSBool
jsX3DConstants::BoundedPhysicsModel (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::BoundedPhysicsModel, vp);
}

JSBool
jsX3DConstants::Box (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Box, vp);
}

JSBool
jsX3DConstants::CADAssembly (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CADAssembly, vp);
}

JSBool
jsX3DConstants::CADFace (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CADFace, vp);
}

JSBool
jsX3DConstants::CADLayer (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CADLayer, vp);
}

JSBool
jsX3DConstants::CADPart (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CADPart, vp);
}

JSBool
jsX3DConstants::Circle2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Circle2D, vp);
}

JSBool
jsX3DConstants::ClipPlane (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ClipPlane, vp);
}

JSBool
jsX3DConstants::CollidableOffset (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CollidableOffset, vp);
}

JSBool
jsX3DConstants::CollidableShape (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CollidableShape, vp);
}

JSBool
jsX3DConstants::Collision (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Collision, vp);
}

JSBool
jsX3DConstants::CollisionCollection (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CollisionCollection, vp);
}

JSBool
jsX3DConstants::CollisionSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CollisionSensor, vp);
}

JSBool
jsX3DConstants::CollisionSpace (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CollisionSpace, vp);
}

JSBool
jsX3DConstants::Color (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Color, vp);
}

JSBool
jsX3DConstants::ColorChaser (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ColorChaser, vp);
}

JSBool
jsX3DConstants::ColorDamper (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ColorDamper, vp);
}

JSBool
jsX3DConstants::ColorInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ColorInterpolator, vp);
}

JSBool
jsX3DConstants::ColorRGBA (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ColorRGBA, vp);
}

JSBool
jsX3DConstants::ComposedCubeMapTexture (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ComposedCubeMapTexture, vp);
}

JSBool
jsX3DConstants::ComposedShader (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ComposedShader, vp);
}

JSBool
jsX3DConstants::ComposedTexture3D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ComposedTexture3D, vp);
}

JSBool
jsX3DConstants::Cone (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Cone, vp);
}

JSBool
jsX3DConstants::ConeEmitter (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ConeEmitter, vp);
}

JSBool
jsX3DConstants::Contact (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Contact, vp);
}

JSBool
jsX3DConstants::Contour2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Contour2D, vp);
}

JSBool
jsX3DConstants::ContourPolyline2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ContourPolyline2D, vp);
}

JSBool
jsX3DConstants::Coordinate (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Coordinate, vp);
}

JSBool
jsX3DConstants::CoordinateChaser (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CoordinateChaser, vp);
}

JSBool
jsX3DConstants::CoordinateDamper (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CoordinateDamper, vp);
}

JSBool
jsX3DConstants::CoordinateDouble (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CoordinateDouble, vp);
}

JSBool
jsX3DConstants::CoordinateInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CoordinateInterpolator, vp);
}

JSBool
jsX3DConstants::CoordinateInterpolator2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CoordinateInterpolator2D, vp);
}

JSBool
jsX3DConstants::Cylinder (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Cylinder, vp);
}

JSBool
jsX3DConstants::CylinderSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::CylinderSensor, vp);
}

JSBool
jsX3DConstants::DISEntityManager (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::DISEntityManager, vp);
}

JSBool
jsX3DConstants::DISEntityTypeMapping (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::DISEntityTypeMapping, vp);
}

JSBool
jsX3DConstants::DirectionalLight (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::DirectionalLight, vp);
}

JSBool
jsX3DConstants::Disk2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Disk2D, vp);
}

JSBool
jsX3DConstants::DoubleAxisHingeJoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::DoubleAxisHingeJoint, vp);
}

JSBool
jsX3DConstants::EaseInEaseOut (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::EaseInEaseOut, vp);
}

JSBool
jsX3DConstants::ElevationGrid (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ElevationGrid, vp);
}

JSBool
jsX3DConstants::EspduTransform (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::EspduTransform, vp);
}

JSBool
jsX3DConstants::ExplosionEmitter (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ExplosionEmitter, vp);
}

JSBool
jsX3DConstants::Extrusion (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Extrusion, vp);
}

JSBool
jsX3DConstants::FillProperties (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::FillProperties, vp);
}

JSBool
jsX3DConstants::FloatVertexAttribute (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::FloatVertexAttribute, vp);
}

JSBool
jsX3DConstants::Fog (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Fog, vp);
}

JSBool
jsX3DConstants::FogCoordinate (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::FogCoordinate, vp);
}

JSBool
jsX3DConstants::FontStyle (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::FontStyle, vp);
}

JSBool
jsX3DConstants::ForcePhysicsModel (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ForcePhysicsModel, vp);
}

JSBool
jsX3DConstants::GeneratedCubeMapTexture (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeneratedCubeMapTexture, vp);
}

JSBool
jsX3DConstants::GeoCoordinate (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoCoordinate, vp);
}

JSBool
jsX3DConstants::GeoElevationGrid (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoElevationGrid, vp);
}

JSBool
jsX3DConstants::GeoLOD (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoLOD, vp);
}

JSBool
jsX3DConstants::GeoLocation (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoLocation, vp);
}

JSBool
jsX3DConstants::GeoMetadata (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoMetadata, vp);
}

JSBool
jsX3DConstants::GeoOrigin (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoOrigin, vp);
}

JSBool
jsX3DConstants::GeoPositionInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoPositionInterpolator, vp);
}

JSBool
jsX3DConstants::GeoProximitySensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoProximitySensor, vp);
}

JSBool
jsX3DConstants::GeoTouchSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoTouchSensor, vp);
}

JSBool
jsX3DConstants::GeoTransform (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoTransform, vp);
}

JSBool
jsX3DConstants::GeoViewpoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GeoViewpoint, vp);
}

JSBool
jsX3DConstants::Group (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Group, vp);
}

JSBool
jsX3DConstants::HAnimDisplacer (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::HAnimDisplacer, vp);
}

JSBool
jsX3DConstants::HAnimHumanoid (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::HAnimHumanoid, vp);
}

JSBool
jsX3DConstants::HAnimJoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::HAnimJoint, vp);
}

JSBool
jsX3DConstants::HAnimSegment (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::HAnimSegment, vp);
}

JSBool
jsX3DConstants::HAnimSite (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::HAnimSite, vp);
}

JSBool
jsX3DConstants::ImageCubeMapTexture (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ImageCubeMapTexture, vp);
}

JSBool
jsX3DConstants::ImageTexture (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ImageTexture, vp);
}

JSBool
jsX3DConstants::ImageTexture3D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ImageTexture3D, vp);
}

JSBool
jsX3DConstants::IndexedFaceSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IndexedFaceSet, vp);
}

JSBool
jsX3DConstants::IndexedLineSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IndexedLineSet, vp);
}

JSBool
jsX3DConstants::IndexedQuadSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IndexedQuadSet, vp);
}

JSBool
jsX3DConstants::IndexedTriangleFanSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IndexedTriangleFanSet, vp);
}

JSBool
jsX3DConstants::IndexedTriangleSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IndexedTriangleSet, vp);
}

JSBool
jsX3DConstants::IndexedTriangleStripSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IndexedTriangleStripSet, vp);
}

JSBool
jsX3DConstants::Inline (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Inline, vp);
}

JSBool
jsX3DConstants::IntegerSequencer (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IntegerSequencer, vp);
}

JSBool
jsX3DConstants::IntegerTrigger (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::IntegerTrigger, vp);
}

JSBool
jsX3DConstants::KeySensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::KeySensor, vp);
}

JSBool
jsX3DConstants::LOD (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LOD, vp);
}

JSBool
jsX3DConstants::Layer (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Layer, vp);
}

JSBool
jsX3DConstants::LayerSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LayerSet, vp);
}

JSBool
jsX3DConstants::Layout (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Layout, vp);
}

JSBool
jsX3DConstants::LayoutGroup (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LayoutGroup, vp);
}

JSBool
jsX3DConstants::LayoutLayer (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LayoutLayer, vp);
}

JSBool
jsX3DConstants::LinePickSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LinePickSensor, vp);
}

JSBool
jsX3DConstants::LineProperties (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LineProperties, vp);
}

JSBool
jsX3DConstants::LineSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LineSet, vp);
}

JSBool
jsX3DConstants::LoadSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LoadSensor, vp);
}

JSBool
jsX3DConstants::LocalFog (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::LocalFog, vp);
}

JSBool
jsX3DConstants::Material (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Material, vp);
}

JSBool
jsX3DConstants::Matrix3VertexAttribute (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Matrix3VertexAttribute, vp);
}

JSBool
jsX3DConstants::Matrix4VertexAttribute (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Matrix4VertexAttribute, vp);
}

JSBool
jsX3DConstants::MetadataBoolean (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MetadataBoolean, vp);
}

JSBool
jsX3DConstants::MetadataDouble (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MetadataDouble, vp);
}

JSBool
jsX3DConstants::MetadataFloat (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MetadataFloat, vp);
}

JSBool
jsX3DConstants::MetadataInteger (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MetadataInteger, vp);
}

JSBool
jsX3DConstants::MetadataSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MetadataSet, vp);
}

JSBool
jsX3DConstants::MetadataString (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MetadataString, vp);
}

JSBool
jsX3DConstants::MotorJoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MotorJoint, vp);
}

JSBool
jsX3DConstants::MovieTexture (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MovieTexture, vp);
}

JSBool
jsX3DConstants::MultiTexture (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MultiTexture, vp);
}

JSBool
jsX3DConstants::MultiTextureCoordinate (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MultiTextureCoordinate, vp);
}

JSBool
jsX3DConstants::MultiTextureTransform (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::MultiTextureTransform, vp);
}

JSBool
jsX3DConstants::NavigationInfo (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NavigationInfo, vp);
}

JSBool
jsX3DConstants::Normal (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Normal, vp);
}

JSBool
jsX3DConstants::NormalInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NormalInterpolator, vp);
}

JSBool
jsX3DConstants::NurbsCurve (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsCurve, vp);
}

JSBool
jsX3DConstants::NurbsCurve2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsCurve2D, vp);
}

JSBool
jsX3DConstants::NurbsOrientationInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsOrientationInterpolator, vp);
}

JSBool
jsX3DConstants::NurbsPatchSurface (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsPatchSurface, vp);
}

JSBool
jsX3DConstants::NurbsPositionInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsPositionInterpolator, vp);
}

JSBool
jsX3DConstants::NurbsSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsSet, vp);
}

JSBool
jsX3DConstants::NurbsSurfaceInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsSurfaceInterpolator, vp);
}

JSBool
jsX3DConstants::NurbsSweptSurface (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsSweptSurface, vp);
}

JSBool
jsX3DConstants::NurbsSwungSurface (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsSwungSurface, vp);
}

JSBool
jsX3DConstants::NurbsTextureCoordinate (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsTextureCoordinate, vp);
}

JSBool
jsX3DConstants::NurbsTrimmedSurface (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::NurbsTrimmedSurface, vp);
}

JSBool
jsX3DConstants::OrientationChaser (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::OrientationChaser, vp);
}

JSBool
jsX3DConstants::OrientationDamper (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::OrientationDamper, vp);
}

JSBool
jsX3DConstants::OrientationInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::OrientationInterpolator, vp);
}

JSBool
jsX3DConstants::OrthoViewpoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::OrthoViewpoint, vp);
}

JSBool
jsX3DConstants::PackagedShader (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PackagedShader, vp);
}

JSBool
jsX3DConstants::ParticleSystem (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ParticleSystem, vp);
}

JSBool
jsX3DConstants::PickableGroup (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PickableGroup, vp);
}

JSBool
jsX3DConstants::PixelTexture (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PixelTexture, vp);
}

JSBool
jsX3DConstants::PixelTexture3D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PixelTexture3D, vp);
}

JSBool
jsX3DConstants::PlaneSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PlaneSensor, vp);
}

JSBool
jsX3DConstants::PointEmitter (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PointEmitter, vp);
}

JSBool
jsX3DConstants::PointLight (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PointLight, vp);
}

JSBool
jsX3DConstants::PointPickSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PointPickSensor, vp);
}

JSBool
jsX3DConstants::PointSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PointSet, vp);
}

JSBool
jsX3DConstants::Polyline2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Polyline2D, vp);
}

JSBool
jsX3DConstants::PolylineEmitter (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PolylineEmitter, vp);
}

JSBool
jsX3DConstants::Polypoint2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Polypoint2D, vp);
}

JSBool
jsX3DConstants::PositionChaser (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PositionChaser, vp);
}

JSBool
jsX3DConstants::PositionChaser2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PositionChaser2D, vp);
}

JSBool
jsX3DConstants::PositionDamper (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PositionDamper, vp);
}

JSBool
jsX3DConstants::PositionDamper2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PositionDamper2D, vp);
}

JSBool
jsX3DConstants::PositionInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PositionInterpolator, vp);
}

JSBool
jsX3DConstants::PositionInterpolator2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PositionInterpolator2D, vp);
}

JSBool
jsX3DConstants::PrimitivePickSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::PrimitivePickSensor, vp);
}

JSBool
jsX3DConstants::ProgramShader (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ProgramShader, vp);
}

JSBool
jsX3DConstants::ProximitySensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ProximitySensor, vp);
}

JSBool
jsX3DConstants::QuadSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::QuadSet, vp);
}

JSBool
jsX3DConstants::ReceiverPdu (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ReceiverPdu, vp);
}

JSBool
jsX3DConstants::Rectangle2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Rectangle2D, vp);
}

JSBool
jsX3DConstants::RigidBody (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::RigidBody, vp);
}

JSBool
jsX3DConstants::RigidBodyCollection (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::RigidBodyCollection, vp);
}

JSBool
jsX3DConstants::ScalarChaser (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ScalarChaser, vp);
}

JSBool
jsX3DConstants::ScalarDamper (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ScalarDamper, vp);
}

JSBool
jsX3DConstants::ScalarInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ScalarInterpolator, vp);
}

JSBool
jsX3DConstants::ScreenFontStyle (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ScreenFontStyle, vp);
}

JSBool
jsX3DConstants::ScreenGroup (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ScreenGroup, vp);
}

JSBool
jsX3DConstants::Script (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Script, vp);
}

JSBool
jsX3DConstants::ShaderPart (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ShaderPart, vp);
}

JSBool
jsX3DConstants::ShaderProgram (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ShaderProgram, vp);
}

JSBool
jsX3DConstants::Shape (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Shape, vp);
}

JSBool
jsX3DConstants::SignalPdu (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SignalPdu, vp);
}

JSBool
jsX3DConstants::SingleAxisHingeJoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SingleAxisHingeJoint, vp);
}

JSBool
jsX3DConstants::SliderJoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SliderJoint, vp);
}

JSBool
jsX3DConstants::Sound (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Sound, vp);
}

JSBool
jsX3DConstants::Sphere (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Sphere, vp);
}

JSBool
jsX3DConstants::SphereSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SphereSensor, vp);
}

JSBool
jsX3DConstants::SplinePositionInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SplinePositionInterpolator, vp);
}

JSBool
jsX3DConstants::SplinePositionInterpolator2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SplinePositionInterpolator2D, vp);
}

JSBool
jsX3DConstants::SplineScalarInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SplineScalarInterpolator, vp);
}

JSBool
jsX3DConstants::SpotLight (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SpotLight, vp);
}

JSBool
jsX3DConstants::SquadOrientationInterpolator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SquadOrientationInterpolator, vp);
}

JSBool
jsX3DConstants::StaticGroup (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::StaticGroup, vp);
}

JSBool
jsX3DConstants::StringSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::StringSensor, vp);
}

JSBool
jsX3DConstants::SurfaceEmitter (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::SurfaceEmitter, vp);
}

JSBool
jsX3DConstants::Switch (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Switch, vp);
}

JSBool
jsX3DConstants::TexCoordChaser2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TexCoordChaser2D, vp);
}

JSBool
jsX3DConstants::TexCoordDamper2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TexCoordDamper2D, vp);
}

JSBool
jsX3DConstants::Text (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Text, vp);
}

JSBool
jsX3DConstants::TextureBackground (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureBackground, vp);
}

JSBool
jsX3DConstants::TextureCoordinate (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureCoordinate, vp);
}

JSBool
jsX3DConstants::TextureCoordinate3D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureCoordinate3D, vp);
}

JSBool
jsX3DConstants::TextureCoordinate4D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureCoordinate4D, vp);
}

JSBool
jsX3DConstants::TextureCoordinateGenerator (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureCoordinateGenerator, vp);
}

JSBool
jsX3DConstants::TextureProperties (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureProperties, vp);
}

JSBool
jsX3DConstants::TextureTransform (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureTransform, vp);
}

JSBool
jsX3DConstants::TextureTransform3D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureTransform3D, vp);
}

JSBool
jsX3DConstants::TextureTransformMatrix3D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TextureTransformMatrix3D, vp);
}

JSBool
jsX3DConstants::TimeSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TimeSensor, vp);
}

JSBool
jsX3DConstants::TimeTrigger (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TimeTrigger, vp);
}

JSBool
jsX3DConstants::TouchSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TouchSensor, vp);
}

JSBool
jsX3DConstants::Transform (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Transform, vp);
}

JSBool
jsX3DConstants::TransformSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TransformSensor, vp);
}

JSBool
jsX3DConstants::TransmitterPdu (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TransmitterPdu, vp);
}

JSBool
jsX3DConstants::TriangleFanSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TriangleFanSet, vp);
}

JSBool
jsX3DConstants::TriangleSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TriangleSet, vp);
}

JSBool
jsX3DConstants::TriangleSet2D (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TriangleSet2D, vp);
}

JSBool
jsX3DConstants::TriangleStripSet (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TriangleStripSet, vp);
}

JSBool
jsX3DConstants::TwoSidedMaterial (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::TwoSidedMaterial, vp);
}

JSBool
jsX3DConstants::UniversalJoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::UniversalJoint, vp);
}

JSBool
jsX3DConstants::Viewpoint (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Viewpoint, vp);
}

JSBool
jsX3DConstants::ViewpointGroup (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::ViewpointGroup, vp);
}

JSBool
jsX3DConstants::Viewport (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::Viewport, vp);
}

JSBool
jsX3DConstants::VisibilitySensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::VisibilitySensor, vp);
}

JSBool
jsX3DConstants::VolumeEmitter (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::VolumeEmitter, vp);
}

JSBool
jsX3DConstants::VolumePickSensor (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::VolumePickSensor, vp);
}

JSBool
jsX3DConstants::WindPhysicsModel (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::WindPhysicsModel, vp);
}

JSBool
jsX3DConstants::WorldInfo (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::WorldInfo, vp);
}

// Abstract node types

JSBool
jsX3DConstants::X3DAppearanceChildNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DAppearanceChildNode, vp);
}

JSBool
jsX3DConstants::X3DAppearanceNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DAppearanceNode, vp);
}

JSBool
jsX3DConstants::X3DBackgroundNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DBackgroundNode, vp);
}

JSBool
jsX3DConstants::X3DBindableNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DBindableNode, vp);
}

JSBool
jsX3DConstants::X3DBoundedObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DBoundedObject, vp);
}

JSBool
jsX3DConstants::X3DChaserNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DChaserNode, vp);
}

JSBool
jsX3DConstants::X3DChildNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DChildNode, vp);
}

JSBool
jsX3DConstants::X3DColorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DColorNode, vp);
}

JSBool
jsX3DConstants::X3DComposedGeometryNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DComposedGeometryNode, vp);
}

JSBool
jsX3DConstants::X3DCoordinateNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DCoordinateNode, vp);
}

JSBool
jsX3DConstants::X3DDamperNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DDamperNode, vp);
}

JSBool
jsX3DConstants::X3DDragSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DDragSensorNode, vp);
}

JSBool
jsX3DConstants::X3DEnvironmentTextureNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DEnvironmentTextureNode, vp);
}

JSBool
jsX3DConstants::X3DEnvironmentalSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DEnvironmentalSensorNode, vp);
}

JSBool
jsX3DConstants::X3DFogObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DFogObject, vp);
}

JSBool
jsX3DConstants::X3DFollowerNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DFollowerNode, vp);
}

JSBool
jsX3DConstants::X3DFontStyleNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DFontStyleNode, vp);
}

JSBool
jsX3DConstants::X3DGeometricPropertyNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DGeometricPropertyNode, vp);
}

JSBool
jsX3DConstants::X3DGeometryNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DGeometryNode, vp);
}

JSBool
jsX3DConstants::X3DGeospatialObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DGeospatialObject, vp);
}

JSBool
jsX3DConstants::X3DGroupingNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DGroupingNode, vp);
}

JSBool
jsX3DConstants::X3DInfoNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DInfoNode, vp);
}

JSBool
jsX3DConstants::X3DInterpolatorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DInterpolatorNode, vp);
}

JSBool
jsX3DConstants::X3DKeyDeviceSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DKeyDeviceSensorNode, vp);
}

JSBool
jsX3DConstants::X3DLayerNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DLayerNode, vp);
}

JSBool
jsX3DConstants::X3DLayoutNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DLayoutNode, vp);
}

JSBool
jsX3DConstants::X3DLightNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DLightNode, vp);
}

JSBool
jsX3DConstants::X3DMaterialNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DMaterialNode, vp);
}

JSBool
jsX3DConstants::X3DMetadataObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DMetadataObject, vp);
}

JSBool
jsX3DConstants::X3DNBodyCollidableNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNBodyCollidableNode, vp);
}

JSBool
jsX3DConstants::X3DNBodyCollisionSpaceNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNBodyCollisionSpaceNode, vp);
}

JSBool
jsX3DConstants::X3DNetworkSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNetworkSensorNode, vp);
}

JSBool
jsX3DConstants::X3DNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNode, vp);
}

JSBool
jsX3DConstants::X3DNormalNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNormalNode, vp);
}

JSBool
jsX3DConstants::X3DNurbsControlCurveNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNurbsControlCurveNode, vp);
}

JSBool
jsX3DConstants::X3DNurbsSurfaceGeometryNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DNurbsSurfaceGeometryNode, vp);
}

JSBool
jsX3DConstants::X3DParametricGeometryNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DParametricGeometryNode, vp);
}

JSBool
jsX3DConstants::X3DParticleEmitterNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DParticleEmitterNode, vp);
}

JSBool
jsX3DConstants::X3DParticlePhysicsModelNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DParticlePhysicsModelNode, vp);
}

JSBool
jsX3DConstants::X3DPickSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPickSensorNode, vp);
}

JSBool
jsX3DConstants::X3DPickableObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPickableObject, vp);
}

JSBool
jsX3DConstants::X3DPointingDeviceSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPointingDeviceSensorNode, vp);
}

JSBool
jsX3DConstants::X3DProductStructureChildNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DProductStructureChildNode, vp);
}

JSBool
jsX3DConstants::X3DProgrammableShaderObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DProgrammableShaderObject, vp);
}

JSBool
jsX3DConstants::X3DPrototypeInstance (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DPrototypeInstance, vp);
}

JSBool
jsX3DConstants::X3DRigidJointNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DRigidJointNode, vp);
}

JSBool
jsX3DConstants::X3DScriptNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DScriptNode, vp);
}

JSBool
jsX3DConstants::X3DSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DSensorNode, vp);
}

JSBool
jsX3DConstants::X3DSequencerNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DSequencerNode, vp);
}

JSBool
jsX3DConstants::X3DShaderNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DShaderNode, vp);
}

JSBool
jsX3DConstants::X3DShapeNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DShapeNode, vp);
}

JSBool
jsX3DConstants::X3DSoundNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DSoundNode, vp);
}

JSBool
jsX3DConstants::X3DSoundSourceNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DSoundSourceNode, vp);
}

JSBool
jsX3DConstants::X3DTexture2DNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTexture2DNode, vp);
}

JSBool
jsX3DConstants::X3DTexture3DNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTexture3DNode, vp);
}

JSBool
jsX3DConstants::X3DTextureCoordinateNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTextureCoordinateNode, vp);
}

JSBool
jsX3DConstants::X3DTextureNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTextureNode, vp);
}

JSBool
jsX3DConstants::X3DTextureTransformNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTextureTransformNode, vp);
}

JSBool
jsX3DConstants::X3DTimeDependentNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTimeDependentNode, vp);
}

JSBool
jsX3DConstants::X3DTouchSensorNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTouchSensorNode, vp);
}

JSBool
jsX3DConstants::X3DTransformMatrix4DNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTransformMatrix4DNode, vp);
}

JSBool
jsX3DConstants::X3DTransformNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTransformNode, vp);
}

JSBool
jsX3DConstants::X3DTriggerNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DTriggerNode, vp);
}

JSBool
jsX3DConstants::X3DUrlObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DUrlObject, vp);
}

JSBool
jsX3DConstants::X3DVertexAttributeNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DVertexAttributeNode, vp);
}

JSBool
jsX3DConstants::X3DViewpointNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DViewpointNode, vp);
}

JSBool
jsX3DConstants::X3DViewpointObject (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DViewpointObject, vp);
}

JSBool
jsX3DConstants::X3DViewportNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DViewportNode, vp);
}

// Tools

JSBool
jsX3DConstants::AngleTool (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::AngleTool, vp);
}

JSBool
jsX3DConstants::GridTool (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::GridTool, vp);
}

JSBool
jsX3DConstants::X3DGridTool (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (context, X3DConstants::X3DGridTool, vp);
}

} // MozillaSpiderMonkey
} // X3D
} // titania
