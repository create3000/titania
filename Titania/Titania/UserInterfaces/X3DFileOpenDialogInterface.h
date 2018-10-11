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

#ifndef __TMP_GLAD2CPP_FILE_OPEN_DIALOG_H__
#define __TMP_GLAD2CPP_FILE_OPEN_DIALOG_H__

#include "../Base/X3DDialogInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for FileOpenDialog.
 */
class X3DFileOpenDialogInterface :
	public X3DDialogInterface
{
public:

	///  @name Construction

	X3DFileOpenDialogInterface () :
		X3DDialogInterface ()
	{ }

	template <class ... Arguments>
	X3DFileOpenDialogInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DDialogInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DFileOpenDialogInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DDialogInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAll () const
	{ return m_FileFilterAll; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAudio () const
	{ return m_FileFilterAudio; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAutodesk3DSMax () const
	{ return m_FileFilterAutodesk3DSMax; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterCompressedVrmlEncoding () const
	{ return m_FileFilterCompressedVrmlEncoding; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterCompressedX3DClassicVRMLEncoding () const
	{ return m_FileFilterCompressedX3DClassicVRMLEncoding; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterCompressedX3DXMLEncoding () const
	{ return m_FileFilterCompressedX3DXMLEncoding; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterFonts () const
	{ return m_FileFilterFonts; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterGLTF () const
	{ return m_FileFilterGLTF; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterImage () const
	{ return m_FileFilterImage; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterPDF () const
	{ return m_FileFilterPDF; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterSVG () const
	{ return m_FileFilterSVG; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterVideo () const
	{ return m_FileFilterVideo; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterVrmlEncoding () const
	{ return m_FileFilterVrmlEncoding; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterWavefrontOBJ () const
	{ return m_FileFilterWavefrontOBJ; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterX3D () const
	{ return m_FileFilterX3D; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterX3DClassicVRMLEncoding () const
	{ return m_FileFilterX3DClassicVRMLEncoding; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterX3DJSONEncoding () const
	{ return m_FileFilterX3DJSONEncoding; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterX3DXMLEncoding () const
	{ return m_FileFilterX3DXMLEncoding; }

	Gtk::Box &
	getPreview () const
	{ return *m_Preview; }

	Gtk::Image &
	getPreviewImage () const
	{ return *m_PreviewImage; }

	Gtk::FileChooserDialog &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getRelativePathBox () const
	{ return *m_RelativePathBox; }

	Gtk::Switch &
	getRelativePathSwitch () const
	{ return *m_RelativePathSwitch; }

	///  @name Signal handlers

	virtual
	void
	on_update_preview () = 0;

	///  @name Destruction

	virtual
	~X3DFileOpenDialogInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterAll;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterAudio;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterAutodesk3DSMax;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterCompressedVrmlEncoding;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterCompressedX3DClassicVRMLEncoding;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterCompressedX3DXMLEncoding;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterFonts;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterGLTF;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterImage;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterPDF;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterSVG;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterVideo;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterVrmlEncoding;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterWavefrontOBJ;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterX3D;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterX3DClassicVRMLEncoding;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterX3DJSONEncoding;
	Glib::RefPtr <Gtk::FileFilter> m_FileFilterX3DXMLEncoding;
	Gtk::Box* m_Preview;
	Gtk::Image* m_PreviewImage;
	Gtk::FileChooserDialog* m_Window;
	Gtk::Box* m_Widget;
	Gtk::Box* m_RelativePathBox;
	Gtk::Switch* m_RelativePathSwitch;

};

} // puck
} // titania

#endif
