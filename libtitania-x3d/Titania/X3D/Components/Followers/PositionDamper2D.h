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

#ifndef __TITANIA_X3D_COMPONENTS_FOLLOWERS_POSITION_DAMPER2D_H__
#define __TITANIA_X3D_COMPONENTS_FOLLOWERS_POSITION_DAMPER2D_H__

#include "../Followers/X3DDamperNode.h"

namespace titania {
namespace X3D {

class PositionDamper2D :
	public X3DDamperNode
{
public:

	PositionDamper2D (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	SFVec2f &
	set_destination ()
	{ return *fields .set_destination; }

	const SFVec2f &
	set_destination () const
	{ return *fields .set_destination; }

	SFVec2f &
	set_value ()
	{ return *fields .set_value; }

	const SFVec2f &
	set_value () const
	{ return *fields .set_value; }

	SFTime &
	tau ()
	{ return *fields .tau; }

	const SFTime &
	tau () const
	{ return *fields .tau; }

	SFFloat &
	tolerance ()
	{ return *fields .tolerance; }

	const SFFloat &
	tolerance () const
	{ return *fields .tolerance; }

	SFBool &
	isActive ()
	{ return *fields .isActive; }

	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	SFVec2f &
	value_changed ()
	{ return *fields .value_changed; }

	const SFVec2f &
	value_changed () const
	{ return *fields .value_changed; }

	SFVec2f &
	initialDestination ()
	{ return *fields .initialDestination; }

	const SFVec2f &
	initialDestination () const
	{ return *fields .initialDestination; }

	SFVec2f &
	defaultValue ()
	{ return *fields .defaultValue; }

	const SFVec2f &
	defaultValue () const
	{ return *fields .defaultValue; }

	SFInt32 &
	order ()
	{ return *fields .order; }

	const SFInt32 &
	order () const
	{ return *fields .order; }


private:

	struct Fields
	{
		Fields ();

		SFVec2f* const set_destination;
		SFVec2f* const set_value;
		SFTime* const tau;
		SFFloat* const tolerance;
		SFBool* const isActive;
		SFVec2f* const value_changed;
		SFVec2f* const initialDestination;
		SFVec2f* const defaultValue;
		SFInt32* const order;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
