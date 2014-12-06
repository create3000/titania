/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
#ifndef __TMP_GLAD2CPP_ANIMATION_EDITOR_H__
#define __TMP_GLAD2CPP_ANIMATION_EDITOR_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DAnimationEditorInterface :
	public X3DUserInterface
{
public:

	X3DAnimationEditorInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DAnimationEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		        filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const Glib::ustring & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	void
	updateWidgets (const std::vector <Glib::ustring> & names) const
	{ getBuilder () -> add_from_file (filename, names); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::AccelGroup> &
	getAccelGroup () const
	{ return m_AccelGroup; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDurationAdjustment () const
	{ return m_DurationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFPSAdjustment () const
	{ return m_FPSAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFrameAdjustment () const
	{ return m_FrameAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTranslationAdjustment () const
	{ return m_TranslationAdjustment; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getNameColumn () const
	{ return m_NameColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getIconCellRenderer () const
	{ return m_IconCellRenderer; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getNameCellRenderer () const
	{ return m_NameCellRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getTaintedColumn () const
	{ return m_TaintedColumn; }

	const Glib::RefPtr <Gtk::CellRendererToggle> &
	getTaintedCellRenderer () const
	{ return m_TaintedCellRenderer; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::ToolButton &
	getNewButton () const
	{ return *m_NewButton; }

	Gtk::ToolButton &
	getOpenButton () const
	{ return *m_OpenButton; }

	Gtk::ToolButton &
	getAddObjectButton () const
	{ return *m_AddObjectButton; }

	Gtk::ToolButton &
	getCutButton () const
	{ return *m_CutButton; }

	Gtk::ToolButton &
	getCopyButton () const
	{ return *m_CopyButton; }

	Gtk::ToolButton &
	getPasteButton () const
	{ return *m_PasteButton; }

	Gtk::ToolButton &
	getFirstFrameButton () const
	{ return *m_FirstFrameButton; }

	Gtk::ToolButton &
	getPlayPauseButton () const
	{ return *m_PlayPauseButton; }

	Gtk::ToolButton &
	getLastFrameButton () const
	{ return *m_LastFrameButton; }

	Gtk::SpinButton &
	getFrameSpinButton () const
	{ return *m_FrameSpinButton; }

	Gtk::ToolButton &
	getTimeButton () const
	{ return *m_TimeButton; }

	Gtk::ComboBoxText &
	getKeyTypeButton () const
	{ return *m_KeyTypeButton; }

	Gtk::Label &
	getTimeLabel () const
	{ return *m_TimeLabel; }

	Gtk::Paned &
	getAnimationBox () const
	{ return *m_AnimationBox; }

	Gtk::TreeView &
	getTreeView () const
	{ return *m_TreeView; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::DrawingArea &
	getDrawingArea () const
	{ return *m_DrawingArea; }

	Gtk::Scrollbar &
	getTranslationSlider () const
	{ return *m_TranslationSlider; }

	Gtk::ToolButton &
	getZoomOutButton () const
	{ return *m_ZoomOutButton; }

	Gtk::ToolButton &
	getZoomInButton () const
	{ return *m_ZoomInButton; }

	Gtk::ToolButton &
	getZoomFitButton () const
	{ return *m_ZoomFitButton; }

	Gtk::ToolButton &
	getZoom100Button () const
	{ return *m_Zoom100Button; }

	Gtk::Dialog &
	getPropertiesDialog () const
	{ return *m_PropertiesDialog; }

	Gtk::Entry &
	getNewNameEntry () const
	{ return *m_NewNameEntry; }

	Gtk::SpinButton &
	getDurationSspinButton () const
	{ return *m_DurationSspinButton; }

	Gtk::SpinButton &
	getFPSSpinButton () const
	{ return *m_FPSSpinButton; }

	Gtk::CheckButton &
	getScaleKeyframesButton () const
	{ return *m_ScaleKeyframesButton; }

	Gtk::Switch &
	getLoopSwitch () const
	{ return *m_LoopSwitch; }

	Gtk::Label &
	getCycleIntervalLabel () const
	{ return *m_CycleIntervalLabel; }

	Gtk::Button &
	getNewCancelButton () const
	{ return *m_NewCancelButton; }

	Gtk::Button &
	getNewOkButton () const
	{ return *m_NewOkButton; }

	virtual
	void
	on_current_frame_changed () = 0;

	virtual
	void
	on_translation_changed () = 0;

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_new () = 0;

	virtual
	void
	on_open () = 0;

	virtual
	void
	on_add_object () = 0;

	virtual
	void
	on_cut () = 0;

	virtual
	void
	on_copy () = 0;

	virtual
	void
	on_paste () = 0;

	virtual
	void
	on_first_frame () = 0;

	virtual
	void
	on_play_pause () = 0;

	virtual
	void
	on_last_frame () = 0;

	virtual
	void
	on_time () = 0;

	virtual
	void
	on_key_type_changed () = 0;

	virtual
	bool
	on_tree_view_draw (const::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	void
	on_row_activated (const TreeModel::Path & path, TreeViewColumn* column) = 0;

	virtual
	void
	on_tainted_toggled (const Glib::ustring &) = 0;

	virtual
	bool
	on_button_press_event (GdkEventButton* event) = 0;

	virtual
	bool
	on_button_release_event (GdkEventButton* event) = 0;

	virtual
	bool
	on_configure_event (GdkEventConfigure* event) = 0;

	virtual
	bool
	on_draw (const::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	bool
	on_focus_in_event (GdkEventFocus* event) = 0;

	virtual
	bool
	on_focus_out_event (GdkEventFocus* event) = 0;

	virtual
	bool
	on_key_press_event (GdkEventKey* event) = 0;

	virtual
	bool
	on_key_release_event (GdkEventKey* event) = 0;

	virtual
	bool
	on_motion_notify_event (GdkEventMotion* event) = 0;

	virtual
	bool
	on_scroll_event (GdkEventScroll* event) = 0;

	virtual
	void
	on_zoom_out () = 0;

	virtual
	void
	on_zoom_in () = 0;

	virtual
	void
	on_zoom_fit () = 0;

	virtual
	void
	on_zoom_100 () = 0;

	virtual
	void
	on_new_name_changed () = 0;

	virtual
	void
	on_new_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_new_name_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_new_cycle_interval_changed () = 0;

	virtual
	~X3DAnimationEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	Glib::RefPtr <Gtk::AccelGroup>         m_AccelGroup;
	Glib::RefPtr <Gtk::Adjustment>         m_DurationAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_FPSAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_FrameAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TranslationAdjustment;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_NameColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_IconCellRenderer;
	Glib::RefPtr <Gtk::CellRendererText>   m_NameCellRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_TaintedColumn;
	Glib::RefPtr <Gtk::CellRendererToggle> m_TaintedCellRenderer;
	Gtk::Window*                           m_Window;
	Gtk::Box*                              m_Widget;
	Gtk::ToolButton*                       m_NewButton;
	Gtk::ToolButton*                       m_OpenButton;
	Gtk::ToolButton*                       m_AddObjectButton;
	Gtk::ToolButton*                       m_CutButton;
	Gtk::ToolButton*                       m_CopyButton;
	Gtk::ToolButton*                       m_PasteButton;
	Gtk::ToolButton*                       m_FirstFrameButton;
	Gtk::ToolButton*                       m_PlayPauseButton;
	Gtk::ToolButton*                       m_LastFrameButton;
	Gtk::SpinButton*                       m_FrameSpinButton;
	Gtk::ToolButton*                       m_TimeButton;
	Gtk::ComboBoxText*                     m_KeyTypeButton;
	Gtk::Label*                            m_TimeLabel;
	Gtk::Paned*                            m_AnimationBox;
	Gtk::TreeView*                         m_TreeView;
	Gtk::Box*                              m_NameBox;
	Gtk::Entry*                            m_NameEntry;
	Gtk::Button*                           m_RenameButton;
	Gtk::DrawingArea*                      m_DrawingArea;
	Gtk::Scrollbar*                        m_TranslationSlider;
	Gtk::ToolButton*                       m_ZoomOutButton;
	Gtk::ToolButton*                       m_ZoomInButton;
	Gtk::ToolButton*                       m_ZoomFitButton;
	Gtk::ToolButton*                       m_Zoom100Button;
	Gtk::Dialog*                           m_PropertiesDialog;
	Gtk::Entry*                            m_NewNameEntry;
	Gtk::SpinButton*                       m_DurationSspinButton;
	Gtk::SpinButton*                       m_FPSSpinButton;
	Gtk::CheckButton*                      m_ScaleKeyframesButton;
	Gtk::Switch*                           m_LoopSwitch;
	Gtk::Label*                            m_CycleIntervalLabel;
	Gtk::Button*                           m_NewCancelButton;
	Gtk::Button*                           m_NewOkButton;

};

} // puck
} // titania

#endif
