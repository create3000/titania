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

#include "FileExportDialog.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"

#include <Titania/X3D/Basic/Traverse.h>
#include <Titania/X3D/Editing/X3DEditor.h>

#include <Titania/OS.h>

#include <regex>

namespace titania {
namespace puck {

FileExportDialog::FileExportDialog (X3DBrowserWindow* const browserWindow) :
	 X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DFileSaveDialog ()
{
	setName ("FileExportDialog");

	getWindow () .set_title (_ ("Export Nodes …"));

	setup ();
}

// Export nodes

std::pair <basic::uri, bool>
FileExportDialog::exportNodes (const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	const bool success = run (getCurrentScene () -> getWorldURL () .parent () + _ ("inline.x3d"));

	if (not success)
		return std::make_pair ("", false);

	const auto worldURL = getUrl ();

	if (not exportNodes (nodes, worldURL, getOutputStyleButton () .get_active_text (), undoStep))
		return std::make_pair (worldURL, false);

	return std::make_pair (worldURL, true);
}

bool
FileExportDialog::exportNodes (const X3D::MFNode & nodes, const basic::uri & worldURL, const std::string & outputStyle, const X3D::UndoStepPtr & undoStep)
{
	using namespace std::placeholders;

	// Temporarily change url's in protos

	const auto protoUndoStep = std::make_shared <X3D::UndoStep> ("Traverse");

	X3D::traverse (getCurrentContext (),
	               std::bind (&X3D::X3DEditor::transform, getCurrentContext () -> getWorldURL (), worldURL, protoUndoStep, _1),
	               true,
	               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
	               X3D::TRAVERSE_PROTO_DECLARATIONS);

	// Change url's in nodes

	X3D::traverse (const_cast <X3D::MFNode &> (nodes),
	               std::bind (&X3D::X3DEditor::transform, getCurrentContext () -> getWorldURL (), worldURL, undoStep, _1),
	               true,
	               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
	               X3D::TRAVERSE_PROTO_DECLARATIONS |
	               X3D::TRAVERSE_ROOT_NODES);

	// Export nodes to stream

	std::ostringstream osstream;

	X3D::X3DEditor::exportNodes (osstream, getCurrentContext (), nodes, false);

	// Undo url change in protos

	protoUndoStep -> undo ();

	// Save scene

	basic::ifilestream stream (osstream .str ());

	const auto scene = getCurrentBrowser () -> createX3DFromStream (worldURL, stream);

	return getBrowserWindow () -> save (scene, worldURL, outputStyle, false);
}

FileExportDialog::~FileExportDialog ()
{
	dispose ();
}

} // puck
} // titania
