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

#ifndef __TITANIA_X3D_TOOLS_GROUPING_X3DBOUNDED_OBJECT_TOOL_H__
#define __TITANIA_X3D_TOOLS_GROUPING_X3DBOUNDED_OBJECT_TOOL_H__

#include "../Core/X3DBaseTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/X3DBrowser.h"

#include "../../Components/Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class X3DBoundedObjectTool :
	virtual public X3DBoundedObject,
	virtual public X3DBaseTool
{
public:

	///  @name Fields

	virtual
	SFVec3f &
	bboxCenter () final override
	{ return getNode <X3DBoundedObject> () -> bboxCenter (); }

	virtual
	const SFVec3f &
	bboxCenter () const final override
	{ return getNode <X3DBoundedObject> () -> bboxCenter (); }

	virtual
	SFVec3f &
	bboxSize () final override
	{ return getNode <X3DBoundedObject> () -> bboxSize (); }

	virtual
	const SFVec3f &
	bboxSize () const final override
	{ return getNode <X3DBoundedObject> () -> bboxSize (); }

	///  @name Operatations

	virtual
	Box3d
	getBBox () const override
	{ return getNode <X3DBoundedObject> () -> getBBox (); }

	virtual
	void
	traverse (const TraverseType type) override;

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }


protected:

	///  @name Construction

	X3DBoundedObjectTool (const Color3f &);

	virtual
	void
	initialize () override;

	virtual
	void
	realize () override;

	///  @name Member acces

	void
	setDisplayCenter (const bool value)
	{ displayCenter = value; }

	void
	setLinetype (const int32_t value)
	{ linetype = value; }

	virtual
	const Matrix4d &
	getMatrix () const
	{ return matrix; }


protected:

	///  @name Operations

	virtual
	void
	reshape ();


private:

	///  @name Members

	int32_t  linetype;
	bool     displayCenter;
	Color3f  color;
	Matrix4d matrix;

};

inline
X3DBoundedObjectTool::X3DBoundedObjectTool (const Color3f & color) :
	X3DBoundedObject (),
	     X3DBaseTool (),
	        linetype (3),
	   displayCenter (false),
	           color (color)
{
	addType (X3DConstants::X3DBoundedObjectTool);
}

inline
void
X3DBoundedObjectTool::initialize ()
{
	requestAsyncLoad ({ get_tool ("BoundingBox.x3dv") .str () });
}

inline
void
X3DBoundedObjectTool::realize ()
{
	try
	{
		const SFNode & tool = getToolNode ();

		tool -> setField <SFBool>  ("displayCenter", displayCenter);
		tool -> setField <SFColor> ("color",         color);
		tool -> setField <SFInt32> ("linetype",      linetype);
	}
	catch (const X3DError & error)
	{ }
}

inline
void
X3DBoundedObjectTool::reshape ()
{
	Box3d bbox = getNode <X3DBoundedObject> () -> getBBox ();
	
	try
	{
		bbox *= inverse (getMatrix ());
	}
	catch (const std::domain_error &)
	{
		bbox = Box3d (Vector3d (), Vector3d ());
	}

	try
	{
	   getBrowser () -> endUpdateForFrame ();
		
		const SFNode & tool = getToolNode ();

		tool -> setField <SFVec3f> ("bboxSize",   bbox .size (),   true);
		tool -> setField <SFVec3f> ("bboxCenter", bbox .center (), true);

		getBrowser () -> processEvents ();
	   getBrowser () -> beginUpdateForFrame ();
	}
	catch (const X3DError &)
	{
		getBrowser () -> beginUpdateForFrame ();
	}
}

inline
void
X3DBoundedObjectTool::traverse (const TraverseType type)
{
	getNode <X3DBoundedObject> () -> traverse (type);

	// Tool

	getModelViewMatrix () .push ();
	getModelViewMatrix () .mult_left (getMatrix ());

	if (type == TraverseType::DISPLAY) // Last chance to process events
		reshape ();

	X3DToolObject::traverse (type);

	getModelViewMatrix () .pop ();
}

} // X3D
} // titania

#endif
