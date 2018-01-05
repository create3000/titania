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

#ifndef __TMP_GLAD2CPP_DEFAULT_PHYSICS_MODEL_EDITOR_H__
#define __TMP_GLAD2CPP_DEFAULT_PHYSICS_MODEL_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for DefaultPhysicsModelEditor.
 */
class X3DDefaultPhysicsModelEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DDefaultPhysicsModelEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DDefaultPhysicsModelEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DDefaultPhysicsModelEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getForceXAdjustment () const
	{ return m_ForceXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getForceYAdjustment () const
	{ return m_ForceYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getForceZAdjustment () const
	{ return m_ForceZAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Expander &
	getExpander () const
	{ return *m_Expander; }

	Gtk::Box &
	getButtonBox () const
	{ return *m_ButtonBox; }

	Gtk::Button &
	getUpButton () const
	{ return *m_UpButton; }

	Gtk::Button &
	getDownButton () const
	{ return *m_DownButton; }

	Gtk::Button &
	getRemoveButton () const
	{ return *m_RemoveButton; }

	///  @name Signal handlers

	///  @name Destruction

	virtual
	~X3DDefaultPhysicsModelEditorInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_ForceXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ForceYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ForceZAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Expander* m_Expander;
	Gtk::Box* m_ButtonBox;
	Gtk::Button* m_UpButton;
	Gtk::Button* m_DownButton;
	Gtk::Button* m_RemoveButton;

};

} // puck
} // titania

#endif