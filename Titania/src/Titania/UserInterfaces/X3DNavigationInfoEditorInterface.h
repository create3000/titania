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

#ifndef __TMP_GLAD2CPP_NAVIGATION_INFO_EDITOR_H__
#define __TMP_GLAD2CPP_NAVIGATION_INFO_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for NavigationInfoEditor.
 */
class X3DNavigationInfoEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DNavigationInfoEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DNavigationInfoEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DNavigationInfoEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCollisionRadiusAdjustment () const
	{ return m_CollisionRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getHeightAdjustment () const
	{ return m_HeightAdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getNavigationInfoTransitionTypeListStore () const
	{ return m_NavigationInfoTransitionTypeListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getNavigationInfoTypeListStore () const
	{ return m_NavigationInfoTypeListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpeedAdjustment () const
	{ return m_SpeedAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getStepHeightAdjustment () const
	{ return m_StepHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransitionTimeAdjustment () const
	{ return m_TransitionTimeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getVisibilityLimitAdjustment () const
	{ return m_VisibilityLimitAdjustment; }

	const Glib::RefPtr <Gtk::CellRendererCombo> &
	getTypeCellRendererCombo () const
	{ return m_TypeCellRendererCombo; }

	const Glib::RefPtr <Gtk::CellRendererCombo> &
	getTransitionTypeCellRendererCombo () const
	{ return m_TransitionTypeCellRendererCombo; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::Paned &
	getPaned () const
	{ return *m_Paned; }

	Gtk::Box &
	getNavigationInfoListBox () const
	{ return *m_NavigationInfoListBox; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Box &
	getNavigationInfoActionBox () const
	{ return *m_NavigationInfoActionBox; }

	Gtk::Button &
	getNewNavigationInfoButton () const
	{ return *m_NewNavigationInfoButton; }

	Gtk::Button &
	getRemoveNavigationInfoButton () const
	{ return *m_RemoveNavigationInfoButton; }

	Gtk::Expander &
	getNavigationInfoExpander () const
	{ return *m_NavigationInfoExpander; }

	Gtk::Grid &
	getNavigationInfoBox () const
	{ return *m_NavigationInfoBox; }

	Gtk::Box &
	getAvatarSizeBox () const
	{ return *m_AvatarSizeBox; }

	Gtk::SpinButton &
	getAvatarSizeSpinButton0 () const
	{ return *m_AvatarSizeSpinButton0; }

	Gtk::SpinButton &
	getAvatarSizeSpinButton1 () const
	{ return *m_AvatarSizeSpinButton1; }

	Gtk::SpinButton &
	getAvatarSizeSpinButton2 () const
	{ return *m_AvatarSizeSpinButton2; }

	Gtk::SpinButton &
	getSpeedSpinButton () const
	{ return *m_SpeedSpinButton; }

	Gtk::CheckButton &
	getHeadlightCheckButton () const
	{ return *m_HeadlightCheckButton; }

	Gtk::SpinButton &
	getTransitionTimeSpinButton () const
	{ return *m_TransitionTimeSpinButton; }

	Gtk::SpinButton &
	getVisibilityLimitSpinButton () const
	{ return *m_VisibilityLimitSpinButton; }

	Gtk::Box &
	getTypeBox () const
	{ return *m_TypeBox; }

	Gtk::TreeView &
	getTypeTreeView () const
	{ return *m_TypeTreeView; }

	Gtk::Button &
	getTypeAddButton () const
	{ return *m_TypeAddButton; }

	Gtk::Button &
	getTypeRemoveButton () const
	{ return *m_TypeRemoveButton; }

	Gtk::Box &
	getTransitionTypeBox () const
	{ return *m_TransitionTypeBox; }

	Gtk::TreeView &
	getTransitionTypeTreeView () const
	{ return *m_TransitionTypeTreeView; }

	Gtk::Button &
	getTransitionTypeAddButton () const
	{ return *m_TransitionTypeAddButton; }

	Gtk::Button &
	getTransitionTypeRemoveButton () const
	{ return *m_TransitionTypeRemoveButton; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	///  @name Signal handlers

	virtual
	void
	on_new_navigation_info_clicked () = 0;

	virtual
	void
	on_remove_navigation_info_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DNavigationInfoEditorInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_CollisionRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_HeightAdjustment;
	Glib::RefPtr <Gtk::ListStore> m_NavigationInfoTransitionTypeListStore;
	Glib::RefPtr <Gtk::ListStore> m_NavigationInfoTypeListStore;
	Glib::RefPtr <Gtk::Adjustment> m_SpeedAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_StepHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransitionTimeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_VisibilityLimitAdjustment;
	Glib::RefPtr <Gtk::CellRendererCombo> m_TypeCellRendererCombo;
	Glib::RefPtr <Gtk::CellRendererCombo> m_TransitionTypeCellRendererCombo;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::Paned* m_Paned;
	Gtk::Box* m_NavigationInfoListBox;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Box* m_NavigationInfoActionBox;
	Gtk::Button* m_NewNavigationInfoButton;
	Gtk::Button* m_RemoveNavigationInfoButton;
	Gtk::Expander* m_NavigationInfoExpander;
	Gtk::Grid* m_NavigationInfoBox;
	Gtk::Box* m_AvatarSizeBox;
	Gtk::SpinButton* m_AvatarSizeSpinButton0;
	Gtk::SpinButton* m_AvatarSizeSpinButton1;
	Gtk::SpinButton* m_AvatarSizeSpinButton2;
	Gtk::SpinButton* m_SpeedSpinButton;
	Gtk::CheckButton* m_HeadlightCheckButton;
	Gtk::SpinButton* m_TransitionTimeSpinButton;
	Gtk::SpinButton* m_VisibilityLimitSpinButton;
	Gtk::Box* m_TypeBox;
	Gtk::TreeView* m_TypeTreeView;
	Gtk::Button* m_TypeAddButton;
	Gtk::Button* m_TypeRemoveButton;
	Gtk::Box* m_TransitionTypeBox;
	Gtk::TreeView* m_TransitionTypeTreeView;
	Gtk::Button* m_TransitionTypeAddButton;
	Gtk::Button* m_TransitionTypeRemoveButton;
	Gtk::Box* m_NameBox;
	Gtk::Entry* m_NameEntry;
	Gtk::Button* m_RenameButton;

};

} // puck
} // titania

#endif
