/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "X3DConstants.h"

#include "../Fields.h"

namespace titania {
namespace X3D {

const Type* const X3DConstants::SFBool      = &X3D::SFBool::type;
const Type* const X3DConstants::SFColor     = &X3D::SFColor::type;
const Type* const X3DConstants::SFColorRGBA = &X3D::SFColorRGBA::type;
const Type* const X3DConstants::SFDouble    = &X3D::SFDouble::type;
const Type* const X3DConstants::SFFloat     = &X3D::SFFloat::type;
const Type* const X3DConstants::SFImage     = &X3D::SFImage::type;
const Type* const X3DConstants::SFInt32     = &X3D::SFInt32::type;
const Type* const X3DConstants::SFMatrix3d  = &X3D::SFMatrix3d::type;
const Type* const X3DConstants::SFMatrix3f  = &X3D::SFMatrix3f::type;
const Type* const X3DConstants::SFMatrix4d  = &X3D::SFMatrix4d::type;
const Type* const X3DConstants::SFMatrix4f  = &X3D::SFMatrix4f::type;
const Type* const X3DConstants::SFNode      = &X3D::SFNode <X3DBasicNode>::type;
const Type* const X3DConstants::SFRotation  = &X3D::SFRotation::type;
const Type* const X3DConstants::SFString    = &X3D::SFString::type;
const Type* const X3DConstants::SFTime      = &X3D::SFTime::type;
const Type* const X3DConstants::SFVec2d     = &X3D::SFVec2d::type;
const Type* const X3DConstants::SFVec2f     = &X3D::SFVec2f::type;
const Type* const X3DConstants::SFVec3d     = &X3D::SFVec3d::type;
const Type* const X3DConstants::SFVec3f     = &X3D::SFVec3f::type;
const Type* const X3DConstants::SFVec4d     = &X3D::SFVec4d::type;
const Type* const X3DConstants::SFVec4f     = &X3D::SFVec4f::type;

const Type* const X3DConstants::MFBool      = &X3D::MFBool::type;
const Type* const X3DConstants::MFColor     = &X3D::MFColor::type;
const Type* const X3DConstants::MFColorRGBA = &X3D::MFColorRGBA::type;
const Type* const X3DConstants::MFDouble    = &X3D::MFDouble::type;
const Type* const X3DConstants::MFFloat     = &X3D::MFFloat::type;
const Type* const X3DConstants::MFImage     = &X3D::MFImage::type;
const Type* const X3DConstants::MFInt32     = &X3D::MFInt32::type;
const Type* const X3DConstants::MFMatrix3d  = &X3D::MFMatrix3d::type;
const Type* const X3DConstants::MFMatrix3f  = &X3D::MFMatrix3f::type;
const Type* const X3DConstants::MFMatrix4d  = &X3D::MFMatrix4d::type;
const Type* const X3DConstants::MFMatrix4f  = &X3D::MFMatrix4f::type;
const Type* const X3DConstants::MFNode      = &X3D::MFNode <X3DBasicNode>::type;
const Type* const X3DConstants::MFRotation  = &X3D::MFRotation::type;
const Type* const X3DConstants::MFString    = &X3D::MFString::type;
const Type* const X3DConstants::MFTime      = &X3D::MFTime::type;
const Type* const X3DConstants::MFVec2d     = &X3D::MFVec2d::type;
const Type* const X3DConstants::MFVec2f     = &X3D::MFVec2f::type;
const Type* const X3DConstants::MFVec3d     = &X3D::MFVec3d::type;
const Type* const X3DConstants::MFVec3f     = &X3D::MFVec3f::type;
const Type* const X3DConstants::MFVec4d     = &X3D::MFVec4d::type;
const Type* const X3DConstants::MFVec4f     = &X3D::MFVec4f::type;

} // X3D
} // titania
