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

#include "jsfield.h"

#include "jsFields.h"
#include "jsString.h"
#include "jsError.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSBool
setValue (JSContext* const cx, X3DFieldDefinition* const field, jsval* const vp)
{
	try
	{
		switch (field -> getType ())
		{
			case X3DConstants::SFBool:
			{
				*static_cast <X3D::SFBool*> (field) = getArgument <bool> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFColor:
			{
				*static_cast <X3D::SFColor*> (field) = *getArgument <jsSFColor> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFColorRGBA:
			{
				*static_cast <X3D::SFColorRGBA*> (field) = *getArgument <jsSFColorRGBA> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFDouble:
			{
				*static_cast <X3D::SFDouble*> (field) = getArgument <double> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFFloat:
			{
				*static_cast <X3D::SFFloat*> (field) = getArgument <double> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFInt32:
			{
				*static_cast <X3D::SFInt32*> (field) = getArgument <int32_t> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFImage:
			{
				*static_cast <X3D::SFImage*> (field) = *getArgument <jsSFImage> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFMatrix3d:
			{
				*static_cast <X3D::SFMatrix3d*> (field) = *getArgument <jsSFMatrix3d> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFMatrix3f:
			{
				*static_cast <X3D::SFMatrix3f*> (field) = *getArgument <jsSFMatrix3f> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFMatrix4d:
			{
				*static_cast <X3D::SFMatrix4d*> (field) = *getArgument <jsSFMatrix4d> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFMatrix4f:
			{
				*static_cast <X3D::SFMatrix4f*> (field) = *getArgument <jsSFMatrix4f> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFNode:
			{
				try
				{
					*static_cast <X3D::SFNode*> (field) = *getArgument <jsSFNode> (cx, vp, 0);
				}
				catch (const std::domain_error &)
				{
					*static_cast <X3D::SFNode*> (field) = nullptr;
				}

				break;
			}
			case X3DConstants::SFRotation:
			{
				*static_cast <X3D::SFRotation*> (field) = *getArgument <jsSFRotation> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFString:
			{
				*static_cast <X3D::SFString*> (field) = to_string (cx, *vp);
				break;
			}
			case X3DConstants::SFTime:
			{
				*static_cast <X3D::SFTime*> (field) = getArgument <double> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFVec2d:
			{
				*static_cast <X3D::SFVec2d*> (field) = *getArgument <jsSFVec2d> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFVec2f:
			{
				*static_cast <X3D::SFVec2f*> (field) = *getArgument <jsSFVec2f> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFVec3d:
			{
				*static_cast <X3D::SFVec3d*> (field) = *getArgument <jsSFVec3d> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFVec3f:
			{
				*static_cast <X3D::SFVec3f*> (field) = *getArgument <jsSFVec3f> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFVec4d:
			{
				*static_cast <X3D::SFVec4d*> (field) = *getArgument <jsSFVec4d> (cx, vp, 0);
				break;
			}
			case X3DConstants::SFVec4f:
			{
				*static_cast <X3D::SFVec4f*> (field) = *getArgument <jsSFVec4f> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFBool:
			{
				*static_cast <X3D::MFBool*> (field) = *getArgument <jsMFBool> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFColor:
			{
				*static_cast <X3D::MFColor*> (field) = *getArgument <jsMFColor> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFColorRGBA:
			{
				*static_cast <X3D::MFColorRGBA*> (field) = *getArgument <jsMFColorRGBA> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFDouble:
			{
				*static_cast <X3D::MFDouble*> (field) = *getArgument <jsMFDouble> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFFloat:
			{
				*static_cast <X3D::MFFloat*> (field) = *getArgument <jsMFFloat> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFImage:
			{
				*static_cast <X3D::MFImage*> (field) = *getArgument <jsMFImage> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFInt32:
			{
				*static_cast <X3D::MFInt32*> (field) = *getArgument <jsMFInt32> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFMatrix3d:
			{
				*static_cast <X3D::MFMatrix3d*> (field) = *getArgument <jsMFMatrix3d> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFMatrix3f:
			{
				*static_cast <X3D::MFMatrix3f*> (field) = *getArgument <jsMFMatrix3f> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFMatrix4d:
			{
				*static_cast <X3D::MFMatrix4d*> (field) = *getArgument <jsMFMatrix4d> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFMatrix4f:
			{
				*static_cast <X3D::MFMatrix4f*> (field) = *getArgument <jsMFMatrix4f> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFNode:
			{
				*static_cast <X3D::MFNode*> (field) = *getArgument <jsMFNode> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFRotation:
			{
				*static_cast <X3D::MFRotation*> (field) = *getArgument <jsMFRotation> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFString:
			{
				*static_cast <X3D::MFString*> (field) = *getArgument <jsMFString> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFTime:
			{
				*static_cast <X3D::MFTime*> (field) = *getArgument <jsMFTime> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFVec2d:
			{
				*static_cast <X3D::MFVec2d*> (field) = *getArgument <jsMFVec2d> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFVec2f:
			{
				*static_cast <X3D::MFVec2f*> (field) = *getArgument <jsMFVec2f> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFVec3d:
			{
				*static_cast <X3D::MFVec3d*> (field) = *getArgument <jsMFVec3d> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFVec3f:
			{
				*static_cast <X3D::MFVec3f*> (field) = *getArgument <jsMFVec3f> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFVec4d:
			{
				*static_cast <X3D::MFVec4d*> (field) = *getArgument <jsMFVec4d> (cx, vp, 0);
				break;
			}
			case X3DConstants::MFVec4f:
			{
				*static_cast <X3D::MFVec4f*> (field) = *getArgument <jsMFVec4f> (cx, vp, 0);
				break;
			}
		}

		*vp = JSVAL_VOID;
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "RuntimeError: %s.", error .what ());
	}
}

JSBool
getValue (JSContext* const cx, X3DFieldDefinition* const field, jsval* const vp)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
			return jsSFBool::create (cx, static_cast <X3D::SFBool*> (field), vp);

		case X3DConstants::SFColor:
			return jsSFColor::create (cx, static_cast <X3D::SFColor*> (field), vp);

		case X3DConstants::SFColorRGBA:
			return jsSFColorRGBA::create (cx, static_cast <X3D::SFColorRGBA*> (field), vp);

		case X3DConstants::SFDouble:
			return jsSFDouble::create (cx, static_cast <X3D::SFDouble*> (field), vp);

		case X3DConstants::SFFloat:
			return jsSFFloat::create (cx, static_cast <X3D::SFFloat*> (field), vp);

		case X3DConstants::SFInt32:
			return jsSFInt32::create (cx, static_cast <X3D::SFInt32*> (field), vp);

		case X3DConstants::SFImage:
			return jsSFImage::create (cx, static_cast <X3D::SFImage*> (field), vp);

		case X3DConstants::SFMatrix3d:
			return jsSFMatrix3d::create (cx, static_cast <X3D::SFMatrix3d*> (field), vp);

		case X3DConstants::SFMatrix3f:
			return jsSFMatrix3f::create (cx, static_cast <X3D::SFMatrix3f*> (field), vp);

		case X3DConstants::SFMatrix4d:
			return jsSFMatrix4d::create (cx, static_cast <X3D::SFMatrix4d*> (field), vp);

		case X3DConstants::SFMatrix4f:
			return jsSFMatrix4f::create (cx, static_cast <X3D::SFMatrix4f*> (field), vp);

		case X3DConstants::SFNode:
			return jsSFNode::create (cx, static_cast <X3D::SFNode*> (field), vp);

		case X3DConstants::SFRotation:
			return jsSFRotation::create (cx, static_cast <X3D::SFRotation*> (field), vp);

		case X3DConstants::SFString:
			return jsSFString::create (cx, static_cast <X3D::SFString*> (field), vp);

		case X3DConstants::SFTime:
			return jsSFTime::create (cx, static_cast <X3D::SFTime*> (field), vp);

		case X3DConstants::SFVec2d:
			return jsSFVec2d::create (cx, static_cast <X3D::SFVec2d*> (field), vp);

		case X3DConstants::SFVec2f:
			return jsSFVec2f::create (cx, static_cast <X3D::SFVec2f*> (field), vp);

		case X3DConstants::SFVec3d:
			return jsSFVec3d::create (cx, static_cast <X3D::SFVec3d*> (field), vp);

		case X3DConstants::SFVec3f:
			return jsSFVec3f::create (cx, static_cast <X3D::SFVec3f*> (field), vp);

		case X3DConstants::SFVec4d:
			return jsSFVec4d::create (cx, static_cast <X3D::SFVec4d*> (field), vp);

		case X3DConstants::SFVec4f:
			return jsSFVec4f::create (cx, static_cast <X3D::SFVec4f*> (field), vp);

		case X3DConstants::MFBool:
			return jsMFBool::create (cx, static_cast <X3D::MFBool*> (field), vp);

		case X3DConstants::MFColor:
			return jsMFColor::create (cx, static_cast <X3D::MFColor*> (field), vp);

		case X3DConstants::MFColorRGBA:
			return jsMFColorRGBA::create (cx, static_cast <X3D::MFColorRGBA*> (field), vp);

		case X3DConstants::MFDouble:
			return jsMFDouble::create (cx, static_cast <X3D::MFDouble*> (field), vp);

		case X3DConstants::MFFloat:
			return jsMFFloat::create (cx, static_cast <X3D::MFFloat*> (field), vp);

		case X3DConstants::MFImage:
			return jsMFImage::create (cx, static_cast <X3D::MFImage*> (field), vp);

		case X3DConstants::MFInt32:
			return jsMFInt32::create (cx, static_cast <X3D::MFInt32*> (field), vp);

		case X3DConstants::MFMatrix3d:
			return jsMFMatrix3d::create (cx, static_cast <X3D::MFMatrix3d*> (field), vp);

		case X3DConstants::MFMatrix3f:
			return jsMFMatrix3f::create (cx, static_cast <X3D::MFMatrix3f*> (field), vp);

		case X3DConstants::MFMatrix4d:
			return jsMFMatrix4d::create (cx, static_cast <X3D::MFMatrix4d*> (field), vp);

		case X3DConstants::MFMatrix4f:
			return jsMFMatrix4f::create (cx, static_cast <X3D::MFMatrix4f*> (field), vp);

		case X3DConstants::MFNode:
			return jsMFNode::create (cx, static_cast <X3D::MFNode*> (field), vp);

		case X3DConstants::MFRotation:
			return jsMFRotation::create (cx, static_cast <X3D::MFRotation*> (field), vp);

		case X3DConstants::MFString:
			return jsMFString::create (cx, static_cast <X3D::MFString*> (field), vp);

		case X3DConstants::MFTime:
			return jsMFTime::create (cx, static_cast <X3D::MFTime*> (field), vp);

		case X3DConstants::MFVec2d:
			return jsMFVec2d::create (cx, static_cast <X3D::MFVec2d*> (field), vp);

		case X3DConstants::MFVec2f:
			return jsMFVec2f::create (cx, static_cast <X3D::MFVec2f*> (field), vp);

		case X3DConstants::MFVec3d:
			return jsMFVec3d::create (cx, static_cast <X3D::MFVec3d*> (field), vp);

		case X3DConstants::MFVec3f:
			return jsMFVec3f::create (cx, static_cast <X3D::MFVec3f*> (field), vp);

		case X3DConstants::MFVec4d:
			return jsMFVec4d::create (cx, static_cast <X3D::MFVec4d*> (field), vp);

		case X3DConstants::MFVec4f:
			return jsMFVec4f::create (cx, static_cast <X3D::MFVec4f*> (field), vp);
	}

	return true;
}

} // MozillaSpiderMonkey
} // X3D
} // titania
