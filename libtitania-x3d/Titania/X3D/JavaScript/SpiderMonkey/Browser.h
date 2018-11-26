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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_BROWSER_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_BROWSER_H__

#include "../../Fields.h"
#include "../../Types/Pointer.h"
#include "ObjectType.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#include <js/jsapi.h>
#pragma GCC diagnostic pop

#include <iostream>

namespace titania {
namespace X3D {
namespace spidermonkey {

class Browser
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

	static JSBool name                (JSContext *, JSObject *, jsid, jsval*);
	static JSBool version             (JSContext *, JSObject *, jsid, jsval*);
	static JSBool currentSpeed        (JSContext *, JSObject *, jsid, jsval*);
	static JSBool currentFrameRate    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool description         (JSContext *, JSObject *, jsid, jsval*);
	static JSBool description         (JSContext *, JSObject *, jsid, JSBool, jsval*);
	static JSBool supportedComponents (JSContext *, JSObject *, jsid, jsval*);
	static JSBool supportedProfiles   (JSContext *, JSObject *, jsid, jsval*);
	static JSBool currentScene        (JSContext *, JSObject *, jsid, jsval*);

	///  @name X3D functions

	static JSBool replaceWorld         (JSContext *, uint32_t, jsval*);
	static JSBool createX3DFromString  (JSContext *, uint32_t, jsval*);
	static JSBool createX3DFromURL     (JSContext *, uint32_t, jsval*);
	static JSBool loadURL              (JSContext *, uint32_t, jsval*);
	static JSBool getRenderingProperty (JSContext *, uint32_t, jsval*);
	static JSBool getBrowserProperty   (JSContext *, uint32_t, jsval*);
	static JSBool getBrowserOption     (JSContext *, uint32_t, jsval*);
	static JSBool setBrowserOption     (JSContext *, uint32_t, jsval*);
	
	static JSBool firstViewpoint       (JSContext *, uint32_t, jsval*);
	static JSBool previousViewpoint    (JSContext *, uint32_t, jsval*);
	static JSBool nextViewpoint        (JSContext *, uint32_t, jsval*);
	static JSBool lastViewpoint        (JSContext *, uint32_t, jsval*);

	static JSBool print                (JSContext *, uint32_t, jsval*);
	static JSBool println              (JSContext *, uint32_t, jsval*);

	///  @name VRML97 functions

	static JSBool getName              (JSContext *, uint32_t, jsval*);
	static JSBool getVersion           (JSContext *, uint32_t, jsval*);
	static JSBool getCurrentSpeed      (JSContext *, uint32_t, jsval*);
	static JSBool getCurrentFrameRate  (JSContext *, uint32_t, jsval*);
	static JSBool getWorldURL          (JSContext *, uint32_t, jsval*);
	static JSBool setDescription       (JSContext *, uint32_t, jsval*);
	static JSBool createVrmlFromString (JSContext *, uint32_t, jsval*);
	static JSBool createVrmlFromURL    (JSContext *, uint32_t, jsval*);
	static JSBool addRoute             (JSContext *, uint32_t, jsval*);
	static JSBool deleteRoute          (JSContext *, uint32_t, jsval*);

	///  @name Event handler

	static
	void
	setSceneAsync (const X3D::SFNode & script, const X3D::SFNode & node, X3D::MFNode* const field, X3D::X3DScenePtr && scene);

	///  @name Static members

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

} // spidermonkey
} // X3D
} // titania

#endif
