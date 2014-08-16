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
	getAmbientIntensityAdjustment () const
	{ return m_AmbientIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackAmbientIntensityAdjustment () const
	{ return m_BackAmbientIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackDiffuseColorAdjustment () const
	{ return m_BackDiffuseColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackEmissiveColorAdjustment () const
	{ return m_BackEmissiveColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackShininessAdjustment () const
	{ return m_BackShininessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackSpecularColorAdjustment () const
	{ return m_BackSpecularColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackTransparencyAdjustment () const
	{ return m_BackTransparencyAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDiffuseColorAdjustment () const
	{ return m_DiffuseColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEmissiveColorAdjustment () const
	{ return m_EmissiveColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getShininessAdjustment () const
	{ return m_ShininessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpecularColorAdjustment () const
	{ return m_SpecularColorAdjustment; }

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

	Gtk::Button &
	getSphereButton () const
	{ return *m_SphereButton; }

	Gtk::Button &
	getModelButton () const
	{ return *m_ModelButton; }

	Gtk::Box &
	getMaterialBox () const
	{ return *m_MaterialBox; }

	Gtk::ComboBoxText &
	getMaterialComboBoxText () const
	{ return *m_MaterialComboBoxText; }

	Gtk::Button &
	getMaterialUnlinkButton () const
	{ return *m_MaterialUnlinkButton; }

	Gtk::Expander &
	getFrontExpander () const
	{ return *m_FrontExpander; }

	Gtk::Grid &
	getFrontBox () const
	{ return *m_FrontBox; }

	Gtk::Box &
	getAmbientIntensityBox () const
	{ return *m_AmbientIntensityBox; }

	Gtk::Scale &
	getAmbientIntensityScale () const
	{ return *m_AmbientIntensityScale; }

	Gtk::Box &
	getShininessBox () const
	{ return *m_ShininessBox; }

	Gtk::Scale &
	getShininessScale () const
	{ return *m_ShininessScale; }

	Gtk::Box &
	getTransparencyBox () const
	{ return *m_TransparencyBox; }

	Gtk::Scale &
	getTransparencyScale () const
	{ return *m_TransparencyScale; }

	Gtk::Box &
	getDiffuseColorBox () const
	{ return *m_DiffuseColorBox; }

	Gtk::Button &
	getDiffuseColorButton () const
	{ return *m_DiffuseColorButton; }

	Gtk::Box &
	getSpecularColorBox () const
	{ return *m_SpecularColorBox; }

	Gtk::Button &
	getSpecularColorButton () const
	{ return *m_SpecularColorButton; }

	Gtk::Box &
	getEmissiveColorBox () const
	{ return *m_EmissiveColorBox; }

	Gtk::Button &
	getEmissiveColorButton () const
	{ return *m_EmissiveColorButton; }

	Gtk::Expander &
	getBackExpander () const
	{ return *m_BackExpander; }

	Gtk::Grid &
	getBackBox () const
	{ return *m_BackBox; }

	Gtk::CheckButton &
	getSeparateBackColorCheckButton () const
	{ return *m_SeparateBackColorCheckButton; }

	Gtk::Box &
	getBackAmbientIntensityBox () const
	{ return *m_BackAmbientIntensityBox; }

	Gtk::Scale &
	getBackAmbientIntensityScale () const
	{ return *m_BackAmbientIntensityScale; }

	Gtk::Box &
	getBackShininessBox () const
	{ return *m_BackShininessBox; }

	Gtk::Scale &
	getBackShininessScale () const
	{ return *m_BackShininessScale; }

	Gtk::Box &
	getBackTransparencyBox () const
	{ return *m_BackTransparencyBox; }

	Gtk::Scale &
	getBackTransparencyScale () const
	{ return *m_BackTransparencyScale; }

	Gtk::Box &
	getBackDiffuseColorBox () const
	{ return *m_BackDiffuseColorBox; }

	Gtk::Button &
	getBackDiffuseColorButton () const
	{ return *m_BackDiffuseColorButton; }

	Gtk::Box &
	getBackSpecularColorBox () const
	{ return *m_BackSpecularColorBox; }

	Gtk::Button &
	getBackSpecularColorButton () const
	{ return *m_BackSpecularColorButton; }

	Gtk::Box &
	getBackEmissiveColorBox () const
	{ return *m_BackEmissiveColorBox; }

	Gtk::Button &
	getBackEmissiveColorButton () const
	{ return *m_BackEmissiveColorButton; }

	virtual
	void
	on_copy () = 0;

	virtual
	void
	on_paste () = 0;

	virtual
	void
	on_sphere_clicked () = 0;

	virtual
	void
	on_model_clicked () = 0;

	virtual
	void
	on_material_changed () = 0;

	virtual
	void
	on_material_unlink_clicked () = 0;

	virtual
	~X3DMaterialEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_AmbientIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackAmbientIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackDiffuseColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackEmissiveColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackShininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackSpecularColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackTransparencyAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DiffuseColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EmissiveColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ShininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpecularColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransparencyAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::MenuItem*                 m_EditMenuItem;
	Gtk::ImageMenuItem*            m_CopyMenuItem;
	Gtk::ImageMenuItem*            m_PasteMenuItem;
	Gtk::Box*                      m_PreviewBox;
	Gtk::Button*                   m_SphereButton;
	Gtk::Button*                   m_ModelButton;
	Gtk::Box*                      m_MaterialBox;
	Gtk::ComboBoxText*             m_MaterialComboBoxText;
	Gtk::Button*                   m_MaterialUnlinkButton;
	Gtk::Expander*                 m_FrontExpander;
	Gtk::Grid*                     m_FrontBox;
	Gtk::Box*                      m_AmbientIntensityBox;
	Gtk::Scale*                    m_AmbientIntensityScale;
	Gtk::Box*                      m_ShininessBox;
	Gtk::Scale*                    m_ShininessScale;
	Gtk::Box*                      m_TransparencyBox;
	Gtk::Scale*                    m_TransparencyScale;
	Gtk::Box*                      m_DiffuseColorBox;
	Gtk::Button*                   m_DiffuseColorButton;
	Gtk::Box*                      m_SpecularColorBox;
	Gtk::Button*                   m_SpecularColorButton;
	Gtk::Box*                      m_EmissiveColorBox;
	Gtk::Button*                   m_EmissiveColorButton;
	Gtk::Expander*                 m_BackExpander;
	Gtk::Grid*                     m_BackBox;
	Gtk::CheckButton*              m_SeparateBackColorCheckButton;
	Gtk::Box*                      m_BackAmbientIntensityBox;
	Gtk::Scale*                    m_BackAmbientIntensityScale;
	Gtk::Box*                      m_BackShininessBox;
	Gtk::Scale*                    m_BackShininessScale;
	Gtk::Box*                      m_BackTransparencyBox;
	Gtk::Scale*                    m_BackTransparencyScale;
	Gtk::Box*                      m_BackDiffuseColorBox;
	Gtk::Button*                   m_BackDiffuseColorButton;
	Gtk::Box*                      m_BackSpecularColorBox;
	Gtk::Button*                   m_BackSpecularColorButton;
	Gtk::Box*                      m_BackEmissiveColorBox;
	Gtk::Button*                   m_BackEmissiveColorButton;

};

} // puck
} // titania

#endif
