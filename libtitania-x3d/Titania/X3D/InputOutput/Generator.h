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

#ifndef __TITANIA_X3D_INPUT_OUTPUT_GENERATOR_H__
#define __TITANIA_X3D_INPUT_OUTPUT_GENERATOR_H__

#include "../Bits/X3DConstants.h"
#include "../Configuration/UnitCategory.h"
#include "../InputOutput/X3DGenerator.h"
#include "../Execution/ExportedNodeIndex.h"
#include "../Execution/ImportedNodeIndex.h"
#include "../Types/String.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>

namespace titania {
namespace X3D {

class X3DFieldDefinition;
class X3DBaseNode;
class X3DExecutionContext;

struct GeneratorObjectType;

class Generator :
	public X3DGenerator
{
public:

	static
	void
	SpecificationVersion (std::ostream & ostream, const SpecificationVersionType value)
	{ get (ostream) -> specificationVersion = value; }

	static
	SpecificationVersionType
	SpecificationVersion (std::ostream & ostream)
	{ return get (ostream) -> specificationVersion; }

	static
	void
	PushScene (std::ostream & ostream, const X3DScene* const);

	static
	void
	PopScene (std::ostream & ostream);

	static
	void
	PushExecutionContext (std::ostream & ostream, const X3DExecutionContext* const);

	static
	void
	PopExecutionContext (std::ostream & ostream);

	static
	const X3DExecutionContext*
	ExecutionContext (std::ostream & ostream)
	{ return get (ostream) -> executionContextStack .back (); }

	static
	void
	EnterScope (std::ostream & ostream);

	static
	void
	LeaveScope (std::ostream & ostream);
	
	static
	void
	ExportedNodes (std::ostream & ostream, const ExportedNodeIndex &);

	static
	void
	ImportedNodes (std::ostream & ostream, const ImportedNodeIndex &);

	bool
	static
	IsSharedNode (std::ostream & ostream, const X3DBaseNode* const);

	static
	bool
	ExistsNode (std::ostream & ostream, const X3DBaseNode* const);

	static
	void
	AddNode (std::ostream & ostream, const X3DBaseNode* const);

	static
	const std::string &
	Name (std::ostream & ostream, const X3DBaseNode* const);

	static
	void
	AddImportedNode (std::ostream & ostream, const X3DBaseNode* const, const std::string &);

	static
	void
	AddRouteNode (std::ostream & ostream, const X3DBaseNode* const);

	static
	bool
	ExistsRouteNode (std::ostream & ostream, const X3DBaseNode* const);

	static
	const std::string &
	LocalName (std::ostream & ostream, const X3DBaseNode* const);

	static
	void
	PushContainerField (std::ostream & ostream, const X3DFieldDefinition* const field)
	{ get (ostream) -> containerFieldStack .emplace_back (field); }

	static
	void
	PopContainerField (std::ostream & ostream)
	{ get (ostream) -> containerFieldStack .pop_back (); }

	static
	const X3DFieldDefinition*
	ContainerField (std::ostream & ostream)
	{ return get (ostream) -> containerFieldStack .back (); }

	static
	void
	Units (std::ostream & ostream, const bool value)
	{ get (ostream) -> units = value; }

	static
	bool
	Units (std::ostream & ostream)
	{ return get (ostream) -> units; }

	static
	void
	PushUnitCategory (std::ostream & ostream, const UnitCategory unit)
	{ get (ostream) -> unitCategories .emplace_back (unit); }

	static
	void
	PopUnitCategory (std::ostream & ostream)
	{ get (ostream) -> unitCategories .pop_back (); }

	static
	UnitCategory
	Unit (std::ostream & ostream, const UnitCategory unit);

	static
	long double
	ToUnit (std::ostream & ostream, const UnitCategory unit, const long double value);

	static
	void
	MetaData (std::ostream & ostream, const bool value)
	{ get (ostream) -> metadata = value; }

	static
	bool
	MetaData (std::ostream & ostream)
	{ return get (ostream) -> metadata; }

	///  @name Destruction

	virtual
	~Generator () final override;


protected:

	///  @name Friends

	friend
	class X3DGenerator;
	
	friend
	std::ostream &
	operator << (std::ostream & ostream, const GeneratorObjectType & value);

	///  @name Construction

	static
	void
	set (std::ostream & ostream, Generator* const generator);

	static
	Generator*
	get (std::ostream & ostream);


private:

	///  @name Construction

	Generator (std::ostream & ostream);

	///  @name Operations

	const std::string &
	Name (const X3DBaseNode* const baseNode);

	bool
	needsName (const X3DBaseNode* const baseNode);

	std::string
	getUniqueName (const std::string & name);

	///  @name Destruction

	static
	void
	dispose (std::ios_base::event event, std::ios_base & stream, int index);

	///  @name Member types

	using ExecutionContextStack = std::vector <X3DExecutionContext*>;
	using NodeIdSet             = std::set <size_t>;
	using NameIndex             = std::map <std::string, const X3DBaseNode*>;
	using NameIndexByNode       = std::map <size_t, std::string>;
	using LocalNodeSet          = std::vector <NodeIdSet>;
	using ImportedNamesIndex    = std::map <size_t, std::string>;
	using FieldStack            = std::vector <const X3DFieldDefinition*>;

	///  @name Static members

	static const int index;

	///  @name Members

	const std::ios_base* const stream;
	SpecificationVersionType   specificationVersion;
	ExecutionContextStack      executionContextStack;
	size_t                     level;
	LocalNodeSet               exportedNodesIndex;
	LocalNodeSet               importedNodesIndex;
	NodeIdSet                  nodes;
	size_t                     newName;
	NameIndex                  names;
	NameIndexByNode            namesByNode;
	ImportedNamesIndex         importedNames;
	NodeIdSet                  routeNodes;
	FieldStack                 containerFieldStack;
	bool                       units;
	std::vector <UnitCategory> unitCategories;
	bool                       metadata;
	const std::string          emptyName;

};

/// Access type

inline
std::ostream &
operator << (std::ostream & ostream, const Generator::X3DAccessType & object)
{
	static const std::string initializeOnlyCharacters = "initializeOnly";
	static const std::string inputOnlyCharacters      = "inputOnly";
	static const std::string outputOnlyCharacters     = "outputOnly";
	static const std::string inputOutputCharacters    = "inputOutput";

	switch (object .accessType)
	{
		case initializeOnly: return ostream << initializeOnlyCharacters;
		case inputOnly:      return ostream << inputOnlyCharacters;
		case outputOnly:     return ostream << outputOnlyCharacters;
		case inputOutput:    return ostream << inputOutputCharacters;
	}

	return ostream;
}

inline
std::ostream &
operator << (std::ostream & ostream, const Generator::VrmlAccessType & object)
{
	static const std::string fieldCharacters        = "field";
	static const std::string eventInCharacters      = "eventIn";
	static const std::string eventOutCharacters     = "eventOut";
	static const std::string exposedFieldCharacters = "exposedField";

	switch (object .accessType)
	{
		case initializeOnly: return ostream << fieldCharacters;
		case inputOnly:      return ostream << eventInCharacters;
		case outputOnly:     return ostream << eventOutCharacters;
		case inputOutput:    return ostream << exposedFieldCharacters;
	}

	return ostream;
}

inline
std::ostream &
operator << (std::ostream & ostream, const AccessType accessType)
{
	if (Generator::SpecificationVersion (ostream) == VRML_V2_0)
		return ostream << Generator::VrmlAccessType { accessType };

	return ostream << Generator::X3DAccessType { accessType };
}

inline
std::string
to_string (const AccessType & accessType)
{
	std::ostringstream osstream;

	osstream << accessType;

	return osstream .str ();
}

// SetGenerator

struct GeneratorObjectType { std::ostream & ostream; };

///  Function to insert a Generator into an output stream.
inline
GeneratorObjectType
SetGenerator (std::ostream & ostream)
{
	return GeneratorObjectType { ostream };
}

inline
std::ostream &
operator << (std::ostream & ostream, const GeneratorObjectType & value)
{
	Generator::set (ostream, Generator::get (value .ostream));
	return ostream;
}

// XMLEncode

struct XMLEncodeObjectType { const X3DObject* const object; };

///  Function to insert a X3DObject into an output stream in X3D XML Encoding style.
inline
XMLEncodeObjectType
XMLEncode (const X3DObject & object)
{
	return XMLEncodeObjectType { &object };
}

///  Function to insert a X3DObject into an output stream in X3D XML Encoding style.
inline
XMLEncodeObjectType
XMLEncode (const X3DObject* const object)
{
	return XMLEncodeObjectType { object };
}

///  Insertion operator for X3DObject values in X3D XML Encoding style.
template <typename CharT, typename Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const XMLEncodeObjectType & value)
{
	value .object -> toXMLStream (ostream);
	return ostream;
}

// JSONEncode

struct JSONEncodeObjectType { const X3DObject* const object; };

///  Function to insert a X3DObject into an output stream in X3D JSON Encoding style.
inline
JSONEncodeObjectType
JSONEncode (const X3DObject & object)
{
	return JSONEncodeObjectType { &object };
}

///  Function to insert a X3DObject into an output stream in X3D JSON Encoding style.
inline
JSONEncodeObjectType
JSONEncode (const X3DObject* const object)
{
	return JSONEncodeObjectType { object };
}

///  Insertion operator for X3DObject values in X3D JSON Encoding style.
template <typename CharT, typename Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const JSONEncodeObjectType & value)
{
	value .object -> toJSONStream (ostream);
	return ostream;
}

} // X3D
} // titania

#endif
