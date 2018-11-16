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
namespace glTF {

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

	struct Image
	{
		std::string name;
		std::string uri;
	};

	using ImagePtr      = std::shared_ptr <Image>;
	using ImagePtrArray = std::vector <ImagePtr>;

	struct Buffer
	{
		std::string contents;
	};

	using BufferPtr      = std::shared_ptr <Buffer>;
	using BufferPtrArray = std::vector <BufferPtr>;

	struct BufferView
	{
		BufferPtr buffer;
		int32_t   byteLength;
		int32_t   byteOffset;
		int32_t   byteStride;
	};

	using BufferViewPtr      = std::shared_ptr <BufferView>;
	using BufferViewPtrArray = std::vector <BufferViewPtr>;

	enum class AccessorType
	{
		SCALAR,
		VEC2,
		VEC3,
		VEC4,
		MAT2,
		MAT3,
		MAT4,
	};

	enum class ComponentType
	{
		BYTE,
		UNSIGNED_BYTE,
		SHORT,
		UNSIGNED_SHORT,
		UNSIGNED_INT,
		FLOAT,
	};

	struct Accessor
	{
		BufferViewPtr bufferView;
		AccessorType  type;
		ComponentType componentType;
		int32_t       byteOffset;
		int32_t       count;
		bool          normalized;
	};

	using AccessorPtr      = std::shared_ptr <Accessor>;
	using AccessorPtrArray = std::vector <AccessorPtr>;

	struct Attributes
	{
		AccessorPtr      position;
		AccessorPtr      normal;
		AccessorPtr      tangent;
		AccessorPtrArray texCoord;
		AccessorPtrArray color;
		AccessorPtrArray joints;
		AccessorPtrArray weights;
	};

	using AttributesPtr      = std::shared_ptr <Attributes>;
	using AttributesPtrArray = std::vector <AttributesPtr>;

	struct Primitive
	{
		AttributesPtr                                          attributes;
		AccessorPtr                                            indices;
		X3D::X3DPtr <X3D::X3DNode>                             material;
		int32_t                                                mode;
		AttributesPtrArray                                     targets;
		X3D::X3DPtr <X3D::Shape>                               shapeNode;
		std::map <int32_t, X3D::X3DPtr <X3D::X3DGeometryNode>> jointGeometryNodes;
	};

	using PrimitivePtr   = std::shared_ptr <Primitive>;
	using PrimitiveArray = std::vector <PrimitivePtr>;

	struct Mesh
	{
		PrimitiveArray primitives;
	};

	using MeshPtr      = std::shared_ptr <Mesh>;
	using MeshPtrArray = std::vector <MeshPtr>;

	struct Node
	{
		X3D::X3DPtr <X3D::Transform> transformNode;
		MeshPtr                      mesh;
		int32_t                      skin;
	};

	using NodePtr      = std::shared_ptr <Node>;
	using NodePtrArray = std::vector <NodePtr>;

	enum class InterpolationType
	{
		LINEAR,
		STEP,
		CUBICSPLINE
	};

	struct AnimationSampler
	{
		InterpolationType interpolation;
		AccessorPtr       input;
		AccessorPtr       output;
	};

	using AnimationSamplerPtr   = std::shared_ptr <AnimationSampler>;
	using AnimationSamplerArray = std::vector <AnimationSamplerPtr>;

	enum class PathType
	{
		TRANSLATION,
		ROTATION,
		SCALE,
		WEIGHTS
	};

	struct AnimationTarget
	{
		PathType path;
		NodePtr  node;
	};

	using AnimationTargetPtr = std::shared_ptr <AnimationTarget>;

	struct Skin
	{
		std::vector <Matrix4d> inverseBindMatrices;
		NodePtrArray           joints;
		NodePtr                skeleton;
	};

	using SkinPtr      = std::shared_ptr <Skin>;
	using SkinPtrArray = std::vector <SkinPtr>;

	///  @name Member access

	X3DBrowser*
	getBrowser () const
	{ return scene -> getBrowser (); }

	///  @name Operations

	void
	importProto (const std::string & name);

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
	
	NodePtr
	node1Value (json_object* const jobj);
	
	void
	node2Object (json_object* const jobj, const NodePtr & transformNode);

	void
	nodeChildrenObject (json_object* const jobj, const X3D::X3DPtr <X3D::Transform> & transformNode);

	void
	meshesObject (json_object* const jobj);

	MeshPtr
	meshArray (json_object* const jobj);

	PrimitiveArray
	primitivesArray (json_object* const jobj);

	PrimitivePtr
	primitiveValue (json_object* const jobj);

	AttributesPtr
	attributesValue (json_object* const jobj);

	AttributesPtrArray
	targetsValue (json_object* const jobj);

	AttributesPtr
	targetValue (json_object* const jobj);

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

	void
	samplersObject (json_object* const jobj);

	X3D::X3DPtr <X3D::TextureProperties>
	samplerValue (json_object* const jobj);

	void
	imagesObject (json_object* const jobj);

	ImagePtr
	imageValue (json_object* const jobj);

	void
	texturesObject (json_object* const jobj);

	X3D::X3DPtr <X3D::X3DTextureNode>
	textureValue (json_object* const jobj);

	void
	materialsObject (json_object* const jobj);

	X3D::SFNode
	materialValue (json_object* const jobj);

	void
	emissiveTextureInfo (json_object* const jobj, const X3D::SFNode & appearance);

	void
	occlusionTextureInfo (json_object* const jobj, const X3D::SFNode & appearance);

	void
	normalTextureInfo (json_object* const jobj, const X3D::SFNode & appearance);

	void
	pbrMetallicRoughness (json_object* const jobj, const X3D::SFNode & appearance);

	void
	materialExtensions (json_object* const jobj, const X3D::SFNode & appearanceNode);
	
	void
	pbrSpecularGlossiness (json_object* const jobj, const X3D::SFNode & appearanceNode);

	X3D::X3DPtr <X3D::X3DTextureNode>
	textureInfoValue (json_object* const jobj);

	X3D::X3DPtr <X3D::Shape>
	createShape (const PrimitivePtr & primitive) const;

	X3D::X3DPtr <X3D::X3DNode>
	createAppearance () const;

	X3D::SFNode
	createMetallicRoughness () const;

	X3D::SFNode
	createSpecularGlossiness () const;

	X3D::X3DPtr <X3D::X3DGeometryNode>
	createGeometry (const PrimitivePtr & primitive, const X3D::X3DPtr <X3D::X3DNode> & material) const;

	X3D::X3DPtr <X3D::IndexedTriangleSet>
	createIndexedTriangleSet (const PrimitivePtr & primitive, const X3D::X3DPtr <X3D::X3DNode> & material) const;

	X3D::X3DPtr <X3D::TriangleSet>
	createTriangleSet (const PrimitivePtr & primitive, const X3D::X3DPtr <X3D::X3DNode> & material) const;

	X3D::X3DPtr <X3D::Coordinate>
	createCoordinate (const AccessorPtr & position) const;

	X3D::X3DPtr <X3D::FloatVertexAttribute>
	createTangent (const AccessorPtr & accessor) const;

	X3D::X3DPtr <X3D::Normal>
	createNormal (const AccessorPtr & accessor) const;

	X3D::X3DPtr <X3D::X3DTextureCoordinateNode>
	createTextureCoordinate (const AccessorPtrArray & accessor) const;

	X3D::X3DPtr <X3D::X3DTextureCoordinateNode>
	createSingleTextureCoordinate (const AccessorPtr & accessor) const;

	X3D::X3DPtr <X3D::X3DColorNode>
	createColor (const AccessorPtr & accessor) const;

	///

	void
	animationsObject (json_object* const jobj);

	X3D::X3DPtr <X3D::Group>
	animationValue (json_object* const jobj);

	void
	animationChannelsObject (json_object* const jobj, const X3D::X3DPtr <X3D::Group> & animation, const AnimationSamplerArray & samplers);

	X3D::X3DPtr <X3D::Group>
	animationChannelValue (json_object* const jobj, const AnimationSamplerArray & samplers);

	AnimationTargetPtr
	animationTargetValue (json_object* const jobj);

	AnimationSamplerArray
	animationSamplersValue (json_object* const jobj);

	AnimationSamplerPtr
	animationSamplerValue (json_object* const jobj);

	X3D::X3DPtr <X3D::X3DInterpolatorNode>
	createTranslationInterpolator (const std::vector <double> & times,
	                               const AnimationSamplerPtr & animationSampler,
	                               const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
	                               const X3D::X3DPtr <X3D::Transform> & transformNode);

	X3D::X3DPtr <X3D::X3DInterpolatorNode>
	createRotationInterpolator (const std::vector <double> & times,
	                            const AnimationSamplerPtr & animationSampler,
	                            const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
	                            const X3D::X3DPtr <X3D::Transform> & transformNode);

	X3D::X3DPtr <X3D::X3DInterpolatorNode>
	createScaleInterpolator (const std::vector <double> & times,
	                         const AnimationSamplerPtr & animationSampler,
	                         const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
	                         const X3D::X3DPtr <X3D::Transform> & transformNode);

	X3D::X3DPtr <X3D::CoordinateInterpolator>
	createCoordinateInterpolator (const AttributesPtrArray & targets,
                                 const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
                                 const X3D::X3DPtr <X3D::X3DGeometryNode> & geometryNode) const;

	X3D::X3DPtr <X3D::NormalInterpolator>
	createNormalInterpolator (const AttributesPtrArray & targets,
	                          const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
	                          const X3D::X3DPtr <X3D::X3DGeometryNode> & geometryNode) const;

	///

	void
	skinsObject (json_object* const jobj);

	SkinPtr
	skinValue (json_object* const jobj);

	NodePtrArray
	jointsValue (json_object* const jobj);

	///

	template <class Type>
	std::vector <Type>
	getScalarArray (const AccessorPtr & accessor) const;

	template <class Type>
	std::vector <Type>
	getVectorArray (const AccessorPtr & accessor) const;

	template <class Type>
	std::vector <Type>
	getMatrixArray (const AccessorPtr & accessor) const;

	///

	bool
	booleanValue (json_object* const jobj, bool & value);

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

	bool
	vector4dValue (json_object* const jobj, Vector4d & value);

	Rotation4d
	getRotation (const Vector4d & value);

	///

	struct json_object*
	json_object_object_get (struct json_object* obj, const std::string & key);

	struct json_object*
	json_object_object_get (struct json_object* obj, const char* key);

	///  @name Member types

	using ElementsFunction = std::function <void (Parser*, json_object* const)>;

	///  @name Static members

	static const bool   USE_PROTOTYPES;
	static const double EPSILON;
	
	static const std::map <ComponentType, size_t>                                      componentSizes;
	static const std::map <ComponentType, std::tuple <double, double, double, double>> normalizedRanges;

	///  @name Members

	const X3D::X3DScenePtr scene;
	const basic::uri       uri;
	std::istream &         istream;

	X3D::X3DPtr <X3D::Switch>                 scenes;
	NodePtrArray                              nodes;
	X3D::X3DPtrArray <X3D::TextureProperties> samplers;
	ImagePtrArray                             images;
	X3D::X3DPtrArray <X3D::X3DTextureNode>    textures;
	X3D::MFNode                               materials;
	MeshPtrArray                              meshes;
	AccessorPtrArray                          accessors;
	BufferViewPtrArray                        bufferViews;
	BufferPtrArray                            buffers;
	std::set <std::string>                    extensions;

};

template <class Type>
std::vector <Type>
Parser::getScalarArray (const AccessorPtr & accessor) const
{
	static constexpr size_t components = 1;

	std::vector <Type> array;

	const auto bufferView    = accessor -> bufferView;
	const auto byteOffset    = accessor -> byteOffset + bufferView -> byteOffset;
	const auto componentType = accessor -> componentType;
	const auto componentSize = componentSizes .at (componentType);
	const auto count         = accessor -> count;
	const auto normalized    = accessor -> normalized;
	const auto buffer        = bufferView -> buffer;
	const auto byteStride    = bufferView -> byteStride;
	const auto stride        = std::max <size_t> (components, byteStride / componentSize);
	const auto first         = buffer -> contents .data () + byteOffset;
	const auto last          = first + byteStride * (count - 1) + componentSize * components;
	const auto bufferFirst   = buffer -> contents .data ();
	const auto bufferLast    = buffer -> contents .data () + buffer -> contents .size ();

	if (first < bufferFirst or first >= bufferLast)
		return array;

	if (last < bufferFirst or last >= bufferLast)
		return array;

	switch (componentType)
	{
		case ComponentType::BYTE:
		{
			auto       data = reinterpret_cast <const int8_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::UNSIGNED_BYTE:
		{
			auto       data = reinterpret_cast <const uint8_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::SHORT:
		{
			auto       data = reinterpret_cast <const int16_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::UNSIGNED_SHORT:
		{
			auto       data = reinterpret_cast <const uint16_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::UNSIGNED_INT:
		{
			auto       data = reinterpret_cast <const uint32_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::FLOAT:
		{
			auto       data = reinterpret_cast <const float*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
	}

	if (normalized)
	{
		switch (componentType)
		{
			case ComponentType::BYTE:
			case ComponentType::UNSIGNED_BYTE:
			case ComponentType::SHORT:
			case ComponentType::UNSIGNED_SHORT:
			case ComponentType::UNSIGNED_INT:
			{
				const auto & range    = normalizedRanges .at (componentType);
				const auto   fromLow  = std::get <0> (range);
				const auto   fromHigh = std::get <1> (range);
				const auto   toLow    = std::get <2> (range);
				const auto   toHigh   = std::get <3> (range);

				std::for_each (array .begin (), array .end (), [fromLow, fromHigh, toLow, toHigh] (Type & value)
				{
					value = project <double> (value, fromLow, fromHigh, toLow, toHigh);
				});

				break;
			}
			case ComponentType::FLOAT:
				break;
		}
	}

	return array;
}

template <class Type>
std::vector <Type>
Parser::getVectorArray (const AccessorPtr & accessor) const
{
	static constexpr size_t components = std::tuple_size <Type> ();

	std::vector <Type> array;

	const auto bufferView    = accessor -> bufferView;
	const auto byteOffset    = accessor -> byteOffset + bufferView -> byteOffset;
	const auto componentType = accessor -> componentType;
	const auto componentSize = componentSizes .at (componentType);
	const auto count         = accessor -> count;
	const auto normalized    = accessor -> normalized;
	const auto buffer        = bufferView -> buffer;
	const auto byteStride    = bufferView -> byteStride;
	const auto stride        = std::max <size_t> (components, byteStride / componentSize);
	const auto first         = buffer -> contents .data () + byteOffset;
	const auto last          = first + byteStride * (count - 1) + componentSize * components;
	const auto bufferFirst   = buffer -> contents .data ();
	const auto bufferLast    = buffer -> contents .data () + buffer -> contents .size ();

	if (first < bufferFirst or first >= bufferLast)
		return array;

	if (last < bufferFirst or last >= bufferLast)
		return array;

	switch (componentType)
	{
		case ComponentType::BYTE:
		{
			auto       data = reinterpret_cast <const int8_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t i = 0; i < components; ++ i)
					value [i] = data [i];

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::UNSIGNED_BYTE:
		{
			auto       data = reinterpret_cast <const uint8_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t i = 0; i < components; ++ i)
					value [i] = data [i];

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::SHORT:
		{
			auto       data = reinterpret_cast <const int16_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t i = 0; i < components; ++ i)
					value [i] = data [i];

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::UNSIGNED_SHORT:
		{
			auto       data = reinterpret_cast <const uint16_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t i = 0; i < components; ++ i)
					value [i] = data [i];

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::UNSIGNED_INT:
		{
			auto       data = reinterpret_cast <const uint32_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t i = 0; i < components; ++ i)
					value [i] = data [i];

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::FLOAT:
		{
			auto       data = reinterpret_cast <const float*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t i = 0; i < components; ++ i)
					value [i] = data [i];

				array .emplace_back (value);
			}

			break;
		}
	}

	if (normalized)
	{
		switch (componentType)
		{
			case ComponentType::BYTE:
			case ComponentType::UNSIGNED_BYTE:
			case ComponentType::SHORT:
			case ComponentType::UNSIGNED_SHORT:
			case ComponentType::UNSIGNED_INT:
			{
				const auto & range    = normalizedRanges .at (componentType);
				const auto   fromLow  = std::get <0> (range);
				const auto   fromHigh = std::get <1> (range);
				const auto   toLow    = std::get <2> (range);
				const auto   toHigh   = std::get <3> (range);

				std::for_each (array .begin (), array .end (), [fromLow, fromHigh, toLow, toHigh] (Type & value)
				{
					for (auto & component : value)
						component = project <double> (component, fromLow, fromHigh, toLow, toHigh);
				});

				break;
			}
			case ComponentType::FLOAT:
				break;
		}
	}

	return array;
}

template <class Type>
std::vector <Type>
Parser::getMatrixArray (const AccessorPtr & accessor) const
{
	static constexpr size_t components = Type () .size ();

	std::vector <Type> array;

	const auto bufferView    = accessor -> bufferView;
	const auto byteOffset    = accessor -> byteOffset + bufferView -> byteOffset;
	const auto componentType = accessor -> componentType;
	const auto componentSize = componentSizes .at (componentType);
	const auto count         = accessor -> count;
	const auto normalized    = accessor -> normalized;
	const auto buffer        = bufferView -> buffer;
	const auto byteStride    = bufferView -> byteStride;
	const auto stride        = std::max <size_t> (components, byteStride / componentSize);
	const auto first         = buffer -> contents .data () + byteOffset;
	const auto last          = first + byteStride * (count - 1) + componentSize * components;
	const auto bufferFirst   = buffer -> contents .data ();
	const auto bufferLast    = buffer -> contents .data () + buffer -> contents .size ();

	if (first < bufferFirst or first >= bufferLast)
		return array;

	if (last < bufferFirst or last >= bufferLast)
		return array;

	switch (componentType)
	{
		case ComponentType::BYTE:
		{
			auto       data = reinterpret_cast <const int8_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t r = 0, i = 0; r < value .rows (); ++ r)
				{
					for (size_t c = 0; c < value .columns (); ++ c, ++ i)
					{
						value [r] [c] = data [i];
					}
				}

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::UNSIGNED_BYTE:
		{
			auto       data = reinterpret_cast <const uint8_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t r = 0, i = 0; r < value .rows (); ++ r)
				{
					for (size_t c = 0; c < value .columns (); ++ c, ++ i)
					{
						value [r] [c] = data [i];
					}
				}

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::SHORT:
		{
			auto       data = reinterpret_cast <const int16_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t r = 0, i = 0; r < value .rows (); ++ r)
				{
					for (size_t c = 0; c < value .columns (); ++ c, ++ i)
					{
						value [r] [c] = data [i];
					}
				}

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::UNSIGNED_SHORT:
		{
			auto       data = reinterpret_cast <const uint16_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t r = 0, i = 0; r < value .rows (); ++ r)
				{
					for (size_t c = 0; c < value .columns (); ++ c, ++ i)
					{
						value [r] [c] = data [i];
					}
				}

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::UNSIGNED_INT:
		{
			auto       data = reinterpret_cast <const uint32_t*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t r = 0, i = 0; r < value .rows (); ++ r)
				{
					for (size_t c = 0; c < value .columns (); ++ c, ++ i)
					{
						value [r] [c] = data [i];
					}
				}

				array .emplace_back (value);
			}

			break;
		}
		case ComponentType::FLOAT:
		{
			auto       data = reinterpret_cast <const float*> (first);
			const auto last = data + stride * count;

			for (; data not_eq last; data += stride)
			{
				Type value;

				for (size_t r = 0, i = 0; r < value .rows (); ++ r)
				{
					for (size_t c = 0; c < value .columns (); ++ c, ++ i)
					{
						value [r] [c] = data [i];
					}
				}

				array .emplace_back (value);
			}

			break;
		}
	}

	if (normalized)
	{
		switch (componentType)
		{
			case ComponentType::BYTE:
			case ComponentType::UNSIGNED_BYTE:
			case ComponentType::SHORT:
			case ComponentType::UNSIGNED_SHORT:
			case ComponentType::UNSIGNED_INT:
			{
				const auto & range    = normalizedRanges .at (componentType);
				const auto   fromLow  = std::get <0> (range);
				const auto   fromHigh = std::get <1> (range);
				const auto   toLow    = std::get <2> (range);
				const auto   toHigh   = std::get <3> (range);

				std::for_each (array .begin (), array .end (), [fromLow, fromHigh, toLow, toHigh] (Type & value)
				{
					for (auto & vector : value)
					{
						for (auto & component : vector)
							component = project <double> (component, fromLow, fromHigh, toLow, toHigh);
					}
				});

				break;
			}
			case ComponentType::FLOAT:
				break;
		}
	}

	return array;
}

} // glTF
} // X3D
} // titania

#endif
