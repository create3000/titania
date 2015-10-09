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

#ifndef __TITANIA_X3D_COMPONENTS_CORE_X3DNODE_H__
#define __TITANIA_X3D_COMPONENTS_CORE_X3DNODE_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Bits/X3DConstants.h"
#include "../../Fields.h"

#include "../../Rendering/OpenGL.h"
#include "../../Types/MatrixStack.h"

namespace titania {
namespace X3D {

class X3DScene;
class X3DLayerNode;
class NavigationInfo;
class X3DViewpointNode;
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
	setMetaData (const std::string & name, const Type & value)
	throw (Error <INVALID_NAME>,
          Error <NOT_SUPPORTED>,
	       Error <DISPOSED>);

	template <class Type>
	Type &
	getMetaData (const std::string &, const bool = false)
	throw (Error <INVALID_NAME>,
          Error <NOT_SUPPORTED>,
	       Error <DISPOSED>);

	template <class Type>
	const Type &
	getMetaData (const std::string &) const
	throw (Error <INVALID_NAME>,
          Error <NOT_SUPPORTED>,
	       Error <DISPOSED>);

	void
	removeMetaData (const std::string & = "")
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

	Matrix4fStack &
	getModelViewMatrix ()
	{ return modelViewMatrix; }

	const Matrix4fStack &
	getModelViewMatrix () const
	{ return modelViewMatrix; }


protected:

	///  @name Construction

	X3DNode ();

	///  @name Member access

	X3DLayerNode*
	getCurrentLayer () const;

	NavigationInfo*
	getCurrentNavigationInfo () const;

	X3DViewpointNode*
	getCurrentViewpoint () const;

	const Matrix4f &
	getCameraSpaceMatrix () const;

	const Matrix4f &
	getInverseCameraSpaceMatrix () const;

	Matrix4f
	getModelViewMatrix (const TraverseType) const;


private:

	///  @name Member access

	MetadataSet*
	getMetadataSet (const std::deque <std::string> &, const bool = false) const
	throw (Error <INVALID_NAME>,
	       Error <DISPOSED>);

	void
	fieldToMetaData (const X3DPtr <MetadataSet> &, const X3DFieldDefinition* const) const;

	///  @name Static members

	static Matrix4fStack modelViewMatrix;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const metadata;
	};

	Fields fields;

};

// set

template <class Type>
void
X3DNode::setMetaData (const std::string & key, const Type & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{ }

template <>
void
X3DNode::setMetaData <bool> (const std::string &, const bool &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Color3f> (const std::string &, const Color3f &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Color4f> (const std::string &, const Color4f &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <double> (const std::string &, const double &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <float> (const std::string &, const float &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <int32_t> (const std::string &, const int32_t &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Rotation4f> (const std::string &, const Rotation4f &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <std::string> (const std::string &, const std::string &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Vector2d> (const std::string &, const Vector2d &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Vector2f> (const std::string &, const Vector2f &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Vector3d> (const std::string &, const Vector3d &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Vector3f> (const std::string &, const Vector3f &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Vector4d> (const std::string &, const Vector4d &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <Vector4f> (const std::string &, const Vector4f &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFBool> (const std::string &, const MFBool &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFDouble> (const std::string &, const MFDouble &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFFloat> (const std::string &, const MFFloat &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFInt32> (const std::string &, const MFInt32 &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
void
X3DNode::setMetaData <MFString> (const std::string &, const MFString &)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

// get

template <class Type>
Type &
X3DNode::getMetaData (const std::string & key, const bool create)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	throw Error <NOT_SUPPORTED> ("X3DNode::getMetaData: type is not supported.");
}

template <class Type>
const Type &
X3DNode::getMetaData (const std::string & key) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	throw Error <NOT_SUPPORTED> ("X3DNode::getMetaData: type is not supported.");
}

template <>
MFBool &
X3DNode::getMetaData <MFBool> (const std::string &, const bool)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
const MFBool &
X3DNode::getMetaData <MFBool> (const std::string &) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
MFDouble &
X3DNode::getMetaData <MFDouble> (const std::string &, const bool )
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
const MFDouble &
X3DNode::getMetaData <MFDouble> (const std::string &) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
MFFloat &
X3DNode::getMetaData <MFFloat> (const std::string &, const bool)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
const MFFloat &
X3DNode::getMetaData <MFFloat> (const std::string &) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
MFInt32 &
X3DNode::getMetaData <MFInt32> (const std::string &, const bool)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
const MFInt32 &
X3DNode::getMetaData <MFInt32> (const std::string &) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
MFString &
X3DNode::getMetaData <MFString> (const std::string &, const bool)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

template <>
const MFString &
X3DNode::getMetaData <MFString> (const std::string &) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>);

} // X3D
} // titania

#endif
