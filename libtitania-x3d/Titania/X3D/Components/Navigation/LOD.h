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

#ifndef __TITANIA_X3D_COMPONENTS_NAVIGATION_LOD_H__
#define __TITANIA_X3D_COMPONENTS_NAVIGATION_LOD_H__

#include "../Grouping/X3DGroupingNode.h"

namespace titania {
namespace X3D {

class LOD :
	virtual public X3DGroupingNode
{
public:

	///  @name Construction

	LOD (X3DExecutionContext* const executionContext);

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

	///  @name Fields

	virtual
	SFBool &
	forceTransitions ()
	{ return *fields .forceTransitions; }

	virtual
	const SFBool &
	forceTransitions () const
	{ return *fields .forceTransitions; }

	virtual
	SFVec3f &
	center ()
	{ return *fields .center; }

	virtual
	const SFVec3f &
	center () const
	{ return *fields .center; }

	virtual
	MFFloat &
	range ()
	{ return *fields .range; }

	virtual
	const MFFloat &
	range () const
	{ return *fields .range; }

	virtual
	SFInt32 &
	level_changed ()
	{ return *fields .level_changed; }

	virtual
	const SFInt32 &
	level_changed () const
	{ return *fields .level_changed; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const override;

	virtual
	Box3d
	getSubBBox () const override
	{ return getBBox (); }

	virtual
	void
	setKeepCurrentLevel (const bool);
	
	virtual
	bool
	getKeepCurrentLevel () const
	{ return keepCurrentLevel; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;


private:

	///  @name Member access

	size_t
	getLevel (X3DBrowser* const browser, const Matrix4d & modelViewMatrix) const;

	double
	getDistance (Matrix4d modelViewMatrix) const;

	///  @name Event handlers

	virtual
	void
	set_cameraObjects () final override;

	virtual
	void
	set_pickableObjects () final override;

	void
	set_child (const int32_t);


	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const forceTransitions;
		SFVec3f* const center;
		MFFloat* const range;
		SFInt32* const level_changed;
	};

	Fields fields;

	mutable float         frameRate;
	bool                  changeLevel;
	bool                  keepCurrentLevel;
	X3DPtr <X3DChildNode> childNode;

};

} // X3D
} // titania

#endif
