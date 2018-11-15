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

#ifndef __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_LOD_H__
#define __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_LOD_H__

#include "../Core/X3DChildNode.h"
#include "../Geospatial/X3DGeospatialObject.h"
#include "../Grouping/Group.h"
#include "../Grouping/X3DBoundedObject.h"
#include "../Networking/Inline.h"

namespace titania {
namespace X3D {

class GeoLOD :
	virtual public X3DChildNode,
	virtual public X3DBoundedObject,
	virtual public X3DGeospatialObject
{
public:

	///  @name Construction

	GeoLOD (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Common members

	virtual
	ComponentType
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
	MFString &
	rootUrl ()
	{ return *fields .rootUrl; }

	virtual
	const MFString &
	rootUrl () const
	{ return *fields .rootUrl; }

	virtual
	MFString &
	child1Url ()
	{ return *fields .child1Url; }

	virtual
	const MFString &
	child1Url () const
	{ return *fields .child1Url; }

	virtual
	MFString &
	child2Url ()
	{ return *fields .child2Url; }

	virtual
	const MFString &
	child2Url () const
	{ return *fields .child2Url; }

	virtual
	MFString &
	child3Url ()
	{ return *fields .child3Url; }

	virtual
	const MFString &
	child3Url () const
	{ return *fields .child3Url; }

	virtual
	MFString &
	child4Url ()
	{ return *fields .child4Url; }

	virtual
	const MFString &
	child4Url () const
	{ return *fields .child4Url; }

	virtual
	SFVec3d &
	center ()
	{ return *fields .center; }

	virtual
	const SFVec3d &
	center () const
	{ return *fields .center; }

	virtual
	SFFloat &
	range ()
	{ return *fields .range; }

	virtual
	const SFFloat &
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

	virtual
	MFNode &
	rootNode ()
	{ return *fields .rootNode; }

	virtual
	const MFNode &
	rootNode () const
	{ return *fields .rootNode; }

	virtual
	MFNode &
	children ()
	{ return *fields .children; }

	virtual
	const MFNode &
	children () const
	{ return *fields .children; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const override;
	
	virtual
	void
	setKeepCurrentLevel (const bool value)
	{ keepCurrentLevel = value; }
	
	virtual
	bool
	getKeepCurrentLevel () const
	{ return keepCurrentLevel; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

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
	set_rootLoadState ();

	void
	set_childLoadState ();

	///  @name Operations

	size_t
	getLevel (const Matrix4d & modelViewMatrix) const;

	double
	getDistance (Matrix4d modelViewMatrix) const;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFString* const rootUrl;
		MFString* const child1Url;
		MFString* const child2Url;
		MFString* const child3Url;
		MFString* const child4Url;
		SFVec3d* const center;
		SFFloat* const range;
		SFInt32* const level_changed;
		MFNode* const rootNode;
		MFNode* const children;
	};

	Fields fields;

	X3DPtr <Group>  rootGroup;
	X3DPtr <Inline> rootInline;
	X3DPtr <Inline> child1Inline;
	X3DPtr <Inline> child2Inline;
	X3DPtr <Inline> child3Inline;
	X3DPtr <Inline> child4Inline;
	bool            childrenLoaded;
	bool            keepCurrentLevel;

};

} // X3D
} // titania

#endif
