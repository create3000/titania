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

#ifndef __TITANIA_X3D_COMPONENTS_GROUPING_X3DGROUPING_NODE_H__
#define __TITANIA_X3D_COMPONENTS_GROUPING_X3DGROUPING_NODE_H__

#include "../Core/X3DChildNode.h"
#include "../Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class ClipPlane;
class LocalFog;
class X3DLightNode;
class X3DPointingDeviceSensorNode;

class X3DGroupingNode :
	virtual public X3DChildNode,
	virtual public X3DBoundedObject
{
public:

	///  @name Fields

	virtual
	MFNode &
	addChildren ()
	{ return *fields .addChildren; }

	virtual
	const MFNode &
	addChildren () const
	{ return *fields .addChildren; }

	virtual
	MFNode &
	removeChildren ()
	{ return *fields .removeChildren; }

	virtual
	const MFNode &
	removeChildren () const
	{ return *fields .removeChildren; }

	virtual
	MFNode &
	children ()
	{ return *fields .children; }

	virtual
	const MFNode &
	children () const
	{ return *fields .children; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const override;

	virtual
	const Matrix4d &
	getMatrix () const
	{ return matrix; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	///  @name Construction

	virtual
	~X3DGroupingNode () override;


protected:

	///  @name Construction

	X3DGroupingNode ();

	virtual
	void
	initialize () override;

	///  @name Visibility

	void
	isHidden (const bool);

	bool
	isHidden () const
	{ return hidden; }

	virtual
	const MFBool &
	getVisible () const
	{ return visible; }

	///  @name Event handlers

	void
	set_children ();

	virtual
	void
	set_cameraObjects ();


private:

	///  @name Event handlers

	void
	set_hidden ();

	void
	set_addChildren ();

	void
	set_removeChildren ();

	///  @name Operations

	void
	connectChildren ();

	void
	add (const MFNode &);

	void
	clear ();

	// Static members

	static const Matrix4d matrix;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFNode* const addChildren;
		MFNode* const removeChildren;
		MFNode* const children;
	};

	Fields fields;

	bool                                      hidden;
	MFBool                                    visible;
	X3DPtrArray <X3DPointingDeviceSensorNode> pointingDeviceSensors;
	X3DPtrArray <X3DChildNode>                cameraObjects;
	X3DPtrArray <ClipPlane>                   clipPlanes;
	X3DPtrArray <LocalFog>                    localFogs;
	X3DPtrArray <X3DLightNode>                lights;
	X3DPtrArray <X3DChildNode>                childNodes;

};

} // X3D
} // titania

#endif
