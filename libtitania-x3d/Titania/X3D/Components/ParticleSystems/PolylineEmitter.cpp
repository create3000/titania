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

#include "PolylineEmitter.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/ParticleSystems/Random.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   PolylineEmitter::component      = Component ("ParticleSystems", 1);
const std::string PolylineEmitter::typeName       = "PolylineEmitter";
const std::string PolylineEmitter::containerField = "emitter";

PolylineEmitter::Fields::Fields () :
	 direction (new SFVec3f (0, 1, 0)),
	coordIndex (new MFInt32 ({ -1 })),
	     coord (new SFNode ())
{ }

PolylineEmitter::PolylineEmitter (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParticleEmitterNode (),
	                fields (),
	         polylineMapId (0),
	      polylineBufferId (0),
	           lengthMapId (0),
	        lengthBufferId (0),
	          polylineNode (new IndexedLineSet (executionContext)),
	          pointEmitter (false),
	      lengthSoFarArray (1),
	             polylines ()
{
	addType (X3DConstants::PolylineEmitter);

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "direction",      direction ());
	addField (inputOutput,    "speed",          speed ());
	addField (inputOutput,    "variation",      variation ());
	addField (initializeOnly, "mass",           mass ());
	addField (initializeOnly, "surfaceArea",    surfaceArea ());
	addField (inputOutput,    "coordIndex",     coordIndex ());
	addField (inputOutput,    "coord",          coord ());

	addChildObjects (polylineNode);

	speed ()       .setUnit (UnitCategory::SPEED);
	mass ()        .setUnit (UnitCategory::MASS);
	surfaceArea () .setUnit (UnitCategory::AREA);
}

X3DBaseNode*
PolylineEmitter::create (X3DExecutionContext* const executionContext) const
{
	return new PolylineEmitter (executionContext);
}

void
PolylineEmitter::initialize ()
{
	X3DParticleEmitterNode::initialize ();

	if (isSoftSystem ())
	{ }
	else
	{
		// Polyline map

		glGenTextures (1, &polylineMapId);
		glGenBuffers  (1, &polylineBufferId);
	
		glGenTextures (1, &lengthMapId);
		glGenBuffers  (1, &lengthBufferId);
	}

	// Setup

	coordIndex () .addInterest (polylineNode -> coordIndex ());
	coord ()      .addInterest (polylineNode -> coord ());

	polylineNode -> setPrivate (true);
	polylineNode -> coordIndex () = coordIndex ();
	polylineNode -> coord ()      = coord ();

	polylineNode -> setup ();
	polylineNode -> rebuilded () .addInterest (&PolylineEmitter::set_polyline, this);

	set_polyline ();
}

void
PolylineEmitter::setExecutionContext (X3DExecutionContext* const executionContext)
{
	polylineNode -> setExecutionContext (executionContext);

	X3DParticleEmitterNode::setExecutionContext (executionContext);
}

Box3d
PolylineEmitter::getBBox ()
{
	if (coord ())
		return polylineNode -> getBBox ();

	return Box3d ();
}

MFString
PolylineEmitter::getShaderUrl () const
{
	return { get_shader ("ParticleSystems/PolylineEmitter.vs") .str () };
}

void
PolylineEmitter::addShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	X3DParticleEmitterNode::addShaderFields (shader);

	shader -> addUserDefinedField (inputOutput, "pointEmitter", new SFBool (pointEmitter));
	shader -> addUserDefinedField (inputOutput, "direction",    new SFVec3f (normalize (direction () .getValue ())));
}

void
PolylineEmitter::setTextureBuffer (const X3DPtr <ComposedShader> & shader) const
{
	shader -> enable ();
	shader -> setTextureBuffer ("polylineMap", GL_TEXTURE_BUFFER, polylineMapId);
	shader -> setTextureBuffer ("lengthMap",   GL_TEXTURE_BUFFER, lengthMapId);
	shader -> disable ();
}

void
PolylineEmitter::setShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	try
	{
		X3DParticleEmitterNode::setShaderFields (shader);

		shader -> setField <SFBool>  ("pointEmitter", pointEmitter, true);
		shader -> setField <SFVec3f> ("direction",    normalize (direction () .getValue ()), true);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
PolylineEmitter::set_polyline ()
{
	if (isSoftSystem ())
	{
		const auto polylinesAux = polylineNode -> getPolylines ();
	
		polylines .assign (polylinesAux .cbegin (), polylinesAux .cend ());

		// Length map

		lengthSoFarArray .resize (1);

		float lengthSoFar = 0;

		for (size_t i = 0, size = polylines .size (); i < size; i += 2)
		{
			lengthSoFar += abs (polylines [i + 1] - polylines [i]);
			lengthSoFarArray .emplace_back (lengthSoFar);
		}
	}
	else
	{
		const auto polylineArrayAux = polylineNode -> getPolylines ();
	
		const std::vector <Vector3f> polylineArray (polylineArrayAux .cbegin (), polylineArrayAux .cend ());
	
		if (polylineArray .empty ())
		{
			pointEmitter = true;
	
			glBindBuffer (GL_TEXTURE_BUFFER, polylineBufferId);
			glBufferData (GL_TEXTURE_BUFFER, 0, 0, GL_STATIC_COPY);
	
			glBindBuffer (GL_TEXTURE_BUFFER, lengthBufferId);
			glBufferData (GL_TEXTURE_BUFFER, 0, 0, GL_STATIC_COPY);
		}
		else
		{
			pointEmitter = false;
	
			glBindBuffer (GL_TEXTURE_BUFFER, polylineBufferId);
			glBufferData (GL_TEXTURE_BUFFER, polylineArray .size () * sizeof (Vector3f), polylineArray .data (), GL_STATIC_COPY);
	
			// Length map
	
			float               length = 0;
			std::vector <float> lengthArray (1);
	
			for (size_t i = 0, size = polylineArray .size (); i < size; i += 2)
			{
				length += abs (polylineArray [i + 1] - polylineArray [i]);
				lengthArray .emplace_back (length);
			}
	
			glBindBuffer (GL_TEXTURE_BUFFER, lengthBufferId);
			glBufferData (GL_TEXTURE_BUFFER, lengthArray .size () * sizeof (float), lengthArray .data (), GL_STATIC_COPY);
		}
	
		// Update textures
	
		glBindTexture (GL_TEXTURE_BUFFER, polylineMapId);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, polylineBufferId);
	
		glBindTexture (GL_TEXTURE_BUFFER, lengthMapId);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, lengthBufferId);
	
		glBindTexture (GL_TEXTURE_BUFFER, 0);
		glBindBuffer (GL_TEXTURE_BUFFER, 0);
	}
}

Vector3f
PolylineEmitter::getRandomPosition () const
{
	if (polylines .empty ())
		return Vector3f ();

	// Determine index0 and weight.

	const size_t size     = lengthSoFarArray .size ();
	const float  fraction = random1 (0, 1) * lengthSoFarArray [size - 1];
	size_t       index0   = 0;
	size_t       index1   = 0;
	float        weight   = 0;

	if (size == 1 || fraction <= lengthSoFarArray [0])
	{
		index0 = 0;
		weight = 0;
	}
	else if (fraction >= lengthSoFarArray [size - 1])
	{
		index0 = size - 2;
		weight = 1;
	}
	else
	{
		const auto iter = std::upper_bound (lengthSoFarArray .cbegin (), lengthSoFarArray .cend (), fraction);

		if (iter not_eq lengthSoFarArray .cend ())
		{
			index1 = iter - lengthSoFarArray .cbegin ();
			index0 = index1 - 1;
	
			const auto key0 = lengthSoFarArray [index0];
			const auto key1 = lengthSoFarArray [index1];
	
			weight = std::clamp <float> ((fraction - key0) / (key1 - key0), 0, 1);
		}
		else
		{
			index0 = 0;
			weight = 0;
		}
	}

	// Interpolate and set position.

	index0 *= 2;
	index1  = index0 + 1;

	const auto vertex1  = polylines [index0];
	const auto vertex2  = polylines [index1];
	const auto position = vertex1 + weight * (vertex2 - vertex1);

	return position;
}

Vector3f
PolylineEmitter::getRandomVelocity () const
{
	if (direction () == Vector3f ())
		return getSphericalRandomVelocity ();

	return normalize (direction () .getValue ()) * getRandomSpeed ();
}

void
PolylineEmitter::dispose ()
{
	try
	{
		ContextLock lock (getBrowser ());

		// Polyline map

		if (polylineMapId)
			glDeleteTextures (1, &polylineMapId);

		if (polylineBufferId)
			glDeleteBuffers (1, &polylineBufferId);

		if (lengthMapId)
			glDeleteTextures (1, &lengthMapId);

		if (lengthBufferId)
			glDeleteBuffers (1, &lengthBufferId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	// Dispose base

	X3DParticleEmitterNode::dispose ();
}

} // X3D
} // titania
