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

#ifndef __TITANIA_X3D_COMPONENTS_INTERPOLATION_SQUAD_ORIENTATION_INTERPOLATOR_H__
#define __TITANIA_X3D_COMPONENTS_INTERPOLATION_SQUAD_ORIENTATION_INTERPOLATOR_H__

#include "../Interpolation/X3DInterpolatorNode.h"

namespace titania {
namespace math {

template <class Type, class Scalar>
class squad_interpolator;

} // math
} // titania

namespace titania {
namespace X3D {

class SquadOrientationInterpolator :
	public X3DInterpolatorNode
{
public:

	///  @name Construction

	SquadOrientationInterpolator (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Fields

	SFBool &
	closed ()
	{ return *fields .closed; }

	const SFBool &
	closed () const
	{ return *fields .closed; }

	MFRotation &
	keyValue ()
	{ return *fields .keyValue; }

	const MFRotation &
	keyValue () const
	{ return *fields .keyValue; }

	//	MFFloat &
	//	keyVelocity ()
	//	{ return *fields .keyVelocity; }
	//
	//	const MFFloat &
	//	keyVelocity () const
	//	{ return *fields .keyVelocity; }
	//
	//	SFBool &
	//	normalizeVelocity ()
	//	{ return *fields .normalizeVelocity; }
	//
	//	const SFBool &
	//	normalizeVelocity () const
	//	{ return *fields .normalizeVelocity; }

	SFRotation &
	value_changed ()
	{ return *fields .value_changed; }

	const SFRotation &
	value_changed () const
	{ return *fields .value_changed; }


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	virtual
	void
	set_keyValue () final override;

	//	void
	//	set_keyVelocity ();

	///  @name Operations

	virtual
	void
	interpolate (size_t, size_t, const float) final override;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const closed;
		MFRotation* const keyValue;
		//		MFFloat* const keyVelocity;
		//		SFBool* const normalizeVelocity;
		SFRotation* const value_changed;
	};

	Fields fields;

	const std::unique_ptr <math::squad_interpolator <Rotation4d, double>> squad;

};

} // X3D
} // titania

#endif
