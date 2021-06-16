// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#ifndef __TITANIA_COMMANDS_INFO_H__
#define __TITANIA_COMMANDS_INFO_H__

#include "../../Applications/main/CommandOptions.h"

#include <Titania/X3D.h>

#include <iostream>

namespace titania {
namespace puck {

class Info
{
public:

	static
	int
	main (const CommandOptions & options)
	{
		if (options .list == "profiles")
			return profiles ();

		if (options .list == "components")
			return components ();

		if (options .list == "nodes")
			return nodes ();

		if (options .list == "fields")
			return fields ();

		return 1;
	}

private:

	static
	int
	profiles ()
	{
		const auto   browser  = X3D::getBrowser ();
		const auto & profiles = browser -> getSupportedProfiles ();

		std::cout .imbue (std::locale::classic ());

		for (const auto & profile : profiles)
		{
			std::cout << profile -> getName () << std::endl;
		}

		return 0;
	}

	static
	int
	components ()
	{
		const auto   browser    = X3D::getBrowser ();
		const auto & components = browser -> getSupportedComponents ();

		std::cout .imbue (std::locale::classic ());

		for (const auto & component : components)
		{
			std::cout << component -> getName () << std::endl;
		}

		return 0;
	}

	static
	int
	nodes ()
	{
		X3D::Generator::Style (std::cout, "compact");

		std::cout .imbue (std::locale::classic ());

		std::cout << X3D::Generator::CompactStyle;

		for (const auto & pair : X3D::getBrowser () -> getSupportedNodes ())
		{
			const auto & node             = pair .second;
			const auto & fieldDefinitions = node -> getFieldDefinitions ();

			std::cout << '[' << node -> getTypeName () << ']' << std::endl;

			for (const auto & field : fieldDefinitions)
			{
				std::cout
				   << '\t'
				   << field -> getName () << " = "
				   << field -> getName () << ";"
				   << X3D::to_string (field -> getAccessType ()) << ";"
				   << field -> getTypeName () << ";";

				if (field -> isInitializable ())
					std::cout << *field;

				std::cout << std::endl;
			}

			std::cout
			   << '\t'
			   << "containerField = "
			   << node -> getContainerField ()
			   << std::endl;

			std::cout
			   << '\t'
			   << "componentName = "
			   << node -> getComponent () .first
			   << std::endl;

			std::cout
			   << '\t'
			   << "componentLevel = "
			   << node -> getComponent () .second
			   << std::endl;

			std::cout << std::endl;
		}

		return 0;
	}

	static
	int
	fields ()
	{
		std::cout .imbue (std::locale::classic ());

		X3D::Generator::Style (std::cout, "compact");

		std::cout << X3D::Generator::CompactStyle;

		for (const auto & field : X3D::getBrowser () -> getSupportedFields ())
		{
			std::cout
			   << '[' << field .second -> getTypeName () << ']' << std::endl
			   << '\t'
			   << "value" << " = "
			   << *field .second
			   << std::endl
			   << std::endl;
		}

		return 0;
	}

};

} // puck
} // titania

#endif
