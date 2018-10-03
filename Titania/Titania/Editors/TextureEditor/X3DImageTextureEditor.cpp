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

#include "X3DImageTextureEditor.h"

#include "../../ComposedWidgets/MFStringURLWidget.h"

namespace titania {
namespace puck {

X3DImageTextureEditor::X3DImageTextureEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	                      url (new MFStringURLWidget (this,
	                           getImageTextureURLTreeView (),
	                           getImageTextureURLCellRendererText (),
	                           getImageTextureURLAddButton (),
	                           getImageTextureURLRemoveButton (),
	                           getImageTextureURLReloadButton (),
	                           getImageTextureURLChooserColumn (),
	                           "url")),
	             imageTexture ()
{
	addChildObjects (imageTexture);
}

void
X3DImageTextureEditor::setImageTexture (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	imageTexture = value;

	getImageTextureBox () .set_visible (imageTexture);

	if (not imageTexture)
		imageTexture = executionContext -> createNode <X3D::ImageTexture> ();

	url -> setNodes ({ imageTexture });
}

const X3D::X3DPtr <X3D::ImageTexture> &
X3DImageTextureEditor::getImageTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	getImageTextureBox () .set_visible (value);

	if (value)
	{
		for (const auto & type : basic::make_reverse_range (value -> getType ()))
		{
			switch (type)
			{
				case X3D::X3DConstants::MovieTexture:
					break;
				case X3D::X3DConstants::X3DUrlObject:
				{
					imageTexture -> url () = value -> getField <X3D::MFString> ("url");
					break;
				}
				default:
					continue;
			}

			break;
		}
	}

	return imageTexture;
}

void
X3DImageTextureEditor::on_embed_image_clicked ()
{
	for (const auto & URL : basic::make_const_range (imageTexture -> url ()))
	{
		try
		{
			X3D::FileLoader loader (imageTexture -> getExecutionContext ());
			const std::string image = loader .loadDocument (URL .raw ());

			bool        result_uncertain;
			std::string contentType = Gio::content_type_guess (loader .getWorldURL () .path (), (guchar*) image .data (), image .size (), result_uncertain);

			const auto  undoStep = std::make_shared <X3D::UndoStep> (_ ("Embed Image"));
			std::string data     = "data:" + contentType + ";base64," + Glib::Base64::encode (image);

			undoStep -> addObjects (imageTexture);
			undoStep -> addUndoFunction (&X3D::MFString::setValue, std::ref (imageTexture -> url ()), imageTexture -> url ());
			imageTexture -> url () .emplace_front (std::move (data));
			undoStep -> addRedoFunction (&X3D::MFString::setValue, std::ref (imageTexture -> url ()), imageTexture -> url ());

			getBrowserWindow () -> addUndoStep (undoStep);
			return;
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

X3DImageTextureEditor::~X3DImageTextureEditor ()
{ }

} // puck
} // titania
