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

#include "ForcePhysicsModelEditor.h"
#include "WindPhysicsModelEditor.h"

#include <Titania/X3D/Components/ParticleSystems/ParticleSystem.h>

namespace titania {
namespace puck {

X3DParticlePhysicsModelNodeEditor::X3DParticlePhysicsModelNodeEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                            parents (),
	                       phyicsBuffer (),
	                       physicsNodes (),
	                            editors ()
{
	addChildObjects (parents, phyicsBuffer, physicsNodes);

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
			parent -> getField <X3D::MFNode> ("phyics") .removeInterest (&X3DParticlePhysicsModelNodeEditor::set_phyics, this);
		}
		catch (const X3D::X3DError & error)
		{ }		
	}

	parents = getNodes <X3D::X3DBaseNode> (selection, { X3D::X3DConstants::ParticleSystem }, false);

	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::MFNode> ("phyics") .addInterest (&X3DParticlePhysicsModelNodeEditor::set_phyics, this);
		}
		catch (const X3D::X3DError & error)
		{ }		
	}

	set_phyics ();
}

void
X3DParticlePhysicsModelNodeEditor::set_phyics ()
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

	physicsNodes = std::move (std::get <0> (tuple));
	editors      = createEditors (physicsNodes);

	getPhysicsExpander () .set_visible (hasParent);

	setGridLabels (getWidget ());
}

void
X3DParticlePhysicsModelNodeEditor::on_add_physics_clicked ()
{
	__LOG__ << std::endl;
}

std::vector <std::shared_ptr <X3DParticlePhysicsModelNodeInterface>>
X3DParticlePhysicsModelNodeEditor::createEditors (const X3D::MFNode & physicsNodes) const
{
	std::vector <std::shared_ptr <X3DParticlePhysicsModelNodeInterface>> editors;

	int32_t top = 0;

	for (const auto & physicsNode : physicsNodes)
	{
		const auto editor = createEditor (physicsNode);

		if (editor)
		{
			const auto label = Gtk::manage (new Gtk::Label ());

			label -> show ();

			editor -> setIndependent (false);
			editor -> setNodes (X3D::MFNode ({ physicsNode }));

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
	}

	return editors;
}

std::shared_ptr <X3DParticlePhysicsModelNodeInterface>
X3DParticlePhysicsModelNodeEditor::createEditor (const X3D::SFNode & physicsNode) const
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
				return nullptr;
			}
		}
	}

	return nullptr;
}

void
X3DParticlePhysicsModelNodeEditor::connectPhysics (const X3D::MFNode & field)
{
	field .removeInterest (&X3DParticlePhysicsModelNodeEditor::connectPhysics, this);
	field .addInterest (&X3DParticlePhysicsModelNodeEditor::set_phyics, this);
}

X3DParticlePhysicsModelNodeEditor::~X3DParticlePhysicsModelNodeEditor ()
{ }

} // puck
} // titania
