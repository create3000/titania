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

#include "jsArrayFields.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <>
JSClass jsX3DArrayField <jsSFBool, X3D::MFBool>::static_class = {
	"MFBool",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFColor, X3D::MFColor>::static_class = {
	"MFColor",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFColorRGBA, X3D::MFColorRGBA>::static_class = {
	"MFColorRGBA",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFDouble, X3D::MFDouble>::static_class = {
	"MFDouble",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFFloat, X3D::MFFloat>::static_class = {
	"MFFloat",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFInt32, X3D::MFInt32>::static_class = {
	"MFInt32",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFImage, X3D::MFImage>::static_class = {
	"MFImage",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFMatrix3d, X3D::MFMatrix3d>::static_class = {
	"MFMatrix3d",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFMatrix3f, X3D::MFMatrix3f>::static_class = {
	"MFMatrix3f",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFMatrix4d, X3D::MFMatrix4d>::static_class = {
	"MFMatrix4d",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFMatrix4f, X3D::MFMatrix4f>::static_class = {
	"MFMatrix4f",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFNode, X3D::MFNode>::static_class = {
	"MFNode",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFRotation, X3D::MFRotation>::static_class = {
	"MFRotation",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFString, X3D::MFString>::static_class = {
	"MFString",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFTime, X3D::MFTime>::static_class = {
	"MFTime",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFVec2d, X3D::MFVec2d>::static_class = {
	"MFVec2d",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFVec2f, X3D::MFVec2f>::static_class = {
	"MFVec2f",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFVec3d, X3D::MFVec3d>::static_class = {
	"MFVec3d",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFVec3f, X3D::MFVec3f>::static_class = {
	"MFVec3f",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFVec4d, X3D::MFVec4d>::static_class = {
	"MFVec4d",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass jsX3DArrayField <jsSFVec4f, X3D::MFVec4f>::static_class = {
	"MFVec4f",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	get1Value,
	set1Value,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc == 0)
		{
			JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new X3D::MFNode ()));
			return true;
		}
		else
		{
			const auto args  = JS::CallArgsFromVp (argc, vp);
			const auto array = new X3D::MFNode ();

			for (unsigned i = 0; i < argc; ++ i)
			{
				try
				{
					array -> emplace_back (get1Argument (cx, args, i));
				}
				catch (const std::domain_error &)
				{
					array -> emplace_back ();
				}	
			}

			args .rval () .set (create (cx, array));
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .new: %s.", getClass () -> name, error .what ());
	}
}

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JSBool strict, JS::MutableHandleValue vp)
{
	try
	{
		if (not JSID_IS_INT (id))
			return true;

		const auto array = getThis <jsX3DArrayField <jsSFNode, X3D::MFNode>> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index < 0)
			return ThrowException (cx, "%s: array index out of range.", getClass () -> name);

		try
		{
			array -> set1Value (index, get1Argument (cx, vp .get (), 0));
		}
		catch (const std::domain_error &)
		{
			array -> set1Value (index, nullptr);
		}

		return true;
	}
	catch (const std::bad_alloc &)
	{
		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::unshift (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .unshift: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <jsX3DArrayField <jsSFNode, X3D::MFNode>> (cx, args);

		try
		{
			array -> emplace_front (get1Argument (cx, args, 0));
		}
		catch (const std::domain_error &)
		{
			array -> emplace_front (nullptr);
		}

		args .rval () .setDouble (array -> size ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .unshift: %s.", getClass () -> name, error .what ());
	}
}

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::push (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .push: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <jsX3DArrayField <jsSFNode, X3D::MFNode>> (cx, args);

		try
		{
			array -> emplace_back (get1Argument (cx, args, 0));
		}
		catch (const std::domain_error &)
		{
			array -> emplace_back (nullptr);
		}

		args .rval () .setNumber ((uint32_t) array -> size ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .push: %s.", getClass () -> name, error .what ());
	}
}

template class jsX3DArrayField <jsSFBool, X3D::MFBool>;
template class jsX3DArrayField <jsSFColor, X3D::MFColor>;
template class jsX3DArrayField <jsSFColorRGBA, X3D::MFColorRGBA>;
template class jsX3DArrayField <jsSFDouble, X3D::MFDouble>;
template class jsX3DArrayField <jsSFFloat, X3D::MFFloat>;
template class jsX3DArrayField <jsSFInt32, X3D::MFInt32>;
template class jsX3DArrayField <jsSFImage, X3D::MFImage>;
template class jsX3DArrayField <jsSFMatrix3d, X3D::MFMatrix3d>;
template class jsX3DArrayField <jsSFMatrix3f, X3D::MFMatrix3f>;
template class jsX3DArrayField <jsSFMatrix4d, X3D::MFMatrix4d>;
template class jsX3DArrayField <jsSFMatrix4f, X3D::MFMatrix4f>;
template class jsX3DArrayField <jsSFNode, X3D::MFNode>;
template class jsX3DArrayField <jsSFRotation, X3D::MFRotation>;
template class jsX3DArrayField <jsSFString, X3D::MFString>;
template class jsX3DArrayField <jsSFTime, X3D::MFTime>;
template class jsX3DArrayField <jsSFVec2d, X3D::MFVec2d>;
template class jsX3DArrayField <jsSFVec2f, X3D::MFVec2f>;
template class jsX3DArrayField <jsSFVec3d, X3D::MFVec3d>;
template class jsX3DArrayField <jsSFVec3f, X3D::MFVec3f>;
template class jsX3DArrayField <jsSFVec4d, X3D::MFVec4d>;
template class jsX3DArrayField <jsSFVec4f, X3D::MFVec4f>;

} // MozillaSpiderMonkey
} // X3D
} // titania
