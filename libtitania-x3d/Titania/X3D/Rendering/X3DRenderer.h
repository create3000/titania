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

#ifndef __TITANIA_X3D_RENDERING_X3DRENDERER_H__
#define __TITANIA_X3D_RENDERING_X3DRENDERER_H__

#include "../Base/Output.h"
#include "../Components/Core/X3DNode.h"
#include "../Rendering/ShapeContainer.h"

namespace titania {
namespace X3D {

class X3DRenderer :
	virtual public X3DNode
{
public:

	X3DRenderer ();

	time_type
	getTraverseTime () { return traverseTime; }

	time_type
	getDrawTime () { return drawTime; }

	size_t
	getNumNodes () { return numNodes; }

	size_t
	getNumNodesDrawn () { return numNodesDrawn; }

	size_t
	getNumTransparentNodes () { return numTransparentNodes; }

	size_t
	getNumTransparentNodesDrawn () { return numTransparentNodesDrawn; }

	void
	addShape (X3DShapeNode*, const float);

	void
	render ();

	virtual
	~X3DRenderer ();


private:

	typedef std::vector <ShapeContainer*> ShapeContainerArray;

	time_type traverseTime;
	time_type drawTime;
	size_t    numNodes;
	size_t    numNodesDrawn;
	size_t    numOpaqueNodes;
	size_t    numTransparentNodes;
	size_t    numTransparentNodesDrawn;

	ShapeContainerArray shapes;
	ShapeContainerArray transparentShapes;

	virtual
	void
	traverse () = 0;

	void
	draw ();

	void
	clear ();

};

} // X3D
} // titania

#endif
