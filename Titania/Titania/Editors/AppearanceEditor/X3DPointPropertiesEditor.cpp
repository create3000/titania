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

#include "X3DPointPropertiesEditor.h"

namespace titania {
namespace puck {

X3DPointPropertiesEditor::X3DPointPropertiesEditor () :
	X3DAppearanceEditorInterface (),
	                 appearances (),
	             pointProperties (),
	       pointPropertiesBuffer (),
	                    nodeName (this, getPointPropertiesNameEntry (), getPointPropertiesRenameButton ()),
	        pointSizeScaleFactor (this,
	                              getPointPropertiesPointSizeScaleFactorAdjustment (),
	                              getPointPropertiesPointSizeScaleFactorSpinButton (),
	                              "pointSizeScaleFactor"),
	           pointSizeMinValue (this,
	                              getPointPropertiesPointSizeMinValueAdjustment (),
	                              getPointPropertiesPointSizeMinValueSpinButton (),
	                              "pointSizeMinValue"),
	           pointSizeMaxValue (this,
	                              getPointPropertiesPointSizeMaxValueAdjustment (),
	                              getPointPropertiesPointSizeMaxValueSpinButton (),
	                              "pointSizeMaxValue"),
	       pointSizeAttenuation0 (this, getPointPropertiesPointSizeAttenuation0Adjustment (), getPointPropertiesPointSizeAttenuation0SpinButton (), "pointSizeAttenuation"),
	       pointSizeAttenuation1 (this, getPointPropertiesPointSizeAttenuation1Adjustment (), getPointPropertiesPointSizeAttenuation1SpinButton (), "pointSizeAttenuation"),
	       pointSizeAttenuation2 (this, getPointPropertiesPointSizeAttenuation2Adjustment (), getPointPropertiesPointSizeAttenuation1SpinButton (), "pointSizeAttenuation"),
			             colorMode (this, getPointPropertiesColorModeComboBoxText (), "colorMode"),
	                    undoStep (),
	                    changing (false)
{
	addChildObjects (appearances,
	                 pointProperties,
	                 pointPropertiesBuffer);

	pointSizeAttenuation0 .setIndex (0);
	pointSizeAttenuation1 .setIndex (1);
	pointSizeAttenuation2 .setIndex (2);
}

void
X3DPointPropertiesEditor::initialize ()
{
	getPreview () -> initialized () .addInterest (&X3DPointPropertiesEditor::set_pointProperties, this);

	pointPropertiesBuffer .addInterest (&X3DPointPropertiesEditor::set_node, this);
}

void
X3DPointPropertiesEditor::set_appearance ()
{
	for (const auto & appearance : appearances)
		appearance -> pointProperties () .removeInterest (&X3DPointPropertiesEditor::set_pointProperties, this);

	appearances = getSelection <X3D::Appearance> ({ X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> pointProperties () .addInterest (&X3DPointPropertiesEditor::set_pointProperties, this);

	set_pointProperties ();
}

void
X3DPointPropertiesEditor::on_pointProperties_unlink_clicked ()
{
	unlinkClone (appearances, "pointProperties", undoStep);
}

void
X3DPointPropertiesEditor::on_pointProperties_toggled ()
{
	try
	{
		if (changing)
			return;

		const auto previewAppearance = getPreview () -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("LineAppearance");

		addUndoFunction <X3D::SFNode> (appearances, "pointProperties", undoStep);

		getPointPropertiesCheckButton () .set_inconsistent (false);
		getPointPropertiesBox ()         .set_sensitive (getPointPropertiesCheckButton () .get_active ());

		const auto executionContext = X3D::X3DExecutionContextPtr (getSelectionContext (appearances));

		for (const auto & appearance : appearances)
		{
			try
			{
				auto & field = appearance -> getField <X3D::SFNode> ("pointProperties");

				field .removeInterest (&X3DPointPropertiesEditor::set_pointProperties, this);
				field .addInterest (&X3DPointPropertiesEditor::connectPointProperties, this);

				if (getPointPropertiesCheckButton () .get_active ())
				{
					X3D::X3DEditor::replaceNode (executionContext, appearance, field, pointProperties, undoStep);
					previewAppearance -> pointProperties () = pointProperties;
				}
				else
				{
					X3D::X3DEditor::replaceNode (executionContext, appearance, field, nullptr, undoStep);
					previewAppearance -> pointProperties () = nullptr;
				}
			}
			catch (const X3D::X3DError &)
			{ }
		}

		addRedoFunction <X3D::SFNode> (appearances, "pointProperties", undoStep);

		getPointPropertiesUnlinkButton () .set_sensitive (getPointPropertiesCheckButton () .get_active () and pointProperties -> getCloneCount () > 1);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DPointPropertiesEditor::set_pointProperties ()
{
	pointPropertiesBuffer .addEvent ();
}

void
X3DPointPropertiesEditor::set_node ()
{
	try
	{
		const auto previewAppearance = getPreview () -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("LineAppearance");

		if (pointProperties)
			pointProperties -> removeInterest (&X3D::X3DBrowser::addEvent, *getPreview ());

		undoStep .reset ();

		auto          tuple     = getSelection <X3D::PointProperties> (appearances, "pointProperties");
		const int32_t active    = std::get <1> (tuple);
		const bool    hasParent = std::get <2> (tuple);
		const bool    hasField  = (active not_eq -2);

		pointProperties = std::move (std::get <0> (tuple));

		previewAppearance -> pointProperties () = pointProperties;

		if (pointProperties)
		{
			pointProperties -> addInterest (&X3D::X3DBrowser::addEvent, *getPreview ());
		}
		else
		{
			const auto executionContext = getSelectionContext (appearances, true);

			pointProperties = executionContext  -> createNode <X3D::PointProperties> ();
			pointProperties -> addInterest (&X3D::X3DBrowser::addEvent, *getPreview ());
		}

		changing = true;

		getPointPropertiesMainBox ()     .set_sensitive (hasField);
		getPointPropertiesCheckButton () .set_sensitive (hasParent);
		getPointPropertiesCheckButton () .set_active (active > 0);
		getPointPropertiesCheckButton () .set_inconsistent (active < 0);

		getPointPropertiesUnlinkButton () .set_sensitive (active > 0 and pointProperties -> getCloneCount () > 1);
		getPointPropertiesBox ()          .set_sensitive (active > 0);

		changing = false;

		const X3D::MFNode nodes = { pointProperties };

		nodeName .setNode (nodes .back ());

		pointSizeScaleFactor  .setNodes (nodes);
		pointSizeMinValue     .setNodes (nodes);
		pointSizeMaxValue     .setNodes (nodes);
		pointSizeAttenuation0 .setNodes (nodes);
		pointSizeAttenuation1 .setNodes (nodes);
		pointSizeAttenuation2 .setNodes (nodes);
		colorMode             .setNodes (nodes);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DPointPropertiesEditor::connectPointProperties (const X3D::SFNode & field)
{
	field .removeInterest (&X3DPointPropertiesEditor::connectPointProperties, this);
	field .addInterest (&X3DPointPropertiesEditor::set_pointProperties, this);
}

X3DPointPropertiesEditor::~X3DPointPropertiesEditor ()
{ }

} // puck
} // titania
