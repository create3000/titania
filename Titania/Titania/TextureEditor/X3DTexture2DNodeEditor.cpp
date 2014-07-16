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

#include "X3DTexture2DNodeEditor.h"

#include "../Browser/BrowserWindow.h"

namespace titania {
namespace puck {

X3DTexture2DNodeEditor::X3DTexture2DNodeEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface ("", ""),
	    X3DImageTextureEditor (),
	            texture2DNode (),
	                 undoStep (),
	                 changing (false)
{ }

void
X3DTexture2DNodeEditor::setTexture2DNode (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	if (value == texture2DNode)
		return;

	setImageTexture (value);

	if (texture2DNode)
	{
		texture2DNode -> repeatS () .removeInterest (this, &X3DTexture2DNodeEditor::set_repeatS);
		texture2DNode -> repeatT () .removeInterest (this, &X3DTexture2DNodeEditor::set_repeatT);
	}

	X3D::X3DPtr <X3D::X3DTexture2DNode> last = texture2DNode;

	texture2DNode = value;

	if (texture2DNode)
	{
		if (last == getTextureNode ())
		{
			texture2DNode -> repeatS () = last -> repeatS ();
			texture2DNode -> repeatT () = last -> repeatT ();
		}

		texture2DNode -> repeatS () .addInterest (this, &X3DTexture2DNodeEditor::set_repeatS);
		texture2DNode -> repeatT () .addInterest (this, &X3DTexture2DNodeEditor::set_repeatT);

		set_repeatS ();
		set_repeatT ();
	}
}

/***********************************************************************************************************************
 *
 *  repeatS
 *
 **********************************************************************************************************************/

void
X3DTexture2DNodeEditor::on_texture2DNode_repeatS_toggled ()
{
	if (changing)
		return;

	addUndoFunction (texture2DNode, texture2DNode -> repeatS (), undoStep);

	texture2DNode -> repeatS () = getTexture2DNodeRepeatSCheckButton () .get_active ();

	texture2DNode -> repeatS () .removeInterest (this, &X3DTexture2DNodeEditor::set_repeatS);
	texture2DNode -> repeatS () .addInterest (this, &X3DTexture2DNodeEditor::connectRepeatS);

	addRedoFunction (texture2DNode -> repeatS (), undoStep);

}

void
X3DTexture2DNodeEditor::set_repeatS ()
{
	changing = true;

	getTexture2DNodeRepeatSCheckButton () .set_active (texture2DNode -> repeatS ());

	changing = false;
}

void
X3DTexture2DNodeEditor::connectRepeatS (const X3D::SFBool & field)
{
	field .removeInterest (this, &X3DTexture2DNodeEditor::connectRepeatS);
	field .addInterest (this, &X3DTexture2DNodeEditor::set_repeatS);
}

/***********************************************************************************************************************
 *
 *  repeatT
 *
 **********************************************************************************************************************/

void
X3DTexture2DNodeEditor::on_texture2DNode_repeatT_toggled ()
{
	if (changing)
		return;

	addUndoFunction (texture2DNode, texture2DNode -> repeatT (), undoStep);

	texture2DNode -> repeatT () = getTexture2DNodeRepeatTCheckButton () .get_active ();

	texture2DNode -> repeatT () .removeInterest (this, &X3DTexture2DNodeEditor::set_repeatT);
	texture2DNode -> repeatT () .addInterest (this, &X3DTexture2DNodeEditor::connectRepeatT);

	addRedoFunction (texture2DNode -> repeatT (), undoStep);

}

void
X3DTexture2DNodeEditor::set_repeatT ()
{
	changing = true;

	getTexture2DNodeRepeatTCheckButton () .set_active (texture2DNode -> repeatT ());

	changing = false;
}

void
X3DTexture2DNodeEditor::connectRepeatT (const X3D::SFBool & field)
{
	field .removeInterest (this, &X3DTexture2DNodeEditor::connectRepeatT);
	field .addInterest (this, &X3DTexture2DNodeEditor::set_repeatT);
}

} // puck
} // titania
