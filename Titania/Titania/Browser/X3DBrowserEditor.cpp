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

#include "BrowserWindow.h"

#include "../Browser/BrowserSelection.h"
#include "../Browser/MagicImport.h"
#include "../Dialogs/FileSaveWarningDialog/FileSaveWarningDialog.h"
#include "../Widgets/OutlineEditor/OutlineTreeViewEditor.h"

#include <Titania/InputOutput/MultiLineComment.h>
#include <Titania/OS.h>
#include <Titania/String.h>
#include <Titania/Utility/Map.h>

namespace titania {
namespace puck {

X3DBrowserEditor::X3DBrowserEditor (int argc, char** argv) :
	X3DBrowserWidget (argc, argv),
	        modified (false),
	   saveConfirmed (false),
	    currentScene (),
	       selection (new BrowserSelection (getBrowserWindow ())),
	     magicImport (new MagicImport (getBrowserWindow ())),
	     undoHistory (),
	    undoMatrices (),
	    fileMonitors ()
{ }

void
X3DBrowserEditor::initialize ()
{
	X3DBrowserWidget::initialize ();

	getBrowser () -> initialized () .addInterest (this, &X3DBrowserEditor::set_initialized);
	getBrowser () -> shutdown ()    .addInterest (this, &X3DBrowserEditor::set_shutdown);

	getBrowser () -> getSelection () -> isActive () .addInterest (this, &X3DBrowserEditor::set_selection_active);
	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &X3DBrowserEditor::set_selection);

	undoHistory .addInterest (this, &X3DBrowserEditor::set_undoHistory);

	set_undoHistory ();
}

void
X3DBrowserEditor::restoreSession ()
{
	// ImportAsInline
	if (getConfig () .hasItem ("importAsInline"))
		getImportAsInlineMenuItem () .set_active (getConfig () .getBoolean ("importAsInline"));

	// Workspace
	if (getConfig () .getString ("workspace") == "BROWSER")
		getBrowserMenuItem () .set_active (true);

	else if (getConfig () .getString ("workspace") == "EDITOR")
		getEditorMenuItem () .set_active (true);

	// SelectLowest
	if (getConfig () .hasItem ("selectLowest"))
		getSelectLowestMenuItem () .set_active (getConfig () .getBoolean ("selectLowest"));

	// FollowPrimarySelection
	if (getConfig () .hasItem ("followPrimarySelection"))
		getFollowPrimarySelectionMenuItem () .set_active (getConfig () .getBoolean ("followPrimarySelection"));

	// This must be done after.
	X3DBrowserWidget::restoreSession ();
}

void
X3DBrowserEditor::set_initialized ()
{
	if (getExecutionContext () not_eq currentScene)
		isModified (false);

	currentScene = getExecutionContext ();
}

void
X3DBrowserEditor::set_shutdown ()
{
	if (isSaved ())
		shutdown ();

	else                                     // Cancel shutdown
		setExecutionContext (getExecutionContext ());
}

void
X3DBrowserEditor::set_selection_active (const bool value)
{
	if (value)
	{
		for (const auto & child : getBrowser () -> getSelection () -> getChildren ())
		{
			const X3D::X3DTransformNodePtr transform (child);

			if (transform)
				undoMatrices [transform] = std::make_pair (transform -> getMatrix (), transform -> center () .getValue ());
		}
	}
	else
	{
		bool changed = false;

		const auto undoStep = std::make_shared <UndoStep> (_ ("Edit Transform"));

		getSelection () -> redoRestoreSelection (undoStep);

		for (const auto & child : getBrowser () -> getSelection () -> getChildren ())
		{
			const X3D::X3DTransformNodePtr transform (child);

			if (transform)
			{
				try
				{
					const X3D::Matrix4f matrix = undoMatrices .at (transform) .first;
					const X3D::Vector3f center = undoMatrices .at (transform) .second;

					if (matrix not_eq transform -> getMatrix () or center not_eq transform -> center ())
					{
						changed = true;

						undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter,
						                             transform,
						                             matrix,
						                             center);

						undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixWithCenter,
						                             transform,
						                             transform -> getMatrix (),
						                             transform -> center ());
					}
				}
				catch (const std::out_of_range &)
				{ }
			}
		}

		undoMatrices .clear ();

		getSelection () -> undoRestoreSelection (undoStep);

		if (changed)
			addUndoStep (undoStep);
	}
}

void
X3DBrowserEditor::set_selection (const X3D::MFNode & selection)
{
	if (selection .empty ())
		return;

	// If the parent node of the selection leader is a Switch, set whichChoice to the index of the leader.

	for (const auto & node : getParentNodes (selection .back ()))
	{
		const auto switchNode = dynamic_cast <X3D::Switch*> (node);

		if (not switchNode)
			continue;

		const auto iter = std::find (switchNode -> children () .begin (),
		                             switchNode -> children () .end (),
		                             selection .back ());

		if (iter == switchNode -> children () .end ())
			continue;

		switchNode -> whichChoice () = iter - switchNode -> children () .begin ();
	}
}

bool
X3DBrowserEditor::isSaved ()
{
	if (saveConfirmed)
		return true;

	if (isModified ())
	{
		const auto responseId = std::dynamic_pointer_cast <FileSaveWarningDialog> (addDialog ("FileSaveWarningDialog", false)) -> run ();

		switch (responseId)
		{
			case Gtk::RESPONSE_OK:
			{
				on_save ();
				saveConfirmed = not isModified ();
				return saveConfirmed;
			}
			case Gtk::RESPONSE_CANCEL:
			case Gtk::RESPONSE_DELETE_EVENT:
			{
				return false;
			}
			default:
				break;
		}
	}

	saveConfirmed = true;
	return true;
}

void
X3DBrowserEditor::isModified (const bool value)
{
	modified      = value;
	saveConfirmed = false;

	updateTitle (value);

	if (not value)
		undoHistory .setSaved ();
}

// File operations

X3D::MFNode
X3DBrowserEditor::importURL (const std::vector <basic::uri> & uris, const bool importAsInline, const UndoStepPtr & undoStep)
{
	if (importAsInline)
	{
		// Import As Inline

		for (const auto & worldURL : uris)
		{
			const auto relativePath = getExecutionContext () -> getWorldURL () .relative_path (worldURL);

			std::string string;

			string += "DEF " + X3D::get_name_from_uri (worldURL) + " Transform {";
			string += "  children Inline {";
			string += "    url [";
			string += "      \"" + relativePath + "\"";
			string += "      \"" + worldURL + "\"";
			string += "    ]";
			string += "  }";
			string += "}";

			const auto scene = getBrowser () -> createX3DFromString (string);

			return importScene (scene, getExecutionContext () -> getRootNodes (), undoStep);
		}

		return X3D::MFNode ();
	}

	// Import into scene graph

	X3D::MFNode nodes;
	auto        selection = getBrowser () -> getSelection () -> getChildren ();

	for (const auto & worldURL : uris)
	{
		try
		{
			const auto scene = getBrowser () -> createX3DFromURL ({ worldURL .str () });

			if (magicImport -> import (selection, scene, undoStep))
				continue;

			const auto importedNodes = importScene (scene, getExecutionContext () -> getRootNodes (), undoStep);

			nodes .insert (nodes .end (), importedNodes .begin (), importedNodes .end());
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	return nodes;
}

X3D::MFNode
X3DBrowserEditor::importScene (const X3D::ScenePtr & scene, X3D::MFNode & field, const UndoStepPtr & undoStep)
{
	const auto & executionContext = getExecutionContext ();

	try
	{
		const size_t size = field .size ();

		using resize = void (X3D::MFNode::*) (const X3D::MFNode::size_type);
	
		undoStep -> addUndoFunction ((resize) &X3D::MFNode::resize, std::ref (field), size);

		// Imported scene

		X3D::MFNode importedNodes;

		executionContext -> import (scene, importedNodes);

		const auto undoRemoveNodes = std::make_shared <UndoStep> ();

		removeNodesFromScene (executionContext, importedNodes, undoRemoveNodes, false);

		undoStep -> addUndoFunction (&UndoStep::redoChanges, undoRemoveNodes);
		undoStep -> addRedoFunction (&UndoStep::undoChanges, undoRemoveNodes);
		undoRemoveNodes -> undoChanges ();

		using append = X3D::X3DArrayField <X3D::SFNode> & (X3D::MFNode::*) (const X3D::X3DArrayField <X3D::SFNode> &);

		undoStep -> addRedoFunction ((append) &X3D::MFNode::append, std::ref (field), importedNodes);
		field .append (importedNodes);

		return importedNodes;
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		return X3D::MFNode ();
	}
}

std::vector <std::tuple <X3D::SFNode, std::string, X3D::SFNode, std::string>>
X3DBrowserEditor::getImportedRoutes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::ScenePtr & scene) const
{
	std::vector <std::tuple <X3D::SFNode, std::string, X3D::SFNode, std::string>> routes;

	for (const auto & pair : executionContext -> getImportedNodes ())
	{
		try
		{
			const auto & inlineNode = pair .second -> getInlineNode ();

			if (inlineNode -> getInternalScene () not_eq scene)
				continue;

			const auto exportedNode = pair .second -> getExportedNode ();

			for (const auto & field : exportedNode -> getFieldDefinitions ())
			{
				for (const auto & route : field -> getInputRoutes ())
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					try
					{
						routes .emplace_back (route -> getSourceNode (), route -> getSourceField (),
						                      route -> getDestinationNode (), route -> getDestinationField ());
					}
					catch (const X3D::X3DError &)
					{ }
				}

				for (const auto & route: field -> getOutputRoutes ())
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					try
					{
						routes .emplace_back (route -> getSourceNode (), route -> getSourceField (),
						                      route -> getDestinationNode (), route -> getDestinationField ());
					}
					catch (const X3D::X3DError &)
					{ }
				}
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return routes;
}

void
X3DBrowserEditor::save (const basic::uri & worldURL, const bool compressed)
{
	X3DBrowserWidget::save (worldURL, compressed);

	isModified (false);
}

std::string
X3DBrowserEditor::exportNodes (X3D::MFNode & nodes) const
{
	std::ostringstream osstream;

	exportNodes (osstream, nodes);

	return osstream .str ();

}

void
X3DBrowserEditor::exportNodes (std::ostream & ostream, X3D::MFNode & nodes) const
{
	// Find proto declarations

	const auto protoNodes = getUsedPrototypes (nodes);
	const auto routes     = getConnectedRouted (nodes);

	// Generate text

	ostream .imbue (std::locale::classic ());

	ostream
		<< "#X3D V3.3 utf8 " << getBrowser () -> getName ()
		<< std::endl
		<< std::endl
		<< '#' << getExecutionContext () -> getWorldURL ()
		<< std::endl
		<< std::endl;

	X3D::Generator::CompactStyle ();
	X3D::Generator::PushContext ();

	if (not protoNodes .empty ())
	{
		for (const auto & protoNode : protoNodes)
			ostream << protoNode << std::endl;

		ostream << std::endl;
	}

	for (const auto & node : nodes)
		ostream << node << std::endl;

	if (not routes .empty ())
	{
		ostream << std::endl;

		for (const auto & route : routes)
			ostream << *route << std::endl;
	}

	X3D::Generator::PopContext ();
}

std::vector <X3D::X3DProtoDeclarationNodePtr>
X3DBrowserEditor::getUsedPrototypes (X3D::MFNode & nodes) const
{
	std::map <X3D::X3DProtoDeclarationNodePtr, size_t> protoIndex;

	X3D::traverse (nodes, [&] (X3D::SFNode & node)
	               {
	                  const X3D::X3DPrototypeInstancePtr protoInstance (node);

	                  if (protoInstance)
	                  {
	                     X3D::traverse (node, [&] (X3D::SFNode & node)
	                                    {
	                                       if (node == protoInstance)
															return true;

	                                       const X3D::X3DPrototypeInstancePtr child (node);

	                                       if (child)
	                                       {
	                                          try
	                                          {
	                                             if (child -> getProtoNode () == getExecutionContext () -> findProtoDeclaration (child -> getTypeName (), X3D::AvailableType { }))
																	protoIndex .emplace (child -> getProtoNode (), protoIndex .size ());
															}
	                                          catch (const X3D::X3DError &)
	                                          { }
														}

	                                       return true;
													},
	                                    true,
	                                    X3D::TRAVERSE_PROTOTYPE_INSTANCES);

	                     protoIndex .emplace (protoInstance -> getProtoNode (), protoIndex .size ());
							}

	                  return true;
						});

	std::vector <X3D::X3DProtoDeclarationNodePtr> protoNodes;

	for (const auto & protoNode : basic::reverse (protoIndex))
		protoNodes .emplace_back (std::move (protoNode .second));

	return protoNodes;
}

std::vector <X3D::Route*>
X3DBrowserEditor::getConnectedRouted (X3D::MFNode & nodes) const
{
	// Create node index

	std::set <X3D::SFNode> nodeIndex;

	X3D::traverse (nodes, [&nodeIndex] (X3D::SFNode & node)
	               {
	                  nodeIndex .emplace (node);
	                  return true;
						});

	// Find connected routes

	std::vector <X3D::Route*> routes;

	X3D::traverse (nodes, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & field: node -> getFieldDefinitions ())
	                  {
	                     for (const auto & route: field -> getOutputRoutes ())
	                     {
	                        if (route -> getExecutionContext () not_eq getExecutionContext ())
	                           continue;

                           try
                           {
                              if (nodeIndex .count (route -> getDestinationNode ()))
											routes .emplace_back (route);
									}
                           catch (const X3D::X3DError &)
                           { }
								}
							}

	                  return true;
						});

	return routes;
}

void
X3DBrowserEditor::removeUnusedPrototypes (const UndoStepPtr & undoStep)
{
	// Get ExternProtos and Prototypes

	std::map <X3D::ExternProtoDeclarationPtr, size_t> externProtos;

	for (const auto & externProto : getExecutionContext () -> getExternProtoDeclarations ())
		externProtos .emplace (externProto, externProtos .size ());

	std::map <X3D::ProtoDeclarationPtr, size_t> prototypes;

	for (const auto & prototype : getExecutionContext () -> getProtoDeclarations ())
		prototypes .emplace (prototype, prototypes .size ());

	// Find proto declaration not used in prototypes or scene.

	removeUsedPrototypes (getExecutionContext (), externProtos, prototypes);

	// Remove ExternProtos.

	const auto externProtoIndex = basic::reverse (externProtos);

	for (const auto & pair : basic::make_reverse_range (externProtoIndex))
	{
		const auto & externProto = pair .second;

		undoStep -> addUndoFunction (&X3D::X3DExecutionContext::updateExternProtoDeclaration, getExecutionContext (), externProto -> getName (), externProto);
		undoStep -> addRedoFunction (&X3D::X3DExecutionContext::removeExternProtoDeclaration, getExecutionContext (), externProto -> getName ());

		getExecutionContext () -> removeExternProtoDeclaration (externProto -> getName ());
	}

	// Remove Prototypes.

	const auto prototypeIndex = basic::reverse (prototypes);

	for (const auto & pair : basic::make_reverse_range (prototypeIndex))
	{
		const auto & prototype = pair .second;

		undoStep -> addUndoFunction (&X3D::X3DExecutionContext::updateProtoDeclaration, getExecutionContext (), prototype -> getName (), prototype);
		undoStep -> addRedoFunction (&X3D::X3DExecutionContext::removeProtoDeclaration, getExecutionContext (), prototype -> getName ());

		getExecutionContext () -> removeProtoDeclaration (prototype -> getName ());
	}
}

void
X3DBrowserEditor::removeUsedPrototypes (X3D::X3DExecutionContext* const executionContext,
                                        std::map <X3D::ExternProtoDeclarationPtr, size_t> & externProtos,
                                        std::map <X3D::ProtoDeclarationPtr, size_t> & prototypes) const
{
	X3D::traverse (executionContext -> getRootNodes (), [&] (X3D::SFNode & node)
	               {
	                  for (const auto & type: basic::make_reverse_range (node -> getType ()))
	                  {
	                     switch (type)
	                     {
									case X3D::X3DConstants::X3DPrototypeInstance:
										{
										   const X3D::X3DPrototypeInstancePtr instance (node);

										   switch (instance -> getProtoNode () -> getType () .back ())
										   {
												case  X3D::X3DConstants::ExternProtoDeclaration:
													{
													   const X3D::ExternProtoDeclarationPtr externProto (instance -> getProtoNode ());

													   externProtos .erase (externProto);
													   break;
													}
												case  X3D::X3DConstants::ProtoDeclaration:
													{
													   const X3D::ProtoDeclarationPtr prototype (instance -> getProtoNode ());

													   prototypes .erase (prototype);

													   // This is not neccessary:
													   // removeUsedPrototypes (prototype, externProtos, prototypes);
													   break;
													}
												default:
													break;
											}

										   break;
										}
									default:
										break;
								}
							}

	                  return true;
						},
	               true, X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

bool
X3DBrowserEditor::close ()
{
	getWidget () .grab_focus ();

	if (isSaved ())
	{
		shutdown ();
		return X3DBrowserWidget::close ();
	}

	return true;
}

// Undo/Redo operations

void
X3DBrowserEditor::addUndoStep (const UndoStepPtr & undoStep)
{
	undoHistory .addUndoStep (undoStep);
}

void
X3DBrowserEditor::undo ()
{
	getBrowser () -> grab_focus ();

	undoHistory .undoChanges ();
}

void
X3DBrowserEditor::redo ()
{
	getBrowser () -> grab_focus ();

	undoHistory .redoChanges ();
}

void
X3DBrowserEditor::set_undoHistory ()
{
	const int index = undoHistory .getIndex ();

	if (index >= 0)
	{
		getUndoMenuItem () .set_label (undoHistory .getUndoDescription ());
		getUndoButton ()   .set_tooltip_text (undoHistory .getUndoDescription ());
		getUndoMenuItem () .set_sensitive (true);
		getUndoButton ()   .set_sensitive (true);
	}
	else
	{
		getUndoMenuItem () .set_label (_ ("Undo"));
		getUndoButton ()   .set_tooltip_text (_ ("Undo last action (Ctrl-Z)."));
		getUndoMenuItem () .set_sensitive (false);
		getUndoButton ()   .set_sensitive (false);
	}

	if (index + 1 < (int) undoHistory .getSize ())
	{
		getRedoMenuItem () .set_label (undoHistory .getRedoDescription ());
		getRedoButton ()   .set_tooltip_text (undoHistory .getRedoDescription ());
		getRedoMenuItem () .set_sensitive (true);
		getRedoButton ()   .set_sensitive (true);
	}
	else
	{
		getRedoMenuItem () .set_label (_ ("Redo"));
		getRedoMenuItem () .set_tooltip_text (_ ("Redo last action (Ctrl-Shift-Z)."));
		getRedoMenuItem () .set_sensitive (false);
		getRedoButton ()   .set_sensitive (false);
	}

	isModified (undoHistory .isModified ());
}

// Clipboard operations

void
X3DBrowserEditor::cutNodes (X3D::MFNode nodes, const UndoStepPtr & undoStep) const
{
	// Detach from group

	detachFromGroup (nodes, true, undoStep);

	// Set clipboard text

	Gtk::Clipboard::get () -> set_text (exportNodes (nodes));

	// Remove nodes

	removeNodesFromScene (getExecutionContext (), nodes, undoStep);
}

void
X3DBrowserEditor::copyNodes (X3D::MFNode nodes) const
{
	// Detach from group

	const auto undoDetachFromGroup = std::make_shared <UndoStep> ();

	detachFromGroup (nodes, true, undoDetachFromGroup);

	// Set clipboard text

	Gtk::Clipboard::get () -> set_text (exportNodes (nodes));

	// Undo detach from group

	undoDetachFromGroup -> undoChanges ();
}

void
X3DBrowserEditor::pasteNodes (X3D::MFNode & nodes, const UndoStepPtr & undoStep)
{
	try
	{
		const Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

		if (clipboard -> wait_is_text_available ())
		{
			basic::ifilestream text (clipboard -> wait_for_text ());

			text .imbue (std::locale::classic ());

			std::string header;

			if (X3D::Grammar::Comment (text, header))
			{
				std::string encoding, specificationVersion, characterEncoding, comment;

				if (X3D::Grammar::Header .FullMatch (header, &encoding, &specificationVersion, &characterEncoding, &comment))
				{
					std::string whiteSpaces;

					X3D::Grammar::WhiteSpaces (text, whiteSpaces);

					std::string worldURL;

					if (X3D::Grammar::Comment (text, worldURL))
					{
						const auto scene = getBrowser () -> createX3DFromStream (worldURL, text);

						if (not magicImport -> import (nodes, scene, undoStep))
						{
							getSelection () -> setChildren (importScene (scene, getExecutionContext () -> getRootNodes (), undoStep),
							                                undoStep);
						}
					}
				}
			}
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserEditor::updatePasteStatus ()
{
	getPasteMenuItem () .set_sensitive (getPasteStatus ());
}

bool
X3DBrowserEditor::getPasteStatus () const
{
	const Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

	if (clipboard -> wait_is_text_available ())
	{
		std::istringstream text (clipboard -> wait_for_text ());

		text .imbue (std::locale::classic ());

		std::string header;

		if (X3D::Grammar::Comment (text, header))
		{
			std::string encoding, specificationVersion, characterEncoding, comment;

			if (X3D::Grammar::Header .FullMatch (header, &encoding, &specificationVersion, &characterEncoding, &comment))
				return true;
		}
	}

	return false;
}

// Edit operations

/***
 *  Replaces in the entire scene graph of current execution context @a node by @a newValue.
 */
void
X3DBrowserEditor::replaceNodes (const X3D::SFNode & node, const X3D::SFNode & newValue, const UndoStepPtr & undoStep) const
{
   replaceNodes (X3D::SFNode (getExecutionContext ()), getExecutionContext () -> getRootNodes (), node, newValue, undoStep);

	X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & parent)
	               {
	                  for (auto & field: parent -> getFieldDefinitions ())
	                  {
	                     switch (field -> getType ())
	                     {
									case X3D::X3DConstants::SFNode:
										{
										   const auto sfnode = static_cast <X3D::SFNode*> (field);

											if (*sfnode == node)
											   replaceNode (parent, *sfnode, newValue, undoStep);

										   break;
										}
									case X3D::X3DConstants::MFNode:
										{
										   const auto mfnode = static_cast <X3D::MFNode*> (field);

										   replaceNodes (parent, *mfnode, node, newValue, undoStep);
										   break;
										}
									default:
										break;
								}
							}

	                  return true;
						});
}

/***
 *  Sets @a sfnode to @a newValue.
 */
void
X3DBrowserEditor::replaceNode (const X3D::SFNode & parent, X3D::SFNode & sfnode, const X3D::SFNode & newValue, const UndoStepPtr & undoStep) const
{
	const auto oldValue = sfnode;

	undoStep -> addObjects (parent);

	undoStep -> addUndoFunction (&X3D::SFNode::setValue, std::ref (sfnode), sfnode);
	undoStep -> addRedoFunction (&X3D::SFNode::setValue, std::ref (sfnode), newValue);

	sfnode = newValue;

	removeNodeFromSceneIfNotExists (getExecutionContext (), oldValue, undoStep);
}

/***
 *  Replaces in @a mfnode all occurences of @a node by @a newValue.
 */
void
X3DBrowserEditor::replaceNodes (const X3D::SFNode & parent, X3D::MFNode & mfnode, const X3D::SFNode & node, const X3D::SFNode & newValue, const UndoStepPtr & undoStep) const
{
	const auto indices = mfnode .indices_of (node);

	if (indices .empty ())
		return;

	for (const auto & index : indices)
		replaceNode (parent, mfnode, index, newValue, undoStep);
}

/***
 *  Sets in @a mfnode at @a index the value to @a newValue.
 */
void
X3DBrowserEditor::replaceNode (const X3D::SFNode & parent, X3D::MFNode & mfnode, const size_t index, const X3D::SFNode & newValue, const UndoStepPtr & undoStep) const
{
	const X3D::SFNode oldValue = mfnode [index];

	undoStep -> addObjects (parent);

	using set1Value = void (X3D::MFNode::*) (const X3D::MFNode::size_type, const X3D::SFNode &);

	undoStep -> addUndoFunction ((set1Value) & X3D::MFNode::set1Value, std::ref (mfnode), index, mfnode [index]);
	undoStep -> addRedoFunction ((set1Value) & X3D::MFNode::set1Value, std::ref (mfnode), index, newValue);

	mfnode [index] = newValue;

	removeNodeFromSceneIfNotExists (getExecutionContext (), oldValue, undoStep);
}

void
X3DBrowserEditor::removeNode (const X3D::SFNode & parent, X3D::SFNode & node, const UndoStepPtr & undoStep) const
{
	replaceNode (parent, node, nullptr, undoStep);
}

void
X3DBrowserEditor::removeNode (const X3D::SFNode & parent, X3D::MFNode & mfnode, const X3D::SFNode & node, const UndoStepPtr & undoStep) const
{
	const auto indices = mfnode .indices_of (node);

	if (indices .empty ())
		return;

	undoStep -> addObjects (parent);

	undoStep -> addUndoFunction (&X3DBrowserEditor::undoEraseNode, this,
	                             std::ref (mfnode),
	                             node,
	                             indices);

	using removeNode = void (X3D::MFNode::*) (const X3D::SFNode &);

	undoStep -> addRedoFunction ((removeNode) & X3D::MFNode::remove, std::ref (mfnode), node);

	mfnode .remove (node);
}

void
X3DBrowserEditor::removeNode (const X3D::SFNode & parent, X3D::MFNode & mfnode, const size_t index, const UndoStepPtr & undoStep) const
{
	const X3D::SFNode oldValue = mfnode [index];

	undoStep -> addObjects (parent);

	undoStep -> addUndoFunction (&X3DBrowserEditor::undoEraseNode, this,
	                             std::ref (mfnode),
	                             mfnode [index],
	                             std::vector <size_t> ({ index }));

	undoStep -> addRedoFunction (&X3DBrowserEditor::eraseNode, this, std::ref (mfnode), index, oldValue);

	eraseNode (mfnode, index, oldValue);

	removeNodeFromSceneIfNotExists (getExecutionContext (), oldValue, undoStep);
}

void
X3DBrowserEditor::eraseNode (X3D::MFNode & mfnode, const size_t index, const X3D::SFNode & value) const
{
	if (index < mfnode .size () and mfnode [index] == value)
		mfnode .erase (mfnode .begin () + index);
}

/***
 *  Removes @a node completely from scene if not exists in scene graph anymore.
 */
void
X3DBrowserEditor::removeNodeFromSceneIfNotExists (const X3D::X3DExecutionContextPtr & executionContext, const X3D::SFNode & node, const UndoStepPtr & undoStep) const
{
	if (node)
	{
		if (node -> getCloneCount () == 0)
			removeNodesFromScene (executionContext, { node }, undoStep, false);
	}
}

/***
 *  Does the same as Menu > Edit > Remove
 */
void
X3DBrowserEditor::removeNodesFromScene (const X3D::X3DExecutionContextPtr & executionContext, X3D::MFNode nodes, const UndoStepPtr & undoStep, const bool doRemoveFromSceneGraph) const
{
	// Remove exported nodes

	if (doRemoveFromSceneGraph)
		removeNodeFromSceneGraph (executionContext, std::set <X3D::SFNode> ( nodes .begin (), nodes .end () ), undoStep);

	// Delete children of node if not in scene graph

	// Collect children

	std::set <X3D::SFNode> children;

	X3D::traverse (nodes, [&children] (X3D::SFNode & child)
	               {
	                  children .emplace (child);
	                  return true;
						},
	               true,
	               X3D::TRAVERSE_PROTOTYPE_INSTANCES);

	if (children .size () > nodes .size ())
	{
		// Filter out nodes still in the scene graph.

		X3D::traverse (executionContext -> getRootNodes (), [&children] (X3D::SFNode & node)
		               {
		                  children .erase (node);
		                  return true;
							},
		               true,
		               X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}

	// Remove rest, these are only nodes that are not in the scene graph anymore.
	removeNodesFromExecutionContext (executionContext, children, undoStep, false);
}

void
X3DBrowserEditor::removeNodesFromExecutionContext (const X3D::X3DExecutionContextPtr & executionContext,
                                                   const std::set <X3D::SFNode> & nodes,
                                                   const UndoStepPtr & undoStep,
                                                   const bool doRemoveFromSceneGraph) const
{
	// Remove node from scene graph

	const X3D::X3DPtr <X3D::X3DScene> scene (executionContext);

	if (scene)
		removeExportedNodes (scene, nodes, undoStep);

	for (const auto & node : nodes)
		deleteRoutes (executionContext, node, undoStep);

	std::set <X3D::InlinePtr> inlineNodes;

	for (const auto & node : nodes)
	{
		X3D::InlinePtr inlineNode (node);

		if (inlineNode and inlineNode -> load ())
			inlineNodes .emplace (inlineNode);
	}
	
	removeImportedNodes (executionContext, inlineNodes, undoStep);				
	removeNamedNodes (executionContext, nodes, undoStep);

	// If it is previously known that the node isn't in the scene graph anymore, it must not removed.

	if (doRemoveFromSceneGraph)
		removeNodeFromSceneGraph (executionContext, nodes, undoStep);

	// Hide node

	for (const auto & node : nodes)
	{
		undoStep -> addUndoFunction (&X3D::X3DBaseNode::beginUpdate, node);
		undoStep -> addRedoFunction (&X3D::X3DBaseNode::endUpdate,   node);
		node -> endUpdate ();

		using isPrivate = void (X3D::X3DBaseNode::*) (const bool);

		undoStep -> addUndoFunction ((isPrivate) & X3D::X3DBaseNode::isPrivate, node, false);
		undoStep -> addRedoFunction ((isPrivate) & X3D::X3DBaseNode::isPrivate, node, true);
		node -> isPrivate (true);
	}
}

void
X3DBrowserEditor::removeNodeFromSceneGraph (const X3D::X3DExecutionContextPtr & executionContext, const std::set <X3D::SFNode> & nodes, const UndoStepPtr & undoStep) const
{
	const X3D::SFNode executionContextNode (executionContext);

	for (const auto & node : nodes)
		removeNode (executionContextNode, executionContext -> getRootNodes (), node, undoStep);

	// Remove node from scene graph

	X3D::traverse (executionContext -> getRootNodes (), [this, &nodes, &undoStep] (X3D::SFNode & parent)
	               {
	                  for (auto & field: parent -> getFieldDefinitions ())
	                  {
	                     switch (field -> getType ())
	                     {
									case X3D::X3DConstants::SFNode:
										{
										   const auto sfnode = static_cast <X3D::SFNode*> (field);

										   if (nodes .count (*sfnode))
										   {
										      undoStep -> addObjects (parent);

										      undoStep -> addUndoFunction (&X3D::SFNode::setValue, sfnode, *sfnode);
										      undoStep -> addRedoFunction (&X3D::SFNode::setValue, sfnode, nullptr);

										      sfnode -> setValue (nullptr);
											}

										   break;
										}
									case X3D::X3DConstants::MFNode:
										{
										   const auto mfnode = static_cast <X3D::MFNode*> (field);

											for (const auto & node : nodes)
										      removeNode (parent, *mfnode, node, undoStep);
										   break;
										}
									default:
										break;
								}
							}

	                  return true;
						},
	               true, X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

void
X3DBrowserEditor::removeExportedNodes (const X3D::X3DPtr <X3D::X3DScene> & scene, const std::set <X3D::SFNode> & nodes, const UndoStepPtr & undoStep) const
{
	// Remove exported nodes

	for (const auto & pair : X3D::ExportedNodeIndex (scene -> getExportedNodes ()))
	{
		try
		{
			const auto & exportedNode = pair .second;
			const auto   localNode    = exportedNode -> getLocalNode ();

			if (nodes .count (localNode))
			{
				undoStep -> addUndoFunction (&X3D::Scene::updateExportedNode, scene, exportedNode -> getExportedName (), localNode);
				undoStep -> addRedoFunction (&X3D::Scene::removeExportedNode, scene, exportedNode -> getExportedName ());

				scene -> removeExportedNode (exportedNode -> getExportedName ());
			}
		}
		catch (...)
		{ }
	}
}

void
X3DBrowserEditor::updateNamedNode (const X3D::X3DExecutionContextPtr & executionContext, const std::string & name, const X3D::SFNode & node, const UndoStepPtr & undoStep) const
{
	try
	{
		if (name .empty ())
			return;

		executionContext -> updateNamedNode (name, node);
		undoStep -> addUndoFunction (&X3D::X3DExecutionContext::removeNamedNode, executionContext, name);
		undoStep -> addRedoFunction (&X3D::X3DExecutionContext::updateNamedNode, executionContext, name, node);
	}
	catch (...)
	{ }
}

void
X3DBrowserEditor::removeNamedNodes (const X3D::X3DExecutionContextPtr & executionContext, const std::set <X3D::SFNode> & nodes, const UndoStepPtr & undoStep) const
{
	// Remove named node

	for (const auto & node : nodes)
	{
		try
		{
			if (executionContext -> getNamedNode (node -> getName ()) == node)
			{
				undoStep -> addUndoFunction (&X3D::X3DExecutionContext::updateNamedNode, executionContext, node -> getName (), node);
				undoStep -> addRedoFunction (&X3D::X3DExecutionContext::removeNamedNode, executionContext, node -> getName ());

				executionContext -> removeNamedNode (node -> getName ());
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

/***
 *  Only pass inline nodes that are loaded and should be unloaded.
 */
void
X3DBrowserEditor::removeImportedNodes (const X3D::X3DExecutionContextPtr & executionContext, const std::set <X3D::InlinePtr> & inlineNodes, const UndoStepPtr & undoStep) const
{
	// Remove nodes imported from node
	
	std::set <X3D::InlinePtr> immediateNodes;

	for (const auto & pair : X3D::ImportedNodeIndex (executionContext -> getImportedNodes ()))
	{
		try
		{
			const auto & importedNode = pair .second;
			const auto   inlineNode   = importedNode -> getInlineNode ();

			if (inlineNodes .count (inlineNode))
			{
				immediateNodes .emplace (inlineNode);

				// Delete routes.

				try
				{
					deleteRoutes (executionContext, importedNode -> getExportedNode (), undoStep);
				}
				catch (const X3D::X3DError &)
				{ }

				// Remove imported node.

				undoStep -> addUndoFunction (&X3D::X3DExecutionContext::addImportedNode, executionContext,
				                             inlineNode,
				                             importedNode -> getExportedName (),
				                             importedNode -> getImportedName ());

				undoStep -> addRedoFunction (&X3D::X3DExecutionContext::removeImportedNode, executionContext,
				                             importedNode -> getImportedName ());

				executionContext -> removeImportedNode (importedNode -> getImportedName ());
			}
		}
		catch (const X3D::X3DError & error)
		{ }
	}

	for (const auto & inlineNode : inlineNodes)
	{
		if (immediateNodes .count (inlineNode))
		{
			undoStep -> addUndoFunction (&X3D::Inline::requestImmediateLoad, inlineNode);
			undoStep -> addUndoFunction (&X3D::Inline::preventNextLoad, inlineNode); // Prevent next load from load field event.
		}

		undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (inlineNode -> load ()), true);
		undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (inlineNode -> load ()), false);
		inlineNode -> load () = false;
	}
}

void
X3DBrowserEditor::deleteRoutes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::SFNode & node, const UndoStepPtr & undoStep) const
{
	// Delete routes from and to node

	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (not field -> getInputRoutes () .empty ())
		{
			for (const auto & route : X3D::RouteSet (field -> getInputRoutes ()))
			{
				try
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					deleteRoute (route -> getExecutionContext (),
					             route -> getSourceNode (),
					             route -> getSourceField (),
					             route -> getDestinationNode (),
					             route -> getDestinationField (),
					             undoStep);
				}
				catch (const X3D::X3DError &)
				{ }
			}
		}

		if (not field -> getOutputRoutes () .empty ())
		{
			for (const auto & route : X3D::RouteSet (field -> getOutputRoutes ()))
			{
				try
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					deleteRoute (route -> getExecutionContext (),
					             route -> getSourceNode (),
					             route -> getSourceField (),
					             route -> getDestinationNode (),
					             route -> getDestinationField (),
					             undoStep);
				}
				catch (const X3D::X3DError &)
				{ }
			}
		}
	}
}

void
X3DBrowserEditor::addRoute (const X3D::X3DExecutionContextPtr & executionContext,
                            const X3D::SFNode & sourceNode,
                            const std::string & sourceField,
                            const X3D::SFNode & destinationNode,
                            const std::string & destinationField,
                            const UndoStepPtr & undoStep) const
throw (X3D::Error <X3D::INVALID_NODE>,
       X3D::Error <X3D::INVALID_FIELD>,
       X3D::Error <X3D::INVALID_OPERATION_TIMING>,
       X3D::Error <X3D::DISPOSED>)
{
	try
	{
		using addRoute    = const X3D::RoutePtr & (X3D::X3DExecutionContext::*) (const X3D::SFNode &, const std::string &, const X3D::SFNode &, const std::string &);
		using deleteRoute = void (X3D::X3DExecutionContext::*) (const X3D::SFNode &, const std::string &, const X3D::SFNode &, const std::string &);
		using NodeFn      = std::function <X3D::SFNode ()>;

		const auto identity = ( [ ] (const X3D::SFNode & value){ return value; });
		const auto call     = ( [ ] (const NodeFn &value){ return value (); });

		const NodeFn sourceNodeFn = executionContext -> isImportedNode (sourceNode)
		                            ? NodeFn (std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)))
											 : NodeFn (std::bind (identity, sourceNode));

		const NodeFn destinationNodeFn = executionContext -> isImportedNode (destinationNode)
		                                 ? NodeFn (std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)))
													: NodeFn (std::bind (identity, destinationNode));

		undoStep -> addUndoFunction ((deleteRoute) & X3D::X3DExecutionContext::deleteRoute, executionContext,
		                             std::bind (call, sourceNodeFn),
		                             sourceField,
		                             std::bind (call, destinationNodeFn),
		                             destinationField);

		undoStep -> addRedoFunction ((addRoute) &X3D::X3DExecutionContext::addRoute, executionContext,
		                             std::bind (call, sourceNodeFn),
		                             sourceField,
		                             std::bind (call, destinationNodeFn),
		                             destinationField);

		executionContext -> addRoute (sourceNode, sourceField, destinationNode, destinationField);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DBrowserEditor::deleteRoute (const X3D::X3DExecutionContextPtr & executionContext,
                               const X3D::SFNode & sourceNode,
                               const std::string & sourceField,
                               const X3D::SFNode & destinationNode,
                               const std::string & destinationField,
                               const UndoStepPtr & undoStep) const
{
	using addRoute    = const X3D::RoutePtr & (X3D::X3DExecutionContext::*) (const X3D::SFNode &, const std::string &, const X3D::SFNode &, const std::string &);
	using deleteRoute = void (X3D::X3DExecutionContext::*) (const X3D::SFNode &, const std::string &, const X3D::SFNode &, const std::string &);

	if (sourceNode -> getRootContext () -> isPrivate ())
		return;

	if (destinationNode -> getRootContext () -> isPrivate ())
		return;

	bool sourceImported      = false;
	bool destinationImported = false;

	try
	{
		sourceImported = executionContext -> isImportedNode (sourceNode);
	}
	catch (...)
	{ }

	try
	{
		destinationImported = executionContext -> isImportedNode (destinationNode);
	}
	catch (...)
	{ }

	if (sourceImported and destinationImported)
	{
		undoStep -> addUndoFunction ((addRoute) &X3D::X3DExecutionContext::addRoute, executionContext,
		                             std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)),
		                             sourceField,
		                             std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)),
		                             destinationField);

		undoStep -> addRedoFunction ((deleteRoute) & X3D::X3DExecutionContext::deleteRoute, executionContext,
		                             std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)),
		                             sourceField,
		                             std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)),
		                             destinationField);
	}
	else if (sourceImported and not destinationImported)
	{
		undoStep -> addUndoFunction ((addRoute) &X3D::X3DExecutionContext::addRoute, executionContext,
		                             std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)),
		                             sourceField,
		                             destinationNode,
		                             destinationField);

		undoStep -> addRedoFunction ((deleteRoute) & X3D::X3DExecutionContext::deleteRoute, executionContext,
		                             std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)),
		                             sourceField,
		                             destinationNode,
		                             destinationField);
	}
	else if (not sourceImported and destinationImported)
	{
		undoStep -> addUndoFunction ((addRoute) &X3D::X3DExecutionContext::addRoute, executionContext,
		                             sourceNode,
		                             sourceField,
		                             std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)),
		                             destinationField);

		undoStep -> addRedoFunction ((deleteRoute) & X3D::X3DExecutionContext::deleteRoute, executionContext,
		                             sourceNode,
		                             sourceField,
		                             std::bind (&X3D::X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)),
		                             destinationField);
	}
	else
	{
		undoStep -> addUndoFunction ((addRoute) &X3D::X3DExecutionContext::addRoute, executionContext,
		                             sourceNode,
		                             sourceField,
		                             destinationNode,
		                             destinationField);

		undoStep -> addRedoFunction ((deleteRoute) & X3D::X3DExecutionContext::deleteRoute, executionContext,
		                             sourceNode,
		                             sourceField,
		                             destinationNode,
		                             destinationField);
	}

	executionContext -> deleteRoute (sourceNode, sourceField, destinationNode, destinationField);
}

void
X3DBrowserEditor::createClone (const X3D::SFNode & clone, const X3D::MFNode & nodes, const UndoStepPtr & undoStep) const
{
	for (const auto & node : nodes)
	{
		if (not node)
			continue;

		X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & parent)
		               {
		                  for (auto & field: parent -> getFieldDefinitions ())
		                  {
		                     switch (field -> getType ())
		                     {
										case X3D::X3DConstants::SFNode:
											{
											   const auto sfnode = static_cast <X3D::SFNode*> (field);

											   if (*sfnode == node)
													replaceNode (parent, *sfnode, clone, undoStep);

											   break;
											}
										case X3D::X3DConstants::MFNode:
											{
											   const auto mfnode = static_cast <X3D::MFNode*> (field);

											   replaceNodes (parent, *mfnode, node, clone, undoStep);
											   break;
											}
										default:
											break;
									}
								}

		                  return true;
							},
		               true, X3D::TRAVERSE_PROTOTYPE_INSTANCES);

		replaceNodes (getExecutionContext () .getValue (), getExecutionContext () -> getRootNodes (), node, clone, undoStep);
	}
}

X3D::MFNode
X3DBrowserEditor::unlinkClone (const X3D::MFNode & clones, const UndoStepPtr & undoStep) const
{
	X3D::MFNode nodes;

	for (const auto & clone : clones)
	{
		nodes .emplace_back (clone);

		bool first = true;

		X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & parent)
		               {
		                  for (auto & field: parent -> getFieldDefinitions ())
		                  {
		                     switch (field -> getType ())
		                     {
										case X3D::X3DConstants::SFNode:
											{
											   try
											   {
											      const auto sfnode = static_cast <X3D::SFNode*> (field);

											      if (*sfnode == clone)
											      {
											         if (not first)
											         {
											            const X3D::SFNode copy = (*sfnode) -> copy (X3D::FLAT_COPY);

											            replaceNode (parent, *sfnode, copy, undoStep);

											            nodes .emplace_back (copy);
														}

											         first = false;
													}
												}
											   catch (const X3D::Error <X3D::NOT_SUPPORTED> &)
											   { }

											   break;
											}
										case X3D::X3DConstants::MFNode:
											{
											   const auto mfnode = static_cast <X3D::MFNode*> (field);

											   unlinkClone (parent, *mfnode, clone, nodes, first, undoStep);

											   break;
											}
										default:
											break;
									}
								}

		                  return true;
							},
		               true, X3D::TRAVERSE_PROTOTYPE_INSTANCES);

		// Unlink in rootNodes array.

		unlinkClone (getExecutionContext () .getValue (), getExecutionContext () -> getRootNodes (), clone, nodes, first, undoStep);
	}

	// Setup new nodes.
	getExecutionContext () -> realize ();

	return nodes;
}

void
X3DBrowserEditor::unlinkClone (const X3D::SFNode & parent,
                               X3D::MFNode & mfnode,
                               const X3D::SFNode & clone,
                               X3D::MFNode & nodes,
                               bool & first,
                               const UndoStepPtr & undoStep) const
{
	const auto indices = mfnode .indices_of (clone);

	if (indices .empty ())
		return;

	for (const auto & index : indices)
	{
		try
		{
			if (not first)
			{
				const X3D::SFNode copy = mfnode [index] -> copy (X3D::FLAT_COPY);

				replaceNode (parent, mfnode, index, copy, undoStep);

				nodes .emplace_back (copy);
			}

			first = false;
		}
		catch (const X3D::Error <X3D::NOT_SUPPORTED> &)
		{ }
	}
}

// Grouping operations

X3D::SFNode
X3DBrowserEditor::groupNodes (const std::string & typeName,
                              const X3D::MFNode & nodes,
                              const UndoStepPtr & undoStep) const
{
	const X3D::X3DPtr <X3D::X3DGroupingNode> group (getExecutionContext () -> createNode (typeName));

	undoStep -> addObjects (group);

	for (const auto & child : nodes)
	{
		if (not child)
			continue;

		// Adjust transformation
		X3D::Matrix4d                  childModelViewMatrix = findModelViewMatrix (child);
		const X3D::X3DTransformNodePtr transform (child);

		if (transform)
		{
			childModelViewMatrix .mult_left (transform -> getMatrix ());

			setMatrix (transform, childModelViewMatrix, undoStep);
		}

		// Remove child from scene graph

		removeNodeFromSceneGraph (getExecutionContext (), { child }, undoStep);

		// Add to group

		emplaceBack (group -> children (), child, undoStep);
	}

	group -> setup ();

	return X3D::SFNode (group);
}

X3D::MFNode
X3DBrowserEditor::ungroupNodes (const X3D::MFNode & groups,
                                const UndoStepPtr & undoStep) const
{
	X3D::MFNode children;

	for (const auto & node : groups)
	{
		try
		{
			const X3D::X3DGroupingNodePtr group (node);

			if (not group)
				continue;

			const auto layers = X3D::findParents <X3D::X3DLayerNode> (node);

			undoStep -> addObjects (node);

			// Ungroup children

			const auto groupingField = static_cast <X3D::MFNode*> (node -> getField ("children"));

			for (const auto & child : *groupingField)
			{
				if (not child)
					continue;

				// Adjust transformation

				X3D::Matrix4d                  childModelViewMatrix = findModelViewMatrix (child);
				const X3D::X3DTransformNodePtr transform (child);

				if (transform)
				{
					childModelViewMatrix .mult_left (transform -> getMatrix ());

					setMatrix (transform, childModelViewMatrix, undoStep);
				}

				// Add to layer

				for (const auto & layer : layers)
				{
					if (layer -> isLayer0 ())
						emplaceBack (getExecutionContext () -> getRootNodes (), child, undoStep);

					else
					{
						undoStep -> addObjects (X3D::SFNode (layer));

						emplaceBack (layer -> children (), child, undoStep);
					}
				}

				undoStep -> addUndoFunction (&X3D::MFNode::push_front, groupingField, child);

				children .emplace_back (child);
			}

			undoStep -> addRedoFunction (&X3D::MFNode::clear, groupingField);

			groupingField -> clear ();

			// Remove group from scene

			removeNodesFromScene (getExecutionContext (), { node }, undoStep);
		}
		catch (const X3D::Error <X3D::INVALID_NODE> &)
		{
			// No grouping field found
		}
	}

	return children;
}

bool
X3DBrowserEditor::addToGroup (const X3D::SFNode & group,
                              const X3D::MFNode & children,
                              const UndoStepPtr & undoStep) const
{
	if (not group)
		return false;

	bool added = false;

	undoStep -> addObjects (group);

	for (const auto & child : children)
	{
		try
		{
			if (child == group)
				continue;

			if (not child)
				continue;

			const auto containerField = getContainerField (group, child);

			// Get group modelview matrix

			X3D::Matrix4d                          groupModelViewMatrix = findModelViewMatrix (group);
			const X3D::X3DTransformMatrix4DNodePtr transform (group);

			if (transform)
				groupModelViewMatrix .mult_left (transform -> getMatrix ());

			// Adjust child transformation

			{
				X3D::Matrix4d                  childModelViewMatrix = findModelViewMatrix (child);
				const X3D::X3DTransformNodePtr transform (child);

				if (transform)
				{
					try
					{
						childModelViewMatrix .mult_left (transform -> getMatrix ());
						childModelViewMatrix .mult_right (~groupModelViewMatrix);

						setMatrix (transform, childModelViewMatrix, undoStep);
					}
					catch (const std::domain_error & error)
					{ }
				}
			}

			// Remove child from scene graph

			removeNodeFromSceneGraph (getExecutionContext (), { child }, undoStep);

			// Add child to group

			const auto sfnode = dynamic_cast <X3D::SFNode*> (containerField);

			if (sfnode)
			{
				undoStep -> addUndoFunction (&X3D::SFNode::setValue, sfnode, *sfnode);
				undoStep -> addRedoFunction (&X3D::SFNode::setValue, sfnode, child);

				sfnode -> setValue (child);

				added = true;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                // XXX Remove previous child completely from scene if not in scene anymore
			}
			else
			{
				const auto mfnode = dynamic_cast <X3D::MFNode*> (containerField);

				if (mfnode)
				{
					emplaceBack (*mfnode, child, undoStep);

					added = true;
				}
			}
		}
		catch (const X3D::Error <X3D::INVALID_NODE> &)
		{
			// No container field found
		}
	}

	return added;
}

void
X3DBrowserEditor::detachFromGroup (X3D::MFNode children,
                                   const bool detachToLayer0,
                                   const UndoStepPtr & undoStep) const
{
	for (const auto & child : children)
	{
		// Get layers before removing from scene graph.

		const auto layers = X3D::findParents <X3D::X3DLayerNode> (child);

		// Adjust transformation

		const X3D::X3DTransformNodePtr transform (child);

		if (transform)
		{
			X3D::Matrix4d childModelViewMatrix = findModelViewMatrix (child);

			childModelViewMatrix .mult_left (transform -> getMatrix ());

			setMatrix (transform, childModelViewMatrix, undoStep);
		}

		// Remove child from scene graph

		removeNodeFromSceneGraph (getExecutionContext (), { child }, undoStep);

		// Add to layers

		if (detachToLayer0)
			emplaceBack (getExecutionContext () -> getRootNodes (), child, undoStep);

		else
		{
			for (const auto & layer : layers)
			{
				if (layer -> isLayer0 ())
					emplaceBack (getExecutionContext () -> getRootNodes (), child, undoStep);

				else
				{
					undoStep -> addObjects (X3D::SFNode (layer));

					emplaceBack (layer -> children (), child, undoStep);
				}
			}
		}
	}
}

X3D::SFNode
X3DBrowserEditor::createParentGroup (const std::string & typeName,
                                     const X3D::MFNode & children,
                                     const UndoStepPtr & undoStep) const
{
	// Add node to group

	const X3D::X3DPtr <X3D::X3DGroupingNode> group (getExecutionContext () -> createNode (typeName));

	getExecutionContext () -> addUninitializedNode (group);
	undoStep -> addObjects (group);

	for (const auto & child : children)
	{
		if (not child)
			continue;

		emplaceBack (group -> children (), child, undoStep);

		X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & parent)
		               {
		                  for (auto & field: parent -> getFieldDefinitions ())
		                  {
		                     switch (field -> getType ())
		                     {
										case X3D::X3DConstants::SFNode:
											{
											   const auto sfnode = static_cast <X3D::SFNode*> (field);

											   if (*sfnode == child)
											   {
											      // Replace node with Transform

											      undoStep -> addObjects (parent);

											      undoStep -> addUndoFunction (&X3D::SFNode::setValue, sfnode, child);
											      undoStep -> addRedoFunction (&X3D::SFNode::setValue, sfnode, group);

											      sfnode -> setValue (group);
												}

											   break;
											}
										case X3D::X3DConstants::MFNode:
											{
											   const auto mfnode = static_cast <X3D::MFNode*> (field);

											   createParentGroup (group, *mfnode, child, parent, undoStep);

											   break;
											}
										default:
											break;
									}
								}

		                  return true;
							});

		createParentGroup (group, getExecutionContext () -> getRootNodes (), child, X3D::SFNode (getExecutionContext ()), undoStep);
	}

	getExecutionContext () -> realize ();
	return X3D::SFNode (group);
}

void
X3DBrowserEditor::createParentGroup (const X3D::X3DPtr <X3D::X3DGroupingNode> & group,
                                     X3D::MFNode & mfnode,
                                     const X3D::SFNode & child,
                                     const X3D::SFNode & parent,
                                     const UndoStepPtr & undoStep) const
{
	const auto indices = mfnode .indices_of (child);

	if (indices .empty ())
		return;

	undoStep -> addObjects (parent);

	for (const auto & index : indices)
	{
		// Replace node with Transform

		using set1Value = void (X3D::MFNode::*) (const X3D::MFNode::size_type, const X3D::SFNode &);

		undoStep -> addUndoFunction ((set1Value) & X3D::MFNode::set1Value, std::ref (mfnode), index, child);
		undoStep -> addRedoFunction ((set1Value) & X3D::MFNode::set1Value, std::ref (mfnode), index, X3D::SFNode (group));

		mfnode [index] = group;
	}
}

void
X3DBrowserEditor::addPrototypeInstance (const std::string & name)
{
	try
	{
		const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Create %s"), name .c_str ()));

		const X3D::SFNode instance (getExecutionContext () -> createPrototypeInstance (name));

		getExecutionContext () -> addUninitializedNode (instance);
		getExecutionContext () -> realize ();

		emplaceBack (getExecutionContext () -> getRootNodes (), instance, undoStep);

		getSelection () -> setChildren ({ instance }, undoStep);
		addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

// Undo functions

void
X3DBrowserEditor::translateSelection (const X3D::Vector3f & translation, const bool alongFrontPlane)
{
	for (const auto & node : basic::make_reverse_range (getBrowser () -> getSelection () -> getChildren ()))
	{
		X3D::X3DTransformNodePtr transform (node);

		if (transform)
		{
			using setValue = void (X3D::SFVec3f::*) (const X3D::Vector3f &);

			const auto undoStep = std::make_shared <UndoStep> (_ ("Nudge"));

			getSelection () -> redoRestoreSelection (undoStep);

			undoStep -> addObjects (node);
			undoStep -> addUndoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (transform -> translation ()), transform -> translation ());
			undoStep -> addRedoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (transform -> translation ()), transform -> translation () + translation);

			transform -> translation () += translation;

			getSelection () -> undoRestoreSelection (undoStep);

			addUndoStep (undoStep);
			break;
		}
	}
}

void
X3DBrowserEditor::saveMatrix (const X3D::SFNode & node, const UndoStepPtr & undoStep) const
{
	X3D::X3DTransformNodePtr transform (node);

	if (transform)
	{
		undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrix,
		                             transform,
		                             transform -> getMatrix ());
	}
}

void
X3DBrowserEditor::setMatrix (const X3D::X3DTransformNodePtr & transform, const X3D::Matrix4d & matrix, const UndoStepPtr & undoStep) const
{
	undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrix,
	                             transform,
	                             transform -> getMatrix ());

	undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrix,
	                             transform,
	                             matrix);

	transform -> setMatrix (matrix);
}

void
X3DBrowserEditor::emplaceBack (X3D::MFNode & array, const X3D::SFNode & node, const UndoStepPtr & undoStep) const
{
	// Add to group

	undoStep -> addUndoFunction (&X3DBrowserEditor::undoInsertNode, this,
	                             std::ref (array),
	                             array .size (),
	                             node);

	undoStep -> addRedoFunction (&X3D::MFNode::push_back, std::ref (array), node);

	array .emplace_back (node);
}

void
X3DBrowserEditor::undoInsertNode (X3D::MFNode & field, size_t index, const X3D::SFNode & node) const
{
	if (index < field .size () and field [index] == node)
	{
		field .erase (field .begin () + index);
	}
	else
	{
		// There has something changed, clear history.
		__LOG__ << std::endl;
	}
}

void
X3DBrowserEditor::undoEraseNode (X3D::MFNode & field, const X3D::SFNode & value, const std::vector <size_t> & indices) const
{
	for (const auto & index : indices)
	{
		if (index <= field .size ())
		{
			field .insert (field .begin () + index, value);
		}
		else
		{
			// There is has something changed, clear history
			__LOG__ << "WARNING" << std::endl;
		}
	}
}

// Misc

X3D::Matrix4d
X3DBrowserEditor::findModelViewMatrix (X3D::X3DBaseNode* const node) const
{
	X3D::Matrix4d modelViewMatrix;

	std::set <X3D::X3DBaseNode*> seen;

	for (const auto & parentNode : getParentNodes (node))
	{
		if (findModelViewMatrix (parentNode, modelViewMatrix, seen))
			break;
	}

	return modelViewMatrix;
}

bool
X3DBrowserEditor::findModelViewMatrix (X3D::X3DBaseNode* const node, X3D::Matrix4d & modelViewMatrix, std::set <X3D::X3DBaseNode*> & seen) const
{
	if (not seen .emplace (node) .second)
		return false;

	if (node == getExecutionContext ())
		return true;

	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		switch (type)
		{
			case X3D::X3DConstants::X3DLayerNode :
				return true;
			case X3D::X3DConstants::X3DProtoDeclarationNode:
			case X3D::X3DConstants::X3DScriptNode:
			case X3D::X3DConstants::X3DProgrammableShaderObject:
			case X3D::X3DConstants::X3DBaseNode:
				return false;
			case X3D::X3DConstants::X3DNode:
				goto NEXT;
			default:
				break;
		}
	}

NEXT:

	// Iterate over parents

	for (const auto & parentNode : getParentNodes (node))
	{
		if (findModelViewMatrix (parentNode, modelViewMatrix, seen))
		{
			const auto transform = dynamic_cast <X3D::X3DTransformMatrix4DNode*> (node);

			if (transform)
				modelViewMatrix .mult_left (transform -> getMatrix ());

			return true;
		}
	}

	return false;
}

std::vector <X3D::X3DBaseNode*>
X3DBrowserEditor::getParentNodes (X3D::X3DBaseNode* const child) const
{
	std::vector <X3D::X3DBaseNode*> parentNodes;

	for (const auto & parent : child -> getParents ())
	{
		const auto sfnode = dynamic_cast <X3D::SFNode*> (parent);

		if (sfnode)
		{
			for (const auto & secondParent : sfnode -> getParents ())
			{
				const auto mfnode = dynamic_cast <X3D::MFNode*> (secondParent);

				if (mfnode)
				{
					for (const auto & thirdParent : mfnode -> getParents ())
					{
						const auto baseNode = dynamic_cast <X3D::X3DBaseNode*> (thirdParent);

						if (baseNode)
						{
							if (baseNode not_eq child)
							{
								if (not baseNode -> isPrivate ())
									parentNodes .emplace_back (baseNode);
							}
						}
					}

					continue;
				}

				const auto baseNode = dynamic_cast <X3D::X3DBaseNode*> (secondParent);

				if (baseNode)
				{
					if (baseNode not_eq child)
					{
						if (not baseNode -> isPrivate ())
							parentNodes .emplace_back (baseNode);
					}

					continue;
				}
			}
		}
	}

	return parentNodes;
}

X3D::X3DFieldDefinition*
X3DBrowserEditor::getContainerField (const X3D::SFNode & parent, const X3D::SFNode & child) const
throw (X3D::Error <X3D::INVALID_NODE>)
{
	try
	{
		const auto field = parent -> getField (child -> getInnerNode () -> getContainerField ());

		if (field -> getAccessType () & X3D::initializeOnly)
		{
			if (field -> getType () == X3D::X3DConstants::SFNode or field -> getType () == X3D::X3DConstants::MFNode)
				return field;
		}
	}
	catch (const X3D::X3DError &)
	{ }

	// Find appropriate field

	try
	{
		const auto field = parent -> getField ("children");

		if (field -> getAccessType () == X3D::initializeOnly or field -> getAccessType () == X3D::inputOutput)
		{
			if (field -> getType () == X3D::X3DConstants::MFNode)
				return field;
		}
	}
	catch (const X3D::X3DError &)
	{ }

	// Find last MFNode

	for (auto & field : basic::make_reverse_range (parent -> getFieldDefinitions ()))
	{
		if (field -> getAccessType () == X3D::initializeOnly or field -> getAccessType () == X3D::inputOutput)
		{
			if (field -> getType () == X3D::X3DConstants::MFNode)
				return field;
		}
	}

	// Find last SFNode not metadata

	for (auto & field : basic::make_reverse_range (parent -> getFieldDefinitions ()))
	{
		if (field -> getAccessType () == X3D::initializeOnly or field -> getAccessType () == X3D::inputOutput)
		{
			if (field -> getType () == X3D::X3DConstants::SFNode and field -> getName () not_eq "metadata")
				return field;
		}
	}

	throw X3D::Error <X3D::INVALID_NODE> ("No appropriate container field found.");
}

void
X3DBrowserEditor::editCDATA (const X3D::SFNode & node)
{
	X3D::MFString* const cdata          = node -> getCDATA ();
	std::string          filename       = "/tmp/titania-XXXXXX.js";
	const int            fileDescriptor = mkstemps (&filename [0], 3);

	if (not cdata or fileDescriptor == -1)
		return;

	// Output file.

	std::ofstream ostream (filename);

	for (const auto & string : *cdata)
	{
		ostream
			<< "<![CDATA["
			<< X3D::escape_cdata (string)
			<< "]]>" << std::endl
			<< std::endl
			<< std::endl;
	}

	std::string name = node -> getName ();
	pcrecpp::RE (R"/((\*/))/") .GlobalReplace ("", &name);

	ostream
		<< "/**" << std::endl
		<< " * " << node -> getTypeName () << " " << name << std::endl
		<< " * " << _ ("This file is automaticaly generated to edit CDATA fields. Each SFString value is enclosed inside a CDATA") << std::endl
		<< " * " << _ ("section.  A CDATA section starts with \"<![CDATA[javascript:\" and ends with \"]]>\".") << std::endl
		<< " * " << std::endl
		<< " * " << _ ("If this is a Script node, a inline script must be enclosed in \"<![CDATA[javascript: your program here ]]>\".") << std::endl
		<< " * " << _ ("If this is a shader, it must be enclosed in \"<![CDATA[data:text/plain, your shader here ]]>\".  Just save") << std::endl
		<< " * " << _ ("this file to apply changes.") << std::endl
		<< " **/" << std::endl;

	// Launch Gnome TextEditor.

	Glib::RefPtr <Gio::File>        file        = Gio::File::create_for_path (filename);
	Glib::RefPtr <Gio::FileMonitor> fileMonitor = file -> monitor_file ();

	fileMonitor -> signal_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DBrowserEditor::on_cdata_changed), node));
	fileMonitors .emplace (file, fileMonitor);

	try
	{
		Gio::AppInfo::create_from_commandline (os::realpath ("/usr/bin/gnome-text-editor"), "", Gio::APP_INFO_CREATE_NONE) -> launch (file);
	}
	catch (...)
	{ }

	::close (fileDescriptor);
}

void
X3DBrowserEditor::on_cdata_changed (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::File> &, Gio::FileMonitorEvent event, const X3D::SFNode & node)
{
	io::multi_line_comment comment ("/*", "*/");
	io::sequence           whitespaces ("\r\n \t,");
	io::inverse_string     cdata_start ("<![CDATA[");
	io::inverse_string     contents ("]]>");

	if (event not_eq Gio::FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
		return;

	// Parse file.

	X3D::MFString string;
	std::string   ws;

	std::ifstream istream (file -> get_path ());

	// Parse CDATA sections.

	while (istream)
	{
		whitespaces (istream, ws);
		comment (istream, ws);

		if (cdata_start (istream, ws))
		{
			std::string value;

			contents (istream, value);

			string .emplace_back (std::move (value));
		}
		else
			break;
	}

	// Set value.

	X3D::MFString* const cdata = node -> getCDATA ();

	if (string not_eq *cdata)
	{
		const auto undoStep = std::make_shared <UndoStep> (_ ("Edit CDATA Field"));

		undoStep -> addObjects (node);
		undoStep -> addUndoFunction (&OutlineTreeViewEditor::queue_draw, getBrowserWindow () -> getOutlineTreeView ());

		undoStep -> addUndoFunction (&X3D::MFString::setValue, cdata, *cdata);
		undoStep -> addRedoFunction (&X3D::MFString::setValue, cdata, string);
		cdata -> setValue (string);

		undoStep -> addRedoFunction (&OutlineTreeViewEditor::queue_draw, getBrowserWindow () -> getOutlineTreeView ());
		getBrowserWindow () -> getOutlineTreeView () -> queue_draw ();

		addUndoStep (undoStep);
	}

	getBrowser () -> println (X3D::SFTime (chrono::now ()) .toUTCString (), ": ", _ ("Script saved."));
}

void
X3DBrowserEditor::shutdown ()
{
	getBrowser () -> makeCurrent ();

	undoHistory  .clear ();

	for (const auto & fileMonitor : fileMonitors)
	{
		fileMonitor .second -> cancel ();
		fileMonitor .first -> remove ();
	}

	fileMonitors .clear ();
}

X3DBrowserEditor::~X3DBrowserEditor ()
{ }

} // puck
} // titania
