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
#ifndef __TMP_GLAD2CPP_FILE_IMPORT_DIALOG_H__
#define __TMP_GLAD2CPP_FILE_IMPORT_DIALOG_H__

#include "../Base/X3DDialogInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DFileImportDialogInterface :
	public X3DDialogInterface
{
public:

	X3DFileImportDialogInterface () :
		X3DDialogInterface ()
	{ }

	template <class ... Arguments>
	X3DFileImportDialogInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DDialogInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	virtual
	const std::string &
	getWidgetName () const
	{ return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAll () const
	{ return m_FileFilterAll; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAudio () const
	{ return m_FileFilterAudio; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterImage () const
	{ return m_FileFilterImage; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterVideo () const
	{ return m_FileFilterVideo; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterX3D () const
	{ return m_FileFilterX3D; }

	Gtk::Image &
	getImportImage1 () const
	{ return *m_ImportImage1; }

	Gtk::FileChooserDialog &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Dialog &
	getImportDialog () const
	{ return *m_ImportDialog; }

	Gtk::RadioButton &
	getImportSceneButton () const
	{ return *m_ImportSceneButton; }

	Gtk::RadioButton &
	getImportAsInlineButton () const
	{ return *m_ImportAsInlineButton; }

	Gtk::RadioButton &
	getImportExternProtosButton () const
	{ return *m_ImportExternProtosButton; }

	Gtk::RadioButton &
	getImportProtosButton () const
	{ return *m_ImportProtosButton; }

	virtual
	~X3DFileImportDialogInterface ();


private:

	virtual
	void
	construct () final override
	{ X3DDialogInterface::construct (); }

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	std::deque <sigc::connection>  m_connections;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterAll;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterAudio;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterImage;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterVideo;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterX3D;
	Gtk::Image*                    m_ImportImage1;
	Gtk::FileChooserDialog*        m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Dialog*                   m_ImportDialog;
	Gtk::RadioButton*              m_ImportSceneButton;
	Gtk::RadioButton*              m_ImportAsInlineButton;
	Gtk::RadioButton*              m_ImportExternProtosButton;
	Gtk::RadioButton*              m_ImportProtosButton;

};

} // puck
} // titania

#endif
