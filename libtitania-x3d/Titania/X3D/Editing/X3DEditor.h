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

#include "../Bits/Traverse.h"
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

	X3DEditor () = delete;

	///  @name Scene handling

	static
	void
	setWorldURL (const X3DScenePtr & scene, const basic::uri & worldURL, const UndoStepPtr & undoStep);

	static
	bool
	transform (const basic::uri & oldWorldURL, const basic::uri & newWorldURL, const UndoStepPtr & undoStep, SFNode & node);

	///  @name Import/Export handling

	static
	MFNode
	importScene (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & field, const X3DScenePtr & scene, const UndoStepPtr & undoStep);

	static
	std::string
	cutNodes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes, const UndoStepPtr & undoStep);

	static
	std::string
	copyNodes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes);

	static
	MFNode
	deepCopyNodes (const X3DExecutionContextPtr & sourceContext,
	               const X3DExecutionContextPtr & destContext,
	               const MFNode & nodes,
	               const UndoStepPtr & undoStep);

	static
	std::string
	exportNodes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes, const bool identifier);

	static
	void
	exportNodes (std::ostream & ostream, const X3DExecutionContextPtr & executionContext, const MFNode & nodes, const bool identifier);

	///  @name Prototype handling

	static
	void
	removeUnusedPrototypes (const X3DExecutionContextPtr & executionContext, const UndoStepPtr & undoStep);

	static
	SFNode
	addPrototypeInstance (const X3DExecutionContextPtr & executionContext, const std::string &, const UndoStepPtr & undoStep);

	///  @name Replace operations

	static
	void
	replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode &, const SFNode &, const UndoStepPtr & undoStep);

	static
	void
	replaceNode (const X3DExecutionContextPtr & executionContext, const SFNode &, SFNode &, const SFNode &, const UndoStepPtr & undoStep);

	static
	void
	replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode &, MFNode &, const MFNode &, const UndoStepPtr & undoStep);

	static
	void
	replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode &, MFNode &, const SFNode &, const SFNode &, const UndoStepPtr & undoStep);

	static
	void
	replaceNode (const X3DExecutionContextPtr & executionContext, const SFNode &, MFNode &, const size_t, const SFNode &, const UndoStepPtr & undoStep);

	///  @name Remove operations

	static
	void
	removeNode (const X3DExecutionContextPtr & executionContext, const SFNode &, SFNode &, const UndoStepPtr & undoStep);

	static
	void
	removeNode (const X3DExecutionContextPtr & executionContext, const SFNode &, MFNode &, const size_t, const UndoStepPtr & undoStep);

	static
	void
	removeNodesFromSceneIfNotExistsInSceneGraph (const X3DExecutionContextPtr & executionContext, const MFNode &, const UndoStepPtr & undoStep);

	static
	void
	removeNodesFromScene (const X3DExecutionContextPtr & executionContext, const MFNode &, const bool, const UndoStepPtr & undoStep);

	static
	void
	removeNodesFromSceneGraph (const X3DExecutionContextPtr & executionContext, const std::set <SFNode> &, const UndoStepPtr & undoStep);

	static
	void
	removeNodesFromSceneGraph (const MFNode &, const std::set <SFNode> &, const UndoStepPtr & undoStep);

	static
	void
	updateImportedNode (const X3DExecutionContextPtr & executionContext,
	                    const X3DPtr <Inline> & inlineNode,
	                    const std::string & exportedName,
	                    const std::string & importedName,
	                    const UndoStepPtr & undoStep);

	static
	void
	removeImportedNodes (const X3DExecutionContextPtr & executionContext, const std::set <X3DPtr <Inline>> &, const UndoStepPtr & undoStep);

	static
	void
	removeImportedNode (const X3DExecutionContextPtr & executionContext, const std::string & importedName, const UndoStepPtr & undoStep);

	///  @name Named node handling

	static
	void
	updateNamedNode (const X3DExecutionContextPtr & executionContext, const std::string & name, const SFNode & node, const UndoStepPtr & undoStep);

	static
	void
	removeNamedNode (const X3DExecutionContextPtr & executionContext, const SFNode & node, const UndoStepPtr & undoStep);

	static
	void
	updateProtoDeclaration (const X3DExecutionContextPtr & executionContext,
	                        const std::string & name,
	                        const ProtoDeclarationPtr & prototype,
	                        const UndoStepPtr & undoStep);

	static
	void
	convertProtoToExternProto (const ProtoDeclarationPtr & prototype, const MFString & url, const UndoStepPtr & undoStep);

	static
	void
	updateExternProtoDeclaration (const X3DExecutionContextPtr & executionContext,
	                              const std::string & name,
	                              const ExternProtoDeclarationPtr & externproto,
	                              const UndoStepPtr & undoStep);

	static
	void
	foldExternProtoBackIntoScene (const ExternProtoDeclarationPtr & externproto, const UndoStepPtr & undoStep);

	///  @name Route handling

	static
	void
	addRoute (const X3DExecutionContextPtr & executionContext,
	          const SFNode & soureceNode,
	          const std::string & sourceField,
	          const SFNode & destinationNode,
	          const std::string & destinationField,
	          const UndoStepPtr & undoStep)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	static
	void
	deleteRoute (const X3DExecutionContextPtr & executionContext,
	             const SFNode & soureceNode,
	             const std::string & sourceField,
	             const SFNode & destinationNode,
	             const std::string & destinationField,
	             const UndoStepPtr & undoStep);

	static
	std::vector <std::tuple <SFNode, std::string, SFNode, std::string>>
	getImportedRoutes (const X3DExecutionContextPtr & executionContext, const X3DScenePtr &);

	///  @name Prototype operations

	static
	void
	requestUpdateInstances (const X3DExecutionContextPtr & executionContext, const UndoStepPtr & undoStep);

	static
	void
	requestUpdateInstances (const SFNode & node, const UndoStepPtr & undoStep);

	static
	void
	addReference (const SFNode & node, X3DFieldDefinition* const field, X3DFieldDefinition* const protoField, const UndoStepPtr & undoStep);
	
	static
	void
	removeReference (const SFNode & node, X3DFieldDefinition* const field, X3DFieldDefinition* const protoField, const UndoStepPtr & undoStep);

	static
	void
	removeReferences (const SFNode & node, const UndoStepPtr & undoStep);

	///  @name Fields operations

	static
	void
	setValue (const SFNode & node, X3DFieldDefinition & field, const X3DFieldDefinition & value, const UndoStepPtr & undoStep);

	///  @name User-defined fields operations

	static
	void
	addUserDefinedField (const SFNode &, X3DFieldDefinition* const, const UndoStepPtr & undoStep);

	static
	void
	updateUserDefinedField (const SFNode & node, const AccessType accessType, const std::string & name, X3DFieldDefinition* const field, const UndoStepPtr & undoStep);

	static
	void
	replaceUserDefinedField (const SFNode &, X3DFieldDefinition* const, X3DFieldDefinition* const, const UndoStepPtr & undoStep);
	
	static
	void
	removeUserDefinedField (const SFNode &, X3DFieldDefinition* const, const UndoStepPtr & undoStep);
	
	static
	void
	setUserDefinedFields (const SFNode &, const FieldDefinitionArray &, const UndoStepPtr & undoStep);

	///  @name Grouping operations

	static
	void
	createClone (const X3DExecutionContextPtr & executionContext, const SFNode &, const MFNode &, const UndoStepPtr & undoStep);

	static
	MFNode
	unlinkClone (const X3DExecutionContextPtr & executionContext, const MFNode &, const UndoStepPtr & undoStep);

	static
	SFNode
	groupNodes (const X3DExecutionContextPtr & executionContext, const std::string &, const MFNode &, const UndoStepPtr & undoStep);

	static
	MFNode
	ungroupNodes (const X3DExecutionContextPtr & executionContext, const MFNode &, const UndoStepPtr & undoStep);

	static
	bool
	addToGroup (const X3DExecutionContextPtr & executionContext, const SFNode &, const MFNode &, const UndoStepPtr & undoStep);

	static
	void
	detachFromGroup (const X3DExecutionContextPtr & executionContext, const MFNode &, const bool, const UndoStepPtr & undoStep);

	static
	SFNode
	createParentGroup (const X3DExecutionContextPtr & executionContext, const std::string &, const std::string &, const MFNode &, const UndoStepPtr & undoStep);

	static
	void
	addNodesToActiveLayer (const WorldPtr &, const MFNode &, const UndoStepPtr & undoStep);

	///  @name Node handling

	template <class NodeType>
	static
	X3DPtrArray <NodeType>
	getNodes (const MFNode &, const std::set <X3DConstants::NodeType> &);

	static
	MFNode
	getParentNodes (const SFNode &);

	static
	X3DFieldDefinition*
	getContainerField (const SFNode &, const SFNode &)
	throw (Error <INVALID_NODE>);

	///  @name Modelview matrix handling

	static
	void
	transformToZero (const MFNode &, const UndoStepPtr & undoStep);

	static
	void
	storeMatrix (const SFNode &, const UndoStepPtr & undoStep);

	static
	void
	setMatrix (const X3DPtr <X3DTransformNode> &, const Matrix4d &, const UndoStepPtr & undoStep);

	static
	Matrix4d
	getModelViewMatrix (const X3DExecutionContextPtr & executionContext, const SFNode &);

	///  @name Polygon reducing

	static
	void
	mergePoints (const X3DPtr <IndexedFaceSet> &, const double, const UndoStepPtr & undoStep);

	static
	void
	undoSetColor (const X3DPtr <X3DColorNode> &, const UndoStepPtr & undoStep);

	///  @name X3DComposedGeometryNode
	
	static
	void
	undoSetTexCoord (const X3DPtr <X3DTextureCoordinateNode> &, const UndoStepPtr & undoStep);
	
	static
	void
	undoSetNormal (const X3DPtr <X3DNormalNode> &, const UndoStepPtr & undoStep);
	
	static
	void
	undoSetCoord (const X3DPtr <X3DCoordinateNode> &, const UndoStepPtr & undoStep);
	
	static
	void
	redoSetColor (const X3DPtr <X3DColorNode> &, const UndoStepPtr & undoStep);
	
	static
	void
	redoSetTexCoord (const X3DPtr <X3DTextureCoordinateNode> &, const UndoStepPtr & undoStep);
	
	static
	void
	redoSetNormal (const X3DPtr <X3DNormalNode> &, const UndoStepPtr & undoStep);
	
	static
	void
	redoSetCoord (const X3DPtr <X3DCoordinateNode> &, const UndoStepPtr & undoStep);

	///  @name Array handling

	static
	void
	pushBackIntoArray (const SFNode & parent, MFNode & array, const SFNode & node, const UndoStepPtr & undoStep);

	static
	void
	insertIntoArray (const SFNode & parent, MFNode & array, const size_t index, const SFNode & node, const UndoStepPtr & undoStep);

	static
	void
	insertIntoArray (const SFNode & parent, MFNode & array, const size_t index, const MFNode::iterator & first, const MFNode::iterator & last, const UndoStepPtr & undoStep);

	static
	void
	moveValueWithinArray (const SFNode & parent, MFNode & array, const size_t fromIndex, const size_t toIndex, const UndoStepPtr & undoStep);

	static
	void
	eraseFromArray (const SFNode & parent, MFNode & array, const size_t index, const UndoStepPtr & undoStep);

	///  @name Destruction

	~X3DEditor () = delete;


private:

	///  @name Clipboard handling

	static
	std::vector <X3DProtoDeclarationNodePtr>
	getUsedPrototypes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes);

	static
	bool
	isSubContext (const X3DExecutionContext* executionContext, X3DExecutionContext* subContext);

	static
	std::vector <Route*>
	getConnectedRoutes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes);

	///  @name Prototype handling

	static
	void
	removeUsedPrototypes (const X3DExecutionContextPtr & executionContext,
	                      std::map <ExternProtoDeclarationPtr, size_t> &,
	                      std::map <ProtoDeclarationPtr, size_t> &);

	///  @name Remove operations

	static
	void
	removeNode (const SFNode &, MFNode &, const SFNode &, const UndoStepPtr & undoStep);

	static
	void
	removeNodesFromExecutionContext (const X3DExecutionContextPtr & executionContext, const std::set <SFNode> &, const UndoStepPtr &, const bool);

	static
	void
	removeExportedNodes (const X3DScenePtr &, const std::set <SFNode> &, const UndoStepPtr & undoStep);

	static
	void
	removeNamedNodes (const X3DExecutionContextPtr & executionContext, const std::set <SFNode> &, const UndoStepPtr & undoStep);

	static
	void
	restoreProtoDeclarations (const X3DExecutionContextPtr & executionContext, const ProtoArray & protos);

	static
	void
	restoreExternProtoDeclarations (const X3DExecutionContextPtr & executionContext, const ExternProtoArray & protos);

	static
	void
	deleteRoutes (const X3DExecutionContextPtr & executionContext, const SFNode &, const UndoStepPtr & undoStep);

	///  @name Fields operations

	static
	const X3DFieldDefinition &
	getField (const FieldPtr & fieldPtr);

	///  @name User-defined fields operations

	static
	void
	addReferences (const SFNode & node, const std::string & fieldName, const FieldDefinitionSet & references);

	static
	void
	getAddRouteFunctions (const SFNode & node,
	                      const AccessType accessType,
	                      const std::string & name,
	                      X3DFieldDefinition* const field,
	                      const UndoStepPtr & undoStep,
	                      std::deque <std::function <void ()>> & functions);

	static
	void
	removeRoutes (X3DFieldDefinition* const, const UndoStepPtr & undoStep);
	
	static
	void
	replaceReferences (const ProtoDeclarationPtr &, X3DFieldDefinition* const, X3DFieldDefinition* const, const UndoStepPtr & undoStep);
	
	static
	bool
	replaceReferencesCallback (SFNode &, X3DFieldDefinition* const, X3DFieldDefinition* const, const UndoStepPtr & undoStep);
	
	static
	void
	removeReferences (const ProtoDeclarationPtr &, X3DFieldDefinition* const, const UndoStepPtr & undoStep);
	
	static
	bool
	removeReferencesCallback (SFNode &, X3DFieldDefinition* const, const UndoStepPtr & undoStep);

	///  @name Grouping operations

	static
	void
	unlinkClone (const X3DExecutionContextPtr & executionContext, const SFNode &, MFNode &, const SFNode &, MFNode &, bool &, const UndoStepPtr & undoStep);

	static
	void
	createParentGroup (const X3DExecutionContextPtr & executionContext, const SFNode &, MFNode &, const SFNode &, const SFNode &, const UndoStepPtr & undoStep);

	static
	void
	addToLayers (const X3DExecutionContextPtr & executionContext, const std::vector <X3DLayerNode*> &, const SFNode &, const UndoStepPtr & undoStep);


	///  @name transformToZero

	static
	void
	transformToZero (const MFNode &, Matrix4dStack &, std::set <X3DBaseNode*> &, const UndoStepPtr & undoStep);

	static
	void
	transformToZero (const SFNode &, Matrix4dStack &, std::set <X3DBaseNode*> &, const UndoStepPtr & undoStep);

	static
	void
	transformToZero (const X3DPtr <X3DGeometryNode> &, const Matrix4d &, std::set <X3DBaseNode*> &, const UndoStepPtr & undoStep);

	static
	void
	transformToZero (const X3DPtr <X3DCoordinateNode> &, const Matrix4d &, std::set <X3DBaseNode*> &, const UndoStepPtr & undoStep);

	///  @name X3DComposedGeometryNode
	
	static
	void
	undoSetTexCoordImpl (const X3DPtr <X3DTextureCoordinateNode> &, const UndoStepPtr & undoStep);
	
	static
	void
	redoSetTexCoordImpl (const X3DPtr <X3DTextureCoordinateNode> &, const UndoStepPtr & undoStep);

};

/***
 *  Traverses @a selection and returns all visible nodes of a type specified in @a types.
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
	},
	TRAVERSE_ROOT_NODES | TRAVERSE_VISIBLE_NODES);

	return nodes;
}

} // X3D
} // titania

#endif
