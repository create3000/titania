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

#ifndef __TITANIA_X3D_TOOLS_GROUPING_X3DTRANSFORM_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_GROUPING_X3DTRANSFORM_NODE_TOOL_H__

#include "../Grouping/X3DTransformMatrix4DNodeTool.h"
#include "../ToolColors.h"

#include "../../Bits/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Layering/X3DLayerNode.h"

namespace titania {
namespace X3D {

template <class Type>
class X3DTransformNodeTool :
	public X3DTransformMatrix4DNodeTool <Type>
{
public:

	///  @name Fields

	virtual
	const SFVec3f &
	translation () const final override
	{ return getNode () -> translation (); }

	virtual
	SFVec3f &
	translation () final override
	{ return getNode () -> translation (); }

	virtual
	SFRotation &
	rotation () final override
	{ return getNode () -> rotation (); }

	virtual
	const SFRotation &
	rotation () const final override
	{ return getNode () -> rotation (); }

	virtual
	SFVec3f &
	scale () final override
	{ return getNode () -> scale (); }

	virtual
	const SFVec3f &
	scale () const final override
	{ return getNode () -> scale (); }

	virtual
	SFRotation &
	scaleOrientation () final override
	{ return getNode () -> scaleOrientation (); }

	virtual
	const SFRotation &
	scaleOrientation () const final override
	{ return getNode () -> scaleOrientation (); }

	virtual
	SFVec3f &
	center () final override
	{ return getNode () -> center (); }

	virtual
	const SFVec3f &
	center () const final override
	{ return getNode () -> center (); }

	///  @name Member access

	virtual
	bool
	getKeepCenter () const final override;

	virtual
	Matrix4d
	getCurrentMatrix () const final override
	{ return getNode () -> getCurrentMatrix (); }

	virtual
	void
	setMatrix (const Matrix4d &) final override;

	virtual
	void
	setMatrixWithCenter (const Matrix4d &, const Vector3f &) final override;

	virtual
	void
	setMatrixKeepCenter (const Matrix4d &) final override;

	virtual
	void
	addAbsoluteMatrix (const Matrix4d &, const bool)
	throw (Error <NOT_SUPPORTED>) final override;

	virtual
	const Matrix4d &
	getTransformationMatrix () const
	throw (Error <NOT_SUPPORTED>) final override
	{ return parentMatrix; }

	virtual
	Selection::ToolType
	getActiveTool () const final override
	{ return Selection::ToolType (getToolNode () -> template getField <SFInt32> ("activeTool_changed") .getValue ()); }

	///  @name Operatations

	virtual
	void
	traverse (const TraverseType type) override;


protected:

	using X3DTransformMatrix4DNodeTool <Type>::addType;
	using X3DTransformMatrix4DNodeTool <Type>::getBrowser;
	using X3DTransformMatrix4DNodeTool <Type>::getNode;
	using X3DTransformMatrix4DNodeTool <Type>::getToolNode;
	using X3DTransformMatrix4DNodeTool <Type>::getCameraSpaceMatrix;
	using X3DTransformMatrix4DNodeTool <Type>::getModelViewMatrix;
	using X3DTransformMatrix4DNodeTool <Type>::requestAsyncLoad;
	using X3DTransformMatrix4DNodeTool <Type>::getMatrix;

	///  @name Construction

	X3DTransformNodeTool ();

	virtual
	void
	initialize () override;


private:

	///  @name Construction

	virtual
	void
	realize () final override;

	void
	eventsProcessed ();

	///  @name Operatations

	void
	reshape ();

	///  @name Members

	Matrix4d parentMatrix;
	Matrix4d matrix;
	bool     changing;

};

template <class Type>
X3DTransformNodeTool <Type>::X3DTransformNodeTool () :
	X3DTransformMatrix4DNodeTool <Type> (ToolColors::GREEN),
	                       parentMatrix (),
	                             matrix (),
	                           changing (false)
{
	addType (X3DConstants::X3DTransformNodeTool);
}

template <class Type>
void
X3DTransformNodeTool <Type>::initialize ()
{
	X3DBaseTool <Type>::initialize ();

	requestAsyncLoad ({ get_tool ("TransformTool.x3dv") .str () });
}

template <class Type>
void
X3DTransformNodeTool <Type>::realize ()
{
	getNode () -> addInterest (this, &X3DTransformNodeTool::eventsProcessed);

	try
	{
		getBrowser () -> hasControlKey () .addInterest (getToolNode () -> getField ("controlKey"));
		getBrowser () -> hasShiftKey () .addInterest (getToolNode () -> getField ("shiftKey"));
		getBrowser () -> hasAltKey () .addInterest (getToolNode () -> getField ("altKey"));
		getToolNode () -> getField ("isActive") -> addInterest (getBrowser () -> getSelection () -> isActive ());

		getToolNode () -> template setField <SFBool> ("controlKey", getBrowser () -> hasControlKey ());
		getToolNode () -> template setField <SFBool> ("shiftKey",   getBrowser () -> hasShiftKey ());
		getToolNode () -> template setField <SFBool> ("altKey",     getBrowser () -> hasAltKey ());
		getToolNode () -> template setField <SFNode> ("transform",  getNode ());
	}
	catch (const X3DError & error)
	{ }
}

template <class Type>
bool
X3DTransformNodeTool <Type>::getKeepCenter () const
{
	try
	{
		return getToolNode () -> template getField <SFBool> ("keepCenter_changed");
	}
	catch (const X3DError & error)
	{
		return false;
	}
}

// Functions for grouping X3DTransformNodeTools together

template <class Type>
void
X3DTransformNodeTool <Type>::setMatrix (const Matrix4d & matrix)
{
	changing = true;

	getNode () -> setMatrix (matrix);
}

template <class Type>
void
X3DTransformNodeTool <Type>::setMatrixWithCenter (const Matrix4d & matrix, const Vector3f & center)
{
	changing = true;

	getNode () -> setMatrixWithCenter (matrix, center);
}

template <class Type>
void
X3DTransformNodeTool <Type>::setMatrixKeepCenter (const Matrix4d & matrix)
{
	changing = true;

	getNode () -> setMatrixKeepCenter (matrix);
}

template <class Type>
void
X3DTransformNodeTool <Type>::addAbsoluteMatrix (const Matrix4d & absoluteMatrix, const bool keepCenter)
throw (Error <NOT_SUPPORTED>)
{
	try
	{
		const auto matrix = Matrix4d (getMatrix ()) * parentMatrix * absoluteMatrix * ~parentMatrix;

		if (keepCenter)
			setMatrixKeepCenter (matrix);
		else
			setMatrix (matrix);
	}
	catch (const std::domain_error &)
	{ }
}

template <class Type>
void
X3DTransformNodeTool <Type>::eventsProcessed ()
{
	try
	{
		if (changing)
			changing = false;

		else
		{
			const auto differenceMatrix = ~(matrix * parentMatrix) * Matrix4d (getMatrix ()) * parentMatrix;

			for (const auto & node : getBrowser () -> getSelection () -> getChildren ())
			{
				try
				{
					if (node == this)
						continue;

					const auto transform = dynamic_cast <X3DTransformNode*> (node .getValue ());

					if (transform)
						transform -> addAbsoluteMatrix (differenceMatrix, transform -> getKeepCenter ());
				}
				catch (const Error <NOT_SUPPORTED> &)
				{ }
			}
		}
	}
	catch (const std::domain_error &)
	{ }
}

// Traverse

template <class Type>
void
X3DTransformNodeTool <Type>::reshape ()
{
	try
	{
	   getBrowser () -> endUpdateForFrame ();

		const auto bbox = getNode () -> X3DGroupingNode::getBBox ();

		getToolNode () -> template setField <SFMatrix4f> ("cameraSpaceMatrix", getCameraSpaceMatrix (),       true);
		getToolNode () -> template setField <SFMatrix4f> ("modelViewMatrix",   getModelViewMatrix () .get (), true);
		getToolNode () -> template setField <SFVec3f>    ("bboxSize",          bbox .size (),                 true);
		getToolNode () -> template setField <SFVec3f>    ("bboxCenter",        bbox .center (),               true);

		getBrowser () -> processEvents ();
	   getBrowser () -> beginUpdateForFrame ();
	}
	catch (const X3DError & error)
	{ }
}

template <class Type>
void
X3DTransformNodeTool <Type>::traverse (const TraverseType type)
{
	getNode () -> traverse (type);

	// Remember matrices

	if (type == TraverseType::CAMERA)
	{
		parentMatrix = getModelViewMatrix () .get ();
		matrix       = getMatrix ();
	}

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
