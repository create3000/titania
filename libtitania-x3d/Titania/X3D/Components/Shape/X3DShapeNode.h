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

#ifndef __TITANIA_X3D_COMPONENTS_SHAPE_X3DSHAPE_NODE_H__
#define __TITANIA_X3D_COMPONENTS_SHAPE_X3DSHAPE_NODE_H__

#include "../Core/X3DChildNode.h"
#include "../Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class X3DAppearanceNode;
class X3DGeometryNode;

class X3DShapeNode :
	virtual public X3DChildNode, public X3DBoundedObject
{
public:

	SFNode <X3DBaseNode> &
	appearance ()
	{ return *fields .appearance; }

	const SFNode <X3DBaseNode> &
	appearance () const
	{ return *fields .appearance; }

	SFNode <X3DBaseNode> &
	geometry ()
	{ return *fields .geometry; }

	const SFNode <X3DBaseNode> &
	geometry () const
	{ return *fields .geometry; }


	virtual
	Box3f
	getBBox ();

	bool
	isTransparent () const;
	
	X3DGeometryNode*
	getGeometry () const
	{ return _geometry; }

	virtual
	void
	traverse (TraverseType);

	virtual
	void
	draw ();

	virtual
	void
	dispose ();


protected:

	X3DShapeNode ();

	virtual
	void
	initialize ();


private:

	void
	set_appearance ();

	void
	set_geometry ();
	
	void
	pick ();

	struct Fields
	{
		Fields ();

		SFNode <X3DBaseNode>* const appearance;
		SFNode <X3DBaseNode>* const geometry;
	};

	Fields fields;


	X3DAppearanceNode* _appearance;
	X3DGeometryNode*   _geometry;

};

} // X3D
} // titania

#endif

