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

#ifndef __TITANIA_X3D_COMPONENTS_LAYOUT_LAYOUT_GROUP_H__
#define __TITANIA_X3D_COMPONENTS_LAYOUT_LAYOUT_GROUP_H__

#include "../../Types/Pointer.h"
#include "../Grouping/X3DGroupingNode.h"
#include "../Grouping/X3DTransformMatrix3DObject.h"
#include "../Layering/X3DViewportNode.h"
#include "../Layout/X3DLayoutNode.h"

namespace titania {
namespace X3D {

class LayoutGroup :
	virtual public X3DGroupingNode,
	virtual public X3DTransformMatrix3DObject
{
public:

	///  @name Construction

	LayoutGroup (X3DExecutionContext* const executionContext);

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

	virtual
	SFNode &
	layout ()
	{ return *fields .layout; }

	virtual
	const SFNode &
	layout () const
	{ return *fields .layout; }

	virtual
	SFNode &
	viewport ()
	{ return *fields .viewport; }

	virtual
	const SFNode &
	viewport () const
	{ return *fields .viewport; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const override;

	virtual
	const Matrix4d &
	getMatrix () const override
	{ return X3DTransformMatrix3DObject::getMatrix (); }

	virtual
	Box3d
	getRectangleBBox () const;

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Event handler

	void
	set_viewport ();

	void
	set_layout ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const viewport;
		SFNode* const layout;
	};

	Fields fields;

	X3DPtr <X3DViewportNode> viewportNode;
	X3DPtr <X3DLayoutNode>   layoutNode;
	Matrix4d                 modelViewMatrix;

};

} // X3D
} // titania

#endif
