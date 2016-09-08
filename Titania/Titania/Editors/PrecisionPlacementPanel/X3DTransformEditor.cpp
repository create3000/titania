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

#include "X3DTransformEditor.h"

#include "../../ComposedWidgets/RotationTool.h"

#include <Titania/X3D/Components/Grouping/X3DTransformNode.h>

namespace titania {
namespace puck {

X3DTransformEditor::X3DTransformEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                        translation (this,
	                                     getTransformTranslationXAdjustment (),
	                                     getTransformTranslationYAdjustment (),
	                                     getTransformTranslationZAdjustment (),
	                                     getTransformTranslationBox (),
	                                     "translation"),
	                           rotation (this,
	                                     getTransformRotationXAdjustment (),
	                                     getTransformRotationYAdjustment (),
	                                     getTransformRotationZAdjustment (),
	                                     getTransformRotationAAdjustment (),
	                                     getTransformRotationBox (),
	                                     "rotation"),
	                       rotationTool (new RotationTool (this, getTransformRotationToolBox (), "rotation")),
	                              scale (this,
	                                     getTransformScaleXAdjustment (),
	                                     getTransformScaleYAdjustment (),
	                                     getTransformScaleZAdjustment (),
	                                     getTransformScaleBox (),
	                                     "scale"),
	                   scaleOrientation (this,
	                                     getTransformScaleOrientationXAdjustment (),
	                                     getTransformScaleOrientationYAdjustment (),
	                                     getTransformScaleOrientationZAdjustment (),
	                                     getTransformScaleOrientationAAdjustment (),
	                                     getTransformScaleOrientationBox (),
	                                     "scaleOrientation"),
	               scaleOrientationTool (new RotationTool (this, getTransformScaleOrientationToolBox (), "scaleOrientation")),
	                             center (this,
	                                     getTransformCenterXAdjustment (),
	                                     getTransformCenterYAdjustment (),
	                                     getTransformCenterZAdjustment (),
	                                     getTransformCenterBox (),
	                                     "center")
{ }

void
X3DTransformEditor::configure ()
{
	getTransformUniformScaleButton () .set_active (getConfig () -> getBoolean ("transformUniformScale"));
}

void
X3DTransformEditor::set_selection (const X3D::MFNode & selection)
{
	const X3D::X3DPtr <X3D::X3DTransformNode> transform (selection .empty () ? nullptr : selection .back ());
	const X3D::MFNode                         transforms (transform ? X3D::MFNode ({ transform }) : X3D::MFNode ());

	getTransformExpander () .set_visible (transform);

	translation      .setNodes (transforms);
	rotation         .setNodes (transforms);
	scale            .setNodes (transforms);
	scaleOrientation .setNodes (transforms);
	center           .setNodes (transforms);

	rotationTool -> setNodes (transforms);
	scaleOrientationTool -> setNodes (transforms);
}

void
X3DTransformEditor::on_transform_uniform_scale_clicked ()
{
	if (getTransformUniformScaleButton () .get_active ())
	{
		getTransformUniformScaleImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getTransformUniformScaleImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

void
X3DTransformEditor::on_transform_move_center_button ()
{
	if (getBrowserWindow () -> getSelection () -> getChildren () .empty ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Center Of Transform To BBox Center"));
	
	for (const auto & node : getBrowserWindow () -> getSelection () -> getChildren ())
	{
		const X3D::X3DPtr <X3D::X3DTransformNode> transform (node);

		if (not transform)
			continue;
	
		undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter,
		                             transform,
		                             transform -> getMatrix (),
		                             transform -> center () .getValue ());
	
		undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixWithCenter,
		                             transform,
		                             transform -> getMatrix (),
		                             transform -> X3D::X3DGroupingNode::getBBox () .center ());
	
		transform -> setMatrixWithCenter (transform -> getMatrix (), transform -> X3D::X3DGroupingNode::getBBox () .center ());
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DTransformEditor::store ()
{
	getConfig () -> setItem ("transformUniformScale", getTransformUniformScaleButton () .get_active ());
}

X3DTransformEditor::~X3DTransformEditor ()
{ }

} // puck
} // titania
