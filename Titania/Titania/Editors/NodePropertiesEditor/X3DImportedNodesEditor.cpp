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

#include "X3DImportedNodesEditor.h"

#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Execution/ExportedNode.h>
#include <Titania/X3D/Parser/Filter.h>

namespace titania {
namespace puck {

enum ImportedNodesEditorColumns
{
	IMPORTED,
	TYPE_NAME,
	EXPORTED_NAME,
	IMPORTED_NAME,
	NOT_VALID

};

X3DImportedNodesEditor::X3DImportedNodesEditor () :
	X3DNodePropertiesEditorInterface (),
	                      inlineNode ()
{
	addChildObjects (inlineNode);
}

void
X3DImportedNodesEditor::setNode (const X3D::SFNode & value)
{
	if (inlineNode)
	{
		inlineNode -> getExecutionContext () -> importedNodes_changed () .removeInterest (&X3DImportedNodesEditor::set_importedNodes, this);
		inlineNode -> checkLoadState () .removeInterest (&X3DImportedNodesEditor::set_importedNodes, this);
	}

	inlineNode = value;

	if (inlineNode)
	{
		inlineNode -> getExecutionContext () -> importedNodes_changed () .addInterest (&X3DImportedNodesEditor::set_importedNodes, this);
		inlineNode -> checkLoadState () .addInterest (&X3DImportedNodesEditor::set_importedNodes, this);

		set_importedNodes ();
	}
	else
	{
		getImportedNodesBox () .set_visible (false);

		getImportedNodesListStore () -> clear ();
	}
}

void
X3DImportedNodesEditor::on_imported_toggled (const Glib::ustring & path)
{
	bool        imported = false;
	std::string exportedName;
	std::string importedName;

	const auto iter = getImportedNodesListStore () -> get_iter (path);

	iter -> get_value (IMPORTED,      imported);
	iter -> get_value (EXPORTED_NAME, exportedName);
	iter -> get_value (IMPORTED_NAME, importedName);

	if (importedName .empty ())
		importedName = exportedName;

	imported = not imported;

	if (not imported)
	{
		// Remove imported node.

		const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove Imported Node »%s«"), importedName .c_str ()));

		inlineNode -> getExecutionContext () -> importedNodes_changed () .removeInterest (&X3DImportedNodesEditor::set_importedNodes, this);
		inlineNode -> getExecutionContext () -> importedNodes_changed () .addInterest (&X3DImportedNodesEditor::connectImportedNodes, this);

		X3D::X3DEditor::removeImportedNode (X3D::X3DExecutionContextPtr (inlineNode -> getExecutionContext ()), importedName, undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);

		// Update ListStore.

		if (inlineNode -> checkLoadState () == X3D::COMPLETE_STATE)
			iter -> set_value (IMPORTED, false);
		else
			getImportedNodesListStore () -> erase (iter);
	}

	const bool valid = validateImportedName (exportedName, importedName);
	iter -> set_value (NOT_VALID, not valid);

	if (valid and imported)
	{
		// Update imported node.
	
		const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Update Imported Node »%s«"), importedName .c_str ()));

		inlineNode -> getExecutionContext () -> importedNodes_changed () .removeInterest (&X3DImportedNodesEditor::set_importedNodes, this);
		inlineNode -> getExecutionContext () -> importedNodes_changed () .addInterest (&X3DImportedNodesEditor::connectImportedNodes, this);

		X3D::X3DEditor::updateImportedNode (X3D::X3DExecutionContextPtr (inlineNode -> getExecutionContext ()), inlineNode, exportedName, importedName, undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);

		// Update ListStore.

		iter -> set_value (IMPORTED, true);
	}

	// Validate all children which are not imported.

	validateImportedNames (iter);
}

void
X3DImportedNodesEditor::on_imported_name_edited (const Glib::ustring & path, const Glib::ustring & new_text)
{
	std::string value = new_text;

	X3D::FilterNonIdCharacters (value);

	std::string exportedName;
	std::string importedName;

	const auto iter = getImportedNodesListStore () -> get_iter (path);

	iter -> get_value (EXPORTED_NAME, exportedName);
	iter -> get_value (IMPORTED_NAME, importedName);

	if (importedName .empty ())
		importedName = exportedName;

	const std::string newImportedName = value .empty () ? exportedName : value;
	const bool        valid           = validateImportedName (exportedName, newImportedName);

	if (not valid)
		return;

	if (newImportedName == importedName)
		return;

	// Update or remove imported node.

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Update Imported Node »%s«"), importedName .c_str ()));

	inlineNode -> getExecutionContext () -> importedNodes_changed () .removeInterest (&X3DImportedNodesEditor::set_importedNodes, this);
	inlineNode -> getExecutionContext () -> importedNodes_changed () .addInterest (&X3DImportedNodesEditor::connectImportedNodes, this);

	X3D::X3DEditor::updateImportedNode (X3D::X3DExecutionContextPtr (inlineNode -> getExecutionContext ()), inlineNode, exportedName, newImportedName, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);

	// Update ListStore.

	iter -> set_value (IMPORTED,      valid);
	iter -> set_value (IMPORTED_NAME, newImportedName == exportedName ? "" : newImportedName);
	iter -> set_value (NOT_VALID,     not valid);

	// Validate all children which are not imported.

	validateImportedNames (iter);
}

void
X3DImportedNodesEditor::validateImportedNames (const Gtk::TreeIter & iter) const
{
	bool        imported = false;
	std::string exportedName;
	std::string importedName;

	// Validate all children which are not imported.

	for (const auto & child : getImportedNodesListStore () -> children ())
	{
		if (child == iter)
			continue;

		child -> get_value (IMPORTED,      imported);
		child -> get_value (EXPORTED_NAME, exportedName);
		child -> get_value (IMPORTED_NAME, importedName);

		if (importedName .empty ())
			importedName = exportedName;

		if (not imported)
			child -> set_value (NOT_VALID, not validateImportedName (exportedName, importedName));
	}
}

bool
X3DImportedNodesEditor::validateImportedName (const std::string & exportedName, const std::string & importedName) const
{
	try
	{
		if (inlineNode -> getExecutionContext () -> getNamedNodes () .count (importedName))
			return false; // There is already a named node.
	}
	catch (const X3D::X3DError &)
	{ }

	try
	{
		const auto iter = inlineNode -> getExecutionContext () -> getImportedNodes () .find (importedName);

		if (iter not_eq inlineNode -> getExecutionContext () -> getImportedNodes () .end ())
		{
			const auto & importedNode = iter -> second;

			if (importedNode -> getInlineNode () not_eq inlineNode)
				return false;     // There is an import from another Inline node with importedName.

			if (importedNode -> getExportedName () not_eq exportedName)
				return false;  // There is another import from this Inline node with importedName.
		}
	}
	catch (const X3D::X3DError &)
	{ }

	return true;
}

void
X3DImportedNodesEditor::set_importedNodes ()
{
	getImportedNodesBox () .set_visible (true);

	getImportedNodesListStore () -> clear ();

	// Get imported nodes where inline == inline from execution context

	std::map <std::string, X3D::ImportedNodePtr> importedNodes;

	for (const auto & importedNode : inlineNode -> getExecutionContext () -> getImportedNodes ())
	{
		try
		{
			if (importedNode .second -> getInlineNode () == inlineNode)
				importedNodes [importedNode .second -> getExportedName ()] = importedNode .second;
		}
		catch (...)
		{ }
	}

	if (inlineNode -> checkLoadState () == X3D::COMPLETE_STATE)
	{
		for (const auto & pair : inlineNode -> getExportedNodes ())
		{
			try
			{
				const auto & exportedNode = pair .second;
				const auto   localNode    = exportedNode -> getLocalNode ();
				const auto   importedNode = importedNodes .find (exportedNode -> getExportedName ());
				const auto   iter         = getImportedNodesListStore () -> append ();

				iter -> set_value (TYPE_NAME,     localNode -> getTypeName ());
				iter -> set_value (EXPORTED_NAME, exportedNode -> getExportedName ());

				if (importedNode not_eq importedNodes .end ())
				{
					const std::string & exportedName = importedNode -> second -> getExportedName ();
					const std::string & importedName = importedNode -> second -> getImportedName ();

					iter -> set_value (IMPORTED,      true);
					iter -> set_value (IMPORTED_NAME, importedName == exportedName ? "" : importedName);
					iter -> set_value (NOT_VALID,     not validateImportedName (exportedName, importedName));
				}
				else
				{
					const std::string & exportedName = exportedNode -> getExportedName ();

					iter -> set_value (IMPORTED,  false);
					iter -> set_value (NOT_VALID, not validateImportedName (exportedName, exportedName));
				}
			}
			catch (...)
			{ }
		}
	}
	else
	{
		for (const auto & pair : importedNodes)
		{
			try
			{
				const auto          iter         = getImportedNodesListStore () -> append ();
				const auto &        importedNode = pair .second;
				const std::string & exportedName = importedNode -> getExportedName ();
				const std::string & importedName = importedNode -> getImportedName ();

				iter -> set_value (TYPE_NAME,     std::string ("ImportedNode"));
				iter -> set_value (EXPORTED_NAME, importedNode -> getExportedName ());
				iter -> set_value (IMPORTED,      true);
				iter -> set_value (IMPORTED_NAME, importedName == exportedName ? "" : importedName);
				iter -> set_value (NOT_VALID,     not validateImportedName (exportedName, importedName));
			}
			catch (...)
			{ }
		}
	}
}

void
X3DImportedNodesEditor::connectImportedNodes (const X3D::SFTime & field)
{
	field .removeInterest (&X3DImportedNodesEditor::connectImportedNodes, this);
	field .addInterest (&X3DImportedNodesEditor::set_importedNodes, this);
}

X3DImportedNodesEditor::~X3DImportedNodesEditor ()
{ }

} // puck
} // titania
