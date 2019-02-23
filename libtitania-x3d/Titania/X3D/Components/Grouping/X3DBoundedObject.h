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

#ifndef __TITANIA_X3D_COMPONENTS_GROUPING_X3DBOUNDED_OBJECT_H__
#define __TITANIA_X3D_COMPONENTS_GROUPING_X3DBOUNDED_OBJECT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Bits/Cast.h"
#include "../../Bits/X3DConstants.h"
#include "../../Fields.h"
#include "../../Types/Geometry.h"

namespace titania {
namespace X3D {

class X3DBoundedObject :
	virtual public X3DBaseNode
{
public:

	///  @name Fields

	virtual
	SFVec3f &
	bboxSize ()
	{ return *fields .bboxSize; }

	virtual
	const SFVec3f &
	bboxSize () const
	{ return *fields .bboxSize; }

	virtual
	SFVec3f &
	bboxCenter ()
	{ return *fields .bboxCenter; }

	virtual
	const SFVec3f &
	bboxCenter () const
	{ return *fields .bboxCenter; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const = 0;

	template <class Type>
	static
	Box3d
	getBBox (const X3DPtrArray <Type> & nodes);

	virtual
	void
	addTransformSensor (TransformSensor* const transformSensorNode);

	virtual
	void
	removeTransformSensor (TransformSensor* const transformSensorNode);

	virtual
	const std::set <TransformSensor*> &
	getTransformSensors () const
	{ return transformSensorNodes; }

	virtual
	const SFTime &
	transformSensors_changed ()
	{ return transformSensorOutput; }

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }

	virtual
	~X3DBoundedObject () override;


protected:

	///  @name Construction

	X3DBoundedObject ();

	virtual
	void
	initialize () override
	{ }


private:

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFVec3f* const bboxSize;
		SFVec3f* const bboxCenter;
	};

	Fields fields;

	///  @name Members

	std::set <TransformSensor*> transformSensorNodes;
	SFTime                      transformSensorOutput;

};

template <class Type>
Box3d
X3DBoundedObject::getBBox (const X3DPtrArray <Type> & nodes)
{
	Box3d bbox;

	// Add bounding boxes

	for (const auto & node : nodes)
	{
		const auto boundedObject = x3d_cast <X3DBoundedObject*> (node);

		if (boundedObject)
			bbox += boundedObject -> getBBox ();
	}

	return bbox;
}

} // X3D
} // titania

#endif
