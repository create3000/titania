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

#include "X3DShaderEditor.h"

#include <Titania/X3D/Prototype/ProtoDeclaration.h>

namespace titania {
namespace puck {

//X3DShaderEditor

const std::map <std::string, std::string> X3DShaderEditor::shaderTypes = {
	std::make_pair ("VERTEX",          "VertexShader"),
	std::make_pair ("TESS_CONTROL",    "TessControlShader"),
	std::make_pair ("TESS_EVALUATION", "TessEvaluationShader"),
	std::make_pair ("GEOMETRY",        "GeometryShader"),
	std::make_pair ("FRAGMENT",        "FragmentShader"),
	std::make_pair ("COMPUTE",         "gtk-execute"),
};

X3DShaderEditor::X3DShaderEditor () :
	     X3DScriptEditorInterface (),
	                         node ()
{
	addChildObjects (node);

	getShaderTypeMenuButton () .set_menu (getShaderTypeMenu ());
}

void
X3DShaderEditor::initialize ()
{ }

void
X3DShaderEditor::set_node (const X3D::SFNode & value)
{
	if (node)
	{
		if (node -> isType ({ X3D::X3DConstants::ShaderPart, X3D::X3DConstants::ShaderProgram }))
			node -> getField <X3D::SFString> ("type") .removeInterest (&X3DShaderEditor::set_type, this);
	}

	node = value;

	if (node)
	{
		if (node -> isType ({ X3D::X3DConstants::ShaderPart, X3D::X3DConstants::ShaderProgram }))
		{
			node -> getField <X3D::SFString> ("type") .addInterest (&X3DShaderEditor::set_type, this);
	
			set_type ();
	
			getShaderTypeMenuButton () .set_visible (true);
		}
		else
			getShaderTypeMenuButton () .set_visible (false);
	}
}

void
X3DShaderEditor::set_type ()
{
	try
	{
		getShaderTypeMenuButton () .set_stock_id (Gtk::StockID (shaderTypes .at (node -> getField <X3D::SFString> ("type"))));
	}
	catch (const std::exception &)
	{
		getShaderTypeMenuButton () .set_stock_id (Gtk::StockID ("VertexShader"));
	}
}

void
X3DShaderEditor::on_vertex_activate ()
{
	on_type_activate ("VERTEX");
}

void
X3DShaderEditor::on_tess_control_activate ()
{
	on_type_activate ("TESS_CONTROL");
}

void
X3DShaderEditor::on_tess_evaluate_activate ()
{
	on_type_activate ("TESS_EVALUATION");
}

void
X3DShaderEditor::on_geometry_activate ()
{
	on_type_activate ("GEOMETRY");
}

void
X3DShaderEditor::on_fragment_activate ()
{
	on_type_activate ("FRAGMENT");
}

void
X3DShaderEditor::on_compute_activate ()
{
	on_type_activate ("COMPUTE");
}

void
X3DShaderEditor::on_type_activate (const std::string & type)
{
	try
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Set Shader »type« To »%s«", type .c_str ())));
		const auto proto    = X3D::ProtoDeclarationPtr (node -> getExecutionContext ());
		auto &     field    = node -> getField <X3D::SFString> ("type");

		if (field == type)
			return;

		undoStep -> addObjects (node);

		if (proto)
			undoStep -> addUndoFunction (&X3D::ProtoDeclaration::requestUpdateInstances, proto);

		undoStep -> addUndoFunction (&X3D::SFString::setValue, std::ref (field), field);
		undoStep -> addRedoFunction (&X3D::SFString::setValue, std::ref (field), type);
		field = type;

		if (proto)
		{
			undoStep -> addRedoFunction (&X3D::ProtoDeclaration::requestUpdateInstances, proto);
			proto -> requestUpdateInstances ();
		}

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

} // puck
} // titania
