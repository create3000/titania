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

#ifndef __TITANIA_DIALOGS_NODE_INDEX_NODE_INDEX_H__
#define __TITANIA_DIALOGS_NODE_INDEX_NODE_INDEX_H__

#include "../../UserInterfaces/X3DNodeIndexInterface.h"

namespace titania {
namespace puck {

class NodeIndex :
	virtual public X3DNodeIndexInterface
{
public:

	///  @name Construction

	NodeIndex (BrowserWindow* const);

	///  @name Operations

	void
	setNamedNodes ();

	void
	setTypes (const std::set <X3D::X3DConstants::NodeType> & value);

	const X3D::SFNode &
	getNode () const
	{ return node; }

	///  @name Destruction

	virtual
	~NodeIndex ();


private:

	enum IndexType
	{
		NAMED_NODES_INDEX,
		TYPE_INDEX

	};

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	refresh ();

	///  @name Operations

	void
	setNodes (X3D::MFNode &&);

	X3D::MFNode
	getNodes (const std::set <X3D::X3DConstants::NodeType> &);

	X3D::MFNode
	getNodes ();

	std::set <X3D::SFNode>
	getImportingInlines () const;

	std::set <X3D::SFNode>
	getExportedNodes () const;

	///  @name Event handlers

	void
	set_executionContext ();

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) final override;

	static
	void
	set_adjustment (const Glib::RefPtr <Gtk::Adjustment> &, const double);

	///  @name Members

	X3D::X3DExecutionContextPtr            executionContext;
	IndexType                              index;
	std::set <X3D::X3DConstants::NodeType> types;
	X3D::MFNode                            nodes;
	X3D::SFNode                            node;

};

} // puck
} // titania

#endif
