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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_BROWSER_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_BROWSER_H__

#include "../../Fields.h"
#include "../../Types/Pointer.h"
#include "jsObjectType.h"

#include <iostream>
#include <jsapi.h>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

class jsBrowser
{
public:	

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr ObjectType
	getId ()
	{ return ObjectType::Browser; }


private:

	///  @name Member types

	enum Property
	{
		NAME,
		VERSION,
		CURRENT_SPEED,
		CURRENT_FRAMERATE,
		DESCRIPTION,
		SUPPORTED_COMPONENTS,
		SUPPORTED_PROFILES,
		CURRENT_SCENE
	};

	///  @name X3D properties

	static JSBool getName                (JSContext*, unsigned, JS::Value*);
	static JSBool getVersion             (JSContext*, unsigned, JS::Value*);
	static JSBool getCurrentSpeed        (JSContext*, unsigned, JS::Value*);
	static JSBool getCurrentFrameRate    (JSContext*, unsigned, JS::Value*);
	static JSBool setDescription         (JSContext*, unsigned, JS::Value*);
	static JSBool getDescription         (JSContext*, unsigned, JS::Value*);
	static JSBool getSupportedComponents (JSContext*, unsigned, JS::Value*);
	static JSBool getSupportedProfiles   (JSContext*, unsigned, JS::Value*);
	static JSBool getCurrentScene        (JSContext*, unsigned, JS::Value*);

	///  @name X3D functions

	static JSBool replaceWorld         (JSContext*, unsigned, JS::Value*);
	static JSBool createX3DFromString  (JSContext*, unsigned, JS::Value*);
	static JSBool createX3DFromURL     (JSContext*, unsigned, JS::Value*);
	static JSBool loadURL              (JSContext*, unsigned, JS::Value*);
	static JSBool getRenderingProperty (JSContext*, unsigned, JS::Value*);
	static JSBool getBrowserProperty   (JSContext*, unsigned, JS::Value*);
	static JSBool getBrowserOption     (JSContext*, unsigned, JS::Value*);
	static JSBool setBrowserOption     (JSContext*, unsigned, JS::Value*);
	
	static JSBool firstViewpoint       (JSContext*, unsigned, JS::Value*);
	static JSBool previousViewpoint    (JSContext*, unsigned, JS::Value*);
	static JSBool nextViewpoint        (JSContext*, unsigned, JS::Value*);
	static JSBool lastViewpoint        (JSContext*, unsigned, JS::Value*);

	static JSBool print                (JSContext*, unsigned, JS::Value*);
	static JSBool println              (JSContext*, unsigned, JS::Value*);

	///  @name VRML97 functions

	static JSBool getWorldURL          (JSContext*, unsigned, JS::Value*);
	static JSBool createVrmlFromString (JSContext*, unsigned, JS::Value*);
	static JSBool createVrmlFromURL    (JSContext*, unsigned, JS::Value*);
	static JSBool addRoute             (JSContext*, unsigned, JS::Value*);
	static JSBool deleteRoute          (JSContext*, unsigned, JS::Value*);

	///  @name Event handler

	static
	void
	setSceneAsync (const X3D::SFNode & script, const X3D::SFNode &, X3D::MFNode* const, X3D::X3DScenePtr &&);

	///  @name Static members

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif
