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

#include "EulerAdjustment.h"

namespace titania {
namespace puck {

EulerAdjustment::EulerAdjustment (X3DBaseInterface* const editor,
                                            const Glib::RefPtr <Gtk::Adjustment> & adjustmentX,
                                            const Glib::RefPtr <Gtk::Adjustment> & adjustmentY,
                                            const Glib::RefPtr <Gtk::Adjustment> & adjustmentZ,
                                            Gtk::Widget & widget,
                                            const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	      adjustments ({ adjustmentX, adjustmentY, adjustmentZ }),
	           widget (widget),
	            scene (),
	            nodes (),
	             name (name),
	         undoStep (),
	            input (-1),
	         changing (false),
	           buffer ()
{
	addChildObjects (scene, nodes, buffer);
	            
	setup ();

	buffer .addInterest (&EulerAdjustment::set_buffer, this);

	adjustments [0] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &EulerAdjustment::on_value_changed), 0));
	adjustments [1] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &EulerAdjustment::on_value_changed), 1));
	adjustments [2] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &EulerAdjustment::on_value_changed), 2));

	getCurrentScene () .addInterest (&EulerAdjustment::set_scene, this);

	set_scene ();
}

void
EulerAdjustment::set_scene ()
{
	// Connect units.

	if (scene)
		scene -> units_changed () .removeInterest (&EulerAdjustment::set_field, this);

	scene = getCurrentScene ();

	scene -> units_changed () .addInterest (&EulerAdjustment::set_field, this);
}

void
EulerAdjustment::setNodes (const X3D::MFNode & value)
{
	// Connect field.

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFRotation> (name) .removeInterest (&EulerAdjustment::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFRotation> (name) .addInterest (&EulerAdjustment::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
EulerAdjustment::on_value_changed (const int id)
{
	if (nodes .empty ())
		return;

	if (changing)
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction <X3D::SFRotation> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFRotation> (name);

			field .removeInterest (&EulerAdjustment::set_field, this);
			field .addInterest (&EulerAdjustment::connect, this);

			X3D::Vector3d euler (getCurrentScene () -> fromUnit (X3D::UnitCategory::ANGLE, adjustments [0] -> get_value ()),
			                     getCurrentScene () -> fromUnit (X3D::UnitCategory::ANGLE, adjustments [1] -> get_value ()),
			                     getCurrentScene () -> fromUnit (X3D::UnitCategory::ANGLE, adjustments [2] -> get_value ()));

			field = fromEuler (euler);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFRotation> (nodes, name, undoStep);
}

void
EulerAdjustment::set_field ()
{
	buffer .addEvent ();
}

void
EulerAdjustment::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last »creaseAngle« field.

	bool hasField = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto euler = toEuler (node -> getField <X3D::SFRotation> (name));

			set_bounds ();

			adjustments [0] -> set_value (getCurrentScene () -> toUnit (X3D::UnitCategory::ANGLE, euler .x ()));
			adjustments [1] -> set_value (getCurrentScene () -> toUnit (X3D::UnitCategory::ANGLE, euler .y ()));
			adjustments [2] -> set_value (getCurrentScene () -> toUnit (X3D::UnitCategory::ANGLE, euler .z ()));

			hasField = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField)
	{
		set_bounds ();

		adjustments [0] -> set_value (0);
		adjustments [1] -> set_value (0);
		adjustments [2] -> set_value (0);
	}

	widget .set_sensitive (hasField);

	changing = false;
}

void
EulerAdjustment::set_bounds ()
{
	for (const auto & adjustment : adjustments)
	{
		adjustment -> set_lower (getCurrentScene () -> toUnit (X3D::UnitCategory::ANGLE, -math::pi <double> * 2));
		adjustment -> set_upper (getCurrentScene () -> toUnit (X3D::UnitCategory::ANGLE, math::pi <double> * 2));
	}
}

void
EulerAdjustment::connect (const X3D::SFRotation & field)
{
	field .removeInterest (&EulerAdjustment::connect, this);
	field .addInterest (&EulerAdjustment::set_field, this);
}

X3D::Rotation4d
EulerAdjustment::fromEuler (const X3D::Vector3d & euler)
{
	const auto roll  = euler .x ();
	const auto pitch = euler .y ();
	const auto yaw   = euler .z ();

	const auto cy = std::cos (yaw / 2);
	const auto sy = std::sin (yaw / 2);
	const auto cp = std::cos (pitch / 2);
	const auto sp = std::sin (pitch / 2);
	const auto cr = std::cos (roll / 2);
	const auto sr = std::sin (roll / 2);
	
	const auto q = X3D::Quaternion4d (cy * cp * sr - sy * sp * cr,
	                                  sy * cp * sr + cy * sp * cr,
	                                  sy * cp * cr - cy * sp * sr,
	                                  cy * cp * cr + sy * sp * sr);

	return X3D::Rotation4d (q);
}

X3D::Vector3d
EulerAdjustment::toEuler (const X3D::Rotation4d & rotation)
{
	const auto & q = rotation .quat ();

	// pitch (y-axis rotation)
	const auto sinp  = 2 * (q .w () * q .y () - q .z () * q .x ());
	const auto pitch = std::abs (sinp) >= 1
	                   ? std::copysign (math::pi <double> / 2, sinp) // use 90 degrees if out of range
	                   : std::asin (sinp);

	// roll (x-axis rotation)
	const auto sinr_cosp = 2 * (q .w () * q .x () + q .y () * q .z ());
	const auto cosr_cosp = 1 - 2 * (q .x () * q .x () + q .y () * q .y ());
	const auto roll      = std::atan2 (sinr_cosp, cosr_cosp);

	// yaw (z-axis rotation)
	const auto siny_cosp = 2 * (q .w () * q .z () + q .x () * q .y ());
	const auto cosy_cosp = 1 - 2 * (q .y () * q .y () + q .z () * q .z ());  
	const auto yaw       = std::atan2 (siny_cosp, cosy_cosp);

	return X3D::Vector3d (roll, pitch, yaw);
}

EulerAdjustment::~EulerAdjustment ()
{
	dispose ();
}

} // puck
} // titania
