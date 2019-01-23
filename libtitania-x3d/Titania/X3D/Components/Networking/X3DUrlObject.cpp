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

#include "X3DUrlObject.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include <regex>

namespace titania {
namespace X3D {

X3DUrlObject::Fields::Fields () :
	url (new MFString ())
{ }

X3DUrlObject::X3DUrlObject () :
	    X3DBaseNode (),
	         fields (),
	      loadState (NOT_STARTED_STATE),
	      loadedUrl (),
 fileChangedOutput (),
	    fileMonitor ()
{
	addType (X3DConstants::X3DUrlObject);

	addChildObjects (loadState, fileChangedOutput);
}

X3DUrlObject*
X3DUrlObject::copy (X3DExecutionContext* const executionContext, const CopyType type) const
{
	X3DUrlObject* const copy = dynamic_cast <X3DUrlObject*> (X3DBaseNode::copy (executionContext, type));

	transform (copy -> url (), getExecutionContext () -> getWorldURL (), executionContext -> getWorldURL ());

	return copy;
}

void
X3DUrlObject::setExecutionContext (X3DExecutionContext* const executionContext)
{
	transform (url (), getExecutionContext () -> getWorldURL (), executionContext -> getWorldURL ());
}

void
X3DUrlObject::initialize ()
{
	fileChangedOutput .addInterest (&X3DUrlObject::set_file, this);
}

void
X3DUrlObject::transform (MFString & url, const basic::uri & oldWorldURL, const basic::uri & newWorldURL)
{
	static const std::regex ECMAScript (R"/((^\s*(?:ecmascript|javascript|vrmlscript)\:))/");

	MFString transformed = url;

	for (MFString::reference value : transformed)
	{
		std::smatch match;
	
		if (std::regex_search (value .get () .raw (), match, ECMAScript))
			continue;

		const basic::uri URL (value .get ());
	
		if (URL .scheme () == "data")
			continue;

		if (URL .is_relative () and not URL .filename (true) .empty ())
		{
			const auto transformed = oldWorldURL .transform (URL);

			value .set (newWorldURL .relative_path (transformed) .str ());
		}
	}

	transformed .erase (std::unique (url .begin (), url .end ()), url .end ());

	url .set (transformed);
}

void
X3DUrlObject::setLoadState (const LoadState value, const bool notify)
{
	loadState = value;

	if (notify)
	{
		getBrowser () -> removeLoadCount (this);

		if (loadState == IN_PROGRESS_STATE)
			getBrowser () -> addLoadCount (this);
	}

	// Reset fileMonitor.

	if (loadState not_eq COMPLETE_STATE)
		fileMonitor .reset ();
}

void
X3DUrlObject::setLoadedUrl (const basic::uri & value)
{
	loadedUrl = value;

	monitorFile (loadedUrl);
}

void
X3DUrlObject::monitorFile (const basic::uri & URL)
{
	try
	{
		fileMonitor .reset ();

		if (URL .empty () or not URL .is_local ())
			return;

		fileMonitor = Gio::File::create_for_path (URL .path ()) -> monitor_file ();
	
		fileMonitor -> signal_changed () .connect (sigc::mem_fun (this, &X3DUrlObject::on_file_changed));
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DUrlObject::on_file_changed (const Glib::RefPtr <Gio::File> & file,
                               const Glib::RefPtr <Gio::File> & other_file,
                               Gio::FileMonitorEvent event)
{
	try
	{
		if (event not_eq Gio::FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
			return;
	
		if (checkLoadState () not_eq COMPLETE_STATE)
			return;
	
		const auto fileInfo = file -> query_info ();
	
		fileChangedOutput = fileInfo -> modification_time () .as_double ();
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DUrlObject::set_file ()
{
	if (not getBrowser () -> getMonitorFiles ())
		return;

	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();
}

void
X3DUrlObject::dispose ()
{
	getBrowser () -> removeLoadCount (this);

	fileMonitor .reset ();
}

} // X3D
} // titania
