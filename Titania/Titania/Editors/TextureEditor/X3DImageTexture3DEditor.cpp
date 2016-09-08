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

#include "X3DImageTexture3DEditor.h"

#include "../../ComposedWidgets/MFStringURLWidget.h"

#include <Titania/Stream/Base64.h>

namespace titania {
namespace puck {

X3DImageTexture3DEditor::X3DImageTexture3DEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	                      url (new MFStringURLWidget (this,
	                           getImageTexture3DURLTreeView (),
	                           getImageTexture3DURLCellRendererText (),
	                           getImageTexture3DURLAddButton (),
	                           getImageTexture3DURLRemoveButton (),
	                           getImageTexture3DURLReloadButton (),
	                           getImageTexture3DURLChooserColumn (),
	                           "url")),
	             imageTexture ()
{
	addChildren (imageTexture);
}

void
X3DImageTexture3DEditor::setImageTexture3D (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	imageTexture = value;

	getImageTexture3DBox () .set_visible (imageTexture);

	if (not imageTexture)
		imageTexture = getCurrentContext () -> createNode <X3D::ImageTexture3D> ();

	url -> setNodes ({ imageTexture });
}

const X3D::X3DPtr <X3D::ImageTexture3D> &
X3DImageTexture3DEditor::getImageTexture3D (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	getImageTexture3DBox () .set_visible (value);

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

//void
//X3DImageTexture3DEditor::on_embed_image_clicked ()
//{
//	for (const auto & URL : imageTexture -> url ())
//	{
//		try
//		{
//			X3D::Loader       loader (imageTexture -> getExecutionContext ());
//			const std::string image = loader .loadDocument (URL);
//
//			bool        result_uncertain;
//			std::string contentType = Gio::content_type_guess (loader .getWorldURL () .path (), (guchar*) image .data (), image .size (), result_uncertain);
//
//			const auto  undoStep = std::make_shared <X3D::UndoStep> (_ ("Embed Image"));
//			std::string data     = "data:" + contentType + ";base64," + basic::base64_encode (image);
//
//			undoStep -> addObjects (imageTexture);
//			undoStep -> addUndoFunction (&X3D::MFString::setValue, std::ref (imageTexture -> url ()), imageTexture -> url ());
//			imageTexture -> url () .emplace_front (std::move (data));
//			undoStep -> addRedoFunction (&X3D::MFString::setValue, std::ref (imageTexture -> url ()), imageTexture -> url ());
//
//			getBrowserWindow () -> addUndoStep (undoStep);
//			return;
//		}
//		catch (const X3D::X3DError &)
//		{ }
//	}
//}

X3DImageTexture3DEditor::~X3DImageTexture3DEditor ()
{ }

} // puck
} // titania
