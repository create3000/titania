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
#ifndef __TMP_GLAD2CPP_PRECISION_PLACEMENT_PANEL_H__
#define __TMP_GLAD2CPP_PRECISION_PLACEMENT_PANEL_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DPrecisionPlacementPanelInterface :
	public X3DEditorInterface
{
public:

	template <class ... Arguments>
	X3DPrecisionPlacementPanelInterface (const std::string & filename, const Arguments & ... arguments) :
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
	getBBoxCenterXAdjustment () const
	{ return m_BBoxCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxCenterYAdjustment () const
	{ return m_BBoxCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxCenterZAdjustment () const
	{ return m_BBoxCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxSizeXAdjustment () const
	{ return m_BBoxSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxSizeYAdjustment () const
	{ return m_BBoxSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxSizeZAdjustment () const
	{ return m_BBoxSizeZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformCenterXAdjustment () const
	{ return m_TransformCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformCenterYAdjustment () const
	{ return m_TransformCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformCenterZAdjustment () const
	{ return m_TransformCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformRotationAAdjustment () const
	{ return m_TransformRotationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformRotationXAdjustment () const
	{ return m_TransformRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformRotationYAdjustment () const
	{ return m_TransformRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformRotationZAdjustment () const
	{ return m_TransformRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleOrientationAAdjustment () const
	{ return m_TransformScaleOrientationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleOrientationXAdjustment () const
	{ return m_TransformScaleOrientationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleOrientationYAdjustment () const
	{ return m_TransformScaleOrientationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleOrientationZAdjustment () const
	{ return m_TransformScaleOrientationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleXAdjustment () const
	{ return m_TransformScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleYAdjustment () const
	{ return m_TransformScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleZAdjustment () const
	{ return m_TransformScaleZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformTranslationXAdjustment () const
	{ return m_TransformTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformTranslationYAdjustment () const
	{ return m_TransformTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformTranslationZAdjustment () const
	{ return m_TransformTranslationZAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Button &
	getIndexButton () const
	{ return *m_IndexButton; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Expander &
	getTransformExpander () const
	{ return *m_TransformExpander; }

	Gtk::Grid &
	getTransformBox () const
	{ return *m_TransformBox; }

	Gtk::Box &
	getTransformTranslationBox () const
	{ return *m_TransformTranslationBox; }

	Gtk::Box &
	getTransformRotationBox () const
	{ return *m_TransformRotationBox; }

	Gtk::Box &
	getTransformRotationToolBox () const
	{ return *m_TransformRotationToolBox; }

	Gtk::Box &
	getTransformScaleOrientationBox () const
	{ return *m_TransformScaleOrientationBox; }

	Gtk::Box &
	getTransformScaleOrientationToolBox () const
	{ return *m_TransformScaleOrientationToolBox; }

	Gtk::Box &
	getTransformScaleBox () const
	{ return *m_TransformScaleBox; }

	Gtk::Box &
	getTransformCenterBox () const
	{ return *m_TransformCenterBox; }

	Gtk::Expander &
	getBoundingBoxExpander () const
	{ return *m_BoundingBoxExpander; }

	Gtk::Grid &
	getBoundingBoxBox () const
	{ return *m_BoundingBoxBox; }

	Gtk::Box &
	getBBoxSizeBox () const
	{ return *m_BBoxSizeBox; }

	Gtk::SpinButton &
	getBBoxSizeXSpinButton () const
	{ return *m_BBoxSizeXSpinButton; }

	Gtk::SpinButton &
	getBBoxSizeYSpinButton () const
	{ return *m_BBoxSizeYSpinButton; }

	Gtk::SpinButton &
	getBBoxSizeZSpinButton () const
	{ return *m_BBoxSizeZSpinButton; }

	Gtk::Box &
	getBBoxCenterBox () const
	{ return *m_BBoxCenterBox; }

	Gtk::SpinButton &
	getBBoxCenterXSpinButton () const
	{ return *m_BBoxCenterXSpinButton; }

	Gtk::SpinButton &
	getBBoxCenterYSpinButton () const
	{ return *m_BBoxCenterYSpinButton; }

	Gtk::SpinButton &
	getBBoxCenterZSpinButton () const
	{ return *m_BBoxCenterZSpinButton; }

	Gtk::Button &
	getFillBoundingBoxFieldsButton () const
	{ return *m_FillBoundingBoxFieldsButton; }

	virtual
	void
	on_index_clicked () = 0;

	virtual
	void
	on_fill_bounding_box_fields_clicked () = 0;

	virtual
	~X3DPrecisionPlacementPanelInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxSizeZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformRotationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleOrientationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleOrientationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleOrientationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleOrientationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformTranslationZAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Button*                   m_IndexButton;
	Gtk::Box*                      m_NameBox;
	Gtk::Entry*                    m_NameEntry;
	Gtk::Button*                   m_RenameButton;
	Gtk::Expander*                 m_TransformExpander;
	Gtk::Grid*                     m_TransformBox;
	Gtk::Box*                      m_TransformTranslationBox;
	Gtk::Box*                      m_TransformRotationBox;
	Gtk::Box*                      m_TransformRotationToolBox;
	Gtk::Box*                      m_TransformScaleOrientationBox;
	Gtk::Box*                      m_TransformScaleOrientationToolBox;
	Gtk::Box*                      m_TransformScaleBox;
	Gtk::Box*                      m_TransformCenterBox;
	Gtk::Expander*                 m_BoundingBoxExpander;
	Gtk::Grid*                     m_BoundingBoxBox;
	Gtk::Box*                      m_BBoxSizeBox;
	Gtk::SpinButton*               m_BBoxSizeXSpinButton;
	Gtk::SpinButton*               m_BBoxSizeYSpinButton;
	Gtk::SpinButton*               m_BBoxSizeZSpinButton;
	Gtk::Box*                      m_BBoxCenterBox;
	Gtk::SpinButton*               m_BBoxCenterXSpinButton;
	Gtk::SpinButton*               m_BBoxCenterYSpinButton;
	Gtk::SpinButton*               m_BBoxCenterZSpinButton;
	Gtk::Button*                   m_FillBoundingBoxFieldsButton;

};

} // puck
} // titania

#endif
