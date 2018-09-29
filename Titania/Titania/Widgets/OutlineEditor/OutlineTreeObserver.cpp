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

#include "OutlineTreeObserver.h"

#include "OutlineTreeModel.h"
#include "X3DOutlineTreeView.h"

#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>

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

			if (treeView -> get_expanded (iter) == OUTLINE_EXPANDED_FULL)
			{
				field -> getInputRoutes ()  .addInterest (&OutlineTreeObserver::toggle_path, this, path);
				field -> getOutputRoutes () .addInterest (&OutlineTreeObserver::toggle_path, this, path);
			}

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				case X3D::X3DConstants::MFNode:
				{
					field -> addInterest (&OutlineTreeObserver::toggle_path, this, path);
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

			executionContext -> getRootNodes ()          .addInterest (&OutlineTreeObserver::toggle_path, this, path);
			executionContext -> importedNodes_changed () .addInterest (&OutlineTreeObserver::toggle_path, this, path);
			executionContext -> prototypes_changed ()    .addInterest (&OutlineTreeObserver::toggle_path, this, path);
			executionContext -> externProtos_changed ()  .addInterest (&OutlineTreeObserver::toggle_path, this, path);

			const auto scene = dynamic_cast <X3D::X3DScene*> (executionContext);

			if (scene)
				scene -> exportedNodes_changed () .addInterest (&OutlineTreeObserver::toggle_path, this, path);

			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			const auto & sfnode     = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());
			const auto   urlObject  = dynamic_cast <X3D::X3DUrlObject*> (sfnode .getValue ());

			if (sfnode)
				sfnode -> fields_changed () .addInterest (&OutlineTreeObserver::toggle_path, this, path);

			if (inlineNode)
				inlineNode -> getInternalScene () .addInterest (&OutlineTreeObserver::toggle_path, this, path);

			else if (urlObject)
				urlObject -> checkLoadState () .addInterest (&OutlineTreeObserver::toggle_path, this, path);

			break;
		}
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & sfnode      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());

			externProto -> fields_changed ()   .addInterest (&OutlineTreeObserver::toggle_path, this, path);
			externProto -> getInternalScene () .addInterest (&OutlineTreeObserver::toggle_path, this, path);
			break;
		}
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

			sfnode -> fields_changed () .addInterest (&OutlineTreeObserver::toggle_path, this, path);
			break;
		}
		case OutlineIterType::ImportedNode:
		{
			const auto & sfnode       = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   importedNode = dynamic_cast <X3D::ImportedNode*> (sfnode .getValue ());
			const auto   inlineNode   = importedNode -> getInlineNode ();

			inlineNode -> getInternalScene () .addInterest (&OutlineTreeObserver::toggle_path, this, path);
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

			field -> addInterest (&OutlineTreeObserver::on_row_changed, this, path);
			break;
		}
		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));

			field -> getInputRoutes ()  .addInterest (&OutlineTreeObserver::on_row_changed, this, path);
			field -> getOutputRoutes () .addInterest (&OutlineTreeObserver::on_row_changed, this, path);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::MFNode:
				{
					field -> getInputRoutes ()  .addInterest (&OutlineTreeObserver::on_row_has_child_toggled, this, path, false);
					field -> getOutputRoutes () .addInterest (&OutlineTreeObserver::on_row_has_child_toggled, this, path, false);

					field -> addInterest (&OutlineTreeObserver::on_row_has_child_toggled, this, path, true);
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
			const auto & sfnode        = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   protoInstance = dynamic_cast <X3D::X3DPrototypeInstance*> (sfnode .getValue ());

			if (sfnode)
				sfnode -> name_changed () .addInterest (&OutlineTreeObserver::on_row_changed, this, path);

			if (protoInstance)
				protoInstance -> typeName_changed () .addInterest (&OutlineTreeObserver::on_row_changed, this, path);

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

			field -> removeInterest (&OutlineTreeObserver::on_row_changed, this);
			break;
		}
		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));

			if (not root)
			{
				field -> getInputRoutes ()  .removeInterest (&OutlineTreeObserver::on_row_changed, this);
				field -> getOutputRoutes () .removeInterest (&OutlineTreeObserver::on_row_changed, this);
			}

			field -> getInputRoutes ()  .removeInterest (&OutlineTreeObserver::toggle_path, this);
			field -> getOutputRoutes () .removeInterest (&OutlineTreeObserver::toggle_path, this);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					field -> removeInterest (&OutlineTreeObserver::toggle_path, this);
					break;
				}
				case X3D::X3DConstants::MFNode:
				{
					field -> removeInterest (&OutlineTreeObserver::toggle_path, this);

					if (not root)
					{
						field -> getInputRoutes ()  .removeInterest (&OutlineTreeObserver::on_row_has_child_toggled, this);
						field -> getOutputRoutes () .removeInterest (&OutlineTreeObserver::on_row_has_child_toggled, this);

						field -> removeInterest (&OutlineTreeObserver::on_row_has_child_toggled, this);
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

			executionContext -> getRootNodes ()          .removeInterest (&OutlineTreeObserver::toggle_path, this);
			executionContext -> importedNodes_changed () .removeInterest (&OutlineTreeObserver::toggle_path, this);
			executionContext -> prototypes_changed ()    .removeInterest (&OutlineTreeObserver::toggle_path, this);
			executionContext -> externProtos_changed ()  .removeInterest (&OutlineTreeObserver::toggle_path, this);

			const auto scene = dynamic_cast <X3D::X3DScene*> (executionContext);

			if (scene)
				scene -> exportedNodes_changed () .removeInterest (&OutlineTreeObserver::toggle_path, this);

			break;
		}
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & sfnode      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());

			externProto -> name_changed ()     .removeInterest (&OutlineTreeObserver::toggle_path, this);
			externProto -> fields_changed ()   .removeInterest (&OutlineTreeObserver::toggle_path, this);
			externProto -> getInternalScene () .removeInterest (&OutlineTreeObserver::toggle_path, this);

			clear_open_path (iter);
			break;
		}
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

			sfnode -> name_changed ()   .removeInterest (&OutlineTreeObserver::on_row_changed, this);
			sfnode -> fields_changed () .removeInterest (&OutlineTreeObserver::toggle_path, this);

			clear_open_path (iter);
			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			const auto & sfnode        = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   protoInstance = dynamic_cast <X3D::X3DPrototypeInstance*> (sfnode .getValue ());
			const auto   inlineNode    = dynamic_cast <X3D::Inline*> (sfnode .getValue ());
			const auto   urlObject     = dynamic_cast <X3D::X3DUrlObject*> (sfnode .getValue ());

			if (sfnode)
			{
				if (not root)
				{
					const auto userData = treeView -> get_user_data (iter);

					if (userData -> paths .size () == 1)
						sfnode -> name_changed () .removeInterest (&OutlineTreeObserver::on_row_changed, this);
				}

				sfnode -> fields_changed () .removeInterest (&OutlineTreeObserver::toggle_path, this);
			}

			if (protoInstance)
				protoInstance -> typeName_changed () .removeInterest (&OutlineTreeObserver::on_row_changed, this);

			if (inlineNode)
				inlineNode -> getInternalScene () .removeInterest (&OutlineTreeObserver::toggle_path, this);

			else if (urlObject)
				urlObject -> checkLoadState () .removeInterest (&OutlineTreeObserver::toggle_path, this);

			clear_open_path (iter);
			break;
		}
		case OutlineIterType::ImportedNode:
		{
			const auto & sfnode       = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   importedNode = dynamic_cast <X3D::ImportedNode*> (sfnode .getValue ());
			const auto   inlineNode   = importedNode -> getInlineNode ();

			inlineNode -> getInternalScene () .removeInterest (&OutlineTreeObserver::toggle_path, this);
			clear_open_path (iter);
			break;
		}
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

void
OutlineTreeObserver::on_row_changed (const Gtk::TreeModel::Path & path)
{
	//__LOG__ << X3D::SFTime (X3D::SFTime::now ()) << " : " << path .to_string () << std::endl;

	treeView -> get_model () -> row_changed (path, treeView -> get_model () -> get_iter (path));

	//Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &OutlineTreeObserver::on_row_changed_impl), path), Glib::PRIORITY_HIGH_IDLE);
}

//void
//OutlineTreeObserver::on_row_changed_impl (const Gtk::TreeModel::Path & path)
//{
//	treeView -> get_model () -> row_changed (path, treeView -> get_model () -> get_iter (path));
//}

void
OutlineTreeObserver::toggle_path (const Gtk::TreeModel::Path & path)
{
	//__LOG__ << X3D::SFTime (X3D::SFTime::now ()) << std::endl;

	if (not treeView -> row_expanded (path))
		return;

	// Determine open paths.

	std::map <size_t,  std::tuple <size_t, bool, size_t>> opened;

	treeView -> get_opened_objects (treeView -> get_model () -> get_iter (path), opened);

	// Collapse path.

	treeView -> collapse_row (path);

	// Expand.

	treeView -> disable_shift_key ();

	treeView -> reopen_objects (treeView -> get_model () -> get_iter (path), opened);

	treeView -> enable_shift_key ();
}

//bool
//OutlineTreeObserver::getToggle (const Gtk::TreeModel::iterator & iter) const
//{
//	if (treeView -> get_data_type (iter) == OutlineIterType::X3DField)
//	{
//		const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));
//
//		switch (field -> getType ())
//		{
//			case X3D::X3DConstants::SFNode:
//			{
//				const auto & sfnode   = *static_cast <X3D::SFNode*> (field);
//				const auto   children = iter -> children ();
//
//				if (children .empty ())
//					return true;
//
//				if (treeView -> get_data_type (children [0]) == OutlineIterType::NULL_)
//				{
//					if (sfnode)
//						return true;
//				}
//
//				const auto rhs = static_cast <X3D::SFNode*> (treeView -> get_object (children [0]));
//
//				return sfnode != *rhs;
//			}
//			case X3D::X3DConstants::MFNode:
//			{
//				const auto & mfnode   = *static_cast <X3D::MFNode*> (field);
//				const auto   children = iter -> children ();
//
//				if (mfnode .size () != children .size ())
//					return true;
//
//				for (size_t i = 0; i < mfnode .size (); ++ i)
//				{
//					if (treeView -> get_data_type (children [i]) == OutlineIterType::NULL_)
//					{
//						if (mfnode [i])
//							return true;
//					}
//
//					const auto rhs = static_cast <X3D::SFNode*> (treeView -> get_object (children [i]));
//
//					if (mfnode [i] != *rhs)
//						return true;
//				}
//
//				return false;
//			}
//			default:
//				return true;
//		}
//	}
//
//	return true;
//}

OutlineTreeObserver::~OutlineTreeObserver ()
{ }

} // puck
} // titania
