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

#include "RangeTool.h"

namespace titania {
namespace puck {

RangeTool::RangeTool (X3DBaseInterface* const editor,
                      const std::string & name,
                      Gtk::Box & box) :
	X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	 X3DGradientTool (editor, name, box, "range", "color"),
	         auxNode (),
	  positionFactor (1)
{
	addChildren (auxNode);
}

void
RangeTool::setNodes (const X3D::MFNode & nodes)
{
	if (nodes .empty ())
		X3DGradientTool::setNodes ({ });

	else
	{
		auxNode = new X3D::FieldSet (getMasterBrowser ());
	
		auxNode -> addUserDefinedField (X3D::inputOutput, "range", new X3D::MFFloat ());
		auxNode -> addUserDefinedField (X3D::inputOutput, "color", new X3D::MFColor ());

		for (const auto & node : nodes)
		{
			try
			{
				//__LOG__ << node -> getTypeName () << std::endl;

				node    -> getField <X3D::MFFloat> ("range") .addInterest (auxNode -> getField <X3D::MFFloat> ("range"));
				auxNode -> getField <X3D::MFFloat> ("range") .addInterest (node    -> getField <X3D::MFFloat> ("range"));

				auxNode -> setField <X3D::MFFloat> ("range", node -> getField <X3D::MFFloat> ("range"));
			}
			catch (const X3D::X3DError & error)
			{
				__LOG__ << error .what () << std::endl;
			}
		}

		auxNode -> setup ();
		X3DGradientTool::setNodes ({ auxNode });
	}
}

X3D::MFFloat
RangeTool::get_position (const X3D::MFFloat & position)
{
	X3D::MFFloat range;

	if (not position .empty ())
	{
		for (const auto & value : position)
			range .emplace_back (value * positionFactor);
	}

	return range;
}

std::pair <X3D::MFFloat, X3D::MFColor>
RangeTool::get_tool_values (const X3D::MFFloat & positionValue, const X3D::MFColor & colorValue)
{
	try
	{
		X3D::MFFloat position;
		X3D::MFColor color;

		if (not positionValue .empty ())
		{
			positionFactor = positionValue .back ();
	
			for (const auto & value : positionValue)
			{
				const auto p = value / positionFactor;

				position .emplace_back (p);
				color    .emplace_back (1 - p, 1 -p, 1 - p);
			}	
		}

		return std::make_pair (position, color);
	}
	catch (const X3D::X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
		return std::make_pair (positionValue, colorValue);
	}
}

} // puck
} // titania
