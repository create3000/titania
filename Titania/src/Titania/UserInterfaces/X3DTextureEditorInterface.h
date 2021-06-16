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

#ifndef __TMP_GLAD2CPP_TEXTURE_EDITOR_H__
#define __TMP_GLAD2CPP_TEXTURE_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for TextureEditor.
 */
class X3DTextureEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DTextureEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DTextureEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DTextureEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeneratedCubeMapTextureSizeAdjustment () const
	{ return m_GeneratedCubeMapTextureSizeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getMovieTextureSpeedAdjustment () const
	{ return m_MovieTextureSpeedAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getMultiTextureAlphaAdjustment () const
	{ return m_MultiTextureAlphaAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getMultiTextureColorAdjustment () const
	{ return m_MultiTextureColorAdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getMultiTextureFunctionListStore () const
	{ return m_MultiTextureFunctionListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getMultiTextureModeListStore () const
	{ return m_MultiTextureModeListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getMultiTextureSourceListStore () const
	{ return m_MultiTextureSourceListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureCoordinateGeneratorParameter0Adjustment () const
	{ return m_TextureCoordinateGeneratorParameter0Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureCoordinateGeneratorParameter1Adjustment () const
	{ return m_TextureCoordinateGeneratorParameter1Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureCoordinateGeneratorParameter2Adjustment () const
	{ return m_TextureCoordinateGeneratorParameter2Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureCoordinateGeneratorParameter3Adjustment () const
	{ return m_TextureCoordinateGeneratorParameter3Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureCoordinateGeneratorParameter4Adjustment () const
	{ return m_TextureCoordinateGeneratorParameter4Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureCoordinateGeneratorParameter5Adjustment () const
	{ return m_TextureCoordinateGeneratorParameter5Adjustment; }

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
	getTextureTransform3DRotationEulerXAdjustment () const
	{ return m_TextureTransform3DRotationEulerXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DRotationEulerYAdjustment () const
	{ return m_TextureTransform3DRotationEulerYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureTransform3DRotationEulerZAdjustment () const
	{ return m_TextureTransform3DRotationEulerZAdjustment; }

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

	const Glib::RefPtr <Gtk::CellRendererCombo> &
	getMultiTextureModeCellRendererCombo () const
	{ return m_MultiTextureModeCellRendererCombo; }

	const Glib::RefPtr <Gtk::CellRendererCombo> &
	getMultiTextureSourceCellRendererCombo () const
	{ return m_MultiTextureSourceCellRendererCombo; }

	const Glib::RefPtr <Gtk::CellRendererCombo> &
	getMultiTextureFunctionCellRendererCombo () const
	{ return m_MultiTextureFunctionCellRendererCombo; }

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

	const Glib::RefPtr <Gtk::CellRendererText> &
	getImageCubeMapTextureURLCellRendererText () const
	{ return m_ImageCubeMapTextureURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getImageCubeMapTextureURLChooserColumn () const
	{ return m_ImageCubeMapTextureURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getImageTextureURLCellrendererPixbuf1 () const
	{ return m_ImageTextureURLCellrendererPixbuf1; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

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

	Gtk::Grid &
	getTextureNameGrid () const
	{ return *m_TextureNameGrid; }

	Gtk::Box &
	getTextureNameBox () const
	{ return *m_TextureNameBox; }

	Gtk::Entry &
	getTextureNameEntry () const
	{ return *m_TextureNameEntry; }

	Gtk::Button &
	getTextureRenameButton () const
	{ return *m_TextureRenameButton; }

	Gtk::Label &
	getTextureFormatLabel () const
	{ return *m_TextureFormatLabel; }

	Gtk::Label &
	getTextureLoadStateLabel () const
	{ return *m_TextureLoadStateLabel; }

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
	getMultiTextureBox () const
	{ return *m_MultiTextureBox; }

	Gtk::Box &
	getMultiTextureColorBox () const
	{ return *m_MultiTextureColorBox; }

	Gtk::Button &
	getMultiTextureColorButton () const
	{ return *m_MultiTextureColorButton; }

	Gtk::Scale &
	getMultiTextureColorScale () const
	{ return *m_MultiTextureColorScale; }

	Gtk::Box &
	getMultiTextureAlphaBox () const
	{ return *m_MultiTextureAlphaBox; }

	Gtk::Scale &
	getMultiTextureAlphaScale () const
	{ return *m_MultiTextureAlphaScale; }

	Gtk::Box &
	getMultiTextureModeBox () const
	{ return *m_MultiTextureModeBox; }

	Gtk::TreeView &
	getMultiTextureModeTreeView () const
	{ return *m_MultiTextureModeTreeView; }

	Gtk::Button &
	getMultiTextureModeAddButton () const
	{ return *m_MultiTextureModeAddButton; }

	Gtk::Button &
	getMultiTextureModeRemoveButton () const
	{ return *m_MultiTextureModeRemoveButton; }

	Gtk::Box &
	getMultiTextureSourceBox () const
	{ return *m_MultiTextureSourceBox; }

	Gtk::TreeView &
	getMultiTextureSourceTreeView () const
	{ return *m_MultiTextureSourceTreeView; }

	Gtk::Button &
	getMultiTextureSourceAddButton () const
	{ return *m_MultiTextureSourceAddButton; }

	Gtk::Button &
	getMultiTextureSourceRemoveButton () const
	{ return *m_MultiTextureSourceRemoveButton; }

	Gtk::Box &
	getMultiTextureFunctionBox () const
	{ return *m_MultiTextureFunctionBox; }

	Gtk::TreeView &
	getMultiTextureFunctionTreeView () const
	{ return *m_MultiTextureFunctionTreeView; }

	Gtk::Button &
	getMultiTextureFunctionAddButton () const
	{ return *m_MultiTextureFunctionAddButton; }

	Gtk::Button &
	getMultiTextureFunctionRemoveButton () const
	{ return *m_MultiTextureFunctionRemoveButton; }

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

	Gtk::Button &
	getPixelTextureOpenButton () const
	{ return *m_PixelTextureOpenButton; }

	Gtk::Button &
	getPixelTextureSaveAsButton () const
	{ return *m_PixelTextureSaveAsButton; }

	Gtk::Box &
	getMovieTextureBox () const
	{ return *m_MovieTextureBox; }

	Gtk::CheckButton &
	getMovieTextureEnabledCheckButton () const
	{ return *m_MovieTextureEnabledCheckButton; }

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

	Gtk::Entry &
	getMovieTextureDescriptionEntry () const
	{ return *m_MovieTextureDescriptionEntry; }

	Gtk::SpinButton &
	getMovieTextureSpeedSpinButton () const
	{ return *m_MovieTextureSpeedSpinButton; }

	Gtk::Box &
	getMovieTextureControlsBox () const
	{ return *m_MovieTextureControlsBox; }

	Gtk::ToggleButton &
	getMovieTextureLoopToggleButton () const
	{ return *m_MovieTextureLoopToggleButton; }

	Gtk::Button &
	getMovieTexturePlayPauseButton () const
	{ return *m_MovieTexturePlayPauseButton; }

	Gtk::Image &
	getMovieTexturePlayPauseImage () const
	{ return *m_MovieTexturePlayPauseImage; }

	Gtk::Button &
	getMovieTextureStopButton () const
	{ return *m_MovieTextureStopButton; }

	Gtk::Image &
	getMovieTextureStopImage () const
	{ return *m_MovieTextureStopImage; }

	Gtk::Label &
	getMovieTextureElapsedTimeLabel () const
	{ return *m_MovieTextureElapsedTimeLabel; }

	Gtk::Label &
	getMovieTextureDurationLabel () const
	{ return *m_MovieTextureDurationLabel; }

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
	getComposedCubeMapTextureBox () const
	{ return *m_ComposedCubeMapTextureBox; }

	Gtk::Notebook &
	getComposedCubeMapNotebook () const
	{ return *m_ComposedCubeMapNotebook; }

	Gtk::Label &
	getComposedCubeMapFrontTextureFormatLabel () const
	{ return *m_ComposedCubeMapFrontTextureFormatLabel; }

	Gtk::Label &
	getComposedCubeMapFrontTextureLoadStateLabel () const
	{ return *m_ComposedCubeMapFrontTextureLoadStateLabel; }

	Gtk::Box &
	getComposedCubeMapFrontTexturePreviewBox () const
	{ return *m_ComposedCubeMapFrontTexturePreviewBox; }

	Gtk::Label &
	getComposedCubeMapBackTextureFormatLabel () const
	{ return *m_ComposedCubeMapBackTextureFormatLabel; }

	Gtk::Label &
	getComposedCubeMapBackTextureLoadStateLabel () const
	{ return *m_ComposedCubeMapBackTextureLoadStateLabel; }

	Gtk::Box &
	getComposedCubeMapBackTexturePreviewBox () const
	{ return *m_ComposedCubeMapBackTexturePreviewBox; }

	Gtk::Label &
	getComposedCubeMapLeftTextureFormatLabel () const
	{ return *m_ComposedCubeMapLeftTextureFormatLabel; }

	Gtk::Label &
	getComposedCubeMapLeftTextureLoadStateLabel () const
	{ return *m_ComposedCubeMapLeftTextureLoadStateLabel; }

	Gtk::Box &
	getComposedCubeMapLeftTexturePreviewBox () const
	{ return *m_ComposedCubeMapLeftTexturePreviewBox; }

	Gtk::Label &
	getComposedCubeMapRightTextureFormatLabel () const
	{ return *m_ComposedCubeMapRightTextureFormatLabel; }

	Gtk::Label &
	getComposedCubeMapRightTextureLoadStateLabel () const
	{ return *m_ComposedCubeMapRightTextureLoadStateLabel; }

	Gtk::Box &
	getComposedCubeMapRightTexturePreviewBox () const
	{ return *m_ComposedCubeMapRightTexturePreviewBox; }

	Gtk::Label &
	getComposedCubeMapTopTextureFormatLabel () const
	{ return *m_ComposedCubeMapTopTextureFormatLabel; }

	Gtk::Label &
	getComposedCubeMapTopTextureLoadStateLabel () const
	{ return *m_ComposedCubeMapTopTextureLoadStateLabel; }

	Gtk::Box &
	getComposedCubeMapTopTexturePreviewBox () const
	{ return *m_ComposedCubeMapTopTexturePreviewBox; }

	Gtk::Label &
	getComposedCubeMapBottomTextureFormatLabel () const
	{ return *m_ComposedCubeMapBottomTextureFormatLabel; }

	Gtk::Label &
	getComposedCubeMapBottomTextureLoadStateLabel () const
	{ return *m_ComposedCubeMapBottomTextureLoadStateLabel; }

	Gtk::Box &
	getComposedCubeMapBottomTexturePreviewBox () const
	{ return *m_ComposedCubeMapBottomTexturePreviewBox; }

	Gtk::Box &
	getGeneratedCubeMapTextureBox () const
	{ return *m_GeneratedCubeMapTextureBox; }

	Gtk::SpinButton &
	getGeneratedCubeMapTexturSizeSpinButton () const
	{ return *m_GeneratedCubeMapTexturSizeSpinButton; }

	Gtk::ComboBoxText &
	getGeneratedCubeMapTextureUpdateComboBoxText () const
	{ return *m_GeneratedCubeMapTextureUpdateComboBoxText; }

	Gtk::Grid &
	getImageCubeMapTextureBox () const
	{ return *m_ImageCubeMapTextureBox; }

	Gtk::Label &
	getImageCubeMapTextureFormatLabel () const
	{ return *m_ImageCubeMapTextureFormatLabel; }

	Gtk::Label &
	getImageCubeMapTextureLoadStateLabel () const
	{ return *m_ImageCubeMapTextureLoadStateLabel; }

	Gtk::Box &
	getImageCubeMapTexturePreviewBox () const
	{ return *m_ImageCubeMapTexturePreviewBox; }

	Gtk::Box &
	getImageCubeMapTextureURLBox () const
	{ return *m_ImageCubeMapTextureURLBox; }

	Gtk::TreeView &
	getImageCubeMapTextureURLTreeView () const
	{ return *m_ImageCubeMapTextureURLTreeView; }

	Gtk::Button &
	getImageCubeMapTextureURLAddButton () const
	{ return *m_ImageCubeMapTextureURLAddButton; }

	Gtk::Button &
	getImageCubeMapTextureURLRemoveButton () const
	{ return *m_ImageCubeMapTextureURLRemoveButton; }

	Gtk::Button &
	getImageCubeMapTextureEmbedButton () const
	{ return *m_ImageCubeMapTextureEmbedButton; }

	Gtk::Button &
	getImageCubeMapTextureURLReloadButton () const
	{ return *m_ImageCubeMapTextureURLReloadButton; }

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

	Gtk::Box &
	getTexturePropertiesNameBox () const
	{ return *m_TexturePropertiesNameBox; }

	Gtk::Entry &
	getTexturePropertiesNameEntry () const
	{ return *m_TexturePropertiesNameEntry; }

	Gtk::Button &
	getTexturePropertiesRenameButton () const
	{ return *m_TexturePropertiesRenameButton; }

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

	Gtk::Grid &
	getTextureTransformNameGrid () const
	{ return *m_TextureTransformNameGrid; }

	Gtk::Box &
	getTextureTransformNameBox () const
	{ return *m_TextureTransformNameBox; }

	Gtk::Entry &
	getTextureTransformNameEntry () const
	{ return *m_TextureTransformNameEntry; }

	Gtk::Button &
	getTextureTransformRenameButton () const
	{ return *m_TextureTransformRenameButton; }

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
	getTextureTransform3DRotationToolBox () const
	{ return *m_TextureTransform3DRotationToolBox; }

	Gtk::Notebook &
	getTextureTransform3DRotationNotebook () const
	{ return *m_TextureTransform3DRotationNotebook; }

	Gtk::Box &
	getTextureTransform3DRotationBox () const
	{ return *m_TextureTransform3DRotationBox; }

	Gtk::Box &
	getTextureTransform3DRotationEulerBox () const
	{ return *m_TextureTransform3DRotationEulerBox; }

	Gtk::Button &
	getTextureTransform3DRotationTypeButton () const
	{ return *m_TextureTransform3DRotationTypeButton; }

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
	getTextureCoordinateGeneratorNameBox () const
	{ return *m_TextureCoordinateGeneratorNameBox; }

	Gtk::Entry &
	getTextureCoordinateGeneratorNameEntry () const
	{ return *m_TextureCoordinateGeneratorNameEntry; }

	Gtk::Button &
	getTextureCoordinateGeneratorRenameButton () const
	{ return *m_TextureCoordinateGeneratorRenameButton; }

	Gtk::SpinButton &
	getTextureCoordinateGeneratorParameter0SpinButton () const
	{ return *m_TextureCoordinateGeneratorParameter0SpinButton; }

	Gtk::SpinButton &
	getTextureCoordinateGeneratorParameter1SpinButton () const
	{ return *m_TextureCoordinateGeneratorParameter1SpinButton; }

	Gtk::SpinButton &
	getTextureCoordinateGeneratorParameter2SpinButton () const
	{ return *m_TextureCoordinateGeneratorParameter2SpinButton; }

	Gtk::SpinButton &
	getTextureCoordinateGeneratorParameter3SpinButton () const
	{ return *m_TextureCoordinateGeneratorParameter3SpinButton; }

	Gtk::SpinButton &
	getTextureCoordinateGeneratorParameter4SpinButton () const
	{ return *m_TextureCoordinateGeneratorParameter4SpinButton; }

	Gtk::SpinButton &
	getTextureCoordinateGeneratorParameter5SpinButton () const
	{ return *m_TextureCoordinateGeneratorParameter5SpinButton; }

	Gtk::Button &
	getTextureCoordinateGeneratorAddParameterButton () const
	{ return *m_TextureCoordinateGeneratorAddParameterButton; }

	Gtk::Button &
	getTextureCoordinateGeneratorRemoveParameterButton () const
	{ return *m_TextureCoordinateGeneratorRemoveParameterButton; }

	Gtk::Box &
	getUsedTexturesBox () const
	{ return *m_UsedTexturesBox; }

	Gtk::Box &
	getPaletteBox () const
	{ return *m_PaletteBox; }

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

	Gtk::Popover &
	getTextureTransform3DRotationPopover () const
	{ return *m_TextureTransform3DRotationPopover; }

	Gtk::RadioButton &
	getTextureTransform3DRotationAxisAngleButton () const
	{ return *m_TextureTransform3DRotationAxisAngleButton; }

	Gtk::RadioButton &
	getTextureTransform3DRotationEulerButton () const
	{ return *m_TextureTransform3DRotationEulerButton; }

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
	getUpdateObjectInPaletteMenuItem () const
	{ return *m_UpdateObjectInPaletteMenuItem; }

	Gtk::ImageMenuItem &
	getRemoveObjectFromPaletteMenuItem () const
	{ return *m_RemoveObjectFromPaletteMenuItem; }

	Gtk::CheckMenuItem &
	getShowDefaultPalettesMenuItem () const
	{ return *m_ShowDefaultPalettesMenuItem; }

	///  @name Signal handlers

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
	on_pixel_texture_open_clicked () = 0;

	virtual
	void
	on_pixel_texture_save_as_clicked () = 0;

	virtual
	void
	on_movie_texture_play_pause_clicked () = 0;

	virtual
	void
	on_movie_texture_stop_clicked () = 0;

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
	on_texture_transform_3d_rotation_type_clicked () = 0;

	virtual
	void
	on_texture_transform_3d_uniform_scale_clicked () = 0;

	virtual
	void
	on_textureCoordinateGenerator_toggled () = 0;

	virtual
	void
	on_textureCoordinateGenerator_unlink_clicked () = 0;

	virtual
	void
	on_texture_coordinate_generator_add_parameter_clicked () = 0;

	virtual
	void
	on_texture_coordinate_generator_remove_parameter_clicked () = 0;

	virtual
	bool
	on_palette_button_press_event (GdkEventButton* button_event) = 0;

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
	on_texture_transform_3d_rotation_axis_angle_toggled () = 0;

	virtual
	void
	on_texture_transform_3d_rotation_euler_toggled () = 0;

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
	on_update_object_in_palette_activate () = 0;

	virtual
	void
	on_remove_object_from_palette_activate () = 0;

	virtual
	void
	on_show_default_palettes_toggled () = 0;

	///  @name Destruction

	virtual
	~X3DTextureEditorInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_GeneratedCubeMapTextureSizeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_MovieTextureSpeedAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_MultiTextureAlphaAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_MultiTextureColorAdjustment;
	Glib::RefPtr <Gtk::ListStore> m_MultiTextureFunctionListStore;
	Glib::RefPtr <Gtk::ListStore> m_MultiTextureModeListStore;
	Glib::RefPtr <Gtk::ListStore> m_MultiTextureSourceListStore;
	Glib::RefPtr <Gtk::Adjustment> m_TextureCoordinateGeneratorParameter0Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureCoordinateGeneratorParameter1Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureCoordinateGeneratorParameter2Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureCoordinateGeneratorParameter3Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureCoordinateGeneratorParameter4Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureCoordinateGeneratorParameter5Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TexturePropertiesAnisotropicDegreeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TexturePropertiesBorderColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TexturePropertiesBorderWidthAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TexturePropertiesTexturePriorityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DRotationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DRotationEulerXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DRotationEulerYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DRotationEulerZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransform3DTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D00Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D01Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D02Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D03Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D04Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D05Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D06Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D07Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D08Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D09Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D10Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D11Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D12Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D13Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D14Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformMatrix3D15Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformRotationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextureTransformTranslationYAdjustment;
	Glib::RefPtr <Gtk::CellRendererCombo> m_MultiTextureModeCellRendererCombo;
	Glib::RefPtr <Gtk::CellRendererCombo> m_MultiTextureSourceCellRendererCombo;
	Glib::RefPtr <Gtk::CellRendererCombo> m_MultiTextureFunctionCellRendererCombo;
	Glib::RefPtr <Gtk::CellRendererText> m_ImageTextureURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_ImageTextureURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_ImageTextureURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText> m_MovieTextureURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_MovieTextureURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_MovieTextureURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText> m_ImageTexture3DURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_ImageTexture3DURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_ImageTexture3DURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText> m_ImageCubeMapTextureURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_ImageCubeMapTextureURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_ImageTextureURLCellrendererPixbuf1;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Notebook* m_TextureChildNotebook;
	Gtk::Expander* m_TextureExpander;
	Gtk::Box* m_TextureBox;
	Gtk::Box* m_SelectTextureBox;
	Gtk::ComboBoxText* m_TextureComboBoxText;
	Gtk::Button* m_TextureUnlinkButton;
	Gtk::Grid* m_TextureNameGrid;
	Gtk::Box* m_TextureNameBox;
	Gtk::Entry* m_TextureNameEntry;
	Gtk::Button* m_TextureRenameButton;
	Gtk::Label* m_TextureFormatLabel;
	Gtk::Label* m_TextureLoadStateLabel;
	Gtk::Box* m_PreviewBox;
	Gtk::Notebook* m_TextureNotebook;
	Gtk::Label* m_TextureLabel;
	Gtk::Box* m_MultiTextureBox;
	Gtk::Box* m_MultiTextureColorBox;
	Gtk::Button* m_MultiTextureColorButton;
	Gtk::Scale* m_MultiTextureColorScale;
	Gtk::Box* m_MultiTextureAlphaBox;
	Gtk::Scale* m_MultiTextureAlphaScale;
	Gtk::Box* m_MultiTextureModeBox;
	Gtk::TreeView* m_MultiTextureModeTreeView;
	Gtk::Button* m_MultiTextureModeAddButton;
	Gtk::Button* m_MultiTextureModeRemoveButton;
	Gtk::Box* m_MultiTextureSourceBox;
	Gtk::TreeView* m_MultiTextureSourceTreeView;
	Gtk::Button* m_MultiTextureSourceAddButton;
	Gtk::Button* m_MultiTextureSourceRemoveButton;
	Gtk::Box* m_MultiTextureFunctionBox;
	Gtk::TreeView* m_MultiTextureFunctionTreeView;
	Gtk::Button* m_MultiTextureFunctionAddButton;
	Gtk::Button* m_MultiTextureFunctionRemoveButton;
	Gtk::Box* m_ImageTextureBox;
	Gtk::Box* m_ImageTextureURLBox;
	Gtk::TreeView* m_ImageTextureURLTreeView;
	Gtk::Button* m_ImageTextureURLAddButton;
	Gtk::Button* m_ImageTextureURLRemoveButton;
	Gtk::Button* m_ImageTextureEmbedButton;
	Gtk::Button* m_ImageTextureURLReloadButton;
	Gtk::Box* m_PixelTextureBox;
	Gtk::Button* m_PixelTextureOpenButton;
	Gtk::Button* m_PixelTextureSaveAsButton;
	Gtk::Box* m_MovieTextureBox;
	Gtk::CheckButton* m_MovieTextureEnabledCheckButton;
	Gtk::Box* m_MovieTextureURLBox;
	Gtk::TreeView* m_MovieTextureURLTreeView;
	Gtk::Button* m_MovieTextureURLAddButton;
	Gtk::Button* m_MovieTextureURLRemoveButton;
	Gtk::Button* m_MovieTextureURLReloadButton;
	Gtk::Entry* m_MovieTextureDescriptionEntry;
	Gtk::SpinButton* m_MovieTextureSpeedSpinButton;
	Gtk::Box* m_MovieTextureControlsBox;
	Gtk::ToggleButton* m_MovieTextureLoopToggleButton;
	Gtk::Button* m_MovieTexturePlayPauseButton;
	Gtk::Image* m_MovieTexturePlayPauseImage;
	Gtk::Button* m_MovieTextureStopButton;
	Gtk::Image* m_MovieTextureStopImage;
	Gtk::Label* m_MovieTextureElapsedTimeLabel;
	Gtk::Label* m_MovieTextureDurationLabel;
	Gtk::Box* m_ComposedTexture3DBox;
	Gtk::Box* m_ImageTexture3DBox;
	Gtk::Box* m_ImageTexture3DURLBox;
	Gtk::TreeView* m_ImageTexture3DURLTreeView;
	Gtk::Button* m_ImageTexture3DURLAddButton;
	Gtk::Button* m_ImageTexture3DURLRemoveButton;
	Gtk::Button* m_ImageTexture3DEmbedButton;
	Gtk::Button* m_ImageTexture3DURLReloadButton;
	Gtk::Box* m_PixelTexture3DBox;
	Gtk::Grid* m_ComposedCubeMapTextureBox;
	Gtk::Notebook* m_ComposedCubeMapNotebook;
	Gtk::Label* m_ComposedCubeMapFrontTextureFormatLabel;
	Gtk::Label* m_ComposedCubeMapFrontTextureLoadStateLabel;
	Gtk::Box* m_ComposedCubeMapFrontTexturePreviewBox;
	Gtk::Label* m_ComposedCubeMapBackTextureFormatLabel;
	Gtk::Label* m_ComposedCubeMapBackTextureLoadStateLabel;
	Gtk::Box* m_ComposedCubeMapBackTexturePreviewBox;
	Gtk::Label* m_ComposedCubeMapLeftTextureFormatLabel;
	Gtk::Label* m_ComposedCubeMapLeftTextureLoadStateLabel;
	Gtk::Box* m_ComposedCubeMapLeftTexturePreviewBox;
	Gtk::Label* m_ComposedCubeMapRightTextureFormatLabel;
	Gtk::Label* m_ComposedCubeMapRightTextureLoadStateLabel;
	Gtk::Box* m_ComposedCubeMapRightTexturePreviewBox;
	Gtk::Label* m_ComposedCubeMapTopTextureFormatLabel;
	Gtk::Label* m_ComposedCubeMapTopTextureLoadStateLabel;
	Gtk::Box* m_ComposedCubeMapTopTexturePreviewBox;
	Gtk::Label* m_ComposedCubeMapBottomTextureFormatLabel;
	Gtk::Label* m_ComposedCubeMapBottomTextureLoadStateLabel;
	Gtk::Box* m_ComposedCubeMapBottomTexturePreviewBox;
	Gtk::Box* m_GeneratedCubeMapTextureBox;
	Gtk::SpinButton* m_GeneratedCubeMapTexturSizeSpinButton;
	Gtk::ComboBoxText* m_GeneratedCubeMapTextureUpdateComboBoxText;
	Gtk::Grid* m_ImageCubeMapTextureBox;
	Gtk::Label* m_ImageCubeMapTextureFormatLabel;
	Gtk::Label* m_ImageCubeMapTextureLoadStateLabel;
	Gtk::Box* m_ImageCubeMapTexturePreviewBox;
	Gtk::Box* m_ImageCubeMapTextureURLBox;
	Gtk::TreeView* m_ImageCubeMapTextureURLTreeView;
	Gtk::Button* m_ImageCubeMapTextureURLAddButton;
	Gtk::Button* m_ImageCubeMapTextureURLRemoveButton;
	Gtk::Button* m_ImageCubeMapTextureEmbedButton;
	Gtk::Button* m_ImageCubeMapTextureURLReloadButton;
	Gtk::Grid* m_Texture2DBox;
	Gtk::CheckButton* m_Texture2DNodeRepeatSCheckButton;
	Gtk::CheckButton* m_Texture2DNodeRepeatTCheckButton;
	Gtk::Grid* m_Texture3DBox;
	Gtk::CheckButton* m_Texture3DNodeRepeatSCheckButton;
	Gtk::CheckButton* m_Texture3DNodeRepeatTCheckButton;
	Gtk::CheckButton* m_Texture3DNodeRepeatRCheckButton;
	Gtk::Expander* m_TexturePropertiesExpander;
	Gtk::Box* m_SelectTexturePropertiesBox;
	Gtk::CheckButton* m_TexturePropertiesCheckButton;
	Gtk::Button* m_TexturePropertiesUnlinkButton;
	Gtk::Grid* m_TexturePropertiesBox;
	Gtk::CheckButton* m_TexturePropertiesGenerateMipMapsCheckButton;
	Gtk::SpinButton* m_TexturePropertiesBorderWidthSpinButton;
	Gtk::SpinButton* m_TexturePropertiesAnisotropicDegreeSpinButton;
	Gtk::ComboBoxText* m_TexturePropertiesMinificationFilterComboBoxText;
	Gtk::ComboBoxText* m_TexturePropertiesMagnificationFilterComboBoxText;
	Gtk::ComboBoxText* m_TexturePropertiesBoundaryModeSComboBoxText;
	Gtk::ComboBoxText* m_TexturePropertiesTextureCompressionComboBoxText;
	Gtk::SpinButton* m_TexturePropertiesTexturePrioritySpinButton;
	Gtk::ComboBoxText* m_TexturePropertiesBoundaryModeTComboBoxText;
	Gtk::ComboBoxText* m_TexturePropertiesBoundaryModeRComboBoxText;
	Gtk::Box* m_TexturePropertiesBorderColorBox;
	Gtk::Button* m_TexturePropertiesBorderColorButton;
	Gtk::Scale* m_TexturePropertiesBorderColorScale;
	Gtk::Box* m_TexturePropertiesNameBox;
	Gtk::Entry* m_TexturePropertiesNameEntry;
	Gtk::Button* m_TexturePropertiesRenameButton;
	Gtk::Expander* m_TextureTransformExpander;
	Gtk::Box* m_TextureTransformNodeBox;
	Gtk::Box* m_SelectTextureTransformBox;
	Gtk::ComboBoxText* m_TextureTransformComboBoxText;
	Gtk::Button* m_TextureTransformUnlinkButton;
	Gtk::Grid* m_TextureTransformNameGrid;
	Gtk::Box* m_TextureTransformNameBox;
	Gtk::Entry* m_TextureTransformNameEntry;
	Gtk::Button* m_TextureTransformRenameButton;
	Gtk::Notebook* m_TextureTransformNotebook;
	Gtk::Label* m_TextureTransformLabel;
	Gtk::Grid* m_TextureTransformBox;
	Gtk::Box* m_TextureTransformTranslationBox;
	Gtk::SpinButton* m_TextureTransformTranslationXSpinButton;
	Gtk::SpinButton* m_TextureTransformTranslationYSpinButton;
	Gtk::Box* m_TextureTransformScaleBox;
	Gtk::SpinButton* m_TextureTransformScaleXSpinButton;
	Gtk::SpinButton* m_TextureTransformScaleYSpinButton;
	Gtk::ToggleButton* m_TextureTransformUniformScaleButton;
	Gtk::Image* m_TextureTransformUniformScaleImage;
	Gtk::Box* m_TextureTransformCenterBox;
	Gtk::SpinButton* m_TextureTransformCenterXSpinButton;
	Gtk::SpinButton* m_TextureTransformCenterYSpinButton;
	Gtk::SpinButton* m_TextureTransformRotationSpinButton;
	Gtk::Grid* m_TextureTransform3DBox;
	Gtk::Box* m_TextureTransform3DTranslationBox;
	Gtk::Box* m_TextureTransform3DRotationToolBox;
	Gtk::Notebook* m_TextureTransform3DRotationNotebook;
	Gtk::Box* m_TextureTransform3DRotationBox;
	Gtk::Box* m_TextureTransform3DRotationEulerBox;
	Gtk::Button* m_TextureTransform3DRotationTypeButton;
	Gtk::Box* m_TextureTransform3DScaleBox;
	Gtk::ToggleButton* m_TextureTransform3DUniformScaleButton;
	Gtk::Image* m_TextureTransform3DUniformScaleImage;
	Gtk::Box* m_TextureTransform3DCenterBox;
	Gtk::Grid* m_TextureTransformMatrix3DBox;
	Gtk::Grid* m_TextureTransformMatrix3DMatrixGrid;
	Gtk::Expander* m_TextureCoordinateGeneratorExpander;
	Gtk::Box* m_TextureCoordinateGeneratorMainBox;
	Gtk::Box* m_SelectTextureCoordinateGeneratorBox;
	Gtk::CheckButton* m_TextureCoordinateGeneratorCheckButton;
	Gtk::Button* m_TextureCoordinateGeneratorUnlinkButton;
	Gtk::Grid* m_TextureCoordinateGeneratorBox;
	Gtk::ComboBoxText* m_TextureCoordinateGeneratorModeComboBoxText;
	Gtk::Box* m_TextureCoordinateGeneratorNameBox;
	Gtk::Entry* m_TextureCoordinateGeneratorNameEntry;
	Gtk::Button* m_TextureCoordinateGeneratorRenameButton;
	Gtk::SpinButton* m_TextureCoordinateGeneratorParameter0SpinButton;
	Gtk::SpinButton* m_TextureCoordinateGeneratorParameter1SpinButton;
	Gtk::SpinButton* m_TextureCoordinateGeneratorParameter2SpinButton;
	Gtk::SpinButton* m_TextureCoordinateGeneratorParameter3SpinButton;
	Gtk::SpinButton* m_TextureCoordinateGeneratorParameter4SpinButton;
	Gtk::SpinButton* m_TextureCoordinateGeneratorParameter5SpinButton;
	Gtk::Button* m_TextureCoordinateGeneratorAddParameterButton;
	Gtk::Button* m_TextureCoordinateGeneratorRemoveParameterButton;
	Gtk::Box* m_UsedTexturesBox;
	Gtk::Box* m_PaletteBox;
	Gtk::Box* m_PalettePreviewBox;
	Gtk::Box* m_ChangePaletteBox;
	Gtk::ComboBoxText* m_PaletteComboBoxText;
	Gtk::Button* m_PalettePreviousButton;
	Gtk::Button* m_PaletteNextButton;
	Gtk::Dialog* m_EditPaletteDialog;
	Gtk::Button* m_EditPaletteCancelButton;
	Gtk::Button* m_EditPaletteOkButton;
	Gtk::Entry* m_PaletteNameEntry;
	Gtk::Popover* m_TextureTransform3DRotationPopover;
	Gtk::RadioButton* m_TextureTransform3DRotationAxisAngleButton;
	Gtk::RadioButton* m_TextureTransform3DRotationEulerButton;
	Gtk::Menu* m_PaletteMenu;
	Gtk::ImageMenuItem* m_AddPaletteMenuItem;
	Gtk::ImageMenuItem* m_RemovePaletteMenuItem;
	Gtk::ImageMenuItem* m_EditPaletteMenuItem;
	Gtk::ImageMenuItem* m_AddObjectToPaletteMenuItem;
	Gtk::ImageMenuItem* m_UpdateObjectInPaletteMenuItem;
	Gtk::ImageMenuItem* m_RemoveObjectFromPaletteMenuItem;
	Gtk::CheckMenuItem* m_ShowDefaultPalettesMenuItem;

};

} // puck
} // titania

#endif
