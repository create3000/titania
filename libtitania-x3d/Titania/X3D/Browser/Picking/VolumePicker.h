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

#ifndef __TITANIA_X3D_BROWSER_PICKING_VOLUME_PICKER_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_PICKING_VOLUME_PICKER_CONTEXT_H__

#include "../../Types/Numbers.h"

#include <btBulletDynamicsCommon.h>
#include <memory>

namespace titania {
namespace X3D {

class VolumePicker
{
public:
	
	///  @name Construction

	VolumePicker ();

	///  @name Member access

	void
	setChildShape1 (const Matrix4f & matrix, const std::shared_ptr <btCompoundShape> & childShape);

	void
	setChildShape2 (const Matrix4f & matrix, const std::shared_ptr <btCompoundShape> & childShape);

	///  @name Operations

	bool
	contactTest ();
	
	///  @name Destruction

	~VolumePicker ();


private:

	///  @name Operations

	void
	setChildShape (const std::shared_ptr <btCompoundShape> & compoundShape, const Matrix4f & matrix, const std::shared_ptr <btCompoundShape> & childShape);

	btTransform
	getTransform (const Vector3f & translation, const Rotation4f & rotation) const;

	///  @name Members

	std::shared_ptr <btBroadphaseInterface>           broadphase;
	std::shared_ptr <btDefaultCollisionConfiguration> collisionConfiguration;
	std::shared_ptr <btCollisionDispatcher>           dispatcher;
	std::shared_ptr <btCollisionWorld>                collisionWorld;
	std::shared_ptr <btCompoundShape>                 compoundShape1;
	std::shared_ptr <btDefaultMotionState>            motionState1;
	std::shared_ptr <btRigidBody>                     rigidBody1;
	std::shared_ptr <btCompoundShape>                 compoundShape2;
	std::shared_ptr <btDefaultMotionState>            motionState2;
	std::shared_ptr <btRigidBody>                     rigidBody2;
};

} // X3D
} // titania

#endif
