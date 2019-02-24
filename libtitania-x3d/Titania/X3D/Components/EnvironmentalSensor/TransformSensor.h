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

#ifndef __TITANIA_X3D_COMPONENTS_ENVIRONMENTAL_SENSOR_TRANSFORM_SENSOR_H__
#define __TITANIA_X3D_COMPONENTS_ENVIRONMENTAL_SENSOR_TRANSFORM_SENSOR_H__

#include "../EnvironmentalSensor/X3DEnvironmentalSensorNode.h"
#include "../../Types/Geometry.h"

namespace titania {
namespace X3D {

class TransformSensor :
	virtual public X3DEnvironmentalSensorNode
{
public:

	///  @name Construction

	TransformSensor (X3DExecutionContext* const executionContext);

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

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Fields

	SFNode &
	targetObject ()
	{ return *fields .targetObject; }

	const SFNode &
	targetObject () const
	{ return *fields .targetObject; }

	SFVec3f &
	position_changed ()
	{ return *fields .position_changed; }

	const SFVec3f &
	position_changed () const
	{ return *fields .position_changed; }

	SFRotation &
	orientation_changed ()
	{ return *fields .orientation_changed; }

	const SFRotation &
	orientation_changed () const
	{ return *fields .orientation_changed; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	virtual
	void
	collect (const Box3d & bbox);

	virtual
	void
	process ();

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~TransformSensor () override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_enabled ();

	void
	set_targetObject ();

	void
	update ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFNode* const targetObject;
		SFVec3f* const position_changed;
		SFRotation* const orientation_changed;
	};

	Fields fields;
	
	///  @name Members

	X3DPtr <X3DBoundedObject> targetObjectNode;
	std::vector <Matrix4d>    modelMatrices;
	std::vector <Box3d>       targetBBoxes;

};

} // X3D
} // titania

#endif
