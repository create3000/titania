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

#include "value.h"

#include "Fields.h"
#include "String.h"
#include "Error.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

JSBool
setValue (JSContext* const cx, X3D::X3DFieldDefinition* const field, jsval* const vp)
{
	try
	{
		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFBool:
			{
				*static_cast <X3D::SFBool*> (field) = getArgument <bool> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFColor:
			{
				*static_cast <X3D::SFColor*> (field) = *getArgument <SFColor> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFColorRGBA:
			{
				*static_cast <X3D::SFColorRGBA*> (field) = *getArgument <SFColorRGBA> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFDouble:
			{
				*static_cast <X3D::SFDouble*> (field) = getArgument <double> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFFloat:
			{
				*static_cast <X3D::SFFloat*> (field) = getArgument <float> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFInt32:
			{
				*static_cast <X3D::SFInt32*> (field) = getArgument <int32_t> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFImage:
			{
				*static_cast <X3D::SFImage*> (field) = *getArgument <SFImage> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFMatrix3d:
			{
				*static_cast <X3D::SFMatrix3d*> (field) = *getArgument <SFMatrix3d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFMatrix3f:
			{
				*static_cast <X3D::SFMatrix3f*> (field) = *getArgument <SFMatrix3f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFMatrix4d:
			{
				*static_cast <X3D::SFMatrix4d*> (field) = *getArgument <SFMatrix4d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFMatrix4f:
			{
				*static_cast <X3D::SFMatrix4f*> (field) = *getArgument <SFMatrix4f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFNode:
			{
				try
				{
					*static_cast <X3D::SFNode*> (field) = *getArgument <SFNode> (cx, vp, 0);
				}
				catch (const std::domain_error &)
				{
					*static_cast <X3D::SFNode*> (field) = nullptr;
				}

				break;
			}
			case X3D::X3DConstants::SFRotation:
			{
				*static_cast <X3D::SFRotation*> (field) = *getArgument <SFRotation> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFString:
			{
				*static_cast <X3D::SFString*> (field) = to_string (cx, *vp);
				break;
			}
			case X3D::X3DConstants::SFTime:
			{
				*static_cast <X3D::SFTime*> (field) = getArgument <double> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFVec2d:
			{
				*static_cast <X3D::SFVec2d*> (field) = *getArgument <SFVec2d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFVec2f:
			{
				*static_cast <X3D::SFVec2f*> (field) = *getArgument <SFVec2f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFVec3d:
			{
				*static_cast <X3D::SFVec3d*> (field) = *getArgument <SFVec3d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFVec3f:
			{
				*static_cast <X3D::SFVec3f*> (field) = *getArgument <SFVec3f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFVec4d:
			{
				*static_cast <X3D::SFVec4d*> (field) = *getArgument <SFVec4d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::SFVec4f:
			{
				*static_cast <X3D::SFVec4f*> (field) = *getArgument <SFVec4f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFBool:
			{
				*static_cast <X3D::MFBool*> (field) = *getArgument <MFBool> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFColor:
			{
				*static_cast <X3D::MFColor*> (field) = *getArgument <MFColor> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFColorRGBA:
			{
				*static_cast <X3D::MFColorRGBA*> (field) = *getArgument <MFColorRGBA> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFDouble:
			{
				*static_cast <X3D::MFDouble*> (field) = *getArgument <MFDouble> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFFloat:
			{
				*static_cast <X3D::MFFloat*> (field) = *getArgument <MFFloat> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFImage:
			{
				*static_cast <X3D::MFImage*> (field) = *getArgument <MFImage> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFInt32:
			{
				*static_cast <X3D::MFInt32*> (field) = *getArgument <MFInt32> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFMatrix3d:
			{
				*static_cast <X3D::MFMatrix3d*> (field) = *getArgument <MFMatrix3d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFMatrix3f:
			{
				*static_cast <X3D::MFMatrix3f*> (field) = *getArgument <MFMatrix3f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFMatrix4d:
			{
				*static_cast <X3D::MFMatrix4d*> (field) = *getArgument <MFMatrix4d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFMatrix4f:
			{
				*static_cast <X3D::MFMatrix4f*> (field) = *getArgument <MFMatrix4f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				*static_cast <X3D::MFNode*> (field) = *getArgument <MFNode> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFRotation:
			{
				*static_cast <X3D::MFRotation*> (field) = *getArgument <MFRotation> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFString:
			{
				*static_cast <X3D::MFString*> (field) = *getArgument <MFString> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFTime:
			{
				*static_cast <X3D::MFTime*> (field) = *getArgument <MFTime> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFVec2d:
			{
				*static_cast <X3D::MFVec2d*> (field) = *getArgument <MFVec2d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFVec2f:
			{
				*static_cast <X3D::MFVec2f*> (field) = *getArgument <MFVec2f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFVec3d:
			{
				*static_cast <X3D::MFVec3d*> (field) = *getArgument <MFVec3d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFVec3f:
			{
				*static_cast <X3D::MFVec3f*> (field) = *getArgument <MFVec3f> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFVec4d:
			{
				*static_cast <X3D::MFVec4d*> (field) = *getArgument <MFVec4d> (cx, vp, 0);
				break;
			}
			case X3D::X3DConstants::MFVec4f:
			{
				*static_cast <X3D::MFVec4f*> (field) = *getArgument <MFVec4f> (cx, vp, 0);
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
getValue (JSContext* const cx, X3D::X3DFieldDefinition* const field, jsval* const vp)
{
	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
			return X3DField::get <SFBool> (cx, *static_cast <X3D::SFBool*> (field), vp);

		case X3D::X3DConstants::SFColor:
			return X3DField::get <SFColor> (cx, *static_cast <X3D::SFColor*> (field), vp);

		case X3D::X3DConstants::SFColorRGBA:
			return X3DField::get <SFColorRGBA> (cx, *static_cast <X3D::SFColorRGBA*> (field), vp);

		case X3D::X3DConstants::SFDouble:
			return X3DField::get <SFDouble> (cx, *static_cast <X3D::SFDouble*> (field), vp);

		case X3D::X3DConstants::SFFloat:
			return X3DField::get <SFFloat> (cx, *static_cast <X3D::SFFloat*> (field), vp);

		case X3D::X3DConstants::SFInt32:
			return X3DField::get <SFInt32> (cx, *static_cast <X3D::SFInt32*> (field), vp);

		case X3D::X3DConstants::SFImage:
			return X3DField::get <SFImage> (cx, *static_cast <X3D::SFImage*> (field), vp);

		case X3D::X3DConstants::SFMatrix3d:
			return X3DField::get <SFMatrix3d> (cx, *static_cast <X3D::SFMatrix3d*> (field), vp);

		case X3D::X3DConstants::SFMatrix3f:
			return X3DField::get <SFMatrix3f> (cx, *static_cast <X3D::SFMatrix3f*> (field), vp);

		case X3D::X3DConstants::SFMatrix4d:
			return X3DField::get <SFMatrix4d> (cx, *static_cast <X3D::SFMatrix4d*> (field), vp);

		case X3D::X3DConstants::SFMatrix4f:
			return X3DField::get <SFMatrix4f> (cx, *static_cast <X3D::SFMatrix4f*> (field), vp);

		case X3D::X3DConstants::SFNode:
			return X3DField::get <SFNode> (cx, *static_cast <X3D::SFNode*> (field), vp);

		case X3D::X3DConstants::SFRotation:
			return X3DField::get <SFRotation> (cx, *static_cast <X3D::SFRotation*> (field), vp);

		case X3D::X3DConstants::SFString:
			return X3DField::get <SFString> (cx, *static_cast <X3D::SFString*> (field), vp);

		case X3D::X3DConstants::SFTime:
			return X3DField::get <SFTime> (cx, *static_cast <X3D::SFTime*> (field), vp);

		case X3D::X3DConstants::SFVec2d:
			return X3DField::get <SFVec2d> (cx, *static_cast <X3D::SFVec2d*> (field), vp);

		case X3D::X3DConstants::SFVec2f:
			return X3DField::get <SFVec2f> (cx, *static_cast <X3D::SFVec2f*> (field), vp);

		case X3D::X3DConstants::SFVec3d:
			return X3DField::get <SFVec3d> (cx, *static_cast <X3D::SFVec3d*> (field), vp);

		case X3D::X3DConstants::SFVec3f:
			return X3DField::get <SFVec3f> (cx, *static_cast <X3D::SFVec3f*> (field), vp);

		case X3D::X3DConstants::SFVec4d:
			return X3DField::get <SFVec4d> (cx, *static_cast <X3D::SFVec4d*> (field), vp);

		case X3D::X3DConstants::SFVec4f:
			return X3DField::get <SFVec4f> (cx, *static_cast <X3D::SFVec4f*> (field), vp);

		case X3D::X3DConstants::MFBool:
			return X3DField::get <MFBool> (cx, *static_cast <X3D::MFBool*> (field), vp);

		case X3D::X3DConstants::MFColor:
			return X3DField::get <MFColor> (cx, *static_cast <X3D::MFColor*> (field), vp);

		case X3D::X3DConstants::MFColorRGBA:
			return X3DField::get <MFColorRGBA> (cx, *static_cast <X3D::MFColorRGBA*> (field), vp);

		case X3D::X3DConstants::MFDouble:
			return X3DField::get <MFDouble> (cx, *static_cast <X3D::MFDouble*> (field), vp);

		case X3D::X3DConstants::MFFloat:
			return X3DField::get <MFFloat> (cx, *static_cast <X3D::MFFloat*> (field), vp);

		case X3D::X3DConstants::MFImage:
			return X3DField::get <MFImage> (cx, *static_cast <X3D::MFImage*> (field), vp);

		case X3D::X3DConstants::MFInt32:
			return X3DField::get <MFInt32> (cx, *static_cast <X3D::MFInt32*> (field), vp);

		case X3D::X3DConstants::MFMatrix3d:
			return X3DField::get <MFMatrix3d> (cx, *static_cast <X3D::MFMatrix3d*> (field), vp);

		case X3D::X3DConstants::MFMatrix3f:
			return X3DField::get <MFMatrix3f> (cx, *static_cast <X3D::MFMatrix3f*> (field), vp);

		case X3D::X3DConstants::MFMatrix4d:
			return X3DField::get <MFMatrix4d> (cx, *static_cast <X3D::MFMatrix4d*> (field), vp);

		case X3D::X3DConstants::MFMatrix4f:
			return X3DField::get <MFMatrix4f> (cx, *static_cast <X3D::MFMatrix4f*> (field), vp);

		case X3D::X3DConstants::MFNode:
			return X3DField::get <MFNode> (cx, *static_cast <X3D::MFNode*> (field), vp);

		case X3D::X3DConstants::MFRotation:
			return X3DField::get <MFRotation> (cx, *static_cast <X3D::MFRotation*> (field), vp);

		case X3D::X3DConstants::MFString:
			return X3DField::get <MFString> (cx, *static_cast <X3D::MFString*> (field), vp);

		case X3D::X3DConstants::MFTime:
			return X3DField::get <MFTime> (cx, *static_cast <X3D::MFTime*> (field), vp);

		case X3D::X3DConstants::MFVec2d:
			return X3DField::get <MFVec2d> (cx, *static_cast <X3D::MFVec2d*> (field), vp);

		case X3D::X3DConstants::MFVec2f:
			return X3DField::get <MFVec2f> (cx, *static_cast <X3D::MFVec2f*> (field), vp);

		case X3D::X3DConstants::MFVec3d:
			return X3DField::get <MFVec3d> (cx, *static_cast <X3D::MFVec3d*> (field), vp);

		case X3D::X3DConstants::MFVec3f:
			return X3DField::get <MFVec3f> (cx, *static_cast <X3D::MFVec3f*> (field), vp);

		case X3D::X3DConstants::MFVec4d:
			return X3DField::get <MFVec4d> (cx, *static_cast <X3D::MFVec4d*> (field), vp);

		case X3D::X3DConstants::MFVec4f:
			return X3DField::get <MFVec4f> (cx, *static_cast <X3D::MFVec4f*> (field), vp);
	}

	return true;
}

} // spidermonkey
} // X3D
} // titania
