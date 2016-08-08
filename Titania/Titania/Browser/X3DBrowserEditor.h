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

#ifndef __TITANIA_BROWSER_X3DBROWSER_EDITOR_H__
#define __TITANIA_BROWSER_X3DBROWSER_EDITOR_H__

#include "../Browser/BrowserSelection.h"
#include "../Browser/X3DBrowserWidget.h"

#include <Titania/X3D/Components/Rendering/X3DCoordinateNode.h>
#include <Titania/X3D/Components/Rendering/X3DGeometryNode.h>
#include <Titania/X3D/Editing/Undo/UndoHistory.h>
#include <Titania/X3D/Editing/X3DEditor.h>

namespace titania {
namespace puck {

class MagicImport;

class X3DBrowserEditor :
	public X3DBrowserWidget,
	public X3D::X3DEditor
{
public:

	using X3DBrowserWidget::save;

	/// @name Member access

	virtual
	void
	setCurrentContext (const X3D::X3DExecutionContextPtr &) final override;

	const X3D::SFBool &
	isEditor () const
	{ return enabled; }

	virtual
	void
	isEditor (const bool);

	void
	isModified (const X3D::BrowserPtr &, const bool);

	virtual
	bool
	isModified (const X3D::BrowserPtr &) const final override;

	/// @name Operations

	void
	blank ();

	virtual
	void
	open (const basic::uri &) final override;

	virtual
	void
	load (const X3D::BrowserPtr &, const basic::uri &) final override;

	X3D::MFNode
	import (const std::vector <basic::uri> &, const X3D::UndoStepPtr & undoStep);

	X3D::MFNode
	importAsInline (const std::vector <basic::uri> &, const X3D::UndoStepPtr & undoStep);

	virtual
	bool
	save (const basic::uri &, const bool, const bool) override;

	virtual
	void
	reload () final override;

	virtual
	void
	close (const X3D::BrowserPtr &) final override;

	virtual
	bool
	quit () final override;

	/// @name Undo/Redo operations

	X3D::UndoHistory &
	getUndoHistory (const X3D::BrowserPtr &);

	const X3D::UndoHistory &
	getUndoHistory (const X3D::BrowserPtr &) const;

	virtual
	void
	addUndoStep (const X3D::UndoStepPtr &) final override;

	void
	addUndoStep (const X3D::BrowserPtr &, const X3D::UndoStepPtr &);

	virtual
	void
	removeUndoStep () final override
	{ getUndoHistory (getCurrentBrowser ()) .removeUndoStep (); }

	virtual
	const X3D::UndoStepPtr &
	getUndoStep () const final override
	{ return getUndoHistory (getCurrentBrowser ()) .getUndoStep (); }

	void
	undo ();

	void
	redo ();

	/// @name Edit operations

	X3D::SFNode
	createNode (const std::string & typeName, const X3D::UndoStepPtr &);

	virtual
	void
	removeNodesFromScene (const X3D::X3DExecutionContextPtr &, const X3D::MFNode &, const bool, const X3D::UndoStepPtr &) const final override;

	/// @name CDATA field operations

	void
	editCDATA (const X3D::SFNode &);

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

	X3DBrowserEditor (const X3D::BrowserPtr &);

	virtual
	void
	configure () override;

	virtual
	void
	initialize () override;

	virtual
	void
	set_selection (const X3D::MFNode &) override;

	virtual
	void
	setBrowser (const X3D::BrowserPtr &) override;

	void
	setViewer (const X3D::X3DConstants::NodeType);

	/// @name Undo

	void
	set_undoHistory ();

	/// @name Operations

	void
	translateSelection (const X3D::Vector3f &, const bool, const ToolType);


private:

	///  @name Event handlers

	void
	set_shutdown ();

	void
	connectShutdown ();

	void
	set_scene ();

	void
	set_executionContext ();

	void
	set_selection_active (const bool);

	// File

	bool
	isSaved (const X3D::BrowserPtr &);

	void
	setMetaData ();

	void
	getMetaData ();

	// Edit

	///  @name CDATA field

	void
	on_cdata_changed (const Glib::RefPtr <Gio::File> &, const Glib::RefPtr <Gio::File> &, Gio::FileMonitorEvent event, const X3D::SFNode &);

	///  @name Members

	using UndoMatrixIndex = std::map <X3D::X3DTransformNodePtr, std::pair <X3D::Matrix4d, X3D::Vector3d>>;

	X3D::SFBool                        enabled;
	std::unique_ptr <BrowserSelection> selection;
	UndoMatrixIndex                    undoMatrices;
	X3D::UndoStepPtr                   nudgeUndoStep;
	double                             undoTime;
	ToolType                           tool;

};

} // puck
} // titania

#endif
