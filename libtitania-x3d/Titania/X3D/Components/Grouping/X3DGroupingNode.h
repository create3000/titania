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

#ifndef __TITANIA_X3D_COMPONENTS_GROUPING_X3DGROUPING_NODE_H__
#define __TITANIA_X3D_COMPONENTS_GROUPING_X3DGROUPING_NODE_H__

#include "../Core/X3DChildNode.h"
#include "../Grouping/X3DBoundedObject.h"

#include "../EnvironmentalEffects/LocalFog.h"
#include "../Lighting/X3DLightNode.h"
#include "../PointingDeviceSensor/X3DPointingDeviceSensorNode.h"

namespace titania {
namespace X3D {

class X3DGroupingNode :
	virtual public X3DChildNode, public X3DBoundedObject
{
public:

	///  @name Fields

	MFNode &
	children ()
	{ return *fields .children; }

	const MFNode &
	children () const
	{ return *fields .children; }

	// Declare this before add and remove, to get a lower address.
	MFNode &
	addChildren ()
	{ return *fields .addChildren; }

	const MFNode &
	addChildren () const
	{ return *fields .addChildren; }

	MFNode &
	removeChildren ()
	{ return *fields .removeChildren; }

	const MFNode &
	removeChildren () const
	{ return *fields .removeChildren; }

	///  @name Member access

	virtual
	Box3f
	getBBox () override;
	
	void
	setVisible (const MFBool &);

	const MFBool &
	getVisible () const
	{ return visible; }

	///  @name Operations

	virtual
	void
	notify () override;

	virtual
	void
	traverse (TraverseType) override;

	virtual
	void
	dispose () override;


protected:

	using X3DChildNode::addChildren;
	using X3DChildNode::removeChildren;

	///  @name Creation

	X3DGroupingNode ();

	virtual
	void
	initialize () override;


private:

	void
	set_addChildren ();

	void
	set_removeChildren ();

	void
	set_children ();

	void
	add (const MFNode &);

	void
	pick ();

	void
	collect ();

	struct Fields
	{
		Fields ();

		MFNode* const addChildren;
		MFNode* const removeChildren;
		MFNode* const children;
	};

	Fields fields;

	MFBool                                     visible;
	std::vector <X3DPointingDeviceSensorNode*> pointingDeviceSensors;
	std::vector <X3DLightNode*>                lights;
	std::vector <LocalFog*>                    localFogs;
	std::vector <X3DChildNode*>                childNodes;

};

} // X3D
} // titania

#endif
