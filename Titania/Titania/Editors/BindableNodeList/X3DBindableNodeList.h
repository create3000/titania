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

#ifndef __TITANIA_WIDGETS_BINDABLE_NODE_LIST_X3DBINDABLE_NODE_LIST_H__
#define __TITANIA_WIDGETS_BINDABLE_NODE_LIST_X3DBINDABLE_NODE_LIST_H__

#include "../../UserInterfaces/X3DBindableNodeListInterface.h"

#include "../../Base/ScrollFreezer.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/X3DViewpointNode.h>
#include <Titania/X3D/Execution/BindableNodeStack.h>
#include <Titania/X3D/Execution/BindableNodeList.h>
#include <Titania/X3D/Parser/Filter.h>

namespace titania {
namespace puck {

template <class Type>
class X3DBindableNodeList :
	public X3DBindableNodeListInterface
{
public:

	///  @name Construction

	X3DBindableNodeList (X3DBrowserWindow* const browserWindow);

	///  @name Member acccess

	void
	setEditing (const bool value);

	bool
	getEditing () const
	{ return editing; }

	void
	setSelectNamedNode (const bool value);

	bool
	getSelectNamedNode () const
	{ return selectNamedNode; }

	const X3D::X3DPtr <Type> &
	getSelection () const
	{ return selection; }

	void
	setSelection (const X3D::X3DPtr <Type> & value, const bool event);

	///  @name Destruction

	virtual
	~X3DBindableNodeList () override;


private:

	///  @name Member types

	struct Columns
	{
		static constexpr int32_t INDEX       = 0;
		static constexpr int32_t NAME        = 1;
		static constexpr int32_t DESCRIPTION = 2;
		static constexpr int32_t TYPE_NAME   = 3;
		static constexpr int32_t WEIGHT      = 4;
		static constexpr int32_t STYLE       = 5;
		static constexpr int32_t BIND        = 6;
	};

	struct Weight
	{
		static const int32_t NORMAL;
		static const int32_t BOLD;
	};

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Member acccess

	const X3D::X3DPtr <X3D::X3DBindableNodeStack <Type>> &
	getStack (const X3D::X3DPtr <X3D::X3DLayerNode> &) const
	{ throw X3D::Error <X3D::NOT_SUPPORTED> ("Not supported"); }

	const X3D::X3DPtr <X3D::X3DBindableNodeList <Type>> &
	getList (const X3D::X3DPtr <X3D::X3DLayerNode> &) const
	{ throw X3D::Error <X3D::NOT_SUPPORTED> ("Not supported"); }

	std::string
	getDescription (const Type* const) const
	{ return ""; }

	///  @name Event handlers

	virtual
	void
	on_map () final override;

	virtual
	void
	on_unmap () final override;

	void
	set_live (const bool value);

	void
	set_browser (const X3D::BrowserPtr & browser);

	void
	set_activeLayer (const X3D::X3DPtr <X3D::X3DLayerNode> & layer);

	void
	set_list ();

	void
	set_stack ();

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) final override;

	virtual
	bool
	on_button_release_event (GdkEventButton* event) final override;

	void
	on_bind_toggled (const Gtk::TreePath & path);

	///  @name Static members

	static const std::string editorName;
	static const std::string description;

	///  @name Members

	X3D::BrowserPtr                 browser;
	X3D::X3DPtr <X3D::X3DLayerNode> activeLayer;
	X3D::X3DPtrArray <Type>         nodes;
	X3D::X3DPtr <Type>              selection;
	bool                            editing;
	bool                            selectNamedNode;

	std::unique_ptr <ScrollFreezer> scrollFreezer;

};

template <class Type>
const int32_t X3DBindableNodeList <Type>::Weight::NORMAL = 400;

template <class Type>
const int32_t X3DBindableNodeList <Type>::Weight::BOLD = 700;

template <class Type>
X3DBindableNodeList <Type>::X3DBindableNodeList (X3DBrowserWindow* const browserWindow) :
	            X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DBindableNodeListInterface (get_ui ("Editors/BindableNodeList.glade")),
	                     browser (getCurrentBrowser ()),
	                 activeLayer (),
	                       nodes (),
	                   selection (),
	                     editing (false),
	             selectNamedNode (false),
	               scrollFreezer (new ScrollFreezer (getTreeView ()))
{
	setName (editorName);

	addChildObjects (browser, activeLayer, nodes, selection);

	getHeaderBar () .set_title (_ (editorName));
	getLabel ()     .set_text (_ (editorName));

	setup ();
}

template <class Type>
void
X3DBindableNodeList <Type>::initialize ()
{
	X3DBindableNodeListInterface::initialize ();

	getImage () .set (Gtk::StockID (editorName), Gtk::IconSize (Gtk::ICON_SIZE_BUTTON));

	getNameCellRenderer ()        -> property_weight_set () = true;
	getDescriptionCellRenderer () -> property_weight_set () = true;
	getTypeNameCellRenderer ()    -> property_weight_set () = true;

	getNameCellRenderer ()        -> property_style_set () = true;
	getDescriptionCellRenderer () -> property_style_set () = true;
	getTypeNameCellRenderer ()    -> property_style_set () = true;
}

template <class Type>
void
X3DBindableNodeList <Type>::on_map ()
{
	getCurrentBrowser () .addInterest (&X3DBindableNodeList::set_browser, this);

	set_browser (getCurrentBrowser ());
}

template <class Type>
void
X3DBindableNodeList <Type>::on_unmap ()
{
	getCurrentBrowser () .removeInterest (&X3DBindableNodeList::set_browser, this);

	set_browser (nullptr);
}

template <class Type>
void
X3DBindableNodeList <Type>::setEditing (const bool value)
{
	editing = value;

	getTypeNameColumn () -> set_visible (editing);
	getNameColumn ()     -> set_visible (editing);
	getPadColumn ()      -> set_visible (editing);
	getBindColumn ()     -> set_visible (editing);
}

template <class Type>
void
X3DBindableNodeList <Type>::setSelectNamedNode (const bool value)
{
	selectNamedNode = value;
}

template <class Type>
void
X3DBindableNodeList <Type>::setSelection (const X3D::X3DPtr <Type> & value, const bool event)
{
	if (selection)
		selection -> isLive () .removeInterest (&X3DBindableNodeList::set_live, this);

	if (not editing or (activeLayer and (getList (activeLayer) -> getList () .empty () or value == getList (activeLayer) -> getList () [0])))
	{
		if (event)
			selection = nullptr;
		else
			selection .set (nullptr);
	}
	else
	{
		if (event)
			selection = value;
		else
			selection .set (value);
	}

	if (selection)
		selection -> isLive () .addInterest (&X3DBindableNodeList::set_live, this);

	if (editing)
		set_stack ();
}

template <class Type>
void
X3DBindableNodeList <Type>::set_live (const bool value)
{
	if (value)
		return;

	setSelection (nullptr, true);
}

template <class Type>
void
X3DBindableNodeList <Type>::set_browser (const X3D::BrowserPtr & value)
{
	if (browser)
		browser -> getActiveLayer () .removeInterest (&X3DBindableNodeList::set_activeLayer, this);

	browser = value;

	if (browser)
	{
		browser -> getActiveLayer () .addInterest (&X3DBindableNodeList::set_activeLayer, this);

		set_activeLayer (browser -> getActiveLayer ());
	}
	else
	   set_activeLayer (nullptr);
}

template <class Type>
void
X3DBindableNodeList <Type>::set_activeLayer (const X3D::X3DPtr <X3D::X3DLayerNode> & value)
{
	if (activeLayer)
	{
		getList  (activeLayer) -> removeInterest (&X3DBindableNodeList::set_list, this);
		getStack (activeLayer) -> removeInterest (&X3DBindableNodeList::set_stack, this);
	}

	activeLayer = value;

	if (activeLayer)
	{
		getList  (activeLayer) -> addInterest (&X3DBindableNodeList::set_list, this);
		getStack (activeLayer) -> addInterest (&X3DBindableNodeList::set_stack, this);

		set_list ();
		setSelection (X3D::X3DPtr <Type> (getStack (activeLayer) -> getTop ()), not selectNamedNode);
	}
	else
	{
		getListStore () -> clear ();
		setSelection (nullptr, not selectNamedNode);
	}
}

template <class Type>
void
X3DBindableNodeList <Type>::set_list ()
{
	try
	{
		if (editing)
		{
			for (const auto & node : nodes)
			   node -> name_changed () .removeInterest (&X3DBindableNodeList::set_stack, this);
		 
			nodes .clear ();
		}
	
		// Clear
	
		scrollFreezer -> freeze ();
	
		getTreeView () .unset_model ();
		getListStore () -> clear ();
	
		// Fill the TreeView's model
	
		if (activeLayer)
		{
			const auto & list  = getList (activeLayer);
			auto         names = std::set <std::string> ({ "" });
	
			for (size_t i = 0, size = list -> getList () .size (); i < size; ++ i)
			{
			   X3D::X3DPtr <Type> node (list -> getList () [i]);
	
				// Test if node should be displayed.

				if (not editing and getDescription (node) .empty ())
				   continue;

				const auto name = X3D::RemoveTrailingNumber (node -> getName ());

				if (selectNamedNode and names .count (name))
					continue;

				names .emplace (name);

				// Append row.

				getListStore () -> append () -> set_value (Columns::INDEX, i);
	
				if (editing)
				{
					node -> name_changed () .addInterest (&X3DBindableNodeList::set_stack, this);
					nodes .emplace_back (std::move (node));
				}
			}
		}
	
		getTreeView () .set_model (getListStore ());
		getTreeView () .set_search_column (Columns::DESCRIPTION);
	
		set_stack ();
	
		processInterests ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DBindableNodeList <Type>::set_stack ()
{
	try
	{
		// Update list store
	
		if (not activeLayer)
			return;
	
		const auto & list  = getList (activeLayer);
		auto         row   = getListStore () -> children () .begin ();
		auto         names = std::set <std::string> ({ "" });
	
		for (size_t i = 0, size = list -> getList () .size (); i < size; ++ i)
		{
		   const X3D::X3DPtr <Type> node (list -> getList () [i]);
	
			// Test if node should be displayed.

			if (not editing and getDescription (node) .empty ())
				continue;
	
			const auto name = X3D::RemoveTrailingNumber (node -> getName ());
	
			if (selectNamedNode and names .count (name))
				continue;

			names .emplace (name);

			// Fill row.
	
			row -> set_value (Columns::TYPE_NAME,   node -> getTypeName ());
			row -> set_value (Columns::NAME,        name);
			row -> set_value (Columns::DESCRIPTION, i ? getDescription (node) : description);
			row -> set_value (Columns::BIND,        node -> isBound () ? std::string ("Bound") : std::string ("Bind"));
	
			if ((editing and node == selection) or (not editing and node -> isBound ()))
			{
				row -> set_value (Columns::WEIGHT, Weight::BOLD);
				row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
			}
			else
			{
				row -> set_value (Columns::WEIGHT, Weight::NORMAL);
				row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
			}
	
			getListStore () -> row_changed (getListStore () -> get_path (row), row);
	
			++ row;
		}
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DBindableNodeList <Type>::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	try
	{
		if (not activeLayer)
			return;
	
		guint index = 0;
	
		getListStore () -> get_iter (path) -> get_value (Columns::INDEX, index);
	
		const X3D::X3DPtr <Type> node (getList (activeLayer) -> getList () .at (index));
	
		if (not editing)
		{
			const auto viewpoint = X3D::X3DPtr <X3D::X3DViewpointNode> (node);
	
			if (viewpoint)
				viewpoint -> setAnimate (true);
	
			if (node -> isBound ())
				node -> transitionStart (node);
	
			else
				node -> set_bind () = true;
		}
	
		setSelection (node, true);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
bool
X3DBindableNodeList <Type>::on_button_release_event (GdkEventButton* event)
{
	Gtk::TreePath        path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	getTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	const auto row = getListStore () -> get_iter (path);

	if (getListStore () -> iter_is_valid (row))
	{
		if (column == getBindColumn () .operator -> ())
		{
			on_bind_toggled (path);
			return true;
		}
	}

	return false;
}

template <class Type>
void
X3DBindableNodeList <Type>::on_bind_toggled (const Gtk::TreePath & path)
{
	try
	{
		if (not activeLayer)
			return;
	
		// Get Node
	
		guint index = 0;
	
		getListStore () -> get_iter (path) -> get_value (Columns::INDEX, index);
	
		const auto node      = getList (activeLayer) -> getList () .at (index);
		const auto viewpoint = X3D::X3DPtr <X3D::X3DViewpointNode> (node);
	
		if (viewpoint)
			viewpoint -> setAnimate (true);
	
		if (node -> isBound ())
			node -> transitionStart (node);
	
		else
			node -> set_bind () = true;
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
X3DBindableNodeList <Type>::~X3DBindableNodeList ()
{
	dispose ();
}

} // puck
} // titania

#endif
