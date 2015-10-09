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

#ifndef __TITANIA_BROWSER_X3DBROWSER_WINDOW_H__
#define __TITANIA_BROWSER_X3DBROWSER_WINDOW_H__

#include "../Browser/X3DBrowserEditor.h"

#include <Titania/X3D/Miscellaneous/Keys.h>

namespace titania {
namespace X3D {

class GridTool;
class AngleTool;

} // X3D
} // titania

namespace titania {
namespace puck {

class HistoryView;
class LibraryView;
class MotionBlurEditor;
class OutlineEditor;
class OutlineTreeViewEditor;
class ViewpointList;
class Console;
class ScriptEditor;
class AnimationEditor;
class GridTool;
class AngleTool;

class X3DBrowserWindow :
	public X3DBrowserEditor
{
public:

	using X3DBrowserEditor::getWorldInfo;

	/// @name Member access

	X3D::Keys &
	getKeys ()
	{ return keys; }

	const X3D::Keys &
	getKeys () const
	{ return keys; }

	void
	hasAccelerators (const bool);

	bool
	hasAccelerators () const
	{ return accelerators; }

	virtual
	X3D::WorldInfoPtr
	getWorldInfo (const bool)
	throw (X3D::Error <X3D::NOT_SUPPORTED>) final override;

	void
	hasGridTool (const bool);

	bool
	hasGridTool () const;

	const X3D::X3DPtr <X3D::GridTool> &
	getGridTool () const;

	void
	hasAngleTool (const bool);

	bool
	hasAngleTool () const;

	const X3D::X3DPtr <X3D::AngleTool> &
	getAngleTool () const;

	/// @name Operations

	virtual
	bool
	save (const basic::uri &, const bool, const bool) final override;

	void
	expandNodes (const X3D::MFNode &);

	///  @name Destruction

	virtual
	~X3DBrowserWindow ();


protected:

	/// @name Construction

	X3DBrowserWindow (const X3D::BrowserPtr &);

	virtual
	void
	initialize ();

	virtual
	void
	setBrowser (const X3D::BrowserPtr &) override;



private:

	/// @name Member access

	const std::shared_ptr <OutlineTreeViewEditor> &
	getOutlineTreeView () const;

	/// @name Operations

	void
	expandNodesImpl (const X3D::MFNode &);

	///  @name Members

	std::unique_ptr <ViewpointList>   viewpointList;
	std::unique_ptr <HistoryView>     historyEditor;
	std::unique_ptr <LibraryView>     libraryView;
	std::unique_ptr <OutlineEditor>   outlineEditor;
	std::unique_ptr <Console>         console;
	std::unique_ptr <ScriptEditor>    scriptEditor;
	std::unique_ptr <AnimationEditor> animationEditor;
	std::unique_ptr <GridTool>        gridTool;
	std::unique_ptr <AngleTool>       angleTool;

	X3D::Keys keys;
	bool      accelerators;

};

} // puck
} // titania

#endif
