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

#ifndef __TITANIA_X3D_BROWSER_RENDERING_PROPERTIES_H__
#define __TITANIA_X3D_BROWSER_RENDERING_PROPERTIES_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Fields.h"
#include "../../Types/Pointer.h"
#include "../Shaders/ShadingType.h"

#include <Titania/Chrono/StopWatch.h>
#include <set>

namespace titania {
namespace X3D {

// Property Name     Value data type      Description
// Shading           String               The type of shading algorithm in use. Typical values are Flat, Gouraud, Phong, Wireframe.
// MaxTextureSize    String               The maximum texture size supported. The format shall be WIDTHxHEIGHT describing the number of pixels in each direction (for example 1024x1024).
// TextureUnits      Integer              The number of texture units supported for doing multitexture.
// MaxLights         Integer              The maximum number of lights supported.
// AntiAliased       Boolean              True or false if the rendering is currently anti-aliased or not
// ColorDepth        Integer              The number of bits of colour depth supported by the screen. Allows for optimized selection of textures, particularly for lower colour depth screen capabilities.
// TextureMemory     Float                The amount of memory in megabytes available for textures to be placed on the video card.

class RenderingProperties :
	public X3DBaseNode
{
public:

	///  @name Construction

	RenderingProperties (X3DExecutionContext* const executionContext);

	virtual
	RenderingProperties*
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

	SFBool &
	Enabled ()
	{ return fields .Enabled; }

	const SFBool &
	Enabled () const
	{ return fields .Enabled; }

	SFFloat &
	CycleInterval ()
	{ return fields .CycleInterval; }

	const SFFloat &
	CycleInterval () const
	{ return fields .CycleInterval; }

	SFString &
	Vendor ()
	{ return *fields .Vendor; }

	const SFString &
	Vendor () const
	{ return *fields .Vendor; }

	SFString &
	Renderer ()
	{ return *fields .Renderer; }

	const SFString &
	Renderer () const
	{ return *fields .Renderer; }

	SFString &
	Version ()
	{ return *fields .Version; }

	const SFString &
	Version () const
	{ return *fields .Version; }

	SFString &
	Shading ()
	{ return *fields .Shading; }

	const SFString &
	Shading () const
	{ return *fields .Shading; }

	SFInt32 &
	MaxTextureSize ()
	{ return *fields .MaxTextureSize; }

	const SFInt32 &
	MaxTextureSize () const
	{ return *fields .MaxTextureSize; }

	SFInt32 &
	TextureUnits ()
	{ return *fields .TextureUnits; }

	const SFInt32 &
	TextureUnits () const
	{ return *fields .TextureUnits; }

	SFInt32 &
	MaxLights ()
	{ return *fields .MaxLights; }

	const SFInt32 &
	MaxLights () const
	{ return *fields .MaxLights; }

	SFBool &
	Antialiased ()
	{ return *fields .Antialiased; }

	const SFBool &
	Antialiased () const
	{ return *fields .Antialiased; }

	SFInt32 &
	ColorDepth ()
	{ return *fields .ColorDepth; }

	const SFInt32 &
	ColorDepth () const
	{ return *fields .ColorDepth; }

	SFDouble &
	TextureMemory ()
	{ return *fields .TextureMemory; }

	const SFDouble &
	TextureMemory () const
	{ return *fields .TextureMemory; }

	const SFBool &
	LogarithmicDepthBuffer () const
	{ return *fields .LogarithmicDepthBuffer; }

	SFBool &
	LogarithmicDepthBuffer ()
	{ return *fields .LogarithmicDepthBuffer; }

	///  @name Member access

	const SFEnum <ShadingType> &
	getShading () const
	{ return shading; }

	double
	getFPS () const
	{ return 1 / clock .average (); }

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream &) const final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_Enabled ();

	void
	set_Shading ();

	void
	set_initialized ();

	void
	reset ();

	void
	prepare ();

	void
	display ();

	void
	build ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool Enabled;
		SFFloat CycleInterval;
		SFString* const Vendor;
		SFString* const Renderer;
		SFString* const Version;
		SFString* const Shading;
		SFInt32* const MaxTextureSize;
		SFInt32* const TextureUnits;
		SFInt32* const MaxLights;
		SFBool* const Antialiased;
		SFInt32* const ColorDepth;
		SFDouble* const TextureMemory;
		SFBool* const LogarithmicDepthBuffer;
	};

	Fields fields;

	SFEnum <ShadingType> shading;

	time_type initialized;

	chrono::stopwatch <double> clock;
	chrono::stopwatch <double> renderClock;

	X3DScenePtr scene;

};

} // X3D
} // titania

#endif
