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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_FUNCTION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_FUNCTION_H__

#include "../Execution/jsExecutionContext.h"
#include "../Objects/jsFunction.h"

namespace titania {
namespace pb {

shared_ptr <Function>
createFunction (jsExecutionContext* const, const std::string & name = "", std::vector <std::string> && formalParameters = { });

/**
 *  Class to represent a scripted JavaScript function.
 */
class Function :
	public jsFunction,
	public jsExecutionContext
{
public:

	///  @name Common members

	///  Returns the a default of its input argument type.
	virtual
	var
	getDefaultValue () const final override
	{ return createFunction (getExecutionContext () .get ()); }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ jsFunction::toStream (ostream); }

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing of an object does not remove object itself.
	virtual
	void
	dispose () final override
	{
		jsFunction::dispose ();
		jsExecutionContext::dispose ();
	}

protected:

	///  @name Friends

	friend
	shared_ptr <Function>
	createFunction (jsExecutionContext* const, const std::string &, std::vector <std::string> &&);

	///  @name Construction

	///  Constructs new Function.
	Function (jsExecutionContext* const executionContext, const std::string & name = "", std::vector <std::string> && formalParameters = { }) :
		        jsFunction (name),
		jsExecutionContext (executionContext, executionContext -> getGlobalObject ()),
		  formalParameters (std::move (formalParameters))
	{ __LOG__ << std::endl; }


private:

	std::vector <std::string> formalParameters;

};

inline
shared_ptr <Function>
createFunction (jsExecutionContext* const executionContext, const std::string & name, std::vector <std::string> && formalParameters)
{
	shared_ptr <Function> function (new Function (executionContext, name, std::move (formalParameters)));

	function -> setup ();

	return function;
}

} // pb
} // titania

#endif
