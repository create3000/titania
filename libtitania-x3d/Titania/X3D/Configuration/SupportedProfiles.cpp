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

#include "SupportedProfiles.h"

#include <iostream>

namespace titania {
namespace X3D {

SupportedProfiles::SupportedProfiles (X3DExecutionContext* const executionContext, const SupportedComponents & supportedComponents) :
	executionContext (executionContext) 
{
	//std::clog << "Creating profile index:" << std::endl;

	SupportedComponents CADInterchange (executionContext);

	CADInterchange .add (supportedComponents .get ("Core"));
	CADInterchange .add (supportedComponents .get ("Networking"));
	CADInterchange .add (supportedComponents .get ("Grouping"));
	CADInterchange .add (supportedComponents .get ("Rendering"));
	CADInterchange .add (supportedComponents .get ("Shape"));
	CADInterchange .add (supportedComponents .get ("Lighting"));
	CADInterchange .add (supportedComponents .get ("Texturing"));
	CADInterchange .add (supportedComponents .get ("Navigation"));
	CADInterchange .add (supportedComponents .get ("Shaders"));
	CADInterchange .add (supportedComponents .get ("CADGeometry"));

	add ("CADInterchange", CADInterchange);

	SupportedComponents Core (executionContext);
	Core .add (supportedComponents .get ("Core"));

	add ("Core", Core);

	SupportedComponents Full (executionContext);
	Full .add (supportedComponents .get ("Core"));
	Full .add (supportedComponents .get ("Time"));
	Full .add (supportedComponents .get ("Networking"));
	Full .add (supportedComponents .get ("Grouping"));
	Full .add (supportedComponents .get ("Rendering"));
	Full .add (supportedComponents .get ("Shape"));
	Full .add (supportedComponents .get ("Geometry3D"));
	Full .add (supportedComponents .get ("Geometry2D"));
	Full .add (supportedComponents .get ("Text"));
	Full .add (supportedComponents .get ("Sound"));
	Full .add (supportedComponents .get ("Lighting"));
	Full .add (supportedComponents .get ("Texturing"));
	Full .add (supportedComponents .get ("Interpolation"));
	Full .add (supportedComponents .get ("PointingDeviceSensor"));
	Full .add (supportedComponents .get ("KeyDeviceSensor"));
	Full .add (supportedComponents .get ("EnvironmentalSensor"));
	Full .add (supportedComponents .get ("Navigation"));
	Full .add (supportedComponents .get ("EnvironmentalEffects"));
	Full .add (supportedComponents .get ("Geospatial"));
	Full .add (supportedComponents .get ("H-Anim"));
	Full .add (supportedComponents .get ("NURBS"));
	Full .add (supportedComponents .get ("DIS"));
	Full .add (supportedComponents .get ("Scripting"));
	Full .add (supportedComponents .get ("EventUtilities"));
	Full .add (supportedComponents .get ("Shaders"));
	Full .add (supportedComponents .get ("CADGeometry"));
	Full .add (supportedComponents .get ("Texturing3D"));
	Full .add (supportedComponents .get ("CubeMapTexturing"));
	Full .add (supportedComponents .get ("Layering"));
	Full .add (supportedComponents .get ("Layout"));
	Full .add (supportedComponents .get ("RigidBodyPhysics"));
	Full .add (supportedComponents .get ("Picking"));
	Full .add (supportedComponents .get ("Followers"));
	Full .add (supportedComponents .get ("ParticleSystems"));

	add ("Full", Full);

	SupportedComponents Immersive (executionContext);
	Immersive .add (supportedComponents .get ("Core"));
	Immersive .add (supportedComponents .get ("Time"));
	Immersive .add (supportedComponents .get ("Networking"));
	Immersive .add (supportedComponents .get ("Grouping"));
	Immersive .add (supportedComponents .get ("Rendering"));
	Immersive .add (supportedComponents .get ("Shape"));
	Immersive .add (supportedComponents .get ("Geometry3D"));
	Immersive .add (supportedComponents .get ("Geometry2D"));
	Immersive .add (supportedComponents .get ("Text"));
	Immersive .add (supportedComponents .get ("Sound"));
	Immersive .add (supportedComponents .get ("Lighting"));
	Immersive .add (supportedComponents .get ("Texturing"));
	Immersive .add (supportedComponents .get ("Interpolation"));
	Immersive .add (supportedComponents .get ("PointingDeviceSensor"));
	Immersive .add (supportedComponents .get ("KeyDeviceSensor"));
	Immersive .add (supportedComponents .get ("EnvironmentalSensor"));
	Immersive .add (supportedComponents .get ("Navigation"));
	Immersive .add (supportedComponents .get ("EnvironmentalEffects"));
	Immersive .add (supportedComponents .get ("Scripting"));
	Immersive .add (supportedComponents .get ("EventUtilities"));

	add ("Immersive", Immersive);

	SupportedComponents Interactive (executionContext);
	Interactive .add (supportedComponents .get ("Core"));
	Interactive .add (supportedComponents .get ("Time"));
	Interactive .add (supportedComponents .get ("Networking"));
	Interactive .add (supportedComponents .get ("Grouping"));
	Interactive .add (supportedComponents .get ("Rendering"));
	Interactive .add (supportedComponents .get ("Shape"));
	Interactive .add (supportedComponents .get ("Geometry3D"));
	Interactive .add (supportedComponents .get ("Lighting"));
	Interactive .add (supportedComponents .get ("Texturing"));
	Interactive .add (supportedComponents .get ("Interpolation"));
	Interactive .add (supportedComponents .get ("PointingDeviceSensor"));
	Interactive .add (supportedComponents .get ("KeyDeviceSensor"));
	Interactive .add (supportedComponents .get ("EnvironmentalSensor"));
	Interactive .add (supportedComponents .get ("Navigation"));
	Interactive .add (supportedComponents .get ("EnvironmentalEffects"));
	Interactive .add (supportedComponents .get ("EventUtilities"));

	add ("Interactive", Interactive);

	SupportedComponents Interchange (executionContext);
	Interchange .add (supportedComponents .get ("Core"));
	Interchange .add (supportedComponents .get ("Time"));
	Interchange .add (supportedComponents .get ("Networking"));
	Interchange .add (supportedComponents .get ("Grouping"));
	Interchange .add (supportedComponents .get ("Rendering"));
	Interchange .add (supportedComponents .get ("Shape"));
	Interchange .add (supportedComponents .get ("Geometry3D"));
	Interchange .add (supportedComponents .get ("Lighting"));
	Interchange .add (supportedComponents .get ("Texturing"));
	Interchange .add (supportedComponents .get ("Interpolation"));
	Interchange .add (supportedComponents .get ("Navigation"));
	Interchange .add (supportedComponents .get ("EnvironmentalEffects"));

	add ("Interchange", Interchange);

	SupportedComponents MPEG4 (executionContext);
	MPEG4 .add (supportedComponents .get ("Core"));
	MPEG4 .add (supportedComponents .get ("Time"));
	MPEG4 .add (supportedComponents .get ("Networking"));
	MPEG4 .add (supportedComponents .get ("Grouping"));
	MPEG4 .add (supportedComponents .get ("Rendering"));
	MPEG4 .add (supportedComponents .get ("Shape"));
	MPEG4 .add (supportedComponents .get ("Geometry3D"));
	MPEG4 .add (supportedComponents .get ("Lighting"));
	MPEG4 .add (supportedComponents .get ("Texturing"));
	MPEG4 .add (supportedComponents .get ("Interpolation"));
	MPEG4 .add (supportedComponents .get ("PointingDeviceSensor"));
	MPEG4 .add (supportedComponents .get ("EnvironmentalSensor"));
	MPEG4 .add (supportedComponents .get ("Navigation"));
	MPEG4 .add (supportedComponents .get ("EnvironmentalEffects"));

	add ("MPEG-4", MPEG4);

	//std::clog << "\tDone creating profile index." << std::endl;
}

void
SupportedProfiles::add (const std::string & name, const SupportedComponents & supportedComponents)
{
	//std::clog << "\tAdding profile " << name << ": " << std::flush;

	add (new ProfileInfo (executionContext, name, supportedComponents .get ()));

	//std::clog << "Done." << std::endl;
}

void
SupportedProfiles::add (const ProfileInfo* profile)
{
	profiles .push_back (profile -> getName (), profile);
}

const ProfileInfo*
SupportedProfiles::get (const std::string & name) const
throw (Error <NOT_SUPPORTED>)
{
	try
	{
		return profiles .find_last (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <NOT_SUPPORTED> ("Unkown profile '" + name + "'");
	}
}

const ProfileInfoArray &
SupportedProfiles::get () const
{
	return profiles;
}

void
SupportedProfiles::dispose ()
{
	for (const auto & profile : profiles)
		delete profile;

	profiles .clear ();
}

SupportedProfiles::~SupportedProfiles ()
{ }

} // X3D
} // titania
