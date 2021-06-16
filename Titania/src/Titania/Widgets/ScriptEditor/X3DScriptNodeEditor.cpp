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

#include "X3DScriptNodeEditor.h"

#include "../../Browser/BrowserSelection.h"

namespace titania {
namespace puck {

X3DScriptNodeEditor::X3DScriptNodeEditor () :
	X3DScriptEditor (),
	   directOutput (this, getDirectOutputToggleButton (), "directOutput"),
	   mustEvaluate (this, getMustEvaluateToggleButton (), "mustEvaluate"),
	     scriptNode ()
{
	addChildObjects (scriptNode);
}

void
X3DScriptNodeEditor::initialize ()
{ }

void
X3DScriptNodeEditor::set_node (const X3D::SFNode & value)
{
	scriptNode = value;

	getDirectOutputToggleButton () .set_visible (scriptNode);
	getMustEvaluateToggleButton () .set_visible (scriptNode);

	const auto nodes = scriptNode ? X3D::MFNode ({ scriptNode }) : X3D::MFNode ();

	directOutput .setNodes (nodes);
	mustEvaluate .setNodes (nodes);
}

void
X3DScriptNodeEditor::on_new_script_clicked ()
{
	static constexpr auto URL = "ecmascript:\n\nfunction initialize ()\n{\n\t\n}\n\nfunction set_field (value, time)\n{\n\t\n}\n\nfunction eventsProcessed ()\n{\n\t\n}\n";

	getNewScriptPopover () .popdown ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New Script"));
	const auto node     = X3D::X3DEditor::createNode (getCurrentWorld (), getCurrentContext (), "Script", undoStep);

	node -> setField <X3D::MFString> ("url", X3D::MFString ({ URL }));

	X3D::X3DEditor::updateNamedNode (getCurrentContext (), getCurrentContext () -> getUniqueName ("NewScript"), node, undoStep);

	getBrowserWindow () -> getSelection () -> setNodes ({ node }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	set_node (node);
}

} // puck
} // titania
