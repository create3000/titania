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

#include "X3DTextureProjectorParallelEditor.h"

#include <Titania/X3D/Components/ProjectiveTextureMapping/TextureProjectorParallel.h>

namespace titania {
namespace puck {

X3DTextureProjectorParallelEditor::X3DTextureProjectorParallelEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                        description (this, getTextureProjectorParallelDescriptionTextView (), "description"),
	                                 on (this, getTextureProjectorParallelOnCheckButton (), "on"),
	                             global (this, getTextureProjectorParallelGlobalCheckButton (), "global"),
	                           location (this,
	                                     getTextureProjectorParallelLocationXAdjustment (),
	                                     getTextureProjectorParallelLocationYAdjustment (),
	                                     getTextureProjectorParallelLocationZAdjustment (),
	                                     getTextureProjectorParallelLocationBox (),
	                                     "location"),
	                          direction (this,
	                                     getTextureProjectorParallelDirectionXAdjustment (),
	                                     getTextureProjectorParallelDirectionYAdjustment (),
	                                     getTextureProjectorParallelDirectionZAdjustment (),
	                                     getTextureProjectorParallelDirectionBox (),
	                                     "direction"),
	                      directionTool (this, getTextureProjectorParallelDirectionToolBox (), "direction"),
	                           upVector (this,
	                                     getTextureProjectorParallelUpVectorXAdjustment (),
	                                     getTextureProjectorParallelUpVectorYAdjustment (),
	                                     getTextureProjectorParallelUpVectorZAdjustment (),
	                                     getTextureProjectorParallelUpVectorBox (),
	                                     "upVector"),
	                       upVectorTool (this, getTextureProjectorParallelUpVectorToolBox (), "upVector"),
	                       fieldOfView0 (this, getTextureProjectorParallelFieldOfViewMinXAdjustment (), getTextureProjectorParallelFieldOfViewSpinButton0 (), "fieldOfView"),
	                       fieldOfView1 (this, getTextureProjectorParallelFieldOfViewMinYAdjustment (), getTextureProjectorParallelFieldOfViewSpinButton1 (), "fieldOfView"),
	                       fieldOfView2 (this, getTextureProjectorParallelFieldOfViewMaxXAdjustment (), getTextureProjectorParallelFieldOfViewSpinButton2 (), "fieldOfView"),
	                       fieldOfView3 (this, getTextureProjectorParallelFieldOfViewMaxYAdjustment (), getTextureProjectorParallelFieldOfViewSpinButton3 (), "fieldOfView"),
	                       nearDistance (this,
	                                     getTextureProjectorParallelNearDistanceAdjustment (),
	                                     getTextureProjectorParallelNearDistanceSpinButton (),
	                                     "nearDistance"),
	                        farDistance (this,
	                                     getTextureProjectorParallelFarDistanceAdjustment (),
	                                     getTextureProjectorParallelFarDistanceSpinButton (),
	                                     "farDistance"),
	                        aspectRatio (this,
	                                     getTextureProjectorParallelAspectRatioAdjustment (),
	                                     getTextureProjectorParallelAspectRatioSpinButton (),
	                                     "aspectRatio")
{
	direction .setNormalize (true);
	upVector  .setNormalize (true);

	fieldOfView0 .setIndex (0);
	fieldOfView1 .setIndex (1);
	fieldOfView2 .setIndex (2);
	fieldOfView3 .setIndex (3);
}

void
X3DTextureProjectorParallelEditor::initialize ()
{ }

void
X3DTextureProjectorParallelEditor::set_selection (const X3D::MFNode & selection)
{
	// Get TextureProjectorParallel

	const auto projectorNode  = X3D::X3DPtr <X3D::TextureProjectorParallel> (selection .empty () ? nullptr : selection .back ());
	const auto projectorNodes = projectorNode ? X3D::MFNode ({ projectorNode }) : X3D::MFNode ();

	getTextureProjectorParallelExpander () .set_visible (projectorNode);

	if (projectorNode and projectorNode -> fieldOfView () .size () < 4)
	{
		projectorNode -> fieldOfView () = {
			projectorNode -> getMinimumX (),
			projectorNode -> getMinimumY (),
			projectorNode -> getMaximumX (),
			projectorNode -> getMaximumY ()
		};
	}

	description   .setNodes (projectorNodes);
	on            .setNodes (projectorNodes);
	global        .setNodes (projectorNodes);
	location      .setNodes (projectorNodes);
	direction     .setNodes (projectorNodes);
	directionTool .setNodes (projectorNodes);
	upVector      .setNodes (projectorNodes);
	upVectorTool  .setNodes (projectorNodes);
	fieldOfView0  .setNodes (projectorNodes);
	fieldOfView1  .setNodes (projectorNodes);
	fieldOfView2  .setNodes (projectorNodes);
	fieldOfView3  .setNodes (projectorNodes);
	nearDistance  .setNodes (projectorNodes);
	farDistance   .setNodes (projectorNodes);
	aspectRatio   .setNodes (projectorNodes);
}

X3DTextureProjectorParallelEditor::~X3DTextureProjectorParallelEditor ()
{ }

} // puck
} // titania
