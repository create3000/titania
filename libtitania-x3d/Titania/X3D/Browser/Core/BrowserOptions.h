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

#ifndef __TITANIA_X3D_BROWSER_BROWSER_OPTIONS_H__
#define __TITANIA_X3D_BROWSER_BROWSER_OPTIONS_H__

#include "../Core/TextureQualityType.h"
#include "../Core/PrimitiveQualityType.h"
#include "../Shaders/ShadingType.h"

#include "../../Basic/X3DBaseNode.h"
#include "../../Fields.h"

namespace titania {
namespace X3D {

// Table 9.2 — Browser options
// Name                    Type/valid range                         Default                      Description
// SplashScreen            Boolean                                  Implementation-dependent     Display browser splash screen on startup
// Dashboard               Boolean                                  False                        Specified by bound NavigationInfo in content. Display browser navigation user interface.
// Rubberband              Boolean                                  True                         Specified by bound NavigationInfo in content. Display rubber band navigation hint.
// EnableInlineViewpoints  Boolean                                  True                         Viewpoints from Inline nodes are included in list of viewpoints if made available by the Inline node.
// Antialiased             Boolean                                  False                        Render using hardware antialiasing if available
// TextureQuality          Low, Medium, High                        Medium                       Quality of texture map display
// PrimitiveQuality        Low, Medium, High                        Medium                       Render quality (tessellation level) for Box, Cone, Cylinder, Sphere
// QualityWhenMoving       Low, Medium, High,                       Same (as while stationary)   Render quality while camera is moving
// Shading                 Point, Wireframe, Flat, Gouraud, Phong   Gouraud                      Specify shading mode for all objects
// MotionBlur              Boolean                                  False                        Render animations with motion blur
//
// MotionBlurIntesity      Number                                   0.25                         Motion blur intesity in the range (0, 1)
// AnimateStairWalks       Boolean                                  False                        Animate stair walks. This can give unexpected results when the floor is animated.
// Gravity                 Number                                   g                            Gravitational acceleration. The standard value is the acceleration of the earth.

class BrowserOptions :
	virtual public X3DBaseNode
{
public:

	///  @name Construction

	BrowserOptions (X3DExecutionContext* const executionContext);

	virtual
	BrowserOptions*
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

	SFBool &
	SplashScreen ()
	{ return *fields .SplashScreen; }

	const SFBool &
	SplashScreen () const
	{ return *fields .SplashScreen; }

	SFBool &
	Dashboard ()
	{ return *fields .Dashboard; }

	const SFBool &
	Dashboard () const
	{ return *fields .Dashboard; }

	SFBool &
	RubberBand ()
	{ return *fields .RubberBand; }

	const SFBool &
	RubberBand () const
	{ return *fields .RubberBand; }

	SFBool &
	EnableInlineViewpoints ()
	{ return *fields .EnableInlineViewpoints; }

	const SFBool &
	EnableInlineViewpoints () const
	{ return *fields .EnableInlineViewpoints; }

	SFBool &
	Antialiased ()
	{ return *fields .Antialiased; }

	const SFBool &
	Antialiased () const
	{ return *fields .Antialiased; }

	SFString &
	TextureQuality ()
	{ return *fields .TextureQuality; }

	const SFString &
	TextureQuality () const
	{ return *fields .TextureQuality; }

	SFString &
	PrimitiveQuality ()
	{ return *fields .PrimitiveQuality; }

	const SFString &
	PrimitiveQuality () const
	{ return *fields .PrimitiveQuality; }

	SFString &
	QualityWhenMoving ()
	{ return *fields .QualityWhenMoving; }

	const SFString &
	QualityWhenMoving () const
	{ return *fields .QualityWhenMoving; }

	SFString &
	Shading ()
	{ return *fields .Shading; }

	const SFString &
	Shading () const
	{ return *fields .Shading; }

	SFBool &
	MotionBlur ()
	{ return *fields .MotionBlur; }

	const SFBool &
	MotionBlur () const
	{ return *fields .MotionBlur; }

	SFFloat &
	MotionBlurIntensity ()
	{ return *fields .MotionBlurIntensity; }

	const SFFloat &
	MotionBlurIntensity () const
	{ return *fields .MotionBlurIntensity; }

	const SFBool &
	AnimateStairWalks () const
	{ return *fields .AnimateStairWalks; }

	SFBool &
	AnimateStairWalks ()
	{ return *fields .AnimateStairWalks; }

	SFFloat &
	Gravity ()
	{ return *fields .Gravity; }

	const SFFloat &
	Gravity () const
	{ return *fields .Gravity; }

	const SFBool &
	LogarithmicDepthBuffer () const
	{ return *fields .LogarithmicDepthBuffer; }

	SFBool &
	LogarithmicDepthBuffer ()
	{ return *fields .LogarithmicDepthBuffer; }

	///  @name Member access

	const SFEnum <TextureQualityType> &
	getTextureQuality () const
	{ return textureQuality; }

	const SFEnum <PrimitiveQualityType> &
	getPrimitiveQuality () const
	{ return primitiveQuality; }

	const SFEnum <ShadingType> &
	getShading () const
	{ return shading; }


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_Antialiased ();

	void
	set_TextureQuality ();

	void
	set_PrimitiveQuality ();

	void
	set_Shading ();

	void
	set_MotionBlur ();

	void
	set_MotionBlurIntensity ();

	void
	set_LogarithmicDepthBuffer ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields (X3DExecutionContext* const executionContext);

		SFBool* const SplashScreen;
		SFBool* const Dashboard;
		SFBool* const RubberBand;
		SFBool* const EnableInlineViewpoints;
		SFBool* const Antialiased;
		SFString* const TextureQuality;
		SFString* const PrimitiveQuality;
		SFString* const QualityWhenMoving;
		SFString* const Shading;
		SFBool* const MotionBlur;
		SFFloat* const MotionBlurIntensity;
		SFBool* const AnimateStairWalks;
		SFFloat* const Gravity;
		SFBool* const LogarithmicDepthBuffer;
	};

	Fields fields;

	SFEnum <TextureQualityType>   textureQuality;
	SFEnum <PrimitiveQualityType> primitiveQuality;
	SFEnum <ShadingType>          shading;

};

} // X3D
} // titania

#endif
