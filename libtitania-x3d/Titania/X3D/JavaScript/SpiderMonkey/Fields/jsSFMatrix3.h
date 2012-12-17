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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX3_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX3_H__

#include "../../../Fields/SFMatrix3.h"
#include "../jsX3DField.h"
#include "jsSFRotation.h"
#include "jsSFVec2.h"

namespace titania {
namespace X3D {

template <class Type>
class jsSFMatrix3 :
	public jsX3DField
{
public:

	typedef Type field_type;

	typedef jsSFVec2 <typename Type::vector2_type> vector2_type;
	typedef jsSFRotation                           rotation_type;

	typedef typename jsSFVec2 <typename Type::vector2_type>::field_type vector2_field_type;
	typedef typename jsSFRotation::field_type                           rotation_field_type;

	static
	void
	init (JSContext*, JSObject*);

	static
	JSBool
	create (JSContext*, Type*, jsval*, const bool = false);

	static
	JSClass*
	getClass () { return &static_class; }


private:

	static JSBool construct (JSContext *, uintN, jsval*);
	static JSBool enumerate (JSContext *, JSObject *, JSIterateOp, jsval *, jsid*);
	static JSBool resolve (JSContext * context, JSObject *, jsid);

	static JSBool get1Value (JSContext *, JSObject *, jsid, jsval*);
	static JSBool set1Value (JSContext *, JSObject *, jsid, JSBool, jsval*);

	static JSBool setTransform (JSContext *, uintN, jsval*);
	static JSBool getTransform (JSContext *, uintN, jsval*);

	static JSBool transpose     (JSContext *, uintN, jsval*);
	static JSBool inverse       (JSContext *, uintN, jsval*);
	static JSBool multLeft      (JSContext *, uintN, jsval*);
	static JSBool multRight     (JSContext *, uintN, jsval*);
	static JSBool multVecMatrix (JSContext *, uintN, jsval*);
	static JSBool multMatrixVec (JSContext *, uintN, jsval*);

	static const size_t   size;
	static JSClass        static_class;
	static JSFunctionSpec functions [ ];

};

template <class Type>
const size_t jsSFMatrix3 <Type>::size = 9;

template <class Type>
JSClass jsSFMatrix3 <Type>::static_class = {
	"Type", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, resolve, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type>
JSFunctionSpec jsSFMatrix3 <Type>::functions [ ] = {
	{ "getName",       getName <X3DChildObject>,     0, 0 },
	{ "getTypeName",   getTypeName <X3DChildObject>, 0, 0 },
	{ "getType",       getType <X3DFieldDefinition>, 0, 0 },

	{ "setTransform",  setTransform,  5, 0 },
	{ "getTransform",  getTransform,  3, 0 },

	{ "transpose",     transpose,     0, 0 },
	{ "inverse",       inverse,       0, 0 },
	{ "multLeft",      multLeft,      1, 0 },
	{ "multRight",     multRight,     1, 0 },
	{ "multVecMatrix", multVecMatrix, 1, 0 },
	{ "multMatrixVec", multMatrixVec, 1, 0 },

	{ "toString",      toString <X3DChildObject>, 0, 0 },

	{ 0 }

};

template <class Type>
void
jsSFMatrix3 <Type>::init (JSContext* context, JSObject* global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, NULL, functions, NULL, NULL);
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::create (JSContext* context, Type* field, jsval* vp, const bool seal)
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

template <class Type>
JSBool
jsSFMatrix3 <Type>::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new Type (), &JS_RVAL (context, vp));
	}
	else if (argc == size)
	{
		jsdouble m11, m12, m13, m21, m22, m23, m31, m32, m33;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "ddddddddd",
		                             &m11, &m12, &m13, &m21, &m22, &m23, &m31, &m32, &m33))
			return JS_FALSE;

		return create (context, new Type (m11, m12, m13, m21, m22, m23, m31, m32, m33), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	if (not JS_GetPrivate (context, obj))
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
				*idp = INT_TO_JSID (size);

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < size)
			{
				if (idp)
					*idp = INT_TO_JSID (*index);

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

template <class Type>
JSBool
jsSFMatrix3 <Type>::resolve (JSContext* context, JSObject* obj, jsid id)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	size_t index = JSID_TO_INT (id);

	if (index >= 0 and index < size)
	{
		// Define the indexed property
		JS_DefineProperty (context, obj, (char*) index, JSVAL_VOID, get1Value, set1Value, JSPROP_PERMANENT | JSPROP_INDEX);

		return JS_TRUE;
	}

	JS_ReportError (context, "index out of range");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	Type* self = (Type*) JS_GetPrivate (context, obj);

	return JS_NewNumberValue (context, self -> get1Value (JSID_TO_INT (id)), vp);
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	Type* self = (Type*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	self -> set1Value (JSID_TO_INT (id), value);

	return JS_TRUE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::setTransform (JSContext* context, uintN argc, jsval* vp)
{
	if (argc >= 0 or argc <= 5)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* translationObj, * rotationObj, * scaleObj, * scaleOrientationObj, * centerObj;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "/ooooo", &translationObj, &rotationObj, &scaleObj, &scaleOrientationObj, &centerObj))
			return JS_FALSE;

		if (argc >= 1)
		{
			if (not JS_InstanceOf (context, translationObj, vector2_type::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument 1 is invalid - should be %s, is %s",
				                vector2_type::getClass () -> name,
				                JS_GetClass (context, translationObj) -> name);

				return JS_FALSE;
			}

			vector2_field_type* translation = (vector2_field_type*) JS_GetPrivate (context, translationObj);

			if (argc >= 2)
			{
				if (not JS_InstanceOf (context, rotationObj, rotation_type::getClass (), NULL))
				{
					JS_ReportError (context, "Type of argument 2 is invalid - should be %s, is %s",
					                rotation_type::getClass () -> name,
					                JS_GetClass (context, rotationObj) -> name);

					return JS_FALSE;
				}

				//rotation_field_type* rotation = (rotation_field_type*) JS_GetPrivate (context, rotationObj);

				if (argc >= 3)
				{
					if (not JS_InstanceOf (context, scaleObj, vector2_type::getClass (), NULL))
					{
						JS_ReportError (context, "Type of argument 3 is invalid - should be %s, is %s",
						                vector2_type::getClass () -> name,
						                JS_GetClass (context, scaleObj) -> name);

						return JS_FALSE;
					}

					//vector2_field_type scale = (vector2_field_type*) JS_GetPrivate (context, scaleObj);

					if (argc >= 4)
					{
						if (not JS_InstanceOf (context, scaleOrientationObj, rotation_type::getClass (), NULL))
						{
							JS_ReportError (context, "Type of argument 4 is invalid - should be %s, is %s",
							                rotation_type::getClass () -> name,
							                JS_GetClass (context, scaleOrientationObj) -> name);

							return JS_FALSE;
						}

						//rotation_field_type* scaleOrientation = (rotation_field_type*) JS_GetPrivate (context, scaleOrientationObj);

						if (argc >= 5)
						{
							if (not JS_InstanceOf (context, centerObj, vector2_type::getClass (), NULL))
							{
								JS_ReportError (context, "Type of argument 5 is invalid - should be %s, is %s",
								                vector2_type::getClass () -> name,
								                JS_GetClass (context, centerObj) -> name);

								return JS_FALSE;
							}

							//vector2_field_type* center = (vector2_field_type*) JS_GetPrivate (context, centerObj);

							//self -> setTransform(*translation, *rotation, *scale, *scaleOrientation, *center);
						}
						else
						{
							//self -> setTransform(*translation, *rotation, *scale, *scaleOrientation);
						}
					}
					else
					{
						//self -> setTransform(*translation, *rotation, *scale);
					}
				}
				else
				{
					//self -> setTransform(*translation, *rotation);
				}
			}
			else
			{
				self -> setTransform (*translation);
			}
		}
		else
		{
			self -> setTransform ();
		}

		JS_SET_RVAL (context, vp, JSVAL_VOID);

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::getTransform (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 3)
	{
		//Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* translationObj, * rotationObj, * scaleObj;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "ooo", &translationObj, &rotationObj, &scaleObj))
			return JS_FALSE;

		if (not JS_InstanceOf (context, translationObj, vector2_type::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be %s, is %s",
			                vector2_type::getClass () -> name,
			                JS_GetClass (context, translationObj) -> name);

			return JS_FALSE;
		}

		if (not JS_InstanceOf (context, rotationObj, rotation_type::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be %s, is %s",
			                rotation_type::getClass () -> name,
			                JS_GetClass (context, rotationObj) -> name);

			return JS_FALSE;
		}

		if (not JS_InstanceOf (context, scaleObj, vector2_type::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be %s, is %s",
			                vector2_type::getClass () -> name,
			                JS_GetClass (context, scaleObj) -> name);

			return JS_FALSE;
		}

		//vector2_field_type*  translation = (vector2_field_type*) JS_GetPrivate (context, translationObj);
		//rotation_field_type* rotation    = (rotation_field_type*) JS_GetPrivate (context, rotationObj);
		//vector2_field_type*  scale       = (vector2_field_type*) JS_GetPrivate (context, scaleObj);

		//self -> getTransform(*translation, *rotation, * scale);

		JS_SET_RVAL (context, vp, JSVAL_VOID);

		return JS_TRUE;
	}

	return JS_TRUE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::transpose (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return create (context, self -> transpose (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::inverse (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return create (context, self -> inverse (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multLeft (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (not JS_InstanceOf (context, rhs, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be %s, is %s",
			                getClass () -> name,
			                JS_GetClass (context, rhs) -> name);

			return JS_FALSE;
		}

		Type* matrix = (Type*) JS_GetPrivate (context, rhs);

		return create (context, self -> multLeft (*matrix), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multRight (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (not JS_InstanceOf (context, rhs, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be %s, is %s",
			                getClass () -> name,
			                JS_GetClass (context, rhs) -> name);

			return JS_FALSE;
		}

		Type* matrix = (Type*) JS_GetPrivate (context, rhs);

		return create (context, self -> multRight (*matrix), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multVecMatrix (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (not JS_InstanceOf (context, rhs, vector2_type::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be %s, is %s",
			                vector2_type::getClass () -> name,
			                JS_GetClass (context, rhs) -> name);

			return JS_FALSE;
		}

		vector2_field_type* vector = (vector2_field_type*) JS_GetPrivate (context, rhs);

		return vector2_type::create (context, self -> multVecMatrix (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multMatrixVec (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (not JS_InstanceOf (context, rhs, vector2_type::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be %s, is %s",
			                vector2_type::getClass () -> name,
			                JS_GetClass (context, rhs) -> name);

			return JS_FALSE;
		}

		vector2_field_type* vector = (vector2_field_type*) JS_GetPrivate (context, rhs);

		return vector2_type::create (context, self -> multMatrixVec (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

extern template class jsSFMatrix3 <SFMatrix3d>;
extern template class jsSFMatrix3 <SFMatrix3f>;

typedef jsSFMatrix3 <SFMatrix3d> jsSFMatrix3d;
typedef jsSFMatrix3 <SFMatrix3f> jsSFMatrix3f;

} // X3D
} // titania

#endif
