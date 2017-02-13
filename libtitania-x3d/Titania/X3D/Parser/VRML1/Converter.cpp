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

#include "Converter.h"

#include "../../Components/Grouping/Transform.h"
#include "../../Components/Rendering/X3DNormalNode.h"
#include "../../Components/Rendering/X3DCoordinateNode.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Texturing/X3DTextureNode.h"
#include "../../Components/Texturing/X3DTextureCoordinateNode.h"
#include "../../Components/Texturing/X3DTextureTransformNode.h"

namespace titania {
namespace X3D {
namespace VRML1 {

Converter::Converter (const X3D::X3DScenePtr & scene) :
	                scene (scene),
	           transforms (),
	               groups (),
	            materials ({ scene -> createNode <X3D::Material> () }),
	             textures ({ nullptr }),
	    textureTransforms ({ nullptr }),
	            texCoords ({ nullptr }),
	              normals ({ nullptr }),
	               coords ({ nullptr }),
	           shapeHints (),
	     materialBindings (),
	       normalBindings (),
	       transformsSize (),
	           groupsSize (),
	         texturesSize (),
	textureTransformsSize (),
	        texCoordsSize (),
	          normalsSize (),
	           coordsSize (),
	       shapeHintsSize (),
	 materialBindingsSize (),
	   normalBindingsSize ()
{ } 

void
Converter::save ()
{
	transformsSize        .emplace_back (transforms        .size ());
	groupsSize            .emplace_back (groups            .size ());
	materialsSize         .emplace_back (materials         .size ());
	texturesSize          .emplace_back (textures          .size ());
	textureTransformsSize .emplace_back (textureTransforms .size ());
	texCoordsSize         .emplace_back (texCoords         .size ());
	normalsSize           .emplace_back (normals           .size ());
	coordsSize            .emplace_back (coords            .size ());
	shapeHintsSize        .emplace_back (shapeHints        .size ());
	materialBindingsSize  .emplace_back (materialBindings  .size ());
	normalBindingsSize    .emplace_back (normalBindings    .size ());
}

void
Converter::restore ()
{
	transforms        .resize (transformsSize        .back ());
	groups            .resize (groupsSize            .back ());
	materials         .resize (materialsSize         .back ());
	textures          .resize (texturesSize          .back ());
	textureTransforms .resize (textureTransformsSize .back ());
	texCoords         .resize (texCoordsSize         .back ());
	normals           .resize (normalsSize           .back ());
	coords            .resize (coordsSize            .back ());
	shapeHints        .resize (shapeHintsSize        .back ());
	materialBindings  .resize (materialBindingsSize  .back ());
	normalBindings    .resize (normalBindingsSize    .back ());

	transformsSize        .pop_back ();
	groupsSize            .pop_back ();
	materialsSize         .pop_back ();
	texturesSize          .pop_back ();
	textureTransformsSize .pop_back ();
	texCoordsSize         .pop_back ();
	normalsSize           .pop_back ();
	coordsSize            .pop_back ();
	shapeHintsSize        .pop_back ();
	materialBindingsSize  .pop_back ();
	normalBindingsSize    .pop_back ();
}

Converter::~Converter ()
{ }

} // VRML1
} // X3D
} // titania
