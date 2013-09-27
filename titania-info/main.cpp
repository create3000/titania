// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include <iostream>

#include <Titania/X3D.h>

#include "anyoption.h"

using namespace titania;

int
componentIndex ()
{
	auto browser = X3D::getBrowser ();

	const X3D::ComponentInfoArray & components = browser -> getSupportedComponents ();

	for (const auto & component : components)
	{
		std::cout << component -> getName () << std::endl;
	}

	return 0;
}

int
nodeIndex ()
{
	X3D::Generator::Style ("compact");

	for (const auto & node : X3D::getBrowser () -> getSupportedNodes ())
	{
		std::cout << '[' << node -> getTypeName () << ']' << std::endl;

		const X3D::FieldDefinitionArray & fieldDefinitions = node -> getFieldDefinitions ();

		for (const auto & fieldDefinition : fieldDefinitions)
		{
			const X3D::X3DFieldDefinition* field = node -> getField (fieldDefinition -> getName ());

			std::cout
				<< '\t'
				<< field -> getName () << " = "
				<< field -> getAliasName () << ";"
				<< X3D::Generator::AccessTypes [field] << ";"
				<< field -> getTypeName () << ";";

			if (field -> isInitializeable ())
				std::cout << field -> toString ();

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
			<< node -> getComponentName ()
			<< std::endl;

		std::cout << std::endl;
	}

	return 0;
}

int
x3d ()
{
	for (const auto & node : X3D::getBrowser () -> getSupportedNodes ())
		std::cout << node << std::endl;

	return 0;
}

int
fields ()
{
	X3D::Generator::Style ("compact");

	for (const auto & field : X3D::getBrowser () -> getSupportedFields ())
	{
		std::cout
			<< '[' << field -> getTypeName () << ']' << std::endl
			<< '\t'
			<< "value" << " = "
			<< *field
			<< std::endl
			<< std::endl;
	}

	return 0;
}

int
main (int argc, char** argv)
{
	AnyOption options;

	options .addUsage ("SYNOPSIS");
	options .addUsage ("       titaniaInfo [-i=index]");
	options .addUsage ("");
	options .addUsage ("DESCRIPTION");
	options .addUsage ("       Format FILE, or standard input, to standard output");
	options .addUsage ("");
	options .addUsage ("       -i=index, --index=index");
	options .addUsage ("              index can be 'profile', 'component' or 'node'");
	options .addUsage ("              nodes is the default index");
	options .addUsage ("              profiles:   get a list of all supported profiles");
	options .addUsage ("              components: get a list of all supported components");
	options .addUsage ("              nodes:      get a list of all supported nodes with it's");
	options .addUsage ("                          associated fields in key file style");
	options .addUsage ("");
	options .addUsage ("       -x, --x3d");
	options .addUsage ("              get a list of all supported nodes in VRML style");
	options .addUsage ("");
	options .addUsage ("       -f, --fields");
	options .addUsage ("              get a list of all supported fields in key file style");
	options .addUsage ("");
	options .addUsage ("       -h, --help");
	options .addUsage ("              prints usage");

	options .setFlag   ("x3d",    'x');
	options .setFlag   ("fields", 'f');
	options .setOption ("index",  'i');

	options .processCommandArgs (argc, argv);

	try
	{
		if (options .getValue ("index") or options .getValue ('i'))
		{
			std::string index = options .getValue ("index") ? options .getValue ("index") : options .getValue ('i');

			if (index == "profiles")
				return componentIndex ();

			if (index == "components")
				return componentIndex ();

			if (index == "nodes")
				return nodeIndex ();
		}

		if (options .getFlag ("x3d") or options .getFlag ('x'))
			return x3d ();

		if (options .getFlag ("fields") or options .getFlag ('f'))
			return fields ();

		options .printUsage ();

		return 0;
	}
	catch (const Glib::Exception & exception)
	{
		std::clog << exception .what () << std::endl;
	}
	catch (const std::exception & exception)
	{
		std::clog << exception .what () << std::endl;
	}
	catch (...)
	{
		std::clog << "Error: Unknown Exception." << std::endl;
	}

	return 1;
}
