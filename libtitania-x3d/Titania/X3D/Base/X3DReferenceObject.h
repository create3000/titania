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

#ifndef __TITANIA_X3D_BASE_X3DREFERENCE_OBJECT_H__
#define __TITANIA_X3D_BASE_X3DREFERENCE_OBJECT_H__

#include "../Base/Output.h"
#include "../Base/X3DChildObject.h"

namespace titania {
namespace X3D {

/***
 *  Class to represent an object that handles reference counting.  X3DReferenceObjects can be a child of X3DWeakPtr.
 */
class X3DReferenceObject :
	public X3DChildObject
{
public:

	///  @name Reference handling

	///  Returns the number of strong references of this object.  Weak references are not counted.
	virtual
	size_t
	getReferenceCount () const final override
	{ return referenceCount; }

	///  Returns number of weak references of this object.
	size_t
	getWeakReferenceCount () const
	{ return getParents () .size () - getReferenceCount (); }

	///  @name Parent handling

	///  Add a parent to this object.
	virtual
	void
	addParent (X3DChildObject* const) final override;

	///  Fast replaces @a parentToRemove with @a parentToAdd.
	virtual
	void
	replaceParent (X3DChildObject* const, X3DChildObject* const) final override;

	///  Remove a parent from this object.
	virtual
	void
	removeParent (X3DChildObject* const) final override;

	///  Add a parent to this object.
	virtual
	void
	addWeakParent (X3DChildObject* const) final override;

	///  Remove a parent from this object.
	virtual
	void
	removeWeakParent (X3DChildObject* const) final override;

	///  @name Shutdown handling

	///  The shutdown service is processed before the object is disposed.  You must release all references to this object
	///  now.  Further access of this object will cause a Segmentation Fault!
	virtual
	const Output &
	shutdown () const
	{ return shutdownOutput; }

	///  @name Destruction

	///  Disposes this object.  You normally do not need to call this function directly.
	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	///  Constructs new X3DReferenceObject.
	X3DReferenceObject ();


private:

	///  @name Reference handling

	///  Increment the reference count for this object.
	virtual
	void
	addReferenceCount () final override;

	///  Decrement the reference count for this object.
	virtual
	void
	removeReferenceCount () final override;

	///  Sets the reference count for this object to 0.
	virtual
	void
	clearReferenceCount () final override;

	///  Processes all interests of shutdown.
	virtual
	void
	processShutdown () final override;

	///  @name Members

	size_t referenceCount;
	bool   inShutdown;
	Output shutdownOutput;

};

} // X3D
} // titania

#endif
