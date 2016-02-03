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

#include "X3DMovieTextureEditor.h"

#include "../../ComposedWidgets/MFStringURLWidget.h"

namespace titania {
namespace puck {

X3DMovieTextureEditor::X3DMovieTextureEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	                 enabled (this, getMovieTextureEnabledCheckButton (), "enabled"),
	                    loop (this, getMovieTextureLoopCheckButton (), "loop"),
	                     url (new MFStringURLWidget (this,
	                          getMovieTextureURLTreeView (),
	                          getMovieTextureURLCellRendererText (),
	                          getMovieTextureURLAddButton (),
	                          getMovieTextureURLRemoveButton (),
	                          getMovieTextureURLReloadButton (),
	                          getMovieTextureURLChooserColumn (),
	                          "url")),
	             movieTexture ()
{ }

void
X3DMovieTextureEditor::setMovieTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	movieTexture = value;

	getMovieTextureBox () .set_visible (movieTexture);

	if (not movieTexture)
		movieTexture = getCurrentContext () -> createNode <X3D::MovieTexture> ();

	X3D::MFNode nodes = { movieTexture };

	enabled .setNodes (nodes);
	loop    .setNodes (nodes);

	url -> setNodes (nodes);
}

const X3D::X3DPtr <X3D::MovieTexture> &
X3DMovieTextureEditor::getMovieTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	getMovieTextureBox () .set_visible (value);

	if (value)
	{
		switch (value -> getType () .back ())
		{
			default:
				break;
		}
	}

	return movieTexture;
}

X3DMovieTextureEditor::~X3DMovieTextureEditor ()
{ }

} // puck
} // titania
