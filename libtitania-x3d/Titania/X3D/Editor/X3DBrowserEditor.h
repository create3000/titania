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

#ifndef __TITANIA_X3D_EDITOR_X3DBROWSER_EDITOR_H__
#define __TITANIA_X3D_EDITOR_X3DBROWSER_EDITOR_H__

#include "../Fields/SFNode.h"
#include "../Fields/MFNode.h"
#include "../Types/Pointer.h"
#include "../Types/MatrixStack.h"

#include "Undo/UndoStep.h"

namespace titania {
namespace X3D {

class X3DGeometryNode;
class X3DCoordinateNode;

class X3DBrowserEditor
{
public:

	///  @name Construction

	X3DBrowserEditor () = default;

	///  @name Clipboard handling

	virtual
	std::string
	cutNodes (const X3DExecutionContextPtr &, const MFNode &, const UndoStepPtr &) const;

	virtual
	std::string
	copyNodes (const X3DExecutionContextPtr &, const MFNode &) const;

	virtual
	std::string
	exportNodes (const X3DExecutionContextPtr &, MFNode &) const;

	virtual
	void
	exportNodes (const X3DExecutionContextPtr &, std::ostream &, MFNode &) const;

	///  @name Prototype handling

	virtual
	void
	removeUnusedPrototypes (const X3DExecutionContextPtr &, const UndoStepPtr &) const;

	virtual
	SFNode
	addPrototypeInstance (const X3DExecutionContextPtr &, const std::string &, const UndoStepPtr &) const;

	///  @name Replace operations

	virtual
	void
	replaceNodes (const X3DExecutionContextPtr &, const SFNode &, const SFNode &, const UndoStepPtr &) const;

	virtual
	void
	replaceNode (const X3DExecutionContextPtr &, const SFNode &, SFNode &, const SFNode &, const UndoStepPtr &) const;

	virtual
	void
	replaceNodes (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const MFNode &, const UndoStepPtr &) const;

	virtual
	void
	replaceNodes (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const SFNode &, const SFNode &, const UndoStepPtr &) const;

	virtual
	void
	replaceNode (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const size_t, const SFNode &, const UndoStepPtr &) const;

	///  @name Remove operations

	virtual
	void
	removeNode (const X3DExecutionContextPtr &, const SFNode &, SFNode &, const UndoStepPtr &) const;

	virtual
	void
	removeNode (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const size_t, const UndoStepPtr &) const;

	virtual
	void
	removeNodesFromSceneIfNotExists (const X3DExecutionContextPtr &, const MFNode &, const UndoStepPtr &) const;

	virtual
	void
	removeNodesFromScene (const X3DExecutionContextPtr &, const MFNode &, const UndoStepPtr &, const bool = true) const;

	virtual
	void
	removeNodesFromSceneGraph (const X3DExecutionContextPtr &, const std::set <SFNode> &, const UndoStepPtr &) const;

	virtual
	void
	removeNodesFromSceneGraph (const MFNode &, const std::set <SFNode> &, const UndoStepPtr &) const;

	virtual
	void
	removeImportedNodes (const X3DExecutionContextPtr &, const std::set <InlinePtr> &, const UndoStepPtr &) const;

	///  @name Named node handling

	virtual
	void
	updateNamedNode (const X3DExecutionContextPtr &, const std::string &, const SFNode &, const UndoStepPtr &) const;

	///  @name Route handling

	virtual
	void
	addRoute (const X3DExecutionContextPtr &, const SFNode &, const std::string &, const SFNode &, const std::string &, const UndoStepPtr &) const
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	void
	deleteRoute (const X3DExecutionContextPtr &, const SFNode &, const std::string &, const SFNode &, const std::string &, const UndoStepPtr &) const;

	///  @name Grouping operations

	virtual
	void
	createClone (const X3DExecutionContextPtr &, const SFNode &, const MFNode &, const UndoStepPtr &) const;

	virtual
	MFNode
	unlinkClone (const X3DExecutionContextPtr &, const MFNode &, const UndoStepPtr &) const;

	virtual
	SFNode
	groupNodes (const X3DExecutionContextPtr &, const std::string &, const MFNode &, const UndoStepPtr &) const;

	virtual
	MFNode
	ungroupNodes (const X3DExecutionContextPtr &, const MFNode &, const UndoStepPtr &) const;

	virtual
	bool
	addToGroup (const X3DExecutionContextPtr &, const SFNode &, const MFNode &, const UndoStepPtr &) const;

	virtual
	void
	detachFromGroup (const X3DExecutionContextPtr &, const MFNode &, const bool, const UndoStepPtr &) const;

	virtual
	SFNode
	createParentGroup (const X3DExecutionContextPtr &, const std::string &, const std::string &, const MFNode &, const UndoStepPtr &) const;

	///  @name Node handling

	virtual
	MFNode
	getParentNodes (const SFNode &) const;

	virtual
	X3DFieldDefinition*
	getContainerField (const SFNode &, const SFNode &) const
	throw (Error <INVALID_NODE>);

	///  @name Modelview matrix handling

	virtual
	Matrix4d
	findModelViewMatrix (const X3DExecutionContextPtr &, const SFNode &) const;

	virtual
	void
	saveMatrix (const SFNode &, const UndoStepPtr &) const;

	virtual
	void
	setMatrix (const X3DTransformNodePtr &, const Matrix4d &, const UndoStepPtr &) const;

	virtual
	void
	transformToZero (const MFNode &, const UndoStepPtr &) const;

	///  @name Array handling

	void
	emplaceBack (MFNode &, const SFNode &, const UndoStepPtr &) const;

	///  @name Destruction

	virtual
	~X3DBrowserEditor () = default;



private:

	///  @name Clipboard handling

	std::vector <X3DProtoDeclarationNodePtr>
	getUsedPrototypes (const X3DExecutionContextPtr &, MFNode &) const;

	std::vector <Route*>
	getConnectedRoutes (const X3DExecutionContextPtr &, MFNode &) const;

	///  @name Prototype handling

	void
	removeUsedPrototypes (const X3DExecutionContextPtr &,
	                      std::map <ExternProtoDeclarationPtr, size_t> &,
	                      std::map <ProtoDeclarationPtr, size_t> &) const;

	///  @name Remove operations

	void
	removeNode (const SFNode &, MFNode &, const SFNode &, const UndoStepPtr &) const;

	void
	removeNodesFromExecutionContext (const X3DExecutionContextPtr &, const std::set <SFNode> &, const UndoStepPtr &, const bool) const;

	void
	removeExportedNodes (const X3DScenePtr &, const std::set <SFNode> &, const UndoStepPtr &) const;

	void
	removeNamedNodes (const X3DExecutionContextPtr &, const std::set <SFNode> &, const UndoStepPtr &) const;

	void
	deleteRoutes (const X3DExecutionContextPtr &, const SFNode &, const UndoStepPtr &) const;
	                      
	///  @name Grouping operations

	void
	unlinkClone (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const SFNode &, MFNode &, bool &, const UndoStepPtr &) const;

	void
	createParentGroup (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const SFNode &, const SFNode &, const UndoStepPtr &) const;

	///  @name transformToZero

	void
	transformToZero (const MFNode &, Matrix4fStack &, const UndoStepPtr &) const;

	void
	transformToZero (const SFNode &, Matrix4fStack &, const UndoStepPtr &) const;

	void
	transformToZero (const X3DPtr <X3DGeometryNode> &, const Matrix4f &, const UndoStepPtr &) const;

	void
	transformToZero (const X3DPtr <X3DCoordinateNode> &, const Matrix4f &, const UndoStepPtr &) const;

};

} // X3D
} // titania

#endif
