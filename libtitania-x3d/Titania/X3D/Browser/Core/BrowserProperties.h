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

#ifndef __TITANIA_X3D_BROWSER_BROWSER_PROPERTIES_H__
#define __TITANIA_X3D_BROWSER_BROWSER_PROPERTIES_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Fields.h"

namespace titania {
namespace X3D {

//	Property Name           Value data type      Description
//	ABSTRACT_NODES          Boolean              The browser implementation supports the ability to describe each node type with interfaces that correspond to the abstract node types as defined in ISO/IEC 19775-1 in addition to the basic requirement to support the X3DNode abstract type. This indicates that the browser supports at least Conformance Level 2.
//	CONCRETE_NODES          Boolean              The browser implementation supports the ability to describe each node type with interfaces that correspond to the concrete node types as defined in ISO/IEC 19775-1 in addition to the requirement to support all of the abstract types. This indicates that the browser supports at least Conformance Level 3.
//	EXTERNAL_INTERACTIONS   Boolean              This SAIBrowserRef supports the additional services required by external interfaces. A SAIBrowserRef provided to user code in internal interactions shall not set this property.
//	PROTOTYPE_CREATE        Boolean              The browser implementation supports the ability to dynamically create PROTO and EXTERNPROTO representations through service requests. The basic service capability only allows the ability to create instances of predefined PROTO structures read from a file format.
//	DOM_IMPORT              Boolean              The browser implementation supports the importDocument service request.
//	XML_ENCODING            Boolean              The browser supports XML as a file format encoding.
//	CLASSIC_VRML_ENCODING   Boolean              The browser supports the Classic VRML encoding.
//	BINARY_ENCODING         Boolean              The browser supports the binary file format encoding.

class BrowserProperties :
	virtual public X3DBaseNode
{
public:

	BrowserProperties (X3DExecutionContext* const executionContext);

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

	///  @name Fields

	SFBool &
	ABSTRACT_NODES ()
	{ return *fields .ABSTRACT_NODES; }

	const SFBool &
	ABSTRACT_NODES () const
	{ return *fields .ABSTRACT_NODES; }

	SFBool &
	CONCRETE_NODES ()
	{ return *fields .CONCRETE_NODES; }

	const SFBool &
	CONCRETE_NODES () const
	{ return *fields .CONCRETE_NODES; }

	SFBool &
	EXTERNAL_INTERACTIONS ()
	{ return *fields .EXTERNAL_INTERACTIONS; }

	const SFBool &
	EXTERNAL_INTERACTIONS () const
	{ return *fields .EXTERNAL_INTERACTIONS; }

	SFBool &
	PROTOTYPE_CREATE ()
	{ return *fields .PROTOTYPE_CREATE; }

	const SFBool &
	PROTOTYPE_CREATE () const
	{ return *fields .PROTOTYPE_CREATE; }

	SFBool &
	DOM_IMPORT ()
	{ return *fields .DOM_IMPORT; }

	const SFBool &
	DOM_IMPORT () const
	{ return *fields .DOM_IMPORT; }

	SFBool &
	XML_ENCODING ()
	{ return *fields .XML_ENCODING; }

	const SFBool &
	XML_ENCODING () const
	{ return *fields .XML_ENCODING; }

	SFBool &
	CLASSIC_VRML_ENCODING ()
	{ return *fields .CLASSIC_VRML_ENCODING; }

	const SFBool &
	CLASSIC_VRML_ENCODING () const
	{ return *fields .CLASSIC_VRML_ENCODING; }

	SFBool &
	BINARY_ENCODING ()
	{ return *fields .BINARY_ENCODING; }

	const SFBool &
	BINARY_ENCODING () const
	{ return *fields .BINARY_ENCODING; }


private:

	virtual
	BrowserProperties*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const ABSTRACT_NODES;
		SFBool* const CONCRETE_NODES;
		SFBool* const EXTERNAL_INTERACTIONS;
		SFBool* const PROTOTYPE_CREATE;
		SFBool* const DOM_IMPORT;
		SFBool* const XML_ENCODING;
		SFBool* const CLASSIC_VRML_ENCODING;
		SFBool* const BINARY_ENCODING;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
