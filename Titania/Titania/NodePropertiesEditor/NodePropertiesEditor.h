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

#ifndef __TITANIA_NODE_PROPERTIES_EDITOR_NODE_PROPERTIES_EDITOR_H__
#define __TITANIA_NODE_PROPERTIES_EDITOR_NODE_PROPERTIES_EDITOR_H__

#include "../UserInterfaces/X3DNodePropertiesEditorInterface.h"

namespace titania {
namespace puck {

class BrowserWindow;

class NodePropertiesEditor :
	public X3DNodePropertiesEditorInterface
{
public:

	NodePropertiesEditor (BrowserWindow* const, const X3D::SFNode &);

	virtual
	~NodePropertiesEditor ();


private:

	class UserDefinedFieldsColumns :
		public Gtk::TreeModel::ColumnRecord
	{
	public:

		UserDefinedFieldsColumns ()
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

	///  @name Operations

	void
	validateIdOnInsert (Gtk::Entry &, const Glib::ustring &, int);

	void
	validateIdOnDelete (Gtk::Entry &, int, int);

	///  @name TypeName

	virtual
	void
	on_type_name_insert_text (const Glib::ustring &, int*) final override;

	virtual
	void
	on_type_name_delete_text (int, int) final override;

	///  @name Name

	virtual
	void
	on_name_insert_text (const Glib::ustring &, int*) final override;

	virtual
	void
	on_name_delete_text (int, int) final override;

	///  @name User defined fields

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
	on_add_field_clicked () final override;

	virtual
	void
	on_remove_field_clicked () final override;

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

	virtual
	void
	on_add_field_ok_clicked () final override;

	virtual
	void
	on_add_field_cancel_clicked () final override;

	bool
	existsField (const std::string &) const;

	void
	addField (X3D::X3DFieldDefinition* const);

	void
	addUserDefinedField (X3D::X3DFieldDefinition* const);

	void
	replaceUserDefinedField (X3D::X3DFieldDefinition* const, X3D::X3DFieldDefinition* const);

	///  @name CDATA field

	virtual
	void
	on_edit_cdata_clicked () final override;

	///  @name Dialog buttons

	virtual
	void
	on_ok () final override;

	virtual
	void
	on_apply () final override;

	virtual
	void
	on_cancel () final override;

	static
	void
	updateNamedNode (BrowserWindow* const, const std::string &, const X3D::SFNode &);

	static
	void
	setUserDefinedFields (BrowserWindow* const,
	                      const X3D::SFNode &,
	                      const X3D::FieldDefinitionArray &,
	                      const X3D::FieldDefinitionArray &);

	///  @name Static members

	static const std::string userDefinedFieldsDragDataType;

	///  @name Members

	const X3D::SFNode node;

	// User defined fields

	UserDefinedFieldsColumns      userDefinedFieldsColumns;
	Glib::RefPtr <Gtk::ListStore> userDefinedFieldsListStore;
	X3D::X3DFieldDefinition*      userDefinedField;
	X3D::FieldDefinitionArray     userDefinedFields;
	X3D::FieldIndex               fields;
	FieldToFieldIndex             fieldsToReplace;
	X3D::FieldDefinitionArray     fieldsToRemove;
	bool                          editUserDefinedField;

};

} // puck
} // titania

#endif
