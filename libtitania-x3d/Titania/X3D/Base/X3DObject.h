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

#ifndef __TITANIA_X3D_BASE_X3DOBJECT_H__
#define __TITANIA_X3D_BASE_X3DOBJECT_H__

#include "../Base/X3DGarbageCollector.h"
#include "../Base/X3DInput.h"
#include "../Base/X3DOutput.h"
#include "../Bits/Error.h"

#include <Titania/LOG.h>

#include <vector>
#include <istream>
#include <memory>
#include <ostream>

namespace titania {
namespace X3D {

using UserDataPtr = std::shared_ptr <X3DBase>;

/**
 *  Class to represent an object that is the base for all X3D objects.
 */
class X3DObject :
	public X3DInput, public X3DOutput
{
public:

	///  @name Type Information

	///  Returns the id of this object.  This is usefull for comparing object.
	virtual
	size_t
	getId () const
	{ return reinterpret_cast <size_t> (this); }

	///  Sets the name of this object.
	virtual
	void
	setName (const std::string & value);

	///  Returns the name of this object.
	virtual
	const std::string &
	getName () const;

	///  Returns the type name of this object.
	///  throws Error <DISPOSED>
	virtual
	const std::string &
	getTypeName () const = 0;

	///  @name Comment handling

	///  Add comments to this object.
	void
	addComments (const std::vector <std::string> & value);

	///  Returns the comments of this object.
	const std::vector <std::string> &
	getComments () const;

	///  @name User data handling

	///  Set new user data for this object.
	virtual
	void
	setUserData (const UserDataPtr & value);

	///  Returns the user data of this object. If no user data are available, an empty pointer is returned.
	virtual
	const UserDataPtr &
	getUserData () const;

	///  Returns the user data of this object. If no user data are available, new user data are created.
	template <class Type>
	std::shared_ptr <Type>
	getUserData ()
	{
		if (not getUserData ())
			setUserData (std::make_shared <Type> ());

		return std::dynamic_pointer_cast <Type> (getUserData ());
	}

	///  @name String creation

	///  Extracts the value for this object from @a string. The string must be in VRML Classic Encoding.
	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	bool
	fromString (const std::string & string);

	///  Extracts the value for this object from @a string with locale support.
	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	bool
	fromLocaleString (const std::string & string, const std::locale & locale = std::locale ());

	///  Generates a string representation of this object in VRML Classic Encoding.
	std::string
	toString () const;

	///  Generates a string representation of this object with locale support.
	virtual
	std::string
	toLocaleString (const std::locale & locale = std::locale ()) const;

	///  Generates a string representation of this object in X3D XML Encoding.
	virtual
	std::string
	toXMLString () const;

	///  Generates a string representation of this object in X3D JSON Encoding.
	virtual
	std::string
	toJSONString () const;

	///  @name Input/Output

	///  Extracts the value for this object from @a istream. The contents of @a istream must be in VRML Classic Encoding style.
	virtual
	void
	fromStream (std::istream & istream) = 0;

	///  Inserts this object into @a ostream in VRML Classic Encoding style.
	virtual
	void
	toStream (std::ostream & ostream) const = 0;

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toXMLStream (std::ostream & ostream) const = 0;

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toJSONStream (std::ostream & ostream) const = 0;

	///  @name Destruction

	///  Disposed this object.  You normally do not need to call this function directly.
	virtual
	void
	dispose () override;

	///  Destructs this object.
	virtual
	~X3DObject () override;


protected:

	///  @name Construction

	///  Constructs new X3DObject.
	X3DObject ();


private:

	///  @name Construction

	///  Realizes this object if needed.
	void
	realize () const;

	///  @name Members

	struct Data
	{
		std::string name;
		std::vector <std::string> comments;
		UserDataPtr userData;
	};

	mutable std::unique_ptr <Data> data;

};


///  @relates X3DObject
///  @name Input/Output operations

///  Extraction operator for X3DObject values. The stream must be in VRML Classic Encoding.
template <class CharT, class Traits>
inline
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, X3DObject & object)
{
	object .fromStream (istream);
	return istream;
}

///  Insertion operator for X3DObject values. The output will be in VRML Classic Encoding.
template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const X3DObject & object)
{
	object .toStream (ostream);
	return ostream;
}

} // X3D
} // titania

#endif
