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
class BrowserSelection;
class X3DBrowserSelection;

class X3DBrowserEditor :
	public X3DBrowserWidget
{
public:

	/// @name Tests

	bool
	isSaved ();

	void
	isModified (const bool);

	bool
	isModified () const
	{ return modified; }

	/// @name File operations

	X3D::MFNode
	importURL (const std::vector <basic::uri> &, const bool, const UndoStepPtr & undoStep);

	X3D::MFNode
	importScene (const X3D::ScenePtr &, X3D::MFNode &, const UndoStepPtr &);

	std::vector <std::tuple <X3D::SFNode, std::string, X3D::SFNode, std::string>>
	getImportedRoutes (const X3D::X3DExecutionContextPtr &, const X3D::ScenePtr &) const;

	virtual
	void
	save (const basic::uri &, const bool) final override;

	std::string
	exportNodes (X3D::MFNode &) const;

	void
	exportNodes (std::ostream &, X3D::MFNode &) const;

	std::vector <X3D::X3DProtoDeclarationNodePtr>
	getUsedPrototypes (X3D::MFNode &) const;

	std::vector <X3D::Route*>
	getConnectedRouted (X3D::MFNode &) const;

	void
	removeUnusedPrototypes (const UndoStepPtr &);

	virtual
	bool
	close () final override;

	/// @name Undo/Redo operations

	void
	addUndoStep (const UndoStepPtr &);
	
	void
	removeUndoStep ()
	{ undoHistory .removeUndoStep (); }

	const std::shared_ptr <UndoStep> &
	getUndoStep () const
	{ return undoHistory .getUndoStep (); }

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
	pasteNodes (X3D::MFNode &, const UndoStepPtr &);

	/// @name Edit operations

	void
	replaceNodes (const X3D::SFNode &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	replaceNode (const X3D::SFNode &, X3D::SFNode &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	replaceNodes (const X3D::SFNode &, X3D::MFNode &, const X3D::SFNode &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	replaceNode (const X3D::SFNode &, X3D::MFNode &, const size_t, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeNode (const X3D::SFNode &, X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeNode (const X3D::SFNode &, X3D::MFNode &, const size_t, const UndoStepPtr &) const;

	void
	removeNodesFromScene (const X3D::X3DExecutionContextPtr &, X3D::MFNode, const UndoStepPtr &, const bool = true) const;

	void
	removeImportedNodes (const X3D::X3DExecutionContextPtr &, const std::set <X3D::InlinePtr> &, const UndoStepPtr &) const;

	void
	createClone (const X3D::SFNode &, const X3D::MFNode &, const UndoStepPtr &) const;

	X3D::MFNode
	unlinkClone (const X3D::MFNode &, const UndoStepPtr &) const;

	X3D::SFNode
	groupNodes (const std::string &, const X3D::MFNode &, const UndoStepPtr &) const;

	X3D::MFNode
	ungroupNodes (const X3D::MFNode &, const UndoStepPtr &) const;

	bool
	addToGroup (const X3D::SFNode &, const X3D::MFNode &, const UndoStepPtr &) const;

	void
	detachFromGroup (X3D::MFNode, const bool, const UndoStepPtr &) const;

	X3D::SFNode
	createParentGroup (const std::string &, const X3D::MFNode &, const UndoStepPtr &) const;

	void
	addPrototypeInstance (const std::string &);

	void
	updateNamedNode (const X3D::X3DExecutionContextPtr &, const std::string &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	addRoute (const X3D::X3DExecutionContextPtr &, const X3D::SFNode &, const std::string &, const X3D::SFNode &, const std::string &, const UndoStepPtr &) const
	throw (X3D::Error <X3D::INVALID_NODE>,
	       X3D::Error <X3D::INVALID_FIELD>,
	       X3D::Error <X3D::INVALID_OPERATION_TIMING>,
	       X3D::Error <X3D::DISPOSED>);

	void
	deleteRoute (const X3D::X3DExecutionContextPtr &, const X3D::SFNode &, const std::string &, const X3D::SFNode &, const std::string &, const UndoStepPtr &) const;

	void
	translateSelection (const X3D::Vector3f &, const bool);

	void
	saveMatrix (const X3D::SFNode &, const UndoStepPtr &) const;

	void
	setMatrix (const X3D::X3DTransformNodePtr &, const X3D::Matrix4d &, const UndoStepPtr &) const;

	void
	emplaceBack (X3D::MFNode &, const X3D::SFNode &, const UndoStepPtr &) const;

	/// @name CDATA field operations

	void
	editCDATA (const X3D::SFNode &);

	/// @name Selection operations

	const std::unique_ptr <BrowserSelection> &
	getSelection () const
	{ return selection; }

	///  @name Destruction

	virtual
	~X3DBrowserEditor ();


protected:

	friend class X3DBrowserSelection;

	/// @name Construction

	X3DBrowserEditor (int, char**);

	virtual
	void
	initialize () override;

	virtual
	void
	restoreSession () final override;

	/// @name Undo

	void
	set_undoHistory ();

	/// @name Clipboard operations

	void
	updatePasteStatus ();

	bool
	getPasteStatus () const;

	///  @name Grouping

	X3D::Matrix4d
	findModelViewMatrix (X3D::X3DBaseNode* const) const;

	std::vector <X3D::X3DBaseNode*>
	getParentNodes (X3D::X3DBaseNode* const) const;


private:

	void
	set_initialized ();

	void
	set_shutdown ();

	void
	set_selection_active (const bool);

	void
	set_selection (const X3D::MFNode &);

	// File

	void
	removeUsedPrototypes (X3D::X3DExecutionContext* const,
	                      std::map <X3D::ExternProtoDeclarationPtr, size_t> &,
	                      std::map <X3D::ProtoDeclarationPtr, size_t> &) const;

	// Edit

	void
	eraseNode (X3D::MFNode &, const size_t, const X3D::SFNode &) const;

	void
	removeNodeFromSceneIfNotExists (const X3D::X3DExecutionContextPtr &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeNode (const X3D::SFNode &, X3D::MFNode &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	removeNodesFromExecutionContext (const X3D::X3DExecutionContextPtr &, const std::set <X3D::SFNode> &, const UndoStepPtr &, const bool) const;

	void
	removeNodeFromSceneGraph (const X3D::X3DExecutionContextPtr &, const std::set <X3D::SFNode> &, const UndoStepPtr &) const;
	
	void
	removeExportedNodes (const X3D::X3DPtr <X3D::X3DScene> &, const std::set <X3D::SFNode> &, const UndoStepPtr &) const;

	void
	removeNamedNodes (const X3D::X3DExecutionContextPtr &, const std::set <X3D::SFNode> &, const UndoStepPtr &) const;

	void
	deleteRoutes (const X3D::X3DExecutionContextPtr &, const X3D::SFNode &, const UndoStepPtr &) const;

	void
	unlinkClone (const X3D::SFNode &, X3D::MFNode &, const X3D::SFNode &, X3D::MFNode &, bool &, const UndoStepPtr &) const;

	void
	createParentGroup (const X3D::X3DPtr <X3D::X3DGroupingNode> &, X3D::MFNode &, const X3D::SFNode &, const X3D::SFNode &, const UndoStepPtr &) const;

	///  @name Undo functions

	void
	undoInsertNode (X3D::MFNode &, size_t, const X3D::SFNode &) const;

	void
	undoEraseNode (X3D::MFNode &, const X3D::SFNode &, const std::vector <size_t> &) const;

	///  @name Misc

	bool
	findModelViewMatrix (X3D::X3DBaseNode* const, X3D::Matrix4d &, std::set <X3D::X3DBaseNode*> &) const;

	X3D::X3DFieldDefinition*
	getContainerField (const X3D::SFNode &, const X3D::SFNode &) const
	throw (X3D::Error <X3D::INVALID_NODE>);

	///  @name CDATA field

	void
	on_cdata_changed (const Glib::RefPtr <Gio::File> &, const Glib::RefPtr <Gio::File> &, Gio::FileMonitorEvent event, const X3D::SFNode &);

	///  @name Destruction

	void
	shutdown ();

	///  @name Members

	using UndoMatrixIndex = std::map <X3D::X3DTransformNodePtr, std::pair <X3D::Matrix4f, X3D::Vector3f>> ;

	bool                        modified;
	bool                        saveConfirmed;
	int                         savedIndex;
	X3D::X3DExecutionContextPtr currentScene;

	std::unique_ptr <BrowserSelection> selection;
	std::unique_ptr <MagicImport>      magicImport;
	UndoHistory                        undoHistory;
	UndoMatrixIndex                    undoMatrices;

	std::map <Glib::RefPtr <Gio::File>, Glib::RefPtr <Gio::FileMonitor>> fileMonitors;

};

} // puck
} // titania

#endif
