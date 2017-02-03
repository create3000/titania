/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
#ifndef __TMP_GLAD2CPP_APPEARANCE_EDITOR_H__
#define __TMP_GLAD2CPP_APPEARANCE_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for AppearanceEditor.
 */
class X3DAppearanceEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DAppearanceEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DAppearanceEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	///  @name Member access

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
	getFillPropertiesHatchColorAdjustment () const
	{ return m_FillPropertiesHatchColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFillPropertiesHatchStyleAdjustment () const
	{ return m_FillPropertiesHatchStyleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLinePropertiesLinetypeAdjustment () const
	{ return m_LinePropertiesLinetypeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLinePropertiesLinewidthScaleFactorAdjustment () const
	{ return m_LinePropertiesLinewidthScaleFactorAdjustment; }

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
	getCopyButton () const
	{ return *m_CopyButton; }

	Gtk::Button &
	getPasteButton () const
	{ return *m_PasteButton; }

	Gtk::Button &
	getSphereButton () const
	{ return *m_SphereButton; }

	Gtk::Button &
	getModelButton () const
	{ return *m_ModelButton; }

	Gtk::Box &
	getSelectAppearanceBox () const
	{ return *m_SelectAppearanceBox; }

	Gtk::CheckButton &
	getAppearanceCheckButton () const
	{ return *m_AppearanceCheckButton; }

	Gtk::Button &
	getAppearanceUnlinkButton () const
	{ return *m_AppearanceUnlinkButton; }

	Gtk::Notebook &
	getAppearanceChildNotebook () const
	{ return *m_AppearanceChildNotebook; }

	Gtk::Box &
	getMaterialBox () const
	{ return *m_MaterialBox; }

	Gtk::Box &
	getSelectMaterialBox () const
	{ return *m_SelectMaterialBox; }

	Gtk::ComboBoxText &
	getMaterialComboBoxText () const
	{ return *m_MaterialComboBoxText; }

	Gtk::Button &
	getMaterialUnlinkButton () const
	{ return *m_MaterialUnlinkButton; }

	Gtk::Grid &
	getMaterialNameGrid () const
	{ return *m_MaterialNameGrid; }

	Gtk::Box &
	getMaterialNameBox () const
	{ return *m_MaterialNameBox; }

	Gtk::Entry &
	getMaterialNameEntry () const
	{ return *m_MaterialNameEntry; }

	Gtk::Button &
	getMaterialRenameButton () const
	{ return *m_MaterialRenameButton; }

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

	Gtk::Expander &
	getFillPropertiesExpander () const
	{ return *m_FillPropertiesExpander; }

	Gtk::Box &
	getFillPropertiesMainBox () const
	{ return *m_FillPropertiesMainBox; }

	Gtk::Box &
	getSelectFillPropertiesBox () const
	{ return *m_SelectFillPropertiesBox; }

	Gtk::CheckButton &
	getFillPropertiesCheckButton () const
	{ return *m_FillPropertiesCheckButton; }

	Gtk::Button &
	getFillPropertiesUnlinkButton () const
	{ return *m_FillPropertiesUnlinkButton; }

	Gtk::Grid &
	getFillPropertiesBox () const
	{ return *m_FillPropertiesBox; }

	Gtk::CheckButton &
	getFillPropertiesFilledCheckButton () const
	{ return *m_FillPropertiesFilledCheckButton; }

	Gtk::CheckButton &
	getFillPropertiesHatchedCheckButton () const
	{ return *m_FillPropertiesHatchedCheckButton; }

	Gtk::Box &
	getFillPropertiesHatchColorBox () const
	{ return *m_FillPropertiesHatchColorBox; }

	Gtk::Button &
	getFillPropertiesHatchColorButton () const
	{ return *m_FillPropertiesHatchColorButton; }

	Gtk::SpinButton &
	getFillPropertiesHatchStyleSpinButton () const
	{ return *m_FillPropertiesHatchStyleSpinButton; }

	Gtk::Box &
	getFillPropertiesNameBox () const
	{ return *m_FillPropertiesNameBox; }

	Gtk::Entry &
	getFillPropertiesNameEntry () const
	{ return *m_FillPropertiesNameEntry; }

	Gtk::Button &
	getFillPropertiesRenameButton () const
	{ return *m_FillPropertiesRenameButton; }

	Gtk::Expander &
	getLinePropertiesExpander () const
	{ return *m_LinePropertiesExpander; }

	Gtk::Box &
	getLinePropertiesMainBox () const
	{ return *m_LinePropertiesMainBox; }

	Gtk::CheckButton &
	getLinePropertiesCheckButton () const
	{ return *m_LinePropertiesCheckButton; }

	Gtk::Button &
	getLinePropertiesUnlinkButton () const
	{ return *m_LinePropertiesUnlinkButton; }

	Gtk::Grid &
	getLinePropertiesBox () const
	{ return *m_LinePropertiesBox; }

	Gtk::CheckButton &
	getLinePropertiesFilledCheckButton () const
	{ return *m_LinePropertiesFilledCheckButton; }

	Gtk::SpinButton &
	getLinePropertiesLinetypeSpinButton () const
	{ return *m_LinePropertiesLinetypeSpinButton; }

	Gtk::SpinButton &
	getLinePropertiesLinewidthScaleFactorSpinButton () const
	{ return *m_LinePropertiesLinewidthScaleFactorSpinButton; }

	Gtk::Box &
	getLinePropertiesNameBox () const
	{ return *m_LinePropertiesNameBox; }

	Gtk::Entry &
	getLinePropertiesNameEntry () const
	{ return *m_LinePropertiesNameEntry; }

	Gtk::Button &
	getLinePropertiesRenameButton () const
	{ return *m_LinePropertiesRenameButton; }

	Gtk::Box &
	getUsedMaterialsBox () const
	{ return *m_UsedMaterialsBox; }

	Gtk::Box &
	getUsedMaterialsBrowserBox () const
	{ return *m_UsedMaterialsBrowserBox; }

	Gtk::Box &
	getUsedMaterialsIndexBox () const
	{ return *m_UsedMaterialsIndexBox; }

	Gtk::Box &
	getPaletteBox () const
	{ return *m_PaletteBox; }

	Gtk::ComboBoxText &
	getPaletteFaceCombo () const
	{ return *m_PaletteFaceCombo; }

	Gtk::Box &
	getPalettePreviewBox () const
	{ return *m_PalettePreviewBox; }

	Gtk::Box &
	getChangePaletteBox () const
	{ return *m_ChangePaletteBox; }

	Gtk::ComboBoxText &
	getPaletteComboBoxText () const
	{ return *m_PaletteComboBoxText; }

	Gtk::Button &
	getPalettePreviousButton () const
	{ return *m_PalettePreviousButton; }

	Gtk::Button &
	getPaletteNextButton () const
	{ return *m_PaletteNextButton; }

	Gtk::Dialog &
	getEditPaletteDialog () const
	{ return *m_EditPaletteDialog; }

	Gtk::Button &
	getEditPaletteCancelButton () const
	{ return *m_EditPaletteCancelButton; }

	Gtk::Button &
	getEditPaletteOkButton () const
	{ return *m_EditPaletteOkButton; }

	Gtk::Entry &
	getPaletteNameEntry () const
	{ return *m_PaletteNameEntry; }

	Gtk::Menu &
	getPaletteMenu () const
	{ return *m_PaletteMenu; }

	Gtk::ImageMenuItem &
	getAddPaletteMenuItem () const
	{ return *m_AddPaletteMenuItem; }

	Gtk::ImageMenuItem &
	getRemovePaletteMenuItem () const
	{ return *m_RemovePaletteMenuItem; }

	Gtk::ImageMenuItem &
	getEditPaletteMenuItem () const
	{ return *m_EditPaletteMenuItem; }

	Gtk::ImageMenuItem &
	getAddObjectToPaletteMenuItem () const
	{ return *m_AddObjectToPaletteMenuItem; }

	Gtk::ImageMenuItem &
	getRemoveObjectFromPaletteMenuItem () const
	{ return *m_RemoveObjectFromPaletteMenuItem; }

	///  @name Signal handlers

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
	on_appearance_toggled () = 0;

	virtual
	void
	on_appearance_unlink_clicked () = 0;

	virtual
	void
	on_material_changed () = 0;

	virtual
	void
	on_material_unlink_clicked () = 0;

	virtual
	void
	on_fillProperties_toggled () = 0;

	virtual
	void
	on_fillProperties_unlink_clicked () = 0;

	virtual
	void
	on_lineProperties_toggled () = 0;

	virtual
	void
	on_lineProperties_unlink_clicked () = 0;

	virtual
	void
	on_palette_face_changed () = 0;

	virtual
	bool
	on_palette_button_press_event (GdkEventButton* event) = 0;

	virtual
	void
	on_palette_changed () = 0;

	virtual
	void
	on_palette_previous_clicked () = 0;

	virtual
	void
	on_palette_next_clicked () = 0;

	virtual
	void
	on_edit_palette_cancel_clicked () = 0;

	virtual
	void
	on_edit_palette_ok_clicked () = 0;

	virtual
	void
	on_palette_name_changed () = 0;

	virtual
	void
	on_palette_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_palette_name_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_add_palette_activate () = 0;

	virtual
	void
	on_remove_palette_activate () = 0;

	virtual
	void
	on_edit_palette_activate () = 0;

	virtual
	void
	on_add_object_to_palette_activate () = 0;

	virtual
	void
	on_remove_object_from_palette_activate () = 0;

	///  @name Destruction

	virtual
	~X3DAppearanceEditorInterface () override;


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ X3DEditorInterface::construct (); }

	void
	create (const std::string &);

	///  @name Static members

	static const std::string m_widgetName;

	///  @name Members

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
	Glib::RefPtr <Gtk::Adjustment> m_FillPropertiesHatchColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_FillPropertiesHatchStyleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LinePropertiesLinetypeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LinePropertiesLinewidthScaleFactorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ShininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpecularColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransparencyAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::ImageMenuItem*            m_CopyMenuItem;
	Gtk::ImageMenuItem*            m_PasteMenuItem;
	Gtk::Box*                      m_PreviewBox;
	Gtk::Button*                   m_CopyButton;
	Gtk::Button*                   m_PasteButton;
	Gtk::Button*                   m_SphereButton;
	Gtk::Button*                   m_ModelButton;
	Gtk::Box*                      m_SelectAppearanceBox;
	Gtk::CheckButton*              m_AppearanceCheckButton;
	Gtk::Button*                   m_AppearanceUnlinkButton;
	Gtk::Notebook*                 m_AppearanceChildNotebook;
	Gtk::Box*                      m_MaterialBox;
	Gtk::Box*                      m_SelectMaterialBox;
	Gtk::ComboBoxText*             m_MaterialComboBoxText;
	Gtk::Button*                   m_MaterialUnlinkButton;
	Gtk::Grid*                     m_MaterialNameGrid;
	Gtk::Box*                      m_MaterialNameBox;
	Gtk::Entry*                    m_MaterialNameEntry;
	Gtk::Button*                   m_MaterialRenameButton;
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
	Gtk::Expander*                 m_FillPropertiesExpander;
	Gtk::Box*                      m_FillPropertiesMainBox;
	Gtk::Box*                      m_SelectFillPropertiesBox;
	Gtk::CheckButton*              m_FillPropertiesCheckButton;
	Gtk::Button*                   m_FillPropertiesUnlinkButton;
	Gtk::Grid*                     m_FillPropertiesBox;
	Gtk::CheckButton*              m_FillPropertiesFilledCheckButton;
	Gtk::CheckButton*              m_FillPropertiesHatchedCheckButton;
	Gtk::Box*                      m_FillPropertiesHatchColorBox;
	Gtk::Button*                   m_FillPropertiesHatchColorButton;
	Gtk::SpinButton*               m_FillPropertiesHatchStyleSpinButton;
	Gtk::Box*                      m_FillPropertiesNameBox;
	Gtk::Entry*                    m_FillPropertiesNameEntry;
	Gtk::Button*                   m_FillPropertiesRenameButton;
	Gtk::Expander*                 m_LinePropertiesExpander;
	Gtk::Box*                      m_LinePropertiesMainBox;
	Gtk::CheckButton*              m_LinePropertiesCheckButton;
	Gtk::Button*                   m_LinePropertiesUnlinkButton;
	Gtk::Grid*                     m_LinePropertiesBox;
	Gtk::CheckButton*              m_LinePropertiesFilledCheckButton;
	Gtk::SpinButton*               m_LinePropertiesLinetypeSpinButton;
	Gtk::SpinButton*               m_LinePropertiesLinewidthScaleFactorSpinButton;
	Gtk::Box*                      m_LinePropertiesNameBox;
	Gtk::Entry*                    m_LinePropertiesNameEntry;
	Gtk::Button*                   m_LinePropertiesRenameButton;
	Gtk::Box*                      m_UsedMaterialsBox;
	Gtk::Box*                      m_UsedMaterialsBrowserBox;
	Gtk::Box*                      m_UsedMaterialsIndexBox;
	Gtk::Box*                      m_PaletteBox;
	Gtk::ComboBoxText*             m_PaletteFaceCombo;
	Gtk::Box*                      m_PalettePreviewBox;
	Gtk::Box*                      m_ChangePaletteBox;
	Gtk::ComboBoxText*             m_PaletteComboBoxText;
	Gtk::Button*                   m_PalettePreviousButton;
	Gtk::Button*                   m_PaletteNextButton;
	Gtk::Dialog*                   m_EditPaletteDialog;
	Gtk::Button*                   m_EditPaletteCancelButton;
	Gtk::Button*                   m_EditPaletteOkButton;
	Gtk::Entry*                    m_PaletteNameEntry;
	Gtk::Menu*                     m_PaletteMenu;
	Gtk::ImageMenuItem*            m_AddPaletteMenuItem;
	Gtk::ImageMenuItem*            m_RemovePaletteMenuItem;
	Gtk::ImageMenuItem*            m_EditPaletteMenuItem;
	Gtk::ImageMenuItem*            m_AddObjectToPaletteMenuItem;
	Gtk::ImageMenuItem*            m_RemoveObjectFromPaletteMenuItem;

};

} // puck
} // titania

#endif
