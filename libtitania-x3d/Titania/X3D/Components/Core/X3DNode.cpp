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

#include "../../Browser/Core/Cast.h"
#include "../../Basic/Traverse.h"
#include "../../Browser/X3DBrowser.h"
#include "../Core/MetadataBoolean.h"
#include "../Core/MetadataDouble.h"
#include "../Core/MetadataFloat.h"
#include "../Core/MetadataInteger.h"
#include "../Core/MetadataString.h"
#include "../Core/MetadataSet.h"
#include "../Layering/X3DLayerNode.h"

namespace titania {
namespace X3D {

static const  std::string SEPARATOR = "/";

X3DNode::Fields::Fields () :
	metadata (new SFNode ())
{ }

X3DNode::X3DNode () :
	X3DBaseNode (),
	     fields ()
{
	addType (X3DConstants::X3DNode);
}

/*
 * createMetaData
 */

template <>
X3DPtr <MetadataBoolean>
X3DNode::createMetaData <MetadataBoolean> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	return metadataSet -> createValue <MetadataBoolean> (names .back ());
}

template <>
X3DPtr <MetadataDouble>
X3DNode::createMetaData <MetadataDouble> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	return metadataSet -> createValue <MetadataDouble> (names .back ());
}

template <>
X3DPtr <MetadataFloat>
X3DNode::createMetaData <MetadataFloat> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	return metadataSet -> createValue <MetadataFloat> (names .back ());
}

template <>
X3DPtr <MetadataInteger>
X3DNode::createMetaData <MetadataInteger> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	return metadataSet -> createValue <MetadataInteger> (names .back ());
}

template <>
X3DPtr <MetadataString>
X3DNode::createMetaData <MetadataString> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	return metadataSet -> createValue <MetadataString> (names .back ());
}

template <>
X3DPtr <MetadataSet>
X3DNode::createMetaData <MetadataSet> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	return metadataSet -> createValue <MetadataSet> (names .back ());
}


/*
 * getMetaData
 */


template <>
X3DPtr <MetadataBoolean>
X3DNode::getMetaData <MetadataBoolean> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, true);

	return metadataSet -> getValue <MetadataBoolean> (names .back ());
}

template <>
X3DPtr <MetadataDouble>
X3DNode::getMetaData <MetadataDouble> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, true);

	return metadataSet -> getValue <MetadataDouble> (names .back ());
}

template <>
X3DPtr <MetadataFloat>
X3DNode::getMetaData <MetadataFloat> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, true);

	return metadataSet -> getValue <MetadataFloat> (names .back ());
}

template <>
X3DPtr <MetadataInteger>
X3DNode::getMetaData <MetadataInteger> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, true);

	return metadataSet -> getValue <MetadataInteger> (names .back ());
}

template <>
X3DPtr <MetadataString>
X3DNode::getMetaData <MetadataString> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, true);

	return metadataSet -> getValue <MetadataString> (names .back ());
}

template <>
X3DPtr <MetadataSet>
X3DNode::getMetaData <MetadataSet> (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, true);

	return metadataSet -> getValue <MetadataSet> (names .back ());
}








/*
 * Misc
 */

template <>
void
X3DNode::setMetaData <bool> (const std::string & key, const bool & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData(key, MFBool ({ value }));
}

template <>
void
X3DNode::setMetaData <Color3f> (const std::string & key, const Color3f & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .r (), value .g (), value .b () }));
}

template <>
void
X3DNode::setMetaData <Color4f> (const std::string & key, const Color4f & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .r (), value .g (), value .b (), value .a () }));
}

template <>
void
X3DNode::setMetaData <double> (const std::string & key, const double & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value }));
}

template <>
void
X3DNode::setMetaData <float> (const std::string & key, const float & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value }));
}

template <>
void
X3DNode::setMetaData <int32_t> (const std::string & key, const int32_t & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFInt32 ({ value }));
}

template <>
void
X3DNode::setMetaData <Rotation4d> (const std::string & key, const Rotation4d & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	Rotation4d::value_type x, y, z, angle;
	
	value .get (x, y, z, angle);

	setMetaData (key, MFDouble ({ x, y, z, angle }));
}

template <>
void
X3DNode::setMetaData <std::string> (const std::string & key, const std::string & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFString ({ value }));
}

template <>
void
X3DNode::setMetaData <Vector2d> (const std::string & key, const Vector2d & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value .x (), value .y () }));
}

template <>
void
X3DNode::setMetaData <Vector2f> (const std::string & key, const Vector2f & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .x (), value .y () }));
}

template <>
void
X3DNode::setMetaData <Vector3d> (const std::string & key, const Vector3d & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value .x (), value .y (), value .z () }));
}

template <>
void
X3DNode::setMetaData <Vector3f> (const std::string & key, const Vector3f & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .x (), value .y (), value .z () }));
}

template <>
void
X3DNode::setMetaData <Vector4d> (const std::string & key, const Vector4d & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value .x (), value .y (), value .z (), value .w () }));
}

template <>
void
X3DNode::setMetaData <Vector4f> (const std::string & key, const Vector4f & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value .x (), value .y (), value .z (), value .w () }));
}

template <>
void
X3DNode::setMetaData <MFBool> (const std::string & key, const MFBool & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	metadataSet -> createValue <MetadataBoolean> (names .back ()) -> value () = value;
}

template <>
void
X3DNode::setMetaData <MFDouble> (const std::string & key, const MFDouble & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	metadataSet -> createValue <MetadataDouble> (names .back ()) -> value () = value;
}

template <>
void
X3DNode::setMetaData <MFFloat> (const std::string & key, const MFFloat & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	metadataSet -> createValue <MetadataFloat> (names .back ()) -> value () = value;
}

template <>
void
X3DNode::setMetaData <MFInt32> (const std::string & key, const MFInt32 & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	metadataSet -> createValue <MetadataInteger> (names .back ()) -> value () = value;
}

template <>
void
X3DNode::setMetaData <MFString> (const std::string & key, const MFString & value)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	metadataSet -> createValue <MetadataString> (names .back ()) -> value () = value;
}

template <>
MFBool &
X3DNode::getMetaData <MFBool> (const std::string & key, const bool create)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, not create);

	if (create)
		return metadataSet -> createValue <MetadataBoolean> (names .back ()) -> value ();

	return metadataSet -> getValue <MetadataBoolean> (names .back ()) -> value ();
}

template <>
MFDouble &
X3DNode::getMetaData <MFDouble> (const std::string & key, const bool create)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, not create);

	if (create)
		return metadataSet -> createValue <MetadataDouble> (names .back ()) -> value ();

	return metadataSet -> getValue <MetadataDouble> (names .back ()) -> value ();
}

template <>
MFFloat &
X3DNode::getMetaData <MFFloat> (const std::string & key, const bool create)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, not create);

	if (create)
		return metadataSet -> createValue <MetadataFloat> (names .back ()) -> value ();

	return metadataSet -> getValue <MetadataFloat> (names .back ()) -> value ();
}

template <>
MFInt32 &
X3DNode::getMetaData <MFInt32> (const std::string & key, const bool create)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, not create);

	if (create)
		return metadataSet -> createValue <MetadataInteger> (names .back ()) -> value ();

	return metadataSet -> getValue <MetadataInteger> (names .back ()) -> value ();
}

template <>
MFString &
X3DNode::getMetaData <MFString> (const std::string & key, const bool create)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, not create);

	if (create)
		return metadataSet -> createValue <MetadataString> (names .back ()) -> value ();

	return metadataSet -> getValue <MetadataString> (names .back ()) -> value ();
}







/*
 * getMetadataSet
 */

MetadataSet*
X3DNode::getMetadataSet (const std::vector <std::string> & names, const bool throw_) const
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	if (names .size () < 3)
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	if (not names .front () .empty ())
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	if (names [1] .empty ())
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	auto metadataSet = x3d_cast <MetadataSet*> (metadata ());

	if (not metadataSet or metadataSet -> name () not_eq names [1])
	{
		if (throw_)
			throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

		metadataSet                 = new MetadataSet (getExecutionContext ());
		metadataSet -> name ()      = names [1];
		metadataSet -> reference () = getBrowser () -> getProviderUrl ();
		metadataSet -> setup ();
		const_cast <X3DNode*> (this) -> metadata () = metadataSet;
	
		getExecutionContext () -> addNamedNode (getExecutionContext () -> getUniqueName (names [1]), const_cast <X3DNode*> (this) -> metadata ());
	}

	for (const auto & name : std::make_pair (names .begin () + 2, names .end () - 1))
	{
	   metadataSet = metadataSet -> getValue <MetadataSet> (name, throw_);
	}

	return metadataSet;
}

/*
 * removeMetaData
 */

void
X3DNode::removeMetaData (const std::string & key)
throw (Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

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
			const auto metadataSet = x3d_cast <MetadataSet*> (metadata ());

			if (not metadataSet)
			   return;
			 
			if (metadataSet -> name () not_eq names [1])
			   return;

			metadata () = nullptr;
			return;
		}
		default:
		{
			std::vector <MetadataSet*> metadataSets;

			const auto metadataSet = x3d_cast <MetadataSet*> (metadata ());

			if (not metadataSet)
				return;
			
			if (metadataSet -> name () not_eq names [1])
				return;

			metadataSets .emplace_back (metadataSet);

			try
			{
				for (const auto & name : std::make_pair (names .begin () + 2, names .end () - 1))
					metadataSets .emplace_back (metadataSets .back () -> getValue <MetadataSet> (name));
			}
			catch (const Error <INVALID_NAME> &)
			{
				return;
			}

			metadataSets .back () -> removeValue (names .back ());

			while (metadataSets .back () -> value () .empty ())
			{
				names .pop_back ();
				metadataSets .pop_back ();

				if (metadataSets .empty ())
				{
					metadata () = nullptr;
					break;
				}

				metadataSets .back () -> removeValue (names .back ());
			}

			return;
		}
	}
}

/*
 * fromMetaData / toMetaData
 */

void
X3DNode::fromMetaData (const X3DPtr <MetadataSet> & metadataSetNode)
{

}

X3DPtr <MetadataSet>
X3DNode::toMetaData () const
{
	auto metadataSetNode = getExecutionContext () -> createNode <MetadataSet> ();

	metadataSetNode -> name () = getName ();
	metadataSetNode -> createValue <MetadataString> ("TypeName") -> value () = { getTypeName () };

	for (const auto & fieldDefinition : getChangedFields ())
		fieldToMetaData (metadataSetNode, fieldDefinition);

	metadataSetNode -> setup ();
	return metadataSetNode;
}

void
X3DNode::fieldToMetaData (const X3DPtr <MetadataSet> & metadataSetNode, const X3DFieldDefinition* const fieldDefinition) const
{
	switch (fieldDefinition -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		{
			const auto & field    = static_cast <const SFBool &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataBoolean> (field .getName ());

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFColor:
		{
			const auto & field    = static_cast <const SFColor &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			metadata -> value () = { field .getRed   (),
			                         field .getGreen (),
			                         field .getBlue  () };

			break;
		}
		case X3D::X3DConstants::SFColorRGBA:
		{
			const auto & field    = static_cast <const SFColorRGBA &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			metadata -> value () = { field .getRed   (),
			                         field .getGreen (),
			                         field .getBlue  (),
			                         field .getAlpha () };

			break;
		}
		case X3D::X3DConstants::SFDouble:
		{
			const auto & field    = static_cast <const SFDouble &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFFloat:
		{
			const auto & field    = static_cast <const SFFloat &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFInt32:
		{
			const auto & field    = static_cast <const SFInt32 &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataInteger> (field .getName ());

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFImage:
		{
			const auto & field    = static_cast <const SFImage &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataInteger> (field .getName ());

			MFInt32 array ({ field .getWidth (),
			                 field .getHeight (),
			                 field .getComponents () });

			for (const auto & pixel : field .getArray ())
			   array .emplace_back (pixel);

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFMatrix3d:
		{
			const auto & field    = static_cast <const SFMatrix3d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			MFDouble array;

			for (size_t i = 0; i < Matrix3d::size (); ++ i)
			   array .emplace_back (field .get1Value (i));

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFMatrix3f:
		{
			const auto & field    = static_cast <const SFMatrix3f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat array;

			for (size_t i = 0; i < Matrix3f::size (); ++ i)
			   array .emplace_back (field .get1Value (i));

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFMatrix4d:
		{
			const auto & field    = static_cast <const SFMatrix4d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			MFDouble array;

			for (size_t i = 0; i < Matrix4d::size (); ++ i)
			   array .emplace_back (field .get1Value (i));

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFMatrix4f:
		{
			const auto & field    = static_cast <const SFMatrix4f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat array;

			for (size_t i = 0; i < Matrix4f::size (); ++ i)
			   array .emplace_back (field .get1Value (i));

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFNode:
		{
			const auto & field    = static_cast <const SFNode &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataSet> (field .getName ());
		
			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFRotation:
		{
			const auto & field    = static_cast <const SFRotation &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			SFRotation::value_type x, y, z, angle;

			field .getValue (x, y, z, angle);

			metadata -> value () = { x, y, z, angle };
			break;
		}
		case X3D::X3DConstants::SFString:
		{
			const auto & field    = static_cast <const SFString &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataString> (field .getName ());

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFTime:
		{
			const auto & field    = static_cast <const SFTime &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFVec2d:
		{
			const auto & field    = static_cast <const SFVec2d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			metadata -> value () = { field .getX (),
			                         field .getY () };

			break;
		}
		case X3D::X3DConstants::SFVec2f:
		{
			const auto & field    = static_cast <const SFVec2f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			metadata -> value () = { field .getX (),
			                         field .getY () };

			break;
		}
		case X3D::X3DConstants::SFVec3d:
		{
			const auto & field    = static_cast <const SFVec3d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			metadata -> value () = { field .getX (),
			                         field .getY (),
			                         field .getZ () };

			break;
		}
		case X3D::X3DConstants::SFVec3f:
		{
			const auto & field    = static_cast <const SFVec3f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			metadata -> value () = { field .getX (),
			                         field .getY (),
			                         field .getZ () };

			break;
		}
		case X3D::X3DConstants::SFVec4d:
		{
			const auto & field    = static_cast <const SFVec4d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			metadata -> value () = { field .getX (),
			                         field .getY (),
			                         field .getZ (),
			                         field .getW () };

			break;
		}
		case X3D::X3DConstants::SFVec4f:
		{
			const auto & field    = static_cast <const SFVec4f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			metadata -> value () = { field .getX (),
			                         field .getY (),
			                         field .getZ (),
			                         field .getW () };

			break;
		}
		case X3D::X3DConstants::MFBool:
		{
			const auto & field    = static_cast <const MFBool &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataBoolean> (field .getName ());

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFColor:
		{
			const auto & field    = static_cast <const MFColor &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getRed   ());
				array .emplace_back (value .getGreen ());
				array .emplace_back (value .getBlue  ());
			}

			break;
		}
		case X3D::X3DConstants::MFColorRGBA:
		{
			const auto & field    = static_cast <const MFColorRGBA &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getRed   ());
				array .emplace_back (value .getGreen ());
				array .emplace_back (value .getBlue  ());
				array .emplace_back (value .getAlpha ());
			}

			break;
		}
		case X3D::X3DConstants::MFDouble:
		{
			const auto & field    = static_cast <const MFDouble &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFFloat:
		{
			const auto & field    = static_cast <const MFFloat &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFImage:
		{
			const auto & field    = static_cast <const MFImage &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataInteger> (field .getName ());

			MFInt32 & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getWidth ());
				array .emplace_back (value .getHeight ());
				array .emplace_back (value .getComponents ());

				for (const auto & pixel : value .getArray ())
				   array .emplace_back (pixel);
			}

			break;
		}
		case X3D::X3DConstants::MFInt32:
		{
			const auto & field    = static_cast <const MFInt32 &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataInteger> (field .getName ());

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFMatrix3d:
		{
			const auto & field    = static_cast <const MFMatrix3d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				for (size_t i = 0; i < Matrix3d::size (); ++ i)
					array .emplace_back (value .get1Value (i));
			}

			break;
		}
		case X3D::X3DConstants::MFMatrix3f:
		{
			const auto & field    = static_cast <const MFMatrix3f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				for (size_t i = 0; i < Matrix3f::size (); ++ i)
					array .emplace_back (value .get1Value (i));
			}

			break;
		}
		case X3D::X3DConstants::MFMatrix4d:
		{
			const auto & field    = static_cast <const MFMatrix4d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				for (size_t i = 0; i < Matrix4d::size (); ++ i)
					array .emplace_back (value .get1Value (i));
			}

			break;
		}
		case X3D::X3DConstants::MFMatrix4f:
		{
			const auto & field    = static_cast <const MFMatrix4f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				for (size_t i = 0; i < Matrix4f::size (); ++ i)
					array .emplace_back (value .get1Value (i));
			}

			break;
		}
		case X3D::X3DConstants::MFNode:
		{
			const auto & field    = static_cast <const MFNode &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataSet> (field .getName ());
		
			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFRotation:
		{
			const auto & field    = static_cast <const MFRotation &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				SFRotation::value_type x, y, z, angle;

				value .getValue (x, y, z, angle);

				array .emplace_back (x);
				array .emplace_back (y);
				array .emplace_back (z);
				array .emplace_back (angle);
			}

			break;
		}
		case X3D::X3DConstants::MFString:
		{
			const auto & field    = static_cast <const MFString &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataString> (field .getName ());

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFTime:
		{
			const auto & field    = static_cast <const MFTime &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			metadata -> value () .assign (field .begin (), field .end ());
			break;
		}
		case X3D::X3DConstants::MFVec2d:
		{
			const auto & field    = static_cast <const MFVec2d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec2f:
		{
			const auto & field    = static_cast <const MFVec2f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec3d:
		{
			const auto & field    = static_cast <const MFVec3d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec3f:
		{
			const auto & field    = static_cast <const MFVec3f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec4d:
		{
			const auto & field    = static_cast <const MFVec4d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataDouble> (field .getName ());

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
				array .emplace_back (value .getW ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec4f:
		{
			const auto & field    = static_cast <const MFVec4f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> createValue <MetadataFloat> (field .getName ());

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
				array .emplace_back (value .getW ());
			}

			break;
		}
	}
}

/*
 * Misc
 */

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

const ViewVolumeStack &
X3DNode::getViewVolumes () const
{
	return getCurrentLayer () -> getViewVolumes ();
}

ViewVolumeStack &
X3DNode::getViewVolumes ()
{
	return getCurrentLayer () -> getViewVolumes ();
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

const Matrix4d &
X3DNode::getCameraSpaceMatrix () const
{
	return getCurrentViewpoint () -> getCameraSpaceMatrix ();
}

const Matrix4d &
X3DNode::getInverseCameraSpaceMatrix () const
{
	return getCurrentViewpoint () -> getInverseCameraSpaceMatrix ();
}

Matrix4dStack &
X3DNode::getProjectionMatrix ()
{
	return getBrowser () -> getProjectionMatrix ();
}

const Matrix4dStack &
X3DNode::getProjectionMatrix () const
{
	return getBrowser () -> getProjectionMatrix ();
}

Matrix4dStack &
X3DNode::getModelViewMatrix ()
{
	return getBrowser () -> getModelViewMatrix ();
}

const Matrix4dStack &
X3DNode::getModelViewMatrix () const
{
	return getBrowser () -> getModelViewMatrix ();
}

Matrix4d
X3DNode::getModelViewMatrix (const TraverseType type) const
{
	if (type == TraverseType::CAMERA)
		return getBrowser () -> getModelViewMatrix () .get () * getInverseCameraSpaceMatrix ();

	return getBrowser () ->  getModelViewMatrix () .get ();
}

} // X3D
} // titania
