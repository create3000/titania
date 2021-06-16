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

#ifndef __TITANIA_BASE_ADJUSTMENT_OBJECT_H__
#define __TITANIA_BASE_ADJUSTMENT_OBJECT_H__

#include <gtkmm/adjustment.h>
#include <gtkmm/scrollable.h>
#include <sigc++/connection.h>
#include <sigc++/trackable.h>

#include <Titania/Chrono/Now.h>

namespace titania {
namespace puck {

class AdjustmentFreezer :
	public sigc::trackable
{
public:

	AdjustmentFreezer () :
		     sigc::trackable (),
		          connection (),
		           startTime (0)
	{ }

	void
	freeze (const Glib::RefPtr <Gtk::Adjustment> & adjustment)
	{
		restore (adjustment, adjustment -> get_value ());
	}

	void
	restore (const Glib::RefPtr <Gtk::Adjustment> & adjustment, const double value)
	{
		startTime = chrono::now ();

		adjustment -> set_value (value);

		connect (adjustment, value);
	}

private:

	void
	block (const Glib::RefPtr <Gtk::Adjustment> & adjustment, const double value)
	{
		connection .disconnect ();

		if (chrono::now () - startTime < 0.4)
		{
			adjustment -> set_value (value);
			adjustment -> signal_changed () .emission_stop ();

			connect (adjustment, value);
		}
	}

	void
	connect (const Glib::RefPtr <Gtk::Adjustment> & adjustment, const double value)
	{
		connection .disconnect ();

		connection = adjustment -> signal_changed () .connect (sigc::bind (sigc::mem_fun (this, &AdjustmentFreezer::block), adjustment, value), false);
	}

	sigc::connection connection;
	double           startTime;

};

class ScrollFreezer :
	public sigc::trackable
{
public:

	ScrollFreezer (Gtk::Scrollable & scrollable) :
		 ScrollFreezer (&scrollable)
	{ }

	ScrollFreezer (Gtk::Scrollable* const scrollable) :
		sigc::trackable (),
		     scrollable (scrollable),
		    hadjustment (),
		    vadjustment ()
	{ }

	void
	freeze ()
	{
		hadjustment .freeze (scrollable -> get_hadjustment ());
		vadjustment .freeze (scrollable -> get_vadjustment ());
	}

	void
	restore (const double x, const double y)
	{
		hadjustment .restore (scrollable -> get_hadjustment (), x);
		vadjustment .restore (scrollable -> get_vadjustment (), y);
	}


private:

	Gtk::Scrollable* const scrollable;
	AdjustmentFreezer      hadjustment;
	AdjustmentFreezer      vadjustment;

};

} // puck
} // titania

#endif
