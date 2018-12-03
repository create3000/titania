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

#ifndef __TITANIA_EDITORS_X3D_EXTERNAL_TOOLS_EDITOR_EXTERNAL_TOOLS_EDITOR_H__
#define __TITANIA_EDITORS_X3D_EXTERNAL_TOOLS_EDITOR_EXTERNAL_TOOLS_EDITOR_H__

#include "../../UserInterfaces/X3DExternalToolsEditorInterface.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcpp"
#include <gtksourceviewmm.h>
#pragma GCC diagnostic pop

namespace titania {
namespace puck {

class ExternalTool;
class Pipe;

class X3DExternalToolsEditor :
	virtual public X3DExternalToolsEditorInterface
{
public:

	///  @name Operations

	static
	void
	createMenu (X3DBrowserWindow* const browserWindow, Gtk::MenuItem & menuItem);

	static
	void
	launchTool (X3DBrowserWindow* const browserWindow,
	            const std::string & name,
	            const std::string & saveType,
	            const std::string & inputType,
	            const std::string & inputEncoding,
	            const std::string & outputType,
	            const std::string & command);

	///  @name Destruction

	virtual
	~X3DExternalToolsEditor () override;


protected:

	///  @name Construction

	X3DExternalToolsEditor ();

	virtual
	void
	initialize () override
	{ }

	virtual
	void
	configure () override
	{ }

	///  @name Member access

	static
	Glib::RefPtr <Gio::File>
	getToolsFolder ();

	std::string
	createTool ();

	void
	removeTool (const Gtk::TreeIter & iter);

	void
	setId (const Gtk::TreeIter & iter, const std::string & value) const;

	std::string
	getId (const Gtk::TreeIter & iter) const;

	void
	setName (const Gtk::TreeIter & iter, const std::string & value) const;

	std::string
	getName (const Gtk::TreeIter & iter) const;

	void
	setText (const std::string & id, const std::string & text) const;

	static
	std::string
	getText (const std::string & id);

	void
	setShortcutKey (const Gtk::TreeIter & iter, const std::string & value) const;

	std::string
	getShortcutKey (const Gtk::TreeIter & iter) const;

	void
	setSaveType (const Gtk::TreeIter & iter, const std::string & value) const;

	std::string
	getSaveType (const Gtk::TreeIter & iter) const;

	void
	setInputType (const Gtk::TreeIter & iter, const std::string & value) const;

	std::string
	getInputType (const Gtk::TreeIter & iter) const;

	void
	setInputEncoding (const Gtk::TreeIter & iter, const std::string & value) const;

	std::string
	getInputEncoding (const Gtk::TreeIter & iter) const;

	void
	setOutputType (const Gtk::TreeIter & iter, const std::string & value) const;

	std::string
	getOutputType (const Gtk::TreeIter & iter) const;

	void
	setApplicabilityType (const Gtk::TreeIter & iter, const std::string & value) const;

	std::string
	getApplicabilityType (const Gtk::TreeIter & iter) const;

	void
	setExpanded (const Gtk::TreeIter & iter, const bool value) const;
	
	bool
	getExpanded (const Gtk::TreeIter & iter) const;

	void
	assignIter (const Gtk::TreeIter & iter, const Gtk::TreeIter & other);

	void
	setLanguage (const std::string & text);

	void
	restoreTree ();

	void
	saveTree ();

	///  @name Destruction

	virtual
	void
	store () override
	{ }


private:

	///  @name Member types

	struct Columns;

	///  @name Member access

	void
	restoreTree (const X3D::X3DPtr <X3D::WorldInfo> & worldInfo,
	             const std::string & key,
	             const Gtk::TreeIter & parent);

	void
	saveTree (const Gtk::TreeNodeChildren & children, const X3D::X3DPtr <X3D::WorldInfo> & worldInfo) const;

	static
	void
	createMenu (X3DBrowserWindow* const browserWindow,
	            const X3D::X3DPtr <X3D::WorldInfo> & worldInfo,
	            const std::string & key,
	            Gtk::MenuItem* const menuItem,
	            Gtk::Menu* menu);

	static
	void
	on_tool_activate (X3DBrowserWindow* const browserWindow, const std::string & key);

	static
	void
	removeTool (ExternalTool* const externalTool, const std::string & name);

	///  @name Static member

	static std::map <ExternalTool*, std::unique_ptr <ExternalTool>> externalTools;

};

} // puck
} // titania

#endif
