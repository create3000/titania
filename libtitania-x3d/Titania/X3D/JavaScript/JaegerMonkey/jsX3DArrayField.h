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
 ******************************************************************************/

#ifndef __TITANIA_X3D_JAVA_SCRIPT_JAEGER_MONKEY_JS_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_JAEGER_MONKEY_JS_X3DARRAY_FIELD_H__

#include "../../Fields/ArrayFields.h"
#include "jsX3DField.h"

namespace titania {
namespace X3D {

template <class Type>
class jsX3DArrayField :
	public jsX3DField
{
protected:

	enum Property {LENGTH};
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];
	static JSBool         getLength (JSContext*, JSObject*, jsid, jsval*);
	static JSBool         setLength (JSContext*, JSObject*, jsid, JSBool, jsval*);

};

template <class Type>
JSPropertySpec jsX3DArrayField <Type>::properties [ ] = { { "length", LENGTH, JSPROP_SHARED | JSPROP_PERMANENT, getLength, setLength }, { 0 } };

template <class Type>
JSFunctionSpec jsX3DArrayField <Type>::functions [ ] = { { "toString", toString, 0, 0 },  { 0, 0, 0, 0 } };

template <class Type>
JSBool
jsX3DArrayField <Type>::getLength (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DArrayField <Type>* x3darrayfield = (X3DArrayField <Type>*)JS_GetPrivate (context, obj);

	return JS_NewNumberValue (context, x3darrayfield -> size (), vp);
}

template <class Type>
JSBool
jsX3DArrayField <Type>::setLength (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	X3DArrayField <Type>* x3darrayfield = (X3DArrayField <Type>*)JS_GetPrivate (context, obj);

	uint32 value;

	if (not JS_ValueToECMAUint32 (context, *vp, &value))
		return JS_FALSE;

	x3darrayfield -> resize (value);

	return JS_TRUE;
}

} // X3D
} // titania

#endif
