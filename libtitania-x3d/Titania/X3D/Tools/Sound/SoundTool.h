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

#ifndef __TITANIA_X3D_TOOLS_SOUND_SOUND_TOOL_H__
#define __TITANIA_X3D_TOOLS_SOUND_SOUND_TOOL_H__

#include "../Sound/X3DSoundNodeTool.h"

#include "../../Components/Sound/Sound.h"
#include "../../Components/Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class SoundTool :
	virtual public Sound,
	public X3DSoundNodeTool,
	public X3DBoundedObject
{
public:

	///  @name Construction

	SoundTool (X3DBaseNode* const node);

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

	///  @name Fields

	virtual
	SFFloat &
	intensity () final override
	{ return getNode <Sound> () -> intensity (); }

	virtual
	const SFFloat &
	intensity () const final override
	{ return getNode <Sound> () -> intensity (); }

	virtual
	SFBool &
	spatialize () final override
	{ return getNode <Sound> () -> spatialize (); }

	virtual
	const SFBool &
	spatialize () const final override
	{ return getNode <Sound> () -> spatialize (); }

	virtual
	SFVec3f &
	location () final override
	{ return getNode <Sound> () -> location (); }

	virtual
	const SFVec3f &
	location () const final override
	{ return getNode <Sound> () -> location (); }

	virtual
	SFVec3f &
	direction () final override
	{ return getNode <Sound> () -> direction (); }

	virtual
	const SFVec3f &
	direction () const final override
	{ return getNode <Sound> () -> direction (); }

	virtual
	SFFloat &
	minBack () final override
	{ return getNode <Sound> () -> minBack (); }

	virtual
	const SFFloat &
	minBack () const final override
	{ return getNode <Sound> () -> minBack (); }

	virtual
	SFFloat &
	minFront () final override
	{ return getNode <Sound> () -> minFront (); }

	virtual
	const SFFloat &
	minFront () const final override
	{ return getNode <Sound> () -> minFront (); }

	virtual
	SFFloat &
	maxBack () final override
	{ return getNode <Sound> () -> maxBack (); }

	virtual
	const SFFloat &
	maxBack () const final override
	{ return getNode <Sound> () -> maxBack (); }

	virtual
	SFFloat &
	maxFront () final override
	{ return getNode <Sound> () -> maxFront (); }

	virtual
	const SFFloat &
	maxFront () const final override
	{ return getNode <Sound> () -> maxFront (); }

	virtual
	SFFloat &
	priority () final override
	{ return getNode <Sound> () -> priority (); }

	virtual
	const SFFloat &
	priority () const final override
	{ return getNode <Sound> () -> priority (); }

	virtual
	SFNode &
	source () final override
	{ return getNode <Sound> () -> source (); }

	virtual
	const SFNode &
	source () const final override
	{ return getNode <Sound> () -> source (); }

	///  @name Member access

	virtual
	Box3d
	getBBox () const final override;

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	virtual
	void
	addTool () final override;

	virtual
	void
	removeTool (const bool really) final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	realize () final override;

};

} // X3D
} // titania

#endif
