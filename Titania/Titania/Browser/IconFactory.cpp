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

#include "IconFactory.h"

#include "../Browser/Image.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../Configuration/config.h"

#include <Titania/X3D/Bits/Error.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Layout/ScreenFontStyle.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Material.h>
#include <Titania/X3D/Components/Shape/TwoSidedMaterial.h>
#include <Titania/X3D/Components/Text/Text.h>
#include <Titania/X3D/Components/Text/X3DFontStyleNode.h>
#include <Titania/X3D/Components/Texturing/X3DTexture2DNode.h>
#include <Titania/X3D/InputOutput/FileLoader.h>

namespace titania {
namespace puck {

IconFactory::IconFactory (X3DBrowserWindow* const browserWindow) :
	 X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	      iconFactory (Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (browserWindow -> getBuilder () -> get_object ("IconFactory"))),
	  materialPreview (X3D::createBrowser (browserWindow -> getMasterBrowser (), { get_ui ("Editors/MaterialEditorPreview.x3dv") + "#CloseViewpoint" })),
	   texturePreview (X3D::createBrowser (browserWindow -> getMasterBrowser (), { get_ui ("Editors/TexturePreview.x3dv") })),
	initializedOutput ()
{
	addChildObjects (materialPreview,
	                 texturePreview);

	setup ();
}

void
IconFactory::initialize ()
{
	// Off-Screen Browser

	materialPreview -> initialized () .addInterest (&IconFactory::set_material_preview, this);
	materialPreview -> setFixedPipeline (false);
	materialPreview -> setup ();

	texturePreview -> initialized () .addInterest (&IconFactory::set_texture_preview, this);
	texturePreview -> setFixedPipeline (false);
	texturePreview -> setup ();
}

void
IconFactory::set_material_preview ()
{
	try
	{
		materialPreview -> getExecutionContext () -> getNamedNode ("Appearance")     -> setPrivate (true);
		materialPreview -> getExecutionContext () -> getNamedNode ("LineAppearance") -> setPrivate (true);

		set_initialized ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
IconFactory::set_texture_preview ()
{
	try
	{
		texturePreview -> getExecutionContext () -> getNamedNode ("Appearance")    -> setPrivate (true);
		texturePreview -> getExecutionContext () -> getNamedNode ("TextureScript") -> setPrivate (true);

		set_initialized ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
IconFactory::set_initialized ()
{
	if (not materialPreview -> initialized ())
		return;

	if (not texturePreview -> initialized ())
		return;

	initializedOutput .processInterests ();
}

Gtk::IconSize
IconFactory::getIconSize (const std::string & name, const int32_t width, const int32_t height)
{
	const auto iconSize = Gtk::IconSize::from_name (name);

	if (iconSize != Gtk::ICON_SIZE_INVALID)
		return iconSize;

	return Gtk::IconSize::register_new (name, width, height);
}

void
IconFactory::createIcon (const std::string & name, const std::string & document)
{
	Glib::RefPtr <Gtk::IconSet> iconSet;

	if (not document .empty ())
	{
		try
		{
			const Image icon (document);

			iconSet = Gtk::IconSet::create (icon .getIcon ());
		}
		catch (const std::exception & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	const Gtk::StockID stockId = Gtk::StockID (name);

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, name));
}

void
IconFactory::createIcon (const std::string & name, Magick::Image && image)
{
	Glib::RefPtr <Gtk::IconSet> iconSet;

	try
	{
		const Image icon (std::move (image));

		iconSet = Gtk::IconSet::create (icon .getIcon ());
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	const Gtk::StockID stockId = Gtk::StockID (name);

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, name));
}

void
IconFactory::createIcon (const std::string & name, const Cairo::RefPtr <Cairo::ImageSurface> & surface)
{
	std::ostringstream osstream;

	surface -> write_to_png_stream (sigc::bind (sigc::mem_fun (this, &IconFactory::write_to_png_stream), sigc::ref (osstream)));

	createIcon (name, osstream .str ());
}

Cairo::ErrorStatus
IconFactory::write_to_png_stream (const unsigned char* data, unsigned int length, std::ostringstream & osstream)
{
	osstream .write (reinterpret_cast <const char*> (data), length);

	return CAIRO_STATUS_SUCCESS;
}

std::string
IconFactory::getIcon (const basic::uri & worldURL, const Gtk::IconSize & iconSize)
{
	std::string image;
	auto        iconSet = Gtk::IconSet::lookup_default (Gtk::StockID (worldURL .filename () .str ()));

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	if (iconSet)
	{
		try
		{
			const auto pixbuf = iconSet -> render_icon_pixbuf (getBrowserWindow () -> getWidget () .get_style_context (), iconSize);

			if (pixbuf)
			{
				gchar* buffer;
				gsize  bufferSize;

				pixbuf -> save_to_buffer (buffer, bufferSize);

				image = std::string (buffer, bufferSize);

				g_free (buffer);
			}
		}
		catch (const Glib::Exception &)
		{ }
	}

	return image;
}

void
IconFactory::createFontIcon (const std::string & stockId, const int32_t width, const int32_t height, const X3D::X3DPtr <X3D::X3DFontStyleNode> & fontStyleNode)
{
	const auto   fontStyle     = fontStyleNode ? fontStyleNode : getCurrentBrowser () -> getDefaultFontStyle ();
	const auto   previewString = _ ("The quick brown fox jumps over the lazy dog.");
	const auto   padX          = 30;
	const auto   surface       = Cairo::ImageSurface::create (Cairo::FORMAT_RGB24, width, height);
	const auto   context       = Cairo::Context::create (surface);
	const auto & font          = fontStyle -> getFont ();
	const auto   fontFace      = Cairo::FtFontFace::create (font .getPattern () .get ());

	// Set font options.

	context -> set_font_face (fontFace);
	context -> set_font_size (height / 2);

	Cairo::FontOptions options;
	options .set_hint_style (Cairo::HINT_STYLE_MEDIUM);

	context -> set_font_options (options);

	// Fill background.

	context -> rectangle (0, 0, width, height);
	context -> set_source_rgb (1, 1, 1);
	context -> fill ();

	// Get text extents.

	Cairo::TextExtents textExtents;

	context -> get_text_extents (previewString, textExtents);

	const auto min = X3D::Vector2d (textExtents .x_bearing, -textExtents .y_bearing - textExtents .height);
	const auto max = min + X3D::Vector2d (textExtents .width, textExtents .height);

	// Draw text.

	const auto scale = (width - padX * 2) / max .x ();
	const auto x     = padX;
	const auto y     = max .y () * scale + (height - max .y () * scale) / 2;

	context -> set_source_rgb (0, 0, 0);
	context -> move_to (x, y);
	context -> scale (scale, scale);
	context -> show_text (previewString);

	// Create Icon.

	createIcon (stockId, surface);
}

void
IconFactory::createMaterialIcon (const std::string & stockId, const int32_t width, const int32_t height, const X3D::X3DPtr <X3D::X3DMaterialNode> & materialNode)
{
	// Configure scene.

	const auto material         = X3D::X3DPtr <X3D::Material> (materialNode);
	const auto twoSidedMaterial = X3D::X3DPtr <X3D::TwoSidedMaterial> (materialNode);
	const auto appearance       = materialPreview -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

	if (not (material or twoSidedMaterial) or not appearance)
		throw X3D::Error <X3D::INVALID_NODE> ("IconFactory::createMaterialIcon");

	if (material)
		appearance -> material () = material;

	else if (twoSidedMaterial)
		appearance -> material () = twoSidedMaterial;

	const auto backMaterial = materialPreview -> getExecutionContext () -> getNamedNode <X3D::Material> ("BackMaterial");

	if (backMaterial and twoSidedMaterial)
	{
		backMaterial -> ambientIntensity () = twoSidedMaterial -> backAmbientIntensity ();
		backMaterial -> diffuseColor ()     = twoSidedMaterial -> backDiffuseColor ();
		backMaterial -> specularColor ()    = twoSidedMaterial -> backSpecularColor ();
		backMaterial -> emissiveColor ()    = twoSidedMaterial -> backEmissiveColor ();
		backMaterial -> shininess ()        = twoSidedMaterial -> backShininess ();
		backMaterial -> transparency ()     = twoSidedMaterial -> backTransparency ();
	}

	const auto sphere = materialPreview -> getExecutionContext () -> getNamedNode <X3D::Switch> ("Sphere");

	sphere -> whichChoice () = twoSidedMaterial;

	// Create Icon.

	createIcon (stockId, materialPreview -> getSnapshot (width, height, false, 8));
}

void
IconFactory::createTextureIcon (const std::string & stockId, const int32_t width, const int32_t height, const X3D::X3DPtr <X3D::X3DTextureNode> & textureNode)
{
	// Configure scene.

	const auto appearance = texturePreview -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

	appearance -> texture () = textureNode;

	set_camera (textureNode, width, height);

	// Create Icon.

	createIcon (stockId, texturePreview -> getSnapshot (width, height, false, 8));
}

void
IconFactory::set_camera (const X3D::X3DPtr <X3D::X3DTextureNode> & textureNode, const int32_t width, const int32_t height)
{
	const X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode (textureNode);

	if (texture2DNode)
		set_camera (texture2DNode -> width (), texture2DNode -> height ());

	else
		set_camera (width, height);
}

void
IconFactory::set_camera (double width, double height)
{
	const X3D::X3DPtr <X3D::OrthoViewpoint> viewpoint (texturePreview -> getExecutionContext () -> getNamedNode ("Texture2DViewpoint"));
	const X3D::X3DPtr <X3D::Transform>      transform (texturePreview -> getExecutionContext () -> getNamedNode ("Texture2D"));

	if (not width or not height)
	{
		width  = 1;
		height = 1;
	}

	viewpoint -> fieldOfView () = { -width, -height, width, height };
	transform -> scale ()       = X3D::Vector3f (width, height, 1);
}

IconFactory::~IconFactory ()
{
	dispose ();
}

} // puck
} // titania
