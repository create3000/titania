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

#ifndef __TITANIA_OUTLINE_EDITOR_CELL_RENDERER_OUTLINE_CELL_RENDERER_H__
#define __TITANIA_OUTLINE_EDITOR_CELL_RENDERER_OUTLINE_CELL_RENDERER_H__

#include "../OutlineTreeData.h"
#include "TextViewEditable.h"

#include <Titania/X3D.h>
#include <gtkmm.h>
#include <memory>

namespace titania {
namespace puck {

class OutlineCellRenderer :
	public Gtk::CellRendererText
{
public:

	OutlineCellRenderer (X3D::X3DBrowser* const);

	// Properties
	Glib::Property <OutlineTreeData*> &
	property_data ()
	{ return data_property; }

	const Glib::Property <OutlineTreeData*> &
	property_data () const
	{ return data_property; }

	// Edited signal
	typedef sigc::signal <void, const Glib::ustring &, const Glib::ustring &> signal_edited_t;

	virtual
	~OutlineCellRenderer ()
	{ }


private:

	void
	on_data ();

	OutlineIterType
	get_data_type () const;

	X3D::X3DChildObject*
	get_object () const;
	
	bool
	get_expanded () const;

	bool
	get_all_expanded () const;

	const Glib::RefPtr <Gdk::Pixbuf> &
	get_icon () const;

	const Glib::RefPtr <Gdk::Pixbuf> &
	get_access_type_icon () const;

	std::string
	get_node_name () const;

	std::string
	get_field_value () const;

	bool
	is_array () const;

	virtual
	void
	get_preferred_width_vfunc (Gtk::Widget & widget, int & minimum_width, int & natural_width) const final;

	virtual
	void
	get_preferred_height_for_width_vfunc (Gtk::Widget & widget, int width, int & minimum_height, int & natural_height) const final;

	virtual
	void
	get_preferred_height_vfunc (Gtk::Widget & widget, int & minimum_height, int & natural_height) const final;

	virtual
	void
	get_preferred_width_for_height_vfunc (Gtk::Widget & widget, int height, int & minimum_width, int & natural_width) const final;

	virtual
	Gtk::CellEditable*
	start_editing_vfunc (GdkEvent*, Gtk::Widget &, const Glib::ustring &, const Gdk::Rectangle &, const Gdk::Rectangle &, Gtk::CellRendererState) final;

	void
	on_editing_done ();

	bool
	set_field_value (X3D::X3DChildObject* const, const std::string &);

	virtual
	void
	render_vfunc (const Cairo::RefPtr <Cairo::Context> &, Gtk::Widget &, const Gdk::Rectangle &, const Gdk::Rectangle &, Gtk::CellRendererState) final;

	//	virtual
	//	bool
	//	activate_vfunc (GdkEvent*, Gtk::Widget &, const Glib::ustring &, const Gdk::Rectangle &, const Gdk::Rectangle &, Gtk::CellRendererState) final;
	//
	//	virtual
	//	Gtk::CellEditable*
	//	start_editing_vfunc (GdkEvent*, Gtk::Widget &, const Glib::ustring &, const Gdk::Rectangle &, const Gdk::Rectangle &, Gtk::CellRendererState) final;
	//
	//	// Manage editing_done event for color_cell_edit_ptr_
	//	void
	//	on_editing_done ();
	//
	//	// Raise the edited event
	//	void
	//	edited (const Glib::ustring & path, const Glib::ustring & new_text);

	typedef std::map <const X3D::X3DConstants::FieldType,  Glib::RefPtr <Gdk::Pixbuf>>               FieldTypeImageIndex;
	typedef std::map <const X3D::X3DConstants::AccessType, std::deque <Glib::RefPtr <Gdk::Pixbuf>>> AccessTypeImageIndex;

	Glib::Property <OutlineTreeData*> data_property;

	mutable Gtk::CellRendererPixbuf cellrenderer_icon;
	mutable Gtk::CellRendererPixbuf cellrenderer_access_type_icon;

	Glib::RefPtr <Gdk::Pixbuf> noneImage;
	Glib::RefPtr <Gdk::Pixbuf> baseNodeImage;
	FieldTypeImageIndex        fieldTypeImages;
	AccessTypeImageIndex       accessTypeImages;
	
	std::unique_ptr <TextViewEditable> textview;

};

} // puck
} // titania

#endif
