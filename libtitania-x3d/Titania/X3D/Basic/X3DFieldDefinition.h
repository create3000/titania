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

#ifndef __TITANIA_X3D_BASIC_X3DFIELD_DEFINITION_H__
#define __TITANIA_X3D_BASIC_X3DFIELD_DEFINITION_H__

#include "../Base/X3DChildObject.h"
#include "../Basic/FieldDefinitionSet.h"
#include "../Bits/X3DConstants.h"
#include "../Routing/RouteSet.h"

#include <vector>

namespace titania {
namespace X3D {

enum AccessType
{
	initializeOnly = 1, // 0b001
	inputOnly      = 2, // 0b010
	outputOnly     = 4, // 0b100
	inputOutput    = 7  // 0b111

};

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
	clone (X3DExecutionContext* const) const;

	virtual
	X3DConstants::FieldType
	getType () const = 0;

	virtual
	bool
	hasRoots (ChildObjectSet &);

	void
	setReference (X3DFieldDefinition* const);

	X3DFieldDefinition*
	getReference () const;

	void
	setAliasName (const std::string &);

	const std::string &
	getAliasName () const;

	void
	setAccessType (const AccessType);

	AccessType
	getAccessType () const;

	bool
	isInitializeable () const;

	virtual
	bool
	isInput () const;

	virtual
	bool
	isOutput () const;

	virtual
	bool
	operator == (const X3DFieldDefinition &) const = 0;

	virtual
	bool
	operator not_eq (const X3DFieldDefinition &) const = 0;

	void
	addInputRoute (X3DRoute* const);

	void
	removeInputRoute (X3DRoute* const);

	const RouteSet &
	getInputRoutes () const;

	void
	addOutputRoute (X3DRoute* const);

	void
	removeOutputRoute (X3DRoute* const);

	const RouteSet &
	getOutputRoutes () const;

	void
	addInterest (X3DFieldDefinition* const);

	void
	addInterest (X3DFieldDefinition &);

	void
	removeInterest (X3DFieldDefinition* const);

	void
	removeInterest (X3DFieldDefinition &);

	virtual
	void
	processEvents (ChildObjectSet &);

	virtual
	void
	processEvent (X3DChildObject* const, ChildObjectSet &);

	virtual
	void
	dispose ();

	virtual
	~X3DFieldDefinition ();


protected:

	X3DFieldDefinition ();


private:

	X3DFieldDefinition* reference;

	AccessType  accessType;
	std::string aliasName;

	RouteSet           inputRoutes;
	RouteSet           outputRoutes;
	FieldDefinitionSet interests;

};

} // X3D
} // titania

#endif
