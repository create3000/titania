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

#include "SupportedFields.h"

#include "../Fields.h"
#include <iostream>

namespace titania {
namespace X3D {

SupportedFields::SupportedFields (X3DExecutionContext* const executionContext)
{
	//std::clog << "Creating field index:" << std::endl;

	add (new SFBool      ());
	add (new SFBool      ());
	add (new SFColor     ());
	add (new SFColorRGBA ());
	add (new SFDouble    ());
	add (new SFFloat     ());
	add (new SFImage     ());
	add (new SFInt32     ());
	add (new SFMatrix3d  ());
	add (new SFMatrix3f  ());
	add (new SFMatrix4d  ());
	add (new SFMatrix4f  ());
	add (new SFNode <X3DBaseNode> ());
	add (new SFRotation  ());
	add (new SFString    ());
	add (new SFTime      ());
	add (new SFVec2d     ());
	add (new SFVec2f     ());
	add (new SFVec3d     ());
	add (new SFVec3f     ());
	add (new SFVec4d     ());
	add (new SFVec4f     ());

	add (new MFBool      ());
	add (new MFBool      ());
	add (new MFColor     ());
	add (new MFColorRGBA ());
	add (new MFDouble    ());
	add (new MFFloat     ());
	add (new MFImage     ());
	add (new MFInt32     ());
	add (new MFMatrix3d  ());
	add (new MFMatrix3f  ());
	add (new MFMatrix4d  ());
	add (new MFMatrix4f  ());
	add (new MFNode <X3DBaseNode> ());
	add (new MFRotation  ());
	add (new MFString    ());
	add (new MFTime      ());
	add (new MFVec2d     ());
	add (new MFVec2f     ());
	add (new MFVec3d     ());
	add (new MFVec3f     ());
	add (new MFVec4d     ());
	add (new MFVec4f     ());

	//std::clog << "\tDone creating field index." << std::endl;
}

void
SupportedFields::add (X3DFieldDefinition* field)
{
	//std::clog << "\tAdding field type " << field -> getTypeName () << ": " << std::flush;

	fields .push_back (field -> getTypeName (), field);

	//std::clog << "Done." << std::endl;
}

const X3DFieldDefinition*
SupportedFields::get (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	try
	{
		return fields .find_last (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Unknown field type '" + name + "'.");
	}
}

const FieldTypesArray &
SupportedFields::get () const
{
	return fields;
}

void
SupportedFields::dispose ()
{
	for (const auto & field : fields)
		delete field;

	fields .clear ();
}

SupportedFields::~SupportedFields ()
{ }

} // X3D
} // titania
