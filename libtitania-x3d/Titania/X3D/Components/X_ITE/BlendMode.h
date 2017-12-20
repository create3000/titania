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

#include "../Grouping/X3DGroupingNode.h"

namespace titania {
namespace X3D {

class BlendMode :
	virtual public X3DGroupingNode
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

	virtual
	SFBool &
	enabled ()
	{ return *fields .enabled; }

	virtual
	const SFBool &
	enabled () const
	{ return *fields .enabled; }

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
	sourceColor ()
	{ return *fields .sourceColor; }

	virtual
	const SFString &
	sourceColor () const
	{ return *fields .sourceColor; }

	virtual
	SFString &
	sourceAlpha ()
	{ return *fields .sourceAlpha; }

	virtual
	const SFString &
	sourceAlpha () const
	{ return *fields .sourceAlpha; }

	virtual
	SFString &
	destinationColor ()
	{ return *fields .destinationColor; }

	virtual
	const SFString &
	destinationColor () const
	{ return *fields .destinationColor; }

	virtual
	SFString &
	destinationAlpha ()
	{ return *fields .destinationAlpha; }

	virtual
	const SFString &
	destinationAlpha () const
	{ return *fields .destinationAlpha; }

	virtual
	SFString &
	modeColor ()
	{ return *fields .modeColor; }

	virtual
	const SFString &
	modeColor () const
	{ return *fields .modeColor; }

	virtual
	SFString &
	modeAlpha ()
	{ return *fields .modeAlpha; }

	virtual
	const SFString &
	modeAlpha () const
	{ return *fields .modeAlpha; }

	///  @name Member access

	GLenum
	getSourceColor () const
	{ return sourceColorType; }

	GLenum
	getSourceAlpha () const
	{ return sourceAlphaType; }

	GLenum
	getDestinationColor () const
	{ return destinationColorType; }

	GLenum
	getDestinationAlpha () const
	{ return destinationAlphaType; }

	GLenum
	getModeColor () const
	{ return modeColorType; }

	GLenum
	getModeAlpha () const
	{ return modeAlphaType; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;


protected:

	virtual
	void
	initialize () override;


private:

	///  @name Event handler

	void
	set_sourceColor ();

	void
	set_sourceAlpha ();

	void
	set_destinationColor ();

	void
	set_destinationAlpha ();

	void
	set_modeColor ();

	void
	set_modeAlpha ();

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

		SFBool* const enabled;
		SFColorRGBA* const blendColor;
		SFString* const sourceColor;
		SFString* const sourceAlpha;
		SFString* const destinationColor;
		SFString* const destinationAlpha;
		SFString* const modeColor;
		SFString* const modeAlpha;
	};

	Fields fields;

	GLenum sourceColorType;
	GLenum sourceAlphaType;
	GLenum destinationColorType;
	GLenum destinationAlphaType;
	GLenum modeColorType;
	GLenum modeAlphaType;

};

} // X3D
} // titania

#endif
