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

#include "Inline.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/Scene.h"
#include "../../Execution/X3DExecutionContext.h"
#include <iostream>
#include <iterator>

namespace titania {
namespace X3D {

Inline::Inline (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	    X3DChildNode (),                                                    
	X3DBoundedObject (),                                                    
	    X3DUrlObject (),                                                    
	            load (true)                                                 // SFBool [in,out] load  TRUE
{
	setComponent ("Networking");
	setTypeName ("Inline");

	addField (inputOutput,    "metadata",   metadata);
	addField (inputOutput,    "load",       load);
	addField (inputOutput,    "url",        url);
	addField (initializeOnly, "bboxSize",   bboxSize);
	addField (initializeOnly, "bboxCenter", bboxCenter);

	setChildren (scene);
}

X3DBaseNode*
Inline::create (X3DExecutionContext* const executionContext) const
{
	return new Inline (executionContext);
}

void
Inline::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();
	X3DUrlObject::initialize ();

	load  .addInterest (this, &Inline::set_load);
	url   .addInterest (this, &Inline::set_url);
	scene .addInterest (this, &Inline::set_scene);
	
	scene = getBrowser () -> createScene ();

	set_url ();
}

Box3f
Inline::getBBox ()
{
	return X3DBoundedObject::getBBox (scene -> getRootNodes ());
}

void
Inline::set_load ()
{
	if (load)
		requestLoad ();
		
	else
		requestUnload ();
}

void
Inline::set_url ()
{
	setLoadState (NOT_STARTED_STATE);

	if (load)
		requestLoad ();
}

void
Inline::set_scene ()
{
	const_cast <MFNode <X3DBaseNode> &> (scene -> getRootNodes ()) .addParent (this);

	if (getBrowser () -> getBrowserOptions () -> enableInlineViewpoints)
	{
		for (auto & layer : getLayers ())
		{
			for (const auto & sceneLayer : scene -> getLayerSet () -> getLayers ())
			{
				for (auto & navigationInfo : sceneLayer -> getNavigationInfos ())
					navigationInfo -> addToLayer (layer);

				for (auto & background : sceneLayer -> getBackgrounds ())
					background -> addToLayer (layer);

				for (auto & fog : sceneLayer -> getFogs ())
					fog -> addToLayer (layer);

				for (auto & viewpoint : sceneLayer -> getViewpoints ())
					viewpoint -> addToLayer (layer);
			}
		}
	}
}

void
Inline::requestLoad ()
{
	requestImmediateLoad ();
}

void
Inline::requestImmediateLoad ()
{
	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	try
	{
		const_cast <MFNode <X3DBaseNode> &> (scene -> getRootNodes ()) .removeParent (this);
	
		scene = createX3DFromURL (url);

		setLoadState (COMPLETE_STATE);
	}
	catch (const X3DError & error)
	{
		setLoadState (FAILED_STATE);

		std::clog << error .what () << std::endl;

		std::copy (urlError .cbegin (), urlError .cend (),
		           std::ostream_iterator <String> (std::clog, "\n"));
	}
}

void
Inline::requestUnload ()
{
	if (checkLoadState () == NOT_STARTED_STATE or checkLoadState () == FAILED_STATE)
		return;
		
	const_cast <MFNode <X3DBaseNode> &> (scene -> getRootNodes ()) .removeParent (this);

	scene = getBrowser () -> createScene ();
		
	setLoadState (NOT_STARTED_STATE);
}

void
Inline::pick ()
{
	for (const auto & rootNode : scene -> getRootNodes ())
		rootNode -> pick ();
}

void
Inline::display ()
{
	for (const auto & rootNode : scene -> getRootNodes ())
		rootNode -> display ();
}

void
Inline::dispose ()
{
	const_cast <MFNode <X3DBaseNode> &> (scene -> getRootNodes ()) .removeParent (this);
	
	scene .dispose ();

	X3DUrlObject::dispose ();
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
