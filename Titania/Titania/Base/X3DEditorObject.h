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

#ifndef __TITANIA_BASE_X3DEDITOR_OBJECT_H__
#define __TITANIA_BASE_X3DEDITOR_OBJECT_H__

#include "../Base/X3DBaseInterface.h"
#include "../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Basic/FieldSet.h>
#include <Titania/X3D/Browser/X3DBrowser.h>
#include <Titania/X3D/Fields/X3DPtrArray.h>

#include <Titania/String/sprintf.h>
#include <Titania/Utility/Range.h>

namespace titania {
namespace puck {

class X3DEditorObject :
	virtual public X3DBaseInterface
{
public:

	void
	setUndo (const bool value)
	{ undo = value; }

	bool
	getUndo () const
	{ return undo; }

	template <class NodeType>
	static
	X3D::X3DPtrArray <NodeType>
	getNodes (const X3D::MFNode &, const std::set <X3D::X3DConstants::NodeType> &);

	///  @name Destruction

	virtual
	~X3DEditorObject ()
	{ }


protected:

	///  @name Construction

	X3DEditorObject () :
		X3DBaseInterface (),
		            undo (true),
		    currentField (),
		          fields (new X3D::FieldSet (getBrowser ())),
		        undoSize (0)
	{ }

	virtual
	void
	setup () override;

	///  @name Operations

	void
	validateIdOnInsert (Gtk::Entry &, const Glib::ustring &, int);

	void
	validateIdOnDelete (Gtk::Entry &, int, int);

	static
	Glib::ustring
	refineName (const Glib::ustring &);

	template <class NodeType>
	X3D::X3DPtrArray <NodeType>
	getSelection (const std::set <X3D::X3DConstants::NodeType> &) const;

	template <class FieldType, class NodeType>
	static
	std::pair <X3D::X3DPtr <FieldType>, int>
	getNode (const X3D::X3DPtrArray <NodeType> &, const std::string &);

	template <class NodeType>
	static
	int
	getBoolean (const X3D::X3DPtrArray <NodeType> &, const std::string &);

	template <class FieldType, class NodeType>
	static
	std::pair <FieldType, int>
	getArray (const X3D::X3DPtrArray <NodeType> &, const std::string &);

	template <class NodeType>
	void
	unlinkClone (const X3D::X3DPtrArray <NodeType> &, const std::string &, UndoStepPtr &);

	template <class FieldType, class NodeType>
	void
	addUndoFunction (const X3D::X3DPtrArray <NodeType> &, const std::string &, UndoStepPtr &);

	template <class FieldType, class NodeType>
	void
	addRedoFunction (const X3D::X3DPtrArray <NodeType> &, const std::string &, UndoStepPtr &);

	template <class FieldType, class NodeType>
	void
	addUndoFunction (const X3D::X3DPtr <NodeType> &, FieldType &, UndoStepPtr &);

	template <class FieldType>
	void
	addRedoFunction (FieldType &, UndoStepPtr &);

	/// @name Destuction
	
	virtual
	void
	dispose () override
	{ }


private:

	///  @name Event handlers
	 
	void
	set_browser (const X3D::BrowserPtr &);

	///  @name Operations

	bool
	validateId (const std::string &) const;

	///  @name Members

	bool                        undo;
	std::string                 currentField;
	X3D::X3DPtr <X3D::FieldSet> fields;
	size_t                      undoSize;

};

template <class NodeType>
X3D::X3DPtrArray <NodeType>
X3DEditorObject::getSelection (const std::set <X3D::X3DConstants::NodeType> & types) const
{
	auto selection = getBrowserWindow () -> getSelection () -> getChildren ();

	return getNodes <NodeType> (selection, types);
}

/***
 *  Traverses @a selection and returns all nodes of a type specified in @a types.
 */
template <class NodeType>
X3D::X3DPtrArray <NodeType>
X3DEditorObject::getNodes (const X3D::MFNode & selection, const std::set <X3D::X3DConstants::NodeType> & types)
{
	// Find X3DGeometryNodes

	X3D::X3DPtrArray <NodeType> nodes;

	X3D::traverse (const_cast <X3D::MFNode &> (selection), [&] (X3D::SFNode & node)
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

/***
 *  Returns the last node of type @a FieldType in @a nodes in field @a fieldName. The second value indicates if the node
 *  was found, where:
 *  -2 means no field named @a fieldName found
 *  -1 means inconsistent
 *   0 means all values are NULL
 *   1 means all values share the found node
 */
template <class FieldType, class NodeType>
std::pair <X3D::X3DPtr <FieldType>, int>
X3DEditorObject::getNode (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName)
{
	X3D::X3DPtr <FieldType> found;
	int                     active = -2;

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

	return std::make_pair (std::move (found), active);
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
int
X3DEditorObject::getBoolean (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName)
{
	int active = -2;

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
std::pair <FieldType, int>
X3DEditorObject::getArray (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName)
{
	FieldType found;
	int       active = -2;

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

	return std::make_pair (std::move (found), active);
}

template <class NodeType>
void
X3DEditorObject::unlinkClone (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, UndoStepPtr & undoStep)
{
	if (nodes .empty ())
		return;

	undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Unlink Clone »%s«"), fieldName .c_str ()));

	const size_t cloneCount = nodes [0] -> template getField <X3D::SFNode> (fieldName) -> getCloneCount ();
	bool         first      = (cloneCount == nodes .size ());

	for (const auto & node : nodes)
	{
		if (not first)
		{
			auto &            field = node -> template getField <X3D::SFNode> (fieldName);
			const X3D::SFNode copy  = field -> copy (X3D::FLAT_COPY);

			getBrowserWindow () -> replaceNode (X3D::SFNode (node), field, copy, undoStep);
		}

		first = false;
	}

	addUndoStep (undoStep);
	nodes [0] -> getExecutionContext () -> realize ();

	undoStep .reset ();
}

template <class FieldType, class NodeType>
void
X3DEditorObject::addUndoFunction (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, UndoStepPtr & undoStep)
{
	if (not undo)
		return;

	const auto lastUndoStep = getUndoStep ();

	if (undoStep and lastUndoStep == undoStep and fieldName == currentField)
	{
		for (const auto & undoFunction : std::make_pair (undoStep -> getUndoFunctions () .rbegin (), undoStep -> getUndoFunctions () .rend () - undoSize))
		{
			try
			{
				undoFunction ();
			}
			catch (const std::exception & error)
			{
				std::clog
					<< std::string (80, '*') << std::endl
					<< "*  Warning:  Undo step not possible:" << std::endl
					<< "*  " << error .what () << std::endl
					<< std::string (80, '*') << std::endl;
			}
		}

		undoStep -> getUndoFunctions () .resize (undoSize);
		undoStep -> getRedoFunctions () .clear ();
		return;
	}

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

	undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Change Field »%s«"), fieldName .c_str ()));

	undoStep -> addObjects (nodes);

	// Undo field change

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> template getField <FieldType> (fieldName);
			
			using setValue = void (FieldType::*) (const typename FieldType::internal_type &);

			undoStep -> addUndoFunction ((setValue) &FieldType::setValue, std::ref (field), field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addUndoStep (undoStep);

	undoSize = undoStep -> getUndoFunctions () .size ();
}

template <class FieldType, class NodeType>
void
X3DEditorObject::addRedoFunction (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, UndoStepPtr & undoStep)
{
	if (not undo)
		return;

	// Test if there is no change.

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> template getField <FieldType> (fieldName);

			try
			{
				if (fields -> template getField <FieldType> (currentField) == field)
				{
					undoStep .reset ();

					removeUndoStep ();
					return;
				}
			}
			catch (const X3D::X3DError &)
			{
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Redo field change

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> template getField <FieldType> (fieldName);

			using setValue = void (FieldType::*) (const typename FieldType::internal_type &);

			undoStep -> addRedoFunction ((setValue) &FieldType::setValue, std::ref (field), field);
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

template <class FieldType, class NodeType>
void
X3DEditorObject::addUndoFunction (const X3D::X3DPtr <NodeType> & node, FieldType & field, UndoStepPtr & undoStep)
{
	if (not undo)
		return;

	const auto fieldName    = node -> getTypeName () + "." + field .getName ();
	const auto lastUndoStep = getUndoStep ();

	if (undoStep and lastUndoStep == undoStep and fieldName == currentField)
	{
		for (const auto & undoFunction : std::make_pair (undoStep -> getUndoFunctions () .rbegin (), undoStep -> getUndoFunctions () .rend () - undoSize))
			undoFunction ();

		undoStep -> getUndoFunctions () .resize (undoSize);
		undoStep -> getRedoFunctions () .clear ();
		return;
	}

	currentField = fieldName;

	fields -> addUserDefinedField (X3D::initializeOnly, fieldName, new FieldType (field));

	// Undo

	undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Change Field »%s«"), field .getName () .c_str ()));

	undoStep -> addObjects (node);

	// Undo field change

	using setValue = void (FieldType::*) (const typename FieldType::internal_type &);

	undoStep -> addUndoFunction ((setValue) & FieldType::setValue, std::ref (field), field);

	addUndoStep (undoStep);

	undoSize = undoStep -> getUndoFunctions () .size ();
}

template <class FieldType>
void
X3DEditorObject::addRedoFunction (FieldType & field, UndoStepPtr & undoStep)
{
	if (not undo)
		return;

	if (fields -> template getField <FieldType> (currentField) == field)
	{
		// No change.

		undoStep .reset ();

		removeUndoStep ();
	}
	else
	{
		// Redo field change

		using setValue = void (FieldType::*) (const typename FieldType::internal_type &);

		undoStep -> addRedoFunction ((setValue) & FieldType::setValue, std::ref (field), field);
	}
}

} // puck
} // titania

#endif
