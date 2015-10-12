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

#ifndef __TITANIA_EDITORS_GRID_EDITOR_X3DGRID_TOOL_H__
#define __TITANIA_EDITORS_GRID_EDITOR_X3DGRID_TOOL_H__

#include "../../Base/X3DBaseInterface.h"

#include <Titania/X3D/Tools/Grids/X3DGridTool.h>

namespace titania {
namespace puck {

class X3DGridTool :
	virtual public X3DBaseInterface
{
public:

	virtual
	const X3D::X3DPtr <X3D::X3DGridTool> &
	getTool () const = 0;

	void
	isEnabled (const bool);

	bool
	isEnabled () const;

	void
	update ();

	///  @name Destruction

	virtual
	~X3DGridTool ();


protected:

	///  @name Construction

	X3DGridTool ();

	///  @name Event handler

	virtual
	void
	configure (const X3D::X3DPtr <X3D::MetadataSet> &) = 0;

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
	set_translation ();

	void
	set_rotation ();

	void
	set_scale ();

	void
	set_dimension ();

	void
	set_majorLineEvery ();

	void
	set_majorLineOffset ();

	void
	set_color ();

	void
	set_lineColor ();

	void
	set_majorLineColor ();


private:

	///  @name Operations

	void
	set_browser (const X3D::BrowserPtr &);

	void
	enable ();

	void
	disable ();

	///  @name Metadata Handling

	X3D::X3DPtr <X3D::MetadataSet>
	createMetaData (const std::string &);

	X3D::X3DPtr <X3D::MetadataSet>
	getMetaData (const std::string &) const;

	///  @name Members

	X3D::BrowserPtr browser;
};

} // puck
} // titania

#endif
