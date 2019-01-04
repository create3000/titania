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
#include "../Configuration/UnitArray.h"

#include <giomm.h>

#include <map>

namespace titania {
namespace X3D {

using MetaDataIndex = std::multimap <std::string, std::string>;

class X3DScene :
	virtual public X3DBaseNode,
	public X3DExecutionContext
{
public:

	///  @name Member access

	///  throws Error <DISPOSED>
	bool
	isMasterScene () const
	{ return this == getExecutionContext (); }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setWorldURL (const basic::uri & value);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const basic::uri &
	getWorldURL () const override
	{ return worldURL; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setEncoding (const EncodingType & value)
	{ encoding = value; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	EncodingType
	getEncoding () const final override
	{ return encoding; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setSpecificationVersion (const std::string &);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setSpecificationVersion (const SpecificationVersionType value)
	{ specificationVersion = value; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	SpecificationVersionType
	getSpecificationVersion () const final override
	{ return specificationVersion; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setCharacterEncoding (const std::string & value)
	{ characterEncoding = value; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const std::string &
	getCharacterEncoding () const final override
	{ return characterEncoding; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setComment (const std::string & value)
	{ comment = value; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const std::string &
	getComment () const final override
	{ return comment; }

	///  @name Profile/Component handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setProfile (const ProfileInfoPtr & value)
	{ profile = value; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const ProfileInfoPtr &
	getProfile () const final override
	{ return profile; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	void
	updateComponent (const ComponentInfoPtr & component) final override;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const ComponentInfoArray &
	getComponents () const final override
	{ return *components; }

	///  @name Unit handling

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	updateUnit (const std::string & category, const std::string & name, const long double conversionFactor);

	const Unit &
	getUnit (const UnitCategory unit) const;
	
	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const UnitArray &
	getUnits () const
	{ return units; }

	///  throws Error <DISPOSED>
	virtual
	long double
	fromUnit (const UnitCategory unit, const long double value) const final override;

	///  throws Error <DISPOSED>
	virtual
	long double
	toUnit (const UnitCategory unit, const long double value) const final override;

	const SFTime &
	units_changed () const
	{ return unitsOutput; }

	///  @name MetaData handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	addStandardMetaData ();

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	addMetaData (const std::string &, const std::string &);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	removeMetaData (const std::string &);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setMetaData (const std::string &, const std::string &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const std::string &
	getMetaData (const std::string &) const;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const MetaDataIndex &
	getMetaDatas () const;

	const SFTime &
	metaData_changed () const
	{ return metaDataOutput; }

	///  @name Exported nodes handling

	///  throws Error <NODE_IN_USE>, Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ExportedNodePtr &
	addExportedNode (const std::string &, const SFNode &);

	///  throws Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ExportedNodePtr &
	updateExportedNode (const std::string &, const SFNode &);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	removeExportedNode (const std::string &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	SFNode
	getExportedNode (const std::string &) const;

	///  throws Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	template <class Type>
	X3DPtr <Type>
	getExportedNode (const std::string &) const;

	const ExportedNodeIndex &
	getExportedNodes () const
	{ return exportedNodes; }

	const SFTime &
	exportedNodes_changed () const
	{ return exportedNodesOutput; }

	///  @name Monitor file changes

	virtual
	const SFTime &
	file_changed () const
	{ return fileChangedOutput; }

	///  @name Import handling

	virtual
	MFNode
	importScene (X3DExecutionContext* const executionContext) final override;

	///  @name Input/Output

	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	fromStream (const basic::uri &, std::istream & istream);

	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	void
	fromStream (std::istream &) override;

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
	importMetaData (const X3DScene* const);

	void
	importExportedNodes (const X3DScene* const);


private:

	///  @name Monitor file changes

	void
	monitorFile (const basic::uri & URL);

	void
	on_file_changed (const Glib::RefPtr <Gio::File> & file,
	                 const Glib::RefPtr <Gio::File> & other_file,
	                 Gio::FileMonitorEvent event);

	///  @name Operations

	std::string
	getVeryUniqueExportedName (const X3DScene* const, std::string = "") const;

	void
	updateExportedNodes (X3DScene* const) const;

	///  @name Static members

	static const UnitIndex unitCategories;
	static const UnitArray standardUnits;

	///  @name Members

	basic::uri                      worldURL;
	EncodingType                    encoding;
	SpecificationVersionType        specificationVersion;
	std::string                     characterEncoding;
	std::string                     comment;
	ProfileInfoPtr                  profile;
	ComponentInfoArrayPtr           components;
	UnitArray                       units;
	SFTime                          unitsOutput;
	MetaDataIndex                   metadatas;
	SFTime                          metaDataOutput;
	ExportedNodeIndex               exportedNodes;
	SFTime                          exportedNodesOutput;
	SFTime                          fileChangedOutput;
	Glib::RefPtr <Gio::FileMonitor> fileMonitor;

};

///  throws Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
template <class Type>
X3DPtr <Type>
X3DScene::getExportedNode (const std::string & name) const
{
	X3DPtr <Type> node (getExportedNode (name));

	if (node)
		return node;

	throw Error <INVALID_NODE> ("Invalid node: node '" + name + "' has other type.");
}

} // X3D
} // titania

#endif
