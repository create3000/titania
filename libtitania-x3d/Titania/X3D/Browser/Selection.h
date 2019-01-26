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

#ifndef __TITANIA_X3D_BROWSER_SELECTION_H__
#define __TITANIA_X3D_BROWSER_SELECTION_H__

#include "../Bits/Traverse.h"
#include "../Components/Core/X3DNode.h"
#include "Tools/ToolType.h"

namespace titania {
namespace X3D {

class Selection :
	virtual public X3DBaseNode
{
public:

	///  @name Construction

	Selection (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Member access

	void
	setSelectMultiple (const bool value)
	{ selectMultiple = value; }

	const SFBool &
	getSelectMultiple () const
	{ return selectMultiple; }

	void
	setSelectLowest (const bool value)
	{ selectLowest = value; }

	const SFBool &
	getSelectLowest () const
	{ return selectLowest; }

	void
	setSelectGeometry (const bool value);

	const SFBool &
	getSelectGeometry () const
	{ return selectGeometry; }

	const SFBool &
	getTouchTime () const
	{ return touchTime; }

	///  @name Member access

	bool
	isSelected (const SFNode & node) const;

	void
	addNodes (const MFNode & value);

	void
	removeNodes (const MFNode & value);

	void
	clearNodes ();

	void
	setNodes (const MFNode & value);

	const MFNode &
	getNodes () const
	{ return nodes; }

	const MFNode &
	getGeometries () const
	{ return geometryNodes; }

	const MFNode &
	getHierarchy () const
	{ return hierarchy; }

	MFNode
	getParents ();
	
	MFNode
	getChildren ();


protected:

	///  @name Friends

	friend class X3DPointingDeviceSensorContext;

	///  @name Operations

	bool
	selectNode (X3DBrowser* const browser);


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_initialized ();
	
	void
	set_shutdown ();
	
	void
	set_sceneGraph ();

	///  @name Operations

	void
	setHierarchy (const SFNode & node, const MFNode & hierarchy);

	MFNode
	getGeometries (const MFNode & nodes) const;

	MFNode
	getLowest (const MFNode & nodes) const;

	SFNode
	getSelection (const MFNode & hierarchy) const;

	Hierarchies
	findNode (const SFNode & node) const;

	Hierarchies
	findNode (const SFNode & parent, const SFNode & node) const;

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	SFBool selectMultiple;
	SFBool selectLowest;
	SFBool selectGeometry;
	SFBool touchTime;
	MFNode nodes;
	MFNode geometryNodes;
	SFNode masterSelection;
	MFNode hierarchy;
	bool   clearHierarchy;

};

} // X3D
} // titania

#endif
