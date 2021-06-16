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

#ifndef __TITANIA_WIDGETS_SCRIPT_EDITOR_X3DSCRIPT_EDITOR_PREFERENCES_H__
#define __TITANIA_WIDGETS_SCRIPT_EDITOR_X3DSCRIPT_EDITOR_PREFERENCES_H__

#include "X3DScriptEditor.h"

namespace titania {
namespace puck {

class X3DScriptEditorPreferences :
	virtual public X3DScriptEditor
{
public:

	virtual
	~X3DScriptEditorPreferences () override;


protected:

	///  @name Construction

	X3DScriptEditorPreferences ();

	virtual
	void
	initialize () override;

	virtual
	void
	configure () override;

	///  @name Destruction

	virtual
	void
	store () override
	{ }


private:

	///  @name Event handlers

	virtual
	void
	on_preferences_clicked () final override;

	virtual
	bool
	on_preferences_delete_event (GdkEventAny*) final override;

	virtual
	void
	on_show_line_numbers_toggled () final override;

	virtual
	void
	on_show_right_margin_toggled () final override;

	virtual
	void
	on_right_margin_changed () final override;

	virtual
	void
	on_wrap_mode_changed () final override;

	virtual
	void
	on_highlight_current_line_togged () final override;

	virtual
	void
	on_highlight_matching_brackets_toggled () final override;

	virtual
	void
	on_tab_width_changed () final override;

	virtual
	void
	on_insert_spaces_instead_of_tabs_toggled () final override;

	virtual
	void
	on_color_theme_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) final override;

	///  @name Members

	size_t themeIndex;

};

} // puck
} // titania

#endif
