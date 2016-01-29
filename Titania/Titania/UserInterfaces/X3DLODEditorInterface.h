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
#ifndef __TMP_GLAD2CPP_LODEDITOR_H__
#define __TMP_GLAD2CPP_LODEDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DLODEditorInterface :
	public X3DEditorInterface
{
public:

	X3DLODEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DLODEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

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
	getLODBBoxCenterXAdjustment () const
	{ return m_LODBBoxCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxCenterYAdjustment () const
	{ return m_LODBBoxCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxCenterZAdjustment () const
	{ return m_LODBBoxCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxSizeXAdjustment () const
	{ return m_LODBBoxSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxSizeYAdjustment () const
	{ return m_LODBBoxSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxSizeZAdjustment () const
	{ return m_LODBBoxSizeZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODCenterXAdjustment () const
	{ return m_LODCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODCenterYAdjustment () const
	{ return m_LODCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODCenterZAdjustment () const
	{ return m_LODCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODLevelAdjustment () const
	{ return m_LODLevelAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODRangeMaxAdjustment () const
	{ return m_LODRangeMaxAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODRangeMinAdjustment () const
	{ return m_LODRangeMinAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRangeAdjustment () const
	{ return m_RangeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRangeColorAdjustment () const
	{ return m_RangeColorAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Button &
	getIndexButton () const
	{ return *m_IndexButton; }

	Gtk::Expander &
	getLODExpander () const
	{ return *m_LODExpander; }

	Gtk::Grid &
	getLODBox () const
	{ return *m_LODBox; }

	Gtk::CheckButton &
	getLODForceTransitionsCheckButton () const
	{ return *m_LODForceTransitionsCheckButton; }

	Gtk::Box &
	getLODNameBox () const
	{ return *m_LODNameBox; }

	Gtk::Entry &
	getLODNameEntry () const
	{ return *m_LODNameEntry; }

	Gtk::Button &
	getLODRenameButton () const
	{ return *m_LODRenameButton; }

	Gtk::Box &
	getLODCenterBox () const
	{ return *m_LODCenterBox; }

	Gtk::SpinButton &
	getLODCenterXSpinButton () const
	{ return *m_LODCenterXSpinButton; }

	Gtk::SpinButton &
	getLODCenterYSpinButton () const
	{ return *m_LODCenterYSpinButton; }

	Gtk::SpinButton &
	getLODCenterZSpinButton () const
	{ return *m_LODCenterZSpinButton; }

	Gtk::Button &
	getLODMoveCenterButton () const
	{ return *m_LODMoveCenterButton; }

	Gtk::Box &
	getLODRangeBox () const
	{ return *m_LODRangeBox; }

	Gtk::SpinButton &
	getLODRangeMinSpinButton () const
	{ return *m_LODRangeMinSpinButton; }

	Gtk::SpinButton &
	getLODRangeMaxSpinButton () const
	{ return *m_LODRangeMaxSpinButton; }

	Gtk::CheckButton &
	getLODMaxCheckButton () const
	{ return *m_LODMaxCheckButton; }

	Gtk::SpinButton &
	getRangeSpinButton () const
	{ return *m_RangeSpinButton; }

	Gtk::Box &
	getRangeGradientBox () const
	{ return *m_RangeGradientBox; }

	Gtk::Grid &
	getRangeColorGrid () const
	{ return *m_RangeColorGrid; }

	Gtk::ScrolledWindow &
	getSkyColorsScrolledWindow () const
	{ return *m_SkyColorsScrolledWindow; }

	Gtk::Button &
	getAddRangeColorButton () const
	{ return *m_AddRangeColorButton; }

	Gtk::Button &
	getRemoveRangeColorButton () const
	{ return *m_RemoveRangeColorButton; }

	Gtk::Box &
	getRangeColorBox () const
	{ return *m_RangeColorBox; }

	Gtk::Button &
	getRangeColorButton () const
	{ return *m_RangeColorButton; }

	Gtk::SpinButton &
	getLODLevelSpinButton () const
	{ return *m_LODLevelSpinButton; }

	Gtk::CheckButton &
	getLODKeepCurrentLevelCheckButton () const
	{ return *m_LODKeepCurrentLevelCheckButton; }

	Gtk::Expander &
	getBoundingBoxExpander () const
	{ return *m_BoundingBoxExpander; }

	Gtk::Box &
	getLODBBoxSizeBox () const
	{ return *m_LODBBoxSizeBox; }

	Gtk::SpinButton &
	getLODBBoxSizeXSpinButton () const
	{ return *m_LODBBoxSizeXSpinButton; }

	Gtk::SpinButton &
	getLODBBoxSizeYSpinButton () const
	{ return *m_LODBBoxSizeYSpinButton; }

	Gtk::SpinButton &
	getLODBBoxSizeZSpinButton () const
	{ return *m_LODBBoxSizeZSpinButton; }

	Gtk::Box &
	getLODBBoxCenterBox () const
	{ return *m_LODBBoxCenterBox; }

	Gtk::SpinButton &
	getLODBBoxCenterXSpinButton () const
	{ return *m_LODBBoxCenterXSpinButton; }

	Gtk::SpinButton &
	getLODBBoxCenterYSpinButton () const
	{ return *m_LODBBoxCenterYSpinButton; }

	Gtk::SpinButton &
	getLODBBoxCenterZSpinButton () const
	{ return *m_LODBBoxCenterZSpinButton; }

	virtual
	void
	on_index_clicked () = 0;

	virtual
	void
	on_lod_move_center_button () = 0;

	virtual
	void
	on_lod_keep_current_level_activate () = 0;

	virtual
	~X3DLODEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxSizeZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODLevelAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODRangeMaxAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODRangeMinAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_RangeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_RangeColorAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Button*                   m_IndexButton;
	Gtk::Expander*                 m_LODExpander;
	Gtk::Grid*                     m_LODBox;
	Gtk::CheckButton*              m_LODForceTransitionsCheckButton;
	Gtk::Box*                      m_LODNameBox;
	Gtk::Entry*                    m_LODNameEntry;
	Gtk::Button*                   m_LODRenameButton;
	Gtk::Box*                      m_LODCenterBox;
	Gtk::SpinButton*               m_LODCenterXSpinButton;
	Gtk::SpinButton*               m_LODCenterYSpinButton;
	Gtk::SpinButton*               m_LODCenterZSpinButton;
	Gtk::Button*                   m_LODMoveCenterButton;
	Gtk::Box*                      m_LODRangeBox;
	Gtk::SpinButton*               m_LODRangeMinSpinButton;
	Gtk::SpinButton*               m_LODRangeMaxSpinButton;
	Gtk::CheckButton*              m_LODMaxCheckButton;
	Gtk::SpinButton*               m_RangeSpinButton;
	Gtk::Box*                      m_RangeGradientBox;
	Gtk::Grid*                     m_RangeColorGrid;
	Gtk::ScrolledWindow*           m_SkyColorsScrolledWindow;
	Gtk::Button*                   m_AddRangeColorButton;
	Gtk::Button*                   m_RemoveRangeColorButton;
	Gtk::Box*                      m_RangeColorBox;
	Gtk::Button*                   m_RangeColorButton;
	Gtk::SpinButton*               m_LODLevelSpinButton;
	Gtk::CheckButton*              m_LODKeepCurrentLevelCheckButton;
	Gtk::Expander*                 m_BoundingBoxExpander;
	Gtk::Box*                      m_LODBBoxSizeBox;
	Gtk::SpinButton*               m_LODBBoxSizeXSpinButton;
	Gtk::SpinButton*               m_LODBBoxSizeYSpinButton;
	Gtk::SpinButton*               m_LODBBoxSizeZSpinButton;
	Gtk::Box*                      m_LODBBoxCenterBox;
	Gtk::SpinButton*               m_LODBBoxCenterXSpinButton;
	Gtk::SpinButton*               m_LODBBoxCenterYSpinButton;
	Gtk::SpinButton*               m_LODBBoxCenterZSpinButton;

};

} // puck
} // titania

#endif
