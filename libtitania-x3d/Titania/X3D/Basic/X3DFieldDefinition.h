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

#ifndef __TITANIA_X3D_BASIC_X3DFIELD_DEFINITION_H__
#define __TITANIA_X3D_BASIC_X3DFIELD_DEFINITION_H__

#include "../Base/X3DChildObject.h"
#include "../Base/X3DObject.h"
#include "../Basic/FieldDefinitionArray.h"
#include "../Basic/FieldDefinitionSet.h"
#include "../Configuration/UnitCategory.h"
#include "../Routing/RouteSet.h"

#include <vector>

namespace titania {
namespace X3D {

class X3DExecutionContext;

class X3DFieldDefinition :
	public X3DChildObject
{
public:

	using X3DChildObject::addInterest;
	using X3DChildObject::removeInterest;

	///  @name Construction

	virtual
	X3DFieldDefinition*
	create () const = 0;

	virtual
	X3DFieldDefinition*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) = 0;

	virtual
	X3DFieldDefinition*
	copy (X3DExecutionContext* const, const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>)
	{ return copy (type); }

	virtual
	void
	copy (X3DExecutionContext* const, X3DFieldDefinition* const, const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) = 0;

	/// Assigns @a field to this X3DFieldDefinition. Both fields must be of the same type.
	X3DFieldDefinition &
	operator = (const X3DFieldDefinition &);

	///  @name Comparators

	/// Compares this X3DFieldDefinition with @a field. Both fields must be of the same type.
	virtual
	bool
	operator == (const X3DFieldDefinition &) const = 0;

	/// Compares this X3DFieldDefinition with @a field. Both fields must be of the same type.
	virtual
	bool
	operator not_eq (const X3DFieldDefinition &) const = 0;

	///  @name Member access

	virtual
	X3DConstants::FieldType
	getType () const = 0;

	void
	setAccessType (const AccessType);

	AccessType
	getAccessType () const;

	bool
	isInitializable () const
	{ return getAccessType () & initializeOnly; }

	virtual
	bool
	isInput () const override
	{ return getAccessType () & inputOnly; }

	virtual
	bool
	isOutput () const override
	{ return getAccessType () & outputOnly; }

	virtual
	bool
	isDefaultValue () const = 0;

	void
	setUnit (const UnitCategory);

	UnitCategory
	getUnit () const;

	void
	isGeospatial (const bool);

	bool
	isGeospatial () const;

	/// Returns true if is set during parse otherwise false;
	void
	isSet (const bool);

	bool
	isSet () const;

	void
	isHidden (const bool);

	bool
	isHidden () const;

	///  @name Reference handling

	///  Returns true if this field is a reference for @a accesType, otherwise false.
	bool
	isReference (const AccessType) const;

	void
	addReference (X3DFieldDefinition* const);

	void
	removeReference (X3DFieldDefinition* const);

	void
	updateReferences ();

	const FieldDefinitionSet &
	getReferences () const
	{ realize (); return io -> references; }

	///  @name Route handling

	void
	addInputRoute (Route* const route)
	{ realize (); io -> inputRoutes .emplace (route); }

	void
	removeInputRoute (Route* const route)
	{ realize (); io -> inputRoutes .erase (route); }

	const RouteSet &
	getInputRoutes () const
	{ realize (); return io -> inputRoutes; }

	void
	addOutputRoute (Route* const route)
	{ realize (); io -> outputRoutes .emplace (route); }

	void
	removeOutputRoute (Route* const route)
	{ realize (); io -> outputRoutes .erase (route); }

	const RouteSet &
	getOutputRoutes () const
	{ realize (); return io -> outputRoutes; }

	///  @name Interest handling

	void
	addInterest (X3DFieldDefinition* const) const;

	void
	addInterest (X3DFieldDefinition & fieldDefinition) const
	{ addInterest (&fieldDefinition); }

	void
	removeInterest (X3DFieldDefinition* const) const;

	void
	removeInterest (X3DFieldDefinition & fieldDefinition) const
	{ removeInterest (&fieldDefinition); }

	const FieldDefinitionSet &
	getInterests () const
	{ realize (); return io -> outputInterests; }

	///  @name Event handling

	virtual
	void
	processEvent (const EventPtr &) override;

	
	///  @name Misc

	virtual
	bool
	isArray () const
	{ return false; }

	virtual
	bool
	hasRootedObjects (ChildObjectSet &) override;

	///  @name Destruction

	///  Disposed this object.  For X3DFieldDefinitions it is save to call this function function directly.
	virtual
	void
	dispose () override;

	///  Destructs this object.
	virtual
	~X3DFieldDefinition () override;


protected:

	///  @name Construction

	///  Constructs new X3DFieldDefinition.
	X3DFieldDefinition ();


private:

	///  @name Construction

	void
	realize () const;

	///  @name Reference handling

	void
	updateIsReference (X3DFieldDefinition* const);

	///  @name Interest handling

	void
	addInputInterest (const X3DFieldDefinition* const) const;

	void
	removeInputInterest (const X3DFieldDefinition* const) const;


	///  @name Static Members

	using FlagsType = uint16_t;

	static constexpr FlagsType ACCESS_TYPE_OFFSET = 0;
	static constexpr FlagsType ACCESS_TYPE_BITS   = 7 << ACCESS_TYPE_OFFSET;
	static constexpr FlagsType UNIT_OFFSET        = 3;
	static constexpr FlagsType UNIT_BITS          = 15 << UNIT_OFFSET;
	static constexpr FlagsType IS_SET_BIT         = 1 << 7;
	static constexpr FlagsType HIDDEN_BIT         = 1 << 8;
	static constexpr FlagsType GEO_BIT            = 1 << 9;

	///  @name Members

	struct IO
	{
		IO () :
			flags (initializeOnly)
		{ }

		FieldDefinitionSet                   references;
		RouteSet                             inputRoutes;
		RouteSet                             outputRoutes;
		std::set <const X3DFieldDefinition*> inputInterests;
		std::set <X3DFieldDefinition*>       outputInterests;
		FlagsType                            flags;
	};

	mutable std::unique_ptr <IO> io;

};

} // X3D
} // titania

#endif
