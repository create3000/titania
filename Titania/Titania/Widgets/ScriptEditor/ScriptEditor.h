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

#ifndef __TITANIA_WIDGETS_SCRIPT_EDITOR_SCRIPT_EDITOR_H__
#define __TITANIA_WIDGETS_SCRIPT_EDITOR_SCRIPT_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DScriptEditorInterface.h"
#include "X3DScriptEditor.h"
#include "X3DShaderPartEditor.h"

#include <gtksourceviewmm/buffer.h>
#include <gtksourceviewmm/view.h>

namespace titania {
namespace puck {

class BrowserWindow;
class NodeIndex;
class Console;

class ScriptEditor :
	virtual public X3DScriptEditorInterface,
	public X3DScriptEditor,
	public X3DShaderPartEditor
{
public:

	///  @name Construction

	ScriptEditor (X3DBrowserWindow* const);

	/// @name Member access

	bool
	isModified () const;

	const Glib::RefPtr <Gsv::Buffer> &
	getTextBuffer () const
	{ return textBuffer; }

	Gsv::View &
	getTextView ()
	{ return textView; }

	const Gsv::View &
	getTextView () const
	{ return textView; }

	///  @name Event handlers

	void
	apply (const UndoStepPtr &);

	///  @name Destruction

	virtual
	~ScriptEditor ();


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Member access

	void
	isModified (const bool value)
	{ modified = value; }

	///  @name Event handlers

	virtual
	void
	on_map () final override;

	void
	set_label ();

	void
	set_node (const X3D::SFNode &);

	bool
	on_focus_in_event (GdkEventFocus*);

	bool
	on_focus_out_event (GdkEventFocus*);

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
	set_cdata ();

	void
	connectCDATA (const X3D::MFString &);

	void
	on_loadState_clicked ();

	void
	set_loadState (const X3D::LoadState);

	///  @name Search and Replace

	virtual
	bool
	on_search_focus_in_event (GdkEventFocus*) final override;

	virtual
	bool
	on_search_focus_out_event (GdkEventFocus*) final override;

	bool
	on_key_press_event (GdkEventKey*);

	bool
	on_key_release_event (GdkEventKey*);

	virtual
	void
	on_search_entry_changed () final override;

	static
	void
	on_occurences_changed (GObject*, GParamSpec*, gpointer);

	virtual
	void
	on_replace_toggled () final override;

	virtual
	void
	on_search_backward_clicked () final override;

	static
	void
	on_search_backward_callback (GObject* const, GAsyncResult* const, const gpointer);

	void
	on_search_backward (GAsyncResult* const);

	virtual
	void
	on_search_forward_clicked () final override;

	static
	void
	on_search_forward_callback (GObject* const, GAsyncResult* const, const gpointer);

	void
	on_search_forward (GAsyncResult* const);

	virtual
	void
	on_hide_search_clicked () final override;

	///  @name Members

	bool                        modified;
	Glib::RefPtr <Gsv::Buffer>  textBuffer;
	Gsv::View                   textView;
	GtkSourceSearchSettings*    searchSettings;
	GtkSourceSearchContext*     searchContext;
	std::unique_ptr <NodeIndex> nodeIndex;
	NameEntry                   nodeName;
	X3D::SFNode                 node;
	size_t                      index;
	std::unique_ptr <Console>   console;
	X3D::Keys                   keys;

};

} // puck
} // titania

#endif
