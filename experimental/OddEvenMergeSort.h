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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <Titania/Algorithm/Remove.h>
#include <Titania/Basic/Path.h>
#include <Titania/Basic/URI.h>
#include <Titania/Chrono/Now.h>
#include <Titania/InputOutput.h>
#include <Titania/Math/Functional.h>
#include <Titania/Math/Geometry/Box2.h>
#include <Titania/Math/Geometry/Box3.h>
#include <Titania/Math/Geometry/Cylinder3.h>
#include <Titania/Math/Geometry/Line3.h>
#include <Titania/Math/Geometry/Plane3.h>
#include <Titania/Math/Numbers/Matrix3.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Math/Numbers/Rotation4.h>
#include <Titania/Math/Numbers/Vector2.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>
#include <Titania/OS.h>
#include <Titania/Stream/InputFileStream.h>
#include <Titania/Stream/InputUrlStream.h>

namespace sfrdd {

typedef titania::math::quaternion <float> Quaternionf;
typedef titania::math::vector2 <float>    Vector2f;
typedef titania::math::vector3 <float>    Vector3f;
typedef titania::math::vector4 <float>    Vector4f;
typedef titania::math::rotation4 <float>  Rotation4f;
typedef titania::math::matrix4 <float>    Matrix4f;
typedef titania::math::box2 <float>       Box2f;
typedef titania::math::box3 <float>       Box3f;
typedef titania::math::cylinder3 <float>  Cylinder3f;
typedef titania::math::plane3 <float>     Plane3f;
typedef titania::math::line3 <float>      Line3f;
typedef titania::math::sphere3 <float>    Sphere3f;
typedef titania::math::matrix4 <float>    Matrix4f;

struct Particle
{
	///  @name Construction

	Particle () :
		   lifetime (-1),
		   position (),
		   velocity (),
		elapsedTime (0),
		   distance (0)
	{ }

	///  @name Members

	float lifetime;
	Vector3f position;
	Vector3f velocity;
	float elapsedTime;
	float distance;

};

class oems;

class OddEvenMergeSort
{
public:

	OddEvenMergeSort ();

	void
	sortStep (oems* const, const int32_t);

	~OddEvenMergeSort ()
	{ }


private:

	void
	sortStep (oems* const);

	void
	reset (oems* const, const int32_t);

	int32_t stepsLeft;
	int32_t pass;
	int32_t stage;
	int32_t size;

};

#define TwoStage       sortParam .x ()
#define Pass_mod_Stage sortParam .y ()
#define TwoStage_PmS_1 sortParam .z ()
#define Pass           sortParam .w ()

class oems
{
public:

	oems (int maxParticles) :
	           read (0),
	          write (1),
		   particles ()
	{
		particles .emplace_back (maxParticles);
		particles .emplace_back (maxParticles);
	}

	void
	step (int numParticles)
	{
		created = 0;
	
		sort .sortStep (this, numParticles);
	
		float deltaTime = 1 / 60.0f;
	
		for (int i = 0; i < numParticles; ++ i)
			main (i, deltaTime);

		swap ();
		
		__LOG__ << numParticles << " : " << created << std::endl;
	}

	void
	swap ()
	{
		std::swap (read, write);
	}

	void
	main (int gl_VertexID, float deltaTime)
	{
		int index = odd_even_merge_sort (gl_VertexID);

		float elapsedTime = particles [read] [index] .elapsedTime + deltaTime;

		if (elapsedTime > particles [read] [index] .lifetime)
		{
			particles [write] [gl_VertexID] .lifetime    = 40;
			particles [write] [gl_VertexID] .elapsedTime = 0.0f;
			
			++created;
		}
		else
		{
			particles [write] [gl_VertexID] .lifetime    = particles [read] [index] .lifetime;
			particles [write] [gl_VertexID] .elapsedTime = elapsedTime;
		}
	}

	int
	odd_even_merge_sort (int self)
	{
		if (self < sortSize)
		{
			// My position within the range to merge.
			float j = std::floor (std::fmod (self, TwoStage));

			if ((j < Pass_mod_Stage) || (j > TwoStage_PmS_1))
			{
				// Must copy.
				return self;
			}

			// Must sort.

			float compare = 1.0f;
			int   partner = self;

			if (std::fmod ((j + Pass_mod_Stage) / Pass, 2.0f) < 1.0f)
			{
				// We are on the left side -> compare with partner on the right.
				partner += int (Pass);
			}
			else
			{
				// We are on the right side -> compare with partner on the left.
				compare  = -1.0f;
				partner -= int (Pass);
			}

			if (partner < sortSize)
				// On the left its a < operation, on the right its a >= operation.
				return getDistance (self) * compare < getDistance (partner) * compare ? self : partner;
		}

		return self;
	}
	
	float
	getDistance (int index)
	{
		return 1.0 / index;
	}

	int read;
	int write;
	std::vector <std::vector <Particle>> particles;

	OddEvenMergeSort sort;
	int sortSize;
	Vector4f sortParam;
	
	int created;

};

OddEvenMergeSort::OddEvenMergeSort () :
	stepsLeft (0),
	     pass (0),
	    stage (0),
	     size (0)
{ }

void
OddEvenMergeSort::sortStep (oems* const shader, const int32_t currentSize)
{
	if (stepsLeft <= 0)
		reset (shader, currentSize);

	if (currentSize < size)
		shader -> sortSize = currentSize;

	sortStep (shader);
}

void
OddEvenMergeSort::sortStep (oems* const shader)
{
	// Sort pass

	-- pass;

	if (pass < 0)
	{
		// Next stage
		pass = ++ stage;
	}

	// Sort step

	const int32_t pstage = 1 << stage;
	const int32_t ppass  = 1 << pass;

	shader -> sortParam = Vector4f (pstage + pstage,
	                              ppass % pstage,
	                              (pstage + pstage) - (ppass % pstage) - 1,
	                              ppass);

	//

	-- stepsLeft;
}

void
OddEvenMergeSort::reset (oems* const shader, const int32_t currentSize)
{
	const int32_t fieldSize = titania::math::next_power_of_two (int32_t (std::ceil (std::sqrt (currentSize))));

	if (fieldSize)
	{
		const int32_t logFieldSize = std::log2 (fieldSize);

		stepsLeft = logFieldSize * (2 * logFieldSize + 1);
	}

	stage = pass = -1;

	// Shader

	shader -> sortSize = currentSize;

	size = currentSize;
}

} // titania
