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

#ifndef __TITANIA_BROWSER_X3DBROWSER_EDITOR_H__
#define __TITANIA_BROWSER_X3DBROWSER_EDITOR_H__

#include "../Browser/X3DBrowserWidget.h"

#include <Titania/X3D/Components/Rendering/X3DCoordinateNode.h>
#include <Titania/X3D/Components/Rendering/X3DGeometryNode.h>
#include <Titania/X3D/Editing/Undo/UndoHistory.h>

namespace titania {
namespace puck {

class BrowserSelection;
class MagicImport;

class X3DBrowserEditor :
	public X3DBrowserWidget
{
public:

	using X3DBrowserWidget::save;

	/// @name Member access

	virtual
	void
	setCurrentContext (const X3D::X3DExecutionContextPtr &) final override;

	const X3D::SFBool &
	getEditing () const
	{ return editing; }

	virtual
	void
	setEditing (const bool value);

	/// @name Operations

	void
	blank ();

	virtual
	void
	open (const basic::uri & URL) final override;

	virtual
	void
	load (const basic::uri & URL) final override;

	X3D::MFNode
	import (const std::vector <basic::uri> & url, const X3D::UndoStepPtr & undoStep);

	X3D::MFNode
	importAsInline (const std::vector <basic::uri> & url, const X3D::UndoStepPtr & undoStep);

	virtual
	bool
	save (const basic::uri & worldURL, const std::string & outputStyle, const bool copy) override;

	virtual
	void
	reload () final override;

	virtual
	void
	close (const NotebookPagePtr page) final override;

	virtual
	bool
	quit () final override;

	/// @name Undo/Redo operations

	virtual
	void
	addUndoStep (const X3D::UndoStepPtr & undoStep) final override;

	virtual
	void
	removeUndoStep () final override;

	virtual
	const X3D::UndoStepPtr &
	getUndoStep () const final override;

	void
	undo ();

	void
	redo ();

	/// @name Clipboard handling

	const X3D::ClipboardPtr &
	getClipboard () const
	{ return clipboard; }

	void
	cutNodes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep);

	void
	copyNodes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes);

	void
	pasteNodes (const X3D::X3DExecutionContextPtr & executionContext, X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep);

	/// @name Edit operations

	X3D::SFNode
	createNode (const std::string & typeName, const X3D::UndoStepPtr & undoStep);

	void
	removeNodesFromScene (const X3D::X3DExecutionContextPtr & executionContext,
	                      const X3D::MFNode & nodes,
	                      const bool,
	                      const X3D::UndoStepPtr & undoStep) const;
	
	/// @name CDATA field operations

	void
	editSourceCode (const X3D::SFNode &);

	/// @name Selection operations

	const std::unique_ptr <BrowserSelection> &
	getSelection () const
	{ return selection; }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBrowserEditor ();


protected:

	friend class X3DBrowserSelection;

	///  @name Member types

	enum ToolType
	{
		NONE_TOOL,
		NUDGE_LEFT,
		NUDGE_RIGHT,
		NUDGE_UP,
		NUDGE_DOWN,
		NUDGE_FRONT,
		NUDGE_BACK
	};

	/// @name Construction

	X3DBrowserEditor (const X3D::BrowserPtr & defaultBrowser);

	virtual
	void
	initialize () override;

	virtual
	void
	configure () override;

	virtual
	void
	set_selection (const X3D::MFNode & selection) override;

	virtual
	void
	setPage (const NotebookPagePtr & value) override;

	void
	setViewer (const X3D::X3DConstants::NodeType viewerType);

	/// @name Undo

	void
	set_undoHistory ();

	/// @name Operations

	void
	translateSelection (const X3D::Vector3f & offset, const bool alongFrontPlane, const ToolType currentTool);


private:

	///  @name Event handlers

	void
	set_executionContext ();

	///  @name File handling

	void
	setMetaData ();

	void
	getMetaData ();

	///  @name Undo handling

	void
	set_tool_undo (const X3D::UndoStepContainerPtr & undoContainer);

	///  @name Clipboard handling

	void
	set_clipboard (const X3D::SFString & string);

	///  @name Source code handling

	void
	on_source_code_changed (const Glib::RefPtr <Gio::File> & file,
	                        const Glib::RefPtr <Gio::File> &,
	                        Gio::FileMonitorEvent event,
	                        const X3D::SFNode & node);

	///  @name Members

	X3D::X3DExecutionContextPtr        executionContext;
	X3D::SFBool                        editing;
	X3D::ClipboardPtr                  clipboard;
	std::unique_ptr <BrowserSelection> selection;
	X3D::UndoStepPtr                   nudgeUndoStep;
	double                             undoTime;
	ToolType                           tool;

};

} // puck
} // titania

#endif
