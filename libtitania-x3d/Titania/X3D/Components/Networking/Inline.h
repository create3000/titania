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

#ifndef __TITANIA_X3D_COMPONENTS_NETWORKING_INLINE_H__
#define __TITANIA_X3D_COMPONENTS_NETWORKING_INLINE_H__

#include "../../Execution/X3DScene.h"
#include "../Grouping/Group.h"
#include "../Networking/X3DUrlObject.h"

#include <memory>

namespace titania {
namespace X3D {

class SceneLoader;

class Inline :
	virtual public X3DChildNode,
	virtual public X3DBoundedObject,
	virtual public X3DUrlObject
{
public:

	///  @name Construction

	Inline (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Fields

	virtual
	SFBool &
	load ()
	{ return *fields .load; }

	virtual
	const SFBool &
	load () const
	{ return *fields .load; }

	///  @name Operations

	virtual
	Box3d
	getBBox () const override;

	virtual
	void
	requestImmediateLoad () override;

	virtual
	void
	preventNextLoad ()
	{ preventLoad = true; }

	///  @name Exported node handling

	virtual
	SFNode
	getExportedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <NODE_NOT_AVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	const ExportedNodeIndex &
	getExportedNodes () const
	throw (Error <NODE_NOT_AVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	template <class Type>
	X3DPtr <Type>
	getExportedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Root node handling

	virtual
	MFNode &
	getRootNodes ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return scene -> getRootNodes (); }

	virtual
	const MFNode &
	getRootNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return scene -> getRootNodes (); }

	virtual
	const X3DScenePtr &
	getInternalScene () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return scene; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType) override;

	virtual
	void
	addTool () override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	setSceneAsync (X3DScenePtr &&);

	void
	setScene (X3DScenePtr &&);

	const X3DScenePtr &
	accessScene () const
	throw (Error <NODE_NOT_AVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	requestAsyncLoad ();

	void
	requestUnload ();

	void
	set_live ();

	void
	set_load ();

	void
	set_url ();

	void
	set_buffer ();

	void
	set_shutdown ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const load;
	};

	Fields fields;

	X3DScenePtr scene;
	GroupPtr    group;
	SFTime      buffer;
	bool        preventLoad;

	std::shared_ptr <SceneLoader> future;

};

template <class Type>
X3DPtr <Type>
Inline::getExportedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DPtr <Type> node (getExportedNode (name));

	if (node)
		return node;

	throw Error <INVALID_NODE> ("Invalid node: node '" + name + "' has other type.");
}

} // X3D
} // titania

#endif
