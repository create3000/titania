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
	             fields ()                                                     
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

	keyValue () .addInterest (this, &SplinePositionInterpolator::set_keyValue);
}

void
SplinePositionInterpolator::set_keyValue ()
{
	if (keyValue () .size () < key () .size ())
		keyValue () .resize (key () .size (), keyValue () .size () ? keyValue () .back () : SFVec3f ());
}

void
SplinePositionInterpolator::interpolate (size_t index0, size_t index1, float weight)
{
	std::vector <Vector3f> T, T0, T1;
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

	//

	Vector4f S (std::pow (weight, 3), math::sqr (weight), weight, 1);

	Matrix4f H (2, -2,  1,  1,
	            -3,  3, -2, -1,
	            0,  0,  1,  0,
	            1,  0,  0,  0);

	Vector3f C [4] = { keyValue () [index0], keyValue () [index1], T0 [index0], T1 [index1] };

	Vector4f SH = S * H;

	value_changed () = Vector3f (SH [0] * C [0] [0] + SH [1] * C [1] [0] + SH [2] * C [2] [0] + SH [3] * C [3] [0],
	                             SH [0] * C [0] [1] + SH [1] * C [1] [1] + SH [2] * C [2] [1] + SH [3] * C [3] [1],
	                             SH [0] * C [0] [2] + SH [1] * C [1] [2] + SH [2] * C [2] [2] + SH [3] * C [3] [2]);

	//	{
	//		std::vector <Vector3f> T;
	//		std::vector <float>    F1;
	//		std::vector <float>    F2;
	//		Vector3f               T0;
	//		Vector3f               T1;
	//
	//		if (index0 >= 0 && (index0 + 1) < keyValue () .size ())
	//		{
	//
	//			int   tMinus1      = (key () .size () - 2);
	//			int   tN           = 1;
	//			float D            = 0.0;
	//			bool  specified    = false;
	//			bool  notSpecified = false;
	//			bool  ignoreClosed = false;
	//
	//			//if the first and last value in keyValue aren't the same, ignore closed
	//			if ((math::norm (keyValue () [0] - keyValue () [(key () .size () - 1)])) > 0.00001)
	//			{
	//				ignoreClosed = true;
	//			}
	//
	//			//Calculate  D = distance from keyValue[0] -> keyValue[N-1]
	//			for (size_t i = 0; i < key () .size () - 1; i ++)  //n=keys.size()
	//			{
	//				D = D + (math::abs (keyValue () [i] - keyValue () [i + 1]));
	//			}
	//
	//			// Calculate T for the different conditions
	//			if (keyVelocity ().size () == keyValue () .size ())
	//			{
	//				if (! normalizeVelocity ())
	//				{
	//					T .assign (keyVelocity () .begin (), keyVelocity () .end ());
	//				}
	//				else if (normalizeVelocity ())
	//				{
	//					Vector3f a;
	//
	//					for (size_t i = 0; i < key () .size (); i ++)
	//					{
	//						a = keyVelocity () [i];
	//						T .push_back (keyVelocity () [i] * (D / math::abs (a)));
	//					}
	//				}
	//
	//				specified = true;
	//			}
	//
	//			else if (closed () && ! ignoreClosed)
	//			{
	//				for (size_t i = 0; i < key () .size (); i ++)
	//				{
	//					if (i == 0)
	//					{
	//						T .push_back ((keyValue () [i + 1] - keyValue () [tMinus1]) / 2.0f);
	//					}
	//					else if (i == (key () .size () - 1))
	//					{
	//						T .push_back ((keyValue () [tN] - keyValue () [i - 1]) / 2.0f);
	//					}
	//					else
	//					{
	//						T .push_back ((keyValue () [i + 1] - keyValue () [i - 1]) / 2.0f);
	//					}
	//				}
	//			}
	//
	//			else if (! closed () or ignoreClosed)
	//			{
	//				T .push_back (Vector3f (0, 0, 0));
	//
	//				for (size_t i = 1; i < key () .size () - 1; i ++)
	//				{
	//					T .push_back ((keyValue () [i + 1] - keyValue () [i - 1]) / 2.0f);
	//				}
	//
	//				notSpecified = true;
	//			}
	//
	//			if (keyVelocity ().size () == 2)
	//			{
	//				if (! normalizeVelocity ())
	//				{
	//					T [0] = keyVelocity () [0];
	//
	//					if (notSpecified)
	//					{
	//						T .push_back (keyVelocity () [1]);
	//					}
	//					else
	//					{
	//						T [key () .size () - 1] = keyVelocity () [1];
	//					}
	//				}
	//				else if (normalizeVelocity ())
	//				{
	//					Vector3f a = keyVelocity () [0];
	//					T [0] = (keyVelocity () [0] * (D / (math::abs (a))));
	//
	//					if (notSpecified)
	//					{
	//						Vector3f b = keyVelocity () [1];
	//						T .push_back (keyVelocity () [1] * (D / math::abs (b)));
	//					}
	//					else
	//					{
	//						Vector3f b = keyVelocity () [1];
	//						T [(key () .size () - 1)] = (keyVelocity () [1] * (D / math::abs (b)));
	//					}
	//				}
	//
	//				specified = true;
	//			}     // Calculations for T are ready
	//
	//			//Calculate F1, F2, T0, T1
	//			if (! closed () or specified or ignoreClosed)
	//			{
	//				for (size_t i = 1; i < key () .size () - 1; i ++)
	//				{
	//					F1 .push_back (2 * ((key () [i] - key () [i - 1]) / (key () [i + 1] - key () [i - 1])));
	//					F2 .push_back (2 * ((key () [i + 1] - key () [i]) / (key () [i + 1] - key () [i - 1])));
	//				}
	//
	//				if ((index0 > 0) && (index0 < key () .size () - 2))
	//				{
	//					T0 = (T [index0] * F1 [index0 - 1]);
	//					T1 = (F2 [index0] * T [index0 + 1]);
	//				}
	//				else if (specified && (index0 == 0))
	//				{
	//					T0 = T [0];
	//					T1 = (F2 [index0] * T [index0 + 1]);
	//				}
	//				else if (notSpecified && (index0 == 0))
	//				{
	//					T0 = Vector3f (0, 0, 0);
	//					T1 = (F2 [index0] * T [index0 + 1]);
	//				}
	//				else if (specified && (index0 == (key () .size () - 2)))
	//				{
	//					T0 = keyValue () [key () .size () - 1];
	//					T1 = T [key () .size () - 1];
	//				}
	//				else if (notSpecified && (index0 == (key () .size () - 1)))
	//				{
	//					T0 = (F1 [index0 - 1] * T [index0]);
	//					T1 = Vector3f (0, 0, 0);
	//				}
	//			}
	//
	//			else if (closed () && ! ignoreClosed)
	//			{
	//				for (size_t i = 0; i < key () .size (); i ++)
	//				{
	//					if (i == 0)
	//					{
	//						F1 .push_back (2 * ((key () [i] - key () [tMinus1]) / (key () [i + 1] - key () [tMinus1])));
	//						F2 .push_back (2 * ((key () [i + 1] - key () [i]) / (key () [i + 1] - key () [tMinus1])));
	//					}
	//					else if (i == (key () .size () - 1))
	//					{
	//						F1 .push_back ((key () [i] - key () [i - 1]) / (key () [tN] - key () [i - 1]));
	//						F2 .push_back (2 * ((key () [tN] - key () [i]) / (key () [tN] - key () [i - 1])));
	//					}
	//					else
	//					{
	//						F1 .push_back (2 * ((key () [i] - key () [i - 1]) / (key () [i + 1] - key () [i - 1])));
	//						F2 .push_back (2 * ((key () [i + 1] - key () [i]) / (key () [i + 1] - key () [i - 1])));
	//					}
	//				}
	//
	//				T0 = (F1 [index0] * T [index0]);
	//				T1 = (F2 [index0 + 1] * T [index0 + 1]);
	//			}
	//
	//			std::vector <Vector3f> C;
	//			C .push_back (keyValue () [index0]);
	//			C .push_back (keyValue () [index0 + 1]);
	//			C .push_back (T0);
	//			C .push_back (T1);
	//
	//			Vector4f S = Vector4f (std::pow (weight, 3), std::pow (weight, 2), weight, 1);
	//
	//			Matrix4f H = Matrix4f (2, -2, 1, 1, -3, 3, -2, -1, 0, 0, 1, 0, 1, 0, 0, 0);
	//
	//			Vector3f HC0 = Vector3f ((H [0] [0] * C [0] [0] + H [0] [1] * C [1] [0] + H [0] [2] * C [2] [0] + H [0] [3] * C [3] [0]), (H [0] [0] * C [0] [1] + H [0] [1] * C [1] [1] + H [0] [2] * C [2] [1] + H [0] [3] * C [3] [1]), (H [0] [0] * C [0] [2] + H [0] [1] * C [1] [2] + H [0] [2] * C [2] [2] + H [0] [3] * C [3] [2]));
	//			Vector3f HC1 = Vector3f ((H [1] [0] * C [0] [0] + H [1] [1] * C [1] [0] + H [1] [2] * C [2] [0] + H [1] [3] * C [3] [0]), (H [1] [0] * C [0] [1] + H [1] [1] * C [1] [1] + H [1] [2] * C [2] [1] + H [1] [3] * C [3] [1]), (H [1] [0] * C [0] [2] + H [1] [1] * C [1] [2] + H [1] [2] * C [2] [2] + H [1] [3] * C [3] [2]));
	//			Vector3f HC2 = Vector3f ((H [2] [0] * C [0] [0] + H [2] [1] * C [1] [0] + H [2] [2] * C [2] [0] + H [2] [3] * C [3] [0]), (H [2] [0] * C [0] [1] + H [2] [1] * C [1] [1] + H [2] [2] * C [2] [1] + H [2] [3] * C [3] [1]), (H [2] [0] * C [0] [2] + H [2] [1] * C [1] [2] + H [2] [2] * C [2] [2] + H [2] [3] * C [3] [2]));
	//			Vector3f HC3 = Vector3f ((H [3] [0] * C [0] [0] + H [3] [1] * C [1] [0] + H [3] [2] * C [2] [0] + H [3] [3] * C [3] [0]), (H [3] [0] * C [0] [1] + H [3] [1] * C [1] [1] + H [3] [2] * C [2] [1] + H [3] [3] * C [3] [1]), (H [3] [0] * C [0] [2] + H [3] [1] * C [1] [2] + H [3] [2] * C [2] [2] + H [3] [3] * C [3] [2]));
	//
	//			std::vector <Vector3f> HC;
	//			HC .push_back (HC0);
	//			HC .push_back (HC1);
	//			HC .push_back (HC2);
	//			HC .push_back (HC3);
	//
	//			float HCS0 = (HC [0] [0] * S [0] + HC [1] [0] * S [1] + HC [2] [0] * S [2] + HC [3] [0] * S [3]);
	//			float HCS1 = (HC [0] [1] * S [0] + HC [1] [1] * S [1] + HC [2] [1] * S [2] + HC [3] [1] * S [3]);
	//			float HCS2 = (HC [0] [2] * S [0] + HC [1] [2] * S [1] + HC [2] [2] * S [2] + HC [3] [2] * S [3]);
	//
	//			value_changed () = Vector3f (HCS0, HCS1, HCS2);
	//		}
	//	}
}

} // X3D
} // titania
