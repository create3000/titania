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

#include "FileExportProtoDialog.h"

#include "../../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>

namespace titania {
namespace puck {

FileExportProtoDialog::FileExportProtoDialog (X3DBrowserWindow* const browserWindow) :
	 X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DFileSaveDialog ()
{
	setName ("FileExportProtoDialog");

	getWindow () .set_title (_ ("Export Prototype …"));

	setup ();
}

bool
FileExportProtoDialog::run (const X3D::ProtoDeclarationPtr & prototype, const X3D::UndoStepPtr & undoStep)
{
	const bool success = X3DFileSaveDialog::run (getCurrentScene () -> getWorldURL () .parent () + prototype -> getName () + ".x3d");

	if (not success)
		return false;

	if (not exportNodes ({ prototype }, getUrl (), getOutputStyleButton () .get_active_text (), undoStep))
		return false;

	const auto worldURL = getUrl () + "#" + prototype -> getName ();

	X3D::MFString url;

	url .emplace_back (prototype -> getExecutionContext () -> getWorldURL () .relative_path (worldURL));
	url .emplace_back (worldURL);

	X3D::X3DEditor::convertProtoToExternProto (prototype, url, undoStep);

	return true;
}

FileExportProtoDialog::~FileExportProtoDialog ()
{
	getConfig () -> removeItem ("currentFolder");

	dispose ();
}

} // puck
} // titania
