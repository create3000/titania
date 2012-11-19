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

#ifndef __TITANIA_X3D_BASE_X3DOBJECT_H__
#define __TITANIA_X3D_BASE_X3DOBJECT_H__

#include "../Base/GarbageCollector.h"
#include "../Base/ObjectSet.h"
#include "../Base/X3DInput.h"
#include "../Base/X3DOutput.h"
#include "../Base/X3DType.h"
#include "../Bits/Error.h"

#include <deque>
#include <istream>
#include <ostream>
#include <typeinfo>

namespace titania {
namespace X3D {

class X3DObject :
	public X3DInput, public X3DOutput, public X3DType
{
public:

	///  @name Type Information

	void
	setName (const basic::id &);

	virtual
	const X3DType*
	getType () const = 0;

	virtual
	const basic::id
	getTypeName () const = 0;

	///  @name Input/Output

	virtual
	void
	write (const X3DObject &) { }

	virtual
	bool
	isInput () const;

	virtual
	bool
	isOutput () const;

	virtual
	void
	processEvents (ObjectSet &) = 0; // XXX

	virtual
	void
	processEvent (X3DObject* const, ObjectSet &) = 0;

	///  @name Garbage Collection

	GarbageCollector &
	getGarbageCollector ();

	// String:
	virtual
	std::string
	toString () const;

	///  @name Stream Handling
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	virtual
	void
	toStream (std::ostream &) const = 0;

	///  @name Destruction Handling

	virtual
	void
	dispose ();

	virtual
	~X3DObject ();


protected:

	X3DObject ();

	virtual
	void
	notify (X3DObject* const) = 0;


private:

	static GarbageCollector garbageCollector;

};

template <class CharT, class Traits>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, X3DObject & object)
{
	object .fromStream (istream);
	return istream;
}

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const X3DObject & object)
{
	object .toStream (ostream);
	return ostream;
}

extern template std::istream & operator >> (std::istream &, X3DObject &);
extern template std::ostream & operator << (std::ostream &, const X3DObject &);
//extern template std::wistream & operator >> (std::wistream &, const X3DObject &);
//extern template std::wostream & operator << (std::wostream &, const X3DObject &);

} // X3D
} // titania

#endif
