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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_BROWSER_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_BROWSER_H__

#include "ObjectType.h"

#include "../../Types/Pointer.h"

#include <jsapi.h>

namespace titania {
namespace X3D {
namespace spidermonkey {

class X3DBrowser
{
public:	

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JSObject*
	create (JSContext* const cx);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ return ObjectType::X3DBrowser; }


private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name X3D properties

	static bool getName                (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getVersion             (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getCurrentSpeed        (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getCurrentFrameRate    (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool setDescription         (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getDescription         (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getSupportedComponents (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getSupportedProfiles   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool getCurrentScene        (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name X3D functions

//	static bool replaceWorld         (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool createX3DFromString  (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool createX3DFromURL     (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool loadURL              (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getRenderingProperty (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getBrowserProperty   (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getBrowserOption     (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool setBrowserOption     (JSContext* cx, unsigned argc, JS::Value* vp);
	
//	static bool firstViewpoint       (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool previousViewpoint    (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool nextViewpoint        (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool lastViewpoint        (JSContext* cx, unsigned argc, JS::Value* vp);

//	static bool print                (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool println              (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name VRML97 functions

//	static bool getWorldURL          (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool setDescription       (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool createVrmlFromString (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool createVrmlFromURL    (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool addRoute             (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool deleteRoute          (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Event handler

//	static
//	void
//	setSceneAsync (const X3D::SFNode & script, const X3D::SFNode & node, X3D::MFNode* const field, X3D::X3DScenePtr && scene);

	///  @name Static members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

} // spidermonkey
} // X3D
} // titania

#endif
