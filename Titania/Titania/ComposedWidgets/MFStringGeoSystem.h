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

#ifndef __TITANIA_COMPOSED_WIDGETS_MFSTRING_GEO_SYSTEM_H__
#define __TITANIA_COMPOSED_WIDGETS_MFSTRING_GEO_SYSTEM_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

class MFStringGeoSystem :
	public X3DComposedWidget
{
public:

	///  @name Construction

	MFStringGeoSystem (X3DBrowserWindow* const,
	                   Gtk::ComboBoxText &,
	                   Gtk::ComboBoxText &,
	                   Gtk::ComboBoxText &,
	                   const Glib::RefPtr <Gtk::Adjustment> &,
	                   Gtk::ComboBoxText &,
	                   Gtk::ComboBoxText &,
	                   Gtk::Widget &,
	                   Gtk::Widget &,
	                   Gtk::Widget &);

	void
	setNode (const X3D::SFNode &);

	///  @name Destruction

	virtual
	~MFStringGeoSystem ()
	{ dispose (); }


private:

	///  @name Event handler

	void
	on_changed (const int);

	void
	set_field ();

	void
	connect (const X3D::MFString &);

	///  @name Members

	Gtk::ComboBoxText &                    coordinateSystem;
	Gtk::ComboBoxText &                    ellipsoid;
	Gtk::ComboBoxText &                    gdOrder;
	const Glib::RefPtr <Gtk::Adjustment>   zone;
	Gtk::ComboBoxText &                    hemisphere;
	Gtk::ComboBoxText &                    utmOrder;
	Gtk::Widget &                          ellipsoidBox;
	Gtk::Widget &                          gdBox;
	Gtk::Widget &                          utmBox;
	X3D::X3DPtr <X3D::X3DGeospatialObject> node;
	UndoStepPtr                            undoStep;
	int                                    input;
	bool                                   changing;

};

inline
MFStringGeoSystem::MFStringGeoSystem (X3DBrowserWindow* const browserWindow,
                                      Gtk::ComboBoxText & coordinateSystem,
                                      Gtk::ComboBoxText & ellipsoid,
                                      Gtk::ComboBoxText & gdOrder,
                                      const Glib::RefPtr <Gtk::Adjustment> & zone,
                                      Gtk::ComboBoxText & hemisphere,
                                      Gtk::ComboBoxText & utmOrder,
                                      Gtk::Widget & ellipsoidBox,
                                      Gtk::Widget & gdBox,
                                      Gtk::Widget & utmBox) :
	 X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DComposedWidget (),
	 coordinateSystem (coordinateSystem),
	        ellipsoid (ellipsoid),
	          gdOrder (gdOrder),
	             zone (zone),
	       hemisphere (hemisphere),
	         utmOrder (utmOrder),
	     ellipsoidBox (ellipsoidBox),
	            gdBox (gdBox),
	           utmBox (utmBox),
	             node (),
	         undoStep (),
	            input (-1),
	         changing (false)
{
	coordinateSystem .signal_changed () .connect (sigc::bind (sigc::mem_fun (*this, &MFStringGeoSystem::on_changed), 0));
	ellipsoid        .signal_changed () .connect (sigc::bind (sigc::mem_fun (*this, &MFStringGeoSystem::on_changed), 1));
	gdOrder          .signal_changed () .connect (sigc::bind (sigc::mem_fun (*this, &MFStringGeoSystem::on_changed), 2));

	zone -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &MFStringGeoSystem::on_changed), 3));
	hemisphere .signal_changed ()   .connect (sigc::bind (sigc::mem_fun (*this, &MFStringGeoSystem::on_changed), 4));
	utmOrder   .signal_changed ()   .connect (sigc::bind (sigc::mem_fun (*this, &MFStringGeoSystem::on_changed), 5));

	setup ();
}

inline
void
MFStringGeoSystem::setNode (const X3D::SFNode & value)
{
	if (node)
		node -> geoSystem () .removeInterest (this, &MFStringGeoSystem::set_field);

	node = value;

	if (node)
	{
		node -> geoSystem () .addInterest (this, &MFStringGeoSystem::set_field);
		set_field ();
	}
	else
	{
		undoStep .reset ();

		changing = true;

		coordinateSystem .set_active (0);
		ellipsoid        .set_active_text ("WE");

		coordinateSystem .set_sensitive (false);
		ellipsoidBox     .set_sensitive (false);

		ellipsoidBox .set_visible (true);
		gdBox        .set_visible (false);
		utmBox       .set_visible (false);

		changing = false;
	}
}

inline
void
MFStringGeoSystem::on_changed (const int id)
{
	if (changing)
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction (node, node -> geoSystem (), undoStep);

	node -> geoSystem () .removeInterest (this, &MFStringGeoSystem::set_field);
	node -> geoSystem () .addInterest (this, &MFStringGeoSystem::connect);

	node -> geoSystem () .clear ();

	switch (coordinateSystem .get_active_row_number ())
	{
		case 0:
		{
			node -> geoSystem () .emplace_back ("GD");
			node -> geoSystem () .emplace_back (ellipsoid .get_active_text ());

			if (gdOrder .get_active_row_number () > 0)
				node -> geoSystem () .emplace_back ("longitude_first");

			break;
		}
		case 1:
		{
			node -> geoSystem () .emplace_back ("UTM");
			node -> geoSystem () .emplace_back (ellipsoid .get_active_text ());
			node -> geoSystem () .emplace_back ("Z" + basic::to_string (zone -> get_value ()));

			if (hemisphere .get_active_row_number () > 0)
				node -> geoSystem () .emplace_back ("S");

			if (utmOrder .get_active_row_number () > 0)
				node -> geoSystem () .emplace_back ("easting_first");

			break;
		}
		case 2:
		{
			node -> geoSystem () = { "GC" };
			break;
		}
		default:
			break;
	}

	addRedoFunction (node -> geoSystem (), undoStep);

}

inline
void
MFStringGeoSystem::set_field ()
{
	undoStep .reset ();

	changing = true;

	coordinateSystem .set_sensitive (true);
	ellipsoidBox     .set_sensitive (true);

	gdBox  .set_visible (false);
	utmBox .set_visible (false);

	switch (X3D::Geospatial::getCoordinateSystem (node -> geoSystem ()))
	{
		case X3D::Geospatial::CoordinateSystemType::GD:
		{
			coordinateSystem .set_active (0);
			ellipsoid        .set_active_text (X3D::Geospatial::getEllipsoidString (node -> geoSystem ()));
			gdOrder          .set_active (not X3D::Geospatial::getLatitudeFirst (node -> geoSystem ()));

			ellipsoidBox .set_visible (true);
			gdBox        .set_visible (true);
			break;
		}
		case X3D::Geospatial::CoordinateSystemType::UTM:
		{
			coordinateSystem .set_active (1);
			ellipsoid        .set_active_text (X3D::Geospatial::getEllipsoidString (node -> geoSystem ()));

			zone -> set_value (X3D::Geospatial::getZone (node -> geoSystem ()));
			hemisphere .set_active (not X3D::Geospatial::getNorthernHemisphere (node -> geoSystem ()));
			utmOrder   .set_active (not X3D::Geospatial::getNorthingFirst (node -> geoSystem ()));

			ellipsoidBox .set_visible (true);
			utmBox       .set_visible (true);
			break;
		}
		case X3D::Geospatial::CoordinateSystemType::GC:
		{
			coordinateSystem .set_active (2);
			ellipsoidBox     .set_visible (false);
			break;
		}
	}

	changing = false;
}

inline
void
MFStringGeoSystem::connect (const X3D::MFString & field)
{
	field .removeInterest (this, &MFStringGeoSystem::connect);
	field .addInterest (this, &MFStringGeoSystem::set_field);
}

} // puck
} // titania

#endif
