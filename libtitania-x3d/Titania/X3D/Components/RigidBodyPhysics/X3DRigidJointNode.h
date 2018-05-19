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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_X3DRIGID_JOINT_NODE_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_X3DRIGID_JOINT_NODE_H__

#include "../Core/X3DNode.h"

#include <btBulletDynamicsCommon.h>

namespace titania {
namespace X3D {

class X3DRigidJointNode :
	virtual public X3DNode
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Fields

	MFString &
	forceOutput ()
	{ return *fields .forceOutput; }

	const MFString &
	forceOutput () const
	{ return *fields .forceOutput; }

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

	void
	setCollection (RigidBodyCollection* const value);

	const X3DPtr <RigidBodyCollection> &
	getCollection () const
	{ return collectionNode; }

	///  @name Destruction

	virtual
	void
	dispose () override;

	~X3DRigidJointNode ();


protected:
	
	///  @name Construction

	X3DRigidJointNode ();

	virtual
	void
	initialize () override;

	///  @name Member access

	const X3DPtr <RigidBody> &
	getBody1 () const
	{ return bodyNode1; }

	const X3DPtr <RigidBody> &
	getBody2 () const
	{ return bodyNode2; }

	const Matrix4f &
	getInitalInverseMatrix1 () const
	{ return initalInverseMatrix1; }

	const Matrix4f &
	getInitalInverseMatrix2 () const
	{ return initalInverseMatrix2; }

	///  @name Joint handling

	virtual
	void
	addJoint () = 0;

	virtual
	void
	removeJoint () = 0;

	///  @name Event handlers

	void
	set_body1 ();

	void
	set_body2 ();

	void
	set_joint ();

	void
	initialize1 ();

	void
	initialize2 ();

	virtual
	void
	update1 () = 0;

	virtual
	void
	update2 () = 0;


private:

	///  @name Fields

	struct Fields
	{
		Fields ();

		MFString* const forceOutput;
		SFNode* const body1;
		SFNode* const body2;
	};

	Fields fields;

	///  @name Members

	X3DPtr <RigidBodyCollection> collectionNode;
	X3DPtr <RigidBody>           bodyNode1;
	X3DPtr <RigidBody>           bodyNode2;
	Matrix4f                     initalInverseMatrix1;
	Matrix4f                     initalInverseMatrix2;

};

} // X3D
} // titania

#endif
