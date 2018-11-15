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

#include "SupportedFields.h"

#include "../Fields.h"
#include <iostream>

namespace titania {
namespace X3D {

SupportedFields::SupportedFields ()
{
	//std::clog << "Creating field index:" << std::endl;

	addField (new SFBool      ());
	addField (new SFColor     ());
	addField (new SFColorRGBA ());
	addField (new SFDouble    ());
	addField (new SFFloat     ());
	addField (new SFImage     ());
	addField (new SFInt32     ());
	addField (new SFMatrix3d  ());
	addField (new SFMatrix3f  ());
	addField (new SFMatrix4d  ());
	addField (new SFMatrix4f  ());
	addField (new SFNode      ());
	addField (new SFRotation  ());
	addField (new SFString    ());
	addField (new SFTime      ());
	addField (new SFVec2d     ());
	addField (new SFVec2f     ());
	addField (new SFVec3d     ());
	addField (new SFVec3f     ());
	addField (new SFVec4d     ());
	addField (new SFVec4f     ());

	addField (new MFBool      ());
	addField (new MFColor     ());
	addField (new MFColorRGBA ());
	addField (new MFDouble    ());
	addField (new MFFloat     ());
	addField (new MFImage     ());
	addField (new MFInt32     ());
	addField (new MFMatrix3d  ());
	addField (new MFMatrix3f  ());
	addField (new MFMatrix4d  ());
	addField (new MFMatrix4f  ());
	addField (new MFNode      ());
	addField (new MFRotation  ());
	addField (new MFString    ());
	addField (new MFTime      ());
	addField (new MFVec2d     ());
	addField (new MFVec2f     ());
	addField (new MFVec3d     ());
	addField (new MFVec3f     ());
	addField (new MFVec4d     ());
	addField (new MFVec4f     ());
}

///  throws Error <INVALID_NAME>
void
SupportedFields::addField (X3DFieldDefinition* const field)
{
	//std::clog << "\tAdding field type " << field -> getTypeName () << ": " << std::flush;

	if (fields .emplace (field -> getTypeName (), field) .second)
		return;

	throw Error <INVALID_NAME> ("Field type '" + field -> getTypeName () + "' already exists.");
}

///  throws Error <NOT_SUPPORTED>
const X3DFieldDefinition*
SupportedFields::getField (const std::string & typeName) const
{
	try
	{
		return fields .at (typeName);
	}
	catch (const std::out_of_range &)
	{
		throw Error <NOT_SUPPORTED> ("Unknown field type '" + typeName + "'.");
	}
}

const SupportedFieldsArray &
SupportedFields::getFields () const
{
	return fields;
}

SupportedFields::~SupportedFields ()
{
	for (const auto & pair : fields)
		delete pair .second;
}

} // X3D
} // titania
