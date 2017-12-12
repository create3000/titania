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

#include <Titania/X3D/Components/ParticleSystems/ParticleSystem.h>
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
	                            parents (),
	                      emitterBuffer (),
	                        emitterNode (),
	                           undoStep (),
	                           changing (false)
{
	addChildObjects (parents, emitterBuffer, emitterNode);

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

	const auto executionContext = X3D::MakePtr (getSelectionContext (parents, true));
	const auto newEmitterNode   = emitterNode ? emitterNode : executionContext -> createNode <X3D::PointEmitter> ();

	addUndoFunction <X3D::SFNode> (parents, "emitter", undoStep);

	for (const auto & parent : parents)
	{
		if (getEmitterCheckButton () .get_active ())
			X3D::X3DEditor::replaceNode (executionContext, parent, parent -> emitter (), newEmitterNode, undoStep);
		else
			X3D::X3DEditor::replaceNode (executionContext, parent, parent -> emitter (), nullptr, undoStep);
	}

	addRedoFunction <X3D::SFNode> (parents, "emitter", undoStep);
}

void
X3DParticleEmitterNodeEditor::on_emitter_type_changed ()
{
	try
	{
		if (changing)
			return;
	
		if (getEmitterTypeButton () .get_active_row_number () == -1)
		{ }
		else
		{
			const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Change Field »geometry«"));
			const auto executionContext = X3D::MakePtr (getSelectionContext (parents, true));
		   auto       node             = executionContext -> createNode (getEmitterTypeButton () .get_active_text ());
	
		   if (emitterNode and emitterNode -> getType () .back () == node -> getType () .back ())
				node = emitterNode;
	
			for (const auto & parent : parents)
				X3D::X3DEditor::replaceNode (executionContext, parent, parent -> emitter (), node, undoStep);
	
			getBrowserWindow () -> addUndoStep (undoStep);
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DParticleEmitterNodeEditor::set_emitter ()
{
	emitterBuffer .addEvent ();
}

void
X3DParticleEmitterNodeEditor::set_node ()
{
	changing = true;

	undoStep .reset ();

	// Remove selection

	for (const auto & parent : parents)
		parent -> emitter () .removeInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);

	parents .clear ();

	// Find Emitter in selection

	const auto & selection    = getBrowserWindow () -> getSelection () -> getNodes ();
	const auto   pair         = getNode <X3D::X3DParticleEmitterNode> (selection, "emitter");
	const bool   inconsistent = pair .second == -1;

	emitterNode = selection .empty () ? nullptr : selection .back ();

	if (pair .second == -2)
	{
		if (emitterNode)
			getEmitterExpander () .set_label (_ (refineName (emitterNode -> getTypeName ())));
	}
	else
	{
		parents     = getSelection <X3D::ParticleSystem> ({ X3D::X3DConstants::ParticleSystem });
		emitterNode = pair .first;

		getEmitterExpander () .set_label (_ (refineName ("Particle Emitter")));
	}

	for (const auto & parent : parents)
		parent -> emitter () .addInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);

	if (inconsistent)
		getEmitterTypeButton () .set_active (-1);
	else
		getEmitterTypeButton () .set_active_text (emitterNode -> getTypeName ());

	getEmitterExpander ()    .set_visible (emitterNode or not parents .empty ());
	getCreateEmitterBox ()   .set_visible (not parents .empty ());
	getEmitterCheckButton () .set_active (emitterNode and not inconsistent);
	getEmitterCheckButton () .set_inconsistent (inconsistent);
	getEmitterTypeButton ()  .set_sensitive (emitterNode or inconsistent);
	getEmitterBox ()         .set_sensitive (emitterNode and not inconsistent);

	// Widgets

	const auto emitterNodes = emitterNode ? X3D::MFNode ({ emitterNode }) : X3D::MFNode ();

	speed       .setNodes (emitterNodes);
	variation   .setNodes (emitterNodes);
	mass        .setNodes (emitterNodes);
	surfaceArea .setNodes (emitterNodes);

	changing = false;
}

X3DParticleEmitterNodeEditor::~X3DParticleEmitterNodeEditor ()
{ }

} // puck
} // titania
