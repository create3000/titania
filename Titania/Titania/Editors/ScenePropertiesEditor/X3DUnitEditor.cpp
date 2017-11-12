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

#include "X3DUnitEditor.h"

#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

X3DUnitEditor::X3DUnitEditor () :
	X3DScenePropertiesEditorInterface (),
	                          keyfile (),
	                         changing (false)
{ }

void
X3DUnitEditor::initialize ()
{
	getCurrentContext () .addInterest (&X3DUnitEditor::set_execution_context, this);

	keyfile .load_from_file (find_data_file ("units/units.ini"));

	if (keyfile .has_group ("angle"))
	{
		for (const auto & key : keyfile .get_keys ("angle"))
			getUnitAngleCombo () .append (key, _ (key .c_str ()));
	}

	if (keyfile .has_group ("force"))
	{
		for (const auto & key : keyfile .get_keys ("force"))
			getUnitForceCombo () .append (key, _ (key .c_str ()));
	}

	if (keyfile .has_group ("length"))
	{
		for (const auto & key : keyfile .get_keys ("length"))
			getUnitLengthCombo () .append (key, _ (key .c_str ()));
	}

	if (keyfile .has_group ("mass"))
	{
		for (const auto & key : keyfile .get_keys ("mass"))
			getUnitMassCombo () .append (key, _ (key .c_str ()));
	}

	set_execution_context ();
}

void
X3DUnitEditor::set_execution_context ()
{
	changing = true;

	const auto & angle  = getCurrentScene () -> getUnit (X3D::UnitCategory::ANGLE);
	const auto & force  = getCurrentScene () -> getUnit (X3D::UnitCategory::FORCE);
	const auto & length = getCurrentScene () -> getUnit (X3D::UnitCategory::LENGTH);
	const auto & mass   = getCurrentScene () -> getUnit (X3D::UnitCategory::MASS);

	getUnitAngleEntry ()  .set_text (angle  .getName ());
	getUnitForceEntry ()  .set_text (force  .getName ());
	getUnitLengthEntry () .set_text (length .getName ());
	getUnitMassEntry ()   .set_text (mass   .getName ());

	getUnitAngleAdjustment ()  -> set_value (angle  .getConversionFactor ());
	getUnitForceAdjustment ()  -> set_value (force  .getConversionFactor ());
	getUnitLengthAdjustment () -> set_value (length .getConversionFactor ());
	getUnitMassAdjustment ()   -> set_value (mass   .getConversionFactor ());

	changing = false;
}

bool
X3DUnitEditor::hasKey (const std::string & group, const std::string & key) const
{
	return keyfile .has_group (group) and keyfile .has_key (group, key);
}

long double
X3DUnitEditor::getKey (const std::string & group, const std::string & key) const
{
	std::istringstream isstream (keyfile .get_string (group, key));

	isstream .imbue (std::locale::classic ());

	long double value = 0;

	isstream >> value;

	return value;
}

/*
 * Entrys
 */

void
X3DUnitEditor::on_unit_angle_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (getUnitAngleEntry (), text, *position);
}

void
X3DUnitEditor::on_unit_angle_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (getUnitAngleEntry (), start_pos, end_pos);
}

void
X3DUnitEditor::on_unit_force_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (getUnitForceEntry (), text, *position);
}

void
X3DUnitEditor::on_unit_force_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (getUnitForceEntry (), start_pos, end_pos);
}

void
X3DUnitEditor::on_unit_length_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (getUnitLengthEntry (), text, *position);
}

void
X3DUnitEditor::on_unit_length_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (getUnitLengthEntry (), start_pos, end_pos);
}

void
X3DUnitEditor::on_unit_mass_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (getUnitMassEntry (), text, *position);
}

void
X3DUnitEditor::on_unit_mass_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (getUnitMassEntry (), start_pos, end_pos);
}

/*
 * Combos
 */

void
X3DUnitEditor::on_unit_angle_changed ()
{
	on_unit_changed (getUnitAngleCombo (), getUnitAngleAdjustment (), "angle", "radian");
}

void
X3DUnitEditor::on_unit_force_changed ()
{
	on_unit_changed (getUnitForceCombo (), getUnitForceAdjustment (), "force", "newton");
}

void
X3DUnitEditor::on_unit_length_changed ()
{
	on_unit_changed (getUnitLengthCombo (), getUnitLengthAdjustment (), "length", "metre");
}

void
X3DUnitEditor::on_unit_mass_changed ()
{
	on_unit_changed (getUnitMassCombo (), getUnitMassAdjustment (), "mass", "kilogram");
}

void
X3DUnitEditor::on_unit_changed (const Gtk::ComboBoxText & combo,
                                const Glib::RefPtr <Gtk::Adjustment> & adjustment,
                                const std::string & category,
                                const std::string & defaultUnit)
{
	if (changing)
		return;

	changing = true;

	std::string name  = "";
	long double value = adjustment -> get_value ();

	if (combo .get_active_row_number () < 0)
		name = combo .get_entry () -> get_text ();
	else
	   name = combo .get_active_id ();

	if (name .empty ())
	{
		name  = defaultUnit;
		value = 1;
		adjustment -> set_value (1);
	}
	else if (hasKey (category, name))
	{
		value = getKey (category, name);
		adjustment -> set_value (value);
	}
	
	getCurrentScene () -> setSpecificationVersion (X3D::LATEST_VERSION);
	getCurrentScene () -> updateUnit (category, name, value);

	getBrowserWindow () -> getCurrentPage () -> setModified (true);

	changing = false;
}

X3DUnitEditor::~X3DUnitEditor ()
{ }

} // puck
} // titania
