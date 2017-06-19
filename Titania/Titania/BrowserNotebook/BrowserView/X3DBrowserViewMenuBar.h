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

#ifndef __TITANIA_BROWSER_NOTEBOOK_BROWSER_VIEW_X3DBROWSER_VIEW_MENU_BAR_H__
#define __TITANIA_BROWSER_NOTEBOOK_BROWSER_VIEW_X3DBROWSER_VIEW_MENU_BAR_H__

#include "X3DBrowserView.h"

namespace titania {
namespace puck {

class ViewpointObserver;

class X3DBrowserViewMenuBar :
	virtual public X3DBrowserView
{
public:

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBrowserViewMenuBar () override;


protected:

	///  @name Construction

	X3DBrowserViewMenuBar ();

	///  @name Event handlers

	virtual
	void
	on_map () override;

	virtual
	void
	on_unmap () override;


private:

	///  @name Event handlers

	void
	set_editing ();

	void
	set_undoHistory ();

	virtual
	void
	undo_view_activate () final override;

	virtual
	void
	redo_view_activate () final override;

	virtual
	void
	on_look_at_selection_activate () final override;

	virtual
	void
	on_look_at_all_activate () final override;

	virtual
	void
	on_reset_user_offsets_activate () final override;

	///  @name Members

	std::unique_ptr <ViewpointObserver> viewpointObserver;

};

} // puck
} // titania

#endif
