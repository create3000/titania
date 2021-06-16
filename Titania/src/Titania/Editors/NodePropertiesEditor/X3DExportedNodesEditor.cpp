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

#include "X3DExportedNodesEditor.h"

#include <Titania/X3D/Execution/ExportedNode.h>

namespace titania {
namespace puck {

struct X3DExportedNodesEditor::Columns
{
	static constexpr int32_t EXPORTED_NAME = 0;
};

X3DExportedNodesEditor::X3DExportedNodesEditor () :
	X3DNodePropertiesEditorInterface (),
	                           scene (),
	                            node (),
	                    selectedPath (),
	                         editing (false)
{
	addChildObjects (scene, node);
}

void
X3DExportedNodesEditor::setNode (const X3D::SFNode & value)
{
	if (scene)
		scene -> exportedNodes_changed () .removeInterest (&X3DExportedNodesEditor::set_exportedNodes, this);

	node  = value;
	scene = X3D::X3DScenePtr (node ? node -> getExecutionContext () : nullptr);

	getExportedNodesBox () .set_visible (scene);

	if (scene)
	{
		scene -> exportedNodes_changed () .addInterest (&X3DExportedNodesEditor::set_exportedNodes, this);

		set_exportedNodes ();
	}
	else
		getExportedNodesListStore () -> clear ();
}

void
X3DExportedNodesEditor::on_exported_node_changed ()
{
	getRemoveExportedNodeButton () .set_sensitive (not getExportedNodesTreeView () .get_selection () -> get_selected_rows () .empty ());
}

void
X3DExportedNodesEditor::on_exported_node_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	editing = true;
	selectedPath = path;

	const auto iter = getExportedNodesListStore () -> get_iter (path);

	std::string exportedName;
	iter -> get_value (Columns::EXPORTED_NAME, exportedName);

	getExportedNameEntry () .set_text (exportedName);
	on_exported_name_changed ();

	getExportedNodeDialog () .set_title (_ ("Edit Exported Node"));
	getExportedNodeDialog () .present ();
}

void
X3DExportedNodesEditor::on_add_exported_node ()
{
	editing = false;

	getExportedNameEntry () .set_text (getNameEntry () .get_text ());
	on_exported_name_changed ();

	getExportedNodeDialog () .set_title (_ ("Add Exported Node"));
	getExportedNodeDialog () .present ();
}

void
X3DExportedNodesEditor::on_remove_exported_node ()
{
	const auto selected = getExportedNodesTreeView () .get_selection () -> get_selected ();

	// Remove exported node.

	std::string exportedName;
	selected -> get_value (Columns::EXPORTED_NAME, exportedName);

	// Erase from list store.

	getExportedNodesListStore () -> erase (selected);

	// Remove

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove Exported Node »%s«"), exportedName .c_str ()));

	scene -> exportedNodes_changed () .removeInterest (&X3DExportedNodesEditor::set_exportedNodes, this);
	scene -> exportedNodes_changed () .addInterest (&X3DExportedNodesEditor::connectExportedNodes, this);

	removeExportedNode (scene, exportedName, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DExportedNodesEditor::on_exported_name_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (getExportedNameEntry (), text, *position);
}

void
X3DExportedNodesEditor::on_exported_name_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (getExportedNameEntry (), start_pos, end_pos);
}

void
X3DExportedNodesEditor::on_exported_name_changed ()
{
	const std::string exportedName = getExportedNameEntry () .get_text ();

	getExportedNodeOkButton () .set_sensitive (not exportedName .empty () and validateExportedName (exportedName));
}

void
X3DExportedNodesEditor::on_exported_node_ok_clicked ()
{
	getExportedNodeDialog () .hide ();

	const std::string exportedName = getExportedNameEntry () .get_text ();
	const auto        undoStep     = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Update Exported Node »%s«"), exportedName .c_str ()));

	scene -> exportedNodes_changed () .removeInterest (&X3DExportedNodesEditor::set_exportedNodes, this);
	scene -> exportedNodes_changed () .addInterest (&X3DExportedNodesEditor::connectExportedNodes, this);

	if (editing)
	{
		// Update ListStore.

		const auto iter = getExportedNodesListStore () -> get_iter (selectedPath);

		std::string oldExportedName;
		iter -> get_value (Columns::EXPORTED_NAME, oldExportedName);
		iter -> set_value (Columns::EXPORTED_NAME, exportedName);

		// Remove old exported node.

		removeExportedNode (scene, oldExportedName, undoStep);
	}
	else
	{
		// Update ListStore.

		const auto iter = getExportedNodesListStore () -> append ();
		iter -> set_value (Columns::EXPORTED_NAME, exportedName);
	}

	// Update exported node.

	updateExportedNode (scene, exportedName, node, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DExportedNodesEditor::on_exported_node_cancel_clicked ()
{
	getExportedNodeDialog () .hide ();
}

bool
X3DExportedNodesEditor::validateExportedName (const std::string & exportedName)
{
	return not scene -> getExportedNodes () .count (exportedName);
}

void
X3DExportedNodesEditor::set_exportedNodes ()
{
	getExportedNodeDialog () .hide ();
	getExportedNodesListStore () -> clear ();

	for (const auto & exportedNode : scene -> getExportedNodes ())
	{
		try
		{
			if (exportedNode .second -> getLocalNode () == node)
			{
				const auto iter = getExportedNodesListStore () -> append ();
				iter -> set_value (Columns::EXPORTED_NAME, exportedNode .first);
			}
		}
		catch (...)
		{ }
	}

	getRemoveExportedNodeButton () .set_sensitive (false);
}

void
X3DExportedNodesEditor::connectExportedNodes (const X3D::SFTime & field)
{
	field .removeInterest (&X3DExportedNodesEditor::connectExportedNodes, this);
	field .addInterest (&X3DExportedNodesEditor::set_exportedNodes, this);
}

void
X3DExportedNodesEditor::updateExportedNode (const X3D::X3DScenePtr & scene, const std::string & exportedName, const X3D::SFNode & node, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const auto & exportedNode = scene -> getExportedNodes () .at (exportedName);

		undoStep -> addUndoFunction (&X3D::X3DScene::updateExportedNode,
		                             scene,
		                             exportedNode -> getExportedName (),
		                             exportedNode -> getLocalNode ());
	}
	catch (...)
	{
		undoStep -> addUndoFunction (&X3D::X3DScene::removeExportedNode,
		                             scene,
		                             exportedName);
	}

	undoStep -> addRedoFunction (&X3D::X3DScene::updateExportedNode,
	                             scene,
	                             exportedName,
	                             node);

	scene -> updateExportedNode (exportedName, node);
}

void
X3DExportedNodesEditor::removeExportedNode (const X3D::X3DScenePtr & scene, const std::string & exportedName, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const auto & exportedNode = scene -> getExportedNodes () .at (exportedName);

		undoStep -> addUndoFunction (&X3D::X3DScene::updateExportedNode,
		                             scene,
		                             exportedNode -> getExportedName (),
		                             exportedNode -> getLocalNode ());
	}
	catch (...)
	{ }

	undoStep -> addRedoFunction (&X3D::X3DScene::removeExportedNode,
	                             scene,
	                             exportedName);

	scene -> removeExportedNode (exportedName);
}

X3DExportedNodesEditor::~X3DExportedNodesEditor ()
{ }

} // puck
} // titania
