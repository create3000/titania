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

#ifndef __TITANIA_EDITORS_GRID_EDITOR_X3DGRID_TOOL_H__
#define __TITANIA_EDITORS_GRID_EDITOR_X3DGRID_TOOL_H__

#include "../../Base/X3DBaseInterface.h"
#include "../../Base/X3DEditorObject.h"

#include <Titania/X3D/Tools/Grids/X3DGridTool.h>

namespace titania {
namespace puck {

class X3DGridTool :
	virtual public X3DBaseInterface,
	public X3DEditorObject
{
public:

	virtual
	void
	setup () final override;

	///  @name Member access

	virtual
	const X3D::X3DPtr <X3D::X3DGridTool> &
	getTool () const = 0;

	void
	setVisible (const bool value);

	const X3D::SFBool &
	getVisible () const
	{ return visible; }

	void
	setPlane (const int32_t index);

	int32_t
	getPlane () const;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~X3DGridTool () override;


protected:

	///  @name Construction

	X3DGridTool ();

	virtual
	void
	fromMetadata () = 0;

	///  @name Event handlers

	virtual
	void
	set_translation ();

	virtual
	void
	set_rotation ();

	virtual
	void
	set_scale ();

	virtual
	void
	set_dimension ();

	virtual
	void
	set_majorLineEvery ();

	virtual
	void
	set_majorLineOffset ();

	virtual
	void
	set_color ();

	virtual
	void
	set_lineColor ();

	virtual
	void
	set_majorLineColor ();

	virtual
	void
	set_snapDistance ();
	
	virtual
	void
	set_snapToCenter ();

	virtual
	void
	set_snapping ();
	
	virtual
	void
	set_collision ();
	
	virtual
	void
	set_active ();


private:

	///  @name Construction

	void
	set_browser ();

	void
	set_scene ();

	void
	set_activeLayer ();

	///  @name Event handlers

	void
	connectTranslation (const X3D::SFVec3f &);

	void
	connectRotation (const X3D::SFRotation &);

	void
	connectScale (const X3D::SFVec3f &);

	void
	connectDimension (const X3D::MFInt32 &);

	void
	connectMajorLineEvery (const X3D::MFInt32 &);

	void
	connectMajorLineOffset (const X3D::MFInt32 &);

	void
	connectColor (const X3D::SFColorRGBA &);

	void
	connectLineColor (const X3D::SFColorRGBA &);

	void
	connectMajorLineColor (const X3D::SFColorRGBA &);

	void
	connectSnapDistance (const X3D::SFDouble &);
	
	void
	connectSnapToCenter (const X3D::SFBool &);

	void
	connectSnapping (const X3D::SFBool &);
	
	void
	connectCollision (const X3D::SFBool &);

	///  @name Members

	X3D::BrowserPtr  browser;
	X3D::SFBool      visible;
	X3D::UndoStepPtr undoStep;
};

} // puck
} // titania

#endif
