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

#include "X3DBaseInterface.h"

#include <Titania/X3D/Browser/X3DBrowser.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Execution/X3DExecutionContext.h>

#include "../Browser/X3DBrowserWindow.h"

#include <cassert>

namespace titania {
namespace puck {

const std::string X3DBaseInterface::typeName = "X3DBaseInterface";

X3DBaseInterface::X3DBaseInterface () :
	X3D::X3DParentObject (nullptr),
	       browserWindow (nullptr)
{
	assert (false);
}

X3DBaseInterface::X3DBaseInterface (X3DBrowserWindow* const browserWindow, X3D::Browser* const browser) :
	     sigc::trackable (),
	X3D::X3DParentObject (browser),
	       browserWindow (browserWindow)
{
	assert (browserWindow);
}

void
X3DBaseInterface::setup ()
{
	X3D::X3DParentObject::setup ();

	browserWindow -> getBrowser () .addInterest (this, &X3DBaseInterface::set_browser);
}

const X3D::BrowserPtr &
X3DBaseInterface::getMasterBrowser () const
{
	return browserWindow -> getMasterBrowser ();
}

const X3D::BrowserPtr &
X3DBaseInterface::getBrowser () const
{
	return browserWindow -> getBrowser ();
}

const X3D::WorldPtr &
X3DBaseInterface::getWorld () const
{
	return getBrowser () -> getWorld ();
}

const X3D::X3DScenePtr &
X3DBaseInterface::getScene () const
{
	return browserWindow -> getScene ();
}
void
X3DBaseInterface::setExecutionContext (const X3D::X3DExecutionContextPtr & executionContext)
{
	return browserWindow -> setExecutionContext (executionContext);
}

const X3D::X3DExecutionContextPtr &
X3DBaseInterface::getExecutionContext () const
{
	return browserWindow -> getExecutionContext ();
}

X3D::X3DExecutionContext*
X3DBaseInterface::getRootContext () const
{
	if (getExecutionContext () -> isRootContext ())
		return getExecutionContext ();

	return getExecutionContext () -> getRootContext ();
}

bool
X3DBaseInterface::inProtoDeclaration () const
{
	return getExecutionContext () -> isProtoDeclaration ();
}

bool
X3DBaseInterface::inPrototypeInstance () const
{
	return dynamic_cast <X3D::X3DPrototypeInstance*> (getExecutionContext () .getValue ());
}

X3D::WorldInfoPtr
X3DBaseInterface::createWorldInfo ()
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	return getWorldInfo (true);
}

X3D::WorldInfoPtr
X3DBaseInterface::getWorldInfo () const
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	return const_cast <X3DBaseInterface*> (this) -> getWorldInfo (false);
}

X3D::WorldInfoPtr
X3DBaseInterface::getWorldInfo (const bool create)
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	return browserWindow -> getWorldInfo (create);
}

void
X3DBaseInterface::addUndoStep (const UndoStepPtr & undoStep)
{
	browserWindow -> addUndoStep (undoStep);
}

void
X3DBaseInterface::removeUndoStep ()
{
	browserWindow -> removeUndoStep ();
}

const std::shared_ptr <UndoStep> &
X3DBaseInterface::getUndoStep () const
{
	return browserWindow -> getUndoStep ();
}

void
X3DBaseInterface::set_browser (const X3D::BrowserPtr & value)
{
	setBrowser (value);
}

X3DBaseInterface::~X3DBaseInterface ()
{ }

} // puck
} // titania
