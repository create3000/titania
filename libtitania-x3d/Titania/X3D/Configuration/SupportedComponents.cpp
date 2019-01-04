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

#include "SupportedComponents.h"

#include <Titania/String/to_string.h>
#include <iostream>

namespace titania {
namespace X3D {

SupportedComponents::SupportedComponents () :
	componentIndex (),
	    components (new ComponentInfoArray ())
{
	//std::clog << "\tCreating component index:" << std::endl;

	add ("Computer-Aided Design (CAD) model geometry", "CADGeometry",          2);
	add ("Core",                                       "Core",                 2);
	add ("Cube map environmental texturing",           "CubeMapTexturing",     3);
	add ("Distributed interactive simulation (DIS)",   "DIS",                  2);
	add ("Environmental effects",                      "EnvironmentalEffects", 4);
	add ("Environmental sensor",                       "EnvironmentalSensor",  3);
	add ("Event utilities",                            "EventUtilities",       1);
	add ("Followers",                                  "Followers",            1);
	add ("Geometry2D",                                 "Geometry2D",           2);
	add ("Geometry3D",                                 "Geometry3D",           4);
	add ("Geospatial",                                 "Geospatial",           2);
	add ("Grouping",                                   "Grouping",             3);
	add ("Humanoid animation (H-Anim)",                "H-Anim",               1);
	add ("Interpolation",                              "Interpolation",        5);
	add ("Key device sensor",                          "KeyDeviceSensor",      2);
	add ("Layering",                                   "Layering",             1);
	add ("Layout",                                     "Layout",               2);
	add ("Lighting",                                   "Lighting",             3);
	add ("Navigation",                                 "Navigation",           3);
	add ("Networking",                                 "Networking",           4);
	add ("Non-uniform Rational B-Spline (NURBS)",      "NURBS",                4);
	add ("Particle systems",                           "ParticleSystems",      3);
	add ("Picking sensor",                             "Picking",              3);
	add ("Pointing device sensor",                     "PointingDeviceSensor", 1);
	add ("Programmable shaders",                       "Shaders",              1);
	add ("Rendering",                                  "Rendering",            5);
	add ("Rigid body physics",                         "RigidBodyPhysics",     2);
	add ("Scripting",                                  "Scripting",            1);
	add ("Shape",                                      "Shape",                4);
	add ("Sound",                                      "Sound",                1);
	add ("Text",                                       "Text",                 1);
	add ("Texturing",                                  "Texturing",            3);
	add ("Texturing3D",                                "Texturing3D",          2);
	add ("Time",                                       "Time",                 2);
	add ("Volume rendering",                           "VolumeRendering",      4);

	add ("Titania",                                    "Titania",              1); // Non standard.
	add ("X_ITE",                                      "X_ITE",                1); // Non standard.

	//std::clog << "\tDone creating component index." << std::endl;
}

void
SupportedComponents::add (const std::string & title, const std::string & name, const int32_t level)
{
	//std::clog << "\t\tAdding component " << name << " : " << level << std::endl;

	const auto component = ComponentInfoPtr (new ComponentInfo (title, name, level));

	componentIndex .emplace (name, component);
	components -> emplace_back (component);
}

///  throws Error <NOT_SUPPORTED>
ComponentInfoPtr
SupportedComponents::get (const std::string & name, const size_t level) const
{
	const ComponentInfoPtr & component = get (name);

	if (level <= component -> getLevel ())
		return ComponentInfoPtr (new ComponentInfo (component -> getTitle (), name, level));

	else
		throw Error <NOT_SUPPORTED> ("Component '" + name + "' at level '" + basic::to_string (level, std::locale::classic ()) + "' is not supported.");
}

///  throws Error <NOT_SUPPORTED>
ComponentInfoPtr
SupportedComponents::get (const std::string & name) const
{
	try
	{
		return ComponentInfoPtr (componentIndex .at (name) -> copy (CopyType::CLONE));
	}
	catch (const std::out_of_range &)
	{
		throw Error <NOT_SUPPORTED> ("Unkown component '" + name + "'.");
	}
}

} // X3D
} // titania
