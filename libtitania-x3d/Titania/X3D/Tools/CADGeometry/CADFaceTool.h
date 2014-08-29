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

#ifndef __TITANIA_X3D_TOOLS_CADGEOMETRY_CADFACE_TOOL_H__
#define __TITANIA_X3D_TOOLS_CADGEOMETRY_CADFACE_TOOL_H__

#include "../CADGeometry/X3DProductStructureChildNodeTool.h"
#include "../Grouping/X3DGroupingNodeTool.h"
#include "../ToolColors.h"

#include "../../Components/CADGeometry/CADFace.h"

namespace titania {
namespace X3D {

class CADFaceTool :
	public X3DProductStructureChildNodeTool <CADFace>,
	public X3DBoundedObjectTool <CADFace>
{
public:

	///  @name Construction

	CADFaceTool (CADFace* const node) :
		                               X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		                     X3DBaseTool <CADFace> (node),
		X3DProductStructureChildNodeTool <CADFace> (),
		            X3DBoundedObjectTool <CADFace> (ToolColors::BROWN)
	{
		//addType (X3DConstants::CADFaceTool);
	}

	///  @name Fields

	virtual
	SFNode &
	shape () final override
	{ return getNode () -> shape (); }

	virtual
	const SFNode &
	shape () const final override
	{ return getNode () -> shape (); }

	/// @name Operations

	virtual
	void
	traverse (const TraverseType type) final override
	{
		X3DProductStructureChildNodeTool <CADFace>::traverse (type);
		X3DBoundedObjectTool <CADFace>::traverse (type);
	}

	/// @name Destruction

	virtual
	void
	dispose () final override
	{
		X3DBoundedObjectTool <CADFace>::dispose ();
		X3DProductStructureChildNodeTool <CADFace>::dispose ();
	}


private:

	using X3DProductStructureChildNodeTool <CADFace>::addType;
	using X3DProductStructureChildNodeTool <CADFace>::getNode;

	virtual
	void
	initialize () final override
	{
		X3DProductStructureChildNodeTool <CADFace>::initialize ();
		X3DBoundedObjectTool <CADFace>::initialize ();
	}

};

} // X3D
} // titania

#endif
