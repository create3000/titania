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

#ifndef __TITANIA_X3D_COMPONENTS_TEXTURING_TEXTURE_COORDINATE_GENERATOR_H__
#define __TITANIA_X3D_COMPONENTS_TEXTURING_TEXTURE_COORDINATE_GENERATOR_H__

#include "../Texturing/X3DTextureCoordinateNode.h"

namespace titania {
namespace X3D {

class TextureCoordinateGenerator :
	public X3DTextureCoordinateNode
{
public:

	TextureCoordinateGenerator (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Fields

	SFString &
	mode ()
	{ return *fields .mode; }

	const SFString &
	mode () const
	{ return *fields .mode; }

	MFFloat &
	parameter ()
	{ return *fields .parameter; }

	const MFFloat &
	parameter () const
	{ return *fields .parameter; }

	///  @name Member access

	virtual
	void
	init (TexCoordArray &, const size_t) const final override;

	virtual
	void
	set1Point (const size_t, const Vector4f &) final override
	{ }

	virtual
	Vector4f
	get1Point (const size_t) final override
	{ return Vector4f (0, 0, 0, 1); }

	virtual
	void
	addTexCoord (const size_t, TexCoordArray &, const size_t) const final override
	{ }

	virtual
	void
	getTexCoord (std::vector <Vector4f> &) const final override
	{ }

	virtual
	void
	enable (const int32_t, const size_t, const std::vector <GLuint> &) const final override;

	virtual
	void
	disable (const int32_t) const final override;

	virtual
	bool
	isEmpty () const final override
	{ return true; }

	virtual
	size_t
	getSize () const final override
	{ return 0; }

	///  @name Operations
	
	virtual
	void
	resize (const size_t value) final override
	{  }


private:

	enum ModeType 
	{
		SPHERE,
		SPHERE_LOCAL,
		SPHERE_REFLECT,
		SPHERE_REFLECT_LOCAL,
		CAMERASPACENORMAL,
		CAMERASPACEPOSITION,
		CAMERASPACEREFLECTIONVECTOR,
		COORD_EYE,
		COORD,
		NOISE_EYE,
		NOISE
	};

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handler

	void
	set_mode ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFString* const mode;
		MFFloat* const parameter;
	};

	Fields fields;

	ModeType modeType;

};

} // X3D
} // titania

#endif
