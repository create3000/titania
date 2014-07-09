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

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DMaterialEditorInterface :
	public X3DEditorInterface
{
public:

	template <class ... Arguments>
	X3DMaterialEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
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
	{ return m_AmbientAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackAmbientAdjustment () const
	{ return m_BackAmbientAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackShininessAdjustment () const
	{ return m_BackShininessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackTransparencyAdjustment () const
	{ return m_BackTransparencyAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getShininessAdjustment () const
	{ return m_ShininessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransparencyAdjustment () const
	{ return m_TransparencyAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::MenuItem &
	getEditMenuItem () const
	{ return *m_EditMenuItem; }

	Gtk::ImageMenuItem &
	getCopyMenuItem () const
	{ return *m_CopyMenuItem; }

	Gtk::ImageMenuItem &
	getPasteMenuItem () const
	{ return *m_PasteMenuItem; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_PreviewBox; }

	Gtk::Box &
	getMaterialBox () const
	{ return *m_MaterialBox; }

	Gtk::ToggleButton &
	getFrontAndBackButton () const
	{ return *m_FrontAndBackButton; }

	Gtk::Expander &
	getFrontExpander () const
	{ return *m_FrontExpander; }

	Gtk::Scale &
	getAmbientScale () const
	{ return *m_AmbientScale; }

	Gtk::Scale &
	getShininessScale () const
	{ return *m_ShininessScale; }

	Gtk::Scale &
	getTransparencyScale () const
	{ return *m_TransparencyScale; }

	Gtk::Button &
	getDiffuseButton () const
	{ return *m_DiffuseButton; }

	Gtk::DrawingArea &
	getDiffuseArea () const
	{ return *m_DiffuseArea; }

	Gtk::Button &
	getSpecularButton () const
	{ return *m_SpecularButton; }

	Gtk::DrawingArea &
	getSpecularArea () const
	{ return *m_SpecularArea; }

	Gtk::Button &
	getEmissiveButton () const
	{ return *m_EmissiveButton; }

	Gtk::DrawingArea &
	getEmissiveArea () const
	{ return *m_EmissiveArea; }

	Gtk::Expander &
	getBackExpander () const
	{ return *m_BackExpander; }

	Gtk::Scale &
	getBackAmbientScale () const
	{ return *m_BackAmbientScale; }

	Gtk::Scale &
	getBackShininessScale () const
	{ return *m_BackShininessScale; }

	Gtk::Scale &
	getBackTransparencyScale () const
	{ return *m_BackTransparencyScale; }

	Gtk::Button &
	getBackDiffuseButton () const
	{ return *m_BackDiffuseButton; }

	Gtk::DrawingArea &
	getBackDiffuseArea () const
	{ return *m_BackDiffuseArea; }

	Gtk::Button &
	getBackSpecularButton () const
	{ return *m_BackSpecularButton; }

	Gtk::DrawingArea &
	getBackSpecularArea () const
	{ return *m_BackSpecularArea; }

	Gtk::Button &
	getBackEmissiveButton () const
	{ return *m_BackEmissiveButton; }

	Gtk::DrawingArea &
	getBackEmissiveArea () const
	{ return *m_BackEmissiveArea; }

	Gtk::ColorSelectionDialog &
	getBackDiffuseDialog () const
	{ return *m_BackDiffuseDialog; }

	Gtk::ColorSelectionDialog &
	getBackEmissiveDialog () const
	{ return *m_BackEmissiveDialog; }

	Gtk::ColorSelectionDialog &
	getBackSpecularDialog () const
	{ return *m_BackSpecularDialog; }

	Gtk::ColorSelectionDialog &
	getDiffuseDialog () const
	{ return *m_DiffuseDialog; }

	Gtk::ColorSelectionDialog &
	getEmissiveDialog () const
	{ return *m_EmissiveDialog; }

	Gtk::ColorSelectionDialog &
	getSpecularDialog () const
	{ return *m_SpecularDialog; }

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
	on_ambient_changed () = 0;

	virtual
	void
	on_shininess_changed () = 0;

	virtual
	void
	on_transparency_changed () = 0;

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
	on_backAmbient_changed () = 0;

	virtual
	void
	on_backShininess_changed () = 0;

	virtual
	void
	on_backTransparency_changed () = 0;

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
	Glib::RefPtr <Gtk::Adjustment> m_AmbientAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackAmbientAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackShininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackTransparencyAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ShininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransparencyAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::MenuItem*                 m_EditMenuItem;
	Gtk::ImageMenuItem*            m_CopyMenuItem;
	Gtk::ImageMenuItem*            m_PasteMenuItem;
	Gtk::Box*                      m_PreviewBox;
	Gtk::Box*                      m_MaterialBox;
	Gtk::ToggleButton*             m_FrontAndBackButton;
	Gtk::Expander*                 m_FrontExpander;
	Gtk::Scale*                    m_AmbientScale;
	Gtk::Scale*                    m_ShininessScale;
	Gtk::Scale*                    m_TransparencyScale;
	Gtk::Button*                   m_DiffuseButton;
	Gtk::DrawingArea*              m_DiffuseArea;
	Gtk::Button*                   m_SpecularButton;
	Gtk::DrawingArea*              m_SpecularArea;
	Gtk::Button*                   m_EmissiveButton;
	Gtk::DrawingArea*              m_EmissiveArea;
	Gtk::Expander*                 m_BackExpander;
	Gtk::Scale*                    m_BackAmbientScale;
	Gtk::Scale*                    m_BackShininessScale;
	Gtk::Scale*                    m_BackTransparencyScale;
	Gtk::Button*                   m_BackDiffuseButton;
	Gtk::DrawingArea*              m_BackDiffuseArea;
	Gtk::Button*                   m_BackSpecularButton;
	Gtk::DrawingArea*              m_BackSpecularArea;
	Gtk::Button*                   m_BackEmissiveButton;
	Gtk::DrawingArea*              m_BackEmissiveArea;
	Gtk::ColorSelectionDialog*     m_BackDiffuseDialog;
	Gtk::ColorSelectionDialog*     m_BackEmissiveDialog;
	Gtk::ColorSelectionDialog*     m_BackSpecularDialog;
	Gtk::ColorSelectionDialog*     m_DiffuseDialog;
	Gtk::ColorSelectionDialog*     m_EmissiveDialog;
	Gtk::ColorSelectionDialog*     m_SpecularDialog;

};

} // puck
} // titania

#endif
