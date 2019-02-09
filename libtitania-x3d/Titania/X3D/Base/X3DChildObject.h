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

	///  @name Contruction

	///  Initializes this object.
	virtual
	void
	setup ();

	///  @name Reference handling

	///  Returns the number of strong references of this object.  Weak references are not counted.
	size_t
	getReferenceCount () const
	{ return referenceCount; }

	///  Returns number of weak references of this object.
	size_t
	getWeakReferenceCount () const
	{ return getParents () .size () - getReferenceCount (); }

	///  Add a parent to this object.
	virtual
	void
	addParent (X3DChildObject* const parent);

	///  Fast replaces @a parentToRemove with @a parentToAdd.
	virtual
	void
	replaceParent (X3DChildObject* const parentToRemove, X3DChildObject* const parentToAdd);

	///  Remove a parent from this object.
	virtual
	void
	removeParent (X3DChildObject* const parent);

	///  Returns a set of all parents of this object.
	const ChildObjectSet &
	getParents () const
	{ return parents; }

	///  Add a weak parent to this object.
	virtual
	void
	addWeakParent (X3DChildObject* const parent);

	///  Remove a weak parent from this object.
	virtual
	void
	removeWeakParent (X3DChildObject* const parent);

	///  @name Clone handling

	///  Virtual function that must be implemented in a derived class or it returns the number of parents.
	virtual
	size_t
	getCloneCount () const
	{ return parents .size (); }

	///  Handler that should be called when the clount count should be incremented @a count.
	virtual
	void
	addCloneCount (const size_t count)
	{ }

	///  Handler that should be called when the clount count should be decremented by @a count.
	virtual
	void
	removeCloneCount (const size_t count)
	{ }

	///  @name Event Handling

	///  Sets the tainted state.  See getTainted ().
	virtual
	void
	setTainted (const bool value)
	{ tainted = value; }

	///  Returns true if this objects has unprocessed events otherwise false.
	virtual
	bool
	getTainted () const
	{ return tainted; }

	/// Set is set state.
	virtual
	void
	isSet (const bool value)
	{ }

	/// Returns true if object was set otherwise false.
	virtual
	bool
	isSet () const
	{ return false; }

	///  Set the value of this object without adding an event.  @a object must be of the same type as this object.
	virtual
	void
	set (const X3DChildObject & object)
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

	///  @name Children handling

	///  Add this node as parent to all @a children.  See addChild.
	template <class ... Args>
	void
	addChildObjects (Args & ... children)
	{ (addChildObject (children), ...); }

	///  Adds a private child object to this object.  The child object is then able to paricipate on event routing.
	virtual
	void
	addChildObject (X3DChildObject & child);

	///  Remove this node as parent from all @a children.  See removeChild.
	template <class ... Args>
	void
	removeChildObjects (Args & ... children)
	{ (removeChildObject (children), ...); }

	///  Removes a private field from this object.  If the reference count of @a object becomes 0 the child will be disposed.
	virtual
	void
	removeChildObject (X3DChildObject & child);

	///  @name Reference handling

	///  Handler that is called when the reference count should be incremented.
	void
	addReferenceCount ();

	///  Handler that is called when the reference count should be decremented.
	void
	removeReferenceCount ();

	///  Handler that is called when the reference count should be set to 0.
	void
	clearReferenceCount ();

	///  @name Event handling

	///  Handler that is called when a child of this object should be marked tainted.
	virtual
	void
	addEventObject (X3DChildObject* const object)
	{ addEvent (); }

	///  Handler that is called when a child (field) of this node should be added to the event queue.
	virtual
	void
	addEventObject (X3DChildObject* const object, const EventPtr & event);

	///  @name Destruction

	virtual
	void
	shutdown ()
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

	ChildObjectSet  parents;     // All parents
	X3DChildObject* bestParent;  // Best parent to find root
	ChildObjectSet  children;    // Internal used fields
	size_t          referenceCount;
	bool            inShutdown;
	bool            tainted;

};

} // X3D
} // titania

#endif
