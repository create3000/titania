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
	m_AlphaAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AlphaAdjustment"));
	m_AnisotropicDegreeAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AnisotropicDegreeAdjustment"));
	m_BorderWidthAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BorderWidthAdjustment"));
	m_BoundryModeListStore         = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("BoundryModeListStore"));
	m_FunctionListStore            = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("FunctionListStore"));
	m_MagnificationFilterListStore = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("MagnificationFilterListStore"));
	m_MinificationFilterListStore  = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("MinificationFilterListStore"));
	m_ModeListStore                = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ModeListStore"));
	m_RotationAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("RotationAdjustment"));
	m_ScaleSAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ScaleSAdjustment"));
	m_ScaleTdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ScaleTdjustment"));
	m_SourceListStore              = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("SourceListStore"));
	m_TextureCompressionListStore  = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("TextureCompressionListStore"));
	m_TexturePriorityAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePriorityAdjustment"));
	m_TextureUnitListStore         = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("TextureUnitListStore"));
	m_TranslateSAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TranslateSAdjustment"));
	m_TranslateTAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TranslateTAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("CopyMenuItem", m_CopyMenuItem);
	m_CopyMenuItem -> set_name ("CopyMenuItem");
	m_builder -> get_widget ("PasteMenuItem", m_PasteMenuItem);
	m_PasteMenuItem -> set_name ("PasteMenuItem");
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_PreviewBox -> set_name ("PreviewBox");
	m_builder -> get_widget ("MultiTextureExpander", m_MultiTextureExpander);
	m_MultiTextureExpander -> set_name ("MultiTextureExpander");
	m_builder -> get_widget ("TextureExpander", m_TextureExpander);
	m_TextureExpander -> set_name ("TextureExpander");
	m_builder -> get_widget ("TextureGrid", m_TextureGrid);
	m_TextureGrid -> set_name ("TextureGrid");
	m_builder -> get_widget ("TexturePropertiesExpander", m_TexturePropertiesExpander);
	m_TexturePropertiesExpander -> set_name ("TexturePropertiesExpander");
	m_builder -> get_widget ("TextureTransformExpander", m_TextureTransformExpander);
	m_TextureTransformExpander -> set_name ("TextureTransformExpander");
	m_builder -> get_widget ("TextureTranformGrid", m_TextureTranformGrid);
	m_TextureTranformGrid -> set_name ("TextureTranformGrid");
	m_builder -> get_widget ("TranslateSButton", m_TranslateSButton);
	m_TranslateSButton -> set_name ("TranslateSButton");
	m_builder -> get_widget ("TranslateTButton", m_TranslateTButton);
	m_TranslateTButton -> set_name ("TranslateTButton");
	m_builder -> get_widget ("RotationScale", m_RotationScale);
	m_RotationScale -> set_name ("RotationScale");
	m_builder -> get_widget ("ScaleSButton", m_ScaleSButton);
	m_ScaleSButton -> set_name ("ScaleSButton");
	m_builder -> get_widget ("ScaleTButton", m_ScaleTButton);
	m_ScaleTButton -> set_name ("ScaleTButton");

	// Call construct handler of base class.
	construct ();
}

X3DTextureEditorInterface::~X3DTextureEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
