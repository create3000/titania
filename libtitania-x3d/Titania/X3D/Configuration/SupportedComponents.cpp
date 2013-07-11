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

#include "SupportedComponents.h"

#include <iostream>

namespace titania {
namespace X3D {

SupportedComponents::SupportedComponents (X3DExecutionContext* const executionContext) :
	executionContext (executionContext),
	      components ()
{
	//std::clog << "Creating component index:" << std::endl;

	add ("CADGeometry",          2);
	add ("Core",                 2);
	add ("CubeMapTexturing",     3);
	add ("DIS",                  2);
	add ("EnvironmentalEffects", 4);
	add ("EnvironmentalSensor",  3);
	add ("EventUtilities",       1);
	add ("Followers",            1);
	add ("Geometry2D",           2);
	add ("Geometry3D",           4);
	add ("Geospatial",           2);
	add ("Grouping",             3);
	add ("H-Anim",               1);
	add ("Interpolation",        5);
	add ("KeyDeviceSensor",      2);
	add ("Layering",             1);
	add ("Layout",               2);
	add ("Lighting",             3);
	add ("Navigation",           3);
	add ("Networking",           4);
	add ("NURBS",                4);
	add ("ParticleSystems",      3);
	add ("Picking",              3);
	add ("PointingDeviceSensor", 1);
	add ("Rendering",            5);
	add ("RigidBodyPhysics",     2);
	add ("Scripting",            1);
	add ("Shaders",              1);
	add ("Shape",                4);
	add ("Sound",                1);
	add ("Text",                 1);
	add ("Texturing",            3);
	add ("Texturing3D",          2);
	add ("Time",                 2);

	//std::clog << "\tDone creating component index." << std::endl;
}

void
SupportedComponents::add (const std::string & name, const int32_t level)
{
	//std::clog << "\tAdding component " << name << ": " << std::flush;

	add (new ComponentInfo (executionContext, name, level));

	//std::clog << "Done." << std::endl;
}

void
SupportedComponents::add (const ComponentInfo* component)
{
	components .push_back (component -> getName (), component);
}

const ComponentInfo*
SupportedComponents::get (const std::string & name, const size_t level) const
throw (Error <NOT_SUPPORTED>)
{
	const ComponentInfo* component = get (name);

	if (level <= component -> getLevel ())
		return component;

	else
		throw Error <NOT_SUPPORTED> ("Component '" + name + "' at level '" + std::to_string (level) + "' is not supported.");
}

const ComponentInfo*
SupportedComponents::get (const std::string & name) const
throw (Error <NOT_SUPPORTED>)
{
	try
	{
		return components .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <NOT_SUPPORTED> ("Unkown component '" + name + "'.");
	}
}

const ComponentInfoArray &
SupportedComponents::get () const
{
	return components;
}

void
SupportedComponents::dispose ()
{
	for (const auto & component : components)
		delete component;

	components .clear ();
}

SupportedComponents::~SupportedComponents ()
{ }

} // X3D
} // titania
