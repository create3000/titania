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

#include "value.h"

#include "Arguments.h"
#include "Fields.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

void
setValue (JSContext* const cx, X3DFieldDefinition* const field, const JS::HandleValue & value)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
		{
			*static_cast <X3D::SFBool*> (field) = getArgument <bool> (cx, value, 0);
			break;
		}
		case X3DConstants::SFColor:
		{
			*static_cast <X3D::SFColor*> (field) = *getArgument <SFColor> (cx, value, 0);
			break;
		}
		case X3DConstants::SFColorRGBA:
		{
			*static_cast <X3D::SFColorRGBA*> (field) = *getArgument <SFColorRGBA> (cx, value, 0);
			break;
		}
		case X3DConstants::SFDouble:
		{
			*static_cast <X3D::SFDouble*> (field) = getArgument <double> (cx, value, 0);
			break;
		}
		case X3DConstants::SFFloat:
		{
			*static_cast <X3D::SFFloat*> (field) = getArgument <double> (cx, value, 0);
			break;
		}
		case X3DConstants::SFInt32:
		{
			*static_cast <X3D::SFInt32*> (field) = getArgument <int32_t> (cx, value, 0);
			break;
		}
		case X3DConstants::SFImage:
		{
			*static_cast <X3D::SFImage*> (field) = *getArgument <SFImage> (cx, value, 0);
			break;
		}
		case X3DConstants::SFMatrix3d:
		{
			*static_cast <X3D::SFMatrix3d*> (field) = *getArgument <SFMatrix3d> (cx, value, 0);
			break;
		}
		case X3DConstants::SFMatrix3f:
		{
			*static_cast <X3D::SFMatrix3f*> (field) = *getArgument <SFMatrix3f> (cx, value, 0);
			break;
		}
		case X3DConstants::SFMatrix4d:
		{
			*static_cast <X3D::SFMatrix4d*> (field) = *getArgument <SFMatrix4d> (cx, value, 0);
			break;
		}
		case X3DConstants::SFMatrix4f:
		{
			*static_cast <X3D::SFMatrix4f*> (field) = *getArgument <SFMatrix4f> (cx, value, 0);
			break;
		}
		case X3DConstants::SFNode:
		{
			try
			{
				*static_cast <X3D::SFNode*> (field) = *getArgument <SFNode> (cx, value, 0);
			}
			catch (const std::domain_error &)
			{
				// value is null object.
				*static_cast <X3D::SFNode*> (field) = nullptr;
			}

			break;
		}
		case X3DConstants::SFRotation:
		{
			*static_cast <X3D::SFRotation*> (field) = *getArgument <SFRotation> (cx, value, 0);
			break;
		}
		case X3DConstants::SFString:
		{
			*static_cast <X3D::SFString*> (field) = getArgument <std::string> (cx, value, 0);
			break;
		}
		case X3DConstants::SFTime:
		{
			*static_cast <X3D::SFTime*> (field) = getArgument <double> (cx, value, 0);
			break;
		}
		case X3DConstants::SFVec2d:
		{
			*static_cast <X3D::SFVec2d*> (field) = *getArgument <SFVec2d> (cx, value, 0);
			break;
		}
		case X3DConstants::SFVec2f:
		{
			*static_cast <X3D::SFVec2f*> (field) = *getArgument <SFVec2f> (cx, value, 0);
			break;
		}
		case X3DConstants::SFVec3d:
		{
			*static_cast <X3D::SFVec3d*> (field) = *getArgument <SFVec3d> (cx, value, 0);
			break;
		}
		case X3DConstants::SFVec3f:
		{
			*static_cast <X3D::SFVec3f*> (field) = *getArgument <SFVec3f> (cx, value, 0);
			break;
		}
		case X3DConstants::SFVec4d:
		{
			*static_cast <X3D::SFVec4d*> (field) = *getArgument <SFVec4d> (cx, value, 0);
			break;
		}
		case X3DConstants::SFVec4f:
		{
			*static_cast <X3D::SFVec4f*> (field) = *getArgument <SFVec4f> (cx, value, 0);
			break;
		}
		case X3DConstants::MFBool:
		{
			*static_cast <X3D::MFBool*> (field) = *getArgument <MFBool> (cx, value, 0);
			break;
		}
		case X3DConstants::MFColor:
		{
			*static_cast <X3D::MFColor*> (field) = *getArgument <MFColor> (cx, value, 0);
			break;
		}
		case X3DConstants::MFColorRGBA:
		{
			*static_cast <X3D::MFColorRGBA*> (field) = *getArgument <MFColorRGBA> (cx, value, 0);
			break;
		}
		case X3DConstants::MFDouble:
		{
			*static_cast <X3D::MFDouble*> (field) = *getArgument <MFDouble> (cx, value, 0);
			break;
		}
		case X3DConstants::MFFloat:
		{
			*static_cast <X3D::MFFloat*> (field) = *getArgument <MFFloat> (cx, value, 0);
			break;
		}
		case X3DConstants::MFImage:
		{
			*static_cast <X3D::MFImage*> (field) = *getArgument <MFImage> (cx, value, 0);
			break;
		}
		case X3DConstants::MFInt32:
		{
			*static_cast <X3D::MFInt32*> (field) = *getArgument <MFInt32> (cx, value, 0);
			break;
		}
		case X3DConstants::MFMatrix3d:
		{
			*static_cast <X3D::MFMatrix3d*> (field) = *getArgument <MFMatrix3d> (cx, value, 0);
			break;
		}
		case X3DConstants::MFMatrix3f:
		{
			*static_cast <X3D::MFMatrix3f*> (field) = *getArgument <MFMatrix3f> (cx, value, 0);
			break;
		}
		case X3DConstants::MFMatrix4d:
		{
			*static_cast <X3D::MFMatrix4d*> (field) = *getArgument <MFMatrix4d> (cx, value, 0);
			break;
		}
		case X3DConstants::MFMatrix4f:
		{
			*static_cast <X3D::MFMatrix4f*> (field) = *getArgument <MFMatrix4f> (cx, value, 0);
			break;
		}
		case X3DConstants::MFNode:
		{
			*static_cast <X3D::MFNode*> (field) = *getArgument <MFNode> (cx, value, 0);
			break;
		}
		case X3DConstants::MFRotation:
		{
			*static_cast <X3D::MFRotation*> (field) = *getArgument <MFRotation> (cx, value, 0);
			break;
		}
		case X3DConstants::MFString:
		{
			*static_cast <X3D::MFString*> (field) = *getArgument <MFString> (cx, value, 0);
			break;
		}
		case X3DConstants::MFTime:
		{
			*static_cast <X3D::MFTime*> (field) = *getArgument <MFTime> (cx, value, 0);
			break;
		}
		case X3DConstants::MFVec2d:
		{
			*static_cast <X3D::MFVec2d*> (field) = *getArgument <MFVec2d> (cx, value, 0);
			break;
		}
		case X3DConstants::MFVec2f:
		{
			*static_cast <X3D::MFVec2f*> (field) = *getArgument <MFVec2f> (cx, value, 0);
			break;
		}
		case X3DConstants::MFVec3d:
		{
			*static_cast <X3D::MFVec3d*> (field) = *getArgument <MFVec3d> (cx, value, 0);
			break;
		}
		case X3DConstants::MFVec3f:
		{
			*static_cast <X3D::MFVec3f*> (field) = *getArgument <MFVec3f> (cx, value, 0);
			break;
		}
		case X3DConstants::MFVec4d:
		{
			*static_cast <X3D::MFVec4d*> (field) = *getArgument <MFVec4d> (cx, value, 0);
			break;
		}
		case X3DConstants::MFVec4f:
		{
			*static_cast <X3D::MFVec4f*> (field) = *getArgument <MFVec4f> (cx, value, 0);
			break;
		}
	}
}

JS::Value
getValue (JSContext* const cx, X3DFieldDefinition* const field)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
			return SFBool::create (cx, *static_cast <X3D::SFBool*> (field));

		case X3DConstants::SFColor:
			return SFColor::create (cx, static_cast <X3D::SFColor*> (field));

		case X3DConstants::SFColorRGBA:
			return SFColorRGBA::create (cx, static_cast <X3D::SFColorRGBA*> (field));

		case X3DConstants::SFDouble:
			return SFDouble::create (cx, *static_cast <X3D::SFDouble*> (field));

		case X3DConstants::SFFloat:
			return SFFloat::create (cx, *static_cast <X3D::SFFloat*> (field));

		case X3DConstants::SFInt32:
			return SFInt32::create (cx, *static_cast <X3D::SFInt32*> (field));

		case X3DConstants::SFImage:
			return SFImage::create (cx, static_cast <X3D::SFImage*> (field));

		case X3DConstants::SFMatrix3d:
			return SFMatrix3d::create (cx, static_cast <X3D::SFMatrix3d*> (field));

		case X3DConstants::SFMatrix3f:
			return SFMatrix3f::create (cx, static_cast <X3D::SFMatrix3f*> (field));

		case X3DConstants::SFMatrix4d:
			return SFMatrix4d::create (cx, static_cast <X3D::SFMatrix4d*> (field));

		case X3DConstants::SFMatrix4f:
			return SFMatrix4f::create (cx, static_cast <X3D::SFMatrix4f*> (field));

		case X3DConstants::SFNode:
			return SFNode::create (cx, *static_cast <X3D::SFNode*> (field));

		case X3DConstants::SFRotation:
			return SFRotation::create (cx, static_cast <X3D::SFRotation*> (field));

		case X3DConstants::SFString:
			return SFString::create (cx, *static_cast <X3D::SFString*> (field));

		case X3DConstants::SFTime:
			return SFTime::create (cx, *static_cast <X3D::SFTime*> (field));

		case X3DConstants::SFVec2d:
			return SFVec2d::create (cx, static_cast <X3D::SFVec2d*> (field));

		case X3DConstants::SFVec2f:
			return SFVec2f::create (cx, static_cast <X3D::SFVec2f*> (field));

		case X3DConstants::SFVec3d:
			return SFVec3d::create (cx, static_cast <X3D::SFVec3d*> (field));

		case X3DConstants::SFVec3f:
			return SFVec3f::create (cx, static_cast <X3D::SFVec3f*> (field));

		case X3DConstants::SFVec4d:
			return SFVec4d::create (cx, static_cast <X3D::SFVec4d*> (field));

		case X3DConstants::SFVec4f:
			return SFVec4f::create (cx, static_cast <X3D::SFVec4f*> (field));

		case X3DConstants::MFBool:
			return MFBool::create (cx, static_cast <X3D::MFBool*> (field));

		case X3DConstants::MFColor:
			return MFColor::create (cx, static_cast <X3D::MFColor*> (field));

		case X3DConstants::MFColorRGBA:
			return MFColorRGBA::create (cx, static_cast <X3D::MFColorRGBA*> (field));

		case X3DConstants::MFDouble:
			return MFDouble::create (cx, static_cast <X3D::MFDouble*> (field));

		case X3DConstants::MFFloat:
			return MFFloat::create (cx, static_cast <X3D::MFFloat*> (field));

		case X3DConstants::MFImage:
			return MFImage::create (cx, static_cast <X3D::MFImage*> (field));

		case X3DConstants::MFInt32:
			return MFInt32::create (cx, static_cast <X3D::MFInt32*> (field));

		case X3DConstants::MFMatrix3d:
			return MFMatrix3d::create (cx, static_cast <X3D::MFMatrix3d*> (field));

		case X3DConstants::MFMatrix3f:
			return MFMatrix3f::create (cx, static_cast <X3D::MFMatrix3f*> (field));

		case X3DConstants::MFMatrix4d:
			return MFMatrix4d::create (cx, static_cast <X3D::MFMatrix4d*> (field));

		case X3DConstants::MFMatrix4f:
			return MFMatrix4f::create (cx, static_cast <X3D::MFMatrix4f*> (field));

		case X3DConstants::MFNode:
			return MFNode::create (cx, static_cast <X3D::MFNode*> (field));

		case X3DConstants::MFRotation:
			return MFRotation::create (cx, static_cast <X3D::MFRotation*> (field));

		case X3DConstants::MFString:
			return MFString::create (cx, static_cast <X3D::MFString*> (field));

		case X3DConstants::MFTime:
			return MFTime::create (cx, static_cast <X3D::MFTime*> (field));

		case X3DConstants::MFVec2d:
			return MFVec2d::create (cx, static_cast <X3D::MFVec2d*> (field));

		case X3DConstants::MFVec2f:
			return MFVec2f::create (cx, static_cast <X3D::MFVec2f*> (field));

		case X3DConstants::MFVec3d:
			return MFVec3d::create (cx, static_cast <X3D::MFVec3d*> (field));

		case X3DConstants::MFVec3f:
			return MFVec3f::create (cx, static_cast <X3D::MFVec3f*> (field));

		case X3DConstants::MFVec4d:
			return MFVec4d::create (cx, static_cast <X3D::MFVec4d*> (field));

		case X3DConstants::MFVec4f:
			return MFVec4f::create (cx, static_cast <X3D::MFVec4f*> (field));
	}

	return JS::UndefinedValue ();
}

} // spidermonkey
} // X3D
} // titania
