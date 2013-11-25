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

#ifndef __TITANIA_OUTLINE_EDITOR_OUTLINE_USER_DATA_H__
#define __TITANIA_OUTLINE_EDITOR_OUTLINE_USER_DATA_H__

#include <Titania/X3D.h>
#include <gtkmm.h>

namespace titania {
namespace puck {

constexpr int OUTLINE_SELECTED        = 1;
constexpr int OUTLINE_SELECTED_INPUT  = 1 << 1;
constexpr int OUTLINE_SELECTED_OUTPUT = 1 << 2;
constexpr int OUTLINE_OVER_INPUT      = 1 << 3;
constexpr int OUTLINE_OVER_OUTPUT     = 1 << 4;

class OutlineUserData :
	public X3D::X3DBase
{
public:

	OutlineUserData () :
		   open_path (),
		       paths (),
		    expanded (false),
		all_expanded (false),
		    selected (0)
	{ }

	Gtk::TreeModel::Path            open_path; // Path of expanded node/clone
	std::set <Gtk::TreeModel::Path> paths;     // All visible paths

	bool expanded;                             // Expanded state
	bool all_expanded;                         // Expanded mode
	int  selected;                             // Selected state


private:

	OutlineUserData (const OutlineUserData &) = delete;

	OutlineUserData (OutlineUserData &&) = delete;

	OutlineUserData &
	operator = (const OutlineUserData &) = delete;

	OutlineUserData &
	operator = (OutlineUserData &&) = delete;

};

typedef std::shared_ptr <OutlineUserData> OutlineUserDataPtr;

} // puck
} // titania

#endif
