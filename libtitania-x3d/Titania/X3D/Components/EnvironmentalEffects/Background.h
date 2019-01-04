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

#ifndef __TITANIA_X3D_COMPONENTS_ENVIRONMENTAL_EFFECTS_BACKGROUND_H__
#define __TITANIA_X3D_COMPONENTS_ENVIRONMENTAL_EFFECTS_BACKGROUND_H__

#include "../EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Texturing/ImageTexture.h"
#include "../Texturing/TextureProperties.h"

namespace titania {
namespace X3D {

class Background :
	public X3DBackgroundNode
{
public:

	///  @name Construction

	Background (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;
	
	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	Background*
	copy (const CopyType type) const final override
	{ return copy (getExecutionContext (), type); }

	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	Background*
	copy (X3DExecutionContext* const, const CopyType) const final override;

	///  Sets the current excecution context to @a executionContext.
	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

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

	MFString &
	frontUrl ()
	{ return *fields .frontUrl; }

	const MFString &
	frontUrl () const
	{ return *fields .frontUrl; }

	MFString &
	backUrl ()
	{ return *fields .backUrl; }

	const MFString &
	backUrl () const
	{ return *fields .backUrl; }

	MFString &
	leftUrl ()
	{ return *fields .leftUrl; }

	const MFString &
	leftUrl () const
	{ return *fields .leftUrl; }

	MFString &
	rightUrl ()
	{ return *fields .rightUrl; }

	const MFString &
	rightUrl () const
	{ return *fields .rightUrl; }

	MFString &
	topUrl ()
	{ return *fields .topUrl; }

	const MFString &
	topUrl () const
	{ return *fields .topUrl; }

	MFString &
	bottomUrl ()
	{ return *fields .bottomUrl; }

	const MFString &
	bottomUrl () const
	{ return *fields .bottomUrl; }


private:

	///  @name Construction

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

		MFString* const frontUrl;
		MFString* const backUrl;
		MFString* const leftUrl;
		MFString* const rightUrl;
		MFString* const topUrl;
		MFString* const bottomUrl;
	};

	Fields fields;

	X3DPtr <ImageTexture>      frontTexture;
	X3DPtr <ImageTexture>      backTexture;
	X3DPtr <ImageTexture>      leftTexture;
	X3DPtr <ImageTexture>      rightTexture;
	X3DPtr <ImageTexture>      topTexture;
	X3DPtr <ImageTexture>      bottomTexture;
	X3DPtr <TextureProperties> textureProperties;

};

} // X3D
} // titania

#endif
