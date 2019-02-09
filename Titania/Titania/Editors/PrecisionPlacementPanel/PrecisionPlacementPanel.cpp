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

#include "PrecisionPlacementPanel.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"
#include "../../Editors/NodeIndex/NodeIndex.h"

namespace titania {
namespace puck {

PrecisionPlacementPanel::PrecisionPlacementPanel (X3DBrowserWindow* const browserWindow) :
	                   X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DPrecisionPlacementPanelInterface (get_ui ("Editors/PrecisionPlacementPanel.glade")),
	                 X3DTransformEditor (),
	                    X3DSwitchEditor (),
	                 X3DBillboardEditor (),
	                 X3DCollisionEditor (),
	                       X3DLODEditor (),
	                    X3DLayoutEditor (),
	                  X3DViewportEditor (),
	              X3DGeoTransformEditor (),
	               X3DGeoLocationEditor (),
	       X3DEnvironmentalSensorEditor (),
	            X3DParticleSystemEditor (),
	       X3DParticleEmitterNodeEditor (),
	         X3DGeometrySelectionEditor (),
	                           nodeName (this, getNameEntry (), getRenameButton ()),
	                           bboxSize (this,
	                                     getBBoxSizeXAdjustment (),
	                                     getBBoxSizeYAdjustment (),
	                                     getBBoxSizeZAdjustment (),
	                                     getBBoxSizeBox (),
	                                     "bboxSize"),
	                         bboxCenter (this,
	                                     getBBoxCenterXAdjustment (),
	                                     getBBoxCenterYAdjustment (),
	                                     getBBoxCenterZAdjustment (),
	                                     getBBoxCenterBox (),
	                                     "bboxCenter"),
	                              scene (browserWindow -> getCurrentScene ()),
	                   executionContext (browserWindow -> getCurrentContext ()),
	                      boundedObject (),
	                       geometryNode ()
{
	addChildObjects (executionContext, boundedObject, geometryNode);

	setup ();
}

void
PrecisionPlacementPanel::initialize ()
{
	X3DPrecisionPlacementPanelInterface::initialize ();
	X3DTransformEditor::initialize ();
	X3DSwitchEditor::initialize ();
	X3DBillboardEditor::initialize ();
	X3DCollisionEditor::initialize ();
	X3DLODEditor::initialize ();
	X3DLayoutEditor::initialize ();
	X3DViewportEditor::initialize ();
	X3DGeoTransformEditor::initialize ();
	X3DGeoLocationEditor::initialize ();
	X3DEnvironmentalSensorEditor::initialize ();
	X3DParticleSystemEditor::initialize ();
	X3DParticleEmitterNodeEditor::initialize ();
	X3DGeometrySelectionEditor::initialize ();
}

void
PrecisionPlacementPanel::configure ()
{
	X3DPrecisionPlacementPanelInterface::configure ();
	X3DTransformEditor::configure ();
	X3DGeoTransformEditor::configure ();
	X3DEnvironmentalSensorEditor::configure ();
	X3DParticleSystemEditor::configure ();
	X3DGeometrySelectionEditor::configure ();

	getBBoxUniformSizeButton () .set_active (getConfig () -> getItem <bool> ("bboxUniformSize"));
}

void
PrecisionPlacementPanel::on_map ()
{
	getCurrentScene ()   .addInterest (&PrecisionPlacementPanel::set_scene,             this);
	getCurrentContext () .addInterest (&PrecisionPlacementPanel::set_execution_context, this);

	set_scene ();
	set_execution_context ();
}

void
PrecisionPlacementPanel::on_unmap ()
{
	getCurrentScene ()   .removeInterest (&PrecisionPlacementPanel::set_scene,             this);
	getCurrentContext () .removeInterest (&PrecisionPlacementPanel::set_execution_context, this);
}

void
PrecisionPlacementPanel::set_scene ()
{
	scene -> units_changed () .removeInterest (&PrecisionPlacementPanel::set_bbox, this);

	scene = getCurrentScene ();

	scene -> units_changed () .addInterest (&PrecisionPlacementPanel::set_bbox, this);
}

void
PrecisionPlacementPanel::set_execution_context ()
{
	executionContext -> bbox_changed () .removeInterest (&PrecisionPlacementPanel::set_bbox, this);

	executionContext = getCurrentContext ();

	executionContext -> bbox_changed () .addInterest (&PrecisionPlacementPanel::set_bbox, this);
}

void
PrecisionPlacementPanel::set_selection (const X3D::MFNode & selection)
{
	X3DPrecisionPlacementPanelInterface::set_selection (selection);
	X3DTransformEditor::set_selection (selection);
	X3DSwitchEditor::set_selection (selection);
	X3DBillboardEditor::set_selection (selection);
	X3DCollisionEditor::set_selection (selection);
	X3DLODEditor::set_selection (selection);
	X3DLayoutEditor::set_selection (selection);
	X3DViewportEditor::set_selection (selection);
	X3DGeoTransformEditor::set_selection (selection);
	X3DGeoLocationEditor::set_selection (selection);
	X3DEnvironmentalSensorEditor::set_selection (selection);
	X3DParticleSystemEditor::set_selection (selection);
	X3DParticleEmitterNodeEditor::set_selection (selection);

	boundedObject = selection .empty () ? nullptr : selection .back ();
	geometryNode  = selection .empty () ? nullptr : selection .back ();

	const auto boundedObjects = boundedObject ? X3D::MFNode ({ boundedObject }) : X3D::MFNode ();

	if (selection .empty ())
		getHeaderBar () .set_subtitle ("");
	else
		getHeaderBar () .set_subtitle ("»" + selection .back () -> getTypeName () + "«");

	nodeName   .setNode  (boundedObject);
	bboxSize   .setNodes (boundedObjects);
	bboxCenter .setNodes (boundedObjects);

	getFillBoundingBoxFieldsButton () .set_sensitive (boundedObject and not boundedObject -> isType ({ X3D::X3DConstants::X3DEnvironmentalSensorNode }));
	getWindow () .resize (getWindow () .get_width (), 1);

	set_bbox ();
}

void
PrecisionPlacementPanel::on_index_clicked ()
{
	const auto nodeIndex = getBrowserWindow () -> addDialog <NodeIndex> ("NodeIndex");

	nodeIndex -> setTypes ({ X3D::X3DConstants::X3DBoundedObject, X3D::X3DConstants::X3DLayerNode });
}

void
PrecisionPlacementPanel::on_bbox_uniform_size_clicked ()
{
	if (getBBoxUniformSizeButton () .get_active ())
	{
		getBBoxUniformSizeImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		bboxSize .setUniform (true);
	}
	else
	{
		getBBoxUniformSizeImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		bboxSize .setUniform (false);
	}
}

void
PrecisionPlacementPanel::on_fill_bounding_box_fields_clicked ()
{
	using setValue = void (X3D::SFVec3f::*) (const X3D::Vector3f &);

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Fill Bounding Box Fields From Scratch"));

	for (const auto & node : getBrowserWindow () -> getSelection () -> getNodes ())
	{
		const X3D::X3DPtr <X3D::X3DBoundedObject> boundedObject (node);
		const X3D::X3DPtr <X3D::X3DGroupingNode>  group (node);

		if (not boundedObject)
			continue;
	
		const X3D::Vector3f bboxSize1   = boundedObject -> bboxSize ();
		const X3D::Vector3f bboxCenter1 = boundedObject -> bboxCenter ();
	
		// Reset to get calculated bbox.
		boundedObject -> bboxSize ()   = X3D::Vector3f (-1, -1, -1);
		boundedObject -> bboxCenter () = X3D::Vector3f ();
	
		const auto bbox2       = group ? group -> X3DGroupingNode::getBBox () : boundedObject -> getBBox ();
		const auto bboxSize2   = bbox2 .size ();
		const auto bboxCenter2 = bbox2 .center ();
	
		undoStep -> addObjects (boundedObject);
		undoStep -> addUndoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (boundedObject -> bboxSize ()), bboxSize1);
		undoStep -> addRedoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (boundedObject -> bboxSize ()), bboxSize2);
		boundedObject -> bboxSize () = bboxSize2;
	
		undoStep -> addObjects (boundedObject);
		undoStep -> addUndoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (boundedObject -> bboxCenter ()), bboxCenter1);
		undoStep -> addRedoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (boundedObject -> bboxCenter ()), bboxCenter2);
		boundedObject -> bboxCenter () = bboxCenter2;

		// Proto support
	
		X3D::X3DEditor::requestUpdateInstances (node, undoStep);
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
PrecisionPlacementPanel::set_bbox ()
{
	X3D::Box3d bbox;

	if (boundedObject)
		bbox = boundedObject -> getBBox ();
	else if (geometryNode)
		bbox = geometryNode -> getBBox ();

	const auto bboxSize   = bbox .size ();
	const auto bboxCenter = bbox .center ();

	getBBoxSizeXLabel () .set_text (basic::to_string (getCurrentScene () -> toUnit (X3D::UnitCategory::LENGTH, bboxSize .x ())));
	getBBoxSizeYLabel () .set_text (basic::to_string (getCurrentScene () -> toUnit (X3D::UnitCategory::LENGTH, bboxSize .y ())));
	getBBoxSizeZLabel () .set_text (basic::to_string (getCurrentScene () -> toUnit (X3D::UnitCategory::LENGTH, bboxSize .z ())));

	getBBoxCenterXLabel () .set_text (basic::to_string (getCurrentScene () -> toUnit (X3D::UnitCategory::LENGTH, bboxCenter .x ())));
	getBBoxCenterYLabel () .set_text (basic::to_string (getCurrentScene () -> toUnit (X3D::UnitCategory::LENGTH, bboxCenter .y ())));
	getBBoxCenterZLabel () .set_text (basic::to_string (getCurrentScene () -> toUnit (X3D::UnitCategory::LENGTH, bboxCenter .z ())));
}

void
PrecisionPlacementPanel::store ()
{
	getConfig () -> setItem ("bboxUniformSize", getBBoxUniformSizeButton () .get_active ());

	X3DGeometrySelectionEditor::store ();
	X3DParticleSystemEditor::store ();
	X3DEnvironmentalSensorEditor::store ();
	X3DGeoTransformEditor::store ();
	X3DTransformEditor::store ();
	X3DPrecisionPlacementPanelInterface::store ();
}

PrecisionPlacementPanel::~PrecisionPlacementPanel ()
{
	dispose ();
}

} // puck
} // titania
