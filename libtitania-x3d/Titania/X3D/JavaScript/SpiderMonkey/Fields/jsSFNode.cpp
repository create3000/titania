/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "jsSFNode.h"

#include "../../../Browser/Browser/X3DBrowser.h"
#include "../../../Components/Scripting/Script.h"
#include "../../../Execution/Scene.h"
#include "../../../InputOutput/Loader.h"
#include "../jsContext.h"
#include "../jsFieldDefinitionArray.h"
#include "../jsFields.h"
#include "../jsString.h"
#include "../jsfield.h"

namespace titania {
namespace X3D {

JSClass jsSFNode::static_class = {
	"SFNode", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, getProperty, setProperty,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSFunctionSpec jsSFNode::functions [ ] = {
	{ "getName",             getName <X3DChildObject>,     0, 0 },
	{ "getTypeName",         getTypeName <X3DChildObject>, 0, 0 },
	{ "getType",             getType,                      0, 0 },
	{ "isReadable",          isReadable,                   0, 0 },
	{ "isWritable",          isWritable,                   0, 0 },

	{ "getNodeName",         getNodeName,         0, 0 },
	{ "getNodeType",         getNodeType,         0, 0 },
	{ "getFieldDefinitions", getFieldDefinitions, 0, 0 },

	{ "toVRMLString",        toVRMLString,        0, 0 },
	{ "toXMLString",         toXMLString,         0, 0 },
	{ "toString",            toString,            0, 0 },

	{ 0 }

};

void
jsSFNode::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, NULL, functions, NULL, NULL);
}

JSBool
jsSFNode::create (JSContext* const context, SFNode* const field, jsval* const vp)
{
	const auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));

	try
	{
		*vp = OBJECT_TO_JSVAL (javaScript -> getObject (field));
	}
	catch (const std::out_of_range &)
	{
		JSObject* const result = JS_NewObject (context, &static_class, NULL, NULL);

		if (result == NULL)
			return JS_FALSE;

		JS_SetPrivate (context, result, field);

		javaScript -> addObject (field, result);

		*vp = OBJECT_TO_JSVAL (result);
	}

	return JS_TRUE;
}

JSBool
jsSFNode::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new SFNode (), &JS_RVAL (context, vp));
	}
	else if (argc == 1)
	{
		JSString* vrmlSyntax;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "S", &vrmlSyntax))
			return JS_FALSE;

		try
		{
			Script* const script = static_cast <jsContext*> (JS_GetContextPrivate (context)) -> getNode ();

			const ScenePtr scene = Loader (script -> getExecutionContext (), script -> getWorldURL ())
			                               .createX3DFromString (JS_GetString (context, vrmlSyntax));

			return create (context,
			               scene and not scene -> getRootNodes () .empty ()
			               ? new SFNode (scene -> getRootNodes () [0])
								: new SFNode (),
			               &JS_RVAL (context, vp));
		}
		catch (const X3DError & error)
		{
			JS_ReportError (context, error .what ());
			return JS_FALSE;
		}
	}

	JS_ReportError (context, "SFNode .construct: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, obj);

	if (not sfnode or not * sfnode or sfnode -> getValue () -> getFieldDefinitions () .empty ())
	{
		*statep = JSVAL_NULL;
		return JS_TRUE;
	}

	size_t* index;

	switch (enum_op)
	{
		case JSENUMERATE_INIT:
		case JSENUMERATE_INIT_ALL:
		{
			index   = new size_t (0);
			*statep = PRIVATE_TO_JSVAL (index);

			if (idp)
				*idp = INT_TO_JSID (sfnode -> getValue () -> getFieldDefinitions () .size ());

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < sfnode -> getValue () -> getFieldDefinitions () .size ())
			{
				X3DFieldDefinition* field = sfnode -> getValue () -> getFieldDefinitions () [*index];
				const std::string & name  = field -> getName ();

				jsval id;
				JS_NewStringValue (context, name, &id);

				if (idp)
					JS_ValueToId (context, id, idp);

				*index = *index + 1;
				break;
			}

			//else done -- cleanup.
		}
		case JSENUMERATE_DESTROY:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);
			delete index;
			*statep = JSVAL_NULL;
		}
	}

	return JS_TRUE;
}

JSBool
jsSFNode::getProperty (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	if (JSID_IS_STRING (id))
	{
		SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, obj);

		if (sfnode -> getValue ())
		{
			try
			{
				X3DFieldDefinition* const field = sfnode -> getValue () -> getField (JS_GetString (context, id));

				if (field -> getAccessType () == inputOnly)
				{
					*vp = JSVAL_VOID;
					return JS_TRUE;
				}

				return JS_NewFieldValue (context, field, vp);
			}
			catch (const Error <INVALID_NAME> &)
			{ }
		}
	}

	return JS_TRUE;
}

JSBool
jsSFNode::setProperty (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	if (JSID_IS_STRING (id))
	{
		SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, obj);

		if (sfnode -> getValue ())
		{
			try
			{
				X3DFieldDefinition* const field = sfnode -> getValue () -> getField (JS_GetString (context, id));

				if (field -> getAccessType () == initializeOnly or field -> getAccessType () == outputOnly)
					return JS_TRUE;

				if (not JS_ValueToField (context, field, vp))
					return JS_FALSE;

				*vp = JSVAL_VOID;

				return JS_TRUE;
			}
			catch (const Error <INVALID_NAME> &)
			{ }
		}
	}

	return JS_TRUE;
}

JSBool
jsSFNode::getNodeName (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return JS_NewStringValue (context, *sfnode ? sfnode -> getValue () -> getName () : "", &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "SFNode .getNodeName: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::getNodeType (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* result = nullptr;

		if (*sfnode)
		{
			X3DBaseNode* const node = sfnode -> getValue ();

			jsval array [node -> getNodeType () .size ()];

			for (size_t i = 0, size = node -> getNodeType () .size (); i < size; ++ i)
				if (not JS_NewNumberValue (context, (size_t) node -> getNodeType () [i], &array [i]))
					return JS_FALSE;

			result = JS_NewArrayObject (context, node -> getNodeType () .size (), array);
		}
		else
		{
			result = JS_NewArrayObject (context, 0, NULL);
		}

		JS_SET_RVAL (context, vp, OBJECT_TO_JSVAL (result));

		return JS_TRUE;
	}

	JS_ReportError (context, "SFNode .getNodeType: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::getFieldDefinitions (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		if (*sfnode)
			return jsFieldDefinitionArray::create (context, &sfnode -> getValue () -> getFieldDefinitions (), &JS_RVAL (context, vp));

		else
			return jsFieldDefinitionArray::create (context, NULL, &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "SFNode .getFieldDefinitions: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::toVRMLString (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Script* const script = static_cast <jsContext*> (JS_GetContextPrivate (context)) -> getNode ();

		Generator::Version (script -> getExecutionContext () -> getVersion ());
		Generator::NicestStyle ();

		SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return JS_NewStringValue (context, sfnode -> toString (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "SFNode .toVRMLString: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::toXMLString (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Script* const script = static_cast <jsContext*> (JS_GetContextPrivate (context)) -> getNode ();

		auto version = script -> getExecutionContext () -> getVersion ();

		if (version == VRML_V2_0)
			version = LATEST_VERSION;

		Generator::Version (version);
		Generator::NicestStyle ();

		SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return JS_NewStringValue (context, sfnode -> toXMLString (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "SFNode .toXMLString: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::toString (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		SFNode* const sfnode = (SFNode*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		if (*sfnode)
			return JS_NewStringValue (context, sfnode -> getValue () -> getTypeName () + " { }", &JS_RVAL (context, vp));

		else
			return JS_NewStringValue (context, "NULL", &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "SFNode .toString: wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // titania
