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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DOBJECT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DOBJECT_H__

#include "../../Basal/X3DChildObject.h"
#include "jsString.h"
#include <jsapi.h>

namespace titania {
namespace X3D {

class jsX3DObject
{
protected:

	template <class Type>
	static JSBool getName (JSContext *, uintN, jsval*);

	template <class Type>
	static JSBool getTypeName (JSContext *, uintN, jsval*);

	template <class Type>
	static JSBool toString (JSContext *, uintN, jsval*);

};

template <class Type>
JSBool
jsX3DObject::getName (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		const Type* const value = static_cast <Type*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

		return JS_NewStringValue (context, value -> getName (), vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsX3DObject::getTypeName (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		const Type* const value = static_cast <Type*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

		return JS_NewStringValue (context, value -> getTypeName (), vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsX3DObject::toString (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Generator::NicestStyle ();

		const Type* const value = static_cast <Type*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

		return JS_NewStringValue (context, value -> toString (), vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

extern template JSBool jsX3DObject::getName <X3DChildObject>     (JSContext *, uintN, jsval*);
extern template JSBool jsX3DObject::getTypeName <X3DChildObject> (JSContext *, uintN, jsval*);
extern template JSBool jsX3DObject::toString <X3DChildObject>    (JSContext *, uintN, jsval*);

} // X3D
} // titania

#endif
