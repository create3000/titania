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

#ifndef __TITANIA_X3D_TOOLS_ENVIRONMENTAL_SENSOR_X3DENVIRONMENTAL_SENSOR_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_ENVIRONMENTAL_SENSOR_X3DENVIRONMENTAL_SENSOR_NODE_TOOL_H__

#include "../Core/X3DSensorNodeTool.h"

#include "../../Bits/config.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Grouping/X3DBoundedObject.h"
#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Rendering/PolygonModeContainer.h"

namespace titania {
namespace X3D {

template <class Type>
class X3DEnvironmentalSensorNodeTool :
	virtual public X3DSensorNodeTool <Type>, public X3DBoundedObject
{
public:

	using X3DSensorNodeTool <Type>::getBrowser;

	///  @name Fields

	virtual
	SFVec3f &
	center () final override
	{ return getNode () -> center (); }

	virtual
	const SFVec3f &
	center () const final override
	{ return getNode () -> center (); }

	virtual
	SFVec3f &
	size () final override
	{ return getNode () -> size (); }

	virtual
	const SFVec3f &
	size () const final override
	{ return getNode () -> size (); }

	virtual
	SFTime &
	enterTime () final override
	{ return getNode () -> enterTime (); }

	virtual
	const SFTime &
	enterTime () const final override
	{ return getNode () -> enterTime (); }

	virtual
	SFTime &
	exitTime () final override
	{ return getNode () -> exitTime (); }

	virtual
	const SFTime &
	exitTime () const final override
	{ return getNode () -> exitTime (); }

	///  @name Operations
	
	virtual
	Box3f
	getBBox () const final override
	{ return Box3f (size (), center ()); }

	virtual
	void
	traverse (const TraverseType type) override;

	virtual
	void
	removeTool (const bool really) final override
	{
		if (really)
			X3DSensorNodeTool <Type>::removeTool ();
	}

	///  @name Destruction

	virtual
	void
	dispose () override
	{
		X3DBoundedObject::dispose ();
		X3DSensorNodeTool <Type>::dispose ();
		
		X3DChildObject::removeChildren (bboxSize (), bboxCenter ());
	}


protected:

	using X3DSensorNodeTool <Type>::getCurrentLayer;
	using X3DSensorNodeTool <Type>::getCameraSpaceMatrix;
	using X3DSensorNodeTool <Type>::getModelViewMatrix;
	using X3DSensorNodeTool <Type>::getNode;
	using X3DToolObject::getToolNode;
	using X3DToolObject::requestAsyncLoad;

	///  @name Construction

	X3DEnvironmentalSensorNodeTool (const Color3f & color) :
		X3DSensorNodeTool <Type> (),
		                   color (color)
	{
		X3DChildObject::addChildren (bboxSize (), bboxCenter ());
	}

	virtual
	void
	initialize () override;

	virtual
	void
	realize () final override;


private:

	void
	reshape ();

	///  @name Members

	Color3f  color;

};

template <class Type>
void
X3DEnvironmentalSensorNodeTool <Type>::initialize ()
{
	X3DSensorNodeTool <Type>::initialize ();
	X3DBoundedObject::initialize ();
	
	requestAsyncLoad ({ get_tool ("EnvironmentalSensor.x3dv") .str () });
}

template <class Type>
void
X3DEnvironmentalSensorNodeTool <Type>::realize ()
{
	try
	{
		getToolNode () -> setField <SFColor> ("color", color);
		getToolNode () -> setField <SFNode>  ("node",  getNode ());

		auto & set_size = getToolNode () -> getField <SFVec3f> ("set_size");
		size () .addInterest (set_size);
		set_size = getNode () -> size ();

		auto & set_center = getToolNode () -> getField <SFVec3f> ("set_center");
		center () .addInterest (set_center);
		set_center = getNode () -> center ();
	}
	catch (const X3DError & error)
	{ }
}

template <class Type>
void
X3DEnvironmentalSensorNodeTool <Type>::reshape ()
{
	try
	{
		getToolNode () -> setField <SFMatrix4f> ("cameraSpaceMatrix", getCameraSpaceMatrix (),       true);
		getToolNode () -> setField <SFMatrix4f> ("modelViewMatrix",   getModelViewMatrix () .get (), true);

		getBrowser () -> getRouter () .processEvents ();
	}
	catch (const X3DError & error)
	{ }
}

template <class Type>
void
X3DEnvironmentalSensorNodeTool <Type>::traverse (const TraverseType type)
{
	getNode () -> traverse (type);

	// Tool

	getCurrentLayer () -> getLocalObjects () .emplace_back (new PolygonModeContainer (GL_FILL));

	if (type == TraverseType::DISPLAY) // Last chance to process events
		reshape ();

	X3DToolObject::traverse (type);

	getCurrentLayer () -> getLocalObjects () .pop_back ();
}

} // X3D
} // titania

#endif
