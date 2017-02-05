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

#ifndef __TITANIA_X3D_EDITOR_X3D_EDITOR_H__
#define __TITANIA_X3D_EDITOR_X3D_EDITOR_H__

#include "../Basic/Traverse.h"
#include "../Fields/SFNode.h"
#include "../Fields/MFNode.h"
#include "../Types/Pointer.h"
#include "../Types/MatrixStack.h"

#include "Undo/UndoStep.h"

namespace titania {
namespace X3D {

class X3DGeometryNode;
class X3DCoordinateNode;

class X3DEditor
{
public:

	///  @name Construction

	X3DEditor () = default;

	///  @name Import/Export handling

	virtual
	MFNode
	importScene (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const X3DScenePtr &, const UndoStepPtr &) const;

	virtual
	std::string
	cutNodes (const X3DExecutionContextPtr &, const MFNode &, const UndoStepPtr &) const;

	virtual
	std::string
	copyNodes (const X3DExecutionContextPtr &, const MFNode &) const;

	virtual
	std::string
	exportNodes (const X3DExecutionContextPtr &, const MFNode &, const bool) const;

	virtual
	void
	exportNodes (const X3DExecutionContextPtr &, std::ostream &, const MFNode &, const bool) const;

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
	removeNodesFromScene (const X3DExecutionContextPtr &, const MFNode &, const bool, const UndoStepPtr &) const;

	virtual
	void
	removeNodesFromSceneGraph (const X3DExecutionContextPtr &, const std::set <SFNode> &, const UndoStepPtr &) const;

	virtual
	void
	removeNodesFromSceneGraph (const MFNode &, const std::set <SFNode> &, const UndoStepPtr &) const;

	virtual
	void
	removeImportedNodes (const X3DExecutionContextPtr &, const std::set <X3DPtr <Inline>> &, const UndoStepPtr &) const;

	///  @name Named node handling

	virtual
	void
	updateNamedNode (const X3DExecutionContextPtr &, const std::string &, const SFNode &, const UndoStepPtr &) const;

	virtual
	void
	updateProtoDeclaration (const X3DExecutionContextPtr &,
	                        const std::string &,
	                        const ProtoDeclarationPtr &,
	                        const UndoStepPtr &) const
	throw (Error <DISPOSED>);

	virtual
	void
	updateExternProtoDeclaration (const X3DExecutionContextPtr &,
	                              const std::string &,
	                              const ExternProtoDeclarationPtr &,
	                              const UndoStepPtr &) const
	throw (Error <DISPOSED>);

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

	virtual
	std::vector <std::tuple <SFNode, std::string, SFNode, std::string>>
	getImportedRoutes (const X3DExecutionContextPtr &, const X3DScenePtr &) const;

	///  @name User-defined fields operations

	virtual
	void
	addUserDefinedField (const SFNode &, X3DFieldDefinition* const, const UndoStepPtr &) const;
	
	virtual
	void
	replaceUserDefinedField (const SFNode &, X3DFieldDefinition* const, X3DFieldDefinition* const, const UndoStepPtr &) const;
	
	virtual
	void
	removeUserDefinedField (const SFNode &, X3DFieldDefinition* const, const UndoStepPtr &) const;
	
	virtual
	void
	setUserDefinedFields (const SFNode &, const FieldDefinitionArray &, const UndoStepPtr &) const;

	virtual
	void
	addReference (X3DFieldDefinition* const, X3DFieldDefinition* const, const UndoStepPtr &) const;
	
	virtual
	void
	removeReference (X3DFieldDefinition* const, X3DFieldDefinition* const, const UndoStepPtr &) const;

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

	virtual
	void
	addNodesToActiveLayer (const WorldPtr &, const MFNode &, const UndoStepPtr &) const;

	///  @name Node handling

	template <class NodeType>
	static
	X3DPtrArray <NodeType>
	getNodes (const MFNode &, const std::set <X3DConstants::NodeType> &);

	virtual
	MFNode
	getParentNodes (const SFNode &) const;

	virtual
	X3DFieldDefinition*
	getContainerField (const SFNode &, const SFNode &) const
	throw (Error <INVALID_NODE>);

	///  @name Modelview matrix handling

	virtual
	void
	transformToZero (const MFNode &, const UndoStepPtr &) const;

	virtual
	void
	storeMatrix (const SFNode &, const UndoStepPtr &) const;

	virtual
	void
	setMatrix (const X3DPtr <X3DTransformNode> &, const Matrix4d &, const UndoStepPtr &) const;

	virtual
	Matrix4d
	getModelViewMatrix (const X3DExecutionContextPtr &, const SFNode &) const;

	///  @name Polygon reducing

	void
	mergePoints (const X3DPtr <IndexedFaceSet> &, const double, const UndoStepPtr &) const;

	void
	undoSetColorColor (const X3DPtr <X3DColorNode> &, const UndoStepPtr &) const;

	///  @name X3DComposedGeometryNode
	
	void
	undoSetTexCoordPoint (const X3DPtr <X3DTextureCoordinateNode> &, const UndoStepPtr &) const;
	
	void
	undoSetNormalVector (const X3DPtr <X3DNormalNode> &, const UndoStepPtr &) const;
	
	void
	undoSetCoordPoint (const X3DPtr <X3DCoordinateNode> &, const UndoStepPtr &) const;
	
	void
	redoSetColorColor (const X3DPtr <X3DColorNode> &, const UndoStepPtr &) const;
	
	void
	redoSetTexCoordPoint (const X3DPtr <X3DTextureCoordinateNode> &, const UndoStepPtr &) const;
	
	void
	redoSetNormalVector (const X3DPtr <X3DNormalNode> &, const UndoStepPtr &) const;
	
	void
	redoSetCoordPoint (const X3DPtr <X3DCoordinateNode> &, const UndoStepPtr &) const;

	///  @name Array handling

	virtual
	void
	pushBackIntoArray (const SFNode &, MFNode &, const SFNode &, const UndoStepPtr &) const;

	virtual
	void
	insertIntoArray (const SFNode &, MFNode &, const size_t, const SFNode &, const UndoStepPtr &) const;

	virtual
	void
	insertIntoArray (const SFNode &, MFNode &, const size_t, const MFNode::iterator &, const MFNode::iterator &, const UndoStepPtr &) const;

	virtual
	void
	moveValueWithinArray (const SFNode &, MFNode &, const size_t, const size_t, const UndoStepPtr &) const;

	virtual
	void
	eraseFromArray (const SFNode &, MFNode &, const size_t, const UndoStepPtr &) const;

	///  @name Destruction

	virtual
	~X3DEditor () = default;


private:

	///  @name Clipboard handling

	std::vector <X3DProtoDeclarationNodePtr>
	getUsedPrototypes (const X3DExecutionContextPtr &, const MFNode &) const;

	std::vector <Route*>
	getConnectedRoutes (const X3DExecutionContextPtr &, const MFNode &) const;

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

	///  @name User-defined fields operations

	void
	removeRoutes (X3DFieldDefinition* const, const UndoStepPtr &) const;
	
	void
	replaceReferences (const ProtoDeclarationPtr &, X3DFieldDefinition* const, X3DFieldDefinition* const, const UndoStepPtr &) const;
	
	bool
	replaceReferencesCallback (SFNode &, X3DFieldDefinition* const, X3DFieldDefinition* const, const UndoStepPtr &) const;
	
	void
	removeReferences (const ProtoDeclarationPtr &, X3DFieldDefinition* const, const UndoStepPtr &) const;
	
	bool
	removeReferencesCallback (SFNode &, X3DFieldDefinition* const, const UndoStepPtr &) const;
	                      
	///  @name Grouping operations

	void
	unlinkClone (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const SFNode &, MFNode &, bool &, const UndoStepPtr &) const;

	void
	createParentGroup (const X3DExecutionContextPtr &, const SFNode &, MFNode &, const SFNode &, const SFNode &, const UndoStepPtr &) const;

	void
	addToLayers (const X3DExecutionContextPtr &, const std::vector <X3DLayerNode*> &, const SFNode &, const UndoStepPtr &) const;


	///  @name transformToZero

	void
	transformToZero (const MFNode &, Matrix4dStack &, std::set <X3DBaseNode*> &, const UndoStepPtr &) const;

	void
	transformToZero (const SFNode &, Matrix4dStack &, std::set <X3DBaseNode*> &, const UndoStepPtr &) const;

	void
	transformToZero (const X3DPtr <X3DGeometryNode> &, const Matrix4d &, std::set <X3DBaseNode*> &, const UndoStepPtr &) const;

	void
	transformToZero (const X3DPtr <X3DCoordinateNode> &, const Matrix4d &, std::set <X3DBaseNode*> &, const UndoStepPtr &) const;

	///  @name X3DComposedGeometryNode
	
	void
	undoSetTexCoordPointImpl (const X3DPtr <X3DTextureCoordinateNode> &, const UndoStepPtr &) const;
	
	void
	redoSetTexCoordPointImpl (const X3DPtr <X3DTextureCoordinateNode> &, const UndoStepPtr &) const;

};

/***
 *  Traverses @a selection and returns all nodes of a type specified in @a types.
 */
template <class NodeType>
X3DPtrArray <NodeType>
X3DEditor::getNodes (const MFNode & selection, const std::set <X3DConstants::NodeType> & types)
{
	// Find X3DGeometryNodes

	X3DPtrArray <NodeType> nodes;

	traverse (const_cast <MFNode &> (selection), [&] (SFNode & node)
	          {
	             for (const auto & type: node -> getType ())
	             {
	                if (types .count (type))
	          		{
	          		   nodes .emplace_back (node);
	          		   return true;
	          		}
	          	}
	          
	             return true;
	          });

	return nodes;
}

} // X3D
} // titania

#endif
