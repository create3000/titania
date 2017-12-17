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

#ifndef __TITANIA_EDITORS_PRECISION_PLACEMENT_PANEL_PARTICLE_SYSTEM_X3DPARTICLE_PHYSICS_MODEL_NODE_EDITOR_H__
#define __TITANIA_EDITORS_PRECISION_PLACEMENT_PANEL_PARTICLE_SYSTEM_X3DPARTICLE_PHYSICS_MODEL_NODE_EDITOR_H__

#include "../../../UserInterfaces/X3DPrecisionPlacementPanelInterface.h"

namespace titania {
namespace puck {

class X3DParticlePhysicsModelNodeInterface;

class X3DParticlePhysicsModelNodeEditor :
	virtual public X3DPrecisionPlacementPanelInterface
{
public:

	///  @name Destruction

	virtual
	~X3DParticlePhysicsModelNodeEditor () override;


protected:

	///  @name Construction

	X3DParticlePhysicsModelNodeEditor ();

	virtual
	void
	initialize () override;

	void
	set_selection (const X3D::MFNode & selection);


private:

	///  @name Construction

	virtual
	void
	on_physics_link_clicked () final override;

	void
	set_physics ();

	void
	set_nodes ();

	virtual
	void
	on_add_physics_clicked () final override;

	virtual
	void
	on_new_force_physics_model_clicked () final override;

	virtual
	void
	on_new_wind_physics_model_clicked () final override;

	void
	on_up_physics_clicked (const size_t index);

	void
	on_down_physics_clicked (const size_t index);
	
	void
	on_remove_physics_clicked (const size_t index);

	std::vector <std::shared_ptr <X3DParticlePhysicsModelNodeInterface>>
	createEditors (const X3D::MFNode & physicsNodes);

	std::shared_ptr <X3DParticlePhysicsModelNodeInterface>
	createEditor (const X3D::SFNode & physicsNode);

	void
	connectPhysics (const X3D::MFNode & field);

	///  @name Members

	X3D::MFNode parents;
	X3D::SFTime phyicsBuffer;

	std::vector <std::shared_ptr <X3DParticlePhysicsModelNodeInterface>> editors;

};

} // puck
} // titania

#endif
