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
#ifndef __TMP_GLAD2CPP_TEXTURE_EDITOR_H__
#define __TMP_GLAD2CPP_TEXTURE_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DTextureEditorInterface :
	public X3DEditorInterface
{
public:

	template <class ... Arguments>
	X3DTextureEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const Glib::ustring & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	void
	updateWidgets (const std::vector <Glib::ustring> & names) const
	{ getBuilder () -> add_from_file (filename, names); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		widget -> set_name (name);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getCenterXAdjustment () const
	{ return m_CenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCenterYAdjustment () const
	{ return m_CenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRotationAdjustment () const
	{ return m_RotationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getScaleXAdjustment () const
	{ return m_ScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getScaleYAdjustment () const
	{ return m_ScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTranslationXAdjustment () const
	{ return m_TranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTranslationYAdjustment () const
	{ return m_TranslationYAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Expander &
	getTextureExpander () const
	{ return *m_TextureExpander; }

	Gtk::ComboBoxText &
	getTextureButton () const
	{ return *m_TextureButton; }

	Gtk::Notebook &
	getTextureNotebook () const
	{ return *m_TextureNotebook; }

	Gtk::CheckButton &
	getRepeatSCheckbutton () const
	{ return *m_RepeatSCheckbutton; }

	Gtk::CheckButton &
	getRepeatTCheckButton () const
	{ return *m_RepeatTCheckButton; }

	Gtk::Expander &
	getTextureTransformExpander () const
	{ return *m_TextureTransformExpander; }

	Gtk::ComboBoxText &
	getTextureTransformButton () const
	{ return *m_TextureTransformButton; }

	Gtk::Notebook &
	getTextureTransformNotebook () const
	{ return *m_TextureTransformNotebook; }

	Gtk::SpinButton &
	getTranslationXSpinButton () const
	{ return *m_TranslationXSpinButton; }

	Gtk::SpinButton &
	getTranslationYSpinButton () const
	{ return *m_TranslationYSpinButton; }

	Gtk::SpinButton &
	getRotationSpinButton () const
	{ return *m_RotationSpinButton; }

	Gtk::SpinButton &
	getScaleXSpinButton () const
	{ return *m_ScaleXSpinButton; }

	Gtk::SpinButton &
	getScaleYSpinButton () const
	{ return *m_ScaleYSpinButton; }

	Gtk::SpinButton &
	getCenterXSpinButton () const
	{ return *m_CenterXSpinButton; }

	Gtk::SpinButton &
	getCenterYSpinButton () const
	{ return *m_CenterYSpinButton; }

	virtual
	~X3DTextureEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_CenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_RotationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TranslationYAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Expander*                 m_TextureExpander;
	Gtk::ComboBoxText*             m_TextureButton;
	Gtk::Notebook*                 m_TextureNotebook;
	Gtk::CheckButton*              m_RepeatSCheckbutton;
	Gtk::CheckButton*              m_RepeatTCheckButton;
	Gtk::Expander*                 m_TextureTransformExpander;
	Gtk::ComboBoxText*             m_TextureTransformButton;
	Gtk::Notebook*                 m_TextureTransformNotebook;
	Gtk::SpinButton*               m_TranslationXSpinButton;
	Gtk::SpinButton*               m_TranslationYSpinButton;
	Gtk::SpinButton*               m_RotationSpinButton;
	Gtk::SpinButton*               m_ScaleXSpinButton;
	Gtk::SpinButton*               m_ScaleYSpinButton;
	Gtk::SpinButton*               m_CenterXSpinButton;
	Gtk::SpinButton*               m_CenterYSpinButton;

};

} // puck
} // titania

#endif
