/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "SupportedProfiles.h"

#include <iostream>

namespace titania {
namespace X3D {

SupportedProfiles::SupportedProfiles (X3DExecutionContext* const executionContext, const SupportedComponents & supportedComponents) :
	executionContext (executionContext)
{
	std::clog << "Creating profile index:" << std::endl;

	SupportedComponents CADInterchange (executionContext);

	CADInterchange .add (supportedComponents .getComponent ("Core"));
	CADInterchange .add (supportedComponents .getComponent ("Networking"));
	CADInterchange .add (supportedComponents .getComponent ("Grouping"));
	CADInterchange .add (supportedComponents .getComponent ("Rendering"));
	CADInterchange .add (supportedComponents .getComponent ("Shape"));
	CADInterchange .add (supportedComponents .getComponent ("Lighting"));
	CADInterchange .add (supportedComponents .getComponent ("Texturing"));
	CADInterchange .add (supportedComponents .getComponent ("Navigation"));
	CADInterchange .add (supportedComponents .getComponent ("Shaders"));
	CADInterchange .add (supportedComponents .getComponent ("CADGeometry"));

	add ("CADInterchange", CADInterchange);

	SupportedComponents Core (executionContext);
	Core .add (supportedComponents .getComponent ("Core"));

	add ("Core", Core);

	SupportedComponents Full (executionContext);
	Full .add (supportedComponents .getComponent ("Core"));
	Full .add (supportedComponents .getComponent ("Time"));
	Full .add (supportedComponents .getComponent ("Networking"));
	Full .add (supportedComponents .getComponent ("Grouping"));
	Full .add (supportedComponents .getComponent ("Rendering"));
	Full .add (supportedComponents .getComponent ("Shape"));
	Full .add (supportedComponents .getComponent ("Geometry3D"));
	Full .add (supportedComponents .getComponent ("Geometry2D"));
	Full .add (supportedComponents .getComponent ("Text"));
	Full .add (supportedComponents .getComponent ("Sound"));
	Full .add (supportedComponents .getComponent ("Lighting"));
	Full .add (supportedComponents .getComponent ("Texturing"));
	Full .add (supportedComponents .getComponent ("Interpolation"));
	Full .add (supportedComponents .getComponent ("PointingDeviceSensor"));
	Full .add (supportedComponents .getComponent ("KeyDeviceSensor"));
	Full .add (supportedComponents .getComponent ("EnvironmentalSensor"));
	Full .add (supportedComponents .getComponent ("Navigation"));
	Full .add (supportedComponents .getComponent ("EnvironmentalEffects"));
	Full .add (supportedComponents .getComponent ("Geospatial"));
	Full .add (supportedComponents .getComponent ("H-Anim"));
	Full .add (supportedComponents .getComponent ("NURBS"));
	Full .add (supportedComponents .getComponent ("DIS"));
	Full .add (supportedComponents .getComponent ("Scripting"));
	Full .add (supportedComponents .getComponent ("EventUtilities"));
	Full .add (supportedComponents .getComponent ("Shaders"));
	Full .add (supportedComponents .getComponent ("CADGeometry"));
	Full .add (supportedComponents .getComponent ("Texturing3D"));
	Full .add (supportedComponents .getComponent ("CubeMapTexturing"));
	Full .add (supportedComponents .getComponent ("Layering"));
	Full .add (supportedComponents .getComponent ("Layout"));
	Full .add (supportedComponents .getComponent ("RigidBodyPhysics"));
	Full .add (supportedComponents .getComponent ("Picking"));
	Full .add (supportedComponents .getComponent ("Followers"));
	Full .add (supportedComponents .getComponent ("ParticleSystems"));

	add ("Full", Full);

	SupportedComponents Immersive (executionContext);
	Immersive .add (supportedComponents .getComponent ("Core"));
	Immersive .add (supportedComponents .getComponent ("Time"));
	Immersive .add (supportedComponents .getComponent ("Networking"));
	Immersive .add (supportedComponents .getComponent ("Grouping"));
	Immersive .add (supportedComponents .getComponent ("Rendering"));
	Immersive .add (supportedComponents .getComponent ("Shape"));
	Immersive .add (supportedComponents .getComponent ("Geometry3D"));
	Immersive .add (supportedComponents .getComponent ("Geometry2D"));
	Immersive .add (supportedComponents .getComponent ("Text"));
	Immersive .add (supportedComponents .getComponent ("Sound"));
	Immersive .add (supportedComponents .getComponent ("Lighting"));
	Immersive .add (supportedComponents .getComponent ("Texturing"));
	Immersive .add (supportedComponents .getComponent ("Interpolation"));
	Immersive .add (supportedComponents .getComponent ("PointingDeviceSensor"));
	Immersive .add (supportedComponents .getComponent ("KeyDeviceSensor"));
	Immersive .add (supportedComponents .getComponent ("EnvironmentalSensor"));
	Immersive .add (supportedComponents .getComponent ("Navigation"));
	Immersive .add (supportedComponents .getComponent ("EnvironmentalEffects"));
	Immersive .add (supportedComponents .getComponent ("Scripting"));
	Immersive .add (supportedComponents .getComponent ("EventUtilities"));

	add ("Immersive", Immersive);

	SupportedComponents Interactive (executionContext);
	Interactive .add (supportedComponents .getComponent ("Core"));
	Interactive .add (supportedComponents .getComponent ("Time"));
	Interactive .add (supportedComponents .getComponent ("Networking"));
	Interactive .add (supportedComponents .getComponent ("Grouping"));
	Interactive .add (supportedComponents .getComponent ("Rendering"));
	Interactive .add (supportedComponents .getComponent ("Shape"));
	Interactive .add (supportedComponents .getComponent ("Geometry3D"));
	Interactive .add (supportedComponents .getComponent ("Lighting"));
	Interactive .add (supportedComponents .getComponent ("Texturing"));
	Interactive .add (supportedComponents .getComponent ("Interpolation"));
	Interactive .add (supportedComponents .getComponent ("PointingDeviceSensor"));
	Interactive .add (supportedComponents .getComponent ("KeyDeviceSensor"));
	Interactive .add (supportedComponents .getComponent ("EnvironmentalSensor"));
	Interactive .add (supportedComponents .getComponent ("Navigation"));
	Interactive .add (supportedComponents .getComponent ("EnvironmentalEffects"));
	Interactive .add (supportedComponents .getComponent ("EventUtilities"));

	add ("Interactive", Interactive);

	SupportedComponents Interchange (executionContext);
	Interchange .add (supportedComponents .getComponent ("Core"));
	Interchange .add (supportedComponents .getComponent ("Time"));
	Interchange .add (supportedComponents .getComponent ("Networking"));
	Interchange .add (supportedComponents .getComponent ("Grouping"));
	Interchange .add (supportedComponents .getComponent ("Rendering"));
	Interchange .add (supportedComponents .getComponent ("Shape"));
	Interchange .add (supportedComponents .getComponent ("Geometry3D"));
	Interchange .add (supportedComponents .getComponent ("Lighting"));
	Interchange .add (supportedComponents .getComponent ("Texturing"));
	Interchange .add (supportedComponents .getComponent ("Interpolation"));
	Interchange .add (supportedComponents .getComponent ("Navigation"));
	Interchange .add (supportedComponents .getComponent ("EnvironmentalEffects"));

	add ("Interchange", Interchange);

	SupportedComponents MPEG4 (executionContext);
	MPEG4 .add (supportedComponents .getComponent ("Core"));
	MPEG4 .add (supportedComponents .getComponent ("Time"));
	MPEG4 .add (supportedComponents .getComponent ("Networking"));
	MPEG4 .add (supportedComponents .getComponent ("Grouping"));
	MPEG4 .add (supportedComponents .getComponent ("Rendering"));
	MPEG4 .add (supportedComponents .getComponent ("Shape"));
	MPEG4 .add (supportedComponents .getComponent ("Geometry3D"));
	MPEG4 .add (supportedComponents .getComponent ("Lighting"));
	MPEG4 .add (supportedComponents .getComponent ("Texturing"));
	MPEG4 .add (supportedComponents .getComponent ("Interpolation"));
	MPEG4 .add (supportedComponents .getComponent ("PointingDeviceSensor"));
	MPEG4 .add (supportedComponents .getComponent ("EnvironmentalSensor"));
	MPEG4 .add (supportedComponents .getComponent ("Navigation"));
	MPEG4 .add (supportedComponents .getComponent ("EnvironmentalEffects"));

	add ("MPEG-4", MPEG4);

	std::clog << "\tDone creating profile index." << std::endl;
}

void
SupportedProfiles::add (const std::string & name, const SupportedComponents & supportedComponents)
{
	//std::clog << "\tAdding profile " << name << ": " << std::flush;

	add (new ProfileInfo (executionContext, name, supportedComponents .getComponents ()));

	//std::clog << "Done." << std::endl;
}

void
SupportedProfiles::add (const ProfileInfo* profile)
{
	profiles .push_back (profile -> getName (), profile);
}

const ProfileInfo*
SupportedProfiles::getProfile (const std::string & name) const
throw (Error <NOT_SUPPORTED>)
{
	try
	{
		return profiles .last (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <NOT_SUPPORTED> ("Unkown profile '" + name + "'");
	}
}

const ProfileInfoArray &
SupportedProfiles::getProfiles () const
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
