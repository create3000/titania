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

	///  @name Construction
	
	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Fields
	
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
	resumeTime ()
	{ return *fields .resumeTime; }

	const SFTime &
	resumeTime () const
	{ return *fields .resumeTime; }

	SFTime &
	pauseTime ()
	{ return *fields .pauseTime; }

	const SFTime &
	pauseTime () const
	{ return *fields .pauseTime; }

	SFTime &
	stopTime ()
	{ return *fields .stopTime; }

	const SFTime &
	stopTime () const
	{ return *fields .stopTime; }

	SFBool &
	isPaused ()
	{ return *fields .isPaused; }

	const SFBool &
	isPaused () const
	{ return *fields .isPaused; }

	virtual
	SFBool &
	isActive () = 0;

	virtual
	const SFBool &
	isActive () const = 0;

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

	///  @name Member access
	
	const SFBool &
	isEvenLive () const
	{ return evenLive; }

	void
	isEvenLive (const bool value)
	{ evenLive = value; }

	///  @name Destruction
	
	virtual
	void
	dispose () override;


protected:

	///  @name Construction
	
	X3DTimeDependentNode ();

	time_type
	getElapsedTime () const;

	virtual
	void
	initialize () override;

	///  @name Member access

	bool
	getLive () const;
	
	///  @name Event handling

	void
	set_loop ();

	virtual
	void
	set_start () = 0;

	virtual
	void
	set_resume (const time_type) = 0;

	virtual
	void
	set_pause () = 0;

	virtual
	void
	set_stop () = 0;

	virtual
	void
	set_time () = 0;

	// Wrapper functions

	void
	do_start ();

	void
	do_pause ();
	
	void
	do_stop ();

	void
	stop ();


private:

	///  @name Member types

	using TimeoutHandler = void (X3DTimeDependentNode::*) ();

	///  @name Event handling

	void
	set_live ();

	void
	set_enabled ();

	void
	set_startTime ();

	void
	set_resumeTime ();

	void
	set_pauseTime ();

	void
	set_stopTime ();

	// Wrapper functions

	void
	real_pause ();

	void
	real_resume ();

	void
	do_resume ();

	// Timeout

	bool
	timeout (TimeoutHandler);

	void
	addTimeout (sigc::connection &, TimeoutHandler, const time_type);

	void
	removeTimeouts ();
	
	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const loop;
		SFTime* const startTime;
		SFTime* const resumeTime;
		SFTime* const pauseTime;
		SFTime* const stopTime;
		SFBool* const isPaused;
		SFTime* const cycleTime;
		SFTime* const elapsedTime;
	};

	Fields fields;
	
	SFTime    initialized;
	time_type startTimeValue;
	time_type resumeTimeValue;
	time_type pauseTimeValue;
	time_type stopTimeValue;

	time_type start;
	time_type pause;
	time_type pauseInterval;

	sigc::connection startTimeout;
	sigc::connection resumeTimeout;
	sigc::connection pauseTimeout;
	sigc::connection stopTimeout;

	SFBool evenLive;
	bool   disabled;

};

} // X3D
} // titania

#endif
