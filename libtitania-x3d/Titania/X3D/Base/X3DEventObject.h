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

#ifndef __TITANIA_X3D_BASE_X3DEVENT_OBJECT_H__
#define __TITANIA_X3D_BASE_X3DEVENT_OBJECT_H__

#include "../Base/X3DChildObject.h"
#include "../Routing/ChildrenList.h"
#include "../Routing/ParentList.h"

#include <atomic>

namespace titania {
namespace X3D {

class X3DBrowser;

/**
 *  Class to represents an object that handles the events of its children.
 */
class X3DEventObject :
	public X3DChildObject
{
public:

	using X3DChildObject::addInterest;
	using X3DChildObject::removeInterest;

	///  @name Construction

	///  Initializes this object.
	virtual
	void
	setup () override;

	///  Returns whether this node is initialized.
	bool
	isInitialized () const
	{ return initialized; }

	///  @name Member access

	///  Returns a pointer to the browser this node belongs to.
	virtual
	X3DBrowser*
	getBrowser () const
	{ return browser; }

	///  @name Event handling

	///  Marks this object as tainted, i.e. all interests of this object will be processed later.
	virtual
	void
	addEvent () override;

	///  This function is called by the router when all events are processed.  You normally do not need to call this
	///  function directly.
	void
	eventsProcessed ();

	///  @name Interest service

	///  Adds an interest to this object.  The @a requester is then notified about a change of this object.  This version
	///  of the function effectivly calls addEvent on @a requester.
	void
	addInterest (X3DEventObject* const requester) const
	{ addInterest ((void (X3DEventObject::*)()) &X3DEventObject::addEvent, requester); }

	///  Removes an interest from this object.  The @a requester will not further notified about a change of this object.
	void
	removeInterest (X3DEventObject* const requester) const
	{ removeInterest ((void (X3DEventObject::*)()) &X3DEventObject::addEvent, requester); }

	///  @name Destruction

	///  Disposes this object.  You normally do not need to call this function directly.
	virtual
	void
	dispose () override;

	///  Destructs this object.
	virtual
	~X3DEventObject () override;


protected:

	///  @name Construction

	///  Constructs new X3DEventObject.
	X3DEventObject (X3DBrowser* const browser);

	///  Set the browser this node belongs to.
	void
	setBrowser (X3DBrowser* const browser);

	///  @name Event handling

	///  If extended event handling is set to true, initializeOnly field behave like inputOutput fields.  Otherwise
	///  initializeOnly fields will not process any events.  The default is true.
	void
	setExtendedEventHandling (const bool value)
	{ extendedEventHandling = value; }

	///  Returns whether extended event handling is enabled.
	bool
	getExtendedEventHandling () const
	{ return extendedEventHandling; }

	///  Handler that is called when a child (field) of this object should be marked tainted.
	virtual
	void
	addEventObject (X3DChildObject* const object) override;

	///  Handler that is called when a child (field) of this object should be added to the event queue.
	virtual
	void
	addEventObject (X3DChildObject* const object, const EventPtr & event) override;


private:

	///  @name Event handling

	///  Removes all fields from the event queue of the router.
	void
	removeEvents ();

	///  @name Members

	X3DBrowser*           browser;               // This objects Browser
	bool                  extendedEventHandling; // Handle initializeOnlys like inputOutput
	ParentId              parentId;              // This object within Router
	std::vector <ChildId> events;                // Children within Router
	bool                  initialized;

};

} // X3D
} // titania

#endif
