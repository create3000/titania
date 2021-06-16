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

#include "X3DColorRampNodeEditor.h"

#include <Titania/X3D/Components/ParticleSystems/ParticleSystem.h>
#include <Titania/X3D/Components/Rendering/Color.h>
#include <Titania/X3D/Components/Rendering/ColorRGBA.h>

namespace titania {
namespace puck {

X3DColorRampNodeEditor::X3DColorRampNodeEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                           gradient (this, "Color Ramp Gradient", getColorRampGradientBox (), "colorKey", "color"),
	                           colorKey (this,
	                                     getColorKeyAdjustment (),
	                                     getColorKeySpinButton (),
	                                     "colorKey"),
	                              color (this,
	                                    getColorRampButton (),
	                                    getColorRampAdjustment (),
	                                    getColorRampButtonBox (),
	                                    getAddColorRampButton (),
	                                    getRemoveColorRampButton (),
	                                    getColorRampScrolledWindow (),
	                                    "color"),
	                         colorRGBA (this,
	                                    getColorRampRGBAButton (),
	                                    getColorRampRGBAAdjustment (),
	                                    getColorRampRGBAButtonBox (),
	                                    getAddColorRampRGBAButton (),
	                                    getRemoveColorRampRGBAButton (),
	                                    getColorRampRGBAScrolledWindow (),
	                                    "color"),
	                            parents (),
	                        colorBuffer (),
	                          colorNode (),
	                           undoStep (),
	                           changing (false)
{
	addChildObjects (parents, colorBuffer, colorNode);

	colorBuffer .addInterest (&X3DColorRampNodeEditor::set_node, this);

	gradient  .signal_index_changed () .connect (sigc::mem_fun (this, &X3DColorRampNodeEditor::on_gradient_index_changed)); 
	color     .signal_index_changed () .connect (sigc::mem_fun (this, &X3DColorRampNodeEditor::on_gradient_color_index_changed)); 
	colorRGBA .signal_index_changed () .connect (sigc::mem_fun (this, &X3DColorRampNodeEditor::on_gradient_color_rgba_index_changed)); 

	gradient .addClass ("notebook");

	color     .setColorsSize (16);
	colorRGBA .setColorsSize (16);
}

void
X3DColorRampNodeEditor::initialize ()
{ }

void
X3DColorRampNodeEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::SFNode> ("colorRamp") .removeInterest (&X3DColorRampNodeEditor::set_color, this);
		}
		catch (const X3D::X3DError & error)
		{ }		
	}

	parents = getNodes <X3D::X3DBaseNode> (selection, { X3D::X3DConstants::ParticleSystem }, false);

	gradient .setPositionNodes (parents);
	colorKey .setNodes (parents);

	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::SFNode> ("colorRamp") .addInterest (&X3DColorRampNodeEditor::set_color, this);
		}
		catch (const X3D::X3DError & error)
		{ }		
	}

	set_color ();
}

void
X3DColorRampNodeEditor::on_gradient_index_changed ()
{
	if (changing)
		return;

	changing = true;

	color     .setIndex (gradient .getIndex ());
	colorRGBA .setIndex (gradient .getIndex ());
	colorKey  .setIndex (gradient .getIndex ());

	changing = false;
}

void
X3DColorRampNodeEditor::on_gradient_color_index_changed ()
{
	if (changing)
		return;

	changing = true;

	gradient .setIndex (color .getIndex ());
	colorKey .setIndex (color .getIndex ());

	changing = false;
}

void
X3DColorRampNodeEditor::on_gradient_color_rgba_index_changed ()
{
	if (changing)
		return;

	changing = true;

	gradient .setIndex (colorRGBA .getIndex ());
	colorKey .setIndex (colorRGBA .getIndex ());

	changing = false;
}

void
X3DColorRampNodeEditor::on_color_ramp_unlink_clicked ()
{
	X3D::UndoStepPtr undoStep;

	unlinkClone (parents, "colorRamp", undoStep);
}

void
X3DColorRampNodeEditor::on_color_ramp_type_changed ()
{
	try
	{
		getColorRampBox ()                .set_visible (getColorRampTypeButton () .get_active_row_number () > 0);
		getColorRampScrolledWindow ()     .set_visible (getColorRampTypeButton () .get_active_row_number () == 1);
		getColorRampRGBAScrolledWindow () .set_visible (getColorRampTypeButton () .get_active_row_number () == 2);
		getColorRampGrid ()               .set_visible (getColorRampTypeButton () .get_active_row_number () == 1);
		getColorRampRGBAGrid ()           .set_visible (getColorRampTypeButton () .get_active_row_number () == 2);

		if (changing)
			return;
	
		beginUndoGroup ("colorRamp", undoStep);
		addUndoFunction <X3D::MFFloat> (parents, "colorKey", undoStep);
		addUndoFunction <X3D::SFNode> (parents, "colorRamp", undoStep);
		endUndoGroup ("colorRamp", undoStep);

		const auto executionContext = X3D::MakePtr (getSelectionContext (parents, true));

		if (getColorRampTypeButton () .get_active_row_number () == -1)
		{
			__LOG__ << "get_active_row_number == 1" << std::endl;
		}
		else if (getColorRampTypeButton () .get_active_row_number () == 0)
		{
			for (const auto & parent : parents)
			{
				try
				{
					parent -> getField <X3D::MFFloat> ("colorKey") .clear ();
				}
				catch (const X3D::X3DError & error)
				{ }		

				try
				{
					auto & field = parent -> getField <X3D::SFNode> ("colorRamp");
	
					field .removeInterest (&X3DColorRampNodeEditor::set_color, this);
					field .addInterest (&X3DColorRampNodeEditor::connectColor, this);
	
					X3D::X3DEditor::replaceNode (executionContext, parent, field, nullptr, undoStep);
				}
				catch (const X3D::X3DError & error)
				{ }		
			}
		}
		else
		{
		   X3D::X3DPtr <X3D::X3DColorNode> newColorNode (executionContext -> createNode (getColorRampTypeButton () .get_active_text ()));
	
		   if (colorNode and colorNode -> getType () .back () == newColorNode -> getType () .back ())
			{
				newColorNode = colorNode;
			}
			else
			{
				for (size_t i = 0, size = colorNode -> getSize (); i < size; ++ i)
					newColorNode -> set1Color (i, colorNode -> get1Color (i));
			}

			colorNode = std::move (newColorNode);

			for (const auto & parent : parents)
			{
				try
				{
					auto & field = parent -> getField <X3D::SFNode> ("colorRamp");

					field .removeInterest (&X3DColorRampNodeEditor::set_color, this);
					field .addInterest (&X3DColorRampNodeEditor::connectColor, this);
	
					X3D::X3DEditor::replaceNode (executionContext, parent, field, colorNode, undoStep);
				}
				catch (const X3D::X3DError & error)
				{ }		
			}
		}

		beginRedoGroup ("colorRamp", undoStep);
		addRedoFunction <X3D::MFFloat> (parents, "colorKey", undoStep);
		addRedoFunction <X3D::SFNode> (parents, "colorRamp", undoStep);
		endRedoGroup ("colorRamp", undoStep);
	
		getColorRampUnlinkButton () .set_sensitive (getColorRampTypeButton () .get_active_row_number () > 0 and colorNode -> getCloneCount () > 1);

		set_widgets ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DColorRampNodeEditor::set_color ()
{
	colorBuffer .addEvent ();
}

void
X3DColorRampNodeEditor::set_node ()
{
	resetUndoGroup ("colorRamp", undoStep);

	// Find Color in selection

	const auto    executionContext = X3D::MakePtr (getSelectionContext (parents, true));
	auto          tuple            = getSelection <X3D::X3DColorNode> (parents, "colorRamp");
	const int32_t active           = std::get <1> (tuple);
	const bool    hasParent        = std::get <2> (tuple);
	const bool    hasField         = (active not_eq -2);

	colorNode = std::move (std::get <0> (tuple));

	changing = true;

	if (active == SAME_NODE)
		getColorRampTypeButton () .set_active_text (colorNode -> getTypeName ());
	else
		getColorRampTypeButton () .set_active (-1);

	getColorRampExpander ()     .set_visible (hasParent or colorNode);
	getColorRampTypeButton ()   .set_sensitive (hasField);
	getColorRampUnlinkButton () .set_sensitive (active > 0 and colorNode -> getCloneCount () > 1);

	changing = false;

	// Widgets

	if (not colorNode)
		colorNode = executionContext -> createNode <X3D::Color> ();

	set_widgets ();
}

void
X3DColorRampNodeEditor::set_widgets ()
{
	const X3D::MFNode colorNodes ({ colorNode });

	gradient .setRGBA (colorNode -> isType ({ X3D::X3DConstants::ColorRGBA }));
	gradient .setColorNodes (colorNodes);
	gradient .setIndex (-1);

	color     .setNodes (colorNode -> isType ({ X3D::X3DConstants::Color })     ? colorNodes : X3D::MFNode ());
	colorRGBA .setNodes (colorNode -> isType ({ X3D::X3DConstants::ColorRGBA }) ? colorNodes : X3D::MFNode ());
}

void
X3DColorRampNodeEditor::connectColor (const X3D::SFNode & field)
{
	field .removeInterest (&X3DColorRampNodeEditor::connectColor, this);
	field .addInterest (&X3DColorRampNodeEditor::set_color, this);
}

X3DColorRampNodeEditor::~X3DColorRampNodeEditor ()
{ }

} // puck
} // titania
