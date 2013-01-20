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

#ifndef __TITANIA_X3D_ROUTING_ROUTE_H__
#define __TITANIA_X3D_ROUTING_ROUTE_H__

#include "../Basic/X3DBaseNode.h"
#include "../Fields.h"
#include "../Routing/X3DRoute.h"

namespace titania {
namespace X3D {

class Route :
	virtual public X3DBaseNode, public X3DRoute
{
public:

	virtual
	bool
	isConnected ();

	const SFNode <X3DBaseNode> &
	getSourceNode () const;

	const basic::id &
	getSourceField () const;

	const SFNode <X3DBaseNode> &
	getDestinationNode () const;

	const basic::id &
	getDestinationField () const;

	virtual
	void
	disconnect ();

	virtual
	void
	toStream (std::ostream &) const;

	virtual
	void
	dispose ();


protected:

	friend class X3DExecutionContext;

	Route (X3DExecutionContext* const,
	       const SFNode <X3DBaseNode> &, X3DFieldDefinition* const,
	       const SFNode <X3DBaseNode> &, X3DFieldDefinition* const);

	// Only X3DExecutionContext can clone routes.
	void
	add (X3DExecutionContext* const) const;


private:

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const;

	virtual
	Route*
	clone (X3DExecutionContext* const) const;

	virtual
	void
	initialize ();

	void
	connect ();

	SFNode <X3DBaseNode> sourceNode;
	SFNode <X3DBaseNode> destinationNode;

	X3DFieldDefinition* const sourceField;
	X3DFieldDefinition* const destinationField;

	bool connected;

};

} // X3D
} // titania

#endif
