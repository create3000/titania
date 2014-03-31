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

#ifndef __TITANIA_X3D_HANDLES_X3DGROUPING_NODE_HANDLE_H__
#define __TITANIA_X3D_HANDLES_X3DGROUPING_NODE_HANDLE_H__

#include "../Handles/X3DBaseHandle.h"

#include "../Bits/config.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Execution/Scene.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Rendering/PolygonModeContainer.h"
#include "../Types/Pointer.h"

namespace titania {
namespace X3D {

template <class Type>
using X3DGroupingNodeBaseHandle = class X3DBaseHandle <Type>;

template <class Type>
class X3DGroupingNodeHandle :
	public X3DGroupingNodeBaseHandle <Type>
{
public:

	///  @name Construction

	X3DGroupingNodeHandle (Type* const);

	virtual
	SFVec3f &
	bboxCenter () final override
	{ return getNode () -> bboxCenter (); }

	virtual
	const SFVec3f &
	bboxCenter () const final override
	{ return getNode () -> bboxCenter (); }

	virtual
	SFVec3f &
	bboxSize () final override
	{ return getNode () -> bboxSize (); }

	virtual
	const SFVec3f &
	bboxSize () const final override
	{ return getNode () -> bboxSize (); }

	virtual
	MFNode &
	addChildren () final override
	{ return getNode () -> addChildren (); }

	virtual
	const MFNode &
	addChildren () const final override
	{ return getNode () -> addChildren (); }

	virtual
	MFNode &
	removeChildren () final override
	{ return getNode () -> removeChildren (); }

	virtual
	const MFNode &
	removeChildren () const final override
	{ return getNode () -> removeChildren (); }

	virtual
	MFNode &
	children () final override
	{ return getNode () -> children (); }

	virtual
	const MFNode &
	children () const final override
	{ return getNode () -> children (); }

	///  @name Root node handling

	virtual
	MFNode &
	getRootNodes ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	virtual
	const MFNode &
	getRootNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Operatations

	virtual
	Box3f
	getBBox () const final override
	{ return getNode () -> getBBox (); }

	virtual
	void
	traverse (const TraverseType type) final override;


private:

	using X3DGroupingNodeBaseHandle <Type>::getBrowser;
	using X3DGroupingNodeBaseHandle <Type>::getCurrentLayer;
	using X3DGroupingNodeBaseHandle <Type>::getNode;

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	reshape ();

	///  @name Members

	ScenePtr scene;

};

template <class Type>
X3DGroupingNodeHandle <Type>::X3DGroupingNodeHandle (Type* const group) :
	                     X3DBaseNode (group -> getExecutionContext () -> getBrowser (), group -> getExecutionContext ()),
	X3DGroupingNodeBaseHandle <Type> (group),
	                           scene ()
{
	X3DChildObject::addChildren (scene);
}

template <class Type>
void
X3DGroupingNodeHandle <Type>::initialize ()
{
	X3DGroupingNodeBaseHandle <Type>::initialize ();

	try
	{
		scene = getBrowser () -> createX3DFromURL ({ get_tool ("BoundingBox.wrl") .str () });
	}
	catch (const X3DError & error)
	{
		std::clog << error .what () << std::endl;

		scene = getBrowser () -> createScene ();
		scene -> setup ();
	}
}

template <class Type>
MFNode &
X3DGroupingNodeHandle <Type>::getRootNodes ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return scene -> getRootNodes ();
}

template <class Type>
const MFNode &
X3DGroupingNodeHandle <Type>::getRootNodes () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return scene -> getRootNodes ();
}

template <class Type>
void
X3DGroupingNodeHandle <Type>::reshape ()
{
	try
	{
		const auto handle = scene -> getNamedNode ("Handle");
		const auto bbox   = getNode () -> X3DGroupingNode::getBBox ();

		handle -> setField <SFVec3f> ("bboxSize",   bbox .size (),   true);
		handle -> setField <SFVec3f> ("bboxCenter", bbox .center (), true);

		getBrowser () -> getRouter () .processEvents ();
	}
	catch (const X3DError & error)
	{ }
}

template <class Type>
void
X3DGroupingNodeHandle <Type>::traverse (const TraverseType type)
{
	getNode () -> traverse (type);

	// Handle

	getCurrentLayer () -> getLocalObjects () .emplace_back (new PolygonModeContainer (GL_FILL));

	if (type == TraverseType::DISPLAY) // Last chance to process events
		reshape ();

	for (const auto & rootNode : scene -> getRootNodes ())
	{
		if (rootNode)
			rootNode -> traverse (type);
	}

	getCurrentLayer () -> getLocalObjects () .pop_back ();
}

} // X3D
} // titania

#endif
