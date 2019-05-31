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

#ifndef __TITANIA_BROWSER_BROWSER_SELECTION_H__
#define __TITANIA_BROWSER_BROWSER_SELECTION_H__

#include "../Base/X3DBaseInterface.h"

namespace titania {
namespace puck {

class BrowserSelection :
	public X3DBaseInterface
{
public:

	///  @name Construction

	BrowserSelection (X3DBrowserWindow* const browserWindow);

	///  @name Member access

	void
	setEnabled (const bool value);

	const X3D::SFBool &
	getEnabled () const
	{ return enabled; }

	void
	setSelectMultiple (const bool value);

	const X3D::SFBool &
	getSelectMultiple () const
	{ return selectMultiple; }

	void
	setSelectLowest (const bool value);

	const X3D::SFBool &
	getSelectLowest () const
	{ return selectLowest; }

	void
	setSelectGeometry (const bool value);

	const X3D::SFBool &
	getSelectGeometry () const
	{ return selectGeometry; }

	const X3D::SFBool &
	getTouchTime () const
	{ return touchTime; }

	bool
	isSelected (const X3D::SFNode & node) const;

	void
	addNodes (const X3D::MFNode & value);

	void
	removeNodes (const X3D::MFNode & value);

	void
	clearNodes ();

	void
	setNodes (const X3D::MFNode & value);

	const X3D::MFNode &
	getNodes () const
	{ return nodes; }

	const X3D::MFNode &
	getGeometries () const
	{ return geometryNodes; }

	const X3D::MFNode &
	getHierarchy () const
	{ return hierarchy; }

	X3D::MFNode
	getParents () const;

	X3D::MFNode
	getChildren () const;

	///  @name Operations

	void
	addNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep);

	void
	removeNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep);

	void
	clearNodes (const X3D::UndoStepPtr & undoStep);

	void
	setNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep);

	void
	undoRestoreNodes (const X3D::UndoStepPtr & undoStep);

	void
	redoRestoreNodes (const X3D::UndoStepPtr & undoStep);

	///  @name Destruction

	virtual
	~BrowserSelection () final override;


private:

	///  @name Event handlers

	void
	set_browser ();

	void
	set_execution_context ();

	void
	set_nodes (const X3D::MFNode & nodes);

	void
	set_selection ();

	X3D::MFNode
	filterSelection (X3D::MFNode value);

	void
	assignNodes ();

	///  @name Members

	X3D::SFBool enabled;
	X3D::SFBool selectMultiple;
	X3D::SFBool selectLowest;
	X3D::SFBool selectGeometry;

	X3D::SFBool     touchTime;
	X3D::MFNode     nodes;
	X3D::MFNode     geometryNodes;
	X3D::MFNode     hierarchy;
	X3D::BrowserPtr browser;

};

} // puck
} // titania

#endif
