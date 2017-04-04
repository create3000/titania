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

#ifndef __TITANIA_MATH_ALGORITHMS_LSYSTEM_H__
#define __TITANIA_MATH_ALGORITHMS_LSYSTEM_H__

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace titania {
namespace math {

class lsystem
{
public:

	///  @name Construction

	lsystem (const size_t iterations,
	         const std::string & constants,
	         const std::string & axiom,
	         const std::vector <std::string> & rules)
	throw (std::runtime_error);

	///  @name Member access

	size_t
	iterations () const
	{ return m_iterations; }

	const std::vector <bool> &
	constants () const
	{ return m_constants; }

	const std::string &
	axiom () const
	{ return m_axiom; }

	const std::vector <std::string> &
	rules () const
	{ return m_rules; }

	const std::string &
	commands () const
	{ return m_commands; }

	///  @name Destruction

	~lsystem ();


private:

	///  @name Operations

	void
	add_constant (const std::string::value_type constant);

	void
	add_rule (const std::string & rule)
	throw (std::runtime_error);

	void
	generate ()
	throw (std::runtime_error);

	///  @name Members

	const size_t                             m_iterations;
	const std::string                        m_axiom;
	const std::vector <std::string>          m_rules;

	std::vector <bool>                              m_constants;
	std::map <std::string::value_type, std::string> m_rules_index;
	std::string                                     m_commands;

};

///  Insertion operator for lsystem values.
inline
std::ostream &
operator << (std::ostream & ostream, const lsystem & ls)
{
	return ostream << ls .commands ();
}

} // math
} // titania

#endif
