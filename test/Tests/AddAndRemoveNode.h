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
 ******************************************************************************/

#include <Titania/X3D.h>
#include <Titania/X3D/Browser/Browser.h>

namespace Test {

using namespace titania::X3D;

inline
size_t
rand (int max)
{
	return (size_t) (max * ((float) std::rand () / (float) RAND_MAX));
}

class AddAndRemoveNode
{
public:

	static constexpr int iterations  = 1000000;
	static constexpr int vector_size = 24;

	SFNode <Scene> scene;

	SFNode <MetadataSet> metadataSet;

	int numNodes;
	int numNodesMax;

	AddAndRemoveNode () :
		numNodes (0),
		numNodesMax (0)
	{
		base ();
		basic ();
		run ();
	}

	void
	base ()
	{ }

	void
	basic ()
	{
		MFNode <X3DBaseNode> array;

		assert (array .size () == 0);

		SFNode <X3DBaseNode> node = getBrowser () -> createNode ("TimeSensor");

		std::clog << "SFNode:      " << node   .getParents () .size () << std::endl;
		std::clog << "X3DBaseNode: " << node -> getParents () .size () << std::endl;

		assert (node   .getParents () .size () == 0);
		assert (node -> getParents () .size () == 1);

		array .push_back (node);
		assert (array .size () == 1);

		std::clog << "SFNode:      " << array .back ()   .getParents () .size () << std::endl;
		std::clog << "X3DBaseNode: " << array .back () -> getParents () .size () << std::endl;

		assert (array .back ()   .getParents () .size () == 1);
		assert (array .back () -> getParents () .size () == 2);

		std::clog << "SFNode:      " << node   .getParents () .size () << std::endl;
		std::clog << "X3DBaseNode: " << node -> getParents () .size () << std::endl;

		assert (node   .getParents () .size () == 0);
		assert (node -> getParents () .size () == 2);

		array .dispose ();

		std::clog << "Array:       " << array .size () << std::endl;
		std::clog << "SFNode:      " << node   .getParents () .size () << std::endl;
		std::clog << "X3DBaseNode: " << node -> getParents () .size () << std::endl;

		assert (array .size () == 0);
		assert (node   .getParents () .size () == 0);
		assert (node -> getParents () .size () == 1);

		node .dispose ();
	}

	SFNode <X3DBaseNode>
	createNode (const std::string & name)
	{
		++ numNodes;
		return scene -> createNode (name);
	}

	void
	run ()
	{
		std::clog << __FILE__ << ':' << __LINE__ << std::endl;
		std::clog << __FILE__ << ':' << __LINE__ << (void*) getBrowser () .getValue () << std::endl;
		std::clog << __FILE__ << ':' << __LINE__ << std::endl;
		scene = getBrowser () -> createScene ();
		std::clog << __FILE__ << ':' << __LINE__ << std::endl;

		metadataSet = createNode ("MetadataSet");

		fill ();

		for (int i = 0; i < iterations; ++ i)
		{
			std::clog << vector_size << ":  " << i << "  " << numNodes << "  max (" << numNodesMax << ")" << std::endl;

			set ();
			metadata ();
			value ();
		}
	}

	void
	fill ()
	{
		for (int i = 0; i < vector_size; ++ i)
			metadataSet -> value .push_back (createNode ("MetadataSet"));

		clear ();
	}

	void
	set ()
	{
		for (int i = 0; i < vector_size; ++ i)
			metadataSet -> value [rand (vector_size - 1)] = createNode ("MetadataSet");

		clear ();
	}

	void
	metadata ()
	{
		for (int i = 0; i < vector_size; ++ i)
			SFNode <MetadataSet> (metadataSet -> value [rand (vector_size - 1)]) -> metadata = metadataSet -> value [rand (vector_size - 1)];

		clear ();
	}

	void
	value ()
	{
		for (int i = 0; i < vector_size; ++ i)
			SFNode <MetadataSet> (metadataSet -> value [rand (vector_size - 1)]) -> value .push_back (metadataSet -> value [rand (vector_size - 1)]);

		clear ();

	}

	void
	clear ()
	{
		numNodes -= getBrowser () -> getGarbageCollector () .size ();

		numNodesMax = std::max (numNodesMax, numNodes);

		getBrowser () -> getGarbageCollector () .dispose ();
	}

};

} // Test
