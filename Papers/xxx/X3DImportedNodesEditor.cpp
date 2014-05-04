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

#include "NodePropertiesEditor.h"


namespace titania {
namespace puck {

static constexpr int IMPORTED_COLUMN      = 0;
static constexpr int EXPORTED_NAME_COLUMN = 1;
static constexpr int IMPORTED_NAME_COLUMN = 2;

X3DImportedNodesEditor::X3DImportedNodesEditor () :
	X3DNodePropertiesEditorBase ()
{
	try
	{
		const auto inlineNode = X3D::x3d_cast <X3D::Inline*> (getNode ());

		if (inlineNode)
		{
			getImportedNodesImportedCellrendererToggle ()   -> property_activatable () = true;
			getImportedNodesImportedNameCellrendererText () -> property_editable ()    = true;

			std::map <X3D::SFNode, X3D::ImportedNodePtr> importedNodes;

			for (const auto & importedNode : getBrowser () -> getExecutionContext () -> getImportedNodes ())
				importedNodes .emplace (importedNode -> getExportedNode (), importedNode);

			for (const auto & exportedNode : inlineNode -> getExportedNodes ())
			{
				const auto row          = getImportedNodesListStore () -> append ();
				const auto importedNode = importedNodes .find (exportedNode -> getNode ());

				if (importedNode not_eq importedNodes .end ())
				{
					row -> set_value (IMPORTED_COLUMN, true);
					row -> set_value (IMPORTED_NAME_COLUMN, importedNode -> second -> getImportedName ());
				}
				else
					row -> set_value (IMPORTED_COLUMN, false);

				row -> set_value (EXPORTED_NAME_COLUMN, exportedNode -> getExportedName ());
			}
		}

		getImportedNodesExpander () .set_visible (inlineNode and not inlineNode -> getExportedNodes () .empty ());
	}
	catch (const X3D::X3DError &)
	{
		// If Inline is not loaded.
		getImportedNodesExpander () .set_visible (false);
	}
}

void
X3DImportedNodesEditor::on_imported_toggled (const Glib::ustring & path)
{
	__LOG__ << std::endl;
	
	const auto iter = getImportedNodesListStore () -> get_iter (path);

	bool imported = false;
	iter -> get_value (IMPORTED_COLUMN, imported);
	iter -> set_value (IMPORTED_COLUMN, not imported);
}

}  // puck

}    // titania
