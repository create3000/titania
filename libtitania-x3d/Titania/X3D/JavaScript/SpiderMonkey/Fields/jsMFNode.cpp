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

#include "jsMFNode.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <>
JSClass jsX3DArrayField <jsSFNode, MFNode>::static_class = {
	"MFNode", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSBool
jsX3DArrayField <jsSFNode, MFNode>::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new field_type (), &JS_RVAL (context, vp));
	}
	else
	{
		field_type* const field = new field_type ();

		jsval* const argv = JS_ARGV (context, vp);

		for (uintN i = 0; i < argc; ++ i)
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, argv [i], &value))
				return JS_FALSE;
			
			if (value)
			{
				if (JS_InstanceOfError (context, value, jsSFNode::getClass ()))
					return JS_FALSE;

				field -> emplace_back (*(SFNode*) JS_GetPrivate (context, value));
			}
			else
				field -> emplace_back ();
		}

		return create (context, field, &JS_RVAL (context, vp));
	}

	return JS_FALSE;
}

template <>
JSBool
jsX3DArrayField <jsSFNode, MFNode>::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	const int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	MFNode* const field = (MFNode*) JS_GetPrivate (context, obj);

	JSObject* value = nullptr;

	if (not JS_ValueToObject (context, *vp, &value))
		return JS_FALSE;
		
	if (value)
	{
		if (JS_InstanceOfError (context, value, jsSFNode::getClass ()))
			return JS_FALSE;

		field -> set1Value (index, *(SFNode*) JS_GetPrivate (context, value));
	}
	else
		field -> set1Value (index, nullptr);

	*vp = JSVAL_VOID;

	return JS_TRUE;
}

template <>
JSBool
jsX3DArrayField <jsSFNode, MFNode>::unshift (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		MFNode* const field = (MFNode*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* value = nullptr;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &value))
			return JS_FALSE;
			
		if (value)
		{
			if (JS_InstanceOfError (context, value, jsSFNode::getClass ()))
				return JS_FALSE;

			field -> emplace_front (*(SFNode*) JS_GetPrivate (context, value));
		}
		else
			field -> emplace_front ();

		return JS_NewNumberValue (context, field -> size (), vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <>
JSBool
jsX3DArrayField <jsSFNode, MFNode>::push (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		MFNode* const field = (MFNode*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* value = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &value))
			return JS_FALSE;

		if (value)
		{
			if (JS_InstanceOfError (context, value, jsSFNode::getClass ()))
				return JS_FALSE;

			field -> emplace_back (*(SFNode*) JS_GetPrivate (context, value));
		}
		else
			field -> emplace_back ();

		return JS_NewNumberValue (context, field -> size (), vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template class jsX3DArrayField <jsSFNode, MFNode>;

} // MozillaSpiderMonkey
} // X3D
} // titania
