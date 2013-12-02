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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_RIGID_BODY_COLLECTION_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_RIGID_BODY_COLLECTION_H__

#include "../Core/X3DChildNode.h"

namespace titania {
namespace X3D {

class RigidBodyCollection :
	virtual public X3DChildNode
{
public:

	RigidBodyCollection (X3DExecutionContext* const);

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

	MFNode &
	set_contacts ()
	{ return *fields .set_contacts; }

	const MFNode &
	set_contacts () const
	{ return *fields .set_contacts; }

	SFBool &
	autoDisable ()
	{ return *fields .autoDisable; }

	const SFBool &
	autoDisable () const
	{ return *fields .autoDisable; }

	MFNode &
	bodies ()
	{ return *fields .bodies; }

	const MFNode &
	bodies () const
	{ return *fields .bodies; }

	SFFloat &
	constantForceMix ()
	{ return *fields .constantForceMix; }

	const SFFloat &
	constantForceMix () const
	{ return *fields .constantForceMix; }

	SFFloat &
	contactSurfaceThickness ()
	{ return *fields .contactSurfaceThickness; }

	const SFFloat &
	contactSurfaceThickness () const
	{ return *fields .contactSurfaceThickness; }

	SFFloat &
	disableAngularSpeed ()
	{ return *fields .disableAngularSpeed; }

	const SFFloat &
	disableAngularSpeed () const
	{ return *fields .disableAngularSpeed; }

	SFFloat &
	disableLinearSpeed ()
	{ return *fields .disableLinearSpeed; }

	const SFFloat &
	disableLinearSpeed () const
	{ return *fields .disableLinearSpeed; }

	SFFloat &
	disableTime ()
	{ return *fields .disableTime; }

	const SFFloat &
	disableTime () const
	{ return *fields .disableTime; }

	SFBool &
	enabled ()
	{ return *fields .enabled; }

	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	SFFloat &
	errorCorrection ()
	{ return *fields .errorCorrection; }

	const SFFloat &
	errorCorrection () const
	{ return *fields .errorCorrection; }

	SFVec3f &
	gravity ()
	{ return *fields .gravity; }

	const SFVec3f &
	gravity () const
	{ return *fields .gravity; }

	SFInt32 &
	iterations ()
	{ return *fields .iterations; }

	const SFInt32 &
	iterations () const
	{ return *fields .iterations; }

	MFNode &
	joints ()
	{ return *fields .joints; }

	const MFNode &
	joints () const
	{ return *fields .joints; }

	SFFloat &
	maxCorrectionSpeed ()
	{ return *fields .maxCorrectionSpeed; }

	const SFFloat &
	maxCorrectionSpeed () const
	{ return *fields .maxCorrectionSpeed; }

	SFBool &
	preferAccuracy ()
	{ return *fields .preferAccuracy; }

	const SFBool &
	preferAccuracy () const
	{ return *fields .preferAccuracy; }

	SFNode &
	collider ()
	{ return *fields .collider; }

	const SFNode &
	collider () const
	{ return *fields .collider; }


private:


	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFNode* const set_contacts;
		SFBool* const autoDisable;
		MFNode* const bodies;
		SFFloat* const constantForceMix;
		SFFloat* const contactSurfaceThickness;
		SFFloat* const disableAngularSpeed;
		SFFloat* const disableLinearSpeed;
		SFFloat* const disableTime;
		SFBool* const enabled;
		SFFloat* const errorCorrection;
		SFVec3f* const gravity;
		SFInt32* const iterations;
		MFNode* const joints;
		SFFloat* const maxCorrectionSpeed;
		SFBool* const preferAccuracy;
		SFNode* const collider;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
