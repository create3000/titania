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

#include "X3DField.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

const std::string X3DField::typeName = "X3DField";

pb::ptr <pb::NativeFunction>
X3DField::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), nullptr, nullptr, 0);
	const auto prototype = pb::make_ptr <pb::Object> (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	prototype -> addOwnProperty ("getName",     new pb::NativeFunction (ec, "getName",     getName,      0), pb::NONE);
	prototype -> addOwnProperty ("getTypeName", new pb::NativeFunction (ec, "getTypeName", getTypeName_, 0), pb::NONE);
	prototype -> addOwnProperty ("getType",     new pb::NativeFunction (ec, "getType",     getType_,     0), pb::NONE);
	prototype -> addOwnProperty ("toString",    new pb::NativeFunction (ec, "toString",    toString,     0), pb::NONE);

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

pb::var
X3DField::getName (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> &)
{
	try
	{
		return getThis <X3DField> (ec, object) -> getName ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError ("X3DField.prototype.getName is not generic.");
	}
}

pb::var
X3DField::getTypeName_ (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> &)
{
	try
	{
		return getThis <X3DField> (ec, object) -> getTypeName ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError ("X3DField.prototype.getTypeName is not generic.");
	}
}

pb::var
X3DField::getType_ (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> &)
{
	try
	{
		return (uint32_t) getThis <X3DField> (ec, object) -> getType ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError ("X3DField.prototype.getType is not generic.");
	}
}

pb::var
X3DField::toString (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> &)
{
	try
	{
		return getThis <X3DField> (ec, object) -> toString ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError ("X3DField.prototype.toString is not generic.");
	}
}

} // peaseblossom
} // X3D
} // titania
