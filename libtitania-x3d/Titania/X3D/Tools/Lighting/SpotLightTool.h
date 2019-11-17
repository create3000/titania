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

#ifndef __TITANIA_X3D_TOOLS_LIGHTING_SPOT_LIGHT_TOOL_H__
#define __TITANIA_X3D_TOOLS_LIGHTING_SPOT_LIGHT_TOOL_H__

#include "../Lighting/X3DLightNodeTool.h"

#include "../../Components/Lighting/SpotLight.h"

namespace titania {
namespace X3D {

class SpotLightTool :
	virtual public SpotLight,
	public X3DLightNodeTool
{
public:

	///  @name Construction

	SpotLightTool (X3DBaseNode* const node);

	///  @name Fields

	virtual
	SFVec3f &
	attenuation () final override
	{ return getNode <SpotLight> () -> attenuation (); }

	virtual
	const SFVec3f &
	attenuation () const final override
	{ return getNode <SpotLight> () -> attenuation (); }

	virtual
	SFFloat &
	beamWidth () final override
	{ return getNode <SpotLight> () -> beamWidth (); }

	virtual
	const SFFloat &
	beamWidth () const final override
	{ return getNode <SpotLight> () -> beamWidth (); }

	virtual
	SFFloat &
	cutOffAngle () final override
	{ return getNode <SpotLight> () -> cutOffAngle (); }

	virtual
	const SFFloat &
	cutOffAngle () const final override
	{ return getNode <SpotLight> () -> cutOffAngle (); }

	virtual
	SFVec3f &
	direction () final override
	{ return getNode <SpotLight> () -> direction (); }

	virtual
	const SFVec3f &
	direction () const final override
	{ return getNode <SpotLight> () -> direction (); }

	virtual
	SFVec3f &
	location () final override
	{ return getNode <SpotLight> () -> location (); }

	virtual
	const SFVec3f &
	location () const final override
	{ return getNode <SpotLight> () -> location (); }

	virtual
	SFFloat &
	radius () final override
	{ return getNode <SpotLight> () -> radius (); }

	virtual
	const SFFloat &
	radius () const final override
	{ return getNode <SpotLight> () -> radius (); }

	///  @name Member access

	virtual
	float
	getRadius () const final override
	{ return getNode <SpotLight> () -> getRadius (); }

	virtual
	float
	getBeamWidth () const final override
	{ return getNode <SpotLight> () -> getBeamWidth (); }

	virtual
	float
	getCutOffAngle () const final override
	{ return getNode <SpotLight> () -> getCutOffAngle (); }

	///  @name Operations

	virtual
	Box3d
	getBBox () const final override;

	virtual
	void
	beginUndo () final override;

	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override
	{ X3DLightNodeTool::initialize (); }

	virtual
	void
	realize () final override;


private:

	///  @name Member access

	Vector3f startLocation;
	Vector3f startDirection;

};

} // X3D
} // titania

#endif
