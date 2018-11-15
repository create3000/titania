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

#include "Arguments.h"
#include "Fields.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

///  throws pb::pbError
void
setValue (X3DFieldDefinition* const field, const pb::var & value)
{
	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		{
			*static_cast <X3D::SFBool*> (field) = get1Argument <bool> (value);
			break;
		}
		case X3D::X3DConstants::SFColor:
		{
			*static_cast <X3D::SFColor*> (field) = *get1Argument <SFColor> (value);
			break;
		}
		case X3D::X3DConstants::SFColorRGBA:
		{
			*static_cast <X3D::SFColorRGBA*> (field) = *get1Argument <SFColorRGBA> (value);
			break;
		}
		case X3D::X3DConstants::SFDouble:
		{
			*static_cast <X3D::SFDouble*> (field) = get1Argument <double> (value);
			break;
		}
		case X3D::X3DConstants::SFFloat:
		{
			*static_cast <X3D::SFFloat*> (field) = get1Argument <float> (value);
			break;
		}
		case X3D::X3DConstants::SFImage:
		{
			*static_cast <X3D::SFImage*> (field) = *get1Argument <SFImage> (value);
			break;
		}
		case X3D::X3DConstants::SFInt32:
		{
			*static_cast <X3D::SFInt32*> (field) = get1Argument <int32_t> (value);
			break;
		}
		case X3D::X3DConstants::SFMatrix3d:
		{
			*static_cast <X3D::SFMatrix3d*> (field) = *get1Argument <SFMatrix3d> (value);
			break;
		}
		case X3D::X3DConstants::SFMatrix3f:
		{
			*static_cast <X3D::SFMatrix3f*> (field) = *get1Argument <SFMatrix3f> (value);
			break;
		}
		case X3D::X3DConstants::SFMatrix4d:
		{
			*static_cast <X3D::SFMatrix4d*> (field) = *get1Argument <SFMatrix4d> (value);
			break;
		}
		case X3D::X3DConstants::SFMatrix4f:
		{
			*static_cast <X3D::SFMatrix4f*> (field) = *get1Argument <SFMatrix4f> (value);
			break;
		}
		case X3D::X3DConstants::SFNode:
		{
			if (value .isNull ())
				*static_cast <X3D::SFNode*> (field) = nullptr;
			else
				*static_cast <X3D::SFNode*> (field) = *get1Argument <SFNode> (value);

			break;
		}
		case X3D::X3DConstants::SFRotation:
		{
			*static_cast <X3D::SFRotation*> (field) = *get1Argument <SFRotation> (value);
			break;
		}
		case X3D::X3DConstants::SFString:
		{
			*static_cast <X3D::SFString*> (field) = get1Argument <X3D::String> (value);
			break;
		}
		case X3D::X3DConstants::SFTime:
		{
			*static_cast <X3D::SFTime*> (field) = get1Argument <double> (value);
			break;
		}
		case X3D::X3DConstants::SFVec2d:
		{
			*static_cast <X3D::SFVec2d*> (field) = *get1Argument <SFVec2d> (value);
			break;
		}
		case X3D::X3DConstants::SFVec2f:
		{
			*static_cast <X3D::SFVec2f*> (field) = *get1Argument <SFVec2f> (value);
			break;
		}
		case X3D::X3DConstants::SFVec3d:
		{
			*static_cast <X3D::SFVec3d*> (field) = *get1Argument <SFVec3d> (value);
			break;
		}
		case X3D::X3DConstants::SFVec3f:
		{
			*static_cast <X3D::SFVec3f*> (field) = *get1Argument <SFVec3f> (value);
			break;
		}
		case X3D::X3DConstants::SFVec4d:
		{
			*static_cast <X3D::SFVec4d*> (field) = *get1Argument <SFVec4d> (value);
			break;
		}
		case X3D::X3DConstants::SFVec4f:
		{
			*static_cast <X3D::SFVec4f*> (field) = *get1Argument <SFVec4f> (value);
			break;
		}
		case X3D::X3DConstants::MFBool:
		{
			*static_cast <X3D::MFBool*> (field) = *get1Argument <MFBool> (value);
			break;
		}
		case X3D::X3DConstants::MFColor:
		{
			*static_cast <X3D::MFColor*> (field) = *get1Argument <MFColor> (value);
			break;
		}
		case X3D::X3DConstants::MFColorRGBA:
		{
			*static_cast <X3D::MFColorRGBA*> (field) = *get1Argument <MFColorRGBA> (value);
			break;
		}
		case X3D::X3DConstants::MFDouble:
		{
			*static_cast <X3D::MFDouble*> (field) = *get1Argument <MFDouble> (value);
			break;
		}
		case X3D::X3DConstants::MFFloat:
		{
			*static_cast <X3D::MFFloat*> (field) = *get1Argument <MFFloat> (value);
			break;
		}
		case X3D::X3DConstants::MFImage:
		{
			*static_cast <X3D::MFImage*> (field) = *get1Argument <MFImage> (value);
			break;
		}
		case X3D::X3DConstants::MFInt32:
		{
			*static_cast <X3D::MFInt32*> (field) = *get1Argument <MFInt32> (value);
			break;
		}
		case X3D::X3DConstants::MFMatrix3d:
		{
			*static_cast <X3D::MFMatrix3d*> (field) = *get1Argument <MFMatrix3d> (value);
			break;
		}
		case X3D::X3DConstants::MFMatrix3f:
		{
			*static_cast <X3D::MFMatrix3f*> (field) = *get1Argument <MFMatrix3f> (value);
			break;
		}
		case X3D::X3DConstants::MFMatrix4d:
		{
			*static_cast <X3D::MFMatrix4d*> (field) = *get1Argument <MFMatrix4d> (value);
			break;
		}
		case X3D::X3DConstants::MFMatrix4f:
		{
			*static_cast <X3D::MFMatrix4f*> (field) = *get1Argument <MFMatrix4f> (value);
			break;
		}
		case X3D::X3DConstants::MFNode:
		{
			*static_cast <X3D::MFNode*> (field) = *get1Argument <MFNode> (value);
			break;
		}
		case X3D::X3DConstants::MFRotation:
		{
			*static_cast <X3D::MFRotation*> (field) = *get1Argument <MFRotation> (value);
			break;
		}
		case X3D::X3DConstants::MFString:
		{
			*static_cast <X3D::MFString*> (field) = *get1Argument <MFString> (value);
			break;
		}
		case X3D::X3DConstants::MFTime:
		{
			*static_cast <X3D::MFTime*> (field) = *get1Argument <MFTime> (value);
			break;
		}
		case X3D::X3DConstants::MFVec2d:
		{
			*static_cast <X3D::MFVec2d*> (field) = *get1Argument <MFVec2d> (value);
			break;
		}
		case X3D::X3DConstants::MFVec2f:
		{
			*static_cast <X3D::MFVec2f*> (field) = *get1Argument <MFVec2f> (value);
			break;
		}
		case X3D::X3DConstants::MFVec3d:
		{
			*static_cast <X3D::MFVec3d*> (field) = *get1Argument <MFVec3d> (value);
			break;
		}
		case X3D::X3DConstants::MFVec3f:
		{
			*static_cast <X3D::MFVec3f*> (field) = *get1Argument <MFVec3f> (value);
			break;
		}
		case X3D::X3DConstants::MFVec4d:
		{
			*static_cast <X3D::MFVec4d*> (field) = *get1Argument <MFVec4d> (value);
			break;
		}
		case X3D::X3DConstants::MFVec4f:
		{
			*static_cast <X3D::MFVec4f*> (field) = *get1Argument <MFVec4f> (value);
			break;
		}
	}
}

pb::var
getValue (Context* const context, X3DFieldDefinition* const field)
{
	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
			return X3DField::get <SFBool> (context, *static_cast <X3D::SFBool*> (field));

		case X3D::X3DConstants::SFColor:
			return X3DField::get <SFColor> (context, *static_cast <X3D::SFColor*> (field));

		case X3D::X3DConstants::SFColorRGBA:
			return X3DField::get <SFColorRGBA> (context, *static_cast <X3D::SFColorRGBA*> (field));

		case X3D::X3DConstants::SFDouble:
			return X3DField::get <SFDouble> (context, *static_cast <X3D::SFDouble*> (field));

		case X3D::X3DConstants::SFFloat:
			return X3DField::get <SFFloat> (context, *static_cast <X3D::SFFloat*> (field));

		case X3D::X3DConstants::SFImage:
			return X3DField::get <SFImage> (context, *static_cast <X3D::SFImage*> (field));

		case X3D::X3DConstants::SFInt32:
			return X3DField::get <SFInt32> (context, *static_cast <X3D::SFInt32*> (field));

		case X3D::X3DConstants::SFMatrix3d:
			return X3DField::get <SFMatrix3d> (context, *static_cast <X3D::SFMatrix3d*> (field));

		case X3D::X3DConstants::SFMatrix3f:
			return X3DField::get <SFMatrix3f> (context, *static_cast <X3D::SFMatrix3f*> (field));

		case X3D::X3DConstants::SFMatrix4d:
			return X3DField::get <SFMatrix4d> (context, *static_cast <X3D::SFMatrix4d*> (field));

		case X3D::X3DConstants::SFMatrix4f:
			return X3DField::get <SFMatrix4f> (context, *static_cast <X3D::SFMatrix4f*> (field));

		case X3D::X3DConstants::SFNode:
			return X3DField::get <SFNode> (context, *static_cast <X3D::SFNode*> (field));

		case X3D::X3DConstants::SFRotation:
			return X3DField::get <SFRotation> (context, *static_cast <X3D::SFRotation*> (field));

		case X3D::X3DConstants::SFString:
			return X3DField::get <SFString> (context, *static_cast <X3D::SFString*> (field));

		case X3D::X3DConstants::SFTime:
			return X3DField::get <SFTime> (context, *static_cast <X3D::SFTime*> (field));

		case X3D::X3DConstants::SFVec2d:
			return X3DField::get <SFVec2d> (context, *static_cast <X3D::SFVec2d*> (field));

		case X3D::X3DConstants::SFVec2f:
			return X3DField::get <SFVec2f> (context, *static_cast <X3D::SFVec2f*> (field));

		case X3D::X3DConstants::SFVec3d:
			return X3DField::get <SFVec3d> (context, *static_cast <X3D::SFVec3d*> (field));

		case X3D::X3DConstants::SFVec3f:
			return X3DField::get <SFVec3f> (context, *static_cast <X3D::SFVec3f*> (field));

		case X3D::X3DConstants::SFVec4d:
			return X3DField::get <SFVec4d> (context, *static_cast <X3D::SFVec4d*> (field));

		case X3D::X3DConstants::SFVec4f:
			return X3DField::get <SFVec4f> (context, *static_cast <X3D::SFVec4f*> (field));

		case X3D::X3DConstants::MFBool:
			return X3DField::get <MFBool> (context, *static_cast <X3D::MFBool*> (field));

		case X3D::X3DConstants::MFColor:
			return X3DField::get <MFColor> (context, *static_cast <X3D::MFColor*> (field));

		case X3D::X3DConstants::MFColorRGBA:
			return X3DField::get <MFColorRGBA> (context, *static_cast <X3D::MFColorRGBA*> (field));

		case X3D::X3DConstants::MFDouble:
			return X3DField::get <MFDouble> (context, *static_cast <X3D::MFDouble*> (field));

		case X3D::X3DConstants::MFFloat:
			return X3DField::get <MFFloat> (context, *static_cast <X3D::MFFloat*> (field));

		case X3D::X3DConstants::MFImage:
			return X3DField::get <MFImage> (context, *static_cast <X3D::MFImage*> (field));

		case X3D::X3DConstants::MFInt32:
			return X3DField::get <MFInt32> (context, *static_cast <X3D::MFInt32*> (field));

		case X3D::X3DConstants::MFMatrix3d:
			return X3DField::get <MFMatrix3d> (context, *static_cast <X3D::MFMatrix3d*> (field));

		case X3D::X3DConstants::MFMatrix3f:
			return X3DField::get <MFMatrix3f> (context, *static_cast <X3D::MFMatrix3f*> (field));

		case X3D::X3DConstants::MFMatrix4d:
			return X3DField::get <MFMatrix4d> (context, *static_cast <X3D::MFMatrix4d*> (field));

		case X3D::X3DConstants::MFMatrix4f:
			return X3DField::get <MFMatrix4f> (context, *static_cast <X3D::MFMatrix4f*> (field));

		case X3D::X3DConstants::MFNode:
			return X3DField::get <MFNode> (context, *static_cast <X3D::MFNode*> (field));

		case X3D::X3DConstants::MFRotation:
			return X3DField::get <MFRotation> (context, *static_cast <X3D::MFRotation*> (field));

		case X3D::X3DConstants::MFString:
			return X3DField::get <MFString> (context, *static_cast <X3D::MFString*> (field));

		case X3D::X3DConstants::MFTime:
			return X3DField::get <MFTime> (context, *static_cast <X3D::MFTime*> (field));

		case X3D::X3DConstants::MFVec2d:
			return X3DField::get <MFVec2d> (context, *static_cast <X3D::MFVec2d*> (field));

		case X3D::X3DConstants::MFVec2f:
			return X3DField::get <MFVec2f> (context, *static_cast <X3D::MFVec2f*> (field));

		case X3D::X3DConstants::MFVec3d:
			return X3DField::get <MFVec3d> (context, *static_cast <X3D::MFVec3d*> (field));

		case X3D::X3DConstants::MFVec3f:
			return X3DField::get <MFVec3f> (context, *static_cast <X3D::MFVec3f*> (field));

		case X3D::X3DConstants::MFVec4d:
			return X3DField::get <MFVec4d> (context, *static_cast <X3D::MFVec4d*> (field));

		case X3D::X3DConstants::MFVec4f:
			return X3DField::get <MFVec4f> (context, *static_cast <X3D::MFVec4f*> (field));
	}

	return pb::undefined;
}

} // peaseblossom
} // X3D
} // titania
