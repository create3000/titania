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

#ifndef __TITANIA_WIDGETS_SCRIPT_EDITOR_SCRIPT_EDITOR_H__
#define __TITANIA_WIDGETS_SCRIPT_EDITOR_SCRIPT_EDITOR_H__

#include "../../ComposedWidgets.h"

#include "X3DScriptEditor.h"
#include "X3DScriptNodeEditor.h"
#include "X3DScriptEditorPreferences.h"
#include "X3DScriptEditorSearch.h"
#include "X3DShaderEditor.h"

namespace titania {
namespace puck {

class BrowserWindow;
class NodeIndex;
class NotebookPage;
class Console;

class ScriptEditor :
	virtual public X3DScriptEditor,
	public X3DScriptNodeEditor,
	public X3DShaderEditor,
	public X3DScriptEditorSearch,
	public X3DScriptEditorPreferences
{
public:

	///  @name Construction

	ScriptEditor (X3DBrowserWindow* const browserWindow);

	/// @name Member access

	bool
	getModified () const;

	///  @name Event handlers

	void
	apply (const X3D::UndoStepPtr &);

	///  @name Destruction

	virtual
	~ScriptEditor () final override;


protected:

	virtual
	void
	set_node (const X3D::SFNode &) final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	void
	restore ();

	///  @name Member access

	void
	setModified (const bool value)
	{ modified = value; }

	///  @name Event handlers

	void
	set_executionContext ();

	void
	set_live ();

	bool
	on_focus_in_event (GdkEventFocus*);

	bool
	on_focus_out_event (GdkEventFocus*);

	virtual
	void
	on_new_clicked () final override;

	virtual
	void
	on_apply_clicked () final override;

	virtual
	void
	on_undo_clicked () final override;

	void
	on_can_undo_changed ();

	void
	on_can_redo_changed ();

	virtual
	void
	on_redo_clicked () final override;

	void
	set_sourceText ();

	void
	connectSourceText (const X3D::MFString &);

	void
	set_loadState (const X3D::LoadState);

	void
	save ();

	X3D::SFNode
	getNodeFromPath (const std::string & nodePath);

	std::string
	getPathFromNode (const X3D::SFNode & node) const;

	virtual
	void
	store () final override;

	///  @name Members

	NotebookPage*                                page;
	bool                                         modified;
	std::unique_ptr <NodeIndex>                  nodeIndex;
	std::unique_ptr <Console>                    console;
	NameEntry                                    nodeName;
	const std::set <X3D::X3DConstants::NodeType> nodeTypes;
	X3D::SFNode                                  node;
	size_t                                       index;

};

} // puck
} // titania

#endif
