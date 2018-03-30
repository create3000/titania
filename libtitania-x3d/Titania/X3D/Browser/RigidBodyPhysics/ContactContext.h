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

#ifndef __TITANIA_X3D_BROWSER_RIGID_BODY_PHYSICS_CONTACT_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_RIGID_BODY_PHYSICS_CONTACT_CONTEXT_H__

#include "../../Types/Geometry.h"

#include <btBulletDynamicsCommon.h>

namespace titania {
namespace X3D {

struct ContactContext
{
	ContactContext () :
		active (false)
	{ }

	bool               active;
	Vector3f           position;
	Vector3f           contactNormal;
	float              depth;
	Vector3f           frictionDirection;
	const btRigidBody* rigidBody1;
	const btRigidBody* rigidBody2;
};

class ContactSensorCallback :
	public btCollisionWorld::ContactResultCallback
{
public:

	ContactSensorCallback(btRigidBody* const rigidBody, ContactContext & context) :
		btCollisionWorld::ContactResultCallback (),
		                              rigidBody (rigidBody),
		                                context (context)
	{ }
	
	virtual
	bool
	needsCollision (btBroadphaseProxy* proxy) const
	{
		if (not btCollisionWorld::ContactResultCallback::needsCollision (proxy))
			return false;

		return rigidBody -> checkCollideWithOverride (static_cast <btCollisionObject*> (proxy -> m_clientObject));
	}
	
	virtual
	btScalar
	addSingleResult (btManifoldPoint & cp,
	                 const btCollisionObjectWrapper* colObj0, int partId0, int index0,
	                 const btCollisionObjectWrapper* colObj1, int partId1, int index1)
	{
		if (colObj0 -> m_collisionObject == rigidBody)
		{
			const auto & p = cp .getPositionWorldOnA ();
			const auto & n = cp .m_normalWorldOnB;
			const auto & f = cp .m_lateralFrictionDir1;

			context .active            = true;
			context .position          = Vector3f (p .x (), p .y (), p .z ());
			context .contactNormal     = negate (Vector3f (n .x (), n .y (), n .z ()));
			context .depth             = cp .getDistance ();
			context .frictionDirection = Vector3f (f .x (), f .y (), f .z ());
			context .rigidBody1        = rigidBody;
			context .rigidBody2        = dynamic_cast <const btRigidBody*> (colObj1 -> m_collisionObject);
		}
		else if (colObj1 -> m_collisionObject == rigidBody)
		{
			const auto & p = cp .getPositionWorldOnB ();
			const auto & n = cp .m_normalWorldOnB;
			const auto & f = cp .m_lateralFrictionDir2;

			context .active            = true;
			context .position          = Vector3f (p .x (), p .y (), p .z ());
			context .contactNormal     = Vector3f (n .x (), n .y (), n .z ());
			context .depth             = cp .getDistance ();
			context .frictionDirection = Vector3f (f .x (), f .y (), f .z ());
			context .rigidBody1        = rigidBody;
			context .rigidBody2        = dynamic_cast <const btRigidBody*> (colObj0 -> m_collisionObject);
		}		

		return 0;
	}


private:

	btRigidBody* const rigidBody;
	ContactContext &   context;
};

} // X3D
} // titania

#endif
