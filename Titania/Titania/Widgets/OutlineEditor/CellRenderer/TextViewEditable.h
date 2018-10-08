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

#ifndef __TITANIA_WIDGETS_OUTLINE_EDITOR_CELL_RENDERER_TEXT_VIEW_EDITABLE_H__
#define __TITANIA_WIDGETS_OUTLINE_EDITOR_CELL_RENDERER_TEXT_VIEW_EDITABLE_H__

#include "../X3DOutlineTreeView.h"
#include "X3DTextViewEditable.h"
#include "OutlineFields.h"

#include <Titania/X3D.h>

namespace titania {
namespace puck {

class TextViewEditable :
	public X3DTextViewEditable
{
public:

	TextViewEditable (X3DOutlineTreeView* const,
	                  const X3D::SFNode &,
	                  X3D::X3DFieldDefinition* const,
	                  const Glib::ustring &,
	                  const bool,
	                  const bool);

	const X3D::SFNode &
	get_node () const
	{ return node; }

	X3D::X3DFieldDefinition*
	get_field () const
	{ return field; }

	const Glib::ustring &
	get_path () const
	{ return path; }

	virtual
	~TextViewEditable () final override;


private:

	void
	on_textview_populate_popup (Gtk::Menu*);

	void
	on_trigger_event ();

	void
	on_toggle_value ();

	void
	on_current_time ();

	template <class Type>
	void
	on_normalize_vector ();

	template <class Type>
	void
	on_normalize_vectors ();

	void
	on_reset_activate ();

	virtual
	void
	on_remove_widget () final override;

	X3DOutlineTreeView* const treeView;
	X3D::SFNode node;
	X3D::X3DFieldDefinition* field;
	const Glib::ustring path;
	const bool useLocale;

};

template <class Type>
void
TextViewEditable::on_normalize_vector ()
{
	const auto & scene = treeView -> getCurrentScene ();
	auto         value = Type (static_cast <Type*> (field) -> normalize ());

	value .setUnit (field -> getUnit ());

	set_text (puck::get_field_value (scene, &value, false, treeView -> get_use_locale ()));

	editing_done ();
}

template <class Type>
void
TextViewEditable::on_normalize_vectors ()
{
	const auto & scene   = treeView -> getCurrentScene ();
	const auto & vectors = *static_cast <Type*> (field);
	auto         value   = Type ();

	value .setUnit (field -> getUnit ());

	for (const auto & vector : vectors)
		value .emplace_back (normalize (vector));

	set_text (puck::get_field_value (scene, &value, false, treeView -> get_use_locale ()));

	editing_done ();
}

} // puck
} // titania

#endif
