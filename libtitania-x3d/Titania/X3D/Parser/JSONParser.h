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
#include "../Parser/X3DParser.h"

struct json_object;

namespace titania {
namespace X3D {

class JSONParser :
	public X3DParser
{
public:

	///  @name Construction

	JSONParser (const X3DScenePtr &, const basic::uri &, std::istream &);

	///  @name Operations

	virtual
	void
	parseIntoScene () final override;

	///  @name Destruction

	virtual
	~JSONParser () final override;


private:

	///  @name Operations

	X3DBrowser*
	getBrowser () const
	{ return scene -> getBrowser (); }

	void
	rootObject (json_object* const jobj);

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

	bool
	componentObject (json_object* const jobj, ComponentInfoPtr & component);

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

	void
	externProtoDeclareObject (json_object* const jobj);

	void
	protoDeclareObject (json_object* const jobj);

	void
	protoInterfaceObject (json_object* const jobj, const ProtoDeclarationPtr & proto);

	void
	protoBodyObject (json_object* const jobj, const ProtoDeclarationPtr & proto);

	void
	importObject (json_object* const jobj);

	void
	routeObject (json_object* const jobj);

	void
	exportObject (json_object* const jobj);

	void
	nodeObject (json_object* const jobj, const std::string & nodeType, SFNode & node);

	void
	nodeFieldsObject (json_object* const jobj, const SFNode & node);

	void
	fieldValueArray (json_object* const jobj, const SFNode & node);

	void
	fieldValueObject (json_object* const jobj, const SFNode & node);

	void
	fieldArray (json_object* const jobj, X3DBaseNode* const node);

	void
	fieldObject (json_object* const jobj, X3DBaseNode* const node);

	void
	sourceTextArray (json_object* const jobj, const SFNode & node);

	void
	isObject (json_object* const jobj, const SFNode & node);

	void
	connectArray (json_object* const jobj, const SFNode & node);

	void
	connectObject (json_object* const jobj, const SFNode & node);

	bool
	nodeNameString (json_object* const jobj, std::string & value)
	{ return stringValue (jobj, value); }

	bool
	fieldValueValue (json_object* const jobj, X3DFieldDefinition* const field);

	bool
	fieldValueValueSwitch (json_object* const jobj, X3DFieldDefinition* const field);

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

	bool
	sfboolValue (json_object* const jobj, SFBool* const field);

	bool
	mfboolValue (json_object* const jobj, MFBool* const field);

	bool
	sfcolorValue (json_object* const jobj, SFColor* const field);

	bool
	mfcolorValue (json_object* const jobj, MFColor* const field);

	bool
	color3fValue (json_object* const jobj, const int32_t i, Color3f & value);

	bool
	sfcolorRGBAValue (json_object* const jobj, SFColorRGBA* const field);

	bool
	mfcolorRGBAValue (json_object* const jobj, MFColorRGBA* const field);

	bool
	color4fValue (json_object* const jobj, const int32_t i, Color4f & value);

	bool
	sfdoubleValue (json_object* const jobj, SFDouble* const field);

	bool
	mfdoubleValue (json_object* const jobj, MFDouble* const field);

	bool
	sffloatValue (json_object* const jobj, SFFloat* const field);

	bool
	mffloatValue (json_object* const jobj, MFFloat* const field);

	bool
	sfimageValue (json_object* const jobj, SFImage* const field);

	bool
	mfimageValue (json_object* const jobj, MFImage* const field);

	void
	imageValue (json_object* const jobj, const size_t index, const size_t width, const size_t height, MFInt32 & array);

	bool
	sfint32Value (json_object* const jobj, SFInt32* const field);

	bool
	mfint32Value (json_object* const jobj, MFInt32* const field);

	bool
	sfmatrix3dValue (json_object* const jobj, SFMatrix3d* const field);

	bool
	mfmatrix3dValue (json_object* const jobj, MFMatrix3d* const field);

	bool
	matrix3dValue (json_object* const jobj, const int32_t i, Matrix3d & value);

	bool
	sfmatrix3fValue (json_object* const jobj, SFMatrix3f* const field);

	bool
	mfmatrix3fValue (json_object* const jobj, MFMatrix3f* const field);

	bool
	matrix3fValue (json_object* const jobj, const int32_t i, Matrix3f & value);

	bool
	sfmatrix4dValue (json_object* const jobj, SFMatrix4d* const field);

	bool
	mfmatrix4dValue (json_object* const jobj, MFMatrix4d* const field);

	bool
	matrix4dValue (json_object* const jobj, const int32_t i, Matrix4d & value);

	bool
	sfmatrix4fValue (json_object* const jobj, SFMatrix4f* const field);

	bool
	mfmatrix4fValue (json_object* const jobj, MFMatrix4f* const field);

	bool
	matrix4fValue (json_object* const jobj, const int32_t i, Matrix4f & value);

	bool
	sfnodeValue (json_object* const jobj, SFNode* const field);

	bool
	mfnodeValue (json_object* const jobj, MFNode* const field);

	bool
	sfrotationValue (json_object* const jobj, SFRotation* const field);

	bool
	mfrotationValue (json_object* const jobj, MFRotation* const field);

	bool
	rotation4dValue (json_object* const jobj, const int32_t i, Rotation4d & value);

	bool
	sfstringValue (json_object* const jobj, SFString* const field);

	bool
	mfstringValue (json_object* const jobj, MFString* const field);

	bool
	sftimeValue (json_object* const jobj, SFTime* const field);

	bool
	mftimeValue (json_object* const jobj, MFTime* const field);

	bool
	sfvec2dValue (json_object* const jobj, SFVec2d* const field);

	bool
	mfvec2dValue (json_object* const jobj, MFVec2d* const field);

	bool
	vector2dValue (json_object* const jobj, const int32_t i, const UnitCategory unit, Vector2d & value);

	bool
	sfvec2fValue (json_object* const jobj, SFVec2f* const field);

	bool
	mfvec2fValue (json_object* const jobj, MFVec2f* const field);

	bool
	vector2fValue (json_object* const jobj, const int32_t i, const UnitCategory unit, Vector2f & value);

	bool
	sfvec3dValue (json_object* const jobj, SFVec3d* const field);

	bool
	mfvec3dValue (json_object* const jobj, MFVec3d* const field);

	bool
	vector3dValue (json_object* const jobj, const int32_t i, const UnitCategory unit, Vector3d & value);

	bool
	sfvec3fValue (json_object* const jobj, SFVec3f* const field);

	bool
	mfvec3fValue (json_object* const jobj, MFVec3f* const field);

	bool
	vector3fValue (json_object* const jobj, const int32_t i, const UnitCategory unit, Vector3f & value);

	bool
	sfvec4dValue (json_object* const jobj, SFVec4d* const field);

	bool
	mfvec4dValue (json_object* const jobj, MFVec4d* const field);

	bool
	vector4dValue (json_object* const jobj, const int32_t i, const UnitCategory unit, Vector4d & value);

	bool
	sfvec4fValue (json_object* const jobj, SFVec4f* const field);

	bool
	mfvec4fValue (json_object* const jobj, MFVec4f* const field);

	bool
	vector4fValue (json_object* const jobj, const int32_t i, const UnitCategory unit, Vector4f & value);

	///

	static
	struct json_object*
	json_object_object_get (struct json_object* obj, const char* key);

	///  @name Member types

	using ElementsFunction = std::function <void (JSONParser*, json_object* const)>;

	///  @name Static members

	static const std::map <std::string, ElementsFunction> objectsIndex;
	static const std::string                              ProtoInstance;

	///  @name Members

	const X3DScenePtr scene;
	const basic::uri  uri;
	std::istream &    istream;

};

} // X3D
} // titania

#endif
