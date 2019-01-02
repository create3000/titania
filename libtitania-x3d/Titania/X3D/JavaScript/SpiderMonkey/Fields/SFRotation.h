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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDERMONKEY_FIELDS_SFROTATION_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDERMONKEY_FIELDS_SFROTATION_H__

#include "../X3DField.h"

#include "../../../Fields/SFRotation4.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

class SFRotation :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = X3D::SFRotation;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JS::Value
	create (JSContext* const cx, X3D::SFRotation* const field);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ return ObjectType::SFRotation; }


private:

	///  @name Member types

	enum Property {X, Y, Z, ANGLE};

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Member access

	template <size_t Index>
	static bool setProperty (JSContext* cx, unsigned argc, JS::Value* vp);

	template <size_t Index>
	static bool getProperty (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Functions

	static bool getAxis  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool setAxis  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool inverse  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multiply (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multVec  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool slerp    (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Static members

	static constexpr size_t Size = std::tuple_size <X3D::SFRotation::internal_type> ();

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

} // spidermonkey
} // X3D
} // titania

#endif
