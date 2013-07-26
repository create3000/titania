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

#ifndef __TITANIA_X3D_COMPONENTS_TIME_X3DTIME_DEPENDENT_NODE_H__
#define __TITANIA_X3D_COMPONENTS_TIME_X3DTIME_DEPENDENT_NODE_H__

#include "../Core/X3DChildNode.h"

#include <sigc++/connection.h>

namespace titania {
namespace X3D {

class X3DTimeDependentNode :
	virtual public X3DChildNode
{
public:

	virtual
	SFBool &
	enabled () = 0;

	virtual
	const SFBool &
	enabled () const = 0;

	SFBool &
	loop ()
	{ return *fields .loop; }

	const SFBool &
	loop () const
	{ return *fields .loop; }

	SFTime &
	startTime ()
	{ return *fields .startTime; }

	const SFTime &
	startTime () const
	{ return *fields .startTime; }

	SFTime &
	stopTime ()
	{ return *fields .stopTime; }

	const SFTime &
	stopTime () const
	{ return *fields .stopTime; }

	SFTime &
	pauseTime ()
	{ return *fields .pauseTime; }

	const SFTime &
	pauseTime () const
	{ return *fields .pauseTime; }

	SFTime &
	resumeTime ()
	{ return *fields .resumeTime; }

	const SFTime &
	resumeTime () const
	{ return *fields .resumeTime; }

	SFBool &
	isPaused ()
	{ return *fields .isPaused; }

	const SFBool &
	isPaused () const
	{ return *fields .isPaused; }

	SFTime &
	cycleTime ()
	{ return *fields .cycleTime; }

	const SFTime &
	cycleTime () const
	{ return *fields .cycleTime; }

	SFTime &
	elapsedTime ()
	{ return *fields .elapsedTime; }

	const SFTime &
	elapsedTime () const
	{ return *fields .elapsedTime; }

	virtual
	void
	dispose () override;


protected:

	X3DTimeDependentNode ();

	time_type
	getElapsedTime () const;

	virtual
	void
	initialize () override;

	virtual
	void
	set_start () = 0;

	virtual
	void
	set_stop () = 0;

	virtual
	void
	set_pause () = 0;

	virtual
	void
	set_resume () = 0;


private:

	typedef bool (X3DTimeDependentNode::* TimeoutHandler)();

	void
	set_initialized ();

	void
	set_enabled ();

	void
	set_loop ();

	void
	set_startTime ();

	bool
	do_start ();

	void
	set_stopTime ();

	bool
	do_stop ();

	void
	set_pauseTime ();

	bool
	do_pause ();

	void
	set_resumeTime ();

	bool
	do_resume ();

	void
	addTimeout (sigc::connection &, TimeoutHandler, const time_type);

	struct Fields
	{
		Fields ();

		SFBool* const loop;
		SFTime* const startTime;
		SFTime* const stopTime;
		SFTime* const pauseTime;
		SFTime* const resumeTime;
		SFBool* const isPaused;
		SFTime* const cycleTime;
		SFTime* const elapsedTime;
	};

	Fields fields;

	time_type pause;

	sigc::connection startTimeout;
	sigc::connection stopTimeout;
	sigc::connection pauseTimeout;
	sigc::connection resumeTimeout;

};

} // X3D
} // titania

#endif
