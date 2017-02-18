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
#include "../../Basic/X3DConstants.h"
#include "../../Fields.h"
#include "../../Rendering/OpenGL.h"

namespace titania {
namespace X3D {

class MetadataBoolean;
class MetadataDouble;
class MetadataFloat;
class MetadataInteger;
class MetadataString;
class MetadataSet;

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

	template <class Type>
	void
	setMetaData (const std::string & key, const Type & value)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <DISPOSED>)
	{
		static_assert (std::is_same <Type, SFBool::internal_type>::value or
		               std::is_same <Type, SFColor::internal_type>::value or
		               std::is_same <Type, SFColorRGBA::internal_type>::value or
		               std::is_same <Type, SFDouble::internal_type>::value or
		               std::is_same <Type, SFFloat::internal_type>::value or
		               std::is_same <Type, SFInt32::internal_type>::value or
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
		               std::is_same <Type, MFDouble>::value or
		               std::is_same <Type, MFFloat>::value or
		               std::is_same <Type, MFInt32>::value or
							std::is_same <Type, MFNode>::value or
		               std::is_same <Type, MFString>::value,
		               "X3DNode::setMetaData: Type is not supported!");

		setMetaData (key, value .getValue ());
	}

	template <class Type>
	Type
	getMetaData (const std::string & key, const Type & defaultValue = Type ())
	throw (Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <DISPOSED>)
	{
		static_assert (std::is_same <Type, SFBool::internal_type>::value or
		               std::is_same <Type, SFColorRGBA::internal_type>::value or
		               std::is_same <Type, SFDouble::internal_type>::value or
		               std::is_same <Type, SFFloat::internal_type>::value or
		               std::is_same <Type, SFInt32::internal_type>::value or
		               std::is_same <Type, SFRotation::internal_type>::value or
		               std::is_same <Type, SFString::internal_type>::value or
		               std::is_same <Type, SFVec3d::internal_type>::value or
		               std::is_same <Type, SFVec3f::internal_type>::value or
		               std::is_same <Type, SFBool>::value or
		               std::is_same <Type, SFColorRGBA>::value or
		               std::is_same <Type, SFDouble>::value or
		               std::is_same <Type, SFFloat>::value or
		               std::is_same <Type, SFInt32>::value or
		               std::is_same <Type, SFNode>::value or
		               std::is_same <Type, SFRotation>::value or
		               std::is_same <Type, SFString>::value or
		               std::is_same <Type, SFVec3d>::value or
		               std::is_same <Type, SFVec3f>::value or
		               std::is_same <Type, MFBool>::value or
		               std::is_same <Type, MFDouble>::value or
		               std::is_same <Type, MFFloat>::value or
		               std::is_same <Type, MFInt32>::value or
							std::is_same <Type, MFNode>::value or
		               std::is_same <Type, MFString>::value,
		               "X3DNode::getMetaData: Type is not supported!");

		return Type (getMetaData <typename Type::internal_type> (key, defaultValue));
	}

	void
	removeMetaData (const std::string & key = "")
	throw (Error <DISPOSED>);

	virtual
	void
	fromMetaData (const X3DPtr <MetadataSet> &);

	virtual
	X3DPtr <MetadataSet>
	toMetaData () const;

	///  @name Member access

	std::vector <X3DLayerNode*>
	getLayers () const;


protected:

	///  @name Construction

	X3DNode ();


private:

private:

	///  @name Metadata handling

	MetadataSet*
	getMetadataSet (const std::vector <std::string> & name, const bool throw_) const
	throw (Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <DISPOSED>);

	virtual
	void
	fieldToMetaData (const X3DPtr <MetadataSet> &, const X3DFieldDefinition* const) const;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const metadata;
	};

	Fields fields;

};

// set

template <>
void
X3DNode::setMetaData <SFBool::internal_type> (const std::string & key, const SFBool::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFColor::internal_type> (const std::string & key, const SFColor::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFColorRGBA::internal_type> (const std::string & key, const SFColorRGBA::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFDouble::internal_type> (const std::string & key, const SFDouble::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFFloat::internal_type> (const std::string & key, const SFFloat::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFInt32::internal_type> (const std::string & key, const SFInt32::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFNode> (const std::string & key, const SFNode & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFRotation::internal_type> (const std::string & key, const SFRotation::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFString::internal_type> (const std::string & key, const SFString::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFVec2d::internal_type> (const std::string & key, const SFVec2d::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFVec2f::internal_type> (const std::string & key, const SFVec2f::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFVec3d::internal_type> (const std::string & key, const SFVec3d::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFVec3f::internal_type> (const std::string & key, const SFVec3f::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFVec4d::internal_type> (const std::string & key, const SFVec4d::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <SFVec4f::internal_type> (const std::string & key, const SFVec4f::internal_type & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFBool> (const std::string & key, const MFBool & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFDouble> (const std::string & key, const MFDouble & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFFloat> (const std::string & key, const MFFloat & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFNode> (const std::string & key, const MFNode & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFInt32> (const std::string & key, const MFInt32 & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFString> (const std::string & key, const MFString & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

// get

template <>
SFBool::internal_type
X3DNode::getMetaData <SFBool::internal_type> (const std::string & key, const SFBool::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFColorRGBA::internal_type
X3DNode::getMetaData <SFColorRGBA::internal_type> (const std::string & key, const SFColorRGBA::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFDouble::internal_type
X3DNode::getMetaData <SFDouble::internal_type> (const std::string & key, const SFDouble::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFFloat::internal_type
X3DNode::getMetaData <SFFloat::internal_type> (const std::string & key, const SFFloat::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFInt32::internal_type
X3DNode::getMetaData <SFInt32::internal_type> (const std::string & key, const SFInt32::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFNode
X3DNode::getMetaData <SFNode> (const std::string & key, const SFNode & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFRotation::internal_type
X3DNode::getMetaData <SFRotation::internal_type> (const std::string & key, const SFRotation::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFString::internal_type
X3DNode::getMetaData <SFString::internal_type> (const std::string & key, const SFString::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFVec3d::internal_type
X3DNode::getMetaData <SFVec3d::internal_type> (const std::string & key, const SFVec3d::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
SFVec3f::internal_type
X3DNode::getMetaData <SFVec3f::internal_type> (const std::string & key, const SFVec3f::internal_type & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
MFBool
X3DNode::getMetaData <MFBool> (const std::string & key, const MFBool & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
MFDouble
X3DNode::getMetaData <MFDouble> (const std::string & key, const MFDouble & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
MFFloat
X3DNode::getMetaData <MFFloat> (const std::string & key, const MFFloat & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
MFInt32
X3DNode::getMetaData <MFInt32> (const std::string & key, const MFInt32 & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
MFNode
X3DNode::getMetaData <MFNode> (const std::string & key, const MFNode & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

template <>
MFString
X3DNode::getMetaData <MFString> (const std::string & key, const MFString & defaultValue)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>);

} // X3D
} // titania

#endif
