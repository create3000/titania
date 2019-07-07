/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_BROWSER_NOTEBOOK_BROWSER_VIEW_VIEWPOINT_OBSERVER_H__
#define __TITANIA_BROWSER_NOTEBOOK_BROWSER_VIEW_VIEWPOINT_OBSERVER_H__

#include "../../Base/X3DEditorObject.h"

#include <Titania/X3D/Browser/Navigation/X3DViewer.h>

namespace titania {
namespace puck {

class ViewpointObserver :
	public X3DEditorObject
{
public:

	/// @name Construction

	ViewpointObserver (X3DBrowserWindow* const browserWindow, const X3D::BrowserPtr & browser);

	/// @name Member access

	const std::unique_ptr <X3D::UndoHistory> &
	getUndoHistory ()
	{ return undoHistory; }

	/// @name Destruction

	virtual
	~ViewpointObserver () final override;


private:

	/// @name Viewpoint handling

	void
	set_executionContext ();

	void
	set_viewer (const X3D::X3DPtr <X3D::X3DViewer> &);

	void
	set_navigationInfo ();

	void
	set_transitionStart (const bool value);

	void
	set_transitionComplete (const bool value);

	void
	set_active (const bool);

	void
	set_scrollTime ();

	void
	set_offsets ();

	///  @name Operations

	virtual
	void
	addUndoStep (const X3D::UndoStepPtr & undoStep) final override
	{ undoHistory -> addUndoStep (undoStep, X3D::SFTime::now ()); }

	virtual
	void
	removeUndoStep () final override
	{ undoHistory -> removeUndoStep (); }

	virtual
	const X3D::UndoStepPtr &
	getUndoStep () const final override
	{ return undoHistory -> getUndoStep (); }

	///  @name Members

	X3D::BrowserPtr                    browser;
	X3D::X3DPtr <X3D::X3DViewer>       viewerNode;
	X3D::X3DPtr <X3D::NavigationInfo>  navigationInfo;
	X3D::Vector3d                      positionOffset;
	X3D::Rotation4d                    orientationOffset;
	X3D::Vector3d                      centerOfRotationOffset;
	double                             fieldOfViewScale;
	std::unique_ptr <X3D::UndoHistory> undoHistory;
	X3D::UndoStepPtr                   transitionUndoStep;
	X3D::UndoStepPtr                   moveUndoStep;
	X3D::UndoStepPtr                   scrollUndoStep;

};

} // puck
} // titania

#endif
