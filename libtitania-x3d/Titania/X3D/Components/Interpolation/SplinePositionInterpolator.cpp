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

#include "SplinePositionInterpolator.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

SplinePositionInterpolator::Fields::Fields () :
	closed (new SFBool ()),
	keyValue (new MFVec3f ()),
	keyVelocity (new MFVec3f ()),
	normalizeVelocity (new SFBool ()),
	value_changed (new SFVec3f ())
{ }

SplinePositionInterpolator::SplinePositionInterpolator (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DInterpolatorNode (),                                                    
	             fields (),                                                    
	                 T0 (),                                                    
	                 T1 ()                                                     
{
	setComponent ("Interpolation");
	setTypeName ("SplinePositionInterpolator");

	addField (inputOutput, "metadata",          metadata ());
	addField (inputOnly,   "set_fraction",      set_fraction ());
	addField (inputOutput, "closed",            closed ());
	addField (inputOutput, "key",               key ());
	addField (inputOutput, "keyValue",          keyValue ());
	addField (inputOutput, "keyVelocity",       keyVelocity ());
	addField (inputOutput, "normalizeVelocity", normalizeVelocity ());
	addField (outputOnly,  "value_changed",     value_changed ());
}

X3DBaseNode*
SplinePositionInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new SplinePositionInterpolator (executionContext);
}

void
SplinePositionInterpolator::initialize ()
{
	X3DInterpolatorNode::initialize ();

	keyValue ()    .addInterest (this, &SplinePositionInterpolator::set_keyValue);
	keyVelocity () .addInterest (this, &SplinePositionInterpolator::set_keyVelocity);

	set_keyVelocity ();
}

void
SplinePositionInterpolator::set_keyValue ()
{
	if (keyValue () .size () < key () .size ())
		keyValue () .resize (key () .size (), keyValue () .size () ? keyValue () .back () : SFVec3f ());

	set_keyVelocity ();
}

void
SplinePositionInterpolator::set_keyVelocity ()
{
	if (keyVelocity () .size ())
	{
		if (keyVelocity () .size () < key () .size ())
			keyVelocity () .resize (key () .size (), keyVelocity () .size () ? keyVelocity () .back () : SFVec3f ());
	}

	std::vector <Vector3f> T;
	std::vector <float>    Fp, Fm;

	T  .reserve (key () .size ());
	T0 .reserve (key () .size ());
	T1 .reserve (key () .size ());
	Fp .reserve (key () .size ());
	Fm .reserve (key () .size ());

	// T

	if (keyVelocity () .empty ())
	{
		if (closed ())
			T .emplace_back ((keyValue () [1] - keyValue () [keyValue () .size () - 2]) / 2.0f);

		else
			T .emplace_back ();

		for (size_t i = 1, size = keyValue () .size () - 1; i < size; ++ i)
			T .emplace_back ((keyValue () [i + 1] - keyValue () [i - 1]) / 2.0f);

		T .emplace_back (T . front ());
	}
	else
	{
		T .assign (keyVelocity () .begin (), keyVelocity () .end ());

		if (normalizeVelocity ())
		{
			float Dtot = 0;

			for (size_t i = 0, size = keyValue () .size () - 1; i < size; ++ i)
				Dtot += math::abs (keyValue () [i] - keyValue () [i + 1]);

			for (auto & Ti : T)
				Ti *= Dtot / math::abs (Ti);
		}
	}

	// Fm, Fp

	if (closed ())
	{
		size_t i_1 = key () .size () - 1;
		size_t i_2 = key () .size () - 2;

		float d = key () [1] - key () [0] + key () [i_1] - key () [i_2];

		Fm .emplace_back (2 * (key () [1] - key () [0]) / d);
		Fp .emplace_back (2 * (key () [i_1] - key () [i_2]) / d);
	}
	else
	{
		Fm .emplace_back (1);
		Fp .emplace_back (1);
	}

	for (size_t i = 1, size = key () .size () - 1; i < size; ++ i)
	{
		float d = key () [i + 1] - key () [i - 1];

		Fm .emplace_back (2 * (key () [i + 1] - key () [i]) / d);
		Fp .emplace_back (2 * (key () [i] - key () [i - 1]) / d);
	}

	Fm .emplace_back (Fm .front ());
	Fp .emplace_back (Fp .front ());

	// T0, T1

	for (size_t i = 0, size = T .size (); i < size; ++ i)
	{
		T0 .emplace_back (Fp [i] * T [i]);
		T1 .emplace_back (Fm [i] * T [i]);
	}

	T0 .shrink_to_fit ();
	T1 .shrink_to_fit ();
}

void
SplinePositionInterpolator::interpolate (size_t index0, size_t index1, float weight)
{
	Vector4f S (std::pow (weight, 3), math::sqr (weight), weight, 1);

	static Matrix4f H (2, -2,  1,  1,
	                   -3,  3, -2, -1,
	                   0,  0,  1,  0,
	                   1,  0,  0,  0);

	Vector3f C [4] = { keyValue () [index0], keyValue () [index1], T0 [index0], T1 [index1] };

	Vector4f SH = S * H;

	value_changed () = Vector3f (SH [0] * C [0] [0] + SH [1] * C [1] [0] + SH [2] * C [2] [0] + SH [3] * C [3] [0],
	                             SH [0] * C [0] [1] + SH [1] * C [1] [1] + SH [2] * C [2] [1] + SH [3] * C [3] [1],
	                             SH [0] * C [0] [2] + SH [1] * C [1] [2] + SH [2] * C [2] [2] + SH [3] * C [3] [2]);
}

} // X3D
} // titania
