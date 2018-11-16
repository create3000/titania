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

#include "Context.h"

#include "Arguments.h"
#include "Browser.h"
#include "Global.h"
#include "Fields.h"
#include "value.h"

#include "../../Thread/SceneFuture.h"

#include <Titania/PeaseBlossom/Debug.h>
#include <cassert>

namespace titania {
namespace X3D {
namespace peaseblossom {

const ComponentType Context::component      = ComponentType::TITANIA;
const std::string   Context::typeName       = "PeaseBlossomContext";
const std::string   Context::containerField = "context";

///  throws std::exception
Context::Context (X3D::Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3D::X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3D::X3DJavaScriptContext (script, ecmascript),
	                 worldURL ({ uri }),
	                  program (pb::createProgram ()),
	                callbacks (),
	                  classes (size_t (ObjectType::SIZE)),
	                  objects (),
	                   values (),
	                   future ()
{
	//__LOG__ << X3D::SFTime (SFTime::now ()) << std::endl;

	try
	{
		addChildObjects (future);

		addClasses ();
		addUserDefinedFields ();

		program -> fromString (getECMAScript ());		
		program -> run ();
	}
	catch (const pb::pbError & error)
	{
		setError (error);

		throw;
	}

	//__LOG__ << X3D::SFTime (SFTime::now ()) << std::endl;
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
		pb::EnumerateCallback (),
		pb::HasPropertyCallback (),
		pb::PropertyGetter (),
		pb::PropertySetter (),
		std::bind (&Context::resolve, this, _1, _2),
		pb::DisposeCallback ()
	};

	program -> getGlobalObject () -> setCallbacks (callbacks);
	program -> getUserData () .emplace_back (this);

	Global::initialize (this, program, program -> getGlobalObject ());
	Browser::initialize (this, program);
}

void
Context::addUserDefinedFields ()
{
	size_t index = 0;

	values .reserve (getScriptNode () -> getFieldDefinitions () .size ());

	for (const auto & field : getScriptNode () -> getUserDefinedFields ())
	{
		values .emplace_back (getValue (this, field));

		switch (field -> getAccessType ())
		{
			case X3D::initializeOnly:
			case X3D::outputOnly:
			{
				defineProperty (program -> getGlobalObject (), field, field -> getName (), index);
				break;
			}
			case X3D::inputOnly:
				break;
			case X3D::inputOutput:
			{
				defineProperty (program -> getGlobalObject (), field, field -> getName (),              index);
				defineProperty (program -> getGlobalObject (), field, field -> getName () + "_changed", index);
				break;
			}
		}
		
		++ index;
	}
}

void
Context::defineProperty (pb::ptr <pb::pbObject> const object,
                         X3DFieldDefinition* const field,
                         const std::string & name,
                         const size_t index)
{
	using namespace std::placeholders;

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		case X3D::X3DConstants::SFDouble:
		case X3D::X3DConstants::SFFloat:
		case X3D::X3DConstants::SFInt32:
		case X3D::X3DConstants::SFNode:
		case X3D::X3DConstants::SFString:
		case X3D::X3DConstants::SFTime:
		{
			object -> defineOwnProperty (name,
			                             pb::undefined,
			                             pb::NONE,
			                             new pb::NativeFunction (program, field -> getName (), std::bind (&Context::getBuildInProperty, this, _1, _2, _3, field), 0),
			                             new pb::NativeFunction (program, field -> getName (), std::bind (&Context::setProperty,        this, _1, _2, _3, field), 1));
			return;
		}
		default:
		{
			object -> defineOwnProperty (name,
			                             pb::undefined,
			                             pb::NONE,
			                             new pb::NativeFunction (program, field -> getName (), std::bind (&Context::getProperty, this, _1, _2, _3, std::ref (values [index])), 0),
			                             new pb::NativeFunction (program, field -> getName (), std::bind (&Context::setProperty, this, _1, _2, _3, field), 1));
			return;
		}
	}
}

bool
Context::resolve (pb::pbObject* const object, const pb::Identifier & identifier)
{
	//__LOG__ << identifier << std::endl;

	static const std::map <pb::Identifier, ObjectType> types = {
		// Fields
		std::pair ("X3DField",      X3DField::getType ()),
		std::pair ("X3DArrayField", X3DArrayField::getType ()),

		// Fields
		std::pair ("SFColor",       SFColor::getType ()),
		std::pair ("SFColorRGBA",   SFColorRGBA::getType ()),
		std::pair ("SFImage",       SFImage::getType ()),
		std::pair ("SFMatrix3d",    SFMatrix3d::getType ()),
		std::pair ("SFMatrix3f",    SFMatrix3f::getType ()),
		std::pair ("SFMatrix4d",    SFMatrix4d::getType ()),
		std::pair ("SFMatrix4f",    SFMatrix4f::getType ()),
		std::pair ("SFNode",        SFNode::getType ()),
		std::pair ("SFRotation",    SFRotation::getType ()),
		std::pair ("SFVec2d",       SFVec2d::getType ()),
		std::pair ("SFVec2f",       SFVec2f::getType ()),
		std::pair ("SFVec3d",       SFVec3d::getType ()),
		std::pair ("SFVec3f",       SFVec3f::getType ()),
		std::pair ("SFVec4d",       SFVec4d::getType ()),
		std::pair ("SFVec4f",       SFVec4f::getType ()),

		// Array Fields
		std::pair ("MFBool",        MFBool::getType ()),
		std::pair ("MFColor",       MFColor::getType ()),
		std::pair ("MFColorRGBA",   MFColorRGBA::getType ()),
		std::pair ("MFDouble",      MFDouble::getType ()),
		std::pair ("MFFloat",       MFFloat::getType ()),
		std::pair ("MFImage",       MFImage::getType ()),
		std::pair ("MFInt32",       MFInt32::getType ()),
		std::pair ("MFMatrix3d",    MFMatrix3d::getType ()),
		std::pair ("MFMatrix3f",    MFMatrix3f::getType ()),
		std::pair ("MFMatrix4d",    MFMatrix4d::getType ()),
		std::pair ("MFMatrix4f",    MFMatrix4f::getType ()),
		std::pair ("MFNode",        MFNode::getType ()),
		std::pair ("MFString",      MFString::getType ()),
		std::pair ("MFTime",        MFTime::getType ()),
		std::pair ("MFRotation",    MFRotation::getType ()),
		std::pair ("MFVec2d",       MFVec2d::getType ()),
		std::pair ("MFVec2f",       MFVec2f::getType ()),
		std::pair ("MFVec3d",       MFVec3d::getType ()),
		std::pair ("MFVec3f",       MFVec3f::getType ()),
		std::pair ("MFVec4d",       MFVec4d::getType ()),
		std::pair ("MFVec4f",       MFVec4f::getType ()),
		
		// VrmlMatrix
		std::pair ("VrmlMatrix",    VrmlMatrix::getType ())
	};


	const auto iter = types .find (identifier);
	
	if (iter == types .end ())
		return false;

	object -> addOwnProperty (identifier, getClass (iter -> second), pb::WRITABLE | pb::CONFIGURABLE);
	return true;
}

///  throws std::out_of_range
const pb::ptr <pb::NativeFunction> &
Context::getClass (const ObjectType type) const
{
	using Initialize = std::function <pb::ptr <pb::NativeFunction> (Context* const, const pb::ptr <pb::Program> &)>;

	static const std::vector <Initialize> functions = {
		// Fields
		X3DField::initialize,
		X3DArrayField::initialize,

		// Fields
		SFColor::initialize,
		SFColorRGBA::initialize,
		SFImage::initialize,
		SFMatrix3d::initialize,
		SFMatrix3f::initialize,
		SFMatrix4d::initialize,
		SFMatrix4f::initialize,
		SFNode::initialize,
		SFRotation::initialize,
		SFVec2d::initialize,
		SFVec2f::initialize,
		SFVec3d::initialize,
		SFVec3f::initialize,
		SFVec4d::initialize,
		SFVec4f::initialize,

		// Array Fields
		MFBool::initialize,
		MFColor::initialize,
		MFColorRGBA::initialize,
		MFDouble::initialize,
		MFFloat::initialize,
		MFImage::initialize,
		MFInt32::initialize,
		MFMatrix3d::initialize,
		MFMatrix3f::initialize,
		MFMatrix4d::initialize,
		MFMatrix4f::initialize,
		MFNode::initialize,
		MFString::initialize,
		MFTime::initialize,
		MFRotation::initialize,
		MFVec2d::initialize,
		MFVec2f::initialize,
		MFVec3d::initialize,
		MFVec3f::initialize,
		MFVec4d::initialize,
		MFVec4f::initialize,

		// VrmlMatrix
		VrmlMatrix::initialize
	};

	auto & standardClass = classes [size_t (type)];

	if (standardClass)
		return standardClass;

	return standardClass = functions .at (size_t (type)) (const_cast <Context*> (this), program);
}

///  throws std::invalid_argument
void
Context::addObject (X3DChildObject* const key, X3D::X3DFieldDefinition* const field, pb::pbObject* const object)
{
	assert (objects .emplace (key, object) .second);

	field -> addParent (this);
}

void
Context::removeObject (X3DChildObject* const key, X3D::X3DFieldDefinition* const field)
noexcept (true)
{
	if (objects .erase (key))
		field -> removeParent (this);
	
	else
		__LOG__ << field -> getTypeName () << " : " << field -> getName () << std::endl;
}

pb::pbObject*
Context::getObject (X3DChildObject* const key) const
noexcept (true)
{
	const auto iter = objects .find (key);

	if (iter not_eq objects .end ())
		return iter -> second;

	return nullptr;
}

pb::var
Context::setProperty (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, X3D::X3DFieldDefinition* const field)
{
	setValue (field, args [0]);

	return pb::undefined;
}

pb::var
Context::getBuildInProperty (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, X3D::X3DFieldDefinition* const field)
{
	return getValue (this, field);
}

pb::var
Context::getProperty (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const pb::var & value)
{
	return value;
}

void
Context::initialize ()
{
	//__LOG__ << X3D::SFTime (SFTime::now ()) << std::endl;

	X3DJavaScriptContext::initialize ();

	shutdown () .addInterest (&Context::set_shutdown, this);

	set_live ();

	try
	{
		program -> getFunctionDeclaration ("initialize") -> call (program -> getGlobalObject ());
	}
	catch (const pb::pbError & error)
	{
		setError (error);
	}
	catch (const std::exception & error)
	{ }

	//__LOG__ << X3D::SFTime (SFTime::now ()) << std::endl;
}

void
Context::setExecutionContext (X3D::X3DExecutionContext* const executionContext)
{
	if (future)
		future -> setExecutionContext (executionContext);

	X3DJavaScriptContext::setExecutionContext (executionContext);
}

void
Context::set_live ()
{
	if (program -> hasFunctionDeclaration ("prepareEvents"))
		getBrowser () -> prepareEvents () .addInterest (&Context::prepareEvents, this);

	if (program -> hasFunctionDeclaration ("eventsProcessed"))
		getScriptNode () -> addInterest (&Context::eventsProcessed, this);

	getScriptNode () -> addInterest (&Context::finish, this);

	for (const auto & field : getScriptNode () -> getUserDefinedFields ())
	{
		switch (field -> getAccessType ())
		{
			case inputOnly:
			{
				const pb::Identifier identifier (field -> getName ());

				if (program -> hasFunctionDeclaration (identifier))
					field -> addInterest (&Context::set_field, this, field, identifier);

				break;
			}
			case inputOutput:
			{
				const pb::Identifier identifier ("set_" + field -> getName ());

				if (program -> hasFunctionDeclaration (identifier))
					field -> addInterest (&Context::set_field, this, field, identifier);

				break;
			}
			default:
				break;
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
	catch (const pb::pbError & error)
	{
		setError (error);
	}
	catch (const std::exception & error)
	{ }
}

void
Context::set_field (X3D::X3DFieldDefinition* const field, const pb::Identifier & identifier)
{
	field -> setTainted (true);

	try
	{
		const auto & function = program -> getFunctionDeclaration (identifier);

		function -> call (program -> getGlobalObject (),
		                  {
		                     getValue (this, field),
		                     getCurrentTime ()
								});
	}
	catch (const pb::pbError & error)
	{
		setError (error);
	}
	catch (const std::exception & error)
	{ }

	field -> setTainted (false);
}

void
Context::eventsProcessed ()
{
	try
	{
		program -> getFunctionDeclaration ("eventsProcessed") -> call (program -> getGlobalObject ());
	}
	catch (const pb::pbError & error)
	{
		setError (error);
	}
	catch (const std::exception & error)
	{ }
}

void
Context::finish ()
{
}

void
Context::set_shutdown ()
{
	const auto p = program .get ();

	classes .clear ();
	values .clear ();
	program .dispose ();

	//pb::debug_roots (p);
	//__LOG__ << p -> getParents () .size () << std::endl;
	//__LOG__ << objects .size () << std::endl;
	assert (p -> getParents () .empty ());
	assert (objects .empty ());
}

void
Context::connectEventsProcessed ()
{
	getScriptNode () -> removeInterest (&Context::connectEventsProcessed, this);
	getScriptNode () -> addInterest (&Context::eventsProcessed, this);
}

void
Context::setError (const pb::pbError & error) const
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
	future .setValue (nullptr);

	X3D::X3DJavaScriptContext::dispose ();
}

Context::~Context ()
{ }

} // peaseblossom
} // X3D
} // titania
