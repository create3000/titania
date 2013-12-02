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

#ifndef __TITANIA_X3D_COMPONENTS_FOLLOWERS_COORDINATE_CHASER_H__
#define __TITANIA_X3D_COMPONENTS_FOLLOWERS_COORDINATE_CHASER_H__

#include "../Followers/X3DChaserNode.h"

namespace titania {
namespace X3D {

class CoordinateChaser :
	public X3DChaserNode
{
public:

	CoordinateChaser (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	MFVec3f &
	set_value ()
	{ return *fields .set_value; }

	const MFVec3f &
	set_value () const
	{ return *fields .set_value; }

	MFVec3f &
	set_destination ()
	{ return *fields .set_destination; }

	const MFVec3f &
	set_destination () const
	{ return *fields .set_destination; }

	MFVec3f &
	initialValue ()
	{ return *fields .initialValue; }

	const MFVec3f &
	initialValue () const
	{ return *fields .initialValue; }

	MFVec3f &
	initialDestination ()
	{ return *fields .initialDestination; }

	const MFVec3f &
	initialDestination () const
	{ return *fields .initialDestination; }

	MFVec3f &
	value_changed ()
	{ return *fields .value_changed; }

	const MFVec3f &
	value_changed () const
	{ return *fields .value_changed; }


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Operations

	template <class LHS, class RHS>
	bool
	equals (const LHS & lhs, const RHS & rhs, float tolerance) const
	{
		float distance = 0;

		for (size_t i = 0, size = lhs .size (); i < size; ++ i)
			distance = std::max (distance, abs (lhs [i] - rhs [i]));

		return distance < tolerance;
	}

	///  @name Event handlers

	void
	_set_value ();

	void
	_set_destination ();

	void
	set_duration ();

	virtual
	void
	prepareEvents () final override;

	float
	updateBuffer ();

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFVec3f* const set_value;
		MFVec3f* const set_destination;
		MFVec3f* const initialValue;
		MFVec3f* const initialDestination;
		MFVec3f* const value_changed;
	};

	Fields fields;

	time_type                            bufferEndTime;
	std::vector <Vector3f>               previousValue;
	std::vector <std::vector <Vector3f>> buffer;

};

} // X3D
} // titania

#endif
