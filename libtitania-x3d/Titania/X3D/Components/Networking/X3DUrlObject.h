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

#ifndef __TITANIA_X3D_COMPONENTS_NETWORKING_X3DURL_OBJECT_H__
#define __TITANIA_X3D_COMPONENTS_NETWORKING_X3DURL_OBJECT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Bits/X3DConstants.h"
#include "../../Fields.h"

#include <Titania/Basic/URI.h>

#include <giomm.h>

namespace titania {
namespace X3D {

class X3DUrlObject :
	virtual public X3DBaseNode
{
public:

	///  @name Fields

	virtual
	MFString &
	url ()
	{ return *fields .url; }

	virtual
	const MFString &
	url () const
	{ return *fields .url; }

	///  @name Construction
	
	virtual
	X3DUrlObject*
	copy (const CopyType type) const override
	{ return copy (getExecutionContext (), type); }

	virtual
	X3DUrlObject*
	copy (X3DExecutionContext* const, const CopyType) const override;

	///  Sets the current excecution context to @a executionContext.
	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Member access

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const
	{ return loadState; }

	const basic::uri &
	getLoadedUrl () const
	{ return loadedUrl; }

	virtual
	const SFTime &
	file_changed () const
	{ return fileChangedOutput; }

	///  @name Operations

	virtual
	void
	requestImmediateLoad ()
	{ }

	static
	void
	transform (MFString & url, const basic::uri & oldWorldURL, const basic::uri & newWorldURL);


protected:

	///  @name Construction

	X3DUrlObject ();

	virtual
	void
	initialize () override;

	///  @name Member access

	void
	setLoadState (const LoadState value, const bool notify = true);

	void
	setLoadedUrl (const basic::uri & value);
	
	///  @name Destruction

	virtual
	void
	dispose () override;


private:

	///  @name Event handlers

	void
	monitorFile (const basic::uri & URL);

	void
	on_file_changed (const Glib::RefPtr <Gio::File> & file,
	                 const Glib::RefPtr <Gio::File> & other_file,
	                 Gio::FileMonitorEvent event);

	void
	set_file ();

	///  @name Fields

	struct Fields
	{
		Fields ();

		MFString* const url;
	};

	Fields fields;

	SFEnum <LoadState> loadState;
	basic::uri         loadedUrl;
	SFTime             fileChangedOutput;

	///  @name Members

	Glib::RefPtr <Gio::FileMonitor> fileMonitor;

};

} // X3D
} // titania

#endif
