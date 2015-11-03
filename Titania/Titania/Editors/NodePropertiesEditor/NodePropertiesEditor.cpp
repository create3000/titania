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

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

NodePropertiesEditor::NodePropertiesEditor (X3DBrowserWindow* const browserWindow) :
	                X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DNodePropertiesEditorInterface (get_ui ("Editors/NodePropertiesEditor.glade"), gconf_dir ()),
	      X3DUserDefinedFieldsEditor (),
	          X3DImportedNodesEditor (),
	          X3DExportedNodesEditor (),
	                            node (),
	                        nodeName (this, getNameEntry (), getRenameButton ())

{
	setup ();
}

void
NodePropertiesEditor::initialize ()
{
	X3DNodePropertiesEditorInterface::initialize ();
	X3DUserDefinedFieldsEditor::initialize ();
	X3DImportedNodesEditor::initialize ();
	X3DExportedNodesEditor::initialize ();

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &NodePropertiesEditor::set_selection);

	set_selection (getBrowserWindow () -> getSelection () -> getChildren ());

	getNodeChildNotebook () .set_current_page (getConfig () .getInteger ("currentPage"));
}

void
NodePropertiesEditor::set_selection (const X3D::MFNode & selection)
{
	if (node)
		node -> name_changed () .removeInterest (this, &NodePropertiesEditor::set_name);

	node = selection .empty () ? nullptr : selection .back ();

	if (node)
	{
		node -> name_changed () .addInterest (this, &NodePropertiesEditor::set_name);

		set_name ();
		getTypeNameEntry ()       .set_text (node -> getTypeName ());
		getComponentEntry ()      .set_text (node -> getBrowser () -> getSupportedComponents () .rfind (node -> getComponent ()) -> getName ());
		getContainerFieldEntry () .set_text (node -> getContainerField ());

		getCDATAFieldExpander () .set_visible (node -> getCDATA ());
	}
	else
	{
		getHeaderLabel ()         .set_text (_ ("Select a node to display its properties."));
		getTypeNameEntry ()       .set_text ("");
		getComponentEntry ()      .set_text ("");
		getContainerFieldEntry () .set_text ("");

		getCDATAFieldExpander () .set_visible (false);
	}

	nodeName .setNode (node);

	X3DUserDefinedFieldsEditor::setNode (node);
	X3DImportedNodesEditor::setNode (node);
	X3DExportedNodesEditor::setNode (node);
}

void
NodePropertiesEditor::set_name ()
{
	getHeaderLabel () .set_text (node -> getTypeName () + " »" + node -> getName () + "«");
}

void
NodePropertiesEditor::on_edit_cdata_clicked ()
{
	getBrowserWindow () -> editCDATA (node);
}

NodePropertiesEditor::~NodePropertiesEditor ()
{
	getConfig () .setItem ("currentPage", getNodeChildNotebook () .get_current_page ());

	dispose ();
}

}  // puck

}    // titania
