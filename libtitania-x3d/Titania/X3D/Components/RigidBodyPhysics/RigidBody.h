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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_RIGID_BODY_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_RIGID_BODY_H__

#include "../Core/X3DNode.h"

#include <btBulletDynamicsCommon.h>

namespace titania {
namespace X3D {

class RigidBody :
	virtual public X3DNode
{
public:

	///  @name Construction

	RigidBody (X3DExecutionContext* const executionContext);

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

	SFBool &
	enabled ()
	{ return *fields .enabled; }

	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	SFBool &
	fixed ()
	{ return *fields .fixed; }

	const SFBool &
	fixed () const
	{ return *fields .fixed; }

	SFVec3f &
	position ()
	{ return *fields .position; }

	const SFVec3f &
	position () const
	{ return *fields .position; }

	SFRotation &
	orientation ()
	{ return *fields .orientation; }

	const SFRotation &
	orientation () const
	{ return *fields .orientation; }

	SFVec3f &
	linearVelocity ()
	{ return *fields .linearVelocity; }

	const SFVec3f &
	linearVelocity () const
	{ return *fields .linearVelocity; }

	SFVec3f &
	angularVelocity ()
	{ return *fields .angularVelocity; }

	const SFVec3f &
	angularVelocity () const
	{ return *fields .angularVelocity; }

	SFBool &
	useFiniteRotation ()
	{ return *fields .useFiniteRotation; }

	const SFBool &
	useFiniteRotation () const
	{ return *fields .useFiniteRotation; }

	SFVec3f &
	finiteRotationAxis ()
	{ return *fields .finiteRotationAxis; }

	const SFVec3f &
	finiteRotationAxis () const
	{ return *fields .finiteRotationAxis; }

	SFBool &
	autoDamp ()
	{ return *fields .autoDamp; }

	const SFBool &
	autoDamp () const
	{ return *fields .autoDamp; }

	SFFloat &
	linearDampingFactor ()
	{ return *fields .linearDampingFactor; }

	const SFFloat &
	linearDampingFactor () const
	{ return *fields .linearDampingFactor; }

	SFFloat &
	angularDampingFactor ()
	{ return *fields .angularDampingFactor; }

	const SFFloat &
	angularDampingFactor () const
	{ return *fields .angularDampingFactor; }

	SFFloat &
	mass ()
	{ return *fields .mass; }

	const SFFloat &
	mass () const
	{ return *fields .mass; }

	SFVec3f &
	centerOfMass ()
	{ return *fields .centerOfMass; }

	const SFVec3f &
	centerOfMass () const
	{ return *fields .centerOfMass; }

	SFBool &
	useGlobalGravity ()
	{ return *fields .useGlobalGravity; }

	const SFBool &
	useGlobalGravity () const
	{ return *fields .useGlobalGravity; }

	MFVec3f &
	forces ()
	{ return *fields .forces; }

	const MFVec3f &
	forces () const
	{ return *fields .forces; }

	MFVec3f &
	torques ()
	{ return *fields .torques; }

	const MFVec3f &
	torques () const
	{ return *fields .torques; }

	SFMatrix3f &
	inertia ()
	{ return *fields .inertia; }

	const SFMatrix3f &
	inertia () const
	{ return *fields .inertia; }

	SFNode &
	massDensityModel ()
	{ return *fields .massDensityModel; }

	const SFNode &
	massDensityModel () const
	{ return *fields .massDensityModel; }

	SFBool &
	autoDisable ()
	{ return *fields .autoDisable; }

	const SFBool &
	autoDisable () const
	{ return *fields .autoDisable; }

	SFFloat &
	disableTime ()
	{ return *fields .disableTime; }

	const SFFloat &
	disableTime () const
	{ return *fields .disableTime; }

	SFFloat &
	disableLinearSpeed ()
	{ return *fields .disableLinearSpeed; }

	const SFFloat &
	disableLinearSpeed () const
	{ return *fields .disableLinearSpeed; }

	SFFloat &
	disableAngularSpeed ()
	{ return *fields .disableAngularSpeed; }

	const SFFloat &
	disableAngularSpeed () const
	{ return *fields .disableAngularSpeed; }

	MFNode &
	geometry ()
	{ return *fields .geometry; }

	const MFNode &
	geometry () const
	{ return *fields .geometry; }

	///  @name Member access

	void
	setCollection (RigidBodyCollection* const value)
	{ collectionNode = value; }

	const X3DPtr <RigidBodyCollection> &
	getCollection () const
	{ return collectionNode; }

	const std::shared_ptr <btRigidBody> &
	getRigidBody () const
	{ return rigidBody; }

	const Matrix4f &
	getMatrix () const
	{ return matrix; }

	///  @name Operations

	void
	applyForces (const Vector3f & gravity);

	void
	update ();

	///  @name Destruction

	virtual
	~RigidBody () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_live ();

	void
	set_position ();

	void
	set_orientation ();

	void
	set_transform ();

	void
	set_linearVelocity ();
	
	void
	set_angularVelocity ();
	
	void
	set_finiteRotationAxis ();
	
	void
	set_damping ();

	void
	set_centerOfMass ();

	void
	set_massProps ();

	void
	set_forces ();

	void
	set_torques ();

	void
	set_disable ();

	void
	set_geometry ();

	void
	set_compoundShape ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFBool* const enabled;
		SFBool* const fixed;
		SFVec3f* const position;
		SFRotation* const orientation;
		SFVec3f* const linearVelocity;
		SFVec3f* const angularVelocity;
		SFBool* const useFiniteRotation;
		SFVec3f* const finiteRotationAxis;
		SFBool* const autoDamp;
		SFFloat* const linearDampingFactor;
		SFFloat* const angularDampingFactor;
		SFFloat* const mass;
		SFVec3f* const centerOfMass;
		SFNode* const massDensityModel;
		SFBool* const useGlobalGravity;
		MFVec3f* const forces;
		MFVec3f* const torques;
		SFMatrix3f* const inertia;
		SFBool* const autoDisable;
		SFFloat* const disableTime;
		SFFloat* const disableLinearSpeed;
		SFFloat* const disableAngularSpeed;
		MFNode* const geometry;
	};

	Fields fields;

	///  @name Members

	X3DPtr <RigidBodyCollection>           collectionNode;
	X3DPtrArray <X3DNBodyCollidableNode>   geometryNodes;
	X3DPtrArray <X3DNBodyCollidableNode>   otherGeometryNodes;
	std::shared_ptr <btCompoundShape>      compoundShape;
	std::shared_ptr <btDefaultMotionState> motionState;
	std::shared_ptr <btRigidBody>          rigidBody;
	SFTime                                 transform;
	Matrix4f                               matrix;
	Vector3f                               force;
	Vector3f                               torque;
	Vector3f                               initialPosition;
	Rotation4d                             initialOrientation;
	Vector3f                               initialLinearVelocity;
	Vector3f                               initialAngularVelocity;

};

} // X3D
} // titania

#endif
