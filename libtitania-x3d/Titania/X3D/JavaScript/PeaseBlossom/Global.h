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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_GLOBAL_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_GLOBAL_H__

#include "../../Browser/X3DBrowser.h"
#include "Context.h"

#include <Titania/PeaseBlossom/pb.h>

namespace titania {
namespace X3D {
namespace peaseblossom {

class Global
{
public:

	static
	void
	initialize (Context* const context, const pb::ptr <pb::Program> & ec, const pb::ptr <pb::pbObject> & global)
	{
		using namespace std::placeholders;

		const auto browser = context -> getBrowser ();

		global -> addOwnProperty ("NULL",  nullptr, pb::NONE);
		global -> addOwnProperty ("FALSE", false,   pb::NONE);
		global -> addOwnProperty ("TRUE",  true,    pb::NONE);

		global -> addOwnProperty ("print", new pb::NativeFunction (ec, "print", std::bind (print, _1, _2, _3, browser), 0), pb::NONE);
		global -> addOwnProperty ("now",   new pb::NativeFunction (ec, "now",   now,                                    0), pb::NONE);
	}

	static
	pb::var
	print (const pb::ptr <pb::pbExecutionContext> &, pb::pbObject* const, const std::vector <pb::var> & args, X3D::X3DBrowser* const browser)
	{
		for (const auto & arg : args)
			browser -> print (arg .toString ());

		browser -> println ();

		return pb::undefined;
	}

	static
	pb::var
	now (const pb::ptr <pb::pbExecutionContext> &,pb::pbObject* const, const std::vector <pb::var> &)
	{
		return chrono::now ();
	}

};

} // peaseblossom
} // X3D
} // titania

#endif
