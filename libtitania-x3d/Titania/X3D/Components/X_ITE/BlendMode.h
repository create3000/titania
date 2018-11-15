/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_COMPONENTS_X_ITE_BLEND_MODE_H__
#define __TITANIA_X3D_COMPONENTS_X_ITE_BLEND_MODE_H__

#include "../Shape/X3DAppearanceChildNode.h"

namespace titania {
namespace X3D {

class BlendMode :
	virtual public X3DAppearanceChildNode
{
public:

	///  @name Construction

	BlendMode (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
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
	SFColorRGBA &
	blendColor ()
	{ return *fields .blendColor; }

	virtual
	const SFColorRGBA &
	blendColor () const
	{ return *fields .blendColor; }

	virtual
	SFString &
	sourceColorFactor ()
	{ return *fields .sourceColorFactor; }

	virtual
	const SFString &
	sourceColorFactor () const
	{ return *fields .sourceColorFactor; }

	virtual
	SFString &
	sourceAlphaFactor ()
	{ return *fields .sourceAlphaFactor; }

	virtual
	const SFString &
	sourceAlphaFactor () const
	{ return *fields .sourceAlphaFactor; }

	virtual
	SFString &
	destinationColorFactor ()
	{ return *fields .destinationColorFactor; }

	virtual
	const SFString &
	destinationColorFactor () const
	{ return *fields .destinationColorFactor; }

	virtual
	SFString &
	destinationAlphaFactor ()
	{ return *fields .destinationAlphaFactor; }

	virtual
	const SFString &
	destinationAlphaFactor () const
	{ return *fields .destinationAlphaFactor; }

	virtual
	SFString &
	colorEquation ()
	{ return *fields .colorEquation; }

	virtual
	const SFString &
	colorEquation () const
	{ return *fields .colorEquation; }

	virtual
	SFString &
	alphaEquation ()
	{ return *fields .alphaEquation; }

	virtual
	const SFString &
	alphaEquation () const
	{ return *fields .alphaEquation; }

	///  @name Member access

	GLenum
	getSourceColorFactor () const
	{ return sourceColorFactorType; }

	GLenum
	getSourceAlphaFactor () const
	{ return sourceAlphaFactorType; }

	GLenum
	getDestinationColorFactor () const
	{ return destinationColorFactorType; }

	GLenum
	getDestinationAlphaFactor () const
	{ return destinationAlphaFactorType; }

	GLenum
	getColorEquation () const
	{ return colorEquationType; }

	GLenum
	getAlphaEquation () const
	{ return alphaEquationType; }

	///  @name Operations

	virtual
	void
	draw (X3DRenderObject* const renderObject) final override
	{ }

	void
	enable ();

	void
	disable ();


protected:

	///  @name Friends

	friend class BlendModeContainer;

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Event handler

	void
	set_sourceColorFactor ();

	void
	set_sourceAlphaFactor ();

	void
	set_destinationColorFactor ();

	void
	set_destinationAlphaFactor ();

	void
	set_colorEquation ();

	void
	set_alphaEquation ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	static const std::map <std::string, GLenum> blendTypes;
	static const std::map <std::string, GLenum> blendModes;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFColorRGBA* const blendColor;
		SFString* const sourceColorFactor;
		SFString* const sourceAlphaFactor;
		SFString* const destinationColorFactor;
		SFString* const destinationAlphaFactor;
		SFString* const colorEquation;
		SFString* const alphaEquation;
	};

	Fields fields;

	GLenum sourceColorFactorType;
	GLenum sourceAlphaFactorType;
	GLenum destinationColorFactorType;
	GLenum destinationAlphaFactorType;
	GLenum colorEquationType;
	GLenum alphaEquationType;

};

} // X3D
} // titania

#endif
