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

#ifndef __TITANIA_X3D_COMPONENTS_ENVIRONMENTAL_EFFECTS_X3DFOG_OBJECT_H__
#define __TITANIA_X3D_COMPONENTS_ENVIRONMENTAL_EFFECTS_X3DFOG_OBJECT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Bits/X3DConstants.h"
#include "../../Fields.h"

#include "../../Rendering/OpenGL.h"

namespace titania {
namespace X3D {

class ShapeContainer;

class X3DFogObject :
	virtual public X3DBaseNode
{
public:

	///  @name Fields

	SFColor &
	color ()
	{ return *fields .color; }

	const SFColor &
	color () const
	{ return *fields .color; }

	SFString &
	fogType ()
	{ return *fields .fogType; }

	const SFString &
	fogType () const
	{ return *fields .fogType; }

	SFFloat &
	visibilityRange ()
	{ return *fields .visibilityRange; }

	const SFFloat &
	visibilityRange () const
	{ return *fields .visibilityRange; }

	///  @name Member access
	
	void
	isHidden (const bool value);

	bool
	isHidden () const
	{ return hidden; }	

	///  @name Operations

	void
	enable ();

	void
	setShaderUniforms (X3DProgrammableShaderObject* const shaderObject);

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	X3DFogObject ();

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_fogType ();

	float
	getVisibilityRange () const;

	GLenum
	getMode () const;

	float
	getDensitiy (const float visibilityRange) const;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFColor* const color;
		SFString* const fogType;
		SFFloat* const visibilityRange;
	};

	Fields fields;

	bool    hidden;
	size_t  mode;

};

} // X3D
} // titania

#endif
