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

#ifndef __TITANIA_X3D_RENDERING_SHAPE_CONTAINER_H__
#define __TITANIA_X3D_RENDERING_SHAPE_CONTAINER_H__

#include "../Browser/Rendering/GeometryType.h"
#include "../Rendering/ClipPlaneContainer.h"
#include "../Rendering/LightContainer.h"
#include "../Rendering/X3DCollectableObject.h"
#include "../Rendering/X3DShapeContainer.h"

#include "../Types/Geometry.h"
#include "../Types/Numbers.h"

#include <memory>

namespace titania {
namespace X3D {

class X3DBrowser;
class X3DFogObject;
class X3DRenderObject;
class X3DShapeNode;

class ShapeContainer :
	public X3DShapeContainer
{
public:

	///  @name Construction

	ShapeContainer (X3DRenderObject* const renderObject, const bool transparent);

	///  @name Member access

	X3DBrowser*
	getBrowser () const;

	X3DRenderObject*
	getRenderer () const
	{ return renderObject; }

	bool
	isTransparent () const
	{ return transparent; }

	void
	setFog (X3DFogObject* const value)
	{ fog = value; }

	X3DFogObject*
	getFog () const
	{ return fog; }

	void
	setLocalLights (const LightContainerArray & value)
	{ localLights = value; }

	const LightContainerArray &
	getLocalLights () const
	{ return localLights; }

	bool
	getShadow () const
	{ return shadow; }

	void
	setShadow (const bool value)
	{ shadow = value; }

	void
	setGeometryType (const GeometryType value)
	{ geometryType = value; }

	GeometryType
	getGeometryType () const
	{ return geometryType; }

	void
	setColorMaterial (const bool value)
	{ colorMaterial = value; }

	bool
	getColorMaterial () const
	{ return colorMaterial; }

	void
	setDistance (double value)
	{ distance = value; }

	double
	getDistance () const
	{ return distance; }

	///  @name Operations

	void
	display ();


private:

	///  @name Members

	X3DRenderObject* const  renderObject;
	bool                    transparent;
	X3DFogObject*           fog;
	LightContainerArray     localLights;
	bool                    shadow;
	GeometryType            geometryType;
	bool                    colorMaterial;
	double                  distance;

};

class ShapeContainerComp
{
public:

	bool
	operator () (const std::shared_ptr <ShapeContainer> & lhs, const std::shared_ptr <ShapeContainer> & rhs) const
	{
		return lhs -> getDistance () < rhs -> getDistance ();
	}

};

using ShapeContainerArray = std::vector <std::shared_ptr <ShapeContainer>>;

} // X3D
} // titania

#endif
