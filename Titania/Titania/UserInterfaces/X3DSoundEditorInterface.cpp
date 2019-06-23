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

#include "X3DSoundEditorInterface.h"

namespace titania {
namespace puck {

void
X3DSoundEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DSoundEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DSoundEditorInterface::create ()
{
	// Get objects.
	m_SoundDirectionXAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundDirectionXAdjustment"));
	m_SoundDirectionYAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundDirectionYAdjustment"));
	m_SoundDirectionZAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundDirectionZAdjustment"));
	m_SoundIntensityAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundIntensityAdjustment"));
	m_SoundLocationXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundLocationXAdjustment"));
	m_SoundLocationYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundLocationYAdjustment"));
	m_SoundLocationZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundLocationZAdjustment"));
	m_SoundMaxBackAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundMaxBackAdjustment"));
	m_SoundMaxFrontAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundMaxFrontAdjustment"));
	m_SoundMinBackAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundMinBackAdjustment"));
	m_SoundMinFrontAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundMinFrontAdjustment"));
	m_SoundPriorityAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundPriorityAdjustment"));
	m_SoundSourceCycleTimeAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourceCycleTimeAdjustment"));
	m_SoundSourceDescriptionTextBuffer = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("SoundSourceDescriptionTextBuffer"));
	m_SoundSourceDurationAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourceDurationAdjustment"));
	m_SoundSourceElapsedTimeAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourceElapsedTimeAdjustment"));
	m_SoundSourcePauseTimeAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourcePauseTimeAdjustment"));
	m_SoundSourcePitchAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourcePitchAdjustment"));
	m_SoundSourceResumeTimeAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourceResumeTimeAdjustment"));
	m_SoundSourceSpeedAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourceSpeedAdjustment"));
	m_SoundSourceURLCellRendererText   = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("SoundSourceURLCellRendererText"));
	m_SoundSourceURLChooserColumn      = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("SoundSourceURLChooserColumn"));
	m_SoundSourceURLCellrendererPixbuf = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("SoundSourceURLCellrendererPixbuf"));
	m_SoundSourceStartTimeAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourceStartTimeAdjustment"));
	m_SoundSourceStopTimeAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SoundSourceStopTimeAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("SoundActionBox", m_SoundActionBox);
	m_builder -> get_widget ("NewSoundButton", m_NewSoundButton);
	m_builder -> get_widget ("RemoveSoundButton", m_RemoveSoundButton);
	m_builder -> get_widget ("IndexButton", m_IndexButton);
	m_builder -> get_widget ("SoundNotebook", m_SoundNotebook);
	m_builder -> get_widget ("SoundExpander", m_SoundExpander);
	m_builder -> get_widget ("SoundBox", m_SoundBox);
	m_builder -> get_widget ("SoundSpatializeCheckButton", m_SoundSpatializeCheckButton);
	m_builder -> get_widget ("SoundNameBox", m_SoundNameBox);
	m_builder -> get_widget ("SoundNameEntry", m_SoundNameEntry);
	m_builder -> get_widget ("SoundRenameButton", m_SoundRenameButton);
	m_builder -> get_widget ("SoundIntensityBox", m_SoundIntensityBox);
	m_builder -> get_widget ("SoundIntensityScale", m_SoundIntensityScale);
	m_builder -> get_widget ("SoundLocationBox", m_SoundLocationBox);
	m_builder -> get_widget ("SoundNormalToolBox", m_SoundNormalToolBox);
	m_builder -> get_widget ("SoundDirectionBox", m_SoundDirectionBox);
	m_builder -> get_widget ("SoundDirectionXSpinButton", m_SoundDirectionXSpinButton);
	m_builder -> get_widget ("SoundDirectionYSpinButton", m_SoundDirectionYSpinButton);
	m_builder -> get_widget ("SoundDirectionZSpinButton", m_SoundDirectionZSpinButton);
	m_builder -> get_widget ("SoundPriorityBox", m_SoundPriorityBox);
	m_builder -> get_widget ("SoundPriorityScale", m_SoundPriorityScale);
	m_builder -> get_widget ("SoundMinBackSpinButton", m_SoundMinBackSpinButton);
	m_builder -> get_widget ("SoundMinFrontSpinButton", m_SoundMinFrontSpinButton);
	m_builder -> get_widget ("SoundMaxBackSpinButton", m_SoundMaxBackSpinButton);
	m_builder -> get_widget ("SoundMaxFrontSpinButton", m_SoundMaxFrontSpinButton);
	m_builder -> get_widget ("SoundSourceExpander", m_SoundSourceExpander);
	m_builder -> get_widget ("SelectSoundSourceBox", m_SelectSoundSourceBox);
	m_builder -> get_widget ("SoundSourceComboBoxText", m_SoundSourceComboBoxText);
	m_builder -> get_widget ("SoundSourceUnlinkButton", m_SoundSourceUnlinkButton);
	m_builder -> get_widget ("SoundSourceNodeBox", m_SoundSourceNodeBox);
	m_builder -> get_widget ("SoundSourceEnabledCheckButton", m_SoundSourceEnabledCheckButton);
	m_builder -> get_widget ("SoundSourceDescriptionTextView", m_SoundSourceDescriptionTextView);
	m_builder -> get_widget ("SoundSourceURLBox", m_SoundSourceURLBox);
	m_builder -> get_widget ("SoundSourceURLTreeView", m_SoundSourceURLTreeView);
	m_builder -> get_widget ("SoundSourceURLAddButton", m_SoundSourceURLAddButton);
	m_builder -> get_widget ("SoundSourceURLRemoveButton", m_SoundSourceURLRemoveButton);
	m_builder -> get_widget ("SoundSourceURLReloadButton", m_SoundSourceURLReloadButton);
	m_builder -> get_widget ("SoundSourceSpeedLabel", m_SoundSourceSpeedLabel);
	m_builder -> get_widget ("SoundSourceSpeedSpinButton", m_SoundSourceSpeedSpinButton);
	m_builder -> get_widget ("SoundSourcePitchSpinButton", m_SoundSourcePitchSpinButton);
	m_builder -> get_widget ("SoundSourceLoopToggleButton", m_SoundSourceLoopToggleButton);
	m_builder -> get_widget ("SoundSourcePlayPauseButton", m_SoundSourcePlayPauseButton);
	m_builder -> get_widget ("SoundSourcePlayPauseImage", m_SoundSourcePlayPauseImage);
	m_builder -> get_widget ("SoundSourceStopButton", m_SoundSourceStopButton);
	m_builder -> get_widget ("SoundSourceStopImage", m_SoundSourceStopImage);
	m_builder -> get_widget ("SoundSourceElapsedTimeLabel", m_SoundSourceElapsedTimeLabel);
	m_builder -> get_widget ("SoundSourceDurationLabel", m_SoundSourceDurationLabel);

	// Connect object Gtk::Button with id 'NewSoundButton'.
	m_NewSoundButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DSoundEditorInterface::on_new_sound_clicked));
	m_RemoveSoundButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DSoundEditorInterface::on_remove_sound_clicked));
	m_IndexButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DSoundEditorInterface::on_index_clicked));

	// Connect object Gtk::ComboBoxText with id 'SoundSourceComboBoxText'.
	m_SoundSourceComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DSoundEditorInterface::on_sound_source_changed));

	// Connect object Gtk::Button with id 'SoundSourceUnlinkButton'.
	m_SoundSourceUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DSoundEditorInterface::on_sound_source_unlink_clicked));
	m_SoundSourcePlayPauseButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DSoundEditorInterface::on_sound_source_play_pause_clicked));
	m_SoundSourceStopButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DSoundEditorInterface::on_sound_source_stop_clicked));
}

X3DSoundEditorInterface::~X3DSoundEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
