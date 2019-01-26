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

#include "PointEmitter.h"

#include "../../Browser/Networking/config.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   PointEmitter::component      = Component ("ParticleSystems", 1);
const std::string PointEmitter::typeName       = "PointEmitter";
const std::string PointEmitter::containerField = "emitter";

PointEmitter::Fields::Fields () :
	 position (new SFVec3f ()),
	direction (new SFVec3f (0, 1, 0))
{ }

PointEmitter::PointEmitter (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParticleEmitterNode (),
	                fields ()
{
	addType (X3DConstants::PointEmitter);

	addField (inputOutput,    "metadata",    metadata ());
	addField (inputOutput,    "position",    position ());
	addField (inputOutput,    "direction",   direction ());
	addField (inputOutput,    "speed",       speed ());
	addField (inputOutput,    "variation",   variation ());
	addField (initializeOnly, "mass",        mass ());
	addField (initializeOnly, "surfaceArea", surfaceArea ());

	position ()    .setUnit (UnitCategory::LENGTH);
	speed ()       .setUnit (UnitCategory::SPEED);
	mass ()        .setUnit (UnitCategory::MASS);
	surfaceArea () .setUnit (UnitCategory::AREA);
}

X3DBaseNode*
PointEmitter::create (X3DExecutionContext* const executionContext) const
{
	return new PointEmitter (executionContext);
}

Box3d
PointEmitter::getBBox ()
{
	return Box3d (Vector3d (1, 1, 1), position () .getValue ());
}

MFString
PointEmitter::getShaderUrl () const
{
	return { get_shader ("ParticleSystems/PointEmitter.vs") .str () };
}

void
PointEmitter::addShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	X3DParticleEmitterNode::addShaderFields (shader);

	shader -> addUserDefinedField (inputOutput, "position",  new SFVec3f (position ()));
	shader -> addUserDefinedField (inputOutput, "direction", new SFVec3f (normalize (direction () .getValue ())));
}

void
PointEmitter::setShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	try
	{
		X3DParticleEmitterNode::setShaderFields (shader);

		shader -> setField <SFVec3f> ("position",  position (), true);
		shader -> setField <SFVec3f> ("direction", normalize (direction () .getValue ()), true);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

Vector3f
PointEmitter::getRandomPosition () const
{
	return position ();
}

Vector3f
PointEmitter::getRandomVelocity () const
{
	if (direction () == Vector3f ())
		return getSphericalRandomVelocity ();

	return normalize (direction () .getValue ()) * getRandomSpeed ();
}

} // X3D
} // titania
