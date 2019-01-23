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

#include "X3DProfileComponentsEditor.h"

#include <Titania/X3D/Components/Core/WorldInfo.h>

namespace titania {
namespace puck {

struct X3DProfileComponentsEditor::Columns
{
	static constexpr int32_t COMPONENT = 0;
	static constexpr int32_t LEVEL     = 1;
};

X3DProfileComponentsEditor::X3DProfileComponentsEditor () :
	X3DScenePropertiesEditorInterface (),
	                            scene (getCurrentScene ()),
	                         changing (false)
{ }

void
X3DProfileComponentsEditor::initialize ()
{ }

void
X3DProfileComponentsEditor::on_profile_components_map ()
{
	getCurrentScene () .addInterest (&X3DProfileComponentsEditor::set_current_scene, this);

	set_current_scene ();
}

void
X3DProfileComponentsEditor::on_profile_components_unmap ()
{
	getCurrentScene () .removeInterest (&X3DProfileComponentsEditor::set_current_scene, this);

	scene -> profile_changed ()    .removeInterest (&X3DProfileComponentsEditor::set_profile,    this);
	scene -> components_changed () .removeInterest (&X3DProfileComponentsEditor::set_components, this);
}

void
X3DProfileComponentsEditor::set_current_scene ()
{
	scene -> profile_changed ()    .removeInterest (&X3DProfileComponentsEditor::set_profile,    this);
	scene -> components_changed () .removeInterest (&X3DProfileComponentsEditor::set_components, this);

	scene = getCurrentScene ();

	scene -> profile_changed ()    .addInterest (&X3DProfileComponentsEditor::set_profile,    this);
	scene -> components_changed () .addInterest (&X3DProfileComponentsEditor::set_components, this);

	// Update profile and components.

	set_profile ();
	set_components ();

	// Infer profile and components button.

	changing = true;

	const auto infer = createWorldInfo (getCurrentScene ()) -> getMetaData <bool> ("/Titania/Scene/inferProfileAndComponents", true);

	getInferProfileAndComponentsButton () .set_active (infer);

	changing = false;
}

void
X3DProfileComponentsEditor::set_profile ()
{
	changing = true;

	// Profile

	const auto & profile = getCurrentScene () -> getProfile ();

	getProfileButton () .set_active_text (profile ? profile -> getName () : "Full");

	changing = false;
}

void
X3DProfileComponentsEditor::set_components ()
{
	// Components

	getComponentsListStore () -> clear ();

	auto components = getCurrentScene () -> getComponents ();

	sortComponents (components);

	for (const auto & component : components)
	{
		const auto row = getComponentsListStore () -> append ();

		row -> set_value (Columns::COMPONENT, component -> getName ());
		row -> set_value (Columns::LEVEL,     basic::to_string (component -> getLevel (), std::locale::classic ()));
	}
}

void
X3DProfileComponentsEditor::on_profile_changed ()
{
	try
	{
		if (changing)
			return;

		const auto profile = getCurrentBrowser () -> getProfile (getProfileButton () .get_active_text ());

		getCurrentScene () -> setProfile (profile);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DProfileComponentsEditor::on_component_edited (const Glib::ustring & path, const Glib::ustring & text)
{
	const auto iter = getComponentsListStore () -> get_iter (Gtk::TreePath (path));

	iter -> set_value (Columns::COMPONENT, text);

	updateComponents ();
}

void
X3DProfileComponentsEditor::on_add_component_clicked ()
{
	const auto row = getComponentsListStore () -> append ();

	row -> set_value (Columns::COMPONENT, std::string ("Core"));

	updateComponents ();
}

void
X3DProfileComponentsEditor::on_remove_component_clicked ()
{
	const auto selected = getComponentsTreeView () .get_selection () -> get_selected ();

	getComponentsListStore () -> erase (selected);

	updateComponents ();
}

void
X3DProfileComponentsEditor::on_components_selection_changed ()
{
	getComponentsRemoveButton () .set_sensitive (not getComponentsTreeView () .get_selection () -> get_selected_rows () .empty ());
}

void
X3DProfileComponentsEditor::on_infer_profile_and_components_toggled ()
{
	const auto active = getInferProfileAndComponentsButton () .get_active ();

	getProfileBox ()    .set_sensitive (not active);
	getComponentsBox () .set_sensitive (not active);

	if (changing)
		return;

	if (active)
		createWorldInfo (getCurrentScene ()) -> removeMetaData ("/Titania/Scene/inferProfileAndComponents");
	else
		createWorldInfo (getCurrentScene ()) -> setMetaData <bool> ("/Titania/Scene/inferProfileAndComponents", active);
}

void
X3DProfileComponentsEditor::updateComponents ()
{
	const auto & supportedComponents = getBrowser () -> getSupportedComponents ();
	auto         components          = X3D::ComponentInfoArray ();

	for (const auto & row : getComponentsListStore () -> children ())
	{
		try
		{
			std::string componentName;

			row -> get_value (Columns::COMPONENT, componentName);

			const auto iter = std::find_if (supportedComponents .cbegin (), supportedComponents .cend (),
			[&] (const X3D::ComponentInfoPtr & component)
			{
				return component -> getName () == componentName;
			});

			if (iter == supportedComponents .cend ())
				continue;

			components .emplace_back (*iter);
		}
		catch (const X3D::X3DError & error)
		{ }
	}

	sortComponents (components);

	getCurrentScene () -> setComponents (components);
}

void
X3DProfileComponentsEditor::sortComponents (X3D::ComponentInfoArray & components) const
{
	std::sort (components .begin (), components .end (),
	[] (const X3D::ComponentInfoPtr & lhs, const X3D::ComponentInfoPtr & rhs)
	{
		return lhs -> getName () < rhs -> getName ();
	});
}

X3DProfileComponentsEditor::~X3DProfileComponentsEditor ()
{ }

} // puck
} // titania
