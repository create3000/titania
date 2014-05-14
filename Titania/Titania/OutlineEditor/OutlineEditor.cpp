/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "OutlineEditor.h"

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"
#include "../OutlineEditor/OutlineTreeModel.h"
#include "../OutlineEditor/OutlineTreeViewEditor.h"

namespace titania {
namespace puck {

OutlineEditor::OutlineEditor (BrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DOutlineEditorInterface (get_ui ("OutlineEditor.xml"), gconf_dir ()),
	                 treeview (new OutlineTreeViewEditor (browserWindow, X3D::X3DExecutionContextPtr (getBrowser () -> getExecutionContext ()))),
	               sceneGroup (),
	               sceneIndex (),
	                   scenes (),
	                     path (),
	                 realized (false)
{ }

void
OutlineEditor::on_map ()
{
	getBrowserWindow () -> getSideBarLabel () .set_text (_ ("Outline Editor"));
}

void
OutlineEditor::initialize ()
{
	X3DOutlineEditorInterface::initialize ();

	getViewport () .add (*treeview);
	treeview -> show ();

	// Register browser interest
	getWorld () .addInterest (this, &OutlineEditor::set_world);
	getBrowser () -> getExecutionContext () .addInterest (this, &OutlineEditor::set_scene);
}

void
OutlineEditor::restoreSession ()
{
	getShowExternProtosMenuItem ()   .set_active (getConfig () .getBoolean ("showExternProtos"));
	getShowPrototypesMenuItem ()     .set_active (getConfig () .getBoolean ("showPrototypes"));
	getShowImportedNodesMenuItem ()  .set_active (getConfig () .getBoolean ("showImportedNodes"));
	getShowExportedNodesMenuItem ()  .set_active (getConfig () .getBoolean ("showExportedNodes"));
	getExpandExternProtosMenuItem () .set_active (getConfig () .getBoolean ("expandExternProtos"));
	getExpandInlineNodesMenuItem ()  .set_active (getConfig () .getBoolean ("expandInlineNodes"));

	realized = true;

	set_world ();
	set_scene ();
}

void
OutlineEditor::set_world ()
{
	addSceneMenuItem (nullptr, getWorld ());
}

void
OutlineEditor::set_scene ()
{
	if (not realized)
		return;

	X3D::ScenePtr currentScene (treeview -> get_model () -> get_execution_context ());

	addSceneMenuItem (currentScene, getBrowser () -> getExecutionContext ()) -> set_active (true);

	treeview -> set_execution_context (X3D::X3DExecutionContextPtr (getBrowser () -> getExecutionContext ()));
}

// Pointing Device

bool
OutlineEditor::on_button_press_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 3:
		{
			select (event -> x, event -> y);

			getPopupMenu () .popup (event -> button, event -> time);
			return true;
		}
		default:
			break;
	}

	return false;
}

// Edit Menu Item

void
OutlineEditor::on_remove_activate ()
{
	__LOG__ << path .to_string () << std::endl;
}

void
OutlineEditor::OutlineEditor::on_create_instance_activate ()
{
	__LOG__ << path .to_string () << std::endl;
}

void
OutlineEditor::on_set_root_to_this_scene_activate ()
{
	const Gtk::TreeIter iter = treeview -> get_model () -> get_iter (path);

	switch (treeview -> get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
		{
			const auto & sfnode   = *static_cast <X3D::SFNode*> (treeview -> get_object (iter));
			const auto inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());

			addSceneMenuItem (getBrowser () -> getExecutionContext (), inlineNode -> getInternalScene ()) -> activate ();
			break;
		}
		case OutlineIterType::ExternProto:
		{
			const auto & sfnode    = *static_cast <X3D::SFNode*> (treeview -> get_object (iter));
			const auto externProto = dynamic_cast <X3D::ExternProto*> (sfnode .getValue ());

			if (externProto -> checkLoadState () == X3D::NOT_STARTED_STATE)
				externProto -> requestImmediateLoad ();

			if (externProto -> checkLoadState () == X3D::COMPLETE_STATE)
				addSceneMenuItem (getBrowser () -> getExecutionContext (), externProto -> getInternalScene ()) -> activate ();

			break;
		}
		default:
			break;
	}
}

void
OutlineEditor::on_scene_activate (Gtk::RadioMenuItem* const menuItem, const size_t index)
{
	try
	{
		if (menuItem -> get_active ())
		{
			const auto & scene = scenes [index] .first;

			if (scene not_eq getBrowser () -> getExecutionContext ())
				getBrowser () -> replaceWorld (scene);
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

Gtk::RadioMenuItem*
OutlineEditor::addSceneMenuItem (const X3D::ScenePtr & currentScene, const X3D::ScenePtr & scene)
{
	const auto basename = scene -> getWorldURL () .basename ();
	const auto iter     = sceneIndex .find (scene);

	getSceneLabel () .set_markup ("<i><b>Scene</b> »" + Glib::Markup::escape_text (basename) + "«</i>");

	if (currentScene)
	{
		if (iter not_eq sceneIndex .end ())
			return scenes [iter -> second] .second;

		// Remove menu items.

		const auto iter = sceneIndex .find (currentScene);

		if (iter not_eq sceneIndex .end ())
		{
			const size_t first = iter -> second + 1;
		
			for (size_t i = first, size = scenes .size (); i < size; ++ i)
			{
				sceneIndex .erase (scenes [i] .first);
				getSceneMenu () .remove (*scenes [i] .second);
			}

			scenes .resize (first);
		}
	}
	else
	{
		// Remove all menu items.

		for (const auto & widget : getSceneMenu () .get_children ())
			getSceneMenu () .remove (*widget);

		sceneIndex .clear ();
		scenes .clear ();
	}

	// Add menu item.

	const auto menuItem = Gtk::manage (new Gtk::RadioMenuItem (sceneGroup, basename));
	menuItem -> set_active (true);
	menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (*this, &OutlineEditor::on_scene_activate), menuItem, scenes .size ()));
	menuItem -> show ();

	sceneIndex .emplace (scene, scenes .size ());
	scenes .emplace_back (scene, menuItem);

	getSceneMenu () .append (*menuItem);
	
	return menuItem;
}

// View Menu Item

void
OutlineEditor::on_show_extern_protos_toggled ()
{
	getConfig () .setItem ("showExternProtos", getShowExternProtosMenuItem () .get_active ());
	treeview -> set_show_extern_protos (getShowExternProtosMenuItem () .get_active ());
	set_scene ();
}

void
OutlineEditor::on_show_prototypes_toggled ()
{
	getConfig () .setItem ("showPrototypes", getShowPrototypesMenuItem () .get_active ());
	treeview -> set_show_prototypes (getShowPrototypesMenuItem () .get_active ());
	set_scene ();
}

void
OutlineEditor::on_show_imported_nodes_toggled ()
{
	getConfig () .setItem ("showImportedNodes", getShowImportedNodesMenuItem () .get_active ());
	treeview -> set_show_imported_nodes (getShowImportedNodesMenuItem () .get_active ());
	set_scene ();
}

void
OutlineEditor::on_show_exported_nodes_toggled ()
{
	getConfig () .setItem ("showExportedNodes", getShowExportedNodesMenuItem () .get_active ());
	treeview -> set_show_exported_nodes (getShowExportedNodesMenuItem () .get_active ());
	set_scene ();
}

void
OutlineEditor::on_expand_extern_protos_toggled ()
{
	getConfig () .setItem ("expandExternProtos", getExpandExternProtosMenuItem () .get_active ());
	treeview -> set_expand_extern_protos (getExpandExternProtosMenuItem () .get_active ());
	set_scene ();
}

void
OutlineEditor::on_expand_inline_nodes_toggled ()
{
	getConfig () .setItem ("expandInlineNodes", getExpandInlineNodesMenuItem () .get_active ());
	treeview -> set_expand_inline_nodes (getExpandInlineNodesMenuItem () .get_active ());
	set_scene ();
}

void
OutlineEditor::select (const double x, const double y)
{
	path = getNodeAtPosition (x, y);

	bool isBaseNode    = false;
	bool isExternProto = false;
	bool isPrototype   = false;
	bool isInlineNode  = false;
	bool isLocalNode   = false;

	if (not path .empty ())
	{
		const Gtk::TreeIter iter = treeview -> get_model () -> get_iter (path);
	
		switch (treeview -> get_data_type (iter))
		{
			case OutlineIterType::X3DBaseNode:
			{
				const auto & sfnode    = *static_cast <X3D::SFNode*> (treeview -> get_object (iter));
				const auto   inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());

				isBaseNode   = sfnode .getValue ();
				isInlineNode = inlineNode and inlineNode -> checkLoadState () == X3D::COMPLETE_STATE;
				isLocalNode  = sfnode -> getExecutionContext () == getBrowser () -> getExecutionContext ();
				break;
			}
			case OutlineIterType::ExternProto:
			{
				const auto & sfnode      = *static_cast <X3D::SFNode*> (treeview -> get_object (iter));
				const auto   externProto = dynamic_cast <X3D::ExternProto*> (sfnode .getValue ());

			   isExternProto = externProto -> checkLoadState () not_eq X3D::FAILED_STATE;
				isLocalNode   = sfnode -> getExecutionContext () == getBrowser () -> getExecutionContext ();
				break;
			}
			case OutlineIterType::Prototype:
			{
				const auto & sfnode = *static_cast <X3D::SFNode*> (treeview -> get_object (iter));

				isPrototype = sfnode .getValue ();
				isLocalNode = sfnode -> getExecutionContext () == getBrowser () -> getExecutionContext ();
				break;
			}
			default:
				break;
		}
	}

	getRemoveMenuItem ()             .set_sensitive (isLocalNode and isBaseNode);
	getCreateInstanceMenuItem ()     .set_sensitive (isLocalNode and (isPrototype or isExternProto));
	getSetRootToThisSceneMenuItem () .set_sensitive (isExternProto or isInlineNode);
}

Gtk::TreePath
OutlineEditor::getNodeAtPosition (const double x, const double y)
{
	Gtk::TreePath path = getPathAtPosition (x, y);

	if (path .empty ())
		return path;

	const Gtk::TreeIter iter = treeview -> get_model () -> get_iter (path);

	switch (treeview -> get_data_type (iter))
	{
		case OutlineIterType::X3DField:
		case OutlineIterType::X3DExecutionContext:
		{
			path .up ();

			const Gtk::TreeIter parent = treeview -> get_model () -> get_iter (path);

			switch (treeview -> get_data_type (parent))
			{
				case OutlineIterType::X3DBaseNode:
				case OutlineIterType::Prototype:
				case OutlineIterType::ExternProto:
					break;
				default:
					path .clear ();
					break;
			}

			break;
		}
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::Prototype:
		case OutlineIterType::ExternProto:
			break;
		default:
			path .clear ();
			break;
	}

	return path;
}


Gtk::TreePath
OutlineEditor::getPathAtPosition (const double x, const double y)
{
	Gtk::TreePath        path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	treeview -> get_path_at_pos (x, y, path, column, cell_x, cell_y);

	return path;
}

} // puck
} // titania
