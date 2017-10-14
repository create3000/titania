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

#include "X3DTextureTransformMatrix3DEditor.h"

namespace titania {
namespace puck {

X3DTextureTransformMatrix3DEditor::X3DTextureTransformMatrix3DEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	         textureTransform (),
	                  value00 (this,
	                           getTextureTransformMatrix3D00Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value01 (this,
	                           getTextureTransformMatrix3D01Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value02 (this,
	                           getTextureTransformMatrix3D02Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value03 (this,
	                           getTextureTransformMatrix3D03Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value04 (this,
	                           getTextureTransformMatrix3D04Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value05 (this,
	                           getTextureTransformMatrix3D05Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value06 (this,
	                           getTextureTransformMatrix3D06Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value07 (this,
	                           getTextureTransformMatrix3D07Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value08 (this,
	                           getTextureTransformMatrix3D08Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value09 (this,
	                           getTextureTransformMatrix3D09Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value10 (this,
	                           getTextureTransformMatrix3D10Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value11 (this,
	                           getTextureTransformMatrix3D11Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value12 (this,
	                           getTextureTransformMatrix3D12Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value13 (this,
	                           getTextureTransformMatrix3D13Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value14 (this,
	                           getTextureTransformMatrix3D14Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix"),
	                  value15 (this,
	                           getTextureTransformMatrix3D15Adjustment (),
	                           getTextureTransformMatrix3DMatrixGrid (),
	                           "matrix")
{
	addChildObjects (textureTransform);

	value00 .setIndex ( 0);
	value01 .setIndex ( 1);
	value02 .setIndex ( 2);
	value03 .setIndex ( 3);
	value04 .setIndex ( 4);
	value05 .setIndex ( 5);
	value06 .setIndex ( 6);
	value07 .setIndex ( 7);
	value08 .setIndex ( 8);
	value09 .setIndex ( 9);
	value10 .setIndex (10);
	value11 .setIndex (11);
	value12 .setIndex (12);
	value13 .setIndex (13);
	value14 .setIndex (14);
	value15 .setIndex (15);
}

void
X3DTextureTransformMatrix3DEditor::configure ()
{ }

void
X3DTextureTransformMatrix3DEditor::setTextureTransformMatrix3D (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DPtr <X3D::X3DTextureTransformNode> & value)
{
	textureTransform = value;

	if (not textureTransform)
		textureTransform = executionContext -> createNode <X3D::TextureTransformMatrix3D> ();

	const X3D::MFNode nodes = { textureTransform };

	value00 .setNodes (nodes);
	value01 .setNodes (nodes);
	value02 .setNodes (nodes);
	value03 .setNodes (nodes);
	value04 .setNodes (nodes);
	value05 .setNodes (nodes);
	value06 .setNodes (nodes);
	value07 .setNodes (nodes);
	value08 .setNodes (nodes);
	value09 .setNodes (nodes);
	value10 .setNodes (nodes);
	value11 .setNodes (nodes);
	value12 .setNodes (nodes);
	value13 .setNodes (nodes);
	value14 .setNodes (nodes);
	value15 .setNodes (nodes);
}

const X3D::X3DPtr <X3D::TextureTransformMatrix3D> &
X3DTextureTransformMatrix3DEditor::getTextureTransformMatrix3D (const X3D::X3DPtr <X3D::X3DTextureTransformNode> & value)
{
	if (not value)
		return textureTransform;

	switch (value -> getType () .back ())
	{
		case X3D::X3DConstants::TextureTransform:
		case X3D::X3DConstants::TextureTransform3D:
		{
			const X3D::X3DPtr <X3D::X3DTextureTransformNode> last (value);

			textureTransform -> matrix () = last -> getMatrix ();
			break;
		}
		case X3D::X3DConstants::TextureTransformMatrix3D:
		{
			const X3D::X3DPtr <X3D::TextureTransformMatrix3D> last (value);

			textureTransform -> matrix () = last -> matrix ();
			break;
		}
		default:
			break;
	}

	return textureTransform;
}

void
X3DTextureTransformMatrix3DEditor::store ()
{ }

X3DTextureTransformMatrix3DEditor::~X3DTextureTransformMatrix3DEditor ()
{ }

} // puck
} // titania
