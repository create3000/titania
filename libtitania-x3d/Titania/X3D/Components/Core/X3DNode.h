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

#ifndef __TITANIA_X3D_COMPONENTS_CORE_X3DNODE_H__
#define __TITANIA_X3D_COMPONENTS_CORE_X3DNODE_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Bits/X3DConstants.h"
#include "../../Fields.h"
#include "../../Rendering/OpenGL.h"

namespace titania {
namespace X3D {

class X3DNode :
	virtual public X3DBaseNode
{
public:

	///  @name Fields

	virtual
	SFNode &
	metadata ()
	{ return *fields .metadata; }

	virtual
	const SFNode &
	metadata () const
	{ return *fields .metadata; }

	///  @name Metadata handling

	///  throws  Error <NOT_SUPPORTED>, Error <INVALID_NAME>, Error <DISPOSED>
	X3DPtr <MetadataSet>
	createMetadataSet (const std::string & key);

	///  throws  Error <INVALID_NODE>, Error <INVALID_NAME>, Error <DISPOSED>
	X3DPtr <MetadataSet>
	getMetadataSet (const std::string & key) const;

	///  throws  Error <DISPOSED>
	template <class Type>
	void
	setMetaData (const std::string & key, const Type & value)
	{
		static_assert (std::is_same <Type, std::string>::value or
		               std::is_same <Type, SFBool::internal_type>::value or
		               std::is_same <Type, SFColor::internal_type>::value or
		               std::is_same <Type, SFColorRGBA::internal_type>::value or
		               std::is_same <Type, SFDouble::internal_type>::value or
		               std::is_same <Type, SFFloat::internal_type>::value or
		               std::is_same <Type, SFImage>::value or
		               std::is_same <Type, SFInt32::internal_type>::value or
							std::is_same <Type, SFMatrix3d::internal_type>::value or
							std::is_same <Type, SFMatrix3f::internal_type>::value or
							std::is_same <Type, SFMatrix4d::internal_type>::value or
							std::is_same <Type, SFMatrix4f::internal_type>::value or
		               std::is_same <Type, SFRotation::internal_type>::value  or
		               std::is_same <Type, SFString::internal_type>::value or
		               std::is_same <Type, SFVec2d::internal_type>::value or
		               std::is_same <Type, SFVec2f::internal_type>::value or
		               std::is_same <Type, SFVec3d::internal_type>::value or
		               std::is_same <Type, SFVec3f::internal_type>::value or
		               std::is_same <Type, SFVec4d::internal_type>::value or
		               std::is_same <Type, SFVec4f::internal_type>::value or
		               std::is_same <Type, SFBool>::value or
		               std::is_same <Type, SFColor>::value or
		               std::is_same <Type, SFColorRGBA>::value or
		               std::is_same <Type, SFDouble>::value or
		               std::is_same <Type, SFFloat>::value or
		               std::is_same <Type, SFInt32>::value or
							std::is_same <Type, SFMatrix3d>::value or
							std::is_same <Type, SFMatrix3f>::value or
							std::is_same <Type, SFMatrix4d>::value or
							std::is_same <Type, SFMatrix4f>::value or
							std::is_same <Type, SFNode>::value or
		               std::is_same <Type, SFRotation>::value  or
		               std::is_same <Type, SFString>::value or
		               std::is_same <Type, SFVec2d>::value or
		               std::is_same <Type, SFVec2f>::value or
		               std::is_same <Type, SFVec3d>::value or
		               std::is_same <Type, SFVec3f>::value or
		               std::is_same <Type, SFVec4d>::value or
		               std::is_same <Type, SFVec4f>::value or
		               std::is_same <Type, MFBool>::value or
		               std::is_same <Type, MFColor>::value or
		               std::is_same <Type, MFColorRGBA>::value or
		               std::is_same <Type, MFDouble>::value or
		               std::is_same <Type, MFFloat>::value or
		               std::is_same <Type, MFImage>::value or
		               std::is_same <Type, MFInt32>::value or
							std::is_same <Type, MFMatrix3d>::value or
							std::is_same <Type, MFMatrix3f>::value or
							std::is_same <Type, MFMatrix4d>::value or
							std::is_same <Type, MFMatrix4f>::value or
							std::is_same <Type, MFNode>::value or
		               std::is_same <Type, MFRotation>::value or
		               std::is_same <Type, MFString>::value or
		               std::is_same <Type, MFVec2d>::value or
		               std::is_same <Type, MFVec2f>::value or
		               std::is_same <Type, MFVec3d>::value or
		               std::is_same <Type, MFVec3f>::value or
		               std::is_same <Type, MFVec4d>::value or
		               std::is_same <Type, MFVec4f>::value,
		               "X3DNode::setMetaData: Type is not supported!");

		setMetaData (key, value .getValue ());
	}

	///  throws  Error <DISPOSED>
	template <class Type>
	Type
	getMetaData (const std::string & key, const Type & defaultValue = Type ())
	{
		static_assert (std::is_same <Type, std::string>::value or
		               std::is_same <Type, SFBool::internal_type>::value or
		               std::is_same <Type, SFColor::internal_type>::value or
		               std::is_same <Type, SFColorRGBA::internal_type>::value or
		               std::is_same <Type, SFDouble::internal_type>::value or
		               std::is_same <Type, SFFloat::internal_type>::value or
		               std::is_same <Type, SFImage>::value or
		               std::is_same <Type, SFInt32::internal_type>::value or
	                  std::is_same <Type, SFMatrix3d::internal_type>::value or
	                  std::is_same <Type, SFMatrix3f::internal_type>::value or
	                  std::is_same <Type, SFMatrix4d::internal_type>::value or
							std::is_same <Type, SFMatrix4f::internal_type>::value or
		               std::is_same <Type, SFRotation::internal_type>::value  or
		               std::is_same <Type, SFString::internal_type>::value or
		               std::is_same <Type, SFVec2d::internal_type>::value or
		               std::is_same <Type, SFVec2f::internal_type>::value or
		               std::is_same <Type, SFVec3d::internal_type>::value or
		               std::is_same <Type, SFVec3f::internal_type>::value or
		               std::is_same <Type, SFVec4d::internal_type>::value or
		               std::is_same <Type, SFVec4f::internal_type>::value or
		               std::is_same <Type, SFBool>::value or
		               std::is_same <Type, SFColor>::value or
		               std::is_same <Type, SFColorRGBA>::value or
		               std::is_same <Type, SFDouble>::value or
		               std::is_same <Type, SFFloat>::value or
		               std::is_same <Type, SFInt32>::value or
		               std::is_same <Type, SFMatrix3d>::value or
		               std::is_same <Type, SFMatrix3f>::value or
		               std::is_same <Type, SFMatrix4d>::value or
		               std::is_same <Type, SFMatrix4f>::value or
		               std::is_same <Type, SFNode>::value or
		               std::is_same <Type, SFRotation>::value  or
		               std::is_same <Type, SFString>::value or
		               std::is_same <Type, SFVec2d>::value or
		               std::is_same <Type, SFVec2f>::value or
		               std::is_same <Type, SFVec3d>::value or
		               std::is_same <Type, SFVec3f>::value or
		               std::is_same <Type, SFVec4d>::value or
		               std::is_same <Type, SFVec4f>::value or
		               std::is_same <Type, MFBool>::value or
		               std::is_same <Type, MFColor>::value or
		               std::is_same <Type, MFColorRGBA>::value or
		               std::is_same <Type, MFDouble>::value or
		               std::is_same <Type, MFFloat>::value or
		               std::is_same <Type, MFImage>::value or
		               std::is_same <Type, MFInt32>::value or
		               std::is_same <Type, MFMatrix3d>::value or
		               std::is_same <Type, MFMatrix3f>::value or
		               std::is_same <Type, MFMatrix4d>::value or
		               std::is_same <Type, MFMatrix4f>::value or
		               std::is_same <Type, MFNode>::value or
		               std::is_same <Type, MFRotation>::value or
		               std::is_same <Type, MFString>::value or
		               std::is_same <Type, MFVec2d>::value or
		               std::is_same <Type, MFVec2f>::value or
		               std::is_same <Type, MFVec3d>::value or
		               std::is_same <Type, MFVec3f>::value or
		               std::is_same <Type, MFVec4d>::value or
		               std::is_same <Type, MFVec4f>::value,
		               "X3DNode::getMetaData: Type is not supported!");

		return Type (getMetaData <typename Type::internal_type> (key, defaultValue));
	}

	///  throws  Error <DISPOSED>
	void
	removeMetaData (const std::string & key = "");

	virtual
	void
	fromMetaData (const X3DPtr <MetadataSet> & metadataSetNode);

	virtual
	void
	toMetaData (const X3DPtr <MetadataSet> & metadataSetNode) const;

	///  @name Member access

	std::vector <X3DLayerNode*>
	getLayers () const;


protected:

	///  @name Construction

	X3DNode ();


private:

	class MetadataHandling;

	///  @name Metadata handling

	///  throws  Error <INVALID_NODE>, Error <INVALID_NAME>, Error <DISPOSED>
	MetadataSet*
	getMetadataSet (const std::vector <std::string> & name, const bool throw_) const;

	void
	fromMetaData (const X3DPtr <MetadataSet> & metadataSetNode, X3DFieldDefinition* const field) const;

	void
	toMetaData (const X3DPtr <MetadataSet> & metadataSetNode, const X3DFieldDefinition* const field) const;

	///  @name Static members

	static const std::string SEPARATOR;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const metadata;
	};

	Fields fields;

};

// set

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFBool::internal_type> (const std::string & key, const SFBool::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFColor::internal_type> (const std::string & key, const SFColor::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFColorRGBA::internal_type> (const std::string & key, const SFColorRGBA::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFDouble::internal_type> (const std::string & key, const SFDouble::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFFloat::internal_type> (const std::string & key, const SFFloat::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFImage> (const std::string & key, const SFImage & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFInt32::internal_type> (const std::string & key, const SFInt32::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFMatrix3d::internal_type> (const std::string & key, const SFMatrix3d::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFMatrix3f::internal_type> (const std::string & key, const SFMatrix3f::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFMatrix4d::internal_type> (const std::string & key, const SFMatrix4d::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFMatrix4f::internal_type> (const std::string & key, const SFMatrix4f::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFNode> (const std::string & key, const SFNode & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFRotation::internal_type> (const std::string & key, const SFRotation::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFString::internal_type> (const std::string & key, const SFString::internal_type & value);

///  throws  Error <DISPOSED>
template <>
inline
void
X3DNode::setMetaData <std::string> (const std::string & key, const std::string & value)
{ setMetaData <SFString::internal_type> (key, value); }

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFVec2d::internal_type> (const std::string & key, const SFVec2d::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFVec2f::internal_type> (const std::string & key, const SFVec2f::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFVec3d::internal_type> (const std::string & key, const SFVec3d::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFVec3f::internal_type> (const std::string & key, const SFVec3f::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFVec4d::internal_type> (const std::string & key, const SFVec4d::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <SFVec4f::internal_type> (const std::string & key, const SFVec4f::internal_type & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFBool> (const std::string & key, const MFBool & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFColor> (const std::string & key, const MFColor & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFColorRGBA> (const std::string & key, const MFColorRGBA & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFDouble> (const std::string & key, const MFDouble & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFFloat> (const std::string & key, const MFFloat & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFImage> (const std::string & key, const MFImage & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFInt32> (const std::string & key, const MFInt32 & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFMatrix3d> (const std::string & key, const MFMatrix3d & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFMatrix3f> (const std::string & key, const MFMatrix3f & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFMatrix4d> (const std::string & key, const MFMatrix4d & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFMatrix4f> (const std::string & key, const MFMatrix4f & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFNode> (const std::string & key, const MFNode & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFRotation> (const std::string & key, const MFRotation & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFString> (const std::string & key, const MFString & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFVec2d> (const std::string & key, const MFVec2d & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFVec2f> (const std::string & key, const MFVec2f & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFVec3d> (const std::string & key, const MFVec3d & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFVec3f> (const std::string & key, const MFVec3f & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFVec4d> (const std::string & key, const MFVec4d & value);

///  throws  Error <DISPOSED>
template <>
void
X3DNode::setMetaData <MFVec4f> (const std::string & key, const MFVec4f & value);

// get

///  throws  Error <DISPOSED>
template <>
SFBool::internal_type
X3DNode::getMetaData <SFBool::internal_type> (const std::string & key, const SFBool::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFColor::internal_type
X3DNode::getMetaData <SFColor::internal_type> (const std::string & key, const SFColor::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFColorRGBA::internal_type
X3DNode::getMetaData <SFColorRGBA::internal_type> (const std::string & key, const SFColorRGBA::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFDouble::internal_type
X3DNode::getMetaData <SFDouble::internal_type> (const std::string & key, const SFDouble::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFFloat::internal_type
X3DNode::getMetaData <SFFloat::internal_type> (const std::string & key, const SFFloat::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFImage
X3DNode::getMetaData <SFImage> (const std::string & key, const SFImage & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFInt32::internal_type
X3DNode::getMetaData <SFInt32::internal_type> (const std::string & key, const SFInt32::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFMatrix3d::internal_type
X3DNode::getMetaData <SFMatrix3d::internal_type> (const std::string & key, const SFMatrix3d::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFMatrix3f::internal_type
X3DNode::getMetaData <SFMatrix3f::internal_type> (const std::string & key, const SFMatrix3f::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFMatrix4d::internal_type
X3DNode::getMetaData <SFMatrix4d::internal_type> (const std::string & key, const SFMatrix4d::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFMatrix4f::internal_type
X3DNode::getMetaData <SFMatrix4f::internal_type> (const std::string & key, const SFMatrix4f::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFNode
X3DNode::getMetaData <SFNode> (const std::string & key, const SFNode & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFRotation::internal_type
X3DNode::getMetaData <SFRotation::internal_type> (const std::string & key, const SFRotation::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFString::internal_type
X3DNode::getMetaData <SFString::internal_type> (const std::string & key, const SFString::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
inline
std::string
X3DNode::getMetaData <std::string> (const std::string & key, const std::string & defaultValue)
{ return getMetaData <SFString::internal_type> (key, defaultValue); }

///  throws  Error <DISPOSED>
template <>
SFVec2d::internal_type
X3DNode::getMetaData <SFVec2d::internal_type> (const std::string & key, const SFVec2d::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFVec2f::internal_type
X3DNode::getMetaData <SFVec2f::internal_type> (const std::string & key, const SFVec2f::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFVec3d::internal_type
X3DNode::getMetaData <SFVec3d::internal_type> (const std::string & key, const SFVec3d::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFVec3f::internal_type
X3DNode::getMetaData <SFVec3f::internal_type> (const std::string & key, const SFVec3f::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFVec4d::internal_type
X3DNode::getMetaData <SFVec4d::internal_type> (const std::string & key, const SFVec4d::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
SFVec4f::internal_type
X3DNode::getMetaData <SFVec4f::internal_type> (const std::string & key, const SFVec4f::internal_type & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFBool
X3DNode::getMetaData <MFBool> (const std::string & key, const MFBool & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFColor
X3DNode::getMetaData <MFColor> (const std::string & key, const MFColor & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFColorRGBA
X3DNode::getMetaData <MFColorRGBA> (const std::string & key, const MFColorRGBA & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFDouble
X3DNode::getMetaData <MFDouble> (const std::string & key, const MFDouble & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFFloat
X3DNode::getMetaData <MFFloat> (const std::string & key, const MFFloat & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFImage
X3DNode::getMetaData <MFImage> (const std::string & key, const MFImage & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFInt32
X3DNode::getMetaData <MFInt32> (const std::string & key, const MFInt32 & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFMatrix3d
X3DNode::getMetaData <MFMatrix3d> (const std::string & key, const MFMatrix3d & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFMatrix3f
X3DNode::getMetaData <MFMatrix3f> (const std::string & key, const MFMatrix3f & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFMatrix4d
X3DNode::getMetaData <MFMatrix4d> (const std::string & key, const MFMatrix4d & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFMatrix4f
X3DNode::getMetaData <MFMatrix4f> (const std::string & key, const MFMatrix4f & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFNode
X3DNode::getMetaData <MFNode> (const std::string & key, const MFNode & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFRotation
X3DNode::getMetaData <MFRotation> (const std::string & key, const MFRotation & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFString
X3DNode::getMetaData <MFString> (const std::string & key, const MFString & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFVec2d
X3DNode::getMetaData <MFVec2d> (const std::string & key, const MFVec2d & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFVec2f
X3DNode::getMetaData <MFVec2f> (const std::string & key, const MFVec2f & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFVec3d
X3DNode::getMetaData <MFVec3d> (const std::string & key, const MFVec3d & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFVec3f
X3DNode::getMetaData <MFVec3f> (const std::string & key, const MFVec3f & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFVec4d
X3DNode::getMetaData <MFVec4d> (const std::string & key, const MFVec4d & defaultValue);

///  throws  Error <DISPOSED>
template <>
MFVec4f
X3DNode::getMetaData <MFVec4f> (const std::string & key, const MFVec4f & defaultValue);

} // X3D
} // titania

#endif
