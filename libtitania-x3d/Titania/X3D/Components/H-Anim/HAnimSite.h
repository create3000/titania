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

#ifndef __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_SITE_H__
#define __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_SITE_H__

#include "../Grouping/X3DGroupingNode.h"

namespace titania {
namespace X3D {

class HAnimSite :
	public X3DGroupingNode
{
public:

	HAnimSite (X3DExecutionContext* const executionContext);

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

	SFVec3f &
	translation ()
	{ return *fields .translation; }

	const SFVec3f &
	translation () const
	{ return *fields .translation; }


private:


	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFVec3f* const center;
		SFString* const name;
		SFRotation* const rotation;
		SFVec3f* const scale;
		SFRotation* const scaleOrientation;
		SFVec3f* const translation;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
