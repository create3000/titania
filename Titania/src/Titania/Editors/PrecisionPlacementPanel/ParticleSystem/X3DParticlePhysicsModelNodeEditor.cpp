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

#include "X3DParticlePhysicsModelNodeEditor.h"

#include "DefaultPhysicsModelEditor.h"
#include "ForcePhysicsModelEditor.h"
#include "WindPhysicsModelEditor.h"

#include <Titania/X3D/Components/ParticleSystems/ForcePhysicsModel.h>
#include <Titania/X3D/Components/ParticleSystems/ParticleSystem.h>
#include <Titania/X3D/Components/ParticleSystems/WindPhysicsModel.h>

namespace titania {
namespace puck {

X3DParticlePhysicsModelNodeEditor::X3DParticlePhysicsModelNodeEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                            parents (),
	                       phyicsBuffer (),
	                            editors ()
{
	addChildObjects (parents, phyicsBuffer);

	phyicsBuffer .addInterest (&X3DParticlePhysicsModelNodeEditor::set_nodes, this);
}

void
X3DParticlePhysicsModelNodeEditor::initialize ()
{ }

void
X3DParticlePhysicsModelNodeEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::MFNode> ("physics") .removeInterest (&X3DParticlePhysicsModelNodeEditor::set_physics, this);
		}
		catch (const X3D::X3DError & error)
		{ }		
	}

	parents = getNodes <X3D::X3DBaseNode> (selection, { X3D::X3DConstants::ParticleSystem }, false);

	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::MFNode> ("physics") .addInterest (&X3DParticlePhysicsModelNodeEditor::set_physics, this);
		}
		catch (const X3D::X3DError & error)
		{ }		
	}

	set_physics ();
}

void
X3DParticlePhysicsModelNodeEditor::on_physics_link_clicked ()
{
	if (parents .empty ())
		return;

	const auto   undoStep = std::make_shared <X3D::UndoStep> (_ ("Create Clones In »physics«"));
	const auto & nodes    = parents .back () -> getField <X3D::MFNode> ("physics");

	for (const auto & parent : parents)
	{
		try
		{
			X3D::X3DEditor::replaceNodes (getCurrentContext (), parent, parent -> getField <X3D::MFNode> ("physics"), nodes, undoStep);
		}
		catch (const X3D::X3DError & error)
		{ }		
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DParticlePhysicsModelNodeEditor::set_physics ()
{
	phyicsBuffer .addEvent ();
}

void
X3DParticlePhysicsModelNodeEditor::set_nodes ()
{
	for (const auto & widget : getPhysicsGrid () .get_children ())
		getPhysicsGrid () .remove (*widget);

	// Find physics in parents.

	auto          tuple     = getArray <X3D::MFNode> (parents, "physics");
	const int32_t active    = std::get <1> (tuple);
	const bool    hasParent = not parents .empty ();

	editors = createEditors (std::get <0> (tuple));

	getPhysicsExpander ()   .set_visible (hasParent);
	getPhysicsGrid ()       .set_visible (active >= 0);
	getPhysicsAddButton ()  .set_sensitive (active >= 0);
	getPhysicsLinkButton () .set_sensitive (active == -1);

	setGridLabels (getWidget ());
}

void
X3DParticlePhysicsModelNodeEditor::on_add_physics_clicked ()
{
	getNewPhysicsPopover () .popup ();
}

void
X3DParticlePhysicsModelNodeEditor::on_new_force_physics_model_clicked ()
{
	getNewPhysicsPopover () .popdown ();

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Create New Force Physics Model"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (parents, true));
	const auto node             = executionContext -> createNode <X3D::ForcePhysicsModel> ();

	for (const auto & parent : parents)
	{
		try
		{
			X3D::X3DEditor::pushBackIntoArray (parent, parent -> getField <X3D::MFNode> ("physics"), node, undoStep);
		}
		catch (const X3D::X3DError & error)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DParticlePhysicsModelNodeEditor::on_new_wind_physics_model_clicked ()
{
	getNewPhysicsPopover () .popdown ();

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Create New Wind Physics Model"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (parents, true));
	const auto node             = executionContext -> createNode <X3D::WindPhysicsModel> ();

	for (const auto & parent : parents)
	{
		try
		{
			X3D::X3DEditor::pushBackIntoArray (parent, parent -> getField <X3D::MFNode> ("physics"), node, undoStep);
		}
		catch (const X3D::X3DError & error)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

std::vector <std::shared_ptr <X3DParticlePhysicsModelNodeInterface>>
X3DParticlePhysicsModelNodeEditor::createEditors (const X3D::MFNode & physicsNodes)
{
	std::vector <std::shared_ptr <X3DParticlePhysicsModelNodeInterface>> editors;

	size_t top   = 0;
	size_t index = 0;

	for (const auto & physicsNode : physicsNodes)
	{
		const auto editor = createEditor (physicsNode);

		if (editor)
		{
			const auto label = Gtk::manage (new Gtk::Label ());

			label -> show ();

			editor -> setIndependent (false);
			editor -> setNodes (X3D::MFNode ({ physicsNode }));

			if (index not_eq 0)
			{
				editor -> getUpButton () .set_sensitive (true);
				editor -> getUpButton () .signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &X3DParticlePhysicsModelNodeEditor::on_up_physics_clicked), index));
			}
			else
			{
				editor -> getUpButton () .set_sensitive (false);
			}

			if (index not_eq (physicsNodes .size () - 1))
			{
				editor -> getDownButton () .set_sensitive (true);
				editor -> getDownButton () .signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &X3DParticlePhysicsModelNodeEditor::on_down_physics_clicked), index));
			}
			else
			{
				editor -> getDownButton () .set_sensitive (false);
			}

			editor -> getRemoveButton () .signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &X3DParticlePhysicsModelNodeEditor::on_remove_physics_clicked), index));

			unparent (editor -> getWidget ());
			getPhysicsGrid () .attach (*label,                 0, top, 1, 1);
			getPhysicsGrid () .attach (editor -> getWidget (), 1, top, 1, 1);

			editors .emplace_back (editor);

			++ top;
		}
		else
		{
			editors .emplace_back (editor);
		}

		++ index;
	}

	return editors;
}

void
X3DParticlePhysicsModelNodeEditor::on_up_physics_clicked (const size_t index)
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Particle Physics Up"));

	for (const auto & parent : parents)
	{
		try
		{
			X3D::X3DEditor::moveValueWithinArray (parent, parent -> getField <X3D::MFNode> ("physics"), index, index - 1, undoStep);
		}
		catch (const X3D::X3DError & error)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DParticlePhysicsModelNodeEditor::on_down_physics_clicked (const size_t index)
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Particle Physics Down"));

	for (const auto & parent : parents)
	{
		try
		{
			X3D::X3DEditor::moveValueWithinArray (parent, parent -> getField <X3D::MFNode> ("physics"), index, index + 2, undoStep);
		}
		catch (const X3D::X3DError & error)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DParticlePhysicsModelNodeEditor::on_remove_physics_clicked (const size_t index)
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Particle Physics"));

	for (const auto & parent : parents)
	{
		try
		{
			X3D::X3DEditor::eraseFromArray (parent, parent -> getField <X3D::MFNode> ("physics"), index, undoStep);
		}
		catch (const X3D::X3DError & error)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

std::shared_ptr <X3DParticlePhysicsModelNodeInterface>
X3DParticlePhysicsModelNodeEditor::createEditor (const X3D::SFNode & physicsNode)
{
	if (physicsNode)
	{
		switch (physicsNode -> getType () .back ())
		{
			case X3D::X3DConstants::ForcePhysicsModel:
			{
				return std::make_shared <ForcePhysicsModelEditor> (getBrowserWindow ());
			}
			case X3D::X3DConstants::WindPhysicsModel:
			{
				return std::make_shared <WindPhysicsModelEditor> (getBrowserWindow ());
			}
			default:
			{
				return std::make_shared <DefaultPhysicsModelEditor> (getBrowserWindow ());
			}
		}
	}

	return std::make_shared <DefaultPhysicsModelEditor> (getBrowserWindow ());
}

void
X3DParticlePhysicsModelNodeEditor::connectPhysics (const X3D::MFNode & field)
{
	field .removeInterest (&X3DParticlePhysicsModelNodeEditor::connectPhysics, this);
	field .addInterest (&X3DParticlePhysicsModelNodeEditor::set_physics, this);
}

X3DParticlePhysicsModelNodeEditor::~X3DParticlePhysicsModelNodeEditor ()
{ }

} // puck
} // titania
