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

#ifndef __TITANIA_EDITORS_NODE_PROPERTIES_EDITOR_X3DUSER_DEFINED_FIELDS_EDITOR_H__
#define __TITANIA_EDITORS_NODE_PROPERTIES_EDITOR_X3DUSER_DEFINED_FIELDS_EDITOR_H__

#include "../../UserInterfaces/X3DNodePropertiesEditorInterface.h"

namespace titania {
namespace puck {

class X3DUserDefinedFieldsEditor :
	virtual public X3DNodePropertiesEditorInterface
{
public:

	///  @name Member access

	void
	setNode (const X3D::SFNode & node);

	const Glib::RefPtr <Gtk::ListStore> &
	getUserDefinedFieldsListStore () const
	{ return userDefinedFieldsListStore; }

	///  @name Destruction

	virtual
	~X3DUserDefinedFieldsEditor () override;


protected:

	///  @name Construction

	X3DUserDefinedFieldsEditor ();

	virtual
	void
	initialize () override;

	virtual
	void
	store ();


private:

	//  @name Member types

	class Columns :
		public Gtk::TreeModel::ColumnRecord
	{
	public:

		Columns ()
		{
			add (type);
			add (name);
			add (accessType);
		}

		Gtk::TreeModelColumn <Glib::RefPtr <Gdk::Pixbuf>>  type;
		Gtk::TreeModelColumn <std::string>                 name;
		Gtk::TreeModelColumn <Glib::RefPtr <Gdk::Pixbuf>>  accessType;

	};

	using FieldToFieldIndex = std::map <X3D::X3DFieldDefinition*, X3D::X3DFieldDefinition*>;

	///  @name Event handlers

	virtual
	bool
	on_user_defined_field_focus_in_event (GdkEventFocus* event) final override;

	virtual
	bool
	on_user_defined_field_focus_out_event (GdkEventFocus* event) final override;

	virtual
	bool
	on_user_defined_field_key_press_event (GdkEventKey* event) final override;

	virtual
	void
	on_user_defined_field_changed () final override;

	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext>&, int, int, const Gtk::SelectionData &, guint,  guint);

	virtual
	void
	on_user_defined_field_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) final override;

	virtual
	void
	on_add_user_defined_field_clicked () final override;

	virtual
	void
	on_remove_user_defined_field_clicked () final override;

	void
	on_access_type_activate (const std::string &);

	void
	on_field_type_activate (const std::string &);

	virtual
	void
	on_field_name_insert_text (const Glib::ustring &, int*) final override;

	virtual
	void
	on_field_name_delete_text (int, int) final override;

	virtual
	void
	on_field_name_changed () final override;

	bool
	hasField (const std::string &) const;

	virtual
	void
	on_add_field_ok_clicked () final override;

	virtual
	void
	on_add_field_cancel_clicked () final override;

	void
	addUserDefinedField (X3D::X3DFieldDefinition* const);

	void
	updateUserDefinedField (const X3D::AccessType accessType, const std::string & typeName, const std::string & name, X3D::X3DFieldDefinition* const field);

	void
	set_fields ();

	void
	connectFields (const X3D::SFTime &);

	///  @name Members

	X3D::SFNode                   node;
	Columns                       columns;
	Glib::RefPtr <Gtk::ListStore> userDefinedFieldsListStore;
	X3D::X3DFieldDefinition*      userDefinedField;
	bool                          editing;
	bool                          isExternproto;

};

} // puck
} // titania

#endif
