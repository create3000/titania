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

#ifndef __TITANIA_X3D_COMPONENTS_TEXTURING_TEXTURE_PROPERTIES_H__
#define __TITANIA_X3D_COMPONENTS_TEXTURING_TEXTURE_PROPERTIES_H__

#include "../Core/X3DNode.h"

namespace titania {
namespace X3D {

enum CompressionMode : uint8_t
{
	LOW,
	MEDIUM,
	HIGH,
	DEFAULT,
	FASTEST,
	NICEST

};

class TextureProperties :
	public X3DNode
{
public:

	///  @name Construction

	TextureProperties (X3DExecutionContext* const executionContext);

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

	SFColorRGBA &
	borderColor ()
	{ return *fields .borderColor; }

	const SFColorRGBA &
	borderColor () const
	{ return *fields .borderColor; }

	SFInt32 &
	borderWidth ()
	{ return *fields .borderWidth; }

	const SFInt32 &
	borderWidth () const
	{ return *fields .borderWidth; }

	SFFloat &
	anisotropicDegree ()
	{ return *fields .anisotropicDegree; }

	const SFFloat &
	anisotropicDegree () const
	{ return *fields .anisotropicDegree; }

	SFBool &
	generateMipMaps ()
	{ return *fields .generateMipMaps; }

	const SFBool &
	generateMipMaps () const
	{ return *fields .generateMipMaps; }

	SFString &
	minificationFilter ()
	{ return *fields .minificationFilter; }

	const SFString &
	minificationFilter () const
	{ return *fields .minificationFilter; }

	SFString &
	magnificationFilter ()
	{ return *fields .magnificationFilter; }

	const SFString &
	magnificationFilter () const
	{ return *fields .magnificationFilter; }

	SFString &
	boundaryModeS ()
	{ return *fields .boundaryModeS; }

	const SFString &
	boundaryModeS () const
	{ return *fields .boundaryModeS; }

	SFString &
	boundaryModeT ()
	{ return *fields .boundaryModeT; }

	const SFString &
	boundaryModeT () const
	{ return *fields .boundaryModeT; }

	SFString &
	boundaryModeR ()
	{ return *fields .boundaryModeR; }

	const SFString &
	boundaryModeR () const
	{ return *fields .boundaryModeR; }

	SFString &
	textureCompression ()
	{ return *fields .textureCompression; }

	const SFString &
	textureCompression () const
	{ return *fields .textureCompression; }

	SFFloat &
	texturePriority ()
	{ return *fields .texturePriority; }

	const SFFloat &
	texturePriority () const
	{ return *fields .texturePriority; }

	///  @name Member access

	GLenum
	getBoundaryModeS () const;

	GLenum
	getBoundaryModeT () const;

	GLenum
	getBoundaryModeR () const;

	GLenum
	getMinificationFilter () const;

	GLenum
	getMagnificationFilter () const;

	CompressionMode
	getTextureCompression () const;

	GLenum
	getInternalFormat (int32_t) const;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Member access

	GLenum
	getBoundaryMode (const std::string &) const;

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFColorRGBA* const borderColor;
		SFInt32* const borderWidth;
		SFFloat* const anisotropicDegree;
		SFBool* const generateMipMaps;
		SFString* const minificationFilter;
		SFString* const magnificationFilter;
		SFString* const boundaryModeS;
		SFString* const boundaryModeT;
		SFString* const boundaryModeR;
		SFString* const textureCompression;
		SFFloat* const texturePriority;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
