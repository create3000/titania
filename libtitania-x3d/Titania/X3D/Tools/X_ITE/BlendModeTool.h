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

#ifndef __TITANIA_X3D_TOOLS_X_ITE_BLEND_MODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_X_ITE_BLEND_MODE_TOOL_H__

#include "../Grouping/X3DGroupingNodeTool.h"

#include "../../Components/X_ITE/BlendMode.h"

namespace titania {
namespace X3D {

class BlendModeTool :
	virtual public BlendMode,
	public X3DGroupingNodeTool
{
public:

	///  @name Construction

	BlendModeTool (X3DBaseNode* const node);

	///  @name Fields

	virtual
	SFBool &
	enabled () final override
	{ return getNode <BlendMode> () -> enabled (); }

	virtual
	const SFBool &
	enabled () const final override
	{ return getNode <BlendMode> () -> enabled (); }

	virtual
	SFColorRGBA &
	blendColor () final override
	{ return getNode <BlendMode> () -> blendColor (); }

	virtual
	const SFColorRGBA &
	blendColor () const final override
	{ return getNode <BlendMode> () -> blendColor (); }

	virtual
	SFString &
	sourceColor ()
	{ return getNode <BlendMode> () -> sourceColor (); }

	virtual
	const SFString &
	sourceColor () const final override
	{ return getNode <BlendMode> () -> sourceColor (); }

	virtual
	SFString &
	sourceAlpha () final override
	{ return getNode <BlendMode> () -> sourceAlpha (); }

	virtual
	const SFString &
	sourceAlpha () const final override
	{ return getNode <BlendMode> () -> sourceAlpha (); }

	virtual
	SFString &
	destinationColor () final override
	{ return getNode <BlendMode> () -> destinationColor (); }

	virtual
	const SFString &
	destinationColor () const final override
	{ return getNode <BlendMode> () -> destinationColor (); }

	virtual
	SFString &
	destinationAlpha () final override
	{ return getNode <BlendMode> () -> destinationAlpha (); }

	virtual
	const SFString &
	destinationAlpha () const final override
	{ return getNode <BlendMode> () -> destinationAlpha (); }

	virtual
	SFString &
	modeColor () final override
	{ return getNode <BlendMode> () -> modeColor (); }

	virtual
	const SFString &
	modeColor () const final override
	{ return getNode <BlendMode> () -> modeColor (); }

	virtual
	SFString &
	modeAlpha () final override
	{ return getNode <BlendMode> () -> modeAlpha (); }

	virtual
	const SFString &
	modeAlpha () const final override
	{ return getNode <BlendMode> () -> modeAlpha (); }

	///  @name Member access

	virtual
	Box3d
	getBBox () const final override
	{ return X3DGroupingNodeTool::getBBox (); }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{ return X3DGroupingNodeTool::traverse (type, renderObject); }

	///  @name Destruction

	virtual
	void
	dispose () final override
	{ X3DGroupingNodeTool::dispose (); }


protected:

	///  @name Construction

	virtual
	void
	initialize () final override
	{ return X3DGroupingNodeTool::initialize (); }


private:

	///  @name Construction

	virtual
	void
	realize () final override;

	///  @name Event handlers

	void
	set_enabled (const bool);

};

} // X3D
} // titania

#endif
