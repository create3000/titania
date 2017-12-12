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

#include "X3DParticleEmitterNodeEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../ComposedWidgets/RotationTool.h"

#include <Titania/X3D/Components/ParticleSystems/PointEmitter.h>
#include <Titania/X3D/Components/ParticleSystems/X3DParticleEmitterNode.h>

namespace titania {
namespace puck {

X3DParticleEmitterNodeEditor::X3DParticleEmitterNodeEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                              speed (this,
	                                     getEmitterSpeedAdjustment (),
	                                     getEmitterSpeedSpinButton (),
	                                     "speed"),
	                          variation (this,
	                                     getEmitterVariationAdjustment (),
	                                     getEmitterVariationSpinButton (),
	                                     "variation"),
	                               mass (this,
	                                     getEmitterMassAdjustment (),
	                                     getEmitterMassSpinButton (),
	                                     "mass"),
	                        surfaceArea (this,
	                                     getEmitterSurfaceAreaAdjustment (),
	                                     getEmitterSurfaceAreaSpinButton (),
	                                     "surfaceArea"),
	                              nodes (),
	                      emitterBuffer (),
	                        emitterNode (),
	                           undoStep (),
	                           changing (false)
{
	addChildObjects (nodes, emitterBuffer, emitterNode);

	emitterBuffer .addInterest (&X3DParticleEmitterNodeEditor::set_node, this);
}

void
X3DParticleEmitterNodeEditor::initialize ()
{ }

void
X3DParticleEmitterNodeEditor::set_selection (const X3D::MFNode & selection)
{
	set_emitter ();
}

void
X3DParticleEmitterNodeEditor::on_emitter_toggled ()
{
	if (changing)
		return;

	getEmitterCheckButton () .set_inconsistent (false);
	getEmitterBox ()         .set_sensitive (getEmitterCheckButton () .get_active ());

	// Set field.

	const auto executionContext = X3D::MakePtr (getSelectionContext (nodes, true));

	addUndoFunction <X3D::SFNode> (nodes, "emitter", undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFNode> ("emitter");

			field .removeInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);
			field .addInterest (&X3DParticleEmitterNodeEditor::connectEmitter, this);

			if (getEmitterCheckButton () .get_active ())
				X3D::X3DEditor::replaceNode (executionContext, node, field, emitterNode, undoStep);
			else
				X3D::X3DEditor::replaceNode (executionContext, node, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (nodes, "emitter", undoStep);
}

void
X3DParticleEmitterNodeEditor::set_emitter ()
{
	emitterBuffer .addEvent ();
}

void
X3DParticleEmitterNodeEditor::set_node ()
{
	undoStep .reset ();

	// Remove selection

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFNode> ("emitter") .removeInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes .clear ();

	// Find Emitter in selection

	const auto & selection        = getBrowserWindow () -> getSelection () -> getNodes ();
	const auto   executionContext = X3D::MakePtr (getSelectionContext (selection, true));

	emitterNode = selection .empty () ? nullptr : selection .back ();

	try
	{
		if (not selection .empty ())
		{
			auto & field = selection .back () -> getField <X3D::SFNode> ("emitter");

			field .addInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);

			nodes       = { selection .back () };
			emitterNode = field;

			changing = true;

			getCreateEmitterBox ()   .set_visible (true);
			getEmitterCheckButton () .set_active (emitterNode);
			getEmitterBox ()         .set_sensitive (emitterNode);

			changing = false;
		
			if (not emitterNode)
				emitterNode = selection .back () -> getExecutionContext () -> createNode <X3D::PointEmitter> ();
		}
	}
	catch (const X3D::X3DError &)
	{ }

	getEmitterExpander ()  .set_visible (emitterNode);
	getCreateEmitterBox () .set_visible (not nodes .empty ());

	// Widgets

	const auto emitterNodes = emitterNode ? X3D::MFNode ({ emitterNode }) : X3D::MFNode ();

	speed       .setNodes (emitterNodes);
	variation   .setNodes (emitterNodes);
	mass        .setNodes (emitterNodes);
	surfaceArea .setNodes (emitterNodes);
}

void
X3DParticleEmitterNodeEditor::connectEmitter (const X3D::SFNode & field)
{
	field .removeInterest (&X3DParticleEmitterNodeEditor::connectEmitter, this);
	field .addInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);
}

X3DParticleEmitterNodeEditor::~X3DParticleEmitterNodeEditor ()
{ }

} // puck
} // titania
