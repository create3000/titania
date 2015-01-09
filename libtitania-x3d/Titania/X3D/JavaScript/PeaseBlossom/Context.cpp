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

#include "Context.h"

#include "Arguments.h"
#include "Global.h"
#include "Fields.h"

#include <Titania/PeaseBlossom/Debug.h>
#include <cassert>

namespace titania {
namespace X3D {
namespace peaseblossom {

const ComponentType Context::component      = ComponentType::TITANIA;
const std::string   Context::typeName       = "Context";
const std::string   Context::containerField = "context";

Context::Context (X3D::Script* const script, const std::string & ecmascript, const basic::uri & uri)
throw (std::exception) :
	         X3D::X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3D::X3DJavaScriptContext (script, ecmascript),
	                 worldURL ({ uri }),
	                  program (pb::createProgram ()),
	                callbacks (),
	                  classes (size_t (ObjectType::SIZE)),
	                  objects ()
{
	__LOG__ << X3D::SFTime (chrono::now ()) << std::endl;

	try
	{
		addClasses ();
		addUserDefinedFields ();

		program -> fromString (getECMAScript ());
		program -> run ();
	}
	catch (const pb::pbException & error)
	{
		setError (error);

		throw;
	}

	__LOG__ << X3D::SFTime (chrono::now ()) << std::endl;
}

X3DBaseNode*
Context::create (X3D::X3DExecutionContext* const) const
{
	return new Context (getScriptNode (), getECMAScript (), worldURL .front ());
}

void
Context::addClasses ()
{
	using namespace std::placeholders;
	
	callbacks = pb::Callbacks {
		pb::PropertyGetter (),
		pb::PropertySetter (),
		pb::IndexedGetter (),
		pb::IndexedSetter (),
		std::bind (&Context::resolve, this, _1, _2),
		pb::DisposeCallback ()
	};

	program -> getGlobalObject () -> setCallbacks (callbacks);
	program -> getUserData () .emplace_back (this);

	Global::initialize (this, program, program -> getGlobalObject ());
}

void
Context::addUserDefinedFields ()
{ }

bool
Context::resolve (pb::pbObject* const object, const pb::Identifier & identifier)
{
	__LOG__ << identifier << std::endl;

	static const std::map <pb::Identifier, ObjectType> types = {
		std::make_pair (pb::Identifier ("X3DField"),      X3DField::getType ()),
		std::make_pair (pb::Identifier ("X3DArrayField"), X3DArrayField::getType ()),
		std::make_pair (pb::Identifier ("SFVec4d"),       SFVec4d::getType ()),
		std::make_pair (pb::Identifier ("SFVec4f"),       SFVec4f::getType ()),
		std::make_pair (pb::Identifier ("MFVec4d"),       MFVec4d::getType ()),
		std::make_pair (pb::Identifier ("MFVec4f"),       MFVec4f::getType ())
	};

	try
	{
		object -> addOwnProperty (identifier, getClass (types .at (identifier)), pb::WRITABLE | pb::CONFIGURABLE);
		return true;
	}
	catch (const std::out_of_range &)
	{
		return false;
	}
}

const pb::ptr <pb::NativeFunction> &
Context::getClass (const ObjectType type) const
{
	using Initialize = std::function <pb::ptr <pb::NativeFunction> (Context* const, const pb::ptr <pb::Program> &)>;

	static const std::map <ObjectType, Initialize> functions = {
		std::make_pair (X3DField::getType (),      X3DField::initialize),
		std::make_pair (X3DArrayField::getType (), X3DArrayField::initialize),
		std::make_pair (SFVec4d::getType (),       SFVec4d::initialize),
		std::make_pair (SFVec4f::getType (),       SFVec4f::initialize),
		std::make_pair (MFVec4d::getType (),       MFVec4d::initialize),
		std::make_pair (MFVec4f::getType (),       MFVec4f::initialize)
	};

	auto & standardClass = const_cast <Context*> (this) -> classes [size_t (type)];

	if (standardClass)
		return standardClass;

	return standardClass = functions .at (type) (const_cast <Context*> (this), program);
}

void
Context::addObject (X3DFieldDefinition* const field, pb::pbObject* const object)
throw (std::invalid_argument)
{
	if (not objects .emplace (field, object) .second)
		throw std::invalid_argument ("Context::addObject");

	field -> addParent (this);
}

void
Context::removeObject (X3D::X3DFieldDefinition* const field)
{
	if (objects .erase (field))
		field -> removeParent (this);
	else
		__LOG__ << field -> getName () << " : " << field -> getTypeName () << std::endl;
}

void
Context::initialize ()
{
	__LOG__ << X3D::SFTime (chrono::now ()) << std::endl;

	X3DJavaScriptContext::initialize ();

	getExecutionContext () -> isLive () .addInterest (this, &Context::set_live);
	isLive () .addInterest (this, &Context::set_live);

	set_live ();

	try
	{
		program -> getFunctionDeclaration ("initialize") -> call (program -> getGlobalObject ());
	}
	catch (const pb::pbException & error)
	{
		setError (error);
	}
	catch (const std::exception & error)
	{ }

	__LOG__ << X3D::SFTime (chrono::now ()) << std::endl;
}

void
Context::setExecutionContext (X3D::X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getExecutionContext () -> isLive () .removeInterest (this, &Context::set_live);

	X3DJavaScriptContext::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (this, &Context::set_live);

		set_live ();
	}
}

void
Context::set_live ()
{
	if (getExecutionContext () -> isLive () and isLive ())
	{
		if (program -> hasFunctionDeclaration ("prepareEvents"))
			getBrowser () -> prepareEvents () .addInterest (this, &Context::prepareEvents);

		if (program -> hasFunctionDeclaration ("eventsProcessed"))
			getScriptNode () -> addInterest (this, &Context::eventsProcessed);

		getScriptNode () -> addInterest (this, &Context::finish);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly   :
				case inputOutput :
					{
						if (program -> hasFunctionDeclaration (field -> getName ()))
							field -> addInterest (this, &Context::set_field, field);

						break;
					}
				default:
					break;
			}
		}
	}
	else
	{
		if (program -> hasFunctionDeclaration ("prepareEvents"))
			getBrowser () -> prepareEvents () .removeInterest (this, &Context::prepareEvents);

		if (program -> hasFunctionDeclaration ("eventsProcessed"))
			getScriptNode () -> removeInterest (this, &Context::eventsProcessed);

		getScriptNode () -> removeInterest (this, &Context::finish);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly   :
				case inputOutput :
					{
						if (program -> hasFunctionDeclaration (field -> getName ()))
							field -> removeInterest (this, &Context::set_field);

						break;
					}
				default:
					break;
			}
		}
	}
}

void
Context::prepareEvents ()
{
	try
	{
		program -> getFunctionDeclaration ("prepareEvents") -> call (program -> getGlobalObject ());
	}
	catch (const pb::pbException & error)
	{
		setError (error);
	}
	catch (const std::exception & error)
	{ }
}

void
Context::set_field (X3D::X3DFieldDefinition* const field)
{
	field -> isTainted (true);

	try
	{
		const auto & function = program -> getFunctionDeclaration (field -> getName ());

		function -> call (program -> getGlobalObject (),
		                  {
		                     pb::var (),
		                     pb::var (getCurrentTime ())
								});
	}
	catch (const pb::pbException & error)
	{
		getBrowser () -> println (error);
	}
	catch (const std::exception & error)
	{ }

	field -> isTainted (false);
}

void
Context::eventsProcessed ()
{
	try
	{
		program -> getFunctionDeclaration ("eventsProcessed") -> call (program -> getGlobalObject ());
	}
	catch (const pb::pbException & error)
	{
		setError (error);
	}
	catch (const std::exception & error)
	{ }
}

void
Context::finish ()
{
	pb::GarbageCollector::deleteObjectsAsync ();
}

void
Context::shutdown ()
{ }

void
Context::setError (const pb::pbException & error) const
{
	X3D::X3DJavaScriptContext::setError (error .toString (),
	                                     error .getFilename (),
	                                     error .getLineNumber (),
	                                     error .getColumn (),
	                                     error .getSourceLine ());
}

void
Context::dispose ()
{
	const auto p = program .get ();

	classes .clear ();
	program .dispose ();

	pb::debug_roots (p);
	__LOG__ << p -> getParents () .size () << std::endl;
	__LOG__ << objects .size () << std::endl;
	//assert (p -> getParents () .empty ());
	//assert (objects .empty ());

	pb::GarbageCollector::deleteObjectsAsync ();

	X3D::X3DJavaScriptContext::dispose ();
}

Context::~Context ()
{ }

} // peaseblossom
} // X3D
} // titania
