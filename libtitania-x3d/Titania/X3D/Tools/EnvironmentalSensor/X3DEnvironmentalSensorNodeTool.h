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

#ifndef __TITANIA_X3D_TOOLS_ENVIRONMENTAL_SENSOR_X3DENVIRONMENTAL_SENSOR_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_ENVIRONMENTAL_SENSOR_X3DENVIRONMENTAL_SENSOR_NODE_TOOL_H__

#include "../Core/X3DSensorNodeTool.h"

#include "../../Components/EnvironmentalSensor/X3DEnvironmentalSensorNode.h"
#include "../../Components/Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class X3DEnvironmentalSensorNodeTool :
	virtual public X3DEnvironmentalSensorNode,
	public X3DSensorNodeTool,
	public X3DBoundedObject
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override
	{ X3DSensorNodeTool::setExecutionContext (executionContext); }

	///  @name Fields

	virtual
	SFVec3f &
	center () final override
	{ return getNode <X3DEnvironmentalSensorNode> () -> center (); }

	virtual
	const SFVec3f &
	center () const final override
	{ return getNode <X3DEnvironmentalSensorNode> () -> center (); }

	virtual
	SFVec3f &
	size () final override
	{ return getNode <X3DEnvironmentalSensorNode> () -> size (); }

	virtual
	const SFVec3f &
	size () const final override
	{ return getNode <X3DEnvironmentalSensorNode> () -> size (); }

	virtual
	SFTime &
	enterTime () final override
	{ return getNode <X3DEnvironmentalSensorNode> () -> enterTime (); }

	virtual
	const SFTime &
	enterTime () const final override
	{ return getNode <X3DEnvironmentalSensorNode> () -> enterTime (); }

	virtual
	SFTime &
	exitTime () final override
	{ return getNode <X3DEnvironmentalSensorNode> () -> exitTime (); }

	virtual
	const SFTime &
	exitTime () const final override
	{ return getNode <X3DEnvironmentalSensorNode> () -> exitTime (); }

	///  @name Operations
	
	virtual
	Box3d
	getBBox () const final override;

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	virtual
	void
	beginUndo () final override;

	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override;

	virtual
	void
	addTool () override;

	virtual
	void
	removeTool (const bool really) final override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DEnvironmentalSensorNodeTool () override;


protected:

	///  @name Construction

	X3DEnvironmentalSensorNodeTool (const Color3f & color);

	virtual
	void
	initialize () override;

	virtual
	void
	realize () final override;

	///  @name Member access

	virtual
	bool
	getPickable (X3DRenderObject* const renderObject) const final override;


private:

	///  @name Members

	Color3f  color;
	Vector3f startSize;
	Vector3f startCenter;

};

} // X3D
} // titania

#endif
