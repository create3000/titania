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

#ifndef __TITANIA_X3D_FIELDS_ARRAY_FIELDS_H__
#define __TITANIA_X3D_FIELDS_ARRAY_FIELDS_H__

#include "../Fields/SFColor.h"
#include "../Fields/SFColorRGBA.h"
#include "../Fields/SFImage.h"
#include "../Fields/SFMatrix3.h"
#include "../Fields/SFMatrix4.h"
#include "../Fields/SFRotation4.h"
#include "../Fields/SFVec2.h"
#include "../Fields/SFVec3.h"
#include "../Fields/SFVec4.h"

#include "../Basic/X3DArrayField.h"

namespace titania {
namespace X3D {

// Inhibit implicit instantiation for required instantiations.
extern template class X3DField <Array <SFColor*>> ;
extern template class X3DField <Array <SFColorRGBA*>> ;
extern template class X3DField <Array <SFImage*>> ;
extern template class X3DField <Array <SFMatrix3d*>> ;
extern template class X3DField <Array <SFMatrix3f*>> ;
extern template class X3DField <Array <SFMatrix4d*>> ;
extern template class X3DField <Array <SFMatrix4f*>> ;
extern template class X3DField <Array <SFRotation*>> ;
extern template class X3DField <Array <SFVec2d*>> ;
extern template class X3DField <Array <SFVec2f*>> ;
extern template class X3DField <Array <SFVec3d*>> ;
extern template class X3DField <Array <SFVec3f*>> ;
extern template class X3DField <Array <SFVec4d*>> ;
extern template class X3DField <Array <SFVec4f*>> ;

// Inhibit implicit instantiation for required instantiations.
extern template class X3DArrayField <SFColor>;
extern template class X3DArrayField <SFColorRGBA>;
extern template class X3DArrayField <SFImage>;
extern template class X3DArrayField <SFMatrix3d>;
extern template class X3DArrayField <SFMatrix3f>;
extern template class X3DArrayField <SFMatrix4d>;
extern template class X3DArrayField <SFMatrix4f>;
extern template class X3DArrayField <SFRotation>;
extern template class X3DArrayField <SFVec2d>;
extern template class X3DArrayField <SFVec2f>;
extern template class X3DArrayField <SFVec3d>;
extern template class X3DArrayField <SFVec3f>;
extern template class X3DArrayField <SFVec4d>;
extern template class X3DArrayField <SFVec4f>;

// Typedefs for standard X3DArrayFields
using MFColor     = X3DArrayField <SFColor>;
using MFColorRGBA = X3DArrayField <SFColorRGBA>;
using MFImage     = X3DArrayField <SFImage>;
using MFMatrix3d  = X3DArrayField <SFMatrix3d>;
using MFMatrix3f  = X3DArrayField <SFMatrix3f>;
using MFMatrix4d  = X3DArrayField <SFMatrix4d>;
using MFMatrix4f  = X3DArrayField <SFMatrix4f>;
using MFRotation  = X3DArrayField <SFRotation>;
using MFVec2d     = X3DArrayField <SFVec2d>;
using MFVec2f     = X3DArrayField <SFVec2f>;
using MFVec3d     = X3DArrayField <SFVec3d>;
using MFVec3f     = X3DArrayField <SFVec3f>;
using MFVec4d     = X3DArrayField <SFVec4d>;
using MFVec4f     = X3DArrayField <SFVec4f>;

} // X3D
} // titania

#endif
