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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#ifndef __TITANIA_X3D_COMPONENTS_LIGHTING_X3DLIGHT_NODE_H__
#define __TITANIA_X3D_COMPONENTS_LIGHTING_X3DLIGHT_NODE_H__

#include "../Core/X3DChildNode.h"

namespace titania {
namespace X3D {

class LightContainer;

class X3DLightNode :
	virtual public X3DChildNode
{
public:

	///  @name Fields

	virtual
	SFBool &
	global ()
	{ return *fields .global; }

	virtual
	const SFBool &
	global () const
	{ return *fields .global; }

	virtual
	SFBool &
	on ()
	{ return *fields .on; }

	virtual
	const SFBool &
	on () const
	{ return *fields .on; }

	virtual
	SFColor &
	color ()
	{ return *fields .color; }

	virtual
	const SFColor &
	color () const
	{ return *fields .color; }

	virtual
	SFFloat &
	intensity ()
	{ return *fields .intensity; }

	virtual
	const SFFloat &
	intensity () const
	{ return *fields .intensity; }

	virtual
	SFFloat &
	ambientIntensity ()
	{ return *fields .ambientIntensity; }

	virtual
	const SFFloat &
	ambientIntensity () const
	{ return *fields .ambientIntensity; }

	///  @name Experimental

	virtual
	SFColor &
	shadowColor ()
	{ return *fields .shadowColor; }

	virtual
	const SFColor &
	shadowColor () const
	{ return *fields .shadowColor; }

	virtual
	SFFloat &
	shadowIntensity ()
	{ return *fields .shadowIntensity; }

	virtual
	const SFFloat &
	shadowIntensity () const
	{ return *fields .shadowIntensity; }

	virtual
	SFFloat &
	shadowDiffusion ()
	{ return *fields .shadowDiffusion; }

	virtual
	const SFFloat &
	shadowDiffusion () const
	{ return *fields .shadowDiffusion; }

	virtual
	SFInt32 &
	shadowMapSize ()
	{ return *fields .shadowMapSize; }

	virtual
	const SFInt32 &
	shadowMapSize () const
	{ return *fields .shadowMapSize; }

	///  @name Operations

	virtual
	void
	push (X3DGroupingNode* const);

	virtual
	void
	pop ();

	virtual
	void
	draw (const GLenum) = 0;

	virtual
	void
	setShaderUniforms (X3DProgrammableShaderObject* const, const size_t, const Matrix4d &) = 0;


protected:

	///  @name Friends

	friend class LightContainer;

	///  @name Construction

	X3DLightNode ();

	///  @name Operations

	virtual
	void
	renderShadowMap (LightContainer* const) = 0;


private:

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const global;
		SFBool* const on;
		SFColor* const color;
		SFFloat* const intensity;
		SFFloat* const ambientIntensity;

		// Experimental
		SFColor* const shadowColor;
		SFFloat* const shadowIntensity;
		SFFloat* const shadowDiffusion;
		SFInt32* const shadowMapSize;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
