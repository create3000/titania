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

#ifndef __TITANIA_X3D_EXECUTION_IMPORTED_NODE_H__
#define __TITANIA_X3D_EXECUTION_IMPORTED_NODE_H__

#include "../Basic/X3DBaseNode.h"
#include "../Fields.h"

namespace titania {
namespace X3D {

class Inline;

class ImportedNode :
	virtual public X3DBaseNode
{
public:

	///  @name Construction

	ImportedNode (X3DExecutionContext* const,
	              const X3DSFNode <Inline> &, const std::string &, const std::string &);

	virtual
	ImportedNode*
	clone (X3DExecutionContext* const) const
	throw (Error <INVALID_NAME>,
          Error <NOT_SUPPORTED>) final;

	virtual
	ImportedNode*
	copy (X3DExecutionContext* const) const
	throw (Error <INVALID_NAME>,
          Error <NOT_SUPPORTED>) final;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final
	{ return containerField; }

	///  @name Member access

	const X3DSFNode <Inline> &
	getInlineNode () const
	{ return inlineNode; }

	const std::string &
	getExportedName () const
	{ return exportedName; }

	const std::string &
	getImportedName () const
	{ return importedName; }

	const SFNode &
	getExportedNode () const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream &) const final;

	///  @name Destruction

	virtual
	void
	dispose () final;


private:

	///  @name Construction

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	X3DSFNode <Inline> inlineNode;

	const std::string exportedName;
	const std::string importedName;

};

} // X3D
} // titania

#endif
