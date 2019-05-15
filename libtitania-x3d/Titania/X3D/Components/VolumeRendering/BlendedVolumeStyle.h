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

#ifndef __TITANIA_X3D_COMPONENTS_VOLUME_RENDERING_BLENDED_VOLUME_STYLE_H__
#define __TITANIA_X3D_COMPONENTS_VOLUME_RENDERING_BLENDED_VOLUME_STYLE_H__

#include "../VolumeRendering/X3DComposableVolumeRenderStyleNode.h"

namespace titania {
namespace X3D {

class BlendedVolumeStyle :
	public X3DComposableVolumeRenderStyleNode
{
public:

	///  @name Construction

	BlendedVolumeStyle (X3DExecutionContext* const executionContext);

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

	virtual
	SFFloat &
	weightConstant1 ()
	{ return *fields .weightConstant1; }

	virtual
	const SFFloat &
	weightConstant1 () const
	{ return *fields .weightConstant1; }

	virtual
	SFFloat &
	weightConstant2 ()
	{ return *fields .weightConstant2; }

	virtual
	const SFFloat &
	weightConstant2 () const
	{ return *fields .weightConstant2; }

	virtual
	SFString &
	weightFunction1 ()
	{ return *fields .weightFunction1; }

	virtual
	const SFString &
	weightFunction1 () const
	{ return *fields .weightFunction1; }

	virtual
	SFString &
	weightFunction2 ()
	{ return *fields .weightFunction2; }

	virtual
	const SFString &
	weightFunction2 () const
	{ return *fields .weightFunction2; }

	virtual
	SFNode &
	weightTransferFunction1 ()
	{ return *fields .weightTransferFunction1; }

	virtual
	const SFNode &
	weightTransferFunction1 () const
	{ return *fields .weightTransferFunction1; }

	virtual
	SFNode &
	weightTransferFunction2 ()
	{ return *fields .weightTransferFunction2; }

	virtual
	const SFNode &
	weightTransferFunction2 () const
	{ return *fields .weightTransferFunction2; }

	virtual
	SFNode &
	renderStyle ()
	{ return *fields .renderStyle; }

	virtual
	const SFNode &
	renderStyle () const
	{ return *fields .renderStyle; }

	virtual
	SFNode &
	voxels ()
	{ return *fields .voxels; }

	virtual
	const SFNode &
	voxels () const
	{ return *fields .voxels; }

	///  @name Member access

	virtual
	const X3DPtr <ComposedShader> &
	getShader () const final override
	{ return shaderNode; }

	///  @name Destruction

	virtual
	~BlendedVolumeStyle () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFFloat* const weightConstant1;
		SFFloat* const weightConstant2;
		SFString* const weightFunction1;
		SFString* const weightFunction2;
		SFNode* const weightTransferFunction1;
		SFNode* const weightTransferFunction2;
		SFNode* const renderStyle;
		SFNode* const voxels;
	};

	Fields fields;

	///  @name Members

	X3DPtr <ComposedShader> shaderNode;

};

} // X3D
} // titania

#endif
