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

#ifndef __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_HUMANOID_H__
#define __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_HUMANOID_H__

#include "../Core/X3DChildNode.h"
#include "../Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class HAnimHumanoid :
	virtual public X3DChildNode, public X3DBoundedObject
{
public:

	HAnimHumanoid (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFVec3f &
	center ()
	{ return *fields .center; }

	const SFVec3f &
	center () const
	{ return *fields .center; }

	MFString &
	info ()
	{ return *fields .info; }

	const MFString &
	info () const
	{ return *fields .info; }

	MFNode &
	joints ()
	{ return *fields .joints; }

	const MFNode &
	joints () const
	{ return *fields .joints; }

	SFString &
	name ()
	{ return *fields .name; }

	const SFString &
	name () const
	{ return *fields .name; }

	SFRotation &
	rotation ()
	{ return *fields .rotation; }

	const SFRotation &
	rotation () const
	{ return *fields .rotation; }

	SFVec3f &
	scale ()
	{ return *fields .scale; }

	const SFVec3f &
	scale () const
	{ return *fields .scale; }

	SFRotation &
	scaleOrientation ()
	{ return *fields .scaleOrientation; }

	const SFRotation &
	scaleOrientation () const
	{ return *fields .scaleOrientation; }

	MFNode &
	segments ()
	{ return *fields .segments; }

	const MFNode &
	segments () const
	{ return *fields .segments; }

	MFNode &
	sites ()
	{ return *fields .sites; }

	const MFNode &
	sites () const
	{ return *fields .sites; }

	MFNode &
	skeleton ()
	{ return *fields .skeleton; }

	const MFNode &
	skeleton () const
	{ return *fields .skeleton; }

	MFNode &
	skin ()
	{ return *fields .skin; }

	const MFNode &
	skin () const
	{ return *fields .skin; }

	SFNode &
	skinCoord ()
	{ return *fields .skinCoord; }

	const SFNode &
	skinCoord () const
	{ return *fields .skinCoord; }

	SFNode &
	skinNormal ()
	{ return *fields .skinNormal; }

	const SFNode &
	skinNormal () const
	{ return *fields .skinNormal; }

	SFVec3f &
	translation ()
	{ return *fields .translation; }

	const SFVec3f &
	translation () const
	{ return *fields .translation; }

	SFString &
	version ()
	{ return *fields .version; }

	const SFString &
	version () const
	{ return *fields .version; }

	MFNode &
	viewpoints ()
	{ return *fields .viewpoints; }

	const MFNode &
	viewpoints () const
	{ return *fields .viewpoints; }

	virtual
	Box3d
	getBBox () const final override;

	virtual
	void
	dispose () final override;


private:

	virtual
	void
	initialize () final override;

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFVec3f* const center;
		MFString* const info;
		MFNode* const joints;
		SFString* const name;
		SFRotation* const rotation;
		SFVec3f* const scale;
		SFRotation* const scaleOrientation;
		MFNode* const segments;
		MFNode* const sites;
		MFNode* const skeleton;
		MFNode* const skin;
		SFNode* const skinCoord;
		SFNode* const skinNormal;
		SFVec3f* const translation;
		SFString* const version;
		MFNode* const viewpoints;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
