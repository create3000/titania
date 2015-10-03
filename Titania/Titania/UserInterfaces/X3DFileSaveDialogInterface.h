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
#ifndef __TMP_GLAD2CPP_FILE_SAVE_DIALOG_H__
#define __TMP_GLAD2CPP_FILE_SAVE_DIALOG_H__

#include "../Base/X3DDialogInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DFileSaveDialogInterface :
	public X3DDialogInterface
{
public:

	X3DFileSaveDialogInterface () :
		X3DDialogInterface ()
	{ }

	template <class ... Arguments>
	X3DFileSaveDialogInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DDialogInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

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

	const Glib::RefPtr <Gtk::Adjustment> &
	getImageAntialiasingAdjustment () const
	{ return m_ImageAntialiasingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getImageCompressionAdjustment () const
	{ return m_ImageCompressionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getImageHeightAdjustment () const
	{ return m_ImageHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getImageWidthAdjustment () const
	{ return m_ImageWidthAdjustment; }

	Gtk::SpinButton &
	getAntialiasingSpinButton () const
	{ return *m_AntialiasingSpinButton; }

	Gtk::FileChooserDialog &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getCompressFileBox () const
	{ return *m_CompressFileBox; }

	Gtk::Switch &
	getCompressFileButton () const
	{ return *m_CompressFileButton; }

	Gtk::Dialog &
	getImageOptionsDialog () const
	{ return *m_ImageOptionsDialog; }

	Gtk::Switch &
	getImageAlphaChannelSwitch () const
	{ return *m_ImageAlphaChannelSwitch; }

	Gtk::Box &
	getImageAntialiasingBox () const
	{ return *m_ImageAntialiasingBox; }

	Gtk::MessageDialog &
	getMessageDialog () const
	{ return *m_MessageDialog; }

	virtual
	~X3DFileSaveDialogInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterAll;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterAudio;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterImage;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterVideo;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterX3D;
	Glib::RefPtr <Gtk::Adjustment> m_ImageAntialiasingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ImageCompressionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ImageHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ImageWidthAdjustment;
	Gtk::SpinButton*               m_AntialiasingSpinButton;
	Gtk::FileChooserDialog*        m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Box*                      m_CompressFileBox;
	Gtk::Switch*                   m_CompressFileButton;
	Gtk::Dialog*                   m_ImageOptionsDialog;
	Gtk::Switch*                   m_ImageAlphaChannelSwitch;
	Gtk::Box*                      m_ImageAntialiasingBox;
	Gtk::MessageDialog*            m_MessageDialog;

};

} // puck
} // titania

#endif
