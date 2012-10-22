/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#ifndef __TITANIA_X3D_JAVA_SCRIPT_JS_BROWSER_H__
#define __TITANIA_X3D_JAVA_SCRIPT_JS_BROWSER_H__

#include <iostream>
#include <jsapi.h>

namespace Titania {
namespace X3D {

class X3DBaseNode;

class jsBrowser
{
	enum Property
	{
		NAME,
		VERSION,
		CURRENTSPEED,
		CURRENTFRAMERATE,
		DESCRIPTION,
		SUPPORTEDCOMPONENTS,
		SUPPORTEDPROFILES,
		CURRENTSCENE
	};

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

	static JSBool getName (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool getVersion (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool getCurrentSpeed (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool getCurrentFrameRate (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool getWorldURL (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool replaceWorld (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool createVrmlFromString (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool createVrmlFromURL (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool addRoute (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool deleteRoute (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool loadURL (JSContext *, JSObject *, uintN, jsval *, jsval*);
	static JSBool setDescription (JSContext *, JSObject *, uintN, jsval *, jsval*);

	static JSBool getName (JSContext *, JSObject *, jsval, jsval*);
	static JSBool getVersion (JSContext *, JSObject *, jsval, jsval*);
	static JSBool getCurrentSpeed (JSContext *, JSObject *, jsval, jsval*);
	static JSBool getCurrentFrameRate (JSContext *, JSObject *, jsval, jsval*);
	static JSBool getDescription (JSContext *, JSObject *, jsval, jsval*);
	static JSBool setDescription (JSContext *, JSObject *, jsval, jsval*);
	static JSBool getSupportedComponents (JSContext *, JSObject *, jsval, jsval*);
	static JSBool getSupportedProfiles (JSContext *, JSObject *, jsval, jsval*);
	static JSBool getCurrentScene (JSContext *, JSObject *, jsval, jsval*);


public:

	static void
	defineObject (JSContext*, JSObject*);

	static JSClass*
	getClass () { return &static_class; }

};

} // X3D
} // Titania

#endif
