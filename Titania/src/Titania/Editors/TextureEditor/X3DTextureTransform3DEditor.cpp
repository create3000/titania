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

#include "X3DTextureTransform3DEditor.h"

#include <Titania/X3D/Components/Texturing/TextureTransform.h>
#include <Titania/X3D/Components/Texturing3D/TextureTransformMatrix3D.h>

namespace titania {
namespace puck {

X3DTextureTransform3DEditor::X3DTextureTransform3DEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	         textureTransform (),
	              translation (this,
	                           getTextureTransform3DTranslationXAdjustment (),
	                           getTextureTransform3DTranslationYAdjustment (),
	                           getTextureTransform3DTranslationZAdjustment (),
	                           getTextureTransform3DTranslationBox (),
	                           "translation"),
	             rotationTool (this, getTextureTransform3DRotationToolBox (), "rotation"),
	                 rotation (this,
	                           getTextureTransform3DRotationXAdjustment (),
	                           getTextureTransform3DRotationYAdjustment (),
	                           getTextureTransform3DRotationZAdjustment (),
	                           getTextureTransform3DRotationAAdjustment (),
	                           getTextureTransform3DRotationBox (),
	                           "rotation"),
	            rotationEuler (this,
	                           getTextureTransform3DRotationEulerXAdjustment (),
	                           getTextureTransform3DRotationEulerYAdjustment (),
	                           getTextureTransform3DRotationEulerZAdjustment (),
	                           getTextureTransform3DRotationEulerBox (),
	                           "rotation"),
	                    scale (this,
	                           getTextureTransform3DScaleXAdjustment (),
	                           getTextureTransform3DScaleYAdjustment (),
	                           getTextureTransform3DScaleZAdjustment (),
	                           getTextureTransform3DScaleBox (),
	                           "scale"),
	                   center (this,
	                           getTextureTransform3DCenterXAdjustment (),
	                           getTextureTransform3DCenterYAdjustment (),
	                           getTextureTransform3DCenterZAdjustment (),
	                           getTextureTransform3DCenterBox (),
	                           "center")
{
	addChildObjects (textureTransform);
}

void
X3DTextureTransform3DEditor::configure ()
{
	switch (getConfig () -> getItem <int32_t> ("textureTransform3DRotationType"))
	{
		case 1:
			getTextureTransform3DRotationEulerButton () .set_active (true);
			break;
		default:
			getTextureTransform3DRotationAxisAngleButton () .set_active (true);
			break;
	}

	getTextureTransform3DUniformScaleButton () .set_active (getConfig () -> getItem <bool> ("textureTransform3DUniformScale"));
}

void
X3DTextureTransform3DEditor::setTextureTransform3D (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DPtr <X3D::X3DTextureTransformNode> & value)
{
	textureTransform = value;

	if (not textureTransform)
		textureTransform = executionContext -> createNode <X3D::TextureTransform3D> ();

	const X3D::MFNode nodes = { textureTransform };

	translation   .setNodes (nodes);
	rotationTool  .setNodes (nodes);
	rotation      .setNodes (nodes);
	rotationEuler .setNodes (nodes);
	scale         .setNodes (nodes);
	center        .setNodes (nodes);

}

const X3D::X3DPtr <X3D::TextureTransform3D> &
X3DTextureTransform3DEditor::getTextureTransform3D (const X3D::X3DPtr <X3D::X3DTextureTransformNode> & value)
{
	if (not value)
		return textureTransform;

	switch (value -> getType () .back ())
	{
		case X3D::X3DConstants::TextureTransform:
		{
			const X3D::X3DPtr <X3D::TextureTransform> last (value);

			textureTransform -> translation () = X3D::Vector3f (last -> translation () .getX (), last -> translation () .getY (), 0);
			textureTransform -> rotation ()    = X3D::Rotation4d (0, 0, 1, last -> rotation ());
			textureTransform -> scale ()       = X3D::Vector3f (last -> scale ()  .getX (), last -> scale ()  .getY (), 1);
			textureTransform -> center ()      = X3D::Vector3f (last -> center () .getX (), last -> center () .getY (), 0);
			break;
		}
		case X3D::X3DConstants::TextureTransform3D:
		{
			const X3D::X3DPtr <X3D::TextureTransform3D> last (value);

			textureTransform -> translation () = last -> translation ();
			textureTransform -> rotation ()    = last -> rotation ();
			textureTransform -> scale ()       = last -> scale ();
			textureTransform -> center ()      = last -> center ();
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
	
				textureTransform -> translation () = -translation;
				textureTransform -> rotation ()    = ~rotation;
				textureTransform -> scale ()       = 1.0f / scale;
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
X3DTextureTransform3DEditor::on_texture_transform_3d_rotation_type_clicked ()
{
	getTextureTransform3DRotationPopover () .popup ();
}

void
X3DTextureTransform3DEditor::on_texture_transform_3d_rotation_axis_angle_toggled ()
{
	getTextureTransform3DRotationPopover () .popdown ();

	if (not getTextureTransform3DRotationAxisAngleButton () .get_active ())
		return;

	for (const auto widget : getTextureTransform3DRotationNotebook () .get_children ())
		widget -> set_visible (false);

	getTextureTransform3DRotationBox () .set_visible (true);
	getTextureTransform3DRotationNotebook () .set_current_page (0);

	getConfig () -> setItem <int32_t> ("textureTransform3DRotationType", 0);
}

void
X3DTextureTransform3DEditor::on_texture_transform_3d_rotation_euler_toggled ()
{
	getTextureTransform3DRotationPopover () .popdown ();

	if (not getTextureTransform3DRotationEulerButton () .get_active ())
		return;

	for (const auto widget : getTextureTransform3DRotationNotebook () .get_children ())
		widget -> set_visible (false);

	getTextureTransform3DRotationEulerBox () .set_visible (true);
	getTextureTransform3DRotationNotebook () .set_current_page (1);

	getConfig () -> setItem <int32_t> ("textureTransform3DRotationType", 1);
}

void
X3DTextureTransform3DEditor::on_texture_transform_3d_uniform_scale_clicked ()
{
	if (getTextureTransform3DUniformScaleButton () .get_active ())
	{
		getTextureTransform3DUniformScaleImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getTextureTransform3DUniformScaleImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

void
X3DTextureTransform3DEditor::store ()
{
	getConfig () -> setItem ("textureTransform3DUniformScale", getTextureTransform3DUniformScaleButton () .get_active ());
}

X3DTextureTransform3DEditor::~X3DTextureTransform3DEditor ()
{ }

} // puck
} // titania
