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
#ifndef __TMP_GLAD2CPP_SOUND_EDITOR_H__
#define __TMP_GLAD2CPP_SOUND_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for SoundEditor.
 */
class X3DSoundEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DSoundEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DSoundEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	virtual
	const std::string &
	getWidgetName () const
	{ return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundDirectionXAdjustment () const
	{ return m_SoundDirectionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundDirectionYAdjustment () const
	{ return m_SoundDirectionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundDirectionZAdjustment () const
	{ return m_SoundDirectionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundIntensityAdjustment () const
	{ return m_SoundIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundLocationXAdjustment () const
	{ return m_SoundLocationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundLocationYAdjustment () const
	{ return m_SoundLocationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundLocationZAdjustment () const
	{ return m_SoundLocationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundMaxBackAdjustment () const
	{ return m_SoundMaxBackAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundMaxFrontAdjustment () const
	{ return m_SoundMaxFrontAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundMinBackAdjustment () const
	{ return m_SoundMinBackAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundMinFrontAdjustment () const
	{ return m_SoundMinFrontAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundPriorityAdjustment () const
	{ return m_SoundPriorityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourceCycleTimeAdjustment () const
	{ return m_SoundSourceCycleTimeAdjustment; }

	const Glib::RefPtr <Gtk::TextBuffer> &
	getSoundSourceDescriptionTextBuffer () const
	{ return m_SoundSourceDescriptionTextBuffer; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourceDurationAdjustment () const
	{ return m_SoundSourceDurationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourceElapsedTimeAdjustment () const
	{ return m_SoundSourceElapsedTimeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourcePauseTimeAdjustment () const
	{ return m_SoundSourcePauseTimeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourcePitchAdjustment () const
	{ return m_SoundSourcePitchAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourceResumeTimeAdjustment () const
	{ return m_SoundSourceResumeTimeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourceSpeedAdjustment () const
	{ return m_SoundSourceSpeedAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourceStartTimeAdjustment () const
	{ return m_SoundSourceStartTimeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundSourceStopTimeAdjustment () const
	{ return m_SoundSourceStopTimeAdjustment; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getSoundSourceURLCellRendererText () const
	{ return m_SoundSourceURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getSoundSourceURLChooserColumn () const
	{ return m_SoundSourceURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getSoundSourceURLCellrendererPixbuf () const
	{ return m_SoundSourceURLCellrendererPixbuf; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getSoundActionBox () const
	{ return *m_SoundActionBox; }

	Gtk::Button &
	getNewSoundButton () const
	{ return *m_NewSoundButton; }

	Gtk::Button &
	getRemoveSoundButton () const
	{ return *m_RemoveSoundButton; }

	Gtk::Button &
	getIndexButton () const
	{ return *m_IndexButton; }

	Gtk::Notebook &
	getSoundNotebook () const
	{ return *m_SoundNotebook; }

	Gtk::Expander &
	getSoundExpander () const
	{ return *m_SoundExpander; }

	Gtk::Grid &
	getSoundBox () const
	{ return *m_SoundBox; }

	Gtk::CheckButton &
	getSoundSpatializeCheckButton () const
	{ return *m_SoundSpatializeCheckButton; }

	Gtk::Box &
	getSoundNameBox () const
	{ return *m_SoundNameBox; }

	Gtk::Entry &
	getSoundNameEntry () const
	{ return *m_SoundNameEntry; }

	Gtk::Button &
	getSoundRenameButton () const
	{ return *m_SoundRenameButton; }

	Gtk::Box &
	getSoundIntensityBox () const
	{ return *m_SoundIntensityBox; }

	Gtk::Scale &
	getSoundIntensityScale () const
	{ return *m_SoundIntensityScale; }

	Gtk::Box &
	getSoundLocationBox () const
	{ return *m_SoundLocationBox; }

	Gtk::Box &
	getSoundNormalToolBox () const
	{ return *m_SoundNormalToolBox; }

	Gtk::Box &
	getSoundDirectionBox () const
	{ return *m_SoundDirectionBox; }

	Gtk::SpinButton &
	getSoundDirectionXSpinButton () const
	{ return *m_SoundDirectionXSpinButton; }

	Gtk::SpinButton &
	getSoundDirectionYSpinButton () const
	{ return *m_SoundDirectionYSpinButton; }

	Gtk::SpinButton &
	getSoundDirectionZSpinButton () const
	{ return *m_SoundDirectionZSpinButton; }

	Gtk::Box &
	getSoundPriorityBox () const
	{ return *m_SoundPriorityBox; }

	Gtk::Scale &
	getSoundPriorityScale () const
	{ return *m_SoundPriorityScale; }

	Gtk::SpinButton &
	getSoundMinBackSpinButton () const
	{ return *m_SoundMinBackSpinButton; }

	Gtk::SpinButton &
	getSoundMinFrontSpinButton () const
	{ return *m_SoundMinFrontSpinButton; }

	Gtk::SpinButton &
	getSoundMaxBackSpinButton () const
	{ return *m_SoundMaxBackSpinButton; }

	Gtk::SpinButton &
	getSoundMaxFrontSpinButton () const
	{ return *m_SoundMaxFrontSpinButton; }

	Gtk::Expander &
	getSoundSourceExpander () const
	{ return *m_SoundSourceExpander; }

	Gtk::Box &
	getSelectSoundSourceBox () const
	{ return *m_SelectSoundSourceBox; }

	Gtk::ComboBoxText &
	getSoundSourceComboBoxText () const
	{ return *m_SoundSourceComboBoxText; }

	Gtk::Button &
	getSoundSourceUnlinkButton () const
	{ return *m_SoundSourceUnlinkButton; }

	Gtk::Grid &
	getSoundSourceNodeBox () const
	{ return *m_SoundSourceNodeBox; }

	Gtk::CheckButton &
	getSoundSourceEnabledCheckButton () const
	{ return *m_SoundSourceEnabledCheckButton; }

	Gtk::TextView &
	getSoundSourceDescriptionTextView () const
	{ return *m_SoundSourceDescriptionTextView; }

	Gtk::Box &
	getSoundSourceURLBox () const
	{ return *m_SoundSourceURLBox; }

	Gtk::TreeView &
	getSoundSourceURLTreeView () const
	{ return *m_SoundSourceURLTreeView; }

	Gtk::Button &
	getSoundSourceURLAddButton () const
	{ return *m_SoundSourceURLAddButton; }

	Gtk::Button &
	getSoundSourceURLRemoveButton () const
	{ return *m_SoundSourceURLRemoveButton; }

	Gtk::Button &
	getSoundSourceURLReloadButton () const
	{ return *m_SoundSourceURLReloadButton; }

	Gtk::SpinButton &
	getSoundSourceSpeedSpinButton () const
	{ return *m_SoundSourceSpeedSpinButton; }

	Gtk::SpinButton &
	getSoundSourcePitchSpinButton () const
	{ return *m_SoundSourcePitchSpinButton; }

	Gtk::CheckButton &
	getSoundSourceLoopCheckButton () const
	{ return *m_SoundSourceLoopCheckButton; }

	Gtk::CheckButton &
	getSoundSourceIsActiveCheckButton () const
	{ return *m_SoundSourceIsActiveCheckButton; }

	Gtk::CheckButton &
	getSoundSourceIsPausedCheckButton () const
	{ return *m_SoundSourceIsPausedCheckButton; }

	Gtk::SpinButton &
	getSoundSourceCycleTimeSpinButton () const
	{ return *m_SoundSourceCycleTimeSpinButton; }

	Gtk::SpinButton &
	getSoundSourceElapsedTimeSpinButton () const
	{ return *m_SoundSourceElapsedTimeSpinButton; }

	Gtk::SpinButton &
	getSoundSourceDurationSpinButton () const
	{ return *m_SoundSourceDurationSpinButton; }

	Gtk::Box &
	getSoundSourceStartTimeBox () const
	{ return *m_SoundSourceStartTimeBox; }

	Gtk::SpinButton &
	getSoundSourceStartTimeSpinButton () const
	{ return *m_SoundSourceStartTimeSpinButton; }

	Gtk::Button &
	getSoundSourceStartTimeButton () const
	{ return *m_SoundSourceStartTimeButton; }

	Gtk::Box &
	getSoundSourceResumeTimeBox () const
	{ return *m_SoundSourceResumeTimeBox; }

	Gtk::SpinButton &
	getSoundSourceResumeTimeSpinButton () const
	{ return *m_SoundSourceResumeTimeSpinButton; }

	Gtk::Button &
	getSoundSourceResumeTimeButton () const
	{ return *m_SoundSourceResumeTimeButton; }

	Gtk::Box &
	getSoundSourcePauseTimeBox () const
	{ return *m_SoundSourcePauseTimeBox; }

	Gtk::SpinButton &
	getSoundSourcePauseTimeSpinButton () const
	{ return *m_SoundSourcePauseTimeSpinButton; }

	Gtk::Button &
	getSoundSourcePauseTimeButton () const
	{ return *m_SoundSourcePauseTimeButton; }

	Gtk::Box &
	getSoundSourceStopTimeBox () const
	{ return *m_SoundSourceStopTimeBox; }

	Gtk::SpinButton &
	getSoundSourceStopTimeSpinButton () const
	{ return *m_SoundSourceStopTimeSpinButton; }

	Gtk::Button &
	getSoundSourceStopTimeButton () const
	{ return *m_SoundSourceStopTimeButton; }

	///  @name Signal handlers

	virtual
	void
	on_new_sound_clicked () = 0;

	virtual
	void
	on_remove_sound_clicked () = 0;

	virtual
	void
	on_index_clicked () = 0;

	virtual
	void
	on_sound_source_changed () = 0;

	virtual
	void
	on_sound_source_unlink_clicked () = 0;

	virtual
	void
	on_sound_source_start_time_clicked () = 0;

	virtual
	void
	on_sound_source_resume_time_clicked () = 0;

	virtual
	void
	on_sound_source_pause_time_clicked () = 0;

	virtual
	void
	on_sound_source_stop_time_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DSoundEditorInterface () override;


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ X3DEditorInterface::construct (); }

	void
	create (const std::string &);

	///  @name Static members

	static const std::string m_widgetName;

	///  @name Members

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundDirectionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundDirectionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundDirectionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundLocationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundLocationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundLocationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundMaxBackAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundMaxFrontAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundMinBackAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundMinFrontAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundPriorityAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourceCycleTimeAdjustment;
	Glib::RefPtr <Gtk::TextBuffer>         m_SoundSourceDescriptionTextBuffer;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourceDurationAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourceElapsedTimeAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourcePauseTimeAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourcePitchAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourceResumeTimeAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourceSpeedAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourceStartTimeAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SoundSourceStopTimeAdjustment;
	Glib::RefPtr <Gtk::CellRendererText>   m_SoundSourceURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_SoundSourceURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_SoundSourceURLCellrendererPixbuf;
	Gtk::Window*                           m_Window;
	Gtk::Box*                              m_Widget;
	Gtk::Box*                              m_SoundActionBox;
	Gtk::Button*                           m_NewSoundButton;
	Gtk::Button*                           m_RemoveSoundButton;
	Gtk::Button*                           m_IndexButton;
	Gtk::Notebook*                         m_SoundNotebook;
	Gtk::Expander*                         m_SoundExpander;
	Gtk::Grid*                             m_SoundBox;
	Gtk::CheckButton*                      m_SoundSpatializeCheckButton;
	Gtk::Box*                              m_SoundNameBox;
	Gtk::Entry*                            m_SoundNameEntry;
	Gtk::Button*                           m_SoundRenameButton;
	Gtk::Box*                              m_SoundIntensityBox;
	Gtk::Scale*                            m_SoundIntensityScale;
	Gtk::Box*                              m_SoundLocationBox;
	Gtk::Box*                              m_SoundNormalToolBox;
	Gtk::Box*                              m_SoundDirectionBox;
	Gtk::SpinButton*                       m_SoundDirectionXSpinButton;
	Gtk::SpinButton*                       m_SoundDirectionYSpinButton;
	Gtk::SpinButton*                       m_SoundDirectionZSpinButton;
	Gtk::Box*                              m_SoundPriorityBox;
	Gtk::Scale*                            m_SoundPriorityScale;
	Gtk::SpinButton*                       m_SoundMinBackSpinButton;
	Gtk::SpinButton*                       m_SoundMinFrontSpinButton;
	Gtk::SpinButton*                       m_SoundMaxBackSpinButton;
	Gtk::SpinButton*                       m_SoundMaxFrontSpinButton;
	Gtk::Expander*                         m_SoundSourceExpander;
	Gtk::Box*                              m_SelectSoundSourceBox;
	Gtk::ComboBoxText*                     m_SoundSourceComboBoxText;
	Gtk::Button*                           m_SoundSourceUnlinkButton;
	Gtk::Grid*                             m_SoundSourceNodeBox;
	Gtk::CheckButton*                      m_SoundSourceEnabledCheckButton;
	Gtk::TextView*                         m_SoundSourceDescriptionTextView;
	Gtk::Box*                              m_SoundSourceURLBox;
	Gtk::TreeView*                         m_SoundSourceURLTreeView;
	Gtk::Button*                           m_SoundSourceURLAddButton;
	Gtk::Button*                           m_SoundSourceURLRemoveButton;
	Gtk::Button*                           m_SoundSourceURLReloadButton;
	Gtk::SpinButton*                       m_SoundSourceSpeedSpinButton;
	Gtk::SpinButton*                       m_SoundSourcePitchSpinButton;
	Gtk::CheckButton*                      m_SoundSourceLoopCheckButton;
	Gtk::CheckButton*                      m_SoundSourceIsActiveCheckButton;
	Gtk::CheckButton*                      m_SoundSourceIsPausedCheckButton;
	Gtk::SpinButton*                       m_SoundSourceCycleTimeSpinButton;
	Gtk::SpinButton*                       m_SoundSourceElapsedTimeSpinButton;
	Gtk::SpinButton*                       m_SoundSourceDurationSpinButton;
	Gtk::Box*                              m_SoundSourceStartTimeBox;
	Gtk::SpinButton*                       m_SoundSourceStartTimeSpinButton;
	Gtk::Button*                           m_SoundSourceStartTimeButton;
	Gtk::Box*                              m_SoundSourceResumeTimeBox;
	Gtk::SpinButton*                       m_SoundSourceResumeTimeSpinButton;
	Gtk::Button*                           m_SoundSourceResumeTimeButton;
	Gtk::Box*                              m_SoundSourcePauseTimeBox;
	Gtk::SpinButton*                       m_SoundSourcePauseTimeSpinButton;
	Gtk::Button*                           m_SoundSourcePauseTimeButton;
	Gtk::Box*                              m_SoundSourceStopTimeBox;
	Gtk::SpinButton*                       m_SoundSourceStopTimeSpinButton;
	Gtk::Button*                           m_SoundSourceStopTimeButton;

};

} // puck
} // titania

#endif
