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

#ifndef __TITANIA_X3D_EXECUTION_IMPORTED_NODE_H__
#define __TITANIA_X3D_EXECUTION_IMPORTED_NODE_H__

#include "../Basic/X3DBaseNode.h"
#include "../Fields/SFNode.h"
#include "../Fields/X3DWeakPtr.h"
#include "../Routing/Route.h"
#include "../Types/Pointer.h"

namespace titania {
namespace X3D {

class Inline;

class ImportedNode :
	virtual public X3DBaseNode
{
public:

	///  @name Construction

	///  throws Error <INVALID_NAME>, Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	ImportedNode (X3DExecutionContext* const executionContext,
	              const X3DPtr <Inline> & inlineNode,
	              const std::string & exportedName,
	              const std::string & importedName);

	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	ImportedNode*
	copy (const CopyType type) const final override
	{ return copy (getExecutionContext (), type); }

	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	ImportedNode*
	copy (X3DExecutionContext* const executionContext, const CopyType type) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Member access

	///  throws Error <DISPOSED>
	X3DPtr <Inline>
	getInlineNode () const;

	const std::string &
	getExportedName () const
	{ return exportedName; }

	const std::string &
	getImportedName () const
	{ return importedName; }

	///  throws Error <DISPOSED>
	SFNode
	getExportedNode () const;

	///  @name Route handling

	void
	addRoute (const SFNode & sourceNode, const std::string & sourceField, const SFNode & destinationNode, const std::string & destinationField);

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream & ostream) const final override;

	virtual
	void
	toXMLStream (std::ostream & ostream) const final override;

	virtual
	void
	toJSONStream (std::ostream & ostream) const final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~ImportedNode () final override;


protected:

	///  @name Friends

	friend class X3DExecutionContext;	


	///  @name Member access

	void
	setImportedName (const std::string & value)
	{ importedName = value; }


private:

	///  @name Member types

	struct UnresolvedRoute
	{
		X3DWeakPtr <X3DBaseNode> sourceNode;
		std::string              sourceField;
		X3DWeakPtr <X3DBaseNode> destinationNode;
		std::string              destinationField;
		X3DWeakPtr <Route>       route;
	};

	using UnresolvedRouteId = std::tuple <size_t, std::string, size_t, std::string>;
	using UnresolvedRoutes  = std::map <UnresolvedRouteId, UnresolvedRoute>;

	///  @name Construction

	///  throws Error <NOT_SUPPORTED>
	[[noreturn]]
	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	virtual
	void
	initialize () final override;

	///  @name Route handling

	void
	resolveRoute (UnresolvedRoute & route);

	void
	deleteRoutes ();

	void
	set_loadState ();

	///  @name Destruction

	void
	set_inlineNode ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	X3DWeakPtr <Inline> inlineNode;
	const std::string   exportedName;
	std::string         importedName;
	UnresolvedRoutes    routeIndex;

};

} // X3D
} // titania

#endif
