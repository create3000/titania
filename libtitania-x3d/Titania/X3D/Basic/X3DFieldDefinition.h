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

#ifndef __TITANIA_X3D_BASIC_X3DFIELD_DEFINITION_H__
#define __TITANIA_X3D_BASIC_X3DFIELD_DEFINITION_H__

#include "../Base/X3DChildObject.h"
#include "../Basic/FieldDefinitionArray.h"
#include "../Basic/FieldDefinitionSet.h"
#include "../Bits/X3DConstants.h"
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

	virtual
	X3DFieldDefinition*
	clone () const = 0;

	virtual
	X3DFieldDefinition*
	clone (X3DExecutionContext* const) const
	{ return clone (); }

	virtual
	void
	clone (X3DExecutionContext* const, X3DFieldDefinition*) const = 0;

	X3DFieldDefinition &
	operator = (const X3DFieldDefinition &);

	virtual
	bool
	isArray () const
	{ return false; }

	virtual
	X3DConstants::FieldType
	getType () const = 0;

	virtual
	bool
	hasRoots (ChildObjectSet &) override;

	void
	addReference (X3DFieldDefinition* const);

	void
	removeReference (X3DFieldDefinition* const);

	void
	updateReferences ();

	const FieldDefinitionSet &
	getReferences () const
	{ return references; }

	void
	setAliasName (const std::string & value)
	{ aliasName = value; }

	const std::string &
	getAliasName () const
	{ return aliasName; }

	void
	setAccessType (const AccessType value)
	{ accessType = value; }

	AccessType
	getAccessType () const
	{ return accessType; }

	bool
	isInitializeable () const
	{ return accessType & initializeOnly; }

	virtual
	bool
	isInput () const override
	{ return accessType & inputOnly; }

	virtual
	bool
	isOutput () const override
	{ return accessType & outputOnly; }

	virtual
	bool
	operator == (const X3DFieldDefinition &) const = 0;

	virtual
	bool
	operator not_eq (const X3DFieldDefinition &) const = 0;

	void
	addInputRoute (Route* const route)
	{ inputRoutes .insert (route); }

	void
	removeInputRoute (Route* const route)
	{ inputRoutes .erase (route); }

	const RouteSet &
	getInputRoutes () const
	{ return inputRoutes; }

	void
	addOutputRoute (Route* const route)
	{ outputRoutes .insert (route); }

	void
	removeOutputRoute (Route* const route)
	{ outputRoutes .erase (route); }

	const RouteSet &
	getOutputRoutes () const
	{ return outputRoutes; }

	void
	addInterest (X3DFieldDefinition* const interest)
	{ interests .insert (interest); }

	void
	addInterest (X3DFieldDefinition & interest)
	{ interests .insert (&interest); }

	void
	removeInterest (X3DFieldDefinition* const interest)
	{ interests .erase (interest); }

	void
	removeInterest (X3DFieldDefinition & interest)
	{ interests .erase (&interest); }

	virtual
	void
	processEvent (Event &) override;

	virtual
	void
	dispose () override;

	virtual
	~X3DFieldDefinition ();


protected:

	X3DFieldDefinition ();


private:

	void
	updateReference (X3DFieldDefinition* const);

	FieldDefinitionSet references;

	AccessType  accessType;
	std::string aliasName;

	RouteSet           inputRoutes;
	RouteSet           outputRoutes;
	FieldDefinitionSet interests;

};

} // X3D
} // titania

#endif
