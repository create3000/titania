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
#ifndef __TMP_GLAD2CPP_MATERIAL_EDITOR_H__
#define __TMP_GLAD2CPP_MATERIAL_EDITOR_H__

#include "../Base/X3DDialogInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DMaterialEditorInterface :
	public X3DDialogInterface
{
public:

	template <class ... Arguments>
	X3DMaterialEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DDialogInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const std::string & name) const
	{ getBuilder () -> add_from_file (filename, name); }

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
	getAmbientAdjustment () const
	{ return m_ambientAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackAmbientAdjustment () const
	{ return m_backAmbientAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackShininessAdjustment () const
	{ return m_backShininessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackTransparencyAdjustment () const
	{ return m_backTransparencyAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getShininessAdjustment () const
	{ return m_shininessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransparencyAdjustment () const
	{ return m_transparencyAdjustment; }

	Gtk::ColorSelectionDialog &
	getBackDiffuseDialog () const
	{ return *m_backDiffuseDialog; }

	Gtk::ColorSelectionDialog &
	getBackEmissiveDialog () const
	{ return *m_backEmissiveDialog; }

	Gtk::ColorSelectionDialog &
	getBackSpecularDialog () const
	{ return *m_backSpecularDialog; }

	Gtk::ColorSelectionDialog &
	getDiffuseDialog () const
	{ return *m_diffuseDialog; }

	Gtk::ColorSelectionDialog &
	getEmissiveDialog () const
	{ return *m_emissiveDialog; }

	Gtk::ColorSelectionDialog &
	getSpecularDialog () const
	{ return *m_specularDialog; }

	Gtk::Window &
	getWindow () const
	{ return *m_window; }

	Gtk::Box &
	getWidget () const
	{ return *m_widget; }

	Gtk::MenuItem &
	getEditMenuItem () const
	{ return *m_editMenuItem; }

	Gtk::ImageMenuItem &
	getCopyMenuItem () const
	{ return *m_copyMenuItem; }

	Gtk::ImageMenuItem &
	getPasteMenuItem () const
	{ return *m_pasteMenuItem; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_previewBox; }

	Gtk::ToggleButton &
	getFrontAndBackButton () const
	{ return *m_frontAndBackButton; }

	Gtk::Expander &
	getFrontExpander () const
	{ return *m_frontExpander; }

	Gtk::Scale &
	getAmbientScale () const
	{ return *m_ambientScale; }

	Gtk::Scale &
	getShininessScale () const
	{ return *m_shininessScale; }

	Gtk::Scale &
	getTransparencyScale () const
	{ return *m_transparencyScale; }

	Gtk::Button &
	getDiffuseButton () const
	{ return *m_diffuseButton; }

	Gtk::DrawingArea &
	getDiffuseArea () const
	{ return *m_diffuseArea; }

	Gtk::Button &
	getSpecularButton () const
	{ return *m_specularButton; }

	Gtk::DrawingArea &
	getSpecularArea () const
	{ return *m_specularArea; }

	Gtk::Button &
	getEmissiveButton () const
	{ return *m_emissiveButton; }

	Gtk::DrawingArea &
	getEmissiveArea () const
	{ return *m_emissiveArea; }

	Gtk::Expander &
	getBackExpander () const
	{ return *m_backExpander; }

	Gtk::Scale &
	getBackAmbientScale () const
	{ return *m_backAmbientScale; }

	Gtk::Scale &
	getBackShininessScale () const
	{ return *m_backShininessScale; }

	Gtk::Scale &
	getBackTransparencyScale () const
	{ return *m_backTransparencyScale; }

	Gtk::Button &
	getBackDiffuseButton () const
	{ return *m_backDiffuseButton; }

	Gtk::DrawingArea &
	getBackDiffuseArea () const
	{ return *m_backDiffuseArea; }

	Gtk::Button &
	getBackSpecularButton () const
	{ return *m_backSpecularButton; }

	Gtk::DrawingArea &
	getBackSpecularArea () const
	{ return *m_backSpecularArea; }

	Gtk::Button &
	getBackEmissiveButton () const
	{ return *m_backEmissiveButton; }

	Gtk::DrawingArea &
	getBackEmissiveArea () const
	{ return *m_backEmissiveArea; }

	virtual
	void
	on_copy () = 0;

	virtual
	void
	on_paste () = 0;

	virtual
	void
	on_frontAndBackButton_toggled () = 0;

	virtual
	void
	on_ambient () = 0;

	virtual
	void
	on_shininess () = 0;

	virtual
	void
	on_transparency () = 0;

	virtual
	void
	on_diffuse_clicked () = 0;

	virtual
	bool
	on_diffuse_draw (const::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	void
	on_specular_clicked () = 0;

	virtual
	bool
	on_specular_draw (const::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	void
	on_emissive_clicked () = 0;

	virtual
	bool
	on_emissive_draw (const::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	void
	on_backAmbient () = 0;

	virtual
	void
	on_backShininess () = 0;

	virtual
	void
	on_backTransparency () = 0;

	virtual
	void
	on_backDiffuse_clicked () = 0;

	virtual
	bool
	on_backDiffuse_draw (const::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	void
	on_backSpecular_clicked () = 0;

	virtual
	bool
	on_backSpecular_draw (const::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	void
	on_backEmissive_clicked () = 0;

	virtual
	bool
	on_backEmissive_draw (const::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	~X3DMaterialEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_ambientAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_backAmbientAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_backShininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_backTransparencyAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_shininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_transparencyAdjustment;
	Gtk::ColorSelectionDialog*     m_backDiffuseDialog;
	Gtk::ColorSelectionDialog*     m_backEmissiveDialog;
	Gtk::ColorSelectionDialog*     m_backSpecularDialog;
	Gtk::ColorSelectionDialog*     m_diffuseDialog;
	Gtk::ColorSelectionDialog*     m_emissiveDialog;
	Gtk::ColorSelectionDialog*     m_specularDialog;
	Gtk::Window*                   m_window;
	Gtk::Box*                      m_widget;
	Gtk::MenuItem*                 m_editMenuItem;
	Gtk::ImageMenuItem*            m_copyMenuItem;
	Gtk::ImageMenuItem*            m_pasteMenuItem;
	Gtk::Box*                      m_previewBox;
	Gtk::ToggleButton*             m_frontAndBackButton;
	Gtk::Expander*                 m_frontExpander;
	Gtk::Scale*                    m_ambientScale;
	Gtk::Scale*                    m_shininessScale;
	Gtk::Scale*                    m_transparencyScale;
	Gtk::Button*                   m_diffuseButton;
	Gtk::DrawingArea*              m_diffuseArea;
	Gtk::Button*                   m_specularButton;
	Gtk::DrawingArea*              m_specularArea;
	Gtk::Button*                   m_emissiveButton;
	Gtk::DrawingArea*              m_emissiveArea;
	Gtk::Expander*                 m_backExpander;
	Gtk::Scale*                    m_backAmbientScale;
	Gtk::Scale*                    m_backShininessScale;
	Gtk::Scale*                    m_backTransparencyScale;
	Gtk::Button*                   m_backDiffuseButton;
	Gtk::DrawingArea*              m_backDiffuseArea;
	Gtk::Button*                   m_backSpecularButton;
	Gtk::DrawingArea*              m_backSpecularArea;
	Gtk::Button*                   m_backEmissiveButton;
	Gtk::DrawingArea*              m_backEmissiveArea;

};

} // puck
} // titania

#endif
