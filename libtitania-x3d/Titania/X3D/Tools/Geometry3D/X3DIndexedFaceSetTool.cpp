/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DIndexedFaceSetTool.h"

#include "../Rendering/CoordinateTool.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../Editing/Selection/FaceSelection.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

static constexpr size_t TRANSLATIONS_EVENTS = 4;

X3DIndexedFaceSetTool::Fields::Fields () :
	undo_changed (new UndoStepContainerPtr ())
{ }

X3DIndexedFaceSetTool::X3DIndexedFaceSetTool () :
	                  IndexedFaceSet (getExecutionContext ()),
	     X3DComposedGeometryNodeTool (),
	X3DIndexedFaceSetSelectionObject (),
	                          fields (),
	                     touchSensor (),
	                     planeSensor (),
	                     translation (),
	                    translations (0),
	                        undoStep (std::make_shared <X3D::UndoStep> (_ ("Empty UndoStep")))
{
	//addType (X3DConstants::X3DIndexedFaceSetTool);

	addChildren (touchSensor,
	             planeSensor);
}

void
X3DIndexedFaceSetTool::initialize ()
{
	X3DComposedGeometryNodeTool::initialize ();
	X3DIndexedFaceSetSelectionObject::initialize ();

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DIndexedFaceSetTool::set_loadState);
}

void
X3DIndexedFaceSetTool::set_loadState ()
{
	try
	{
		const auto & inlineNode         = getCoordinateTool () -> getInlineNode ();
		const auto   activeFaceGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("ActiveFaceGeometry");

		planeSensor = inlineNode -> getExportedNode <PlaneSensor>      ("PlaneSensor");
		touchSensor = inlineNode -> getExportedNode <TouchSensor>      ("TouchSensor");

		getBrowser () -> hasControlKey ()  .addInterest (this, &X3DIndexedFaceSetTool::set_touch_sensor_hitPoint);
		touchSensor -> hitPoint_changed () .addInterest (this, &X3DIndexedFaceSetTool::set_touch_sensor_hitPoint);

		planeSensor -> isActive ()            .addInterest (this, &X3DIndexedFaceSetTool::set_plane_sensor_active);
		planeSensor -> translation_changed () .addInterest (this, &X3DIndexedFaceSetTool::set_plane_sensor_translation);
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetTool::set_touch_sensor_hitPoint ()
{
	try
	{
		// Setup PlaneSensor

		switch (getActionType ())
		{
			case ActionType::SELECT:
			case ActionType::TRANSLATE:
			{
				if (planeSensor -> isActive ())
					return;

				switch (getHotPoints () .size ())
				{
					case 0:
					{
						planeSensor -> enabled () = false;
						break;
					}
					case 1:
					{
						// Translate over screen plane

						const auto vector       = inverse (getModelViewMatrix ()) .mult_dir_matrix (Vector3d (0, 0, 1));
						const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), vector);

						planeSensor -> enabled ()      = not paintSelection ();
						planeSensor -> axisRotation () = axisRotation;
						planeSensor -> offset ()       = Vector3d ();
						planeSensor -> maxPosition ()  = Vector2d (-1, -1);
						break;
					}
					case 2:
					{
						// Translate along edge

						const auto vector       = getCoord () -> get1Point (getHotPoints () [0]) - getCoord () -> get1Point (getHotPoints () [1]);
						const auto axisRotation = Rotation4d (Vector3d (1, 0, 0), vector);

						planeSensor -> enabled ()      = not paintSelection ();
						planeSensor -> axisRotation () = axisRotation;
						planeSensor -> offset ()       = Vector3d ();
						planeSensor -> maxPosition ()  = Vector2d (-1, 0);
						break;
					}
					default:
					{
						const auto normal = getPolygonNormal (getFaceSelection () -> getFaceVertices (getHotFace ()));
							
						if (getBrowser () -> hasControlKey ())
						{
							// Translate along face normal

							const auto axisRotation = Rotation4d (Vector3d (1, 0, 0), Vector3d (normal));

							planeSensor -> enabled ()      = not paintSelection ();
							planeSensor -> axisRotation () = axisRotation;
							planeSensor -> offset ()       = Vector3d ();
							planeSensor -> maxPosition ()  = Vector2d (-1, 0);
						}
						else
						{
							// Translate along plane

							const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), Vector3d (normal));

							planeSensor -> enabled ()      = not paintSelection ();
							planeSensor -> axisRotation () = axisRotation;
							planeSensor -> offset ()       = Vector3d ();
							planeSensor -> maxPosition ()  = Vector2d (-1, -1);
						}
				
						break;
					}
				}

				break;
			}
			default:
			   break;
		}
	}
	catch (const std::exception & error)
	{ }
}

void
X3DIndexedFaceSetTool::set_plane_sensor_active (const bool active)
{
	switch (getActionType ())
	{
		case ActionType::SELECT:
		case ActionType::TRANSLATE:
		{
			if (active)
			{
				// Create undo step for translation.

				undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Translate %s »point«"), getCoord () -> getTypeName () .c_str ()));

			   undoSetCoordPoint (undoStep);
			}
			else
			{
				setActionType (ActionType::SELECT);

				redoSetCoordPoint (undoStep);

				// Reset fields and send undo step.

				if (abs (translation))
					undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);

				translation   = Vector3d ();
				translations  = 0;
			}

			break;
		}
		default:
		   break;
	}
}

void
X3DIndexedFaceSetTool::set_plane_sensor_translation ()
{
	switch (getActionType ())
	{
		case ActionType::SELECT:
		case ActionType::TRANSLATE:
		{
			// Prevent accidentially move.
			if (translations ++ < TRANSLATIONS_EVENTS)
			   return;

			translation = planeSensor -> translation_changed () .getValue ();

			if (abs (translation))
			{
				setActionType (ActionType::TRANSLATE);

				for (const auto & pair : getSelectedPoints ())
					getCoord () -> set1Point (pair .first, pair .second + translation);

				break;
			}
		}
		default:
		   break;
	}
}

void
X3DIndexedFaceSetTool::undoSetColorIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (colorIndex ()), colorIndex ());
}

void
X3DIndexedFaceSetTool::redoSetColorIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (colorIndex ()), colorIndex ());
}

void
X3DIndexedFaceSetTool::undoSetTexCoordIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (texCoordIndex ()), texCoordIndex ());
}

void
X3DIndexedFaceSetTool::redoSetTexCoordIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (texCoordIndex ()), texCoordIndex ());
}

void
X3DIndexedFaceSetTool::undoSetNormalIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (normalIndex ()), normalIndex ());
}

void
X3DIndexedFaceSetTool::redoSetNormalIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (normalIndex ()), normalIndex ());
}

void
X3DIndexedFaceSetTool::undoSetCoordIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (coordIndex ()), coordIndex ());
}

void
X3DIndexedFaceSetTool::redoSetCoordIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (coordIndex ()), coordIndex ());
}

void
X3DIndexedFaceSetTool::dispose ()
{
	__LOG__ << std::endl;

	X3DIndexedFaceSetSelectionObject::dispose ();
	X3DComposedGeometryNodeTool::dispose ();
}

X3DIndexedFaceSetTool::~X3DIndexedFaceSetTool ()
{ }

} // X3D
} // titania
