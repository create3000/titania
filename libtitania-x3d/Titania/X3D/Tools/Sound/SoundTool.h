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

#ifndef __TITANIA_X3D_TOOLS_SOUND_SOUND_TOOL_H__
#define __TITANIA_X3D_TOOLS_SOUND_SOUND_TOOL_H__

#include "../Sound/X3DSoundNodeTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"

#include "../../Components/Sound/Sound.h"

namespace titania {
namespace X3D {

class SoundTool :
	virtual public Sound,
	public X3DSoundNodeTool
{
public:

	///  @name Construction

	SoundTool (Sound* const);

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

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type) final override;

	virtual
	void
	addTool () final override;

	virtual
	void
	removeTool (const bool) final override;


protected:

	virtual
	void
	initialize () final override;

	virtual
	void
	realize () final override;

};

inline
SoundTool::SoundTool (Sound* const node) :
	     X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	           Sound (node -> getExecutionContext ()),
	     X3DBaseTool (node),
	X3DSoundNodeTool ()
{
	//addType (X3DConstants::SoundTool);
}

inline
void
SoundTool::initialize ()
{
	X3DSoundNodeTool::initialize ();

	requestAsyncLoad ({ get_tool ("SoundTool.x3dv") .str () });
}

inline
void
SoundTool::realize ()
{
	try
	{
		getToolNode () -> setField <SFNode>  ("sound", getNode <Sound> ());
	}
	catch (const X3DError & error)
	{ }
}

inline
void
SoundTool::traverse (const TraverseType type)
{
	getNode <Sound> () -> traverse (type);

	X3DToolObject::traverse (type);
}

inline
void
SoundTool::addTool ()
{
	try
	{
		getToolNode () -> setField <SFBool> ("selected", getBrowser () -> getSelection () -> isSelected (this));
	}
	catch (const X3DError &)
	{ }
}

inline
void
SoundTool::removeTool (const bool really)
{
	if (really)
		X3DSoundNodeTool::removeTool ();
	
	else
	{
		try
		{
			getToolNode () -> setField <SFBool> ("selected", false);
		}
		catch (const X3DError &)
		{ }
	}
}

} // X3D
} // titania

#endif
