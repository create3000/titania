/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "MagicImport.h"

#include "../Browser/BrowserWindow.h"

namespace titania {
namespace puck {

using namespace std::placeholders;

MagicImport::MagicImport (BrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	 importFunctions ({ std::make_pair ("Material", std::bind (std::mem_fn (&MagicImport::material), this, _1, _2)) })
{ }

bool
MagicImport::import (const X3D::X3DSFNode <X3D::Scene> & scene, const UndoStepPtr & undoStep)
{
	try
	{
		std::string magic          = scene -> getMetaData ("titania magic");
		auto        importFunction = importFunctions .find (magic);

		if (importFunction == importFunctions .end ())
			return false;

		return importFunction -> second (scene, undoStep);
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		return false;
	}
}

bool
MagicImport::material (const X3D::X3DSFNode <X3D::Scene> & scene, const UndoStepPtr & undoStep)
{
	auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return false;

	// Find first material node in scene

	X3D::SFNode material;

	X3D::traverse (scene -> getRootNodes (), [this, &material] (X3D::SFNode & node)
	               {
	                  X3D::X3DSFNode <X3D::X3DMaterialNode> m (node);

	                  if (m)
	                  {
	                     material = m -> copy (getBrowser () -> getExecutionContext ());
	                     material -> setup ();
	                     return false;
							}

	                  return true;
						});

	// Assign material to all appearances in selection

	X3D::traverse (selection, [this, &material, &undoStep] (X3D::SFNode & node)
	               {
	                  auto appearance = dynamic_cast <X3D::Appearance*> (node .getValue ());

	                  if (appearance)
	                  {
	                     auto oldMaterial = appearance -> material ();

	                     if (oldMaterial)
	                     {
	                        if (oldMaterial -> getNumClones () == 1)
										getBrowserWindow () -> removeNodes ({ oldMaterial }, undoStep);
								}

	                     undoStep -> addVariables (node);
	                     undoStep -> addUndoFunction (std::mem_fn (&X3D::SFNode::setValue), std::ref (appearance -> material ()), oldMaterial);
	                     undoStep -> addRedoFunction (std::mem_fn (&X3D::SFNode::setValue), std::ref (appearance -> material ()), material);

	                     appearance -> material () = material;
							}

	                  return true;
						});

	getBrowserWindow () -> select (selection, undoStep);

	return true;
}

MagicImport::~MagicImport ()
{ }

} // puck
} // titania
