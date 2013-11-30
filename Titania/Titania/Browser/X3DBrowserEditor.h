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

#include "../Browser/X3DBrowserWidget.h"
#include "../Undo/UndoHistory.h"

namespace titania {
namespace puck {

class MagicImport;

class X3DBrowserEditor :
	public X3DBrowserWidget
{
public:

	/// @name Tests

	bool
	isSaved ();

	void
	isModified (bool);

	bool
	isModified () const
	{ return modified; }

	/// @name File operations

	void
	import (const basic::uri &, const bool);

	void
	import (const X3D::X3DSFNode <X3D::Scene> &, const UndoStepPtr &);

	virtual
	void
	save (const basic::uri &, const bool) final;

	virtual
	bool
	close () final;

	/// @name Undo/Redo operations

	void
	addUndoStep (const std::shared_ptr <UndoStep> &);

	std::shared_ptr <const UndoStep>
	getLastUndoStep () const
	{ return undoHistory .getLastUndoStep (); }

	void
	undo ();

	void
	redo ();

	/// @name Clipboard operations

	void
	cutNodes (X3D::MFNode, const UndoStepPtr &) const;

	void
	copyNodes (X3D::MFNode) const;

	void
	pasteNodes (const X3D::MFNode &, const UndoStepPtr &);

	/// @name Edit operations

	void
	replaceNode (const X3D::SFNode &, X3D::SFNode &, const X3D::SFNode &, const UndoStepPtr &);

	void
	removeNodes (const X3D::MFNode &, const UndoStepPtr &) const;

	X3D::SFNode
	groupNodes (const X3D::MFNode &, const UndoStepPtr &) const;

	X3D::MFNode
	ungroupNodes (const X3D::MFNode &, const UndoStepPtr &) const;

	bool
	addToGroup (const X3D::SFNode &, const X3D::MFNode &, const UndoStepPtr &) const;

	void
	detachFromGroup (X3D::MFNode, bool, const UndoStepPtr &) const;

	X3D::MFNode
	createParentGroup (const X3D::MFNode &, const UndoStepPtr &) const;

	void
	addRoute (X3D::X3DExecutionContext* const, const X3D::SFNode &, const std::string &, const X3D::SFNode &, const std::string &, const UndoStepPtr &) const
	throw (X3D::Error <X3D::INVALID_NODE>,
	       X3D::Error <X3D::INVALID_FIELD>,
	       X3D::Error <X3D::INVALID_OPERATION_TIMING>,
	       X3D::Error <X3D::DISPOSED>);

	void
	deleteRoute (X3D::X3DExecutionContext* const, const X3D::SFNode &, const std::string &, const X3D::SFNode &, const std::string &, const UndoStepPtr &) const;

	/// @name Selection operations

	void
	addSelection (const X3D::MFNode &, const UndoStepPtr &) const;

	void
	removeSelection (const X3D::MFNode &, const UndoStepPtr &) const;

	void
	select (const X3D::MFNode &, const UndoStepPtr &) const;

	void
	selectAll (const UndoStepPtr &) const;

	void
	deselectAll (const UndoStepPtr &) const;

	void
	saveMatrix (const X3D::SFNode &, const UndoStepPtr &) const;


protected:

	/// @name Construction

	X3DBrowserEditor (const basic::uri &);

	virtual
	void
	initialize () override;

	virtual
	void
	restoreSession () final;

	/// @name Undo

	void
	set_undoHistory ();

	/// @name Clipboard operations

	void
	updatePasteStatus ();

	bool
	getPasteStatus () const;


private:

	void
	set_initialized ();

	void
	set_shutdown ();

	void
	set_selection_active (bool);

	// Clipboard

	std::string
	toString (X3D::MFNode &) const;

	// Edit

	void
	removeNodeFromScene (const X3D::X3DSFNode <X3D::Scene> &, X3D::SFNode, const UndoStepPtr &) const;

	void
	removeExportedNodes (const X3D::X3DSFNode <X3D::Scene> &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeNodeFromExecutionContext (X3D::X3DExecutionContext* const, X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeNodeFromSceneGraph (X3D::X3DExecutionContext* const, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeNodeFromMFNode (X3D::MFNode &, const X3D::SFNode &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeNamedNode (X3D::X3DExecutionContext* const, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeImportedNodes (X3D::X3DExecutionContext* const, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	deleteRoutes (X3D::X3DExecutionContext* const, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	removePrototypes (X3D::X3DExecutionContext* const, X3D::SFNode &, const UndoStepPtr &) const;

	void
	createParentGroup (X3D::MFNode &, const X3D::SFNode &, const X3D::SFNode &, X3D::MFNode &, const UndoStepPtr &) const;

	///  @name Undo functions

	void
	setMatrix (const X3D::X3DSFNode <X3D::X3DTransformNode> &, const X3D::Matrix4f &, const UndoStepPtr &) const;

	void
	emplaceBack (X3D::MFNode &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	undoInsertNode (X3D::MFNode &, size_t, const X3D::SFNode &) const;

	void
	undoEraseNode (X3D::MFNode &, const X3D::SFNode &, const std::vector <size_t> &) const;

	///  @name Misc

	X3D::Matrix4f
	findModelViewMatrix (X3D::X3DBaseNode* const) const;

	bool
	findModelViewMatrix (X3D::X3DBaseNode* const, X3D::Matrix4f &, std::set <X3D::X3DBaseNode*> &) const;

	std::deque <X3D::X3DBaseNode*>
	getParentNodes (X3D::X3DBaseNode* const) const;

	X3D::X3DFieldDefinition*
	getContainerField (const X3D::SFNode &, const X3D::SFNode &) const
	throw (X3D::Error <X3D::INVALID_NODE>);

	X3D::MFNode*
	getGroupingField (const X3D::SFNode & node) const
	throw (X3D::Error <X3D::INVALID_NODE>);

	///  @name Members

	bool                        modified;
	bool                        saveConfirmed;
	int                         savedIndex;
	X3D::X3DSFNode <X3D::Scene> scene;

	std::shared_ptr <MagicImport> magicImport;

	using MatrixIndex = std::map <X3D::TransformHandle*, std::pair <X3D::Matrix4f, X3D::Vector3f>> ;

	UndoHistory undoHistory;
	MatrixIndex matrices;

};

} // puck
} // titania

#endif
