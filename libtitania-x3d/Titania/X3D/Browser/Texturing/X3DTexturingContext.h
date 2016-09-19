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

#ifndef __TITANIA_X3D_BROWSER_TEXTURING_X3DTEXTURING_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_TEXTURING_X3DTEXTURING_CONTEXT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Fields.h"

#include <stack>
#include <vector>

namespace titania {
namespace X3D {

class TextureProperties;
class X3DTextureCoordinateNode;
class X3DTextureNode;
class X3DTextureTransformNode;

using TextureUnitStack = std::stack <int32_t, std::vector <int32_t>>;
using TextureArray     = std::vector <int32_t>;

class X3DTexturingContext :
	virtual public X3DBaseNode
{
public:

	///  @name Member access

	size_t
	getTextureMemory () const
	{ return textureMemory; }

	size_t
	getAvailableTextureMemory () const;

	size_t
	getMinTextureSize () const
	{ return minTextureSize; }

	size_t
	getMaxTextureSize () const
	{ return maxTextureSize; }

	size_t
	getMaxTextureUnits () const
	{ return maxTextureUnits; }

	size_t
	getMaxCombinedTextureUnits () const
	{ return maxCombinedTextureUnits; }

	TextureUnitStack &
	getTextureUnits ()
	{ return textureUnits; }

	TextureUnitStack &
	getCombinedTextureUnits ()
	{ return combinedTextureUnits; }

	TextureArray &
	getTextureStages ()
	{ return textureStages; }

	#ifdef FIXED_PIPELINE
	const X3DPtr <X3DTextureCoordinateNode> &
	getDefaultTexCoord () const
	{ return defaultTexCoord; }
	#endif

	const X3DPtr <TextureProperties> &
	getDefaultTextureProperties () const
	{ return defaultTextureProperties; }

	const X3DPtr <X3DTextureTransformNode> &
	getDefaultTextureTransform () const
	{ return defaultTextureTransform; }

	void
	setTexture (X3DTextureNode* const value)
	{ texture = value; }

	X3DTextureNode*
	getTexture () const
	{ return texture; }

	void
	setTextureTransform (X3DTextureTransformNode* const value)
	{ textureTransform = value; }

	X3DTextureTransformNode*
	getTextureTransform () const
	{ return textureTransform; }

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }

	virtual
	~X3DTexturingContext ();


protected:

	///  @name Construction

	X3DTexturingContext ();

	virtual
	void
	initialize () override;


private:

	///  @name Members

	size_t           textureMemory;
	size_t           minTextureSize;
	int32_t          maxTextureSize;
	int32_t          maxTextureUnits;
	int32_t          maxCombinedTextureUnits;
	TextureUnitStack textureUnits;
	TextureUnitStack combinedTextureUnits;
	TextureArray     textureStages;

	#ifdef FIXED_PIPELINE
	X3DPtr <X3DTextureCoordinateNode> defaultTexCoord;
	#endif
	X3DPtr <TextureProperties>        defaultTextureProperties;
	X3DPtr <X3DTextureTransformNode>  defaultTextureTransform;
	X3DTextureNode*                   texture;
	X3DTextureTransformNode*          textureTransform;

};

} // X3D
} // titania

#endif
