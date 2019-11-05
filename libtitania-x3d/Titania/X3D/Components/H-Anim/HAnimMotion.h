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

#ifndef __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_MOTION_H__
#define __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_MOTION_H__

#include "../Core/X3DChildNode.h"

namespace titania {
namespace X3D {

class HAnimMotion :
	public X3DChildNode
{
public:

	///  @name Construction

	HAnimMotion (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
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

	SFString &
	channels ()
	{ return *fields .channels; }

	const SFString &
	channels () const
	{ return *fields .channels; }

	MFBool &
	channelsEnabled ()
	{ return *fields .channelsEnabled; }

	const MFBool &
	channelsEnabled () const
	{ return *fields .channelsEnabled; }

	SFTime &
	cycleTime ()
	{ return *fields .cycleTime; }

	const SFTime &
	cycleTime () const
	{ return *fields .cycleTime; }

	SFString &
	description ()
	{ return *fields .description; }

	const SFString &
	description () const
	{ return *fields .description; }

	SFTime &
	elapsedTime ()
	{ return *fields .elapsedTime; }

	const SFTime &
	elapsedTime () const
	{ return *fields .elapsedTime; }

	SFBool &
	enabled ()
	{ return *fields .enabled; }

	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	SFInt32 &
	frameCount ()
	{ return *fields .frameCount; }

	const SFInt32 &
	frameCount () const
	{ return *fields .frameCount; }

	SFTime &
	frameDuration ()
	{ return *fields .frameDuration; }

	const SFTime &
	frameDuration () const
	{ return *fields .frameDuration; }

	SFInt32 &
	frameIncrement ()
	{ return *fields .frameIncrement; }

	const SFInt32 &
	frameIncrement () const
	{ return *fields .frameIncrement; }

	SFInt32 &
	frameIndex ()
	{ return *fields .frameIndex; }

	const SFInt32 &
	frameIndex () const
	{ return *fields .frameIndex; }

	MFString &
	joints ()
	{ return *fields .joints; }

	const MFString &
	joints () const
	{ return *fields .joints; }

	SFInt32 &
	loa ()
	{ return *fields .loa; }

	const SFInt32 &
	loa () const
	{ return *fields .loa; }

	SFBool &
	loop ()
	{ return *fields .loop; }

	const SFBool &
	loop () const
	{ return *fields .loop; }

	SFBool &
	next ()
	{ return *fields .next; }

	const SFBool &
	next () const
	{ return *fields .next; }

	SFBool &
	previous ()
	{ return *fields .previous; }

	const SFBool &
	previous () const
	{ return *fields .previous; }

	MFFloat &
	values ()
	{ return *fields .values; }

	const MFFloat &
	values () const
	{ return *fields .values; }

	///  @name Destruction

	virtual
	~HAnimMotion () final override;


protected:

	///  @name Contruction

	virtual
	void
	initialize () final override;


private:

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFString* const channels;
		MFBool* const channelsEnabled;
		SFTime* const cycleTime;
		SFString* const description;
		SFTime* const elapsedTime;
		SFBool* const enabled;
		SFInt32* const frameCount;
		SFTime* const frameDuration;
		SFInt32* const frameIncrement;
		SFInt32* const frameIndex;
		MFString* const joints;
		SFInt32* const loa;
		SFBool* const loop;
		SFBool* const next;
		SFBool* const previous;
		MFFloat* const values;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
