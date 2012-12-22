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
 ******************************************************************************/

#include "jsSFNode.h"

#include "../../../Browser/Browser.h"
#include "../../../Components/Scripting/X3DScriptNode.h"
#include "../JavaScriptContext.h"
#include "../jsFieldDefinitionArray.h"
#include "../jsFields.h"
#include "../jsfield.h"
#include "../String.h"

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
	{ "getType",             getType <X3DFieldDefinition>, 0, 0 },
	
	{ "getNodeName",         getNodeName,         0, 0 },
	{ "getNodeType",         getNodeType,         0, 0 },
	{ "getFieldDefinitions", getFieldDefinitions, 0, 0 },
	
	{ "toVRMLString",        toVRMLString,        0, 0 },
	{ "toXMLString",         toXMLString,         0, 0 },
	{ "toString",            toString,            0, 0 },
	
	{ 0 }

};

void
jsSFNode::init (JSContext* context, JSObject* global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, NULL, functions, NULL, NULL);
}

JSBool
jsSFNode::create (JSContext* context, X3DField <X3DBaseNode*>* field, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, field);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsSFNode::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new SFNode <X3DBaseNode> (), &JS_RVAL (context, vp));
	}
	else if (argc == 1)
	{
		JSString* vrmlSyntax;
		
		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "S", &vrmlSyntax))
			return JS_FALSE;

		X3DScriptNode* script = static_cast <JavaScriptContext*> (JS_GetContextPrivate (context)) -> getNode (); // XXX

		SFNode <Scene> scene;

		try
		{
			scene = script -> createX3DFromString (JS_GetString (context, vrmlSyntax)); // XXX X3DBaseNode::createX3DFromString
		}
		catch (const X3DError & error)
		{
			std::cerr << "Warning Browser: " << std::endl << error .what () << std::endl;
		}

		// XXX save X3DExecutionContext for generated nodes.
		
		return create (context,
		               scene and scene -> getRootNodes () .size ()
		               ? new SFNode <X3DBaseNode> (scene -> getRootNodes () [0])
		               : new SFNode <X3DBaseNode> (),
		               &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, obj);

	if (not sfnode or not *sfnode or not sfnode -> getValue () -> getFieldDefinitions () .size ())
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
		X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, obj);

		if (sfnode -> getValue ())
		{
			X3DFieldDefinition* field = sfnode -> getValue () -> getField (JS_GetString (context, id));

			if (field)
			{
				if (field -> getAccessType () == inputOnly)
				{
					*vp = JSVAL_VOID;
					return JS_TRUE;
				}

				return JS_NewFieldValue (context, field, vp);
			}
		}
	}

	return JS_TRUE;
}

JSBool
jsSFNode::setProperty (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	if (JSID_IS_STRING (id))
	{
		X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, obj);

		if (sfnode -> getValue ())
		{
			X3DFieldDefinition* field = sfnode -> getValue () -> getField (JS_GetString (context, id));

			if (field)
			{
				if (field -> getAccessType () == initializeOnly or field -> getAccessType () == outputOnly)
					return JS_TRUE;

				if (not JS_ValueToField (context, field, vp))
					return JS_FALSE;

				*vp = JSVAL_VOID;

				return JS_TRUE;
			}
		}
	}

	return JS_TRUE;
}

JSBool
jsSFNode::getNodeName (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return JS_NewStringValue (context, *sfnode ? sfnode -> getValue () -> getName () : "", &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::getNodeType (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* result;

		if (*sfnode)
		{
			X3DBaseNode* node = sfnode -> getValue ();

			jsval array [node -> getNodeType () .size ()];

			NodeTypeArray::size_type i;

			for (i = 0; i < node -> getNodeType () .size (); ++ i)
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

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::getFieldDefinitions (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		if (*sfnode)
			return jsFieldDefinitionArray::create (context, &sfnode -> getValue () -> getFieldDefinitions (), &JS_RVAL (context, vp));
		else
			return jsFieldDefinitionArray::create (context, NULL, &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::toVRMLString (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return JS_NewStringValue (context, sfnode -> toString (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::toXMLString (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		//X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return JS_NewStringValue (context, "", &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFNode::toString (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DField <X3DBaseNode*>* sfnode = (X3DField <X3DBaseNode*>*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		if (*sfnode)
			return JS_NewStringValue (context, sfnode -> getValue () -> getTypeName () + " { }", &JS_RVAL (context, vp));
			
		else
			return JS_NewStringValue (context, "NULL", &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // titania
