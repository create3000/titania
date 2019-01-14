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

#ifndef __TITANIA_X3D_TOOLS_NURBS_NURBS_CURVE_TOOL_H__
#define __TITANIA_X3D_TOOLS_NURBS_NURBS_CURVE_TOOL_H__

#include "../../Components/NURBS/NurbsCurve.h"
#include "../NURBS/X3DParametricGeometryNodeTool.h"
#include "../Rendering/X3DLineGeometryNodeTool.h"

namespace titania {
namespace X3D {

class NurbsCurveTool :
	virtual public NurbsCurve,
	public X3DParametricGeometryNodeTool,
	public X3DLineGeometryNodeTool
{
public:

	///  @name Construction

	NurbsCurveTool (X3DBaseNode* const node);

	///  @name Fields

	virtual
	SFInt32 &
	tessellation () final override
	{ return getNode <NurbsCurve> () -> tessellation (); }

	virtual
	const SFInt32 &
	tessellation () const final override
	{ return getNode <NurbsCurve> () -> tessellation (); }

	virtual
	SFBool &
	closed () final override
	{ return getNode <NurbsCurve> () -> closed (); }

	virtual
	const SFBool &
	closed () const final override
	{ return getNode <NurbsCurve> () -> closed (); }

	virtual
	SFInt32 &
	order () final override
	{ return getNode <NurbsCurve> () -> order (); }

	virtual
	const SFInt32 &
	order () const final override
	{ return getNode <NurbsCurve> () -> order (); }

	virtual
	MFDouble &
	knot () final override
	{ return getNode <NurbsCurve> () -> knot (); }

	virtual
	const MFDouble &
	knot () const final override
	{ return getNode <NurbsCurve> () -> knot (); }

	virtual
	MFDouble &
	weight () final override
	{ return getNode <NurbsCurve> () -> weight (); }

	virtual
	const MFDouble &
	weight () const final override
	{ return getNode <NurbsCurve> () -> weight (); }

	virtual
	SFNode &
	controlPoint () final override
	{ return getNode <NurbsCurve> () -> controlPoint (); }

	virtual
	const SFNode &
	controlPoint () const final override
	{ return getNode <NurbsCurve> () -> controlPoint (); }

	///  @name Member access

	virtual
	bool
	isTransparent () const final override
	{ return X3DLineGeometryNodeTool::isTransparent (); }

	virtual
	const X3DPtr <X3DCoordinateNode> &
	getControlPoint () const final override
	{ return getNode <NurbsCurve> () -> getControlPoint (); }

	///  @name Operations

	virtual
	SFNode
	toPrimitive () const override
	{ return getNode <NurbsCurve> () -> toPrimitive (); }

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~NurbsCurveTool () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Event handlers

	void
	set_load_state ();

	void
	set_controlPoint ();

	void
	eventProcessed ();

	///  @name Members

	X3DPtr <IndexedLineSet> controlPointLines;

};

} // X3D
} // titania

#endif
