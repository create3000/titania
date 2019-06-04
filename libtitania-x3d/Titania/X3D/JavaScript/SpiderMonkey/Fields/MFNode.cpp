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

#include "MFNode.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

template <>
const JSClass MFNode::static_class = {
	"MFNode",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

template <>
const JSFunctionSpec MFNode::functions [ ] = {
	JS_FN ("pop",          pop,          0, JSPROP_PERMANENT),
	JS_FN ("push",         push,         0, JSPROP_PERMANENT),
	JS_FN ("shift",        shift,        0, JSPROP_PERMANENT),
	JS_FN ("splice",       splice,       2, JSPROP_PERMANENT),
	JS_FN ("unshift",      unshift,      0, JSPROP_PERMANENT),
	JS_FN ("toString",     toString,     0, JSPROP_PERMANENT),
	JS_FN ("toVRMLString", toVRMLString, 0, JSPROP_PERMANENT),
	JS_FN ("toXMLString",  toXMLString,  0, JSPROP_PERMANENT),
	JS_FS_END
};

template <>
bool
MFNode::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = new X3D::MFNode ();

		for (unsigned i = 0; i < argc; ++ i)
		{
			try
			{
				array -> emplace_back (getArgument <SFNode> (cx, args, i));
			}
			catch (const std::domain_error & error)
			{
				array -> emplace_back ();
			}
		}

		args .rval () .set (create (cx, array));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, error .what ());
	}
}

template <>
bool
MFNode::set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::HandleValue vp, JS::ObjectOpResult & result)
{
	try
	{
		const auto array = getThis <MFNode> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index >= 0)
		{
			try
			{
				array -> set1Value (index, getArgument <SFNode> (cx, vp, 0));
			}
			catch (const std::domain_error & error)
			{
				array -> set1Value (index, nullptr);
			}
		}

		result .succeed ();
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: out of memory.", getClass () -> name, JSID_TO_INT (id));
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, JSID_TO_INT (id), error .what ());
	}
}

template <>
bool
MFNode::push (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <MFNode> (cx, args);

		for (unsigned i = 0; i < argc; ++ i)
		{
			try
			{
				array -> emplace_back (getArgument <SFNode> (cx, args, i));
			}
			catch (const std::domain_error & error)
			{
				array -> emplace_back ();
			}
		}

		args .rval () .setNumber (uint32_t (array -> size ()));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .push: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .push: %s.", getClass () -> name, error .what ());
	}
}

template <>
bool
MFNode::splice (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc < 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .splice: wrong number of arguments.", getClass () -> name);

		const auto args        = JS::CallArgsFromVp (argc, vp);
		const auto array       = getThis <MFNode> (cx, args);
		auto       index       = spidermonkey::getArgument <int32_t> (cx, args, 0);
		auto       deleteCount = spidermonkey::getArgument <int32_t> (cx, args, 1);
		auto       result      = new X3D::MFNode ();

		if (index > (int32_t) array -> size ())
			index = array -> size ();

		if (index < 0)
		{
			index = array -> size () + index;

			if (index < 0)
				index = 0;
		}

		if (deleteCount < 0)
			deleteCount = 0;

		if (index + deleteCount > (int32_t) array -> size ())
			deleteCount = array -> size () - index;

		result -> insert (result -> begin (), array -> cbegin () + index, array -> cbegin () + (index + deleteCount));
		array  -> erase (array -> begin () + index, array -> begin () + (index + deleteCount));

		for (ssize_t i = argc - 1; i >= 2; -- i)
		{
			try
			{
				array -> emplace (array -> begin () + index, getArgument <SFNode> (cx, args, i));
			}
			catch (const std::domain_error & error)
			{
				array -> emplace (array -> begin () + index, nullptr);
			}
		}

		args .rval () .set (create (cx, result));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .splice: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .splice: %s.", getClass () -> name, error .what ());
	}
}

template <>
bool
MFNode::unshift (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <MFNode> (cx, args);

		for (ssize_t i = argc - 1; i >= 0; -- i)
		{
			try
			{
				array -> emplace_front (getArgument <SFNode> (cx, args, i));
			}
			catch (const std::domain_error & error)
			{
				array -> emplace_front ();
			}
		}

		args .rval () .setNumber (uint32_t (array -> size ()));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .unshift: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .unshift: %s.", getClass () -> name, error .what ());
	}
}

template <>
bool
MFNode::toString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toString: wrong number of arguments.", getClass () -> name);

		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <MFNode> (cx, args);

		std::ostringstream ostream;

		switch (array -> size ())
		{
			case 0:
			{
				ostream << "[ ]";
				break;
			}
			case 1:
			{
				const auto & node = array -> front ();

				if (node)
					ostream << node -> getTypeName () + " { }";
				else
					ostream << "NULL" << std::endl;

				break;
			}
			default:
			{
				ostream
					<< '['
					<< '\n';

				for (const auto & node : *array)
				{
					ostream << "  ";

					if (node)
						ostream << node -> getTypeName () + " { }";
					else
						ostream << "NULL" << std::endl;

					ostream << '\n';
				}

				ostream << ']';

				break;
			}
		}

		args .rval () .set (StringValue (cx, ostream .str ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toString: %s.", getClass () -> name, error .what ());
	}
}

template <>
bool
MFNode::toVRMLString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toVRMLString: wrong number of arguments.", getClass () -> name);

		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <MFNode> (cx, args);

		args .rval () .set (StringValue (cx, array -> toString ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toVRMLString: %s.", getClass () -> name, error .what ());
	}
}

template <>
bool
MFNode::toXMLString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toXMLString: wrong number of arguments.", getClass () -> name);

		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <MFNode> (cx, args);

		args .rval () .set (StringValue (cx, array -> toXMLString ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toXMLString: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania
