/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_EXECUTION_X3DSCENE_H__
#define __TITANIA_X3D_EXECUTION_X3DSCENE_H__

#include "../Execution/ExportedNodeArray.h"
#include "../Execution/X3DExecutionContext.h"

#include <map>

namespace titania {
namespace X3D {

using MetaDataIndex = std::multimap <std::string, std::string>;

class X3DScene :
	virtual public X3DBaseNode, public X3DExecutionContext
{
public:

	///  @name Construction

	X3DScene ();
	
	///  @name Member access
	
	std::string
	getTitle () const;

	void
	isCompressed (bool value)
	{ compressed = value; }
	
	bool
	isCompressed () const
	{ return compressed; }

	///  @name MetaData handling

	void
	addMetaData (const std::string &, const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	setMetaData (const std::string &, const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const std::string &
	getMetaData (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const MetaDataIndex &
	getMetaDatas () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Exported nodes handling

	const ExportedNodePtr &
	addExportedNode (const std::string &, const SFNode &)
	throw (Error <NODE_IN_USE>,
	       Error <INVALID_NAME>,
	       Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeExportedNode (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	updateExportedNode (const std::string &, const SFNode &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	SFNode
	getExportedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ExportedNodeArray &
	getExportedNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return exportedNodes; }

	///  @name Input/Output

	void
	fromStream (const basic::uri &, std::istream & istream)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	virtual
	void
	toStream (std::ostream &) const override;

	virtual
	void
	toXMLStream (std::ostream &) const override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	///  @name Import handling


protected:

	virtual
	void
	initialize () override;

	void
	importMetaData (const X3DScene* const)
	throw (Error <INVALID_NAME>,
          Error <NOT_SUPPORTED>);

	void
	importExportedNodes (const X3DScene* const)
	throw (Error <INVALID_NAME>,
          Error <NOT_SUPPORTED>);


private:

	///  @name Members

	typedef std::map <X3DBaseNode*, std::string> ExportedNamesIndex;

	bool               compressed;
	MetaDataIndex      metadatas;
	ExportedNodeArray  exportedNodes;
	ExportedNamesIndex exportedNames;

};

} // X3D
} // titania

#endif
