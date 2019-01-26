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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_CONTACT_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_CONTACT_H__

#include "../Core/X3DNode.h"

namespace titania {
namespace X3D {

class Contact :
	virtual public X3DNode
{
public:

	Contact (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFVec3f &
	position ()
	{ return *fields .position; }

	const SFVec3f &
	position () const
	{ return *fields .position; }

	SFVec3f &
	contactNormal ()
	{ return *fields .contactNormal; }

	const SFVec3f &
	contactNormal () const
	{ return *fields .contactNormal; }

	SFFloat &
	depth ()
	{ return *fields .depth; }

	const SFFloat &
	depth () const
	{ return *fields .depth; }

	SFVec3f &
	frictionDirection ()
	{ return *fields .frictionDirection; }

	const SFVec3f &
	frictionDirection () const
	{ return *fields .frictionDirection; }

	MFString &
	appliedParameters ()
	{ return *fields .appliedParameters; }

	const MFString &
	appliedParameters () const
	{ return *fields .appliedParameters; }

	SFFloat &
	bounce ()
	{ return *fields .bounce; }

	const SFFloat &
	bounce () const
	{ return *fields .bounce; }

	SFFloat &
	minBounceSpeed ()
	{ return *fields .minBounceSpeed; }

	const SFFloat &
	minBounceSpeed () const
	{ return *fields .minBounceSpeed; }

	SFVec2f &
	frictionCoefficients ()
	{ return *fields .frictionCoefficients; }

	const SFVec2f &
	frictionCoefficients () const
	{ return *fields .frictionCoefficients; }

	SFVec2f &
	surfaceSpeed ()
	{ return *fields .surfaceSpeed; }

	const SFVec2f &
	surfaceSpeed () const
	{ return *fields .surfaceSpeed; }

	SFVec2f &
	slipCoefficients ()
	{ return *fields .slipCoefficients; }

	const SFVec2f &
	slipCoefficients () const
	{ return *fields .slipCoefficients; }

	SFFloat &
	softnessConstantForceMix ()
	{ return *fields .softnessConstantForceMix; }

	const SFFloat &
	softnessConstantForceMix () const
	{ return *fields .softnessConstantForceMix; }

	SFFloat &
	softnessErrorCorrection ()
	{ return *fields .softnessErrorCorrection; }

	const SFFloat &
	softnessErrorCorrection () const
	{ return *fields .softnessErrorCorrection; }

	SFNode &
	geometry1 ()
	{ return *fields .geometry1; }

	const SFNode &
	geometry1 () const
	{ return *fields .geometry1; }

	SFNode &
	geometry2 ()
	{ return *fields .geometry2; }

	const SFNode &
	geometry2 () const
	{ return *fields .geometry2; }

	SFNode &
	body1 ()
	{ return *fields .body1; }

	const SFNode &
	body1 () const
	{ return *fields .body1; }

	SFNode &
	body2 ()
	{ return *fields .body2; }

	const SFNode &
	body2 () const
	{ return *fields .body2; }


private:


	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFVec3f* const position;
		SFVec3f* const contactNormal;
		SFFloat* const depth;
		SFVec3f* const frictionDirection;
		MFString* const appliedParameters;
		SFFloat* const bounce;
		SFFloat* const minBounceSpeed;
		SFVec2f* const frictionCoefficients;
		SFVec2f* const surfaceSpeed;
		SFVec2f* const slipCoefficients;
		SFFloat* const softnessConstantForceMix;
		SFFloat* const softnessErrorCorrection;
		SFNode* const geometry1;
		SFNode* const geometry2;
		SFNode* const body1;
		SFNode* const body2;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
