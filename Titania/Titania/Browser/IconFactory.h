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

#ifndef __TITANIA_BROWSER_ICON_FACTORY_H__
#define __TITANIA_BROWSER_ICON_FACTORY_H__

#include "../Base/X3DBaseInterface.h"
#include "../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Types/Pointer.h>

#include <gtkmm.h>

#include <string>

namespace titania {
namespace puck {

class IconFactory :
	public X3DBaseInterface
{
public:

	///  @name Construction

	IconFactory (X3DBrowserWindow* const browserWindow);

	void
	initialize ();

	const X3D::Output &
	initialized () const
	{ return initializedOutput; }

	///  @name Operations

	Gtk::IconSize
	getIconSize (const std::string & name, const int32_t width, const int32_t height);

	void
	createIcon (const std::string & name, const std::string & document);

	void
	createIcon (const std::string & name, Magick::Image && image);

	void
	createIcon (const std::string & name, const Cairo::RefPtr <Cairo::ImageSurface> & surface);

	std::string
	getIcon (const basic::uri & uri, const Gtk::IconSize & iconSize);

	void
	createFontIcon (const std::string & stockId, const int32_t width, const int32_t height, const X3D::X3DPtr <X3D::X3DFontStyleNode> & fontStyleNode);

	void
	createMaterialIcon (const std::string & stockId, const int32_t width, const int32_t height, const X3D::X3DPtr <X3D::X3DMaterialNode> & materialNode);
	
	void
	createTextureIcon (const std::string & stockId, const int32_t width, const int32_t height, const X3D::X3DPtr <X3D::X3DTextureNode> & textureNode);

	///  @name Destruction

	~IconFactory ();


private:

	///  @name Construction
	
	void
	set_material_preview ();
	
	void
	set_texture_preview ();
	
	void
	set_initialized ();

	///  @name Member access

	const Glib::RefPtr <Gtk::IconFactory> &
	getIconFactory () const
	{ return iconFactory; }

	Cairo::ErrorStatus
	write_to_png_stream (const unsigned char* data, unsigned int length, std::ostringstream & osstream);

	void
	set_camera (const X3D::X3DPtr <X3D::X3DTextureNode> & textureNode, const int32_t width, const int32_t height);

	void
	set_camera (double width, double height);

	///  @name Members

	Glib::RefPtr <Gtk::IconFactory> iconFactory;
	X3D::BrowserPtr                 materialPreview;
	X3D::BrowserPtr                 texturePreview;
	X3D::Output                     initializedOutput;

};

} // puck
} // titania

#endif
