/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_VIEWPOINT_H__
#define __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_VIEWPOINT_H__

#include "../Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

class GeoViewpoint :
	public X3DViewpointNode
{
public:

	GeoViewpoint (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	SFRotation &
	set_orientation ()
	{ return *fields .set_orientation; }

	const SFRotation &
	set_orientation () const
	{ return *fields .set_orientation; }

	SFVec3d &
	set_position ()
	{ return *fields .set_position; }

	const SFVec3d &
	set_position () const
	{ return *fields .set_position; }

	SFFloat &
	fieldOfView ()
	{ return *fields .fieldOfView; }

	const SFFloat &
	fieldOfView () const
	{ return *fields .fieldOfView; }

	SFBool &
	headlight ()
	{ return *fields .headlight; }

	const SFBool &
	headlight () const
	{ return *fields .headlight; }

	MFString &
	navType ()
	{ return *fields .navType; }

	const MFString &
	navType () const
	{ return *fields .navType; }

	SFNode &
	geoOrigin ()
	{ return *fields .geoOrigin; }

	const SFNode &
	geoOrigin () const
	{ return *fields .geoOrigin; }

	MFString &
	geoSystem ()
	{ return *fields .geoSystem; }

	const MFString &
	geoSystem () const
	{ return *fields .geoSystem; }

	SFVec3d &
	position ()
	{ return *fields .position; }

	const SFVec3d &
	position () const
	{ return *fields .position; }

	SFFloat &
	speedFactor ()
	{ return *fields .speedFactor; }

	const SFFloat &
	speedFactor () const
	{ return *fields .speedFactor; }

	virtual
	Vector3f
	getPosition () const;

	virtual
	void
	reshape (const float, const float);

	virtual
	void
	traverse (TraverseType);


private:

	Vector3f
	getLookAtPositionOffset (Box3f);

	struct Fields
	{
		Fields ();

		SFRotation* const set_orientation;
		SFVec3d* const set_position;
		SFFloat* const fieldOfView;
		SFBool* const headlight;
		MFString* const navType;
		SFNode* const geoOrigin;
		MFString* const geoSystem;
		SFVec3d* const position;
		SFFloat* const speedFactor;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
