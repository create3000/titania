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

#ifndef __TITANIA_X3D_BROWSER_PROPERTIES_BROWSER_PROPERTIES_H__
#define __TITANIA_X3D_BROWSER_PROPERTIES_BROWSER_PROPERTIES_H__

#include "../../Components/Core/X3DPropertyNode.h"

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
	public X3DPropertyNode
{
public:

	BrowserProperties (X3DExecutionContext* const executionContext);

	SFBool &
	abstractNodes ()
	{ return *fields .abstractNodes; }

	const SFBool &
	abstractNodes () const
	{ return *fields .abstractNodes; }

	SFBool &
	concreteNodes ()
	{ return *fields .concreteNodes; }

	const SFBool &
	concreteNodes () const
	{ return *fields .concreteNodes; }

	SFBool &
	externalInteractions ()
	{ return *fields .externalInteractions; }

	const SFBool &
	externalInteractions () const
	{ return *fields .externalInteractions; }

	SFBool &
	prototypeCreate ()
	{ return *fields .prototypeCreate; }

	const SFBool &
	prototypeCreate () const
	{ return *fields .prototypeCreate; }

	SFBool &
	domImport ()
	{ return *fields .domImport; }

	const SFBool &
	domImport () const
	{ return *fields .domImport; }

	SFBool &
	xmlEncoding ()
	{ return *fields .xmlEncoding; }

	const SFBool &
	xmlEncoding () const
	{ return *fields .xmlEncoding; }

	SFBool &
	classicVrmlEncoding ()
	{ return *fields .classicVrmlEncoding; }

	const SFBool &
	classicVrmlEncoding () const
	{ return *fields .classicVrmlEncoding; }

	SFBool &
	binaryEncoding ()
	{ return *fields .binaryEncoding; }

	const SFBool &
	binaryEncoding () const
	{ return *fields .binaryEncoding; }


private:

	virtual
	BrowserProperties*
	create (X3DExecutionContext* const executionContext) const final;

	struct Fields
	{
		Fields ();

		SFBool* const abstractNodes;
		SFBool* const concreteNodes;
		SFBool* const externalInteractions;
		SFBool* const prototypeCreate;
		SFBool* const domImport;
		SFBool* const xmlEncoding;
		SFBool* const classicVrmlEncoding;
		SFBool* const binaryEncoding;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
