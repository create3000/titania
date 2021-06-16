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

#include "X3DRenderPanelInterface.h"

namespace titania {
namespace puck {

void
X3DRenderPanelInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DRenderPanelInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DRenderPanelInterface::create ()
{
	// Get objects.
	m_AntialiasingAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AntialiasingAdjustment"));
	m_DurationAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DurationAdjustment"));
	m_FrameAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FrameAdjustment"));
	m_FrameRateAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FrameRateAdjustment"));
	m_HeightAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("HeightAdjustment"));
	m_WidthAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("WidthAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("MenuBar", m_MenuBar);
	m_builder -> get_widget ("PanelsMenuItem", m_PanelsMenuItem);
	m_builder -> get_widget ("RecordButton", m_RecordButton);
	m_builder -> get_widget ("LoopButton", m_LoopButton);
	m_builder -> get_widget ("PlayPauseButton", m_PlayPauseButton);
	m_builder -> get_widget ("StopButton", m_StopButton);
	m_builder -> get_widget ("FrameSpinButton", m_FrameSpinButton);
	m_builder -> get_widget ("ElapsedTimeLabel", m_ElapsedTimeLabel);
	m_builder -> get_widget ("DurationLabel", m_DurationLabel);
	m_builder -> get_widget ("LoadStateBox", m_LoadStateBox);
	m_builder -> get_widget ("LoadStateLabel", m_LoadStateLabel);
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_builder -> get_widget ("TextureFormatLabel", m_TextureFormatLabel);
	m_builder -> get_widget ("TextureLoadStateLabel", m_TextureLoadStateLabel);
	m_builder -> get_widget ("PropertiesDialog", m_PropertiesDialog);
	m_builder -> get_widget ("PropertiesCancelButton", m_PropertiesCancelButton);
	m_builder -> get_widget ("PropertiesOkButton", m_PropertiesOkButton);
	m_builder -> get_widget ("PropertiesHeaderBar", m_PropertiesHeaderBar);
	m_builder -> get_widget ("DurationSspinButton", m_DurationSspinButton);
	m_builder -> get_widget ("TimeLabel", m_TimeLabel);
	m_builder -> get_widget ("AntialiasingBox", m_AntialiasingBox);
	m_builder -> get_widget ("FileChooserButton", m_FileChooserButton);
	m_builder -> get_widget ("FileLabel", m_FileLabel);
	m_builder -> get_widget ("FileChooserButton1", m_FileChooserButton1);
	m_builder -> get_widget ("ViewpointLabel", m_ViewpointLabel);
	m_builder -> get_widget ("SizesButton", m_SizesButton);
	m_builder -> get_widget ("CodecButton", m_CodecButton);
	m_builder -> get_widget ("FPSSpinButton", m_FPSSpinButton);
	m_builder -> get_widget ("ShadingButton", m_ShadingButton);

	// Connect object Gtk::Adjustment with id 'DurationAdjustment'.
	m_DurationAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DRenderPanelInterface::on_properties_time_changed));

	// Connect object Gtk::ToolButton with id 'RecordButton'.
	m_RecordButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRenderPanelInterface::on_record_clicked));

	// Connect object Gtk::ToggleToolButton with id 'LoopButton'.
	m_LoopButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DRenderPanelInterface::on_loop_toggled));

	// Connect object Gtk::ToolButton with id 'PlayPauseButton'.
	m_PlayPauseButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRenderPanelInterface::on_play_pause_clicked));
	m_StopButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRenderPanelInterface::on_stop_clicked));

	// Connect object Gtk::Adjustment with id 'FrameRateAdjustment'.
	m_FrameRateAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DRenderPanelInterface::on_properties_time_changed));

	// Connect object Gtk::Button with id 'FileChooserButton'.
	m_FileChooserButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRenderPanelInterface::on_properties_file_chooser_button_clicked));
	m_FileChooserButton1 -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRenderPanelInterface::on_properties_viewpoint_chooser_button_clicked));
}

X3DRenderPanelInterface::~X3DRenderPanelInterface ()
{
	delete m_Window;
	delete m_PropertiesDialog;
}

} // puck
} // titania
