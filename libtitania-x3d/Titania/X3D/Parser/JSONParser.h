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

#ifndef __TITANIA_X3D_PARSER_JSON_PARSER_H__
#define __TITANIA_X3D_PARSER_JSON_PARSER_H__

#include "../Execution/X3DScene.h"

struct json_object;

namespace titania {
namespace X3D {

class JSONParser
{
public:

	///  @name Construction

	JSONParser (const X3DScenePtr &, const basic::uri &, std::istream &);

	///  @name Operations

	void
	parseIntoScene ();

	///  @name Destruction

	virtual
	~JSONParser ();


private:

	///  @name Operations

	X3DBrowser*
	getBrowser () const
	{ return scene -> getBrowser (); }

	void
	jsonObject (json_object* const jobj);

	void
	x3dObject (json_object* const jobj);

	bool
	encodingString (json_object* const jobj, std::string & encodingCharacters);
	
	bool
	profileString (json_object* const jobj, std::string & profileCharacters);

	bool
	versionString (json_object* const jobj, std::string & specificationVersionCharacters);

	void
	headObject (json_object* const jobj);

	void
	componentArray (json_object* const jobj);

	void
	componentObject (json_object* const jobj);

	bool
	componentNameString (json_object* const jobj, std::string & componentNameCharacters)
	{ return stringValue (jobj, componentNameCharacters); }
	
	bool
	componentSupportLevelNumber (json_object* const jobj, int32_t & componentSupportLevel)
	{ return integerValue (jobj, componentSupportLevel); }

	void
	unitArray (json_object* const jobj);

	bool
	categoryNameString (json_object* const jobj, std::string & value)
	{ return stringValue (jobj, value); }

	bool
	unitNameString (json_object* const jobj, std::string & value)
	{ return stringValue (jobj, value); }

	bool
	unitConversionFactorNumber (json_object* const jobj, double & value)
	{ return doubleValue (jobj, value); }

	void
	unitObject (json_object* const jobj);

	void
	metaArray (json_object* const jobj);

	void
	metaObject (json_object* const jobj);

	bool
	metakey (json_object* const jobj, std::string & value)
	{ return stringValue (jobj, value); }

	bool
	metavalue (json_object* const jobj, std::string & value)
	{ return stringValue (jobj, value); }

	void
	sceneObject (json_object* const jobj);

	void
	childrenArray (json_object* const jobj, MFNode & nodes);

	bool
	childObject (json_object* const jobj, SFNode & node);

	bool
	externProtoDeclareObject (const std::string & key, json_object* const jobj);

	bool
	protoDeclareObject (const std::string & key, json_object* const jobj);

	bool
	importObject (const std::string & key, json_object* const jobj);

	bool
	routeObject (const std::string & key, json_object* const jobj);

	bool
	exportObject (const std::string & key, json_object* const jobj);

	bool
	nodeObject (const std::string & key, json_object* const jobj, SFNode & node);

	void
	nodeFieldsObject (json_object* const jobj, const SFNode & node);

	void
	fieldValueArray (json_object* const jobj, const SFNode & node);
	
	void
	fieldArray (json_object* const jobj, const SFNode & node);
	
	void
	isObject (json_object* const jobj, const SFNode & node);

	bool
	nodeNameString (json_object* const jobj, std::string & value)
	{ return stringValue (jobj, value); }

	void
	fieldTypeObject (json_object* const jobj, X3DFieldDefinition* const field);

	///  @name Value handling

	bool
	booleanValue (json_object* const jobj, bool & value);

	bool
	doubleValue (json_object* const jobj, double & value);

	bool
	integerValue (json_object* const jobj, int32_t & value);

	bool
	stringValue (json_object* const jobj, std::string & value);

	///  @name Field value handling
	
	void
	sfboolValue (json_object* const jobj, SFBool* const field);
	
	void
	mfboolValue (json_object* const jobj, MFBool* const field);

	void
	sfdoubleValue (json_object* const jobj, SFDouble* const field);

	void
	mfdoubleValue (json_object* const jobj, MFDouble* const field);

	void
	sffloatValue (json_object* const jobj, SFFloat* const field);

	void
	mffloatValue (json_object* const jobj, MFFloat* const field);

	void
	sfint32Value (json_object* const jobj, SFInt32* const field);

	void
	mfint32Value (json_object* const jobj, MFInt32* const field);

	void
	sfnodeValue (json_object* const jobj, SFNode* const field);

	void
	mfnodeValue (json_object* const jobj, MFNode* const field);

	void
	sfstringValue (json_object* const jobj, SFString* const field);
	
	void
	mfstringValue (json_object* const jobj, MFString* const field);

	///  @name Execution context handling

	void
	pushExecutionContext (X3DExecutionContext* const executionContext);
	
	void
	popExecutionContext ();
	
	X3DExecutionContext*
	getExecutionContext () const;
	
	bool
	isInsideProtoDefinition () const;

	///  @name Member types

	typedef std::vector <X3DExecutionContext*> ExecutionContextStack;

	///  @name Members

	const X3DScenePtr scene;
	const basic::uri  uri;
	std::istream &    istream;

	ExecutionContextStack executionContextStack;

};

} // X3D
} // titania

#endif
