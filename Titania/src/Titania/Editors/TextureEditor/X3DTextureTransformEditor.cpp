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

#include "X3DTextureTransformEditor.h"

#include <Titania/X3D/Components/Texturing3D/TextureTransform3D.h>
#include <Titania/X3D/Components/Texturing3D/TextureTransformMatrix3D.h>

namespace titania {
namespace puck {

using math::pi;

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
{
	addChildObjects (textureTransform);
}

void
X3DTextureTransformEditor::configure ()
{
	getTextureTransformUniformScaleButton () .set_active (getConfig () -> getItem <bool> ("textureTransformUniformScale"));
}

void
X3DTextureTransformEditor::setTextureTransform (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DPtr <X3D::X3DTextureTransformNode> & value)
{
	textureTransform = value;

	if (not textureTransform)
		textureTransform = executionContext -> createNode <X3D::TextureTransform> ();

	const X3D::MFNode nodes = { textureTransform };

	translation .setNodes (nodes);
	rotation    .setNodes (nodes);
	scale       .setNodes (nodes);
	center      .setNodes (nodes);
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
		case X3D::X3DConstants::TextureTransform3D:
		{
			const X3D::X3DPtr <X3D::TextureTransform3D> last (value);

			auto proj = last -> rotation () .getValue () .mult_vec_rot (X3D::Vector3d (1, 0, 0));
			proj .z (0);
			const auto r = X3D::Rotation4d (X3D::Vector3d (1, 0, 0), proj);

			textureTransform -> translation () = X3D::Vector2f (last -> translation () .getX (), last -> translation () .getY ());
			textureTransform -> rotation ()    = r .axis () .z () > 0 ? r .angle () : pi <double> * 2 - r .angle ();
			textureTransform -> scale ()       = X3D::Vector2f (last -> scale ()  .getX (), last -> scale ()  .getY ());
			textureTransform -> center ()      = X3D::Vector2f (last -> center () .getX (), last -> center () .getY ());
			break;
		}
		case X3D::X3DConstants::TextureTransformMatrix3D:
		{
			try
			{
				const X3D::X3DPtr <X3D::TextureTransformMatrix3D> last (value);
	
				X3D::Vector3f   translation;
				X3D::Rotation4d rotation;
				X3D::Vector3f   scale;
				X3D::Rotation4d scaleOrientation;
	
				math::inverse (last -> matrix () .getValue ()) .get (translation, rotation, scale, scaleOrientation);
	
				translation .negate ();
				rotation .inverse ();
				scale = 1.0f / scale;
	
				auto proj = rotation .mult_vec_rot (X3D::Vector3d (1, 0, 0));
				proj .z (0);
				const auto r = X3D::Rotation4d (X3D::Vector3d (1, 0, 0), proj);
	
				textureTransform -> translation () = X3D::Vector2f (translation .x (), translation .y ());
				textureTransform -> rotation ()    = r .axis () .z () > 0 ? r .angle () : pi <double> * 2 - r .angle ();
				textureTransform -> scale ()       = X3D::Vector2f (scale .x (), scale .y ());
			}
			catch (const std::domain_error &)
			{ }

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

void
X3DTextureTransformEditor::store ()
{
	getConfig () -> setItem ("textureTransformUniformScale", getTextureTransformUniformScaleButton () .get_active ());
}

X3DTextureTransformEditor::~X3DTextureTransformEditor ()
{ }

} // puck
} // titania
