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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DCONSTANTS_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DCONSTANTS_H__

#include <iostream>
#include <jsapi.h>

namespace titania {
namespace X3D {

class jsX3DConstants
{
public:

	static
	void
	init (JSContext* const, JSObject* const);

	static
	JSClass*
	getClass () { return &static_class; }


private:

	enum Property
	{
		// Event types

		JS_INITIALIZED_EVENT,
		JS_SHUTDOWN_EVENT,

		// Error types

		JS_CONNECTION_ERROR,
		JS_INITIALIZED_ERROR,

		// Load state

		JS_NOT_STARTED_STATE,
		JS_IN_PROGRESS_STATE,
		JS_COMPLETE_STATE,
		JS_FAILED_STATE,

		// Access types

		JS_INITIALIZE_ONLY,
		JS_INPUT_ONLY,
		JS_OUTPUT_ONLY,
		JS_INPUT_OUTPUT,

		// Field types

		JS_SFBOOL,
		JS_SFCOLOR,
		JS_SFCOLORRGBA,
		JS_SFDOUBLE,
		JS_SFFLOAT,
		JS_SFINT32,
		JS_SFIMAGE,
		JS_SFMATRIX3D,
		JS_SFMATRIX3F,
		JS_SFMATRIX4D,
		JS_SFMATRIX4F,
		JS_SFNODE,
		JS_SFROTATION,
		JS_SFSTRING,
		JS_SFTIME,
		JS_SFVEC2D,
		JS_SFVEC2F,
		JS_SFVEC3D,
		JS_SFVEC3F,
		JS_SFVEC4D,
		JS_SFVEC4F,

		JS_MFBOOL,
		JS_MFCOLOR,
		JS_MFCOLORRGBA,
		JS_MFDOUBLE,
		JS_MFFLOAT,
		JS_MFIMAGE,
		JS_MFINT32,
		JS_MFMATRIX3D,
		JS_MFMATRIX3F,
		JS_MFMATRIX4D,
		JS_MFMATRIX4F,
		JS_MFNODE,
		JS_MFROTATION,
		JS_MFSTRING,
		JS_MFTIME,
		JS_MFVEC2D,
		JS_MFVEC2F,
		JS_MFVEC3D,
		JS_MFVEC3F,
		JS_MFVEC4D,
		JS_MFVEC4F,

		// Base node type

		JS_X3D_BASE_NODE,

		// Basic node types

		JS_ANCHOR,
		JS_APPEARANCE,
		JS_ARC2D,
		JS_ARC_CLOSE2D,
		JS_AUDIO_CLIP,
		JS_BACKGROUND,
		JS_BALL_JOINT,
		JS_BILLBOARD,
		JS_BOOLEAN_FILTER,
		JS_BOOLEAN_SEQUENCER,
		JS_BOOLEAN_TOGGLE,
		JS_BOOLEAN_TRIGGER,
		JS_BOUNDED_PHYSICS_MODEL,
		JS_BOX,
		JS_CAD_ASSEMBLY,
		JS_CAD_FACE,
		JS_CAD_LAYER,
		JS_CAD_PART,
		JS_CIRCLE2D,
		JS_CLIP_PLANE,
		JS_COLLIDABLE_OFFSET,
		JS_COLLIDABLE_SHAPE,
		JS_COLLISION,
		JS_COLLISION_COLLECTION,
		JS_COLLISION_SENSOR,
		JS_COLLISION_SPACE,
		JS_COLOR,
		JS_COLOR_CHASER,
		JS_COLOR_DAMPER,
		JS_COLOR_INTERPOLATOR,
		JS_COLORRGBA,
		JS_COMPOSED_CUBE_MAP_TEXTURE,
		JS_COMPOSED_SHADER,
		JS_COMPOSED_TEXTURE3D,
		JS_CONE,
		JS_CONE_EMITTER,
		JS_CONTACT,
		JS_CONTOUR2D,
		JS_CONTOUR_POLYLINE2D,
		JS_COORDINATE,
		JS_COORDINATE_CHASER,
		JS_COORDINATE_DAMPER,
		JS_COORDINATE_DOUBLE,
		JS_COORDINATE_INTERPOLATOR,
		JS_COORDINATE_INTERPOLATOR2D,
		JS_CYLINDER,
		JS_CYLINDER_SENSOR,
		JS_DIS_ENTITY_MANAGER,
		JS_DIS_ENTITY_TYPE_MAPPING,
		JS_DIRECTIONAL_LIGHT,
		JS_DISK2D,
		JS_DOUBLE_AXIS_HINGE_JOINT,
		JS_EASE_IN_EASE_OUT,
		JS_ELEVATION_GRID,
		JS_ESPDU_TRANSFORM,
		JS_EXPLOSION_EMITTER,
		JS_EXTRUSION,
		JS_FILL_PROPERTIES,
		JS_FLOAT_VERTEX_ATTRIBUTE,
		JS_FOG,
		JS_FOG_COORDINATE,
		JS_FONT_STYLE,
		JS_FORCE_PHYSICS_MODEL,
		JS_GENERATED_CUBE_MAP_TEXTURE,
		JS_GEO_COORDINATE,
		JS_GEO_ELEVATION_GRID,
		JS_GEOLOD,
		JS_GEO_LOCATION,
		JS_GEO_METADATA,
		JS_GEO_ORIGIN,
		JS_GEO_POSITION_INTERPOLATOR,
		JS_GEO_PROXIMITY_SENSOR,
		JS_GEO_TOUCH_SENSOR,
		JS_GEO_TRANSFORM,
		JS_GEO_VIEWPOINT,
		JS_GROUP,
		JS_H_ANIM_DISPLACER,
		JS_H_ANIM_HUMANOID,
		JS_H_ANIM_JOINT,
		JS_H_ANIM_SEGMENT,
		JS_H_ANIM_SITE,
		JS_IMAGE_CUBE_MAP_TEXTURE,
		JS_IMAGE_TEXTURE,
		JS_IMAGE_TEXTURE3D,
		JS_INDEXED_FACE_SET,
		JS_INDEXED_LINE_SET,
		JS_INDEXED_QUAD_SET,
		JS_INDEXED_TRIANGLE_FAN_SET,
		JS_INDEXED_TRIANGLE_SET,
		JS_INDEXED_TRIANGLE_STRIP_SET,
		JS_INLINE_NODE,
		JS_INTEGER_SEQUENCER,
		JS_INTEGER_TRIGGER,
		JS_KEY_SENSOR,
		JS_LOD,
		JS_LAYER,
		JS_LAYER_SET,
		JS_LAYOUT,
		JS_LAYOUT_GROUP,
		JS_LAYOUT_LAYER,
		JS_LINE_PICK_SENSOR,
		JS_LINE_PROPERTIES,
		JS_LINE_SET,
		JS_LOAD_SENSOR,
		JS_LOCAL_FOG,
		JS_MATERIAL,
		JS_MATRIX3_VERTEX_ATTRIBUTE,
		JS_MATRIX4_VERTEX_ATTRIBUTE,
		JS_METADATA_BOOLEAN,
		JS_METADATA_DOUBLE,
		JS_METADATA_FLOAT,
		JS_METADATA_INTEGER,
		JS_METADATA_SET,
		JS_METADATA_STRING,
		JS_MOTOR_JOINT,
		JS_MOVIE_TEXTURE,
		JS_MULTI_TEXTURE,
		JS_MULTI_TEXTURE_COORDINATE,
		JS_MULTI_TEXTURE_TRANSFORM,
		JS_NAVIGATION_INFO,
		JS_NORMAL,
		JS_NORMAL_INTERPOLATOR,
		JS_NURBS_CURVE,
		JS_NURBS_CURVE2D,
		JS_NURBS_ORIENTATION_INTERPOLATOR,
		JS_NURBS_PATCH_SURFACE,
		JS_NURBS_POSITION_INTERPOLATOR,
		JS_NURBS_SET,
		JS_NURBS_SURFACE_INTERPOLATOR,
		JS_NURBS_SWEPT_SURFACE,
		JS_NURBS_SWUNG_SURFACE,
		JS_NURBS_TEXTURE_COORDINATE,
		JS_NURBS_TRIMMED_SURFACE,
		JS_ORIENTATION_CHASER,
		JS_ORIENTATION_DAMPER,
		JS_ORIENTATION_INTERPOLATOR,
		JS_ORTHO_VIEWPOINT,
		JS_PACKAGED_SHADER,
		JS_PARTICLE_SYSTEM,
		JS_PICKABLE_GROUP,
		JS_PIXEL_TEXTURE,
		JS_PIXEL_TEXTURE3D,
		JS_PLANE_SENSOR,
		JS_POINT_EMITTER,
		JS_POINT_LIGHT,
		JS_POINT_PICK_SENSOR,
		JS_POINT_SET,
		JS_POLYLINE2D,
		JS_POLYLINE_EMITTER,
		JS_POLYPOINT2D,
		JS_POSITION_CHASER,
		JS_POSITION_CHASER2D,
		JS_POSITION_DAMPER,
		JS_POSITION_DAMPER2D,
		JS_POSITION_INTERPOLATOR,
		JS_POSITION_INTERPOLATOR2D,
		JS_PRIMITIVE_PICK_SENSOR,
		JS_PROGRAM_SHADER,
		JS_PROXIMITY_SENSOR,
		JS_QUAD_SET,
		JS_RECEIVER_PDU,
		JS_RECTANGLE2D,
		JS_RIGID_BODY,
		JS_RIGID_BODY_COLLECTION,
		JS_SCALAR_CHASER,
		JS_SCALAR_DAMPER,
		JS_SCALAR_INTERPOLATOR,
		JS_SCREEN_FONT_STYLE,
		JS_SCREEN_GROUP,
		JS_SCRIPT,
		JS_SHADER_PART,
		JS_SHADER_PROGRAM,
		JS_SHAPE,
		JS_SIGNAL_PDU,
		JS_SINGLE_AXIS_HINGE_JOINT,
		JS_SLIDER_JOINT,
		JS_SOUND,
		JS_SPHERE,
		JS_SPHERE_SENSOR,
		JS_SPLINE_POSITION_INTERPOLATOR,
		JS_SPLINE_POSITION_INTERPOLATOR2D,
		JS_SPLINE_SCALAR_INTERPOLATOR,
		JS_SPOT_LIGHT,
		JS_SQUAD_ORIENTATION_INTERPOLATOR,
		JS_STATIC_GROUP,
		JS_STRING_SENSOR,
		JS_SURFACE_EMITTER,
		JS_SWITCH,
		JS_TEX_COORD_CHASER2D,
		JS_TEX_COORD_DAMPER2D,
		JS_TEXT,
		JS_TEXTURE_BACKGROUND,
		JS_TEXTURE_COORDINATE,
		JS_TEXTURE_COORDINATE3D,
		JS_TEXTURE_COORDINATE4D,
		JS_TEXTURE_COORDINATE_GENERATOR,
		JS_TEXTURE_PROPERTIES,
		JS_TEXTURE_TRANSFORM,
		JS_TEXTURE_TRANSFORM3D,
		JS_TEXTURE_TRANSFORM_MATRIX3D,
		JS_TIME_SENSOR,
		JS_TIME_TRIGGER,
		JS_TOUCH_SENSOR,
		JS_TRANSFORM,
		JS_TRANSFORM_SENSOR,
		JS_TRANSMITTER_PDU,
		JS_TRIANGLE_FAN_SET,
		JS_TRIANGLE_SET,
		JS_TRIANGLE_SET2D,
		JS_TRIANGLE_STRIP_SET,
		JS_TWO_SIDED_MATERIAL,
		JS_UNIVERSAL_JOINT,
		JS_VIEWPOINT,
		JS_VIEWPOINT_GROUP,
		JS_VIEWPORT,
		JS_VISIBILITY_SENSOR,
		JS_VOLUME_EMITTER,
		JS_VOLUME_PICK_SENSOR,
		JS_WIND_PHYSICS_MODEL,
		JS_WORLD_INFO,

		// Abstract node types

		JS_X3D_APPEARANCE_CHILD_NODE,
		JS_X3D_APPEARANCE_NODE,
		JS_X3D_BACKGROUND_NODE,
		JS_X3D_BINDABLE_NODE,
		JS_X3D_BOUNDED_OBJECT,
		JS_X3D_CHASER_NODE,
		JS_X3D_CHILD_NODE,
		JS_X3D_COLOR_NODE,
		JS_X3D_COMPOSED_GEOMETRY_NODE,
		JS_X3D_COORDINATE_NODE,
		JS_X3D_DAMPER_NODE,
		JS_X3D_DRAG_SENSOR_NODE,
		JS_X3D_ENVIRONMENT_TEXTURE_NODE,
		JS_X3D_ENVIRONMENTAL_SENSOR_NODE,
		JS_X3D_FOG_OBJECT,
		JS_X3D_FOLLOWER_NODE,
		JS_X3D_FONT_STYLE_NODE,
		JS_X3D_GEOMETRIC_PROPERTY_NODE,
		JS_X3D_GEOMETRY_NODE,
		JS_X3D_GEOSPATIAL_OBJECT,
		JS_X3D_GROUPING_NODE,
		JS_X3D_INFO_NODE,
		JS_X3D_INTERPOLATOR_NODE,
		JS_X3D_KEY_DEVICE_SENSOR_NODE,
		JS_X3D_LAYER_NODE,
		JS_X3D_LAYOUT_NODE,
		JS_X3D_LIGHT_NODE,
		JS_X3D_MATERIAL_NODE,
		JS_X3D_METADATA_OBJECT,
		JS_X3DN_BODY_COLLIDABLE_NODE,
		JS_X3DN_BODY_COLLISION_SPACE_NODE,
		JS_X3D_NETWORK_SENSOR_NODE,
		JS_X3D_NODE,
		JS_X3D_NORMAL_NODE,
		JS_X3D_NURBS_CONTROL_CURVE_NODE,
		JS_X3D_NURBS_SURFACE_GEOMETRY_NODE,
		JS_X3D_PARAMETRIC_GEOMETRY_NODE,
		JS_X3D_PARTICLE_EMITTER_NODE,
		JS_X3D_PARTICLE_PHYSICS_MODEL_NODE,
		JS_X3D_PICK_SENSOR_NODE,
		JS_X3D_PICKABLE_OBJECT,
		JS_X3D_POINTING_DEVICE_SENSOR_NODE,
		JS_X3D_PRODUCT_STRUCTURE_CHILD_NODE,
		JS_X3D_PROGRAMMABLE_SHADER_OBJECT,
		JS_X3D_PROTOTYPE_INSTANCE,
		JS_X3D_RIGID_JOINT_NODE,
		JS_X3D_SCRIPT_NODE,
		JS_X3D_SENSOR_NODE,
		JS_X3D_SEQUENCER_NODE,
		JS_X3D_SHADER_NODE,
		JS_X3D_SHAPE_NODE,
		JS_X3D_SOUND_NODE,
		JS_X3D_SOUND_SOURCE_NODE,
		JS_X3D_TEXTURE2D_NODE,
		JS_X3D_TEXTURE3D_NODE,
		JS_X3D_TEXTURE_COORDINATE_NODE,
		JS_X3D_TEXTURE_NODE,
		JS_X3D_TEXTURE_TRANSFORM_NODE,
		JS_X3D_TIME_DEPENDENT_NODE,
		JS_X3D_TOUCH_SENSOR_NODE,
		JS_X3D_TRANSFORM_MATRIX4D_NODE,
		JS_X3D_TRANSFORM_NODE,
		JS_X3D_TRIGGER_NODE,
		JS_X3D_URL_OBJECT,
		JS_X3D_VERTEX_ATTRIBUTE_NODE,
		JS_X3D_VIEWPOINT_NODE,
		JS_X3D_VIEWPOINT_OBJECT,
		JS_X3D_VIEWPORT_NODE
	};

	// Event types

	static JSBool INITIALIZED_EVENT (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SHUTDOWN_EVENT    (JSContext *, JSObject *, jsid, jsval*);

	// Error types

	static JSBool CONNECTION_ERROR  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool INITIALIZED_ERROR (JSContext *, JSObject *, jsid, jsval*);

	// Load state

	static JSBool NOT_STARTED_STATE (JSContext *, JSObject *, jsid, jsval*);
	static JSBool IN_PROGRESS_STATE (JSContext *, JSObject *, jsid, jsval*);
	static JSBool COMPLETE_STATE    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool FAILED_STATE      (JSContext *, JSObject *, jsid, jsval*);

	// Access types

	static JSBool initializeOnly (JSContext *, JSObject *, jsid, jsval*);
	static JSBool inputOnly      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool outputOnly     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool inputOutput    (JSContext *, JSObject *, jsid, jsval*);

	// Field types

	static JSBool SFBool      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFColor     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFColorRGBA (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFDouble    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFFloat     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFInt32     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFImage     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFMatrix3d  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFMatrix3f  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFMatrix4d  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFMatrix4f  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFNode      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFRotation  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFString    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFTime      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec2d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec2f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec3d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec3f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec4d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool SFVec4f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFBool      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFColor     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFColorRGBA (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFDouble    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFFloat     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFImage     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFInt32     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFMatrix3d  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFMatrix3f  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFMatrix4d  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFMatrix4f  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFNode      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFRotation  (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFString    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFTime      (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec2d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec2f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec3d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec3f     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec4d     (JSContext *, JSObject *, jsid, jsval*);
	static JSBool MFVec4f     (JSContext *, JSObject *, jsid, jsval*);

	// Base node type

	static JSBool X3DBaseNode (JSContext*, JSObject*, jsid, jsval*);

	// Basic node types

	static JSBool Anchor                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Appearance                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Arc2D                        (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ArcClose2D                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool AudioClip                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Background                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool BallJoint                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Billboard                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool BooleanFilter                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool BooleanSequencer             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool BooleanToggle                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool BooleanTrigger               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool BoundedPhysicsModel          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Box                          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CADAssembly                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CADFace                      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CADLayer                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CADPart                      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Circle2D                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ClipPlane                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CollidableOffset             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CollidableShape              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Collision                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CollisionCollection          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CollisionSensor              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CollisionSpace               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Color                        (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ColorChaser                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ColorDamper                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ColorInterpolator            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ColorRGBA                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ComposedCubeMapTexture       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ComposedShader               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ComposedTexture3D            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Cone                         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ConeEmitter                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Contact                      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Contour2D                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ContourPolyline2D            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Coordinate                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CoordinateChaser             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CoordinateDamper             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CoordinateDouble             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CoordinateInterpolator       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CoordinateInterpolator2D     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Cylinder                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool CylinderSensor               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool DISEntityManager             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool DISEntityTypeMapping         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool DirectionalLight             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Disk2D                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool DoubleAxisHingeJoint         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool EaseInEaseOut                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ElevationGrid                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool EspduTransform               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ExplosionEmitter             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Extrusion                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool FillProperties               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool FloatVertexAttribute         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Fog                          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool FogCoordinate                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool FontStyle                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ForcePhysicsModel            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeneratedCubeMapTexture      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoCoordinate                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoElevationGrid             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoLOD                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoLocation                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoMetadata                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoOrigin                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoPositionInterpolator      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoProximitySensor           (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoTouchSensor               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoTransform                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool GeoViewpoint                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Group                        (JSContext*, JSObject*, jsid, jsval*);
	static JSBool HAnimDisplacer               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool HAnimHumanoid                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool HAnimJoint                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool HAnimSegment                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool HAnimSite                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ImageCubeMapTexture          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ImageTexture                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ImageTexture3D               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool IndexedFaceSet               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool IndexedLineSet               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool IndexedQuadSet               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool IndexedTriangleFanSet        (JSContext*, JSObject*, jsid, jsval*);
	static JSBool IndexedTriangleSet           (JSContext*, JSObject*, jsid, jsval*);
	static JSBool IndexedTriangleStripSet      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Inline                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool IntegerSequencer             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool IntegerTrigger               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool KeySensor                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LOD                          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Layer                        (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LayerSet                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Layout                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LayoutGroup                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LayoutLayer                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LinePickSensor               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LineProperties               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LineSet                      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LoadSensor                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool LocalFog                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Material                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Matrix3VertexAttribute       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Matrix4VertexAttribute       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MetadataBoolean              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MetadataDouble               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MetadataFloat                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MetadataInteger              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MetadataSet                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MetadataString               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MotorJoint                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MovieTexture                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MultiTexture                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MultiTextureCoordinate       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool MultiTextureTransform        (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NavigationInfo               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Normal                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NormalInterpolator           (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsCurve                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsCurve2D                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsOrientationInterpolator (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsPatchSurface            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsPositionInterpolator    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsSet                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsSurfaceInterpolator     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsSweptSurface            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsSwungSurface            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsTextureCoordinate       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool NurbsTrimmedSurface          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool OrientationChaser            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool OrientationDamper            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool OrientationInterpolator      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool OrthoViewpoint               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PackagedShader               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ParticleSystem               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PickableGroup                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PixelTexture                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PixelTexture3D               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PlaneSensor                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PointEmitter                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PointLight                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PointPickSensor              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PointSet                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Polyline2D                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PolylineEmitter              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Polypoint2D                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PositionChaser               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PositionChaser2D             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PositionDamper               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PositionDamper2D             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PositionInterpolator         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PositionInterpolator2D       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool PrimitivePickSensor          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ProgramShader                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ProximitySensor              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool QuadSet                      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ReceiverPdu                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Rectangle2D                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool RigidBody                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool RigidBodyCollection          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ScalarChaser                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ScalarDamper                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ScalarInterpolator           (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ScreenFontStyle              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ScreenGroup                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Script                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ShaderPart                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ShaderProgram                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Shape                        (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SignalPdu                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SingleAxisHingeJoint         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SliderJoint                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Sound                        (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Sphere                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SphereSensor                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SplinePositionInterpolator   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SplinePositionInterpolator2D (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SplineScalarInterpolator     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SpotLight                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SquadOrientationInterpolator (JSContext*, JSObject*, jsid, jsval*);
	static JSBool StaticGroup                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool StringSensor                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool SurfaceEmitter               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Switch                       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TexCoordChaser2D             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TexCoordDamper2D             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Text                         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureBackground            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureCoordinate            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureCoordinate3D          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureCoordinate4D          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureCoordinateGenerator   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureProperties            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureTransform             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureTransform3D           (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TextureTransformMatrix3D     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TimeSensor                   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TimeTrigger                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TouchSensor                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Transform                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TransformSensor              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TransmitterPdu               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TriangleFanSet               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TriangleSet                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TriangleSet2D                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TriangleStripSet             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool TwoSidedMaterial             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool UniversalJoint               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Viewpoint                    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool ViewpointGroup               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool Viewport                     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool VisibilitySensor             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool VolumeEmitter                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool VolumePickSensor             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool WindPhysicsModel             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool WorldInfo                    (JSContext*, JSObject*, jsid, jsval*);

	// Abstract node types

	static JSBool X3DAppearanceChildNode       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DAppearanceNode            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DBackgroundNode            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DBindableNode              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DBoundedObject             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DChaserNode                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DChildNode                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DColorNode                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DComposedGeometryNode      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DCoordinateNode            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DDamperNode                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DDragSensorNode            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DEnvironmentTextureNode    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DEnvironmentalSensorNode   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DFogObject                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DFollowerNode              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DFontStyleNode             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DGeometricPropertyNode     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DGeometryNode              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DGeospatialObject          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DGroupingNode              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DInfoNode                  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DInterpolatorNode          (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DKeyDeviceSensorNode       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DLayerNode                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DLayoutNode                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DLightNode                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DMaterialNode              (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DMetadataObject            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DNBodyCollidableNode       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DNBodyCollisionSpaceNode   (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DNetworkSensorNode         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DNode                      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DNormalNode                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DNurbsControlCurveNode     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DNurbsSurfaceGeometryNode  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DParametricGeometryNode    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DParticleEmitterNode       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DParticlePhysicsModelNode  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DPickSensorNode            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DPickableObject            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DPointingDeviceSensorNode  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DProductStructureChildNode (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DProgrammableShaderObject  (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DPrototypeInstance         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DRigidJointNode            (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DScriptNode                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DSensorNode                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DSequencerNode             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DShaderNode                (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DShapeNode                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DSoundNode                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DSoundSourceNode           (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTexture2DNode             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTexture3DNode             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTextureCoordinateNode     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTextureNode               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTextureTransformNode      (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTimeDependentNode         (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTouchSensorNode           (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTransformMatrix4DNode     (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTransformNode             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DTriggerNode               (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DUrlObject                 (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DVertexAttributeNode       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DViewpointNode             (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DViewpointObject           (JSContext*, JSObject*, jsid, jsval*);
	static JSBool X3DViewportNode              (JSContext*, JSObject*, jsid, jsval*);

	///  @name Static Members

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

} // X3D
} // titania

#endif
