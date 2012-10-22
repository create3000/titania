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

#ifndef __TITANIA_X3D_BASIC_INTERFACE_DECLARATION_H__
#define __TITANIA_X3D_BASIC_INTERFACE_DECLARATION_H__

#include "../Basic/X3DFieldDefinition.h"

namespace titania {
namespace X3D {

class InterfaceDeclaration :
	public X3DFieldDefinition
{
public:

	InterfaceDeclaration (const X3DConstants::X3DAccessType, const basic::id &, const X3DFieldDefinition*);

	virtual
	InterfaceDeclaration*
	copy () const;

	virtual
	const X3DType*
	getType () const;

	virtual
	const basic::id
	getTypeName () const;

	X3DConstants::X3DAccessType
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
	isDefaultValue () const;

	// Stream:
	//@{
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	void
	toStream (std::ostream &) const;
	//@}

	virtual
	~InterfaceDeclaration ();


private:

	X3DConstants::X3DAccessType accessType;
	const X3DFieldDefinition*   value;

};

//template <class Type, class ValueT>
//inline
//InterfaceDeclaration
//inputOutput (const std::string & name, const ValueT & value)
//{
//	return InterfaceDeclaration (X3DConstants::inputOutput, name, new Type (value))
//}

} // X3D
} // titania

#endif
