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

#ifndef __TITANIA_X3D_BROWSER_PROPERTIES_BROWSER_OPTIONS_H__
#define __TITANIA_X3D_BROWSER_PROPERTIES_BROWSER_OPTIONS_H__

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

class X3DParticleEmitterNode;
class Arc2DOptions;
class ArcClose2DOptions;
class Circle2DOptions;
class Disk2DOptions;
class Rectangle2DOptions;
class BoxOptions;
class X3DSphereOptionNode;
class MotionBlur;

class BrowserOptions :
	virtual public X3DBaseNode
{
public:

	BrowserOptions (X3DExecutionContext* const);

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFBool &
	splashScreen ()
	{ return *fields .splashScreen; }

	const SFBool &
	splashScreen () const
	{ return *fields .splashScreen; }

	SFBool &
	dashboard ()
	{ return *fields .dashboard; }

	const SFBool &
	dashboard () const
	{ return *fields .dashboard; }

	SFBool &
	rubberBand ()
	{ return *fields .rubberBand; }

	const SFBool &
	rubberBand () const
	{ return *fields .rubberBand; }

	SFBool &
	enableInlineViewpoints ()
	{ return *fields .enableInlineViewpoints; }

	const SFBool &
	enableInlineViewpoints () const
	{ return *fields .enableInlineViewpoints; }

	SFBool &
	antialiased ()
	{ return *fields .antialiased; }

	const SFBool &
	antialiased () const
	{ return *fields .antialiased; }

	SFString &
	textureQuality ()
	{ return *fields .textureQuality; }

	const SFString &
	textureQuality () const
	{ return *fields .textureQuality; }

	SFString &
	primitiveQuality ()
	{ return *fields .primitiveQuality; }

	const SFString &
	primitiveQuality () const
	{ return *fields .primitiveQuality; }

	SFString &
	qualityWhenMoving ()
	{ return *fields .qualityWhenMoving; }

	const SFString &
	qualityWhenMoving () const
	{ return *fields .qualityWhenMoving; }

	SFString &
	shading ()
	{ return *fields .shading; }

	const SFString &
	shading () const
	{ return *fields .shading; }

	SFBool &
	motionBlur ();

	const SFBool &
	motionBlur () const;

	SFFloat &
	motionBlurIntensity ();

	const SFFloat &
	motionBlurIntensity () const;

	const SFBool &
	animateStairWalks () const
	{ return *fields .animateStairWalks; }

	SFBool &
	animateStairWalks ()
	{ return *fields .animateStairWalks; }

	SFFloat &
	gravity ()
	{ return *fields .gravity; }

	const SFFloat &
	gravity () const
	{ return *fields .gravity; }

	X3DPtr <MotionBlur> &
	motionBlurOptions ()
	{ return fields .motionBlurOptions; }

	const X3DPtr <MotionBlur> &
	motionBlurOptions () const
	{ return fields .motionBlurOptions; }

	X3DPtr <X3DParticleEmitterNode> &
	emitter ()
	{ return fields .emitter; }

	const X3DPtr <X3DParticleEmitterNode> &
	emitter () const
	{ return fields .emitter; }

	X3DPtr <Arc2DOptions> &
	arc2D ()
	{ return fields .arc2D; }

	const X3DPtr <Arc2DOptions> &
	arc2D () const
	{ return fields .arc2D; }

	X3DPtr <ArcClose2DOptions> &
	arcClose2D ()
	{ return fields .arcClose2D; }

	const X3DPtr <ArcClose2DOptions> &
	arcClose2D () const
	{ return fields .arcClose2D; }

	X3DPtr <Circle2DOptions> &
	circle2D ()
	{ return fields .circle2D; }

	const X3DPtr <Circle2DOptions> &
	circle2D () const
	{ return fields .circle2D; }

	X3DPtr <Disk2DOptions> &
	disc2D ()
	{ return fields .disc2D; }

	const X3DPtr <Disk2DOptions> &
	disc2D () const
	{ return fields .disc2D; }

	X3DPtr <Rectangle2DOptions> &
	rectangle2D ()
	{ return fields .rectangle2D; }

	const X3DPtr <Rectangle2DOptions> &
	rectangle2D () const
	{ return fields .rectangle2D; }

	X3DPtr <BoxOptions> &
	box ()
	{ return fields .box; }

	const X3DPtr <BoxOptions> &
	box () const
	{ return fields .box; }

	X3DPtr <X3DSphereOptionNode> &
	sphere ()
	{ return fields .sphere; }

	const X3DPtr <X3DSphereOptionNode> &
	sphere () const
	{ return fields .sphere; }


private:

	virtual
	BrowserOptions*
	create (X3DExecutionContext* const) const final override;

	virtual
	void
	initialize () final override;

	void
	set_antialiased ();

	void
	set_textureQuality ();

	void
	set_primitiveQuality ();

	void
	set_shading ();

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields (X3DExecutionContext* const);

		SFBool* const splashScreen;
		SFBool* const dashboard;
		SFBool* const rubberBand;
		SFBool* const enableInlineViewpoints;
		SFBool* const antialiased;
		SFString* const textureQuality;
		SFString* const primitiveQuality;
		SFString* const qualityWhenMoving;
		SFString* const shading;
		SFBool* const animateStairWalks;
		SFFloat* const gravity;
		X3DPtr <MotionBlur> motionBlurOptions;
		X3DPtr <X3DParticleEmitterNode> emitter;
		X3DPtr <Arc2DOptions> arc2D;
		X3DPtr <ArcClose2DOptions> arcClose2D;
		X3DPtr <Circle2DOptions> circle2D;
		X3DPtr <Disk2DOptions> disc2D;
		X3DPtr <Rectangle2DOptions> rectangle2D;
		X3DPtr <BoxOptions> box;
		X3DPtr <X3DSphereOptionNode> sphere;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
