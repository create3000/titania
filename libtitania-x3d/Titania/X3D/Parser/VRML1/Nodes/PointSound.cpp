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

#include "PointSound.h"

#include "../../../Execution/X3DExecutionContext.h"
#include "../Converter.h"

namespace titania {
namespace X3D {
namespace VRML1 {

// VRML 1.1

const ComponentType PointSound::component      = ComponentType::TITANIA;
const std::string   PointSound::typeName       = "PointSound";
const std::string   PointSound::containerField = "children";

PointSound::Fields::Fields () :
	       name (new X3D::MFString ({ "" })),
	description (new X3D::SFString ("")),
	  intensity (new X3D::SFFloat (1)),
	   location (new X3D::SFVec3f (0, 0, 0)),
	   minRange (new X3D::SFFloat (10)),
	   maxRange (new X3D::SFFloat (10)),
	       loop (new X3D::SFBool (false)),
	      start (new X3D::SFTime (0)),
	      pause (new X3D::SFTime (0))
{ }

PointSound::PointSound (X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       VRML1Node (),
	          fields ()
{
	addField (initializeOnly, "name", *fields .name);
	addField (initializeOnly, "description", *fields .description);
	addField (initializeOnly, "intensity", *fields .intensity);
	addField (initializeOnly, "location", *fields .location);
	addField (initializeOnly, "minRange", *fields .minRange);
	addField (initializeOnly, "maxRange", *fields .maxRange);
	addField (initializeOnly, "loop", *fields .loop);
	addField (initializeOnly, "start", *fields .start);
	addField (initializeOnly, "pause", *fields .pause);
	addField (initializeOnly, "children", children ());
}

X3D::X3DBaseNode*
PointSound::create (X3D::X3DExecutionContext* const executionContext) const
{
	return new PointSound (executionContext);
}

void
PointSound::convert (Converter* const converter)
{ }

PointSound::~PointSound ()
{ }

} // VRML1
} // X3D
} // titania
