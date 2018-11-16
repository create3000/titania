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

#include "PrototypeEditor.h"

#include "../NodePropertiesEditor/NodePropertiesEditor.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../ComposedWidgets/MFStringURLWidget.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileOpenDialog/FileImportAsExternProtoDialog.h"
#include "../../Dialogs/FileSaveDialog/FileExportProtoDialog.h"
#include "../../Editors/NodeIndex/NodeIndex.h"

#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>

namespace titania {
namespace puck {

PrototypeEditor::PrototypeEditor (X3DBrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DPrototypeEditorInterface (get_ui ("Editors/PrototypeEditor.glade")),
	                   nodeName (this, getNameEntry (), getRenameButton ()),
	                        url (this,
	                             getURLTreeView (),
	                             getURLCellRendererText (),
	                             getURLAddButton (),
	                             getURLRemoveButton (),
	                             getURLReloadButton (),
	                             getURLChooserColumn (),
	                             "url"),
	       nodePropertiesEditor (new NodePropertiesEditor (browserWindow)),
	                  nodeIndex (new NodeIndex (browserWindow)),
	           executionContext (),
	                  protoNode (),
							  urlNode (new X3D::FieldSet (getMasterBrowser ()))
{
	addChildObjects (executionContext, protoNode, urlNode);

	nodeName .name_changed () .addInterest (&PrototypeEditor::set_name,                 this);
	nodeName .name_changed () .addInterest (&PrototypeEditor::on_create_prototype_menu, this);

	urlNode -> hasEvents (false);
	urlNode -> setup ();

	nodeIndex -> setName (getName () + "." + nodeIndex -> getName ());

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

	nodeIndex -> reparent (getInstancesBox (), getWindow ());
	nodeIndex -> setShowWidget (true);

	// 

	getCurrentContext () .addInterest (&PrototypeEditor::set_executionContext, this);

	set_executionContext ();
}

void
PrototypeEditor::setProto (const X3D::X3DPtr <X3D::X3DProtoDeclarationNode> & protoNode)
{
	const auto path = getNodePath (protoNode);

	createWorldInfo (getCurrentScene ()) -> setMetaData ("/Titania/Prototype/path", X3D::MFString (path .begin (), path .end ()));

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

X3D::X3DPtr <X3D::X3DProtoDeclarationNode>
PrototypeEditor::getProto () const
{
	try
	{
		auto path = getWorldInfo (getCurrentScene ()) -> getMetaData <X3D::MFString> ("/Titania/Prototype/path");

		X3D::X3DProtoDeclarationNode* protoNode = getCurrentScene () -> findProtoDeclaration (path .at (0) .get ());

		path .pop_front ();

		for (const auto & name : basic::make_const_range (path))
		{
			const auto proto = dynamic_cast <X3D::ProtoDeclaration*> (protoNode);

			if (not proto)
				break;

			protoNode = proto -> findProtoDeclaration (name);
		}

		return X3D::X3DPtr <X3D::X3DProtoDeclarationNode> (protoNode);
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}

void
PrototypeEditor::setProtoDeclarationNode (const X3D::X3DProtoDeclarationNodePtr & value)
{
	if (protoNode)
		protoNode -> name_changed () .removeInterest (&PrototypeEditor::set_name, this);

	protoNode = value;

	if (protoNode)
	{
		//const auto editText   = protoNode -> isExternproto () ? _ ("Edit Extern Protoype Properties") : _ ("Edit Protoype Properties");
		//getEditLabel () .set_text (editText);
	
		// Header
	
		getEditPrototypeImage () .set_sensitive (true);
		getEditPrototypeImage () .set (Gtk::StockID (protoNode -> isExternproto () ? "ExternProto" : "Prototype"), Gtk::ICON_SIZE_BUTTON);
	
		// Create instance button
	
		getNameBox ()                   .set_sensitive (true);
		getConvertProtoButton ()        .set_visible (not protoNode -> isExternproto ());
		getFoldExternProtoBackButton () .set_visible (protoNode -> isExternproto ());
		getFoldExternProtoBackButton () .set_sensitive (protoNode -> checkLoadState () == X3D::COMPLETE_STATE);
		getCreateInstanceButton ()      .set_sensitive (true);
		getUpdateInstancesButton ()     .set_sensitive (true);
	
		// Select prototype button
	
		getPrototypeImage () .set_sensitive (true);
		getPrototypeImage () .set (Gtk::StockID (protoNode -> isExternproto () ? "ExternProto" : "Prototype"), Gtk::ICON_SIZE_BUTTON);
	
		// Name entry and Interface
	
		nodePropertiesEditor -> X3DUserDefinedFieldsEditor::setNode (protoNode);
	
		// Name
	
		protoNode -> name_changed () .addInterest (&PrototypeEditor::set_name, this);
		nodeName .setNode (protoNode);
		set_name ();
	
		// URL
	
		getURLScrolledWindow () .set_visible (protoNode -> isExternproto ());
	
		if (protoNode -> isExternproto ())
		{
		   const X3D::X3DPtr <X3D::X3DUrlObject> urlObject (protoNode); 
	
			urlNode -> addUserDefinedField (urlObject -> url () .getAccessType (), "url", &urlObject -> url ());
	
			url .setNodes ({ urlNode });
		}
		else
		{
			urlNode -> removeUserDefinedField ("url");
	
			url .setNodes ({ });
		}
	
		nodeIndex -> setProto (protoNode);
	}
	else
	{
		// Setup widgets
	
		//getEditLabel ()   .set_text (_ ("Edit Protoype Properties"));
		getHeaderBar () .set_subtitle (_ ("Select a prototype to display its properties."));
	
		getNameBox ()                   .set_sensitive (false);
		getConvertProtoButton ()        .set_visible (false);
		getFoldExternProtoBackButton () .set_visible (false);
		getCreateInstanceButton ()      .set_sensitive (false);
		getURLScrolledWindow ()         .set_visible (false);
		getUpdateInstancesButton ()     .set_sensitive (false);

		nodePropertiesEditor -> X3DUserDefinedFieldsEditor::setNode (nullptr);
	
		nodeName .setNode (nullptr);
		url .setNodes ({ });
		urlNode -> removeUserDefinedField ("url");
		nodeIndex -> setProto (nullptr);
	}
}

void
PrototypeEditor::set_executionContext ()
{
	if (executionContext)
	{
		executionContext -> prototypes_changed ()   .removeInterest (&PrototypeEditor::set_prototypes, this);
		executionContext -> externProtos_changed () .removeInterest (&PrototypeEditor::set_prototypes, this);
	}

	executionContext = getCurrentContext ();

	executionContext -> prototypes_changed ()   .addInterest (&PrototypeEditor::set_prototypes, this);
	executionContext -> externProtos_changed () .addInterest (&PrototypeEditor::set_prototypes, this);

	set_prototypes ();
}

void
PrototypeEditor::set_prototypes ()
{
	on_create_prototype_menu ();

	setProtoDeclarationNode (getProto ());
}

void
PrototypeEditor::set_name ()
{
	const auto headerText = protoNode -> isExternproto () ? _ ("Extern Prototype »%s«") : _ ("Prototype »%s«");

	getHeaderBar () .set_subtitle (basic::sprintf (headerText, protoNode -> getName () .c_str ()));
	getPrototypeLabel () .set_text (protoNode -> getName ());

	url .setFragment (protoNode -> getName ());

	setProto (protoNode);
}

void
PrototypeEditor::on_create_prototype_menu ()
{
	getEditPrototypeImage () .set_sensitive (false);
	getEditPrototypeImage () .set (Gtk::StockID ("Prototype"), Gtk::ICON_SIZE_BUTTON);

	getPrototypeImage () .set_sensitive (false);
	getPrototypeImage () .set (Gtk::StockID ("Prototype"), Gtk::ICON_SIZE_BUTTON);
	getPrototypeLabel () .set_text (_ ("Select a Prototype"));

	// Find all available proto objects

	const auto protoIndex = getCurrentContext () -> findProtoDeclarations ();
	auto       protoNodes = std::vector <X3D::X3DPtr <X3D::X3DProtoDeclarationNode>> ();

	for (const auto & pair : protoIndex)
		protoNodes .emplace_back (std::move (pair .second));

	// Sort by name and extern protos on top

	std::sort (protoNodes .begin (), protoNodes .end (),
	           [ ] (const X3D::X3DProtoDeclarationNodePtr & lhs, const X3D::X3DProtoDeclarationNodePtr & rhs)
	           { return std::pair (not lhs -> isExternproto (), lhs -> getName ()) < std::pair (not rhs -> isExternproto (), rhs -> getName ()); });

	// Remove all menu items

	for (const auto & widget : getSelectPrototypeMenu () .get_children ())
		getSelectPrototypeMenu () .remove (*widget);

	for (const auto & protoNode : protoNodes)
	{
		const auto image    = Gtk::manage (new Gtk::Image (Gtk::StockID (protoNode -> isExternproto () ? "ExternProto" : "Prototype"), Gtk::ICON_SIZE_MENU));
		const auto menuItem = Gtk::manage (new Gtk::ImageMenuItem (*image, protoNode -> getName ()));

		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &PrototypeEditor::on_prototype_activate), X3D::X3DPtr <X3D::X3DProtoDeclarationNode> (protoNode)));

		menuItem -> set_always_show_image (true);
		menuItem -> show ();

		getSelectPrototypeMenu () .append (*menuItem);
	}
}

void
PrototypeEditor::on_prototype_activate (const X3D::X3DProtoDeclarationNodePtr & protoNode)
{
	setProtoDeclarationNode (protoNode);

	setProto (protoNode);
}

void
PrototypeEditor::on_create_proto_popup_clicked ()
{
	getCreateProtoPopover () .popup ();
}

void
PrototypeEditor::on_create_proto_clicked ()
{
	getCreateProtoPopover () .popdown ();

	const auto name     = getCurrentContext () -> getUniqueProtoName ("NewPrototype");
	const auto proto    = getCurrentContext () -> createProtoDeclaration (name, { });
	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create New Prototype »%s«"), name .c_str ()));

	X3D::X3DEditor::updateProtoDeclaration (getCurrentContext (), name, proto, undoStep);

	setProtoDeclarationNode (proto);
	setProto (proto);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
PrototypeEditor::on_create_externproto_clicked ()
{
	getCreateProtoPopover () .popdown ();

	const auto name        = getCurrentContext () -> getUniqueExternProtoName ("NewExternProto");
	const auto externproto = getCurrentContext () -> createExternProtoDeclaration (name, { }, { });
	const auto undoStep    = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create New Extern Proto »%s«"), name .c_str ()));

	X3D::X3DEditor::updateExternProtoDeclaration (getCurrentContext (), name, externproto, undoStep);

	setProtoDeclarationNode (externproto);
	setProto (externproto);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
PrototypeEditor::on_import_extern_proto_clicked ()
{
	const auto dialog = addDialog <FileImportAsExternProtoDialog> ("FileImportAsExternProtoDialog");

	dialog -> run ();
}

void
PrototypeEditor::on_convert_prototype_clicked ()
{
	const auto proto    = X3D::ProtoDeclarationPtr (protoNode);
	const auto dialog   = addDialog <FileExportProtoDialog> ("FileExportProtoDialog");
	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Convert Prototype »%s« To Extern Proto"), proto -> getName () .c_str ()));

	if (dialog -> run (proto, undoStep))
	{
		setProtoDeclarationNode (getCurrentContext () -> getExternProtoDeclaration (proto -> getName ()));

		getBrowserWindow () -> addUndoStep (undoStep);
	}
}

void
PrototypeEditor::on_fold_extern_proto_back_clicked ()
{
	const auto externproto = X3D::ExternProtoDeclarationPtr (protoNode);
	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Fold Extern Proto »%s« Back Into Scene"), externproto -> getName () .c_str ()));

	X3D::X3DEditor::foldExternProtoBackIntoScene (externproto, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
PrototypeEditor::on_create_instance_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create %s"), protoNode -> getName () .c_str ()));
	const auto instance = X3D::X3DEditor::createProto (getCurrentWorld (), getCurrentContext (), protoNode -> getName () .c_str (), undoStep);

	getBrowserWindow () -> getSelection () -> setNodes ({ instance }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
PrototypeEditor::on_update_instances_clicked ()
{
	X3D::X3DEditor::requestUpdateInstances (protoNode, std::make_shared <X3D::UndoStep> ());
}

PrototypeEditor::~PrototypeEditor ()
{
	dispose ();
}

} // puck
} // titania
