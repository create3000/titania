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

#ifndef __TITANIA_WIDGETS_OUTLINE_EDITOR_CELL_RENDERER_OUTLINE_CELL_RENDERER_H__
#define __TITANIA_WIDGETS_OUTLINE_EDITOR_CELL_RENDERER_OUTLINE_CELL_RENDERER_H__

#include "../OutlineTreeData.h"

#include <gtkmm.h>
#include <memory>

namespace titania {
namespace puck {

enum class OutlineCellContent
{
	NONE,
	ICON,
	NAME,
	INPUT,
	OUTPUT,
	INPUT_CONNECTOR,
	OUTPUT_CONNECTOR

};

class OutlineTreeModel;
class TextViewEditable;
class X3DOutlineTreeView;

class OutlineCellRenderer :
	public Gtk::CellRendererText
{
public:

	///  @name Construction

	OutlineCellRenderer (const X3D::BrowserPtr & browser, X3DOutlineTreeView* const treeView);

	void
	initialize ();

	///  @name Properties

	Glib::Property <OutlineTreeData*> &
	property_data ()
	{ return data_property; }

	const Glib::Property <OutlineTreeData*> &
	property_data () const
	{ return data_property; }

	// Selection handling

	void
	add_routes (const OutlineRoutes & value)
	{ routes .insert (value .begin (), value .end ()); }

	const OutlineRoutes &
	get_routes () const
	{ return routes; }

	void
	clear_routes ()
	{ routes .clear (); }

	///  @name Operations

	OutlineCellContent
	pick (Gtk::Widget &, const Gdk::Rectangle &, double, double);

	///  @name Destruction

	virtual
	~OutlineCellRenderer ();


private:

	void
	set_material_preview ();

	void
	set_texture_preview ();

	void
	on_data ();

	OutlineIterType
	get_data_type () const;

	X3D::X3DChildObject*
	get_object () const;

	OutlineExpanded
	get_expanded () const;

	const Glib::RefPtr <Gdk::Pixbuf> &
	get_icon () const;

	const Glib::RefPtr <Gdk::Pixbuf> &
	get_access_type_icon (X3D::AccessType & accessType) const;

	std::string
	get_node_name (const X3D::SFNode &, std::string name) const;

	bool
	is_array () const;

	virtual
	void
	get_preferred_width_vfunc (Gtk::Widget & widget, int & minimum_width, int & natural_width) const final override;

	virtual
	void
	get_preferred_height_for_width_vfunc (Gtk::Widget & widget, int width, int & minimum_height, int & natural_height) const final override;

	virtual
	void
	get_preferred_height_vfunc (Gtk::Widget & widget, int & minimum_height, int & natural_height) const final override;

	virtual
	void
	get_preferred_width_for_height_vfunc (Gtk::Widget & widget, int height, int & minimum_width, int & natural_width) const final override;

	virtual
	Gtk::CellEditable*
	start_editing_vfunc (GdkEvent*, Gtk::Widget &, const Glib::ustring &, const Gdk::Rectangle &, const Gdk::Rectangle &, Gtk::CellRendererState) final override;

	void
	on_editing_done ();

	void
	on_remove_widget ();

	bool
	set_field_value (const X3D::SFNode &, X3D::X3DFieldDefinition* const, const std::string &);

	bool
	set_field_value (const X3D::SFNode &, X3D::X3DFieldDefinition* const, const std::string &, const bool);

	static
	void
	row_changed (const Glib::RefPtr <OutlineTreeModel> &, const Glib::ustring &);

	virtual
	void
	render_vfunc (const Cairo::RefPtr <Cairo::Context> &, Gtk::Widget &, const Gdk::Rectangle &, const Gdk::Rectangle &, Gtk::CellRendererState) final override;

	void
	render_routes (const Cairo::RefPtr <Cairo::Context> &, Gtk::Widget &, const Gdk::Rectangle &, const Gdk::Rectangle &, int, Gtk::CellRendererState);

	bool
	have_selected_routes (const OutlineRoutes &);

	void
	set_camera (const X3D::SFNode & node);
	
	void
	set_camera (double width, double height);

	using FieldTypeImageIndex  = std::map <const X3D::FieldType,  Glib::RefPtr <Gdk::Pixbuf>>;
	using AccessTypeImageIndex = std::map <const X3D::AccessType, std::vector <Glib::RefPtr <Gdk::Pixbuf>>>;

	X3DOutlineTreeView* const treeView;

	Glib::Property <OutlineTreeData*> data_property;

	mutable Gtk::CellRendererPixbuf cellrenderer_icon;
	mutable Gtk::CellRendererPixbuf cellrenderer_material_icon;
	mutable Gtk::CellRendererPixbuf cellrenderer_texture_icon;
	mutable Gtk::CellRendererPixbuf cellrenderer_access_type_icon;

	Glib::RefPtr <Gdk::Pixbuf> noneImage;
	Glib::RefPtr <Gdk::Pixbuf> executionContextImage;
	Glib::RefPtr <Gdk::Pixbuf> baseNodeImage;
	Glib::RefPtr <Gdk::Pixbuf> baseNodeUImage;
	Glib::RefPtr <Gdk::Pixbuf> NULLImage;
	Glib::RefPtr <Gdk::Pixbuf> prototypeImage;
	Glib::RefPtr <Gdk::Pixbuf> externProtoImage;
	Glib::RefPtr <Gdk::Pixbuf> importedNodeImage;
	Glib::RefPtr <Gdk::Pixbuf> exportedNodeImage;
	Glib::RefPtr <Gdk::Pixbuf> sharedNodeImage;
	Glib::RefPtr <Gdk::Pixbuf> sharedNodeUImage;
	Glib::RefPtr <Gdk::Pixbuf> routeImage;
	FieldTypeImageIndex fieldTypeImages;
	AccessTypeImageIndex accessTypeImages;
	X3D::AccessType accessType;

	OutlineRoutes routes;

	const X3D::BrowserPtr materialPreview;
	const X3D::BrowserPtr texturePreview;

	std::unique_ptr <TextViewEditable> textview;

};

} // puck
} // titania

#endif
