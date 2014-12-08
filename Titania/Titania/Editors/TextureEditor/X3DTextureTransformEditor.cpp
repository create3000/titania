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

#include "X3DTextureTransformEditor.h"

namespace titania {
namespace puck {

X3DTextureTransformEditor::X3DTextureTransformEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	         textureTransform (),
	              translation (this,
	                           getTextureTransformTranslationXAdjustment (),
	                           getTextureTransformTranslationYAdjustment (),
	                           getTextureTransformTranslationBox (),
	                           "translation"),
	                 rotation (this,
	                           getTextureTransformRotationAdjustment (),
	                           getTextureTransformRotationSpinButton (),
	                           "rotation"),
	                    scale (this,
	                           getTextureTransformScaleXAdjustment (),
	                           getTextureTransformScaleYAdjustment (),
	                           getTextureTransformScaleBox (),
	                           "scale"),
	                   center (this,
	                           getTextureTransformCenterXAdjustment (),
	                           getTextureTransformCenterYAdjustment (),
	                           getTextureTransformCenterBox (),
	                           "center")
{ }

void
X3DTextureTransformEditor::initialize ()
{
	getTextureTransformUniformScaleButton () .set_active (getConfig () .getBoolean ("textureTransformUniformScale"));
}

void
X3DTextureTransformEditor::setTextureTransform (const X3D::X3DPtr <X3D::X3DTextureTransformNode> & value)
{
	textureTransform = value;

	if (not textureTransform)
	{
		textureTransform = new X3D::TextureTransform (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (textureTransform);
		getExecutionContext () -> realize ();
	}

	translation .setNodes ({ textureTransform });
	rotation    .setNodes ({ textureTransform });
	scale       .setNodes ({ textureTransform });
	center      .setNodes ({ textureTransform });
}

const X3D::X3DPtr <X3D::TextureTransform> &
X3DTextureTransformEditor::getTextureTransform (const X3D::X3DPtr <X3D::X3DTextureTransformNode> & value)
{
	if (not value)
		return textureTransform;

	switch (value -> getType () .back ())
	{
		case X3D::X3DConstants::TextureTransform:
		{
			const X3D::X3DPtr <X3D::TextureTransform> last (value);

			textureTransform -> translation () = last -> translation ();
			textureTransform -> rotation ()    = last -> rotation ();
			textureTransform -> scale ()       = last -> scale ();
			textureTransform -> center ()      = last -> center ();
			break;
		}
		default:
			break;
	}

	return textureTransform;
}

void
X3DTextureTransformEditor::on_texture_transform_uniform_scale_clicked ()
{
	if (getTextureTransformUniformScaleButton () .get_active ())
	{
		getTextureTransformUniformScaleImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getTextureTransformUniformScaleImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

X3DTextureTransformEditor::~X3DTextureTransformEditor ()
{
	getConfig () .setItem ("textureTransformUniformScale", getTextureTransformUniformScaleButton () .get_active ());
}

} // puck
} // titania
