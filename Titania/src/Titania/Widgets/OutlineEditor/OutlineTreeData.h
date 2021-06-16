/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_TREE_DATA_H__
#define __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_TREE_DATA_H__

#include "../../Browser/UserData.h"

#include <Titania/X3D.h>
#include <gtkmm.h>

namespace titania {
namespace puck {

enum class OutlineIterType
{
	Separator,
	X3DInputRoute,
	X3DOutputRoute,
	X3DFieldValue,
	X3DField,
	X3DExecutionContext,
	X3DBaseNode,
	NULL_,
	ExternProtoDeclaration,
	ProtoDeclaration,
	ImportedNode,
	ExportedNode

};

using OutlineRoutes = std::set <std::pair <Gtk::TreeModel::Path, Gtk::TreeModel::Path>>;

class OutlineTreeData :
	public Glib::Object
{
public:

	///  @name Construction

	OutlineTreeData (const OutlineIterType type_, X3D::X3DChildObject* const object_, const Gtk::TreeModel::Path & path_, const size_t index, const bool selected);

	///  @name Comparision operations

	bool
	is (X3D::X3DChildObject* const value) const;

	///  @name Member access

	X3D::X3DChildObject*
	get_object () const
	{ return object; }

	OutlineIterType
	get_type () const
	{ return type; }

	const Gtk::TreeModel::Path &
	get_path () const
	{ return path; }

	size_t
	get_index () const
	{ return index; }

	UserDataPtr
	get_user_data () const;

	// Selection handling

	void
	setSelected (const bool value)
	{ selected = value; }

	bool
	getSelected () const
	{ return selected; }

	///  @name Route handling

	OutlineRoutes &
	get_inputs_below ()
	{ return inputs_below; }

	OutlineRoutes &
	get_inputs_above ()
	{ return inputs_above; }

	OutlineRoutes &
	get_outputs_below ()
	{ return outputs_below; }

	OutlineRoutes &
	get_outputs_above ()
	{ return outputs_above; }

	bool &
	get_self_connection ()
	{ return self_connection; }

	OutlineRoutes &
	get_connections ()
	{ return connections; }

	///  @name Destruction

	~OutlineTreeData ();


private:

	OutlineTreeData (const OutlineTreeData &) = delete;

	OutlineTreeData (OutlineTreeData &&) = delete;

	OutlineTreeData &
	operator = (const OutlineTreeData &) = delete;

	OutlineTreeData &
	operator = (OutlineTreeData &&) = delete;

	///  @name Members

	X3D::SFNode                parent;
	X3D::X3DChildObject*       object;
	const OutlineIterType      type;
	const Gtk::TreeModel::Path path;
	const size_t               index;

	bool selected;

	OutlineRoutes inputs_below;
	OutlineRoutes inputs_above;
	OutlineRoutes outputs_below;
	OutlineRoutes outputs_above;
	bool          self_connection;
	OutlineRoutes connections;

};

} // puck
} // titania

#endif
