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

#include "SupportedProfiles.h"

#include <algorithm>

namespace titania {
namespace X3D {

SupportedProfiles::SupportedProfiles (const std::shared_ptr <SupportedComponents> & supportedComponents) :
	profiles (new ProfileInfoArray ())
{
	//std::clog << "Creating profile index:" << std::endl;

	add ("Computer-Aided Design (CAD) interchange", "CADInterchange", {
		supportedComponents -> get ("Core",        1),
		supportedComponents -> get ("Networking",  2),
		supportedComponents -> get ("Grouping",    1),
		supportedComponents -> get ("Rendering",   4),
		supportedComponents -> get ("Shape",       2),
		supportedComponents -> get ("Lighting",    1),
		supportedComponents -> get ("Texturing",   2),
		supportedComponents -> get ("Navigation",  2),
		supportedComponents -> get ("Shaders",     1),
		supportedComponents -> get ("CADGeometry", 2)
	});

	add ("Core", "Core", {
		supportedComponents -> get ("Core", 1)
	});

	add ("Full", "Full", {
		supportedComponents -> get ("Core",                 2),
		supportedComponents -> get ("Time",                 2),
		supportedComponents -> get ("Networking",           3),
		supportedComponents -> get ("Grouping",             3),
		supportedComponents -> get ("Rendering",            5),
		supportedComponents -> get ("Shape",                4),
		supportedComponents -> get ("Geometry3D",           4),
		supportedComponents -> get ("Geometry2D",           2),
		supportedComponents -> get ("Text",                 1),
		supportedComponents -> get ("Sound",                1),
		supportedComponents -> get ("Lighting",             3),
		supportedComponents -> get ("Texturing",            3),
		supportedComponents -> get ("Interpolation",        5),
		supportedComponents -> get ("PointingDeviceSensor", 1),
		supportedComponents -> get ("KeyDeviceSensor",      2),
		supportedComponents -> get ("EnvironmentalSensor",  3),
		supportedComponents -> get ("Navigation",           3),
		supportedComponents -> get ("EnvironmentalEffects", 4),
		supportedComponents -> get ("Geospatial",           2),
		supportedComponents -> get ("H-Anim",               1),
		supportedComponents -> get ("NURBS",                4),
		supportedComponents -> get ("DIS",                  2),
		supportedComponents -> get ("Scripting",            1),
		supportedComponents -> get ("EventUtilities",       1),
		supportedComponents -> get ("Shaders",              1),
		supportedComponents -> get ("CADGeometry",          2),
		supportedComponents -> get ("Texturing3D",          2),
		supportedComponents -> get ("CubeMapTexturing",     3),
		supportedComponents -> get ("Layering",             1),
		supportedComponents -> get ("Layout",               2),
		supportedComponents -> get ("RigidBodyPhysics",     2),
		supportedComponents -> get ("Picking",              3),
		supportedComponents -> get ("Followers",            1),
		supportedComponents -> get ("ParticleSystems",      3) /*,
		supportedComponents -> get ("VolumeRendering",      4) */
	});

	add ("Immersive", "Immersive", {
		supportedComponents -> get ("Core",                 2),
		supportedComponents -> get ("Time",                 1),
		supportedComponents -> get ("Networking",           3),
		supportedComponents -> get ("Grouping",             2),
		supportedComponents -> get ("Rendering",            3),
		supportedComponents -> get ("Shape",                2),
		supportedComponents -> get ("Geometry3D",           4),
		supportedComponents -> get ("Geometry2D",           1),
		supportedComponents -> get ("Text",                 1),
		supportedComponents -> get ("Sound",                1),
		supportedComponents -> get ("Lighting",             2),
		supportedComponents -> get ("Texturing",            3),
		supportedComponents -> get ("Interpolation",        2),
		supportedComponents -> get ("PointingDeviceSensor", 1),
		supportedComponents -> get ("KeyDeviceSensor",      2),
		supportedComponents -> get ("EnvironmentalSensor",  2),
		supportedComponents -> get ("Navigation",           2),
		supportedComponents -> get ("EnvironmentalEffects", 2),
		supportedComponents -> get ("Scripting",            1),
		supportedComponents -> get ("EventUtilities",       1)
	});

	add ("Interactive", "Interactive", {
		supportedComponents -> get ("Core",                 1),
		supportedComponents -> get ("Time",                 1),
		supportedComponents -> get ("Networking",           2),
		supportedComponents -> get ("Grouping",             2),
		supportedComponents -> get ("Rendering",            3),
		supportedComponents -> get ("Shape",                1),
		supportedComponents -> get ("Geometry3D",           3),
		supportedComponents -> get ("Lighting",             2),
		supportedComponents -> get ("Texturing",            2),
		supportedComponents -> get ("Interpolation",        2),
		supportedComponents -> get ("PointingDeviceSensor", 1),
		supportedComponents -> get ("KeyDeviceSensor",      1),
		supportedComponents -> get ("EnvironmentalSensor",  1),
		supportedComponents -> get ("Navigation",           1),
		supportedComponents -> get ("EnvironmentalEffects", 1),
		supportedComponents -> get ("EventUtilities",       1)
	});

	add ("Interchange", "Interchange", {
		supportedComponents -> get ("Core",                 1),
		supportedComponents -> get ("Time",                 1),
		supportedComponents -> get ("Networking",           1),
		supportedComponents -> get ("Grouping",             1),
		supportedComponents -> get ("Rendering",            3),
		supportedComponents -> get ("Shape",                1),
		supportedComponents -> get ("Geometry3D",           2),
		supportedComponents -> get ("Lighting",             1),
		supportedComponents -> get ("Texturing",            2),
		supportedComponents -> get ("Interpolation",        2),
		supportedComponents -> get ("Navigation",           1),
		supportedComponents -> get ("EnvironmentalEffects", 1)
	});

//	add ("Medical interchange", "MedicalInterchange", {
//	        supportedComponents -> get ("Core",                 1),
//	        supportedComponents -> get ("Time",                 1),
//	        supportedComponents -> get ("Networking",           2),
//	        supportedComponents -> get ("Grouping",             3),
//	        supportedComponents -> get ("Rendering",            5),
//	        supportedComponents -> get ("Shape",                3),
//	        supportedComponents -> get ("Geometry3D",           2),
//	        supportedComponents -> get ("Geometry2D",           2),
//	        supportedComponents -> get ("Text",                 1),
//	        supportedComponents -> get ("Lighting",             1),
//	        supportedComponents -> get ("Texturing",            2),
//	        supportedComponents -> get ("Interpolation",        2),
//	        supportedComponents -> get ("Navigation",           3),
//	        supportedComponents -> get ("EnvironmentalEffects", 1),
//	        supportedComponents -> get ("EventUtilities",       1),
//	        supportedComponents -> get ("Texturing3D",          2),
//	        supportedComponents -> get ("VolumeRendering",      4)
//		  });

	add ("MPEG-4 interactive", "MPEG-4", {
		supportedComponents -> get ("Core",                 1),
		supportedComponents -> get ("Time",                 1),
		supportedComponents -> get ("Networking",           2),
		supportedComponents -> get ("Grouping",             2),
		supportedComponents -> get ("Rendering",            1),
		supportedComponents -> get ("Shape",                1),
		supportedComponents -> get ("Geometry3D",           2),
		supportedComponents -> get ("Lighting",             2),
		supportedComponents -> get ("Texturing",            1),
		supportedComponents -> get ("Interpolation",        2),
		supportedComponents -> get ("PointingDeviceSensor", 1),
		supportedComponents -> get ("EnvironmentalSensor",  1),
		supportedComponents -> get ("Navigation",           1),
		supportedComponents -> get ("EnvironmentalEffects", 1)
	});

	//std::clog << "\tDone creating profile index." << std::endl;
}

void
SupportedProfiles::add (const std::string & title, const std::string & name, const ComponentInfoArray & components)
{
	//std::clog << "\tAdding profile " << name << ": " << std::flush;

	add (ProfileInfoPtr (new ProfileInfo (title, name, components)));

	//std::clog << "Done." << std::endl;
}

void
SupportedProfiles::add (const ProfileInfoPtr & profile)
{
	profiles -> emplace_back (profile);
}

///  throws Error <NOT_SUPPORTED>
ProfileInfoPtr
SupportedProfiles::get (const std::string & name) const
{
	const auto iter = std::find_if (profiles -> begin (), profiles -> end (),
	[&name] (const ProfileInfoPtr & profile)
	{
		return name == profile -> getName ();
	});

	if (iter == profiles -> end ())
		throw Error <NOT_SUPPORTED> ("Unkown profile '" + name + "'");

	return ProfileInfoPtr ((*iter) -> copy (CopyType::CLONE));
}

} // X3D
} // titania
