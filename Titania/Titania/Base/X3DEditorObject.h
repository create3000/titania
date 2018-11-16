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

#ifndef __TITANIA_BASE_X3DEDITOR_OBJECT_H__
#define __TITANIA_BASE_X3DEDITOR_OBJECT_H__

#include "../Base/X3DBaseInterface.h"
#include "../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Basic/FieldSet.h>
#include <Titania/X3D/Browser/X3DBrowser.h>
#include <Titania/X3D/Fields/X3DPtrArray.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Prototype/X3DProtoDeclarationNode.h>

#include <Titania/String/sprintf.h>
#include <Titania/String/to_string.h>
#include <Titania/Utility/Range.h>

#include <gtkmm.h>

namespace titania {
namespace puck {

class X3DEditorObject :
	virtual public X3DBaseInterface
{
public:

	///  @name Construction

	virtual
	void
	setup () override;

	///  @name Member access

	void
	setUndo (const bool value)
	{ undo = value; }

	bool
	getUndo () const
	{ return undo; }

	template <class NodeType>
	static
	X3D::X3DPtrArray <NodeType>
	getNodes (const X3D::MFNode & nodes, const std::set <X3D::X3DConstants::NodeType> & types, const bool traverse = true);

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }

	virtual
	~X3DEditorObject ()
	{ }


protected:

	///  @name Construction

	X3DEditorObject ();

	///  @name Construction

	virtual
	void
	initialize ()
	{ }

	///  @name Operations

	void
	validateIdOnInsert (Gtk::Entry &, const Glib::ustring &, int);

	void
	validateIdOnDelete (Gtk::Entry &, int, int);

	void
	validateFolderOnInsert (Gtk::Entry &, const Glib::ustring &, int);

	void
	validateFolderOnDelete (Gtk::Entry &, int, int);

	Glib::ustring
	refineName (const Glib::ustring &) const;

	template <class NodeType>
	X3D::X3DPtrArray <NodeType>
	getSelection (const std::set <X3D::X3DConstants::NodeType> & types, const bool traverse = true);

	template <class FieldType, class NodeType>
	std::tuple <X3D::X3DPtr <FieldType>, int32_t, int32_t>
	getSelection (const X3D::X3DPtrArray <NodeType> &, const std::string &);

	template <class FieldType, class NodeType>
	std::pair <X3D::X3DPtr <FieldType>, int32_t>
	getNode (const X3D::X3DPtrArray <NodeType> &, const std::string &);

	template <class NodeType>
	static
	int32_t
	getBoolean (const X3D::X3DPtrArray <NodeType> &, const std::string &);

	template <class FieldType, class NodeType>
	static
	std::pair <FieldType, int32_t>
	getArray (const X3D::X3DPtrArray <NodeType> &, const std::string &);

	template <class NodeType>
	void
	unlinkClone (const X3D::X3DPtrArray <NodeType> &, const std::string &, X3D::UndoStepPtr &);

	template <class FieldType, class NodeType>
	bool
	addUndoFunction (const X3D::X3DPtrArray <NodeType> &, const std::string &, X3D::UndoStepPtr &);

	template <class FieldType, class NodeType>
	bool
	addRedoFunction (const X3D::X3DPtrArray <NodeType> &, const std::string &, X3D::UndoStepPtr &);

	template <class FieldType, class NodeType>
	bool
	addUndoFunction (const X3D::X3DPtr <NodeType> &, FieldType &, X3D::UndoStepPtr &);

	template <class FieldType, class NodeType>
	bool
	addRedoFunction (const X3D::X3DPtr <NodeType> &, FieldType &, X3D::UndoStepPtr &);

	void
	resetUndoGroup (const std::string &, X3D::UndoStepPtr &);

	void
	beginUndoGroup (const std::string &, X3D::UndoStepPtr &);

	void
	endUndoGroup (const std::string &, X3D::UndoStepPtr &);

	void
	beginRedoGroup (const std::string &, X3D::UndoStepPtr &);

	void
	endRedoGroup (const std::string &, X3D::UndoStepPtr &);

	/// @name Static members
	
	static constexpr int32_t NO_FIELD     = -2;
	static constexpr int32_t INCONSISTENT = -1;
	static constexpr int32_t FIELD_NULL   =  0;
	static constexpr int32_t SAME_NODE    =  1;


private:

	///  @name Event handlers
	 
	void
	set_browser (const X3D::BrowserPtr &);

	///  @name Operations

	bool
	validateId (const std::string &) const;

	bool
	validateFolder (const std::string &) const;

	const X3D::MFNode &
	getSelectedNodes () const;

	///  @name Members

	bool                        undo;
	bool                        undoGroupChanged;
	std::string                 undoGroup;
	std::string                 redoGroup;
	std::string                 lastUndoGroup;
	std::string                 currentField;
	X3D::X3DPtr <X3D::FieldSet> fields;

};

template <class NodeType>
X3D::X3DPtrArray <NodeType>
X3DEditorObject::getNodes (const X3D::MFNode & nodes, const std::set <X3D::X3DConstants::NodeType> & types, const bool traverse)
{
	if (traverse)
		return X3D::X3DEditor::getNodes <NodeType> (nodes, types);

	X3D::X3DPtrArray <NodeType> found;

	for (const auto & node : nodes)
	{
		if (not node)
			continue;

		if (node -> isType (types))
			found .emplace_back (node);
	}

	return found;
}

template <class NodeType>
X3D::X3DPtrArray <NodeType>
X3DEditorObject::getSelection (const std::set <X3D::X3DConstants::NodeType> & types, const bool traverse)
{
	return getNodes <NodeType> (getSelectedNodes (), types, traverse);
}

/***
 *  Returns the last node of type @a FieldType in @a nodes in field @a fieldName. The second value indicates if the node
 *  was found, where:
 *  -2 means no field named @a fieldName found
 *  -1 means inconsistent
 *   0 means all values are NULL
 *   1 means all values share the found node
 *  The third value indicates whether the node has a parent or is a direct selection.
 */
template <class FieldType, class NodeType>
std::tuple <X3D::X3DPtr <FieldType>, int32_t, int32_t>
X3DEditorObject::getSelection (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName)
{
	// Check if there is a direct master selecection of our node type.

	const auto & selection = getSelectedNodes ();

	if (not selection .empty ())
	{
		const X3D::X3DPtr <FieldType> node (selection .back ());

		if (node)
			return std::tuple (node, SAME_NODE, false);
	}

	auto pair = getNode <FieldType, NodeType> (nodes, fieldName);

	return std::tuple (std::move (pair .first), pair .second, pair .second not_eq -2);
}
	
/***
 *  Returns the last node of type @a FieldType in @a nodes in field @a fieldName. The second value indicates if the node
 *  was found, where:
 *  -2 means no field named @a fieldName found
 *  -1 means inconsistent
 *   0 means all values are NULL
 *   1 means all values share the found node
 */
template <class FieldType, class NodeType>
std::pair <X3D::X3DPtr <FieldType>, int32_t>
X3DEditorObject::getNode (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName)
{
	X3D::X3DPtr <FieldType> found;
	int32_t                 active = -2;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> template getField <X3D::SFNode> (fieldName);

			if (active < 0)
			{
				found  = field;
				active = bool (found);
			}
			else if (field not_eq found)
			{
				if (not found)
					found = field;

				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return std::pair (std::move (found), active);
}

/***
 *  Returns the boolean in @a nodes in field @a fieldName. The value indicates if the field
 *  was found and what value it has, where:
 *  -2 means no field named @a fieldName found
 *  -1 means inconsistent
 *   0 means all values are false
 *   1 means all values are true
 */
template <class NodeType>
int32_t
X3DEditorObject::getBoolean (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName)
{
	int32_t active = -2;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> template getField <X3D::SFBool> (fieldName);

			if (active < 0)
			{
				active = field;
			}
			else if (field .getValue () not_eq active)
			{
				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return active;
}

/***
 *  Returns the string in @a nodes in field @a fieldName. The second value indicates if the field
 *  was found and what value it has, where:
 *  -2 means no field named @a fieldName found
 *  -1 means inconsistent
 *   0 means all values are empty
 *   1 means all values are equal
 */
template <class FieldType, class NodeType>
std::pair <FieldType, int32_t>
X3DEditorObject::getArray (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName)
{
	FieldType found;
	int32_t   active = -2;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> template getField <FieldType> (fieldName);

			if (active < 0)
			{
				found  = field;
				active = not found .empty ();
			}
			else if (field not_eq found)
			{
				if (found .empty ())
					found = field;

				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return std::pair (std::move (found), active);
}

template <class NodeType>
void
X3DEditorObject::unlinkClone (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, X3D::UndoStepPtr & undoStep)
{
	if (nodes .empty ())
		return;

	undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Unlink Clone »%s«"), fieldName .c_str ()));

	const size_t cloneCount = nodes [0] -> template getField <X3D::SFNode> (fieldName) -> getCloneCount ();
	bool         first      = (cloneCount == nodes .size ());

	for (const auto & node : nodes)
	{
		if (not first)
		{
			auto & field = node -> template getField <X3D::SFNode> (fieldName);

			const X3D::SFNode copy (field -> copy (getCurrentContext (), X3D::FLAT_COPY));

			X3D::X3DEditor::replaceNode (getCurrentContext (), node, field, copy, undoStep);
		}

		first = false;
	}

	addUndoStep (undoStep);

	undoStep .reset ();
}

template <class FieldType, class NodeType>
bool
X3DEditorObject::addUndoFunction (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, X3D::UndoStepPtr & undoStep)
{
	if (not undo)
		return false;

	if (nodes .empty ())
		return false;

	const auto lastUndoStep = getUndoStep ();

	if (undoStep and lastUndoStep == undoStep)
	{
		if (undoGroup .empty ())
		{
			if (fieldName == currentField)
			{
				endUndoGroup (undoGroup, undoStep);
				return false;
			}
		}
		else
		{
			if (lastUndoGroup == undoGroup)
				return false;
		}
	}
	else
		resetUndoGroup (undoGroup, undoStep);

	currentField = fieldName;

	// Remember field value if all values are equal.

	{
		bool      first        = true;
		bool      inconsistent = false;
		FieldType value;

		for (const auto & node : nodes)
		{
			try
			{
				auto & field = node -> template getField <FieldType> (fieldName);

				if (first)
				{
					first = false;
					value = field;
				}
				else if (field not_eq value)
				{
					inconsistent = true;
					break;
				}
			}
			catch (const X3D::X3DError &)
			{ }
		}

		if (inconsistent)
			fields -> removeUserDefinedField (fieldName);
		else
			fields -> addUserDefinedField (X3D::initializeOnly, fieldName, new FieldType (std::move (value)));
	}

	// Undo

	std::string nodeTypeName = nodes .back () -> getTypeName ();

	if (undoGroup .empty ())
	{
		undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Change Field %s »%s«"), nodeTypeName .c_str (), fieldName .c_str ()));
	}
	else
	{
		if (not undoStep)
		{
			undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Change %s »%s«"), nodeTypeName .c_str (), undoGroup .c_str ()));
		}
	}

	// Undo field change

	undoStep -> addObjects (nodes);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> template getField <FieldType> (fieldName);

			undoStep -> addUndoFunction (&FieldType::setValue, std::ref (field), field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (undoStep not_eq lastUndoStep)
		addUndoStep (undoStep);

	return true;
}

template <class FieldType, class NodeType>
bool
X3DEditorObject::addRedoFunction (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, X3D::UndoStepPtr & undoStep)
{
	if (not undo)
		return false;

	// Test if there is no change.

	bool changed = false;

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> template getField <FieldType> (fieldName);

			try
			{
				if (fields -> template getField <FieldType> (fieldName) not_eq field)
				{
					changed = true;
					break;
				}
			}
			catch (const X3D::X3DError & error)
			{
				changed = true;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not changed)
	{
		if (redoGroup .empty ())
			endRedoGroup (redoGroup, undoStep);

		return false;
	}

	undoGroupChanged = true;

	// Redo field change

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> template getField <FieldType> (fieldName);

			undoStep -> addRedoFunction (&FieldType::setValue, std::ref (field), field);

			// Prototype support

			X3D::X3DEditor::requestUpdateInstances (node, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return true;
}

template <class FieldType, class NodeType>
bool
X3DEditorObject::addUndoFunction (const X3D::X3DPtr <NodeType> & node, FieldType & field, X3D::UndoStepPtr & undoStep)
{
	if (not undo)
		return false;

	const auto fieldName    = basic::to_string (node -> getId (), std::locale::classic ()) + "." + basic::to_string (field .getId (), std::locale::classic ());
	const auto lastUndoStep = getUndoStep ();

	if (undoStep and lastUndoStep == undoStep)
	{
		if (undoGroup .empty ())
		{
			if (fieldName == currentField)
			{
				endUndoGroup (undoGroup, undoStep);
				return false;
			}
		}
		else
		{
			if (lastUndoGroup == undoGroup)
				return false;
		}
	}
	else
		resetUndoGroup (undoGroup, undoStep);

	currentField = fieldName;

	fields -> addUserDefinedField (X3D::initializeOnly, fieldName, new FieldType (field));

	// Undo

	std::string nodeTypeName = node -> getTypeName ();

	if (undoGroup .empty ())
	{
		undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Change Field %s »%s«"), node -> getTypeName () .c_str (), field .getName () .c_str ()));
	}
	else
	{
		if (not undoStep)
		{
			undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Change %s »%s«"), nodeTypeName .c_str (), undoGroup .c_str ()));
		}
	}

	// Undo field change

	undoStep -> addObjects (node);

	undoStep -> addUndoFunction (&FieldType::setValue, std::ref (field), field);

	if (undoStep not_eq lastUndoStep)
		addUndoStep (undoStep);

	return true;
}

template <class FieldType, class NodeType>
bool
X3DEditorObject::addRedoFunction (const X3D::X3DPtr <NodeType> & node, FieldType & field, X3D::UndoStepPtr & undoStep)
{
	if (not undo)
		return false;

	const auto fieldName = basic::to_string (node -> getId (), std::locale::classic ()) + "." + basic::to_string (field .getId (), std::locale::classic ());

	if (fields -> template getField <FieldType> (fieldName) == field)
	{
		// No change.

		if (redoGroup .empty ())
			endRedoGroup (redoGroup, undoStep);

		return false;
	}

	undoGroupChanged = true;

	// Redo field change

	undoStep -> addRedoFunction (&FieldType::setValue, std::ref (field), field);

	// Prototype support

	X3D::X3DEditor::requestUpdateInstances (node, undoStep);

	return true;
}

} // puck
} // titania

#endif
