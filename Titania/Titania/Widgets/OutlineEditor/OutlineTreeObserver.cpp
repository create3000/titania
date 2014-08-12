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

#include "OutlineTreeObserver.h"

#include "OutlineTreeModel.h"
#include "X3DOutlineTreeView.h"

namespace titania {
namespace puck {

OutlineTreeObserver::OutlineTreeObserver (X3DOutlineTreeView* const treeView) :
	X3D::X3DInput (),
	     treeView (treeView)
{ }

void
OutlineTreeObserver::watch (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));

			if (treeView -> is_full_expanded (iter))
			{
				field -> getInputRoutes ()  .addInterest (this, &OutlineTreeObserver::toggle_field, path);
				field -> getOutputRoutes () .addInterest (this, &OutlineTreeObserver::toggle_field, path);
			}

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				case X3D::X3DConstants::MFNode:
				{
					field -> addInterest (this, &OutlineTreeObserver::update_path, path);
					break;
				}
				default:
					break;
			}

			break;
		}
		case OutlineIterType::X3DExecutionContext:
		{
			const auto & sfnode           = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   executionContext = dynamic_cast <X3D::X3DExecutionContext*> (sfnode .getValue ());

			executionContext -> getRootNodes ()          .addInterest (this, &OutlineTreeObserver::toggle_field, path);
			executionContext -> importedNodes_changed () .addInterest (this, &OutlineTreeObserver::toggle_field, path);
			executionContext -> prototypes_changed ()    .addInterest (this, &OutlineTreeObserver::toggle_field, path);
			executionContext -> externProtos_changed ()  .addInterest (this, &OutlineTreeObserver::toggle_field, path);

			const auto scene = dynamic_cast <X3D::Scene*> (executionContext);

			if (scene)
				scene -> exportedNodes_changed () .addInterest (this, &OutlineTreeObserver::toggle_field, path);

			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			const auto & sfnode     = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());

			if (inlineNode)
				inlineNode -> getInternalScene () .addInterest (this, &OutlineTreeObserver::update_path, path);

			break;
		}
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & sfnode      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());

			externProto -> getInternalScene () .addInterest (this, &OutlineTreeObserver::update_path, path);
			break;
		}
		default:
			break;
	}

	watch_children (iter);
}

void
OutlineTreeObserver::watch_children (const Gtk::TreeModel::iterator & parent)
{
	for (const auto & child : parent -> children ())
		watch_child (child, treeView -> get_model () -> get_path (child));
}

void
OutlineTreeObserver::watch_child (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::Separator:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::X3DExecutionContext:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
		case OutlineIterType::NULL_:
			break;

		case OutlineIterType::X3DFieldValue:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));

			field -> addInterest (this, &OutlineTreeObserver::on_row_changed, path);
			break;
		}
		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));

			field -> getInputRoutes ()  .addInterest (this, &OutlineTreeObserver::on_row_changed, path);
			field -> getOutputRoutes () .addInterest (this, &OutlineTreeObserver::on_row_changed, path);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::MFNode:
				{
					field -> getInputRoutes ()  .addInterest (this, &OutlineTreeObserver::on_row_has_child_toggled, path, false);
					field -> getOutputRoutes () .addInterest (this, &OutlineTreeObserver::on_row_has_child_toggled, path, false);

					field -> addInterest (this, &OutlineTreeObserver::on_row_has_child_toggled, path, true);
					break;
				}
				default:
					break;
			}

			break;
		}
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::X3DBaseNode:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

			if (sfnode)
				sfnode -> name_changed () .addInterest (this, &OutlineTreeObserver::on_row_changed, path);

			break;
		}
	}
}

void
OutlineTreeObserver::unwatch_tree (const Gtk::TreeModel::iterator & iter, const bool root)
{
	unwatch_child (iter, root);

	for (const auto & child : iter -> children ())
		unwatch_tree (child, false);
}

void
OutlineTreeObserver::unwatch_child (const Gtk::TreeModel::iterator & iter, const bool root)
{
	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::Separator:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::NULL_:
			break;

		case OutlineIterType::X3DFieldValue:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));

			field -> removeInterest (this, &OutlineTreeObserver::on_row_changed);
			break;
		}
		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));

			if (not root)
			{
				field -> getInputRoutes ()  .removeInterest (this, &OutlineTreeObserver::on_row_changed);
				field -> getOutputRoutes () .removeInterest (this, &OutlineTreeObserver::on_row_changed);
			}

			field -> getInputRoutes ()  .removeInterest (this, &OutlineTreeObserver::toggle_field);
			field -> getOutputRoutes () .removeInterest (this, &OutlineTreeObserver::toggle_field);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					field -> removeInterest (this, &OutlineTreeObserver::update_path);
					break;
				}
				case X3D::X3DConstants::MFNode:
				{
					field -> removeInterest (this, &OutlineTreeObserver::update_path);

					if (not root)
					{
						field -> getInputRoutes ()  .removeInterest (this, &OutlineTreeObserver::on_row_has_child_toggled);
						field -> getOutputRoutes () .removeInterest (this, &OutlineTreeObserver::on_row_has_child_toggled);

						field -> removeInterest (this, &OutlineTreeObserver::on_row_has_child_toggled);
					}

					break;
				}
				default:
					break;
			}

			break;
		}
		case OutlineIterType::X3DExecutionContext:
		{
			const auto & sfnode           = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   executionContext = dynamic_cast <X3D::X3DExecutionContext*> (sfnode .getValue ());

			executionContext -> getRootNodes ()          .removeInterest (this, &OutlineTreeObserver::toggle_field);
			executionContext -> importedNodes_changed () .removeInterest (this, &OutlineTreeObserver::toggle_field);
			executionContext -> prototypes_changed ()    .removeInterest (this, &OutlineTreeObserver::toggle_field);
			executionContext -> externProtos_changed ()  .removeInterest (this, &OutlineTreeObserver::toggle_field);

			const auto scene = dynamic_cast <X3D::Scene*> (executionContext);

			if (scene)
				scene -> exportedNodes_changed () .removeInterest (this, &OutlineTreeObserver::toggle_field);

			break;
		}
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & sfnode      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());

			externProto -> name_changed ()     .removeInterest (this, &OutlineTreeObserver::update_path);
			externProto -> getInternalScene () .removeInterest (this, &OutlineTreeObserver::update_path);

			clear_open_path (iter);
			break;
		}
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

			sfnode -> name_changed () .removeInterest (this, &OutlineTreeObserver::on_row_changed);
			clear_open_path (iter);
			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			const auto & sfnode     = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());

			if (sfnode)
				sfnode -> name_changed () .removeInterest (this, &OutlineTreeObserver::on_row_changed);

			if (inlineNode)
				inlineNode -> getInternalScene () .removeInterest (this, &OutlineTreeObserver::update_path);

			clear_open_path (iter);
			break;
		}
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
		{
			clear_open_path (iter);
			break;
		}
	}
}

void
OutlineTreeObserver::clear_open_path (const Gtk::TreeModel::iterator & iter)
{
	// Clear open_path.

	const auto open_path = treeView -> get_open_path (iter);

	if (open_path .size () and open_path == treeView -> get_model () -> get_path (iter))
		treeView -> set_open_path (iter, Gtk::TreeModel::Path ());
}

void
OutlineTreeObserver::on_row_has_child_toggled (const Gtk::TreeModel::Path & path, const bool expand)
{
	treeView -> get_model () -> row_has_child_toggled (path, treeView -> get_model () -> get_iter (path));

	if (expand)
		treeView -> expand_row (path, false);
}

namespace {

void
on_row_changed_impl (const Glib::RefPtr <Gtk::TreeModel> & model, const Gtk::TreeModel::Path & path)
{
	model -> row_changed (path, model -> get_iter (path));
}

} // namespace

void
OutlineTreeObserver::on_row_changed (const Gtk::TreeModel::Path & path)
{
	//__LOG__ << X3D::SFTime (chrono::now ()) << std::endl;

	//treeView -> get_model () -> row_changed (path, treeView -> get_model () -> get_iter (path));

	Glib::signal_idle () .connect_once (sigc::bind (sigc::ptr_fun (&on_row_changed_impl), treeView -> get_model (), path));
}

void
OutlineTreeObserver::update_path (const Gtk::TreeModel::Path & path)
{
	//__LOG__ << path .to_string () << std::endl;

	const Gtk::TreeModel::iterator iter = treeView -> get_model () -> get_iter (path);

	treeView -> update_row (iter, path);
}

void
OutlineTreeObserver::toggle_field (const Gtk::TreeModel::Path & path)
{
	Gtk::TreeModel::iterator iter = treeView -> get_model () -> get_iter (path);

	treeView -> collapse_row (path);
	treeView -> get_model () -> row_has_child_toggled (path, iter);

	treeView -> disable_shift_key ();

	treeView -> is_full_expanded (iter, true);
	treeView -> expand_row (path, false);

	treeView -> enable_shift_key ();
}

OutlineTreeObserver::~OutlineTreeObserver ()
{ }

} // puck
} // titania
