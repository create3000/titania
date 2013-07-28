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

#ifndef __TITANIA_X3D_COMPONENTS_TIME_TIME_SENSOR_H__
#define __TITANIA_X3D_COMPONENTS_TIME_TIME_SENSOR_H__

#include "../Core/X3DSensorNode.h"
#include "../Time/X3DTimeDependentNode.h"

namespace titania {
namespace X3D {

class TimeSensor :
	public X3DSensorNode, public X3DTimeDependentNode
{
public:

	TimeSensor (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	virtual
	SFBool &
	enabled () final
	{ return X3DSensorNode::enabled (); }

	virtual
	const SFBool &
	enabled () const final
	{ return X3DSensorNode::enabled (); }

	SFTime &
	cycleInterval ()
	{ return *fields .cycleInterval; }

	const SFTime &
	cycleInterval () const
	{ return *fields .cycleInterval; }

	SFFloat &
	fraction_changed ()
	{ return *fields .fraction_changed; }

	virtual
	SFBool &
	isActive () final
	{ return X3DSensorNode::isActive (); }

	virtual
	const SFBool &
	isActive () const final
	{ return X3DSensorNode::isActive (); }

	const SFFloat &
	fraction_changed () const
	{ return *fields .fraction_changed; }

	SFTime &
	time ()
	{ return *fields .time; }

	const SFTime &
	time () const
	{ return *fields .time; }


private:

	virtual
	void
	prepareEvents () final;

	virtual
	void
	set_start () final;

	virtual
	void
	set_stop () final;

	virtual
	void
	set_pause () final;

	virtual
	void
	set_resume (time_type) final;

	struct Fields
	{
		Fields ();

		SFTime* const cycleInterval;
		SFFloat* const fraction_changed;
		SFTime* const time;
	};

	Fields fields;

	time_type cycle;
	time_type interval;

};

} // X3D
} // titania

#endif
