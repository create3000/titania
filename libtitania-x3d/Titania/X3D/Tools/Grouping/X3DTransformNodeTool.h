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

#ifndef __TITANIA_X3D_TOOLS_GROUPING_X3DTRANSFORM_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_GROUPING_X3DTRANSFORM_NODE_TOOL_H__

#include "../Grouping/X3DTransformMatrix3DNodeTool.h"

#include "../../Browser/Tools/ToolType.h"
#include "../../Components/Grouping/X3DTransformNode.h"

namespace titania {
namespace X3D {

class X3DTransformNodeTool :
	virtual public X3DTransformNode,
	public X3DTransformMatrix3DNodeTool
{
public:

	///  @name Construction

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

	///  @name Fields

	virtual
	const SFVec3f &
	translation () const final override
	{ return getNode <X3DTransformNode> () -> translation (); }

	virtual
	SFVec3f &
	translation () final override
	{ return getNode <X3DTransformNode> () -> translation (); }

	virtual
	SFRotation &
	rotation () final override
	{ return getNode <X3DTransformNode> () -> rotation (); }

	virtual
	const SFRotation &
	rotation () const final override
	{ return getNode <X3DTransformNode> () -> rotation (); }

	virtual
	SFVec3f &
	scale () final override
	{ return getNode <X3DTransformNode> () -> scale (); }

	virtual
	const SFVec3f &
	scale () const final override
	{ return getNode <X3DTransformNode> () -> scale (); }

	virtual
	SFRotation &
	scaleOrientation () final override
	{ return getNode <X3DTransformNode> () -> scaleOrientation (); }

	virtual
	const SFRotation &
	scaleOrientation () const final override
	{ return getNode <X3DTransformNode> () -> scaleOrientation (); }

	virtual
	SFVec3f &
	center () final override
	{ return getNode <X3DTransformNode> () -> center (); }

	virtual
	const SFVec3f &
	center () const final override
	{ return getNode <X3DTransformNode> () -> center (); }

	///  @name Hidden fields

	/// If true, this node can be transform a group leader.
	SFBool &
	grouping ()
	{ return *fields .grouping; }

	const SFBool &
	grouping () const
	{ return *fields .grouping; }

	/// Available tool.
	MFString &
	tools ()
	{ return *fields .tools; }

	const MFString &
	tools () const
	{ return *fields .tools; }

	/// 
	SFBool &
	scaleXAxis ()
	{ return *fields .scaleXAxis; }

	const SFBool &
	scaleXAxis () const
	{ return *fields .scaleXAxis; }

	SFBool &
	scaleYAxis ()
	{ return *fields .scaleYAxis; }

	const SFBool &
	scaleYAxis () const
	{ return *fields .scaleYAxis; }

	SFBool &
	scaleZAxis ()
	{ return *fields .scaleZAxis; }

	const SFBool &
	scaleZAxis () const
	{ return *fields .scaleZAxis; }

	SFBool &
	scaleXBackAxis ()
	{ return *fields .scaleXBackAxis; }

	const SFBool &
	scaleXBackAxis () const
	{ return *fields .scaleXBackAxis; }

	SFBool &
	scaleYBackAxis ()
	{ return *fields .scaleYBackAxis; }

	const SFBool &
	scaleYBackAxis () const
	{ return *fields .scaleYBackAxis; }

	SFBool &
	scaleZBackAxis ()
	{ return *fields .scaleZBackAxis; }

	const SFBool &
	scaleZBackAxis () const
	{ return *fields .scaleZBackAxis; }

	SFBool &
	scaleUniform ()
	{ return *fields .scaleUniform; }

	const SFBool &
	scaleUniform () const
	{ return *fields .scaleUniform; }

	SFBool &
	scaleFromEdge ()
	{ return *fields .scaleFromEdge; }

	const SFBool &
	scaleFromEdge () const
	{ return *fields .scaleFromEdge; }

	SFBool &
	scaleFromCenter ()
	{ return *fields .scaleFromCenter; }

	const SFBool &
	scaleFromCenter () const
	{ return *fields .scaleFromCenter; }

	MFString &
	connectedAxes ()
	{ return *fields .connectedAxes; }

	const MFString &
	connectedAxes () const
	{ return *fields .connectedAxes; }

	SFBool &
	displayBBox ()
	{ return *fields .displayBBox; }

	const SFBool &
	displayBBox () const
	{ return *fields .displayBBox; }

	SFBool &
	displayCenter ()
	{ return *fields .displayCenter; }

	const SFBool &
	displayCenter () const
	{ return *fields .displayCenter; }

	SFBool &
	isPickable ()
	{ return *fields .isPickable; }

	const SFBool &
	isPickable () const
	{ return *fields .isPickable; }

	SFColor &
	color ()
	{ return *fields .color; }

	const SFColor &
	color () const
	{ return *fields .color; }

	/// If true, this node is part of a transformation group.
	SFBool &
	grouped ()
	{ return *fields .grouped; }

	const SFBool &
	grouped () const
	{ return *fields .grouped; }

	/// If true, if the user has actived (clicked) on of the handles.
	SFBool &
	isActive ()
	{ return *fields .isActive; }

	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	/// Send if the tool was touched by the user.
	SFTime &
	touchTime ()
	{ return *fields .touchTime; }

	const SFTime &
	touchTime () const
	{ return *fields .touchTime; }

	///  @name Member access

	virtual
	const SFBool &
	isCameraObject () const final override
	{ return X3DChildNode::isCameraObject (); }

	virtual
	bool
	getKeepCenter () const final override;

	virtual
	Matrix4d
	getCurrentMatrix () const final override
	{ return getNode <X3DTransformNode> () -> getCurrentMatrix (); }

	virtual
	void
	setMatrix (const Matrix4d & matrix) final override;

	virtual
	void
	setMatrixWithCenter (const Matrix4d & matrix, const Vector3d & center) final override;

	virtual
	void
	setMatrixKeepCenter (const Matrix4d & matrix) final override;

	void
	addAbsoluteMatrix (const Matrix4d & absoluteMatrix, const bool keepCenter);

	const Matrix4d &
	getModelMatrix () const
	{ return modelMatrix; }

	const Matrix4d &
	getGroupMatrix () const
	{ return groupMatrix; }

	ToolType
	getActiveTool () const
	{ return ToolType (getToolNode () -> getField <SFInt32> ("activeTool_changed") .getValue ()); }

	void
	setChanging (const bool value)
	{ changing = value; }

	virtual
	void
	beginUndo () final override;

	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override;

	///  @name Operatations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	X3DTransformNodeTool ();

	virtual
	void
	initialize () override;

	///  @name Member access

	virtual
	bool
	getPickable (X3DRenderObject* const renderObject) const final override;


private:

	///  @name Construction

	virtual
	void
	realize () final override;

	///  @name Event handlers

	void
	set_grouped ();

	void
	set_tools ();

	void
	set_active ();

	void
	eventsProcessed ();

	///  @name Operatations

	void
	reshape (X3DRenderObject* const renderObject);

	///  @name Member types

	using UndoMatrix = std::pair <X3D::Matrix4d, X3D::Vector3d>;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const grouping;
		MFString* const tools;
		SFBool* const scaleXAxis;
		SFBool* const scaleYAxis;
		SFBool* const scaleZAxis;
		SFBool* const scaleXBackAxis;
		SFBool* const scaleYBackAxis;
		SFBool* const scaleZBackAxis;
		SFBool* const scaleUniform;
		SFBool* const scaleFromEdge;
		SFBool* const scaleFromCenter;
		MFString* const connectedAxes;
		SFBool* const displayBBox;
		SFBool* const displayCenter;
		SFBool* const isPickable;
		SFColor* const color;
		SFBool* const grouped;
		SFBool* const isActive;
		SFTime* const touchTime;
	};

	Fields fields;

	X3DPtr <TouchGroup> handlesTouchGroup;
	std::set <ToolType> availableTools;
	Matrix4d            modelMatrix;
	Matrix4d            groupMatrix;
	UndoMatrix          undoMatrix;
	bool                changing;

};

} // X3D
} // titania

#endif
