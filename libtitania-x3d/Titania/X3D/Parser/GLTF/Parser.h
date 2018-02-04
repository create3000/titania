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

#ifndef __TITANIA_X3D_PARSER_GLTF_PARSER_H__
#define __TITANIA_X3D_PARSER_GLTF_PARSER_H__

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DScene.h"
#include "../../Parser/X3DParser.h"

struct json_object;

namespace titania {
namespace X3D {
namespace GLTF {

class Parser :
	public X3D::X3DParser
{
public:

	///  @name Construction

	Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream);

	///  @name Operations

	virtual
	void
	parseIntoScene () final override;

	///  @name Destruction

	virtual
	~Parser () final override;


private:

	///  @name Member types

	struct Buffer
	{
		std::string data;
	};

	using BufferPtr = std::shared_ptr <Buffer>;

	struct BufferView
	{
		BufferPtr buffer;
		int32_t   byteLength;
		int32_t   byteOffset;
		int32_t   byteStride;
	};

	using BufferViewPtr = std::shared_ptr <BufferView>;

	struct Accessor
	{
		BufferViewPtr        bufferView;
		std::string          type;
		int32_t              componentType;
		int32_t              byteOffset;
		int32_t              count;
	};

	using AccessorPtr = std::shared_ptr <Accessor>;

	struct Attributes
	{
		AccessorPtr position;
		AccessorPtr normal;
		AccessorPtr tangent;
		AccessorPtr texxCoord0;
	};

	using AttributesPtr = std::shared_ptr <Attributes>;

	struct Primitive
	{
		AttributesPtr attributes;
		AccessorPtr   indices;
		int32_t       mode;
	};

	using PrimitivePtr   = std::shared_ptr <Primitive>;
	using PrimitiveArray = std::vector <PrimitivePtr>;

	///  @name Member access

	X3DBrowser*
	getBrowser () const
	{ return scene -> getBrowser (); }

	///  @name Operations

	void
	rootObject (json_object* const jobj);

	void
	assetObject (json_object* const jobj);
	
	void
	scenesObject (json_object* const jobj);
	
	void
	sceneNumber (json_object* const jobj);
	
	void
	sceneObject (json_object* const jobj);

	void
	sceneNodesObject (json_object* const jobj, const X3D::X3DPtr <X3D::Group> & groupNode);

	void
	nodesObject (json_object* const jobj);
	
	void
	node1Object (json_object* const jobj);
	
	void
	node2Object (json_object* const jobj, const X3D::X3DPtr <X3D::Transform> & transformNode);

	void
	nodeChildrenObject (json_object* const jobj, const X3D::X3DPtr <X3D::Transform> & transformNode);

	void
	meshesObject (json_object* const jobj);

	bool
	meshObject (json_object* const jobj);

	X3D::X3DPtrArray <X3D::Shape>
	meshArray (json_object* const jobj);

	X3D::X3DPtr <X3D::Shape>
	createShape (const PrimitivePtr & primitive);

	X3D::X3DPtr <X3D::IndexedTriangleSet>
	createIndexedTriangleSet (const PrimitivePtr & primitive);

	X3D::X3DPtr <X3D::TriangleSet>
	createTriangleSet (const PrimitivePtr & primitive);

	PrimitiveArray
	primitivesArray (json_object* const jobj);

	PrimitivePtr
	primitiveValue (json_object* const jobj);

	AttributesPtr
	attributesValue (json_object* const jobj);

	void
	asseccorsObject (json_object* const jobj);

	AccessorPtr
	accessorValue (json_object* const jobj);

	void
	bufferViewsObject (json_object* const jobj);

	BufferViewPtr
	bufferViewValue (json_object* const jobj);

	void
	buffersObject (json_object* const jobj);

	BufferPtr
	bufferValue (json_object* const jobj);

	///

	bool
	doubleValue (json_object* const jobj, double & value);

	bool
	integerValue (json_object* const jobj, int32_t & value);

	bool
	matrix4dValue (json_object* const jobj, Matrix4d & value);

	bool
	rotation4dValue (json_object* const jobj, Rotation4d & value);

	bool
	stringValue (json_object* const jobj, std::string & value);

	bool
	vector3dValue (json_object* const jobj, Vector3d & value);

	struct json_object*
	json_object_object_get (struct json_object* obj, const char *key);

	///  @name Member types

	using ElementsFunction = std::function <void (Parser*, json_object* const)>;

	///  @name Static members
	
	static const std::map <std::string, ElementsFunction> objectsIndex;

	///  @name Members

	const X3D::X3DScenePtr scene;
	const basic::uri       uri;
	std::istream &         istream;

	X3D::X3DPtr <X3D::Switch>                   scenes;
	X3D::X3DPtrArray <X3D::Transform>           nodes;
	std::vector <X3D::X3DPtrArray <X3D::Shape>> meshes;
	std::vector <AccessorPtr>                   accessors;
	std::vector <BufferViewPtr>                 bufferViews;
	std::vector <BufferPtr>                     buffers;

};

} // GLTF
} // X3D
} // titania

#endif
