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

#include "../Base/X3DDialogInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DTextureEditorInterface :
	public X3DDialogInterface
{
public:

	template <class ... Arguments>
	X3DTextureEditorInterface (const std::string & filename, const Arguments & ... arguments) :
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
	getAlphaAdjustment () const
	{ return m_AlphaAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAnisotropicDegreeAdjustment () const
	{ return m_AnisotropicDegreeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBorderWidthAdjustment () const
	{ return m_BorderWidthAdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getBoundryModeListStore () const
	{ return m_BoundryModeListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getFunctionListStore () const
	{ return m_FunctionListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getMagnificationFilterListStore () const
	{ return m_MagnificationFilterListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getMinificationFilterListStore () const
	{ return m_MinificationFilterListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getModeListStore () const
	{ return m_ModeListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRotationAdjustment () const
	{ return m_RotationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getScaleSAdjustment () const
	{ return m_ScaleSAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getScaleTdjustment () const
	{ return m_ScaleTdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getSourceListStore () const
	{ return m_SourceListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getTextureCompressionListStore () const
	{ return m_TextureCompressionListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTexturePriorityAdjustment () const
	{ return m_TexturePriorityAdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getTextureUnitListStore () const
	{ return m_TextureUnitListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTranslateSAdjustment () const
	{ return m_TranslateSAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTranslateTAdjustment () const
	{ return m_TranslateTAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::ImageMenuItem &
	getCopyMenuItem () const
	{ return *m_CopyMenuItem; }

	Gtk::ImageMenuItem &
	getPasteMenuItem () const
	{ return *m_PasteMenuItem; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_PreviewBox; }

	Gtk::Expander &
	getMultiTextureExpander () const
	{ return *m_MultiTextureExpander; }

	Gtk::Expander &
	getTextureExpander () const
	{ return *m_TextureExpander; }

	Gtk::Grid &
	getTextureGrid () const
	{ return *m_TextureGrid; }

	Gtk::Expander &
	getTexturePropertiesExpander () const
	{ return *m_TexturePropertiesExpander; }

	Gtk::Expander &
	getTextureTransformExpander () const
	{ return *m_TextureTransformExpander; }

	Gtk::Grid &
	getTextureTranformGrid () const
	{ return *m_TextureTranformGrid; }

	Gtk::SpinButton &
	getTranslateSButton () const
	{ return *m_TranslateSButton; }

	Gtk::SpinButton &
	getTranslateTButton () const
	{ return *m_TranslateTButton; }

	Gtk::Scale &
	getRotationScale () const
	{ return *m_RotationScale; }

	Gtk::SpinButton &
	getScaleSButton () const
	{ return *m_ScaleSButton; }

	Gtk::SpinButton &
	getScaleTButton () const
	{ return *m_ScaleTButton; }

	virtual
	~X3DTextureEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_AlphaAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AnisotropicDegreeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BorderWidthAdjustment;
	Glib::RefPtr <Gtk::ListStore>  m_BoundryModeListStore;
	Glib::RefPtr <Gtk::ListStore>  m_FunctionListStore;
	Glib::RefPtr <Gtk::ListStore>  m_MagnificationFilterListStore;
	Glib::RefPtr <Gtk::ListStore>  m_MinificationFilterListStore;
	Glib::RefPtr <Gtk::ListStore>  m_ModeListStore;
	Glib::RefPtr <Gtk::Adjustment> m_RotationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ScaleSAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ScaleTdjustment;
	Glib::RefPtr <Gtk::ListStore>  m_SourceListStore;
	Glib::RefPtr <Gtk::ListStore>  m_TextureCompressionListStore;
	Glib::RefPtr <Gtk::Adjustment> m_TexturePriorityAdjustment;
	Glib::RefPtr <Gtk::ListStore>  m_TextureUnitListStore;
	Glib::RefPtr <Gtk::Adjustment> m_TranslateSAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TranslateTAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::ImageMenuItem*            m_CopyMenuItem;
	Gtk::ImageMenuItem*            m_PasteMenuItem;
	Gtk::Box*                      m_PreviewBox;
	Gtk::Expander*                 m_MultiTextureExpander;
	Gtk::Expander*                 m_TextureExpander;
	Gtk::Grid*                     m_TextureGrid;
	Gtk::Expander*                 m_TexturePropertiesExpander;
	Gtk::Expander*                 m_TextureTransformExpander;
	Gtk::Grid*                     m_TextureTranformGrid;
	Gtk::SpinButton*               m_TranslateSButton;
	Gtk::SpinButton*               m_TranslateTButton;
	Gtk::Scale*                    m_RotationScale;
	Gtk::SpinButton*               m_ScaleSButton;
	Gtk::SpinButton*               m_ScaleTButton;

};

} // puck
} // titania

#endif
