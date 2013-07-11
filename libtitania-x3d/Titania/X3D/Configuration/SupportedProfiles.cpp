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

	ComponentInfoArray CADInterchange;

	add ("CADInterchange", {
	        supportedComponents .get ("Core"),
	        supportedComponents .get ("Networking"),
	        supportedComponents .get ("Grouping"),
	        supportedComponents .get ("Rendering"),
	        supportedComponents .get ("Shape"),
	        supportedComponents .get ("Lighting"),
	        supportedComponents .get ("Texturing"),
	        supportedComponents .get ("Navigation"),
	        supportedComponents .get ("Shaders"),
	        supportedComponents .get ("CADGeometry")
		  });

	add ("Core", {
	        supportedComponents .get ("Core")
		  });

	add ("Full", {
	        supportedComponents .get ("Core"),
	        supportedComponents .get ("Time"),
	        supportedComponents .get ("Networking"),
	        supportedComponents .get ("Grouping"),
	        supportedComponents .get ("Rendering"),
	        supportedComponents .get ("Shape"),
	        supportedComponents .get ("Geometry3D"),
	        supportedComponents .get ("Geometry2D"),
	        supportedComponents .get ("Text"),
	        supportedComponents .get ("Sound"),
	        supportedComponents .get ("Lighting"),
	        supportedComponents .get ("Texturing"),
	        supportedComponents .get ("Interpolation"),
	        supportedComponents .get ("PointingDeviceSensor"),
	        supportedComponents .get ("KeyDeviceSensor"),
	        supportedComponents .get ("EnvironmentalSensor"),
	        supportedComponents .get ("Navigation"),
	        supportedComponents .get ("EnvironmentalEffects"),
	        supportedComponents .get ("Geospatial"),
	        supportedComponents .get ("H-Anim"),
	        supportedComponents .get ("NURBS"),
	        supportedComponents .get ("DIS"),
	        supportedComponents .get ("Scripting"),
	        supportedComponents .get ("EventUtilities"),
	        supportedComponents .get ("Shaders"),
	        supportedComponents .get ("CADGeometry"),
	        supportedComponents .get ("Texturing3D"),
	        supportedComponents .get ("CubeMapTexturing"),
	        supportedComponents .get ("Layering"),
	        supportedComponents .get ("Layout"),
	        supportedComponents .get ("RigidBodyPhysics"),
	        supportedComponents .get ("Picking"),
	        supportedComponents .get ("Followers"),
	        supportedComponents .get ("ParticleSystems")
		  });

	add ("Immersive", {
	        supportedComponents .get ("Core"),
	        supportedComponents .get ("Time"),
	        supportedComponents .get ("Networking"),
	        supportedComponents .get ("Grouping"),
	        supportedComponents .get ("Rendering"),
	        supportedComponents .get ("Shape"),
	        supportedComponents .get ("Geometry3D"),
	        supportedComponents .get ("Geometry2D"),
	        supportedComponents .get ("Text"),
	        supportedComponents .get ("Sound"),
	        supportedComponents .get ("Lighting"),
	        supportedComponents .get ("Texturing"),
	        supportedComponents .get ("Interpolation"),
	        supportedComponents .get ("PointingDeviceSensor"),
	        supportedComponents .get ("KeyDeviceSensor"),
	        supportedComponents .get ("EnvironmentalSensor"),
	        supportedComponents .get ("Navigation"),
	        supportedComponents .get ("EnvironmentalEffects"),
	        supportedComponents .get ("Scripting"),
	        supportedComponents .get ("EventUtilities")
		  });

	add ("Interactive", {
	        supportedComponents .get ("Core"),
	        supportedComponents .get ("Time"),
	        supportedComponents .get ("Networking"),
	        supportedComponents .get ("Grouping"),
	        supportedComponents .get ("Rendering"),
	        supportedComponents .get ("Shape"),
	        supportedComponents .get ("Geometry3D"),
	        supportedComponents .get ("Lighting"),
	        supportedComponents .get ("Texturing"),
	        supportedComponents .get ("Interpolation"),
	        supportedComponents .get ("PointingDeviceSensor"),
	        supportedComponents .get ("KeyDeviceSensor"),
	        supportedComponents .get ("EnvironmentalSensor"),
	        supportedComponents .get ("Navigation"),
	        supportedComponents .get ("EnvironmentalEffects"),
	        supportedComponents .get ("EventUtilities")
		  });

	add ("Interchange", {
	        supportedComponents .get ("Core"),
	        supportedComponents .get ("Time"),
	        supportedComponents .get ("Networking"),
	        supportedComponents .get ("Grouping"),
	        supportedComponents .get ("Rendering"),
	        supportedComponents .get ("Shape"),
	        supportedComponents .get ("Geometry3D"),
	        supportedComponents .get ("Lighting"),
	        supportedComponents .get ("Texturing"),
	        supportedComponents .get ("Interpolation"),
	        supportedComponents .get ("Navigation"),
	        supportedComponents .get ("EnvironmentalEffects")
		  });

	add ("MPEG-4", {
	        supportedComponents .get ("Core"),
	        supportedComponents .get ("Time"),
	        supportedComponents .get ("Networking"),
	        supportedComponents .get ("Grouping"),
	        supportedComponents .get ("Rendering"),
	        supportedComponents .get ("Shape"),
	        supportedComponents .get ("Geometry3D"),
	        supportedComponents .get ("Lighting"),
	        supportedComponents .get ("Texturing"),
	        supportedComponents .get ("Interpolation"),
	        supportedComponents .get ("PointingDeviceSensor"),
	        supportedComponents .get ("EnvironmentalSensor"),
	        supportedComponents .get ("Navigation"),
	        supportedComponents .get ("EnvironmentalEffects")
		  });

	//std::clog << "\tDone creating profile index." << std::endl;
}

void
SupportedProfiles::add (const std::string & name, std::initializer_list <const ComponentInfo*> component_list)
{
	//std::clog << "\tAdding profile " << name << ": " << std::flush;

	ComponentInfoArray components;

	for (const auto & component : component_list)
		components .push_back (component -> getName (), component);

	add (new ProfileInfo (executionContext, name, std::move (components)));

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
		return profiles .rfind (name);
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
