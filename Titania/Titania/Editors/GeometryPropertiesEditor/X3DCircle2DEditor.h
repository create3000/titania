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

#ifndef __TITANIA_EDITORS_GEOMETRY_PROPERTIES_EDITOR_X3DCIRCLE2DEDITOR_H__
#define __TITANIA_EDITORS_GEOMETRY_PROPERTIES_EDITOR_X3DCIRCLE2DEDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DGeometryPropertiesEditorInterface.h"

#include <Titania/X3D/Components/Geometry2D/Circle2D.h>

namespace titania {
namespace puck {

class X3DCircle2DEditor :
	virtual public X3DGeometryPropertiesEditorInterface
{
public:

	///  @name Destruction

	virtual
	~X3DCircle2DEditor ();


protected:

	///  @name Construction

	X3DCircle2DEditor ();

	virtual
	void
	initialize () override;


private:

	///  @name Construction

	void
	set_selection ();

	void
	set_geometry ();

	void
	set_arc2D (const X3D::X3DPtr <X3D::Circle2D> &);

	///  @name Members

	X3D::X3DPtrArray <X3D::X3DShapeNode> shapeNodes;

	X3DFieldAdjustment <X3D::SFFloat> radius;

};

} // puck
} // titania

#endif
