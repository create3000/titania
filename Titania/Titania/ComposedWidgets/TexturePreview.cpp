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

#include "TexturePreview.h"

#include "../Bits/String.h"
#include "../Configuration/config.h"

#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Components/Texturing/MultiTexture.h>
#include <Titania/X3D/Components/Texturing/X3DTexture2DNode.h>
#include <Titania/X3D/Components/Texturing3D/X3DTexture3DNode.h>
#include <Titania/X3D/Components/CubeMapTexturing/X3DEnvironmentTextureNode.h>

namespace titania {
namespace puck {

TexturePreview::TexturePreview (X3DBaseInterface* const editor,
                                Gtk::Box & box,
                                Gtk::Label & formatLabel,
                                Gtk::Label & loadStateLabel) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getMasterBrowser ()),
	X3DComposedWidget (editor),
	              box (box),
	      formatLabel (formatLabel),
	   loadStateLabel (loadStateLabel),
	          preview (X3D::createBrowser (editor -> getMasterBrowser (), { get_ui ("Editors/TexturePreview.x3dv") })),
	      textureNode (),
	     textureNodes ()
{
	addChildObjects (preview, textureNode, textureNodes);

	// Browser

	preview -> signal_configure_event () .connect (sigc::mem_fun (this, &TexturePreview::on_configure_event));
	preview -> initialized () .addInterest (&TexturePreview::set_initialized, this);
	preview -> setAntialiasing (4);
	preview -> show ();

	box .pack_start (*preview, true, true, 0);

	// Setup

	setup ();
}

void
TexturePreview::setTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	if (textureNode)
	{
		textureNode -> removeInterest (&X3D::Browser::addEvent, *preview);
		textureNode -> checkLoadState () .removeInterest (&TexturePreview::set_loadState, this);

		const X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode (textureNode);
	
		if (texture2DNode)
		{
			texture2DNode -> width ()  .removeInterest (&TexturePreview::set_loadState, this);
			texture2DNode -> height () .removeInterest (&TexturePreview::set_loadState, this);
		}

		try
		{
			textureNode -> getField <X3D::MFNode> ("texture") .removeInterest (&TexturePreview::set_textures, this);
		}
		catch (const X3D::X3DError &)
		{ }

		set_textures ({ });
	}

	textureNode = value;

	if (textureNode)
	{
		textureNode -> addInterest (&X3D::Browser::addEvent, *preview);
		textureNode -> checkLoadState () .addInterest (&TexturePreview::set_loadState, this);

		const X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode (textureNode);
	
		if (texture2DNode)
		{
			texture2DNode -> width ()  .addInterest (&TexturePreview::set_loadState, this);
			texture2DNode -> height () .addInterest (&TexturePreview::set_loadState, this);
		}

		try
		{
			const auto & texture = textureNode -> getField <X3D::MFNode> ("texture");

			texture .addInterest (&TexturePreview::set_textures, this);

			set_textures (texture);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_texture ();
}

void
TexturePreview::set_textures (const X3D::MFNode & value)
{
	for (const auto & node : textureNodes)
		node -> removeInterest (&X3D::Browser::addEvent, *preview);

	textureNodes = value;

	for (const auto & node : textureNodes)
		node -> addInterest (&X3D::Browser::addEvent, *preview);
}

void
TexturePreview::set_initialized ()
{
	try
	{
		preview -> initialized () .removeInterest (&TexturePreview::set_initialized, this);
		preview -> getExecutionContext () -> getNamedNode ("Appearance")    -> setPrivate (true);
		preview -> getExecutionContext () -> getNamedNode ("TextureScript") -> setPrivate (true);
	}
	catch (const X3D::X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}

	set_texture ();
}

void
TexturePreview::set_texture ()
{
	try
	{
		const auto texture2DViewpoint = preview -> getExecutionContext () -> getNamedNode <X3D::OrthoViewpoint> ("Texture2DViewpoint");
		const auto texture3DViewpoint = preview -> getExecutionContext () -> getNamedNode <X3D::Viewpoint>      ("Texture3DViewpoint");
		const auto cubeMapViewpoint   = preview -> getExecutionContext () -> getNamedNode <X3D::Viewpoint>      ("CubeMapViewpoint");
		const auto appearance         = preview -> getExecutionContext () -> getNamedNode <X3D::Appearance>     ("Appearance");

		texture2DViewpoint -> resetUserOffsets ();
		texture3DViewpoint -> resetUserOffsets ();
		cubeMapViewpoint   -> resetUserOffsets ();

		const X3D::X3DPtr <X3D::MultiTexture> multiTexture (textureNode);

		if (multiTexture)
		{
			const X3D::X3DPtr <X3D::MultiTexture> localTexture (textureNode -> copy (appearance -> getExecutionContext (), X3D::FLAT_COPY));

			multiTexture -> color ()    .addInterest (localTexture -> color ());
			multiTexture -> alpha ()    .addInterest (localTexture -> alpha ());
			multiTexture -> mode ()     .addInterest (localTexture -> mode ());
			multiTexture -> source ()   .addInterest (localTexture -> source ());
			multiTexture -> function () .addInterest (localTexture -> function ());
			multiTexture -> texture ()  .addInterest (localTexture -> texture ());

			appearance -> texture () = localTexture;
		}
		else
			appearance -> texture () = textureNode;
	}
	catch (const X3D::X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}

	set_loadState ();
}

void
TexturePreview::set_loadState ()
{
	set_camera ();

	// Set label

	formatLabel    .set_text (textureInfo (textureNode));
	loadStateLabel .set_text (loadStateInfo (textureNode ? textureNode -> checkLoadState () : X3D::NOT_STARTED_STATE));
}

bool
TexturePreview::on_configure_event (GdkEventConfigure* const)
{
	set_camera ();
	return false;
}

void
TexturePreview::set_camera ()
{
	const X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode (textureNode);

	if (texture2DNode)
		set_camera (texture2DNode -> width (), texture2DNode -> height ());

	else
		set_camera (512, 512);
}

void
TexturePreview::set_camera (double width, double height)
{
	try
	{
		const X3D::X3DPtr <X3D::OrthoViewpoint> viewpoint (preview -> getExecutionContext () -> getNamedNode ("Texture2DViewpoint"));
		const X3D::X3DPtr <X3D::Transform>      transform (preview -> getExecutionContext () -> getNamedNode ("Texture2D"));

		if (viewpoint and transform)
		{
			if (not width or not height)
			{
				width  = 1;
				height = 1;
			}

			viewpoint -> fieldOfView () = { -width, -height, width, height };
			transform -> scale ()       = X3D::Vector3f (width, height, 1);
		}
	}
	catch (const X3D::X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

TexturePreview::~TexturePreview ()
{
	dispose ();
}

} // puck
} // titania
