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

#include "PrototypeEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../NodePropertiesEditor/NodePropertiesEditor.h"
#include "../../ComposedWidgets/MFStringURLWidget.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"

#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>

namespace titania {
namespace puck {

PrototypeEditor::PrototypeEditor (X3DBrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DPrototypeEditorInterface (get_ui ("Editors/PrototypeEditor.glade"), gconf_dir ()),
	       nodePropertiesEditor (new NodePropertiesEditor (browserWindow)),
	                        url (new MFStringURLWidget (this,
	                             getURLTreeView (),
	                             getURLCellRendererText (),
	                             getURLAddButton (),
	                             getURLRemoveButton (),
	                             getURLReloadButton (),
	                             getURLChooserColumn (),
	                             "url")),
	                  nodeIndex (new NodeIndex (browserWindow)),
	           executionContext (),
	                  protoNode (),
							  urlNode (new X3D::FieldSet (getMasterBrowser ()))
{
	setup ();
}

void
PrototypeEditor::initialize ()
{
	X3DPrototypeEditorInterface::initialize ();

	// Add User-defined fields widged to gui

	nodePropertiesEditor -> getWindow () .set_transient_for (getWindow ());
	nodePropertiesEditor -> getUserDefinedFieldsWidget () .unparent ();
	getInterfaceBox () .pack_start (nodePropertiesEditor -> getUserDefinedFieldsWidget (), true, true);

	// Node index

	nodeIndex -> getHeaderBox () .set_visible (false);
	nodeIndex -> getFooterBox () .set_visible (false);
	nodeIndex -> getScrolledWindow () .set_size_request (0, 0);
	nodeIndex -> reparent (getInstancesBox (), getWindow ());

	// 

	getCurrentContext () .addInterest (this, &PrototypeEditor::set_executionContext);

	set_executionContext ();
}

void
PrototypeEditor::set_executionContext ()
{
	if (executionContext)
	{
		executionContext -> prototypes_changed ()   .removeInterest (this, &PrototypeEditor::on_create_prototype_menu);
		executionContext -> externProtos_changed () .removeInterest (this, &PrototypeEditor::on_create_prototype_menu);
	}

	executionContext = getCurrentContext ();

	executionContext -> prototypes_changed ()   .addInterest (this, &PrototypeEditor::on_create_prototype_menu);
	executionContext -> externProtos_changed () .addInterest (this, &PrototypeEditor::on_create_prototype_menu);

	// Setup widgets

	//getEditLabel ()   .set_text (_ ("Edit Protoype Properties"));
	getHeaderLabel () .set_text (_ ("Select a prototype to display its properties."));

	getCreateInstanceButton ()  .set_sensitive (false);
	getNameBox ()               .set_sensitive (false);
	getURLBox ()                .set_visible (false);
	getUpdateInstancesButton () .set_sensitive (false);

	on_create_prototype_menu ();

	nodePropertiesEditor -> X3DUserDefinedFieldsEditor::setNode (nullptr);

	urlNode -> removeUserDefinedField ("url");
	url -> setNodes ({ });
	nodeIndex -> setProto (nullptr);
}

void
PrototypeEditor::on_create_prototype_menu ()
{
	getEditPrototypeImage () .set_sensitive (false);
	getEditPrototypeImage () .set (Gtk::StockID ("Prototype"), Gtk::ICON_SIZE_DIALOG);

	getPrototypeImage () .set_sensitive (false);
	getPrototypeImage () .set (Gtk::StockID ("Prototype"), Gtk::ICON_SIZE_BUTTON);
	getPrototypeLabel () .set_text (_ ("Select a Prototype"));

	// Find all available proto objects

	const auto protoNodes = getCurrentContext () -> findProtoDeclarations ();

	// Remove all menu items

	for (const auto & widget : getPrototypeMenu () .get_children ())
		getPrototypeMenu () .remove (*widget);

	for (const auto & pair : protoNodes)
	{
		const auto image    = Gtk::manage (new Gtk::Image (Gtk::StockID (pair .second -> isExternproto () ? "ExternProto" : "Prototype"), Gtk::ICON_SIZE_MENU));
		const auto menuItem = Gtk::manage (new Gtk::ImageMenuItem (*image, pair .first));

		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (*this, &PrototypeEditor::set_prototype), pair .second));

		menuItem -> set_always_show_image (true);
		menuItem -> show ();

		getPrototypeMenu () .append (*menuItem);
	}
}

void
PrototypeEditor::set_prototype (const X3D::X3DPtr <X3D::X3DProtoDeclarationNode> & value)
{
	protoNode = value;

	// Header

	getEditPrototypeImage () .set_sensitive (true);
	getEditPrototypeImage () .set (Gtk::StockID (protoNode -> isExternproto () ? "ExternProto" : "Prototype"), Gtk::ICON_SIZE_DIALOG);

	//const auto editText   = protoNode -> isExternproto () ? _ ("Edit Extern Protoype Properties") : _ ("Edit Protoype Properties");
	const auto headerText = protoNode -> isExternproto () ? _ ("Extern Prototype »%s«") : _ ("Prototype »%s«");

	//getEditLabel ()   .set_text (editText);
	getHeaderLabel () .set_text (basic::sprintf (headerText, protoNode -> getName () .c_str ()));

	// Create instance button

	getCreateInstanceButton () .set_sensitive (true);
	getNameBox () .set_sensitive (true);
	getUpdateInstancesButton () .set_sensitive (true);

	// Select prototype button

	getPrototypeImage () .set_sensitive (true);
	getPrototypeImage () .set (Gtk::StockID (protoNode -> isExternproto () ? "ExternProto" : "Prototype"), Gtk::ICON_SIZE_BUTTON);
	getPrototypeLabel () .set_text (protoNode -> getName ());

	// Name entry and Interface

	getNameEntry () .set_text (protoNode -> getName ());

	nodePropertiesEditor -> X3DUserDefinedFieldsEditor::setNode (X3D::SFNode (protoNode));

	// URL

	getURLBox () .set_visible (protoNode -> isExternproto ());

	if (protoNode -> isExternproto ())
	{
	   X3D::X3DPtr <X3D::X3DUrlObject> urlObject (protoNode); 

		urlNode -> addUserDefinedField (urlObject -> url () .getAccessType (), "url", &urlObject -> url ());

		url -> setNodes ({ urlNode });
	}
	else
	{
		urlNode -> removeUserDefinedField ("url");

		url -> setNodes ({ });
	}

	nodeIndex -> setProto (protoNode);
}

void
PrototypeEditor::on_create_proto_clicked ()
{
	const auto typeName = getCurrentContext () -> getUniqueProtoName ("NewPrototype");
	const auto proto    = getCurrentContext () -> createProtoDeclaration (typeName, { });

	getCurrentContext () -> updateProtoDeclaration (typeName, proto);
}

void
PrototypeEditor::on_create_externproto_clicked ()
{
	const auto typeName    = getCurrentContext () -> getUniqueExternProtoName ("NewExternProto");
	const auto externProto = getCurrentContext () -> createExternProtoDeclaration (typeName, { }, { });

	getCurrentContext () -> updateExternProtoDeclaration (typeName, externProto);
}

void
PrototypeEditor::on_create_instance_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create %s"), protoNode -> getName () .c_str ()));
	const auto instance = getBrowserWindow () -> addPrototypeInstance (getCurrentContext (), protoNode -> getName () .c_str (), undoStep);

	getBrowserWindow () -> getSelection () -> setChildren ({ instance }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
PrototypeEditor::on_name_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (getNameEntry (), text, *position);
}

void
PrototypeEditor::on_name_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (getNameEntry (), start_pos, end_pos);
}

bool
PrototypeEditor::on_name_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
		{
			on_rename_clicked ();
			return true;
		}
		case GDK_KEY_Escape:
		{
			getNameEntry () .set_text (protoNode -> getName ());
			getRenameButton () .grab_focus ();
			return true;
		}
		default:
			break;
	}

	return false;
}

void
PrototypeEditor::on_rename_clicked ()
{
	const auto name = getNameEntry () .get_text ();

	if (protoNode -> isExternproto ())
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> ("Rename Extern Prototype");

		getBrowserWindow () -> updateExternProtoDeclaration (getCurrentContext (), name, X3D::ExternProtoDeclarationPtr (protoNode), undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	else
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> ("Rename Prototype");

		getBrowserWindow () -> updateProtoDeclaration (getCurrentContext (), name, X3D::ProtoDeclarationPtr (protoNode), undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);
	}
		
	on_create_prototype_menu ();
	set_prototype (protoNode);
}

void
PrototypeEditor::on_update_instances_clicked ()
{
	protoNode -> updateInstances ();
}

PrototypeEditor::~PrototypeEditor ()
{
	dispose ();
}

} // puck
} // titania
