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

#include "X3DNode.h"

#include "../../Bits/Cast.h"
#include "../../Bits/Traverse.h"
#include "../../Browser/X3DBrowser.h"
#include "../Core/MetadataSet.h"
#include "../Layering/X3DLayerNode.h"

namespace titania {
namespace X3D {

Matrix4fStack X3DNode::modelViewMatrix;

X3DNode::Fields::Fields () :
	metadata (new SFNode ())
{ }

X3DNode::X3DNode () :
	X3DBaseNode (),
	     fields ()
{
	addType (X3DConstants::X3DNode);
}

template <>
void
X3DNode::setMetaData <bool> (const std::string & key, const bool & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData(key, MFBool ({ value }));
}

template <>
void
X3DNode::setMetaData <Color3f> (const std::string & key, const Color3f & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .r (), value .g (), value .b () }));
}

template <>
void
X3DNode::setMetaData <Color4f> (const std::string & key, const Color4f & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .r (), value .g (), value .b (), value .a () }));
}

template <>
void
X3DNode::setMetaData <double> (const std::string & key, const double & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value }));
}

template <>
void
X3DNode::setMetaData <float> (const std::string & key, const float & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value }));
}

template <>
void
X3DNode::setMetaData <int32_t> (const std::string & key, const int32_t & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFInt32 ({ value }));
}

template <>
void
X3DNode::setMetaData <Rotation4f> (const std::string & key, const Rotation4f & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	float x, y, z, angle;
	
	value .get (x, y, z, angle);

	setMetaData (key, MFFloat ({ x, y, z, angle }));
}

template <>
void
X3DNode::setMetaData <std::string> (const std::string & key, const std::string & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFString ({ value }));
}

template <>
void
X3DNode::setMetaData <Vector2d> (const std::string & key, const Vector2d & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value .x (), value .y () }));
}

template <>
void
X3DNode::setMetaData <Vector2f> (const std::string & key, const Vector2f & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .x (), value .y () }));
}

template <>
void
X3DNode::setMetaData <Vector3d> (const std::string & key, const Vector3d & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value .x (), value .y (), value .z () }));
}

template <>
void
X3DNode::setMetaData <Vector3f> (const std::string & key, const Vector3f & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .x (), value .y (), value .z () }));
}

template <>
void
X3DNode::setMetaData <Vector4d> (const std::string & key, const Vector4d & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value .x (), value .y (), value .z (), value .w () }));
}

template <>
void
X3DNode::setMetaData <Vector4f> (const std::string & key, const Vector4f & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .x (), value .y (), value .z (), value .w () }));
}

template <>
void
X3DNode::setMetaData <MFBool> (const std::string & key, const MFBool & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, true);

	set -> setMetaData (names .back (), value);
}

template <>
void
X3DNode::setMetaData <MFDouble> (const std::string & key, const MFDouble & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, true);

	set -> setMetaData (names .back (), value);
}

template <>
void
X3DNode::setMetaData <MFFloat> (const std::string & key, const MFFloat & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, true);

	set -> setMetaData (names .back (), value);
}

template <>
void
X3DNode::setMetaData <MFInt32> (const std::string & key, const MFInt32 & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, true);

	set -> setMetaData (names .back (), value);
}

template <>
void
X3DNode::setMetaData <MFString> (const std::string & key, const MFString & value)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, true);

	set -> setMetaData (names .back (), value);
}

template <>
const MFBool &
X3DNode::getMetaData <MFBool> (const std::string & key) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names);

	return set -> getMetaData <MFBool> (names .back ());
}

template <>
const MFDouble &
X3DNode::getMetaData <MFDouble> (const std::string & key) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names);

	return set -> getMetaData <MFDouble> (names .back ());
}

template <>
const MFFloat &
X3DNode::getMetaData <MFFloat> (const std::string & key) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names);

	return set -> getMetaData <MFFloat> (names .back ());
}

template <>
const MFInt32 &
X3DNode::getMetaData <MFInt32> (const std::string & key) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names);

	return set -> getMetaData <MFInt32> (names .back ());
}

template <>
const MFString &
X3DNode::getMetaData <MFString> (const std::string & key) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names);

	return set -> getMetaData <MFString> (names .back ());
}

//try
//{
//	const auto & t = worldInfo -> getMetaData <MFFloat> ("/X3D/Grid/translation");
//
//	getGrid () -> translation () .setValue (t .at (0), t .at (1), t .at (2));
//}
//catch (...)
//{
//	getGrid () -> translation () = X3D::Vector3f ();
//}
//
//try
//{
//	getGrid () -> transparency () = worldInfo -> getMetaData <MFFloat> ("/X3D/Grid/transparency") .at (0);
//}
//catch (...)
//{
//	getGrid () -> transparency () = 0.75;
//}
//
//worldInfo -> setMetaData <X3D::Vector3f> ("/X3D/Grid/translation", getGrid () -> translation ());

MetadataSet*
X3DNode::getMetadataSet (const std::deque <std::string> & names, const bool create) const
throw (Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	if (names .size () < 3)
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	if (not names .front () .empty ())
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	if (names [1] .empty ())
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	auto set = x3d_cast <MetadataSet*> (metadata ());

	if (not set or set -> name () not_eq names [1])
	{
		if (not create)
			throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

		set            = new MetadataSet (getExecutionContext ());
		set -> name () = names [1];
		set -> setup ();
		const_cast <X3DNode*> (this) -> metadata () = set;
	}

	for (const auto & name : std::make_pair (names .begin () + 2, names .end () - 1))
		set = set -> getSet (name, create);

	return set;
}

void
X3DNode::removeMetaData (const std::string & key)
throw (Error <DISPOSED>)
{
	auto names = basic::split (key, "/");

	if (not names .empty () and not names .front () .empty ())
		throw Error <INVALID_NAME> ("X3DNode::removeMetaData: Invalid key.");

	switch (names .size ())
	{
		case 0:
		{
			metadata () = nullptr;
			return;
		}
		case 1:
			return;
		case 2:
		{
			const auto set = x3d_cast <MetadataSet*> (metadata ());

			if (set -> name () == names [1])
				metadata () = nullptr;

			return;
		}
		default:
		{
			std::vector <MetadataSet*> sets;

			const auto set = x3d_cast <MetadataSet*> (metadata ());

			if (not set or set -> name () not_eq names [1])
				return;

			sets .emplace_back (set);

			try
			{
				for (const auto & name : std::make_pair (names .begin () + 2, names .end () - 1))
					sets .emplace_back (sets .back () -> getSet (name));
			}
			catch (const Error <INVALID_NAME> &)
			{
				return;
			}

			sets .back () -> removeMetaData (names .back ());

			while (sets .back () -> value () .empty ())
			{
				names .pop_back ();
				sets .pop_back ();

				if (sets .empty ())
				{
					metadata () = nullptr;
					break;
				}

				sets .back () -> removeMetaData (names .back ());
			}

			return;
		}
	}
}

std::vector <X3DLayerNode*>
X3DNode::getLayers () const
{
	return findParents <X3DLayerNode> (this, TRAVERSE_VISIBLE_NODES);
}

X3DLayerNode*
X3DNode::getCurrentLayer () const
{
	return getBrowser () -> getLayers () .top ();
}

NavigationInfo*
X3DNode::getCurrentNavigationInfo () const
{
	return getCurrentLayer () -> getNavigationInfo ();
}

X3DViewpointNode*
X3DNode::getCurrentViewpoint () const
{
	return getCurrentLayer () -> getViewpoint ();
}

const Matrix4f &
X3DNode::getCameraSpaceMatrix () const
{
	return getCurrentViewpoint () -> getCameraSpaceMatrix ();
}

const Matrix4f &
X3DNode::getInverseCameraSpaceMatrix () const
{
	return getCurrentViewpoint () -> getInverseCameraSpaceMatrix ();
}

Matrix4f
X3DNode::getModelViewMatrix (const TraverseType type) const
{
	if (type == TraverseType::CAMERA)
		return getModelViewMatrix () .get () * getInverseCameraSpaceMatrix ();

	return getModelViewMatrix () .get ();
}

} // X3D
} // titania
