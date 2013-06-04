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

#ifndef __TITANIA_X3D_COMPONENTS_DIS_DISENTITY_TYPE_MAPPING_H__
#define __TITANIA_X3D_COMPONENTS_DIS_DISENTITY_TYPE_MAPPING_H__

#include "../Core/X3DInfoNode.h"

namespace titania {
namespace X3D {

class DISEntityTypeMapping :
	public X3DInfoNode
{
public:

	DISEntityTypeMapping (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	MFString &
	url ()
	{ return *fields .url; }

	const MFString &
	url () const
	{ return *fields .url; }

	SFInt32 &
	category ()
	{ return *fields .category; }

	const SFInt32 &
	category () const
	{ return *fields .category; }

	SFInt32 &
	country ()
	{ return *fields .country; }

	const SFInt32 &
	country () const
	{ return *fields .country; }

	SFInt32 &
	domain ()
	{ return *fields .domain; }

	const SFInt32 &
	domain () const
	{ return *fields .domain; }

	SFInt32 &
	extra ()
	{ return *fields .extra; }

	const SFInt32 &
	extra () const
	{ return *fields .extra; }

	SFInt32 &
	kind ()
	{ return *fields .kind; }

	const SFInt32 &
	kind () const
	{ return *fields .kind; }

	SFInt32 &
	specific ()
	{ return *fields .specific; }

	const SFInt32 &
	specific () const
	{ return *fields .specific; }

	SFInt32 &
	subcategory ()
	{ return *fields .subcategory; }

	const SFInt32 &
	subcategory () const
	{ return *fields .subcategory; }


private:

	struct Fields
	{
		Fields ();

		MFString* const url;
		SFInt32* const category;
		SFInt32* const country;
		SFInt32* const domain;
		SFInt32* const extra;
		SFInt32* const kind;
		SFInt32* const specific;
		SFInt32* const subcategory;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
