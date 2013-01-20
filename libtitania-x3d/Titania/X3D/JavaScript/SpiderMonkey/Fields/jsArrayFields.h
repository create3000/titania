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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_ARRAY_FIELDS_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_ARRAY_FIELDS_H__

#include "../../../Fields/ArrayFields.h"
#include "../jsX3DArrayField.h"

#include "jsSFColor.h"
#include "jsSFColorRGBA.h"
#include "jsSFImage.h"
#include "jsSFMatrix3.h"
#include "jsSFMatrix4.h"
#include "jsSFNode.h"
#include "jsSFRotation.h"
#include "jsSFVec2.h"
#include "jsSFVec3.h"
#include "jsSFVec4.h"
#include "jsX3DScalar.h"

namespace titania {
namespace X3D {

extern template class jsX3DArrayField <jsSFColor, MFColor>;
extern template class jsX3DArrayField <jsSFColorRGBA, MFColorRGBA>;
extern template class jsX3DArrayField <jsSFImage, MFImage>;
extern template class jsX3DArrayField <jsSFMatrix3d, MFMatrix3d>;
extern template class jsX3DArrayField <jsSFMatrix3f, MFMatrix3f>;
extern template class jsX3DArrayField <jsSFMatrix4d, MFMatrix4d>;
extern template class jsX3DArrayField <jsSFMatrix4f, MFMatrix4f>;
extern template class jsX3DArrayField <jsSFRotation, MFRotation>;
extern template class jsX3DArrayField <jsSFVec2d, MFVec2d>;
extern template class jsX3DArrayField <jsSFVec2f, MFVec2f>;
extern template class jsX3DArrayField <jsSFVec3d, MFVec3d>;
extern template class jsX3DArrayField <jsSFVec3f, MFVec3f>;
extern template class jsX3DArrayField <jsSFVec4d, MFVec4d>;
extern template class jsX3DArrayField <jsSFVec4f, MFVec4f>;

typedef jsX3DArrayField <jsSFColor, MFColor>         jsMFColor;
typedef jsX3DArrayField <jsSFColorRGBA, MFColorRGBA> jsMFColorRGBA;
typedef jsX3DArrayField <jsSFImage, MFImage>         jsMFImage;
typedef jsX3DArrayField <jsSFMatrix3d, MFMatrix3d>   jsMFMatrix3d;
typedef jsX3DArrayField <jsSFMatrix3f, MFMatrix3f>   jsMFMatrix3f;
typedef jsX3DArrayField <jsSFMatrix4d, MFMatrix4d>   jsMFMatrix4d;
typedef jsX3DArrayField <jsSFMatrix4f, MFMatrix4f>   jsMFMatrix4f;
typedef jsX3DArrayField <jsSFRotation, MFRotation>   jsMFRotation;
typedef jsX3DArrayField <jsSFVec2d, MFVec2d>         jsMFVec2d;
typedef jsX3DArrayField <jsSFVec2f, MFVec2f>         jsMFVec2f;
typedef jsX3DArrayField <jsSFVec3d, MFVec3d>         jsMFVec3d;
typedef jsX3DArrayField <jsSFVec3f, MFVec3f>         jsMFVec3f;
typedef jsX3DArrayField <jsSFVec4d, MFVec4d>         jsMFVec4d;
typedef jsX3DArrayField <jsSFVec4f, MFVec4f>         jsMFVec4f;

} // X3D
} // titania

#endif
