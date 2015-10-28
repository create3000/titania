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

#ifndef __TITANIA_X3D_INPUT_OUTPUT_GENERATOR_H__
#define __TITANIA_X3D_INPUT_OUTPUT_GENERATOR_H__

#include "../Basic/X3DConstants.h"
#include "../InputOutput/X3DBaseGenerator.h"
#include "../Execution/ExportedNodeIndex.h"
#include "../Execution/ImportedNodeIndex.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace titania {
namespace X3D {

class X3DFieldDefinition;
class X3DBaseNode;
class X3DExecutionContext;

class Generator :
	public X3DGenerator
{
public:

	enum StyleType : uint8_t
	{
		SMALLEST,
		SMALL,
		COMPACT,
		NICEST
	};

	class AccessTypesIndex
	{
	public:

		AccessTypesIndex ();

		const std::string &
		operator [ ] (const X3DFieldDefinition* const) const;

	};

	class X3DAccessTypesIndex
	{
	public:

		X3DAccessTypesIndex ();

		const std::string &
		operator [ ] (const X3DFieldDefinition* const) const;


	private:

		const std::vector <std::string> array;
	};

	class VrmlAccessTypesIndex
	{
	public:

		VrmlAccessTypesIndex ();

		const std::string &
		operator [ ] (const X3DFieldDefinition* const) const;


	private:

		const std::vector <std::string> array;

	};

	class NodeTypeIndex
	{
	public:

		NodeTypeIndex ();

		const std::string &
		operator [ ] (const X3DBaseNode* const) const;

		const std::string &
		operator [ ] (const X3DConstants::NodeType index) const
		{ return array [index]; }


	private:

		const std::vector <std::string> array;

	};

	///  @name Member access

	static
	void
	Style (const std::string &);

	static
	StyleType
	Style ()
	{ return style; }

	static
	void
	SmallestStyle ();

	static
	void
	SmallStyle ();

	static
	void
	CompactStyle ();

	static
	void
	NicestStyle ();

	static
	void
	SpecificationVersion (const SpecificationVersionType value)
	{ specificationVersion = value; }

	static
	SpecificationVersionType
	SpecificationVersion ()
	{ return specificationVersion; }

	static
	void
	PushExecutionContext (const X3DExecutionContext* const);

	static
	void
	PopExecutionContext ();

	static
	const X3DExecutionContext*
	ExecutionContext ()
	{ return executionContextStack .back (); }

	static
	void
	EnterScope ();

	static
	void
	LeaveScope ();
	
	static
	void
	ExportedNodes (const ExportedNodeIndex &);

	static
	void
	ImportedNodes (const ImportedNodeIndex &);

	bool
	static
	IsSharedNode (const X3DBaseNode* const);

	static
	bool
	ExistsNode (const X3DBaseNode* const);

	static
	void
	AddNode (const X3DBaseNode* const);

	static
	const std::string &
	Name (const X3DBaseNode* const);

	static
	void
	AddImportedNode (const X3DBaseNode* const, const std::string &);

	static
	const std::string &
	LocalName (const X3DBaseNode* const);

	static
	void
	PushContainerField (const X3DFieldDefinition* const field)
	{ containerFieldStack .emplace_back (field); }

	static
	void
	PopContainerField ()
	{ containerFieldStack .pop_back (); }

	static
	const X3DFieldDefinition*
	ContainerField ()
	{ return containerFieldStack .back (); }

	static
	void
	XMLEncode (std::ostream &, const std::string &);

	/// @name Static members

	static const AccessTypesIndex     AccessTypes;
	static const X3DAccessTypesIndex  X3DAccessTypes;
	static const VrmlAccessTypesIndex VrmlAccessTypes;
	static const NodeTypeIndex   NodeTypes;


private:

	static
	bool
	needsName (const X3DBaseNode* const);

	static
	std::string
	getUniqueName ();

	using ExecutionContextStack = std::vector <const X3DExecutionContext*>;
	using NodeSet               = std::set <const X3DBaseNode*>;
	using NameIndex             = std::map <std::string, const X3DBaseNode*>;
	using NameIndexByNode       = std::map <const X3DBaseNode*, std::string>;
	using LocalNodeSet          = std::map <const X3DExecutionContext*, NodeSet>;
	using ImportedNamesIndex    = std::map <const X3DBaseNode*, std::string>;
	using FieldStack            = std::vector <const X3DFieldDefinition*>;

	static StyleType                style;
	static SpecificationVersionType specificationVersion;

	static ExecutionContextStack executionContextStack;
	static size_t                level;
	static LocalNodeSet          exportedNodesIndex;
	static LocalNodeSet          importedNodesIndex;
	static NodeSet               nodes;
	static NameIndex             names;
	static NameIndexByNode       namesByNode;
	static size_t                newName;
	static ImportedNamesIndex    importedNames;
	static FieldStack            containerFieldStack;

};

template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
SmallestStyle (std::basic_ostream <CharT, Traits> & ostream)
{
	Generator::SmallestStyle ();
	return ostream;
}

template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
SmallStyle (std::basic_ostream <CharT, Traits> & ostream)
{
	Generator::SmallStyle ();
	return ostream;
}

template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
CompactStyle (std::basic_ostream <CharT, Traits> & ostream)
{
	Generator::CompactStyle ();
	return ostream;
}

template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
NicestStyle (std::basic_ostream <CharT, Traits> & ostream)
{
	Generator::NicestStyle ();
	return ostream;
}

// XMLEncode

struct XMLEncodeStringType { const std::string & string; };

inline
XMLEncodeStringType
XMLEncode (const std::string & string)
{
	return XMLEncodeStringType { string };
}

template <typename CharT, typename Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const XMLEncodeStringType & value)
{
	Generator::XMLEncode (ostream, value .string);
	return ostream;
}

} // X3D
} // titania

#endif
