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

#include "field.h"

#include "Fields.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

v8::Handle <v8::Value>
setValue (Context* const context, X3D::X3DFieldDefinition* const field, const v8::Local <v8::Value> & value)
throw (std::invalid_argument)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
			return SFBool::setValue (context, static_cast <X3D::SFBool*> (field), value);

		case X3DConstants::SFColor:
			return SFColor::setValue (context, static_cast <X3D::SFColor*> (field), value);

		case X3DConstants::SFColorRGBA:
			return SFColorRGBA::setValue (context, static_cast <X3D::SFColorRGBA*> (field), value);

		case X3DConstants::SFDouble:
			return SFDouble::setValue (context, static_cast <X3D::SFDouble*> (field), value);

		case X3DConstants::SFFloat:
			return SFFloat::setValue (context, static_cast <X3D::SFFloat*> (field), value);

		case X3DConstants::SFImage:
			return SFImage::setValue (context, static_cast <X3D::SFImage*> (field), value);

		case X3DConstants::SFInt32:
			return SFInt32::setValue (context, static_cast <X3D::SFInt32*> (field), value);

		case X3DConstants::SFMatrix3d:
			return SFMatrix3d::setValue (context, static_cast <X3D::SFMatrix3d*> (field), value);

		case X3DConstants::SFMatrix3f:
			return SFMatrix3f::setValue (context, static_cast <X3D::SFMatrix3f*> (field), value);

		case X3DConstants::SFMatrix4d:
			return SFMatrix4d::setValue (context, static_cast <X3D::SFMatrix4d*> (field), value);

		case X3DConstants::SFMatrix4f:
			return SFMatrix4f::setValue (context, static_cast <X3D::SFMatrix4f*> (field), value);

		case X3DConstants::SFNode:
			return SFNode::setValue (context, static_cast <X3D::SFNode*> (field), value);

		case X3DConstants::SFRotation:
			return SFRotation::setValue (context, static_cast <X3D::SFRotation*> (field), value);

		case X3DConstants::SFString:
			return SFString::setValue (context, static_cast <X3D::SFString*> (field), value);

		case X3DConstants::SFTime:
			return SFTime::setValue (context, static_cast <X3D::SFTime*> (field), value);

		case X3DConstants::SFVec2d:
			return SFVec2d::setValue (context, static_cast <X3D::SFVec2d*> (field), value);

		case X3DConstants::SFVec2f:
			return SFVec2f::setValue (context, static_cast <X3D::SFVec2f*> (field), value);

		case X3DConstants::SFVec3d:
			return SFVec3d::setValue (context, static_cast <X3D::SFVec3d*> (field), value);

		case X3DConstants::SFVec3f:
			return SFVec3f::setValue (context, static_cast <X3D::SFVec3f*> (field), value);

		case X3DConstants::SFVec4d:
			return SFVec4d::setValue (context, static_cast <X3D::SFVec4d*> (field), value);

		case X3DConstants::SFVec4f:
			return SFVec4f::setValue (context, static_cast <X3D::SFVec4f*> (field), value);


		case X3DConstants::MFBool:
			return MFBool::setValue (context, static_cast <X3D::MFBool*> (field), value);

		case X3DConstants::MFColor:
			return MFColor::setValue (context, static_cast <X3D::MFColor*> (field), value);

		case X3DConstants::MFColorRGBA:
			return MFColorRGBA::setValue (context, static_cast <X3D::MFColorRGBA*> (field), value);

		case X3DConstants::MFDouble:
			return MFDouble::setValue (context, static_cast <X3D::MFDouble*> (field), value);

		case X3DConstants::MFFloat:
			return MFFloat::setValue (context, static_cast <X3D::MFFloat*> (field), value);

		case X3DConstants::MFImage:
			return MFImage::setValue (context, static_cast <X3D::MFImage*> (field), value);

		case X3DConstants::MFInt32:
			return MFInt32::setValue (context, static_cast <X3D::MFInt32*> (field), value);

		case X3DConstants::MFMatrix3d:
			return MFMatrix3d::setValue (context, static_cast <X3D::MFMatrix3d*> (field), value);

		case X3DConstants::MFMatrix3f:
			return MFMatrix3f::setValue (context, static_cast <X3D::MFMatrix3f*> (field), value);

		case X3DConstants::MFMatrix4d:
			return MFMatrix4d::setValue (context, static_cast <X3D::MFMatrix4d*> (field), value);

		case X3DConstants::MFMatrix4f:
			return MFMatrix4f::setValue (context, static_cast <X3D::MFMatrix4f*> (field), value);

		case X3DConstants::MFNode:
			return MFNode::setValue (context, static_cast <X3D::MFNode*> (field), value);

		case X3DConstants::MFRotation:
			return MFRotation::setValue (context, static_cast <X3D::MFRotation*> (field), value);

		case X3DConstants::MFString:
			return MFString::setValue (context, static_cast <X3D::MFString*> (field), value);

		case X3DConstants::MFTime:
			return MFTime::setValue (context, static_cast <X3D::MFTime*> (field), value);

		case X3DConstants::MFVec2d:
			return MFVec2d::setValue (context, static_cast <X3D::MFVec2d*> (field), value);

		case X3DConstants::MFVec2f:
			return MFVec2f::setValue (context, static_cast <X3D::MFVec2f*> (field), value);

		case X3DConstants::MFVec3d:
			return MFVec3d::setValue (context, static_cast <X3D::MFVec3d*> (field), value);

		case X3DConstants::MFVec3f:
			return MFVec3f::setValue (context, static_cast <X3D::MFVec3f*> (field), value);

		case X3DConstants::MFVec4d:
			return MFVec4d::setValue (context, static_cast <X3D::MFVec4d*> (field), value);

		case X3DConstants::MFVec4f:
			return MFVec4f::setValue (context, static_cast <X3D::MFVec4f*> (field), value);
	}

	throw std::invalid_argument ("setValue");
}

v8::Handle <v8::Value>
getValue (Context* const context, X3D::X3DFieldDefinition* const field)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
			return SFBool::create (context, static_cast <X3D::SFBool*> (field));

		case X3DConstants::SFColor:
			return SFColor::create (context, static_cast <X3D::SFColor*> (field));

		case X3DConstants::SFColorRGBA:
			return SFColorRGBA::create (context, static_cast <X3D::SFColorRGBA*> (field));

		case X3DConstants::SFDouble:
			return SFDouble::create (context, static_cast <X3D::SFDouble*> (field));

		case X3DConstants::SFFloat:
			return SFFloat::create (context, static_cast <X3D::SFFloat*> (field));

		case X3DConstants::SFImage:
			return SFImage::create (context, static_cast <X3D::SFImage*> (field));

		case X3DConstants::SFInt32:
			return SFInt32::create (context, static_cast <X3D::SFInt32*> (field));

		case X3DConstants::SFMatrix3d:
			return SFMatrix3d::create (context, static_cast <X3D::SFMatrix3d*> (field));

		case X3DConstants::SFMatrix3f:
			return SFMatrix3f::create (context, static_cast <X3D::SFMatrix3f*> (field));

		case X3DConstants::SFMatrix4d:
			return SFMatrix4d::create (context, static_cast <X3D::SFMatrix4d*> (field));

		case X3DConstants::SFMatrix4f:
			return SFMatrix4f::create (context, static_cast <X3D::SFMatrix4f*> (field));

		case X3DConstants::SFNode:
			return SFNode::create (context, static_cast <X3D::SFNode*> (field));

		case X3DConstants::SFRotation:
			return SFRotation::create (context, static_cast <X3D::SFRotation*> (field));

		case X3DConstants::SFString:
			return SFString::create (context, static_cast <X3D::SFString*> (field));

		case X3DConstants::SFTime:
			return SFTime::create (context, static_cast <X3D::SFTime*> (field));

		case X3DConstants::SFVec2d:
			return SFVec2d::create (context, static_cast <X3D::SFVec2d*> (field));

		case X3DConstants::SFVec2f:
			return SFVec2f::create (context, static_cast <X3D::SFVec2f*> (field));

		case X3DConstants::SFVec3d:
			return SFVec3d::create (context, static_cast <X3D::SFVec3d*> (field));

		case X3DConstants::SFVec3f:
			return SFVec3f::create (context, static_cast <X3D::SFVec3f*> (field));

		case X3DConstants::SFVec4d:
			return SFVec4d::create (context, static_cast <X3D::SFVec4d*> (field));

		case X3DConstants::SFVec4f:
			return SFVec4f::create (context, static_cast <X3D::SFVec4f*> (field));


		case X3DConstants::MFBool:
			return MFBool::create (context, static_cast <X3D::MFBool*> (field));

		case X3DConstants::MFColor:
			return MFColor::create (context, static_cast <X3D::MFColor*> (field));

		case X3DConstants::MFColorRGBA:
			return MFColorRGBA::create (context, static_cast <X3D::MFColorRGBA*> (field));

		case X3DConstants::MFDouble:
			return MFDouble::create (context, static_cast <X3D::MFDouble*> (field));

		case X3DConstants::MFFloat:
			return MFFloat::create (context, static_cast <X3D::MFFloat*> (field));

		case X3DConstants::MFImage:
			return MFImage::create (context, static_cast <X3D::MFImage*> (field));

		case X3DConstants::MFInt32:
			return MFInt32::create (context, static_cast <X3D::MFInt32*> (field));

		case X3DConstants::MFMatrix3d:
			return MFMatrix3d::create (context, static_cast <X3D::MFMatrix3d*> (field));

		case X3DConstants::MFMatrix3f:
			return MFMatrix3f::create (context, static_cast <X3D::MFMatrix3f*> (field));

		case X3DConstants::MFMatrix4d:
			return MFMatrix4d::create (context, static_cast <X3D::MFMatrix4d*> (field));

		case X3DConstants::MFMatrix4f:
			return MFMatrix4f::create (context, static_cast <X3D::MFMatrix4f*> (field));

		case X3DConstants::MFNode:
			return MFNode::create (context, static_cast <X3D::MFNode*> (field));

		case X3DConstants::MFRotation:
			return MFRotation::create (context, static_cast <X3D::MFRotation*> (field));

		case X3DConstants::MFString:
			return MFString::create (context, static_cast <X3D::MFString*> (field));

		case X3DConstants::MFTime:
			return MFTime::create (context, static_cast <X3D::MFTime*> (field));

		case X3DConstants::MFVec2d:
			return MFVec2d::create (context, static_cast <X3D::MFVec2d*> (field));

		case X3DConstants::MFVec2f:
			return MFVec2f::create (context, static_cast <X3D::MFVec2f*> (field));

		case X3DConstants::MFVec3d:
			return MFVec3d::create (context, static_cast <X3D::MFVec3d*> (field));

		case X3DConstants::MFVec3f:
			return MFVec3f::create (context, static_cast <X3D::MFVec3f*> (field));

		case X3DConstants::MFVec4d:
			return MFVec4d::create (context, static_cast <X3D::MFVec4d*> (field));

		case X3DConstants::MFVec4f:
			return MFVec4f::create (context, static_cast <X3D::MFVec4f*> (field));
	}

	throw std::invalid_argument ("getValue");
}

} // GoogleV8
} // X3D
} // titania
