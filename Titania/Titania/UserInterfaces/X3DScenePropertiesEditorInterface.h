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
#ifndef __TMP_GLAD2CPP_SCENE_PROPERTIES_EDITOR_H__
#define __TMP_GLAD2CPP_SCENE_PROPERTIES_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DScenePropertiesEditorInterface :
	public X3DEditorInterface
{
public:

	X3DScenePropertiesEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DScenePropertiesEditorInterface (const std::string & filename, const Arguments & ... arguments) :
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
	getUnitAngleAdjustment () const
	{ return m_UnitAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getUnitForceAdjustment () const
	{ return m_UnitForceAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getUnitLengthAdjustment () const
	{ return m_UnitLengthAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getUnitMassAdjustment () const
	{ return m_UnitMassAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Notebook &
	getScenePropertiesNotebook () const
	{ return *m_ScenePropertiesNotebook; }

	Gtk::Expander &
	getUnitsExpander () const
	{ return *m_UnitsExpander; }

	Gtk::ComboBoxText &
	getUnitMassCombo () const
	{ return *m_UnitMassCombo; }

	Gtk::Entry &
	getUnitMassEntry () const
	{ return *m_UnitMassEntry; }

	Gtk::ComboBoxText &
	getUnitLengthCombo () const
	{ return *m_UnitLengthCombo; }

	Gtk::Entry &
	getUnitLengthEntry () const
	{ return *m_UnitLengthEntry; }

	Gtk::ComboBoxText &
	getUnitForceCombo () const
	{ return *m_UnitForceCombo; }

	Gtk::Entry &
	getUnitForceEntry () const
	{ return *m_UnitForceEntry; }

	Gtk::ComboBoxText &
	getUnitAngleCombo () const
	{ return *m_UnitAngleCombo; }

	Gtk::Entry &
	getUnitAngleEntry () const
	{ return *m_UnitAngleEntry; }

	virtual
	void
	on_unit_angle_changed () = 0;

	virtual
	void
	on_unit_force_changed () = 0;

	virtual
	void
	on_unit_length_changed () = 0;

	virtual
	void
	on_unit_mass_changed () = 0;

	virtual
	void
	on_unit_mass_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_unit_mass_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_unit_length_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_unit_length_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_unit_force_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_unit_force_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_unit_angle_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_unit_angle_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	~X3DScenePropertiesEditorInterface ();


private:

	virtual
	void
	construct () final override
	{ X3DEditorInterface::construct (); }

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	std::deque <sigc::connection>  m_connections;
	Glib::RefPtr <Gtk::Adjustment> m_UnitAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_UnitForceAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_UnitLengthAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_UnitMassAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Notebook*                 m_ScenePropertiesNotebook;
	Gtk::Expander*                 m_UnitsExpander;
	Gtk::ComboBoxText*             m_UnitMassCombo;
	Gtk::Entry*                    m_UnitMassEntry;
	Gtk::ComboBoxText*             m_UnitLengthCombo;
	Gtk::Entry*                    m_UnitLengthEntry;
	Gtk::ComboBoxText*             m_UnitForceCombo;
	Gtk::Entry*                    m_UnitForceEntry;
	Gtk::ComboBoxText*             m_UnitAngleCombo;
	Gtk::Entry*                    m_UnitAngleEntry;

};

} // puck
} // titania

#endif
