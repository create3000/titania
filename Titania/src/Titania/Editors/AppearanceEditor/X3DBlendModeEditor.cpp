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

#include "X3DBlendModeEditor.h"

#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/X_ITE/BlendMode.h>

namespace titania {
namespace puck {

X3DBlendModeEditor::X3DBlendModeEditor () :
	X3DAppearanceEditorInterface (),
	                    nodeName (this, getBlendModeNameEntry (), getBlendModeRenameButton ()),
	                  blendColor (this, getBlendModeBlendColorButton (), getBlendModeBlendColorAdjustment (), getBlendModeBlendColorBox (), "blendColor"),
	           sourceColorFactor (this, getBlendModeSourceColorFactorButton (), "sourceColorFactor"),
	           sourceAlphaFactor (this, getBlendModeSourceAlphaFactorButton (), "sourceAlphaFactor"),
	      destinationColorFactor (this, getBlendModeDestinationColorFactorButton (), "destinationColorFactor"),
	      destinationAlphaFactor (this, getBlendModeDestinationAlphaFactorButton (), "destinationAlphaFactor"),
	               colorEquation (this, getBlendModeColorEquationButton (), "colorEquation"),
	               alphaEquation (this, getBlendModeAlphaEquationButton (), "alphaEquation"),
	                 appearances (),
	                   blendMode (),
	             blendModeBuffer (),
	                    undoStep (),
	                    changing (false)
{
	addChildObjects (appearances,
	                 blendMode,
	                 blendModeBuffer);
}

void
X3DBlendModeEditor::initialize ()
{
	blendModeBuffer .addInterest (&X3DBlendModeEditor::set_node, this);
}

void
X3DBlendModeEditor::set_appearance ()
{
	for (const auto & appearance : appearances)
		appearance -> blendMode () .removeInterest (&X3DBlendModeEditor::set_blendMode, this);

	appearances = getSelection <X3D::Appearance> ({ X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> blendMode () .addInterest (&X3DBlendModeEditor::set_blendMode, this);

	set_blendMode ();
}

void
X3DBlendModeEditor::on_blendMode_unlink_clicked ()
{
	unlinkClone (appearances, "blendMode", undoStep);
}

void
X3DBlendModeEditor::on_blendMode_toggled ()
{
	try
	{
		if (changing)
			return;

		addUndoFunction <X3D::SFNode> (appearances, "blendMode", undoStep);

		getBlendModeCheckButton () .set_inconsistent (false);
		getBlendModeBox ()         .set_sensitive (getBlendModeCheckButton () .get_active ());

		const auto executionContext = X3D::X3DExecutionContextPtr (getSelectionContext (appearances));

		for (const auto & appearance : appearances)
		{
			try
			{
				auto & field = appearance -> getField <X3D::SFNode> ("blendMode");

				field .removeInterest (&X3DBlendModeEditor::set_blendMode, this);
				field .addInterest (&X3DBlendModeEditor::connectBlendMode, this);

				if (getBlendModeCheckButton () .get_active ())
				{
					X3D::X3DEditor::replaceNode (executionContext, appearance, field, blendMode, undoStep);
				}
				else
				{
					X3D::X3DEditor::replaceNode (executionContext, appearance, field, nullptr, undoStep);
				}
			}
			catch (const X3D::X3DError &)
			{ }
		}

		addRedoFunction <X3D::SFNode> (appearances, "blendMode", undoStep);

		getBlendModeUnlinkButton () .set_sensitive (getBlendModeCheckButton () .get_active () and blendMode -> getCloneCount () > 1);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DBlendModeEditor::set_blendMode ()
{
	blendModeBuffer .addEvent ();
}

void
X3DBlendModeEditor::set_node ()
{
	try
	{
		undoStep .reset ();

		if (blendMode)
			blendMode -> removeInterest (&X3DBlendModeEditor::set_equation, this);

		auto          tuple     = getSelection <X3D::BlendMode> (appearances, "blendMode");
		const int32_t active    = std::get <1> (tuple);
		const bool    hasParent = std::get <2> (tuple);
		const bool    hasField  = (active not_eq -2);

		blendMode = std::move (std::get <0> (tuple));

		if (not blendMode)
		{
			const auto executionContext = X3D::X3DExecutionContextPtr (getSelectionContext (appearances, true));

			blendMode = executionContext -> createNode <X3D::BlendMode> ();
		}

		blendMode -> addInterest (&X3DBlendModeEditor::set_equation, this);

		set_equation ();

		changing = true;

		getSelectBlendModeBox ()   .set_sensitive (hasParent);
		getBlendModeMainBox ()     .set_sensitive (hasField);
		getBlendModeCheckButton () .set_active (active > 0);
		getBlendModeCheckButton () .set_inconsistent (active < 0);

		getBlendModeUnlinkButton () .set_sensitive (active > 0 and blendMode -> getCloneCount () > 1);
		getBlendModeBox ()          .set_sensitive (active > 0);

		changing = false;

		// Widgets

		const X3D::MFNode nodes = { blendMode };

		nodeName .setNode (nodes .back ());

		blendColor             .setNodes (nodes);
		sourceColorFactor      .setNodes (nodes);
		sourceAlphaFactor      .setNodes (nodes);
		destinationColorFactor .setNodes (nodes);
		destinationAlphaFactor .setNodes (nodes);
		colorEquation          .setNodes (nodes);
		alphaEquation          .setNodes (nodes);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DBlendModeEditor::set_equation ()
{
	getBlendModeRedLabel ()   .set_text (getEquation (0, blendMode -> getSourceColorFactor (), blendMode -> getDestinationColorFactor (), blendMode -> getColorEquation ()));
	getBlendModeGreenLabel () .set_text (getEquation (1, blendMode -> getSourceColorFactor (), blendMode -> getDestinationColorFactor (), blendMode -> getColorEquation ()));
	getBlendModeBlueLabel ()  .set_text (getEquation (2, blendMode -> getSourceColorFactor (), blendMode -> getDestinationColorFactor (), blendMode -> getColorEquation ()));
	getBlendModeAlphaLabel () .set_text (getEquation (3, blendMode -> getSourceAlphaFactor (), blendMode -> getDestinationAlphaFactor (), blendMode -> getAlphaEquation ()));
}

std::string
X3DBlendModeEditor::getEquation (const size_t i, const GLenum sourceFactor, const GLenum destinationFactor, const GLenum equation) const
{
	static const std::vector <std::string> channels = { "R", "G", "B", "A" };

	const auto & channel = channels [i];

	switch (equation)
	{
		case GL_FUNC_ADD:
		{
			return getEquationTerm (i, "s", channel, sourceFactor) + " + " + getEquationTerm (i, "d", channel, destinationFactor) + " = r" + channel;
		}
		case GL_FUNC_SUBTRACT:
		{
			return getEquationTerm (i, "s", channel, sourceFactor) + " - " + getEquationTerm (i, "d", channel, destinationFactor) + " = r" + channel;
		}
		case GL_FUNC_REVERSE_SUBTRACT:
		{
			return getEquationTerm (i, "d", channel, destinationFactor) + " - " + getEquationTerm (i, "s", channel, sourceFactor) + " = r" + channel;
		}
		case GL_MIN:
		{
			return "min (s" + channel + ", d" + channel + ")" + " = r" + channel;
		}
		case GL_MAX:
		{
			return "max (s" + channel + ", d" + channel + ")" + " = r" + channel;
		}
		default:
		{
			return "n/a";
		}
	}
}

std::string
X3DBlendModeEditor::getEquationTerm (const size_t i, const std::string & side, const std::string & channel, const GLenum factor) const
{
	return "(" + side + "" + channel + " × " + getEquationFactor (i, channel, factor) + ")";
}

std::string
X3DBlendModeEditor::getEquationFactor (const size_t i, const std::string & channel, const GLenum factor) const
{
	switch (factor)
	{
	   case GL_ZERO:
			return "0";
	   case GL_ONE:
			return "1";
	   case GL_SRC_COLOR:
			return "s" + channel;
	   case GL_ONE_MINUS_SRC_COLOR:
			return "(1 - s" + channel + ")";
	   case GL_DST_COLOR:
			return "d" + channel;
	   case GL_ONE_MINUS_DST_COLOR:
			return "(1 - d" + channel + ")";
	   case GL_SRC_ALPHA:
			return "sA";
	   case GL_ONE_MINUS_SRC_ALPHA:
			return "(1 - sA)";
	   case GL_DST_ALPHA:
			return "dA";
	   case GL_ONE_MINUS_DST_ALPHA:
			return "(1 - dA)";
	   case GL_SRC_ALPHA_SATURATE:
			return "min (sA, 1 - dA)";
	   case GL_CONSTANT_COLOR:
			return basic::to_string (blendMode -> blendColor () [i]);
	   case GL_ONE_MINUS_CONSTANT_COLOR:
			return "(1 - " + basic::to_string (blendMode -> blendColor () [i]) + ")";
	   case GL_CONSTANT_ALPHA:
			return basic::to_string (blendMode -> blendColor () [3]);
	   case GL_ONE_MINUS_CONSTANT_ALPHA:
			return "(1 - " + basic::to_string (blendMode -> blendColor () [3]) + ")";
		default:
			return "n/a";
	}
}

void
X3DBlendModeEditor::connectBlendMode (const X3D::SFNode & field)
{
	field .removeInterest (&X3DBlendModeEditor::connectBlendMode, this);
	field .addInterest (&X3DBlendModeEditor::set_blendMode, this);
}

X3DBlendModeEditor::~X3DBlendModeEditor ()
{ }

} // puck
} // titania
