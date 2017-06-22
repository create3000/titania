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

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Scripting/Script.h"
#include "../../InputOutput/Generator.h"
#include "Fields/ArrayFields.h"
#include "Fields/MFNode.h"
#include "Fields/SFNode.h"
#include "ComponentInfoArray.h"
#include "Context.h"
#include "ProfileInfoArray.h"
#include "String.h"
#include "X3DExecutionContext.h"
#include "X3DScene.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

JSClass X3DConstants::static_class = {
	"X3DConstants", 0,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec X3DConstants::properties [ ] = {
	// Event types

	{ "INITIALIZED_EVENT", JS_INITIALIZED_EVENT, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, INITIALIZED_EVENT, nullptr },
	{ "SHUTDOWN_EVENT",    JS_SHUTDOWN_EVENT,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SHUTDOWN_EVENT,    nullptr },

	// Error types

	{ "CONNECTION_ERROR",  JS_CONNECTION_ERROR,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CONNECTION_ERROR,  nullptr },
	{ "INITIALIZED_ERROR", JS_INITIALIZED_ERROR, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, INITIALIZED_ERROR, nullptr },

	// Load state

	{ "NOT_STARTED_STATE", JS_NOT_STARTED_STATE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NOT_STARTED_STATE, nullptr },
	{ "IN_PROGRESS_STATE", JS_IN_PROGRESS_STATE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IN_PROGRESS_STATE, nullptr },
	{ "COMPLETE_STATE",    JS_COMPLETE_STATE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, COMPLETE_STATE,    nullptr },
	{ "FAILED_STATE",      JS_FAILED_STATE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FAILED_STATE,      nullptr },

	// Unit category

	{ "ANGLE",        0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ANGLE,        nullptr },
	{ "FORCE",        0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FORCE,        nullptr },
	{ "LENGTH",       0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LENGTH,       nullptr },
	{ "MASS",         0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MASS,         nullptr },
	{ "ACCELERATION", 0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ACCELERATION, nullptr },
	{ "ANGULAR_RATE", 0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ANGULAR_RATE, nullptr },
	{ "AREA",         0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, AREA,         nullptr },
	{ "SPEED",        0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SPEED,        nullptr },
	{ "VOLUME",       0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, VOLUME,       nullptr },

	// Access types

	{ "initializeOnly", JS_INITIALIZE_ONLY, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, initializeOnly, nullptr },
	{ "inputOnly",      JS_INPUT_ONLY,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, inputOnly,      nullptr },
	{ "outputOnly",     JS_OUTPUT_ONLY,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, outputOnly,     nullptr },
	{ "inputOutput",    JS_INPUT_OUTPUT,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, inputOutput,    nullptr },

	// Field types

	{ "SFBool",      JS_SFBOOL,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFBool,      nullptr },
	{ "SFColor",     JS_SFCOLOR,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFColor,     nullptr },
	{ "SFColorRGBA", JS_SFCOLORRGBA, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFColorRGBA, nullptr },
	{ "SFDouble",    JS_SFDOUBLE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFDouble,    nullptr },
	{ "SFFloat",     JS_SFFLOAT,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFFloat,     nullptr },
	{ "SFInt32",     JS_SFINT32,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFInt32,     nullptr },
	{ "SFImage",     JS_SFIMAGE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFImage,     nullptr },
	{ "SFMatrix3d",  JS_SFMATRIX3D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix3d,  nullptr },
	{ "SFMatrix3f",  JS_SFMATRIX3F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix3f,  nullptr },
	{ "SFMatrix4d",  JS_SFMATRIX4D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix4d,  nullptr },
	{ "SFMatrix4f",  JS_SFMATRIX4F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFMatrix4f,  nullptr },
	{ "SFNode",      JS_SFNODE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFNode,      nullptr },
	{ "SFRotation",  JS_SFROTATION,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFRotation,  nullptr },
	{ "SFString",    JS_SFSTRING,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFString,    nullptr },
	{ "SFTime",      JS_SFTIME,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFTime,      nullptr },
	{ "SFVec2d",     JS_SFVEC2D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec2d,     nullptr },
	{ "SFVec2f",     JS_SFVEC2F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec2f,     nullptr },
	{ "SFVec3d",     JS_SFVEC3D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec3d,     nullptr },
	{ "SFVec3f",     JS_SFVEC3F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec3f,     nullptr },
	{ "SFVec4d",     JS_SFVEC4D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec4d,     nullptr },
	{ "SFVec4f",     JS_SFVEC4F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SFVec4f,     nullptr },

	{ "MFBool",      JS_MFBOOL,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFBool,      nullptr },
	{ "MFColor",     JS_MFCOLOR,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFColor,     nullptr },
	{ "MFColorRGBA", JS_MFCOLORRGBA, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFColorRGBA, nullptr },
	{ "MFDouble",    JS_MFDOUBLE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFDouble,    nullptr },
	{ "MFFloat",     JS_MFFLOAT,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFFloat,     nullptr },
	{ "MFImage",     JS_MFIMAGE,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFImage,     nullptr },
	{ "MFInt32",     JS_MFINT32,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFInt32,     nullptr },
	{ "MFMatrix3d",  JS_MFMATRIX3D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix3d,  nullptr },
	{ "MFMatrix3f",  JS_MFMATRIX3F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix3f,  nullptr },
	{ "MFMatrix4d",  JS_MFMATRIX4D,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix4d,  nullptr },
	{ "MFMatrix4f",  JS_MFMATRIX4F,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFMatrix4f,  nullptr },
	{ "MFNode",      JS_MFNODE,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFNode,      nullptr },
	{ "MFRotation",  JS_MFROTATION,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFRotation,  nullptr },
	{ "MFString",    JS_MFSTRING,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFString,    nullptr },
	{ "MFTime",      JS_MFTIME,      JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFTime,      nullptr },
	{ "MFVec2d",     JS_MFVEC2D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec2d,     nullptr },
	{ "MFVec2f",     JS_MFVEC2F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec2f,     nullptr },
	{ "MFVec3d",     JS_MFVEC3D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec3d,     nullptr },
	{ "MFVec3f",     JS_MFVEC3F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec3f,     nullptr },
	{ "MFVec4d",     JS_MFVEC4D,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec4d,     nullptr },
	{ "MFVec4f",     JS_MFVEC4F,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MFVec4f,     nullptr },

	// Base node type

	{ "X3DBaseNode", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBaseNode, nullptr },

	// Basic node types

	{ "Anchor",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Anchor,                       nullptr },
	{ "Appearance",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Appearance,                   nullptr },
	{ "Arc2D",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Arc2D,                        nullptr },
	{ "ArcClose2D",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ArcClose2D,                   nullptr },
	{ "AudioClip",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, AudioClip,                    nullptr },
	{ "Background",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Background,                   nullptr },
	{ "BallJoint",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BallJoint,                    nullptr },
	{ "Billboard",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Billboard,                    nullptr },
	{ "BooleanFilter",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BooleanFilter,                nullptr },
	{ "BooleanSequencer",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BooleanSequencer,             nullptr },
	{ "BooleanToggle",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BooleanToggle,                nullptr },
	{ "BooleanTrigger",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BooleanTrigger,               nullptr },
	{ "BoundedPhysicsModel",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, BoundedPhysicsModel,          nullptr },
	{ "Box",                          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Box,                          nullptr },
	{ "CADAssembly",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CADAssembly,                  nullptr },
	{ "CADFace",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CADFace,                      nullptr },
	{ "CADLayer",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CADLayer,                     nullptr },
	{ "CADPart",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CADPart,                      nullptr },
	{ "Circle2D",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Circle2D,                     nullptr },
	{ "ClipPlane",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ClipPlane,                    nullptr },
	{ "CollidableOffset",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollidableOffset,             nullptr },
	{ "CollidableShape",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollidableShape,              nullptr },
	{ "Collision",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Collision,                    nullptr },
	{ "CollisionCollection",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollisionCollection,          nullptr },
	{ "CollisionSensor",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollisionSensor,              nullptr },
	{ "CollisionSpace",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CollisionSpace,               nullptr },
	{ "Color",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Color,                        nullptr },
	{ "ColorChaser",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ColorChaser,                  nullptr },
	{ "ColorDamper",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ColorDamper,                  nullptr },
	{ "ColorInterpolator",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ColorInterpolator,            nullptr },
	{ "ColorRGBA",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ColorRGBA,                    nullptr },
	{ "ComposedCubeMapTexture",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ComposedCubeMapTexture,       nullptr },
	{ "ComposedShader",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ComposedShader,               nullptr },
	{ "ComposedTexture3D",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ComposedTexture3D,            nullptr },
	{ "Cone",                         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Cone,                         nullptr },
	{ "ConeEmitter",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ConeEmitter,                  nullptr },
	{ "Contact",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Contact,                      nullptr },
	{ "Contour2D",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Contour2D,                    nullptr },
	{ "ContourPolyline2D",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ContourPolyline2D,            nullptr },
	{ "Coordinate",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Coordinate,                   nullptr },
	{ "CoordinateChaser",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateChaser,             nullptr },
	{ "CoordinateDamper",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateDamper,             nullptr },
	{ "CoordinateDouble",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateDouble,             nullptr },
	{ "CoordinateInterpolator",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateInterpolator,       nullptr },
	{ "CoordinateInterpolator2D",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CoordinateInterpolator2D,     nullptr },
	{ "Cylinder",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Cylinder,                     nullptr },
	{ "CylinderSensor",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, CylinderSensor,               nullptr },
	{ "DISEntityManager",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, DISEntityManager,             nullptr },
	{ "DISEntityTypeMapping",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, DISEntityTypeMapping,         nullptr },
	{ "DirectionalLight",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, DirectionalLight,             nullptr },
	{ "Disk2D",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Disk2D,                       nullptr },
	{ "DoubleAxisHingeJoint",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, DoubleAxisHingeJoint,         nullptr },
	{ "EaseInEaseOut",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, EaseInEaseOut,                nullptr },
	{ "ElevationGrid",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ElevationGrid,                nullptr },
	{ "EspduTransform",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, EspduTransform,               nullptr },
	{ "ExplosionEmitter",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ExplosionEmitter,             nullptr },
	{ "Extrusion",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Extrusion,                    nullptr },
	{ "FillProperties",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FillProperties,               nullptr },
	{ "FloatVertexAttribute",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FloatVertexAttribute,         nullptr },
	{ "Fog",                          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Fog,                          nullptr },
	{ "FogCoordinate",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FogCoordinate,                nullptr },
	{ "FontStyle",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, FontStyle,                    nullptr },
	{ "ForcePhysicsModel",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ForcePhysicsModel,            nullptr },
	{ "GeneratedCubeMapTexture",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeneratedCubeMapTexture,      nullptr },
	{ "GeoCoordinate",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoCoordinate,                nullptr },
	{ "GeoElevationGrid",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoElevationGrid,             nullptr },
	{ "GeoLOD",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoLOD,                       nullptr },
	{ "GeoLocation",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoLocation,                  nullptr },
	{ "GeoMetadata",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoMetadata,                  nullptr },
	{ "GeoOrigin",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoOrigin,                    nullptr },
	{ "GeoPositionInterpolator",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoPositionInterpolator,      nullptr },
	{ "GeoProximitySensor",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoProximitySensor,           nullptr },
	{ "GeoTouchSensor",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoTouchSensor,               nullptr },
	{ "GeoTransform",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoTransform,                 nullptr },
	{ "GeoViewpoint",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GeoViewpoint,                 nullptr },
	{ "Group",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Group,                        nullptr },
	{ "HAnimDisplacer",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimDisplacer,               nullptr },
	{ "HAnimHumanoid",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimHumanoid,                nullptr },
	{ "HAnimJoint",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimJoint,                   nullptr },
	{ "HAnimSegment",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimSegment,                 nullptr },
	{ "HAnimSite",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, HAnimSite,                    nullptr },
	{ "ImageCubeMapTexture",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ImageCubeMapTexture,          nullptr },
	{ "ImageTexture",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ImageTexture,                 nullptr },
	{ "ImageTexture3D",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ImageTexture3D,               nullptr },
	{ "IndexedFaceSet",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedFaceSet,               nullptr },
	{ "IndexedLineSet",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedLineSet,               nullptr },
	{ "IndexedQuadSet",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedQuadSet,               nullptr },
	{ "IndexedTriangleFanSet",        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedTriangleFanSet,        nullptr },
	{ "IndexedTriangleSet",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedTriangleSet,           nullptr },
	{ "IndexedTriangleStripSet",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IndexedTriangleStripSet,      nullptr },
	{ "Inline",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Inline,                       nullptr },
	{ "IntegerSequencer",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IntegerSequencer,             nullptr },
	{ "IntegerTrigger",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, IntegerTrigger,               nullptr },
	{ "KeySensor",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, KeySensor,                    nullptr },
	{ "LOD",                          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LOD,                          nullptr },
	{ "Layer",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Layer,                        nullptr },
	{ "LayerSet",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LayerSet,                     nullptr },
	{ "Layout",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Layout,                       nullptr },
	{ "LayoutGroup",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LayoutGroup,                  nullptr },
	{ "LayoutLayer",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LayoutLayer,                  nullptr },
	{ "LinePickSensor",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LinePickSensor,               nullptr },
	{ "LineProperties",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LineProperties,               nullptr },
	{ "LineSet",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LineSet,                      nullptr },
	{ "LoadSensor",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LoadSensor,                   nullptr },
	{ "LocalFog",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, LocalFog,                     nullptr },
	{ "Material",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Material,                     nullptr },
	{ "Matrix3VertexAttribute",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Matrix3VertexAttribute,       nullptr },
	{ "Matrix4VertexAttribute",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Matrix4VertexAttribute,       nullptr },
	{ "MetadataBoolean",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataBoolean,              nullptr },
	{ "MetadataDouble",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataDouble,               nullptr },
	{ "MetadataFloat",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataFloat,                nullptr },
	{ "MetadataInteger",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataInteger,              nullptr },
	{ "MetadataSet",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataSet,                  nullptr },
	{ "MetadataString",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MetadataString,               nullptr },
	{ "MotorJoint",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MotorJoint,                   nullptr },
	{ "MovieTexture",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MovieTexture,                 nullptr },
	{ "MultiTexture",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MultiTexture,                 nullptr },
	{ "MultiTextureCoordinate",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MultiTextureCoordinate,       nullptr },
	{ "MultiTextureTransform",        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, MultiTextureTransform,        nullptr },
	{ "NavigationInfo",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NavigationInfo,               nullptr },
	{ "Normal",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Normal,                       nullptr },
	{ "NormalInterpolator",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NormalInterpolator,           nullptr },
	{ "NurbsCurve",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsCurve,                   nullptr },
	{ "NurbsCurve2D",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsCurve2D,                 nullptr },
	{ "NurbsOrientationInterpolator", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsOrientationInterpolator, nullptr },
	{ "NurbsPatchSurface",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsPatchSurface,            nullptr },
	{ "NurbsPositionInterpolator",    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsPositionInterpolator,    nullptr },
	{ "NurbsSet",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsSet,                     nullptr },
	{ "NurbsSurfaceInterpolator",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsSurfaceInterpolator,     nullptr },
	{ "NurbsSweptSurface",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsSweptSurface,            nullptr },
	{ "NurbsSwungSurface",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsSwungSurface,            nullptr },
	{ "NurbsTextureCoordinate",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsTextureCoordinate,       nullptr },
	{ "NurbsTrimmedSurface",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, NurbsTrimmedSurface,          nullptr },
	{ "OrientationChaser",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, OrientationChaser,            nullptr },
	{ "OrientationDamper",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, OrientationDamper,            nullptr },
	{ "OrientationInterpolator",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, OrientationInterpolator,      nullptr },
	{ "OrthoViewpoint",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, OrthoViewpoint,               nullptr },
	{ "PackagedShader",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PackagedShader,               nullptr },
	{ "ParticleSystem",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ParticleSystem,               nullptr },
	{ "PickableGroup",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PickableGroup,                nullptr },
	{ "PixelTexture",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PixelTexture,                 nullptr },
	{ "PixelTexture3D",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PixelTexture3D,               nullptr },
	{ "PlaneSensor",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PlaneSensor,                  nullptr },
	{ "PointEmitter",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PointEmitter,                 nullptr },
	{ "PointLight",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PointLight,                   nullptr },
	{ "PointPickSensor",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PointPickSensor,              nullptr },
	{ "PointSet",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PointSet,                     nullptr },
	{ "Polyline2D",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Polyline2D,                   nullptr },
	{ "PolylineEmitter",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PolylineEmitter,              nullptr },
	{ "Polypoint2D",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Polypoint2D,                  nullptr },
	{ "PositionChaser",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionChaser,               nullptr },
	{ "PositionChaser2D",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionChaser2D,             nullptr },
	{ "PositionDamper",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionDamper,               nullptr },
	{ "PositionDamper2D",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionDamper2D,             nullptr },
	{ "PositionInterpolator",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionInterpolator,         nullptr },
	{ "PositionInterpolator2D",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PositionInterpolator2D,       nullptr },
	{ "PrimitivePickSensor",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, PrimitivePickSensor,          nullptr },
	{ "ProgramShader",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ProgramShader,                nullptr },
	{ "ProximitySensor",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ProximitySensor,              nullptr },
	{ "QuadSet",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, QuadSet,                      nullptr },
	{ "ReceiverPdu",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ReceiverPdu,                  nullptr },
	{ "Rectangle2D",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Rectangle2D,                  nullptr },
	{ "RigidBody",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, RigidBody,                    nullptr },
	{ "RigidBodyCollection",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, RigidBodyCollection,          nullptr },
	{ "ScalarChaser",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScalarChaser,                 nullptr },
	{ "ScalarDamper",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScalarDamper,                 nullptr },
	{ "ScalarInterpolator",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScalarInterpolator,           nullptr },
	{ "ScreenFontStyle",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScreenFontStyle,              nullptr },
	{ "ScreenGroup",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ScreenGroup,                  nullptr },
	{ "Script",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Script,                       nullptr },
	{ "ShaderPart",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ShaderPart,                   nullptr },
	{ "ShaderProgram",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ShaderProgram,                nullptr },
	{ "Shape",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Shape,                        nullptr },
	{ "SignalPdu",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SignalPdu,                    nullptr },
	{ "SingleAxisHingeJoint",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SingleAxisHingeJoint,         nullptr },
	{ "SliderJoint",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SliderJoint,                  nullptr },
	{ "Sound",                        JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Sound,                        nullptr },
	{ "Sphere",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Sphere,                       nullptr },
	{ "SphereSensor",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SphereSensor,                 nullptr },
	{ "SplinePositionInterpolator",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SplinePositionInterpolator,   nullptr },
	{ "SplinePositionInterpolator2D", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SplinePositionInterpolator2D, nullptr },
	{ "SplineScalarInterpolator",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SplineScalarInterpolator,     nullptr },
	{ "SpotLight",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SpotLight,                    nullptr },
	{ "SquadOrientationInterpolator", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SquadOrientationInterpolator, nullptr },
	{ "StaticGroup",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, StaticGroup,                  nullptr },
	{ "StringSensor",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, StringSensor,                 nullptr },
	{ "SurfaceEmitter",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, SurfaceEmitter,               nullptr },
	{ "Switch",                       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Switch,                       nullptr },
	{ "TexCoordChaser2D",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TexCoordChaser2D,             nullptr },
	{ "TexCoordDamper2D",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TexCoordDamper2D,             nullptr },
	{ "Text",                         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Text,                         nullptr },
	{ "TextureBackground",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureBackground,            nullptr },
	{ "TextureCoordinate",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureCoordinate,            nullptr },
	{ "TextureCoordinate3D",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureCoordinate3D,          nullptr },
	{ "TextureCoordinate4D",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureCoordinate4D,          nullptr },
	{ "TextureCoordinateGenerator",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureCoordinateGenerator,   nullptr },
	{ "TextureProperties",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureProperties,            nullptr },
	{ "TextureTransform",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureTransform,             nullptr },
	{ "TextureTransform3D",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureTransform3D,           nullptr },
	{ "TextureTransformMatrix3D",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TextureTransformMatrix3D,     nullptr },
	{ "TimeSensor",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TimeSensor,                   nullptr },
	{ "TimeTrigger",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TimeTrigger,                  nullptr },
	{ "TouchGroup",                   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TouchGroup,                   nullptr },
	{ "TouchSensor",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TouchSensor,                  nullptr },
	{ "Transform",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Transform,                    nullptr },
	{ "TransformMatrix3D",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TransformMatrix3D,            nullptr },
	{ "TransformSensor",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TransformSensor,              nullptr },
	{ "TransmitterPdu",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TransmitterPdu,               nullptr },
	{ "TriangleFanSet",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TriangleFanSet,               nullptr },
	{ "TriangleSet",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TriangleSet,                  nullptr },
	{ "TriangleSet2D",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TriangleSet2D,                nullptr },
	{ "TriangleStripSet",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TriangleStripSet,             nullptr },
	{ "TwoSidedMaterial",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, TwoSidedMaterial,             nullptr },
	{ "UniversalJoint",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, UniversalJoint,               nullptr },
	{ "Viewpoint",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Viewpoint,                    nullptr },
	{ "ViewpointGroup",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, ViewpointGroup,               nullptr },
	{ "Viewport",                     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, Viewport,                     nullptr },
	{ "VisibilitySensor",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, VisibilitySensor,             nullptr },
	{ "VolumeEmitter",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, VolumeEmitter,                nullptr },
	{ "VolumePickSensor",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, VolumePickSensor,             nullptr },
	{ "WindPhysicsModel",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, WindPhysicsModel,             nullptr },
	{ "WorldInfo",                    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, WorldInfo,                    nullptr },

	// Abstract node types

	{ "X3DAppearanceChildNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DAppearanceChildNode,       nullptr },
	{ "X3DAppearanceNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DAppearanceNode,            nullptr },
	{ "X3DBackgroundNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBackgroundNode,            nullptr },
	{ "X3DBindableNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBindableNode,              nullptr },
	{ "X3DBoundedObject",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DBoundedObject,             nullptr },
	{ "X3DChaserNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DChaserNode,                nullptr },
	{ "X3DChildNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DChildNode,                 nullptr },
	{ "X3DColorNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DColorNode,                 nullptr },
	{ "X3DComposedGeometryNode",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DComposedGeometryNode,      nullptr },
	{ "X3DCoordinateNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DCoordinateNode,            nullptr },
	{ "X3DDamperNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DDamperNode,                nullptr },
	{ "X3DDragSensorNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DDragSensorNode,            nullptr },
	{ "X3DEnvironmentTextureNode",    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DEnvironmentTextureNode,    nullptr },
	{ "X3DEnvironmentalSensorNode",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DEnvironmentalSensorNode,   nullptr },
	{ "X3DFogObject",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFogObject,                 nullptr },
	{ "X3DFollowerNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFollowerNode,              nullptr },
	{ "X3DFontStyleNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DFontStyleNode,             nullptr },
	{ "X3DGeometricPropertyNode",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGeometricPropertyNode,     nullptr },
	{ "X3DGeometryNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGeometryNode,              nullptr },
	{ "X3DGeospatialObject",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGeospatialObject,          nullptr },
	{ "X3DGroupingNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGroupingNode,              nullptr },
	{ "X3DInfoNode",                  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DInfoNode,                  nullptr },
	{ "X3DInterpolatorNode",          JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DInterpolatorNode,          nullptr },
	{ "X3DKeyDeviceSensorNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DKeyDeviceSensorNode,       nullptr },
	{ "X3DLayerNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLayerNode,                 nullptr },
	{ "X3DLayoutNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLayoutNode,                nullptr },
	{ "X3DLightNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DLightNode,                 nullptr },
	{ "X3DMaterialNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DMaterialNode,              nullptr },
	{ "X3DMetadataObject",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DMetadataObject,            nullptr },
	{ "X3DNBodyCollidableNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNBodyCollidableNode,       nullptr },
	{ "X3DNBodyCollisionSpaceNode",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNBodyCollisionSpaceNode,   nullptr },
	{ "X3DNetworkSensorNode",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNetworkSensorNode,         nullptr },
	{ "X3DNode",                      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNode,                      nullptr },
	{ "X3DNormalNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNormalNode,                nullptr },
	{ "X3DNurbsControlCurveNode",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNurbsControlCurveNode,     nullptr },
	{ "X3DNurbsSurfaceGeometryNode",  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DNurbsSurfaceGeometryNode,  nullptr },
	{ "X3DParametricGeometryNode",    JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParametricGeometryNode,    nullptr },
	{ "X3DParticleEmitterNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParticleEmitterNode,       nullptr },
	{ "X3DParticlePhysicsModelNode",  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DParticlePhysicsModelNode,  nullptr },
	{ "X3DPickSensorNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPickSensorNode,            nullptr },
	{ "X3DPickableObject",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPickableObject,            nullptr },
	{ "X3DPointingDeviceSensorNode",  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPointingDeviceSensorNode,  nullptr },
	{ "X3DProductStructureChildNode", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DProductStructureChildNode, nullptr },
	{ "X3DProgrammableShaderObject",  JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DProgrammableShaderObject,  nullptr },
	{ "X3DPrototypeInstance",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DPrototypeInstance,         nullptr },
	{ "X3DRigidJointNode",            JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DRigidJointNode,            nullptr },
	{ "X3DScriptNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DScriptNode,                nullptr },
	{ "X3DSensorNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSensorNode,                nullptr },
	{ "X3DSequencerNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSequencerNode,             nullptr },
	{ "X3DShaderNode",                JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DShaderNode,                nullptr },
	{ "X3DShapeNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DShapeNode,                 nullptr },
	{ "X3DSoundNode",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSoundNode,                 nullptr },
	{ "X3DSoundSourceNode",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DSoundSourceNode,           nullptr },
	{ "X3DTexture2DNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTexture2DNode,             nullptr },
	{ "X3DTexture3DNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTexture3DNode,             nullptr },
	{ "X3DTextureCoordinateNode",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureCoordinateNode,     nullptr },
	{ "X3DTextureNode",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureNode,               nullptr },
	{ "X3DTextureTransformNode",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTextureTransformNode,      nullptr },
	{ "X3DTimeDependentNode",         JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTimeDependentNode,         nullptr },
	{ "X3DTouchSensorNode",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTouchSensorNode,           nullptr },
	{ "X3DTransformMatrix3DNode",     JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTransformMatrix3DNode,     nullptr },
	{ "X3DTransformMatrix3DObject",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTransformMatrix3DObject,   nullptr },
	{ "X3DTransformNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTransformNode,             nullptr },
	{ "X3DTriggerNode",               JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DTriggerNode,               nullptr },
	{ "X3DUrlObject",                 JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DUrlObject,                 nullptr },
	{ "X3DVertexAttributeNode",       JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DVertexAttributeNode,       nullptr },
	{ "X3DViewpointNode",             JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewpointNode,             nullptr },
	{ "X3DViewpointObject",           JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewpointObject,           nullptr },
	{ "X3DViewportNode",              JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DViewportNode,              nullptr },

	// Tools

	{ "AngleGridTool", JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, AngleGridTool,   nullptr },
	{ "GridTool",      JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, GridTool,    nullptr },
	{ "X3DGridTool",   JS_X3D_BASE_NODE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, X3DGridTool, nullptr },

	{ 0 }

};

JSFunctionSpec X3DConstants::functions [ ] = {
	{ 0, 0, 0, 0 }

};

JSObject*
X3DConstants::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineObject (cx, global, "X3DConstants", &static_class, nullptr, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT);
	
	return proto;
}

// Event types

JSBool
X3DConstants::INITIALIZED_EVENT (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::INITIALIZED_EVENT, vp);
}

JSBool
X3DConstants::SHUTDOWN_EVENT (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SHUTDOWN_EVENT, vp);
}

// Error types

JSBool
X3DConstants::CONNECTION_ERROR (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CONNECTION_ERROR, vp);
}

JSBool
X3DConstants::INITIALIZED_ERROR (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::INITIALIZED_ERROR, vp);
}

// Load state

JSBool
X3DConstants::NOT_STARTED_STATE (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NOT_STARTED_STATE, vp);
}

JSBool
X3DConstants::IN_PROGRESS_STATE (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IN_PROGRESS_STATE, vp);
}

JSBool
X3DConstants::COMPLETE_STATE (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::COMPLETE_STATE, vp);
}

JSBool
X3DConstants::FAILED_STATE (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::FAILED_STATE, vp);
}

JSBool
X3DConstants::ANGLE (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::ANGLE, vp);
}

JSBool
X3DConstants::FORCE (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::FORCE, vp);
}

JSBool
X3DConstants::LENGTH (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::LENGTH, vp);
}

JSBool
X3DConstants::MASS (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::MASS, vp);
}

JSBool
X3DConstants::ACCELERATION (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::ACCELERATION, vp);
}

JSBool
X3DConstants::ANGULAR_RATE (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::ANGULAR_RATE, vp);
}

JSBool
X3DConstants::AREA (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::AREA, vp);
}

JSBool
X3DConstants::SPEED (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::SPEED, vp);
}

JSBool
X3DConstants::VOLUME (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, (double) X3D::UnitCategory::VOLUME, vp);
}

// Access types

JSBool
X3DConstants::initializeOnly (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::initializeOnly, vp);
}

JSBool
X3DConstants::inputOnly (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::inputOnly, vp);
}

JSBool
X3DConstants::outputOnly (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::outputOnly, vp);
}

JSBool
X3DConstants::inputOutput (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::inputOutput, vp);
}

// X3D properties

JSBool
X3DConstants::SFBool (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFBool, vp);
}

JSBool
X3DConstants::SFColor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFColor, vp);
}

JSBool
X3DConstants::SFColorRGBA (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFColorRGBA, vp);
}

JSBool
X3DConstants::SFDouble (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFDouble, vp);
}

JSBool
X3DConstants::SFFloat (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFFloat, vp);
}

JSBool
X3DConstants::SFInt32 (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFInt32, vp);
}

JSBool
X3DConstants::SFImage (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFImage, vp);
}

JSBool
X3DConstants::SFMatrix3d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFMatrix3d, vp);
}

JSBool
X3DConstants::SFMatrix3f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFMatrix3f, vp);
}

JSBool
X3DConstants::SFMatrix4d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFMatrix4d, vp);
}

JSBool
X3DConstants::SFMatrix4f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFMatrix4f, vp);
}

JSBool
X3DConstants::SFNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFNode, vp);
}

JSBool
X3DConstants::SFRotation (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFRotation, vp);
}

JSBool
X3DConstants::SFString (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFString, vp);
}

JSBool
X3DConstants::SFTime (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFTime, vp);
}

JSBool
X3DConstants::SFVec2d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFVec2d, vp);
}

JSBool
X3DConstants::SFVec2f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFVec2f, vp);
}

JSBool
X3DConstants::SFVec3d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFVec3d, vp);
}

JSBool
X3DConstants::SFVec3f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFVec3f, vp);
}

JSBool
X3DConstants::SFVec4d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFVec4d, vp);
}

JSBool
X3DConstants::SFVec4f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SFVec4f, vp);
}

JSBool
X3DConstants::MFBool (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFBool, vp);
}

JSBool
X3DConstants::MFColor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFColor, vp);
}

JSBool
X3DConstants::MFColorRGBA (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFColorRGBA, vp);
}

JSBool
X3DConstants::MFDouble (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFDouble, vp);
}

JSBool
X3DConstants::MFFloat (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFFloat, vp);
}

JSBool
X3DConstants::MFImage (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFImage, vp);
}

JSBool
X3DConstants::MFInt32 (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFInt32, vp);
}

JSBool
X3DConstants::MFMatrix3d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFMatrix3d, vp);
}

JSBool
X3DConstants::MFMatrix3f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFMatrix3f, vp);
}

JSBool
X3DConstants::MFMatrix4d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFMatrix4d, vp);
}

JSBool
X3DConstants::MFMatrix4f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFMatrix4f, vp);
}

JSBool
X3DConstants::MFNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFNode, vp);
}

JSBool
X3DConstants::MFRotation (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFRotation, vp);
}

JSBool
X3DConstants::MFString (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFString, vp);
}

JSBool
X3DConstants::MFTime (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFTime, vp);
}

JSBool
X3DConstants::MFVec2d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFVec2d, vp);
}

JSBool
X3DConstants::MFVec2f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFVec2f, vp);
}

JSBool
X3DConstants::MFVec3d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFVec3d, vp);
}

JSBool
X3DConstants::MFVec3f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFVec3f, vp);
}

JSBool
X3DConstants::MFVec4d (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFVec4d, vp);
}

JSBool
X3DConstants::MFVec4f (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MFVec4f, vp);
}

// Base node type

JSBool
X3DConstants::X3DBaseNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DBaseNode, vp);
}

// Basic node types

JSBool
X3DConstants::Anchor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Anchor, vp);
}

JSBool
X3DConstants::Appearance (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Appearance, vp);
}

JSBool
X3DConstants::Arc2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Arc2D, vp);
}

JSBool
X3DConstants::ArcClose2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ArcClose2D, vp);
}

JSBool
X3DConstants::AudioClip (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::AudioClip, vp);
}

JSBool
X3DConstants::Background (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Background, vp);
}

JSBool
X3DConstants::BallJoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::BallJoint, vp);
}

JSBool
X3DConstants::Billboard (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Billboard, vp);
}

JSBool
X3DConstants::BooleanFilter (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::BooleanFilter, vp);
}

JSBool
X3DConstants::BooleanSequencer (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::BooleanSequencer, vp);
}

JSBool
X3DConstants::BooleanToggle (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::BooleanToggle, vp);
}

JSBool
X3DConstants::BooleanTrigger (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::BooleanTrigger, vp);
}

JSBool
X3DConstants::BoundedPhysicsModel (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::BoundedPhysicsModel, vp);
}

JSBool
X3DConstants::Box (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Box, vp);
}

JSBool
X3DConstants::CADAssembly (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CADAssembly, vp);
}

JSBool
X3DConstants::CADFace (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CADFace, vp);
}

JSBool
X3DConstants::CADLayer (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CADLayer, vp);
}

JSBool
X3DConstants::CADPart (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CADPart, vp);
}

JSBool
X3DConstants::Circle2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Circle2D, vp);
}

JSBool
X3DConstants::ClipPlane (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ClipPlane, vp);
}

JSBool
X3DConstants::CollidableOffset (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CollidableOffset, vp);
}

JSBool
X3DConstants::CollidableShape (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CollidableShape, vp);
}

JSBool
X3DConstants::Collision (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Collision, vp);
}

JSBool
X3DConstants::CollisionCollection (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CollisionCollection, vp);
}

JSBool
X3DConstants::CollisionSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CollisionSensor, vp);
}

JSBool
X3DConstants::CollisionSpace (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CollisionSpace, vp);
}

JSBool
X3DConstants::Color (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Color, vp);
}

JSBool
X3DConstants::ColorChaser (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ColorChaser, vp);
}

JSBool
X3DConstants::ColorDamper (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ColorDamper, vp);
}

JSBool
X3DConstants::ColorInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ColorInterpolator, vp);
}

JSBool
X3DConstants::ColorRGBA (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ColorRGBA, vp);
}

JSBool
X3DConstants::ComposedCubeMapTexture (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ComposedCubeMapTexture, vp);
}

JSBool
X3DConstants::ComposedShader (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ComposedShader, vp);
}

JSBool
X3DConstants::ComposedTexture3D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ComposedTexture3D, vp);
}

JSBool
X3DConstants::Cone (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Cone, vp);
}

JSBool
X3DConstants::ConeEmitter (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ConeEmitter, vp);
}

JSBool
X3DConstants::Contact (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Contact, vp);
}

JSBool
X3DConstants::Contour2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Contour2D, vp);
}

JSBool
X3DConstants::ContourPolyline2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ContourPolyline2D, vp);
}

JSBool
X3DConstants::Coordinate (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Coordinate, vp);
}

JSBool
X3DConstants::CoordinateChaser (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CoordinateChaser, vp);
}

JSBool
X3DConstants::CoordinateDamper (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CoordinateDamper, vp);
}

JSBool
X3DConstants::CoordinateDouble (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CoordinateDouble, vp);
}

JSBool
X3DConstants::CoordinateInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CoordinateInterpolator, vp);
}

JSBool
X3DConstants::CoordinateInterpolator2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CoordinateInterpolator2D, vp);
}

JSBool
X3DConstants::Cylinder (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Cylinder, vp);
}

JSBool
X3DConstants::CylinderSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::CylinderSensor, vp);
}

JSBool
X3DConstants::DISEntityManager (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::DISEntityManager, vp);
}

JSBool
X3DConstants::DISEntityTypeMapping (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::DISEntityTypeMapping, vp);
}

JSBool
X3DConstants::DirectionalLight (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::DirectionalLight, vp);
}

JSBool
X3DConstants::Disk2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Disk2D, vp);
}

JSBool
X3DConstants::DoubleAxisHingeJoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::DoubleAxisHingeJoint, vp);
}

JSBool
X3DConstants::EaseInEaseOut (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::EaseInEaseOut, vp);
}

JSBool
X3DConstants::ElevationGrid (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ElevationGrid, vp);
}

JSBool
X3DConstants::EspduTransform (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::EspduTransform, vp);
}

JSBool
X3DConstants::ExplosionEmitter (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ExplosionEmitter, vp);
}

JSBool
X3DConstants::Extrusion (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Extrusion, vp);
}

JSBool
X3DConstants::FillProperties (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::FillProperties, vp);
}

JSBool
X3DConstants::FloatVertexAttribute (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::FloatVertexAttribute, vp);
}

JSBool
X3DConstants::Fog (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Fog, vp);
}

JSBool
X3DConstants::FogCoordinate (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::FogCoordinate, vp);
}

JSBool
X3DConstants::FontStyle (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::FontStyle, vp);
}

JSBool
X3DConstants::ForcePhysicsModel (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ForcePhysicsModel, vp);
}

JSBool
X3DConstants::GeneratedCubeMapTexture (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeneratedCubeMapTexture, vp);
}

JSBool
X3DConstants::GeoCoordinate (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoCoordinate, vp);
}

JSBool
X3DConstants::GeoElevationGrid (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoElevationGrid, vp);
}

JSBool
X3DConstants::GeoLOD (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoLOD, vp);
}

JSBool
X3DConstants::GeoLocation (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoLocation, vp);
}

JSBool
X3DConstants::GeoMetadata (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoMetadata, vp);
}

JSBool
X3DConstants::GeoOrigin (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoOrigin, vp);
}

JSBool
X3DConstants::GeoPositionInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoPositionInterpolator, vp);
}

JSBool
X3DConstants::GeoProximitySensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoProximitySensor, vp);
}

JSBool
X3DConstants::GeoTouchSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoTouchSensor, vp);
}

JSBool
X3DConstants::GeoTransform (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoTransform, vp);
}

JSBool
X3DConstants::GeoViewpoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GeoViewpoint, vp);
}

JSBool
X3DConstants::Group (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Group, vp);
}

JSBool
X3DConstants::HAnimDisplacer (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::HAnimDisplacer, vp);
}

JSBool
X3DConstants::HAnimHumanoid (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::HAnimHumanoid, vp);
}

JSBool
X3DConstants::HAnimJoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::HAnimJoint, vp);
}

JSBool
X3DConstants::HAnimSegment (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::HAnimSegment, vp);
}

JSBool
X3DConstants::HAnimSite (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::HAnimSite, vp);
}

JSBool
X3DConstants::ImageCubeMapTexture (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ImageCubeMapTexture, vp);
}

JSBool
X3DConstants::ImageTexture (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ImageTexture, vp);
}

JSBool
X3DConstants::ImageTexture3D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ImageTexture3D, vp);
}

JSBool
X3DConstants::IndexedFaceSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IndexedFaceSet, vp);
}

JSBool
X3DConstants::IndexedLineSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IndexedLineSet, vp);
}

JSBool
X3DConstants::IndexedQuadSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IndexedQuadSet, vp);
}

JSBool
X3DConstants::IndexedTriangleFanSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IndexedTriangleFanSet, vp);
}

JSBool
X3DConstants::IndexedTriangleSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IndexedTriangleSet, vp);
}

JSBool
X3DConstants::IndexedTriangleStripSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IndexedTriangleStripSet, vp);
}

JSBool
X3DConstants::Inline (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Inline, vp);
}

JSBool
X3DConstants::IntegerSequencer (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IntegerSequencer, vp);
}

JSBool
X3DConstants::IntegerTrigger (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::IntegerTrigger, vp);
}

JSBool
X3DConstants::KeySensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::KeySensor, vp);
}

JSBool
X3DConstants::LOD (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LOD, vp);
}

JSBool
X3DConstants::Layer (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Layer, vp);
}

JSBool
X3DConstants::LayerSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LayerSet, vp);
}

JSBool
X3DConstants::Layout (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Layout, vp);
}

JSBool
X3DConstants::LayoutGroup (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LayoutGroup, vp);
}

JSBool
X3DConstants::LayoutLayer (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LayoutLayer, vp);
}

JSBool
X3DConstants::LinePickSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LinePickSensor, vp);
}

JSBool
X3DConstants::LineProperties (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LineProperties, vp);
}

JSBool
X3DConstants::LineSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LineSet, vp);
}

JSBool
X3DConstants::LoadSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LoadSensor, vp);
}

JSBool
X3DConstants::LocalFog (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::LocalFog, vp);
}

JSBool
X3DConstants::Material (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Material, vp);
}

JSBool
X3DConstants::Matrix3VertexAttribute (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Matrix3VertexAttribute, vp);
}

JSBool
X3DConstants::Matrix4VertexAttribute (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Matrix4VertexAttribute, vp);
}

JSBool
X3DConstants::MetadataBoolean (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MetadataBoolean, vp);
}

JSBool
X3DConstants::MetadataDouble (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MetadataDouble, vp);
}

JSBool
X3DConstants::MetadataFloat (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MetadataFloat, vp);
}

JSBool
X3DConstants::MetadataInteger (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MetadataInteger, vp);
}

JSBool
X3DConstants::MetadataSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MetadataSet, vp);
}

JSBool
X3DConstants::MetadataString (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MetadataString, vp);
}

JSBool
X3DConstants::MotorJoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MotorJoint, vp);
}

JSBool
X3DConstants::MovieTexture (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MovieTexture, vp);
}

JSBool
X3DConstants::MultiTexture (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MultiTexture, vp);
}

JSBool
X3DConstants::MultiTextureCoordinate (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MultiTextureCoordinate, vp);
}

JSBool
X3DConstants::MultiTextureTransform (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::MultiTextureTransform, vp);
}

JSBool
X3DConstants::NavigationInfo (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NavigationInfo, vp);
}

JSBool
X3DConstants::Normal (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Normal, vp);
}

JSBool
X3DConstants::NormalInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NormalInterpolator, vp);
}

JSBool
X3DConstants::NurbsCurve (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsCurve, vp);
}

JSBool
X3DConstants::NurbsCurve2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsCurve2D, vp);
}

JSBool
X3DConstants::NurbsOrientationInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsOrientationInterpolator, vp);
}

JSBool
X3DConstants::NurbsPatchSurface (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsPatchSurface, vp);
}

JSBool
X3DConstants::NurbsPositionInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsPositionInterpolator, vp);
}

JSBool
X3DConstants::NurbsSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsSet, vp);
}

JSBool
X3DConstants::NurbsSurfaceInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsSurfaceInterpolator, vp);
}

JSBool
X3DConstants::NurbsSweptSurface (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsSweptSurface, vp);
}

JSBool
X3DConstants::NurbsSwungSurface (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsSwungSurface, vp);
}

JSBool
X3DConstants::NurbsTextureCoordinate (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsTextureCoordinate, vp);
}

JSBool
X3DConstants::NurbsTrimmedSurface (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::NurbsTrimmedSurface, vp);
}

JSBool
X3DConstants::OrientationChaser (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::OrientationChaser, vp);
}

JSBool
X3DConstants::OrientationDamper (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::OrientationDamper, vp);
}

JSBool
X3DConstants::OrientationInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::OrientationInterpolator, vp);
}

JSBool
X3DConstants::OrthoViewpoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::OrthoViewpoint, vp);
}

JSBool
X3DConstants::PackagedShader (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PackagedShader, vp);
}

JSBool
X3DConstants::ParticleSystem (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ParticleSystem, vp);
}

JSBool
X3DConstants::PickableGroup (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PickableGroup, vp);
}

JSBool
X3DConstants::PixelTexture (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PixelTexture, vp);
}

JSBool
X3DConstants::PixelTexture3D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PixelTexture3D, vp);
}

JSBool
X3DConstants::PlaneSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PlaneSensor, vp);
}

JSBool
X3DConstants::PointEmitter (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PointEmitter, vp);
}

JSBool
X3DConstants::PointLight (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PointLight, vp);
}

JSBool
X3DConstants::PointPickSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PointPickSensor, vp);
}

JSBool
X3DConstants::PointSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PointSet, vp);
}

JSBool
X3DConstants::Polyline2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Polyline2D, vp);
}

JSBool
X3DConstants::PolylineEmitter (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PolylineEmitter, vp);
}

JSBool
X3DConstants::Polypoint2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Polypoint2D, vp);
}

JSBool
X3DConstants::PositionChaser (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PositionChaser, vp);
}

JSBool
X3DConstants::PositionChaser2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PositionChaser2D, vp);
}

JSBool
X3DConstants::PositionDamper (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PositionDamper, vp);
}

JSBool
X3DConstants::PositionDamper2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PositionDamper2D, vp);
}

JSBool
X3DConstants::PositionInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PositionInterpolator, vp);
}

JSBool
X3DConstants::PositionInterpolator2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PositionInterpolator2D, vp);
}

JSBool
X3DConstants::PrimitivePickSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::PrimitivePickSensor, vp);
}

JSBool
X3DConstants::ProgramShader (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ProgramShader, vp);
}

JSBool
X3DConstants::ProximitySensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ProximitySensor, vp);
}

JSBool
X3DConstants::QuadSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::QuadSet, vp);
}

JSBool
X3DConstants::ReceiverPdu (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ReceiverPdu, vp);
}

JSBool
X3DConstants::Rectangle2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Rectangle2D, vp);
}

JSBool
X3DConstants::RigidBody (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::RigidBody, vp);
}

JSBool
X3DConstants::RigidBodyCollection (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::RigidBodyCollection, vp);
}

JSBool
X3DConstants::ScalarChaser (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ScalarChaser, vp);
}

JSBool
X3DConstants::ScalarDamper (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ScalarDamper, vp);
}

JSBool
X3DConstants::ScalarInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ScalarInterpolator, vp);
}

JSBool
X3DConstants::ScreenFontStyle (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ScreenFontStyle, vp);
}

JSBool
X3DConstants::ScreenGroup (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ScreenGroup, vp);
}

JSBool
X3DConstants::Script (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Script, vp);
}

JSBool
X3DConstants::ShaderPart (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ShaderPart, vp);
}

JSBool
X3DConstants::ShaderProgram (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ShaderProgram, vp);
}

JSBool
X3DConstants::Shape (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Shape, vp);
}

JSBool
X3DConstants::SignalPdu (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SignalPdu, vp);
}

JSBool
X3DConstants::SingleAxisHingeJoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SingleAxisHingeJoint, vp);
}

JSBool
X3DConstants::SliderJoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SliderJoint, vp);
}

JSBool
X3DConstants::Sound (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Sound, vp);
}

JSBool
X3DConstants::Sphere (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Sphere, vp);
}

JSBool
X3DConstants::SphereSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SphereSensor, vp);
}

JSBool
X3DConstants::SplinePositionInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SplinePositionInterpolator, vp);
}

JSBool
X3DConstants::SplinePositionInterpolator2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SplinePositionInterpolator2D, vp);
}

JSBool
X3DConstants::SplineScalarInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SplineScalarInterpolator, vp);
}

JSBool
X3DConstants::SpotLight (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SpotLight, vp);
}

JSBool
X3DConstants::SquadOrientationInterpolator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SquadOrientationInterpolator, vp);
}

JSBool
X3DConstants::StaticGroup (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::StaticGroup, vp);
}

JSBool
X3DConstants::StringSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::StringSensor, vp);
}

JSBool
X3DConstants::SurfaceEmitter (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::SurfaceEmitter, vp);
}

JSBool
X3DConstants::Switch (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Switch, vp);
}

JSBool
X3DConstants::TexCoordChaser2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TexCoordChaser2D, vp);
}

JSBool
X3DConstants::TexCoordDamper2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TexCoordDamper2D, vp);
}

JSBool
X3DConstants::Text (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Text, vp);
}

JSBool
X3DConstants::TextureBackground (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureBackground, vp);
}

JSBool
X3DConstants::TextureCoordinate (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureCoordinate, vp);
}

JSBool
X3DConstants::TextureCoordinate3D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureCoordinate3D, vp);
}

JSBool
X3DConstants::TextureCoordinate4D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureCoordinate4D, vp);
}

JSBool
X3DConstants::TextureCoordinateGenerator (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureCoordinateGenerator, vp);
}

JSBool
X3DConstants::TextureProperties (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureProperties, vp);
}

JSBool
X3DConstants::TextureTransform (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureTransform, vp);
}

JSBool
X3DConstants::TextureTransform3D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureTransform3D, vp);
}

JSBool
X3DConstants::TextureTransformMatrix3D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TextureTransformMatrix3D, vp);
}

JSBool
X3DConstants::TimeSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TimeSensor, vp);
}

JSBool
X3DConstants::TimeTrigger (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TimeTrigger, vp);
}

JSBool
X3DConstants::TouchGroup (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TouchGroup, vp);
}

JSBool
X3DConstants::TouchSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TouchSensor, vp);
}

JSBool
X3DConstants::Transform (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Transform, vp);
}

JSBool
X3DConstants::TransformMatrix3D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TransformMatrix3D, vp);
}

JSBool
X3DConstants::TransformSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TransformSensor, vp);
}

JSBool
X3DConstants::TransmitterPdu (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TransmitterPdu, vp);
}

JSBool
X3DConstants::TriangleFanSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TriangleFanSet, vp);
}

JSBool
X3DConstants::TriangleSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TriangleSet, vp);
}

JSBool
X3DConstants::TriangleSet2D (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TriangleSet2D, vp);
}

JSBool
X3DConstants::TriangleStripSet (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TriangleStripSet, vp);
}

JSBool
X3DConstants::TwoSidedMaterial (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::TwoSidedMaterial, vp);
}

JSBool
X3DConstants::UniversalJoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::UniversalJoint, vp);
}

JSBool
X3DConstants::Viewpoint (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Viewpoint, vp);
}

JSBool
X3DConstants::ViewpointGroup (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::ViewpointGroup, vp);
}

JSBool
X3DConstants::Viewport (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::Viewport, vp);
}

JSBool
X3DConstants::VisibilitySensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::VisibilitySensor, vp);
}

JSBool
X3DConstants::VolumeEmitter (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::VolumeEmitter, vp);
}

JSBool
X3DConstants::VolumePickSensor (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::VolumePickSensor, vp);
}

JSBool
X3DConstants::WindPhysicsModel (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::WindPhysicsModel, vp);
}

JSBool
X3DConstants::WorldInfo (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::WorldInfo, vp);
}

// Abstract node types

JSBool
X3DConstants::X3DAppearanceChildNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DAppearanceChildNode, vp);
}

JSBool
X3DConstants::X3DAppearanceNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DAppearanceNode, vp);
}

JSBool
X3DConstants::X3DBackgroundNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DBackgroundNode, vp);
}

JSBool
X3DConstants::X3DBindableNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DBindableNode, vp);
}

JSBool
X3DConstants::X3DBoundedObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DBoundedObject, vp);
}

JSBool
X3DConstants::X3DChaserNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DChaserNode, vp);
}

JSBool
X3DConstants::X3DChildNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DChildNode, vp);
}

JSBool
X3DConstants::X3DColorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DColorNode, vp);
}

JSBool
X3DConstants::X3DComposedGeometryNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DComposedGeometryNode, vp);
}

JSBool
X3DConstants::X3DCoordinateNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DCoordinateNode, vp);
}

JSBool
X3DConstants::X3DDamperNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DDamperNode, vp);
}

JSBool
X3DConstants::X3DDragSensorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DDragSensorNode, vp);
}

JSBool
X3DConstants::X3DEnvironmentTextureNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DEnvironmentTextureNode, vp);
}

JSBool
X3DConstants::X3DEnvironmentalSensorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DEnvironmentalSensorNode, vp);
}

JSBool
X3DConstants::X3DFogObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DFogObject, vp);
}

JSBool
X3DConstants::X3DFollowerNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DFollowerNode, vp);
}

JSBool
X3DConstants::X3DFontStyleNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DFontStyleNode, vp);
}

JSBool
X3DConstants::X3DGeometricPropertyNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DGeometricPropertyNode, vp);
}

JSBool
X3DConstants::X3DGeometryNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DGeometryNode, vp);
}

JSBool
X3DConstants::X3DGeospatialObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DGeospatialObject, vp);
}

JSBool
X3DConstants::X3DGroupingNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DGroupingNode, vp);
}

JSBool
X3DConstants::X3DInfoNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DInfoNode, vp);
}

JSBool
X3DConstants::X3DInterpolatorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DInterpolatorNode, vp);
}

JSBool
X3DConstants::X3DKeyDeviceSensorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DKeyDeviceSensorNode, vp);
}

JSBool
X3DConstants::X3DLayerNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DLayerNode, vp);
}

JSBool
X3DConstants::X3DLayoutNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DLayoutNode, vp);
}

JSBool
X3DConstants::X3DLightNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DLightNode, vp);
}

JSBool
X3DConstants::X3DMaterialNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DMaterialNode, vp);
}

JSBool
X3DConstants::X3DMetadataObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DMetadataObject, vp);
}

JSBool
X3DConstants::X3DNBodyCollidableNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DNBodyCollidableNode, vp);
}

JSBool
X3DConstants::X3DNBodyCollisionSpaceNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DNBodyCollisionSpaceNode, vp);
}

JSBool
X3DConstants::X3DNetworkSensorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DNetworkSensorNode, vp);
}

JSBool
X3DConstants::X3DNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DNode, vp);
}

JSBool
X3DConstants::X3DNormalNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DNormalNode, vp);
}

JSBool
X3DConstants::X3DNurbsControlCurveNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DNurbsControlCurveNode, vp);
}

JSBool
X3DConstants::X3DNurbsSurfaceGeometryNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DNurbsSurfaceGeometryNode, vp);
}

JSBool
X3DConstants::X3DParametricGeometryNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DParametricGeometryNode, vp);
}

JSBool
X3DConstants::X3DParticleEmitterNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DParticleEmitterNode, vp);
}

JSBool
X3DConstants::X3DParticlePhysicsModelNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DParticlePhysicsModelNode, vp);
}

JSBool
X3DConstants::X3DPickSensorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DPickSensorNode, vp);
}

JSBool
X3DConstants::X3DPickableObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DPickableObject, vp);
}

JSBool
X3DConstants::X3DPointingDeviceSensorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DPointingDeviceSensorNode, vp);
}

JSBool
X3DConstants::X3DProductStructureChildNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DProductStructureChildNode, vp);
}

JSBool
X3DConstants::X3DProgrammableShaderObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DProgrammableShaderObject, vp);
}

JSBool
X3DConstants::X3DPrototypeInstance (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DPrototypeInstance, vp);
}

JSBool
X3DConstants::X3DRigidJointNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DRigidJointNode, vp);
}

JSBool
X3DConstants::X3DScriptNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DScriptNode, vp);
}

JSBool
X3DConstants::X3DSensorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DSensorNode, vp);
}

JSBool
X3DConstants::X3DSequencerNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DSequencerNode, vp);
}

JSBool
X3DConstants::X3DShaderNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DShaderNode, vp);
}

JSBool
X3DConstants::X3DShapeNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DShapeNode, vp);
}

JSBool
X3DConstants::X3DSoundNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DSoundNode, vp);
}

JSBool
X3DConstants::X3DSoundSourceNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DSoundSourceNode, vp);
}

JSBool
X3DConstants::X3DTexture2DNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTexture2DNode, vp);
}

JSBool
X3DConstants::X3DTexture3DNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTexture3DNode, vp);
}

JSBool
X3DConstants::X3DTextureCoordinateNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTextureCoordinateNode, vp);
}

JSBool
X3DConstants::X3DTextureNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTextureNode, vp);
}

JSBool
X3DConstants::X3DTextureTransformNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTextureTransformNode, vp);
}

JSBool
X3DConstants::X3DTimeDependentNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTimeDependentNode, vp);
}

JSBool
X3DConstants::X3DTouchSensorNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTouchSensorNode, vp);
}

JSBool
X3DConstants::X3DTransformMatrix3DNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTransformMatrix3DNode, vp);
}

JSBool
X3DConstants::X3DTransformMatrix3DObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTransformMatrix3DObject, vp);
}

JSBool
X3DConstants::X3DTransformNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTransformNode, vp);
}

JSBool
X3DConstants::X3DTriggerNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DTriggerNode, vp);
}

JSBool
X3DConstants::X3DUrlObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DUrlObject, vp);
}

JSBool
X3DConstants::X3DVertexAttributeNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DVertexAttributeNode, vp);
}

JSBool
X3DConstants::X3DViewpointNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DViewpointNode, vp);
}

JSBool
X3DConstants::X3DViewpointObject (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DViewpointObject, vp);
}

JSBool
X3DConstants::X3DViewportNode (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DViewportNode, vp);
}

// Tools

JSBool
X3DConstants::AngleGridTool (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::AngleGridTool, vp);
}

JSBool
X3DConstants::GridTool (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::GridTool, vp);
}

JSBool
X3DConstants::X3DGridTool (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	return JS_NewNumberValue (cx, X3D::X3DConstants::X3DGridTool, vp);
}

} // spidermonkey
} // X3D
} // titania
