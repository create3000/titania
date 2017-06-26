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
#ifndef __TMP_GLAD2CPP_RENDER_PANEL_H__
#define __TMP_GLAD2CPP_RENDER_PANEL_H__

#include "../Base/X3DPanelInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for RenderPanel.
 */
class X3DRenderPanelInterface :
	public X3DPanelInterface
{
public:

	///  @name Construction

	X3DRenderPanelInterface () :
		X3DPanelInterface ()
	{ }

	template <class ... Arguments>
	X3DRenderPanelInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DPanelInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DRenderPanelInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DPanelInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFrameAdjustment () const
	{ return m_FrameAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::MenuBar &
	getMenuBar () const
	{ return *m_MenuBar; }

	Gtk::MenuItem &
	getPanelsMenuItem () const
	{ return *m_PanelsMenuItem; }

	Gtk::ToolButton &
	getRecordButton () const
	{ return *m_RecordButton; }

	Gtk::ToolButton &
	getNewButton1 () const
	{ return *m_NewButton1; }

	Gtk::ToolButton &
	getNewButton2 () const
	{ return *m_NewButton2; }

	Gtk::ToolButton &
	getNewButton3 () const
	{ return *m_NewButton3; }

	Gtk::SpinButton &
	getFrameSpinButton () const
	{ return *m_FrameSpinButton; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_PreviewBox; }

	Gtk::Label &
	getTextureFormatLabel () const
	{ return *m_TextureFormatLabel; }

	Gtk::Label &
	getTextureLoadStateLabel () const
	{ return *m_TextureLoadStateLabel; }

	///  @name Signal handlers

	virtual
	void
	on_record_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DRenderPanelInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_FrameAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::MenuBar* m_MenuBar;
	Gtk::MenuItem* m_PanelsMenuItem;
	Gtk::ToolButton* m_RecordButton;
	Gtk::ToolButton* m_NewButton1;
	Gtk::ToolButton* m_NewButton2;
	Gtk::ToolButton* m_NewButton3;
	Gtk::SpinButton* m_FrameSpinButton;
	Gtk::Box* m_PreviewBox;
	Gtk::Label* m_TextureFormatLabel;
	Gtk::Label* m_TextureLoadStateLabel;

};

} // puck
} // titania

#endif
