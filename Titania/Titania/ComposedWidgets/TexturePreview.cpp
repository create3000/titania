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
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	              box (box),
	      formatLabel (formatLabel),
	   loadStateLabel (loadStateLabel),
	          preview (X3D::createBrowser (editor -> getMasterBrowser (), { get_ui ("Editors/TexturePreview.x3dv") })),
	      textureNode (),
	     textureNodes ()
{
	addChildren (preview, textureNode, textureNodes);

	// Browser

	preview -> signal_configure_event () .connect (sigc::mem_fun (this, &TexturePreview::on_configure_event));
	preview -> initialized () .addInterest (this, &TexturePreview::set_initialized);
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
		textureNode -> removeInterest (*preview, &X3D::Browser::addEvent);
		textureNode -> checkLoadState () .removeInterest (this, &TexturePreview::set_loadState);

		try
		{
			textureNode -> getField <X3D::MFNode> ("texture") .removeInterest (this, &TexturePreview::set_textures);
		}
		catch (const X3D::X3DError &)
		{ }

		set_textures ({ });
	}

	textureNode = value;

	if (textureNode)
	{
		textureNode -> addInterest (*preview, &X3D::Browser::addEvent);
		textureNode -> checkLoadState () .addInterest (this, &TexturePreview::set_loadState);

		try
		{
			const auto & texture = textureNode -> getField <X3D::MFNode> ("texture");

			texture .addInterest (this, &TexturePreview::set_textures);

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
		node -> removeInterest (*preview, &X3D::Browser::addEvent);

	textureNodes = value;

	for (const auto & node : textureNodes)
		node -> addInterest (*preview, &X3D::Browser::addEvent);
}

void
TexturePreview::set_initialized ()
{
	try
	{
		preview -> initialized () .removeInterest (this, &TexturePreview::set_initialized);
		preview -> set_opacity (1);
		preview -> getExecutionContext () -> getNamedNode ("Appearance")    -> isPrivate (true);
		preview -> getExecutionContext () -> getNamedNode ("TextureScript") -> isPrivate (true);
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
		const X3D::X3DPtr <X3D::OrthoViewpoint> texture2DViewpoint (preview -> getExecutionContext () -> getNamedNode ("Texture2DViewpoint"));
		const X3D::X3DPtr <X3D::Viewpoint>      texture3DViewpoint (preview -> getExecutionContext () -> getNamedNode ("Texture3DViewpoint"));
		const X3D::X3DPtr <X3D::Viewpoint>      cubeMapViewpoint   (preview -> getExecutionContext () -> getNamedNode ("CubeMapViewpoint"));
		const X3D::AppearancePtr                appearance         (preview -> getExecutionContext () -> getNamedNode ("Appearance"));

		texture2DViewpoint -> resetUserOffsets ();
		texture3DViewpoint -> resetUserOffsets ();
		cubeMapViewpoint   -> resetUserOffsets ();

		if (appearance)
		{
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

	formatLabel .set_text ("");

	const X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode (textureNode);

	if (texture2DNode)
	{
		std::string components;
	
		switch (texture2DNode -> getComponents ())
		{
			case 1: components = _ ("GRAY");       break;
			case 2: components = _ ("GRAY ALPHA"); break;
			case 3: components = _ ("RGB");        break;
			case 4: components = _ ("RGBA");       break;
			default:
				break;
		}
	
		formatLabel .set_text (std::to_string (texture2DNode -> getImageWidth ()) +
		                       " × " +
		                       std::to_string (texture2DNode -> getImageHeight ()) +
		                       " (" +
		                       components +
		                       ")");
	}

	const X3D::X3DPtr <X3D::X3DTexture3DNode> texture3DNode (textureNode);

	if (texture3DNode)
	{
		std::string components;
	
		switch (texture3DNode -> getComponents ())
		{
			case 1: components = _ ("GRAY");       break;
			case 2: components = _ ("GRAY ALPHA"); break;
			case 3: components = _ ("RGB");        break;
			case 4: components = _ ("RGBA");       break;
			default:
				break;
		}
	
		formatLabel .set_text (std::to_string (texture3DNode -> getImageWidth ()) +
		                       " × " +
		                       std::to_string (texture3DNode -> getImageHeight ()) +
		                       " × " +
		                       std::to_string (texture3DNode -> getDepth ()) +
		                       " (" +
		                       components +
		                       ")");
	}

	const X3D::X3DPtr <X3D::X3DEnvironmentTextureNode> environmentTexture (textureNode);

	if (environmentTexture)
	{
		std::string components;
	
		switch (environmentTexture -> getComponents ())
		{
			case 1: components = _ ("GRAY");       break;
			case 2: components = _ ("GRAY ALPHA"); break;
			case 3: components = _ ("RGB");        break;
			case 4: components = _ ("RGBA");       break;
			default:
				break;
		}

		formatLabel .set_text (std::to_string (environmentTexture -> getWidth ()) +
		                       " × " +
		                       std::to_string (environmentTexture -> getHeight ()) +
		                       " × 6 " +
		                       " (" +
		                       components +
		                       ")");
	}

	if (textureNode)
	{
		switch (textureNode -> checkLoadState ())
		{
			case X3D::NOT_STARTED_STATE:
			{
				loadStateLabel .set_text (_ ("NOT STARTED"));
				break;
			}
			case X3D::IN_PROGRESS_STATE:
			{
				loadStateLabel.set_text (_ ("IN PROGRESS"));
				break;
			}
			case X3D::COMPLETE_STATE:
			{
				loadStateLabel .set_text (_ ("COMPLETE"));
				break;
			}
			case X3D::FAILED_STATE:
			{
				loadStateLabel .set_text (_ ("FAILED"));
				break;
			}
		}
	}
	else
		loadStateLabel .set_text (_ ("NOT STARTED"));
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
		set_camera (texture2DNode -> getImageWidth (), texture2DNode -> getImageHeight ());

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
