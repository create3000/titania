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

#include "../../Utility/Range.h"

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include <Titania/LOG.h>

namespace titania {
namespace math {

template <class String>
class lsystem
{
public:

	///  @name Member types

	using string_type    = String;
	using size_type      = size_t;
	using constants_type = std::set <typename String::value_type>;
	using rules_type     = std::map <typename String::value_type, String>;

	///  @name Construction

	lsystem ();

	///  throws std::runtime_error
	template <class Iterator>
	lsystem (const size_type iterations,
	         const String & constants,
	         const String & axiom,
	         const Iterator & first_rule,
	         const Iterator & last_rule);

	///  @name Member access

	void
	iterations (const size_type value)
	{ m_iterations = value; }

	size_type
	iterations () const
	{ return m_iterations; }

	void
	add_constant (const typename String::value_type & constant);

	bool
	is_constant (const typename String::value_type & constant) const;

	const constants_type &
	constants () const
	{ return m_constants; }

	void
	axiom (const String & value)
	{ m_axiom = remove_spaces (value); }

	const String &
	axiom () const
	{ return m_axiom; }

	///  throws std::runtime_error
	void
	add_rule (const String & rule);

	const rules_type &
	rules () const
	{ return m_rules; }

	const String &
	commands () const
	{ return m_commands; }

	///  @name Operations

	///  throws std::runtime_error
	void
	generate ();

	///  @name Destruction

	~lsystem ();


private:

	///  @name Operations

	String
	remove_spaces (const String & string);

	///  @name Members

	size_type      m_iterations;
	String         m_axiom;
	constants_type m_constants;
	rules_type     m_rules;
	String         m_commands;

};

template <class String>
lsystem <String>::lsystem () :
	m_iterations (0),
	     m_axiom (),
	 m_constants (),
	     m_rules (),
	  m_commands ()
{ }

///  throws std::runtime_error
template <class String>
template <class Iterator>
lsystem <String>::lsystem (const size_type iterations,
                           const String & constants,
                           const String & axiom,
                           const Iterator & first_rule,
                           const Iterator & last_rule) :
	m_iterations (iterations),
	     m_axiom (remove_spaces (axiom)),
	 m_constants (),
	     m_rules (),
	  m_commands ()
{
	for (const auto & constant : constants)
		add_constant (constant);

	for (const auto & rule : std::make_pair (first_rule, last_rule))
		add_rule (rule);

	generate ();
}

template <class String>
String
lsystem <String>::remove_spaces (const String & string)
{
	String spaceless;

	for (const auto & c : string)
	{
		if (std::isspace (c))
			continue;

		spaceless += c;
	}

	return spaceless;
}

template <class String>
inline
void
lsystem <String>::add_constant (const typename String::value_type & constant)
{
	m_constants .emplace (constant);
}

template <class String>
inline
bool
lsystem <String>::is_constant (const typename String::value_type & constant) const
{
	return m_constants .count (constant);
}

///  throws std::runtime_error
template <class String>
void
lsystem <String>::add_rule (const String & rule)
{
	const auto rule_without_spaces = remove_spaces (rule);

	if (rule_without_spaces .empty ())
		return;

	if (rule_without_spaces .size () > 1 and rule_without_spaces [1] == '=')
		m_rules .emplace (rule_without_spaces [0], rule_without_spaces .substr (2));

	else
		throw std::runtime_error ("lsystem::add_rule: rule '" + rule + "' does not match.");
}

///  throws std::runtime_error
template <class String>
void
lsystem <String>::generate ()
{
	m_commands = axiom ();

	// Process for each iteration.
	for (size_type i = 0; i < iterations (); ++ i)
	{
		std::ostringstream commands;
		
		// Process each character of the axiom.
		for (const auto c : m_commands)
		{
			const auto iter = m_rules .find (c);

			if (iter not_eq m_rules .end ())
				commands << iter -> second;

			else
				commands << c;

			if (commands .tellp () > 100'000'000)
			  throw std::runtime_error ("lsystem::generate: generated command string too large! 100,000,000 commands maximum.");
		}

		m_commands = commands .str ();
	}
}

template <class String>
lsystem <String>::~lsystem ()
{ }

///  Insertion operator for lsystem values.
template <class String>
inline
std::ostream &
operator << (std::ostream & ostream, const lsystem <String> & ls)
{
	return ostream << ls .commands ();
}

extern template class lsystem <std::string>;

} // math
} // titania

#endif
