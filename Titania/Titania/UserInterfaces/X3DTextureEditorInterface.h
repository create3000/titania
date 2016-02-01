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

class X3DTextureEditorInterface :
	public X3DEditorInterface
{
public:

	X3DTextureEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DTextureEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
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

	const Glib::RefPtr <Gtk::Adjustment> &
	getTexturePropertiesAnisotropicDegreeAdjustment () const
	{ return m_TexturePropertiesAnisotropicDegreeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTexturePropertiesBorderColorAdjustment () const
	{ return m_TexturePropertiesBorderColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTexturePropertiesBorderWidthAdjustment () const
	{ return m_TexturePropertiesBorderWidthAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTexturePropertiesTexturePriorityAdjustment () const
	{ return m_TexturePropertiesTexturePriorityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DCenterXAdjustment () const
	{ return m_TextureTransform3DCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DCenterYAdjustment () const
	{ return m_TextureTransform3DCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DCenterZAdjustment () const
	{ return m_TextureTransform3DCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DRotationAAdjustment () const
	{ return m_TextureTransform3DRotationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DRotationXAdjustment () const
	{ return m_TextureTransform3DRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DRotationYAdjustment () const
	{ return m_TextureTransform3DRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DRotationZAdjustment () const
	{ return m_TextureTransform3DRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DScaleXAdjustment () const
	{ return m_TextureTransform3DScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DScaleYAdjustment () const
	{ return m_TextureTransform3DScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DScaleZAdjustment () const
	{ return m_TextureTransform3DScaleZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DTranslationXAdjustment () const
	{ return m_TextureTransform3DTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DTranslationYAdjustment () const
	{ return m_TextureTransform3DTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DTranslationZAdjustment () const
	{ return m_TextureTransform3DTranslationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformCenterXAdjustment () const
	{ return m_TextureTransformCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformCenterYAdjustment () const
	{ return m_TextureTransformCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D00Adjustment () const
	{ return m_TextureTransformMatrix3D00Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D01Adjustment () const
	{ return m_TextureTransformMatrix3D01Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D02Adjustment () const
	{ return m_TextureTransformMatrix3D02Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D03Adjustment () const
	{ return m_TextureTransformMatrix3D03Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D04Adjustment () const
	{ return m_TextureTransformMatrix3D04Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D05Adjustment () const
	{ return m_TextureTransformMatrix3D05Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D06Adjustment () const
	{ return m_TextureTransformMatrix3D06Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D07Adjustment () const
	{ return m_TextureTransformMatrix3D07Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D08Adjustment () const
	{ return m_TextureTransformMatrix3D08Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D09Adjustment () const
	{ return m_TextureTransformMatrix3D09Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D10Adjustment () const
	{ return m_TextureTransformMatrix3D10Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D11Adjustment () const
	{ return m_TextureTransformMatrix3D11Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D12Adjustment () const
	{ return m_TextureTransformMatrix3D12Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D13Adjustment () const
	{ return m_TextureTransformMatrix3D13Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D14Adjustment () const
	{ return m_TextureTransformMatrix3D14Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformMatrix3D15Adjustment () const
	{ return m_TextureTransformMatrix3D15Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformRotationAdjustment () const
	{ return m_TextureTransformRotationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformScaleXAdjustment () const
	{ return m_TextureTransformScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformScaleYAdjustment () const
	{ return m_TextureTransformScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformTranslationXAdjustment () const
	{ return m_TextureTransformTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransformTranslationYAdjustment () const
	{ return m_TextureTransformTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getImageTextureURLCellRendererText () const
	{ return m_ImageTextureURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getImageTextureURLChooserColumn () const
	{ return m_ImageTextureURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getImageTextureURLCellrendererPixbuf () const
	{ return m_ImageTextureURLCellrendererPixbuf; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getMovieTextureURLCellRendererText () const
	{ return m_MovieTextureURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getMovieTextureURLChooserColumn () const
	{ return m_MovieTextureURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getMovieTextureURLCellrendererPixbuf () const
	{ return m_MovieTextureURLCellrendererPixbuf; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getImageTexture3DURLCellRendererText () const
	{ return m_ImageTexture3DURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getImageTexture3DURLChooserColumn () const
	{ return m_ImageTexture3DURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getImageTexture3DURLCellrendererPixbuf () const
	{ return m_ImageTexture3DURLCellrendererPixbuf; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Notebook &
	getTextureChildNotebook () const
	{ return *m_TextureChildNotebook; }

	Gtk::Expander &
	getTextureExpander () const
	{ return *m_TextureExpander; }

	Gtk::Box &
	getTextureBox () const
	{ return *m_TextureBox; }

	Gtk::Box &
	getSelectTextureBox () const
	{ return *m_SelectTextureBox; }

	Gtk::ComboBoxText &
	getTextureComboBoxText () const
	{ return *m_TextureComboBoxText; }

	Gtk::Button &
	getTextureUnlinkButton () const
	{ return *m_TextureUnlinkButton; }

	Gtk::Label &
	getTextureFormatLabel () const
	{ return *m_TextureFormatLabel; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_PreviewBox; }

	Gtk::Notebook &
	getTextureNotebook () const
	{ return *m_TextureNotebook; }

	Gtk::Label &
	getTextureLabel () const
	{ return *m_TextureLabel; }

	Gtk::Box &
	getImageTextureBox () const
	{ return *m_ImageTextureBox; }

	Gtk::Box &
	getImageTextureURLBox () const
	{ return *m_ImageTextureURLBox; }

	Gtk::TreeView &
	getImageTextureURLTreeView () const
	{ return *m_ImageTextureURLTreeView; }

	Gtk::Button &
	getImageTextureURLAddButton () const
	{ return *m_ImageTextureURLAddButton; }

	Gtk::Button &
	getImageTextureURLRemoveButton () const
	{ return *m_ImageTextureURLRemoveButton; }

	Gtk::Button &
	getImageTextureEmbedButton () const
	{ return *m_ImageTextureEmbedButton; }

	Gtk::Button &
	getImageTextureURLReloadButton () const
	{ return *m_ImageTextureURLReloadButton; }

	Gtk::Box &
	getPixelTextureBox () const
	{ return *m_PixelTextureBox; }

	Gtk::Box &
	getMovieTextureBox () const
	{ return *m_MovieTextureBox; }

	Gtk::Box &
	getMovieTextureURLBox () const
	{ return *m_MovieTextureURLBox; }

	Gtk::TreeView &
	getMovieTextureURLTreeView () const
	{ return *m_MovieTextureURLTreeView; }

	Gtk::Button &
	getMovieTextureURLAddButton () const
	{ return *m_MovieTextureURLAddButton; }

	Gtk::Button &
	getMovieTextureURLRemoveButton () const
	{ return *m_MovieTextureURLRemoveButton; }

	Gtk::Button &
	getMovieTextureURLReloadButton () const
	{ return *m_MovieTextureURLReloadButton; }

	Gtk::Box &
	getComposedTexture3DBox () const
	{ return *m_ComposedTexture3DBox; }

	Gtk::Box &
	getImageTexture3DBox () const
	{ return *m_ImageTexture3DBox; }

	Gtk::Box &
	getImageTexture3DURLBox () const
	{ return *m_ImageTexture3DURLBox; }

	Gtk::TreeView &
	getImageTexture3DURLTreeView () const
	{ return *m_ImageTexture3DURLTreeView; }

	Gtk::Button &
	getImageTexture3DURLAddButton () const
	{ return *m_ImageTexture3DURLAddButton; }

	Gtk::Button &
	getImageTexture3DURLRemoveButton () const
	{ return *m_ImageTexture3DURLRemoveButton; }

	Gtk::Button &
	getImageTexture3DEmbedButton () const
	{ return *m_ImageTexture3DEmbedButton; }

	Gtk::Button &
	getImageTexture3DURLReloadButton () const
	{ return *m_ImageTexture3DURLReloadButton; }

	Gtk::Box &
	getPixelTexture3DBox () const
	{ return *m_PixelTexture3DBox; }

	Gtk::Grid &
	getTexture2DBox () const
	{ return *m_Texture2DBox; }

	Gtk::CheckButton &
	getTexture2DNodeRepeatSCheckButton () const
	{ return *m_Texture2DNodeRepeatSCheckButton; }

	Gtk::CheckButton &
	getTexture2DNodeRepeatTCheckButton () const
	{ return *m_Texture2DNodeRepeatTCheckButton; }

	Gtk::Grid &
	getTexture3DBox () const
	{ return *m_Texture3DBox; }

	Gtk::CheckButton &
	getTexture3DNodeRepeatSCheckButton () const
	{ return *m_Texture3DNodeRepeatSCheckButton; }

	Gtk::CheckButton &
	getTexture3DNodeRepeatTCheckButton () const
	{ return *m_Texture3DNodeRepeatTCheckButton; }

	Gtk::CheckButton &
	getTexture3DNodeRepeatRCheckButton () const
	{ return *m_Texture3DNodeRepeatRCheckButton; }

	Gtk::Expander &
	getTexturePropertiesExpander () const
	{ return *m_TexturePropertiesExpander; }

	Gtk::Box &
	getSelectTexturePropertiesBox () const
	{ return *m_SelectTexturePropertiesBox; }

	Gtk::CheckButton &
	getTexturePropertiesCheckButton () const
	{ return *m_TexturePropertiesCheckButton; }

	Gtk::Button &
	getTexturePropertiesUnlinkButton () const
	{ return *m_TexturePropertiesUnlinkButton; }

	Gtk::Grid &
	getTexturePropertiesBox () const
	{ return *m_TexturePropertiesBox; }

	Gtk::CheckButton &
	getTexturePropertiesGenerateMipMapsCheckButton () const
	{ return *m_TexturePropertiesGenerateMipMapsCheckButton; }

	Gtk::SpinButton &
	getTexturePropertiesBorderWidthSpinButton () const
	{ return *m_TexturePropertiesBorderWidthSpinButton; }

	Gtk::SpinButton &
	getTexturePropertiesAnisotropicDegreeSpinButton () const
	{ return *m_TexturePropertiesAnisotropicDegreeSpinButton; }

	Gtk::ComboBoxText &
	getTexturePropertiesMinificationFilterComboBoxText () const
	{ return *m_TexturePropertiesMinificationFilterComboBoxText; }

	Gtk::ComboBoxText &
	getTexturePropertiesMagnificationFilterComboBoxText () const
	{ return *m_TexturePropertiesMagnificationFilterComboBoxText; }

	Gtk::ComboBoxText &
	getTexturePropertiesBoundaryModeSComboBoxText () const
	{ return *m_TexturePropertiesBoundaryModeSComboBoxText; }

	Gtk::ComboBoxText &
	getTexturePropertiesTextureCompressionComboBoxText () const
	{ return *m_TexturePropertiesTextureCompressionComboBoxText; }

	Gtk::SpinButton &
	getTexturePropertiesTexturePrioritySpinButton () const
	{ return *m_TexturePropertiesTexturePrioritySpinButton; }

	Gtk::ComboBoxText &
	getTexturePropertiesBoundaryModeTComboBoxText () const
	{ return *m_TexturePropertiesBoundaryModeTComboBoxText; }

	Gtk::ComboBoxText &
	getTexturePropertiesBoundaryModeRComboBoxText () const
	{ return *m_TexturePropertiesBoundaryModeRComboBoxText; }

	Gtk::Box &
	getTexturePropertiesBorderColorBox () const
	{ return *m_TexturePropertiesBorderColorBox; }

	Gtk::Button &
	getTexturePropertiesBorderColorButton () const
	{ return *m_TexturePropertiesBorderColorButton; }

	Gtk::Scale &
	getTexturePropertiesBorderColorScale () const
	{ return *m_TexturePropertiesBorderColorScale; }

	Gtk::Expander &
	getTextureTransformExpander () const
	{ return *m_TextureTransformExpander; }

	Gtk::Box &
	getTextureTransformNodeBox () const
	{ return *m_TextureTransformNodeBox; }

	Gtk::Box &
	getSelectTextureTransformBox () const
	{ return *m_SelectTextureTransformBox; }

	Gtk::ComboBoxText &
	getTextureTransformComboBoxText () const
	{ return *m_TextureTransformComboBoxText; }

	Gtk::Button &
	getTextureTransformUnlinkButton () const
	{ return *m_TextureTransformUnlinkButton; }

	Gtk::Notebook &
	getTextureTransformNotebook () const
	{ return *m_TextureTransformNotebook; }

	Gtk::Label &
	getTextureTransformLabel () const
	{ return *m_TextureTransformLabel; }

	Gtk::Grid &
	getTextureTransformBox () const
	{ return *m_TextureTransformBox; }

	Gtk::Box &
	getTextureTransformTranslationBox () const
	{ return *m_TextureTransformTranslationBox; }

	Gtk::SpinButton &
	getTextureTransformTranslationXSpinButton () const
	{ return *m_TextureTransformTranslationXSpinButton; }

	Gtk::SpinButton &
	getTextureTransformTranslationYSpinButton () const
	{ return *m_TextureTransformTranslationYSpinButton; }

	Gtk::Box &
	getTextureTransformScaleBox () const
	{ return *m_TextureTransformScaleBox; }

	Gtk::SpinButton &
	getTextureTransformScaleXSpinButton () const
	{ return *m_TextureTransformScaleXSpinButton; }

	Gtk::SpinButton &
	getTextureTransformScaleYSpinButton () const
	{ return *m_TextureTransformScaleYSpinButton; }

	Gtk::ToggleButton &
	getTextureTransformUniformScaleButton () const
	{ return *m_TextureTransformUniformScaleButton; }

	Gtk::Image &
	getTextureTransformUniformScaleImage () const
	{ return *m_TextureTransformUniformScaleImage; }

	Gtk::Box &
	getTextureTransformCenterBox () const
	{ return *m_TextureTransformCenterBox; }

	Gtk::SpinButton &
	getTextureTransformCenterXSpinButton () const
	{ return *m_TextureTransformCenterXSpinButton; }

	Gtk::SpinButton &
	getTextureTransformCenterYSpinButton () const
	{ return *m_TextureTransformCenterYSpinButton; }

	Gtk::SpinButton &
	getTextureTransformRotationSpinButton () const
	{ return *m_TextureTransformRotationSpinButton; }

	Gtk::Grid &
	getTextureTransform3DBox () const
	{ return *m_TextureTransform3DBox; }

	Gtk::Box &
	getTextureTransform3DTranslationBox () const
	{ return *m_TextureTransform3DTranslationBox; }

	Gtk::Box &
	getTextureTransform3DRotationBox () const
	{ return *m_TextureTransform3DRotationBox; }

	Gtk::Box &
	getTextureTransform3DRotationToolBox () const
	{ return *m_TextureTransform3DRotationToolBox; }

	Gtk::Box &
	getTextureTransform3DScaleBox () const
	{ return *m_TextureTransform3DScaleBox; }

	Gtk::ToggleButton &
	getTextureTransform3DUniformScaleButton () const
	{ return *m_TextureTransform3DUniformScaleButton; }

	Gtk::Image &
	getTextureTransform3DUniformScaleImage () const
	{ return *m_TextureTransform3DUniformScaleImage; }

	Gtk::Box &
	getTextureTransform3DCenterBox () const
	{ return *m_TextureTransform3DCenterBox; }

	Gtk::Grid &
	getTextureTransformMatrix3DBox () const
	{ return *m_TextureTransformMatrix3DBox; }

	Gtk::Grid &
	getTextureTransformMatrix3DMatrixGrid () const
	{ return *m_TextureTransformMatrix3DMatrixGrid; }

	Gtk::Expander &
	getTextureCoordinateGeneratorExpander () const
	{ return *m_TextureCoordinateGeneratorExpander; }

	Gtk::Box &
	getTextureCoordinateGeneratorMainBox () const
	{ return *m_TextureCoordinateGeneratorMainBox; }

	Gtk::Box &
	getSelectTextureCoordinateGeneratorBox () const
	{ return *m_SelectTextureCoordinateGeneratorBox; }

	Gtk::CheckButton &
	getTextureCoordinateGeneratorCheckButton () const
	{ return *m_TextureCoordinateGeneratorCheckButton; }

	Gtk::Button &
	getTextureCoordinateGeneratorUnlinkButton () const
	{ return *m_TextureCoordinateGeneratorUnlinkButton; }

	Gtk::Grid &
	getTextureCoordinateGeneratorBox () const
	{ return *m_TextureCoordinateGeneratorBox; }

	Gtk::ComboBoxText &
	getTextureCoordinateGeneratorModeComboBoxText () const
	{ return *m_TextureCoordinateGeneratorModeComboBoxText; }

	Gtk::Box &
	getPaletteBox () const
	{ return *m_PaletteBox; }

	Gtk::Box &
	getPalettePreviewBox () const
	{ return *m_PalettePreviewBox; }

	Gtk::ComboBoxText &
	getPaletteComboBoxText () const
	{ return *m_PaletteComboBoxText; }

	Gtk::Button &
	getPalettePreviousButton () const
	{ return *m_PalettePreviousButton; }

	Gtk::Button &
	getPaletteNextButton () const
	{ return *m_PaletteNextButton; }

	virtual
	void
	on_texture_changed () = 0;

	virtual
	void
	on_texture_unlink_clicked () = 0;

	virtual
	void
	on_embed_image_clicked () = 0;

	virtual
	void
	on_textureProperties_toggled () = 0;

	virtual
	void
	on_textureProperties_unlink_clicked () = 0;

	virtual
	void
	on_textureTransform_changed () = 0;

	virtual
	void
	on_textureTransform_unlink_clicked () = 0;

	virtual
	void
	on_texture_transform_uniform_scale_clicked () = 0;

	virtual
	void
	on_texture_transform3D_uniform_scale_clicked () = 0;

	virtual
	void
	on_textureCoordinateGenerator_toggled () = 0;

	virtual
	void
	on_textureCoordinateGenerator_unlink_clicked () = 0;

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
	~X3DTextureEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	Glib::RefPtr <Gtk::Adjustment>         m_TexturePropertiesAnisotropicDegreeAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TexturePropertiesBorderColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TexturePropertiesBorderWidthAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TexturePropertiesTexturePriorityAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DRotationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransform3DTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D00Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D01Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D02Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D03Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D04Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D05Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D06Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D07Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D08Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D09Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D10Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D11Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D12Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D13Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D14Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformMatrix3D15Adjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformRotationAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_TextureTransformTranslationYAdjustment;
	Glib::RefPtr <Gtk::CellRendererText>   m_ImageTextureURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_ImageTextureURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_ImageTextureURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText>   m_MovieTextureURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_MovieTextureURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_MovieTextureURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText>   m_ImageTexture3DURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_ImageTexture3DURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_ImageTexture3DURLCellrendererPixbuf;
	Gtk::Window*                           m_Window;
	Gtk::Box*                              m_Widget;
	Gtk::Notebook*                         m_TextureChildNotebook;
	Gtk::Expander*                         m_TextureExpander;
	Gtk::Box*                              m_TextureBox;
	Gtk::Box*                              m_SelectTextureBox;
	Gtk::ComboBoxText*                     m_TextureComboBoxText;
	Gtk::Button*                           m_TextureUnlinkButton;
	Gtk::Label*                            m_TextureFormatLabel;
	Gtk::Box*                              m_PreviewBox;
	Gtk::Notebook*                         m_TextureNotebook;
	Gtk::Label*                            m_TextureLabel;
	Gtk::Box*                              m_ImageTextureBox;
	Gtk::Box*                              m_ImageTextureURLBox;
	Gtk::TreeView*                         m_ImageTextureURLTreeView;
	Gtk::Button*                           m_ImageTextureURLAddButton;
	Gtk::Button*                           m_ImageTextureURLRemoveButton;
	Gtk::Button*                           m_ImageTextureEmbedButton;
	Gtk::Button*                           m_ImageTextureURLReloadButton;
	Gtk::Box*                              m_PixelTextureBox;
	Gtk::Box*                              m_MovieTextureBox;
	Gtk::Box*                              m_MovieTextureURLBox;
	Gtk::TreeView*                         m_MovieTextureURLTreeView;
	Gtk::Button*                           m_MovieTextureURLAddButton;
	Gtk::Button*                           m_MovieTextureURLRemoveButton;
	Gtk::Button*                           m_MovieTextureURLReloadButton;
	Gtk::Box*                              m_ComposedTexture3DBox;
	Gtk::Box*                              m_ImageTexture3DBox;
	Gtk::Box*                              m_ImageTexture3DURLBox;
	Gtk::TreeView*                         m_ImageTexture3DURLTreeView;
	Gtk::Button*                           m_ImageTexture3DURLAddButton;
	Gtk::Button*                           m_ImageTexture3DURLRemoveButton;
	Gtk::Button*                           m_ImageTexture3DEmbedButton;
	Gtk::Button*                           m_ImageTexture3DURLReloadButton;
	Gtk::Box*                              m_PixelTexture3DBox;
	Gtk::Grid*                             m_Texture2DBox;
	Gtk::CheckButton*                      m_Texture2DNodeRepeatSCheckButton;
	Gtk::CheckButton*                      m_Texture2DNodeRepeatTCheckButton;
	Gtk::Grid*                             m_Texture3DBox;
	Gtk::CheckButton*                      m_Texture3DNodeRepeatSCheckButton;
	Gtk::CheckButton*                      m_Texture3DNodeRepeatTCheckButton;
	Gtk::CheckButton*                      m_Texture3DNodeRepeatRCheckButton;
	Gtk::Expander*                         m_TexturePropertiesExpander;
	Gtk::Box*                              m_SelectTexturePropertiesBox;
	Gtk::CheckButton*                      m_TexturePropertiesCheckButton;
	Gtk::Button*                           m_TexturePropertiesUnlinkButton;
	Gtk::Grid*                             m_TexturePropertiesBox;
	Gtk::CheckButton*                      m_TexturePropertiesGenerateMipMapsCheckButton;
	Gtk::SpinButton*                       m_TexturePropertiesBorderWidthSpinButton;
	Gtk::SpinButton*                       m_TexturePropertiesAnisotropicDegreeSpinButton;
	Gtk::ComboBoxText*                     m_TexturePropertiesMinificationFilterComboBoxText;
	Gtk::ComboBoxText*                     m_TexturePropertiesMagnificationFilterComboBoxText;
	Gtk::ComboBoxText*                     m_TexturePropertiesBoundaryModeSComboBoxText;
	Gtk::ComboBoxText*                     m_TexturePropertiesTextureCompressionComboBoxText;
	Gtk::SpinButton*                       m_TexturePropertiesTexturePrioritySpinButton;
	Gtk::ComboBoxText*                     m_TexturePropertiesBoundaryModeTComboBoxText;
	Gtk::ComboBoxText*                     m_TexturePropertiesBoundaryModeRComboBoxText;
	Gtk::Box*                              m_TexturePropertiesBorderColorBox;
	Gtk::Button*                           m_TexturePropertiesBorderColorButton;
	Gtk::Scale*                            m_TexturePropertiesBorderColorScale;
	Gtk::Expander*                         m_TextureTransformExpander;
	Gtk::Box*                              m_TextureTransformNodeBox;
	Gtk::Box*                              m_SelectTextureTransformBox;
	Gtk::ComboBoxText*                     m_TextureTransformComboBoxText;
	Gtk::Button*                           m_TextureTransformUnlinkButton;
	Gtk::Notebook*                         m_TextureTransformNotebook;
	Gtk::Label*                            m_TextureTransformLabel;
	Gtk::Grid*                             m_TextureTransformBox;
	Gtk::Box*                              m_TextureTransformTranslationBox;
	Gtk::SpinButton*                       m_TextureTransformTranslationXSpinButton;
	Gtk::SpinButton*                       m_TextureTransformTranslationYSpinButton;
	Gtk::Box*                              m_TextureTransformScaleBox;
	Gtk::SpinButton*                       m_TextureTransformScaleXSpinButton;
	Gtk::SpinButton*                       m_TextureTransformScaleYSpinButton;
	Gtk::ToggleButton*                     m_TextureTransformUniformScaleButton;
	Gtk::Image*                            m_TextureTransformUniformScaleImage;
	Gtk::Box*                              m_TextureTransformCenterBox;
	Gtk::SpinButton*                       m_TextureTransformCenterXSpinButton;
	Gtk::SpinButton*                       m_TextureTransformCenterYSpinButton;
	Gtk::SpinButton*                       m_TextureTransformRotationSpinButton;
	Gtk::Grid*                             m_TextureTransform3DBox;
	Gtk::Box*                              m_TextureTransform3DTranslationBox;
	Gtk::Box*                              m_TextureTransform3DRotationBox;
	Gtk::Box*                              m_TextureTransform3DRotationToolBox;
	Gtk::Box*                              m_TextureTransform3DScaleBox;
	Gtk::ToggleButton*                     m_TextureTransform3DUniformScaleButton;
	Gtk::Image*                            m_TextureTransform3DUniformScaleImage;
	Gtk::Box*                              m_TextureTransform3DCenterBox;
	Gtk::Grid*                             m_TextureTransformMatrix3DBox;
	Gtk::Grid*                             m_TextureTransformMatrix3DMatrixGrid;
	Gtk::Expander*                         m_TextureCoordinateGeneratorExpander;
	Gtk::Box*                              m_TextureCoordinateGeneratorMainBox;
	Gtk::Box*                              m_SelectTextureCoordinateGeneratorBox;
	Gtk::CheckButton*                      m_TextureCoordinateGeneratorCheckButton;
	Gtk::Button*                           m_TextureCoordinateGeneratorUnlinkButton;
	Gtk::Grid*                             m_TextureCoordinateGeneratorBox;
	Gtk::ComboBoxText*                     m_TextureCoordinateGeneratorModeComboBoxText;
	Gtk::Box*                              m_PaletteBox;
	Gtk::Box*                              m_PalettePreviewBox;
	Gtk::ComboBoxText*                     m_PaletteComboBoxText;
	Gtk::Button*                           m_PalettePreviousButton;
	Gtk::Button*                           m_PaletteNextButton;

};

} // puck
} // titania

#endif
