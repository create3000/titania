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
	              X3DPointEmitterEditor (),
	               X3DConeEmitterEditor (),
	           X3DPolylineEmitterEditor (),
	             X3DVolumeEmitterEditor (),
	          X3DExplosionEmitterEditor (),
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
	for (const auto & parent : parents)
		parent -> emitter () .removeInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);

	parents = getNodes <X3D::ParticleSystem> (selection, { X3D::X3DConstants::ParticleSystem }, false);

	for (const auto & parent : parents)
		parent -> emitter () .addInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);

	set_emitter ();
}

void
X3DParticleEmitterNodeEditor::on_emitter_unlink_clicked ()
{
	X3D::UndoStepPtr undoStep;

	unlinkClone (parents, "emitter", undoStep);
}

void
X3DParticleEmitterNodeEditor::on_emitter_type_changed ()
{
	try
	{
		getEmitterBox () .set_visible (getEmitterTypeButton () .get_active_row_number () > 0);

		if (changing)
			return;
	
		addUndoFunction <X3D::SFNode> (parents, "emitter", undoStep);

		const auto executionContext = X3D::MakePtr (getSelectionContext (parents, true));

		if (getEmitterTypeButton () .get_active_row_number () == -1)
		{
			__LOG__ << "get_active_row_number == 1" << std::endl;
		}
		else if (getEmitterTypeButton () .get_active_row_number () == 0)
		{
			for (const auto & parent : parents)
			{
				auto & field = parent -> emitter ();

				field .removeInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);
				field .addInterest (&X3DParticleEmitterNodeEditor::connectEmitter, this);

				X3D::X3DEditor::replaceNode (executionContext, parent, field, nullptr, undoStep);
			}
		}
		else
		{
		   X3D::X3DPtr <X3D::X3DParticleEmitterNode> newEmitterNode (executionContext -> createNode (getEmitterTypeButton () .get_active_text ()));
	
		   if (emitterNode and emitterNode -> getType () .back () == newEmitterNode -> getType () .back ())
			{
				newEmitterNode = emitterNode;
			}
			else
			{
				try
				{
					newEmitterNode -> getField <X3D::SFVec3f> ("position") = emitterNode -> getField <X3D::SFVec3f> ("position");
				}
				catch (const X3D::X3DError & error)
				{ }

				try
				{
					newEmitterNode -> getField <X3D::SFVec3f> ("direction") = emitterNode -> getField <X3D::SFVec3f> ("direction");
				}
				catch (const X3D::X3DError & error)
				{ }
			
				newEmitterNode -> speed ()       = emitterNode -> speed ();
				newEmitterNode -> variation ()   = emitterNode -> variation ();
				newEmitterNode -> mass ()        = emitterNode -> mass ();
				newEmitterNode -> surfaceArea () = emitterNode -> surfaceArea ();
			}

			emitterNode = std::move (newEmitterNode);

			for (const auto & parent : parents)
			{
				auto & field = parent -> emitter ();

				field .removeInterest (&X3DParticleEmitterNodeEditor::set_emitter, this);
				field .addInterest (&X3DParticleEmitterNodeEditor::connectEmitter, this);

				X3D::X3DEditor::replaceNode (executionContext, parent, field, emitterNode, undoStep);
			}
		}

		addRedoFunction <X3D::SFNode> (parents, "emitter", undoStep);
	
		getEmitterUnlinkButton () .set_sensitive (getEmitterTypeButton () .get_active_row_number () > 0 and emitterNode -> getCloneCount () > 1);

		// Widgets

		set_widgets ();
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
	undoStep .reset ();

	// Find Emitter in selection

	const auto    executionContext = X3D::MakePtr (getSelectionContext (parents, true));
	auto          tuple            = getSelection <X3D::X3DParticleEmitterNode> (parents, "emitter");
	const int32_t active           = std::get <1> (tuple);
	const bool    hasParent        = std::get <2> (tuple);
	const bool    hasField         = (active not_eq -2);

	emitterNode = std::move (std::get <0> (tuple));

	changing = true;

	if (active == SAME_NODE)
		getEmitterTypeButton () .set_active_text (emitterNode -> getTypeName ());
	else
		getEmitterTypeButton () .set_active (-1);

	getEmitterExpander ()     .set_visible (hasParent or emitterNode);
	getCreateEmitterBox ()    .set_sensitive (hasParent);
	getEmitterTypeButton ()   .set_sensitive (hasField);
	getEmitterUnlinkButton () .set_sensitive (active > 0 and emitterNode -> getCloneCount () > 1);
	getEmitterBox ()          .set_visible (active > 0);

	changing = false;

	// Widgets

	if (not emitterNode)
		emitterNode = executionContext -> createNode <X3D::PointEmitter> ();

	set_widgets ();
}

void
X3DParticleEmitterNodeEditor::set_widgets ()
{
	const X3D::MFNode emitterNodes ({ emitterNode });

	X3DPointEmitterEditor::set_widgets (emitterNodes);
	X3DConeEmitterEditor::set_widgets (emitterNodes);
	X3DPolylineEmitterEditor::set_widgets (emitterNodes);
	X3DVolumeEmitterEditor::set_widgets (emitterNodes);
	X3DExplosionEmitterEditor::set_widgets (emitterNodes);

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
