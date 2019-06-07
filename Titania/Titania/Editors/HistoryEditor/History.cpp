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
			setItem ("BlendMode",                    "http://media.create3000.de/components/X_ITE/BlendMode/BlendMode.x3d");
			setItem ("WaterQuality",                 "http://media.create3000.de/components/X3D/WaterQuality/WaterQuality.x3d");
			setItem ("Vattenfall",                   "http://media.create3000.de/components/X3D/Vattenfall/Vattenfall.x3d");
			setItem ("TreasureIsland",               "http://media.create3000.de/components/X3D/TreasureIsland/TreasureIsland.x3d");
			setItem ("TikiWorld",                    "http://media.create3000.de/components/X3D/TikiWorld/TikiWorld.x3d");
			setItem ("SugarSmack",                   "http://media.create3000.de/components/X3D/SugarSmack/SugarSmack.x3d");
			setItem ("SmashingBoxes",                "http://media.create3000.de/components/X3D/SmashingBoxes/SmashingBoxes.x3d");
			setItem ("SmartyBubbles",                "http://media.create3000.de/components/X3D/SmartyBubbles/SmartyBubbles.x3d");
			setItem ("SlidingPuzzle",                "http://media.create3000.de/components/X3D/SlidingPuzzle/SlidingPuzzle.x3d");
			setItem ("SecretLabyrinth",              "http://media.create3000.de/components/X3D/SecretLabyrinth/SecretLabyrinth.x3d");
			setItem ("Pong",                         "http://media.create3000.de/components/X3D/Pong/Pong.x3d");
			setItem ("MilkywayAndBeyond",            "http://media.create3000.de/components/X3D/MilkywayAndBeyond/MilkywayAndBeyond.x3d");
			setItem ("MagicMushrooms",               "http://media.create3000.de/components/X3D/MagicMushrooms/MagicMushrooms.x3d");
			setItem ("LustForLife",                  "http://media.create3000.de/components/X3D/LustForLife/LustForLife.x3d");
			setItem ("LogoPieces",                   "http://media.create3000.de/components/X3D/LogoPieces/LogoPieces.x3d");
			setItem ("FoldUp",                       "http://media.create3000.de/components/X3D/FoldUp/FoldUp.x3d");
			setItem ("FlashingLights",               "http://media.create3000.de/components/X3D/FlashingLights/FlashingLights.x3d");
			setItem ("Currencies",                   "http://media.create3000.de/components/X3D/Currencies/Currencies.x3d");
			setItem ("CrazySpiral",                  "http://media.create3000.de/components/X3D/CrazySpiral/CrazySpiral.x3d");
			setItem ("Circles",                      "http://media.create3000.de/components/X3D/Circles/Circles.x3d");
			setItem ("Chomp",                        "http://media.create3000.de/components/X3D/Chomp/Chomp.x3d");
			setItem ("BeyondGermany",                "http://media.create3000.de/components/X3D/BeyondGermany/BeyondGermany.x3d");
			setItem ("BIC",                          "http://media.create3000.de/components/X3D/BIC/BIC.x3d");
			setItem ("Astronomy",                    "http://media.create3000.de/components/X3D/Astronomy/Astronomy.x3d");
			setItem ("Arcadia",                      "http://media.create3000.de/components/X3D/Arcadia/Arcadia.x3d");
			setItem ("Approach",                     "http://media.create3000.de/components/X3D/Approach/Approach.x3d");
			setItem ("Appartment",                   "http://media.create3000.de/components/X3D/Appartment/Appartment.x3d");
			setItem ("TimeSensor",                   "http://media.create3000.de/components/Time/TimeSensor/TimeSensor.x3d");
			setItem ("PixelTexture",                 "http://media.create3000.de/components/Texturing/PixelTexture/PixelTexture.x3d");
			setItem ("MultiTexture",                 "http://media.create3000.de/components/Texturing/MultiTexture/MultiTexture.x3d");
			setItem ("MovieTexture",                 "http://media.create3000.de/components/Texturing/MovieTexture/MovieTexture.x3d");
			setItem ("ImageTexture",                 "http://media.create3000.de/components/Texturing/ImageTexture/ImageTexture.x3d");
			setItem ("Text",                         "http://media.create3000.de/components/Text/Text/Text.x3d");
			setItem ("FontStyle",                    "http://media.create3000.de/components/Text/FontStyle/FontStyle.x3d");
			setItem ("AudioClip",                    "http://media.create3000.de/components/Sound/AudioClip/AudioClip.x3d");
			setItem ("TwoSidedMaterial",             "http://media.create3000.de/components/Shape/TwoSidedMaterial/TwoSidedMaterial.x3d");
			setItem ("Material",                     "http://media.create3000.de/components/Shape/Material/Material.x3d");
			setItem ("LineProperties",               "http://media.create3000.de/components/Shape/LineProperties/LineProperties.x3d");
			setItem ("FillProperties",               "http://media.create3000.de/components/Shape/FillProperties/FillProperties.x3d");
			setItem ("Appearance",                   "http://media.create3000.de/components/Shape/Appearance/Appearance.x3d");
			setItem ("ShaderPart",                   "http://media.create3000.de/components/Shaders/ShaderPart/ShaderPart.x3d");
			setItem ("FloatVertexAttribute",         "http://media.create3000.de/components/Shaders/FloatVertexAttribute/FloatVertexAttribute.x3d");
			setItem ("ComposedShader",               "http://media.create3000.de/components/Shaders/ComposedShader/ComposedShader.x3d");
			setItem ("SliderJoint",                  "http://media.create3000.de/components/RigidBodyPhysics/SliderJoint/SliderJoint.x3d");
			setItem ("SingleAxisHingeJoint",         "http://media.create3000.de/components/RigidBodyPhysics/SingleAxisHingeJoint/SingleAxisHingeJoint.x3d");
			setItem ("RigidBodyCollection",          "http://media.create3000.de/components/RigidBodyPhysics/RigidBodyCollection/RigidBodyCollection.x3d");
			setItem ("RigidBody",                    "http://media.create3000.de/components/RigidBodyPhysics/RigidBody/RigidBody.x3d");
			setItem ("CollisionCollection",          "http://media.create3000.de/components/RigidBodyPhysics/CollisionCollection/CollisionCollection.x3d");
			setItem ("CollidableShape",              "http://media.create3000.de/components/RigidBodyPhysics/CollidableShape/CollidableShape.x3d");
			setItem ("BallJoint",                    "http://media.create3000.de/components/RigidBodyPhysics/BallJoint/BallJoint.x3d");
			setItem ("PointSet",                     "http://media.create3000.de/components/Rendering/PointSet/PointSet.x3d");
			setItem ("LineSet",                      "http://media.create3000.de/components/Rendering/LineSet/LineSet.x3d");
			setItem ("IndexedTriangleSet",           "http://media.create3000.de/components/Rendering/IndexedTriangleSet/IndexedTriangleSet.x3d");
			setItem ("IndexedTriangleFanSet",        "http://media.create3000.de/components/Rendering/IndexedTriangleFanSet/IndexedTriangleFanSet.x3d");
			setItem ("IndexedLineSet",               "http://media.create3000.de/components/Rendering/IndexedLineSet/IndexedLineSet.x3d");
			setItem ("Coordinate",                   "http://media.create3000.de/components/Rendering/Coordinate/Coordinate.x3d");
			setItem ("ColorRGBA",                    "http://media.create3000.de/components/Rendering/ColorRGBA/ColorRGBA.x3d");
			setItem ("Color",                        "http://media.create3000.de/components/Rendering/Color/Color.x3d");
			setItem ("ClipPlane",                    "http://media.create3000.de/components/Rendering/ClipPlane/ClipPlane.x3d");
			setItem ("PlaneSensor",                  "http://media.create3000.de/components/PointingDeviceSensor/PlaneSensor/PlaneSensor.x3d");
			setItem ("CylinderSensor",               "http://media.create3000.de/components/PointingDeviceSensor/CylinderSensor/CylinderSensor.x3d");
			setItem ("PrimitivePickSensor",          "http://media.create3000.de/components/Picking/PrimitivePickSensor/PrimitivePickSensor.x3d");
			setItem ("PointPickSensor",              "http://media.create3000.de/components/Picking/PointPickSensor/PointPickSensor.x3d");
			setItem ("LinePickSensor",               "http://media.create3000.de/components/Picking/LinePickSensor/LinePickSensor.x3d");
			setItem ("WindPhysicsModel",             "http://media.create3000.de/components/ParticleSystems/WindPhysicsModel/WindPhysicsModel.x3d");
			setItem ("VolumeEmitter",                "http://media.create3000.de/components/ParticleSystems/VolumeEmitter/VolumeEmitter.x3d");
			setItem ("SurfaceEmitter",               "http://media.create3000.de/components/ParticleSystems/SurfaceEmitter/SurfaceEmitter.x3d");
			setItem ("PolylineEmitter",              "http://media.create3000.de/components/ParticleSystems/PolylineEmitter/PolylineEmitter.x3d");
			setItem ("PointEmitter",                 "http://media.create3000.de/components/ParticleSystems/PointEmitter/PointEmitter.x3d");
			setItem ("ParticleSystem",               "http://media.create3000.de/components/ParticleSystems/ParticleSystem/ParticleSystem.x3d");
			setItem ("ForcePhysicsModel",            "http://media.create3000.de/components/ParticleSystems/ForcePhysicsModel/ForcePhysicsModel.x3d");
			setItem ("ExplosionEmitter",             "http://media.create3000.de/components/ParticleSystems/ExplosionEmitter/ExplosionEmitter.x3d");
			setItem ("ConeEmitter",                  "http://media.create3000.de/components/ParticleSystems/ConeEmitter/ConeEmitter.x3d");
			setItem ("LoadSensor",                   "http://media.create3000.de/components/Networking/LoadSensor/LoadSensor.x3d");
			setItem ("Inline",                       "http://media.create3000.de/components/Networking/Inline/Inline.x3d");
			setItem ("Anchor",                       "http://media.create3000.de/components/Networking/Anchor/Anchor.x3d");
			setItem ("Viewpoint",                    "http://media.create3000.de/components/Navigation/Viewpoint/Viewpoint.x3d");
			setItem ("OrthoViewpoint",               "http://media.create3000.de/components/Navigation/OrthoViewpoint/OrthoViewpoint.x3d");
			setItem ("NavigationInfo",               "http://media.create3000.de/components/Navigation/NavigationInfo/NavigationInfo.x3d");
			setItem ("LogarithmicDepthBuffer",       "http://media.create3000.de/components/Navigation/LogarithmicDepthBuffer/LogarithmicDepthBuffer.x3d");
			setItem ("Collision",                    "http://media.create3000.de/components/Navigation/Collision/Collision.x3d");
			setItem ("Billboard",                    "http://media.create3000.de/components/Navigation/Billboard/Billboard.x3d");
			setItem ("NurbsSweptSurface",            "http://media.create3000.de/components/NURBS/NurbsSweptSurface/NurbsSweptSurface.x3d");
			setItem ("NurbsPatchSurface",            "http://media.create3000.de/components/NURBS/NurbsPatchSurface/NurbsPatchSurface.x3d");
			setItem ("NurbsCurve",                   "http://media.create3000.de/components/NURBS/NurbsCurve/NurbsCurve.x3d");
			setItem ("SpotLight",                    "http://media.create3000.de/components/Lighting/SpotLight/SpotLight.x3d");
			setItem ("Shadows",                      "http://media.create3000.de/components/Lighting/Shadows/Shadows.x3d");
			setItem ("PointLight",                   "http://media.create3000.de/components/Lighting/PointLight/PointLight.x3d");
			setItem ("DirectionalLight",             "http://media.create3000.de/components/Lighting/DirectionalLight/DirectionalLight.x3d");
			setItem ("Viewport",                     "http://media.create3000.de/components/Layering/Viewport/Viewport.x3d");
			setItem ("LayerSet",                     "http://media.create3000.de/components/Layering/LayerSet/LayerSet.x3d");
			setItem ("StringSensor",                 "http://media.create3000.de/components/KeyDeviceSensor/StringSensor/StringSensor.x3d");
			setItem ("KeySensor",                    "http://media.create3000.de/components/KeyDeviceSensor/KeySensor/KeySensor.x3d");
			setItem ("SquadOrientationInterpolator", "http://media.create3000.de/components/Interpolation/SquadOrientationInterpolator/SquadOrientationInterpolator.x3d");
			setItem ("SplineScalarInterpolator",     "http://media.create3000.de/components/Interpolation/SplineScalarInterpolator/SplineScalarInterpolator.x3d");
			setItem ("ScalarInterpolator",           "http://media.create3000.de/components/Interpolation/ScalarInterpolator/ScalarInterpolator.x3d");
			setItem ("PositionInterpolator2D",       "http://media.create3000.de/components/Interpolation/PositionInterpolator2D/PositionInterpolator2D.x3d");
			setItem ("PositionInterpolator",         "http://media.create3000.de/components/Interpolation/PositionInterpolator/PositionInterpolator.x3d");
			setItem ("OrientationInterpolator",      "http://media.create3000.de/components/Interpolation/OrientationInterpolator/OrientationInterpolator.x3d");
			setItem ("CoordinateInterpolator",       "http://media.create3000.de/components/Interpolation/CoordinateInterpolator/CoordinateInterpolator.x3d");
			setItem ("ColorInterpolator",            "http://media.create3000.de/components/Interpolation/ColorInterpolator/ColorInterpolator.x3d");
			setItem ("HAnimHumanoid",                "http://media.create3000.de/components/H-Anim/HAnimHumanoid/HAnimHumanoid.x3d");
			setItem ("Transform",                    "http://media.create3000.de/components/Grouping/Transform/Transform.x3d");
			setItem ("Switch",                       "http://media.create3000.de/components/Grouping/Switch/Switch.x3d");
			setItem ("Group",                        "http://media.create3000.de/components/Grouping/Group/Group.x3d");
			setItem ("GeoViewpoint",                 "http://media.create3000.de/components/Geospatial/GeoViewpoint/GeoViewpoint.x3d");
			setItem ("GeoLOD",                       "http://media.create3000.de/components/Geospatial/GeoLOD/GeoLOD.x3d");
			setItem ("GeoElevationGrid",             "http://media.create3000.de/components/Geospatial/GeoElevationGrid/GeoElevationGrid.x3d");
			setItem ("Sphere",                       "http://media.create3000.de/components/Geometry3D/Sphere/Sphere.x3d");
			setItem ("IndexedFaceSet",               "http://media.create3000.de/components/Geometry3D/IndexedFaceSet/IndexedFaceSet.x3d");
			setItem ("Extrusion",                    "http://media.create3000.de/components/Geometry3D/Extrusion/Extrusion.x3d");
			setItem ("ElevationGrid",                "http://media.create3000.de/components/Geometry3D/ElevationGrid/ElevationGrid.x3d");
			setItem ("Cylinder",                     "http://media.create3000.de/components/Geometry3D/Cylinder/Cylinder.x3d");
			setItem ("Cone",                         "http://media.create3000.de/components/Geometry3D/Cone/Cone.x3d");
			setItem ("Box",                          "http://media.create3000.de/components/Geometry3D/Box/Box.x3d");
			setItem ("TriangleSet2D",                "http://media.create3000.de/components/Geometry2D/TriangleSet2D/TriangleSet2D.x3d");
			setItem ("Rectangle2D",                  "http://media.create3000.de/components/Geometry2D/Rectangle2D/Rectangle2D.x3d");
			setItem ("Polypoint2D",                  "http://media.create3000.de/components/Geometry2D/Polypoint2D/Polypoint2D.x3d");
			setItem ("Polyline2D",                   "http://media.create3000.de/components/Geometry2D/Polyline2D/Polyline2D.x3d");
			setItem ("Disk2D",                       "http://media.create3000.de/components/Geometry2D/Disk2D/Disk2D.x3d");
			setItem ("Circle2D",                     "http://media.create3000.de/components/Geometry2D/Circle2D/Circle2D.x3d");
			setItem ("ArcClose2D",                   "http://media.create3000.de/components/Geometry2D/ArcClose2D/ArcClose2D.x3d");
			setItem ("Arc2D",                        "http://media.create3000.de/components/Geometry2D/Arc2D/Arc2D.x3d");
			setItem ("ScalarChaser",                 "http://media.create3000.de/components/Followers/ScalarChaser/ScalarChaser.x3d");
			setItem ("ColorDamper",                  "http://media.create3000.de/components/Followers/ColorDamper/ColorDamper.x3d");
			setItem ("ColorChaser",                  "http://media.create3000.de/components/Followers/ColorChaser/ColorChaser.x3d");
			setItem ("IntegerSequencer",             "http://media.create3000.de/components/EventUtilities/IntegerSequencer/IntegerSequencer.x3d");
			setItem ("VisibilitySensor",             "http://media.create3000.de/components/EnvironmentalSensor/VisibilitySensor/VisibilitySensor.x3d");
			setItem ("TransformSensor",              "http://media.create3000.de/components/EnvironmentalSensor/TransformSensor/TransformSensor.x3d");
			setItem ("ProximitySensor",              "http://media.create3000.de/components/EnvironmentalSensor/ProximitySensor/ProximitySensor.x3d");
			setItem ("TextureBackground",            "http://media.create3000.de/components/EnvironmentalEffects/TextureBackground/TextureBackground.x3d");
			setItem ("LocalFog",                     "http://media.create3000.de/components/EnvironmentalEffects/LocalFog/LocalFog.x3d");
			setItem ("Fog",                          "http://media.create3000.de/components/EnvironmentalEffects/Fog/Fog.x3d");
			setItem ("Background",                   "http://media.create3000.de/components/EnvironmentalEffects/Background/Background.x3d");
			setItem ("GeneratedCubeMapTexture",      "http://media.create3000.de/components/CubeMapTexturing/GeneratedCubeMapTexture/GeneratedCubeMapTexture.x3d");
			setItem ("ComposedCubeMapTexture",       "http://media.create3000.de/components/CubeMapTexturing/ComposedCubeMapTexture/ComposedCubeMapTexture.x3d");
			setItem ("QuadSet",                      "http://media.create3000.de/components/CADGeometry/QuadSet/QuadSet.x3d");

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
