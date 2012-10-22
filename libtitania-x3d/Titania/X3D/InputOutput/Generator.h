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

#ifndef __TITANIA_X3D_INPUT_OUTPUT_GENERATOR_H__
#define __TITANIA_X3D_INPUT_OUTPUT_GENERATOR_H__

#include <map>
#include <set>
#include <string>
#include <vector>

#include <Titania/Basic/InputOutput/Generator.h>

namespace titania {
namespace X3D {

class X3DFieldDefinition;
class X3DBasicNode;

class Generator :
	public basic::Generator
{
private:

	class AccessTypesIndex :
		private std::vector <std::string>
	{
	public:

		AccessTypesIndex ();

		const std::string &
		operator [ ] (const X3DFieldDefinition* const) const;

	};

	class VrmlAccessTypesIndex :
		private std::vector <std::string>
	{
	public:

		VrmlAccessTypesIndex ();

		const std::string &
		operator [ ] (const X3DFieldDefinition* const) const;

	};

	class NodeTypesIndex :
		private std::vector <std::string>
	{
	public:

		using std::vector <std::string>::operator [ ];
		using std::vector <std::string>::at;

		NodeTypesIndex ();

		const std::string &
		operator [ ] (const X3DBasicNode*) const;

	};


public:

	static void
	Style (const std::string &);

	static const std::string &
	Style () { return style; }

	static void
	CleanStyle ();

	static void
	CompactStyle ();

	static void
	TidyStyle ();

	static void
	ExpandNodes (const bool value) { expandNodes = value; }

	static bool
	ExpandNodes () { return expandNodes; }

	static void
	X3DFieldNames (const bool value) { x3dFieldNames = value; }

	static bool
	X3DFieldNames () { return x3dFieldNames; }

	static void
	X3DAccessTypes (const bool value) { x3dAccessTypes = value; }

	static bool
	X3DAccessTypes () { return x3dAccessTypes; }

	static void
	PushLevel ();

	static void
	PopLevel ();

	static bool
	ExistsNode (const X3DBasicNode*);

	static void
	AddNode (const X3DBasicNode*);

	static std::string
	GetName (const X3DBasicNode*);

	static const VrmlAccessTypesIndex VrmlAccessTypes;
	static const AccessTypesIndex     AccessTypes;
	static const NodeTypesIndex       NodeTypes;


private:

	typedef std::set <const X3DBasicNode*>              NodesSet;
	typedef std::map <const X3DBasicNode*, std::string> NewNamesMap;

	static size_t      level;
	static NodesSet    nodes;
	static NewNamesMap newNames;
	static size_t      newName;

	static bool        expandNodes;
	static std::string style;
	static bool        x3dFieldNames;
	static bool        x3dAccessTypes;

};

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
CleanStyle (std::basic_ostream <CharT, Traits> & ostream)
{
	Generator::CleanStyle ();
	return ostream;
}

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
CompactStyle (std::basic_ostream <CharT, Traits> & ostream)
{
	Generator::CompactStyle ();
	return ostream;
}

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
TidyStyle (std::basic_ostream <CharT, Traits> & ostream)
{
	Generator::TidyStyle ();
	return ostream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern template std::ostream & CleanStyle   (std::ostream &);
extern template std::ostream & CompactStyle (std::ostream &);
extern template std::ostream & TidyStyle    (std::ostream &);

} // X3D
} // titania

#endif
