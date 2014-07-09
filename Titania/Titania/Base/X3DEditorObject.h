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
#include "../Browser/BrowserWindow.h"
#include "../Undo/UndoStep.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace puck {

class X3DEditorObject :
	virtual public X3DBaseInterface
{
public:

	///  @name Destruction

	virtual
	~X3DEditorObject ();


protected:

	///  @name Construction

	X3DEditorObject ();

	virtual
	void
	on_number_insert_text (const Glib::ustring &, int*, Gtk::Entry &);

	virtual
	void
	on_number_delete_text (int, int, Gtk::Entry &);

	///  @name Operations

	template <class FieldType, class NodeType>
	void
	addUndoFunction (const X3D::X3DPtrArray <NodeType> & nodes, const std::string &, UndoStepPtr & undoStep);

	template <class FieldType, class NodeType>
	void
	addRedoFunction (const X3D::X3DPtrArray <NodeType> & nodes, const std::string &, const UndoStepPtr & undoStep);

	template <class FieldType, class NodeType>
	void
	addUndoFunction (const X3D::X3DPtr <NodeType> &, FieldType &, UndoStepPtr &);

	template <class FieldType>
	void
	addRedoFunction (FieldType &, const UndoStepPtr &);

	double
	toDouble (const std::string &);


private:

	///  @name Operations

	bool
	validateNumber (const std::string &) const;

	///  @name Members

	std::string currentField;

};

template <class FieldType, class NodeType>
void
X3DEditorObject::addUndoFunction (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, UndoStepPtr & undoStep)
{
	const auto lastUndoStep = getBrowserWindow () -> getLastUndoStep ();

	if (undoStep and lastUndoStep == undoStep and fieldName == currentField)
		return;

	currentField = fieldName;

	undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Change Field »%s«"), fieldName .c_str ()));

	undoStep -> addVariables (nodes);

	// Undo field change

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

	getBrowserWindow () -> addUndoStep (undoStep);
}

template <class FieldType, class NodeType>
void
X3DEditorObject::addRedoFunction (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, const UndoStepPtr & undoStep)
{
	undoStep -> clearRedoFunctions ();

	// Redo field change

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> template getField <FieldType> (fieldName);

			undoStep -> addRedoFunction (&FieldType::setValue, std::ref (field), field);
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

template <class FieldType, class NodeType>
void
X3DEditorObject::addUndoFunction (const X3D::X3DPtr <NodeType> & node, FieldType & field, UndoStepPtr & undoStep)
{
	const auto fieldName    = node -> getTypeName () + "." + field .getName ();
	const auto lastUndoStep = getBrowserWindow () -> getLastUndoStep ();

	if (undoStep and lastUndoStep == undoStep and fieldName == currentField)
		return;

	currentField = fieldName;

	undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Change Field »%s«"), field .getName () .c_str ()));

	undoStep -> addVariables (node);

	// Undo field change

	undoStep -> addUndoFunction (&FieldType::setValue, std::ref (field), field);

	getBrowserWindow () -> addUndoStep (undoStep);
}

template <class FieldType>
void
X3DEditorObject::addRedoFunction (FieldType & field, const UndoStepPtr & undoStep)
{
	undoStep -> clearRedoFunctions ();

	// Redo field change

	undoStep -> addRedoFunction (&FieldType::setValue, std::ref (field), field);
}

} // puck
} // titania

#endif
