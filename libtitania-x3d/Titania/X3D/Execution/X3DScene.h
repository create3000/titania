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

#ifndef __TITANIA_X3D_EXECUTION_X3DSCENE_H__
#define __TITANIA_X3D_EXECUTION_X3DSCENE_H__

#include "../Execution/ExportedNodeIndex.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Configuration/X3DUnitContext.h"

#include <map>

namespace titania {
namespace X3D {

using MetaDataIndex = std::multimap <std::string, std::string>;

class X3DScene :
	virtual public X3DBaseNode,
	public X3DExecutionContext,
	public X3DUnitContext
{
public:

	///  @name Member access

	bool
	isMasterScene () const
	throw (Error <DISPOSED>)
	{ return this == getExecutionContext (); }

	virtual
	bool
	isScene () const
	throw (Error <DISPOSED>) final override
	{ return true; }

	virtual
	std::string
	getTitle () const
	throw (Error <DISPOSED>) final override;

	void
	setWorldURL (const basic::uri & value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ worldURL = value; }

	const basic::uri &
	getWorldURL () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return worldURL; }

	void
	setEncoding (const std::string & value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ encoding = value; }

	virtual
	const std::string &
	getEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return encoding; }

	void
	setSpecificationVersion (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	setSpecificationVersion (const SpecificationVersionType value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ specificationVersion = value; }

	virtual
	SpecificationVersionType
	getSpecificationVersion () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return specificationVersion; }

	void
	setCharacterEncoding (const std::string & value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ characterEncoding = value; }

	virtual
	const std::string &
	getCharacterEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return characterEncoding; }

	void
	setComment (const std::string & value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ comment = value; }

	virtual
	const std::string &
	getComment () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return comment; }

	///  @name Profile/Component handling

	void
	setProfile (const ProfileInfoPtr & value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ profile = value; }

	virtual
	const ProfileInfoPtr &
	getProfile ()  const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return profile; }

	virtual
	void
	updateComponent (const ComponentInfoPtr & component)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	virtual
	const ComponentInfoArray &
	getComponents () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return components; }

	///  @name MetaData handling

	void
	addStandardMetaData ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	addMetaData (const std::string &, const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeMetaData (const std::string &)
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

	const SFTime &
	metaData_changed () const
	{ return metaDataOutput; }

	///  @name Exported nodes handling

	const ExportedNodePtr &
	addExportedNode (const std::string &, const SFNode &)
	throw (Error <NODE_IN_USE>,
	       Error <INVALID_NAME>,
	       Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ExportedNodePtr &
	updateExportedNode (const std::string &, const SFNode &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeExportedNode (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	SFNode
	getExportedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	template <class Type>
	X3DPtr <Type>
	getExportedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ExportedNodeIndex &
	getExportedNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return exportedNodes; }

	const SFTime &
	exportedNodes_changed () const
	{ return exportedNodesOutput; }

	///  @name Import handling

	virtual
	void
	import (X3DExecutionContext* const, MFNode &)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

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

	virtual
	void
	toJSONStream (std::ostream &) const final override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DScene () override;


protected:

	///  @name Construction

	X3DScene ();

	///  @name Construction

	virtual
	void
	initialize () override;

	///  @name Import handling

	void
	importMetaData (const X3DScene* const)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	void
	importExportedNodes (const X3DScene* const)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);


private:

	///  @name Operations

	std::string
	getUniqueExportedName (const X3DScene* const, std::string = "") const;

	void
	updateExportedNodes (X3DScene* const) const;

	///  @name Members

	basic::uri               worldURL;
	std::string              encoding;
	SpecificationVersionType specificationVersion;
	std::string              characterEncoding;
	std::string              comment;
	ProfileInfoPtr           profile;
	ComponentInfoArray       components;
	MetaDataIndex            metadatas;
	SFTime                   metaDataOutput;
	ExportedNodeIndex        exportedNodes;
	SFTime                   exportedNodesOutput;

};

template <class Type>
X3DPtr <Type>
X3DScene::getExportedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DPtr <Type> node (getExportedNode (name));

	if (node)
		return node;

	throw Error <INVALID_NODE> ("Invalid node: node '" + name + "' has other type.");
}

} // X3D
} // titania

#endif
