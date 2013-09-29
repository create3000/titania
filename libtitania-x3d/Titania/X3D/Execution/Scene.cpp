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

#include "Scene.h"

#include "../Browser/X3DBrowser.h"
#include "../Parser/RegEx.h"

namespace titania {
namespace X3D {

const std::string Scene::componentName  = "Browser";
const std::string Scene::typeName       = "Scene";
const std::string Scene::containerField = "scene";

Scene::Scene (X3DBrowser* const browser) :
	X3DBaseNode (browser, this),
	   X3DScene ()
{
	addField (inputOutput, "metadata", metadata ());
}

Scene*
Scene::create (X3DExecutionContext* const executionContext) const
{
	return new Scene (executionContext -> getBrowser ());
}

void
Scene::importScene (const X3DSFNode <Scene> & scene)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (getBrowser () -> makeCurrent ())
	{
		if (scene -> getProfile () or not scene -> getComponents () .empty ())
		{
			if (getProfile ())
			{
				if (scene -> getProfile ())
					addComponents (scene -> getProfile () -> getComponents ());

				addComponents (scene -> getComponents ());
			}
			else
				setProfile (getBrowser () -> getProfile ("Full"));
		}

		importMetaData (scene);

		importExternProtos (scene);
		importProtos (scene);

		updateNamedNodes (scene);
		importRootNodes (scene);

		initialize ();

		importImportedNodes (scene);
		importRoutes (scene);
		importExportedNodes (scene);

		return;
	}

	throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

void
Scene::updateNamedNodes (const X3DSFNode <Scene> & scene)
{
	for (const auto & node : NamedNodeIndex (scene -> getNamedNodes ()))
	{
		scene -> removeNamedNode (node .first);
		scene -> updateNamedNode (getUniqueName (scene, node .first), node .second);
	}
}

std::string
Scene::getUniqueName (const X3DSFNode <Scene> & scene, std::string name) const
{
	__LOG__ << name << std::endl;

	RegEx::_LastNumber .Replace ("", &name);
	RegEx::LastNumber .Replace ("", &name);

	if (name .empty ())
		return getUniqueName (scene);

	else
	{
		std::string newName = name;
		size_t      i       = 0;

		for ( ; ;)
		{
			try
			{
				getNamedNode (newName);
			}
			catch (const Error <INVALID_NAME> &)
			{
				try
				{
					scene -> getNamedNode (newName);
				}
				catch (const Error <INVALID_NAME> &)
				{
					break;
				}
			}

			newName = name + std::to_string (++ i);

			__LOG__ << "\t" << newName << std::endl;
		}

		__LOG__ << "\t" << newName << std::endl;

		return newName;
	}
}

std::string
Scene::getUniqueName (const X3DSFNode <Scene> & scene) const
{
	std::string name;
	size_t      i = 0;

	for ( ; ;)
	{
		name = '_' + std::to_string (++ i);

		try
		{
			getNamedNode (name);
		}
		catch (const Error <INVALID_NAME> &)
		{
			try
			{
				scene -> getNamedNode (name);
			}
			catch (const Error <INVALID_NAME> &)
			{
				break;
			}
		}
	}

	return name;
}

Scene::~Scene ()
{
	__LOG__ << std::endl;

	getGarbageCollector () .trimFreeMemory ();
}

} // X3D
} // titania
