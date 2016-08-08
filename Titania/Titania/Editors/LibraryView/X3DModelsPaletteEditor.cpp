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

#include "X3DModelsPaletteEditor.h"

#include "../../Browser/MagicImport.h"

#include <Titania/X3D/Components/Geometry3D/Box.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Components/Networking/LoadSensor.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Material.h>
#include <Titania/X3D/Components/Shape/Shape.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>

namespace titania {
namespace puck {

X3DModelsPaletteEditor::X3DModelsPaletteEditor () :
	X3DPaletteEditor <X3DLibraryViewInterface> ("Models")
{ }

void
X3DModelsPaletteEditor::addObject (const std::string & URL)
{
	try
	{
		const auto undoStep   = std::make_shared <X3D::UndoStep> (_ ("Import"));
		const auto inlineNode = getPreview () -> getExecutionContext () -> createNode <X3D::Inline> ();
		const auto loadSensor = getPreview () -> getExecutionContext () -> createNode <X3D::LoadSensor> ();
		const auto transform  = getPreview () -> getExecutionContext () -> createNode <X3D::Transform> ();
		const auto group      = getPreview () -> getExecutionContext () -> createNode <X3D::Transform> ();
		const auto shape      = getPreview () -> getExecutionContext () -> createNode <X3D::Shape> ();
		const auto appearance = getPreview () -> getExecutionContext () -> createNode <X3D::Appearance> ();
		const auto material   = getPreview () -> getExecutionContext () -> createNode <X3D::Material> ();
		const auto box        = getPreview () -> getExecutionContext () -> createNode <X3D::Box> ();
	
		inlineNode -> checkLoadState () .addInterest (this, &X3DModelsPaletteEditor::set_loadState, inlineNode .getValue (), loadSensor .getValue (), group .getValue ());

		inlineNode -> url () = { URL };
		group -> children () = { inlineNode, loadSensor };

		material -> transparency () = 0.9;
		appearance -> material ()   = material;
		shape -> appearance ()      = appearance;
		shape -> geometry ()        = box;
		transform -> children ()    = { group, shape };
	
		X3DPaletteEditor <X3DLibraryViewInterface>::addObject (URL, transform);
	}
	catch (...)
	{ }
}

void
X3DModelsPaletteEditor::set_loadState (const X3D::X3DPtr <X3D::Inline> & inlineNode,
                                       const X3D::X3DPtr <X3D::LoadSensor> & loadSensor,
                                       const X3D::X3DPtr <X3D::Transform> & transform)
{
	switch (inlineNode -> checkLoadState ())
	{
		case X3D::COMPLETE_STATE:
		{	
			// Collect Inlines.

			X3D::MFNode urlObjects;

			X3D::traverse (inlineNode -> getInternalScene (), [&urlObjects] (X3D::SFNode & node)
			               {
			                  if (node -> getType () .back () == X3D::X3DConstants::Inline)
										urlObjects .emplace_back (node);

			                  return true;
								},
	                     true,
	                     X3D::TRAVERSE_ROOT_NODES |
	                     X3D::TRAVERSE_PROTO_INSTANCES);

			// Wait for bbox calculation until all Inlines are loaded

			if (urlObjects .empty ())
				set_loadTime (inlineNode, loadSensor, transform);

			else
			{
				loadSensor -> loadTime () .addInterest (this, &X3DModelsPaletteEditor::set_loadTime, inlineNode .getValue (), loadSensor .getValue (), transform .getValue ());

				loadSensor -> watchList () = std::move (urlObjects);
			}

			// Proceed with next case:
		}
		case X3D::FAILED_STATE:
			inlineNode -> checkLoadState () .removeInterest (this, &X3DModelsPaletteEditor::set_loadState);
			break;
		default:
			break;
	}
}

void
X3DModelsPaletteEditor::set_loadTime (const X3D::X3DPtr <X3D::Inline> & inlineNode,
                                      const X3D::X3DPtr <X3D::LoadSensor> & loadSensor,
                                      const X3D::X3DPtr <X3D::Transform> & transform)
{
	loadSensor -> loadTime () .removeInterest (this, &X3DModelsPaletteEditor::set_loadTime);

	// Center and scale Inline depending on bbox in palette.

	const auto bbox   = inlineNode -> getBBox ();
	const auto size   = bbox .size ();
	const auto center = bbox .center ();
	const auto scale  = X3D::Vector3d (1.8, 1.8, 1.8) / std::max ({ size .x (), size .y (), size .z () });

	transform -> translation () = -center * scale;
	transform -> scale ()       = scale;
}

void
X3DModelsPaletteEditor::setTouchTime (const std::string & URL)
{
	try
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Import Model From Library"));
		const auto nodes    = getBrowserWindow () -> import ({ URL }, undoStep);

		getBrowserWindow () -> getSelection () -> setChildren (nodes, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

bool
X3DModelsPaletteEditor::createScene (const X3D::X3DScenePtr & scene)
{
	using namespace std::placeholders;

	auto selection = getBrowserWindow () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return false;

	// Temporarily change url's in protos

	const auto undoStep = std::make_shared <X3D::UndoStep> ("Traverse");

	X3D::traverse (getCurrentContext (),
	               std::bind (&X3DBrowserWidget::transform, getCurrentContext () -> getWorldURL (), scene -> getWorldURL (), undoStep, _1),
	               true,
	               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
	               X3D::TRAVERSE_PROTO_DECLARATIONS);

	// Change url's in nodes

	X3D::traverse (selection,
	               std::bind (&X3DBrowserWidget::transform, getCurrentContext () -> getWorldURL (), scene -> getWorldURL (), undoStep, _1),
	               true,
	               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
	               X3D::TRAVERSE_PROTO_DECLARATIONS |
	               X3D::TRAVERSE_ROOT_NODES);

	// Export nodes to stream

	std::stringstream sstream;

	getBrowserWindow () -> exportNodes (getCurrentContext (), sstream, selection, false);

	// Undo url change in protos and selection

	undoStep -> undo ();

	// Parse exported nodes into scene.

	scene -> fromStream (sstream);

	return true;
}

X3DModelsPaletteEditor::~X3DModelsPaletteEditor ()
{ }

} // puck
} // titania
