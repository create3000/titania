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

#include "../Browser/BrowserWindow.h"

namespace titania {
namespace puck {

X3DTextureTransformEditor::X3DTextureTransformEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface ("", ""),
	         textureTransform (),
	                 undoStep (),
	                 changing (false)
{ }

void
X3DTextureTransformEditor::setTextureTransform (const X3D::X3DPtr <X3D::X3DTextureTransformNode> & value)
{
	if (value == textureTransform)
		return;

	if (textureTransform)
	{
		textureTransform -> translation () .removeInterest (this, &X3DTextureTransformEditor::set_translation);
		textureTransform -> rotation ()    .removeInterest (this, &X3DTextureTransformEditor::set_rotation);
		textureTransform -> scale ()       .removeInterest (this, &X3DTextureTransformEditor::set_scale);
		textureTransform -> center ()      .removeInterest (this, &X3DTextureTransformEditor::set_center);
	}

	X3D::X3DPtr <X3D::TextureTransform> last = textureTransform;

	textureTransform = value;

	if (textureTransform)
	{
		if (last == getTextureTransformNode ())
		{
			textureTransform -> translation () = last -> translation ();
			textureTransform -> rotation ()    = last -> rotation ();
			textureTransform -> scale ()       = last -> scale ();
			textureTransform -> center ()      = last -> center ();
		}

		textureTransform -> translation () .addInterest (this, &X3DTextureTransformEditor::set_translation);
		textureTransform -> rotation ()    .addInterest (this, &X3DTextureTransformEditor::set_rotation);
		textureTransform -> scale ()       .addInterest (this, &X3DTextureTransformEditor::set_scale);
		textureTransform -> center ()      .addInterest (this, &X3DTextureTransformEditor::set_center);

		set_translation ();
		set_rotation ();
		set_scale ();
		set_center ();
	}
}

/***********************************************************************************************************************
 *
 *  translation
 *
 **********************************************************************************************************************/

void
X3DTextureTransformEditor::on_textureTransform_translation_changed ()
{
	if (changing)
		return;

	addUndoFunction (textureTransform, textureTransform -> translation (), undoStep);

	textureTransform -> translation () .setX (getTextureTransformTranslationXSpinButton () .get_value ());
	textureTransform -> translation () .setY (getTextureTransformTranslationYSpinButton () .get_value ());

	textureTransform -> translation () .removeInterest (this, &X3DTextureTransformEditor::set_translation);
	textureTransform -> translation () .addInterest (this, &X3DTextureTransformEditor::connectTranslation);

	addRedoFunction (textureTransform -> translation (), undoStep);

}

void
X3DTextureTransformEditor::set_translation ()
{
	changing = true;

	getTextureTransformTranslationXSpinButton () .set_value (textureTransform -> translation () .getX ());
	getTextureTransformTranslationYSpinButton () .set_value (textureTransform -> translation () .getY ());

	changing = false;
}

void
X3DTextureTransformEditor::connectTranslation (const X3D::SFVec2f & field)
{
	field .removeInterest (this, &X3DTextureTransformEditor::connectTranslation);
	field .addInterest (this, &X3DTextureTransformEditor::set_translation);
}

/***********************************************************************************************************************
 *
 *  rotation
 *
 **********************************************************************************************************************/

void
X3DTextureTransformEditor::on_textureTransform_rotation_changed ()
{
	if (changing)
		return;

	addUndoFunction (textureTransform, textureTransform -> rotation (), undoStep);

	textureTransform -> rotation () = getTextureTransformRotationSpinButton () .get_value ();

	textureTransform -> rotation () .removeInterest (this, &X3DTextureTransformEditor::set_rotation);
	textureTransform -> rotation () .addInterest (this, &X3DTextureTransformEditor::connectRotation);

	addRedoFunction (textureTransform -> rotation (), undoStep);

}

void
X3DTextureTransformEditor::set_rotation ()
{
	changing = true;

	getTextureTransformRotationSpinButton () .set_value (textureTransform -> rotation ());

	changing = false;
}

void
X3DTextureTransformEditor::connectRotation (const X3D::SFFloat & field)
{
	field .removeInterest (this, &X3DTextureTransformEditor::connectRotation);
	field .addInterest (this, &X3DTextureTransformEditor::set_rotation);
}

/***********************************************************************************************************************
 *
 *  scale
 *
 **********************************************************************************************************************/

void
X3DTextureTransformEditor::on_textureTransform_scale_changed ()
{
	if (changing)
		return;

	addUndoFunction (textureTransform, textureTransform -> scale (), undoStep);

	textureTransform -> scale () .setX (getTextureTransformScaleXSpinButton () .get_value ());
	textureTransform -> scale () .setY (getTextureTransformScaleYSpinButton () .get_value ());

	textureTransform -> scale () .removeInterest (this, &X3DTextureTransformEditor::set_scale);
	textureTransform -> scale () .addInterest (this, &X3DTextureTransformEditor::connectScale);

	addRedoFunction (textureTransform -> scale (), undoStep);

}

void
X3DTextureTransformEditor::set_scale ()
{
	changing = true;

	getTextureTransformScaleXSpinButton () .set_value (textureTransform -> scale () .getX ());
	getTextureTransformScaleYSpinButton () .set_value (textureTransform -> scale () .getY ());

	changing = false;
}

void
X3DTextureTransformEditor::connectScale (const X3D::SFVec2f & field)
{
	field .removeInterest (this, &X3DTextureTransformEditor::connectScale);
	field .addInterest (this, &X3DTextureTransformEditor::set_scale);
}

/***********************************************************************************************************************
 *
 *  center
 *
 **********************************************************************************************************************/

void
X3DTextureTransformEditor::on_textureTransform_center_changed ()
{
	if (changing)
		return;

	addUndoFunction (textureTransform, textureTransform -> center (), undoStep);

	textureTransform -> center () .setX (getTextureTransformCenterXSpinButton () .get_value ());
	textureTransform -> center () .setY (getTextureTransformCenterYSpinButton () .get_value ());

	textureTransform -> center () .removeInterest (this, &X3DTextureTransformEditor::set_center);
	textureTransform -> center () .addInterest (this, &X3DTextureTransformEditor::connectCenter);

	addRedoFunction (textureTransform -> center (), undoStep);
}

void
X3DTextureTransformEditor::set_center ()
{
	changing = true;

	getTextureTransformCenterXSpinButton () .set_value (textureTransform -> center () .getX ());
	getTextureTransformCenterYSpinButton () .set_value (textureTransform -> center () .getY ());

	changing = false;
}

void
X3DTextureTransformEditor::connectCenter (const X3D::SFVec2f & field)
{
	field .removeInterest (this, &X3DTextureTransformEditor::connectCenter);
	field .addInterest (this, &X3DTextureTransformEditor::set_center);
}

} // puck
} // titania
