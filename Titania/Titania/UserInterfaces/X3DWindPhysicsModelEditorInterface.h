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

#ifndef __TMP_GLAD2CPP_WIND_PHYSICS_MODEL_EDITOR_H__
#define __TMP_GLAD2CPP_WIND_PHYSICS_MODEL_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for WindPhysicsModelEditor.
 */
class X3DWindPhysicsModelEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DWindPhysicsModelEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DWindPhysicsModelEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DWindPhysicsModelEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionXAdjustment () const
	{ return m_DirectionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionYAdjustment () const
	{ return m_DirectionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionZAdjustment () const
	{ return m_DirectionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getForceXAdjustment () const
	{ return m_ForceXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getForceYAdjustment () const
	{ return m_ForceYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getForceZAdjustment () const
	{ return m_ForceZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGustinessAdjustment () const
	{ return m_GustinessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpeedAdjustment () const
	{ return m_SpeedAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTurbulenceAdjustment () const
	{ return m_TurbulenceAdjustment; }

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

	Gtk::CheckButton &
	getEnabledCheckButton () const
	{ return *m_EnabledCheckButton; }

	Gtk::Box &
	getDirectionToolBox () const
	{ return *m_DirectionToolBox; }

	Gtk::Box &
	getDirectionBox () const
	{ return *m_DirectionBox; }

	Gtk::SpinButton &
	getSpeedSpinButton () const
	{ return *m_SpeedSpinButton; }

	Gtk::SpinButton &
	getGustinessSpinButton () const
	{ return *m_GustinessSpinButton; }

	Gtk::SpinButton &
	getTurbulenceSpinButton () const
	{ return *m_TurbulenceSpinButton; }

	Gtk::Box &
	getButtonBox () const
	{ return *m_ButtonBox; }

	///  @name Signal handlers

	///  @name Destruction

	virtual
	~X3DWindPhysicsModelEditorInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_DirectionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DirectionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DirectionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ForceXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ForceYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ForceZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GustinessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpeedAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TurbulenceAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Expander* m_Expander;
	Gtk::CheckButton* m_EnabledCheckButton;
	Gtk::Box* m_DirectionToolBox;
	Gtk::Box* m_DirectionBox;
	Gtk::SpinButton* m_SpeedSpinButton;
	Gtk::SpinButton* m_GustinessSpinButton;
	Gtk::SpinButton* m_TurbulenceSpinButton;
	Gtk::Box* m_ButtonBox;

};

} // puck
} // titania

#endif
