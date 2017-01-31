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

#ifndef __TITANIA_X3D_BASE_X3DCHILD_OBJECT_H__
#define __TITANIA_X3D_BASE_X3DCHILD_OBJECT_H__

#include "../Base/ChildObjectSet.h"
#include "../Base/Event.h"
#include "../Base/X3DGarbageCollector.h"
#include "../Base/X3DObject.h"

namespace titania {
namespace X3D {

/***
 *  Class that represents an object that can have parents.  This class manages garbage collection and event bubbling.
 */
class X3DChildObject :
	public X3DObject,
	public X3DGarbageCollector
{
public:

	///  @name Reference handling

	///  Returns the number of strong references of this object.  Weak references are not counted.
	virtual
	size_t
	getReferenceCount () const
	{ return parents .size (); }

	///  Add a parent to this object.
	virtual
	void
	addParent (X3DChildObject* const);

	///  Fast replaces @a parentToRemove with @a parentToAdd.
	virtual
	void
	replaceParent (X3DChildObject* const, X3DChildObject* const);

	///  Remove a parent from this object.
	virtual
	void
	removeParent (X3DChildObject* const);

	///  Returns a set of all parents of this object.
	const ChildObjectSet &
	getParents () const
	{ return parents; }

	///  @name Clone handling

	///  Virtual function that must be implemented in a derived class or it returns the number of parents.
	virtual
	size_t
	getCloneCount () const
	{ return parents .size (); }

	///  Handler that should be called when the clount count should be incremented @a count.
	virtual
	void
	addClones (const size_t)
	{ }

	///  Handler that should be called when the clount count should be decremented by @a count.
	virtual
	void
	removeClones (const size_t)
	{ }

	///  @name Event Handling

	///  Sets the tainted state.  See isTainted ().
	void
	isTainted (const bool value)
	{ tainted = value; }

	///  Returns true if this objects has unprocessed events otherwise false.
	bool
	isTainted () const
	{ return tainted; }

	///  Set the value of this object without adding an event.  @a object must be of the same type as this object.
	virtual
	void
	set (const X3DChildObject &)
	{ }

	///  Notify object about a change and add an event.
	virtual
	void
	addEvent ();

	///  See X3DFieldDefinition processEvent.
	virtual
	void
	processEvent (const EventPtr &)
	{ }

	///  @name Destruction

	///  Disposes this object.  You normally do not need to call this function directly.
	virtual
	void
	dispose () override;

	///  Destructs this object.
	virtual
	~X3DChildObject () override;


protected:

	///  @name Construction

	///  Constructs new X3DChildObject.
	X3DChildObject ();

	///  @name Reference handling

	///  Add a weak parent to this object.
	virtual
	void
	addWeakParent (X3DChildObject* const);

	///  Remove a weak parent from this object.
	virtual
	void
	removeWeakParent (X3DChildObject* const);

	///  Handler that is called when the reference count should be incremented.
	virtual
	void
	addReferenceCount ()
	{ }

	///  Handler that is called when the reference count should be decremented.
	virtual
	void
	removeReferenceCount ()
	{ }

	///  Handler that is called when the reference count should be set to 0.
	virtual
	void
	clearReferenceCount ()
	{ }

	///  @name Event handling

	///  Handler that is called when a child of this object should be marked tainted.
	virtual
	void
	addEvent (X3DChildObject* const)
	{ addEvent (); }

	///  Handler that is called when a child (field) of this node should be added to the event queue.
	virtual
	void
	addEvent (X3DChildObject* const, const EventPtr &);

	///  @name Destruction

	///  Handler that is called when the shutdown service should be processed.
	virtual
	void
	processShutdown ()
	{ }

	///  Returns true if this object has root objects and collects in @a seen all objects seen.
	virtual
	bool
	hasRootedObjects (X3DChildObject* object, ChildObjectSet & seen)
	{ return object -> hasRootedObjects (seen); }

	///  Returns true if this object has root objects and collects in @a seen all objects seen.
	virtual
	bool
	hasRootedObjects (ChildObjectSet &);


private:

	///  @name Members

	ChildObjectSet  parents;
	X3DChildObject* bestParent;
	bool            tainted;

};

} // X3D
} // titania

#endif
