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

#ifndef __TITANIA_BROWSER_X3DBROWSER_EDITOR_H__
#define __TITANIA_BROWSER_X3DBROWSER_EDITOR_H__

#include "../Browser/X3DBrowserWidget.h"

namespace titania {
namespace puck {

class X3DBrowserEditor :
	public X3DBrowserWidget
{
public:

	/// @name Tests

	bool
	isSaved ();

	/// @name Member access

	void
	setEdited (bool value)
	{ edited = value; }

	bool
	getEdited () const
	{ return edited; }

	/// @name File operations

	void
	import (const basic::uri &);

	virtual
	void
	save (const basic::uri &, bool) final;

	virtual
	bool
	close () final;


protected:

	/// @name Edit operations

	X3DBrowserEditor (const basic::uri &);

	void
	addNode (const std::string &)
	throw (X3D::Error <X3D::INVALID_NAME>);

	void
	removeNode (const X3D::SFNode &)
	throw (X3D::Error <X3D::INVALID_NODE>);

	X3D::X3DSFNode <X3D::X3DGroupingNode>
	groupNodes (const X3D::MFNode &)
	throw (X3D::Error <X3D::INVALID_NODE>);

	X3D::MFNode
	ungroupNode (const X3D::SFNode &)
	throw (X3D::Error <X3D::INVALID_NODE>,
          X3D::Error <X3D::INVALID_NAME>);

	void
	addToGroup (const X3D::SFNode &, const X3D::SFNode &)
	throw (X3D::Error <X3D::INVALID_NODE>,
          X3D::Error <X3D::INVALID_NAME>);

	void
	detachFromGroup (const X3D::X3DSFNode <X3D::X3DNode> &, bool)
	throw (X3D::Error <X3D::INVALID_NODE>);

	X3D::MFNode
	createParentGroup (const X3D::SFNode &)
	throw (X3D::Error <X3D::INVALID_NODE>);


private:

	typedef std::function <bool (const X3D::SFNode &, X3D::MFNode*, X3D::SFNode*, size_t)> TraverseCallback;

	void
	removeNode (const X3D::X3DSFNode <X3D::Scene> &, const X3D::SFNode &) const;

	void
	removeExportedNodes (const X3D::X3DSFNode <X3D::Scene> &, const X3D::SFNode &) const;

	void
	removeNode (X3D::X3DExecutionContext* const, const X3D::SFNode &) const;

	void
	removeNodeFromSceneGraph (X3D::X3DExecutionContext* const, const X3D::SFNode &) const;

	void
	removeNamedNode (X3D::X3DExecutionContext* const, const X3D::SFNode &) const;

	void
	removeImportedNodes (X3D::X3DExecutionContext* const, const X3D::SFNode &) const;

	void
	deleteRoutes (X3D::X3DExecutionContext* const, const X3D::SFNode &) const;

	bool
	traverse (const X3D::X3DSFNode <X3D::Scene> & scene, const TraverseCallback & callback) const
	{ return traverse (scene .getValue (), callback); }

	bool
	traverse (X3D::X3DExecutionContext* const, const TraverseCallback &) const;

	bool
	traverse (const X3D::SFNode &, const TraverseCallback &) const;

	bool
	traverse (const X3D::SFNode &, const TraverseCallback &, X3D::ChildObjectSet &) const;

	X3D::Matrix4f
	findModelViewMatrix (X3D::X3DBaseNode* const) const;

	bool
	findModelViewMatrix (X3D::X3DBaseNode* const, X3D::Matrix4f &, std::set <X3D::X3DBaseNode*> &) const;

	std::deque <X3D::X3DBaseNode*>
	getParentNodes (X3D::X3DBaseNode* const) const;

	X3D::X3DFieldDefinition*
	getContainerField (const X3D::SFNode &, const X3D::SFNode &) const
	throw (X3D::Error <X3D::INVALID_NAME>);

	X3D::MFNode*
	getGroupingField (const X3D::SFNode & node) const
	throw (X3D::Error <X3D::INVALID_NAME>);
	
	///  @name Members

	bool edited;

};

} // puck
} // titania

#endif
