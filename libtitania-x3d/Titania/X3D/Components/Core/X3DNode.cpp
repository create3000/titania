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
MFBool &
X3DNode::getMetaData <MFBool> (const std::string & key, const bool create)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, create);

	return set -> getMetaData <MFBool> (names .back (), create);
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
MFDouble &
X3DNode::getMetaData <MFDouble> (const std::string & key, const bool create)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, create);

	return set -> getMetaData <MFDouble> (names .back (), create);
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
MFFloat &
X3DNode::getMetaData <MFFloat> (const std::string & key, const bool create)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, create);

	return set -> getMetaData <MFFloat> (names .back (), create);
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
MFInt32 &
X3DNode::getMetaData <MFInt32> (const std::string & key, const bool create)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, create);

	return set -> getMetaData <MFInt32> (names .back (), create);
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
MFString &
X3DNode::getMetaData <MFString> (const std::string & key, const bool create)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto names = basic::split (key, "/");
	const auto set   = getMetadataSet (names, create);

	return set -> getMetaData <MFString> (names .back (), create);
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

void
X3DNode::fromMetaData (const X3DPtr <MetadataSet> & metadataSetNode)
{

}

X3DPtr <MetadataSet>
X3DNode::toMetaData () const
{
	auto metadataSetNode = getExecutionContext () -> createNode <MetadataSet> ();

	for (const auto & fieldDefinition : getChangedFields ())
	   fieldToMetaData (metadataSetNode, fieldDefinition);

	return metadataSetNode;
}

void
X3DNode::fieldToMetaData (const X3DPtr <MetadataSet> & metadataSetNode, const X3DFieldDefinition* const fieldDefinition) const
{
	switch (fieldDefinition -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		{
			const auto & field = static_cast <const SFBool &> (*fieldDefinition);

			metadataSetNode -> setMetaData <MFBool> (field .getName (), { field });
			break;
		}
		case X3D::X3DConstants::SFColor:
		{
			const auto & field = static_cast <const SFColor &> (*fieldDefinition);

			MFFloat array ({ field .getRed   (),
			                 field .getGreen (),
			                 field .getBlue  () });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFColorRGBA:
		{
			const auto & field = static_cast <const SFColorRGBA &> (*fieldDefinition);

			MFFloat array ({ field .getRed   (),
			                 field .getGreen (),
			                 field .getBlue  (),
			                 field .getAlpha () });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFDouble:
		{
			const auto & field = static_cast <const SFDouble &> (*fieldDefinition);

			metadataSetNode -> setMetaData <MFDouble> (field .getName (), { field });
			break;
		}
		case X3D::X3DConstants::SFFloat:
		{
			const auto & field = static_cast <const SFFloat &> (*fieldDefinition);

			metadataSetNode -> setMetaData <MFFloat> (field .getName (), { field });
			break;
		}
		case X3D::X3DConstants::SFInt32:
		{
			const auto & field = static_cast <const SFInt32 &> (*fieldDefinition);

			metadataSetNode -> setMetaData <MFInt32> (field .getName (), { field });
			break;
		}
		case X3D::X3DConstants::SFImage:
		{
			const auto & field = static_cast <const SFImage &> (*fieldDefinition);

			MFInt32 array ({ field .getWidth (),
			                 field .getHeight (),
			                 field .getComponents () });

			for (const auto & pixel : field .getArray ())
			   array .emplace_back (pixel);

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFMatrix3d:
		{
			const auto & field = static_cast <const SFMatrix3d &> (*fieldDefinition);

			MFDouble array;

			for (size_t i = 0; i < Matrix3d::size (); ++ i)
			   array .emplace_back (field .get1Value (i));

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFMatrix3f:
		{
			const auto & field = static_cast <const SFMatrix3f &> (*fieldDefinition);

			MFFloat array;

			for (size_t i = 0; i < Matrix3f::size (); ++ i)
			   array .emplace_back (field .get1Value (i));

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFMatrix4d:
		{
			const auto & field = static_cast <const SFMatrix4d &> (*fieldDefinition);

			MFDouble array;

			for (size_t i = 0; i < Matrix4d::size (); ++ i)
			   array .emplace_back (field .get1Value (i));

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFMatrix4f:
		{
			const auto & field = static_cast <const SFMatrix4f &> (*fieldDefinition);

			MFFloat array;

			for (size_t i = 0; i < Matrix4f::size (); ++ i)
			   array .emplace_back (field .get1Value (i));

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFNode:
		{
			break;
		}
		case X3D::X3DConstants::SFRotation:
		{
			const auto & field = static_cast <const SFRotation &> (*fieldDefinition);

			SFRotation::value_type x, y, z, angle;

			field .getValue (x, y, z, angle);

			MFDouble array ({ x, y, z, angle });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFString:
		{
			const auto & field = static_cast <const SFString &> (*fieldDefinition);

			metadataSetNode -> setMetaData <MFString> (field .getName (), { field });
			break;
		}
		case X3D::X3DConstants::SFTime:
		{
			const auto & field = static_cast <const SFTime &> (*fieldDefinition);

			metadataSetNode -> setMetaData <MFDouble> (field .getName (), { field });
			break;
		}
		case X3D::X3DConstants::SFVec2d:
		{
			const auto & field = static_cast <const SFVec2d &> (*fieldDefinition);

			MFDouble array ({ field .getX (),
			                  field .getY () });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFVec2f:
		{
			const auto & field = static_cast <const SFVec2f &> (*fieldDefinition);

			MFFloat array ({ field .getX (),
			                 field .getY () });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFVec3d:
		{
			const auto & field = static_cast <const SFVec3d &> (*fieldDefinition);

			MFDouble array ({ field .getX (),
			                  field .getY (),
			                  field .getZ () });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFVec3f:
		{
			const auto & field = static_cast <const SFVec3f &> (*fieldDefinition);

			MFFloat array ({ field .getX (),
			                 field .getY (),
			                 field .getZ () });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFVec4d:
		{
			const auto & field = static_cast <const SFVec4d &> (*fieldDefinition);

			MFDouble array ({ field .getX (),
			                  field .getY (),
			                  field .getZ (),
			                  field .getW () });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::SFVec4f:
		{
			const auto & field = static_cast <const SFVec4f &> (*fieldDefinition);

			MFFloat array ({ field .getX (),
			                 field .getY (),
			                 field .getZ (),
			                 field .getW () });

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFBool:
		{
			const auto & field = static_cast <const MFBool &> (*fieldDefinition);

			metadataSetNode -> setMetaData (field .getName (), field);
			break;
		}
		case X3D::X3DConstants::MFColor:
		{
			const auto & field = static_cast <const MFColor &> (*fieldDefinition);

			MFFloat array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getRed   ());
				array .emplace_back (value .getGreen ());
				array .emplace_back (value .getBlue  ());
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFColorRGBA:
		{
			const auto & field = static_cast <const MFColorRGBA &> (*fieldDefinition);

			MFFloat array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getRed   ());
				array .emplace_back (value .getGreen ());
				array .emplace_back (value .getBlue  ());
				array .emplace_back (value .getAlpha ());
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFDouble:
		{
			const auto & field = static_cast <const MFDouble &> (*fieldDefinition);

			metadataSetNode -> setMetaData (field .getName (), field);
			break;
		}
		case X3D::X3DConstants::MFFloat:
		{
			const auto & field = static_cast <const MFFloat &> (*fieldDefinition);

			metadataSetNode -> setMetaData (field .getName (), field);
			break;
		}
		case X3D::X3DConstants::MFImage:
		{
			const auto & field = static_cast <const MFImage &> (*fieldDefinition);

			MFInt32 array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getWidth ());
				array .emplace_back (value .getHeight ());
				array .emplace_back (value .getComponents ());

				for (const auto & pixel : value .getArray ())
				   array .emplace_back (pixel);
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFInt32:
		{
			const auto & field = static_cast <const MFInt32 &> (*fieldDefinition);

			metadataSetNode -> setMetaData (field .getName (), field);
			break;
		}
		case X3D::X3DConstants::MFMatrix3d:
		{
			const auto & field = static_cast <const MFMatrix3d &> (*fieldDefinition);

			MFDouble array;

			for (const auto & value : field)
			{
				for (size_t i = 0; i < Matrix3d::size (); ++ i)
					array .emplace_back (value .get1Value (i));
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFMatrix3f:
		{
			const auto & field = static_cast <const MFMatrix3f &> (*fieldDefinition);

			MFFloat array;

			for (const auto & value : field)
			{
				for (size_t i = 0; i < Matrix3f::size (); ++ i)
					array .emplace_back (value .get1Value (i));
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFMatrix4d:
		{
			const auto & field = static_cast <const MFMatrix4d &> (*fieldDefinition);

			MFDouble array;

			for (const auto & value : field)
			{
				for (size_t i = 0; i < Matrix4d::size (); ++ i)
					array .emplace_back (value .get1Value (i));
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFMatrix4f:
		{
			const auto & field = static_cast <const MFMatrix4f &> (*fieldDefinition);

			MFFloat array;

			for (const auto & value : field)
			{
				for (size_t i = 0; i < Matrix4f::size (); ++ i)
					array .emplace_back (value .get1Value (i));
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFNode:
		{
			break;
		}
		case X3D::X3DConstants::MFRotation:
		{
			const auto & field = static_cast <const MFRotation &> (*fieldDefinition);

			MFDouble array;

			for (const auto & value : field)
			{
				SFRotation::value_type x, y, z, angle;

				value .getValue (x, y, z, angle);

				array .emplace_back (x);
				array .emplace_back (y);
				array .emplace_back (z);
				array .emplace_back (angle);
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFString:
		{
			const auto & field = static_cast <const MFString &> (*fieldDefinition);

			metadataSetNode -> setMetaData (field .getName (), field);
			break;
		}
		case X3D::X3DConstants::MFTime:
		{
			const auto & field = static_cast <const MFTime &> (*fieldDefinition);

			MFDouble array (field .begin (), field .end ());

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFVec2d:
		{
			const auto & field = static_cast <const MFVec2d &> (*fieldDefinition);

			MFDouble array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFVec2f:
		{
			const auto & field = static_cast <const MFVec2f &> (*fieldDefinition);

			MFFloat array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFVec3d:
		{
			const auto & field = static_cast <const MFVec3d &> (*fieldDefinition);

			MFDouble array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFVec3f:
		{
			const auto & field = static_cast <const MFVec3f &> (*fieldDefinition);

			MFFloat array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFVec4d:
		{
			const auto & field = static_cast <const MFVec4d &> (*fieldDefinition);

			MFDouble array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
				array .emplace_back (value .getW ());
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
		}
		case X3D::X3DConstants::MFVec4f:
		{
			const auto & field = static_cast <const MFVec4f &> (*fieldDefinition);

			MFFloat array;

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
				array .emplace_back (value .getW ());
			}

			metadataSetNode -> setMetaData (field .getName (), array);
			break;
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
