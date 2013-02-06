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

#include "../../Components/Core/X3DPropertyNode.h"
#include "../../Components/Text/FontStyle.h"
#include "../../Components/Texturing/TextureProperties.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Geometry2D/Arc2DProperties.h"
#include "../Geometry2D/ArcClose2DProperties.h"
#include "../Geometry2D/Circle2DProperties.h"
#include "../Geometry2D/Disk2DProperties.h"
#include "../Geometry2D/Rectangle2DProperties.h"
#include "../Geometry3D/BoxProperties.h"
#include "../Geometry3D/X3DSpherePropertyNode.h"
#include "../Properties/MotionBlur.h"

namespace titania {
namespace X3D {

// Table 9.2 — Browser options
// Name                    Type/valid range                         Default                      Description
// SplashScreen            Boolean                                  Implementation-dependent     Display browser splash screen on startup
// Dashboard               Boolean                                                               Specified by bound NavigationInfo in contentDisplay browser navigation user interface
// EnableInlineViewpoints  Boolean                                  True                         Viewpoints from Inline nodes are included in list of viewpoints if made available by the Inline node.
// Antialiased             Boolean                                  False                        Render using hardware antialiasing if available
// TextureQuality          Low, Medium, High                        Medium                       Quality of texture map display
// PrimitiveQuality        Low, Medium, High                        Medium                       Render quality (tesselation level) for Box, Cone, Cylinder, Sphere
// QualityWhenMoving       Low, Medium, High,                       Same (as while stationary)   SameRender quality while camera is moving
// Shading                 Point, Wireframe, Flat, Gouraud, Phong   Gouraud                      Specify shading mode for all objects
// MotionBlur              Boolean                                  False                        Render animations with motion blur

class BrowserOptions :
	public X3DPropertyNode
{
public:

	SFBool   splashScreen;
	SFBool   dashboard;
	SFBool   enableInlineViewpoints;
	SFBool   antialiased;
	SFString textureQuality;
	SFString primitiveQuality;
	SFString qualityWhenMoving;
	SFString shading;

	SFNode <MotionBlur>            motionBlur;
	SFNode <TextureProperties>     textureProperties;
	SFNode <Arc2DProperties>       arc2DProperties;
	SFNode <ArcClose2DProperties>  arcClose2DProperties;
	SFNode <Circle2DProperties>    circle2DProperties;
	SFNode <Disk2DProperties>      disc2DProperties;
	SFNode <Rectangle2DProperties> rectangle2DProperties;
	SFNode <BoxProperties>         boxProperties;
	SFNode <X3DSpherePropertyNode> sphereProperties;
	SFNode <X3DFontStyleNode>      fontStyle;

	BrowserOptions (X3DExecutionContext* const);


private:

	virtual
	BrowserOptions*
	create (X3DExecutionContext* const)  const;

	virtual
	void
	initialize ();

	void
	set_textureQuality ();

	void
	set_primitiveQuality ();

	void
	set_shading ();

};

} // X3D
} // titania

#endif
