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

#include "History.h"

#include "../../Configuration/config.h"

#include <Titania/String.h>

#include <giomm.h>
#include <iostream>

namespace titania {
namespace puck {

History::History () :
	X3D::X3DOutput (),
	      filename (config_dir ("history.db")),
	      database (),
	   fileMonitor ()
{
	try
	{
		const bool have_history = Glib::file_test (filename, Glib::FILE_TEST_EXISTS);
		const auto configdir    = Gio::File::create_for_path (config_dir ());

		if (not configdir -> query_exists ())
			configdir -> make_directory_with_parents ();

		database .open (filename);

		database .query ("CREATE TABLE IF NOT EXISTS History ("
		                 "id           INTEGER,"
		                 "title        TEXT,"
		                 "worldURL     TEXT,"
		                 "visited      INTEGER DEFAULT 1,"
		                 "lastAccess   REAL    DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now')),"
		                 "creationTime REAL    DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now')),"
		                 "PRIMARY KEY (id ASC))");

		database .try_query ("ALTER TABLE History ADD preview BLOB DEFAULT NULL");
		database .try_query ("ALTER TABLE History ADD contextPath TEXT");
		database .try_query ("ALTER TABLE History DROP icon"); // sqlite cannot drop columns

		if (not have_history)
		{
			setItem ("BlendMode",                    "https://create3000.github.io/media/examples/X_ITE/BlendMode/BlendMode.x3d");
			setItem ("WaterQuality",                 "https://create3000.github.io/media/examples/X3D/WaterQuality/WaterQuality.x3d");
			setItem ("Vattenfall",                   "https://create3000.github.io/media/examples/X3D/Vattenfall/Vattenfall.x3d");
			setItem ("TreasureIsland",               "https://create3000.github.io/media/examples/X3D/TreasureIsland/TreasureIsland.x3d");
			setItem ("TikiWorld",                    "https://create3000.github.io/media/examples/X3D/TikiWorld/TikiWorld.x3d");
			setItem ("SugarSmack",                   "https://create3000.github.io/media/examples/X3D/SugarSmack/SugarSmack.x3d");
			setItem ("SmashingBoxes",                "https://create3000.github.io/media/examples/X3D/SmashingBoxes/SmashingBoxes.x3d");
			setItem ("SmartyBubbles",                "https://create3000.github.io/media/examples/X3D/SmartyBubbles/SmartyBubbles.x3d");
			setItem ("SlidingPuzzle",                "https://create3000.github.io/media/examples/X3D/SlidingPuzzle/SlidingPuzzle.x3d");
			setItem ("SecretLabyrinth",              "https://create3000.github.io/media/examples/X3D/SecretLabyrinth/SecretLabyrinth.x3d");
			setItem ("Pong",                         "https://create3000.github.io/media/examples/X3D/Pong/Pong.x3d");
			setItem ("NYC",                          "https://create3000.github.io/media/examples/X3D/NYC/NYC.x3d");
			setItem ("MilkywayAndBeyond",            "https://create3000.github.io/media/examples/X3D/MilkywayAndBeyond/MilkywayAndBeyond.x3d");
			setItem ("MagicMushrooms",               "https://create3000.github.io/media/examples/X3D/MagicMushrooms/MagicMushrooms.x3d");
			setItem ("LustForLife",                  "https://create3000.github.io/media/examples/X3D/LustForLife/LustForLife.x3d");
			setItem ("LogoPieces",                   "https://create3000.github.io/media/examples/X3D/LogoPieces/LogoPieces.x3d");
			setItem ("FoldUp",                       "https://create3000.github.io/media/examples/X3D/FoldUp/FoldUp.x3d");
			setItem ("FlashingLights",               "https://create3000.github.io/media/examples/X3D/FlashingLights/FlashingLights.x3d");
			setItem ("Currencies",                   "https://create3000.github.io/media/examples/X3D/Currencies/Currencies.x3d");
			setItem ("CrazySpiral",                  "https://create3000.github.io/media/examples/X3D/CrazySpiral/CrazySpiral.x3d");
			setItem ("Circles",                      "https://create3000.github.io/media/examples/X3D/Circles/Circles.x3d");
			setItem ("Chomp",                        "https://create3000.github.io/media/examples/X3D/Chomp/Chomp.x3d");
			setItem ("BeyondGermany",                "https://create3000.github.io/media/examples/X3D/BeyondGermany/BeyondGermany.x3d");
			setItem ("BIC",                          "https://create3000.github.io/media/examples/X3D/BIC/BIC.x3d");
			setItem ("Astronomy",                    "https://create3000.github.io/media/examples/X3D/Astronomy/Astronomy.x3d");
			setItem ("Arcadia",                      "https://create3000.github.io/media/examples/X3D/Arcadia/Arcadia.x3d");
			setItem ("Approach",                     "https://create3000.github.io/media/examples/X3D/Approach/Approach.x3d");
			setItem ("Appartment",                   "https://create3000.github.io/media/examples/X3D/Appartment/Appartment.x3d");
			setItem ("TimeSensor",                   "https://create3000.github.io/media/examples/Time/TimeSensor/TimeSensor.x3d");
			setItem ("PixelTexture",                 "https://create3000.github.io/media/examples/Texturing/PixelTexture/PixelTexture.x3d");
			setItem ("MultiTexture",                 "https://create3000.github.io/media/examples/Texturing/MultiTexture/MultiTexture.x3d");
			setItem ("MovieTexture",                 "https://create3000.github.io/media/examples/Texturing/MovieTexture/MovieTexture.x3d");
			setItem ("ImageTexture",                 "https://create3000.github.io/media/examples/Texturing/ImageTexture/ImageTexture.x3d");
			setItem ("Text",                         "https://create3000.github.io/media/examples/Text/Text/Text.x3d");
			setItem ("FontStyle",                    "https://create3000.github.io/media/examples/Text/FontStyle/FontStyle.x3d");
			setItem ("AudioClip",                    "https://create3000.github.io/media/examples/Sound/AudioClip/AudioClip.x3d");
			setItem ("TwoSidedMaterial",             "https://create3000.github.io/media/examples/Shape/TwoSidedMaterial/TwoSidedMaterial.x3d");
			setItem ("Material",                     "https://create3000.github.io/media/examples/Shape/Material/Material.x3d");
			setItem ("LineProperties",               "https://create3000.github.io/media/examples/Shape/LineProperties/LineProperties.x3d");
			setItem ("FillProperties",               "https://create3000.github.io/media/examples/Shape/FillProperties/FillProperties.x3d");
			setItem ("Appearance",                   "https://create3000.github.io/media/examples/Shape/Appearance/Appearance.x3d");
			setItem ("ShaderPart",                   "https://create3000.github.io/media/examples/Shaders/ShaderPart/ShaderPart.x3d");
			setItem ("FloatVertexAttribute",         "https://create3000.github.io/media/examples/Shaders/FloatVertexAttribute/FloatVertexAttribute.x3d");
			setItem ("ComposedShader",               "https://create3000.github.io/media/examples/Shaders/ComposedShader/ComposedShader.x3d");
			setItem ("SliderJoint",                  "https://create3000.github.io/media/examples/RigidBodyPhysics/SliderJoint/SliderJoint.x3d");
			setItem ("SingleAxisHingeJoint",         "https://create3000.github.io/media/examples/RigidBodyPhysics/SingleAxisHingeJoint/SingleAxisHingeJoint.x3d");
			setItem ("RigidBodyCollection",          "https://create3000.github.io/media/examples/RigidBodyPhysics/RigidBodyCollection/RigidBodyCollection.x3d");
			setItem ("RigidBody",                    "https://create3000.github.io/media/examples/RigidBodyPhysics/RigidBody/RigidBody.x3d");
			setItem ("CollisionCollection",          "https://create3000.github.io/media/examples/RigidBodyPhysics/CollisionCollection/CollisionCollection.x3d");
			setItem ("CollidableShape",              "https://create3000.github.io/media/examples/RigidBodyPhysics/CollidableShape/CollidableShape.x3d");
			setItem ("BallJoint",                    "https://create3000.github.io/media/examples/RigidBodyPhysics/BallJoint/BallJoint.x3d");
			setItem ("PointSet",                     "https://create3000.github.io/media/examples/Rendering/PointSet/PointSet.x3d");
			setItem ("LineSet",                      "https://create3000.github.io/media/examples/Rendering/LineSet/LineSet.x3d");
			setItem ("IndexedTriangleSet",           "https://create3000.github.io/media/examples/Rendering/IndexedTriangleSet/IndexedTriangleSet.x3d");
			setItem ("IndexedTriangleFanSet",        "https://create3000.github.io/media/examples/Rendering/IndexedTriangleFanSet/IndexedTriangleFanSet.x3d");
			setItem ("IndexedLineSet",               "https://create3000.github.io/media/examples/Rendering/IndexedLineSet/IndexedLineSet.x3d");
			setItem ("Coordinate",                   "https://create3000.github.io/media/examples/Rendering/Coordinate/Coordinate.x3d");
			setItem ("ColorRGBA",                    "https://create3000.github.io/media/examples/Rendering/ColorRGBA/ColorRGBA.x3d");
			setItem ("Color",                        "https://create3000.github.io/media/examples/Rendering/Color/Color.x3d");
			setItem ("ClipPlane",                    "https://create3000.github.io/media/examples/Rendering/ClipPlane/ClipPlane.x3d");
			setItem ("PlaneSensor",                  "https://create3000.github.io/media/examples/PointingDeviceSensor/PlaneSensor/PlaneSensor.x3d");
			setItem ("CylinderSensor",               "https://create3000.github.io/media/examples/PointingDeviceSensor/CylinderSensor/CylinderSensor.x3d");
			setItem ("PrimitivePickSensor",          "https://create3000.github.io/media/examples/Picking/PrimitivePickSensor/PrimitivePickSensor.x3d");
			setItem ("PointPickSensor",              "https://create3000.github.io/media/examples/Picking/PointPickSensor/PointPickSensor.x3d");
			setItem ("LinePickSensor",               "https://create3000.github.io/media/examples/Picking/LinePickSensor/LinePickSensor.x3d");
			setItem ("WindPhysicsModel",             "https://create3000.github.io/media/examples/ParticleSystems/WindPhysicsModel/WindPhysicsModel.x3d");
			setItem ("VolumeEmitter",                "https://create3000.github.io/media/examples/ParticleSystems/VolumeEmitter/VolumeEmitter.x3d");
			setItem ("SurfaceEmitter",               "https://create3000.github.io/media/examples/ParticleSystems/SurfaceEmitter/SurfaceEmitter.x3d");
			setItem ("PolylineEmitter",              "https://create3000.github.io/media/examples/ParticleSystems/PolylineEmitter/PolylineEmitter.x3d");
			setItem ("PointEmitter",                 "https://create3000.github.io/media/examples/ParticleSystems/PointEmitter/PointEmitter.x3d");
			setItem ("ParticleSystem",               "https://create3000.github.io/media/examples/ParticleSystems/ParticleSystem/ParticleSystem.x3d");
			setItem ("ForcePhysicsModel",            "https://create3000.github.io/media/examples/ParticleSystems/ForcePhysicsModel/ForcePhysicsModel.x3d");
			setItem ("ExplosionEmitter",             "https://create3000.github.io/media/examples/ParticleSystems/ExplosionEmitter/ExplosionEmitter.x3d");
			setItem ("ConeEmitter",                  "https://create3000.github.io/media/examples/ParticleSystems/ConeEmitter/ConeEmitter.x3d");
			setItem ("LoadSensor",                   "https://create3000.github.io/media/examples/Networking/LoadSensor/LoadSensor.x3d");
			setItem ("Inline",                       "https://create3000.github.io/media/examples/Networking/Inline/Inline.x3d");
			setItem ("Anchor",                       "https://create3000.github.io/media/examples/Networking/Anchor/Anchor.x3d");
			setItem ("Viewpoint",                    "https://create3000.github.io/media/examples/Navigation/Viewpoint/Viewpoint.x3d");
			setItem ("OrthoViewpoint",               "https://create3000.github.io/media/examples/Navigation/OrthoViewpoint/OrthoViewpoint.x3d");
			setItem ("NavigationInfo",               "https://create3000.github.io/media/examples/Navigation/NavigationInfo/NavigationInfo.x3d");
			setItem ("LogarithmicDepthBuffer",       "https://create3000.github.io/media/examples/Navigation/LogarithmicDepthBuffer/LogarithmicDepthBuffer.x3d");
			setItem ("Collision",                    "https://create3000.github.io/media/examples/Navigation/Collision/Collision.x3d");
			setItem ("Billboard",                    "https://create3000.github.io/media/examples/Navigation/Billboard/Billboard.x3d");
			setItem ("NurbsSweptSurface",            "https://create3000.github.io/media/examples/NURBS/NurbsSweptSurface/NurbsSweptSurface.x3d");
			setItem ("NurbsPatchSurface",            "https://create3000.github.io/media/examples/NURBS/NurbsPatchSurface/NurbsPatchSurface.x3d");
			setItem ("NurbsCurve",                   "https://create3000.github.io/media/examples/NURBS/NurbsCurve/NurbsCurve.x3d");
			setItem ("SpotLight",                    "https://create3000.github.io/media/examples/Lighting/SpotLight/SpotLight.x3d");
			setItem ("Shadows",                      "https://create3000.github.io/media/examples/Lighting/Shadows/Shadows.x3d");
			setItem ("PointLight",                   "https://create3000.github.io/media/examples/Lighting/PointLight/PointLight.x3d");
			setItem ("DirectionalLight",             "https://create3000.github.io/media/examples/Lighting/DirectionalLight/DirectionalLight.x3d");
			setItem ("Viewport",                     "https://create3000.github.io/media/examples/Layering/Viewport/Viewport.x3d");
			setItem ("LayerSet",                     "https://create3000.github.io/media/examples/Layering/LayerSet/LayerSet.x3d");
			setItem ("StringSensor",                 "https://create3000.github.io/media/examples/KeyDeviceSensor/StringSensor/StringSensor.x3d");
			setItem ("KeySensor",                    "https://create3000.github.io/media/examples/KeyDeviceSensor/KeySensor/KeySensor.x3d");
			setItem ("SquadOrientationInterpolator", "https://create3000.github.io/media/examples/Interpolation/SquadOrientationInterpolator/SquadOrientationInterpolator.x3d");
			setItem ("SplineScalarInterpolator",     "https://create3000.github.io/media/examples/Interpolation/SplineScalarInterpolator/SplineScalarInterpolator.x3d");
			setItem ("ScalarInterpolator",           "https://create3000.github.io/media/examples/Interpolation/ScalarInterpolator/ScalarInterpolator.x3d");
			setItem ("PositionInterpolator2D",       "https://create3000.github.io/media/examples/Interpolation/PositionInterpolator2D/PositionInterpolator2D.x3d");
			setItem ("PositionInterpolator",         "https://create3000.github.io/media/examples/Interpolation/PositionInterpolator/PositionInterpolator.x3d");
			setItem ("OrientationInterpolator",      "https://create3000.github.io/media/examples/Interpolation/OrientationInterpolator/OrientationInterpolator.x3d");
			setItem ("CoordinateInterpolator",       "https://create3000.github.io/media/examples/Interpolation/CoordinateInterpolator/CoordinateInterpolator.x3d");
			setItem ("ColorInterpolator",            "https://create3000.github.io/media/examples/Interpolation/ColorInterpolator/ColorInterpolator.x3d");
			setItem ("HAnimHumanoid",                "https://create3000.github.io/media/examples/H-Anim/HAnimHumanoid/HAnimHumanoid.x3d");
			setItem ("Transform",                    "https://create3000.github.io/media/examples/Grouping/Transform/Transform.x3d");
			setItem ("Switch",                       "https://create3000.github.io/media/examples/Grouping/Switch/Switch.x3d");
			setItem ("Group",                        "https://create3000.github.io/media/examples/Grouping/Group/Group.x3d");
			setItem ("GeoViewpoint",                 "https://create3000.github.io/media/examples/Geospatial/GeoViewpoint/GeoViewpoint.x3d");
			setItem ("GeoLOD",                       "https://create3000.github.io/media/examples/Geospatial/GeoLOD/GeoLOD.x3d");
			setItem ("GeoElevationGrid",             "https://create3000.github.io/media/examples/Geospatial/GeoElevationGrid/GeoElevationGrid.x3d");
			setItem ("Sphere",                       "https://create3000.github.io/media/examples/Geometry3D/Sphere/Sphere.x3d");
			setItem ("IndexedFaceSet",               "https://create3000.github.io/media/examples/Geometry3D/IndexedFaceSet/IndexedFaceSet.x3d");
			setItem ("Extrusion",                    "https://create3000.github.io/media/examples/Geometry3D/Extrusion/Extrusion.x3d");
			setItem ("ElevationGrid",                "https://create3000.github.io/media/examples/Geometry3D/ElevationGrid/ElevationGrid.x3d");
			setItem ("Cylinder",                     "https://create3000.github.io/media/examples/Geometry3D/Cylinder/Cylinder.x3d");
			setItem ("Cone",                         "https://create3000.github.io/media/examples/Geometry3D/Cone/Cone.x3d");
			setItem ("Box",                          "https://create3000.github.io/media/examples/Geometry3D/Box/Box.x3d");
			setItem ("TriangleSet2D",                "https://create3000.github.io/media/examples/Geometry2D/TriangleSet2D/TriangleSet2D.x3d");
			setItem ("Rectangle2D",                  "https://create3000.github.io/media/examples/Geometry2D/Rectangle2D/Rectangle2D.x3d");
			setItem ("Polypoint2D",                  "https://create3000.github.io/media/examples/Geometry2D/Polypoint2D/Polypoint2D.x3d");
			setItem ("Polyline2D",                   "https://create3000.github.io/media/examples/Geometry2D/Polyline2D/Polyline2D.x3d");
			setItem ("Disk2D",                       "https://create3000.github.io/media/examples/Geometry2D/Disk2D/Disk2D.x3d");
			setItem ("Circle2D",                     "https://create3000.github.io/media/examples/Geometry2D/Circle2D/Circle2D.x3d");
			setItem ("ArcClose2D",                   "https://create3000.github.io/media/examples/Geometry2D/ArcClose2D/ArcClose2D.x3d");
			setItem ("Arc2D",                        "https://create3000.github.io/media/examples/Geometry2D/Arc2D/Arc2D.x3d");
			setItem ("ScalarChaser",                 "https://create3000.github.io/media/examples/Followers/ScalarChaser/ScalarChaser.x3d");
			setItem ("CoordinateChaser",             "https://create3000.github.io/media/examples/Followers/CoordinateChaser/CoordinateChaser.x3d");
			setItem ("ColorDamper",                  "https://create3000.github.io/media/examples/Followers/ColorDamper/ColorDamper.x3d");
			setItem ("ColorChaser",                  "https://create3000.github.io/media/examples/Followers/ColorChaser/ColorChaser.x3d");
			setItem ("IntegerSequencer",             "https://create3000.github.io/media/examples/EventUtilities/IntegerSequencer/IntegerSequencer.x3d");
			setItem ("VisibilitySensor",             "https://create3000.github.io/media/examples/EnvironmentalSensor/VisibilitySensor/VisibilitySensor.x3d");
			setItem ("TransformSensor",              "https://create3000.github.io/media/examples/EnvironmentalSensor/TransformSensor/TransformSensor.x3d");
			setItem ("ProximitySensor",              "https://create3000.github.io/media/examples/EnvironmentalSensor/ProximitySensor/ProximitySensor.x3d");
			setItem ("TextureBackground",            "https://create3000.github.io/media/examples/EnvironmentalEffects/TextureBackground/TextureBackground.x3d");
			setItem ("LocalFog",                     "https://create3000.github.io/media/examples/EnvironmentalEffects/LocalFog/LocalFog.x3d");
			setItem ("Fog",                          "https://create3000.github.io/media/examples/EnvironmentalEffects/Fog/Fog.x3d");
			setItem ("Background",                   "https://create3000.github.io/media/examples/EnvironmentalEffects/Background/Background.x3d");
			setItem ("GeneratedCubeMapTexture",      "https://create3000.github.io/media/examples/CubeMapTexturing/GeneratedCubeMapTexture/GeneratedCubeMapTexture.x3d");
			setItem ("ComposedCubeMapTexture",       "https://create3000.github.io/media/examples/CubeMapTexturing/ComposedCubeMapTexture/ComposedCubeMapTexture.x3d");
			setItem ("QuadSet",                      "https://create3000.github.io/media/examples/CADGeometry/QuadSet/QuadSet.x3d");

			setItem ("about:date",  get_page ("about/date.x3dv"));
			setItem ("about:gears", get_page ("about/gears.x3dv"));
			setItem ("about:home",  get_page ("about/home.x3dv"));
		}

		// Watch for changes

		connect ();
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
History::connect ()
{
	// Use:
	// http://www.sqlite.org/c3ref/update_hook.html
	//
	//	void *sqlite3_update_hook (
	//		sqlite3*,
	//		void(*)(void *,int ,char const *,char const *,sqlite3_int64),
	//		void*
	//	);

	fileMonitor = Gio::File::create_for_path (filename) -> monitor_file ();
	fileMonitor -> signal_changed () .connect (sigc::mem_fun (this, &History::on_history_changed));
}

void
History::disconnect ()
{
	fileMonitor = Glib::RefPtr <Gio::FileMonitor> ();
}

void
History::on_history_changed (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::File> &, Gio::FileMonitorEvent event)
{
	if (event == Gio::FILE_MONITOR_EVENT_CHANGED)
		processInterests ();
}

///  throws std::out_of_range, std::invalid_argument
std::string
History::getId (const basic::uri & worldURL) const
{
	const auto & result = database .query_array ("SELECT id FROM History WHERE "
	                                             "worldURL = " + database .quote (worldURL .escape ()));

	return result .at (0) .at (0);
}

///  throws std::out_of_range, std::invalid_argument
//const std::string &
//History::getIndex (const std::string & worldURL) const
//{
//	const auto & result = database .query_array ("SELECT "
//	                                             "(SELECT COUNT(0) - 1 FROM History h1 WHERE h1 .lastAccess >= h2 .lastAccess) AS 'rowid' "
//	                                             "FROM History h2 "
//	                                             "WHERE worldURL = " + database .quote (worldURL) + " " +
//	                                             "ORDER BY lastAccess DESC");
//
//	const std::string & index = result .at (0) .at (0);
//
//	if (index == "-1")
//		throw std::out_of_range ("URL '" + worldURL + "' not found.");
//
//	return index;
//}

void
History::setPreview (const basic::uri & worldURL, const std::string & image)
{
	try
	{
		database .write_blob ("UPDATE History SET preview = ? WHERE id = " + getId (worldURL), image);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

///  throws std::invalid_argument
std::string
History::getPreview (const std::string & id) const
{
	std::string value;

	database .read_blob ("SELECT preview FROM History WHERE id = " + id, value);

	return value;
}

void
History::setContextPath (const basic::uri & worldURL, const std::string & contextPath)
{
	try
	{
		database .query ("UPDATE History "
		                 "SET "
		                 "contextPath = " + database .quote (contextPath) + " "
		                 "WHERE worldURL = " + database .quote (worldURL .escape ()));
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

std::string
History::getContextPath (const basic::uri & worldURL) const
{
	try
	{
		const auto & items = database .query_assoc ("SELECT contextPath FROM History WHERE worldURL = " + database .quote (worldURL .escape ()));

		return items .at (0) .at ("contextPath");
	}
	catch (const std::exception & error)
	{
		return "";
	}
}

void
History::setItem (const std::string & title, const basic::uri & worldURL)
{
	try
	{
		try
		{
			update (getId (worldURL), title);
		}
		catch (const std::out_of_range &)
		{
			insert (title, worldURL);
		}
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
History::setItem (const basic::uri & worldURL)
{
	try
	{
		try
		{
			update (getId (worldURL), worldURL .escape (" ") .basename ());
		}
		catch (const std::out_of_range &)
		{
			insert (worldURL .escape (" ") .basename (), worldURL);
		}
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

sql::sqlite3::assoc_row_type
History::getItem (const std::string & id) const
{
	try
	{
		return database .query_assoc ("SELECT title, worldURL FROM History WHERE id = " + id) .at (0);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;

		return sql::sqlite3::assoc_row_type ();
	}
}

sql::sqlite3::assoc_type
History::getItems (const size_t offset, const size_t size, const std::string & search) const
{
	try
	{
		sql::sqlite3::assoc_type result;

		const std::string where = getWhere (search);
		const std::string order = getOrder (LAST_ACCESS, DESC);
		const std::string limit = getLimit (offset, size);

		const auto & items = database .query_assoc ("SELECT "
		                                            "id, "
		                                            "title, "
		                                            "worldURL, "
		                                            "(strftime('%s', lastAccess) || substr (lastAccess, 20)) AS lastAccess "
		                                            "FROM History " +
		                                            where + " " +
		                                            order + " " +
		                                            limit);

		return items;
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;

		static const sql::sqlite3::assoc_type empty;

		return empty;
	}
}

void
History::removeItem (const std::string & id)
{
	try
	{
		database .query ("DELETE FROM History WHERE id = " + id);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
History::setSize (const int32_t months)
{
	try
	{
		if (months > 0)
			database .query ("DELETE FROM History WHERE lastAccess < date ('now','-" + basic::to_string (months, std::locale::classic ()) + " month')");
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

size_t
History::getSize () const
{
	try
	{
		size_t size = 0;

		std::istringstream isstream (database .query_array ("SELECT COUNT (*) FROM History") .at (0) .at (0));

		isstream >> size;

		return size;
	}
	catch (...)
	{
		return 0;
	}
}

std::string
History::getWhere (const std::string & search) const
{
	if (search .empty ())
		return "";

	auto words = std::vector <std::string> ();
	auto terms = std::vector <std::string> ();

	basic::split (std::back_inserter (words), search, " ");

	for (const auto & word : words)
	{
		if (word .empty ())
			continue;

		terms .emplace_back ("(title LIKE " + database .quote ("%" + word + "%") + " OR worldURL LIKE " + database .quote ("%" + word + "%") + ")");
	}

	return "WHERE " + basic::join (terms, " AND ");
}

std::string
History::getOrder (const Columns column, const SortOrder sortOrder) const
{
	std::string order;

	switch (column)
	{
		case TITLE:
			order = "ORDER BY title";
			break;
		case LAST_ACCESS:
			order = "ORDER BY lastAccess";
			break;
	}

	switch (sortOrder)
	{
		case ASC:
			order += " ASC";
			break;
		case DESC:
			order += " DESC";
			break;
	}

	return order;
}

std::string
History::getLimit (const size_t offset, const size_t size) const
{
	std::string limit;

	if (size)
		limit = "LIMIT " + basic::to_string (size, std::locale::classic ()) + " OFFSET " + basic::to_string (offset, std::locale::classic ());

	return limit;
}

void
History::insert (const std::string & title, const basic::uri & worldURL)
{
	database .query ("INSERT INTO History "
	                 "(title, worldURL)"
	                 "VALUES ("
	                 + database .quote (title) + ","
	                 + database .quote (worldURL .escape ())
	                 + ")");
}

void
History::update (const std::string & id, const std::string & title)
{
	database .query ("UPDATE History "
	                 "SET "
	                 "title = " + database .quote (title) + ","
	                 "visited    = (visited + 1), "
	                 "lastAccess = strftime('%Y-%m-%d %H:%M:%f', 'now') "
	                 "WHERE id = " + id);
}

History::~History ()
{
	try
	{
		for (const auto item : getItems (0, 0))
		{
			const auto worldURL = basic::uri (item .at ("worldURL"));

			if (worldURL .is_local () and not Glib::file_test (worldURL .path (), Glib::FILE_TEST_EXISTS))
			{
				removeItem (item .at ("id"));
			}
		}
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

} // puck
} // titania
