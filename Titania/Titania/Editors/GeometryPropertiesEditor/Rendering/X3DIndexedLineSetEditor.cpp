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

#include "X3DIndexedLineSetEditor.h"

#include <Titania/X3D/Browser/Rendering/LSystemOptions.h>
#include <Titania/X3D/Components/Rendering/IndexedLineSet.h>

#include <regex>

namespace titania {
namespace puck {

X3DIndexedLineSetEditor::X3DIndexedLineSetEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                   lSystemIterations (this, getIndexLineSetLSystemIterationsAdjustment (), getIndexLineSetLSystemIterationsSpinButton (), "iterations"),
	                        lSystemAngle (this, getIndexLineSetLSystemAngleAdjustment (), getIndexLineSetLSystemAngleBox (), "angle"),
	                    lSystemConstants (this, getIndexLineSetLSystemConstantsEntry (), "constants"),
	                        lSystemAxiom (this, getIndexLineSetLSystemAxiomEntry (), "axiom"),
	                         lSystemRule (this,
	                                      getIndexLineSetLSystemRuleBox (),
	                                      getIndexLineSetLSystemAddRuleButton (),
	                                      "rule"),
	                               nodes (),
	                            changing (false)
{
	addChildObjects (nodes);

	getIndexLineSetLSystemAngleAdjustment () -> set_upper (math::pi <double> * 2);

	lSystemConstants .setFilter (&X3DIndexedLineSetEditor::validateLSystemConstants);
	lSystemAxiom     .setFilter (&X3DIndexedLineSetEditor::validateLSystemAxiom);
	lSystemRule      .setFilter (&X3DIndexedLineSetEditor::validateLSystemRule);
}

void
X3DIndexedLineSetEditor::set_geometry ()
{
	// Hidden fields

	for (const auto & node : nodes)
		node -> getField <X3D::SFNode> ("options") .removeInterest (&X3DIndexedLineSetEditor::set_options, this);

	// Fields

	nodes = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::IndexedLineSet });

	#ifdef TITANIA_DEBUG
	getIndexedLineSetBox () .set_visible (not nodes .empty ());
	#else
	getIndexedLineSetBox () .set_visible (false);
	#endif

	// Hidden fields

	for (const auto & node : nodes)
		node -> getField <X3D::SFNode> ("options") .addInterest (&X3DIndexedLineSetEditor::set_options, this);

	set_options ();
}

void
X3DIndexedLineSetEditor::on_indexed_line_set_type_changed ()
{
	if (changing)
		return;

	const auto undoStep   = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Change IndexedLineSet Type To »%s«", getIndexedLineSetTypeButton () .get_active_text () .c_str ())));
	auto       optionNode = X3D::SFNode ();

	switch (getIndexedLineSetTypeButton () .get_active_row_number ())
	{
		case 1:
		{
			optionNode = X3D::MakePtr <X3D::LSystemOptions> (getCurrentContext ());
			break;
		}
		default:
		{
			break;
		}
	}

	for (const auto & node : nodes)
	{
		auto &     options = node -> getField <X3D::SFNode> ("options");
		const auto copy    = optionNode ? X3D::SFNode (optionNode -> copy (getCurrentContext (), X3D::FLAT_COPY)) : optionNode;

		X3D::X3DEditor::replaceNode (getCurrentContext (), node, options, copy, undoStep);
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DIndexedLineSetEditor::set_options ()
{
	// Set composed widgets.

	X3D::MFNode              optionsNodes;
	std::set <X3D::NodeType> types;

	for (const auto & node : nodes)
	{
		const auto & optionsNode = node -> getField <X3D::SFNode> ("options");

		if (optionsNode)
		{
			optionsNodes .emplace_back (optionsNode);
			types .emplace (optionsNode -> getType () .back ());
		}
	}

	const auto global       = optionsNodes .empty ();
	const auto inconsistent = not (optionsNodes .size () == nodes .size () and types .size () == 1);

	lSystemIterations .setNodes (optionsNodes);
	lSystemAngle      .setNodes (optionsNodes);
	lSystemConstants  .setNodes (optionsNodes);
	lSystemAxiom      .setNodes (optionsNodes);
	lSystemRule       .setNodes (optionsNodes);

	// Set global widget.

	changing = true;

	getIndexedLineSetLSystemOptions () .set_sensitive (not global and not inconsistent);
	getIndexedLineSetLSystemOptions () .set_visible (false);

	if (global)
	{
		getIndexedLineSetTypeButton () .set_active (0);
	}
	else if (inconsistent)
	{
		getIndexedLineSetTypeButton () .set_active (-1);
	}
	else
	{
		switch (optionsNodes .back () -> getType () .back ())
		{
			case X3D::X3DConstants::LSystemOptions:
			{
				getIndexedLineSetTypeButton ()     .set_active (1);
				getIndexedLineSetLSystemOptions () .set_visible (true);
				break;
			}
			default:
			{
				getIndexedLineSetTypeButton () .set_active (-1);
				break;
			}
		}
	}

	changing = false;
}

bool
X3DIndexedLineSetEditor::validateLSystemConstants (const std::string & text)
{
	static const std::regex constants (R"/([ A-Za-z0-9\[\]\+\-]+)/");

	return std::regex_match (text, constants);
}

bool
X3DIndexedLineSetEditor::validateLSystemAxiom (const std::string & text)
{
	static const std::regex constants (R"/([ A-Za-z0-9\[\]\+\-]+)/");

	return std::regex_match (text, constants);
}

bool
X3DIndexedLineSetEditor::validateLSystemRule (const std::string & text)
{
	static const std::regex constants (R"/([ A-Za-z0-9\[\]\+\-=]+)/");

	return std::regex_match (text, constants);
}

X3DIndexedLineSetEditor::~X3DIndexedLineSetEditor ()
{ }

} // puck
} // titania
