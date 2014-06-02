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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX4_H__

#include "../../../Fields/SFMatrix4.h"
#include "../../../Fields/VrmlMatrix.h"
#include "../jsContext.h"
#include "../jsX3DField.h"
#include "../jsError.h"
#include "jsSFRotation.h"
#include "jsSFVec3.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <class Type>
class jsSFMatrix4 :
	public jsX3DField
{
public:

	typedef Type field_type;

	typedef jsSFVec3 <typename Type::vector3_type> vector3_type;
	typedef jsSFRotation                           rotation_type;

	typedef typename jsSFVec3 <typename Type::vector3_type>::field_type vector3_field_type;
	typedef typename jsSFRotation::field_type                           rotation_field_type;

	static
	void
	init (JSContext* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, Type*, jsval* const);

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
	static JSBool multDirMatrix (JSContext *, uintN, jsval*);
	static JSBool multMatrixDir (JSContext *, uintN, jsval*);

	static const size_t   size;
	static JSClass        static_class;
	static JSFunctionSpec functions [ ];

};

template <class Type>
const size_t jsSFMatrix4 <Type>::size = 16;

template <class Type>
JSClass jsSFMatrix4 <Type>::static_class = {
	"Type", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, resolve, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type>
JSFunctionSpec jsSFMatrix4 <Type>::functions [ ] = {
	{ "getName",       getName <X3DChildObject>,     0, 0 },
	{ "getTypeName",   getTypeName <X3DChildObject>, 0, 0 },
	{ "getType",       getType,                      0, 0 },
	{ "isReadable",    isReadable,                   0, 0 },
	{ "isWritable",    isWritable,                   0, 0 },

	{ "setTransform",  setTransform,  5, 0 },
	{ "getTransform",  getTransform,  3, 0 },

	{ "transpose",     transpose,     0, 0 },
	{ "inverse",       inverse,       0, 0 },
	{ "multLeft",      multLeft,      1, 0 },
	{ "multRight",     multRight,     1, 0 },
	{ "multVecMatrix", multVecMatrix, 1, 0 },
	{ "multMatrixVec", multMatrixVec, 1, 0 },
	{ "multDirMatrix", multDirMatrix, 1, 0 },
	{ "multMatrixDir", multMatrixDir, 1, 0 },

	{ "toString",      toString <X3DChildObject>, 0, 0 },

	{ 0 }

};

template <class Type>
void
jsSFMatrix4 <Type>::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, NULL, functions, NULL, NULL);
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::create (JSContext* const context, Type* const field, jsval* const vp)
{
	auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));

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

template <class Type>
JSBool
jsSFMatrix4 <Type>::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new Type (), &JS_RVAL (context, vp));
	}
	else if (argc == size)
	{
		jsdouble m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "dddddddddddddddd",
		                             &m11, &m12, &m13, &m14, &m21, &m22, &m23, &m24, &m31, &m32, &m33, &m34, &m41, &m42, &m43, &m44))
			return JS_FALSE;

		return create (context, new Type (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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
jsSFMatrix4 <Type>::resolve (JSContext* context, JSObject* obj, jsid id)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	const size_t index = JSID_TO_INT (id);

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
jsSFMatrix4 <Type>::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	Type* const self = (Type*) JS_GetPrivate (context, obj);

	return JS_NewNumberValue (context, self -> get1Value (JSID_TO_INT (id)), vp);
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	Type* const self = (Type*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	self -> set1Value (JSID_TO_INT (id), value);

	return JS_TRUE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::setTransform (JSContext* context, uintN argc, jsval* vp)
{
	if (argc >= 0 or argc <= 5)
	{
		Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* translationObj, * rotationObj, * scaleObj, * scaleOrientationObj, * centerObj;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "/ooooo", &translationObj, &rotationObj, &scaleObj, &scaleOrientationObj, &centerObj))
			return JS_FALSE;

		if (argc >= 1)
		{
			if (JS_InstanceOfError (context, translationObj, vector3_type::getClass ()))
				return JS_FALSE;

			vector3_field_type* const translation = (vector3_field_type*) JS_GetPrivate (context, translationObj);

			if (argc >= 2)
			{
				if (JS_InstanceOfError (context, rotationObj, jsSFRotation::getClass ()))
					return JS_FALSE;

				rotation_field_type* const rotation = (rotation_field_type*) JS_GetPrivate (context, rotationObj);

				if (argc >= 3)
				{
					if (JS_InstanceOfError (context, scaleObj, vector3_type::getClass ()))
						return JS_FALSE;

					vector3_field_type* const scale = (vector3_field_type*) JS_GetPrivate (context, scaleObj);

					if (argc >= 4)
					{
						if (JS_InstanceOfError (context, scaleOrientationObj, jsSFRotation::getClass ()))
							return JS_FALSE;

						rotation_field_type* const scaleOrientation = (rotation_field_type*) JS_GetPrivate (context, scaleOrientationObj);

						if (argc >= 5)
						{
							if (JS_InstanceOfError (context, centerObj, vector3_type::getClass ()))
								return JS_FALSE;

							vector3_field_type* const center = (vector3_field_type*) JS_GetPrivate (context, centerObj);

							self -> setTransform (*translation, *rotation, *scale, *scaleOrientation, *center);
						}
						else
						{
							self -> setTransform (*translation, *rotation, *scale, *scaleOrientation);
						}
					}
					else
					{
						self -> setTransform (*translation, *rotation, *scale);
					}
				}
				else
				{
					self -> setTransform (*translation, *rotation);
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
jsSFMatrix4 <Type>::getTransform (JSContext* context, uintN argc, jsval* vp)
{
	if (argc < 5)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* translationObj      = nullptr;
		JSObject* rotationObj         = nullptr;
		JSObject* scaleObj            = nullptr;
		JSObject* scaleOrientationObj = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "/oooo", &translationObj, &rotationObj, &scaleObj, &scaleOrientationObj))
			return JS_FALSE;

		vector3_field_type*  translation      = nullptr;
		rotation_field_type* rotation         = nullptr;
		vector3_field_type*  scale            = nullptr;
		rotation_field_type* scaleOrientation = nullptr;

		bool noTranslation      = false;
		bool noRotation         = false;
		bool noScale            = false;
		bool noScaleOrientation = false;

		if (translationObj)
		{
			if (JS_InstanceOfError (context, translationObj, vector3_type::getClass ()))
				return JS_FALSE;
				
			else
				translation = (vector3_field_type*) JS_GetPrivate (context, translationObj);
		}
		else if (argc > 0)
		{
			translation   = new vector3_field_type ();
			noTranslation = true;
		}

		if (rotationObj)
		{
			if (JS_InstanceOfError (context, rotationObj, jsSFRotation::getClass ()))
			{
				if (noTranslation)
					delete translation;

				return JS_FALSE;
			}
			else
				rotation = (rotation_field_type*) JS_GetPrivate (context, rotationObj);
		}
		else if (argc > 1)
		{
			rotation   = new rotation_field_type ();
			noRotation = true;
		}

		if (scaleObj)
		{
			if (JS_InstanceOfError (context, scaleObj, vector3_type::getClass ()))
			{
				if (noTranslation)
					delete translation;

				if (noRotation)
					delete rotation;

				return JS_FALSE;
			}
			else
				scale = (vector3_field_type*) JS_GetPrivate (context, scaleObj);
		}
		else if (argc > 2)
		{
			scale   = new vector3_field_type ();
			noScale = true;
		}

		if (scaleOrientationObj)
		{
			if (JS_InstanceOfError (context, scaleObj, vector3_type::getClass ()))
			{
				if (noTranslation)
					delete translation;

				if (noRotation)
					delete rotation;

				if (noScale)
					delete scale;

				return JS_FALSE;
			}
			else
				scaleOrientation = (rotation_field_type*) JS_GetPrivate (context, scaleOrientationObj);
		}
		else if (argc > 3)
		{
			scaleOrientation   = new rotation_field_type ();
			noScaleOrientation = true;
		}

		switch (argc)
		{
			case 0:
				break;
			case 1:
				self -> getTransform (*translation);
				break;
			case 2:
				self -> getTransform (*translation, *rotation);
				break;
			case 3:
				self -> getTransform (*translation, *rotation, *scale);
				break;
			case 4:
				self -> getTransform (*translation, *rotation, *scale, *scaleOrientation);
				break;
		}

		if (noTranslation)
			delete translation;

		if (noRotation)
			delete rotation;

		if (noScale)
			delete scale;

		if (noScaleOrientation)
			delete scaleOrientation;

		JS_SET_RVAL (context, vp, JSVAL_VOID);

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::transpose (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return create (context, self -> transpose (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::inverse (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		try
		{
			Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

			return create (context, self -> inverse (), &JS_RVAL (context, vp));
		}
		catch (const std::domain_error & error)
		{
			JS_ReportError (context, "%s .inverse: %s", getClass () -> name, error .what ());
			return JS_FALSE;
		}
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multLeft (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (JS_InstanceOfError (context, rhs, getClass ()))
			return JS_FALSE;

		Type* const matrix = (Type*) JS_GetPrivate (context, rhs);

		return create (context, self -> multLeft (*matrix), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multRight (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (JS_InstanceOfError (context, rhs, getClass ()))
			return JS_FALSE;

		Type* const matrix = (Type*) JS_GetPrivate (context, rhs);

		return create (context, self -> multRight (*matrix), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multVecMatrix (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (JS_InstanceOfError (context, rhs, vector3_type::getClass ()))
			return JS_FALSE;

		vector3_field_type* const vector = (vector3_field_type*) JS_GetPrivate (context, rhs);

		return vector3_type::create (context, self -> multVecMatrix (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multMatrixVec (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (JS_InstanceOfError (context, rhs, vector3_type::getClass ()))
			return JS_FALSE;

		vector3_field_type* const vector = (vector3_field_type*) JS_GetPrivate (context, rhs);

		return vector3_type::create (context, self -> multMatrixVec (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multDirMatrix (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (JS_InstanceOfError (context, rhs, vector3_type::getClass ()))
			return JS_FALSE;

		vector3_field_type* const vector = (vector3_field_type*) JS_GetPrivate (context, rhs);

		return vector3_type::create (context, self -> multDirMatrix (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multMatrixDir (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* const self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (JS_InstanceOfError (context, rhs, vector3_type::getClass ()))
			return JS_FALSE;

		vector3_field_type* const vector = (vector3_field_type*) JS_GetPrivate (context, rhs);

		return vector3_type::create (context, self -> multMatrixDir (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

extern template class jsSFMatrix4 <SFMatrix4d>;
extern template class jsSFMatrix4 <SFMatrix4f>;
extern template class jsSFMatrix4 <VrmlMatrix>;

typedef jsSFMatrix4 <SFMatrix4d> jsSFMatrix4d;
typedef jsSFMatrix4 <SFMatrix4f> jsSFMatrix4f;
typedef jsSFMatrix4 <VrmlMatrix> jsVrmlMatrix;

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif
