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

#include "LSystem.h"

#include <regex>

#include <Titania/LOG.h>

namespace titania {
namespace math {

namespace {
const std::regex spaces_rx (R"/(\s+)/");
}

lsystem::lsystem (const size_t iterations,
                  const std::string & constants,
                  const std::string & axiom,
                  const std::vector <std::string> & rules)
throw (std::runtime_error) :
	 m_iterations (iterations),
	      m_axiom (std::regex_replace (axiom, spaces_rx, "")),
	      m_rules (rules),
	  m_constants (256),
	m_rules_index (),
	   m_commands ()
{
	for (const auto & constant : constants)
		add_constant (constant);

	for (const auto & rule : m_rules)
		add_rule (rule);

	generate ();
}

void
lsystem::add_constant (const std::string::value_type constant)
{
	const size_t index = constant;

	if (index < m_constants .size ())
		m_constants [index] = true;
}

void
lsystem::add_rule (const std::string & rule)
{
	static const std::regex rule_rx (R"/(^\s*([A-Za-z0-9])\s*=([\sA-Za-z0-9\[\]\+\-]*)$)/");

	std::smatch match;

	if (std::regex_match (rule, match, rule_rx))
		m_rules_index .emplace (match .str (1) .front (), std::regex_replace (match .str (2), spaces_rx, ""));

	//else
	//	throw std::domain_error ("lsystem::add_rule: rule '" + rule + "' does not match.");
}

void
lsystem::generate ()
{
	if (iterations ())
		m_commands = axiom ();

	// process for each iteration
	for (size_t i = 0; i < iterations (); ++ i)
	{
		std::string commands;
		
		// Process each character of the axiom.
		for (const auto c : m_commands)
		{
			const auto iter = m_rules_index .find (c);

			if (iter not_eq m_rules_index .end ())
				commands += iter -> second;

			else
				commands += c;
			
			if (commands .size () > 100'000'000)
			  throw std::runtime_error ("lsystem::generate: generated command string too large! 100,000,000 commands maximum.");
		}

		m_commands = std::move (commands);
	}
}

lsystem::~lsystem ()
{ }

} // math
} // titania
