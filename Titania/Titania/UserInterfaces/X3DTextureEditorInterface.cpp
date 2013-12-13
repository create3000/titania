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
#include "X3DTextureEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DTextureEditorInterface::m_widgetName = "TextureEditor";

void
X3DTextureEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_alphaAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AlphaAdjustment"));
	m_anisotropicDegreeAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AnisotropicDegreeAdjustment"));
	m_borderWidthAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BorderWidthAdjustment"));
	m_boundryModeListStore         = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("BoundryModeListStore"));
	m_functionListStore            = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("FunctionListStore"));
	m_magnificationFilterListStore = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("MagnificationFilterListStore"));
	m_minificationFilterListStore  = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("MinificationFilterListStore"));
	m_modeListStore                = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ModeListStore"));
	m_rotationAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("RotationAdjustment"));
	m_scaleSAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ScaleSAdjustment"));
	m_scaleTdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ScaleTdjustment"));
	m_sourceListStore              = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("SourceListStore"));
	m_textureCompressionListStore  = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("TextureCompressionListStore"));
	m_texturePriorityAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePriorityAdjustment"));
	m_textureUnitListStore         = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("TextureUnitListStore"));
	m_translateSAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TranslateSAdjustment"));
	m_translateTAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TranslateTAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_window);
	m_window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_widget);
	m_widget -> set_name ("Widget");
	m_builder -> get_widget ("CopyMenuItem", m_copyMenuItem);
	m_copyMenuItem -> set_name ("CopyMenuItem");
	m_builder -> get_widget ("PasteMenuItem", m_pasteMenuItem);
	m_pasteMenuItem -> set_name ("PasteMenuItem");
	m_builder -> get_widget ("PreviewBox", m_previewBox);
	m_previewBox -> set_name ("PreviewBox");
	m_builder -> get_widget ("MultiTextureExpander", m_multiTextureExpander);
	m_multiTextureExpander -> set_name ("MultiTextureExpander");
	m_builder -> get_widget ("TextureExpander", m_textureExpander);
	m_textureExpander -> set_name ("TextureExpander");
	m_builder -> get_widget ("TextureGrid", m_textureGrid);
	m_textureGrid -> set_name ("TextureGrid");
	m_builder -> get_widget ("TexturePropertiesExpander", m_texturePropertiesExpander);
	m_texturePropertiesExpander -> set_name ("TexturePropertiesExpander");
	m_builder -> get_widget ("TextureTransformExpander", m_textureTransformExpander);
	m_textureTransformExpander -> set_name ("TextureTransformExpander");
	m_builder -> get_widget ("TextureTranformGrid", m_textureTranformGrid);
	m_textureTranformGrid -> set_name ("TextureTranformGrid");
	m_builder -> get_widget ("TranslateSButton", m_translateSButton);
	m_translateSButton -> set_name ("TranslateSButton");
	m_builder -> get_widget ("TranslateTButton", m_translateTButton);
	m_translateTButton -> set_name ("TranslateTButton");
	m_builder -> get_widget ("RotationScale", m_rotationScale);
	m_rotationScale -> set_name ("RotationScale");
	m_builder -> get_widget ("ScaleSButton", m_scaleSButton);
	m_scaleSButton -> set_name ("ScaleSButton");
	m_builder -> get_widget ("ScaleTButton", m_scaleTButton);
	m_scaleTButton -> set_name ("ScaleTButton");

	// Call construct handler of base class.
	construct ();
}

X3DTextureEditorInterface::~X3DTextureEditorInterface ()
{
	delete m_window;
}

} // puck
} // titania
